#include "TYPES.H"
#include "BATTLE_EFX.H"

/*
 * Battle-presentation sub-effect at 0x080ca1fc, part of the 0x03001eec
 * "battle work" subsystem already partly recovered at recon/tbs/en/
 * main/080e7404.c, 080d82b0.c and games/THE BROKEN SEAL/src/battle/effects/
 * member_orbit/run.c.  Unlike member_orbit (single object argument), this
 * owner takes a second explicit `mode` argument that both selects the
 * Resource_GetTableEntry palette resource (via the established Value_ literal-pool
 * trick) and gates several halving/branch decisions throughout the frame
 * loop.
 *
 * Semantic summary: opens display kinds 46 and 47 (draw-rectangle blit
 * routines cached in gWorkSlot[]), copies a palette through the
 * generic word-copy helper (_call_via_r3 taking the 0x03001388 word-copy
 * routine as a trailing callback argument -- _call_via_r3 is the r3 slot
 * of the _call_via_rN trampoline at recon/tbs/raw/080072e4.s, modeled as a
 * direct call with the real callee as a trailing argument per that
 * trampoline's established convention), seeds a 256-slot particle pool at
 * 0x02010000 from one party member's position, then runs 128 frames.
 * Each frame redraws the first 128 particles whose staggered reveal
 * window is open and whose phase field is non-negative, projecting each
 * through EffectPosition_ApplyBaseAndYOffset and blitting through the kind-46 routine (also a
 * genuinely traced function pointer, the r4 slot of the same trampoline).
 * Mode 1 additionally nudges each drawn particle vertically by +-8192 and
 * drives ObjectGroup_UpdateMembers portrait callouts differently than mode 0.
 * 2026-09-29 slice 4: alchemy permute cannot parse this draft, because
 * M2C_FIELD takes a type as a macro argument. Preprocessed, it scores 3,733
 * with 1 symbols the linked build does not define, too far for a 10-minute
 * search, so none was run.
 */
#define M2C_FIELD(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))

typedef void (*WordCopyFn)(void *dest, void *src, s32 size);

extern void *gWorkSlot[];
extern const u16 ParticleStreams_CellOffsets[];
extern u8 Value_00000073;
extern u8 Value_0000007b;
extern u8 Value_0000007c;

void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
void _call_via_r3(void *dest, void *src, s32 size, WordCopyFn copier);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void **GetBattleObjectSlotFar(s32 member_id);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void EffectPosition_ApplyBaseAndYOffset(const void *source, void *screen);
void EffectStep_AdvanceWithGravity3D(void *particle, s32 a, s32 b);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void Audio_PlayCue(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);

