#pragma once

#include <type_traits>
#include <vector>
#include <string>
#include <fstream>

#include "Storable.h"
#include "Factory.h"
#include "Storage.h"

namespace ttx {

template<typename T> 
T* filter(const std::vector<Storable*>& vec) {
    T* ret = nullptr;
    for (Storable* it : vec) {
        if ((ret = dynamic_cast<T*>(it)) != nullptr)
            return ret;
    }
    return ret;
}

void store(const std::string& path, std::vector<Storable*>& items);
void store(std::ostream& stream, std::vector<Storable*>& items);

std::vector<Storable*> load(std::istream& stream);
std::vector<Storable*> load(const std::string& path);

void file2string(void* ptr);
std::string string2file(void* str);
void addStorable(Storable* item);
void addDependency(Storable* item);
void afterDecode();

}

// 定义 Class 的 Value 为一个需要被序列化的非公有成员
#define TTX_VARIABLE_PRI(Class, Value) \
    Class::__ttx_##Class##_##Value##_helper::__ttx_##Class##_##Value##_helper() { \
        ttx::Storage<Class>::Variable<decltype(Class::Value)> \
            Register(#Value, offsetof(Class,Class::Value)); \
    } \
    Class::__ttx_##Class##_##Value##_helper __ttx_##Class##_##Value;

// 定义 Class 的 Value 为一个需要被序列化的公有成员
#define TTX_VARIABLE_PUB(Class, Value) \
    struct __ttx_##Class##_##Value##_helper { \
        __ttx_##Class##_##Value##_helper() { \
            ttx::Storage<Class>::Variable<decltype(Class::Value)> \
                Register(#Value, offsetof(Class, Class::Value)); \
        } \
    } __ttx_##Class##_##Value;

// 导出 Class 的私有成员 Value
#define TTX_EXPORT(Class, Value) \
    struct __ttx_##Class##_##Value##_helper { \
        __ttx_##Class##_##Value##_helper(); \
    };

#define TTX_CHECK_SELF_LOAD_SUCCESSFUL(self) \
    if (ttx::Storage<self>::getInstance()->decode(this, name, stream)) return true;

#define TTX_CHECK_PARENT_LOAD_SUCCESSFUL(parent) \
    if (parent::decode(name, stream)) return true;

#define TTX_ENCODE_PARENT(parent) \
    parent::encode(stream);

#define TTX_ENCODE_BODY_1(self) \
    ttx::Storage<self>::getInstance()->encode(this, stream);
#define TTX_ENCODE_BODY_2(self, p0) \
    TTX_ENCODE_BODY_1(self); \
    TTX_ENCODE_PARENT(p0);
#define TTX_ENCODE_BODY_3(self, p0, p1) \
    TTX_ENCODE_BODY_2(self, p0); \
    TTX_ENCODE_PARENT(p1);
#define TTX_ENCODE_BODY_4(self, p0, p1, p2) \
    TTX_ENCODE_BODY_3(self, p0, p1); \
    TTX_ENCODE_PARENT(p2);

#define TTX_DECODE_BODY_1(self) \
    TTX_CHECK_SELF_LOAD_SUCCESSFUL(self);
#define TTX_DECODE_BODY_2(self, p0) \
    TTX_DECODE_BODY_1(self); \
    TTX_CHECK_PARENT_LOAD_SUCCESSFUL(p0);
#define TTX_DECODE_BODY_3(self, p0, p1) \
    TTX_DECODE_BODY_2(self, p0); \
    TTX_CHECK_PARENT_LOAD_SUCCESSFUL(p1);
#define TTX_DECODE_BODY_4(self, p0, p1, p2) \
    TTX_DECODE_BODY_3(self, p0, p1); \
    TTX_CHECK_PARENT_LOAD_SUCCESSFUL(p2);

#define TTX_DECODE_CLASS(self) \
    StoreID value; stream >> value; setLastStoreID(value); ttx::addStorable(this);
#define TTX_ENCODE_CLASS_HEAD(self) \
    stream << "[beginclass] " << #self << ' ' << getStoreID() <<' ';
#define TTX_ENCODE_CLASS_FOOT(self) \
    stream << "[endclass]\n";

