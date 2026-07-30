#include "types.h"

struct Position_0809a294 {
    s32 x;
    s32 y;
    s32 z;
};

struct Root_0809a294 {
    u8 pad_00[4];
    s32 x;
    s32 y;
    s32 z;
};

struct Particle_0809a294 {
    u8 pad_00[6];
    u16 angle;
    u8 pad_08[0x28];
    s32 scale;
    s32 baseScale;
    u8 pad_38[0x1d];
    u8 mode;
};

void Func_08097384(void);
struct Particle_0809a294 *Func_0809a3c4(s32 x, s32 y, s32 z, s32 angle);
void Func_080030f8(s32 frames);
void Func_08096bec(struct Particle_0809a294 *particle, s32 speed, s32 angle);
void Func_08009158(struct Particle_0809a294 *particle);
void Func_080f9010(s32 sound);
struct Particle_0809a294 *Func_08096c80(s32 kind, s32 x, s32 y, s32 z);
void Func_08009098(struct Particle_0809a294 *particle, const void *callback);
u32 Func_08004458(void);
void Func_080090d0(struct Particle_0809a294 *particle);
void Func_0809748c(void);
extern u8 Data_0809f0d4;

void Func_0809a294(void)
{
    struct Root_0809a294 *root = *(struct Root_0809a294 **)0x03001f30;
    struct Particle_0809a294 *handles[2];
    struct Position_0809a294 pos;
    struct Particle_0809a294 **cursor;
    struct Particle_0809a294 *object;
    s32 count;

    Func_08097384();
    pos.x = root->x;
    pos.y = root->y + 0x100000;
    pos.z = root->z;
    handles[0] = Func_0809a3c4(pos.x + 0x200000, pos.y, pos.z, 0x8000);
    handles[1] = Func_0809a3c4(pos.x - 0x200000, pos.y, pos.z, 0);
    Func_080030f8(15);

    cursor = handles;
    count = 1;
    do {
        object = *cursor++;
        if (object != 0)
            Func_08096bec(object, 0x180000, object->angle);
        count--;
    } while (count >= 0);

    Func_08009158(handles[0]);
    Func_080f9010(134);

    count = 23;
    do {
        pos.x = root->x;
        pos.y = root->y + 0x100000;
        pos.z = root->z;
        object = Func_08096c80(0x11d, pos.x, pos.y, pos.z);
        if (object != 0) {
            u32 scale;
            s32 speed;
            s32 baseScale = 0x20000;

            Func_08009098(object, &Data_0809f0d4);
            scale = Func_08004458();
            object->baseScale = baseScale;
            scale += baseScale;
            object->scale = (s32)scale;
            object->mode = 0;
            speed = Func_08004458() * 24 + 0x80000;
            Func_08096bec(object, speed, Func_08004458());
        }
        count--;
    } while (count >= 0);

    Func_080090d0(handles[0]);
    Func_080090d0(handles[1]);
    Func_0809748c();
}
