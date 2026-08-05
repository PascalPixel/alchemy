#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))

#define REG_DISPCNT  (*(volatile u16 *)0x04000000)
#define REG_BG2CNT   (*(volatile u16 *)0x0400000c)
#define REG_BLDCNT   (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)
#define REG_DMA3SAD  (*(volatile u32 *)0x040000d4)
#define REG_DMA3DAD  (*(volatile u32 *)0x040000d8)
#define REG_DMA3CNT  (*(volatile u32 *)0x040000dc)
#define REG_IME      (*(volatile u16 *)0x04000208)

struct IntroState_080f26ec {
    s32 tile_step;
    s32 map_step;
    s32 frame;
    s32 phase;
    s32 transition;
};

void *Func_080048b0(s32, s32);
void Func_08004760(void);
void Func_0800403c(void);
void Func_080030f8(s32);
void Func_080040e8(void);
void Func_080f24a0(void);
void Func_080f377c(void);
void Func_080f3824(s32, s32);
void Func_080f3858(s32);
void Func_080041d8(const void *, s32);
s32 Func_080022fc(s32, s32);
void Func_08003bf8(s32);
void Func_08004278(const void *);
void *Func_08002f40(s32);
void Func_080053e8(const void *, void *);
void Func_0800479c(void);
void *Func_080048f4(s32, s32);
s32 Func_08004080(void);
s32 Func_08003fa4(s32, s32, s32);
void Func_08002dd8(s32);
void Func_08003c3c(s32);
void Func_08003ce0(void);
void Func_08003dec(void *, s32);
s32 Func_08002304(s32, s32);

static void QueueDma_080f26ec(const void *source, void *destination, u32 control)
{
    REG_DMA3SAD = (u32)source;
    REG_DMA3DAD = (u32)destination;
    REG_DMA3CNT = control;
}

