#include "types.h"

#define FIELD(p, type, off) (*(type *)((u8 *)(p) + (off)))

typedef struct Particle {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 life;
} Particle;

typedef void (*DrawFn)(void *, const void *, s32, s32, s32, s32);
typedef s32 (*CopyFn)(void *, const void *, s32);

extern u16 Data_080ede48[];
extern u8 Data_080eeadc[];
extern u8 Value_000000c4;
extern u8 Value_00000073;

void Func_080cdb24(s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
void Func_080e0524(s32, void *, s32, s32);
s32 Func_08004458(void);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080041d8(void *, s32);
void Func_080f9010(s32);
void Func_080b50e8(s32);
s32 Func_080022fc(s32, s32);
void Func_080e3908(void *, s32, s32);
void Func_080d6888(s16, s32, s32, s32, s32);
void Func_080e155c(s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08004278(void *);
void Func_08002dd8(s32);
void Func_080cdbc0(void);

void Func_080db264(void *object)
{
    void **cache;
    void **cursor;
    void *work;
    void *canvas;
    void *source;
    void *sprite;
    DrawFn rectangles[2];
    Particle *particle;
    s32 frame;
    s32 pass;
    s32 i;
    s32 spawned;
    s32 angle;
    s32 radius;
    s32 center_x;
    s32 center_y;
    s32 random_angle;
    s32 velocity;
    s32 size;
    s32 member;
    s32 member_offset;
    s32 member_frame;
    s32 limit;

    cache = (void **)0x03001eec;
    cursor = cache;
    work = *cursor++;
    canvas = *cursor;
    source = cache[2];
    FIELD(work, void *, 0x7828) = object;
    Func_080cdb24(1);

    if (FIELD(object, s32, 24) == 2)
        *(volatile u16 *)0x04000020 = 0x80;
    else
        *(volatile u16 *)0x04000020 = 0x100;

    Func_080ed408(46, 7, 7, 3, 2);
    rectangles[0] = (DrawFn)((void **)0x03001e50)[46];
    Func_080ed408(47, 7, 7, 7, 3);
    rectangles[1] = (DrawFn)((void **)0x03001e50)[47];
    Func_080e0524((s32)&Value_000000c4, (u8 *)work + 0x60e, 1, 1);
    Func_080e0524((s32)&Value_00000073, source, 0, 0);

    if (FIELD(object, s32, 24) == 2) {
        if (FIELD(object, s32, 4) == 1)
            *(volatile u32 *)0x04000028 = 0xfffff000;
        else
            *(volatile u32 *)0x04000028 = 0x1000;
    } else if (FIELD(object, s32, 4) == 1) {
        *(volatile u32 *)0x04000028 = 0xffff8000;
    }

    particle = (Particle *)0x02010000;
    i = 0;
    do {
        radius = (Func_08004458() & 0x3ff) + 0x100;
        random_angle = (Func_08004458() & 0x7fff) - 0x4000;
        particle->x = 0x4000;
        particle->y = 0x7000;
        particle->z = radius * Func_08002322(random_angle) >> 16;
        particle->vz = -(radius * Func_0800231c(random_angle) * 2) >> 16;
        particle->life = 0;
        i++;
        particle++;
    } while (i != 1024);

    FIELD(work, s32, 0x7780) = 2;
    FIELD(work, s32, 0x7784) = 75;
    Func_080041d8((void *)0x080cd261, 0x480);
    Func_080f9010(138);

    frame = 0;
    do {
        if (frame == 20)
            Func_080b50e8(133);
        if (frame <= 15) {
            if (Func_080022fc(frame, 5) == 2)
                ((CopyFn)0x03000168)(canvas, (void *)0x10101010, 0x4000);
            pass = 0;
            angle = frame << 11;
            do {
                spawned = 0;
                radius = pass * (angle + 0x4000);
                center_x = ((32 - frame) * Func_08002322(radius) >> 16) + 64;
                center_y = -(Func_0800231c(radius) * 8 >> 16) - 8;
                sprite = (u8 *)work + (Func_08004458() & 3) * 0xb40 + 0x60e;
                if (FIELD(object, s32, 24) == 0) {
                    rectangles[0](canvas, sprite,
                        center_x + (Func_08004458() & 7) - 16,
                        center_y, 24, 120);
                } else {
                    rectangles[pass & 1](canvas, sprite,
                        center_x + (Func_08004458() & 7) - 16,
                        center_y, 24, 120);
                }
                particle = (Particle *)0x02010000;
                i = 0;
                do {
                    if (particle->life == 0) {
                        velocity = (Func_08004458() & 0x1ff) + 0x80;
                        random_angle = (Func_08004458() & 0x7fff) - 0x4000;
                        particle->y = (center_y + 112) << 16;
                        particle->x = center_x << 16;
                        particle->vx = velocity * Func_08002322(random_angle) >> 9;
                        particle->vy = -(velocity * Func_0800231c(random_angle) * 2) >> 7;
                        particle->life = (Func_08004458() & 7) + 32;
                        spawned++;
                        if (spawned == Data_080eeadc[FIELD(object, s32, 24) * 2 + 1])
                            break;
                    }
                    i++;
                    particle++;
                } while (i != 1024);
                pass++;
            } while (pass != 4);
            FIELD(work, s32, 0x77a8) = 1;
        }

        particle = (Particle *)0x02010000;
        i = 0;
        do {
            if (particle->life > 0) {
                particle->life--;
                Func_080e3908(particle, 60, -0x800);
                if (particle->y > 0x780000) {
                    particle->vy = (-particle->vy + ((u32)-particle->vy >> 31)) >> 1;
                } else if ((u32)particle->x <= 0x7effff && particle->y >= 0) {
                    size = particle->life;
                    if (size < 0)
                        size += 7;
                    size = (size >> 3) + 1;
                    rectangles[0](canvas,
                        (u8 *)source + Data_080ede48[size - 1],
                        (particle->x >> 16) - ((size + ((u32)size >> 31)) >> 1),
                        (particle->y >> 16) - size, size, size * 2);
                }
            }
            i++;
            particle++;
        } while (i != 1024);

        if ((u32)(frame - 4) <= 91) {
            limit = FIELD(object, s32, 20);
            member = 0;
            member_offset = 36;
            member_frame = 4;
            while (member != limit) {
                if (frame == member_frame)
                    Func_080d6888(FIELD(object, s16, member_offset), 7, 5, member, 10);
                member++;
                member_offset += 2;
                member_frame += 4;
            }
        }
        Func_080e155c(2, 4);
        Func_080cd52c();
        FIELD(work, s32, 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    } while (frame != 64);

    Func_08004278((void *)0x080cd261);
    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_080cdbc0();
}
