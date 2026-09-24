#include "TYPES.H"

void Engine_EventBegin();
s32 Engine_ActorGet();
void Engine_GameFlagSet();
void Engine_GameFlagClear();
s32 Engine_MapCopyCellAttributes();
void Engine_EventEnd();



/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

void MakyuriHeya_Func0200251c(void)
{
    s32 i;
    s32 flag;
    s32 x;

    Engine_EventBegin();
    for (i = 0, flag = 0x330; i <= 3; i++, flag += 2) {
        x = *(s32 *)(Value1(Engine_ActorGet, i + 15) + 8) / 0x100000;
        if (x == (i << 2) + 39) {
            Engine_GameFlagSet(flag);
            Engine_GameFlagClear(flag + 1);
        } else if (x == (i << 2) + 41) {
            Engine_GameFlagSet(flag + 1);
            Engine_GameFlagClear(flag);
        } else {
            Engine_GameFlagClear(flag);
            Engine_GameFlagClear(flag + 1);
        }
    }
    x = *(s32 *)(Value1(Engine_ActorGet, 19) + 8) / 0x100000;
    if (x == 57) {
        Call1(Engine_GameFlagSet, 0x338);
        Call1(Engine_GameFlagClear, 0x339);
        Call6(Engine_MapCopyCellAttributes, 53, 10, 1, 1, 58, 7);
    } else if (x == 59) {
        Call1(Engine_GameFlagSet, 0x339);
        Call1(Engine_GameFlagClear, 0x338);
        Call6(Engine_MapCopyCellAttributes, 53, 10, 1, 1, 58, 7);
    } else {
        Call1(Engine_GameFlagClear, 0x338);
        Call1(Engine_GameFlagClear, 0x339);
        Call6(Engine_MapCopyCellAttributes, 53, 11, 1, 1, 58, 7);
    }
    Engine_EventEnd();
}
