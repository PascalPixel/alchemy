#include "TYPES.H"

void RamakanSabaku_ClaimSandEffectVram();
void Engine_TaskAddCallback();
void Main_080091b8();
void Main_0808a5e0();
void RamakanSabaku_ApplyEntryState();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_00000059[];
extern u8 Data_0000005a[];
extern u8 Data_0000005b[];
extern u8 Data_0000005c[];
extern u8 Data_02000240[];
extern u16 Data_02000240_t[][2];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Lamakan Desert: reset the area timers and, outside the town variant, open
 * the map cells of the stored area variant before applying the entry state. */
s32 RamakanSabaku_EnterArea(s32 a0, s32 a1)
{
    s32 v5;

    Data_02000240_t[139][0] = 0x258;
    Data_02000240_t[139][1] = 0;
    Data_02000240_t[140][0] = 0x119;
    if ((s16)Data_02000240_t[112][0] == (s32)Data_0000005c) {
    } else {
        *(s32 *)((*(s32 *)0x03001ebc + 0x1c0)) = 0x100;
        RamakanSabaku_ClaimSandEffectVram();
        Call2(Engine_TaskAddCallback, 0x2008cd1, 0xc80);
        if ((s16)Data_02000240_t[112][0] == (s32)Data_00000059) {
            Call6(Main_080091b8, 22, 7, 4, 2, 64, 126);
            Call6(Main_080091b8, 8, 10, 4, 2, 68, 126);
            Call6(Main_080091b8, 23, 21, 4, 2, 72, 126);
            Call6(Main_080091b8, 16, 42, 4, 2, 76, 126);
            Call6(Main_080091b8, 36, 44, 4, 2, 80, 126);
            Call6(Main_080091b8, 14, 55, 4, 2, 84, 126);
        } else {
            if ((s16)Data_02000240_t[112][0] != (s32)Data_0000005a) {
                goto L_020017fe;
            }
            Call6(Main_080091b8, 42, 5, 4, 2, 64, 126);
            Call6(Main_080091b8, 20, 11, 4, 2, 68, 126);
            Call6(Main_080091b8, 14, 12, 4, 2, 72, 126);
            Call6(Main_080091b8, 56, 18, 4, 2, 76, 126);
            Call6(Main_080091b8, 7, 22, 4, 2, 80, 126);
            Call6(Main_080091b8, 44, 23, 4, 2, 84, 126);
            Call6(Main_080091b8, 38, 24, 4, 2, 88, 126);
            Call6(Main_080091b8, 26, 28, 4, 2, 92, 126);
            Call6(Main_080091b8, 17, 35, 4, 2, 96, 126);
            Call6(Main_080091b8, 50, 36, 4, 2, 100, 126);
            Call6(Main_080091b8, 34, 43, 4, 2, 104, 126);
            Call6(Main_080091b8, 6, 46, 4, 2, 108, 126);
            Call6(Main_080091b8, 27, 55, 4, 2, 112, 126);
            Call6(Main_080091b8, 43, 56, 4, 2, 116, 126);
        }
        goto L_02001842;
        L_020017fe:;
        if ((s16)Data_02000240_t[112][0] == (s32)Data_0000005b) {
            v5 = 124;
            Main_0808a5e0(169);
            Call6(Main_080091b8, 8, 14, 4, 4, 64, v5);
            Call6(Main_080091b8, 6, 18, 4, 4, 68, v5);
            Call6(Main_080091b8, 10, 21, 4, 4, 72, v5);
        }
        L_02001842:;
        RamakanSabaku_ApplyEntryState();
    }
    return 0;
}
