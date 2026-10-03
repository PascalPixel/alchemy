#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "ARUTIN.H"
#include "CALL.H"

extern const struct SceneEntrance gArutinMuraEntrances1[];
extern const struct SceneEntrance gArutinMuraEntrances2[];
extern const struct SceneEntrance gArutinMuraEntrancesOther[];

/* The scene's message table, laid out after the code. */
extern u8 Placement_Messages[];

extern u8 gArutinMuraPlacements1[];
extern u8 gArutinMuraPlacements2[];
extern u8 gArutinMuraPlacementsOther[];
void FieldScene_PrepareActors(void *placements);

extern const struct SceneEvent gArutinMuraEvents1[];
extern const struct SceneEvent gArutinMuraEvents2[];
extern const struct SceneEvent gArutinMuraEventsOther[];

extern u8 MsgArutinDefeatedThoseMonstersDidnt[];
extern u8 MsgArutinDidSeeWaterGushingOut[];
extern u8 MsgArutinDoWantWeapons[];
extern u8 MsgArutinGirlFromXianBoughtLot[];
extern u8 MsgArutinGirlFromXianWasAsking[];
extern u8 MsgArutinHowAboutArentImpressedBy[];
extern u8 MsgArutinItsGreatCanSellArmor[];
extern u8 MsgArutinMyStoreSubmergedWantSell[];
extern u8 MsgArutinNoneMyGoodsWereDamaged[];
extern u8 MsgArutinOnesWhoDefeatedWaterBeasts[];
extern u8 MsgArutinThankGoodnessWaterHasReceded[];
extern u8 MsgArutinThereFewBeastsInMine[];
extern u8 MsgArutinThereSmallTempleWestAltin[];
extern u8 MsgArutinTrueFoundAncientRuinsIn[];
extern u8 MsgArutinTryingFindYourWayWest[];
extern u8 MsgArutinWeCantDrinkWaterMonsters[];
extern u8 MsgArutinWeGotLittleDampBut[];
extern u8 MsgArutinWillDoIfMyMerchandise[];
extern u8 MsgArutinYoullHaveFindPassageIn[];
extern u8 MsgArutinYourFirstTimeVisitAltin[];
void FieldScene_RunMiddleSequence(void);
void FieldScene_RunScene3a3SequenceD(void);

/* Where the party appears in each of Altin's two areas. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ArutinMura1) {
        return gArutinMuraEntrances1;
    }
    if (v == (s32)&SceneId_ArutinMura2) {
        return gArutinMuraEntrances2;
    }
    return gArutinMuraEntrancesOther;
}

/* The table accessors between the scene-id selectors at the head of the
   overlay. */
s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

/* The actors placed in each of Altin's two areas, patched in place by the
   story flags: the overlay image is writable. The second area's table is
   prepared before it is returned. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 id = gGameState.scene;
    if (id == (s32)&SceneId_ArutinMura1) {
        if (Engine_GameFlagIsSet(0x909)) {
            gArutinMuraPlacements1[142] = 0;
            gArutinMuraPlacements1[166] = 0;
        }
        return (const struct ScenePlacement *)gArutinMuraPlacements1;
    }
    if (id == (s32)&SceneId_ArutinMura2) {
        if (Engine_GameFlagIsSet(0x8fd))
            gArutinMuraPlacements2[46] = 1;
        if (Engine_GameFlagIsSet(0x8fe) || Engine_GameFlagIsSet(0x907))
            gArutinMuraPlacements2[94] = 1;
        FieldScene_PrepareActors(gArutinMuraPlacements2);
        return (const struct ScenePlacement *)gArutinMuraPlacements2;
    }
    return (const struct ScenePlacement *)gArutinMuraPlacementsOther;
}

/* What each of Altin's two areas answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s16 v = gGameState.scene;

    if (v == (s32)&SceneId_ArutinMura1) {
        return gArutinMuraEvents1;
    }
    if (v == (s32)&SceneId_ArutinMura2) {
        return gArutinMuraEvents2;
    }
    return gArutinMuraEventsOther;
}

void FieldScene_RunActorEightPromptDialogue(void)
{
    u8 *work;

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinYourFirstTimeVisitAltin);
    /* r1 is set before r0; the argument order is unchanged. */
    Engine_EventOpenMessage(8, 0);

    if (Engine_EventChooseYesNo(0, 0) == 1) {
        Engine_EventShowMessage(8, 0);
    } else {
        work = (u8 *)gEventWork;
        *(u16 *)(work + 472) = (u16)(*(u16 *)(work + 472) + 1);
        Engine_EventAskYesNo(8, 0);
    }

    Engine_EventEnd();
}

