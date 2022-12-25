#pragma once

#include <type_traits>
#include <vector>
#include <string>
#include <fstream>

#include "Storable.h"
#include "Factory.h"
#include "Storage.h"

namespace Store {
    template<typename T> 
    T* filter(const std::vector<Storable*>& vec) {
        T* ret = nullptr;
        for (Storable* it : vec) {
            if ((ret = dynamic_cast<T*>(it)) != nullptr)
                return ret;
        }
        return ret;
    }
}

#define STORE_PUBLIC(Class,Value) \
	Store::Storage<Class>::Public<decltype(Class::Value)> \
    __STORE_##Class##_##Value(#Value,offsetof(Class,Class::Value));
#define STORE_PRIVATE(Class,Value) \
	struct __STORE_##Class##_##Value##_helper { \
        __STORE_##Class##_##Value##_helper() { \
            Store::Storage<Class>::Public<decltype(Class::Value)> \
            Register(#Value,offsetof(Class,Class::Value)); \
        } \
    } __STORE_##Class##_##Value;
#define STORE_EXPORT_HELPER(Class,Value) \
    struct __STORE_##Class##_##Value##_helper;
#define STORE_EXPORT(Class,Value) \
    friend __STORE_##Class##_##Value##_helper;

#define STORE_CHECK_SELF_LOAD_SUCCESSFUL(self) \
    if (Store::Storage<self>::getInstance()->decode(this, name, stream)) return true;
#define STORE_CHECK_PARENT_LOAD_SUCCESSFUL(parent) \
    if (parent::decode(name, stream)) return true;
#define STORE_ENCODE_PARENT(parent) \
    parent::encode(stream);

#define STORE_ENCODE_BODY_1(self) \
    Store::Storage<self>::getInstance()->encode(this, stream);
#define STORE_ENCODE_BODY_2(self,p0) \
    STORE_ENCODE_BODY_1(self); \
    STORE_ENCODE_PARENT(p0);
#define STORE_ENCODE_BODY_3(self,p0,p1) \
    STORE_ENCODE_BODY_2(self,p0); \
    STORE_ENCODE_PARENT(p1);
#define STORE_ENCODE_BODY_4(self,p0,p1,p2) \
    STORE_ENCODE_BODY_3(self,p0,p1); \
    STORE_ENCODE_PARENT(p2);

#define STORE_DECODE_BODY_1(self) \
    STORE_CHECK_SELF_LOAD_SUCCESSFUL(self);
#define STORE_DECODE_BODY_2(self,p0) \
    STORE_DECODE_BODY_1(self); \
    STORE_CHECK_PARENT_LOAD_SUCCESSFUL(p0);
#define STORE_DECODE_BODY_3(self,p0,p1) \
    STORE_DECODE_BODY_2(self,p0); \
    STORE_CHECK_PARENT_LOAD_SUCCESSFUL(p1);
#define STORE_DECODE_BODY_4(self,p0,p1,p2) \
    STORE_DECODE_BODY_3(self,p0,p1); \
    STORE_CHECK_PARENT_LOAD_SUCCESSFUL(p2);

#define STORE_DECODE_CLASS(self) \
    StoreID value; stream >> value; setLastStoreID(value); Store::addStorable(this);
#define STORE_ENCODE_CLASS_HEAD(self) \
    stream << "[class] " << #self << ' ' << getStoreID() <<' ';
#define STORE_ENCODE_CLASS_FOOT(self) \
    stream << "[endclass]\n";

#define STORABLE_DECLARE \
    virtual bool decode(const std::string& name, std::ifstream& stream) override; \
    virtual bool decodeFirstClass(std::ifstream& stream) override; \
    virtual void encode(std::ofstream& stream) override; \
    virtual void encodeFirstClass(std::ofstream& stream) override;
#define STORABLE_DECLARE_AND_IMPL(self) \
    virtual bool decode(const std::string& name, std::ifstream& stream) override { STORE_DECODE_BODY_1(self); return false; } \
    virtual bool decodeFirstClass(std::ifstream& stream) override { STORE_DECODE_CLASS(self); return false; } \
    virtual void encode(std::ofstream& stream) override { STORE_ENCODE_BODY_1(self); } \
    virtual void encodeFirstClass(std::ofstream& stream) override { STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_1(self); STORE_ENCODE_CLASS_FOOT(self);}