s32 Func_080f26ec(s32 long_version)
{
    struct IntroState_080f26ec *state;
    u8 saved_scene;
    s32 skipped = 0;
    u16 saved_ime;
    u16 *display_state = (u16 *)0x03001ad0;

    saved_scene = *(u8 *)0x03001f58;
    state = Func_080048b0(0x2b, 0xe0);
    Func_08004760();
    Func_0800403c();
    Func_080030f8(1);
    Func_080040e8();
    *(u8 *)0x03001d18 = 0;
    *(u8 *)0x03001f58 = 0;
    Func_080f24a0();
    Func_080f377c();
    Func_080f3824(2, 0);

    saved_ime = REG_IME;
    REG_IME = 0;
    if (*(u16 *)0x02002090 <= 0x1f) {
        u16 slot = (*(u16 *)0x02002090)++;
        u32 *request = (u32 *)(0x02002094 + slot * 12);
        request[0] = 0x0000f740;
        request[1] = 0x04000000;
        request[2] = 0x00020000;
    }
    REG_IME = saved_ime;

    Func_080f3858(0x3c);
    Func_080041d8((const void *)0x080f2029, 0x480);

    for (;;) {
        s32 frame = state->frame;

        if ((u32)(frame - 0x15) <= 0xd9 &&
            (*(u32 *)0x03001c94 & 9)) {
            state->transition = 1;
            state->frame = 0xef;
            frame = 0xef;
        }

        frame++;
        state->frame = frame;
        if (frame > 0x116) {
            if (frame == 0x119)
                state->transition = 2;
            else if (frame == 0x121)
                state->transition = 0;
            else if (frame == 0x118)
                break;
            goto wait_frame;
        }

        {
            s32 phase = state->phase;
            if (Func_080022fc(phase, 3) == 0) {
                display_state[3]--;
                if (!(display_state[3] & 7)) {
                    s32 offset = state->tile_step * 0x3c0;
                    QueueDma_080f26ec(
                        (const void *)(0x02012580 - offset),
                        (void *)(0x06004b00 - offset), 0x800001e0);
                    state->tile_step++;
                    phase = state->phase;
                }
            }

            if (!(phase & 1)) {
                display_state[5]--;
                if (!(display_state[5] & 7)) {
                    s32 width = state->map_step * 8;

                    if (width <= 0x18) {
                        s32 offset = state->map_step * 0x780;
                        QueueDma_080f26ec(
                            (const void *)(0x020199c0 - offset),
                            (void *)(0x0600e4c0 - offset), 0x800003c0);
                    } else {
                        s16 blank = 0;
                        s16 *tilemap;
                        u32 row;
                        s32 destination =
                            Func_080022fc(0xa0 - width, 0xa0) * 0xf0 +
                            0x06004ec0;

                        QueueDma_080f26ec(
                            &blank, (void *)destination, 0x810003c0);
                        tilemap = (s16 *)0x0600f6c0;
                        for (row = 0; row < 5; row++) {
                            u32 column;
                            for (column = 0; column < 32; column++)
                                *tilemap++ = 0x013b;
                        }
                    }
                    state->map_step++;
                }
            }
        }

wait_frame:
        if (state->frame == 0xef)
            state->transition = 1;
        Func_080030f8(1);
    }

    {
        void *palette;
        s16 *tilemap;
        s16 tile = 0x100;
        u32 row;

        Func_08003bf8(1);
        Func_08004278((const void *)0x080f2029);
        *(u8 *)0x03001d18 = 1;
        Func_080030f8(1);
        REG_BG2CNT = 0x0681;
        REG_DISPCNT = 0x1440;
        display_state[5] = 0;

        palette = Func_08002f40(0x16);
        QueueDma_080f26ec(palette, (void *)0x05000000, 0x84000078);
        *(u16 *)0x05000000 = 0;
        Func_080053e8((u8 *)palette + 0x200, (void *)0x02010000);
        QueueDma_080f26ec(
            (const void *)0x02010000, (void *)0x06004000, 0x80004b00);

        tilemap = (s16 *)0x06003000;
        for (row = 0; row < 20; row++) {
            u32 column;
            for (column = 0; column < 30; column++)
                *tilemap++ = tile++;
            *tilemap++ = 0x01ff;
            *tilemap++ = 0x01ff;
        }

        Func_0800479c();
        Func_08004760();

        if (long_version) {
            u8 *tiles = Func_080048f4(0x0e, 0x400);
            u8 *objects = (u8 *)state + 0x80;
            u32 i;

            Func_080053e8((const void *)0x080f38bc, tiles);
            for (i = 0; i < 5; i++) {
                S32_AT(objects, 0) = 0;
                S32_AT(objects, 4) = 0x40004000;
                S32_AT(objects, 8) = Func_08003fa4(
                    Func_08004080(), 0x80,
                    (s32)(tiles + i * 0x80));
                objects += 12;
            }
            Func_08002dd8(0x0e);
        }

        Func_08003c3c(0x1e);
        Func_08003ce0();
        REG_DISPCNT = 0x1540;

        {
            u32 duration = long_version ? 0xe10 : 0x12c;
            u32 frame_index = 0;

            while (frame_index < duration) {
                if (long_version) {
                    u8 *object = (u8 *)state + 0x80;
                    s32 x = 0x50;
                    u32 i;

                    for (i = 0; i < 3; i++) {
                        U16_AT(object, 6) =
                            (U16_AT(object, 6) & 0xfe00) | (x & 0x1ff);
                        S8_AT(object, 4) = 0x7c;
                        Func_08003dec(object, 0);
                        object += 12;
                        x += 0x20;
                    }

                    {
                        u8 blend = U8_AT(
                            (void *)0x080f39b1,
                            Func_08002304(frame_index, 0x3c));
                        REG_BLDCNT = 0x2f50;
                        REG_BLDALPHA = ((0x10 - blend) << 8) | blend;
                    }
                }

                if (!(*(u32 *)0x03001c94 & 9)) {
                    frame_index++;
                    Func_080030f8(1);
                } else {
                    skipped = 1;
                    break;
                }
            }
        }
    }

    *(u8 *)0x03001f58 = saved_scene;
    Func_08002dd8(0x2b);
    REG_BLDCNT = 0;
    REG_BLDALPHA = 0;
    Func_080030f8(1);
    return skipped;
}
