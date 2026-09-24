#include "TYPES.H"

void Engine_GameFlagClear();
void Engine_MapCopyCells();
void Engine_MapCopyCellAttributes();
void Engine_MapCopyCellsLayered();
void Engine_ActorSetPosition();
s32 Engine_MapObjectSetPosition();
s32 Engine_DisplayScrollStartHBlankDma();


extern u8 Data_00000059[];
extern u8 Data_0000005a[];
extern u8 Data_0000005b[];
extern u8 Data_0000005c[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

static __inline__ s32 Value7(s32 (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6)
{
    return f(a0, a1, a2, a3, a4, a5, a6);
}

/* Lamakan Desert entry: clear flag 0x201, lay out the map cells for the
 * entrance taken, park the scene actors and, away from entrance 0x5c,
 * start the heat-shimmer scroll. */
s32 RamakanSabaku_ApplyEntryState(void)
{
    u32 i;
    s32 record;
    s32 v5;

    Call1(Engine_GameFlagClear, 0x201);
    if (Data_02000240_t[224][0] == (s32)Data_00000059) {
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 22, 7);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 8, 10);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 23, 21);
        Call6(Engine_MapCopyCellAttributes, 70, 68, 4, 1, 23, 23);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 16, 42);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 36, 44);
        Call6(Engine_MapCopyCells, 70, 68, 4, 2, 14, 55);
    } else {
        if (Data_02000240_t[224][0] != (s32)Data_0000005a) {
        } else {
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 42, 5);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 20, 11);
            Call6(Engine_MapCopyCellAttributes, 70, 68, 4, 1, 20, 13);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 14, 12);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 56, 18);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 7, 22);
            Call6(Engine_MapCopyCellAttributes, 70, 68, 4, 1, 7, 24);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 44, 23);
            Call6(Engine_MapCopyCellAttributes, 70, 68, 4, 1, 44, 25);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 38, 24);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 26, 28);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 17, 35);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 50, 36);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 34, 43);
            Call6(Engine_MapCopyCellAttributes, 70, 68, 4, 1, 34, 45);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 6, 46);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 27, 55);
            Call6(Engine_MapCopyCells, 70, 68, 4, 2, 43, 56);
            goto L_020007e6;
        }
        if (Data_02000240_t[224][0] == (s32)Data_0000005b) {
            Call6(Engine_MapCopyCellsLayered, 69, 99, 4, 2, 8, 16);
            Call6(Engine_MapCopyCellsLayered, 69, 99, 4, 2, 6, 20);
            Call6(Engine_MapCopyCellsLayered, 69, 99, 4, 2, 10, 23);
            Call6(Engine_MapCopyCellAttributes, 69, 99, 4, 2, 8, 14);
            Call6(Engine_MapCopyCellAttributes, 69, 99, 4, 2, 6, 18);
            Call6(Engine_MapCopyCellAttributes, 69, 99, 4, 1, 6, 20);
            Call6(Engine_MapCopyCellAttributes, 69, 99, 4, 2, 10, 21);
            Call6(Engine_MapCopyCells, 0, 121, 5, 7, 8, 32);
            Call6(Engine_MapCopyCells, 0, 121, 5, 7, 43, 32);
            Call6(Engine_MapCopyCells, 6, 120, 3, 1, 9, 5);
            Call6(Engine_MapCopyCells, 9, 120, 3, 1, 44, 5);
            Call6(Engine_MapCopyCellAttributes, 9, 0, 3, 3, 9, 6);
        }
    }
    L_020007e6:;
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    Engine_ActorSetPosition(13, 0, 0);
    v5 = 100;
    do {
        record = Engine_MapObjectSetPosition(v5, 0, 0);
        v5 = (v5 + 1);
    } while (v5 <= 107);
    if (Data_02000240_t[224][0] != (s32)Data_0000005c) {
        record = Value7(Engine_DisplayScrollStartHBlankDma, 0, 0x40000, 0x10000, 0x2000, 0x10000, 0x8000, 0x4000);
        return record;
    }
    return record;
}
