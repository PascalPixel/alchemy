typedef signed int s32;

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
    Func_0200731c(handle, 1);
    Func_02007344(0x241e);
    Func_0200735c(handle, 0);
    Func_02007388(handle, 258, 60);
    Func_0200735e(0x241f);
    Func_02007376(handle, 0);
    Func_02007338(handle, 4);
    Func_02007376(0x2420);
    Func_0200738e(handle, 0);
}