void SceneDialogue_ShowLine1918(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinDidSeeWaterGushingOut);
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

/*
 * One of two branches of the opening setup: a short branch that only moves
 * actor 14, or a longer branch that positions actor 18 and a second record
 * from their x/y/z fields at +8/+12/+16, clearing a byte at +85 of a
 * separately looked-up record on the way.
 */
void FieldScene_RunOpeningAuxiliarySequence(void)
{

    u8 *rec18;
    u8 *ready_flag;
    u8 *record;

    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinOnesWhoDefeatedWaterBeasts);
        Engine_EventAskYesNo(14, 0); /* object 14, action 0 */
    } else {
        Engine_ActorSetAnimation(14, 4); /* object 14, action 4 */
        Engine_EventSetMessage((s32)MsgArutinWeCantDrinkWaterMonsters);
        Engine_EventShowMessageAndWait(14, 0, 10);
        ready_flag = Engine_GameFlagIsSet(0x8ff);
        if (ready_flag == 0) {
            rec18 = (u8 *)Object_GetById(18);
            /* Clear the byte at +85 of the lookup result. */
            *(u8 *)(Battle_GetWorkObject1e0() + 85) = ready_flag;
            Engine_CameraSetSpeed(0x10000, 0x2000);
            Engine_CameraMoveTo(*(s32 *)(rec18 + 8), *(s32 *)(rec18 + 12), *(s32 *)(rec18 + 16), 1); /* use_setter 1 */
            Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 0x4000, 0);
            Engine_ActorFaceDirection(14, 0x3000, 0);
            Engine_CameraWaitForMove();
            Engine_EventWait(120); /* should_wait 120 */
            record = (u8 *)Object_GetById(0);
            Engine_CameraMoveTo(*(s32 *)(record + 8), *(s32 *)(record + 12), *(s32 *)(record + 16), 1); /* use_setter 1 */
            Engine_CameraWaitForMove();
        }
        Engine_ActorSetAnimationAndWait(14, 4);
    }
    Engine_EventEnd();
}

void SceneDialogue_RunActor17Message1924(void)
{
    void Engine_EventEnd(void);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinTrueFoundAncientRuinsIn);
    Engine_EventAskYesNo(17, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor9Message1932(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinGirlFromXianWasAsking);
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor10Message18d9(void)
{
    void Engine_EventBegin(void);
    void Engine_EventEnd(void);

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinTryingFindYourWayWest);
    Engine_EventAskYesNo(10, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor14Message18e1(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinDefeatedThoseMonstersDidnt);
    Engine_EventAskYesNo(14, 0);
    Engine_EventEnd();
}

void SceneDialogue_RunActor21Message194a(void)
{

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinThereFewBeastsInMine);
    Engine_EventAskYesNo(21, 0);
    Engine_EventEnd();
}

s32 SceneActor_IsSlotZeroAngleInRange(void)
{
    struct Slot02000338 *slot = (struct Slot02000338 *)Object_GetById(0);

    if ((u32)((slot->angle + 0x5FFF) << 16) <= 0x3FFE0000) {
        return 1;
    }
    return 0;
}

