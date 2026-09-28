#include "TOPIC.H"

void OverlayObject_SetField54(s32 arg0, s32 arg1)
{
    u8 *entry = (u8 *)Engine_ActorGet(arg0);

    if (entry != 0) {
        u8 *field = entry + 0x54;

        *field = arg1;
    }
}
