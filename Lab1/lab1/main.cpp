#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>

using namespace std;

// vector cu cuvintele cheie definite in mini-limbj
static const vector<string> keyWords = {"#include", "<iostream>", "using", "namespace", "std", "main()", "int",
                                 "float", "Fractie", "if", "else", "while", "return", "cin", "cout"};
// vector cu operatorii definiti in mini-limbaj
static const vector<string> operands = {"+", "-", "*", "/", "%", "=", ">>", "<<", "==", ">=", "<=", ">", "<"};
// vector cu delimitatorii definiti in mini-limbaj
static const vector<string> delimiters = {"(", ")", "{", "}", ";", ","};

/**
 * cauta in linia citita din fisier atomii ce se afla in lista de atomi data si actualizeaza in lista de
 * atomi gasiti in text
 * @param givenAtoms - lista de atomi data
 * @param foundAtoms - lista de atomi gasiti pana la momentul dat
 * @param givenLine - o linie citita din fisier
 */
void checkInListOfAtoms(vector<string> givenAtoms, map<string, int> &foundAtoms, string &givenLine){
    for (auto atom: givenAtoms)
        while (givenLine.find(atom) != string::npos){
            foundAtoms.insert(pair<string, int> (atom, foundAtoms.size()));
            givenLine.erase(givenLine.find(atom), atom.length());
        }
}

/**
 * verifica ca stringul este format doar din litere
 * @param word - stringul dat
 */
bool hasOnlyLetters(string word) {
    return regex_match(word, regex("[A-Za-z]*")) && word != "return" && word.length() < 250;
}

/**
 * verifica ca stringul este format doar din cifre
 * @param word - stringul dat
 */
bool hasOnlyDigits(string word) {
    return regex_match(word, regex("[0-9]*"));
}

/**
 * diferentiaza Constantele de Identificatori
 * @param constants - dictionarul in care se tin constantele
 * @param identifiers - dictionarul in care se tin identificatorii
 * @param foundCONST - booleana ce retine daca a fost gasita o constanta
 * @param foundID - booleana ce retine daca a fost gaist un identificator
 * @param givenLine - linia de cod data
 */
void checkForConstsAndIds(map<string, int> &constants, map<string, int> &identifiers, bool &foundCONST, bool &foundID, string givenLine){
    size_t pos = 0;
    string word;

    while ((pos = givenLine.find(" ")) != std::string::npos)
    {
        word = givenLine.substr(0, pos);
        if(word.length() > 0){
            if (hasOnlyDigits(word))
                constants.insert(pair<string, int> (word, constants.size()));
            else
                if (hasOnlyLetters(word))
                    identifiers.insert(pair<string, int> (word, identifiers.size()));
        }
        givenLine.erase(0, pos + 1);
    }

    word = givenLine;
    if(word.length() > 0){
        // constantele sunt formate doar din cifre => daca se gaseste vreo cifra printre caracterele ramase,
        // ea face parte dintr-o constanta
        if (hasOnlyDigits(word))
            constants.insert(pair<string, int> (word, constants.size())), foundCONST = true;
        else
            // identificatorii sunt formati doar din litere => daca se gaseste vreo litera printre caracterele
            // ramase, ea face parte dintr-un identificator
            if (hasOnlyLetters(word))
                identifiers.insert(pair<string, int> (word, identifiers.size())), foundID = true;
    }
}

/**
 * creeaza liste cu ID-ul atomilor, constantelor si identificatorilor
 * @param atoms - dictionar in care se retin atomii
 * @param constants - dictionar in care se retin constantele
 * @param identifiers - dictionar in care se retin identificatorii
 */
