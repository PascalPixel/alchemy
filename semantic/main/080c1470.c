#include "types.h"

/*
 * Set up the title backdrop: allocate the 0x13d0-byte scene block, seed
 * sixteen particles and three rotating rings from the random source, clear the
 * work surface, install the shared palette, DMA the mode's own palette over
 * it, reset the BG2 affine registers to identity, register the two scene
 * handlers, and start the two scripts.
 *
 * The mov ip,pc ; bx sl / bx r6 sequences at 0x080c14c8, 0x080c14d8,
 * 0x080c155c, 0x080c156c, 0x080c157c and 0x080c158c are ordinary indirect
 * calls to the pooled 0x03000118, the IWRAM-relocated ARM fixed-point
 * multiply, each landing site consuming r0 in the same frame, as recorded for
 * 0x0800ebec.  The movs r0,r0 before four of them is inside the instruction
 * stream, aligning the return address that mov ip,pc captures so it lands past
 * the bx; the two already-aligned sites carry no nop.
 *
 * The bl Func_080072f0 at 0x080c15c8 and 0x080c15e2 is the runtime
 * _call_via_r3 thunk inside 0x080072e4..0x08007320 - index 3 - an indirect
 * call through the pooled 0x03000164 (the IWRAM-relocated block clear) and
 * 0x03001388 respectively.
 *
 * The single stmia r3!,{r0,r1,r2} at 0x080c1624 writes the three DMA3
 * registers through the pooled 0x040000d4; the register walk that follows
 * reaches back to 0x04000028 and forward again over BG2X, BG2Y and the four
 * BG2 affine halfwords, so it is one pointer stepping over the I/O block, not
 * further transfers.
 *
 * The epilogue at 0x080c1678 ends pop {r1}; bx r1 with N != 0, so r0 is the
 * result, and the value reaching it is the second Func_080041d8 return.
 */

struct Particle_080c1470 {
    s32 x;
    s32 y;
    s32 z;
    s32 velocity_x;
    s32 velocity_y;
    s32 velocity_z;
    s32 life;
};

struct Ring_080c1470 {
    s32 x;
    s32 y;
    s32 velocity_x;
    s32 velocity_y;
    s32 frame;
};

struct Scene_080c1470 {
    u8 unknown_0000[4544];
    struct Particle_080c1470 particles[16];
    struct Ring_080c1470 rings[3];
    s32 field13bc;
    s32 field13c0;
    u8 unknown_13c4[8];
    s32 field13cc;
};

struct Runtime_080c1470 {
    u8 unknown_00[8];
    s32 active;
};

struct Dma3_080c1470 {
    const void *source;
    void *destination;
    u32 control;
};

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_080c1470)(s32 a, s32 b);
/* 0x03000164, the IWRAM-relocated block clear. */
typedef void (*Clear_080c1470)(void *block, s32 size);
/* 0x03001388, the IWRAM-relocated palette upload. */
typedef void (*Upload_080c1470)(void *destination, const void *source,
                                s32 size);

extern struct Runtime_080c1470 *Data_03001f00;
extern u8 Data_03001e50[];

struct Scene_080c1470 *Func_080048f4(s32 kind, s32 size);
void Func_080048b0(s32 kind, s32 size);
s32 Func_08004458(void);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void *Func_08002f40(s32 resource);
void Func_08005340(const void *source, void *destination);
void Func_080c9000(s32 kind, s32 x, s32 y, s32 z, s32 mode);
s32 Func_080041d8(s32 script, s32 argument);

s32 Func_080c1470(s32 mode)
{
    volatile struct Dma3_080c1470 *dma =
        (volatile struct Dma3_080c1470 *)0x040000d4;
    Multiply_080c1470 multiply = (Multiply_080c1470)0x03000118;
    struct Scene_080c1470 *scene;
    struct Particle_080c1470 *particle;
    struct Ring_080c1470 *ring;
    u8 *palette;
    s32 count;
    s32 angle;
    s32 resource;

    Data_03001f00->active = 1;

    scene = Func_080048f4(39, 0x13d0);
    Func_080048b0(40, 0x4000);

    particle = scene->particles;
    for (count = 15; count >= 0; count--) {
        s32 heading = Func_08004458();
        u32 span = 0x10000 + (u32)Func_08004458();
        s32 scale = (s32)(span >> 1);

        particle->x = multiply(Func_0800231c(heading), scale);
        particle->y = multiply(Func_08002322(heading), scale);
        if ((particle->x & 1) != 0)
            particle->x = -particle->x;
        if ((particle->y & 1) != 0)
            particle->y = -particle->y;

        particle->z = (s32)(((u32)Func_08004458() + 0x8000) >> 2);
        particle->velocity_x = ((-particle->x) >> 7) + (particle->y >> 8);
        particle->velocity_y = ((-particle->y) >> 7) + ((-particle->x) >> 8);
        particle->velocity_z = 0;
        particle->life = (s32)(span >> 13) + 1;
        particle++;
    }

    ring = scene->rings;
    angle = 0;
    for (count = 2; count >= 0; count--) {
        ring->x = multiply(Func_0800231c(angle), 0x1000);
        ring->y = multiply(Func_08002322(angle), 0x1000);
        ring->velocity_x = multiply(Func_0800231c(angle), 512);
        ring->velocity_y = multiply(Func_08002322(angle), 512);
        ring->frame = 0;
        angle += 0x5555;
        ring++;
    }

    scene->field13bc = 0;
    scene->field13c0 = 0;
    scene->field13cc = 0;

    ((Clear_080c1470)0x03000164)(*(void **)(Data_03001e50 + 160), 0x4000);

    palette = (u8 *)Func_08002f40(0xc9);
    ((Upload_080c1470)0x03001388)((void *)0x05000000, palette, 128);
    Func_08005340(palette + 128, scene);

    switch (mode) {
    case 0:
        resource = 0xc8;
        break;
    case 1:
        resource = 0xc9;
        break;
    case 2:
        resource = 0xca;
        break;
    default:
        resource = 0xcb;
        break;
    }

    dma->source = Func_08002f40(resource);
    dma->destination = (void *)0x05000000;
    dma->control = 0x84000020;

    *(volatile u32 *)0x04000028 = 0;
    *(volatile u32 *)0x0400002c = 0;
    *(volatile u16 *)0x04000020 = 256;
    *(volatile u16 *)0x04000022 = 0;
    *(volatile u16 *)0x04000024 = 0;
    *(volatile u16 *)0x04000026 = 256;

    Func_080c9000(46, 7, 7, 3, 3);
    Func_080c9000(47, 7, 7, 3, 2);

    Func_080041d8(0x080c11ed, 3200);
    return Func_080041d8(0x080c1439, 3200);
}
