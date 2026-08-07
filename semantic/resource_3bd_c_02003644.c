#include "types.h"

/*
 * resource_3bd owner at 0x02003644, 1004 bytes: THE OVERLAY'S ENTRY
 * DRIVER -- the target of the header veneer at image offset 4
 * (bun tools/overlay_driver.ts resource_3bd), so the loader enters
 * here and no bl inside the image reaches it, which is why it sat in
 * no structural inventory row.
 *
 * This driver reads BOTH loader words.  It opens by DERIVING the
 * sub-selector from the scene id when the sub-selector is still zero:
 * scene 0x93 -> 10, 0x94 -> 20, 0x95 -> 30, 0x96 -> 40, 0x97 -> 50.
 * (The compiled form re-reads the scene id after each store; the
 * stores go to index 225 and cannot change it, so the four later tests
 * are on the same value as the first.)  It then raises flag 0x201 and
 * runs one block per scene:
 * - 0x92: sub-selector 1 and 2 each set the balance snapshot's first
 *   byte (0 and 5 respectively) when flag 0x109 is clear, and raise
 *   flag 0x201;
 * - 0x93: flag 0x962 places record 8 at the origin; otherwise record
 *   8's +80 record gets its +9 low nibble rewritten (clear bits 0, 2,
 *   3, set bit 2), +38 set to 2 and +30 set to 0x4000;
 * - 0x95: raise flag 512, run Func_020008c0 on 8, 9 and 10, then flag
 *   0x211 either puts record 11 in mode 5 and draws panel
 *   (76, 16, 1, 1, 73, 17) or sets bit 1 of record 11's +35; seat
 *   record 11; flag 0x212 draws panel (30, 20, 1, 1, 32, 20);
 * - 0x97: the long one -- raise flag 512, Func_020008c0 on 8, 9, 10,
 *   give all three callback 0x0200b611 at +108, and at sub-selector 52
 *   fire a DMA3 fixed-source fill of three words from a zeroed stack
 *   word to the address held at 0x0200bf6c, then (flag 0x109 clear)
 *   write 0, 0, 4 into the balance snapshot's first three bytes.  Then
 *   snapshot byte 1 == 99 draws two panels, snapshot byte 1 == 2 runs
 *   Func_02000f6c on Func_03000380(byte2 << 16, 5) + 0x4000, records
 *   11..15 are cleared at +85/+89, given 0x10000 at +24 and +28,
 *   seated and put in modes 1..5, records 11..20 take fixed modes,
 *   and records 16 and 20 get bits 2 and 3 of their +80 record's +9,
 *   +35 set to 2, and are seated.
 * Finally, flag 512 either runs Func_02000b30 or writes 516 and 24
 * into the workspace's +448 and +456 s32s.  Returns 0.
 *
 * Complete owner: `push {r5, r6, r7, lr}` + r8 spill at
 * 0x02003644-0x02003648 with `sub sp, #12` at 0x02003656, through the
 * unspill and `bx r1` at 0x02003a1e-0x02003a28.  NOTE the layout, the
 * resource_3a0 trap again: the nineteen-word literal pool at
 * 0x020039b8-0x02003a03 sits INSIDE the function, with the tail at
 * 0x02003a04-0x02003a28 after it and one further pool word at
 * 0x02003a2c; the next function begins at 0x02003a30.  A span measured
 * as "code then pool" stops 118 bytes early here.
 *
 * Cross-checked per the 2026-08-01 ruling, hand-drafted from the bytes
 * first: tools/m2c_guard.ts clears this row of the call-through-
 * register hazard, finds no jump table, and reports full call coverage
 * against this draft.  Every bl was resolved as an ABSOLUTE image
 * offset (stored displacement + 2), not pc-relative.
 *
 * The DMA3 register block (0x040000d4 = REG_DMA3SAD, +4 = REG_DMA3DAD,
 * +8 = REG_DMA3CNT) is public GBA hardware, spelled out by address per
 * this project's convention.  The control word 0x85000003 is recorded
 * as read: bit 31 enable, 32-bit units, fixed source, count 3.
 *
 * Uncertainty: flag ids, scene ids, record ids and modes are literal;
 * record fields +9 (of the +80 record), +24, +28, +30, +35, +38, +85,
 * +89, +108 are recorded by offset with roles open.  Data_02001000 is
 * the established balance-snapshot scalar whose address this owner
 * treats as the base of a small byte array, matching
 * resource_3b7_c_020009f8.c.  0x0200b611 is an in-image code pointer
 * (image offset 0x3610 | 1) installed as a callback and never called
 * here -- a data-installed pointer of the kind neither a BL sweep nor
 * the unindexed check can see.  0x0200bf6c is an in-image word (image
 * offset 0x3f6c) holding the DMA destination and is not read further.
 */

extern s16 Data_02000240[];
extern u8 Data_02001000[];
extern u8 *Data_03001ebc;

extern s32 Func_080770c0(s32 flagId);
extern void Func_080770c8(s32 flagId);
extern void Func_080770d0(s32 flagId);
extern void Func_080091c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091c8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern void Func_080091e0(void *record, s32 arg1);
extern u8 *Func_0808a080(s32 id);
extern void Func_0808a0f0(s32 id, s32 x, s32 z);
extern void Func_0808a100(s32 id, s32 arg1);
extern void Func_0808a158(s32 id, s32 arg1);
extern s32 Func_03000380(s32 arg0, s32 arg1);

