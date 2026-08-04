typedef unsigned char u8;
typedef unsigned short u16;
typedef signed short s16;
typedef signed int s32;

/*
 * resource_3a4 owner at 0x020020d0, 576 bytes: the flag-0x8fd scene
 * gate -- the largest of the reconciliation family (siblings
 * 0x2310/0x2490/0x25c0/0x26c0), keyed to the FIRST sink-and-flicker
 * twin (Func_020006dc sets 0x8fd) and owning the selector-99 handoff
 * to the twin's cutscene bracket Func_02002d58.
 *
 * Shape:
 * - selector 99 with flag 0x109 clear: run the local cutscene bracket
 *   Func_02002d58 (which itself runs the 6dc twin), then always the
 *   exact Func_02003a44;
 * - flag 0x8fd clear: park actor 8; set: mark records 8 and 9 done
 *   (`[0x23] = 2`, seat via Func_080091e0(rec, 0));
 * - main gate, flag 0x8fd still clear: Func_0808a158(10, 2), then one
 *   of two near-identical staging blocks picked by flags 0x905 / 0x904
 *   (neither set: nothing).  Both install the local callback constant
 *   0x0200ace1 (image offset 0x2ce0 | 1) into record 9's +0x6c, zero
 *   its +0x55, lift it to y = 32.0 (0x200000) and pop panel (2, 0, 1,
 *   1, 18, 13); they differ in actor 10's placement x (240 << 15 vs
 *   130 << 17, same z 215 << 16) and the tail: the 0x905 block zeroes
 *   actor 10's heading and runs the local mover Func_02002bb0(0x820000,
 *   0, 0xa80000, 0), the 0x904 block instead attaches script blob
 *   0x0200bd34 (image offset 0x3d34) via Func_0808a168(10, 0x10000,
 *   script);
 * - flag 0x8fd set: park actor 10, panel (0, 0, 1, 2, 3, 14) -- the
 *   6dc twin's own closing panel shape -- clear bit 9 of scene-state
 *   +20, then the familiar per-actor marker motif on 8 (flag 0x200,
 *   +12 zeroed) and 9 (flag 0x201, +12 = 0x200000).
 *
 * Complete owner: `push {r5, r6, lr}` at 0x020020d0, `sub sp, #8`,
 * through `add sp, #8 / pop {r5, r6} / pop {r0} / bx r0` at
 * 0x20022de-0x20022e4, trailing pool at 0x20022e6-0x200230f,
 * immediately followed by the drafted owner at 0x02002310.  No
 * arguments, void.
 *
 * All 41 bl sites resolve with the `+2` stored-displacement rule; 37
 * veneers, four overlay-local (0x2d58, 0x3a44 exact, 0x2bb0 drafted,
 * 0x3460 exact x2).  Flags tested: 0x109, 0x8fd (twice), 0x905,
 * 0x904, 0x200, 0x201.
 */

s32 Func_080770c0();           /* test story flag, established */
u8 *Func_0808a080();           /* scene-record accessor, established */
void Func_0808a0f0();          /* place entity (id, x, z), established */
void Func_080091e0();          /* (record, n), established */
void Func_0808a158();          /* (id, n), established */
void Func_0808a100();          /* (id, n) */
void Func_080091c0();          /* six-argument panel call */
void Func_0808a168();          /* (id, rate, script) */
void Func_02002d58(void);      /* local, resource_3a4_c_02002d58.c */
void Func_02003a44();          /* local, exact */
void Func_02002bb0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void Func_02003460(s32 selector);   /* local, exact */

extern u8 Data_0200bd34[];     /* image offset 0x3d34: script blob */

void Func_020020d0(void)
{
    u8 *record;
    u8 *state;

    if (Func_080770c0(0x109) == 0
        && *(s16 *)((u8 *)0x02000240 + 450) == 99) {
        Func_02002d58();
    }
    Func_02003a44();

    if (Func_080770c0(0x8fd) == 0) {
        Func_0808a0f0(8, 0, 0);
    } else {
        record = Func_0808a080(8);
        if (record != 0) {
            record[0x23] = 2;
            Func_080091e0(record, 0);
        }
    }

    record = Func_0808a080(9);
    if (record != 0) {
        record[0x23] = 2;
        Func_080091e0(record, 0);
    }

    if (Func_080770c0(0x8fd) == 0) {
        Func_0808a158(10, 2);

        if (Func_080770c0(0x905) != 0) {
            Func_0808a100(9, 0);
            *(s32 *)(Func_0808a080(9) + 0x6c) = 0x0200ace1;
            Func_0808a080(9)[0x55] = 0;
            *(s32 *)(Func_0808a080(9) + 12) = 0x200000;     /* 32.0 */
            Func_080091c0(2, 0, 1, 1, 18, 13);
            Func_0808a0f0(10, 240 << 15, 215 << 16);
            *(u16 *)(Func_0808a080(10) + 6) = 0;
            Func_0808a100(10, 3);
            Func_02002bb0(0x820000, 0, 0xa80000, 0);
        } else if (Func_080770c0(0x904) != 0) {
            Func_0808a100(9, 0);
            *(s32 *)(Func_0808a080(9) + 0x6c) = 0x0200ace1;
            Func_0808a080(9)[0x55] = 0;
            *(s32 *)(Func_0808a080(9) + 12) = 0x200000;     /* 32.0 */
            Func_080091c0(2, 0, 1, 1, 18, 13);
            Func_0808a0f0(10, 130 << 17, 215 << 16);
            Func_0808a168(10, 0x10000, Data_0200bd34);
        }
    } else {
        state = *(u8 **)0x03001e70;
        Func_0808a0f0(10, 0, 0);
        Func_080091c0(0, 0, 1, 2, 3, 14);
        *(u16 *)(state + 20) &= 0xfdff;

        Func_02003460(8);
        if (Func_080770c0(0x200) != 0) {
            Func_0808a100(8, 5);
            Func_080091c0(7, 13, 1, 1, 9, 13);
            record = Func_0808a080(8);
            *(s32 *)(record + 12) = 0;
            record[0x23] |= 2;
        }

        Func_02003460(9);
        if (Func_080770c0(0x201) != 0) {
            Func_0808a100(9, 5);
            Func_080091c0(29, 1, 3, 1, 17, 13);
            record = Func_0808a080(9);
            *(s32 *)(record + 12) = 0x200000;
            record[0x23] |= 2;
        }
    }
}
