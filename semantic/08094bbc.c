#include "types.h"

struct Particle_08094bbc {
    u8 reserved00[4];
    s8 screenY;
    u8 attr0High;
    u16 attr1;
    u16 attr2;
    u8 reserved0a[2];
    s32 x;
    s32 groundY;
    s32 y;
    s32 verticalOffset;
    u16 lifetime;
    u8 reserved1e[2];
};

struct ParticleManager_08094bbc {
    u8 reserved00[4];
    s32 tileIndex;
    struct Particle_08094bbc particles[32];
};

struct Position_08094bbc {
    s32 x;
    s32 reserved04;
    s32 y;
};

struct ViewState_08094bbc {
    struct Position_08094bbc *source;
    u8 reserved04[0xe0];
    s32 x;
    s32 y;
};

s32 Func_080770c0(s32 flag);
u32 Func_08004458(void);
s32 Func_080091a8(s32 layer, s32 x, s32 y);
void Func_08003dec(void *entry, s32 limit);

/*
 * Update the manager's 32 particles and emit at most eight replacements in a
 * frame.  Positions are 16.16 fixed point; the first ten bytes of each record
 * are the display entry consumed by Func_08003dec.
 */
void Func_08094bbc(void)
{
    struct ParticleManager_08094bbc *manager =
        *(struct ParticleManager_08094bbc **)0x03001ec4;
    struct ViewState_08094bbc *view =
        *(struct ViewState_08094bbc **)0x03001e70;
    struct Particle_08094bbc *particle = manager->particles;
    u32 spawned = 0;
    u32 index;

    for (index = 0; index < 32; index++, particle++) {
        u16 lifetime = particle->lifetime - 1;

        particle->lifetime = lifetime;
        if (lifetime != 0xffff) {
            s32 viewX = view->x;
            s32 viewY = view->y;
            s32 screenX;
            s32 screenY;
            u16 tile;

            if (Func_080770c0(0x166) != 0) {
                particle->lifetime++;
                particle->verticalOffset--;
            }

            screenX = ((particle->x - viewX) >> 16) +
                (((Func_08004458() & 1) + (Func_08004458() & 1)) >> 1);
            screenY = ((particle->y - particle->groundY - viewY) >> 16) -
                (u16)lifetime;

            if ((u32)(screenX + 15) <= 255 &&
                screenY >= -32 && screenY <= 159) {
                if (lifetime <= 59) {
                    tile = (manager->tileIndex + 16) & 0x3ff;
                    particle->verticalOffset += 3;
                } else if (lifetime <= 89) {
                    tile = (manager->tileIndex + 8) & 0x3ff;
                    particle->verticalOffset += 1;
                } else {
                    tile = manager->tileIndex & 0x3ff;
                }

                particle->attr2 = (particle->attr2 & 0xfc00) | tile;
                if ((*(u32 *)0x03001e40 >> 3) & 1)
                    particle->attr2 =
                        (particle->attr2 & 0xfc00) |
                        ((particle->attr2 + 4) & 0x3ff);

                particle->attr1 =
                    (particle->attr1 & 0xfe00) | ((screenX - 1) & 0x1ff);
                particle->screenY =
                    screenY - (particle->verticalOffset >> 2);
                particle->attr0High &= 0x3f;
                particle->attr1 =
                    (particle->attr1 & 0x3fff) | 0x4000;
                Func_08003dec(particle, 0xf0);
            } else {
                particle->lifetime = 0;
            }
        }

        if (spawned <= 7 && particle->lifetime == 0) {
            struct Position_08094bbc *source = view->source;
            s32 x = source->x + (Func_08004458() << 8) - 0x800000;
            s32 y = source->y + (Func_08004458() << 8) - 0x800000;

            particle->y = y;
            particle->x = x;
            particle->groundY =
                (s32)((u32)Func_080091a8(0, x >> 16, y >> 16) << 16);
            particle->lifetime = 120;
            particle->verticalOffset = 0;
            spawned++;
        }
    }
}
