/* Unnamed scene-runtime variant; owner identities remain in source-paths.json. */

#include "types.h"

/*
 * resource_36f owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x20085f8.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20085f8 is image offset
 * 0x5f8 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_02000030(void)
{
    return (u8 *)0x020085f8;
}

#include "types.h"

s32 Func_02000038(void)
{
    return 0;
}

#include "types.h"

/*
 * resource_36f owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2008628.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008628 is image offset
 * 0x628 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_0200003c(void)
{
    return (u8 *)0x02008628;
}

#include "types.h"

/*
 * resource_36f owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x200862c.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200862c is image offset
 * 0x62c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_02000044(void)
{
    return (u8 *)0x0200862c;
}

#include "types.h"

/*
 * resource_36f owner at 0x0200004c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000050 holding 0x2008644.
 *
 * LEAF RESIDUE. Published at image offset 0x24; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200004c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008644 is image offset
 * 0x644 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */


u8 *Func_0200004c(void)
{
    return (u8 *)0x02008644;
}

#include "types.h"

/* Signed halfwords in the shared work area; index 225 is the same slot src/0808b1d8.c reads. */
extern s16 Data_02000240[];

extern s32 Data_03001c94;
extern s32 Data_03001ae8;

extern u8 Value_00000000;
extern u8 Value_00000001;
extern u8 Value_00000004;
extern u8 Value_0000000b;

u8 *Func_02000620(void *);
void Func_0200036a(s32);
void Func_020005c0(s32);
void Func_020005d2(s32);
void Func_02000656(s32);
void Func_02000662(void);
void Func_02000668(s32, s32);
void Func_0200066c(s32);
void Func_0200066e(s32);
void Func_0200067c(s32);
void Func_02000680(s32);
void Func_02000692(s32);
void Func_0200069e(s32, s32);
s32 Func_020006a4(void);
void Func_020006ac(s32);
void Func_020006b4(s32);
void Func_020006b8(s32);
void Func_020006c0(void);
void Func_020006ce(s32);
void Func_020006e2(s32);
void Func_020006ec(s32);
void Func_020006f0(s32);
s32 Func_020006f4(s32);
void Func_02000706(s32);
void Func_0200070a(void);
void Func_0200070c(s32);
void Func_02000710(s32, s32);
void Func_02000716(s32);
void Func_02000718(void);
void Func_0200072a(s32, s32);
void Func_02000736(s32);
void Func_0200073e(s32);
void Func_02000750(s32);
void Func_02000768(s32);
void Func_0200076e(s32);
void Func_02000784(s32);

s32 Func_02000054(void)
{
    s32 wait;
    s16 mode = Data_02000240[225];

    if (mode == 10) {
        u8 *object = Func_02000620(*(void **)&Data_02000240[250]);

        object[85] = 0;
        Func_0200066c(75);
        Func_0200036a(0);
        Func_020005c0(120);
        /* The wait is a guarded do-while, not a plain while. A plain while
           leaves the test at the top, and the bottom of the loop is then an
           unconditional jump back rather than the conditional back-edge the
           reference has. */
        wait = 0;
        if (Data_03001c94 == 0) {
            do {
                Func_020005d2(1);
                if (++wait > 3599) {
                    break;
                }
            } while (Data_03001c94 == 0);
        }
        Func_02000668((s32) (u32) &Value_00000000, 2);
        return 0;
    }
    if (mode == 9) {
        Func_020006ac(67);
        Func_02000692(0);
        Func_020006b8(17);
        Func_02000656(60);
        Func_02000662();
        Func_02000680(240);
        Func_020006ce(19);
        Func_0200069e((s32) (u32) &Value_00000001, 2);
        return 0;
    }
    Func_0200066e((s32) (u32) &Value_0000000b);
    if (Data_02000240[225] == 2) {
        for (;;) {
            Func_020006ec(19);
            Func_020006e2(0);
            Func_020006f0(0);
            if (Func_020006a4() <= 0) {
                goto stop;
            }
            Func_02000706(70);
            if (Func_020006f4(1) != 0) {
                goto stop;
            }
            Func_02000716(17);
            Func_020006b4(30);
            Func_020006c0();
            wait = 0;
            if (Data_03001ae8 == 0) {
                do {
                    Func_0200067c(1);
                    if (++wait > 119) {
                        break;
                    }
                } while (Data_03001ae8 == 0);
            }
        }
stop:
        Func_02000710((s32) (u32) &Value_00000001, 1);
    } else {
        Func_02000750(64);
        Func_0200073e(0);
        Func_0200070a();
        Func_0200072a((s32) (u32) &Value_00000004, 16);
        Func_02000768(17);
    }
    Func_0200076e(17);
    Func_0200070c(30);
    Func_02000718();
    Func_02000736(60);
    Func_02000784(19);
    return 0;
}

