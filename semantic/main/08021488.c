typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Effect {
    s32 timer;
    s32 flags;
    s32 value;
};

void Func_08016418(s32, s32);
void *Func_080165d8(s32, s32, s32, s32, const u16 *, s32);
u32 Func_08019d2c(void);
void Func_0801e41c(s32, s32, s32, s32);
void Func_08003dec(struct Effect *, s32);

void Func_08021488(u32 arg0, u32 arg1)
{
    u32 saved = arg1;
    s32 handle2;
    s32 value;
    s32 handle1;
    struct Effect second;
    struct Effect first;
    struct Effect *firstp = &first;
    struct Effect *secondp;
    s32 object = 0;
    s32 zero;
    void *state = *(void **)0x03001e8c;

    object = Func_080162d4(1, 1, 28, 5, object);
    zero = 0;
    if (object != 0) {
        Func_0801e41c(object, 8, 0, 4);
        *(s8 *)((s8 *)state + 0xea3) = 1;
        Func_08021360(arg0);
        Func_0801a4fc(Func_08019d2c(), 0, &handle1, &value, 14, zero);
        firstp->timer = zero;
        first.flags = 0x800c000c;
        first.value = value | 0xe000;
        {
            secondp = &second;
            Func_08021360(saved);
            Func_0801a4fc(Func_08019d2c(), 0, &handle2, &value, 15, zero);
            secondp->timer = zero;
            second.flags = 0x802c000c;
            second.value = value | 0xf000;
            *(s16 *)((s8 *)state + 0x12f4) = zero;
            *(s16 *)((s8 *)state + 0x12f6) = zero;
            Func_08019908(arg0, 1);
            Func_08019908(saved, 1);
            Func_080165d8(
                object, Func_08019ba0(29), 68, 2, 0, zero);
            Func_080f9010(81);
            do {
                Func_08003dec(firstp, 250);
                Func_08003dec(secondp, 250);
                Func_080030f8(1);
            } while (Func_080f9048() != 0 &&
                     (*(s32 *)0x03001c94 & 0x303) == 0);
        }
        Func_08016418(object, 2);
        Func_080030f8(1);
        Func_08003f3c(handle1);
        Func_08003f3c(handle2);
    }
}
