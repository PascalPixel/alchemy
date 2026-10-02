#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "BATTLE_EFX.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "RESOURCE_IDS.H"
#include "SCENE.H"
#include "RESOURCE.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "MOTION_OBJECT.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
extern u8 gCameraWork[];
void BattleFx_ArmBg2AffineHBlankDma(void);
void BattlePresentation_ProcessPendingGraphicsTransfer(void);

/*
 * Battle-presentation sub-effect: entry 34 of the effect callback table at
 * 0x080ee2b4.  The single argument is the effect state pointer, which the
 * owner republishes at work + 0x7828.
 */
#define FIELD_AT_OFFSET(expr, type_ptr, offset) \
    (*(type_ptr)((u8 *)(expr) + (offset)))
void BattleFx_BeginCanvasLayer(s32 mode);
void *Resource_GetTableEntry(s32 id);
u32 Resource_DecodeType01(const void *source, void *destination);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 a, s32 b);
void **GetBattleObjectSlotFar(s32 member_id);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
s32 BattleFx_EndCanvasLayer(void);

void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode);

/* A small absolute link-time constant.  The resource id must be built from a
 * literal pool word, which an ordinary integer literal cannot produce. */

/*
 * Sets the BG2 affine scale, loads the palette and the 32x32 sprite frames
 * into the work block, prepares the two rectangle-blit routines, and runs
 * member_count * 16 + 48 frames of a sine-swept scanline table with four
 * sprites orbiting each member whose window is open.  Binding each callee's
 * result to `status` is load-bearing: it makes the call a set of r0 and so
 * fixes the order of the following argument setup.
 */
void BattleFx_RunMemberOrbit(void *object)
{
    void **heap_cache;
    void **cursor;
    void *work;
    void *canvas;
    void *palette;
    s32 status;
    void *rectangle[2];
    s32 record[3];
    s32 screen[3];
    s32 member;
    s32 y_offset;
    void **rectangle_slot;
    void *rect2;
    s32 *record_slot;
    s32 frame;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    FIELD_AT_OFFSET(work, void **, 0x7828) = object;
    BattleFx_BeginCanvasLayer(0);
    FIELD_AT_OFFSET((void *)0x04000020, s16 *, 0) = 0x100;
    palette = Resource_GetTableEntry((s32)&ResourceId_SpiralSheet);
    status = Iwram_CopyWords((void *)0x05000000, palette, 128);
    status = Resource_DecodeType01((u8 *)palette + 128, work);
    status = BattleEffect_LoadWork(46, 7, 7, 3, 2);
    rectangle[0] = heap_cache[7];
    status = BattleEffect_LoadWork(47, 7, 7, 15, 2);
    rect2 = heap_cache[8];
    rectangle_slot = rectangle;
    rectangle_slot[1] = rect2;
    Scheduler_AddOrUpdateCallback((void *)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    FIELD_AT_OFFSET(work, s32 *, 0x7780) = 2;
    FIELD_AT_OFFSET(work, s32 *, 0x7784) = 50;
    Scheduler_AddOrUpdateCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 4) == 1) {
        FIELD_AT_OFFSET((void *)0x04000028, s32 *, 0) = -0x6800;
        y_offset = -112;
    } else {
        y_offset = 0;
    }
    for (frame = 0;
            frame != (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20)
                * 16) + 48;
            frame++) {
        s32 facing;
        s32 *scanline;
        s32 i;
        s32 id_ofs;

        facing = *(s32 *)gCameraWork;
        scanline = (s32 *)((u8 *)work + 0x6980);
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 4) == 0) {
            s32 angle;
            s32 ceiling;

            for (i = 0, ceiling = 0x80000, angle = frame << 10;
                    i != 160; i++) {
                *scanline++ = (ceiling - (Trig_Sin(angle) << 3)) >> 10;
                angle += 1024;
            }
        } else {
            s32 angle;

            for (i = 0, angle = frame << 10; i != 160; i++) {
                *scanline++ = ((Trig_Sin(angle) << 3) >> 10) - 0x7000;
                angle += 1024;
            }
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork(facing, facing + 12);
        member = 0;
        if (FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20) != 0) {
            record_slot = record;
            id_ofs = 36;
            while (member
                != FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s32 *, 20)) {
                void *member_object;

                member_object = *GetBattleObjectSlotFar(
                    FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828), s16 *,
                        id_ofs));
                if (frame > member * 16 && frame < (member * 16) + 60) {
                    s32 spin;

                    if (frame == (member * 16) + 32) {
                        ObjectGroup_UpdateMembers(
                            FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x7828),
                                s16 *, id_ofs),
                            0, 5, -1, 0);
                    }
                    record_slot[0] = FIELD_AT_OFFSET(member_object, s32 *, 8);
                    record_slot[1] = 0x280000;
                    record_slot[2] = FIELD_AT_OFFSET(member_object, s32 *, 16);
                    EffectPosition_ApplyBaseAndYOffset(record_slot, (struct EffectPosition *)screen);
                    for (i = 0; i != 4; i++) {
                        s32 x;
                        s32 y;
                        s32 slot;

                        spin = (frame << 9) + (i << 14);
                        x = (screen[0] + ((Trig_Sin(spin) << 4) >> 16))
                            + y_offset;
                        y = screen[1] + ((Trig_Cos(spin) << 4) >> 16);
                        slot = frame / 16;
                        ((DrawRectangleFn)rectangle_slot[slot & 1])(
                            canvas,
                            (u8 *)work + (((frame / 4) - (slot * 4)) << 10),
                            x - 16, y - 16, 32, 32);
                    }
                }
                id_ofs += 2;
                member++;
            }
        }
        FIELD_AT_OFFSET(work, s32 *, 0x7824) = 1;
        WaitFrames(1);
    }
    Scheduler_RemoveCallback((void *)BattlePresentation_ProcessPendingGraphicsTransfer);
    Scheduler_RemoveCallback((void *)BattleFx_ArmBg2AffineHBlankDma);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    BattleFx_EndCanvasLayer();
}

