#include "types.h"

struct DmaChannel_080f7460 {
    const void *source;
    void *destination;
    u32 control;
};

struct EffectParticle_080f7460 {
    s32 x;
    s32 y;
    s32 unused_08;
    s32 velocity_x;
    s32 velocity_y;
    s32 unused_14;
    s32 timer;
};

struct MenuRecord_080f7460 {
    s32 enabled;
    u8 choices[21];
    u8 selection;
    u8 cancel_selection;
    u8 padding;
};

struct EffectState_080f7460 {
    struct MenuRecord_080f7460 menu[5];
    s32 mode;
    s32 field_90;
    s32 field_94;
    s32 option_count;
    u8 padding_a0[2];
    u16 field_a2;
    u8 padding_a4[4];
    s32 fade_counter;
    s32 enabled_icons[7];
    u8 padding_c8[0x400];
    s32 message_window;
};

/*
 * __call_via_rN veneer sites, resolved per-site against the ROM. Three `bl`
 * sites land inside the 0x080072e4 bank; each is an indirect call through the
 * named register, not a call to a function at the branch target.
 *
 * 0x080f78de -- `bl 0x080072f0` = __call_via_r3, r3 from pool 0x080f7964 =
 * 0x03000168, the relocated IWRAM fill (destination, size, value). r0 =
 * [sp, #40] (the canvas), r1 = 0x8000, r2 = 0 -- all three set deliberately.
 * r3 is an argument register, so the draft's fourth argument WAS the callee.
 *
 * 0x080f75c6 -- `bl 0x080072f0` = __call_via_r3, r3 from pool 0x080f7614 =
 * 0x03000164. TWO arguments, not three. Only r0 (pool 0x080f7618) and r1
 * (0x300) are set for this call; r2's last write on the path is
 * `ldr r2, [pc, #100]` at 0x080f75a0, the DMA control word 0x84000008 used by
 * the loop just above. The draft's third argument, a literal 0, is not in the
 * assembly at all -- r2 is a live leftover, and a leftover with a value that
 * cannot be mistaken for an argument. This is the second site to show the
 * two-argument form of 0x03000164, after 0x080bd87e in
 * semantic/main/080bd850.c, where nothing in the whole function writes r2.
 * 0x03000164 itself remains UNESTABLISHED -- an exact-lane question -- and
 * nothing here settles what it does.
 *
 * A TWO-ENTRY RENDERER TABLE LIVES IN THIS FRAME, the same idiom as
 * semantic/main/080db264.c. r5 = pool 0x080f7960 = 0x03001e50, the
 * Func_080048b0 slot table; +184 (slot 46) goes to [sp, #48] at 0x080f78ba
 * and +188 (slot 47) goes to [sp, #52] at 0x080f78d4 via `str r3, [r7, #4]`
 * with r7 = sp + 48. The base address sp + 48 is parked in [sp, #12] at
 * 0x080f78d0. Each load follows its own `Func_080c9000(46/47, ...)` publish.
 *
 * 0x080f7b56 -- `bl 0x080072f4` = __call_via_r4, and r4 is
 * `ldr r4, [r2, r0]` at 0x080f7b4c with r0 = [sp, #12] (the table base) and
 * r2 = (r8 & 1) << 2, r8 being the particle index. The draft READ that index
 * correctly and even named the two renderers -- it simply passed the chosen
 * one as a seventh ARGUMENT instead of calling through it.
 *
 * ARITY of the renderer: SIX, not seven. r0..r3 are set at 0x080f7b50 /
 * 0x080f7b4e / 0x080f7b54 / 0x080f7b52 and two more words go out at [sp, #0]
 * and [sp, #4] (0x080f7b44, 0x080f7b48). r4 is above the argument registers,
 * so no argument slot holds the callee.
 *
 * UNCERTAINTY, left standing: what slots 46 and 47 CONTAIN is not settled
 * here. The slot table unifies the addressing, never the contents.
 */
typedef void (*Renderer_080f7460)(
    void *target, const void *source, s32 x, s32 y, s32 width, s32 height);
typedef void (*ArmFill_080f7460)(void *destination, u32 size, u32 value);
/* Unestablished IWRAM routine; see the header. Two arguments here. */
typedef void (*Iwram_03000164_080f7460)(void *destination, u32 size);

