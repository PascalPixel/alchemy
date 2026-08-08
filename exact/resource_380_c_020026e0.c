#include "types.h"

/*
 * resource_380 owner at 0x020026e0, 108 bytes: the talk handler for
 * actor 5, and the twin of 0x02002674.
 *
 * PUBLISHED, NOT CALLED. Its Thumb pointer is written into a script
 * record; no `bl` reaches it. Found by
 * `cargo run --release --manifest-path tools/overlay-published/Cargo.toml -- resource_380`. See
 * resource_380_c_02002674.c for the full seven-callback bank and the
 * way it corroborates the scene wiring at 0x0200227c.
 *
 * DIFFED, NOT ASSUMED. Instruction for instruction this owner is
 * identical to 0x02002674; the only differences are the actor id
 * (5 rather than 9) and three of the five pool words:
 *
 *     pool word   0x02002674      0x020026e0
 *     +0          0x083e          0x083e     (first story flag)
 *     +4          0x10cb          0x10c9     (line, flag-set arm)
 *     +8          0x083c          0x083c     (second story flag)
 *     +12         0x1079          0x107a     (line, second flag clear)
 *     +16         0x107b          0x107c     (line, second flag set)
 *
 * The pairing 0x1079/0x107a and 0x107b/0x107c across the two actors
 * suggests the lines are laid out per-actor within a shared block, but
 * that is an observation about the id numbering, not something this
 * owner establishes.
 *
 * Branch senses were re-read here rather than carried over: `beq` at
 * 0x020026ee, `bne` at 0x02002708 -- same opposite polarities as the
 * twin.
 *
 * Complete owner: `push {lr}` at 0x020026e0 through `pop {r0} / bx r0`
 * at 0x02002734-0x02002736, then the five-word literal pool
 * 0x02002738-0x0200274b; the next owner's prologue is at 0x0200274c.
 *
 * All four `bl` targets resolved through the import-veneer table under
 * the +2 rule (cargo run --release --manifest-path tools/overlay-call-targets/Cargo.toml --).
 *
 * Uncertainty: as in the twin, the zero arguments to Func_0808a150 and
 * Func_0808a180 have no established meaning.
 */

extern void Func_02007080(void);
extern s32 Func_0200704e(s32 flagId);
extern void Func_02007148(s32 dialogueId);
extern void Func_02007160(s32 id, s32 arg1);
extern s32 Func_02007068(s32 flagId);
extern void Func_02007162(s32 dialogueId);
extern void Func_0200716a(s32 dialogueId);
extern void Func_0200715c(s32 id, s32 arg1, s32 arg2);
extern void Func_020070ba(s32 frames);
extern void Func_02007192(s32 id, s32 arg1);
extern void Func_020070d6(void);

void Func_020026e0(void)
{
    Func_02007080();
    if (Func_0200704e(0x83e)) {
        Func_02007148(0x10c9);
        Func_02007160(5, 0);
    } else {
        if (Func_02007068(0x83c) == 0)
            Func_02007162(0x107a);
        else
            Func_0200716a(0x107c);
        Func_0200715c(5, 0, 0);
        Func_020070ba(10);
        Func_02007192(5, 0);
    }
    Func_020070d6();
}
