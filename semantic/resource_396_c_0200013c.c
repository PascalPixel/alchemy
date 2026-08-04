#include "types.h"

struct Sprite {
    u8 unknown00[9];
    u8 flags9;
    u8 unknown0a[20];
    u16 angle;
    u8 unknown20[6];
    u8 state26;
};

struct Particle {
    u8 unknown00[24];
    s32 accum18;
    s32 accum1c;
    u8 unknown20[3];
    u8 flags23;
    u8 unknown24[12];
    s32 rate30;
    s32 rate34;
    u8 unknown38[12];
    s32 velocityX;
    s32 velocityY;
    s32 velocityZ;
    struct Sprite *sprite;
    u8 unknown54;
    u8 mode55;
    u8 unknown56[14];
    u16 step64;
    u8 unknown66[6];
    void (*callback)(s32 *particle);
};

struct EffectParams {
    s32 shape;
    s32 mode;
    s32 color1;
    s32 color2;
    s32 rate1;
    s32 rate2;
    s16 id;
    u8 unknown1a[2];
    s32 selector;
    u16 angle;
    u16 step;
    void (*callback)(s32 *particle);
};

struct Selector {
    u8 unknown00[12];
    s32 duration;
};

extern struct Selector *Data_02009da8[];
extern s32 Func_03000380(s32 value, s32 divisor);
extern struct Particle *Func_0808a080(s32 actor);
extern struct Particle *Func_080090c8(s32 id, s32 x, s32 y, s32 z);
extern void Func_08009080(struct Particle *particle, s32 mode);
extern void Func_08009098(struct Particle *particle, struct Selector *selector);
extern void Func_0808a160(struct Particle *particle, s32 mode);
extern void Func_02000104(s32 *particle);

/* Create and configure one particle from the overlay's flag-driven ABI. */
void Func_0200013c(s32 x, s32 y, s32 z,
                   s32 velocityX, s32 velocityY, s32 velocityZ,
                   u32 flags, struct EffectParams *params)
{
    struct Particle *template = Func_0808a080(0);
    struct Particle *particle;
    struct Sprite *sprite;
    struct Selector *selector;
    s32 id;
    s32 delta;

    if ((flags & 0x00100000) != 0 && params != 0)
        id = params->id;
    else
        id = 222;

    particle = Func_080090c8(id, x, y, z);
    if (particle == 0)
        return;
    sprite = particle->sprite;

    Func_08009080(particle, (flags + 1) & 15);
    Func_08009098(particle, Data_02009da8[flags & 15]);
    particle->mode55 = 0;
    sprite->state26 = 0;
    particle->callback = Func_02000104;
    particle->velocityX = velocityX;
    particle->velocityY = velocityY;
    particle->velocityZ = velocityZ;
    sprite->flags9 = (u8)((sprite->flags9 & ~12)
                          | (template->sprite->flags9 & 12));
    particle->rate30 = 0;
    particle->rate34 = 0;
    particle->step64 = 0;

    if ((flags & 0xffff0000) == 0 || params == 0)
        return;

    if ((flags & 0x00010000) != 0)
        Func_0808a160(particle, params->mode);

    if ((flags & 0x00020000) != 0) {
        particle->flags23 &= 0xfe;
        sprite->flags9 = (u8)((sprite->flags9 & ~12)
                              | ((params->shape & 3) << 2));
    }

    if ((flags & 0x00080000) != 0) {
        particle->accum18 = params->color1;
        particle->accum1c = params->color2;
    }

    if ((flags & 0x00040000) != 0) {
        selector = Data_02009da8[flags & 15];
        if ((flags & 0x00080000) != 0) {
            particle->rate30 =
                Func_03000380(params->rate1 - particle->accum18,
                              selector->duration);
            delta = params->rate2 - particle->accum1c;
        } else {
            particle->rate30 =
                Func_03000380(params->rate1 - 0x10000,
                              selector->duration);
            delta = params->rate2 - 0x10000;
        }
        particle->rate34 = Func_03000380(delta, selector->duration);
    }

    if ((flags & 0x00200000) != 0) {
        Func_08009080(particle, 1);
        Func_08009098(particle, (struct Selector *)params->selector);
    }
    if ((flags & 0x00400000) != 0)
        sprite->angle = params->angle;
    if ((flags & 0x00800000) != 0)
        particle->step64 = params->step;
    if ((flags & 0x01000000) != 0)
        particle->callback = params->callback;
}
