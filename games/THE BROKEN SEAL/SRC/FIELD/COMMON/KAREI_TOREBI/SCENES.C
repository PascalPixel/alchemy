#include "KAREI.H"

void FieldScene_RunScene3ae_020008cc(void)
{
    u32 i;
    s32 record;

    if (gGameState.entrance == 1) {
        if (GameFlag_IsSet(0x8ac) == 0) {
            GameFlag_Set(0x8ac);
            FieldScene_RunScene3aeSequenceA();
        }
    }
    if (gGameState.entrance == 2) {
        if (GameFlag_IsSet(0x109) == 0) {
            GameFlag_Clear(0x8a9);
        }
    }
    if (GameFlag_IsSet(0x911) != 0) {
        if (GameFlag_IsSet(0x8a9) == 0) {
            Actor_SetPosition(12, 0x580000, 0x5180000);
            Actor_FaceDirection(12, 0, 0);
        }
    }
}