void BattleFx_RunParticlePool(void *object, s32 mode)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    void * volatile second_blit_kind;
    void *member_object;
    DrawRectangleFn draw_rectangle_fn;
    s32 facing;
    s32 look_target;
    s32 status;
    s32 outer;
    s32 i;
    s32 j;
    s32 member;
    s32 member_id_offset;
    void *particle;
    s32 position[3];
    s32 screen[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    facing = *(s32 *)((u8 *)heap_cache - 108);
    extra_target = heap_cache[2];
    M2C_FIELD(work, void **, 0x7828) = object;

    if (mode == 0) {
        BattleFx_BeginCanvasLayer(0);
    } else {
        BattleFx_BeginCanvasLayer(1);
    }

    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw_rectangle_fn = (DrawRectangleFn)gWorkSlot[46];
    status = BattleEffect_LoadWork(47, 7, 7, 11, 2);
    second_blit_kind = gWorkSlot[47];

    Resource_LoadAndDecompress((s32)&Value_00000073, extra_target, 0, 0);

    _call_via_r3((void *)(160 << 19),
        Resource_GetTableEntry(mode == 0 ? (s32)&Value_0000007c : (s32)&Value_0000007b),
        128, (WordCopyFn)0x03001388);

    M2C_FIELD(work, s32 *, 0x7780) = 2;
    M2C_FIELD(work, s32 *, 0x7784) = 75;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    member_object = *GetBattleObjectSlotFar(
        M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 8));

    particle = (void *)0x02010000;
    i = 0;
    do {
        u32 rand1;
        u32 angle;
        s32 radius;
        s32 sin_val;
        s32 cos_val;

        rand1 = Random16() & 0x3FF;
        angle = Random16() & 0xFFFF;
        M2C_FIELD(particle, s32 *, 0) = M2C_FIELD(member_object, s32 *, 8);
        M2C_FIELD(particle, s32 *, 4) =
            M2C_FIELD(member_object, s32 *, 12) + 0x50000;
        M2C_FIELD(particle, s32 *, 8) = M2C_FIELD(member_object, s32 *, 16);
        sin_val = Trig_Sin((s32)angle);
        radius = rand1 + 32;
        M2C_FIELD(particle, s32 *, 0xC) = (sin_val * radius) >> 8;
        M2C_FIELD(particle, s32 *, 0x10) =
            ((s32)(Random16() & 0xFF) - 32) << 9;
        cos_val = Trig_Cos((s32)angle);
        M2C_FIELD(particle, s32 *, 0x14) = -(cos_val * radius * 2) >> 8;
        M2C_FIELD(particle, s32 *, 0x18) =
            (s32)(Random16() & 0x1F) + 48;
        if (mode == 0) {
            M2C_FIELD(particle, s32 *, 0xC) =
                M2C_FIELD(particle, s32 *, 0xC) / 2;
            M2C_FIELD(particle, s32 *, 0x14) =
                M2C_FIELD(particle, s32 *, 0x14) / 2;
        }
        i++;
        particle = (u8 *)particle + 28;
    } while (i != 256);

    look_target = facing + 12;
    outer = 0;
    do {
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, look_target);

        particle = (void *)0x02010000;
        j = 0;
        do {
            s32 threshold;

            threshold = (j / 32) * 8;
            if (outer >= threshold) {
                s32 phase;

                phase = M2C_FIELD(particle, s32 *, 0x18);
                if (phase >= 0) {
                    s32 z;
                    s32 offset;
                    s32 idx;
                    s32 doubled;
                    s32 half;
                    void *src_addr;
                    s32 x;
                    s32 y;

                    position[0] = M2C_FIELD(particle, s32 *, 0)
                        + (Trig_Sin(((j * 4) + phase) << 10) << 4);
                    position[1] = M2C_FIELD(particle, s32 *, 4);
                    position[2] = M2C_FIELD(particle, s32 *, 8);
                    EffectPosition_ApplyBaseAndYOffset(position, screen);

                    screen[0] = screen[0] >> 1;
                    z = screen[2];
                    if (z <= 313) {
                        z = 314;
                        screen[2] = 314;
                    }
                    if (z > 634) {
                        screen[2] = 634;
                        z = 634;
                    }

                    offset = z - 314;
                    if (offset < 0) {
                        offset = z - 251;
                    }
                    idx = 6 - (offset >> 6);
                    doubled = idx * 2;
                    src_addr = (u8 *)extra_target + ParticleStreams_CellOffsets[idx - 1];
                    half = idx / 2;
                    x = screen[0] - half;
                    y = screen[1] - idx;
                    draw_rectangle_fn(draw_destination, src_addr, x, y,
                        idx, doubled);

                    EffectStep_AdvanceWithGravity3D(particle, 62, 1024);

                    if (mode == 1) {
                        if (M2C_FIELD(member_object, s32 *, 8) < 0) {
                            M2C_FIELD(particle, s32 *, 0xC) =
                                M2C_FIELD(particle, s32 *, 0xC) + 8192;
                        } else {
                            M2C_FIELD(particle, s32 *, 0xC) =
                                M2C_FIELD(particle, s32 *, 0xC) - 8192;
                        }
                    }

                    M2C_FIELD(particle, s32 *, 0x18) = phase - 1;
                }
            }
            j++;
            particle = (u8 *)particle + 28;
        } while (j != 128);

        if (mode == 1) {
            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)
                    != 0) {
                s32 stagger;

                member_id_offset = 36;
                stagger = 48;
                do {
                    if (outer == stagger) {
                        s32 member_id;

                        BattleEventRuntime_BeginPhaseFar(-1);
                        member_id = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *,
                            member_id_offset);
                        ObjectGroup_UpdateMembers(member_id, 7, 5, member, 8);
                    }
                    member++;
                    member_id_offset += 2;
                    stagger += 8;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        20));
            }
        } else {
            member = 0;
            if (M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *, 20)
                    != 0) {
                s32 stagger;

                member_id_offset = 36;
                stagger = 48;
                do {
                    if (outer == stagger) {
                        s32 member_id;

                        Audio_PlayCue(126);
                        BattleEventRuntime_BeginPhaseFar(-1);
                        member_id = M2C_FIELD(
                            M2C_FIELD(work, void **, 0x7828), s16 *,
                            member_id_offset);
                        ObjectGroup_UpdateMembers(member_id, 7, -1, member, 8);
                    }
                    member++;
                    member_id_offset += 2;
                    stagger += 8;
                } while (member
                    != M2C_FIELD(M2C_FIELD(work, void **, 0x7828), s32 *,
                        20));
            }
        }

        ObjectGroup_TickMemberTimers();
        M2C_FIELD(work, s32 *, 0x7824) = 1;
        WaitFrames(1);

        outer++;
    } while (outer != 128);

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
