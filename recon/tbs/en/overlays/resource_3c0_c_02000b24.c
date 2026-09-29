/* Astra 2026-09-27 phased actor-id trial: reuse a1 for state->words[125]
 * after the 11/12 dispatch, then pass a1 to ActorGet. Whole result is
 * 220/220 bytes, 18 differing halfwords / 12 aligned edits. The original
 * eight r5/r6 substitutions remain; final load now targets r5 and adds a
 * move to r0, consuming the original alignment halfword. All eight pool
 * words remain exact. Full normalized diff rejects the predicted lifetime
 * correction; restore the direct final lookup, with no new credit. */
#include "FIELD_EVENT.H"

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

void Main_080770e8(s32 counter, s32 value);
void Engine_Import0808a250(s32 actor, s32 mode);


extern u8 Data_000000a4[];
extern u8 Data_000000a5[];
extern u8 gGameState[];
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

/* 2026-09-27 babi-land final audit: complete 220-byte extent, existing
 * alignment and eight pool words rescored. Only eight a1/state r5/r6
 * substitutions remain. The EventWork trigger, state-row and motion_flags
 * accesses agree with FIELD_EVENT.H; no omitted consumer argument or
 * supported lifetime change was found. Preserve the byte-publication
 * boundary and stopped axes below. DONE +0, new alignment +0.
 * NONMATCHING: 220 of 220 bytes, 8 differing halfwords / 8 aligned edits
 * (2026-09-27). Whole owner 02000b24..02000c00: return at 02000bdc,
 * alignment at 02000bde, eight pool words at 02000be0..02000bfc.
 * H1: a one-pass byte-publication block admits the separate state copy
 * after the store. It also restores the (98, 5) argument order and the
 * exact pool placement; the complete normalized diff now contains only
 * the a1/state r5/r6 swap. Ordinary and diagnostic assembly are identical.
 * CSE no longer retains generated base79 across the tail: its lifetime
 * is now 3 uses/10 instructions in one block, copied to user state36
 * (3 uses/44 instructions/4 calls). Parameter33 still has 5 uses/42
 * instructions/5 calls. Unlike the matched scale scene, these priorities
 * are not tied: declaration reordering is not the indicated next action.
 * Freeze the admitted byte-publication/copy boundary for further work.
 * Earlier 216-byte baseline:
 * Three bounded shared-state trials: direct union rows produce 220 bytes,
 * 43 differing halfwords / 22 edits with the exact pool, but lose saved r6
 * and reload the state for the final word read. A post-store pointer and a
 * one-member pointer aggregate both reproduce the original 216-byte shape.
 * The typed pointer preserved the frame and shared state lifetime, but
 * lacked the local-to-saved copy and reversed the (98, 5) argument setup.
 * Allocator: a1 pseudo 33 crosses five calls; base pseudo 79 crosses four.
 * No further pointer spelling sweep without evidence changing that lifetime.
 * 2026-09-27: FIELD_EVENT.H actor/flag interfaces and the typed motion_flags
 * field emit identical bytes. Transferring the direct (98, 5) call from exact
 * StoryScene_CompleteActor98 also emits identical bytes. Both axes closed;
 * the saved-state lifetime remains the residual, not an omitted argument.
 * Transfer check: IMIRU_MURA/ENTRY_STATE.C's exact Call1 flag boundary
 * was applied to both GameFlagSet calls, with a1 + 0x2f9 and a1 + 0x309.
 * The complete 220-byte candidate is binary-identical: all eight r5/r6
 * differences remain. Unlike Imil, these argument expressions do not
 * shorten the actor-id lifetime. Stop this helper transfer; keep the body.
 * Astra 2026-09-27: express the final actor-id dispatch as a switch to
 * test whether its shared selector changes the surviving input's lifetime.
 * Complete result 220/220 bytes, 40 halfwords / 21 edits; five uses and
 * the r5/r6 swap remain, with an extra branch and reversed guards.
 * Reject the switch and retain the original nested dispatch. */
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
    /* FAKEMATCH: publish the byte before retaining the scene pointer. */
    do {
        Data_02000240_t.bytes[277][1] = 3;
    } while (0);
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
    Engine_ActorGet(state->words[125])->motion_flags = 3;
}
