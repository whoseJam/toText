#include "ToText.h"

#include <iostream>

using namespace std;

struct MapWrapper: public ttx::Storable {
    TTX_DECLARE(MapWrapper);
public:
    MapWrapper() = default;
    map<string, int> count;
};
TTX_IMPL(MapWrapper);
TTX_VARIABLE_PUB(MapWrapper, count);

int main() {
    MapWrapper* mp = new MapWrapper();
    mp->count["string"] = 5;
    mp->count["tag"] = 10;
    mp->count["reorder"] = 20;
    vector<ttx::Storable*> testdata = {mp};
    ttx::store(cout, testdata);
    ttx::store("./temp.txt", testdata);

    vector<ttx::Storable*> result = ttx::load("./temp.txt");
    MapWrapper* Mp = dynamic_cast<MapWrapper*>(result[0]);
    assert(Mp != nullptr);
    
    vector<pair<string, int>> testdataMap;
    vector<pair<string, int>> resultMap;
    for (auto& it : mp->count) testdataMap.push_back(it);
    for (auto& it : Mp->count) resultMap.push_back(it);
    assert(testdataMap.size() == resultMap.size());
    for (int i = 0; i < resultMap.size(); i++) {
        assert(testdataMap[i].first == resultMap[i].first);
        assert(testdataMap[i].second == resultMap[i].second);
    }
    cout << "test map passed\n";
    return 0;
}