void *Func_080048b0(s32 resource, s32 size);
void *Func_080048f4(s32 resource, s32 size);
void Func_08002f3c(void);
void Func_080040e8(void);
const void *Func_08002f40(s32 resource);
void Func_08002dd8(s32 resource);
void Func_080030f8(s32 frames);
void Func_080041d8(const void *source, u32 size);
void Func_08004278(const void *source);
u32 Func_08004458(void);
s32 Func_080022ec(s32 value, s32 shift);
s32 Func_080022fc(s32 value, s32 divisor);
s32 Func_08002304(s32 value, s32 divisor);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_08005340(const void *source, void *destination);
void Func_08015000(void);
s32 Func_08015010(s32 kind, s32 x, s32 y, s32 style, s32 layer);
void Func_08015080(s32 message, s32 window, s32 x, s32 flags);
s32 Func_080772e0(s32 id);
void Func_080c9000(s32 resource, s32 x, s32 y, s32 style, s32 layer);
void Func_080f6038(u16 *source, u16 *destination, s32 scale, s32 count);
void Func_080f6148(void);
void Func_080f61e8(s32 resource);
void Func_080f62b8(s32 x0, s32 y0, s32 x1, s32 y1, s32 colour);
void Func_080f731c(void);

/*
 * Build and run the complete status/menu presentation.
 *
 * Owner scope: the bounded 0x080f7460-0x080f7db4 region (2,388 bytes).
 * The routine constructs both background maps and palettes, randomises five
 * menu records, creates the initial windows, then owns the complete animated
 * frame loop and its teardown.
 */
