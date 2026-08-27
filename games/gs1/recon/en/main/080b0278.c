#include "types.h"

extern void *Data_03001f2c;

s32 Func_080b26c8(s32);
void Func_080b26cc(s32);
void Func_080b010c(void);
s32 Func_080b2764(s32);
void *Func_0808a080(s32);
s32 Func_080150f8(s32, s32, s32, s32);
s32 Func_08015010(s32, s32, s32, s32, s32);
s32 Func_080150c8(s32, s32, s32, s32);
void Func_080b0a20(s32, s32, s32);
void Func_080b04dc(s32);
s32 Func_08015380(s32);
s32 Func_080b2720(s32, s32);
void Func_080b1a14(void);
s32 Func_080b0070(void);
void Func_080b0aac(void);
void Func_080030f8(s32);
void Func_080b2110(void);
void Func_08015018(s32, s32);
void Func_080b0204(void);

s32 Func_080b0278(s32 mode, s32 object_id)
{
    void *state;
    void *object;
    s32 resource_window;
    s32 room_price;
    void *object2;
    s32 mode2;

    if (mode >= Func_080b26c8(mode) || mode < 0) {
        mode = 0;
    }

    Func_080b26cc(mode);
    Func_080b010c();
    state = Data_03001f2c;

    *(u8 *)((u8 *)state + 0x3a9) = (u8)Func_080b2764(mode);

    if (mode == 16 || mode == 17 || mode == 18) {
        *(u8 *)((u8 *)state + 940) = 1;
    }

    object = Func_0808a080(object_id);
    *(u16 *)((u8 *)state + 932) =
        **(u16 **)((u8 *)(*(u8 **)((u8 *)object + 80)) + 40);
    resource_window = Func_080150f8(*(u16 *)((u8 *)state + 932), 0, 0, 0);

    if (resource_window == 0) {
        resource_window = Func_08015010(-5, 0, 5, 5, 2);
    }

    room_price = *(u16 *)((u8 *)state + 912);
    object2 = (void *)Func_080150c8(room_price, 0x40000000, resource_window, 0);
    *(u8 *)((u8 *)object2 + 5) = 1;
    *(u8 *)((u8 *)object2 + 4) = 0x3aa;

    Func_080b0a20((s32)((u8 *)state + 896), -32, 112);
    *(void **)((u8 *)state + 896) = object2;
    Func_080b04dc(0xc9b);

    mode2 = 0;
    for (;;) {
        mode2 = Func_08015380(mode2);
        *(u8 *)((u8 *)state + 620) = (u8)mode2;

        if (mode2 == 0) {
            s32 result = Func_080b2720(mode, (s32)((u8 *)state + 620));
            *(u8 *)((u8 *)state + 620) = (u8)result;
            Func_080b04dc(0xca7);
            Func_080b04dc(0xca9);
            Func_080b0aac();
        } else if (mode2 == 1) {
            Func_080b04dc(0xcb8);
            Func_080b1a14();
        } else if (mode2 == 2) {
            if (Func_080b0070() != 0) {
                Func_080b04dc(0xca7);
                Func_080b04dc(0xca9);
                Func_080b0aac();
            } else {
                Func_080b04dc(0xcb7);
                Func_080030f8(1);
            }
        } else if (mode2 == 3) {
            Func_080b04dc(0xcb9);
            Func_080b2110();
        } else {
            Func_080b04dc(0xca5);
            Func_08015018(resource_window, 2);
            Func_080b0204();
            return 0;
        }

        Func_080b0a20((s32)((u8 *)state + 896), -32, 112);
        Func_080b04dc(0xca4);
    }
}
