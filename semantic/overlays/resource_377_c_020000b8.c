typedef signed short s16;
typedef signed int s32;

/*
 * resource_377 owner at 0x020000b8, 96 bytes.  Complete owner: 'push {r5, lr}'
 * prologue and the interworking return 'pop {r5} ; pop {r1} ; bx r1'.  The popped
 * return address goes to r1, not r0, so r0 survives and IS the result -- this
 * owner returns a value (a script/table address).
 *
 * A three-way selector: one state test on the shared s16 table, then two
 * progress-flag queries, each choosing one of four in-image scripts.  Every
 * result but the first is also announced with Func_0808a038 before being
 * returned; the early state-19 path returns without announcing.
 *
 * Literal pool: the seven words at owner offsets 0x44..0x5f, preceded by an
 * alignment 'movs r0,r0' at 0x020000fa.  The control-flow walk from the prologue
 * never reaches them (the epilogue is at 0x020000f4), so the whole tail is pool:
 *   0x02000240  the s16 state table, spelled as the byte-exact siblings
 *               assets/code/resource_371_c_020004a0.c do
 *   0x0200a518, 0x0200a410, 0x0200a338, 0x0200a218  even in-image addresses;
 *               under this family's proven 0x02008000 link base they are file
 *               offsets 0x2518/0x2410/0x2338/0x2218, i.e. data, not callbacks
 *               (the parity rule).  The byte-exact sibling
 *               assets/code/resource_377_c_02000094.c returns 0x0200A1DC /
 *               0x0200A198 from the same shape, so the raw pool value is the
 *               spelling to keep.
 *   0x0000087a, 0x00000815  progress-flag ids
 *
 * All three calls resolved with
 * 'bun tools/overlay_call_targets.ts resource_377 00b8 --json' (3 sites,
 * 2 distinct veneer targets; Func_080770c0 twice):
 *   0x00d0 -> 0x18ec -> Func_080770c0(0x87a)   result tested
 *   0x00de -> 0x18ec -> Func_080770c0(0x815)   result tested
 *   0x00ee -> 0x191c -> Func_0808a038(script)
 * Func_080770c0 is the same flag query resource_377's 0x020002e0 uses with flag
 * 0x834.  The disassembler's own 'bl' annotations are wrong, as on every overlay.
 */

extern s16 Data_02000240[];

s32 Func_080770c0();
void Func_0808a038();

s32 Func_020000b8(void)
{
    s32 script;

    if (Data_02000240[225] == 19) {
        return 0x0200A518;
    }

    if (Func_080770c0(0x87A) != 0) {
        script = 0x0200A410;
    } else if (Func_080770c0(0x815) != 0) {
        script = 0x0200A338;
    } else {
        script = 0x0200A218;
    }

    Func_0808a038(script);
    return script;
}
