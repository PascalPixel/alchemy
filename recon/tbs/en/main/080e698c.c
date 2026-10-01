#include "TYPES.H"
#include "RESOURCE_IDS.H"
#include "BATTLE_EFX.H"

/*
 * Draft for the battle-presentation sub-effect at 0x080e698c.
 *
 * Assigned family template: games/THE BROKEN SEAL/SRC/BATTLE/EFFECT/MEMBER_ORBIT.C
 * (main:080ce85c, template-main-080ce85c, family score 8067/10000).  Like
 * the measured siblings 080d59b0/080d82b0/080dc1ec/080e01e4/080e7404 in
 * this same 0x03001eec "battle work" subsystem, this owner's callee set,
 * work+0x7780/0x7784/0x7824/0x7828 field layout, and the M2C_FIELD /
 * DrawRectangleFn conventions those files established carry over directly.
 * The body itself is a materially different sub-effect from the template,
 * though: this owner runs a fixed 70-frame loop (not member_count*16+48),
 * never reads a member count at all, and spends its prologue computing a
 * distance/velocity setup between two specific tracked members (the ids at
 * target+8 and (s16)target+36) rather than looping over a member list.
 *
 * _call_via_r2 is the r2-slot `_call_via_rN` veneer at
 * recon/tbs/raw/080072e4.s (0x080072e4 + 4*2).  The retained assembly loads
 * 0x030001D8 -- the same relocated IWRAM square-root routine documented in
 * games/THE BROKEN SEAL/src/math/fixed_sqrt.c and games/THE BROKEN SEAL/src/unidentified/main/
 * battle/battle_owner_52.c -- into r2 immediately before the call, so it is
 * modeled the same way those files model their own veneer slot: a direct
 * call to the veneer's own symbol with the real jump target passed as a
 * trailing argument.  The middle argument is never assigned between the
 * preceding __divsi3 call and this call in the retained assembly (no
 * instruction touches r1 in between), so it is passed uninitialized here,
 * matching FixedSqrt's own "unused1"/"unused2" idiom for the identical
 * situation.
 *
 * EffectPosition_ApplyAlternateStepAndYOffset is EffectPosition_ApplyAlternateStepAndYOffset
 * (games/THE BROKEN SEAL/INCLUDE/TYPES.H), sibling of EffectPosition_ApplyBaseAndYOffset
 * (EffectPosition_ApplyBaseAndYOffset, used by the template).  Its first
 * argument here is read from target+8 as a plain s32 -- the same field
 * passed directly to GetBattleObjectSlotFar earlier in this owner -- so it is
 * modeled as taking a member id rather than a position-record pointer.
 */