extern void Func_020008c0(s32 id);
extern void Func_02000b30(void);
extern void Func_02000f6c(s32 arg0);

s32 Func_02003644(void)
{
    s32 permuted_4;
    s32 permuted_3;
    volatile u32 *dma3 = (volatile u32 *)0x040000d4;
    u8 *record;
    u8 *inner;
    s32 index;
    s32 stackWord;

    if (Data_02000240[225] == 0) {
        if (Data_02000240[224] == 0x93) {
            Data_02000240[225] = 10;
        }
        if (Data_02000240[224] == 0x94) {
            Data_02000240[225] = 20;
        }
        if (Data_02000240[224] == 0x95) {
            Data_02000240[225] = 30;
        }
        if (Data_02000240[224] == 0x96) {
            Data_02000240[225] = 40;
        }
        if (Data_02000240[224] == 0x97) {
            Data_02000240[225] = 50;
        }
    }

    Func_080770c8(512);
    Func_080770d0(0x201);

    if (Data_02000240[224] == 0x92) {
        if (Data_02000240[225] == 1) {
            if (Func_080770c0(0x109) == 0) {
                Data_02001000[4] = 0;
            }
            Func_080770c8(0x201);
        }
        if (Data_02000240[225] == 2) {
            if (Func_080770c0(0x109) == 0) {
                Data_02001000[4] = 5;
            }
            Func_080770c8(0x201);
        }
    }

    if (Data_02000240[224] == 0x93) {
        if (Func_080770c0(0x962) != 0) {
            Func_0808a0f0(8, 0, 0);
        } else {
            record = Func_0808a080(8);
            permuted_3 = *(u8 **)(record + 80);
            (*(u8 **)(record + 80))[38] = 2;
            inner  = permuted_3;
            permuted_4 = (inner[9] & ~13) | 4;
            *(u16 *)(*(u8 **)(record + 80) + 30) = 0x4000;
            inner[9]  = permuted_4;
        }
    }

    if (Data_02000240[224] == 0x95) {
        Func_080770d0(512);
        Func_020008c0(8);
        Func_020008c0(9);
        Func_020008c0(10);
        if (Func_080770c0(0x211) != 0) {
            Func_0808a100(11, 5);
            Func_080091c0(76, 16, 1, 1, 73, 17);
        } else {
            Func_0808a080(11)[35] |= 2;
        }
        Func_080091e0(Func_0808a080(11), 0);
        if (Func_080770c0(0x212) != 0) {
            Func_080091c0(30, 20, 1, 1, 32, 20);
        }
    }

    if (Data_02000240[224] == 0x97) {
        Func_080770d0(512);
        Func_020008c0(8);
        Func_020008c0(9);
        Func_020008c0(10);
        *(s32 *)(Func_0808a080(8) + 108) = 0x0200b611;
        *(s32 *)(Func_0808a080(9) + 108) = 0x0200b611;
        *(s32 *)(Func_0808a080(10) + 108) = 0x0200b611;

        if (Data_02000240[225] == 52) {
            dma3[0] = (u32)&stackWord;
            stackWord = 0;
            dma3[1] = *(u32 *)0x0200bf6c;
            dma3[2] = 0x85000003;
            if (Func_080770c0(0x109) == 0) {
                Data_02001000[0] = 0;
                Data_02001000[1] = 0;
                Data_02001000[2] = 4;
            }
        }

        if (*(s8 *)&Data_02001000[1] == 99) {
            Func_080091c8(41, 55, 3, 2, 30, 55);
            Func_080091c0(42, 8, 1, 1, 31, 8);
        }
        if (Data_02001000[1] == 2) {
            Func_02000f6c(Func_03000380(*(s8 *)&Data_02001000[2] << 16, 5) + 0x4000);
        }

        for (index = 0; index <= 4; index++) {
            record = Func_0808a080(11 + index);
            record[85] = 0;
            record[89] = 0;
            *(s32 *)(record + 24) = 0x10000;
            *(s32 *)(record + 28) = 0x10000;
            Func_080091e0(Func_0808a080(11 + index), 0);
            Func_0808a100(11 + index, index + 1);
        }

        Func_0808a158(11, 1);
        Func_0808a158(12, 4);
        Func_0808a158(13, 11);
        Func_0808a158(14, 2);
        Func_0808a158(15, 3);
        Func_0808a158(16, 6);
        Func_0808a158(17, 6);
        Func_0808a158(18, 6);
        Func_0808a158(19, 6);
        Func_0808a158(20, 6);

        inner = *(u8 **)(Func_0808a080(16) + 80);
        inner[9] |= 12;
        inner[9] |= 12;
        inner = *(u8 **)(Func_0808a080(20) + 80);
        Func_0808a080(16)[35] = 2;
        Func_0808a080(20)[35] = 2;
        Func_080091e0(Func_0808a080(16), 0);
        Func_080091e0(Func_0808a080(20), 0);
    }

    if (Func_080770c0(512) != 0) {
        Func_02000b30();
    } else {
        *(s32 *)(Data_03001ebc + 448) = 516;
        *(s32 *)(Data_03001ebc + 456) = 24;
    }
    return 0;
}
