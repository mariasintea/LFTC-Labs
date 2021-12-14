package entity;


import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

public class Production {
    private List<String> prodElems;
    private int currentElementId;
    private int prodId;

    public Production(String productionString, int prodId, int currentProd) {
        prodElems = Arrays.asList(productionString.replace(" ->", "").split(" "));
        this.currentElementId = currentProd;
        this.prodId = prodId;
    }

    public String getOrigin() {
        return prodElems.get(0);
    }

    public List<String> getProductionElements() {
        List<String> resultList = new ArrayList<>();
        for (int step = 1; step < prodElems.size(); step++) {
            resultList.add(prodElems.get(step));
        }
        return resultList;
    }

    public boolean canDerivate() {
        return currentElementId < prodElems.size();
    }

    public String getCurrentElem() {
        if(currentElementId > 0 && canDerivate())
            return prodElems.get(currentElementId);
        return "";
    }

    @Override
    public String toString() {
        String result = getOrigin() + " ->";
        for (int step = 1; step < prodElems.size(); step++) {
            if (step == currentElementId) {
                result += " .";
            }
            result += " " + prodElems.get(step);
        }
        if (currentElementId == prodElems.size()) {
            result += " .";
        }
        return result;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;

        Production that = (Production) o;

        return currentElementId == that.currentElementId && prodId == that.prodId && prodElems.equals(that.prodElems);

    }

    @Override
    public int hashCode() {
        int result = prodElems.hashCode();
        result = 31 * result + currentElementId;
        result = 31 * result + prodId;
        return result;
    }

    public Production derivate() throws Exception {
        if (canDerivate()) {
            String result = getOrigin() + " ->";
            for (int step = 1; step < prodElems.size(); step++)
                result += " " + prodElems.get(step);
            if (currentElementId == -1) {
                return new Production(result, prodId, 1);
            }
            return new Production(result, prodId, currentElementId + 1);
        } else {
            throw new Exception("Unable to derivate production" + toString());
        }
    }

    public int getProdId() {
        return prodId;
    }
}
