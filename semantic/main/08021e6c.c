#include "types.h"

#define REG16(address) (*(volatile u16 *)(address))
#define PTR32(address) (*(void **)(address))
#define S32_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))
#define U32_AT(base, offset) (*(u32 *)((u8 *)(base) + (offset)))
#define U16_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define U8_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))

void *Func_080040b4(s32);
void *Func_08004970(s32);
void *Func_080162d4(s32, s32, s32, s32, s32);
void *Func_08077000(s32);
void Func_080b5130(s32, void *);
void Func_08021d88(void *, s32);
void Func_080041d8(void (*)(void), s32);
void Func_0800307c(s32, s32, const void *);
s32 Func_08003d28(const void *);
void Func_08016478(void *);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_08003dec(void *, s32);
void *Func_08021e48(s32, s32, s32);
void Func_080b5128(s32, s32);
void Func_08016418(void *, s32);
s32 Func_080022fc(s32, s32);
void Func_080f9010(s32);
void Func_0800387c(volatile void *, s32);
void Func_080030f8(s32);
void Func_08003f3c(u32);
void Func_08002df0(void *);
void Func_08004278(const void *);

static void FirstFrame_08021e6c(void *tilemap, s32 count, s32 alternate)
{
    s32 item;
    s32 row;
    s32 column;
    u16 blank = 0xf07f;
    u32 characterBase = alternate ? 0x0600fd6c : 0x0600fd68;
    s32 mapBase = alternate ? 0x46c : 0x468;
    s32 blankBase = alternate ? 0x448 : 0x444;

    for (item = 0; item < 6 - count; item++) {
        u8 *block = (u8 *)tilemap + item * 6;
        for (row = 0; row < 3; row++)
            for (column = 0; column < 3; column++)
                *(u16 *)(block + row * 64 + blankBase +
                    (column & 3) * 2) = blank;
    }

    for (item = 0; item < count; item++) {
        for (row = 0; row < 3; row++) {
            for (column = 0; column < 3; column++) {
                s32 sourceIndex = item * 3 + row * 32 + (column & 3);
                s32 destinationIndex = sourceIndex - count * 3;
                *(volatile u16 *)(characterBase + destinationIndex * 2) =
                    (u16)(0x100 + item * 16 + row * 4 + column);
                *(u16 *)((u8 *)tilemap + sourceIndex * 2 -
                    count * 6 + mapBase) = 0;
            }
        }
    }
}

