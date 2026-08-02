typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 378 owner at 0x02000360 (392 bytes, through 0x020004e7).
 *
 * The body has 44 imported/direct calls and nine branch sites.  The word at
 * 0x020004e4 is the sole literal-pool entry; 0x020004e8 is the next
 * prologue.  Three tests gate a common finishing sequence: the first two
 * test the result of Func_0808a070(0, 0), and the last path checks the
 * unsigned halfword at offset +6 of the actor returned by
 * Func_0808a080(8).  The successful range is inclusive [0xa000, 0xe000].
 *
 * The two byte writes at +0x5a are observable state changes, not temporary
 * compiler bookkeeping: the first clears bit 0 and the second sets it.
 * The direct call at 0x020004aa resolves to the in-overlay owner at
 * 0x0200187c (the listing's PC-relative spelling is not the loaded target),
 * and is retained as an ordinary call here.
 */

extern u8 *Func_0808a080(s32 id);
extern s32 Func_0808a070(s32 id, s32 arg1);

extern void Func_0200187c(void);
extern void Func_080770c8(s32 flag);
extern void Func_0808a010(s32 frames);
extern void Func_0808a018(void);
extern void Func_0808a020(void);
extern void Func_0808a0c8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0d0(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a0e8(s32 id);
extern void Func_0808a110(s32 id, s32 arg1);
extern void Func_0808a170(s32 value);
extern void Func_0808a178(s32 id, s32 arg1);
extern void Func_0808a1b8(s32 id, s32 arg1, s32 arg2);
extern void Func_0808a248(s32 frames);
extern void Func_0808a330(s32 id, s32 arg1);
extern void Func_0808a348(s32 frames);
extern void Func_0808a090(s32 id, s32 arg1, s32 arg2);
extern void Func_080f9010(s32 value);
extern void Func_080f9078(void);

void Func_02000360(void)
{
    u8 *actor;

    Func_0808a018();
    Func_0808a170(0x1164);
    Func_0808a178(8, 0);

    if (Func_0808a070(0, 0) == 0) {
        Func_0808a110(8, 3);
        Func_0808a010(20);
        goto common;
    }

    Func_0808a010(20);
    Func_0808a178(8, 0);
    if (Func_0808a070(0, 0) != 0) {
        goto common;
    }

    Func_0808a010(20);
    Func_0808a178(8, 0);
    if (Func_0808a070(0, 0) != 0) {
        goto common;
    }

    Func_0808a010(20);
    actor = Func_0808a080(8);
    if (*(u16 *)(actor + 6) < 0xa000) {
        goto fallback;
    }

    actor = Func_0808a080(8);
    if (*(u16 *)(actor + 6) > 0xe000) {
        goto fallback;
    }

    Func_0808a090(8, 0x8000, 0x4000);
    Func_0808a1b8(8, 0, 0);
    Func_0808a010(10);

    actor = Func_0808a080(8);
    *(actor + 0x5a) &= (u8)~1;

    Func_0808a0d0(8, 0x98, 0x78);
    Func_0808a010(1);

    actor = Func_0808a080(8);
    *(actor + 0x5a) |= 1;

    Func_0808a010(20);
    Func_0808a110(8, 3);
    Func_0808a010(20);
    Func_0808a0d0(0, 0xa8, 0x78);
    Func_0808a0c8(0, 0xc0, 0xa8);
    Func_0808a010(20);
    Func_0808a0d0(8, 0xa8, 0x78);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a0e8(0);
    goto common;

fallback:
    Func_0808a0c8(0, 0xc0, 0xa8);
    Func_0808a010(20);
    Func_0808a1b8(8, 0x3000, 0);
    Func_0808a0e8(0);

common:
    Func_0200187c();
    Func_0808a330(0, 0);
    Func_0808a348(120);
    Func_0808a010(120);
    Func_080f9010(86);
    Func_080f9078();
    Func_080770c8(0x9f0);
    Func_0808a248(30);
    Func_0808a020();
}
