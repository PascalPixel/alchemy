#include "types.h"
#include "scene.h"
#include "abi/overlays/scene/dialogue/topic/set_field_54.h"

extern u8 *Talk_Run(s32);

void OvObj_SetField54(s32 arg0, s32 arg1)
{
    u8 *entry = Talk_Run(arg0);

    if (entry != 0) {
        u8 *field = entry + 0x54;

        *field = arg1;
    }
}
