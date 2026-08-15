/*
 * resource_392 @ 0x02000608 (696 bytes through the pool at 0x020008bc).
 *
 * Six-argument staged actor movement/effect owner.  It derives a footprint
 * from the selected four-word displacement row, temporarily moves actor and
 * camera state, redraws the old and new occupied rectangles, and restores the
 * normal callbacks.  The sibling banks in resources 373/389/391/393 share the
 * control flow but use different displacement/orientation tables, so those
 * addresses are intentionally resolved per overlay.  The sixth argument is
 * invoked through this overlay's generic call-through veneer.
 */
#include "types.h"
#include "staged_actor.h"

extern s32 Data_02008f20[][4];
extern s32 Data_02008ec8[];

u8 *Func_02001460();
u8 *Func_0200146c();
void Func_02000902();
void Func_02001504();
void Func_0200152c();
void Func_020014ea();
void Func_02001544();
u8 *Func_0200153a();
void Func_02001524();
void Func_020014ec();
void Func_020014f6();
void Func_020015b4();
void Func_02001510();
void Func_0200158e();
void Func_020015a6();
void Func_02001590();
void Func_020015bc();
void Func_020015d4();
void Func_020015ec();
u8 *Func_020015ca();
void Func_0200157c();
void Func_0200162c();
void Func_02001632();
void Func_020015fa();
void Func_02000a6e();
void Func_02000a80();
void Func_02001656();
void Func_02000acc();
void Func_02001708();

#define StagedActorDisplacements Data_02008f20
#define StagedActorDirectionSteps Data_02008ec8
#define GetLeadStagedActor Func_02001460
#define GetStagedActor Func_0200146c
#define ClearOccupiedRectangle Func_02000902
#define SetLeadActorMotionRates Func_02001504
#define SetLeadActorMode Func_0200152c
#define WaitSceneFrames Func_020014ea
#define SetLeadActorMoveDelta Func_02001544
#define GetLeadActorRuntime Func_0200153a
#define PrepareMovementCallback Func_02001524
#define ApplyActorFacingModeA Func_020014ec
#define ApplyActorFacingModeB Func_020014f6
#define PlayActorMoveSound Func_020015b4
#define SetStagedActorPosition Func_02001510
#define FinishStagedActorMove Func_0200158e
#define SetActorTransitionMode Func_020015a6
#define SetActorMotionRates Func_02001590
#define SetLeadActorDirectionStep Func_020015bc
#define FinishLeadActorMove Func_020015d4
#define SetLeadActorTransition Func_020015ec
#define GetLeadActorRuntimeForRestore Func_020015ca
#define RestoreMovementCallback Func_0200157c
#define PlayActorArrivalSound Func_0200162c
#define PlayActorStopSound Func_02001632
#define CommitStagedActorPosition Func_02001590
#define RedrawNewActorFootprint Func_020015fa
#define FillCollisionLayer0 Func_02000a6e
#define FillCollisionLayer2 Func_02000a80
#define RedrawOldActorFootprint Func_02001656
#define ClearOldActorFootprint Func_02000acc
#define FinishActorFootprintRedraw Func_02001708
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