void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_FetchRectangleBlitters(s32 flag, DrawRectangleFn *out_pair);
void **GetBattleObjectSlotFar(s32 member_id);
s32 __divsi3(s32 numerator, s32 denominator);
s32 _call_via_r2(s32 a, s32 b, s32 target);
void Object_ResetMotion(void *object);
void Object_SetMoveTargetFar(void *object, s32 x, s32 y, s32 z);
void Object_SetMode(void *object, s32 mode);
s32 Scheduler_AddOrUpdateCallback(void *callback, s32 interval);
void Scheduler_RemoveCallback(void *callback);
void EffectPosition_ApplyAlternateStepAndYOffset(s32 member_id, void *screen);
void BattleEventRuntime_BeginPhaseFar(s32 id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void BattleMotion_ApplyVariantMotionFar(s32 member_id, s32 mode);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void EffectStep_AdvanceWithGravity2D(void *particle, s32 count, s32 flags);
s32 __modsi3(s32 a, s32 b);
void Camera_ApplyShake(s32 a, s32 b);
void ObjectGroup_TickMemberTimers(void);
void WaitFrames(s32 frames);
void Runtime_ReleaseHeapBlock(s32 id);
s32 BattleFx_EndCanvasLayer(void);

extern const u16 Data_080eee02[];
extern const s8 Data_080eee10[];
extern const s8 Data_080eee17[];
extern const u8 Data_080eedf4[];
extern const u8 Data_080eedfb[];

s32 Unnamed_080e698c(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *draw_destination;
    void *extra_target;
    void *member_a;
    void *member_b;
    DrawRectangleFn draw_pair[2];
    s32 dx;
    s32 dz;
    s32 dx_scaled;
    s32 dz_scaled;
    s32 speed;
    s32 leftover;
    s32 new_x;
    s32 new_z;
    s32 frame;
    s32 screen[3];

    heap_cache = (void **)0x03001EEC;
    cursor = heap_cache;
    work = *cursor++;
    draw_destination = *cursor;
    extra_target = heap_cache[2];
    (*(void **)((u8 *)work + 0x7828)) = object;
    BattleFx_BeginCanvasLayer(0);
    (*(s16 *)((u8 *)((void *)0x04000020) + 0)) = 0x100;
    Resource_LoadAndDecompress((s32)&ResourceId_ParticleSpritesA, extra_target, 0, 0);
    Resource_LoadAndDecompress((s32)&ResourceId_FireSwirlSheet, work, 1, 1);
    Resource_LoadAndDecompress((s32)&ResourceId_PinkStarSheet, (u8 *)work + 0x3E80, 1, 0);
    BattleFx_FetchRectangleBlitters(
        (*(s32 *)((u8 *)((*(void **)((u8 *)work + 0x7828))) + 4)), draw_pair);

    (*(s32 *)((u8 *)work + 0x7780)) = 2;
    (*(s32 *)((u8 *)work + 0x7784)) = 50;
    Scheduler_AddOrUpdateCallback((void *)0x080CD261, 0x480);

    {
        s32 *reset_cursor;
        s32 i;

        reset_cursor = (s32 *)0x02010018;
        i = 0;
        do {
            i++;
            *reset_cursor = 0;
            reset_cursor += 7;
        } while (i != 1024);
    }

    member_a = *GetBattleObjectSlotFar(
        (*(s32 *)((u8 *)((*(void **)((u8 *)work + 0x7828))) + 8)));
    member_b = *GetBattleObjectSlotFar(
        (*(s16 *)((u8 *)((*(void **)((u8 *)work + 0x7828))) + 36)));
    new_x = (*(s32 *)((u8 *)member_a + 8));
    dx = (*(s32 *)((u8 *)member_b + 8)) - new_x;
    dx_scaled = __divsi3(dx * 80, 100);
    dz = (*(s32 *)((u8 *)member_b + 16)) - (*(s32 *)((u8 *)member_a + 16));
    dz_scaled = __divsi3(dz * 80, 100);
    new_x = new_x + dx_scaled;
    new_z = (*(s32 *)((u8 *)member_a + 16)) + dz_scaled;
    speed = __divsi3(
        _call_via_r2(
            ((dx_scaled >> 8) * (dx_scaled >> 8))
                + ((dz_scaled >> 8) * (dz_scaled >> 8)),
            leftover, 0x030001D8)
            << 8,
        20);
    (*(s32 *)((u8 *)member_a + 0x34)) = speed;
    (*(s32 *)((u8 *)member_a + 0x30)) = speed;
    (*(s8 *)((u8 *)member_a + 0x58)) = 1;
    (*(s32 *)((u8 *)member_a + 0x28)) = 0x70000;
    (*(s32 *)((u8 *)member_a + 0x48)) = 0xDEB8;
    (*(s32 *)((u8 *)member_a + 0x44)) = 0;
    (*(s8 *)((u8 *)member_a + 0x5A)) = 1;
    Object_ResetMotion(member_a);
    Object_SetMoveTargetFar(member_a, new_x, 0, new_z);
    Object_SetMode(member_a, 2);

    for (frame = 0; frame != 70; frame++) {
        EffectPosition_ApplyAlternateStepAndYOffset(
            (*(s32 *)((u8 *)((*(void **)((u8 *)work + 0x7828))) + 8)), screen);
        (*(s32 *)((u8 *)((void *)0x04000028) + 0)) = (0x50 - screen[0]) << 8;

        if ((u32)(frame - 8) <= 15) {
            s32 index;

            index = (frame - 8) / 2;
            if (index > 6) {
                index = 6;
            }
            if ((*(s32 *)((u8 *)((*(void **)((u8 *)work + 0x7828))) + 4)) == 0) {
                draw_pair[0](
                    draw_destination,
                    (u8 *)work + Data_080eee02[index],
                    Data_080eee10[index] + 30,
                    (Data_080eee17[index] + screen[1]) - 60,
                    Data_080eedf4[index], Data_080eedfb[index]);
            } else {
                s32 width;

                width = Data_080eedf4[index];
                draw_pair[0](
                    draw_destination,
                    (u8 *)work + Data_080eee02[index],
                    (-Data_080eee10[index] - width) + 108,
                    (Data_080eee17[index] + screen[1]) - 60,
                    width, Data_080eedfb[index]);
            }
        }

        if (frame == 18) {
            u8 *particle;
            s32 i;

            BattleEventRuntime_BeginPhaseFar(134);
            ObjectGroup_UpdateMembers(
                (*(s16 *)((u8 *)((*(void **)((u8 *)work + 0x7828))) + 36)),
                7, 5, 0, 8);
            BattleMotion_ApplyVariantMotionFar(
                (*(s16 *)((u8 *)((*(void **)((u8 *)work + 0x7828))) + 36)), 6);
            (*(s32 *)((u8 *)work + 0x77A8)) = 4;

            particle = (u8 *)0x02010000;
            for (i = 0; i != 16; i++) {
                s32 magnitude;
                u32 angle;

                magnitude = (Random16() & 63) + 256;
                angle = Random16() & 0xFFFF;
                (*(s32 *)((u8 *)particle + 0)) = 0x400000;
                (*(s32 *)((u8 *)particle + 4)) = 0x500000;
                (*(s32 *)((u8 *)particle + 12)) =
                    (magnitude * Trig_Sin((s32)angle)) >> 7;
                (*(s32 *)((u8 *)particle + 16)) =
                    -(magnitude * Trig_Cos((s32)angle)) >> 6;
                (*(s32 *)((u8 *)particle + 24)) = (Random16() & 15) + 16;
                particle += 28;
            }
        }

        {
            u8 *particle;
            s32 i;

            particle = (u8 *)0x02010000;
            for (i = 0; i != 128; i++) {
                s32 lifetime;

                lifetime = (*(s32 *)((u8 *)particle + 24));
                if (lifetime > 0) {
                    (*(s32 *)((u8 *)particle + 24)) = lifetime - 1;
                    EffectStep_AdvanceWithGravity2D(particle, 60, 0);
                    if ((*(s32 *)((u8 *)particle + 4)) > 0x680000) {
                        (*(s32 *)((u8 *)particle + 16)) =
                            -(*(s32 *)((u8 *)particle + 16)) / 2;
                    } else {
                        s32 x;
                        s32 y;

                        x = (*(s32 *)((u8 *)particle + 0));
                        y = (*(s32 *)((u8 *)particle + 4));
                        if ((u32)x <= 0x7EFFFFU && y >= 0) {
                            s32 rx;
                            s32 ry;
                            s32 cell;

                            rx = x >> 16;
                            ry = y >> 16;
                            cell = frame + i;
                            if (cell < 0) {
                                cell += 3;
                            }
                            cell = __modsi3(cell >> 2, 6);
                            draw_pair[0](
                                draw_destination,
                                (u8 *)work + 0x3E80 + (cell << 8),
                                rx - 8, ry - 8, 16, 16);
                        }
                    }
                }
                particle += 28;
            }
        }

        Camera_ApplyShake(8, 8);
        ObjectGroup_TickMemberTimers();
        (*(s32 *)((u8 *)work + 0x7824)) = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((void *)0x080CD261);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    return BattleFx_EndCanvasLayer();
}
