/* NONMATCHING: 888 bytes, candidate 844, 424 differing halfwords, 315
 * halfword edits (2026-09-25). FieldScene_RunScene3a5SequenceA, meant for
 * FIELD/RAMAKAN_SABAKU/F_018A4.C as a single-overlay unit binding its names
 * at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: Fresh reconstruction corrects actor stride,
 * tile-buffer header, sparse-copy extent and signed map-layer indexing.
 * Allocation and DMA setup still differ.
 * WALL: Whole-function allocation and DMA setup ordering. */
#include "DMA.H"
#include "TYPES.H"

struct VramBlock {
    u16 base;
    u16 offset;
};

struct Sprite {
    u32 words[3];
};

extern struct VramBlock Data_03001b10[];
extern u32 Data_02000240[];
extern s16 Data_02000240_t[][2];
extern u32 gFrameCount;

s32 Engine_MathDivide(s32 dividend, s32 divisor);
s32 Main_08000168(s32 size, void *base);
void Main_080001c0(s32 layer);
void Engine_VramLoad(s32 slot, s32 size, void *src);
void Main_080001e8(struct Sprite *sprite, s32 value);
s32 Main_08000320(void *dst, u32 value);
s32 Engine_GameFlagIsSet(s32 flag);
void Runtime_BumpFreeFar(void *allocation);

#define EFFECT_TIME (*(s16 *)0x0200a6be)
#define EFFECT_PHASE (*(s16 *)0x0200a6bc)
#define EFFECT_SCROLL (*(s16 *)0x0200a6c0)
#define MAP_LAYER (*(s16 *)0x0200a6d0)
#define MAP_MODE (*(s16 *)0x0200b030)
#define SCENE_RUNTIME (*(u8 **)0x03001ecc)
#define FRAME_COUNT (*(u32 *)0x03001e40)

/* FAKEMATCH: wrappers retain the reference argument evaluation order. */
static __inline__ s32 Value2(s32 (*fn)(), s32 a0, s32 a1)
{
    return fn(a0, a1);
}

static __inline__ void Call3(void (*fn)(), s32 a0, s32 a1, s32 a2)
{
    fn(a0, a1, a2);
}

void FieldScene_RunScene3a5SequenceA(void)
{
    u32 tile_offset;
    s32 timer;
    u8 *work;
    u8 *dst;
    u8 *src;
    u16 *tile;
    s32 frame;
    s32 angle;
    s32 x;
    s32 y;
    s32 i;
    s32 slot;
    s32 alloc_arg;
    u16 *timerp;
    u32 *sprite_words;
    struct Sprite *sprite;

    tile_offset = Data_03001b10[(s16)MAP_LAYER].offset >> 5;
    timerp = (u16 *)0x0200a6be;
    if (MAP_MODE != 0) {
        *timerp = 2;
    } else if (Engine_GameFlagIsSet(0x104) != 0) {
        if (*(s16 *)timerp > 0)
            (*timerp)--;
    } else if (*(s16 *)timerp <= 1) {
        (*timerp)++;
        if (*timerp == 2)
            Dma_Set((const void *)0x02009f80, (void *)0x050003c0,
                -0x7ffffff0, (volatile u32 *)0x040000d4);
    }

    timer = *(s16 *)timerp;
    if (timer == 0) {
        Main_080001c0(MAP_LAYER);
        return;
    }

    work = SCENE_RUNTIME;
    if (work != 0) {
        u32 actor;

        actor = work[0x539];
        dst = work + actor * 0x284 + 0x26;
        for (i = 0; i <= 143; i++) {
            *(u16 *)dst = (u16)(timer << 3);
            dst += 4;
        }
    }

    alloc_arg = (s32)dst;
    sprite_words = (u32 *)Value2(Main_08000168, 0x900, alloc_arg);
    Dma_Set((const void *)0x02009f80, sprite_words, -0x7ffffff0,
        (volatile u32 *)0x040000d4);

    tile = (u16 *)((u8 *)sprite_words + 12);
    for (slot = 6; slot <= 11; slot++, tile++) {
        u32 packed;
        s32 raw_x;
        s32 raw_y;
        s32 scroll;

        packed = (u32)*tile;
        raw_x = (packed >> 8) & 31;
        raw_y = (packed >> 5) & 31;
        angle = EFFECT_PHASE;
        x = Engine_MathDivide(angle, 3) + raw_x;
        scroll = Engine_MathDivide(angle, 6);
        y = (((packed >> 10) & 31) - 20) - scroll + 20;
        if (angle > 60 && (FRAME_COUNT & 1) != 0)
            raw_y = raw_y + Engine_MathDivide(angle << 6, 120) - 32;
        if ((u32)x > 31)
            x = 31;
        if ((u32)raw_y > 31)
            raw_y = 31;
        if ((u32)y > 31)
            y = 31;
        *tile = (u16)((y << 10) | (raw_y << 5) | x);
    }

    Main_08000320((void *)0x050003cc,
        *(u32 *)((u8 *)sprite_words + 12));
    ((s32 (*)(void *, u32))Main_08000320)((void *)0x050003d0,
        *(u32 *)((u8 *)sprite_words + 16));
    Value2(Main_08000320, 0x050003d4,
        *(u32 *)((u8 *)sprite_words + 20));

    frame = *(s16 *)((u8 *)Data_02000240 + 0x232);
    angle = Engine_MathDivide(((frame << 4) - frame) << 3,
        Data_02000240_t[139][0]);
    EFFECT_PHASE = angle;
    if (angle > 118)
        EFFECT_SCROLL = 0x77;
    if (EFFECT_SCROLL != 0) {
        EFFECT_PHASE = EFFECT_SCROLL;
        EFFECT_SCROLL -= 8;
        if (EFFECT_SCROLL <= 0)
            EFFECT_SCROLL = 0;
    }

    Dma_Set((const void *)0x0200a730, sprite_words, -0x7bfffdc0,
        (volatile u32 *)0x040000d4);
    if (EFFECT_SCROLL <= 118) {
        dst = (u8 *)sprite_words + 80;
        if (12 < 128 - EFFECT_PHASE) {
            i = 12;
            do {
                *(u32 *)(dst + 32) = 0xeeeeeeee;
                *(u32 *)dst = 0xeeeeeeee;
                dst += 4;
                if ((i & 7) == 7)
                    dst += 32;
                i++;
            } while (i < 128 - EFFECT_PHASE);
        }
        *(u32 *)dst = *(u32 *)sprite_words;
        *(u32 *)(dst + 32) = *(u32 *)((u8 *)sprite_words + 32);
    }

    src = (u8 *)sprite_words + 0x480;
    dst = (u8 *)sprite_words;
    for (i = 0; i < 0x480; i++) {
        if (src[i + 1] != 0)
            dst[i] = src[i + 1];
    }
    Call3(Engine_VramLoad, MAP_LAYER, 0x480, (s32)sprite_words);

    sprite = (struct Sprite *)0x0200a6e0;
    for (i = 0; i <= 4; i++) {
        sprite->words[0] = 0;
        sprite->words[1] = (((((EFFECT_TIME << 3) - 16) & 0x1ff) << 16)
            | (8 + i * 32) | (i == 4 ? 0x40000000 : 0x80000000));
        sprite->words[2] = 0xe400 | tile_offset;
        Main_080001e8(sprite, 255);
        sprite++;
        tile_offset += 8;
    }
    Runtime_BumpFreeFar(sprite_words);
}
