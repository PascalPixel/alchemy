#include "types.h"

s32 Object_Destroy();

void Object_DestroyIfPresent(s32 arg0) {
    if (arg0 != 0) {
        Object_Destroy();
    }
}
