#include "RAMAKAN.H"

void FieldScene_RunScene3a5_02000c6c(s32 a0)
{
    s32 i;
    s32 p8;
    s32 record;
    s32 value;
    s32 v5;
    s32 v6;

    p8 = a0;
    for (i = 0; i <= 2; i++) {
        value = Value0(Engine_RandomNext);
        v6 = (u32)((value << 1) + value) >> 16;
        v5 = v6 + 0x303;
        record = GameFlag_IsSet(v5);
        if (record == 0) {
            GameFlag_Set(v5);
            break;
        }
    }
    Event_Begin();
    Event_SetMessage((s32)((s32)(((s32)p8 << 1) + p8) + v6) + 0x1a10);
    Event_ShowMessage((v6 + 1), 0);
    Event_End();
}

