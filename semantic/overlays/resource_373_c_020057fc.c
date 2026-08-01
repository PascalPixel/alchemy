typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;

/*
 * Resource 373: rising-dust burst played from a source entity.
 *
 * Complete owner: `push {r5, r6, r7, lr}` plus the r8 save at 0x020057fc
 * through the single epilogue at 0x020058d6..0x020058de, followed by its
 * four-word literal pool at 0x020058e0.  Both counted loops fall into the
 * common tail; nothing stays live past the return.
 *
 * All twelve calls are placed.  None is an interworking `call_via rN` site.
 *
 * Both loops count down in r8 and test `bge`, so each body runs one more
 * time than the initial value suggests: 31 lift steps and 8 particles.
 *
 * UNCERTAINTY 1: the called service addresses are the ones encoded in the
 * overlay image (shared 0x02000000 namespace, load-time fixups).
 * UNCERTAINTY 2: 0x0200e6e4 is passed to Func_0200b760 as a plain pointer;
 * whether it is animation data or a callback is not established here.
 */

struct Resource373Emitter {
    u8 unknown_00[6];
    u16 field06;
    s32 x;                          /* 0x08 */
    s32 y;                          /* 0x0c */
    s32 z;                          /* 0x10 */
    u8 unknown_14[4];
    s32 field18;
    s32 field1c;
    u8 unknown_20[4];
    s32 field24;
    s32 field28;
    s32 field2c;
    u8 unknown_30[8];
    s32 field38;
    s32 field3c;
    s32 field40;
};

struct Resource373Particle {
    u8 unknown_00[0x28];
    s32 lifetime;                   /* 0x28 */
    u8 unknown_2c[4];
    s32 field30;
    s32 field34;
    u8 unknown_38[0x10];
    s32 field48;
    u8 unknown_4c[9];
    u8 field55;
};

void Func_0200b954();
void Func_0200b6c0();
struct Resource373Particle *Func_0200b754();
void Func_0200b7a8();
void Func_0200b760();
s32 Func_0200b70c();
s32 Func_0200b726();
s32 Func_0200b72c();
s32 Func_0200b734();
s32 Func_0200b744();
void Func_0200b19a();
void Func_0200ba08();

void Func_020057fc(struct Resource373Emitter *emitter)
{
    s32 remaining;

    Func_0200b954(154);

    for (remaining = 30; remaining >= 0; remaining--) {
        emitter->y += 0x10000;              /* 0x80 << 9 */
        emitter->field06 = (u16)(emitter->field06 + 0x2000);  /* 0x80 << 6 */
        emitter->field18 += -2048;          /* pool word 0xfffff800 */
        emitter->field1c += -2048;
        Func_0200b6c0(1);
    }

    for (remaining = 7; remaining >= 0; remaining--) {
        struct Resource373Particle *particle =
            Func_0200b754(0x11d, emitter->x, emitter->y, emitter->z);

        if (particle != 0) {
            s32 height;
            s32 speed;

            Func_0200b7a8(particle, 0);
            Func_0200b760(particle, (const void *)0x0200e6e4);

            height = Func_0200b70c() + 0x10000;
            particle->field34 = 0x10000;
            particle->field30 = height;
            particle->field55 = 2;
            particle->field48 = 0x0a3d;

            particle->lifetime = Func_0200b726() - Func_0200b72c();

            speed = Func_0200b734();
            speed = ((speed * 2 + speed) << 3) + 0x80000;   /* 24x + 0x80000 */
            Func_0200b19a(particle, speed, Func_0200b744());
        }
    }

    Func_0200ba08(131);

    emitter->x = 0;
    emitter->y = 0;
    emitter->z = 0;
    emitter->field38 = (s32)0x80000000;
    emitter->field3c = (s32)0x80000000;
    emitter->field40 = (s32)0x80000000;
    emitter->field24 = 0;
    emitter->field28 = 0;
    emitter->field2c = 0;
}
