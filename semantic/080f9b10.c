#include "types.h"

/*
 * Core-drive row 0x080f9b10, 48 bytes (kind shared_stack_continuation_module
 * -- reached via a bare tail branch into an interior label of the
 * already-registered 0x080f9ac0, per that entry's own comment: "The
 * 0x080f9ae0 subroutine-call handler tail-branches to this entry with its
 * own return address still in lr, so the push {lr} here is what saves it."
 * Same shape here: this owner's tail branch to `.L_080f9ac2` (0x080f9ac2,
 * i.e. Func_080f9ac0's body immediately after ITS OWN `push {lr}`) is
 * behaviourally an ordinary call to Func_080f9ac0(player, track), since
 * this owner has already done its own `push {lr}` and the two share the
 * same `pop {r0}; bx r0` tail.
 *
 * Reuses the established `struct Track_080f9ac0` and `Func_080f9ac0`
 * signature from semantic/main/080f9ac0.c; the two extra byte fields this
 * owner touches (the repeat-count flag at +0, the repeat sub-counter at
 * +3) are not part of that struct's known layout, so they are read through
 * raw byte casts rather than extending a struct whose other fields are
 * still unknown.
 *
 * Sound-sequence repeat handling: if the repeat-count byte at the stream
 * cursor is 0, skip past it and parse the following 4-byte operand
 * normally (delegates to Func_080f9ac0). Otherwise increment the track's
 * own repeat sub-counter and pull the next stream byte through
 * Func_080f9ab4 (established in semantic/main/080f9a80.c, which advances
 * the stream cursor itself); while the sub-counter is still below that
 * byte, keep delegating to Func_080f9ac0 to re-parse the SAME 4-byte
 * operand (the cursor is not advanced during the repeat loop). Once the
 * sub-counter catches up, reset it to 0 and advance the stream past the
 * whole 5-byte repeat block (1 count byte + 4 data bytes) using the
 * cursor value read at entry -- transcribed exactly as the reference
 * does it, including that this final advance is based on the ORIGINAL
 * cursor snapshot, not on wherever Func_080f9ab4's own internal advance
 * left it.
 *
 * Complete owner: asm/080f9b10.s in full, `push {lr}` through `pop {r0};
 * bx r0`, plus the tail branches; no literal pool.
 */

struct Track_080f9ac0 {
    u8 unknown_00[64];
    const u8 *command;
};

void Func_080f9ac0(void *player, struct Track_080f9ac0 *track);
s32 Func_080f9ab4(void *object);

void Func_080f9b10(void *player, struct Track_080f9ac0 *track)
{
    const u8 *cursor = track->command;
    u8 repeatFlag = cursor[0];
    u8 *raw = (u8 *)track;

    if (repeatFlag == 0) {
        track->command = cursor + 1;
        Func_080f9ac0(player, track);
        return;
    }

    {
        u8 subCounter = raw[3] + 1;
        u8 streamByte;
        raw[3] = subCounter;
        streamByte = (u8)Func_080f9ab4(track);
        if (subCounter < streamByte) {
            Func_080f9ac0(player, track);
            return;
        }
    }

    raw[3] = 0;
    track->command = cursor + 5;
}
