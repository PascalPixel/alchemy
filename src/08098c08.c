#include "types.h"

struct Vector_08098c08 {
    s32 x;
    s32 y;
    s32 z;
};

struct Object_08096bec;

void Func_080f9010(s32);
u8 *Func_08096c80(s32, s32, s32, s32);
void Func_08009098(void *, const void *);
u32 Func_08004458(void);
void Func_08096bec(struct Object_08096bec *, s32, s32);

s32 Func_08098c08(void *argument)
{
    struct Vector_08098c08 position;
    void *first_object;
    void *child;
    register s32 count;
    register s32 speed;
    register struct Vector_08098c08 *position_pointer;

    Func_080f9010(0x86);
    position.x = *(s32 *)((u8 *)argument + 8);
    position.y = *(s32 *)((u8 *)argument + 12);
    position.z = *(s32 *)((u8 *)argument + 16);
    first_object = Func_08096c80(
        0x11B,
        position.x,
        position.y - 0x200000,
        position.z);
    if (first_object != 0) {
        u8 *field = (u8 *)first_object + 0x55;
        s32 timer;

        field[0] = 0;
        field += 9;
        timer = 20;
        *(u16 *)field = timer;
        Func_08009098(first_object, (const void *)0x0809F0B0);
    }

    position_pointer = &position;
    count = 11;
    speed = 0x10000;
    do {
        child = Func_08096c80(
            0x11D,
            position_pointer->x,
            position_pointer->y,
            position_pointer->z);
        if (child != 0) {
            s32 x;
            s32 y;

            Func_08009098(child, (const void *)0x0809F0D4);
            x = Func_08004458();
            x += speed;
            *(s32 *)((u8 *)child + 0x34) = speed;
            *(s32 *)((u8 *)child + 0x30) = x;
            *(u8 *)((u8 *)child + 0x55) = 0;
            y = Func_08004458() * 24 + 0x80000;
            Func_08096bec(
                (struct Object_08096bec *)child,
                y,
                Func_08004458());
        }
        count--;
    } while (count >= 0);
    return 0;
}
