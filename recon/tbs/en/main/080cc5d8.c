/* 2026-09-30 (Mars): linked in place, 67 differing halfwords (from 70):
   the projection read after EffectPosition_ApplyStepAndYOffset goes
   through an r5 copy as in the reference. Left: the reference keeps
   0x04000028 in r1 and adds 42 for the blend write (hiding the constant
   grows the frame, 271), and the low temporaries after it. */
/* 2026-09-24: 69 differing halfwords (from 70) after a do-while wrap and
   statement-swap sweep; the do-while wraps are search artefacts.
   2026-09-29 slice 4: alchemy permute cannot parse this draft, because
   M2C_FIELD takes a type as a macro argument. Preprocessed, it scores 2,050
   with 21 symbols the linked build does not define, too far for a 10-minute
   search, so none was run.
   A 5-minute search (2 jobs) on the preprocessed copy reached 1675 after 25
   rewrites, pointer and temporary spellings that cannot be written back to
   this macro form. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"

/*
 * Battle-presentation sub-effect at 0x080cc5d8.  Family-matched to
 * games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C (owner 080ce85c) at
 * structural score 8031/10000, but this owner allocates its own kind-39
 * (work), kind-40 (canvas) and kind-41 (trail_source) heap blocks up front
 * via Runtime_AllocateHeapBlock instead of reading pre-existing ones out of the shared
 * heap-allocation cache, and frees them again (in LIFO order) at the end --
 * see games/THE BROKEN SEAL/src/battle/effects/objects/start_effect_22.c for the
 * Runtime_AllocateHeapBlock(asset_id, size) signature.
 *
 * gWorkSlot[kind] is that same heap-allocation cache (see the comment
 * in recon/tbs/en/main/080e7404.c and games/THE BROKEN SEAL/src/battle/effects/
 * puff_arc/run.c); this owner reads it directly at kinds 46 and 47 rather
 * than through a locally-renamed "heap_cache" pointer, since it never reads
 * kinds 39/40/41 back out of it (it made those blocks itself).
 *
 * Field offsets 0x7780/0x7784/0x7824/0x7828 and the BattleFx_BeginCanvasLayer/
 * Resource_LoadAndDecompress/Scheduler_AddOrUpdateCallback/Scheduler_RemoveCallback/BattleEffect_LoadWork/Runtime_ReleaseHeapBlock/
 * BattleFx_EndCanvasLayer/ObjectGroup_UpdateMembers calling shapes follow the 0x03001eec "battle
 * work" subsystem already recovered in
 * games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C and
 * recon/tbs/en/main/080d59b0.c.  The fixed 0x02010000 "star" array and
 * its 28-byte, {f0,f4,f8,f24}-field record shape is the same one used by
 * recon/tbs/en/main/080d59b0.c; this owner also keeps a second,
 * independent copy of that record shape inside its own work block at
 * +0x7080 (64 entries) for a per-frame sparkle trail around a single
 * projected point, which is why the fixed 0x02010000 array here is only
 * ever written, never read back inside this function.
 *
 * Value_00000045/00000046/00000047/00000048/00000057/00000076 are the
 * established spelling for small absolute link-time constants (see
 * games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C's Value_000000af comment):
 * every one of these is loaded from a literal pool rather than an
 * immediate, which an ordinary integer literal cannot produce.
 *
 * Data_080ee058/080ee05c/080ee060 and BattleFx6_FlareCells are pre-existing ROM
 * tables already catalogued in games/THE BROKEN SEAL/SRC/BATTLE/DATA/SENTOU_KOUKA_HYOU_A.JSON
 * (hyou_a_030/031/032 and hyou_a_001 respectively); ParticleStreams_CellOffsets in
 * recon/tbs/en/main/080dc1ec.c documents the extern-array convention
 * used for that same asset.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

extern void *gWorkSlot[];
extern const u8 Data_080ee058[4];
extern const u8 Data_080ee05c[4];
extern const u8 Data_080ee060[4];
extern const u16 BattleFx6_FlareCells[];
extern u8 Value_00000046;
extern u8 Value_00000047;
extern u8 Value_00000048;
extern u8 Value_00000057;

void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Random16(void);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void Audio_PlayCue(s32 id);
void EffectPosition_ApplyStepAndYOffset(s32 source, void *screen);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 Math_Div(s32 numerator, s32 denominator);
s32 Math_Mod(s32 numerator, s32 denominator);
void Runtime_ReleaseHeapBlock(s32 id);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
s32 BattleFx_EndCanvasLayer(void);

void Func_080cc5d8(void *object)
{
    void *work;
    void *canvas;
    DrawRectangleFn rectangle[2];
    void *trail_source;
    void *palette;
    s32 status;
    s32 palette_id;
    u8 *star;
    s32 i;
    s32 frame;
    s32 screen[3];
    WordCopyFn copy;
    s32 callback_interval;
    void *display_base;

    work = Runtime_AllocateHeapBlock(39, 0x782c);
    canvas = Runtime_AllocateHeapBlock(40, 0x4000);
    trail_source = Runtime_AllocateHeapBlock(41, 0x60e);
    M2C_FIELD(work, void **, 0x7828) = object;
    BattleFx_BeginCanvasLayer(0);

    M2C_FIELD(work, s32 *, 0x77b4) = 24;
    M2C_FIELD(work, s32 *, 0x77b8) = 0;
    M2C_FIELD((void *)0x04000052, u16 *, 0) = 0x100c;
    M2C_FIELD((void *)0x04000020, u16 *, 0) = 0x100;

    Resource_LoadAndDecompress((s32)&ResourceId_CyanSparkSheet, work, 1, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesD, trail_source, 0, 0);

    switch (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 0)) {
    case 0:
        palette_id = (s32)&Value_00000048;
        break;
    case 1:
        palette_id = (s32)&Value_00000057;
        break;
    case 2:
        palette_id = (s32)&Value_00000047;
        break;
    default:
        palette_id = (s32)&Value_00000046;
        break;
    }
    palette = Resource_GetTableEntry(palette_id);
    copy = (WordCopyFn)0x03001388;
    status = copy((void *)0x05000000, palette, 128);

    star = (u8 *)0x02010000;
    for (i = 0; i != 128; i++) {
        M2C_FIELD(star, s32 *, 4) = 0x800000;
        M2C_FIELD(star, s32 *, 0) = (s32)(Random16() & 0xFFFF);
        M2C_FIELD(star, s32 *, 8) = (s32)(Random16() & 0x1FF) + 1024;
        M2C_FIELD(star, s32 *, 24) = -i;
        star += 28;
    }

    for (i = 0; i != 64; i++) {
        u8 *slot = (u8 *)work + 0x7080 + i * 28;
        M2C_FIELD(slot, s32 *, 0) = (s32)(Random16() & 0xFFFF);
        M2C_FIELD(slot, s32 *, 4) = (s32)(Random16() & 31) + 16;
        M2C_FIELD(slot, s32 *, 24) = (i & 15) + 16;
    }

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    callback_interval = 0x480;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, callback_interval);

    status = BattleEffect_LoadWork(46, 7, 7, 7, 3);
    rectangle[0] = (DrawRectangleFn)gWorkSlot[46];
    Audio_PlayCue(140);

    for (frame = 0; frame != 56; frame++) {
        {
            register s32 *pos asm("r5") = screen; /* FAKEMATCH: the reference reads the projection through r5 */
            EffectPosition_ApplyStepAndYOffset(M2C_FIELD(object, s32 *, 8), screen);
            display_base = (void *)0x04000028;
            M2C_FIELD(display_base, s32 *, 0) = (64 - pos[0]) << 8;
            if (frame > 49) {
                M2C_FIELD(display_base, u16 *, 42) =
                    (112 - frame * 2) | 0x1000;
            }
        }

        if (frame == 26) {
            Audio_PlayCue(212);
            ObjectGroup_UpdateMembers(
                M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s16 *, 36),
                7, -1, 0, 20);
        }

        if ((u32)(frame - 28) <= 20) {
            s32 sprite_frame = Math_Div(frame - 28, 3);

            rectangle[0](
                canvas, (u8 *)work + 0x1400 + sprite_frame * 0x900,
                40, screen[1] - 24, 48, 48);
        }

        if ((u32)frame <= 14) {
            s32 offset = (Math_Mod(Math_Div(frame, 3), 5)) << 10;

            for (i = 0; i != 4; i++) {
                s32 x;
                s32 y;

                status = BattleEffect_LoadWork(47, 7, 7, Data_080ee060[i] | 3, 2);
                x = (s8)Data_080ee058[i] + 32;
                y = (screen[1] + (s8)Data_080ee05c[i]) - 32;
                rectangle[1] = (DrawRectangleFn)gWorkSlot[47];
                rectangle[1](canvas, (u8 *)work + offset, x, y, 32, 32);
                Runtime_ReleaseHeapBlock(47);
            }
        }

        if (frame >= 0) {
            u8 *slot = (u8 *)work + 0x7080;

            for (i = 0; i != 64; i++) {
                if (M2C_FIELD(slot, s32 *, 24) >= 0
                        && M2C_FIELD(slot, s32 *, 4) > 0) {
                    s32 radius_x;
                    s32 radius_y;
                    s32 half;
                    s32 full;

                    half = (M2C_FIELD(slot, s32 *, 24) >> 3) + 1;
                    radius_x = ((M2C_FIELD(slot, s32 *, 4)
                        * Trig_Sin(M2C_FIELD(slot, s32 *, 0))) >> 16)
                        + 64;
                    radius_y = ((M2C_FIELD(slot, s32 *, 4)
                        * Trig_Cos(M2C_FIELD(slot, s32 *, 0))) >> 16)
                        + screen[1];
                    if (half <= 0) {
                        half = 1;
                    }
                    full = half << 1;
                    rectangle[0](
                        canvas, trail_source + BattleFx6_FlareCells[half - 1],
                        radius_x - half, radius_y - half, full, full);
                    M2C_FIELD(slot, s32 *, 4) -= 2;
                    M2C_FIELD(slot, s32 *, 24) -= 1;
                }
                slot += 28;
            }
        }

        ObjectGroup_TickMemberTimers();
        do { M2C_FIELD(work, s32 *, 0x7824) = 1; } while (0);
        WaitFrames(1);
    }

    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((void *)0x080CD261);
    BattleFx_EndCanvasLayer();
    Runtime_ReleaseHeapBlock(41);
    Runtime_ReleaseHeapBlock(40);
    Runtime_ReleaseHeapBlock(39);
}