u32 Func_08021e6c(s32 alternate)
{
    void *tilemap = Func_080040b4(0x400);
    void *state = Func_08004970(0x1e0);
    void *world = PTR32(0x03001e8c);
    void *session = PTR32(0x03001f34);
    void *window;
    void *message;
    s32 firstFrame = 1;
    s32 byteSum = 0;
    s32 count;
    s32 index;
    s32 result;

    for (index = 0; index < 0x100; index++)
        U8_AT(state, index) = 0xff;
    U32_AT(state, 0x100) = 1;

    window = Func_080162d4(
        alternate == 0 ? 20 : 22,
        17,
        alternate == 0 ? 10 : 8,
        3,
        6);
    U32_AT(state, 0x1ac) = (u32)window;
    S32_AT(state, 0x1c4) = -1;

    if (alternate == 0) {
        S32_AT(state, 0x1c8) = 14;
        S32_AT(state, 0x1cc) = 4;
        S32_AT(state, 0x1d0) = 7;
        U32_AT(state, 0x1d4) = (u32)window;
        S32_AT(state, 0x1c0) = S32_AT(session, 60);
    } else {
        u8 bytes[4];
        s32 next = 2;

        S32_AT(state, 0x1c0) = S32_AT(session, 64);
        S32_AT(state, 0x1c8) = 0;
        S32_AT(state, 0x1cc) = 1;
        if (S32_AT(Func_08077000(0), 0) != 0) {
            Func_080b5130(0, bytes);
            byteSum = bytes[0] + bytes[1] + bytes[2] + bytes[3];
            S32_AT(state, 0x1c8 + next++ * 4) = 15;
            if (byteSum != 0)
                S32_AT(state, 0x1c8 + next++ * 4) = 16;
        }
        S32_AT(state, 0x1c8 + next++ * 4) = 2;
        S32_AT(state, 0x1c8 + next++ * 4) = 3;
        S32_AT(state, 0x1c8 + next * 4) = -1;
    }

    U16_AT(state, 0x110) = (u16)(u32)tilemap;
    count = 0;
    while (count <= 5 && S32_AT(state, 0x1c8 + count * 4) != -1) {
        Func_08021d88(state, count);
        count++;
        U16_AT(state, 0x12c + (count - 1) * 28) = (u16)(u32)tilemap;
    }
    S32_AT(state, 0x1b0) = count;
    U16_AT(state, 0x1b4) = 320;
    U16_AT(state, 0x1b6) = 304;
    U16_AT(state, 0x1c0) = 0;

    for (index = 0; index < count; index++) {
        S32_AT(state, 0x114 + index * 28) =
            (index - count) * 24 + (alternate ? 171 : 155);
        S32_AT(state, 0x118 + index * 28) = 136;
    }

    Func_080041d8((void (*)(void))0x08021dfd, 0x480);
    Func_0800307c(2, 136, (const void *)0x08021dfd);

    for (;;) {
        s32 wave = ((const u16 *)0x080366f8)
            [(*(s32 *)0x03001e40 * 2) & 31];
        s32 coordinate = (wave - 256) / 4 + 304;
        s32 selected = S32_AT(state, 0x1c0);
        s32 pressed;
        s32 repeated;

        U16_AT(state, 0x1b4) = (u16)coordinate;
        U16_AT(state, 0x1b6) = (u16)coordinate;
        REG16(0x03001ad4) = 0x0ea6;
        REG16(0x03001ad6) = 32;

        if (firstFrame != 0) {
            firstFrame = 0;
            U8_AT(world, 0x0ea6) = 1;
            FirstFrame_08021e6c(world, count, alternate != 0);
        }

        if (S32_AT(state, 0x1c4) != selected) {
            Func_08016478(window);
            Func_0801e7c0(
                S32_AT(state, 0x11c + selected * 28) - 0x200,
                window, 0, 0);
            S32_AT(state, 0x1c4) = selected;
            Func_08021d88(state, S32_AT(state, 0x1c8 + selected * 4));
        }

        S32_AT(state, 0x1bc) = Func_08003d28((u8 *)state + 0x1b4);
        for (index = 0; index < count; index++) {
            u8 *object = (u8 *)state + 0x104 + index * 28;
            if (index == selected) {
                U8_AT(object, 7) =
                    (U8_AT(object, 7) & 0xc0) |
                    ((U8_AT(state, 0x1bc) & 31) << 1);
                U8_AT(object, 5) |= 3;
                U16_AT(object, 6) =
                    (U16_AT(object, 6) & 0xfe00) |
                    ((S32_AT(object, 16) +
                    ((coordinate * 7) / 512) - 14) & 0x1ff);
                U8_AT(object, 4) = (u8)(S32_AT(object, 20) +
                    ((coordinate * 3) / 256) - 20);
                Func_08003dec(object, 241);
            } else {
                U16_AT(object, 6) =
                    (U16_AT(object, 6) & 0xfe00) |
                    (S32_AT(object, 16) & 0x1ff);
                U8_AT(object, 4) = (u8)S32_AT(object, 20);
                U8_AT(object, 7) &= 0xc0;
                U8_AT(object, 5) &= 0xfc;
            }
        }

        {
            u8 *control = session;
            s32 *phase = (s32 *)(control + 216);
            s32 *timer = (s32 *)(control + 220);
            s32 mode = S32_AT(control, 224);

            pressed = *(s32 *)0x03001c94;
            repeated = *(s32 *)0x03001b04;
            if (*phase != 0) {
                if (*timer == 0) {
                    if (mode == 1) {
                        if (S32_AT(state, 0x1c8 + selected * 4) == 3) {
                            pressed = repeated = 1;
                        } else {
                            pressed = repeated = 32;
                        }
                        *timer = 30;
                    } else if (mode == 0) {
                        s32 choice = S32_AT(state, 0x1c8 + selected * 4);
                        if (choice == 16 ||
                            (byteSum == 0 && choice == 15)) {
                            if (*phase == 1) {
                                message = Func_08021e48(
                                    choice == 15 ? 0x0c4a : 0x0c49,
                                    15,
                                    8);
                                Func_080b5128(102, 155);
                                Func_08016418(message, 1);
                                (*phase)++;
                                *timer = 45;
                            } else {
                                pressed = repeated = 1;
                                *timer = 200;
                            }
                        } else {
                            pressed = repeated = 16;
                            *timer = 40;
                        }
                    } else {
                        *timer = 60;
                        pressed = repeated = 1;
                    }
                } else {
                    (*timer)--;
                }
            }
        }

        if ((pressed & 0x300) != 0 && alternate != 0) {
            result = -2;
            break;
        }
        if ((pressed & 1) != 0) {
            result = S32_AT(state, 0x1c8 + selected * 4);
            break;
        }
        if (alternate != 0 && (pressed & 2) != 0) {
            Func_080f9010(113);
            result = -1;
            break;
        }
        if ((repeated & 0x90) != 0) {
            Func_080f9010(111);
            S32_AT(state, 0x1c0) = Func_080022fc(selected + 1, count);
        } else if ((repeated & 0x60) != 0) {
            Func_080f9010(111);
            S32_AT(state, 0x1c0) =
                Func_080022fc(selected + count - 1, count);
        } else if (S32_AT(session, 76) == 0) {
            result = selected;
            break;
        } else {
            continue;
        }

        Func_0800387c((volatile void *)0x04000000, 0x1741);
        U8_AT(world, 0x0ea6) = 0;
        Func_080030f8(1);
    }

    if (alternate != 0)
        S32_AT(session, 64) = S32_AT(state, 0x1c0);
    else
        S32_AT(session, 60) = S32_AT(state, 0x1c0);

    for (index = 0; index < count; index++)
        Func_08003f3c(U16_AT(state, 0x110 + index * 28));
    U8_AT(world, 0x0ea6) = 1;
    Func_08016418(window, 1);

    for (index = 0; index < 7; index++) {
        s32 row;
        s32 column;
        for (row = 0; row < 3; row++)
            for (column = 0; column < 3; column++)
                *(u16 *)((u8 *)world + (index * 3 + row * 32 +
                    (column & 3)) * 2 + 0x44a) = 0xf07f;
    }
    U8_AT(world, 0x0ea6) = 1;
    Func_080030f8(0);
    Func_08004278((const void *)0x08021dfd);
    Func_0800307c(2, 0, 0);

    {
        volatile u16 *dmaControl = (volatile u16 *)0x04000208;
        u16 saved = *dmaControl;
        *dmaControl = (u16)(u32)dmaControl;
        if (REG16(0x02002090) <= 31) {
            s32 slot = REG16(0x02002090)++;
            u32 *entry = (u32 *)(0x02002094 + slot * 12);
            entry[0] = 0x00001541;
            entry[1] = 0x04000000;
            entry[2] = 0x00020000;
        }
        *dmaControl = saved;
    }

    Func_08002df0(state);
    U8_AT(world, 0x0ea3) = 0;
    Func_080030f8(1);
    return (u32)result;
}