void findElements(map<string, int> &atoms, map<string, int> &constants, map<string, int> &identifiers){
    ifstream fin("SOURCE.txt");
    string line;
    while (!fin.eof()){
        getline(fin, line); // citire linie din fisier

        // actualizare lista de atomi gasiti
        checkInListOfAtoms(keyWords, atoms, line);
        checkInListOfAtoms(operands, atoms, line);
        checkInListOfAtoms(delimiters, atoms, line);

        bool foundCONST = false, foundID = false;
        checkForConstsAndIds(constants, identifiers, foundCONST, foundID, line);

        if (foundCONST)
            atoms.insert(pair<string, int> ("CONST", atoms.size()));

        if (foundID)
            atoms.insert(pair<string, int> ("ID", atoms.size()));
    }
    fin.close();
}

/**
 * genereaza forma interna a programului sursa
 * @param fip - vector de pair-uri in care se retin codificarile
 * @param atoms - dictionar in care se retin atomii
 * @param constants - dictionar in care se retin constantele
 * @param identifiers - dictionar in care se retin identificatorii
 * @return -1 - daca in cod nu s-au identificat erori lexicale
 *      i - daca in cod nu s-au identificat erori lexicale la linia i
 */
int createFIP(vector<pair<int, int>> &fip, map<string, int> &atoms, map<string, int> &constants, map<string, int> &identifiers) {
    ifstream fin("SOURCE.txt");
    string line;
    int openedA = 0; // cate acolade au fost deschise, dar nu si inchise
    int i = 0; // linia curenta de cod;

    getline(fin, line);
    i++;
    if (line == "#include <iostream>"){ // codificare : #include <iostream>
        fip.push_back(pair<int, int> (atoms["#include"], -1));
        fip.push_back(pair<int, int> (atoms["<iostream>"], -1));
    }
    else return i;

    getline(fin, line);
    i++;
    if (line == "using namespace std;"){ // codificare : using namespace std;
        fip.push_back(pair<int, int> (atoms["using"], -1));
        fip.push_back(pair<int, int> (atoms["namespace"], -1));
        fip.push_back(pair<int, int> (atoms["std"], -1));
        fip.push_back(pair<int, int> (atoms[";"], -1));
    } else return i;

    getline(fin, line);
    i++;
    if (line == "int main() {"){ // codificare int main() {
        fip.push_back(pair<int, int> (atoms["int"], -1));
        fip.push_back(pair<int, int> (atoms["main()"], -1));
        fip.push_back(pair<int, int> (atoms["{"], -1));
        openedA++;
    } else return i;

    getline(fin, line);
    i++;
    // codificare lista de declaratii
    while((line.find("int ") != std::string::npos || line.find("float ") != std::string::npos
        || line.find("Fractie ") != std::string::npos) && !fin.eof()){
        while(line[0] == ' ')
            line.erase(0, 1);

        string word = line.substr(0, line.find(" "));
        if (word == "int" || word == "float" || word == "Fractie")
            fip.push_back(pair<int, int> (atoms[word], -1));
        else
            return i;

        word = line.substr(line.find(" ") + 1);
        if (word[word.length() - 1] == ';'){
            word.pop_back();
            if (hasOnlyLetters(word))
                fip.push_back(pair<int, int> (atoms["ID"], identifiers[word]));
            else
                return i;
            fip.push_back(pair<int, int> (atoms[";"], -1));
        }
        else return i;

        getline(fin, line);
        i++;
    }

    while(line.find("return 0;") == std::string::npos && !fin.eof()){
        while(line[0] == ' ')
            line.erase(0, 1);

        string word = line.substr(0, line.find(" "));
        if (word == "cin"){  // operatia de citire: cin >> ID;
            fip.push_back(pair<int, int> (atoms[word], -1));
            line.erase(0, word.length() + 1);
            word = line.substr(0, line.find(" "));
            if (word == ">>"){
                fip.push_back(pair<int, int> (atoms[word], -1));
                line.erase(0, word.length() + 1);
                word = line.substr(0, line.find(" "));
                if (word[word.length() - 1] == ';'){
                    word.pop_back();
                    if (hasOnlyLetters(word))
                        fip.push_back(pair<int, int> (atoms["ID"], identifiers[word]));
                    else
                        return i;
                    fip.push_back(pair<int, int> (atoms[";"], -1));
                }
                else return i;
            }
            else
                return i;
        }
        else
            if (word == "cout") { // operatia de afisare: cout << ID;
                fip.push_back(pair<int, int>(atoms[word], -1));
                line.erase(0, word.length() + 1);
                word = line.substr(0, line.find(" "));
                if (word == "<<") {
                    fip.push_back(pair<int, int>(atoms[word], -1));
                    line.erase(0, word.length() + 1);
                    word = line.substr(0, line.find(" "));
                    if (word[word.length() - 1] == ';') {
                        word.pop_back();
                        if (hasOnlyLetters(word))
                            fip.push_back(pair<int, int>(atoms["ID"], identifiers[word]));
                        else
                            return i;
                        fip.push_back(pair<int, int> (atoms[";"], -1));
                    } else return i;
                }
                else return i;
            }
            else
                if (word == "if" || word == "while") { // if (conditie) { si while(conditie) {
                    fip.push_back(pair<int, int>(atoms[word], -1));
                    line.erase(0, word.length() + 1);
                    word = line.substr(0, line.find(" "));
                    if (word[0] == '(') { // prima expresie din conditie
                        fip.push_back(pair<int, int>(atoms["("], -1));
                        line.erase(0, 1);
                        word = line.substr(0, line.find(" "));
                        while (!word.empty() && word != "==" && word != ">=" && word != "<=" && word != ">" && word != "<") {
                            if (hasOnlyLetters(word))
                                fip.push_back(pair<int, int>(atoms["ID"], identifiers[word]));
                            else if (hasOnlyDigits(word))
                                fip.push_back(pair<int, int>(atoms["CONST"], constants[word]));
                            else if (word == "+" || word == "-" || word == "*" || word == "%" || word == "/")
                                fip.push_back(pair<int, int>(atoms[word], -1));
                            else
                                return i;
                            line.erase(0, word.length() + 1);
                            word = line.substr(0, line.find(" "));
                        }
                        if(word.empty())
                            return i;

                        if (word == "==" || word == ">=" || word == "<=" || word == ">" || word == "<")
                            fip.push_back(pair<int, int>(atoms[word], -1));
                        else
                            return i;
                        line.erase(0, word.length() + 1);
                        word = line.substr(0, line.find(" "));

                        while (!word.empty() && word[word.length() - 1] != ')') { // a doua expresie din conditie
                            if (hasOnlyLetters(word))
                                fip.push_back(pair<int, int>(atoms["ID"], identifiers[word]));
                            else
                            if (hasOnlyDigits(word))
                                fip.push_back(pair<int, int>(atoms["CONST"], constants[word]));
                            else
                            if (word == "+" || word == "-" || word == "*" || word == "%" || word == "/")
                                fip.push_back(pair<int, int>(atoms[word], -1));
                            else
                                return i;
                            line.erase(0, word.length() + 1);
                            word = line.substr(0, line.find(" "));
                        }

                        if(word.empty())
                            return i;

                        if (word[word.length() - 1] == ')') {
                            word.pop_back();
                            if (hasOnlyLetters(word))
                                fip.push_back(pair<int, int>(atoms["ID"], identifiers[word]));
                            else if (hasOnlyDigits(word))
                                fip.push_back(pair<int, int>(atoms["CONST"], constants[word]));
                            else
                                return i;
                            fip.push_back(pair<int, int>(atoms[")"], -1));
                        } else return i;

                        line.erase(0, word.length() + 2);
                        if (line == "{")
                            fip.push_back(pair<int, int>(atoms["{"], -1)), openedA++;
                        else return i;
                    }
                }
                else
                    if (word == "else") {
                        fip.push_back(pair<int, int>(atoms[word], -1));
                    }
                    else
                        if (hasOnlyLetters(word)){ // operatia de atribuire: ID = expresie
                            fip.push_back(pair<int, int>(atoms["ID"], identifiers[word]));
                            line.erase(0, word.length() + 1);
                            word = line.substr(0, line.find(" "));
                            if (word == "=") {
                                fip.push_back(pair<int, int>(atoms[word], -1));
                                line.erase(0, word.length() + 1);
                                word = line.substr(0, line.find(" "));
                                while (!word.empty() && word[word.length() - 1] != ';') { // codificare expresie
                                    if (hasOnlyLetters(word))
                                        fip.push_back(pair<int, int>(atoms["ID"], identifiers[word]));
                                    else
                                        if (hasOnlyDigits(word))
                                            fip.push_back(pair<int, int>(atoms["CONST"], constants[word]));
                                        else
                                            if (word == "+" || word == "-" || word == "*" || word == "%" || word == "/")
                                                fip.push_back(pair<int, int>(atoms[word], -1));
                                            else
                                                return i;
                                    line.erase(0, word.length() + 1);
                                    word = line.substr(0, line.find(" "));
                                }
                                if(word.empty())
                                    return i;
                                if (word[word.length() - 1] == ';') {
                                    word.pop_back();
                                    if (hasOnlyLetters(word))
                                        fip.push_back(pair<int, int>(atoms["ID"], identifiers[word]));
                                    else
                                        if (hasOnlyDigits(word))
                                            fip.push_back(pair<int, int>(atoms["CONST"], constants[word]));
                                        else
                                            return i;
                                    fip.push_back(pair<int, int> (atoms[";"], -1));
                                } else return i;
                            }
                        }
                        else
                            if (word == "}")
                                fip.push_back(pair<int, int>(atoms[word], -1)), openedA--;
                            else
                                return i;
        getline(fin, line);
        i++;
    }

    if (line.find("return 0;") != std::string::npos) {
        while(line[0] == ' ')
            line.erase(0, 1);
        fip.push_back(pair<int, int> (atoms["return"], -1));
        fip.push_back(pair<int, int> (atoms["CONST"], constants["0"]));
        fip.push_back(pair<int, int> (atoms[";"], -1));
    }else return i;

    getline(fin, line);
    i++;
    if (line == "}")
        fip.push_back(pair<int, int> (atoms["}"], -1)), openedA--;
    else return i;

    i++;
    if (!fin.eof() || openedA != 0) // verificare daca s-a ajuns la finalul fisierului si parantezarea este corecta
        return i;

    fin.close();
    return -1; // nu a aparut vreo eroare de compilare
}

