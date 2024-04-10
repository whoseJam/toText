#pragma once

#include <map>
#include <iostream>
#include <string>
#include <functional>

namespace ttx {

template<typename BaseClass>
class Factory {
public:
    template<typename DerivedClass>
    class registerClass {
    public:
        template<typename... Args>
        registerClass(const std::string& key, Args... args) {
            std::function<BaseClass* ()> func = [=]() {return new DerivedClass(args...); };
            Factory<BaseClass>::getInstance()->
                Map[key] = func;
        }
    };
    static BaseClass* get(const std::string& key) {
        std::function<BaseClass* ()> fun = Factory::getInstance()->find(key);
        return fun();
    }
protected:
private:
    Factory() {};
    Factory(const Factory&) = delete;
    Factory(Factory&&) = delete;
    Factory& operator=(const Factory&) = delete;
    static Factory<BaseClass>* getInstance() {
        static Factory<BaseClass> f;
        return &f;
    }
    std::function<BaseClass* ()> find(const std::string& key) {
        auto it = Map.find(key);
        if (it == Map.end()) {
            std::cout << "key = " << key << " not found (factory)";
            exit(-1);
        }
        return it->second;
    }
    std::map<std::string, std::function<BaseClass* ()>> Map;
};

}