typedef signed short s16;
typedef unsigned char u8;
typedef signed int s32;

struct SceneRecord {
    u8 padding_000[166];
    u8 field_166;
    u8 padding_167[23];
    u8 field_190;
    u8 padding_191[23];
    u8 field_214;
    u8 padding_215[23];
    u8 field_238;
};

/*
 * resource_38d owner at 0x0200009c, 100 bytes: code 0x0200009c-0x020000eb and
 * the five-word literal pool at 0x020000ec-0x020000ff.  A control-flow walk
 * from the prologue reaches the pool only through `ldr rN, [pc, #imm]`; the
 * last instruction before it is `bx r1`, so nothing falls into it.
 * `assets/code/resource_38d_overlay.s` spells the pool literally as four
 * `.4byte`s plus a split `.2byte` pair, which confirms the boundary.
 *
 * Byte-exact sibling: `assets/code/resource_38d_c_02000100.c` is the same
 * selector with the record-initialising arm removed - same
 * `Data_02000240[224] == 0x21` guard, same "one of two records" return.  Its
 * field offset and pool spelling are reused here rather than rederived.
 *
 * Behaviour: when the published word at Data_02000240 + 448 selects scene
 * 0x21, this returns the record at 0x0200a9b4, first handing it to
 * Func_0808a038 and, if story flag 0x84e is set, stamping four bytes of it.
 * Otherwise it returns the record at 0x0200a99c untouched.
 *
 * Link base: this overlay is linked at 0x02008000, witnessed by the installed
 * handler words 0x020085f5 and 0x02008569 near the end of
 * `resource_38d_overlay.s`, which are Func_020005f4 + 1 and Func_02000568 + 1
 * (both real prologues in this overlay's inventory).  So 0x0200a9b4 and
 * 0x0200a99c are in-image data at file offsets 0x29b4 and 0x299c - the overlay
 * image is writable EWRAM, hence the stores below - while 0x02000240 is below
 * the image and is an ordinary external EWRAM global, exactly as the
 * byte-exact sibling declares it.
 *
 * Call accounting: 2 `bl` sites, both resolved with
 * `bun tools/overlay_call_targets.ts resource_38d 009c --json`; both are
 * import veneers and both appear below exactly once.  No intra-overlay call,
 * no `call_via` slot (neither r3 nor r4 is loaded before any branch).
 *
 * The epilogue is `pop {r5} / pop {r1} / bx r1` with N != 0, so r0 survives
 * and IS the result.  Both arms set r0 before reaching it.
 *
 * UNCERTAINTIES:
 *  - The four stores are at +166, +190, +214 and +238, i.e. one field repeated
 *    at a 24-byte stride over four entries, written 2, 0, 3, 1.  The stride is
 *    plain from the `adds r2, #24`; the entry layout is not established, so
 *    the offsets are left literal.
 *  - The return value is typed as a record pointer because Func_0808a038 is
 *    handed the same value; the byte-exact sibling spells the same two words
 *    as s32.  Nothing here distinguishes the two spellings.
 */

extern s16 Data_02000240[];     /* external EWRAM globals, not overlay data */
extern u8 Value_00000021;
extern u8 Data_0200a9b4[];      /* in-image at file offset 0x29b4 */
extern u8 Data_0200a99c[];      /* in-image at file offset 0x299c */

/* Imports. Old-style declarations: one name can take different argument
 * counts at different sites in this overlay. */
void Func_02002536();           /* record hand-off; raw per-site import label */
s32 Func_0200250c();            /* story-flag test; raw per-site import label */

s32 Func_0200009c(void)
{
    u8 *record;

    /* `ldrsh` through a zero index register, so a signed halfword read. */
    if (Data_02000240[224] == (s32)&Value_00000021) {
        record = Data_0200a9b4;
        Func_02002536(record);

        if (Func_0200250c(0x84e) != 0) {
            struct SceneRecord *scene = (struct SceneRecord *)record;

            scene->field_166 = 2;
            scene->field_190 = 0;
            scene->field_214 = 3;
            scene->field_238 = 1;
        }

        return (s32)record;
    }
    return (s32)Data_0200a99c;
}
