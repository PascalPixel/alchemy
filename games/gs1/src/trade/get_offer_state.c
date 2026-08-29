#include "types.h"

void *Owner_GetState();

s32 Trade_GetOfferState(s32 arg0) {
    if (arg0 != 0) {
        return Owner_GetState(0x83);
    }
    return 0x0200024C;
}
