/* NONMATCHING: 888 bytes, candidate 848, 355 differing halfwords, 218
 * aligned edits (2026-09-27). FieldScene_RunScene3a5SequenceA, meant for
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
 * These are the only admitted follow-up; no timer/mask spelling sweep.
 * Interface H2: one TravelState replaces the two aliases, and a block-local
 * s16 phase records the quotient before its signed comparison. Result is
 * 852/888, 394 halfwords / 235 aligned edits; frame remains 12. The exact
 * entry prefix grows from 8 to 22 instructions. The counter/limit arithmetic
 * at reference 01a84..01a9e now has the same registers and operations, apart
 * from relocated pool-load displacements. Narrow phase semantics are fixed,
 * but sign extension before its store still differs from the reference's
 * shifted comparison after the store. Colour-loop mask/counter lifetime,
 * the timer's paired signed/unsigned reads, fill-loop precheck, sprite array
 * cursor and pool positions remain nonmatching. Retain this semantic model;
 * stop after ABI/layout model plus one follow-up. No new DONE bytes.
 * Lamakan phase H1 (2026-09-27): compare the stored signed EFFECT_PHASE,
 * rather than a narrow local assigned before the store. Prediction: store
 * the full quotient first, then use the shifted halfword comparison with
 * no pre-store arithmetic-right-shift. One complete-diff trial; frame,
 * extent and pools plus production gates remain mandatory for adoption.
 * H1 result: 852/888, 394 halfwords / 230 aligned edits, 12-byte frame.
 * Quotient store and shifted signed comparison at reference 01aa2..01aae
 * now have the exact registers and instruction order, apart from relocated
 * pool/branch displacements. The phase pointer stays in r5 throughout the
 * exact scroll-update operations. Retain; no local-phase spelling sweep.
 * Separate timer evidence: GCSE's mem/s:HI union view forwards the just
 * stored unsigned timer at the branch join and keeps a u16 increment local;
 * reference instead compares the signed stored halfword and reloads it.
 * Lamakan timer H2: one signed timer object replaces the union's unsigned
 * arithmetic view and the u16 shadow. Compare the stored halfword after
 * incrementing it. Prediction: pooled initial 2, paired signed/unsigned
 * loads, shifted comparison after the store and a fresh branch-join read.
 * One trial, complete normalized diff; retain the phase H1 invariant.
 * H2 result: 852/888, 411 halfwords / 232 aligned edits, 12-byte frame.
 * The initial 2 is now pooled, but the two timer views still collapse and
 * the branch join still forwards its value. Reject this reload prediction;
 * no signed/unsigned timer declaration sweep. Full diff exposes a new
 * phase fact: reference 0191e shifts 128 by 9, comparing the incremented
 * signed timer to 1, whereas this draft compares it to 2 (shift by 10).
 * One follow-up may correct that phase condition; other timer axes stop.
 * Lamakan timer H3, final phase trial: the increment from 0 to 1 starts
 * the palette DMA, rather than the subsequent increment from 1 to 2.
 * Prediction: the comparison constant shifts 128 by 9, matching reference
 * 0191e, with unchanged 852-byte extent and 12-byte frame. One condition
 * change, whole normalized diff, both-ROM compare and full final gate;
 * no further timer-view or reload spelling experiments.
 * H3 result: 852/888, 411 halfwords / 231 aligned edits, 12-byte frame.
 * Only the predicted comparison constant changes from H2. Retain the
 * reference-proven 0-to-1 palette trigger despite the signed-view model's
 * explained one-edit regression versus H1's semantically wrong trigger.
 * The stored EFFECT_PHASE comparison and scroll-update invariants remain.
 * Bounded timer/phase axes are exhausted; remaining work is paired timer
 * reads and join reload, shared colour-mask/counter lifetimes, fill-loop
 * precheck/counter lifetime, sprite array cursor, and resulting pools.
 * Whole owner remains not-yet-C; no exact-function or alignment credit.
 * Lamakan word-mask H1 (2026-09-27): reference 019a6 word-loads 31
 * from 019e4 and 019ae retains it in fp across the RGB loop's calls;
 * 019c4..019ca reuse it only for the two upper-channel extractions.
 * The three clamps still compare/materialize immediate 31. Test one u32
 * opaque link-symbol producer for those extraction consumers, unlike the
 * rejected u16 producer's left-shifted stack reload. Prediction: shared
 * word pool before the RGB phase, mask in fp and counter in r4/sp+0.
 * Admission: frame 12, correct shared mask/pool and preserved 0-to-1
 * timer trigger plus phase store/signed comparison. Exact adoption also
 * requires all 888 bytes and compare/coverage/verify. Read the full
 * normalized diff, extent, frame, mask and counter liveness. Budget:
 * at most three informed trials/30 minutes; stop this producer axis on
 * failed admission without a new structural fact. Record result here.
 * H1 result: 856/888, 411 differing halfwords / 236 aligned edits,
 * frame 12. Trial source was u32 mask = (u32)&Value_0000001f before
 * the loop, consumed by (packed >> 21) & mask and (packed >> 26) & mask;
 * clamps retained literal 31. Full diff and generated assembly show the
 * word mask reloaded into r0 inside every iteration from the later pool,
 * immediately beside the phase-pointer load. fp still holds the counter;
 * no r4 spill at either RGB division and no first-colour-loop pool appear.
 * Unlike the u16 trial it adds no shifted stack slot, but producer width
 * alone does not establish the reference's call-spanning mask lifetime.
 * The timer trigger and phase store/signed reload invariant are unchanged.
 * Reject admission and restore the canonical 852/888 body (231 edits).
 * Stop after one informed trial: no new fact justifies another mask
 * spelling/declaration-order sweep. No new DONE bytes; owner not-yet-C.
 * Timer snapshots (2026-09-27): word-mode signed/unsigned bitfield views
 * alone give 856/888, 415 halfwords / 233 edits; the unsigned read is too
 * late. Snapshot both views before testing active, retaining the initial
 * cast-pointer store of 2: 848/888, 355 halfwords / 219 edits, frame 12.
 * Increment through DMA now matches, including paired reads and shifted
 * stored comparison. The volatile join forces a fresh read but emits
 * ldrh/lsl/asr instead of ldrsh, so the join remains nonmatching.
 * A plain u8 extraction mask then gives 848/888, 355 halfwords / 223
 * edits: still rematerialized per iteration, counter still in fp. Reject
 * that mask trial; retain timer snapshots only. No new DONE credit.
 * Fill pretest: for (i = 12; i < end; i++) gives the reference's
 * register-bound pretest, 848/888, 355 halfwords / 218 edits, frame 12.
 * Retain it. Reusing i for the RGB loop puts every counter in r4 with
 * the reference's sp+0 call spill, but moves tile_offset to fp and shrinks
 * the frame to 8 (848/888, 360 halfwords / 192 edits). Indexed sprite
 * submission on top adds a separate cursor and moves flags into r8, but
 * not the reference's byte offset (856/888, 360 halfwords / 192 edits).
 * Reject both despite the lower edit score: the frame invariant fails.
 * Their useful evidence is whole-function counter priority, not a mask
 * width. Stop after these three structural trials; retain frame 12. */
