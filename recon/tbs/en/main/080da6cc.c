/* Draft, not exact (2026-09-24): candidate=1192 reference=1192 differing_halfwords=534. Constants the reference loads from
   the literal pool are spelled as link-time Value_ symbols, which restores
   the reference size; wraps marked FAKEMATCH only move scheduling. */
#include "TYPES.H"
#include "RESOURCE_IDS.H"
extern u8 Value_00007828;
extern u8 Value_00001010;
extern u8 Value_000077ac;
extern u8 Value_000077b0;
extern u8 Value_00000480;
extern u8 Value_00007780;
extern u8 Value_00007784;
extern u8 Value_00000100;
extern u8 Value_0000031f;
extern u8 Value_000077a8;
extern u8 Value_00007824;
#include "BATTLE_EFX.H"

/*
 * Battle-presentation sub-effect at 0x080da6cc.  The family matcher's
 * closest structural template is games/THE BROKEN SEAL/src/battle/effects/member_orbit/
 * run.c (main:080ce85c, already exact), but this owner's size (1192 bytes
 * vs. the template's 724) shows it is a genuinely different sub-effect: a
 * per-member burst of falling "star" particles rather than an orbiting
 * ring.  Field offsets, the M2C_FIELD macro, the DrawRectangleFn calling
 * convention (an indirect call through the r4 slot of the _call_via_rN
 * trampoline at recon/tbs/raw/080072e4.s -- see recon/tbs/en/main/
 * 080dc1ec.json's score.note for the full derivation), and most callee
 * signatures come from that template and from games/THE BROKEN SEAL/src/battle/
 * effects/puff_arc/run.c (also exact) and the measured-draft siblings
 * 080e01e4.c / 080d82b0.c in this same 0x03001eec "battle work" family.
 *
 * Sixty-four Star records (at 0x02010000, 28 bytes each) are seeded once
 * from a single member's screen position with random velocities.  Each
 * animation frame, a per-variant subset of them (the raw count at
 * Data_080eea41[variant]) is checked: once `idx / 2` frames have passed a
 * still-unreleased (state == -1) star draws a shrinking trail sprite and
 * drifts by its own velocity (kind 47 while still within its scheduled
 * window, kind 46 once 48 frames overdue); once 48 frames overdue it is
 * also re-homed every frame toward its member's live screen position
 * (damped for the first 37 of those frames), and once it crosses above the
 * top of the screen (y < 0) it "lands": state flips to 0 and a second,
 * member-independent scan over all 64 stars draws a fixed twelve-frame
 * "landed" glint sequence (kind 47, keyed by state / 2) through four small
 * per-frame tables.
 */


typedef struct Star {
    s32 x;
    s32 y;
    s32 z;
    s32 vx;
    s32 vy;
    s32 vz;
    s32 state;
} Star;

extern u8 gWorkSlot[];
extern const u16 ParticleStreams_CellOffsets[];
extern const u8 Data_080eea41[];
extern const u8 Data_080eea44[];
extern const u8 Data_080eea4a[];
extern const u8 Data_080eea50[];
extern const u16 Data_080eea56[];

void BattleFx_BeginCanvasLayer(s32 mode);
void **GetBattleObjectSlotFar(s32 member_id);
s32 Battle_GetObjectTableValueFar(s32 member_id);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void BattlePres_SetupTransitionSceneFar(s32 a, s32 b, s32 c, s32 d);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void EffectPosition_ApplyBaseAndYOffset(void *source, void *screen);
s32 __modsi3(s32 a, s32 b);
void Audio_PlayCue(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);

