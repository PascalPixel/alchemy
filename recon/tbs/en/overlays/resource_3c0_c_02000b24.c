#include "TYPES.H"

/* Unit bindings for scoring (declare as absolute_symbols of a unit on
 * resource_3c0:02000b24):
 *   Engine_GameFlagClear = 0x0200925c (thumb)
 *   Engine_GameFlagSet = 0x02009254 (thumb)
 *   Main_080770e8 = 0x0200926c (thumb)
 *   Engine_Import0808a250 = 0x0200933c (thumb)
 *   Engine_ActorStop = 0x020092a4 (thumb)
 *   Engine_ActorSetPosition = 0x020092cc (thumb)
 *   Engine_ActorGet = 0x0200928c (thumb)
 */

void Engine_GameFlagClear();
void Engine_GameFlagSet();
void Main_080770e8();
void Engine_Import0808a250();
void Engine_ActorStop();
void Engine_ActorSetPosition();
s32 Engine_ActorGet();


extern u8 Data_000000a4[];
extern u8 Data_000000a5[];
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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* NONMATCHING: 216 of 220 bytes, 30 halfword edits (2026-09-24). The game
 * state base must be shared between the +0x22b store and the later reads; a1
 * and the base swap r5 and r6. */
void Func_02000b24(s32 a0, s32 a1)
{
    u32 i;
    s32 record;
    s32 base2_2000240;

    if (*(s16 *)((*(s32 *)0x03001ebc + 0x182)) == 99) {
        {
            u16 *target = (u16 *)((*(s32 *)0x03001ebc + 0x182));
            s32 shown = 0;
        
            *target = shown;
        }
    }
    Call1(Engine_GameFlagClear, 0x20f);
    if (Data_02000240_t[224][0] == (s32)Data_000000a4) {
        Engine_GameFlagSet((a1 + 0x2f9));
    } else {
        if (Data_02000240_t[224][0] == (s32)Data_000000a5) {
            Engine_GameFlagSet((a1 + 0x309));
        }
    }
    Call2(Main_080770e8, 0x210, 0);
    Call2((void (*)())Engine_Import0808a250, 98, 5);
    base2_2000240 = (s32)Data_02000240_t;
    *(u8 *)((base2_2000240 + 0x22b)) = 3;
    if (*(s16 *)(base2_2000240 + 0x1c0) == (s32)Data_000000a5) {
        if (a1 == 11) {
            Engine_Import0808a250(98, 7);
        } else {
            if (a1 == 12) {
                Engine_Import0808a250(98, 6);
                Engine_ActorStop(12);
                Engine_ActorSetPosition(12, 0, 0);
            }
        }
    }
    *(u8 *)(Engine_ActorGet(*(s32 *)(base2_2000240 + 0x1f4)) + 85) = 3;
}