#include "DMA.H"
#include "FIELD_EVENT.H"
#include "TYPES.H"
#include "VRAM_BLOCK.H"

struct Sprite {
    u32 words[3];
};

struct TravelState {
    u8 unknown_000[0x22c];
    s16 limit;
    s16 mode;
    s16 damage;
    s16 steps;
};

extern struct TravelState Data_02000240_t;
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

/* FAKEMATCH: word-mode views preserve the paired signed/unsigned reads. */
union SceneTimer {
    s32 signed_value : 16;
    u32 value : 16;
};

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
    union SceneTimer *timerp;
    u32 *sprite_words;
    struct Sprite *sprite;

    tile_offset = gVramBlockCache[(s16)MAP_LAYER].offset >> 5;
    if (MAP_MODE != 0) {
        timerp = (union SceneTimer *)0x0200a6be;
        *(s16 *)timerp = 2;
    } else if (Engine_GameFlagIsSet(0x104) != 0) {
        timerp = (union SceneTimer *)0x0200a6be;
        {
            s32 active = timerp->signed_value;
            u32 value = timerp->value;

            if (active > 0)
                timerp->signed_value = value - 1;
        }
    } else {
        s32 active;
        u32 value;

        timerp = (union SceneTimer *)0x0200a6be;
        active = timerp->signed_value;
        value = timerp->value;
        if (active <= 1) {
            timerp->signed_value = value + 1;
            if (timerp->signed_value == 1)
                Dma_Set((const void *)0x02009f80, (void *)0x050003c0,
                    -0x7ffffff0, (volatile u32 *)0x040000d4);
        }
    }

    /* FAKEMATCH: the phase join reads the signed timer again. */
    timer = *(volatile s16 *)timerp;
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

    {
        frame = Data_02000240_t.steps;
        EFFECT_PHASE = (((frame << 4) - frame) << 3) / Data_02000240_t.limit;
        if (EFFECT_PHASE > 118)
            EFFECT_SCROLL = 0x77;
    }
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
        for (i = 12; i < end; i++) {
            *(u32 *)(dst + 32) = 0xeeeeeeee;
            *(u32 *)dst = 0xeeeeeeee;
            dst += 4;
            if ((i & 7) == 7)
                dst += 32;
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
