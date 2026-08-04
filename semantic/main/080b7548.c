#include "types.h"

struct Object_080b7548 {
    u8 padding[12];
    s32 x;
    s32 y;
};

void Func_080b7424(u16 *, s32, s32 *, s32 *);
struct Object_080b7548 *Func_080b7dd0(s32);

void Func_080b7548(void) {
    u16 ids[14];
    s32 xs[6];
    s32 ys[6];
    s16 *source;
    s32 check;
    s32 count;
    struct Object_080b7548 *object;

    source = (s16 *)(*(u8 **)0x03001E74 + 2);
    count = 0;
    check = source[50];
    if (check != 255) {
        s32 sourceOffset = 100;
        s32 arrayOffset = 0;
copy:
        count++;
        *(u16 *)((u8 *)ids + arrayOffset) =
            *(u16 *)((u8 *)source + sourceOffset);
        sourceOffset += 2;
        arrayOffset += 2;
        if (count <= 5) {
            check = *(volatile s16 *)((u8 *)source + sourceOffset);
            if (check != 255)
                goto copy;
        }
    }

    Func_080b7424(ids, count, xs, ys);
    if (count > 0) {
        s32 arrayOffset = 0;
        s32 sourceOffset = 100;
        do {
            if (*(s16 *)((u8 *)source + sourceOffset) != 254) {
                object = Func_080b7dd0(
                    *(s16 *)((u8 *)source + sourceOffset));
                object->x =
                    *(s32 *)((u8 *)xs + arrayOffset) << 16;
                object->y =
                    *(s32 *)((u8 *)ys + arrayOffset) << 16;
            }
            count--;
            arrayOffset += 4;
            sourceOffset += 2;
        } while (count != 0);
    }
}
