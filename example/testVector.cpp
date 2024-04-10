#include "ToText.h"

#include <iostream>
#include <cassert>

using namespace std;

constexpr double eps = 1e-2;

template<typename T>
void checkVectorIsSameAs(const T& a, const T& b) {
    assert(a.size() == b.size());
    for (int i = 0; i < a.size(); i++)
        assert(a[i] == b[i]);
}

int dcmp(double x) {
    if (x > eps) return 1;
    return x < -eps ? -1 : 0;
}

class VectorGroup: public ttx::Storable {
    TTX_DECLARE(VectorGroup);
public:
    VectorGroup() = default;
    vector<int> intVector;
    vector<int*> intVectorPtr;
    vector<vector<int>> intVector2d;

    TTX_EXPORT(VectorGroup, doubleVector);
    TTX_EXPORT(VectorGroup, stringVector);

    void checkIsSameAs(const VectorGroup& other) {
        checkVectorIsSameAs(intVector, other.intVector);
        checkVectorIsSameAs(stringVector, other.stringVector);
        assert(doubleVector.size() == other.doubleVector.size());
        for (int i = 0; i < doubleVector.size(); i++)
            assert(dcmp(doubleVector[i] - other.doubleVector[i]) == 0);
        assert(intVectorPtr.size() == other.intVectorPtr.size());
        for (int i = 0; i < intVectorPtr.size(); i++)
            assert(*intVectorPtr[i] == *other.intVectorPtr[i]);
        assert(intVector2d.size() == other.intVector2d.size());
        for (int i = 0; i < intVector2d.size(); i++) {
            assert(intVector2d[i].size() == other.intVector2d[i].size());
            for (int j = 0; j < intVector2d[i].size(); j++)
                assert(intVector2d[i][j] == other.intVector2d[i][j]);
        }
    }
private:
    friend VectorGroup* randVectorGroup();
    vector<double> doubleVector;
    vector<string> stringVector;
};
TTX_IMPL(VectorGroup);
TTX_VARIABLE_PUB(VectorGroup, intVector);
TTX_VARIABLE_PUB(VectorGroup, intVectorPtr);
TTX_VARIABLE_PUB(VectorGroup, intVector2d);
TTX_VARIABLE_PRI(VectorGroup, doubleVector);
TTX_VARIABLE_PRI(VectorGroup, stringVector);

int R(int l, int r) {
    return rand() % (r - l + 1) + l;
}

double FR(double l, double r) {
    return (1.0 * rand() / RAND_MAX) * (r - l) + l;
}

string randomString(int len) {
    string ans;
    for (int i = 1; i <= len; i++)
        ans.push_back(R('a', 'z'));
    return ans;
}

VectorGroup* randVectorGroup() {
    int len = 10;
    VectorGroup* vg = new VectorGroup();
    for (int i = 0; i < len; i++) {
        vg->intVector.push_back(R(1, 10));
        vg->intVectorPtr.push_back(new int(R(1, 10)));
        vg->doubleVector.push_back(FR(1.0, 10.0));
        vg->stringVector.push_back(randomString(R(1, 4)));
        vg->intVector2d.push_back(vector{1, 3, 1, 3, 1});
    }
    return vg;
}


int main() {
    vector<ttx::Storable*> testdata = {
        randVectorGroup(),
        randVectorGroup()
    };
    ttx::store(std::cout, testdata);
    
    ttx::store("./temp.txt", testdata);
    
    vector<ttx::Storable*> result = ttx::load("./temp.txt");

    assert(result.size() == testdata.size());
    for (int i = 0; i < result.size(); i++) {
        VectorGroup* a = dynamic_cast<VectorGroup*>(result[i]);
        VectorGroup* b = dynamic_cast<VectorGroup*>(testdata[i]);
        a->checkIsSameAs(*b);
    }

    cout << "test vector passed\n";

    return 0;
}