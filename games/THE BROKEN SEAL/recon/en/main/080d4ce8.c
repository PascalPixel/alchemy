#include "TYPES.H"

typedef void (*DrawRectangle)(void *, const void *, s32, s32, s32, s32);
typedef s32 (*CopyWords)(void *, const void *, s32);
typedef struct Effect {
    s32 kind, side, actor, unk0c, unk10, count, variant, unk1c, unk20;
    s16 actors[8];
} Effect;
typedef struct Particle {
    s32 x, y, vx, unused0c, vy, unused14, life;
} Particle;
typedef struct Anchor {
    s32 x, y, z, unused0c, unused10, unused14, unused18;
} Anchor;
extern void *Data_03001e50[];
extern u8 Value_00000085, Value_00000073, Value_00000086, Value_00000087;
extern u8 Data_080ee29a[], Data_080ee2a9[];
extern s8 Data_080ee29d[];
extern u16 Data_080ede48[];
void Func_080cd594(s32);
void *Func_08002f40(s32);
void Func_080e0524(s32, void *, s32, s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void **Func_080b5098(s32);
s32 Func_08004458(void);
s32 Func_080041d8(s32, s32);
void Func_08004278(s32);
void Func_080049ac(void);
void Func_080051d8(void *, void *);
void Func_080b50e8(s32);
void Func_080e3944(void *, s32 *);
void Func_080f9010(s32);
s32 Func_080022fc(s32, s32);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080b5088(s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08002dd8(s32);
s32 Func_080cdbc0(void);
#define EFFECT (*(Effect **)(work + 0x7828))
#define PARTICLES ((Particle *)0x02010000)

/* Four projected columns emit particles into a shared pool. The complete
 * owner ends at 080d5258, including both internal and final literal pools. */
void Func_080d4ce8(Effect *effect)
{
    void **cache;
    void **entry;
    u8 *work, *extra;
    void *destination;
    void *camera;
    void *actor;
    void *palette;
    DrawRectangle draw[2];
    DrawRectangle *draws;
    DrawRectangle second;
    Particle *particle;
    Anchor *anchor;
    s32 screen[3];
    s32 frame, i, slot, status;
    s32 alternate;
    u8 *trigger;
    u8 *overlay;

    cache = &Data_03001e50[39];
    entry = cache;
    work = *entry++;
    destination = *entry;
    extra = cache[2];
    EFFECT = effect;
    Func_080cd594(1);
    *(u16 *)0x04000052 = 0x1010;
    Func_080e0524((s32)&Value_00000085, work, 1, 1);
    Func_080e0524((s32)&Value_00000073, extra, 0, 0);
    if (EFFECT->variant == 0) {
        palette = Func_08002f40((s32)&Value_00000086);
        status = ((CopyWords)0x03001388)((void *)0x05000000, palette, 128);
    } else if (EFFECT->variant == 2) {
        palette = Func_08002f40((s32)&Value_00000087);
        status = ((CopyWords)0x03001388)((void *)0x05000000, palette, 128);
    }
    status = Func_080ed408(46, 7, 7, 3, 2);
    draw[0] = (DrawRectangle)Data_03001e50[46];
    status = Func_080ed408(47, 7, 7, 7, 2);
    second = (DrawRectangle)Data_03001e50[47];
    draws = draw;
    draws[1] = second;
    i = 0;
    particle = PARTICLES;
    do {
        i++;
        particle->life = 0;
        particle++;
    } while (i != 1024);
    actor = *Func_080b5098(EFFECT->actors[0]);
    anchor = (Anchor *)(work + 0x7080);
    i = 0;
    do {
        s32 x;
        x = ((Func_08004458() & 15) + 72) << 16;
        anchor->y = 0;
        anchor->x = x;
        anchor->z = Data_080ee29d[EFFECT->variant * 4 + i] << 16;
        if (*(s32 *)((u8 *)actor + 8) < 0)
            anchor->x = -x;
        i++;
        anchor++;
    } while (i != 4);
    *(s32 *)(work + 0x7780) = 2;
    *(s32 *)(work + 0x7784) = 50;
    Func_080041d8(0x080cd261, 0x480);
    frame = 0;
    do {
        camera = Data_03001e50[12];
        if (EFFECT->variant == 2 && frame <= 63) {
            if (EFFECT->side == 0)
                *(u16 *)((u8 *)camera + 54) += 192;
            else
                *(u16 *)((u8 *)camera + 54) -= 192;
        }
        if (frame == 16)
            Func_080b50e8(134);
        Func_080049ac();
        Func_080051d8(camera, (u8 *)camera + 12);
        if (frame <= 63) {
            slot = 0;
            if (Data_080ee29a[EFFECT->variant] != 0) {
                alternate = frame & 1;
                anchor = (Anchor *)(work + 0x7080);
                overlay = work + 0xdd0;
                trigger = Data_080ee2a9;
                do {
                    Func_080e3944(anchor, screen);
                    screen[0] /= 2;
                    screen[1] -= 8;
                    if (frame == *trigger)
                        Func_080f9010(145);
                    if (frame >= *trigger + 4) {
                        s32 spin;
                        spin = Func_080022fc(slot * 25 + (frame << 4), 104);
                        draws[slot & 1](destination, work, screen[0] - 17,
                            screen[1] - spin - 104, 34, 104);
                        draws[slot & 1](destination, work, screen[0] - 17,
                            screen[1] - spin, 34, spin);
                        if (alternate != 0) {
                            draw[0](destination, overlay, screen[0] - 20, screen[1] - 24, 20, 37);
                            draws[1](destination, overlay, screen[0], screen[1] - 24, 20, 37);
                        } else {
                            draw[0](destination, work + 0x10b4, screen[0] - 20, screen[1] - 24, 20, 37);
                            draws[1](destination, work + 0x10b4, screen[0], screen[1] - 24, 20, 37);
                        }
                    }
                    if (frame == *trigger || frame >= *trigger + 16) {
                        s32 spawned;
                        spawned = 0;
                        i = 0;
                        particle = PARTICLES;
                        do {
                            if (particle->life == 0) {
                                s32 magnitude, angle;
                                magnitude = Func_08004458() & 0x3ff;
                                angle = Func_08004458();
                                particle->x = screen[0] << 8;
                                particle->y = (screen[1] << 8) + 4096;
                                angle = (angle & 0x7fff) - 0x4000;
                                particle->vx = ((magnitude + 32) * Func_08002322(angle)) >> 15;
                                magnitude += 32;
                                particle->vy = -((magnitude * Func_0800231c(angle)) << 1) >> 15;
                                spawned++;
                                if (frame == *trigger) {
                                    particle->life = (Func_08004458() & 7) + 48;
                                    if (spawned == 200) break;
                                } else {
                                    particle->life = (Func_08004458() & 7) + 24;
                                    if (spawned == 4) break;
                                }
                            }
                            i++;
                            particle++;
                        } while (i != 1024);
                    }
                    if (frame == Data_080ee2a9[slot]) {
                        *(s32 *)(work + 0x77a8) = 2;
                        i = 0;
                        while (i != EFFECT->count) {
                            Func_080d6888(EFFECT->actors[i], 10, 5, i, 8);
                            Func_080b5088(EFFECT->actors[i], 1);
                            i++;
                        }
                    }
                    slot++;
                    anchor++;
                    trigger++;
                } while (slot != Data_080ee29a[EFFECT->variant]);
            }
        }
        i = 0;
        particle = PARTICLES;
        do {
            s32 life;
            life = particle->life;
            if (life > 0) {
                s32 x, y, px, py, size;
                particle->life = life - 1;
                x = particle->x + particle->vx;
                y = particle->y + particle->vy;
                particle->x = x;
                particle->y = y;
                particle->vx = (particle->vx * 60) / 64;
                particle->vy = (particle->vy * 60) / 64 - 16;
                py = y / 256;
                if (py > 120) {
                    particle->vy = -particle->vy / 2;
                } else if (x >= 0) {
                    px = x >> 8;
                    if (px <= 126 && y >= 0) {
                        size = (life - 17) / 8;
                        if (size <= 0) size = 1;
                        draws[i & 1](destination, extra + Data_080ede48[size - 1],
                            px - size / 2, py - size, size, size * 2);
                    }
                }
            }
            i++;
            particle++;
        } while (i != 1024);
        Func_080e155c(16, 16);
        Func_080cd52c();
        *(s32 *)(work + 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 96);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278(0x080cd261);
    Func_080cdbc0();
}
