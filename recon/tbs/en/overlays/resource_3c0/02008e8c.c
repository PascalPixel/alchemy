/* Draft of SuharaSabaku_RunSceneScript, resource_3c0 at 0x02008e8c (was
 * FIELD/SUHARA_SABAKU/SCENE_SCRIPT.C).
 * Remaining difference: the ROM loads scenes 0xa4 and 0xa5 from its literal
 * pool as link-time values, which this C spells as symbols no link defines.
 * The listing keeps these rows. */
#include "TYPES.H"
extern struct MapRenderWork *gMapWork;

void Engine_TaskAddCallback();
void Engine_AudioPlayCue();

/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
/* The game-state rows are read as halfwords and written as bytes through
 * one symbol, so both keep the base-plus-index address form. */
union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

s32 SuharaSabaku_RunSceneScript(void)
{
    u8 *map;
    s16 (*rows)[1];

    {
        u8 **globals = (u8 **)&gMapWork;

        map = globals[0];
        *(s32 *)(globals[19] + 448) = 0x201;
    }
    if (Value1(GameFlag_GetByte, 0x210) != 0) {
        gGameState.bytes[249][0] = 2;
        Call2(Engine_TaskAddCallback, (s32)Func_02000400, 0xc80);
    }
    rows = gGameState.halves;
    if (rows[224][0] == 0xa4 || rows[224][0] == 0xa5) {
        *(u16 *)0x02009a00 = *(u16 *)0x0500019e;
        Func_02000d24();
    }
    if (rows[224][0] == 0xa4) {
        SuharaSabaku_ApplyVisitFlagBlend();
    } else if (rows[224][0] == 0xa5) {
        SuharaSabaku_ApplyAltarFlagBlend();
    } else {
        Call1(Engine_AudioPlayCue, 0x120);
    }
    if (gGameState.halves[225][0] == 0) {
        *(u16 *)(map + 20) &= ~0x200;
    }
    return 0;
}
