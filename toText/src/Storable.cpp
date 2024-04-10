
#include "Storable.h"

using namespace std;

namespace ttx {

StoreID Storable::storeCurrentID = 0;

Storable::Storable() {
    //TODO mutex ? 
    afterDecodeFlag = false;
    storeID = ++storeCurrentID;
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

void Storable::afterDecode() {
}

void Storable::afterDecodeWrapper() {
    if (afterDecodeFlag) return;
    afterDecodeFlag = true;
    afterDecode();
}

}