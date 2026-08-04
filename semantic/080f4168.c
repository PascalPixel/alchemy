#include "types.h"

/*
 * Func_080f4168 - the two-dice gambling minigame.
 *
 * Advertised as a 432-byte manifest row, the real owner is one 4,596-byte
 * function spread over eight rows (080f4168, 080f4318, 080f47bc, 080f4ab2,
 * 080f4af0, 080f4eca, 080f4f04) plus three embedded literal pools.  The live
 * 768-byte frame and the high registers r8-r11 are carried straight across
 * every row boundary, so the rows are one function and not eight.
 *
 * The row at 080f474a is halfword literal data; the rows at 080f4ab2 and
 * 080f4eca reconstruct as instructions but are, by dataflow, literal pools as
 * well (every word in them is the target of a pc-relative load from the
 * surrounding code and no instruction ever branches into them).
 *
 * Internal control edges that appear as `bl` in the disassembly (the linker
 * could not reach them with a short branch) are expressed here as ordinary C
 * flow:
 *   bl 080f44b8  - back edge of the per-frame loop
 *   bl 080f535e  - forward jump into the fade-out / teardown tail (x2)
 * The two `bl 080072f0` sites are `_call_via_r3` interworking thunks, i.e.
 * indirect calls through r3; both pass r3 = 0x030001d8, the relocated integer
 * square-root helper in IWRAM.
 */

/* ------------------------------------------------------------------ */
/* External services                                                    */
/* ------------------------------------------------------------------ */

void *Func_080048b0(s32 owner, s32 size);
void *Func_080048f4(s32 owner, s32 size);
void Func_08002dd8(s32 owner);
void Func_08002f3c(s32 resource);
const void *Func_08002f40(s32 resource);
void Func_080040e8(void);
void Func_080049ac(void);
void Func_080051d8(void *object, void *position);
void Func_08005268(const void *source, s32 *projected);
void Func_08005340(const void *source, void *destination);
void Func_080030f8(s32 frames);
u32 Func_08004458(void);              /* pseudo-random word */
s32 Func_080022ec(s32 numerator, s32 denominator);   /* signed divide */
s32 Func_080022fc(s32 numerator, s32 denominator);   /* signed modulo */
s32 Func_08002304(s32 value, s32 modulus);           /* unsigned modulo */
s32 Func_0800231c(s32 angle);                        /* cosine-like */
s32 Func_08002322(s32 angle);                        /* sine-like */
void Func_08015000(void);
void *Func_08015010(s32 x, s32 y, s32 width, s32 height, s32 layer);
void Func_08015018(void *window, s32 mode);
void Func_08015080(s32 message, void *window, s32 x, s32 y);
void Func_080150b0(s32 value, s32 digits, void *window, s32 x, s32 y);
void Func_08077230(s32 amount);       /* add/remove coins */
s32 Func_080772c8(void);              /* current bet unit */
void Func_080f9010(s32 sound);
void Func_080f4028(void);
void Func_080f4100(const u16 *source, u16 *destination, s32 level, s32 count);

/* The relocated IWRAM helper reached through the `_call_via_r3` thunk.  Both
   call sites pass a sum of squares and use the result as a magnitude, so this
   is the integer square root. */
typedef s32 (*Magnitude_080f4168)(s32 squared);
#define MAGNITUDE_080f4168(squared) (((Magnitude_080f4168)0x030001d8)(squared))

/* ------------------------------------------------------------------ */
/* Hardware and shared-memory access                                    */
/* ------------------------------------------------------------------ */

#define REG16_080F4168(address) (*(volatile u16 *)(address))

struct DmaChannel_080f4168 {
    const void *source;
    void *destination;
    u32 control;
};

#define DMA3_080F4168 ((volatile struct DmaChannel_080f4168 *)0x040000d4)

static void Dma3Copy_080f4168(const void *source, void *destination, u32 control)
{
    DMA3_080F4168->source = source;
    DMA3_080F4168->destination = destination;
    DMA3_080F4168->control = control;
}

/* Input latches maintained by the interrupt handler. */
#define KEYS_PRESSED_080F4168 (*(volatile u32 *)0x03001b04)
#define KEYS_HELD_080F4168    (*(volatile u32 *)0x03001ae8)

