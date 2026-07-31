typedef unsigned char u8;
typedef unsigned int u32;
typedef signed int s32;
#define NULL ((void *)0)

/*
 * Resource 39a overlay object spawn at 0x02001c18.
 *
 * Complete owner: the two-stage `push {r5, r6, r7, lr}` / high-register
 * `push` prologue at 0x02001c18 with `sub sp, #12`, and the matching
 * `add sp, #12 / pop {r3, r5, r6, r7} / ... / pop {r0} / bx r0` at
 * 0x02001d5a; every early exit branches to that same epilogue, so nothing is
 * returned.  Three pool words follow the return.  All seven distinct branch
 * targets in the row are placed.
 *
 * Frame map: 44 bytes sit below the caller's stack (eight saved words plus
 * `sub sp, #12`), so `[sp,#44]` .. `[sp,#56]` are this owner's fifth
 * through eighth arguments and `[sp,#0]` .. `[sp,#8]` are the local
 * three-word table copied from 0x0200a418.
 *
 * 0x02009bdd is 0x02008000 + 0x1bdc plus the Thumb bit, i.e. the in-image
 * routine 0x02001bdc installed as this object's callback.  Together with the
 * export veneer for 0x02001750 that is a second witness for the 0x02008000
 * link base, so pool words in 0x0200_8xxx..0200_bxxx are in-image while the
 * `bl` band is not.
 *
 * Call convention used throughout this overlay: every `bl` computes an
 * address in the band above the last code row.  The reconstruction's code ends
 * at file offset 0x2258 and the whole image is 0x3328 bytes, yet this overlay's
 * branch targets run from 0x2260 up to 0x5124 - far past the image - so an
 * encoded `bl` address is an import identity, not a place to disassemble.
 * That is the convention the byte-exact sources in this overlay already use
 * (`assets/code/resource_39a_c_02000030.c` declares `Func_02002442`), so
 * imports are named by the address their call site computes and their
 * interfaces are left open.  Declarations are old-style because one name is
 * reached with different argument counts.
 */

/* In-image data: three pointers at 0x02008000 + 0x2418. */
extern void *Data_0200a418[3];
/* The installed callback, named by its in-image address. */
s32 Func_03000380();
void Func_08009080();
void Func_08009098();
s32 Func_080090c8();
void Func_0808a160();
extern void Func_02001bdc();

/* Imports.  0x02003f04, 0x02003f84, 0x02003fa0 and 0x02003fae are used for
 * their return value. */
u8 *Func_02003f04();
void Func_02003f02();
void Func_02003f22();
void Func_02004058();
s32 Func_02003f84();
s32 Func_02003fa0();
s32 Func_02003fae();

void Func_02001c18(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, u32 flags,
                   u8 *source)
{
    void *table[3];
    u8 *object;
    u8 *record;
    u8 *entry;
    s32 scaled;

    /* ldmia/stmia copy the three-word table into the frame before anything
     * else runs. */
    table[0] = Data_0200a418[0];
    table[1] = Data_0200a418[1];
    table[2] = Data_0200a418[2];

    object = Func_080090c8(222, a, b, c);
    if (object == NULL) {
        return;
    }

    record = *(u8 **)(object + 80);
    /* r0 is not reloaded, so the object is this call's first argument. */
    Func_08009080(object, (s32)((flags + 1) & 15));

    /*
     * Uncertainty: the selector is masked to four bits but the frame table
     * only holds three entries, so values above 2 would read past it.  The
     * mask is reproduced as written.
     */
    entry = (u8 *)table[flags & 15];
    Func_08009098(object, entry);

    object[85] = 0;
    record[38] = 0;
    *(void **)(object + 108) = (void *)Func_02001bdc;
    *(s32 *)(object + 68) = d;
    *(s32 *)(object + 72) = e;
    *(s32 *)(object + 76) = f;
    *(s32 *)(object + 48) = 0;
    *(s32 *)(object + 52) = 0;
    /* movs r0,#13 / negs r0,r0 gives the mask ~0x0c, kept in fp for reuse. */
    record[9] = (u8)((record[9] & ~0x0c) | 0x04);

    if ((flags & 0xffff0000) == 0 || source == NULL) {
        return;
    }

    if ((flags & 0x10000) != 0) {
        Func_0808a160(object, *(s32 *)(source + 4));
    }

    if ((flags & 0x20000) != 0) {
        object[35] &= (u8)0xfe;
        record[9] = (u8)((record[9] & ~0x0c) | ((source[0] & 3) << 2));
    }

    if ((flags & 0x80000) != 0) {
        *(s32 *)(object + 24) = *(s32 *)(source + 8);
        *(s32 *)(object + 28) = *(s32 *)(source + 12);
    }

    if ((flags & 0x40000) == 0) {
        return;
    }

    /* The same table entry is reloaded here; its word at +12 is the divisor
     * of both scaling calls. */
    entry = (u8 *)table[flags & 15];
    if ((flags & 0x80000) != 0) {
        *(s32 *)(object + 48) =
            Func_03000380(*(s32 *)(source + 16) - *(s32 *)(object + 24),
                          *(s32 *)(entry + 12));
        scaled = *(s32 *)(source + 20) - *(s32 *)(object + 28);
    } else {
        *(s32 *)(object + 48) =
            Func_03000380(*(s32 *)(source + 16) + (s32)0xffff0000,
                          *(s32 *)(entry + 12));
        scaled = *(s32 *)(source + 20) + (s32)0xffff0000;
    }
    /* Both arms converge on this call. */
    *(s32 *)(object + 52) = Func_03000380(scaled, *(s32 *)(entry + 12));
}