#define STORABLE_DECLARE_AND_IMPL_1(self,p0) \
    virtual bool decode(const std::string& name, std::ifstream& stream) override { STORE_DECODE_BODY_2(self,p0); return false; } \
    virtual bool decodeFistClass(std::ifstream& stream) override { STORE_DECODE_CLASS(self); return false;} \
    virtual void encode(std::ofstream& stream) override { STORE_ENCODE_BODY_2(self,p0); } \
    virtual void encodeFirstClass(std::ofstream& stream) override { STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_2(self,p0); STORE_ENCODE_CLASS_FOOT(self);}
#define STORABLE_DECLARE_AND_IMPL_2(self,p0,p1) \
    virtual bool decode(const std::string& name, std::ifstream& stream) override { STORE_DECODE_BODY_3(self,p0,p1); return false; } \
    virtual bool decodeFirstClass(std::ifstream& stream) override { STORE_DECODE_CLASS(self); return false; } \
    virtual void encode(std::ofstream& stream) override { STORE_ENCODE_BODY_3(self,p0,p1); } \
    virtual void encodeFirstClass(std::ofstream& stream) override { STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_3(self,p0,p1); STORE_ENCODE_CLASS_FOOT(self);}
#define STORABLE_DECLARE_AND_IMPL_3(self,p0,p1,p2) \
    virtual bool decode(const std::string& name, std::ifstream& stream) override { STORE_DECODE_BODY_4(self,p0,p1,p2); return false; } \
    virtual bool decodeFirstClass(std::ifstream& stream) override { STORE_DECODE_CLASS(self); return false; } \
    virtual void encode(std::ofstream& stream) override { STORE_ENCODE_BODY_4(self,p0,p1,p2); } \
    virtual void encodeFirstClass(std::ofstream& stream) override { STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_4(self,p0,p1,p2); STORE_ENCODE_CLASS_FOOT(self);}
#define STORABLE_IMPL(self) \
    bool self::decode(const std::string& name, std::ifstream& stream) { STORE_DECODE_BODY_1(self); return false; } \
    bool self::decodeFirstClass(std::ifstream& stream) {STORE_DECODE_CLASS(self); return false; } \
    void self::encode(std::ofstream& stream) {STORE_ENCODE_BODY_1(self);} \
    void self::encodeFirstClass(std::ofstream& stream) {STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_1(self); STORE_ENCODE_CLASS_FOOT(self); }
#define STORABLE_IMPL_1(self,p0) \
    bool self::decode(const std::string& name, std::ifstream& stream) { STORE_DECODE_BODY_2(self,p0); return false; } \
    bool self::decodeFirstClass(std::ifstream& stream) {STORE_DECODE_CLASS(self); return false; } \
    void self::encode(std::ofstream& stream) {STORE_ENCODE_BODY_2(self,p0);} \
    void self::encodeFirstClass(std::ofstream& stream) {STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_2(self,p0); STORE_ENCODE_CLASS_FOOT(self); }
#define STORABLE_IMPL_2(self,p0,p1) \
    bool self::decode(const std::string& name, std::ifstream& stream) { STORE_DECODE_BODY_3(self,p0,p1); return false; } \
    bool self::decodeFirstClass(std::ifstream& stream) {STORE_DECODE_CLASS(self); return false; } \
    void self::encode(std::ofstream& stream) {STORE_ENCODE_BODY_3(self,p0,p1);} \
    void self::encodeFirstClass(std::ofstream& stream) {STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_3(self,p0,p1); STORE_ENCODE_CLASS_FOOT(self); }
#define STORABLE_IMPL_3(self,p0,p1,p2) \
    bool self::decode(const std::string& name, std::ifstream& stream) { STORE_DECODE_BODY_4(self,p0,p1,p2); return false; } \
    bool self::decodeFirstClass(std::ifstream& stream) {STORE_DECODE_CLASS(self); return false; } \
    void self::encode(std::ofstream& stream) {STORE_ENCODE_BODY_2(self,p0,p1,p2);} \
    void self::encodeFirstClass(std::ofstream& stream) {STORE_ENCODE_CLASS_HEAD(self); STORE_ENCODE_BODY_4(self,p0,p1,p2); STORE_ENCODE_CLASS_FOOT(self); }

#define STORE(Class) \
    Store::Factory<Storable>::registerClass<Class> __STORE_FACTORY_##Class(#Class)

namespace Store {
    void store(const std::string& path, std::vector<Storable*>);
    std::vector<Storable*> load(std::ifstream& stream);
    std::vector<Storable*> load(const std::string& path);
}