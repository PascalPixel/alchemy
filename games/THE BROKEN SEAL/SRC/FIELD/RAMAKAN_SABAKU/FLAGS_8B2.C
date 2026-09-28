#include "RAMAKAN.H"

extern const struct SceneEvent RamakanSabaku_Events[];

void FieldScene_RunFlags8B2And8B3Steps(void)
{
    u32 i;
    s32 record;

    Event_Begin();
    if (GameFlag_IsSet(0x8b2) == 0) {
        if (GameFlag_IsSet(0x8b3) == 0) {
            GameFlag_Set(0x8b3);
            GameFlag_Set(0x8b2);
        }
    }
    Audio_PlayCue(123);
    Event_RequestExit(3);
    Event_End();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return RamakanSabaku_Events;
}

