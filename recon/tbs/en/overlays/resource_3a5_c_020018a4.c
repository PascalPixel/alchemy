/* NONMATCHING: 888 bytes, candidate 848, 401 differing halfwords, 251
 * halfword edits (2026-09-27). FieldScene_RunScene3a5SequenceA, meant for
 * FIELD/RAMAKAN_SABAKU/F_018A4.C as a single-overlay unit binding its names
 * at their runtime addresses (an import veneer's listing offset plus
 * 0x8000). Remaining: Fresh reconstruction corrects actor stride,
 * tile-buffer header, sparse-copy extent and signed map-layer indexing.
 * This pass fixes the one-argument allocator, RGB555 extraction, unsigned
 * loop bounds, frozen fill extent and sequential palette destinations.
 * Timer-view and word-mode decrement hypotheses improve 269 to 251 edits.
 * Remaining: timer reloads, channel lifetimes and sprite allocation.
 * Family H1 (2026-09-27): all four divisions resolve through the overlay
 * import to signed division at 0x03000380. Transfer ordinary C division
 * from exact COMMON/EFFECT/SPAWN.C and the improved Toreto palette model,
 * binding __divsi3 to that existing import. Complete normalized diff stays
 * 848/888, 401 halfwords / 251 edits: only the final numerator shift moves
 * after the denominator load, as required. No palette caller-save or pool
 * improvement. Keep the natural arithmetic, stop this call-spelling axis.
 * Distinct remaining evidence: reference loads a shared 31 extraction mask
 * from its first colour-loop pool and keeps it in fp; the draft instead
 * materializes 31 per iteration and keeps the loop counter in fp. Timer
 * loads/narrow comparisons and final indexed sprite argument also differ.
 * Family H2: a u16 shared extraction mask from Value_0000001f, following
 * the narrow link-constant family in exact MAKYURI/PALETTE_CYCLE.C.
 * Complete diff: 860/888, 399 halfwords / 266 edits. The mask is stored
 * left-shifted in a new stack slot and reloaded/right-shifted per iteration;
 * it does not displace the loop counter from fp or recreate the early pool.
 * Frame grows from the correct 12 to 16 bytes. Reject this long-lived
 * narrow-mask model, preserved at c04a58975; H1 is restored here. No mask
 * declaration or scalar-order sweeps. Whole owner remains not-yet-C,
 * with 0 new DONE bytes.
 * Interface audit (2026-09-27): RUNTIME.S proves 080001c0 -> 08003f78
 * Resource_ActivateEntry and 080001c8 -> 08003fa4 VramBlock_LoadCached;
 * both exact bodies return s32. FIELD_EVENT.H now supplies the latter's
 * declaration and direct call; VRAM_BLOCK.H supplies the shared cache.
 * This model is byte-identical to the prior 848/888, 401-halfword/251-edit
 * candidate. The two corrected return types alone do not close allocation.
 * Distinct semantic evidence in the complete diff: reference 01aa6..01aae
 * compares the signed halfword just written to EFFECT_PHASE, but this draft
 * compares the full-width quotient. Also counter +0x232 and limit +0x22c
 * use two differently typed global aliases, unlike exact Field_ProcessStep.
 * These are the only admitted follow-up; no timer/mask spelling sweep. */
#include "DMA.H"
#include "FIELD_EVENT.H"
#include "TYPES.H"
#include "VRAM_BLOCK.H"

struct Sprite {
    u32 words[3];
};

union HalfWord {
    u16 value;
    s16 signed_value;
};

extern u32 Data_02000240[];
extern s16 Data_02000240_t[][2];
extern u32 gFrameCount;

void *Main_08000168(s32 size);
s32 Main_080001c0(u32 layer);
void Main_080001e8(struct Sprite *sprite, s32 value);
void Main_08000320(void *dst, u32 value);
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

