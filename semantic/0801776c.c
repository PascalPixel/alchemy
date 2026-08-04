#include "types.h"

void Func_080187ac(
    s32 object,
    s32 *x,
    s32 *y,
    s32 *width,
    s32 *height);
void Func_0808a278(s32 map, s32 position[2]);
s32 Func_08017658(s32 object, s32 x, s32 y, s32 options);
s32 Func_08017364(void);
void Func_080030f8(u32 frames);
void Func_08016418(s32 object, s32 mode);
s32 Func_08017394(s32 object);

void Func_0801776c(s32 object, s32 options)
{
    u8 *state = *(u8 **)0x03001e8c;
    s32 position[2] = { 0, 0 };
    s32 x = 0;
    s32 y = 0;
    s32 width;
    s32 height;
    s32 mode = options & 1;
    s32 created;

    if (options & 2)
        state[0x12f9] = 1;

    Func_080187ac(object, &x, &y, &width, &height);
    x = (30 - width) >> 1;
    y = (12 - height) >> 1;

    if (options & 8) {
        y += 4;
    } else if (options & 0x40) {
        y += 12;
    } else {
        Func_0808a278(*(s32 *)0x02000434, position);
        y = position[1] >> 3;
        if (y > 9)
            y -= 5;
        else
            y += 4;
    }

    created = Func_08017658(object, x, y, mode);
    if (created != 0) {
        while (Func_08017364() == 0)
            Func_080030f8(1);

        if (options & 0x20)
            (*(u8 **)0x03001e8c)[0xea6] = 1;

        if (!(options & 4)) {
            Func_08016418(created, mode);
            while (Func_08017394(created) == 0)
                Func_080030f8(1);
        }
    }

    state[0x12f9] = 0;
    *(u16 *)(state + 0x12f4) = 0;
    *(u16 *)(state + 0x12f6) = 0;
    Func_080030f8(3);
}
