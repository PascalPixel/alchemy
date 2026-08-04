#include "types.h"

/*
 * Resource 3bf actor script at 0x02001c4c.
 *
 * r6 holds the actor handle passed in r0 and is re-supplied to every call that
 * takes it; r5 walks a small run of consecutive resource identifiers starting
 * at 0x241e (`adds r0,r5,#1` and `adds r5,#2` produce 0x241f and 0x2420).
 *
 * The callees are other entry points of this same overlay module, named by the
 * tree's offset convention (see the note on the module's link base below);
 * none of them is reconstructed here.
 *
 * Link-base note.  This overlay's absolute pool constants sit 0x8000 above the
 * offsets the disassembler prints, so the module is linked at 0x02008000: the
 * dispatcher at 0x02004638 loads its jump-table base as 0x0200c64c while the
 * table itself is embedded at offset 0x464c, and every entry in it likewise
 * points 0x8000 past the case body it selects.  Both the existing byte-exact
 * sources under assets/code and this file keep the printed-offset spelling for
 * symbol names and the raw pool value for absolute data addresses, which is
 * self-consistent; nothing here depends on which of the two bases is real.  Note that Func_02007376 is reached twice with
 * different argument shapes — once as (handle, 0) and once as (identifier) —
 * which is why these services keep old-style declarations rather than being
 * given an invented common prototype.
 *
 * The epilogue is `pop {r5,r6}; pop {r0}; bx r0`, leaving r0 as the last
 * call produced it; nothing is deliberately returned, so this is a void
 * routine here.
 *
 * STILL-OPEN residual (14/88 bytes as of this pass, real per-site call
 * symbols already applied above): the reference loads the `id = 0x241e` pool
 * word right after the first call, scheduled ahead of nothing else, while the
 * routed compiler's list scheduler hoists that independent load earlier,
 * ahead of the first `bl`. A second, smaller residual is the
 * `Func_02007388(handle, 258, 60)` call, where the reference sets r0 (handle)
 * between the two halves of the shifted-immediate build for 258 (`movs
 * r1,#129` / `lsls r1,r1,#1`) and the routed compiler emits both `lsls`
 * halves before the r0 setter -- the same shape as the documented
 * resource_3bf:1cf0 route, which is on record as not fixing this sibling.
 * No single compiler mode in the routed sweep closes either residual; an
 * `__asm__` barrier was tried and rejected per project policy (no inline asm
 * or fixed-register tricks), so this stays open rather than papered over.
 */
void Func_0200731c();
void Func_02007338();
void Func_02007344();
void Func_0200735c();
void Func_0200735e();
void Func_02007376();
void Func_02007388();
void Func_0200738e();

void Func_02001c4c(s32 handle)
{
    s32 id;

    Func_0200731c(handle, 1);
    id = 0x241e;
    Func_02007344(id);
    Func_0200735c(handle, 0);
    Func_02007388(handle, 258, 60);
    Func_0200735e(id + 1);
    Func_02007376(handle, 0);
    id += 2;
    Func_02007338(handle, 4);
    Func_02007376(id);
    Func_0200738e(handle, 0);
}
