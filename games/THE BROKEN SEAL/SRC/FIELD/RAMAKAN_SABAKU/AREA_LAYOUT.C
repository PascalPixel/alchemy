#include "TYPES.H"

void Engine_GameFlagClear();
void Engine_GameFlagSet();
void Main_080091b8();
void Engine_MapCopyCellAttributes();
void Engine_ActorSetPosition();
s32 Engine_GameFlagIsSet();
void Engine_MapObjectSetPosition();
s32 Main_0808a3d0();
s32 Main_0808a380();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
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

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Lamakan Desert: open the map cells and place the actors for the area
 * variant stored at +0x1c0 of the game state, then reset the map objects. */
s32 RamakanSabaku_ConfigureAreaLayout(void)
{
    s32 i;
    s32 record;

    Call1(Engine_GameFlagClear, 0x200);
    Call1(Engine_GameFlagSet, 0x201);
    if (Data_02000240_t[224][0] == (s32)Data_00000059) {
        Call6(Main_080091b8, 64, 126, 4, 2, 22, 7);
        Call6(Main_080091b8, 68, 126, 4, 2, 8, 10);
        Call6(Main_080091b8, 72, 126, 4, 2, 23, 21);
        Call6(Engine_MapCopyCellAttributes, 72, 126, 4, 2, 23, 22);
        Call6(Main_080091b8, 76, 126, 4, 2, 16, 42);
        Call6(Main_080091b8, 80, 126, 4, 2, 36, 44);
        Call6(Main_080091b8, 84, 126, 4, 2, 14, 55);
        Call3(Engine_ActorSetPosition, 9, 0x1900000, 0x16c0000);
    } else {
        if (Data_02000240_t[224][0] != (s32)Data_0000005a) {
            goto L_02000af6;
        }
        Call6(Main_080091b8, 64, 126, 4, 2, 42, 5);
        Call6(Main_080091b8, 68, 126, 4, 2, 20, 11);
        Call6(Engine_MapCopyCellAttributes, 68, 126, 4, 2, 20, 12);
        Call6(Main_080091b8, 72, 126, 4, 2, 14, 12);
        Call6(Main_080091b8, 76, 126, 4, 2, 56, 18);
        Call6(Main_080091b8, 80, 126, 4, 2, 7, 22);
        Call6(Engine_MapCopyCellAttributes, 80, 126, 4, 2, 7, 23);
        Call6(Main_080091b8, 84, 126, 4, 2, 44, 23);
        Call6(Engine_MapCopyCellAttributes, 84, 126, 4, 2, 44, 24);
        Call6(Main_080091b8, 88, 126, 4, 2, 38, 24);
        Call6(Main_080091b8, 92, 126, 4, 2, 26, 28);
        Call6(Main_080091b8, 96, 126, 4, 2, 17, 35);
        Call6(Main_080091b8, 100, 126, 4, 2, 50, 36);
        Call6(Main_080091b8, 104, 126, 4, 2, 34, 43);
        Call6(Engine_MapCopyCellAttributes, 104, 126, 4, 2, 34, 44);
        Call6(Main_080091b8, 108, 126, 4, 2, 6, 46);
        Call6(Main_080091b8, 112, 126, 4, 2, 27, 55);
        Call6(Main_080091b8, 116, 126, 4, 2, 43, 56);
        Call3(Engine_ActorSetPosition, 9, 0x1600000, 0xcc0000);
        Call3(Engine_ActorSetPosition, 10, 0x2e00000, 0x18c0000);
        Call3(Engine_ActorSetPosition, 11, 0x900000, 0x17c0000);
        Call3(Engine_ActorSetPosition, 12, 0x2400000, 0x2cc0000);
        Call3(Engine_ActorSetPosition, 13, 0x2880000, 0x1980000);
    }
    goto L_02000bde;
    L_02000af6:;
    if (Data_02000240_t[224][0] == (s32)Data_0000005b) {
        Call6(Main_080091b8, 64, 124, 4, 4, 8, 14);
        Call6(Main_080091b8, 68, 124, 4, 4, 6, 18);
        Call6(Engine_MapCopyCellAttributes, 68, 124, 4, 1, 6, 20);
        Call6(Main_080091b8, 72, 124, 4, 4, 10, 21);
        Call6(Main_080091b8, 10, 121, 5, 7, 8, 32);
        Call6(Main_080091b8, 5, 121, 5, 7, 43, 32);
        Call6(Main_080091b8, 0, 120, 3, 1, 9, 5);
        Call6(Main_080091b8, 3, 120, 3, 1, 44, 5);
        Call3(Engine_ActorSetPosition, 8, 0xa80000, 0x5c0000);
        Call3(Engine_ActorSetPosition, 9, 0x800000, 0x13c0000);
        Call6(Engine_MapCopyCellAttributes, 6, 0, 3, 3, 9, 6);
        if (Value1(Engine_GameFlagIsSet, 0x90a) == 0) {
            Call6(Main_080091b8, 0, 119, 3, 1, 9, 5);
        }
    }
    L_02000bde:;
    for (i = 100; i <= 107; i++) {
        Call3(Engine_MapObjectSetPosition, i, -1, -1);
    }
    record = Main_0808a3d0();
    if (Data_02000240_t[224][0] != (s32)Data_0000005c) {
        record = Main_0808a380();
        return record;
    }
    return record;
}
