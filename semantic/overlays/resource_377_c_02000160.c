typedef signed short s16;
typedef signed int s32;

/*
 * resource_377 owner at 0x02000160, 128 bytes.  Complete owner: 'push {lr}'
 * prologue and the interworking return 'pop {r1} ; bx r1'.  The popped return
 * address goes to r1, not r0, so r0 survives and IS the result: this owner
 * returns a script/table address and takes no argument.
 *
 * A pure selector, the sibling of resource_377's 0x020000b8 (same state test on
 * Data_02000240[225] == 19, same Func_080770c0 flag queries, same shape of
 * in-image results) but with no announcement call and one more branch.
 *
 * Literal pool: owner offsets 0x58..0x7f, preceded by an alignment 'movs r0,r0'
 * at 0x020001b6.  Walking the owner from the prologue, every path reaches the
 * epilogue at 0x020001b2, so the whole tail is pool:
 *   0x02000240  the s16 state table, spelled as the byte-exact siblings
 *               assets/code/resource_371_c_020004a0.c do
 *   0x00000834, 0x0000087a, 0x00000815  progress-flag ids (0x950 is built inline
 *               as 'movs r0,#149 ; lsls r0,r0,#4')
 *   0x0200ac5c, 0x0200ab9c, 0x0200a5a8, 0x0200a980, 0x0200a800, 0x0200a6b0
 *               all even, so in-image data under this family's proven 0x02008000
 *               link base (file offsets 0x4c5c, 0x4b9c, 0x25a8, 0x2980, 0x2800,
 *               0x26b0), not callbacks.  The byte-exact sibling
 *               assets/code/resource_377_c_02000094.c returns such words
 *               verbatim, so the raw pool value is the spelling kept here.
 *
 * All four calls resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 0160 --json': 4 sites, one
 * distinct veneer target, Func_080770c0, at 0x0176, 0x0188, 0x0196 and 0x01a4 --
 * exactly the four calls written below.  The disassembler's own 'bl' annotations
 * are wrong, as on every overlay.
 */

extern s16 Data_02000240[];

s32 Func_080770c0();

s32 Func_02000160(void)
{
    if (Data_02000240[225] == 19) {
        if (Func_080770c0(0x950) != 0) {
            return 0x0200AC5C;
        }
        return 0x0200AB9C;
    }

    if (Func_080770c0(0x834) != 0) {
        return 0x0200A5A8;
    }
    if (Func_080770c0(0x87A) != 0) {
        return 0x0200A980;
    }
    if (Func_080770c0(0x815) != 0) {
        return 0x0200A800;
    }
    return 0x0200A6B0;
}