void Unnamed_080da6cc(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    s32 facing;
    s32 facing2;
    s32 status;
    void *rectangle[2];
    void **rectangle_slot;
    void *second_rectangle;
    void *member_obj;
    s32 y0;
    Star *star;
    s32 n;
    s32 frame;

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    facing = *(s32 *)((u8 *)heap_cache - 108);
    extra_target = heap_cache[2];
    (*(void **)((u8 *)(work) + 0x7828)) = object;
    if ((*(s32 *)((u8 *)(object) + 4)) == 1) {
        BattleFx_BeginCanvasLayer(1);
    } else {
        BattleFx_BeginCanvasLayer(0);
    }
    Resource_LoadAndDecompress((s32) &ResourceId_EmberStreakSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32) &ResourceId_ParticleSpritesA, extra_target, 0, 0);
    status = BattleEffect_LoadWork(46, 7, 7, 3, 3);
    rectangle[0] = *(void **) (gWorkSlot + 46 * 4);
    status = BattleEffect_LoadWork(47, 7, 7, 3, 2);
    second_rectangle = *(void **) (gWorkSlot + 47 * 4);
    rectangle_slot = rectangle;
    rectangle_slot[1] = second_rectangle;
    *(s16 *) 0x04000052 = 0x1010;

    member_obj = *GetBattleObjectSlotFar(
        (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 8)));
    y0 = (*(s32 *)((u8 *)(member_obj) + 12))
        + Battle_GetObjectTableValueFar((*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 8)));

    star = (Star *) 0x02010000;
    n = 0;
    do {
        u32 rand1;
        s32 mag;

        rand1 = Random16();
        mag = (Random16() & 127) + 127;
        star->vx = (mag * Trig_Sin((s32) rand1)) >> 6;
        star->vy = (((Random16() & 127) - 16) << 16) >> 6;
        star->vz = (mag * Trig_Cos((s32) rand1)) >> 6;
        star->x = (*(s32 *)((u8 *)(member_obj) + 8));
        star->y = y0;
        star->z = (*(s32 *)((u8 *)(member_obj) + 16));
        n++;
        star->state = -1;
        star++;
    } while (n != 64);

    (*(s32 *)((u8 *)(work) + ((s32)&Value_000077ac))) = 0;
    (*(s32 *)((u8 *)(work) + 0x77B0)) = 0;
    Scheduler_AddOrUpdateCallback((void *) 0x080D6505, 0x480);
    (*(s32 *)((u8 *)(work) + ((s32)&Value_00007780))) = 2;
    (*(s32 *)((u8 *)(work) + 0x7784)) = 0x4B;
    Scheduler_AddOrUpdateCallback((void *) 0x080CD261, 0x480);

    frame = 0;
    if ((Data_080eea41[
            (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18))] >> 1)
            != -132) {
        facing2 = facing + 12;
        do {
            s32 variant;

            (*(s32 *)((u8 *)(work) + ((s32)&Value_000077ac))) =
                ((u32) (frame - 17) <= 62) ? 256 : 0;

            variant = (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18));
            if (frame == (Data_080eea41[variant] >> 1) + 108) {
                BattleEventRuntime_BeginPhaseFar(133);
            }

            BattlePres_SetupTransitionSceneFar(0, 0, 0, 100);
            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork(facing, facing2);

            if (Data_080eea41[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18))] != 0) {
                s32 idx;
                Star *cur;

                idx = 0;
                cur = (Star *) 0x02010000;
                do {
                    s32 half_idx;
                    s32 screen[3];

                    half_idx = idx / 2;

                    if (frame > half_idx) {
                        if (cur->state == -1) {
                            s32 dist;
                            s32 size;
                            s32 size2;
                            s32 slot;

                            EffectPosition_ApplyBaseAndYOffset(cur, screen);
                            dist = screen[2];
                            screen[0] = screen[0] >> 1;
                            if (dist <= 159) {
                                screen[2] = 160;
                            }
                            if (dist > 799) {
                                screen[2] = (s32)&Value_0000031f;
                                dist = 799;
                            }
                            size = 10 - ((dist - 160) / 64);
                            slot = (frame < half_idx + 48) ? 4 : 0;
                            size2 = size * 2;
                            ((DrawRectangleFn) *(void **)
                                ((u8 *) rectangle_slot + slot))(
                                draw_destination,
                                (u8 *) extra_target
                                    + ParticleStreams_CellOffsets[size - 1],
                                screen[0] - (size / 2),
                                screen[1] - size,
                                size, size2);
                            cur->x += cur->vx;
                            cur->y += cur->vy;
                            cur->z += cur->vz;
                        }
                    }

                    if (frame > half_idx + 48) {
                        if (cur->state == -1) {
                            s32 member_index;
                            s32 member_id;
                            void *member_obj2;
                            s32 vx;
                            s32 vy;
                            s32 vz;

                            member_index = __modsi3(idx,
                                (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x14)));
                            member_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (36 + member_index * 2)));
                            member_obj2 = *GetBattleObjectSlotFar(member_id);

                            vx = cur->vx
                                + (((*(s32 *)((u8 *)(member_obj2) + 8)) - cur->x)
                                    >> 9);
                            cur->vx = vx;
                            vy = cur->vy
                                + (((*(s32 *)((u8 *)(member_obj2) + 12)) - cur->y)
                                    >> 9);
                            cur->vy = vy;
                            vz = cur->vz
                                + (((*(s32 *)((u8 *)(member_obj2) + 16)) - cur->z)
                                    >> 9);
                            cur->vz = vz;

                            if (frame < half_idx + 85) {
                                cur->vx = (vx * 60) / 64;
                                cur->vy = (vy * 60) / 64;
                                cur->vz = (vz * 60) / 64;
                            }

                            if (cur->y < 0) {
                                s32 landed_index;
                                s32 landed_id;

                                cur->state = 0;
                                cur->x = screen[0];
                                cur->y = screen[1];
                                Audio_PlayCue(136);
                                landed_index = __modsi3(idx,
                                    (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x14)));
                                landed_id = (*(s16 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + (36 + landed_index * 2)));
                                ObjectGroup_UpdateMembers(landed_id, 10, 5, landed_index, 4);
                                (*(s32 *)((u8 *)(work) + 0x77A8)) = 2;
                            }
                        }
                    }

                    idx++;
                    cur++;
                } while (idx != Data_080eea41[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18))]);
            }

            {
                s32 m;
                Star *cur2;

                m = 0;
                if (Data_080eea41[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18))]
                        != 0) {
                    cur2 = (Star *) 0x02010000;
                    do {
                        if ((u32) cur2->state <= 11) {
                            s32 sidx;
                            void *src;
                            u32 w;
                            s32 x;
                            s32 y;

                            sidx = cur2->state / 2;
                            src = (u8 *) work + Data_080eea56[sidx];
                            w = Data_080eea44[sidx];
                            x = cur2->x - (w >> 1);
                            y = (cur2->y + Data_080eea50[sidx]) - 56;
                            ((DrawRectangleFn) rectangle_slot[1])(
                                draw_destination, src,
                                x, y, w, Data_080eea4a[sidx]);
                            cur2->state += 1;
                        }
                        m++;
                        cur2++;
                    } while (m != Data_080eea41[(*(s32 *)((u8 *)((*(void **)((u8 *)(work) + 0x7828))) + 0x18))]);
                }
            }

            if ((*(s32 *)((u8 *)(work) + 0x77B0)) == 0) {
                (*(s32 *)((u8 *)(work) + ((s32)&Value_000077b0))) = 1;
            }
            Camera_ApplyShake(8, 8);
            ObjectGroup_TickMemberTimers();
            (*(s32 *)((u8 *)(work) + 0x7824)) = 1;
            WaitFrames(1);

            frame++;
        } while (frame != (
            (Data_080eea41[
                (*(s32 *)((u8 *)((*(void **)((u8 *)(work) + ((s32)&Value_00007828)))) + 0x18))] >> 1)
            + 132));
    }

    Scheduler_RemoveCallback((void *) 0x080CD261);
    Scheduler_RemoveCallback((void *) 0x080D6505);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}
