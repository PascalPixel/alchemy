typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

/*
 * Resource 3b8 scene-step dispatcher at 0x020040b4.
 *
 * Complete owner: `push {r5, r6, r7, lr} / sub sp, #32` at 0x020040b4 and the
 * matching `add sp, #32 / pop {r5, r6, r7} / pop {r1} / bx r1` interworking
 * return at 0x02004310, returning 0 in r0.  The literal pool at
 * 0x02004318-0x0200433f follows the return and is data.
 *
 * `Data_02000240` is the shared scene workspace already proven by the
 * byte-exact sources in this overlay (`resource_3b8_c_02000030.c`,
 * `..._02000070.c`, `..._02004034.c`): a halfword table whose element 0xe0 is
 * the area identity compared against 0x8b, and whose element 0xe1 is the step
 * this routine reads and rewrites.
 *
 * Every `bl` here reaches the overlay's import band (the 0x02008xxx addresses
 * the branches compute after the tool's nominal 0x02000000 base).  As in the
 * byte-exact sources for this overlay, each import is named by the address its
 * call site computes and its interface is left open; the numeric name carries
 * no further meaning.  Several imports are reached with different argument
 * counts from this one owner (0x02008594 with three and with one), which is
 * why the declarations below are old-style.
 */

void Func_0200842c();
void Func_02008432();
void Func_02008438();
void Func_02008488();
void Func_0200849a();
void Func_020084a2();
void Func_020084ca();
void Func_02008516();
void Func_020084ee();
void Func_020084ec();
void Func_02008564();
void Func_02008622();
void Func_020061e2();
void Func_02008636();
void Func_0200855a();
void Func_0200865e();
void Func_02008612();
void Func_02008626();
void Func_0200862c();
void Func_02008632();
void Func_02008646();
void Func_02005a62();
void Func_02008672();
void Func_02008686();
void Func_0200868c();
void Func_02008692();
void Func_020086a6();
void Func_02008684();
void Func_0200868a();
void Func_02008748();
void Func_020087ac();
void Func_02004974();

/* Imports whose results are consumed. */
s32 Func_020084c2();
s32 Func_020084cc();
u8 *Func_02008544();
s32 Func_020084c6();
s32 Func_02008552();
s32 Func_020085dc();
s32 Func_0200858a();
s32 Func_02008594();
s32 Func_02008602();
u16 *Func_020085e2();
s32 Func_02008662();
u16 *Func_02008642();
u8 *Func_0200861a();

/*
 * The two identical sweeps at 0x02004240 and 0x020042a0 copy each listed
 * entity's halfword pair at +52/+54 to +56/+58 and then hand the entity to a
 * per-sweep import.  They differ only in the three imports they use, so they
 * are written once here.
 */
static void latch_and_release(const s16 *list, s32 count,
                              u16 *(*lookup)(), void (*release)())
{
    s32 index = 0;

    do {
        u16 *entity = lookup(list[index]);

        entity[28] = entity[26];
        entity[29] = entity[27];
        release(list[index]);
        index++;
        count--;
    } while (count != 0);
}

s32 Func_020040b4(void)
{
    s16 *table = (s16 *)0x02000240;
    s16 listA[8];
    s16 listB[8];
    s16 step;
    u16 raw;
    s32 count;

    Func_0200842c(1);
    Func_02008432(2);
    Func_02008438(4);

    step = table[0xe1];
    raw = (u16)table[0xe1];
    if (step == 90) {
        Func_02008488(0x962);
        raw = (u16)table[0xe1];
    }
    /* `lsls #16` on the zero-extended halfword before the compare, so this is
     * an exact halfword test against 91 rather than a signed one. */
    if (raw == 91) {
        Func_0200849a(0x962);
        Func_020084a2(0x950);
    }

    if (table[0xe0] == 0x8b)
        return 0;

    if (table[0xe1] == 11)
        Func_020084ca(0x12f);

    if (Func_020084c2(0x950) != 0) {
        s32 slot = Func_020084cc(0xf31);

        if (slot != 0) {
            Func_02008594(16, 0, 0);
        } else {
            u8 *record = Func_02008544(16);
            u8 *sub = *(u8 **)(record + 0x50);
            s32 handle;

            record[0x5c] = 1;
            record[0x55] = (u8)slot;              /* slot is 0 on this path */
            *(u32 *)(record + 0x0c) = 0x40000;
            sub[0x27] = (u8)slot;
            sub[5] = (u8)(sub[5] & ~0x20);
            sub[9] = (u8)(sub[9] & 0x0f);

            handle = Func_020084c6(17, 0x608);
            Func_02008516(205);                   /* result discarded */
            handle += 0x400;
            Func_020084ee(sub[28], 128, handle);
            Func_020084ec(17);
        }

        if (table[0xe1] == 33 && Func_02008552(0x96f) == 0) {
            Func_02008564(0x96f);
            Func_02008622(14, 0xd00000, 0x2c00000);
            Func_020061e2();
        }
        Func_02008636(14, 5);
        /* r0 is not reloaded between these two branches; whether 0x020085dc
         * really returns a value or leaves 14 in r0 is unverified. */
        Func_0200855a(Func_020085dc(14), 0);
    } else if (Func_0200858a(0x962) != 0 && Func_02008594(0x966) == 0) {
        Func_0200865e(10, 0x780000, 0x480000);
    }

    /*
     * A second copy of the same workspace, reached through the IWRAM pointer
     * at 0x03001ebc.  The store is a word at byte offset 0x1c0, so it writes
     * element 0xe0 = 0x209 and element 0xe1 = 0 together.
     */
    *(u32 *)(*(u8 **)0x03001ebc + 0x1c0) = 0x209;

    {
        u8 *flags = Func_0200861a(9);

        flags[0x59] = (u8)(flags[0x59] | 4);
    }

    if (table[0xe1] == 99) {
        count = Func_02008602(listA);
        if (count > 0)
            latch_and_release(listA, count, Func_020085e2, Func_02008612);
        Func_02008626(1);
        Func_0200862c(2);
        Func_02008632(3);
        Func_02008646();
        Func_02005a62();
        table[0xe1] = 8;
    }

    if (table[0xe1] == 98) {
        count = Func_02008662(listB);
        if (count > 0)
            latch_and_release(listB, count, Func_02008642, Func_02008672);
        Func_02008686(1);
        Func_0200868c(2);
        Func_02008692(3);
        Func_020086a6();
        Func_02008684(0x966);
        Func_0200868a(0x967);
        Func_02008748(10, 0x380000, 0x780000);
        Func_020087ac(10, 0xf000, 0);
        Func_02004974();
        table[0xe1] = 8;
    }

    return 0;
}
