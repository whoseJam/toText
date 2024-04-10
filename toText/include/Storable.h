#pragma once

#include <string>
#include <fstream>

#include "Traiter.h"

namespace ttx {

/*    Storable
*    �κ���Ҫ�����л����඼Ӧ�ü̳��Դ˻���
*/
struct Storable {
public:
    Storable();
    
    // ����Ϊname�ĳ�Ա�������л��������ostream��
    virtual void encode(std::ostream& stream) = 0;

    // Ϊʵ������һ��Ψһ��StoreID
    virtual void encodeFirstClass(std::ostream& stream) = 0;

    // ����Ϊname�ĳ�Ա���������л�����istream������
    virtual bool decode(const std::string& name, std::istream& stream) = 0;

    // Ϊʵ������һ��Ψһ��StoreID
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