void FieldScene_RunActorFifteenFlagBranch(void)
{
    void Engine_EventSetMessage();
    void Engine_ShopOpen();

    if (Engine_GameFlagIsSet(0x242) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgArutinDoWantWeapons);
        /* r1 is set before r0 here; the argument order is unchanged. */
        Engine_EventAskYesNo(15, 0);
        Engine_EventEnd();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Engine_ShopOpen(19, 15);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinThankGoodnessWaterHasReceded);
    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinYoullHaveFindPassageIn);
    }
    Engine_EventShowMessage(15, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorTwentyFlagBranch(void)
{
    void Engine_EventEnd(void);
    void Engine_EventEnd(void);
    void Engine_EventSetMessage(s32);

    if (Engine_GameFlagIsSet(0x241) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgArutinMyStoreSubmergedWantSell);
        Engine_EventShowMessage(20, 0);
        Engine_EventEnd();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Engine_ShopOpen(20, 17);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinItsGreatCanSellArmor);
    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinHowAboutArentImpressedBy);
    }
    Engine_EventShowMessage(17, 0);
    Engine_EventEnd();
}

void FieldScene_RunActorTwentyOneFlagBranch(void)
{
    if (Engine_GameFlagIsSet(0x240) == 0) {
        Engine_EventBegin();
        Engine_EventSetMessage((s32)MsgArutinWillDoIfMyMerchandise);
        Engine_EventShowMessage(21, 0);
        Engine_EventEnd();
        return;
    }

    if (SceneActor_IsSlotZeroAngleInRange() != 0) {
        Engine_ShopOpen(21, 16);
        return;
    }

    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgArutinNoneMyGoodsWereDamaged);
    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinGirlFromXianBoughtLot);
    }
    Engine_EventShowMessage(16, 0);
    Engine_EventEnd();
}

void FieldScene_RunFacingGatedDialogue18(void)
{
    u8 *rec;

    rec = (u8 *)Object_GetById(0);

    if ((u32)((*(u16 *)(rec + 6) + 0x5fff) << 16) <= 0x3ffe0000) {
        Engine_InnOpen(6, 18);
        return;
    }

    Engine_EventBegin();

    if (Engine_GameFlagIsSet(0x909) != 0) {
        Engine_EventSetMessage((s32)MsgArutinThereSmallTempleWestAltin);
        Engine_EventShowMessage(18, 0);
    } else {
        Engine_EventSetMessage((s32)MsgArutinWeGotLittleDampBut);
        Engine_EventAskYesNo(18, 0);
    }

    Engine_EventEnd();
}