/* Wallet block; only the coin total at +0x10 is touched here. */
#define COINS_080F4168 (*(volatile u32 *)0x02000250)

/* Scroll shadow written by the shared display update. */
#define SCROLL_SHADOW_080F4168 ((volatile u16 *)0x03001ad0)

/* Fixed ROM tables just past the end of this owner. */
#define WIN_ICON_COUNT_080F4168 ((const u8 *)0x080f53fc)
#define WIN_ICON_TILE_080F4168  ((const u8 *)0x080f5400)
#define DIE_FACE_TILE_080F4168  ((const u8 *)0x080f5408)   /* 18 spin steps */
#define DEPTH_BAND_080F4168     ((const s16 *)0x080f541a)  /* 6 thresholds   */

/* ------------------------------------------------------------------ */
/* Owner-local layout                                                   */
/* ------------------------------------------------------------------ */

/*
 * One die.  Positions and velocities are Q16.16 world units; `spin` counts
 * 0..0x47ff, i.e. six faces of 0x0c00 each, three animation steps per face.
 */
struct Die_080f4168 {
    s32 x;
    s32 y;      /* height above the table; 0 is the table surface */
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 spin;
};

struct OamEntry_080f4168 {
    u32 attributes;
    u32 tile;
};

/*
 * Owner 45's allocation.  The first 200 bytes are used by the shared object
 * builder; this function owns the 128 sprite entries that are DMA'd to OAM and
 * the three text-window handles that follow them.
 */
struct SlotDisplay_080f4168 {
    u8 reserved[200];
    struct OamEntry_080f4168 entries[128];
    void *coin_window;      /* +1224 */
    void *payout_window;    /* +1228 */
    void *prompt_window;    /* +1232 */
};

#define DICE_080F4168(work) \
    ((struct Die_080f4168 *)((u8 *)(work) + 0x7080))

/* Signed halving, open-coded by the original as `(v + (v >>> 31)) >> 1`. */
static s32 Half_080f4168(s32 value)
{
    return (value + (s32)((u32)value >> 31)) >> 1;
}

/* Rounded arithmetic the original open-codes with `if (v < 0) v += 63; v >>= 6`. */
static s32 ScaleBy64_080f4168(s32 value)
{
    if (value < 0) {
        value += 63;
    }
    return value >> 6;
}

/* ------------------------------------------------------------------ */

