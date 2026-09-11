#include "types.h"
#include "scene.h"

/* party/remap_character_id_by_flags.c */
s32 GameFlag_IsSet(s32);

s32 Party_RemapCharacterIdByFlags(s32 arg0)
{
    s32 result;

    result = arg0;
    if (result <= 8) {
        if (GameFlag_IsSet(0x20) != 0) {
            if (result == 0) {
                result = 0x12;
            }
            if (result == 1) {
                result = 0x13;
            }
        } else if ((GameFlag_IsSet(0x21) != 0) && (result == 0)) {
            result = 0x11;
        }
    }
    return result;
}

/* party/remap_character_id_if_enabled.c */
s32 Party_RemapCharacterIdIfEnabled(s32 arg0, s32 arg1)
{
    s32 result;

    result = arg0;
    if ((result <= 8) && (arg1 != 0)) {
        if (result == 0) {
            result = 0x12;
        }
        if (result == 1) {
            result = 0x13;
        }
    }
    return result;
}
