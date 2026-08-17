/*
 * resource_373 @ 0x02000608 (696 bytes through the pool at 0x020008bc).
 *
 * Six-argument staged actor movement/effect owner.  It derives a footprint
 * from the selected four-word displacement row, temporarily moves actor and
 * camera state, redraws the old and new occupied rectangles, and restores the
 * normal callbacks.  The sibling banks in resources 389/391/392/393 share the
 * control flow but use different displacement/orientation tables, so those
 * addresses are intentionally resolved per overlay.  The sixth argument is
 * invoked through this overlay's generic call-through veneer.
 */
#include "types.h"
#include "staged_actor.h"

extern s32 Data_0200e1e8[][4];
extern s32 Data_0200e190[];

struct StagedActor *Func_02006610(s32 slot);
struct StagedActor *Func_0200661c(s32 slot);
void Func_020066b4(s32 id, s32 value, s32 callback);
void Func_02006740(s32 id, s32 value, s32 callback);
void Func_02006714(s32 id, s32 mode);
void Func_0200678e(s32 id, s32 mode);
void Func_020067d4(s32 id, s32 mode);
void Func_0200668a(s32 frames);
void Func_020066c4();
void Func_0200672c(s32 id, s32 x, s32 y);
void Func_020067a4(s32 id, s32 x, s32 y);
void Func_02006776(s32 id);
void Func_020067bc(s32 id);
struct StagedActor *Func_020066ea(s32 slot);
struct StagedActor *Func_0200677a(s32 slot);
void Func_02006614(struct StagedActor *actor, s32 mode);
void Func_0200661e(struct StagedActor *actor, s32 mode);
void Func_020066b8(struct StagedActor *actor, s32 mode);
void Func_0200687c(s32 sound);
void Func_020068f4(s32 sound);
void Func_020068fa(s32 sound);
void Func_02006658(struct StagedActor *actor, s32 x, s32 elevation, s32 y);
void Func_02006752(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
void Func_020067ae(s32 x, s32 y, s32 width, s32 height,
                   s32 localX, s32 localY);
void Func_020069d0(void);
s32 Func_02000902(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
s32 Func_02000a6e(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
s32 Func_02000a80(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);
s32 Func_02000acc(s32 layer, s32 x, s32 y, s32 width, s32 height, s32 value);

#define StagedActorDisplacements Data_0200e1e8
#define StagedActorDirectionSteps Data_0200e190
#define GetLeadStagedActor Func_02006610
#define GetStagedActor Func_0200661c
#define ClearOccupiedRectangle Func_02000902
#define SetLeadActorMotionRates Func_020066b4
#define SetLeadActorMode Func_02006714
#define WaitSceneFrames Func_0200668a
#define SetLeadActorMoveDelta Func_0200672c
#define GetLeadActorRuntime Func_020066ea
#define PrepareMovementCallback Func_020066c4
#define ApplyActorFacingModeA Func_02006614
#define ApplyActorFacingModeB Func_0200661e
#define PlayActorMoveSound Func_0200687c
#define SetStagedActorPosition Func_02006658
#define FinishStagedActorMove Func_02006776
#define SetActorTransitionMode Func_0200678e
#define SetActorMotionRates Func_02006740
#define SetLeadActorDirectionStep Func_020067a4
#define FinishLeadActorMove Func_020067bc
#define SetLeadActorTransition Func_020067d4
#define GetLeadActorRuntimeForRestore Func_0200677a
#define RestoreMovementCallback Func_020066c4
#define PlayActorArrivalSound Func_020068f4
#define PlayActorStopSound Func_020068fa
#define CommitStagedActorPosition Func_020066b8
#define RedrawNewActorFootprint Func_02006752
#define FillCollisionLayer0 Func_02000a6e
#define FillCollisionLayer2 Func_02000a80
#define RedrawOldActorFootprint Func_020067ae
#define ClearOldActorFootprint Func_02000acc
#define FinishActorFootprintRedraw Func_020069d0
#define MoveStagedActorAndRedraw Func_02000608

void MoveStagedActorAndRedraw(struct StagedActorMoveArgs args)
{
#define DISP_WORD(offset) (StagedActorDisplacements[args.displacement_row][(offset) >> 2])
    u8 *world = *(u8 **)0x03001e70;
    struct StagedActor *lead_actor = GetLeadStagedActor(0);
    s32 direction = lead_actor->direction_and_kind >> 12;
    struct StagedActor *actor = GetStagedActor(args.actor_slot);
    s32 dy0 = DISP_WORD(4) < 0 ? -DISP_WORD(4) : DISP_WORD(4);
    s32 dy1 = DISP_WORD(12) < 0 ? -DISP_WORD(12) : DISP_WORD(12);
    s32 height = (dy0 + dy1) >> 4;
    s32 dx0 = DISP_WORD(0) < 0 ? -DISP_WORD(0) : DISP_WORD(0);
    s32 dx1 = DISP_WORD(8) < 0 ? -DISP_WORD(8) : DISP_WORD(8);
    s32 width = (dx0 + dx1) >> 4;
    s32 oldPosition[3];
    s32 occupiedPosition[3];
    s32 baseX;
    s32 baseY;
    s32 targetWorldX;
    s32 targetWorldY;

    actor->move_rate_x = 0x8000;
    actor->move_rate_z = 0x1999;
    oldPosition[0] = actor->x.value;
    oldPosition[2] = actor->z.value;
    occupiedPosition[0] = actor->x.value
        + (DISP_WORD(0) << 16);
    occupiedPosition[2] = actor->z.value
        + (DISP_WORD(4) << 16);
    occupiedPosition[0] >>= 20;
    occupiedPosition[2] >>= 20;
    ClearOccupiedRectangle(0, occupiedPosition[0], occupiedPosition[2], width, height, 0);
    SetLeadActorMotionRates(0, 0x8000, 0x1999);
    SetLeadActorMode(0, 8);
    WaitSceneFrames(15);

    {
        s32 moveX = args.target_x - oldPosition[0];
        if (moveX < 0)
            moveX += 0x1ffff;
        moveX >>= 17;
        {
            s32 moveY = args.target_z - oldPosition[2];
            if (moveY < 0)
                moveY += 0x1ffff;
            moveY >>= 17;
            SetLeadActorMoveDelta(0, moveX, moveY);
        }
    }

    lead_actor = GetLeadActorRuntime(0);
    lead_actor->movement_callback = 0x020082a9;
    PrepareMovementCallback(4);
    if ((u32)(direction - 6) <= 7)
        ApplyActorFacingModeA(actor, 3);
    else
        ApplyActorFacingModeB(actor, 2);
    PlayActorMoveSound(0xef);
    SetStagedActorPosition(actor, args.target_x, args.elevation, args.target_z);
    FinishStagedActorMove(0);
    SetActorTransitionMode(0, 2);
    SetActorMotionRates(0, 0x4ccc, 0x1999);

    {
        s32 packed = StagedActorDirectionSteps[direction];
        SetLeadActorDirectionStep(0, (s16)(packed >> 16) / 2, (s16)packed / 2);
    }
    if (args.callback)
        args.callback();
    FinishLeadActorMove(0);
    SetLeadActorTransition(0, 1);

    lead_actor = GetLeadActorRuntimeForRestore(0);
    lead_actor->movement_callback = 0;
    RestoreMovementCallback(actor);
    PlayActorArrivalSound(0x120);
    PlayActorStopSound(0xd5);
    actor->x.value = args.target_x;
    actor->z.value = args.target_z;
    actor->unknown_24 = 0;
    actor->unknown_2c = 0;
    CommitStagedActorPosition(actor, 1);

    args.target_x += DISP_WORD(0) << 16;
    args.target_z += DISP_WORD(4) << 16;
    args.target_x >>= 20;
    args.target_z >>= 20;
    baseX = *(s32 *)(world + 316) >> 20;
    baseY = *(s32 *)(world + 320) >> 20;
    targetWorldX = baseX + args.target_x;
    targetWorldY = baseY + args.target_z;
    RedrawNewActorFootprint(args.target_x, args.target_z, width, height,
                  targetWorldX, targetWorldY);
    FillCollisionLayer0(0, args.target_x, args.target_z, width, height, 255);
    FillCollisionLayer2(2, args.target_x, args.target_z, width, height, 255);

    oldPosition[0] += DISP_WORD(0) << 16;
    oldPosition[2] += DISP_WORD(4) << 16;
    oldPosition[0] >>= 20;
    oldPosition[2] >>= 20;
    RedrawOldActorFootprint(baseX + oldPosition[0], baseY + oldPosition[2], width, height,
                  oldPosition[0], oldPosition[2]);
    ClearOldActorFootprint(2, oldPosition[0], oldPosition[2], width, height, 0);
    FinishActorFootprintRedraw();
#undef DISP_WORD
}
