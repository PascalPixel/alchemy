#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE_IDS.H"
#include "RESOURCE.H"
#include "BATTLE_EFX.H"
#include "BATTLE_EFFECT_WORK.H"
#include "BATTLE_PRESENTATION.H"
#include "EFFECT_STEP.H"
#include "CALLBACK_SCHEDULER.H"
#include "SYSTEM.H"
#include "FIXED_MATH.H"
#include "B5_CONTEXT.H"
#include "MOTION_OBJECT.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

extern u8 gBattleFxWork[];
extern DrawRectangle gWorkSlot[];
extern struct BattleCamera *gCameraWork;

/* Which crescent picture each of a mote's eight steps shows. */
extern u8 MemberBeam_MotePictures[];
/* The beam head's three pictures: place in the sheet, width and height. */
extern u16 MemberBeam_HeadOffsets[];
extern u8 MemberBeam_HeadWidths[];
extern u8 MemberBeam_HeadHeights[];

#define gMotes ((struct EffectStep *)Ram_MapCellBuffer)

typedef s32 (*WordCopy)(void *, const void *, s32);

void BattlePresentation_ProcessPendingGraphicsTransfer(void);
void BattleFx_BeginCanvasLayer(s32 mode);
void BattleFx_EndCanvasLayer(void);
void BattleFx_PrepareCanvasEffect(struct BattleEffectArgument *effect, s32 kind, s32 side,
    s32 anchor, s32 *out_x, s32 *out_y);
void BattleFx_FetchRectangleBlitters(s32 alternate, DrawRectangle *output);
struct B5Context *GetBattleObjectSlotFar(s32 id);
void BattleEventRuntime_BeginPhaseFar(s32 phase);
void AudioCommand_PlayFar(s32 value);
void Render_ResetTransformState(void);
void Graphics_PrepareTransferInIwramWork(s32 first, s32 last);
void ObjectGroup_UpdateMembers(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void SceneTransform_ApplyPosition(s32 *position);
u32 Resource_DecodeType01(const void *source, void *destination);

static __inline__ void CopyPalette(WordCopy copy, void *destination, const void *source, s32 size)
{
    /* FAKEMATCH: the palette copies share the routine's address but build
       the palette address again at each call, which only an inlined constant
       argument compiles to; a direct call keeps it in a register. */
    copy(destination, source, size);
}

void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode);

/* Mode entries of the member beam effect. */

void BattleFx_RunTwoModeAMode0(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBeam(effect, 0);
}

void BattleFx_RunTwoModeAMode1(struct BattleEffectArgument *effect)
{
    BattleFx_RunMemberBeam(effect, 1);
}

/* Battle effect: motes sink towards each target, then a beam comes down on
   it from the top of the screen. */
