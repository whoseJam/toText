
#include "Storable.h"

using namespace std;

StoreID Storable::storeCurrentID = 0;

Storable::Storable() {
    //TODO mutex ? 
    storeID = storeCurrentID++;
}

StoreID Storable::getStoreID() const {
    return storeID;
}

StoreID Storable::getLastStoreID() const {
    return lastStoreID;
}

void Storable::setLastStoreID(StoreID id) {
    lastStoreID = id;
}