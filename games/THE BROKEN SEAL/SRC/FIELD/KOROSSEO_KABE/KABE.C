#include "RUNTIME_MEM.H"
#include "TASK.H"
#include "TYPES.H"
#include "IO_REG.H"
#include "CALLBACK_SCHEDULER.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "RESOURCE_IDS.H"
#include "FIELD_SCENE.H"

/* The scene's tables, which the entry veneers publish to the map engine. */
extern const struct SceneEntrance gKorosseoKabeEntrances[];
extern const u32 gKorosseoKabeExits[];
extern const struct ScenePlacement gKorosseoKabePlacements[];
void Engine_TaskWait();
void Engine_ObjectSetPosition();
void Object_CommitPosition();
void Engine_MapCopyCellAttributes();
void Engine_EventWait();
void Engine_ActorSetAnimation();
void Engine_AudioPlayCue();

/* The game state's cells, read here as halfwords. */
extern s16 gCell[];
void SceneActor_MovePairByTileOffset(s32 actor, s32 dx, s32 dz);

/* The buttons held this frame. */
extern u32 gKeysHeld;
void Object_CommitPosition(struct FieldActor *object);
void ObjectDispatch_InitFromTable4WithArgument(s32 handle, struct FieldActor *object);

/* The log's rolling animation for each quarter of the pusher's heading. */
extern u8 KorosseoKabe_RollLogScript[];
void Object_RefreshSelectorById(s32 actor);
void BattleFx_SetWeightedResult(s32 value, s32 mode);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
void Event_SetPair1d4(s32 scene, s32 entrance);
extern const struct SceneEvent gKorosseoKabeEvents[];
void SceneState_ApplyRectsForActors15To17(void);
s32 Korosseo_ShowItemIcon(s32 slot, s32 item);
void Korosseo_SelectSoloCompetitor(s32 index);
void SceneActor_PlacePartyAtSavedTiles(void);
void KorosseoKabe_RunScriptedTransition(s32 value);
void Korosseo_RunGreetScene(s32 actor);
void FieldScene_RunSixSteps896To936(void);
void FieldScene_RunPairedEntranceWalk(s32 direction);
void FieldScene_RunSupplementalSequenceOne(void);
void KorosseoKabe_MarkSceneProgress(void);
void Map_UpdateCellRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);


extern u8 MsgKorosseoSiteSecondFinals[];
void Engine_EventBegin();
void Engine_ActorSetSpeed();
void Engine_CameraFollowActor();
void Engine_EventOpenScreen();
void Engine_EventWaitForScreen();
void Engine_EventSetMessage();
void Engine_ActorSetAnimationAndWait();
void Engine_EventShowMessage();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorSetDestination();
void Engine_ActorWalkToAndWait();
void Engine_ActorFaceDirection();
void Engine_EventEnd();

extern u8 MsgKorosseoMainThingStage[];
extern u8 MsgKorosseoStageDubbedMini[];
void Korosseo_FinishSoloRound();
s32 KorosseoKabe_RunStateInteraction();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
s32 Korosseo_FadeInCompetitor();
void Engine_ActorSetAttachedEffect();
void Korosseo_RestoreCompetitor();
void KorosseoKabe_ShowFollowUpPrompt();
extern u8 MsgKorosseoHereYourObjectiveRideLogs[];
extern u8 MsgKorosseoLogRollingStage[];
extern u8 MsgKorosseoOperatorLiftsWillCheerFor[];
extern u8 MsgKorosseoShiftingFloorStage[];
extern u8 MsgKorosseoFansJustCallWall[];
extern u8 MsgKorosseoScalingWallQuickly[];
void ObjectMotion_CommitCurrentPositionAndActivate();
void BattleFx_RunRisingObjectSequence();
void battle_owner_69();
extern u8 MsgKorosseoFocusRollingLogs[];
extern u8 MsgKorosseoPlaceCalledBoard[];
void Korosseo_FinishSoloRound(void);
s32 KorosseoKabe_RunStateInteraction(s32 speaker, s32 base);
s32 Korosseo_FadeInCompetitor(s32 actor, s32 x, s32 z);
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row);
void Korosseo_RestoreCompetitor(s32 actor);
void KorosseoKabe_ShowFollowUpPrompt(s32 speaker, s32 base);

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gKorosseoKabeEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gKorosseoKabeExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gKorosseoKabePlacements;
}

/*
 * Two bytes, `bx lr', with no prologue and no pool.  Two data-table slots
 * install it as a handler, so the empty body is deliberate rather than
 * padding.
 */
void SceneData_NoOpHandler(void)
{
}

void FieldScene_RunSingleStep(void)
{
    StagedActor_PushActorAhead();
}

