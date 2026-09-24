#include "TYPES.H"
extern u8 Data_0200b2bc[];

s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
s32 Engine_GameStateSetReturn();
void Engine_Import0808a250();
void Engine_EventEnd();
void Engine_MapAnimateCells();
void Engine_AudioPlayCue();
void Engine_EventRequestExit();


/* FAKEMATCH: ids the reference loads from the literal pool rather than
 * building inline are spelled as link symbols at those values. */
extern u8 Data_0000007e[];
extern u8 Data_02000240[];
extern s16 Data_02000240_t[][1];

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call11(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10)
{
    f(a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10);
}

/* Crossbone Isle: on the first visit to an area (flag 0x8c8 + area) show its
 * title card; later visits animate the entrance cells and leave. */
void TakaraShima_RunAreaEntry(void)
{
    if (Engine_GameFlagIsSet(Data_02000240_t[224][0] + (0x8c8 - (s32)Data_0000007e)) == 0) {
        Engine_EventBegin();
        Engine_GameStateSetReturn(Data_02000240_t[224][0], 5);
        ((u8 *)Data_02000240_t)[0x22b] = 3;
        switch (Data_02000240_t[224][0] - (s32)Data_0000007e) {
        case 0:
            Engine_Import0808a250(63, 0);
            break;
        case 1:
            Engine_Import0808a250(63, 1);
            break;
        case 2:
            Engine_Import0808a250(63, 2);
            break;
        case 3:
            Engine_Import0808a250(63, 3);
            break;
        case 4:
            Engine_Import0808a250(84, 0);
            break;
        case 5:
            Engine_Import0808a250(84, 1);
            break;
        case 6:
            Engine_Import0808a250(84, 2);
            break;
        case 7:
            Engine_Import0808a250(84, 3);
            break;
        case 8:
            Engine_Import0808a250(84, 4);
            break;
        }
        Engine_EventEnd();
    } else {
        Engine_MapAnimateCells((s32)Data_0200b2bc, 44, 7);
        Engine_AudioPlayCue(183);
        Engine_EventRequestExit(3);
    }
}
