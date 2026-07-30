typedef unsigned char u8;
typedef signed int s32;

/*
 * resource_3c5 owner at 0x020026fc, 58 bytes: "if the object directly ahead of
 * the player is on roughly the same level, run the push interaction".
 *
 * Disassembly:
 *
 *   push {r5, lr}
 *   r0 = 0; bl <veneer> -> Func_0808a080(0)      the player record
 *   r5 = r0
 *   bl <in-image 0x02002660>(r5)                 occupant one square ahead
 *   cmp r0, #0; beq  -> return
 *   r3 = [r0, #12]        occupant->y
 *   r0 = [r5, #12]        player->y
 *   r2 = r3 - r0
 *   cmp r2, #0; blt  .neg
 *   r3 = 128 << 12 = 0x80000
 *   cmp r2, r3; bge  -> return
 *   b   .call
 * .neg:
 *   r2 = 128
 *   r3 = r0 - r3          player->y - occupant->y
 *   r2 = r2 << 12 = 0x80000
 *   cmp r3, r2; bge  -> return
 * .call:
 *   bl <in-image 0x020000c4>()
 *   pop {r5}; pop {r0}; bx r0
 *
 * Signature: the epilogue is `pop {r0} ; bx r0`, so r0 holds the popped return
 * address and nothing is returned -- the owner is void, notwithstanding the
 * trailing call.  It also takes no arguments (r0 is written before any read).
 *
 * All three calls were resolved with `bun tools/overlay_call_targets.ts
 * resource_3c5 26fc`, which reports sites=3 / distinct=3:
 *   0x02002700 -> veneer at 0x2e50, publishing Func_0808a080
 *   0x02002706 -> in-image prologue 0x02002660 (this overlay's "occupant of the
 *                 square ahead" probe, converted alongside this owner)
 *   0x0200272c -> in-image prologue 0x020000c4 (the push interaction)
 * The listing's printed targets (0x02005552 / 0x02004d68 / 0x020027f2) are the
 * usual overlay `bl` decoding artefact and are not used.
 *
 * Both comparisons are signed (`blt` / `bge` after `cmp`), and the negative arm
 * recomputes the difference the other way round rather than negating, so the
 * whole test is the absolute height difference against one square, 0x80000 in
 * 16.16 units -- the same 0x00100000/2 order of magnitude the 0x020000c4 owner
 * uses when it probes one square down.  Note the boundary is exclusive: a
 * difference of exactly 0x80000 in either direction returns without acting.
 *
 * Func_0808a080(0) is the same "record by slot index" import the 0x020000c4
 * source uses to fetch the player, with slot 0 being the player.
 */

struct LevelCheckRecord {
    u8 unknown_00[12];
    s32 y;                          /* +0x0c */
};

/* Old-style declarations: overlay imports vary their arity between sites. */
struct LevelCheckRecord *Func_0808a080();   /* record by slot index */
struct LevelCheckRecord *Func_02002660();   /* occupant one square ahead, or 0 */
void Func_020000c4();                       /* push the object ahead one square */

void Func_020026fc(void)
{
    struct LevelCheckRecord *player = Func_0808a080(0);
    struct LevelCheckRecord *ahead = Func_02002660(player);
    s32 difference;

    if (ahead == 0) {
        return;
    }

    difference = ahead->y - player->y;
    if (difference >= 0) {
        if (difference >= 0x80000) {
            return;
        }
    } else if (player->y - ahead->y >= 0x80000) {
        return;
    }

    Func_020000c4();
}
