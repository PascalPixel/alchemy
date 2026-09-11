#include "types.h"
#include "scene.h"
#include "abi/party/lookup_character_value_by_flag32.h"

extern s16 gRom[];
extern s16 gRom2[];
s32 GameFlag_IsSet(s32);

s32 Party_LookupCharacterValueByFlag32(u32 index)
{
    if (index > 8) {
        return 0;
    }
    if (GameFlag_IsSet(32) == 0) {
        return gRom[index];
    }
    return gRom2[index];
}
