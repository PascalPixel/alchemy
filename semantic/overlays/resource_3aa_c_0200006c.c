typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;

/*
 * resource_3aa owner at 0x0200006c, 56 bytes: code 0x0200006c-0x02000093 and
 * the four-word literal pool at 0x02000094-0x020000a3.  The next inventory row
 * starts at 0x020000a4, so the owner is exactly its advertised span and no
 * frame or register state escapes it.
 *
 * This is the third member of a family whose other two members are already
 * byte-exact: `assets/code/resource_3aa_c_02000030.c` and
 * `assets/code/resource_3aa_c_020000a4.c`.  All three test the same signed
 * halfword `Data_02000240[224]` against 0x67 and return one of two in-image
 * data blocks.  This one differs only in that it also hands the "match" block
 * to one import before returning it.
 *
 * Return type, by the interworking-epilogue rule: `pop {r5} / pop {r1} /
 * bx r1` pops the return address into r1, not r0, so r0 survives and *is* the
 * result.  Both arms leave a data pointer in r0.
 *
 * Link base.  This overlay is linked at 0x02008000 like the other confirmed
 * ones: the pool words 0x02009df4 and 0x02009ddc resolve to file offsets
 * 0x1df4 and 0x1ddc, both inside the 0x2184-byte image, and the byte-exact
 * siblings above already spell such words as `Data_0200xxxx` data symbols.
 * 0x02000240 is below the base and is therefore an ordinary RAM global, again
 * matching the byte-exact siblings.
 *
 * Call accounting: one `bl`, at 0x02000084, resolved with
 * `bun tools/overlay_call_targets.ts resource_3aa 006c` to the import veneer
 * at 0x02001a94, whose trailing word names the main-image import
 * Func_0808a038.  (The disassembler's own annotation, 0x02001b1a, is the usual
 * overlay `bl` mis-decoding and is not the callee.)
 *
 * Uncertainty: only r0 is set before that call, and r0 already held the same
 * pointer, so the import's arity beyond one argument is not observable here.
 * The old-style declaration below records that.
 */

/* Import via the veneer at 0x02001a94. Old-style: its real interface is not
 * established by this owner alone. */
void Func_0808a038();

/* Signed halfword table in RAM; index 224 is the edition/language selector the
 * byte-exact siblings test the same way. */
extern s16 Data_02000240[];

/* In-image data blocks at file offsets 0x1df4 and 0x1ddc. */
extern u8 Data_02009df4[];
extern u8 Data_02009ddc[];

s32 Func_0200006c(void)
{
    if (Data_02000240[224] == 0x67) {
        Func_0808a038(Data_02009df4);
        return (s32)Data_02009df4;
    }
    return (s32)Data_02009ddc;
}