#define TTX_PARENT_CREATE_1(p0) \
    p0(0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
#define TTX_PARENT_CREATE_2(p0,p1) \
    TTX_PARENT_CREATE_1(p0), TTX_PARENT_CREATE_1(p1)
#define TTX_PARENT_CREATE_3(p0,p1,p2) \
    TTX_PARENT_CREATE_2(p0, p1), TTX_PARENT_CREATE_1(p2)

#define __TTX_REGISTER(Class) \
    ttx::Factory<ttx::Storable>::registerClass<Class> __TTX_FACTORY_##Class(#Class, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1);

#define TTX_DECLARE(self) \
protected: \
    friend ttx::Factory<Storable>; \
    self(int,int,int,int,int,int,int,int,int,int); \
public: \
    virtual bool decode(const std::string& name, std::istream& stream) override; \
    virtual bool decodeFirstClass(std::istream& stream) override; \
    virtual void encode(std::ostream& stream) override; \
    virtual void encodeFirstClass(std::ostream& stream) override;

#define TTX_IMPL(self) \
    self::self(int,int,int,int,int,int,int,int,int,int) { } \
    bool self::decode(const std::string& name, std::istream& stream) { \
        TTX_DECODE_BODY_1(self); \
        return false; \
    } \
    bool self::decodeFirstClass(std::istream& stream) { \
        TTX_DECODE_CLASS(self); \
        return false; \
    } \
    void self::encode(std::ostream& stream) { \
        TTX_ENCODE_BODY_1(self); \
    } \
    void self::encodeFirstClass(std::ostream& stream) { \
        TTX_ENCODE_CLASS_HEAD(self); \
        TTX_ENCODE_BODY_1(self); \
        TTX_ENCODE_CLASS_FOOT(self); \
    } \
    __TTX_REGISTER(self)

#define TTX_IMPL_1(self,p0) \
    self::self(int,int,int,int,int,int,int,int,int,int) : TTX_PARENT_CREATE_1(p0) { } \
    bool self::decode(const std::string& name, std::istream& stream) { TTX_DECODE_BODY_2(self,p0); return false; } \
    bool self::decodeFirstClass(std::istream& stream) {TTX_DECODE_CLASS(self); return false; } \
    void self::encode(std::ostream& stream) {TTX_ENCODE_BODY_2(self,p0);} \
    void self::encodeFirstClass(std::ostream& stream) {TTX_ENCODE_CLASS_HEAD(self); TTX_ENCODE_BODY_2(self,p0); TTX_ENCODE_CLASS_FOOT(self); } \
    __TTX_REGISTER(self);

#define TTX_IMPL_2(self,p0,p1) \
    self::self(int,int,int,int,int,int,int,int,int,int) : TTX_PARENT_CREATE_2(p0,p1) { } \
    bool self::decode(const std::string& name, std::istream& stream) { TTX_DECODE_BODY_3(self,p0,p1); return false; } \
    bool self::decodeFirstClass(std::istream& stream) {TTX_DECODE_CLASS(self); return false; } \
    void self::encode(std::ostream& stream) {TTX_ENCODE_BODY_3(self,p0,p1);} \
    void self::encodeFirstClass(std::ostream& stream) {TTX_ENCODE_CLASS_HEAD(self); TTX_ENCODE_BODY_3(self,p0,p1); TTX_ENCODE_CLASS_FOOT(self); } \
    __TTX_REGISTER(self);

#define TTX_IMPL_3(self,p0,p1,p2) \
    self::self(int,int,int,int,int,int,int,int,int,int) : TTX_PARENT_CREATE_3(p0,p1,p2) { } \
    bool self::decode(const std::string& name, std::istream& stream) { TTX_DECODE_BODY_4(self,p0,p1,p2); return false; } \
    bool self::decodeFirstClass(std::istream& stream) {TTX_DECODE_CLASS(self); return false; } \
    void self::encode(std::ostream& stream) {TTX_ENCODE_BODY_2(self,p0,p1,p2);} \
    void self::encodeFirstClass(std::ostream& stream) {TTX_ENCODE_CLASS_HEAD(self); TTX_ENCODE_BODY_4(self,p0,p1,p2); TTX_ENCODE_CLASS_FOOT(self); } \
    __TTX_REGISTER(self)
