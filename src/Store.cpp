
#include <vector>
#include <set>

#include "Store.h"
#include "StoreBase.h"

using namespace std;

namespace Store {
	map<StoreID, Storable*> memory;
	vector<Storable*> dependency;
	set<Storable*> encoded;

	Storable* load(std::string& className, ifstream& stream) {
		string name;
		Storable* item = Factory<Storable>::get(className);
		item->decodeFirstClass(stream);
		stream >> name;
		while (name != "[endclass]") {
			item->decode(name, stream);
			stream >> name;
		}
		return item;
	}

	vector<Storable*> load(ifstream& stream) {
		string className;
		vector<Storable*> ans;
		stream >> className; //[beginclass]
		while (className == "[beginclass]") {
			stream >> className; //className
			Storable* tmp = load(className, stream);
			if (tmp == nullptr) break;
			ans.push_back(tmp);
			stream >> className; //[beginclass] or empty
		}
		for (Storable* obj : ans)
			obj->afterDecode();
		return ans;
	}

	vector<Storable*> load(const string& path) {
		ifstream input;
		input.open(path);
		return load(input);
	}

	Storable* getStorable(StoreID id) {
		if (memory.find(id) != memory.end())
			return memory[id];
		return nullptr;
	}
	void addStorable(Storable* item) {
		std::cout << "insert " << item->getLastStoreID() << "\n";
		memory[item->getLastStoreID()] = item;
	}
	void addDependency(Storable* depend) {
		dependency.push_back(depend);
	}
	void afterDecode() {
		for (Storable* ptr : dependency) {
			StoreID id = *((StoreID*)ptr);
			cout << "id=" << id << " addr="<<ptr<<"\n";
			Storable* item = getStorable(id);
			if (item == nullptr) {
				cout << "File Broken\n";
				exit(-1);
			}
			*((Storable**)ptr) = item;
		}
		dependency.clear();
	}
    
    void store(const string& path, vector<Storable*> objects) {
		//encoded.clear();
		std::cout << "Object_size=" << path << " " << encoded.size() << "\n";
        ofstream output;
        output.open(path);
        for (Storable* object : objects) {
			std::cout << "haha\n";

			if (encoded.find(object) != encoded.end()) {
				std::cout << "continue\n";
				continue;
			}
            object->encodeFirstClass(output);
			encoded.insert(object);
			std::cout << "output\n";
        }
		while (dependency.size() > 0) {
			Storable* object = dependency[dependency.size() - 1];
			dependency.pop_back();
			if (encoded.find(object) != encoded.end())
				continue;
			object->encodeFirstClass(output);
			encoded.insert(object);
		}
		output.close();
    }
}

