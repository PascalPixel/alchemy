#include "types.h"

extern u8 Value_0000241e;

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
 * sources under games/gs1/assets/code and this file keep the printed-offset spelling for
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
 * The resource run is represented as the address of Value_0000241e rather
 * than as an integer constant.  That preserves its pointer identity and makes
 * the compiler materialize it after the first call, where the reference does.
 * Keeping the 258 call in its small source helper likewise preserves the
 * reference's interleaving of the handle move with the two-insn constant.
 */
void Func_0200731c();
void Func_02007338();
void Func_02007344();
void Func_0200735c();
void Func_0200735e();
void Func_02007376();
void Func_02007388();
void Func_0200738e();

static __inline__ void Call_02007388(s32 handle)
{
    Func_02007388(handle, 258, 60);
}

#define RunActorScriptedSequenceB Func_02001c4c
void RunActorScriptedSequenceB(s32 handle)
{
    u8 *id;

    Func_0200731c(handle, 1);
    id = &Value_0000241e;
    Func_02007344((s32)id);
    Func_0200735c(handle, 0);
    Call_02007388(handle);
    Func_0200735e((s32)(id + 1));
    Func_02007376(handle, 0);
    id += 2;
    Func_02007338(handle, 4);
    Func_02007376((s32)id);
    Func_0200738e(handle, 0);
}