void FieldScene_RunEarlySequence(void)
{
    u32 i;
    u8 *work;
    u8 *record;
    s32 idx;
    u8 *tbl;
    s32 off;
    s32 o4;
    s32 a;
    s32 b;
    s32 c;

    work = (u8 *)gEventWork;
    Engine_EventBegin();
    for (i = 8; i <= 65; i++) {
        record = (u8 *)Object_GetById(i);
        if (record != 0) {
            record[85] = 0;
        }
    }
    tbl = ArutinMura_TriggerTable;
    idx = ((s32)((s32)(*(u16 *)(work + 0x16c) - 2) << 16) >> 16);
    off = idx << 3;
    o4 = off + 4;
    a = *(s16 *)(tbl + o4);
    b = *(s16 *)(tbl + o4 + 2);
    if (idx == 1) {
        Engine_AudioPlayCue(188);
        Engine_MapCopyCellsTo(42, 33, a, b, 2, 2);
        c = a + 2;
        Engine_MapCopyCellsTo(42, 35, c, b, 2, 2);
        Engine_EventWait(4);
        Engine_MapCopyCellsTo(40, 33, a, b, 2, 2);
        Engine_MapCopyCellsTo(40, 35, c, b, 2, 2);
        Engine_EventWait(4);
    } else {
        Engine_AudioPlayCue(158);
        if (idx == 3) {
            Engine_MapCopyCellsTo(33, 42, 8, 17, 1, 2);
        }
        Engine_MapAnimateCells(*(s32 *)(tbl + off), a, b);
    }
    Engine_ActorSetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x100;
    *(u8 *)((s32)Object_GetById(0) + 85) = 0;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    if (idx == 6) {
        Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, 0);
    } else {
        if (idx != 1) {
            Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -4);
        } else {
            Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 2);
            Engine_ActorSetDestinationOffset(ACTOR_PARTY_LEADER, 0, -4);
        }
    }
    Engine_EventWait(10);
    Engine_EventRequestExit(*(s16 *)(work + 0x16c));
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void FieldScene_RunScene3a3SequenceB(void)
{
    u8 *work;

    work = (u8 *)gEventWork;
    Engine_EventBegin();
    *(u8 *)((s32)Object_GetById(0) + 85) = 0;
    Engine_AudioPlayCue(123);
    Engine_ActorCenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    Engine_EventRequestExit(*(s16 *)(work + 0x16c));
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void SceneMotion_UpdateTimedActor(struct SceneMotion *work)
{
    if (work->delay != 0) {
        if (--work->delay == 1)
            Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
    }
    if (work->velocity == 0) {
        Object_SetMode(work, 1);
        work->y += -0x18000;
        if (work->y < work->ground) {
            if (work->active != 0) {
                Engine_AudioPlayCue(229);
                work->active = 0;
                work->delay = 4;
                Engine_WorkSetValuesIfNonNegative(0, 0x10000, 0x10000);
            }
            work->y = work->ground;
        }
        work->state = 1;
    } else {
        work->state = 0;
    }
    if (work->timer == 0) {
        Engine_AudioPlayCue(152);
        work->active = 1;
        Object_SetMode(work, 2);
        work->velocity = 0x30000;
    }
    if (++work->timer == 60)
        work->timer = 0;
}

void FieldScene_RunScene3a3SequenceC(void)
{
    struct SceneMotion *motion;

    motion = (struct SceneMotion *)Object_GetById(18);
    motion->timer = 0;
    motion->delay = 0;
    *(s32 *)((u8 *)motion + 72) = 0x6666;
    motion->callback = SceneMotion_UpdateTimedActor;
    Engine_ActorSetSpeed(18, 0x13333, 0x9999);
    Engine_ActorMoveToAndWait(18, 28, 0x1cc);
    Engine_ActorMoveToAndWait(18, 24, 0x1c0);
    Engine_AudioPlayCue(229);
    Engine_ActorDestroy(18);
    Engine_WorkSetValuesIfNonNegative(0, 0x10000, 0x10000);
    Engine_EventWait(4);
    Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
    Engine_EventWait(40);
    Engine_ActorSetAnimation(18, 1);
}

void SceneState_SetFlag906ByActorNineteenX(void)
{
    struct Actor *p = (struct Actor *)Object_GetById(19);

    if ((p->f08 >> 20) == 22) {
        Engine_GameFlagSet(0x906);
    } else {
        Engine_GameFlagClear(0x906);
    }
}

/*
 * Altin's scene start: mirror three progress flags into three scene flags,
 * then run the continuation of the area the party enters. Every call here
 * leaves through its own veneer, so the sites stay separate.
 */
s32 Scene_Initialize(void)
{
    s16 scene;

    if (Engine_GameFlagIsSet(0x8fd) != 0) {
        Engine_GameFlagSet(0x240);
    }

    if (Engine_GameFlagIsSet(0x8fe) != 0 || Engine_GameFlagIsSet(0x907) != 0) {
        Engine_GameFlagSet(0x241);
    }

    if (Engine_GameFlagIsSet(0x8fe) != 0 && Engine_GameFlagIsSet(0x907) != 0) {
        Engine_GameFlagSet(0x242);
    }

    scene = gGameState.scene;
    if (scene == (s32)&SceneId_ArutinMura1) {
        FieldScene_RunMiddleSequence();
    } else if (scene == (s32)&SceneId_ArutinMura2) {
        FieldScene_RunScene3a3SequenceD();
    }

    return 0;
}

void FieldScene_RunMiddleSequence(void)
{
    s32 scene;
    s32 rec5;
    s32 rec6;
    s32 rec0;
    s32 kind;

    scene = Object_GetById(0);
    rec5 = Engine_GameFlagIsSet(0x242);
    if (rec5 != 0) {
        Engine_MapCopyCellsTo(64, 32, 0, 32, 32, 32);
        Engine_MapCopyCellAttributes(64, 32, 32, 32, 0, 0);
        kind = 20;
    } else {
        rec6 = Engine_GameFlagIsSet(0x241);
        if (rec6 != 0) {
            Engine_MapCopyCellsTo(64, 0, 0, 32, 32, 32);
            /* FAKEMATCH: the void result is discarded; Call6 changes argument allocation. */
            Value6(Engine_MapCopyCellAttributes, 64, 0, 32, 32, rec5, rec5);
            Engine_ActorDestroy(17);
            kind = 20;
        } else {
            rec0 = Engine_GameFlagIsSet(0x240);
            if (rec0 == 0) {
                goto L_020009b8;
            }
            Engine_MapCopyCellsTo(0, 64, 0, 32, 32, 32);
            /* FAKEMATCH: the void result is discarded; Call6 changes argument allocation. */
            Value6(Engine_MapCopyCellAttributes, 0, 64, 32, 32, rec6, rec6);
            Engine_ActorDestroy(16);
            kind = 17;
        }
    }
    Engine_ActorDestroy(kind);
    Engine_ActorDestroy(21);
    goto L_020009da;
L_020009b8:
    Engine_MapCopyCellAttributes(0, 32, 32, 32, rec0, rec0);
    Engine_ActorDestroy(15);
    Engine_ActorDestroy(16);
    Engine_ActorDestroy(17);
L_020009da:
    if (Engine_GameFlagIsSet(0x8ff) != 0) {
        Engine_ActorDestroy(18);
    } else {
        BattleFx_SetQueuedSoundAndPlay(170);
        Engine_ActorSetChildValue(18, 2);
        Engine_ActorSetAnimation(18, 3);
        ((void (*)())Engine_TaskAddCallback)((s32)SceneEffect_SpawnDriftingParticle, 0xc80);
    }
    if (gGameState.entrance == 3) {
        Engine_GameFlagClear(FLAG_ARRIVAL_EVENT_PENDING);
    }
    Engine_MapCopyCellAttributes(0, 33, 4, 3, 20, 41);
    if (Engine_GameFlagIsSet(0x906) != 0) {
        Engine_ActorSetPosition(19, 0x1680000, 0xa80000);
    }
    Engine_ActorSetSpriteFlags((s32)Object_GetById(19), 0);
    Engine_ActorSetChildValue(22, 15);
    Engine_ActorSetChildValue(23, 15);
    Engine_ActorSetChildValue(24, 15);
    {
        u8 bits = 8;
        u8 *flags = (u8 *)Object_GetById(22) + 89;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = (u8 *)Object_GetById(23) + 89;
        value = *flags;
        value |= bits;
        *flags = value;
        flags = (u8 *)Object_GetById(24) + 89;
        bits |= *flags;
        *flags = bits;
    }
    {
        u8 bits = 2;
        u8 *flags = (u8 *)Object_GetById(22) + 35;
        u8 value = *flags;

        value |= bits;
        *flags = value;
        flags = (u8 *)Object_GetById(23) + 35;
        value = *flags;
        value |= bits;
        *flags = value;
        flags = (u8 *)Object_GetById(24) + 35;
        bits |= *flags;
        *flags = bits;
    }
    Engine_ActorSetSpritePriority(22, 1);
    Engine_ActorSetSpritePriority(23, 1);
    Engine_ActorSetSpritePriority(24, 1);
    Engine_TaskWait(1);
    Engine_EventBegin();
    Engine_CameraMoveTo(*(s32 *)(scene + 8), *(s32 *)(scene + 12), *(s32 *)(scene + 16), 0);
    Engine_MapRedraw();
    Engine_EventEnd();
    Engine_TaskWait(1);
}

void FieldScene_RunScene3a3SequenceD(void)
{
    u8 *actor;
    s32 facing;

    if (Engine_GameFlagIsSet(0x240) == 0) {
        Engine_ActorSetPosition(8, 0x3280000, 0x2d70000);
        actor = (u8 *)Object_GetById(8);
        facing = 0x3000;
        *(u16 *)(actor + 6) = facing;
        Engine_ActorSetPosition(9, 0x31a0000, 0x3390000);
    }
    if (Engine_GameFlagIsSet(0x241) == 0) {
        Engine_ActorSetPosition(10, 0x2300000, 0x2c60000);
        actor = (u8 *)Object_GetById(10);
        facing = 0x1000;
        *(u16 *)(actor + 6) = facing;
        Engine_ActorSetPosition(11, 0x2400000, 0x2c60000);
    }
    if (Engine_GameFlagIsSet(0x242) == 0) {
        Engine_ActorSetPosition(15, 0x1270000, 0x2e80000);
        actor = (u8 *)Object_GetById(15);
        facing = 0xb000;
        *(u16 *)(actor + 6) = facing;
    } else {
        u8 flags;

        actor = (u8 *)Object_GetById(15);
        flags = 4;
        flags |= actor[89];
        actor[89] = flags;
    }
    actor = (u8 *)Object_GetById(17);
    if (actor != 0) {
        u8 flags = 4;

        flags |= actor[89];
        actor[89] = flags;
    }
    actor = (u8 *)Object_GetById(16);
    if (actor != 0) {
        u8 flags = 4;

        flags |= actor[89];
        actor[89] = flags;
    }
}

void SceneActor_ResetStateAndSpan(struct Actor02000c0c *actor)
{

    u8 *state = &actor->state;
    s32 clear = 0;
    u8 *attached;

    *state = (u8)clear;
    attached = actor->attached;
    clear -= 13;
    attached[9] = (clear & attached[9]) | 4;
    ObjectGroup_SetChildValue(actor, 3);
    Engine_ActorSetSpriteFlags((struct FieldActor *)actor, 0);
    actor->span = 0x4CCC;
    actor->reach = 0x4CCC;
}

void SceneEffect_UpdateDriftingParticle(struct SceneMotion *work)
{
    work->x += (work->timer << 12) +
        ((s16)((s32)((Random16() * 2) >> 16) - 1) << 15);
    if (work->timer <= 3) {
        work->z += -((Random16() * 0x8000) >> 16) - 0x10000;
        work->scale_x += 0x2666;
        work->scale_y += -0xa3d;
    } else {
        work->z += 0x20000;
        work->scale_x += 0x7ae;
        work->scale_y += 0x7ae;
    }
    if ((Random16() * work->timer) >> 16 == 0)
        ObjectGroup_SetChildValue(work, 7);
    if (work->timer != 0)
        work->timer--;
    else
        work->timer = ((Random16() * 5) >> 16) * 2 + 2;
    if (--work->active == 0) {
        work->callback = 0;
        Engine_ObjectDispatchRelease(work);
    }
}

void SceneEffect_SpawnDriftingParticle(void)
{
    struct SceneMotion *work;
    if ((gFrameCount & 3) == 0) {
        work = ((struct SceneMotion *)Value4((struct SceneMotion *(*)(s32, s32, s32, s32))Engine_ObjectCreate, 222, 0x400000, 0, 0x1900000));
        if (work != 0) {
            work->timer = 20;
            work->delay = 0;
            work->active = 20;
            SceneActor_ResetStateAndSpan(work);
            work->callback = SceneEffect_UpdateDriftingParticle;
            Object_SetMode(work, 1);
        }
    }
}

void ArutinMura_RunDriftEndScene(void)
{
    Engine_EventBegin();
    Engine_CameraSetSpeed(0x6666, 0xccc);
    Engine_CameraMoveTo(0x3f0000, -1, 0x1c20000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(30);
    Engine_ActorSetAnimation(18, 1);
    BattleFx_SetQueuedSoundAndPlay(-1);
    Engine_TaskRemoveCallback(SceneEffect_SpawnDriftingParticle);
    Engine_EventWait(20);
    Engine_ActorFaceActor(ACTOR_PARTY_LEADER, 18, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, 0x4000, 0);
    Engine_ActorFaceDirection(18, 0, 20);
    Engine_ActorFaceDirection(18, 0xd000, 40);
    Engine_AudioPlayCue(147);
    Engine_ActorRunRepeatedMotion(18, 2);
    Engine_EventWait(20);
    Engine_ActorFaceDirection(18, 0xb000, 40);
    FieldScene_RunScene3a3SequenceC();
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 1);
    Engine_CameraWaitForMove();
    Engine_ActorSetAnimationAndWait(14, 4);
    Engine_GameFlagSet(0x8ff);
    Engine_EventEnd();
}
