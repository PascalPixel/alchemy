#include "types.h"

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
 * Resolver-confirmed main-image symbols name every external call below.
 * Old-style declarations keep the source honest where a shared engine helper
 * is reached with more than one argument shape.
 */

void Func_02000674();
void Func_020017e8();
void Func_02002014();
s32 Func_08000140();
void Func_08000150();
void Func_080001c8();
void Func_08009190();
void Func_080091e0();
void Func_08015250();
u16 *Func_08077008();
s32 Func_080770c0();
void Func_080770c8();
void Func_080770d0();
void Func_08077128();
void Func_08077150();
s32 Func_08077158();
void Func_08077318();
u8 *Func_0808a080();
void Func_0808a0f0();
void Func_0808a100();
void Func_0808a1b8();

s32 Func_020040b4(void)
{
    s32 permuted_13;
    s16 *table = (s16 *)0x02000240;
    s16 listA[8];
    s16 listB[8];
    s16 step;
    u16 raw;
    s32 count;
    s32 index;
    u16 *entity;

    Func_08009190(1);
    Func_08009190(2);
    Func_08009190(4);

    step  = permuted_13;
    permuted_13 = table[0xe1];
    raw = (u16)table[0xe1];
    if (step == 90) {
        Func_080770c8(0x962);
        raw = (u16)table[0xe1];
    }
    /* `lsls #16` on the zero-extended halfword before the compare, so this is
     * an exact halfword test against 91 rather than a signed one. */
    if (raw == 91) {
        Func_080770c8(0x962);
        Func_080770c8(0x950);
    }

    if (table[0xe0] == 0x8b)
        return 0;

    if (table[0xe1] == 11)
        Func_080770d0(0x12f);

    if (Func_080770c0(0x950) != 0) {
        s32 slot = Func_080770c0(0xf31);

        if (slot != 0) {
            Func_0808a0f0(16, 0, 0);
        } else {
            u8 *record = Func_0808a080(16);
            u8 *sub = *(u8 **)(record + 0x50);
            s32 handle;

            record[0x55] = (u8)slot;              /* slot is 0 on this path */
            sub[5] = (u8)(sub[5] & ~0x20);
            *(u32 *)(record + 0x0c) = 0x40000;
            record[0x5c] = 1;
            sub[0x27] = (u8)slot;
            sub[9] = (u8)(sub[9] & 0x0f);

            handle = Func_08000140(17, 0x608);
            handle += 0x400;
            Func_08015250(205);                   /* result discarded */
            Func_080001c8(sub[28], 128, handle);
            Func_08000150(17);
        }

        if (table[0xe1] == 33 && Func_080770c0(0x96f) == 0) {
            Func_080770c8(0x96f);
            Func_0808a0f0(14, 0xd00000, 0x2c00000);
            Func_02002014();
        }
        Func_0808a100(14, 5);
        /* Hand the selected record directly to the engine transition. */
        Func_080091e0(Func_0808a080(14), 0);
    } else if (Func_080770c0(0x962) != 0 && Func_080770c0(0x966) == 0) {
        Func_0808a0f0(10, 0x780000, 0x480000);
    }

    /*
     * A second copy of the same workspace, reached through the IWRAM pointer
     * at 0x03001ebc.  The store is a word at byte offset 0x1c0, so it writes
     * element 0xe0 = 0x209 and element 0xe1 = 0 together.
     */
    *(u32 *)(*(u8 **)0x03001ebc + 0x1c0) = 0x209;

    {
        u8 *flags = Func_0808a080(9);

        flags[0x59] = (u8)(flags[0x59] | 4);
    }

    if (table[0xe1] == 99) {
        count = Func_08077158(listA);
        if (count > 0) {
            for (index = 0; index < count; index++) {
                entity = Func_08077008(listA[index]);
                entity[28] = entity[26];
                entity[29] = entity[27];
                Func_08077128(listA[index]);
            }
        }
        Func_08077150(1);
        Func_08077150(2);
        table[0xe1] = 8;
        Func_08077150(3);
        Func_08077318();
        Func_020017e8();
    }

    if (table[0xe1] == 98) {
        count = Func_08077158(listB);
        if (count > 0) {
            for (index = 0; index < count; index++) {
                entity = Func_08077008(listB[index]);
                entity[28] = entity[26];
                entity[29] = entity[27];
                Func_08077128(listB[index]);
            }
        }
        Func_08077150(1);
        Func_08077150(2);
        Func_08077150(3);
        Func_08077318();
        Func_080770c8(0x966);
        Func_080770c8(0x967);
        Func_0808a0f0(10, 0x380000, 0x780000);
        Func_0808a1b8(10, 0xf000, 0);
        Func_02000674();
        table[0xe1] = 8;
    }

    return 0;
}
