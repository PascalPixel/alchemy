#include "types.h"

s32 Owner_GetState(s32);

s32 Trade_GetOfferState(s32 arg0) {
    if (arg0 != 0) {
        return Owner_GetState(0x83);
    }
    return 0x0200024C;
}
