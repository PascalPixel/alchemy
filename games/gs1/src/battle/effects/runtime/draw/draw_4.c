#include "types.h"
#include "scene.h"

/* battle/effects/runtime/draw/render_mode_0_or_1.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 BattleFx_RenderMode(void *, s32);

void BattleFx_RenderMode0Or1(void *effect)
{
    if (FIELD_AT_OFFSET(effect, s32 *, 0x18) == 0) {
        BattleFx_RenderMode(effect, 0);
        return;
    }
    BattleFx_RenderMode(effect, 1);
}

/* battle/effects/runtime/draw/render_mode_1.c */
s32 BattleFx_RenderMode(s32, s32);

void BattleFx_RenderMode1(s32 effect)
{
    BattleFx_RenderMode(effect, 1);
}

/* battle/effects/runtime/draw/render_mode_2.c */
s32 BattleFx_RenderMode(s32, s32);

void BattleFx_RenderMode2(s32 effect)
{
    BattleFx_RenderMode(effect, 2);
}

/* battle/effects/runtime/draw/render_mode_3.c */
s32 BattleFx_RenderMode(s32, s32);

void BattleFx_RenderMode3(s32 effect)
{
    BattleFx_RenderMode(effect, 3);
}

/* battle/effects/runtime/draw/render_mode_4.c */
s32 BattleFx_RenderMode(s32, s32);

void BattleFx_RenderMode4(s32 effect)
{
    BattleFx_RenderMode(effect, 4);
}

/* battle/effects/runtime/draw/render_mode_5.c */
s32 BattleFx_RenderMode(s32, s32);

void BattleFx_RenderMode5(s32 effect)
{
    BattleFx_RenderMode(effect, 5);
}

/* battle/effects/runtime/draw/render_mode_7.c */
s32 BattleFx_RenderMode(s32, s32);

void BattleFx_RenderMode7(s32 effect)
{
    BattleFx_RenderMode(effect, 7);
}

/* battle/effects/runtime/draw/render_mode_6.c */
s32 BattleFx_RenderMode(s32, s32);

void BattleFx_RenderMode6(s32 effect)
{
    BattleFx_RenderMode(effect, 6);
}