/**
 * afiseaza in fisiere rezultatele
 * @param atoms - dictionar in care se retin atomii
 * @param constants - dictionar in care se retin constantele
 * @param identifiers - dictionar in care se retin identificatorii
 * @param fip - vector de pair-uri in care se retin codificarile
 */
void printResults(map<string, int> atoms, map<string, int> constants, map<string, int> identifiers, vector<pair<int, int>> fip){
    ofstream foutA("ATOMS.txt");
    ofstream foutC("TS_CONST.txt");
    ofstream foutI("TS_ID.txt");
    ofstream foutF("FIP.txt");

    // afisare atomi in fisier
    for(auto atom: atoms)
        foutA << atom.first << " " << atom.second << "\n";
    foutA.close();

    // afisare constante in fisier
    for(auto con: constants)
        foutC << con.first << " " << con.second << "\n";
    foutC.close();

    // afisare identificatori in fisier
    for(auto id: identifiers)
        foutI << id.first << " " << id.second << "\n";
    foutI.close();

    // afisare fip in fisier
    for(auto elem: fip)
        if (elem.second == -1)
            foutF << elem.first << " - \n";
        else
            foutF << elem.first << " " << elem.second << "\n";
    foutF.close();
}

int main() {
    map<string, int> atoms;
    map<string, int> constants;
    map<string, int> identifiers;
    vector<pair<int, int>> fip;

    findElements(atoms, constants, identifiers);
    int line = createFIP(fip, atoms, constants, identifiers);
    if (line == -1)
        printResults(atoms, constants, identifiers, fip);
    else
        cout << "Compilation error at line " << line << "!";
    return 0;
}
