#include "SORU.H"
#include "CALL.H"

/* The overlay's work, past its loaded image. */
s32 SoruIriguchi_CueTimer __attribute__((section(".bss")));

void SoruIriguchi_ApplyEntryState(void)
{
    GameFlag_Set(0x144);
    *(s32 *)((*(u8 *volatile *)&gWork + 0x1c0)) = 0x100;
    if (GameFlag_IsSet(0x814) != 0) {
        s32 *timer = &SoruIriguchi_CueTimer;
        s32 zero = 0;
        *timer = zero;
        Call2(Engine_TaskAddCallback, (s32)Scene_UpdateCueTimer, 0xc80);
    }
    if (GameFlag_IsSet(0x879) != 0) {
        Map_CopyCellAttributes(5, 6, 1, 1, 6, 6);
        Map_CopyCellAttributes(5, 6, 1, 1, 7, 6);
        Map_CopyCellAttributes(5, 6, 1, 1, 8, 6);
        Map_CopyCellAttributes(0, 1, 3, 1, 6, 5);
    }
    if (GameFlag_IsSet(FLAG_PARTY_LEFT_VALE) != 0) {
        Actor_SetPosition(8, 0x780000, 0xe80000);
        Map_CopyCellAttributes(2, 10, 1, 1, 6, 14);
        Map_CopyCellAttributes(2, 10, 1, 1, 7, 14);
        Map_CopyCellAttributes(2, 10, 1, 1, 8, 14);
    }
}