void BattleFx_RunMemberBurstMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBurst(effect, 0);
}

void BattleFx_RunMemberBurstMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBurst(effect, 1);
}

void BattleFx_RunMemberBurstMode2(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBurst(effect, 2);
}

/* For each mode: how many motes burst from each member and how many frames
   the effect runs after the last member's turn. */
extern u8 MemberBurst_Counts[];

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)

u32 Battle_GetObjectTableValueFar(s32 actor_id);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void ObjectGroup_TickMemberTimers(void);

/* Battle effect: motes burst out of the actor, hang in the air and then
   home in on each target in turn. */
void BattleFx_RunMemberBurst(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition position;
    DrawRectangle draw[2];
    void **heap_cache;
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 k;
    struct BattleCamera *camera;
    s32 resource;
    s32 i;

    heap_cache = (void **)gBattleFxWork;
    cursor = heap_cache;
    work = *cursor++;
    canvas = *cursor;
    camera = *(struct BattleCamera **)((u8 *)heap_cache - 108);
    work->effect = effect;
    BattleFx_BeginCanvasLayer(1);
    Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_MemberBurstImage), work);
    if (mode == 0)
        resource = (s32)&ResourceId_PinkBurstSheet;
    else if (mode == 1)
        resource = (s32)&ResourceId_MarsDjinnSheet;
    else
        resource = (s32)&ResourceId_MercuryDjinnSheet;
    Iwram_CopyWords((void *)0x05000000, Resource_GetTableEntry(resource), 128);
    BattleFx_FetchRectangleBlitters(work->effect->side, draw);

    for (i = 0; i != 1024; i++)
        gMotes[i].variant = -1;
    for (k = 0; k != work->effect->count; k++) {
        struct MotionObject *source = *GetBattleObjectSlotFar(work->effect->actor);
        s32 height = Battle_GetObjectTableValueFar(work->effect->actor);

        for (i = 0; i != 128; i++) {
            struct EffectStep *mote = &gMotes[k * 128 + i];

            mote->x = source->x;
            mote->y = height;
            mote->z = source->z;
            mote->velocity_x = ((Random16() & 255) - 128) << 10;
            mote->velocity_y = ((Random16() & 255) - 128) << 10;
            mote->velocity_z = ((Random16() & 255) - 128) << 10;
            mote->variant = 0;
        }
    }

    Scheduler_AddOrUpdateCallback((s32)BattleFx_ArmBg2AffineHBlankDma, 0x480);
    work->transfer_mode = 2;
    work->transfer_value = 75;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);
    AudioCommand_PlayFar(146);

    for (frame = 0; frame != MemberBurst_Counts[mode * 2 + 1] + work->effect->count * 20; frame++) {
        s32 *row;

        if (frame == 80) {
            if (mode == 0)
                BattleEventRuntime_BeginPhaseFar(134);
            else
                BattleEventRuntime_BeginPhaseFar(133);
        }
        Render_ResetTransformState();
        Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
        row = work->bg2_x;
        for (i = 0; i != 160; i++)
            *row++ = (0x100000 - Trig_Sin((frame + i) << 10) * 16) >> 10;
        for (k = 0; k != work->effect->count; k++) {
            struct MotionObject *target = *GetBattleObjectSlotFar(work->effect->actors[k]);
            s32 half = (s32)Battle_GetObjectTableValueFar(work->effect->actors[k]) / 2;

            if (frame == k * 20 + 71) {
                if (mode == 0)
                    AudioCommand_PlayFar(134);
                else
                    AudioCommand_PlayFar(133);
            }
            if (frame == k * 20 + 70)
                ObjectGroup_UpdateMembers(work->effect->actors[k], 7, 5, k, 26);
            if (frame > k * 20) {
                for (i = 0; i != MemberBurst_Counts[mode * 2]; i++) {
                    struct EffectStep *mote = &gMotes[k * 128 + i];

                    if (frame > (k * 10 + i) * 2 && mote->variant >= 0) {
                        EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &position);
                        position.x >>= 1;
                        draw[0](canvas, work->sheet + i % 3 * 640,
                            position.x - 10, position.y - 16, 20, 32);
                        EffectStep_AdvanceWithGravity3D(mote, 62, 0);
                        if (frame > k * 20 + i + 30) {
                            s32 dx = (target->x - mote->x) >> 9;
                            s32 dy = (target->y + half - mote->y) >> 9;
                            s32 dz = (target->z - mote->z) >> 9;

                            mote->velocity_x += dx;
                            mote->velocity_y += dy;
                            mote->velocity_z += dz;
                            if ((u32)(dx + 0xfff) <= 0x1ffe && (u32)(dz + 0xfff) <= 0x1ffe)
                                mote->variant = -1;
                        }
                    }
                }
            }
        }
        ObjectGroup_TickMemberTimers();
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    Runtime_ReleaseHeapBlock(47);
    Runtime_ReleaseHeapBlock(46);
    Scheduler_RemoveCallback((u32)BattleFx_ArmBg2AffineHBlankDma);
    BattleFx_EndCanvasLayer();
}