void Func_080f4168(void)
{
    /* Resources.  The 41 allocation is only reserved, never referenced. */
    u16 *object_palette_source;      /* owner 40, 512 bytes  (sp+116) */
    u8 *work;                        /* owner 39, 0x782c     (sp+112) */
    struct SlotDisplay_080f4168 *display; /* owner 45, 1560  (sp+108) */
    s32 *camera;                     /* owner 12, 76         (sp+104) */

    struct Die_080f4168 *dice;
    s32 frame;                       /* sp+100 */
    s32 state;                       /* sp+96  */
    s32 bet;                         /* sp+92  */
    s32 phase;                       /* sp+88  */
    s32 win_kind;                    /* sp+84  */
    s32 payout;                      /* sp+80  */
    s32 payout_timer;                /* sp+76  */
    s32 payout_digits;               /* sp+72  */
    s32 sprite_index;                /* sp+68  */
    s32 settled;                     /* sp+56  */

    s32 target[2];                   /* sp+136 */
    s32 highlight[2];                /* sp+144 */
    s32 spin_rate[2];                /* sp+152 */
    s32 order[2];                    /* sp+128 */
    s32 result[4];                   /* sp+224 */
    s32 depth[4];                    /* sp+240 */
    s32 projected[3];                /* sp+200 */
    s32 shadow_source[3];            /* sp+212 */
    s16 affine[4];                   /* sp+160 */
    u16 bg_palette_source[256];      /* sp+256 */

    s32 i;
    s32 j;
    s32 k;
    s32 reset_dice;                  /* r8 */

    /* ---------------------------------------------------------------- */
    /* One-time setup                                                    */
    /* ---------------------------------------------------------------- */

    Func_080048b0(41, 0x60e);
    object_palette_source = (u16 *)Func_080048b0(40, 512);
    work = (u8 *)Func_080048f4(39, 0x782c);
    display = (struct SlotDisplay_080f4168 *)Func_080048f4(45, 1560);
    camera = (s32 *)Func_080048f4(12, 76);
    dice = DICE_080F4168(work);

    Func_08002f3c(12);
    Func_080f4028();
    Func_080040e8();
    *(volatile u8 *)0x03001d18 = 0;

    /* Background map: 20 rows of 32 tiles, tile = 30 * row + column. */
    {
        volatile u16 *map = (volatile u16 *)0x06003000;
        s32 cell = 0;

        for (j = 0; j < 20; j++) {
            for (i = 0; i < 32; i++) {
                map[cell] = (u16)(i + j * 30);
                cell++;
            }
        }
    }

    /* Object graphics and palettes. */
    {
        const u8 *source;

        source = (const u8 *)Func_08002f40(0x42);

        Dma3Copy_080f4168(source, (void *)0x05000000, 0x84000070);
        Dma3Copy_080f4168((const void *)0x05000000, bg_palette_source,
                          0x84000080);
        Func_08005340(source + 448, (void *)0x02010000);
        Dma3Copy_080f4168((const void *)0x02010000, (void *)0x06004000,
                          0x84002580);

        source = (const u8 *)Func_08002f40(0x43);
        Dma3Copy_080f4168(source, object_palette_source, 0x84000080);
        Dma3Copy_080f4168(source, (void *)0x05000200, 0x84000080);
        Func_08005340(source + 512, (void *)0x02010000);
        Dma3Copy_080f4168((const void *)0x02010000, (void *)0x06010000,
                          0x84001f00);
    }

    /* Start fully faded out. */
    Func_080f4100(object_palette_source, (u16 *)0x05000200, 0, 256);
    Func_080f4100(bg_palette_source, (u16 *)0x05000000, 0, 256);

    REG16_080F4168(0x0400000a) = 0x0686;   /* BG1CNT   */
    REG16_080F4168(0x04000048) = 0x3737;   /* WININ    */
    REG16_080F4168(0x0400004a) = 0x2723;   /* WINOUT   */
    REG16_080F4168(0x04000000) = 0x3340;   /* DISPCNT  */
    REG16_080F4168(0x04000050) = 0x3f44;   /* BLDCNT   */
    REG16_080F4168(0x04000052) = 0x0810;   /* BLDALPHA */

    SCROLL_SHADOW_080F4168[2] = 0;
    SCROLL_SHADOW_080F4168[3] = 0;

    REG16_080F4168(0x04000014) = 0;        /* BG1HOFS */
    REG16_080F4168(0x04000016) = 0xff60;   /* BG1VOFS */
    REG16_080F4168(0x04000018) = 0;        /* BG2HOFS */
    REG16_080F4168(0x0400001a) = 0xff60;   /* BG2VOFS */
    REG16_080F4168(0x04000040) = 0x00f0;   /* WIN0H   */
    REG16_080F4168(0x04000044) = 0x00a0;   /* WIN0V   */
    REG16_080F4168(0x04000042) = 0x00f0;   /* WIN1H   */
    REG16_080F4168(0x04000046) = 0x00a0;   /* WIN1V   */

    state = 0;
    phase = 0;
    win_kind = -1;
    payout = 0;
    payout_timer = 0;
    bet = 0;
    payout_digits = 0;
    settled = 0;
    sprite_index = 0;

    /* Random launch velocities; the positions set here are immediately
       overwritten by the fixed launch pose below. */
    for (i = 0; i < 2; i++) {
        dice[i].x = (s32)((Func_08004458() & 127) - 64) << 16;
        dice[i].y = (s32)(Func_08004458() & 31) << 16;
        dice[i].z = (s32)((Func_08004458() & 127) - 64) << 16;
        dice[i].vx = (s32)((Func_08004458() & 127) - 64) << 14;
        dice[i].vy = (s32)((Func_08004458() & 31) + 32) << 12;
        dice[i].vz = (s32)((Func_08004458() & 127) - 64) << 14;
        dice[i].spin = 0;
        spin_rate[i] = 300;
    }

    result[0] = 0;
    result[1] = 0;
    result[2] = 0;
    result[3] = 0;
    highlight[0] = 0;
    highlight[1] = 0;

    /* Two distinct target faces for this round. */
    target[0] = Func_08002304((s32)Func_08004458(), 6);
    do {
        target[1] = Func_08002304((s32)Func_08004458(), 6);
    } while (target[0] == target[1]);

    dice[0].x = 160 << 14;
    dice[0].y = 192 << 15;
    dice[0].z = (s32)0xff600000;
    dice[1].x = (s32)0xffd80000;
    dice[1].y = 192 << 15;
    dice[1].z = (s32)0xff600000;

    Func_08015000();

    display->coin_window = Func_08015010(18, 0, 12, 3, 6);
    Func_08015080(0x0906, display->coin_window, 48, 0);
    Func_080150b0((s32)COINS_080F4168, 6, display->coin_window, 0, 0);

    display->prompt_window = Func_08015010(22, 16, 8, 4, 6);
    Func_08015080(0x0910, display->prompt_window, 0, 0);
    Func_08015080(0x0911, display->prompt_window, 0, 8);

    frame = 0;

    /* ---------------------------------------------------------------- */
    /* Per-frame loop                                                    */
    /* ---------------------------------------------------------------- */

    for (;;) {
        reset_dice = 0;

        if (frame <= 16) {
            Func_080f4100(object_palette_source, (u16 *)0x05000200,
                          frame << 12, 256);
            Func_080f4100(bg_palette_source, (u16 *)0x05000000,
                          frame << 12, 256);
        }

        Func_080049ac();
        Func_080051d8(camera, camera + 3);

        /* Fixed backdrop sprites. */
        display->entries[0].attributes = 0xc0006000;
        display->entries[0].tile = 214 << 2;
        display->entries[1].attributes = 0x40102020;
        display->entries[1].tile = 230 << 2;
        display->entries[2].attributes = 0x40102030;
        display->entries[2].tile = 232 << 2;
        display->entries[3].attributes = 0x40042020;
        display->entries[3].tile = 234 << 2;
        display->entries[4].attributes = 0x40042030;
        display->entries[4].tile = 234 << 2;

        /* The two target faces the player is trying to roll. */
        for (i = 0; i < 2; i++) {
            display->entries[5 + i].attributes = (u32)(32 + 16 * i) | 0x40202000;
            display->entries[5 + i].tile = (u32)((target[i] << 3) + (236 << 2));
        }

        sprite_index = 7;

        if ((u32)(state - 2) <= 1) {
            /* Result readout: up to four face icons appear one by one. */
            s32 x_field = 224 << 14;

            for (i = 0; i < 4; i++) {
                if (phase == 5 * i) {
                    Func_080f9010(0x12f);
                }
                if (phase >= 5 * i) {
                    display->entries[sprite_index].attributes =
                        (u32)x_field | 0x80002040;
                    display->entries[sprite_index].tile =
                        (u32)((result[i] << 5) + 152);
                    sprite_index++;
                }
                x_field += 128 << 14;
            }

            if (phase > 19 && win_kind >= 0) {
                s32 count = WIN_ICON_COUNT_080F4168[win_kind];

                if (count != 0) {
                    s32 tile = (WIN_ICON_TILE_080F4168[win_kind] << 3) + 152;
                    s32 x = (158 << 2) - (count << 4);

                    for (i = 0; i < count; i++) {
                        display->entries[sprite_index].attributes =
                            (u32)((x & 0x1ff) << 16) | 96u | 0x80002000u;
                        display->entries[sprite_index].tile = (u32)tile;
                        sprite_index++;
                        tile += 32;
                        x += 32;
                    }
                }
            }
        }

        if (state == 2) {
            /* Paying out. */
            if (phase == 20 && payout > 0) {
                Func_08077230(payout);

                payout_digits = 1;
                if (payout > 9) {
                    payout_digits = 2;
                    if (payout > 99) {
                        payout_digits = 3;
                        if (payout > 999) {
                            payout_digits = 4;
                            if (payout > 9999) {
                                payout_digits = 5;
                            }
                        }
                    }
                }

                display->payout_window =
                    Func_08015010(9 - Half_080f4168(payout_digits), 16,
                                  payout_digits + 12, 3, 6);
                Func_08015080(0x0907, display->payout_window, 0, 0);
                Func_08015080(0x0906, display->payout_window,
                              (payout_digits << 3) + 48, 0);
                Func_080150b0(payout, payout_digits, display->payout_window,
                              48, 0);
            }

            if (payout_timer > 0) {
                payout_timer--;
            }

            if (phase > 19 && (KEYS_PRESSED_080F4168 & 1) != 0) {
                Func_080f9010(112);
                if (payout > 0) {
                    state = 3;
                    payout_timer = 0;
                } else {
                    state = 0;
                    display->prompt_window = Func_08015010(22, 16, 8, 4, 6);
                    Func_08015080(0x0910, display->prompt_window, 0, 0);
                    Func_08015080(0x0911, display->prompt_window, 0, 8);
                    reset_dice = 1;
                }
            }

            phase++;
        } else if (state == 3) {
            /* Closing the payout window. */
            if (payout_timer == 0) {
                payout = 0;
                Func_080150b0(0, payout_digits, display->payout_window, 40, 0);
                Func_080150b0((s32)COINS_080F4168, 6, display->coin_window,
                              0, 0);
            }
            if (payout_timer == 15) {
                state = 0;
                Func_08015018(display->payout_window, 1);
                display->prompt_window = Func_08015010(22, 16, 8, 4, 6);
                Func_08015080(0x0910, display->prompt_window, 0, 0);
                Func_08015080(0x0911, display->prompt_window, 0, 8);
                reset_dice = 1;
            }
            payout_timer++;
        } else if (state == 0 && frame > 17) {
            /* Idle: adjust the bet and wait for a throw. */
            (void)KEYS_PRESSED_080F4168;
            (void)KEYS_PRESSED_080F4168;

            if ((KEYS_HELD_080F4168 & 0x40) != 0) {
                dice[0].y += 128 << 11;
                dice[1].y += 128 << 11;
            }
            if ((KEYS_HELD_080F4168 & 0x80) != 0) {
                dice[0].y += (s32)0xfffc0000;
                dice[1].y += (s32)0xfffc0000;
            }

            for (i = 0; i < 2; i++) {
                if (dice[i].y <= 0x001fffff) {
                    dice[i].y = 128 << 14;
                }
                if (dice[i].y > (160 << 16)) {
                    dice[i].y = 160 << 16;
                }
            }

            if (COINS_080F4168 < (u32)(Func_080772c8() * 10)) {
                /* Cannot cover the minimum bet: leave the minigame. */
                Func_08015018(display->prompt_window, 1);
                goto teardown;
            }

            if ((KEYS_PRESSED_080F4168 & 2) != 0) {
                Func_080f9010(113);
                Func_08015018(display->prompt_window, 1);
                goto teardown;
            }

            if ((KEYS_PRESSED_080F4168 & 1) != 0) {
                Func_080f9010(302);
                Func_08015018(display->prompt_window, 1);
                state = 1;

                bet = Func_080772c8() * 10;
                if ((u32)bet > COINS_080F4168) {
                    bet = (s32)COINS_080F4168;
                }
                Func_08077230(-bet);
                Func_080150b0((s32)COINS_080F4168, 6, display->coin_window,
                              0, 0);

                for (i = 0; i < 2; i++) {
                    /* The original guards this pose with `i > 1`, which never
                       holds for a two-die loop; it is kept for fidelity. */
                    if (i > 1) {
                        dice[i].z = (s32)0xff4c0000;
                        dice[i].y = 192 << 15;
                    }
                    dice[i].vx = (s32)((Func_08004458() & 127) - 64) << 12;
                    dice[i].vy = (s32)((Func_08004458() & 15) + 48) << 13;
                    {
                        s32 lateral = (s32)(Func_08004458() & 63);

                        dice[i].vz = ((lateral + 140) << 12)
                                   + Func_080022ec(dice[i].y, 6);
                    }
                    dice[i].spin = Func_08002304((s32)Func_08004458(),
                                                 144 << 7);
                    spin_rate[i] = 300;
                }
            }
        }

        if (reset_dice == 1) {
            dice[0].x = 160 << 14;
            dice[0].y = 192 << 15;
            dice[0].z = (s32)0xff600000;
            dice[1].x = (s32)0xffd80000;
            dice[1].y = 192 << 15;
            dice[1].z = (s32)0xff600000;

            target[0] = Func_08002304((s32)Func_08004458(), 6);
            do {
                target[1] = Func_08002304((s32)Func_08004458(), 6);
            } while (target[0] == target[1]);
        }

        /* ------------------------------------------------------------ */
        /* Depth sort (insertion sort over two entries)                  */
        /* ------------------------------------------------------------ */
        {
            s32 position = 0;
            s32 key_z = dice[1].z;

            order[0] = 0;
            order[1] = 1;
            while (position >= 0 && dice[order[position]].z > key_z) {
                order[position + 1] = order[position];
                position--;
            }
            order[position + 1] = 1;
        }

        /* ------------------------------------------------------------ */
        /* Die sprites                                                   */
        /* ------------------------------------------------------------ */
        for (i = 0; i < 2; i++) {
            struct Die_080f4168 *die = &dice[order[i]];
            s32 distance;
            s32 radius;
            s32 screen_x;
            s32 screen_y;

            Func_08005268(die, projected);

            if (projected[2] <= 0x15d) {
                projected[2] = 350;
            }
            if (projected[2] > 0x28a) {
                projected[2] = 0x28a;
            }
            depth[order[i]] = projected[2] - 350;

            distance = depth[order[i]];
            distance = Half_080f4168(distance) + 128;
            radius = Func_080022ec(128 << 4, distance);

            screen_x = projected[0] - radius;
            screen_y = projected[1] - radius;

            if (screen_y + 96 > -16) {
                display->entries[sprite_index].attributes =
                    ((u32)order[i] << 25)
                    | ((u32)((screen_x + 632) & 0x1ff) << 16)
                    | ((u32)(screen_y + 352) & 0xff)
                    | 0x40002300u;

                {
                    s32 step = die->spin;

                    if (step < 0) {
                        step += 0x3ff;
                    }
                    display->entries[sprite_index].tile =
                        ((u32)DIE_FACE_TILE_080F4168[step >> 10] << 3) | 1024u;
                }
                sprite_index++;
            }
        }

        /* ------------------------------------------------------------ */
        /* Shadows (idle state only)                                     */
        /* ------------------------------------------------------------ */
        if (state == 0) {
            for (i = 0; i < 2; i++) {
                s32 distance;
                s32 screen_x;
                s32 screen_y;

                Func_08005268(&dice[i], projected);

                distance = Half_080f4168(depth[order[i]]) + 128;
                screen_x = projected[0] - Func_080022ec(128 << 4, distance);
                screen_y = projected[1] + Func_080022ec(192 << 2, distance);

                display->entries[sprite_index].attributes =
                    ((u32)((screen_x + 632) & 0x1ff) << 16)
                    | ((u32)(screen_y + 352) & 0xff)
                    | 0x80002000u;
                if (i == 1) {
                    display->entries[sprite_index].attributes |= 128u << 21;
                }
                display->entries[sprite_index].tile = 0x958;
                sprite_index++;
            }
        }

        /* ------------------------------------------------------------ */
        /* Ground markers under each die                                 */
        /* ------------------------------------------------------------ */
        {
            u32 palette_field = 128u << 19;

            for (i = 0; i < 2; i++) {
                shadow_source[0] = dice[i].x;
                shadow_source[1] = 0;
                shadow_source[2] = dice[i].z;

                Func_08005268(shadow_source, projected);

                if (projected[2] <= 0x15d) {
                    projected[2] = 350;
                }
                if (projected[2] > 0x28a) {
                    projected[2] = 0x28a;
                }
                depth[2 + i] = (projected[2] - 350)
                             + (s16)(dice[i].y >> 16);

                if (projected[1] + 88 > -16) {
                    display->entries[sprite_index].attributes =
                        ((u32)((projected[0] + 624) & 0x1ff) << 16)
                        | palette_field
                        | ((u32)(projected[1] + 344) & 0xff)
                        | 0x40002300u;
                    display->entries[sprite_index].tile = 137 << 4;
                    sprite_index++;
                }
                palette_field += 128u << 18;
            }
        }

        /* ------------------------------------------------------------ */
        /* Physics (rolling state only)                                  */
        /* ------------------------------------------------------------ */
        if (state == 1) {
            settled = 0;

            for (i = 0; i < 2; i++) {
                struct Die_080f4168 *die = &dice[i];
                s32 limit;

                die->x += die->vx;
                die->y += die->vy;
                die->z += die->vz;

                /* Push the dice apart when they overlap. */
                for (j = 0; j < 2; j++) {
                    s32 dx;
                    s32 dy;
                    s32 dz;
                    s32 squared;
                    s32 magnitude;

                    if (i == j) {
                        continue;
                    }
                    dx = (die->x - dice[j].x) >> 16;
                    dy = (die->y - dice[j].y) >> 16;
                    dz = (die->z - dice[j].z) >> 16;
                    squared = dx * dx + dy * dy + dz * dz;
                    if (squared > 0x9c3) {
                        continue;
                    }
                    magnitude = MAGNITUDE_080f4168(squared);
                    die->vx += Func_080022ec(dx << 15, magnitude);
                    die->vy += Func_080022ec(dy << 15, magnitude);
                    die->vz += Func_080022ec(dz << 15, magnitude);
                }

                /* Side walls: bounce back with three quarters of the speed. */
                if (die->x < (s32)0xff380000) {
                    die->x = (s32)0xff380000;
                    die->vx = ScaleBy64_080f4168(-die->vx * 48);
                }
                if (die->x > (200 << 16)) {
                    die->x = 200 << 16;
                    die->vx = ScaleBy64_080f4168(-die->vx * 48);
                }
                if (die->z < (s32)0xff6a0000) {
                    die->z = (s32)0xff6a0000;
                    die->vz = ScaleBy64_080f4168(-die->vz * 48);
                }

                limit = die->y + (180 << 16);
                if (die->z > limit) {
                    die->z = limit;
                    if (die->vz > 0) {
                        die->vy += Half_080f4168(die->vz);
                        die->vz = ScaleBy64_080f4168(-die->vz * 48);
                    }
                }

                die->vy += (s32)0xffff8000;   /* gravity */

                if (die->y <= 0) {
                    /* Table bounce. */
                    die->y = 0;
                    die->vy = ScaleBy64_080f4168(-die->vy * 48);

                    if ((Func_08004458() & 3) == 0) {
                        /* Occasionally scatter the horizontal velocity. */
                        s32 speed = MAGNITUDE_080f4168(
                            (die->vx >> 8) * (die->vx >> 8)
                            + (die->vz >> 8) * (die->vz >> 8));
                        s32 angle;

                        speed = ScaleBy64_080f4168(speed * 56);
                        angle = (s32)(Func_08004458() & 0xffff);
                        die->vx = (speed * Func_08002322(angle)) >> 8;
                        die->vz = (speed * Func_0800231c(angle)) >> 8;
                    } else {
                        die->vx = ScaleBy64_080f4168(die->vx * 56);
                        die->vz = ScaleBy64_080f4168(die->vz * 56);
                    }

                    if (spin_rate[i] > 19) {
                        spin_rate[i] -= 20;
                    }

                    if (die->vy <= 0x2ffff) {
                        /* Slow enough to come to rest: snap to a face. */
                        s32 step;
                        s32 remainder;

                        die->vx = 0;
                        die->vy = 0;
                        die->vz = 0;
                        spin_rate[i] = 0;

                        step = die->spin;
                        if (step < 0) {
                            step += 0x3ff;
                        }
                        remainder = Func_080022fc(step >> 10, 3);
                        if (remainder == 1) {
                            die->spin += 128 << 4;
                        } else if (remainder == 2) {
                            die->spin += 128 << 3;
                        }
                    }
                }

                /* Air drag: keep 63/64 of each velocity component. */
                die->vx = ScaleBy64_080f4168(die->vx * 63);
                die->vy = ScaleBy64_080f4168(die->vy * 63);
                die->vz = ScaleBy64_080f4168(die->vz * 63);

                die->spin += spin_rate[i];
                if (die->spin > 0x47ff) {
                    die->spin += (s32)0xffffb800;
                }
                if (spin_rate[i] > 0) {
                    spin_rate[i]--;
                }

                if (die->vx == 0 && die->vy == 0 && die->vz == 0
                    && die->y == 0) {
                    result[i] = Func_080022ec(die->spin, 192 << 4);

                    if (die->z > (DEPTH_BAND_080F4168[0] << 16)) {
                        result[2 + i] = 0;
                    } else {
                        k = 0;
                        do {
                            k++;
                            if (k == 6) {
                                break;
                            }
                        } while (die->z <= (DEPTH_BAND_080F4168[k] << 16));

                        if (k != 6) {
                            result[2 + i] = k;
                        }
                    }
                    settled++;
                }
            }

            if (settled == 2) {
                /* Both dice have come to rest: score the round. */
                state = 2;
                phase = 0;
                win_kind = -1;
                payout = 0;
                payout_timer = 60;

                if (result[0] == result[1] && result[0] == result[2]
                    && result[0] == result[3]) {
                    highlight[0] = 120;
                    highlight[1] = 120;
                    win_kind = 0;

                    if (result[0] == target[0]) {
                        payout = bet * 15;
                    } else if (result[0] == target[1]) {
                        payout = bet * 25;
                    } else {
                        payout = bet * 5;
                    }

                    Func_080150b0((s32)COINS_080F4168, 6,
                                  display->coin_window, 0, 0);
                    Func_080f9010(93);
                } else {
                    /*
                     * Count equal pairs among the four result cells.  The
                     * original also copies each matched value into a scratch
                     * buffer that overlaps `order`; that buffer is never read
                     * back, so only the count is reproduced here.
                     */
                    s32 matches = 0;

                    for (i = 0; i < 3; i++) {
                        for (j = i + 1; j < 4; j++) {
                            if (result[i] == result[j]) {
                                matches++;
                            }
                        }
                    }

                    if (matches == 1) {
                        payout = bet;
                        Func_080150b0((s32)COINS_080F4168, 6,
                                      display->coin_window, 0, 0);
                        win_kind = 3;
                    }
                    if (matches == 2) {
                        payout = bet * 2;
                        Func_080150b0((s32)COINS_080F4168, 6,
                                      display->coin_window, 0, 0);
                        highlight[Func_08004458() & 1] = 60;
                        win_kind = 2;
                        Func_080f9010(91);
                    }
                    if (matches == 3) {
                        payout = bet * 3;
                        Func_080150b0((s32)COINS_080F4168, 6,
                                      display->coin_window, 0, 0);
                        highlight[Func_08004458() & 1] = 60;
                        win_kind = 1;
                        Func_080f9010(92);
                    }
                }
            }
        }

        /* ------------------------------------------------------------ */
        /* Blank the unused sprite slots and publish OAM                 */
        /* ------------------------------------------------------------ */
        while (sprite_index != 128) {
            display->entries[sprite_index].attributes = 0x40f02000;
            display->entries[sprite_index].tile = 0;
            sprite_index++;
        }

        /* Four affine matrices, one scale per projected depth. */
        for (k = 0; k < 4; k++) {
            s32 scale = Half_080f4168(depth[k]) + 128;

            affine[3] = (s16)scale;   /* sp+166 */
            affine[2] = 0;            /* sp+164 */
            affine[1] = 0;            /* sp+162 */
            affine[0] = (s16)scale;   /* sp+160 */

            display->entries[4 * k + 0].tile |= (u32)(u16)affine[3] << 16;
            display->entries[4 * k + 1].tile |= (u32)(u16)affine[2] << 16;
            display->entries[4 * k + 2].tile |= (u32)(u16)affine[1] << 16;
            display->entries[4 * k + 3].tile |= (u32)(u16)affine[0] << 16;
        }

        Dma3Copy_080f4168(display->entries, (void *)0x07000000, 0x84000100);
        Func_080030f8(1);
        frame++;
    }

teardown:
    /* Fade the palettes back out over 17 frames, then release the owners. */
    for (i = 0; i < 17; i++) {
        s32 level = 65536 - (i << 12);

        Func_080f4100(object_palette_source, (u16 *)0x05000200, level, 256);
        Func_080f4100(bg_palette_source, (u16 *)0x05000000, level, 256);
        Func_080030f8(1);
    }

    Func_08002dd8(12);
    Func_08002dd8(45);
    Func_08002dd8(40);
    Func_08002dd8(39);
    Func_08002dd8(41);
}