void SceneState_ConfigureRegionByActorElevenColumn(void)
{
    u8 *work;
    s32 v0;
    s32 v1;

    work = (u8 *)Object_GetById(11);
    if ((*(s32 *)(work + 8) >> 20) == 36) {
        GameFlag_Set(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Map_CopyCellAttributes(0x23, 0x4E, 1, 1, v0, v1);
    } else {
        GameFlag_Clear(0x335);
        v0 = 0x23;
        v1 = 0x4D;
        Map_CopyCellAttributes(0x22, 0x4D, 1, 1, v0, v1);
    }
}

void FieldScene_RunTwoStepSequence(void)
{
    StagedActor_PushActorAhead();
    SceneState_ConfigureRegionByActorElevenColumn();
}

void SceneState_StoreSlotTileXToWork832To848(void)
{
    /* The two stack arguments each need their own local: the reference builds
     * both into separate registers before storing either, and a literal pair
     * lets the compiler reuse one register for both. */
    s32 fifth = 14;
    s32 sixth = 11;
    s32 *record;
    s32 value;

    Map_CopyCellAttributes(100, 11, 12, 4, fifth, sixth);

    record = (s32 *)Object_GetById(12);
    value = record[2] >> 20;
    GameFlag_SetByte(832, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);

    record = (s32 *)Object_GetById(13);
    value = record[2] >> 20;
    GameFlag_SetByte(840, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);

    record = (s32 *)Object_GetById(14);
    value = record[2] >> 20;
    GameFlag_SetByte(848, value);
    Map_CopyCellAttributes(71, 16, 1, 1, value, 16);
}

void SceneState_SetWorkByte35(void)
{
    u8 *record = *(u8 **)gEffectWork;

    record[53] = 1;
}

void KorosseoKabe_SpinActorAway(void)
{
    s32 rec2;
    s32 frames;
    s32 spin;
    s32 frames2;
    s32 spin2;
    u8 *p6;

    rec2 = (s32)Object_GetById(30);
    p6 = *(s32 *)(rec2 + 80);
    Call1(Engine_GameFlagSet, 0x330);
    *(s32 *)(rec2 + 52) = 0x1999;
    *(s32 *)(rec2 + 48) = 0x13333;
    Engine_AudioPlayCue(227);
    Call4(Engine_ObjectSetPosition, rec2, 0x1500000, 0xa0000, 0x1080000);
    for (spin = 0, frames = 9; frames >= 0; frames--) {
        *(u16 *)((s32)p6 + 30) -= spin;
        Engine_TaskWait(1);
        spin += 36;
    }
    Call4(Engine_ObjectSetPosition, rec2, 0x14a0000, -0x100000, 0x1080000);
    for (spin2 = 0x168, frames2 = 21; frames2 >= 0; frames2--) {
        *(u16 *)((s32)p6 + 30) -= spin2;
        Engine_TaskWait(1);
        spin2 += 36;
    }
    Object_CommitPosition(rec2);
    Engine_EventWait(2);
    Engine_AudioPlayCue(240);
    {
        s32 shown = 0;
    
        *(u16 *)((s32)p6 + 30) = shown;
    }
    Engine_ActorSetAnimation(30, 4);
    *(s32 *)(rec2 + 8) = 0x1500000;
    *(s32 *)(rec2 + 12) = -0x80000;
    *(s32 *)(rec2 + 16) = 0x1080000;
    *(s32 *)(rec2 + 40) = 0;
    *(s32 *)(rec2 + 36) = 0;
    Call6(Engine_MapCopyCellAttributes, 19, 16, 1, 1, 20, 16);
    Call6(Engine_MapCopyCellAttributes, 20, 80, 1, 1, 21, 80);
}

void SceneState_SetFlag331AndConfigureRegion46_17(void)
{
    u8 *p;

    GameFlag_Set(0x331);
    p = (u8 *)Object_GetById(20) + 85;
    *p = 0;
    {
        s32 p5 = 44;
        s32 p6 = 17;

        Map_CopyCellAttributes(46, 17, 1, 1, p5, p6);
    }
}

void FieldScene_SetFlag332AndDrawTiles(void)
{
    u8 *slot;

    GameFlag_Set(0x332);
    slot = (u8 *)Object_GetById(21) + 85;
    *slot = 0;
    {
        s32 v5 = 50;
        s32 v6 = 17;

        Map_CopyCellAttributes(46, 17, 1, 1, v5, v6);
    }
}

void FieldScene_SetFlag333AndDrawTiles(void)
{
    GameFlag_Set(0x333);
    {
        s32 width = 32;
        s32 height = 77;

        Map_CopyCellAttributes(32, 37, 1, 4, width, height);
    }
}

void SceneState_SendWord250With6(void)
{
    s16 *tbl = gCell;

    BattleFx_RunRisingObjectSequence(*(s32 *)(tbl + 250), 6, 0);
}

void FieldScene_Forward4358(void)
{
    Engine_LeaderCheckAhead();
}

void SceneActor_MovePairByTileOffset(s32 a0, s32 a1, s32 a2)
{

    MovedObject *p;
    MovedObject *q;
    s32 x;
    s32 y;

    p = (MovedObject *)Object_GetById(gGameState.selected_actor);
    q = (MovedObject *)Object_GetById(a0);
    Engine_EventBegin();
    {
        x = ((p->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((p->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        p->f30 = 0x10000;
        p->f34 = 0x8000;
        Engine_ObjectSetPosition(p, x, p->f0c, y);
    }
    Object_SetMode(p, 27);
    {
        x = ((q->f08 + (a1 << 16)) & 0xFFF00000) + 0x80000;
        y = ((q->f10 + (a2 << 16)) & 0xFFF00000) + 0x80000;

        q->f30 = 0x10000;
        q->f34 = 0x8000;
        Engine_ObjectSetPosition(q, x, q->f0c, y);
    }
    if (a1 < 0 || a2 < 0) {
        Object_SetMode(q, 4);
    } else {
        Object_SetMode(q, 3);
    }
    Audio_PlayCue(226);
    Object_CommitPosition(p);
    Audio_PlayCue(288);
    Object_SetMode(q, 2);
    Engine_EventEnd();
}

/* When the leader stands in line with wall block 32 (or 33 on the far rows)
 * and pushes it along the row (left past column 51, right before it), moves
 * the block pair and redraws the cells under both blocks. */
void KorosseoKabe_PushAlignedWall(void)
{
    struct FieldActor *leader = Object_GetById(gGameState.selected_actor);
    s32 x = leader->x.fixed >> 20;
    s32 push = 0;
    s32 block = 32;

    if (leader->z.fixed >> 20 > 12) {
        block = 33;
    }
    if (Object_GetById(block)->x.fixed >> 20 != x) {
        return;
    }
    if (x > 51) {
        if (gKeysHeld & KEY_LEFT) {
            push = -64;
        }
    } else if (gKeysHeld & KEY_RIGHT) {
        push = 64;
    }
    if (push != 0) {
        SceneActor_MovePairByTileOffset(block, push, 0);
        Engine_MapCopyCellAttributes(120, 10, 5, 6, 48, 10);
        x = Object_GetById(32)->x.fixed >> 20;
        Engine_MapCopyCellAttributes(52, 28, 1, 3, x, 10);
        x = Object_GetById(33)->x.fixed >> 20;
        Engine_MapCopyCellAttributes(52, 28, 1, 3, x, 13);
    }
}

/* The leader rolls log id to cell (column / 2, row) on the Board Walk: the
 * log rolls there with the animation for the push direction while the
 * leader follows half the distance behind it. */
void KorosseoKabe_RollLogToCell(s32 id, s32 column, s32 row)
{
    s32 pusher = gGameState.selected_actor;
    struct FieldActor *leader = Object_GetById(pusher);
    struct FieldActor *log = Object_GetById(id);
    s32 heading = (leader->facing + 0x1000) & 0xe000;
    s32 along_x = (log->x.fixed >> 20) != column / 2;
    s32 dx;
    s32 dz;

    column <<= 19;
    row <<= 19;
    if (along_x) {
        dx = (column - log->x.fixed) / 2;
        dz = 0;
    } else {
        dx = 0;
        dz = (row - log->z.fixed) / 2;
    }
    Engine_EventBegin();
    Engine_ActorSetAnimation(pusher, 8);
    Engine_EventWait(6);
    log->speed = 0x8000;
    log->acceleration = 0x3333;
    Object_SetMode(log, KorosseoKabe_RollLogScript[heading / 0x4000]);
    Engine_ObjectSetPosition(log, column, 0, row);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(pusher, 2);
    ObjectDispatch_InitFromTable4WithArgument(*(s32 *)((u8 *)Runtime_AllocateBlock(27, 0xccc) + 480), log);
    Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
    Object_SetMode(leader, 2);
    Engine_ObjectSetPosition(leader, leader->x.fixed + dx, 0, leader->z.fixed + dz);
    Engine_AudioPlayCue(239);
    Object_CommitPosition(leader);
    Object_SetMode(leader, 1);
    Object_CommitPosition(log);
    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(213);
    Object_SetMode(log, 1);
    Engine_EventWait(15);
    Engine_EventEnd();
}

/*
 * One fixed line, then three whose fifth or sixth argument is a field of the
 * record fetched for participants 15, 16 and 17.  Records 15 and 16 contribute
 * their word at +8, record 17 its word at +16, which moves to the sixth
 * argument slot while a literal 18 takes the fifth.  The shift is arithmetic,
 * so the fields are signed fixed-point with 20 fractional bits.  Only those two
 * fields are asserted; what the six arguments mean is not established here.
 */
void SceneState_ApplyRectsForActors15To17(void)
{
    s32 field;

    Map_CopyCellAttributes(100, 11, 12, 4, 14, 11);

    field = ((s32 *)Object_GetById(15))[2] >> 20;
    Map_CopyCellAttributes(13, 28, 1, 4, field, 11);

    field = ((s32 *)Object_GetById(16))[2] >> 20;
    Map_CopyCellAttributes(13, 28, 1, 4, field, 11);

    field = ((s32 *)Object_GetById(17))[4] >> 20;
    Map_CopyCellAttributes(13, 28, 4, 1, 18, field);
}

void FieldScene_RunStep15At29By26(void)
{
    KorosseoKabe_RollLogToCell(15, 29, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep15At33By26(void)
{
    KorosseoKabe_RollLogToCell(15, 33, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep16At45By26(void)
{
    KorosseoKabe_RollLogToCell(16, 45, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep16At49By26(void)
{
    KorosseoKabe_RollLogToCell(16, 49, 26);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep17At40By23(void)
{
    KorosseoKabe_RollLogToCell(17, 40, 23);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunStep17At40By25(void)
{
    KorosseoKabe_RollLogToCell(17, 40, 25);
    SceneState_ApplyRectsForActors15To17();
}

void FieldScene_RunSupplementalSequenceOne(void)
{
    s32 i;
    s32 rec8;
    s32 rec7;
    s32 xa;
    s32 xb;
    s32 xd;
    s32 ya;
    s32 yb;
    s32 yd;
    s32 record;

    rec8 = (s32)Object_GetById(gGameState.selected_actor);
    for (i = 22; i <= 25; i++) {
        rec7 = Object_GetById(i);
        *(u8 *)(rec7 + 91) = 0;
        xa = *(s32 *)(rec7 + 8);
        xb = *(s32 *)(rec8 + 8);
        xd = xa - xb;
        if (xd >= 0) {
            if (xd > 0x9ffff) {
                continue;
            }
        } else {
            xb = xb - xa;
            if (xb > 0x9ffff) {
                continue;
            }
        }
        ya = *(s32 *)(rec7 + 16);
        yb = *(s32 *)(rec8 + 16);
        yd = ya - yb;
        if (yd >= 0) {
            if (yd > 0x9ffff) {
                continue;
            }
        } else {
            yb = yb - ya;
            if (yb > 0x9ffff) {
                continue;
            }
        }
        if (GameFlag_IsSet(0x104) != 0) {
            yb = *(s32 *)(rec7 + 16);
        } else {
            yb = *(s32 *)(rec8 + 16);
            yd = *(s32 *)(rec7 + 44);
            yb = yb + yd;
        }
        *(s32 *)(rec8 + 16) = yb;
    }
    if (KorosseoKabe_SpectatorTimer != 0
        && *(s32 *)(rec7 + 56) == (s32)0x80000000) {
        if (KorosseoKabe_SpectatorPhase == 0) {
            Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
        } else {
            Map_CopyCellAttributes(58, 10, 1, 1, 58, 11);
        }
    } else {
        Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
        Map_CopyCellAttributes(58, 14, 7, 1, 58, 13);
    }
    if (KorosseoKabe_SpectatorTimer == 0) {
        KorosseoKabe_SpectatorPhase ^= 1;
        if (KorosseoKabe_SpectatorPhase != 0) {
            record = (s32)Object_GetById(22);
            Call4(Engine_ObjectSetPosition, record, 0x3a80000, 0, 0xb80000);
            record = (s32)Object_GetById(23);
            Call4(Engine_ObjectSetPosition, record, 0x3c80000, 0, 0xf80000);
            record = (s32)Object_GetById(24);
            Call4(Engine_ObjectSetPosition, record, 0x3e80000, 0, 0xb80000);
            record = (s32)Object_GetById(25);
            Call4(Engine_ObjectSetPosition, record, 0x4080000, 0, 0xf80000);
            Engine_ActorSetAnimation(31, 11);
        } else {
            record = (s32)Object_GetById(22);
            Call4(Engine_ObjectSetPosition, record, 0x3a80000, 0, 0xd80000);
            record = (s32)Object_GetById(23);
            Call4(Engine_ObjectSetPosition, record, 0x3c80000, 0, 0xd80000);
            record = (s32)Object_GetById(24);
            Call4(Engine_ObjectSetPosition, record, 0x3e80000, 0, 0xd80000);
            record = (s32)Object_GetById(25);
            Call4(Engine_ObjectSetPosition, record, 0x4080000, 0, 0xd80000);
            Engine_ActorSetAnimation(31, 10);
        }
    }
    KorosseoKabe_SpectatorTimer++;
    if (KorosseoKabe_SpectatorTimer > 119) {
        if (GameFlag_IsSet(0x104) == 0) {
            KorosseoKabe_SpectatorTimer = 0;
        }
    }
}

void FieldScene_PlaceSpectatorRow(void)
{
    KorosseoKabe_SpectatorTimer = 0;
    KorosseoKabe_SpectatorPhase = 0;
    Engine_TaskRemoveCallback((s32)FieldScene_RunSupplementalSequenceOne);
    Actor_SetPosition(22, 0x3a80000, 0xd80000);
    Actor_SetPosition(23, 0x3c80000, 0xd80000);
    Actor_SetPosition(24, 0x3e80000, 0xd80000);
    Actor_SetPosition(25, 0x4080000, 0xd80000);
    Engine_ActorSetAnimation(31, 10);
}

void SceneState_ApplyTable8715AndValue104(void)
{
    Engine_TaskAddCallback(FieldScene_RunSupplementalSequenceOne, 0xC85);
    GameFlag_Clear(0x104);
}

/*
 * Spin until the first status word reaches zero with the second equal to 75,
 * giving up after 600 polls. Both words are re-read on every pass, because the
 * poll call lets the task that publishes them run.
 * The plain while loop is the spelling that reproduces the reference. What the
 * two words mean is not established here -- only that another task publishes
 * them while this owner spins.
 */
void SceneState_WaitForStatusWords(void)
{
    s32 cnt;

    /* The frame count is a literal ten. */
    Engine_TaskWait(10);

    cnt = 0;
    while (KorosseoKabe_SpectatorPhase != 0 || KorosseoKabe_SpectatorTimer != 75) {
        Engine_TaskWait(1);
        cnt++;
        if (cnt >= 600) {
            return;
        }
    }
}

void SceneState_InstallTask8714AndApplyTwoRects(void)
{
    BattleEffect_PauseObject(31);
    GameFlag_Set(820);                 /* 205 << 2 */

    if (KorosseoKabe_SpectatorPhase != 0) {
        KorosseoKabe_SpectatorTimer = 0;
    }

    Engine_TaskWait(30);
    Engine_TaskWait(1);

    /* The task is published as its entry address with the Thumb bit set. The
     * `.thumb_set` alias the exact reconstruction emits for a Thumb symbol already carries
     * bit 0, so adding it again here overshoots by one. */
    Engine_TaskRemoveCallback((s32)FieldScene_RunSupplementalSequenceOne);

    Map_CopyCellAttributes(58, 28, 7, 1, 58, 13);
    Map_CopyCellAttributes(57, 11, 1, 1, 58, 11);
}

/* The wall stage's start: the party walks up to the guide, the two face each
 * other and bow, then walk on to the wall; the result the approach returned
 * weights the round, and a retreat returns to the wall's entrance four. */
void KorosseoKabe_RunStageStart(void)
{
    s32 rec8;
    s32 v5;
    s32 base;
    s32 base3_2000240;

    base = 2;
    SceneState_EmptyHook();
    Engine_EventBegin();
    rec8 = FieldScene_RunFlag211ApproachScene(77, 89);
    SceneState_WaitUntilStatusNine();
    v5 = 9;
    do {
        Object_RefreshSelectorById(8);
        v5 = (v5 - 1);
    } while (v5 >= 0);
    Call3((void (*)())Engine_ActorSetSpeed, 8, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkTo, 8, 88, 0x100);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3((void (*)())Engine_ActorWalkToAndWait, 0, 120, 0x100);
    Engine_ActorSetAnimation(8, 1);
    Engine_ActorFaceEachOther(0, 8, 0);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(8, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Call3((void (*)())Engine_ActorSetSpeed, 0, 0x20000, 0x10000);
    Call3((void (*)())Engine_ActorSetSpeed, 8, 0x20000, 0x10000);
    Call3((void (*)())Engine_ActorWalkTo, 0, 112, 0x100);
    Engine_ActorWalkToAndWait(8, 96, 0x100);
    Engine_ActorSetAnimation(0, 16);
    Engine_ActorSetAnimation(8, 9);
    Engine_EventWait(10);
    /* FAKEMATCH: base is set at the top of the function, so 2 - rec8 + 1
     * is not folded into 3 - rec8. */
    BattleFx_SetWeightedResult(72, base - rec8 + 1);
    base3_2000240 = (s32)&gGameState;
    /* FAKEMATCH: the do/while keeps the stage flag store ahead of the
     * pool load that follows it. */
    do {
        *(u8 *)((base3_2000240 + 0x22b)) = 3;
    } while (0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_KorosseoKabe, 4);
    Event_SetPair1d4((s32)&SceneId_KorosseoKabe, 5);
    Engine_GameFlagSet(0x11a);
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gKorosseoKabeEvents;
}

void FieldScene_RunPairedEntranceWalk(s32 a0)
{

    u32 i;
    s32 record;

    Engine_ActorDestroy(40);
    Engine_ActorDestroy(41);
    Owner_RefreshActiveRatios(1);
    Engine_EventBegin();
    Actor_SetPosition(8, 0x580000, 0x1000000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0x780000, 0x1000000);
    Actor_FaceActor(8, 0x4000, 0);
    Actor_FaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
    if (a0 < 0) {
        Engine_ActorSetAnimation(8, 10);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 35);
    } else {
        Engine_ActorSetAnimation(8, 8);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 28);
    }
    Engine_TaskWait(1);
    Camera_MoveTo(0x680000, 0, 0xc00000, 0);
    FieldScene_RunLateSequence(a0);
    Engine_EventEnd();
}

/* Wall arena entry: record the arrival, set the pillars, ledges and item icons by the story flags, then start the entrance's opening scene. */
s32 KorosseoKabe_ApplyEntryState(void)
{
    /* FAKEMATCH: the game state read as rows of halfwords and written as rows
     * of bytes keeps the base-plus-index address form for both accesses, where
     * its fields fold the offsets into the pool. */
    union GameStateRows {
        u8 bytes[512][2];
        s16 halves[512][1];
    };

    struct FieldActor *actor;
    s32 x;
    s32 i;
    s32 col;
    s32 k;
    s32 pos;
    s32 row;
    s32 zero;

    gEventWork->start_transition = 0;
    Engine_GameFlagSet(0x144);
    Call6(Engine_MapCopyCellAttributes, 14, 11, 12, 4, 100, 11);
    Call6(Engine_MapCopyCellAttributes, 48, 10, 5, 6, 120, 10);
    for (i = 26; i <= 30; i++) {
        actor = Object_GetById(i);
        Object_SetMode(actor, 4);
        actor->motion_flags = 0;
        actor->y.fixed = 0;
        actor->priority_flags = 2;
    }
    Object_GetById(18)->priority_flags = 2;
    if (Engine_GameFlagIsSet(0x330)) {
        actor = Object_GetById(30);
        actor->x.fixed = 0x1500000;
        actor->y.fixed = -0x80000;
        actor->z.fixed = 0x1080000;
        Call6(Engine_MapCopyCellAttributes, 19, 16, 1, 1, 20, 16);
        Call6(Engine_MapCopyCellAttributes, 20, 80, 1, 1, 21, 80);
    } else {
        actor = Object_GetById(30);
        Object_SetMode(actor, 3);
        actor->y.fixed = 0x100000;
    }
    Object_GetById(11)->priority_flags = 2;
    zero = 0;
    if (Engine_GameFlagIsSet(0x335)) {
        Call6(Engine_MapCopyCellAttributes, 35, 78, 1, 1, 35, 77);
    }
    if (Engine_GameFlagIsSet(0x333)) {
        Engine_ActorSetAnimation(19, 4);
        Call6(Engine_MapCopyCellAttributes, 32, 37, 1, 4, 32, 77);
    }
    if (Engine_GameFlagIsSet(0x331)) {
        Object_GetById(20)->motion_flags = zero;
        Object_GetById(20)->priority_flags = 2;
        Engine_ActorSetAnimation(20, 5);
        Call6(Engine_MapCopyCellAttributes, 46, 17, 1, 1, 44, 17);
    }
    if (Engine_GameFlagIsSet(0x332)) {
        Object_GetById(21)->motion_flags = zero;
        Object_GetById(21)->priority_flags = 2;
        Engine_ActorSetAnimation(21, 5);
        Call6(Engine_MapCopyCellAttributes, 46, 17, 1, 1, 50, 17);
    }
    actor = Object_GetById(32);
    col = actor->x.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Engine_MapCopyCellAttributes(52, 28, 1, 3, col, 10);
    actor = Object_GetById(33);
    col = actor->x.fixed >> 20;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Engine_MapCopyCellAttributes(52, 28, 1, 3, col, 13);

    x = GameFlag_GetByte(0x340);
    if (x == 0) {
        x = 73;
    }
    actor = Object_GetById(12);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Engine_MapCopyCellAttributes(71, 16, 1, 1, x, 16);
    x = GameFlag_GetByte(0x348);
    if (x == 0) {
        x = 76;
    }
    actor = Object_GetById(13);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Engine_MapCopyCellAttributes(71, 16, 1, 1, x, 16);
    x = GameFlag_GetByte(0x350);
    if (x == 0) {
        x = 79;
    }
    actor = Object_GetById(14);
    actor->x.fixed = (x << 20) + 0x80000;
    actor->motion_flags = zero;
    actor->priority_flags = 2;
    Engine_MapCopyCellAttributes(71, 16, 1, 1, x, 16);

    SceneState_ApplyRectsForActors15To17();
    Engine_ActorSetAnimation(31, 10);
    if (Engine_GameFlagIsSet(0x334)) {
        for (k = 22, row = 13, pos = 58; k <= 25; k++, pos += 2) {
            actor = Object_GetById(k);
            actor->priority_flags = 2;
            Object_SetMode(actor, 4);
            Engine_MapCopyCellAttributes(56, 13, 1, 1, pos, row);
        }
        Engine_ActorSetAnimation(31, 10);
        BattleEffect_PauseObject(31);
    } else {
        for (k = 22; k <= 25; k++) {
            actor = Object_GetById(k);
            actor->priority_flags = 2;
            Object_SetMode(actor, 4);
            actor->speed = 0x8000;
            actor->acceleration = 0x3333;
        }
        Engine_TaskAddCallback(FieldScene_RunSupplementalSequenceOne, 0xc85);
        Scheduler_SetCallbackMask((u32)(FieldScene_RunSupplementalSequenceOne), 1);
    }
    Engine_ActorSetAnimation(8, 9);
    ((union GameStateRows *)&gGameState)->bytes[249][0] = 0;
    Korosseo_ShowItemIcon(41, 89);
    Korosseo_ShowItemIcon(40, 77);
    Engine_ActorSetChildValue(8, 1);
    switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
    case 1:
        /* FAKEMATCH: called as returning a value, the call sets its first
         * argument last, as the game does. */
        ((s32 (*)(s32, s32, s32, s32, s32, s32, s32))FieldScene_BuildDescriptorAndInstallTask)(
            0, 8, 5, 0x680000, 0x1000000, 40, 41);
        Call6(Map_UpdateCellRect, 127, 0, 1, 2, 79, 6);
        Engine_ActorDestroy(34);
        Engine_ActorDestroy(35);
        Engine_ActorDestroy(36);
        Engine_ActorDestroy(37);
        Engine_ActorDestroy(38);
        Engine_ActorDestroy(39);
        if (!Engine_GameFlagIsSet(0x109)) {
            Engine_AudioPlayCue(17);
            Korosseo_SelectSoloCompetitor(0);
            SceneActor_PlacePartyAtSavedTiles();
            KorosseoKabe_RunScriptedTransition(2);
        }
        Object_LinkObjectAndSetCallback(1, 0);
        Object_LinkObjectAndSetCallback(2, 0);
        Object_LinkObjectAndSetCallback(3, 0);
        SceneState_InitControlRecordAndStartTask((s32)&ResourceId_RivalPathB);
        break;
    case 2:
        Engine_TaskAddCallback(KorosseoKabe_MarkSceneProgress, 0xc80);
        Engine_ActorDestroy(40);
        Engine_ActorDestroy(41);
        if (!Engine_GameFlagIsSet(0x109)) {
            SceneActor_PlacePartyAtSavedTiles();
            Korosseo_SelectSoloCompetitor(1);
            KorosseoKabe_RunScriptedTransition(0);
        }
        break;
    case 3:
        if (!Engine_GameFlagIsSet(0x109)) {
            Korosseo_RunGreetScene(34);
            FieldScene_RunSixSteps896To936();
        }
        break;
    case 4:
        FieldScene_RunPairedEntranceWalk(2);
        Engine_EventRequestExit(4);
        break;
    case 5:
        FieldScene_RunPairedEntranceWalk(-2);
        Engine_EventRequestExit(5);
        break;
    }
    return 0;
}

/* Colosso: line the other competitors up around actor a0, show its message
 * and walk it back to its place while the others gather on actor 0. */
/* Colosso: the competitors are greeted before a trial. The same scene sits
 * in the wall and log-rolling trial overlays. */
void Korosseo_RunGreetScene(s32 a0)
{
    s32 p10;
    s32 p9;
    struct FieldActor *record;

    record = Object_GetById(a0);
    p9 = record->x.part.pixel;
    p10 = record->z.part.pixel;
    Engine_EventBegin();
    Call3(Engine_ActorSetSpeed, a0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 1, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 2, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 3, 0x10000, 0x8000);
    Engine_ActorSetPosition(0, (p9 << 16), ((p10 << 16) - 0x300000));
    Engine_ActorSetPosition(1, (p9 << 16) - 0x100000, (p10 << 16) - 0x280000);
    Engine_ActorSetPosition(2, (p9 << 16) + 0x100000, (p10 << 16) - 0x280000);
    Engine_ActorSetPosition(3, (p9 << 16), ((p10 << 16) - 0x200000));
    Engine_ActorSetPosition(a0, (p9 << 16), ((p10 << 16) - 0x500000));
    record = Object_GetById(0);
    record->facing = 0xc000;
    Engine_CameraFollowActor(0, 0);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventSetMessage((s32)MsgKorosseoSiteSecondFinals);
    Engine_ActorSetAnimationAndWait(a0, 3);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorRunRepeatedMotion(a0, 2);
    Engine_EventShowMessage(a0, 0);
    Engine_ActorSetAnimation(3, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(1, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(1, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorSetAnimation(2, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(2, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorSetAnimation(3, 2);
    record = Object_GetById(0);
    if (record != 0) {
        Engine_ActorSetDestination(3, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 64));
    Engine_ActorSetPosition(1, 0, 0);
    Engine_ActorSetPosition(2, 0, 0);
    Engine_ActorSetPosition(3, 0, 0);
    Engine_ActorWalkToAndWait(a0, (p9 - 16), (p10 - 16));
    Engine_ActorWalkToAndWait(a0, p9, p10);
    Engine_ActorFaceDirection(a0, 0xc000, 10);
    Engine_EventEnd();
}

/* Colosso wall stage: unless the stage is already cleared, walk the player
 * along the wall, show the introduction and hand over to the stage. */
void KorosseoKabe_RunStageIntro(s32 a0)
{
    /* FAKEMATCH: retained return/prototype call casts preserve the current call lowering. */

    s32 rec;
    struct FieldActor *record;

    if (gCell[225] == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec = KorosseoKabe_RunStateInteraction(a0, 1);
        if (rec == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoStageDubbedMini);
            Engine_CameraSetSpeed(0x30000, 0x6000);
            Engine_CameraMoveTo(0x4c80000, -1, 0xb80000, 1);
            Engine_CameraWaitForMove();
            Engine_EventShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x4f8, 168);
            Call3(Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x508, 184);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x508, 216);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x4c8, 216);
            Engine_EventShowMessage(a0, 0);
            ((s32 (*)())SceneActor_PlaceWithScale14000)(0, 0x4c8, 248);
            SceneActor_PlaceWithScale14000(0, 0x4a8, 248);
            Engine_EventWait(3);
            record = Object_GetById(0);
            record->velocity_y = 0x40000;
            Engine_ActorSetAnimation(0, 28);
            Engine_ActorSetAttachedEffect(0, 0x102);
            Engine_EventWait(30);
            Engine_EventShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(0, 0);
            KorosseoKabe_ShowFollowUpPrompt(a0, 1);
        } else {
            if (rec == 1) {
                Engine_EventSetMessage((s32)MsgKorosseoMainThingStage);
                Engine_EventShowMessage(a0, 0);
            }
        }
        ((s32 (*)())FieldScene_RunMiddleSequence)(rec, a0, 1);
        Engine_EventEnd();
    }
}

void FieldScene_RunSecondActorInteraction(s32 a0)
{
    u32 i;
    s32 rec;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec = KorosseoKabe_RunStateInteraction(a0, 2);
        if (rec == 0) {
            Engine_EventSetMessage((s32)MsgKorosseoShiftingFloorStage);
            FieldScene_PlaceSpectatorRow();
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x3d80000, -1, 0xe80000, 1);
            Engine_CameraWaitForMove();
            /* FAKEMATCH: the void result is discarded; Call2 changes argument allocation. */
            Value2(Engine_EventShowMessage, a0, 0);
            SceneState_ApplyTable8715AndValue104();
            Event_ShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x438, 0x108);
            Engine_EventWait(15);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(SceneActor_PlaceWithScale14000, 0, 0x438, 216);
            SceneActor_PlaceWithScale14000(0, 0x428, 216);
            SceneState_WaitForStatusWords();
            Engine_LeaderCheckAhead();
            Camera_MoveTo(-1, -1, -1, 0);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            KorosseoKabe_ShowFollowUpPrompt(a0, 2);
        } else {
            if (rec == 1) {
                Engine_EventSetMessage((s32)MsgKorosseoOperatorLiftsWillCheerFor);
                Event_ShowMessage(a0, 0);
            }
        }
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(FieldScene_RunMiddleSequence, rec, a0, 2);
        Engine_EventEnd();
    }
}

void FieldScene_RunSceneThreeCoordinator(s32 a0)
{
    u32 i;
    s32 rec2;
    s32 record;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
    } else {
        Engine_EventBegin();
        rec2 = KorosseoKabe_RunStateInteraction(a0, 3);
        if (rec2 != 0) {
        } else {
            Engine_EventSetMessage((s32)MsgKorosseoLogRollingStage);
            Camera_SetSpeed(0x30000, 0x6000);
            Camera_MoveTo(0x2f00000, -1, 0xc00000, 1);
            Engine_CameraWaitForMove();
            Engine_EventWait(60);
            Camera_SetSpeed(0x10000, 0x2000);
            Camera_MoveTo(0x2f00000, -1, 0xe00000, 1);
            Engine_CameraWaitForMove();
            Event_ShowMessage(a0, 0);
            Korosseo_FadeInCompetitor(0, 0x358, 0x108);
            Engine_EventWait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x18000, 0xc000);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(SceneActor_PlaceWithScale14000, 0, 0x358, 0x108);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(SceneActor_PlaceWithScale14000, 0, 0x358, 232);
            Event_ShowMessage(a0, 0);
            SceneActor_PlaceWithScale14000(0, 0x348, 232);
            Engine_EventWait(10);
            /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
            Value3(SceneActor_MovePairByTileOffset, 33, -64, 0);
            Camera_MoveTo(0x2f00000, -1, 0xd80000, 1);
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
            Engine_EventWait(10);
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
            Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x2f8, 232);
            Engine_EventWait(10);
            Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x4000, 30);
            Event_ShowMessage(a0, 0);
            Korosseo_RestoreCompetitor(0);
            Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
            Actor_SetPosition(33, 0x3480000, 0xe80000);
            KorosseoKabe_ShowFollowUpPrompt(a0, 3);
            goto L_020016b0;
        }
        if (rec2 == 1) {
            Engine_EventSetMessage((s32)MsgKorosseoHereYourObjectiveRideLogs);
            Event_ShowMessage(a0, 0);
        }
        L_020016b0:;
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(FieldScene_RunMiddleSequence, rec2, a0, 3);
        Engine_EventEnd();
    }
}

/* In transition phase 2 only the fast path runs. Otherwise route 0 plays the
 * full presentation, passing actor 0's position on with Y raised by 0x400000,
 * route 1 plays the short revisit, and every such path ends in the common
 * coordinator tail. */
void FieldScene_RunLiftedActorCoordinator(s32 scene)
{
    void *actor;
    s32 path;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    path = KorosseoKabe_RunStateInteraction(scene, 4);
    if (path == 0) {
        Engine_EventSetMessage((s32)MsgKorosseoFansJustCallWall);
        Engine_CameraSetSpeed(196608, 24576);
        Call4(Engine_CameraMoveTo, 35127296, -1, 15728640, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(45);
        Call2(Engine_CameraSetSpeed, 65536, 8192);
        Call4(Engine_CameraMoveTo, 35127296, -1, 12582912, 1);
        Engine_CameraWaitForMove();
        Engine_EventShowMessage(scene, 0);
        Korosseo_FadeInCompetitor(0, 632, 264);
        Actor_SetSpeed(ACTOR_PARTY_LEADER, 65536, 32768);
        Call3(Engine_ActorWalkToAndWait, 0, 616, 264);
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(Engine_ActorFaceDirection, 0, 49152, 20);
        battle_owner_69();
        Call2(Engine_CameraSetSpeed, 16384, 2048);
        Call4(Engine_CameraMoveTo, 35127296, -1, 10485760, 1);
        Call3(Engine_ActorSetSpeed, 0, 32768, 16384);
        Engine_ActorSetAnimation(0, 10);
        actor = Object_GetById(0);
        Engine_ObjectSetPosition(actor, *(s32 *)((u8 *)actor + 8),
            *(s32 *)((u8 *)actor + 12) + 4194304,
            *(s32 *)((u8 *)actor + 16));
        ObjectMotion_CommitCurrentPositionAndActivate(0);
        battle_owner_69();
        Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
        Event_ShowMessage(scene, 0);
        Call3(Engine_ActorSetSpeed, 0, 98304, 49152);
        /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
        Value3(SceneActor_PlaceWithScale14000, 0, 488, 248);
        Call3(Engine_ActorFaceDirection, 0, 16384, 20);
        BattleFx_RunRisingObjectSequence(0, 6, 0);
        Engine_CameraMoveTo(35127296, -1, 10485760, 1);
        Engine_EventShowMessage(scene, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(0, 0);
        KorosseoKabe_ShowFollowUpPrompt(scene, 4);
    } else if (path == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoScalingWallQuickly);
        Engine_EventShowMessage(scene, 0);
    }
    /* FAKEMATCH: the void result is discarded; Call3 changes argument allocation. */
    Value3(FieldScene_RunMiddleSequence, path, scene, 4);
    Engine_EventEnd();
}

/* The leader pushes wall block id to cell (column / 2, row): the block slides
 * there and the leader follows half the distance behind it. */
void KorosseoKabe_PushBlockToCell(s32 id, s32 column, s32 row)
{
    s32 pusher = gGameState.selected_actor;
    struct FieldActor *leader = Object_GetById(pusher);
    struct FieldActor *block = Object_GetById(id);
    s32 along_x = (block->x.fixed >> 20) != column / 2;
    s32 dx;
    s32 dz;

    column <<= 16;
    row <<= 16;
    if (along_x) {
        dx = (column - block->x.fixed) / 2;
        dz = 0;
    } else {
        dx = 0;
        dz = (row - block->z.fixed) / 2;
    }
    Engine_ActorSetAnimation(pusher, 8);
    Engine_EventWait(6);
    block->speed = 0x8000;
    block->acceleration = 0x3333;
    Engine_AudioPlayCue(239);
    Object_SetMode(block, 3);
    Engine_ObjectSetPosition(block, column, 0, row);
    Engine_EventWait(6);
    Engine_ActorSetAnimation(pusher, 2);
    Engine_ActorSetSpeed(pusher, 0x8000, 0x3333);
    Object_SetMode(leader, 2);
    Engine_ObjectSetPosition(leader, leader->x.fixed + dx, 0, leader->z.fixed + dz);
    Object_CommitPosition(leader);
    Object_SetMode(leader, 1);
    Object_CommitPosition(block);
    Object_SetMode(block, 1);
    Engine_AudioPlayCue(288);
    Engine_AudioPlayCue(213);
    Engine_EventWait(15);
}

/* The Colosso guide at the Board Walk: on the first visit pans the camera
 * over the course and walks the competitor through pushing a block while
 * explaining it; afterwards reminds that the logs are rolled into a path. */
void KorosseoKabe_RunGuideTalk(s32 speaker)
{
    /* FAKEMATCH: retained return/prototype call casts preserve the current call lowering. */

    s32 result;

    if (gGameState.entrance == 2) {
        Korosseo_FinishSoloRound();
        return;
    }
    Engine_EventBegin();
    result = KorosseoKabe_RunStateInteraction(speaker, 5);
    if (result == 0) {
        s32 x;
        s32 z;

        Engine_EventSetMessage((s32)MsgKorosseoPlaceCalledBoard);
        Engine_CameraSetSpeed(0x20000, 0x4000);
        Call4((void (*)())Engine_CameraMoveTo, 0x1480000, -1, 0x1080000, 1);
        Engine_CameraWaitForMove();
        Engine_EventWait(30);
        Call2((void (*)())Engine_CameraSetSpeed, 0x18000, 0x3000);
        x = 408;
        z = 264;
        Engine_CameraMoveTo(0x1380000, -1, 0xb00000, 1);
        Engine_CameraWaitForMove();
        Engine_EventShowMessage(speaker, 0);
        Korosseo_FadeInCompetitor(0, x, z);
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
        ((s32 (*)(s32, s32, s32))SceneActor_PlaceWithScale14000)(0, x, 216);
        Call3((void (*)())Engine_ActorFaceDirection, 0, 0x8000, 10);
        Engine_EventShowMessage(speaker, 0);
        KorosseoKabe_PushBlockToCell(16, 360, 208);
        Engine_ActorShowEmote(0, z, 45);
        Call3((void (*)())Engine_ActorSetSpeed, 0, 0x18000, 0xc000);
        x -= 32;
        ((s32 (*)(s32, s32, s32))SceneActor_PlaceWithScale14000)(0, x, 216);
        ((s32 (*)(s32, s32, s32))SceneActor_PlaceWithScale14000)(0, x, 248);
        SceneActor_PlaceWithScale14000(0, 312, 248);
        Engine_EventShowMessage(speaker, 0);
        Korosseo_RestoreCompetitor(0);
        Engine_CameraFollowActor(0, 0);
        Call3((void (*)())Engine_ActorSetPosition, 16, 0x1880000, 0xd00000);
        KorosseoKabe_ShowFollowUpPrompt(speaker, 5);
    } else if (result == 1) {
        Engine_EventSetMessage((s32)MsgKorosseoFocusRollingLogs);
        Engine_EventShowMessage(speaker, 0);
    }
    ((s32 (*)(s32, s32, s32))FieldScene_RunMiddleSequence)(result, speaker, 5);
    Engine_EventEnd();
}

void SceneActor_PlacePartyAtSavedTiles(void)
{
    {
        s32 x = GameFlag_GetByte(896);
        s32 y = GameFlag_GetByte(904);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_GERALD, x, y);
    }
    {
        s32 x = GameFlag_GetByte(912);
        s32 y = GameFlag_GetByte(920);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_IVAN, x, y);
    }
    {
        s32 x = GameFlag_GetByte(928);
        s32 y = GameFlag_GetByte(936);

        x <<= 20;
        x += 0x80000;
        y <<= 20;
        y += 0x80000;
        Actor_SetPosition(ACTOR_MIA, x, y);
    }
}
