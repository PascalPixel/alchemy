#include "types.h"

#include "resource_3ac.h"

typedef struct {
    u8 filler0[6];
    u16 unk6;
} T;

extern u8 Data_02000240[];
extern u8 Data_02008c98[];
extern u8 Data_02008a64[];
extern u8 Data_02008824[];
extern u8 Value_00001be0;
extern u8 Value_00000069;

s32 Func_020004fe(s32);
T *Func_02000552(s32);
s32 Func_0200053c(s32);
void Func_020005a0(s32, s32);
void Func_02000566(void);
s32 Func_02000554(s32);
void Func_0200058e(s32);
void Func_020005a6(s32, s32);
void Func_0200059e(s32);
void Func_020005b6();
void Func_0200059a(void);
void Func_020005b0(void);
void Func_020005ce(s32);
void Func_020005e6(s32, s32);
void Func_020005d2(void);
void Func_020005d8(void);
void Func_020005f6(s32);
void Func_02000606(s32, s32);
void Func_020005de(s32);
void Func_020005fa(void);
void Func_02000600(void);
void Func_0200061e(s32);
s32 Func_02000636(s32, s32);
void Func_0200061a(void);
T *Func_02000632(s32);
void Func_0200066c(s32);
s32 Func_02000624(s32);
void Func_02000644(void);
void Func_02000662(s32);
void Func_02000672(s32, s32);
void Func_0200065e(void);
void Func_0200065c(void);
void Func_0200067a(s32);
void Func_0200068a(s32, s32);
s32 Func_02000676();
void Func_02000696(void);
void Func_020006b4(s32);
void Func_020006c4(s32, s32);
void Func_020006b0(void);
void Func_020006ae(void);
void Func_020006cc(s32);
void Func_020006dc(s32, s32);
void Func_020006c8(void);
T *Func_020006ea(s32);
void Func_0200071e(s32, s32);
s32 Func_020006de(s32);
void Func_020006fe(void);
void Func_0200071c(s32);
void Func_0200072c(s32, s32);
void Func_02000718(void);
void Func_02000716(void);
void Func_02000734(s32);
void Func_02000744(s32, s32);
void Func_02000730(void);
s32 Func_02000732(s32);
void Func_0200076c(s32);
void Func_0200077c();
void Func_0200078c(s32, s32);
T *Func_02000796(s32);
s32 Func_02000776(s32);
void Func_020007d4(s32, s32);
void Func_020007aa(void);
void Func_020007c8(s32);
void Func_020007d8(s32, s32);
void Func_020007c4(void);
void Func_020007dc(s32);
void Func_020007ec(s32, s32);
s32 Func_020007d6(s32);
void Func_02000810(s32);
void Func_02000820();
void Func_02000830(s32, s32);
T *Func_0200083a(s32);
s32 Func_0200081a(s32);
void Func_02000878(s32, s32);
void Func_0200084e(void);
void Func_0200086c(s32);
void Func_0200087c(s32, s32);
void Func_02000868(void);
void Func_02000880(s32);
void Func_02000890(s32, s32);
void Func_020008a6(s32);
s32 Func_020008d6(s32);
void Func_020008ac(s32, s32);
s32 Func_020008e2(s32);
void Func_020008b8(s32, s32);
s32 Func_020008ee(s32);
void Func_020008c4(s32, s32);
T *Func_0200091a(s32);
void Func_02000954(s32);
void Func_0200093c(s32);
void Func_0200094c(s32, s32);

/* Contiguous unnamed leaf-owner run for resource_3ac. */

/*
 * resource_3ac owner at 0x02000030, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000034 holding 0x20084f8.
 *
 * LEAF RESIDUE. Published at image offset 0xc; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000030 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x20084f8 is image offset
 * 0x4f8 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3ac owner at 0x0200003c, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000040 holding 0x2008630.
 *
 * LEAF RESIDUE. Published at image offset 0x14; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x0200003c reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x2008630 is image offset
 * 0x630 under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

/*
 * resource_3ac owner at 0x02000044, 8 bytes: `ldr r0, [pc, #0] / bx lr` plus the
 * one-word literal pool at 0x2000048 holding 0x200865c.
 *
 * LEAF RESIDUE. Published at image offset 0x1c; sweep B resolved that
 * word and, before 2026-08-01, discarded it for not opening with a `push`.
 *
 * THE SPAN IS 8 BYTES, NOT 4. The pool word sits past the `bx lr`, and the
 * `pc`-relative load at 0x02000044 reads it, so it belongs to this owner.
 * Recording 4 would orphan a word and manufacture a phantom gap.
 *
 * The pool word is an ADDRESS -- 0x200865c is image offset
 * 0x65c under the base + 0x8000 spelling -- loaded and returned
 * without being dereferenced, so this is a getter for an in-image table.
 *
 * One of the 191 rows sharing this exact body across the tree, and every
 * one of them returns a DIFFERENT address. Identical bytes are not
 * identical semantics; this row's pool word was resolved on its own.
 */

u8 *Func_02000030(void)
{
    return (u8 *)0x020084f8;
}

s32 Func_02000038(void)
{
    return 0;
}

u8 *Func_0200003c(void)
{
    return (u8 *)0x02008630;
}

