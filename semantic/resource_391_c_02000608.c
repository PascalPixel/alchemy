/* resource_391 0x02000608-0x020008bf: staged actor move/redraw family. */
#include "types.h"
#include "staged_actor.h"

extern s32 Data_0200adc0[][4];
extern s32 Data_0200ad68[];

u8 *Func_02003250();
u8 *Func_0200325c();
void Func_02000902();
void Func_020032f4();
void Func_02003354();
void Func_020032da();
void Func_0200336c();
u8 *Func_0200332a();
void Func_02003314();
void Func_020032c4();
void Func_020032ce();
void Func_02003454();
void Func_02003308();
void Func_020033b6();
void Func_020033ce();
void Func_02003380();
void Func_020033e4();
void Func_020033fc();
void Func_02003414();
u8 *Func_020033ba();
void Func_02003374();
void Func_020034cc();
void Func_020034d2();
void Func_02003368();
void Func_020033f2();
void Func_02000a6e();
void Func_02000a80();
void Func_0200344e();
void Func_02000acc();
void Func_020035a8();

#define StagedActorDisplacements Data_0200adc0
#define StagedActorDirectionSteps Data_0200ad68
#define GetLeadStagedActor Func_02003250
#define GetStagedActor Func_0200325c
#define ClearOccupiedRectangle Func_02000902
#define SetLeadActorMotionRates Func_020032f4
#define SetLeadActorMode Func_02003354
#define WaitSceneFrames Func_020032da
#define SetLeadActorMoveDelta Func_0200336c
#define GetLeadActorRuntime Func_0200332a
#define PrepareMovementCallback Func_02003314
#define ApplyActorFacingModeA Func_020032c4
#define ApplyActorFacingModeB Func_020032ce
#define PlayActorMoveSound Func_02003454
#define SetStagedActorPosition Func_02003308
#define FinishStagedActorMove Func_020033b6
#define SetActorTransitionMode Func_020033ce
#define SetActorMotionRates Func_02003380
#define SetLeadActorDirectionStep Func_020033e4
#define FinishLeadActorMove Func_020033fc
#define SetLeadActorTransition Func_02003414
#define GetLeadActorRuntimeForRestore Func_020033ba
#define RestoreMovementCallback Func_02003374
#define PlayActorArrivalSound Func_020034cc
#define PlayActorStopSound Func_020034d2
#define CommitStagedActorPosition Func_02003368
#define RedrawNewActorFootprint Func_020033f2
#define FillCollisionLayer0 Func_02000a6e
#define FillCollisionLayer2 Func_02000a80
#define RedrawOldActorFootprint Func_0200344e
#define ClearOldActorFootprint Func_02000acc
#define FinishActorFootprintRedraw Func_020035a8
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