void BattleFx_RunMemberBeam(struct BattleEffectArgument *effect, s32 mode)
{
    struct EffectPosition single;
    struct EffectPosition position;
    s32 ground[3];
    s32 point[3];
    s32 screen_x;
    s32 screen_y;
    DrawRectangle draw[2];
    void **cursor;
    struct BattleEffectWork *work;
    void *canvas;
    s32 frame;
    s32 offset;
    u8 *palette;
    s32 i;
    s32 k;

    cursor = (void **)gBattleFxWork;
    work = *cursor++;
    canvas = *cursor;
    work->effect = effect;
    BattleFx_BeginCanvasLayer(0);
    if (work->effect->unknown_001c == 1)
        BattleFx_PrepareCanvasEffect(effect, mode, work->effect->side ^ work->effect->unknown_001c, 0, &screen_x, &screen_y);
    palette = Resource_GetTableEntry((s32)&ResourceId_EarthWallSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work);
    palette = Resource_GetTableEntry((s32)&ResourceId_CrescentMoonSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x2710);
    if (mode == 0)
        palette = Resource_GetTableEntry((s32)&ResourceId_VenusDjinnSheet);
    else
        palette = Resource_GetTableEntry((s32)&ResourceId_MercuryDjinnSheet);
    CopyPalette(Iwram_CopyWords, (void *)0x05000000, palette, 128);
    palette += 128;
    Resource_DecodeType01(palette, work->sheet + 0x65c0);

    for (i = 0; i != 512; i++) {
        gMotes[i].x = 0;
        gMotes[i].y = 0x500000;
        gMotes[i].z = (Random16() | -32) << 14;
        gMotes[i].variant = Random16() & 255;
    }
    *(volatile u16 *)0x04000020 = 0x100;
    if (work->effect->count == 1) {
        EffectPosition_ApplyStepAndYOffset(work->effect->actors[0], &single);
        offset = 64 - single.x;
    } else {
        if (work->effect->side == 1)
            offset = -112;
        else
            offset = 0;
    }
    *(volatile s32 *)0x04000028 = offset << 8;
    work->transfer_mode = 2;
    work->transfer_value = 50;
    Scheduler_AddOrUpdateCallback((s32)BattlePresentation_ProcessPendingGraphicsTransfer, 0x480);

    for (frame = 0; frame != work->effect->count * 16 + 116; frame++) {
        struct BattleCamera *camera = gCameraWork;

        if (frame == 64)
            AudioCommand_PlayFar(212);
        if (frame == 80)
            BattleEventRuntime_BeginPhaseFar(0);
        k = work->effect->unknown_001c;
        if (k == 1) {
            s32 angle = frame << 11;
            s32 x = (-Trig_Sin(angle) * 20 >> 16) + screen_x + offset - 20;
            s32 y = (Trig_Cos(angle) * 4 >> 16) + screen_y - 24;

            BattleFx_FetchRectangleBlitters(work->effect->side ^ k, draw);
            if (frame > 32)
                y = y - frame * 2 + 64;
            draw[1](canvas, work->sheet + 0x65c0, x, y, 40, 40);
            if (frame <= 3)
                draw[1](canvas, work->sheet + 0x65c0, x, y, 40, 40);
            Runtime_ReleaseHeapBlock(47);
            Runtime_ReleaseHeapBlock(46);
        }
        BattleEffect_LoadWork(46, 7, 7, 3, 2);
        draw[0] = gWorkSlot[46];
        BattleEffect_LoadWork(47, 7, 7, 7, 2);
        draw[1] = gWorkSlot[47];
        for (k = 0; k != work->effect->count; k++) {
            struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[k])->object;
            s32 start = k * 16;

            Render_ResetTransformState();
            Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
            point[0] = target->x;
            point[1] = 0;
            point[2] = target->z;
            SceneTransform_ApplyPosition(point);
            if (frame > start) {
                for (i = 0; i != 8; i++) {
                    struct EffectStep *mote = &gMotes[k * 64 + i];

                    if (frame > i * 8 + start && mote->y > 0x80000) {
                        s32 sway;

                        EffectPosition_ApplyBaseAndYOffset((s32 *)mote, &position);
                        position.x += offset;
                        sway = Trig_Sin(mote->variant << 10) * 16 >> 16;
                        if (i & 1)
                            position.x -= sway;
                        else
                            position.x += sway;
                        draw[i & 1](canvas,
                            work->sheet + MemberBeam_MotePictures[mote->variant / 8 & 7] * 576 + 0x2710,
                            position.x - 12, position.y - 12, 24, 24);
                        mote->y -= 0x10000;
                        mote->variant++;
                    }
                }
            }
        }
        for (i = 0; i != work->effect->count; i++) {
            s32 start = i * 16;

            if (frame >= start + 72) {
                struct MotionObject *target = GetBattleObjectSlotFar(work->effect->actors[i])->object;

                Render_ResetTransformState();
                Graphics_PrepareTransferInIwramWork((s32)camera, (s32)camera->pos);
                if (frame == start + 72)
                    ObjectGroup_UpdateMembers(work->effect->actors[i], 1, -1, -1, 0);
                if (frame == start + 88)
                    ObjectGroup_UpdateMembers(work->effect->actors[i], 0, -1, -1, 0);
                ground[0] = target->x;
                ground[1] = 0;
                ground[2] = target->z;
                EffectPosition_ApplyBaseAndYOffset(ground, &position);
                position.x += offset;
                if (frame < start + 104) {
                    s32 picture = frame / 4;
                    s32 width = 6;

                    if (frame > start + 88)
                        width = 6 - (frame - start - 88) / 3;
                    if (picture > 2)
                        picture = (picture & 1) + 1;
                    if (frame < start + 100) {
                        draw[0](canvas, work->sheet + MemberBeam_HeadOffsets[picture],
                            position.x - MemberBeam_HeadWidths[picture],
                            position.y - MemberBeam_HeadHeights[picture] + 8,
                            MemberBeam_HeadWidths[picture], MemberBeam_HeadHeights[picture]);
                        draw[1](canvas, work->sheet + MemberBeam_HeadOffsets[picture],
                            position.x,
                            position.y - MemberBeam_HeadHeights[picture] + 8,
                            MemberBeam_HeadWidths[picture], MemberBeam_HeadHeights[picture]);
                    }
                    for (k = 0; k != position.y; k++) {
                        draw[0](canvas, work->sheet + 5, position.x - width, k, width, 1);
                        draw[1](canvas, work->sheet + 5, position.x, k, width, 1);
                    }
                }
            }
        }
        Runtime_ReleaseHeapBlock(47);
        Runtime_ReleaseHeapBlock(46);
        work->transfer_pending = 1;
        WaitFrames(1);
    }

    Scheduler_RemoveCallback((u32)BattlePresentation_ProcessPendingGraphicsTransfer);
    BattleFx_EndCanvasLayer();
}
