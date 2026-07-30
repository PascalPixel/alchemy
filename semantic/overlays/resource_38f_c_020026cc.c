typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;

/*
 * resource_38f owner at 0x020026cc, 224 bytes: code 0x020026cc-0x02002771,
 * two bytes of alignment at 0x02002772, and the fourteen-word literal pool at
 * 0x02002774-0x020027ab.  The next inventory row starts at 0x020027ac with its
 * own `push {r5, r6, r7, lr}`, so the owner is exactly its advertised span.
 *
 * Epilogue `pop {r5} / pop {r1} / bx r1`: the popped return address goes to r1,
 * not r0, so r0 survives and IS the result.  r0 is set to 0 immediately before
 * it on the only path there, so the owner always returns 0.
 *
 * Call accounting: 2 `bl` sites, both resolved with
 * `bun tools/overlay_call_targets.ts resource_38f 26cc` - both import veneers,
 * Func_080f9010 at 0x02002758 and Func_08009098 at 0x02002760.  The
 * disassembler's own `bl` annotations are wrong in the usual overlay way.
 *
 * The single parameter arrives in r0 and is only ever dereferenced at +8 and
 * +0x10.  Those are the 16.16 X and Z words of the actor record that
 * Func_0808a080 returns, the layout resource_39f and resource_373 already
 * settled (+8/+12/+16 = X/Y/Z), so this is a "is the actor standing in one of
 * these rectangles" trigger test.
 *
 * All three pool words in the 0x0200_bxxx band are in-image addresses under
 * this overlay's confirmed 0x02008000 link base (see the jump-table witness
 * documented in semantic/overlays/resource_38f_c_02000304.c): 0x0200b69c ->
 * file offset 0x369c, 0x0200b5ec -> 0x35ec, 0x0200b698 -> 0x3698.  The
 * byte-exact assets/code/resource_38f_c_02002608.c already spells siblings of
 * these as `extern` Data_0200bxxx symbols, which is followed here.
 *
 * The eleven remaining pool words are the rectangle bounds.  Each X test is
 * the standard unsigned range idiom `(u32)(x + K) <= L`, i.e.
 * `-K <= x <= -K + L`; the intervals are given in 16.16 units in the comments
 * and are all whole-tile boundaries, which is the confirmation that the
 * reading is right.
 *
 * UNCERTAINTIES:
 *  - Data_0200b69c selects between two entirely different rectangle sets and
 *    Data_0200b698 is set to 1 on a hit; they read as "cutscene variant" and
 *    "trigger already fired", but neither is proven from this owner alone.
 *  - The original loads the Z word only inside the arms whose X test passed.
 *    It is hoisted here because the pointer is unconditionally valid on every
 *    path that reads X; no observable difference.
 *  - Func_08009098's second argument is the in-image block Data_0200b5ec.  Its
 *    shape is not reconstructed, so it stays an opaque byte array.
 */

/* In-image data under the 0x02008000 link base. */
extern s32 Data_0200b69c;
extern s32 Data_0200b698;
extern u8 Data_0200b5ec[];

/* Old-style declarations are mandatory in overlay sources. */
void Func_080f9010();
void Func_08009098();

s32 Func_020026cc(u8 *record)
{
    s32 x;
    s32 z;
    s32 inside;

    x = *(s32 *)(record + 8);
    z = *(s32 *)(record + 16);
    inside = 0;

    if (Data_0200b69c != 0) {
        /* x in [59.0, 141.0), z in (211.0, 256.99998]. */
        if ((u32)(x + 0xffc4ffff) <= 0x0051fffe &&
            z > 0x00d30000 && z <= 0x0100ffff) {
            inside = 1;
        /* x in [69.0, 122.0), z in (194.0, 276.99998]. */
        } else if ((u32)(x + 0xffbaffff) <= 0x0034fffe &&
                   z > 0x00c20000 && z <= 0x0114ffff) {
            inside = 1;
        }
    } else {
        /* x in [59.0, 111.0), z in (194.0, 230.0). */
        if ((u32)(x + 0xffc4ffff) <= 0x0033fffe &&
            z > 0x00c20000 && z < 0x00e60000) {
            inside = 1;
        /* x in [111.0, 141.0), z in (216.0, 250.0). */
        } else if ((u32)(x + 0xff90ffff) <= 0x001dfffe &&
                   z > 0x00d80000 && z < 0x00fa0000) {
            inside = 1;
        /* x in [78.0, 122.0), z in (241.0, 276.99998]. */
        } else if ((u32)(x + 0xffb1ffff) <= 0x002bfffe &&
                   z > 0x00f10000 && z <= 0x0114ffff) {
            inside = 1;
        }
    }

    if (inside) {
        Func_080f9010(106);
        Func_08009098(record, Data_0200b5ec);
        Data_0200b698 = 1;
    }

    return 0;
}
