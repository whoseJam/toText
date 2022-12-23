
#include <vector>
#include <set>

#include "Store.h"
#include "StoreBase.h"

using namespace std;

namespace Store {
	map<StoreID, Storable*> memory;
	vector<void*> delay;
	vector<Storable*> publicDependency;
	vector<
		pair<
		function<Storable* (void)>,
		function<void(Storable*)>
		>
	> privateDependency;
	set<Storable*> encoded;

	Storable* getStorable(StoreID id) {
		if (memory.find(id) != memory.end())
			return memory[id];
		return nullptr;
	}
	void addStorable(Storable* item) {
		memory[item->getLastStoreID()] = item;
	}
	void addDependency(
		function<Storable*(void)> getter, 
		function<void(Storable*)> setter) {
		privateDependency.push_back(
			make_pair(getter, setter)
		);
	}
	void addDependency(Storable* depend) {
		publicDependency.push_back(depend);
	}
	void afterDecode() {
		for (Storable* ptr : publicDependency) {
			StoreID id = *((StoreID*)ptr);
			cout << "id=" << id << " addr="<<ptr<<"\n";
			Storable* item = getStorable(id);
			if (item == nullptr) {
				cout << "File Broken\n";
				exit(-1);
			}
			*((Storable**)ptr) = item;
		}
		publicDependency.clear();
		for (auto& it : privateDependency) {
			auto getter = it.first;
			auto setter = it.second;
			StoreID id = *((StoreID*)getter());
			Storable* item = getStorable(id);
			if (item == nullptr) {
				cout << "File Broken\n";
				exit(-1);
			}
			setter(item);
		}
		privateDependency.clear();
	}
    
    void store(const string& path, vector<Storable*> objects) {
        ofstream output;
        output.open(path);
        for (Storable* object : objects) {
			if (encoded.find(object) != encoded.end())
				continue;
            object->encodeFirstClass(output);
			encoded.insert(object);
        }
		while (publicDependency.size() > 0) {
			Storable* object = publicDependency[publicDependency.size() - 1];
			publicDependency.pop_back();
			if (encoded.find(object) != encoded.end())
				continue;
			object->encodeFirstClass(output);
			encoded.insert(object);
		}
    }
}

