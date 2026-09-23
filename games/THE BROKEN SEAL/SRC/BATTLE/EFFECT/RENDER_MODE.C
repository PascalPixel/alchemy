#include "TYPES.H"
#include "SCENE.H"

/* Mode entries of the battle render effect. */

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

s32 BattleFx_RenderMode(void *effect, s32 mode);

void BattleFx_RenderMode0Or1(void *effect)
{
    if (FIELD_AT_OFFSET(effect, s32 *, 0x18) == 0) {
        BattleFx_RenderMode(effect, 0);
        return;
    }
    BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode1(void *effect)
{
    BattleFx_RenderMode(effect, 1);
}

void BattleFx_RenderMode2(void *effect)
{
    BattleFx_RenderMode(effect, 2);
}

void BattleFx_RenderMode3(void *effect)
{
    BattleFx_RenderMode(effect, 3);
}

void BattleFx_RenderMode4(void *effect)
{
    BattleFx_RenderMode(effect, 4);
}

void BattleFx_RenderMode5(void *effect)
{
    BattleFx_RenderMode(effect, 5);
}

void BattleFx_RenderMode7(void *effect)
{
    BattleFx_RenderMode(effect, 7);
}

void BattleFx_RenderMode6(void *effect)
{
    BattleFx_RenderMode(effect, 6);
}
