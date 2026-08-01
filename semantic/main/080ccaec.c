/*
 * Correctness fix, veneer audit (mars, 2026-08-01).
 *
 * `Func_080072f0` is not a function.  0x080072e4 begins the GCC
 * `__call_via_rN` veneer bank -- fifteen four-byte `bx rN; nop` entries,
 * r0..lr, ending at 0x08007320 -- so 0x080072f0 is `__call_via_r3` and
 * `bl 0x80072f0` calls whatever r3 holds.
 *
 * At every site in this file the ROM loads r3 from the literal pool with
 * the constant 0x03001388, so the callee is the relocated IWRAM word copy
 * at that address.  Its signature is not guessed: the EXACT source
 * src/080d40ec.c declares it as
 * `void *(*)(void *destination, const void *source, s32 size)` and
 * src/080e0524.c casts the same address to the same shape.
 *
 * Note what the previous draft had already half-seen: it passed
 * 0x03001388 as a fourth ARGUMENT.  That value was never an argument --
 * it is the callee, and the register load that produced it is the call
 * target, not a parameter.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *(*WordCopy)(void *destination, const void *source, s32 size);

void *Func_08002f40(s32);
void Func_080041d8(const void *, s32);
void *Func_080048b0(s32, s32);
void Func_080cd594(s32);

/*
 * Initialize the affine-background scene, choose its palette by mode, reset
 * the scene counters, and register both frame callbacks.
 */
void Func_080ccaec(u32 mode) {
    u8 *scene;
    s32 palette_resource;

    scene = Func_080048b0(0x27, 0x782C);
    Func_080048b0(0x28, 0x4000);
    Func_080cd594(0);

    *(s32 *)(scene + 0x77B4) = 0x18;
    *(volatile u16 *)0x04000020 = 0x0100;
    *(volatile u16 *)0x04000052 = 0x1010;

    switch (mode) {
    case 0:
        palette_resource = 0xC8;
        break;
    case 1:
        palette_resource = 0xCF;
        break;
    case 2:
        palette_resource = 0xB4;
        break;
    case 3:
        palette_resource = 0xCB;
        break;
    default:
        palette_resource = 0xBE;
        break;
    }
    ((WordCopy)0x03001388)(
        (const void *)0x05000000,
        Func_08002f40(palette_resource),
        0x80);

    *(s32 *)(scene + 0x778C) = 0;
    *(s32 *)(scene + 0x7780) = 3;
    *(s32 *)(scene + 0x7784) = 0x06060606;
    Func_080041d8((const void *)0x080CC961, 0xC80);
    Func_080041d8((const void *)0x080CD261, 0x480);
}