u8 *Func_02000044(void)
{
    return (u8 *)0x0200865c;
}

u8 *Func_0200004c(void)
{
    s16 *table = ((s16 *)Data_02000240);

    if (table[225] == 10) {
        return Data_02008c98;
    }
    if (Func_020004fe(0x941) != 0) {
        return Data_02008a64;
    }
    return Data_02008824;
}

void Func_0200008c(void)
{
    T *p;

    p = Func_02000552(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        if (Func_0200053c(0x941) != 0) {
            Func_020005a0(8, 17);
            return;
        }
    }
    Func_02000566();
    if (Func_02000554(0x941) != 0) {
        Func_0200058e(0x24fb);
        Func_020005a6(17, 0);
    } else {
        Func_0200059e(0x1bd0);
        Func_020005b6(17, 0);
    }
    Func_0200059a();
}

void Func_020000fc(void)
{
    Func_020005b0();
    Func_020005ce(0x1bd5);
    Func_020005e6(20, 0);
    Func_020005b6(0x940);
    Func_020005d2();
}

void Func_02000124(void)
{
    Func_020005d8();
    Func_020005f6(0x1bdb);
    Func_02000606(20, 0);
    Func_020005de(0x940);
    Func_020005fa();
}

void Func_0200014c(void)
{
    Func_02000600();
    Func_0200061e(0x24fe);
    Func_02000636(18, 0);
    Func_0200061a();
}

void Func_0200016c(void)
{
    T *p;

    p = Func_02000632(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Func_0200066c(21);
    } else if (Func_02000624(0x941) != 0) {
        Func_02000644();
        Func_02000662(0x2507);
        Func_02000672(21, 0);
        Func_0200065e();
    } else {
        Func_0200065c();
        Func_0200067a(0x1bdc);
        Func_0200068a(21, 0);
        Func_02000676();
    }
}

void Func_020001d8(void)
{
    if (Func_02000676(0x941) != 0) {
        Func_02000696();
        Func_020006b4(0x24fa);
        Func_020006c4(16, 0);
        Func_020006b0();
    } else {
        Func_020006ae();
        Func_020006cc((s32)&Value_00001be0);
        Func_020006dc(16, 0);
        Func_020006c8();
    }
}

void Func_02000224(void)
{
    T *p;

    p = Func_020006ea(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Func_0200071e(25, 16);
    } else if (Func_020006de(0x941) != 0) {
        Func_020006fe();
        Func_0200071c(0x24f9);
        Func_0200072c(16, 0);
        Func_02000718();
    } else {
        Func_02000716();
        Func_02000734(0x1bcf);
        Func_02000744(16, 0);
        Func_02000730();
    }
}

void Func_02000294(void)
{
    if (Func_02000732(0x941) != 0) {
        Func_0200076c(0x24f6);
        Func_0200077c(14, 0);
    } else {
        Func_0200077c(0x1bde);
        Func_0200078c(14, 0);
    }
}

void Func_020002d0(void)
{
    T *p;
    s32 v;

    p = Func_02000796(0);
    v = p->unk6;
    if (Func_02000776(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Func_020007d4(29, 14);
        } else {
            Func_020007aa();
            Func_020007c8(0x24f5);
            Func_020007d8(14, 0);
            Func_020007c4();
        }
    } else {
        Func_020007dc(0x1bcd);
        Func_020007ec(14, 0);
    }
}

void Func_02000338(void)
{
    if (Func_020007d6(0x941) != 0) {
        Func_02000810(0x24f8);
        Func_02000820(15, 0);
    } else {
        Func_02000820(0x1bdf);
        Func_02000830(15, 0);
    }
}

void Func_02000374(void)
{
    T *p;
    s32 v;

    p = Func_0200083a(0);
    v = p->unk6;
    if (Func_0200081a(0x941) != 0) {
        if ((u32)(v - 0xa001) <= 0x3ffe) {
            Func_02000878(30, 15);
        } else {
            Func_0200084e();
            Func_0200086c(0x24f7);
            Func_0200087c(15, 0);
            Func_02000868();
        }
    } else {
        Func_02000880(0x1bce);
        Func_02000890(15, 0);
    }
}

s32 Func_020003dc(void)
{
    u8 *state;
    s32 *dst;
    u8 *tbl;
    s32 off;
    s32 v;
    s32 n;

    state = *(u8 **)0x03001ebc;
    dst = (s32 *)(state + 0x1c0);
    off = 0x209;
    *dst = off;
    tbl = Data_02000240;
    v = *(s16 *)(tbl + off - 71);
    if (v == 10) {
        Func_020008a6(0x12f);
        n = (s32)&Value_00000069;
        *(u16 *)(tbl + 0x1c4) = n;
        *(u16 *)(tbl + 0x1c6) = v;
    }
    Func_020008ac(Func_020008d6(23), 0);
    Func_020008b8(Func_020008e2(24), 0);
    Func_020008c4(Func_020008ee(25), 0);
    return 0;
}

void Func_02000454(void)
{
    T *p;

    p = Func_0200091a(0);
    if ((u32)(p->unk6 - 0xa001) <= 0x3ffe) {
        Func_02000954(21);
    } else {
        Func_0200093c(0x266b);
        Func_0200094c(22, 0);
    }
}
