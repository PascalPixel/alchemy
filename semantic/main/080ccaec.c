typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

void *Func_08002f40(s32);
void Func_080041d8(const void *, s32);
void *Func_080048b0(s32, s32);
void Func_080072f0(const void *, const void *, s32, void *);
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
    Func_080072f0(
        (const void *)0x05000000,
        Func_08002f40(palette_resource),
        0x80,
        (void *)0x03001388
    );

    *(s32 *)(scene + 0x778C) = 0;
    *(s32 *)(scene + 0x7780) = 3;
    *(s32 *)(scene + 0x7784) = 0x06060606;
    Func_080041d8((const void *)0x080CC961, 0xC80);
    Func_080041d8((const void *)0x080CD261, 0x480);
}
