#include "types.h"
#include "scene.h"
#include "abi/graphics/palette/step_fade_transfer.h"

struct Globals {
    void *target;
    u8 unknown[116];
    u8 *base;
};

extern struct Globals gBattleWork;

void Palette_StepFadeTransfer(void)
{
    u8 *base = gBattleWork.base;
    s32 *remaining = (s32 *)(base + 0x77B4);
    void *target = gBattleWork.target;

    if (*remaining > 0) {
        s32 *counter = (s32 *)(base + 0x77B8);
        s32 value = ++*counter;

        Sys_SetMode(
            (u8 *)target + 0x544,
            (void *)0x050000C0,
            0x10000 - value * 1092,
            128
        );
        (*remaining)--;
    }
}
