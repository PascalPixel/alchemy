typedef signed int s32;

/*
 * Resource 3bf scene step at 0x02000c78.
 *
 * Programs actor slot 12 with two fixed 16.16 quantities (0x00F80000 and
 * 0x01780000, built by the usual `movs`/`lsls` immediate pair) and then
 * tail-calls the follow-up routine, returning its result unchanged
 * (`pop {r0}; bx r0` with r0 left as the callee produced it).
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
 * self-consistent; nothing here depends on which of the two bases is real.
 *
 * Uncertainty: Func_020017d6 is reached with r0/r1/r2 still holding the
 * previous call's outputs, so it is declared without a prototype rather than
 * given invented arguments.
 */
void Func_02006326(s32 slot, s32 a, s32 b);
s32 Func_020017d6();

s32 Func_02000c78(void)
{
    Func_02006326(12, 0x00F80000, 0x01780000);
    return Func_020017d6();
}
