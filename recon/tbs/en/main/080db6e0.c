#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"

/*
 * Battle/overlay particle-field effect owner at 0x080db6e0.  The retained
 * assembly already names the global entry point RunParticleFieldEffect, so
 * that name is kept in recon/tbs/source-paths.json, but the working symbol
 * here is RunParticleFieldEffect so the family-transplant/candidate-show tooling can
 * find it by address like every other in-progress owner.
 *
 * Structural template: games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C
 * (owner 080ce85c, already exact) establishes the 0x03001eec heap_cache /
 * work-pointer prologue, the M2C_FIELD(work, 0x7828) object-pointer field,
 * the Value_ pool-symbol convention for small link-time byte constants, and
 * the DrawRectangleFn / WordCopyFn trampoline call shapes.  This owner is a
 * different, size-mismatched sibling (1092 bytes vs the template's own
 * size): it drives no member-orbit sprites and no per-frame sine sweep at
 * all.  Instead it runs two independently-seeded particle arrays (32
 * entries at work+0x7080, 1024 entries at the fixed EWRAM buffer
 * 0x02010000, both stride 0x1C) through a per-frame while loop whose
 * iteration count, and whose two active-particle counts, come from a
 * 3-bytes-per-row lookup table at 0x080eeae2 indexed by a runtime "mode"
 * selector (mode 1 is forced by the palette variant; other variants read
 * the mode from the caller's object at offset 24).  recon/tbs/en/
 * main/080e7404.c is the fuller sibling draft in this same 0x03001eec
 * family: its 128- and 512- record particle initializers at 0x02010000 /
 * 0x02010e00 (masked-RNG field 0xC/0x10/0x14 triples) are the same
 * structural shape as this owner's two initializer loops, and its
 * EffectStep_AdvanceWithGravity3D / Resource_LoadAndDecompress / gWorkSlot / ParticleStreams_CellOffsets usages are
 * reused here verbatim.  Following that draft's own established style,
 * every temporary below is declared flat at the top of the function
 * (never in a nested block) so the compiler's size-class frame allocator
 * lays the stack out the same way the reference does.
 */

typedef s32 (*WordCopyFn)(void *dest, const void *src, s32 words);

/* Heap-allocation cache: gWorkSlot[kind] holds kind's block address.
   This owner reads kind 46 only (its single rectangle-blit routine). */
extern void *gWorkSlot[];

/* Ten halfword cell offsets indexed by a clamped depth bucket 0..9; the
   same symbol recon/tbs/en/main/080e7404.c already declares and
   indexes the same way. */
extern u16 ParticleStreams_CellOffsets[];

/* Per-mode 3-byte row: [far_active_count, near_active_count, total_frames],
   selected by the mode/variant value this owner computes into `mode`. */
extern u8 Data_080eeae2[];

/* Sprite-offset / size halfword pair tables, indexed together by the same
   __divsi3 selection while a near-field particle is still growing. */
extern u16 Data_080eeaec[];
extern u16 Data_080eeafa[];


void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void EffectPosition_ApplyBaseAndYOffset(void *source, void *screen);
void EffectStep_AdvanceWithGravity3D(void *record, s32 a, s32 b);
s32 Random16(void);
s32 __divsi3(s32 a, s32 b);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 mode);
void WaitFrames(s32 frames);
void Audio_PlayCue(s32 id);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);

