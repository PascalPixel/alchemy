#include "TYPES.H"

extern u8 Data_02008401[];


s32 Main_080770e0();
void Engine_TaskAddCallback();
void FieldScene_RunOpeningAuxiliarySequence();
void Local_02000f50();
void Local_02001094();
void Engine_AudioPlayCue();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_000000a4[];
extern u8 Data_000000a5[];
/* The game-state rows are read as halfwords and written as bytes through
 * one symbol, so both keep the base-plus-index address form. */
union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
};

extern union GameStateRows Data_02000240_t;

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
        u8 **globals = (u8 **)0x03001e70;

        map = globals[0];
        *(s32 *)(globals[19] + 448) = 0x201;
    }
    if (Value1(Main_080770e0, 0x210) != 0) {
        Data_02000240_t.bytes[249][0] = 2;
        Call2(Engine_TaskAddCallback, (s32)Data_02008401, 0xc80);
    }
    rows = Data_02000240_t.halves;
    if (rows[224][0] == (s32)Data_000000a4 || rows[224][0] == (s32)Data_000000a5) {
        *(u16 *)0x02009a00 = *(u16 *)0x0500019e;
        FieldScene_RunOpeningAuxiliarySequence();
    }
    if (rows[224][0] == (s32)Data_000000a4) {
        Local_02000f50();
    } else if (rows[224][0] == (s32)Data_000000a5) {
        Local_02001094();
    } else {
        Call1(Engine_AudioPlayCue, 0x120);
    }
    if (Data_02000240_t.halves[225][0] == 0) {
        *(u16 *)(map + 20) &= ~0x200;
    }
    return 0;
}
