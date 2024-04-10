#pragma once

#include <string>
#include <fstream>

#include "Traiter.h"

namespace ttx {

/** Storable
 * 任何想要被序列化类都应该继承此基类
 * 设该类中需要被序列化的成员构成一个集合S
*/
struct Storable {
public:
    Storable();
    
    // 将stream反序列化输入到S中的成员
    virtual void encode(std::ostream& stream) = 0;

    // 将stream反序列化输入到S中的成员，并为自己分配一个StoreID
    virtual void encodeFirstClass(std::ostream& stream) = 0;

    // 将S<name>这个成员序列化
    virtual bool decode(const std::string& name, std::istream& stream) = 0;

    // 将S中的成员序列化输出到stream中，并且输出自己的StoreID
    virtual bool decodeFirstClass(std::istream& stream) = 0;

    void afterDecodeWrapper();

    StoreID getStoreID() const;
    StoreID getLastStoreID() const;
protected:
    virtual void afterDecode();
    void setLastStoreID(StoreID id);
private:
    bool afterDecodeFlag;
    StoreID storeID;
    StoreID lastStoreID;
    static StoreID storeCurrentID;
};

}