void RunParticleFieldEffect(void *object, s32 variant)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    s32 status;
    s32 mode;
    void *draw_rectangle;
    s32 frame;
    s32 base;
    s32 far_idx;
    s32 near_idx;
    s32 total_idx;
    s32 facing;
    s32 i;
    s16 *pal;
    s32 gray;
    void *entry;
    void *particle;
    s32 screen[3];
    s32 depth;
    s32 index;
    s32 size;
    s32 x;
    s32 age;
    s32 which;
    s32 sprite_off;
    s32 member_count;
    s32 member_id_offset;
    s16 member_id;
    u32 half;
    u8 *table;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    (*(void **)((u8 *)(work) + 0x7828)) = object;
    BattleFx_BeginCanvasLayer(1);
    Resource_LoadAndDecompress((s32)&ResourceId_BlastSheet, work, 1, 0);
    if (variant == 1) {
        i = 0;
        pal = (s16 *)0x05000000;
        do {
            gray = i / 2;
            *pal = (s16) ((gray << 10) | (gray << 5) | gray);
            i++;
            pal++;
        } while (i != 64);
        mode = 1;
    } else {
        palette = Resource_GetTableEntry((s32)&ResourceId_FireStreakSheet);
        status = ((WordCopyFn)0x03001388)((void *)0x05000000, palette, 128);
        mode = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 24));
    }
    entry = (u8 *)work + 0x7080;
    i = 0;
    do {
        if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 4)) == 1) {
            status = 200 << 14;
        } else {
            status = -(200 << 14);
        }
        (*(s32 *)((u8 *)(entry) + 0)) = status;
        (*(s32 *)((u8 *)(entry) + 4)) = 0;
        (*(s32 *)((u8 *)(entry) + 8)) = 0;
        (*(s32 *)((u8 *)(entry) + 12)) =
            (s32) (((Random16() & 63) - 32) << 13);
        (*(s32 *)((u8 *)(entry) + 16)) =
            (s32) (((Random16() & 63) + 16) << 12);
        (*(s32 *)((u8 *)(entry) + 20)) =
            (s32) (((Random16() & 63) - 32) << 13);
        i++;
        (*(s32 *)((u8 *)(entry) + 24)) = 0;
        entry = (u8 *)entry + 28;
    } while (i != 32);
    entry = (void *)0x02010000;
    i = 0;
    do {
        if ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 4)) == 1) {
            status = 200 << 14;
        } else {
            status = -(200 << 14);
        }
        (*(s32 *)((u8 *)(entry) + 0)) = status;
        (*(s32 *)((u8 *)(entry) + 4)) = 0;
        (*(s32 *)((u8 *)(entry) + 8)) = 0;
        (*(s32 *)((u8 *)(entry) + 12)) =
            (s32) (((Random16() & 63) - 32) << 13);
        (*(s32 *)((u8 *)(entry) + 16)) =
            (s32) (((Random16() & 31) + 8) << 13);
        (*(s32 *)((u8 *)(entry) + 20)) =
            (s32) (((Random16() & 63) - 32) << 13);
        i++;
        (*(s32 *)((u8 *)(entry) + 24)) = 0;
        entry = (u8 *)entry + 28;
    } while (i != 1024);
    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    draw_rectangle = gWorkSlot[46];
    (*(s32 *)((u8 *)(work) + 0x7780)) = 2;
    (*(s32 *)((u8 *)(work) + 0x7784)) = 75;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    base = mode * 2;
    far_idx = base + mode;
    near_idx = far_idx + 1;
    total_idx = far_idx + 2;
    table = Data_080eeae2;
    frame = 0;
    while (frame != table[total_idx]) {
        facing = *(s32 *)0x03001E80;
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        if (frame == 2) {
            Audio_PlayCue(144);
        }
        if (frame == Data_080eeae2[base + mode + 2] - 48) {
            BattleEventRuntime_BeginPhaseFar(133);
        }
        if (table[far_idx] != 0) {
            particle = (void *)0x02010000;
            i = 0;
            do {
                if ((*(s32 *)((u8 *)(particle) + 4)) >= 0) {
                    EffectPosition_ApplyBaseAndYOffset(particle, screen);
                    screen[0] = screen[0] >> 1;
                    screen[0] = screen[0]
                        + ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 4)) << 5)
                        - 16;
                    if (screen[2] <= 159) {
                        screen[2] = 160;
                    }
                    if (screen[2] > 0x31F) {
                        screen[2] = 0x31F;
                    }
                    depth = screen[2] - 160;
                    if (depth < 0) {
                        depth += 63;
                    }
                    depth >>= 6;
                    index = 9 - depth;
                    size = index * 2;
                    ((DrawRectangleFn)draw_rectangle)(
                        canvas,
                        (u8 *)work + ParticleStreams_CellOffsets[index - 1]
                            + ((i & 1) * 0x302) + 0x3200,
                        screen[0] - (index / 2), screen[1] - index,
                        index, size);
                    EffectStep_AdvanceWithGravity3D(particle, 64, 0xFFFFE000);
                }
                i++;
                particle = (u8 *)particle + 28;
            } while (i != table[far_idx]);
        }
        if (frame > 2 && table[near_idx] != 0) {
            particle = (u8 *)work + 0x7080;
            i = 0;
            do {
                if (i < frame && (*(s32 *)((u8 *)(particle) + 4)) >= 0) {
                    EffectPosition_ApplyBaseAndYOffset(particle, screen);
                    x = screen[0] >> 1;
                    x = x
                        + ((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 4)) << 5)
                        - 16;
                    age = (*(s32 *)((u8 *)(particle) + 24));
                    if ((u32)age <= 20) {
                        which = __divsi3(age, 3);
                        sprite_off = Data_080eeaec[which];
                        size = Data_080eeafa[which];
                        half = (u32) size >> 1;
                        ((DrawRectangleFn)draw_rectangle)(canvas,
                            (u8 *)work + sprite_off,
                            x - (s32) half,
                            screen[1] - (s32) half,
                            size, size);
                    }
                    age = (*(s32 *)((u8 *)(particle) + 24));
                    if (age <= 20) {
                        (*(s32 *)((u8 *)(particle) + 24)) = age + 1;
                    }
                    EffectStep_AdvanceWithGravity3D(particle, 64, 0xFFFFE000);
                }
                i++;
                particle = (u8 *)particle + 28;
            } while (i != table[near_idx]);
        }
        if (variant == 0) {
            member_count =
                (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 20));
            if (member_count != 0) {
                i = 0;
                member_id_offset = 36;
                do {
                    if (frame == i + 6) {
                        member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (member_id_offset)));
                        ObjectGroup_UpdateMembers(member_id, 7, 5, i, 10);
                        member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (member_id_offset)));
                        BattleMotion_ApplyVariantMotionFar(member_id, 2);
                    }
                    i++;
                    member_id_offset += 2;
                    member_count = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 20));
                } while (i != member_count);
            }
        } else {
            member_count =
                (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 20));
            if (member_count != 0) {
                i = 0;
                member_id_offset = 36;
                do {
                    if (frame == i + 6) {
                        member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (member_id_offset)));
                        ObjectGroup_UpdateMembers(member_id, 7, 5, i, 10);
                    }
                    i++;
                    member_id_offset += 2;
                    member_count = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 20));
                } while (i != member_count);
            }
        }
        if (frame == 2) {
            (*(s32 *)((u8 *)(work) + 0x77A8)) = 6;
        }
        Camera_ApplyShake(16, 16);
        ObjectGroup_TickMemberTimers();
        (*(s32 *)((u8 *)(work) + 0x7824)) = 1;
        WaitFrames(1);
        frame++;
    }
    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
