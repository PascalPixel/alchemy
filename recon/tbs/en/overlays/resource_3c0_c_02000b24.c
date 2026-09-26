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
union GameStateRows {
    u8 bytes[512][2];
    s16 halves[512][1];
    s32 words[256];
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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

/* NONMATCHING: 216 of 220 bytes, 71 differing halfwords / 30 aligned edits
 * (2026-09-26). Whole owner 02000b24..02000c00: return at 02000bdc,
 * alignment at 02000bde, eight pool words at 02000be0..02000bfc.
 * Three bounded shared-state trials: direct union rows produce 220 bytes,
 * 43 differing halfwords / 22 edits with the exact pool, but lose saved r6
 * and reload the state for the final word read. A post-store pointer and a
 * one-member pointer aggregate both reproduce the original 216-byte shape.
 * Retain the typed pointer: it preserves the frame and shared state lifetime.
 * Remaining: a1/base use r5/r6 instead of r6/r5, the store lacks the local r1
 * to saved r5 copy, and the (98, 5) call prepares its arguments in reverse.
 * Allocator: a1 pseudo 33 crosses five calls; base pseudo 79 crosses four.
 * No further pointer spelling sweep without evidence changing that lifetime. */
void Func_02000b24(s32 a0, s32 a1)
{
    u32 i;
    s32 record;
    union GameStateRows *state;

    if (*(s16 *)((*(s32 *)0x03001ebc + 0x182)) == 99) {
        {
            u16 *target = (u16 *)((*(s32 *)0x03001ebc + 0x182));
            s32 shown = 0;
        
            *target = shown;
        }
    }
    Call1(Engine_GameFlagClear, 0x20f);
    if (Data_02000240_t.halves[224][0] == (s32)Data_000000a4) {
        Engine_GameFlagSet((a1 + 0x2f9));
    } else {
        if (Data_02000240_t.halves[224][0] == (s32)Data_000000a5) {
            Engine_GameFlagSet((a1 + 0x309));
        }
    }
    Call2(Main_080770e8, 0x210, 0);
    Call2((void (*)())Engine_Import0808a250, 98, 5);
    Data_02000240_t.bytes[277][1] = 3;
    state = &Data_02000240_t;
    if (state->halves[224][0] == (s32)Data_000000a5) {
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
    *(u8 *)(Engine_ActorGet(state->words[125]) + 85) = 3;
}
