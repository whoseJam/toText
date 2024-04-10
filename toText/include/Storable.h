#pragma once

#include <string>
#include <fstream>

#include "Traiter.h"

namespace ttx {

/*    Storable
*    任何需要被序列化的类都应该继承自此基类
*/
struct Storable {
public:
    Storable();
    
    // 将名为name的成员变量序列化，输出到ostream中
    virtual void encode(std::ostream& stream) = 0;

    // 为实例创建一个唯一的StoreID
    virtual void encodeFirstClass(std::ostream& stream) = 0;

    // 将名为name的成员变量反序列化，从istream中输入
    virtual bool decode(const std::string& name, std::istream& stream) = 0;

    // 为实例创建一个唯一的StoreID
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