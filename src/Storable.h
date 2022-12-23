#pragma once

#include <string>
#include <fstream>
#include "StoreBase.h"

struct Storable {
public:
	Storable();
	virtual void encode(std::ofstream& file) = 0;
	virtual void encodeFirstClass(std::ofstream& file) = 0;
	virtual bool decode(const std::string& name, std::ifstream& file) = 0;
	virtual bool decodeFirstClass(std::ifstream& file) = 0;
	StoreID getStoreID() const;
	StoreID getLastStoreID() const;
protected:
	void setLastStoreID(StoreID id);
private:
	StoreID storeID;
	StoreID lastStoreID;
	static StoreID storeCurrentID;
};