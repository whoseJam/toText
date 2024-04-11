
#include <vector>
#include <set>

#include "Storable.h"
#include "Traiter.h"
#include "Store.h"

using namespace std;

namespace ttx {

map<StoreID, Storable*> memory;
vector<Storable*> dependency;
set<Storable*> encoded;

string string2file(void* ptr) {
    string str = *((string*)ptr);
    if (str == "") return "<empty>";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == ' ') str[i] = '@';
        if (str[i] == '\n')str[i] = '$';
    }
    return str;
}
void file2string(void* ptr) {
    string& str = *((string*)ptr);
    if (str == "<empty>") str = "";
    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '@') str[i] = ' ';
        if (str[i] == '$') str[i] = '\n';
     }
}

Storable* load(std::string& className, istream& stream) {
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

vector<Storable*> load(istream& stream) {
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
    afterDecode();
    for (Storable* obj : ans) {
        obj->afterDecodeWrapper();
    }
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
    memory[item->getLastStoreID()] = item;
}
void addDependency(Storable* depend) {
    dependency.push_back(depend);
}
void afterDecode() {
    for (Storable* ptr : dependency) {
        StoreID id = *((StoreID*)ptr);
        Storable* item = getStorable(id);
        if (item == nullptr) {
            cout << "File Broken\n";
            exit(-1);
        }
        *((Storable**)ptr) = item;
    }
    dependency.clear();
    memory.clear();
}

void store(const string& path, vector<Storable*>& items) {
    encoded.clear();
    ofstream output;
    output.open(path);
    store(output, items);
    output.close();
}

void store(ostream& stream, vector<Storable*>& items) {
    for (auto& it : items) {
        if (encoded.find(it) != encoded.end())
            continue;
        it->encodeFirstClass(stream);
        encoded.insert(it);
    }
    while (dependency.size() > 0) {
        Storable* item = dependency[dependency.size() - 1];
        dependency.pop_back();
        if (encoded.find(item) != encoded.end())
            continue;
        item->encodeFirstClass(stream);
        encoded.insert(item);
    }
}

}