void Func_080f7460(void)
{
    volatile struct DmaChannel_080f7460 *dma =
        (volatile struct DmaChannel_080f7460 *)0x040000d4;
    volatile u16 *blend_control = (volatile u16 *)0x04000050;
    volatile u16 *blend_alpha = (volatile u16 *)0x04000052;
    volatile u16 *bg_scroll = (volatile u16 *)0x04000014;
    volatile u16 *display_control = (volatile u16 *)0x04000000;
    u16 *map = (u16 *)0x06002800;
    u16 *secondary_map = (u16 *)0x06003000;
    u16 *palette_work;
    u16 *palette_source;
    u8 *tiles = Func_080048b0(41, 0x60e);
    u8 *canvas = Func_080048b0(40, 0x8000);
    u8 *workspace = Func_080048f4(39, 0x782c);
    struct EffectState_080f7460 *state =
        Func_080048f4(45, 0x61c);
    u32 unique_values[8];
    Renderer_080f7460 renderers[2];
    s32 frame;
    s32 row;
    s32 column;
    s32 index;
    s32 next_tile;

    Func_08002f3c();
    *(u8 *)0x0200036c = 0xff;
    state->field_a2 = 0;
    state->option_count = 1;
    Func_080040e8();
    *(u8 *)0x03001d18 = 0;

    next_tile = 0;
    for (row = 0; row < 20; row++) {
        for (column = 0; column < 32; column++) {
            if (column >= 5 && column <= 24 && row >= 3 && row <= 13)
                map[row * 32 + column] = 0xa1a6;
            else if (column > 29)
                map[row * 32 + column] = 0;
            else
                map[row * 32 + column] = 0xa1a8 + next_tile++;
        }
    }

    palette_source = (u16 *)Func_08002f40(0x76);
    Func_08005340(palette_source, tiles);
    palette_source = (u16 *)Func_08002f40(0x3f);
    dma->source = palette_source;
    dma->destination = (void *)0x05000140;
    dma->control = 0x84000008;
    Func_08005340((u8 *)palette_source + 32, (void *)0x0600b500);

    index = 0;
    for (row = 0; row < 20; row++) {
        u8 *source = (u8 *)0x02010000 + row * 15 * 64;
        for (column = 0; column < 30; column++, source += 32) {
            if (!(column >= 5 && column <= 24 && row >= 3 && row <= 13)) {
                dma->source = source;
                dma->destination = (void *)(0x06002d00 + index * 32);
                dma->control = 0x84000008;
                index++;
            }
        }
    }
    ((Iwram_03000164_080f7460)0x03000164)(
        (void *)0x06002d00, 0x300);

    for (row = 0; row < 20; row++) {
        for (column = 0; column < 32; column++) {
            if ((u32)(row - 2) <= 14)
                secondary_map[row * 32 + column] =
                    (u16)(row * 32 + column + 148);
            else
                secondary_map[row * 32 + column] = 0x509;
        }
    }

    *(volatile u16 *)0x0400000a = 0x509;
    *(volatile u16 *)0x0400000c = 0x680;
    {
        volatile u16 *matrix = (volatile u16 *)0x03001ad0;
        matrix[0] = 0;
        matrix[1] = 0;
        matrix[2] = 0;
        matrix[3] = 0;
        matrix[4] = 0;
        matrix[5] = 0;
    }
    *(volatile u16 *)0x04000048 = 0x3737;
    *(volatile u16 *)0x0400004a = 0x2727;
    *blend_control = 0x3f44;
    *blend_alpha = 0x1010;
    bg_scroll[0] = 0;
    bg_scroll[2] = 0;
    bg_scroll[1] = 0xff60;
    bg_scroll[3] = 0xff60;
    *(volatile u16 *)0x04000040 = 0x28c8;
    *(volatile u16 *)0x04000044 = 0x1878;
    *(volatile u16 *)0x04000040 = 0xf0;
    *(volatile u16 *)0x04000044 = 0xa0;
    *(volatile u16 *)0x04000042 = 0xf0;
    *(volatile u16 *)0x04000046 = 0xa0;

    state->mode = 0;
    state->field_90 = 0;
    state->field_94 = 0;
    *(s32 *)(workspace + 0x778c) = 0;
    state->fade_counter = 0;
    palette_work = (u16 *)(workspace + 0x200);

    palette_source = (u16 *)Func_08002f40(0x8f);
    dma->source = palette_source;
    dma->destination = (void *)0x05000080;
    dma->control = 0x84000020;
    ((volatile u16 *)0x05000080)[0] = 0x2f8b;
    ((volatile u16 *)0x05000080)[1] = 0x5bf6;

    palette_source = (u16 *)Func_08002f40(0x40);
    dma->source = palette_source;
    dma->destination = (void *)0x05000200;
    dma->control = 0x84000078;
    Func_08005340(
        (u8 *)palette_source + 480,
        (void *)0x06010000);
    dma->source = (const void *)0x02010000;
    dma->destination = (void *)0x06010000;
    dma->control = 0x84001b30;

    palette_source = (u16 *)Func_08002f40(0x41);
    dma->source = palette_source;
    dma->destination = (void *)0x050003e0;
    dma->control = 0x84000008;
    Func_08005340((u8 *)palette_source + 32, (void *)0x06016e00);
    dma->source = (const void *)0x02010000;
    dma->destination = (void *)0x06010000;
    dma->control = 0x84000480;

    Func_08015000();
    Func_080f731c();

    for (row = 0; row < 5; row++) {
        struct MenuRecord_080f7460 *record = &state->menu[row];

        record->enabled = 8;
        record->selection = 0;
        record->cancel_selection = 0xff;
        for (column = 0; column < 21; column++)
            record->choices[column] =
                (u8)Func_08002304((s32)Func_08004458(), 5);
    }

    for (row = 0; row < 5; row++) {
        struct MenuRecord_080f7460 *record = &state->menu[row];

        for (column = 0; column < 8; column++) {
            s32 value;
            s32 prior;

            do {
                value = Func_08002304((s32)Func_08004458(), 21);
                for (prior = 0; prior < column; prior++) {
                    if (unique_values[prior] == (u32)value)
                        break;
                }
            } while (prior != column);
            unique_values[column] = (u32)value;
        }
        for (column = 0; column < 8; column++) {
            s32 rank = column > 5 ? 5 : column;
            record->choices[unique_values[column] + 4] = (u8)rank;
        }
    }

    Func_080c9000(46, 8, 7, 3, 2);
    renderers[0] = *(Renderer_080f7460 *)0x03001f08;
    Func_080c9000(47, 8, 7, 3, 3);
    renderers[1] = *(Renderer_080f7460 *)0x03001f0c;

    ((ArmFill_080f7460)0x03000168)(canvas, 0x8000, 0);
    dma->source = canvas;
    dma->destination = (void *)0x06003500;
    dma->control = 0x84002000;
    dma->source = (const void *)0x05000000;
    dma->destination = workspace;
    dma->control = 0x84000080;
    dma->source = palette_work;
    dma->destination = (void *)0x05000200;
    dma->control = 0x84000080;

    Func_080f6038(palette_work, (u16 *)0x05000200, 0, 256);
    Func_080f6038(
        (u16 *)workspace,
        (u16 *)0x05000000,
        0,
        256);
    *display_control = 0x909;

    if (Func_080772e0(228) == 1) {
        state->message_window = Func_08015010(6, 16, 18, 3, 6);
        Func_08015080(0x909, state->message_window, 0, 0);
    } else {
        state->message_window = Func_08015010(2, 16, 26, 4, 6);
        Func_08015080(0x908, state->message_window, 0, 0);
        Func_08015080(0x909, state->message_window, 0, 8);
    }

    *(s32 *)(workspace + 0x7824) = 0;
    Func_080041d8((const void *)0x080f6441, 0x480);
    Func_080041d8((const void *)0x080f60a1, 0x480);

    frame = 0;
    while (state->mode != 10) {
        if (frame <= 16) {
            s32 scale = frame << 12;
            Func_080f6038(
                palette_work, (u16 *)0x05000200, scale, 256);
            Func_080f6038(
                (u16 *)workspace,
                (u16 *)0x05000000,
                scale,
                256);
        }

        if (state->mode == 3) {
            s32 phase = Func_080022fc(frame, 80);
            if (phase <= 15)
                Func_080f61e8(0x91);
            else if (phase <= 31)
                Func_080f61e8(0x93);
            else if (phase <= 47)
                Func_080f61e8(0xb4);
            else if (phase <= 63)
                Func_080f61e8(0xa0);
            else
                Func_080f61e8(0x8f);
            if (state->fade_counter <= 15)
                Func_080f6148();

            if (state->fade_counter > 16 && (frame & 7) == 0) {
                s32 group = ((frame < 0 ? frame + 7 : frame) >> 3) & 3;
                s32 common_x =
                    ((Func_08004458() & 127) + 56) << 16;
                s32 common_y =
                    ((Func_08004458() & 31) + 48) << 16;
                struct EffectParticle_080f7460 *particle =
                    (struct EffectParticle_080f7460 *)
                    (0x02010000 + group * 7 * 1024);

                for (index = 0; index < 256; index++, particle++) {
                    s32 speed = (Func_08004458() & 255) + 64;
                    s32 angle = Func_08004458() & 0xffff;

                    particle->x = common_x;
                    particle->y = common_y;
                    particle->velocity_x =
                        speed * Func_08002322(angle) >> 6;
                    particle->velocity_y =
                        -(speed * Func_0800231c(angle)) >> 6;
                    particle->timer =
                        (Func_08004458() & 15) + 16;
                }
            }

            {
                struct EffectParticle_080f7460 *particle =
                    (struct EffectParticle_080f7460 *)0x02010000;

                for (index = 0; index < 1024; index++, particle++) {
                    if (particle->timer > 0) {
                        s32 size;

                        particle->timer--;
                        if ((u32)particle->x <= 0x00ffffff &&
                            particle->y >= 0 &&
                            particle->y <= 0x007fffff) {
                            size =
                                Func_080022ec(particle->timer, 12) + 1;
                            renderers[index & 1](
                                canvas,
                                tiles +
                                    ((const u16 *)0x080f86f8)[size - 1],
                                (particle->x >> 16) - size,
                                (particle->y >> 16) - size,
                                size * 2,
                                size * 2);
                        }
                        particle->x += particle->velocity_x;
                        particle->y += particle->velocity_y;
                        particle->velocity_x =
                            particle->velocity_x * 60 / 64;
                        particle->velocity_y =
                            particle->velocity_y * 60 / 64;
                    }
                }
            }
        }

        if (state->mode == 0 || state->mode == 2) {
            s32 icons[7] = { 0, 0, 0, 0, 0, 0, 0 };

            if (state->mode == 0) {
                icons[3] = 1;
                if (state->option_count > 1)
                    icons[2] = icons[4] = 1;
                if (state->option_count > 2)
                    icons[1] = icons[5] = 1;
                if (state->option_count > 3)
                    icons[0] = icons[6] = 1;
            } else if ((frame & 7) <= 3) {
                for (index = 0; index < 7; index++)
                    icons[index] = state->enabled_icons[index];
            }

            for (index = 0; index < 3; index++) {
                s32 colour = 65 - (index != 1);

                if (icons[1])
                    Func_080f62b8(20, index + 19, 200, 65, colour);
                if (icons[2])
                    Func_080f62b8(28, index + 35, 200, 65, colour);
                if (icons[3])
                    Func_080f62b8(20, index + 51, 200, 65, colour);
                if (icons[4])
                    Func_080f62b8(28, index + 67, 200, 65, colour);
                if (icons[5])
                    Func_080f62b8(20, index + 83, 200, 65, colour);
                if (icons[0])
                    Func_080f62b8(
                        28, index + 5, 200, index + 91, colour);
                if (icons[6])
                    Func_080f62b8(
                        28, index + 97, 200, index + 11, colour);
            }
        }

        *(s32 *)(workspace + 0x7824) = 1;
        Func_080030f8(1);
        frame++;
    }

    for (frame = 0; frame < 17; frame++) {
        s32 scale = 0x10000 - (frame << 12);

        Func_080f6038(
            palette_work, (u16 *)0x05000200, scale, 256);
        Func_080f6038(
            (u16 *)workspace,
            (u16 *)0x05000000,
            scale,
            256);
        Func_080030f8(1);
    }

    Func_08002dd8(47);
    Func_08002dd8(46);
    Func_08004278((const void *)0x080f60a1);
    Func_08004278((const void *)0x080f6441);
    Func_08002dd8(45);
    Func_08002dd8(40);
    Func_08002dd8(39);
    Func_08002dd8(41);
}