/* FAKEMATCH: the word-sized step keeps subtraction out of halfword mode. */
static __inline__ void Half_Add(union HalfWord *dst, s32 step)
{
    dst->value = dst->value + step;
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
    u32 i;
    u32 slot;
    union HalfWord *timerp;
    u32 *sprite_words;
    struct Sprite *sprite;

    tile_offset = gVramBlockCache[(s16)MAP_LAYER].offset >> 5;
    if (MAP_MODE != 0) {
        timerp = (union HalfWord *)0x0200a6be;
        timerp->value = 2;
    } else if (Engine_GameFlagIsSet(0x104) != 0) {
        timerp = (union HalfWord *)0x0200a6be;
        if (timerp->signed_value > 0)
            Half_Add(timerp, -1);
    } else {
        timerp = (union HalfWord *)0x0200a6be;
        if (timerp->signed_value <= 1) {
            u16 value = timerp->value + 1;

            timerp->value = value;
            if (value == 2)
                Dma_Set((const void *)0x02009f80, (void *)0x050003c0,
                    -0x7ffffff0, (volatile u32 *)0x040000d4);
        }
    }

    timer = timerp->signed_value;
    if (timer == 0) {
        Main_080001c0(MAP_LAYER);
        return;
    }

    work = SCENE_RUNTIME;
    if (work != 0) {
        u32 actor;

        actor = work[0x539];
        dst = work + actor * 0x284 + 0x26;
        i = 0;
        do {
            i++;
            *(s16 *)dst = (s16)(timer << 3);
            dst += 4;
        } while (i <= 143);
    }

    sprite_words = Main_08000168(0x900);
    Dma_Set((const void *)0x02009f80, sprite_words, -0x7ffffff0,
        (volatile u32 *)0x040000d4);

    tile = (u16 *)((u8 *)sprite_words + 12);
    for (slot = 6; slot <= 11; slot++, tile++) {
        u32 packed;
        s32 raw_y;
        s32 scroll;

        packed = (u32)(s16)*tile << 16;
        x = (packed & 0x1f0000) >> 16;
        raw_y = (packed >> 21) & 31;
        y = (packed >> 26) & 31;
        angle = EFFECT_PHASE;
        x += angle / 3;
        y -= 20;
        scroll = angle / 6;
        y -= scroll;
        y += 20;
        if (angle > 60 && (FRAME_COUNT & 1) != 0)
            raw_y = raw_y + (angle << 6) / 120 - 32;
        if ((u32)x > 31)
            x = 31;
        if ((u32)raw_y > 31)
            raw_y = 31;
        if ((u32)y > 31)
            y = 31;
        *tile = (u16)((y << 10) | (raw_y << 5) | x);
    }

    {
        u32 *palette = (u32 *)0x050003cc;
        u32 *colors = sprite_words + 3;

        Main_08000320(palette++, *colors++);
        Main_08000320(palette++, *colors++);
        Main_08000320(palette, *colors);
    }

    frame = *(s16 *)((u8 *)Data_02000240 + 0x232);
    angle = (((frame << 4) - frame) << 3) / Data_02000240_t[139][0];
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
        u32 end = 128 - EFFECT_PHASE;

        dst = (u8 *)sprite_words + 80;
        if (12 < end) {
            i = 12;
            do {
                *(u32 *)(dst + 32) = 0xeeeeeeee;
                *(u32 *)dst = 0xeeeeeeee;
                dst += 4;
                if ((i & 7) == 7)
                    dst += 32;
                i++;
            } while (i < end);
        }
        *(u32 *)dst = *(u32 *)sprite_words;
        *(u32 *)(dst + 32) = *(u32 *)((u8 *)sprite_words + 32);
    }

    src = (u8 *)sprite_words + 0x480;
    dst = (u8 *)sprite_words;
    i = 0;
    do {
        u8 value = *src++;

        if (value != 0)
            *dst = value;
        i++;
        dst++;
    } while (i <= 0x47f);
    Engine_VramLoad(MAP_LAYER, 0x480, sprite_words);

    {
        u32 flags = 0x80008000;
        u32 pos = 8;

        sprite = (struct Sprite *)0x0200a6e0;
        for (i = 0; i <= 4; i++) {
            s32 y = ((EFFECT_TIME << 3) - 16) & 0x1ff;

            if (i == 4)
                flags = 0x40000000;
            sprite->words[0] = 0;
            sprite->words[1] = (y << 16) | pos | flags;
            sprite->words[2] = 0xe400 | tile_offset;
            Main_080001e8(sprite, 255);
            sprite++;
            tile_offset += 8;
            pos += 32;
        }
    }
    Runtime_BumpFreeFar(sprite_words);
}
