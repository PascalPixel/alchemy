#include "VILLAGE.H"

extern u8 MsgFieldPeeredWell[];
extern u8 MsgRunpaWellFrogs[];

extern u8 MsgRunpaEastGuardAsksIfFrom[];
extern u8 MsgRunpaEastGuardDemandsAuthorization[];
extern u8 MsgRunpaEastGuardFearsBlame[];
extern u8 MsgRunpaEastGuardThinksMerchantHarmless[];
extern u8 MsgRunpaEastGuardTrustsCaveGate[];
extern u8 MsgRunpaVillagerAAsksHowLong[];
extern u8 MsgRunpaVillagerBShivers[];
extern u8 MsgRunpaVillagerCAsksAboutKidnapping[];
extern u8 MsgRunpaVillagerDAsksAboutCommotion[];
extern u8 MsgRunpaVillagerGAsksAboutDonpa[];
extern u8 MsgRunpaWestGuardAsksAboutEntering[];
extern u8 MsgRunpaLeftGuardHearsSomeone[];
extern u8 MsgRunpaLeftGuardRecognizesHammet[];
extern u8 MsgRunpaLeftGuardResentsDodonpa[];

extern u8 MsgRunpaLeftGuardGlimpsedMerchant[];
extern u8 MsgRunpaLeftGuardDismissesThought[];
extern u8 MsgRunpaLeftGuardThoughts[];
extern u8 MsgRunpaRightGuardBoasts[];
extern u8 MsgRunpaRightGuardFeelsCreepy[];
extern u8 MsgRunpaRightGuardWondersHow[];
extern u8 MsgRunpaYoudShadowSneak[];
extern u8 MsgRunpaRightGuardReopenedThoughts[];

extern u8 MsgRunpaGeraldRefusesToReturn[];
extern u8 MsgRunpaGuardsWarnPartyAway[];
extern u8 MsgRunpaGuardsCatchParty[];

extern u8 MsgRunpaGuardForbidsReturn[];

/* Where the party appears in the village and at the fortress gate. */
const struct SceneEntrance *Scene_GetEntrances(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura1) {
        return gLunpaEntrances;
    }
    if (scene == (s32)&SceneId_RunpaMura2) {
        return gGateEntrances;
    }
    return gLunpaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return NULL;
}

/* Where the village's and the gate's exits lead. */
const u32 *Scene_GetExits(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura1) {
        return gLunpaExits;
    }
    if (scene == (s32)&SceneId_RunpaMura2) {
        return gGateExits;
    }
    return gLunpaExits;
}

/* The actors standing in the village or at the gate. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura1) {
        return gLunpaPlacements;
    }
    if (scene == (s32)&SceneId_RunpaMura2) {
        return gGatePlacements;
    }
    return gLunpaPlacements;
}

void FloatingNut_Catch(void)
{
    Engine_EventBegin();
    Actor_SetPosition(ACTOR_FLOATING_NUT, 0, 0);
    GameFlag_Set(FLAG_LUNPA_NUT_CAUGHT);
    Engine_ItemShowFound(ITEM_NUT, 3);
    Engine_PartyGiveItem(ITEM_NUT, 0);
    Engine_EventEnd();
}

/* The frozen puddle stands as a pillar of ice the party can climb. */
void HiddenPuddle_Freeze(void)
{
    struct FieldActor *pillar;

    pillar = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    Actor_Get(ACTOR_PARTY_LEADER);
    Map_CopyCellAttributes(17, 4, 1, 1, 14, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 15, 4);
    Map_CopyCellAttributes(15, 3, 1, 1, 13, 4);
    if (pillar != NULL) {
        Engine_ActorSetSpriteFlags(pillar, 0);
        pillar->motion_flags = ACTOR_FALLS;
        pillar->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
    GameFlag_Set(FLAG_LUNPA_PUDDLE_FROZEN);
}

/* A leader standing on the pillar draws above it. */
void IcePillar_UpdateDrawOrder(void)
{
    if (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed >= PILLAR_TOP_HEIGHT) {
        Actor_Get(ACTOR_HIDDEN_PUDDLE)->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    } else {
        Actor_Get(ACTOR_HIDDEN_PUDDLE)->priority_flags = ACTOR_PRIORITY_AUTOMATIC;
    }
}

void Reveal_ShowSecrets(void)
{
    struct FieldActor *actor;
    s32 cell_x;
    s32 cell_z;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    cell_x = actor->x.fixed / CELL_SIZE;
    cell_z = actor->z.fixed / CELL_SIZE;
    if (GameFlag_IsSet(FLAG_LUNPA_PSYNERGY_STONE) == 0) {
        if (cell_x == 7 && cell_z == 16) {
            Actor_WalkByAndWait(ACTOR_PARTY_LEADER, 0, 16);
        }
        MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, -1, -1);
        Map_CopyCellAttributes(28, 31, 1, 1, 7, 16);
    }
    Map_CopyCells(47, 4, 1, 1, 46, 4);
    Map_CopyCellAttributes(34, 37, 3, 3, 13, 3);
    Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, PIXELS(232), PIXELS(72));
    Actor_Get(ACTOR_HIDDEN_PUDDLE)->y.fixed = 0;
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
    if (GameFlag_IsSet(FLAG_LUNPA_PUDDLE_FROZEN)) {
        Map_CopyCellAttributes(17, 4, 1, 1, 14, 4);
        Map_CopyCellAttributes(15, 3, 1, 1, 15, 4);
        Map_CopyCellAttributes(15, 3, 1, 1, 13, 4);
    }
#endif
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        Map_CopyCells(41, 49, 3, 4, 1, 14);
        Map_CopyCells(44, 49, 3, 4, 33, 14);
        Map_CopyCells(47, 49, 3, 4, 1, 46);
    } else {
        Actor_SetPosition(ACTOR_SWITCH_GLINT, PIXELS(56), PIXELS(268));
        Engine_ActorSetSpriteFlags(Actor_Get(ACTOR_SWITCH_GLINT), 0);
        actor = Actor_Get(ACTOR_SWITCH_GLINT);
        if (actor != NULL) {
            actor->motion_flags = GLINT_MOTION_FLAGS;
            actor->y.fixed = PIXELS(16);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            actor->scale_x = GLINT_SCALE_X;
            actor->scale_y = GLINT_SCALE_Y;
        }
    }
    Engine_TaskAddCallback(IcePillar_UpdateDrawOrder, TASK_PRIORITY_SCENE);
    GameFlag_Clear(FLAG_LUNPA_SECRETS_HIDDEN);
}

void Reveal_HideSecrets(void)
{
    struct FieldActor *puddle;

    Actor_Get(ACTOR_PARTY_LEADER);
    Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
    Actor_SetPosition(ACTOR_HIDDEN_PUDDLE, 0, 0);
    Map_CopyCells(38, 38, 1, 1, 46, 4);
    Map_CopyCellAttributes(37, 37, 3, 3, 13, 3);
    Map_CopyCellAttributes(37, 37, 1, 1, 14, 2);
    Map_CopyCellAttributes(8, 16, 1, 1, 7, 16);
    MapObject_SetPosition(MAP_OBJECT_PSYNERGY_STONE, 0, 0);
    Map_CopyCells(32, 42, 3, 2, 1, 15);
    GameFlag_Clear(FLAG_LUNPA_PUDDLE_FROZEN);
    Engine_ActorSetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
    puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    puddle->update = NULL;
    Engine_ObjectSetPartPalettes(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
    Engine_TaskRemoveCallback(IcePillar_UpdateDrawOrder);
    GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
}

void Well_Search(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
    Engine_MessageShowCentered((s32)MsgRunpaWellFrogs, 1);
    Engine_EventEnd();
}

/* What the village answers before and after Lunpa trades again, and what
 * the gate answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    s32 scene = gGameState.scene;

    if (scene == (s32)&SceneId_RunpaMura2) {
        /* The gate asks whether Lunpa trades again but answers alike either way. */
        GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED);
        return gGateEvents;
    }
    if (scene == (s32)&SceneId_RunpaMura1 && GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        return gLunpaReopenedEvents;
    }
    return gLunpaSealedEvents;
}

void WestGuard_Talk(void)
{
    Engine_EventBegin();
    Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 60);
    Engine_EventSetMessage((s32)MsgRunpaWestGuardAsksAboutEntering);
    Event_AskYesNo(ACTOR_WEST_GUARD, 0);
    Engine_EventEnd();
}

/*
 * Once the guards suspect Kalay, the east guard questions the party; each
 * reply draws a different line from him.
 */
void EastGuard_Talk(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
        Engine_EventSetMessage((s32)MsgRunpaEastGuardDemandsAuthorization);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaEastGuardAsksIfFrom);
        Event_OpenMessage(ACTOR_EAST_GUARD, 0);
        if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            gEventWork->message++;
            Event_OpenMessage(ACTOR_EAST_GUARD, 0);
            if (Engine_EventChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
                gEventWork->message++;
            }
        }
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
    Engine_EventEnd();
}

void EastGuard_MindRead(void)
{
    s32 thoughts;

    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) == 0) {
        thoughts = GameFlag_IsSet(FLAG_GUARDS_SUSPECT_KALAY);
        if (thoughts == 0) {
            thoughts = (s32)MsgRunpaEastGuardFearsBlame;
        } else {
            thoughts = (s32)MsgRunpaEastGuardTrustsCaveGate;
        }
        Engine_EventSetMessage(thoughts);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaEastGuardThinksMerchantHarmless);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
}

void VillagerA_Talk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRunpaVillagerAAsksHowLong);
    Event_AskYesNo(ACTOR_VILLAGER_A, 0);
    Engine_EventEnd();
}

void VillagerC_Talk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRunpaVillagerCAsksAboutKidnapping);
    Event_AskYesNo(ACTOR_VILLAGER_C, 0);
    Engine_EventEnd();
}

void VillagerG_Talk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRunpaVillagerGAsksAboutDonpa);
    Event_AskYesNo(ACTOR_VILLAGER_G, 0);
    Engine_EventEnd();
}

void VillagerB_Shivers(void)
{
    Engine_EventBegin();
    Engine_ActorRunRepeatedMotion(ACTOR_VILLAGER_B, 3);
    Engine_EventSetMessage((s32)MsgRunpaVillagerBShivers);
    Event_ShowMessage(ACTOR_VILLAGER_B, 0);
    Engine_EventEnd();
}

void VillagerD_Talk(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgRunpaVillagerDAsksAboutCommotion);
    Event_AskYesNo(ACTOR_VILLAGER_D, 0);
    Engine_EventEnd();
}

void LeftGuard_Talk(void)
{
    s32 recognition;

    if (gGameState.cloaked != 0) {
        Engine_EventSetMessage((s32)MsgRunpaLeftGuardHearsSomeone);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
               && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
        recognition = (s32)MsgRunpaLeftGuardRecognizesHammet;
        Engine_EventSetMessage(recognition + RECOGNITION_SEEN_THAT_MAN);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Engine_EventSetMessage(recognition + RECOGNITION_IMPOSSIBLE);
        GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaLeftGuardResentsDodonpa);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

void RightGuard_Talk(void)
{
    if (gGameState.cloaked != 0) {
        Engine_EventSetMessage((s32)MsgRunpaRightGuardFeelsCreepy);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventSetMessage((s32)MsgRunpaRightGuardWondersHow);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaRightGuardBoasts);
    }
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/* Reading the left guard's mind while Hammet is near makes him look up. */
void LeftGuard_MindRead(void)
{
    s32 seen;

    if (gGameState.cloaked == 0 && GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
        && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        seen = GameFlag_IsSet(FLAG_GATE_GUARD_SAW_HAMMET);
        if (seen == 0) {
            gEventWork->psynergy_request = seen;
            Engine_PsynergyCancel();
            Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
            Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
            Engine_EventSetMessage((s32)MsgRunpaLeftGuardRecognizesHammet);
            GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
        } else {
            Engine_EventSetMessage((s32)MsgRunpaLeftGuardGlimpsedMerchant);
        }
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Engine_EventSetMessage((s32)MsgRunpaLeftGuardDismissesThought);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaLeftGuardThoughts);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

void RightGuard_MindRead(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Engine_EventSetMessage((s32)MsgRunpaRightGuardReopenedThoughts);
    } else {
        Engine_EventSetMessage((s32)MsgRunpaYoudShadowSneak);
    }
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
}

/* After the escape, walking up to the fortress raises Gerald's objection. */
void Party_WatchForFortress(void)
{
    s32 cell_z = Actor_Get(ACTOR_PARTY_LEADER)->z.fixed / CELL_SIZE;

    if (GameFlag_IsSet(FLAG_GATE_TURNING_BACK) == 0 && cell_z == 10) {
        GameFlag_Set(FLAG_GATE_TURNING_BACK);
        gEventWork->touched_trigger = TRIGGER_FORTRESS_APPROACH;
    }
}

void Gerald_RefusesToReturn(void)
{
    Engine_EventBegin();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_EventSetMessage((s32)MsgRunpaGeraldRefusesToReturn);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 100);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    GameFlag_Clear(FLAG_GATE_TURNING_BACK);
    Engine_EventEnd();
}

/* The door opens, the leader steps through, and the scene changes. */
void Door_Enter(void)
{
    struct EventWork *work;
    struct FieldActor *actor;
    u32 id;
    s32 index;

    work = gEventWork;
    Engine_EventBegin();
    for (id = ACTOR_FIRST_PLACED; id <= ACTOR_LAST_PLACED; id++) {
        actor = Actor_Get(id);
        if (actor != NULL) {
            actor->motion_flags = 0;
        }
    }
    Audio_PlayCue(SOUND_DOOR_OPEN);
    index = work->touched_trigger - TRIGGER_FIRST_HOME_DOOR;
    Map_AnimateCells(gLunpaDoors[index].steps, gLunpaDoors[index].x, gLunpaDoors[index].y);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    if (index != DOOR_TEMPLE) {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
        Engine_EventWait(10);
    }
    Engine_EventRequestExit(work->touched_trigger);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

/* The opened passage can be walked into only while Reveal lasts. */
void HiddenPassage_Enter(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
            Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
            Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
            Engine_EventWait(13);
            Engine_EventRequestExit(LUNPA_EXIT_TO_JAIL);
        }
    }
}

void FortressGate_Enter(void)
{
    GameFlag_Set(FLAG_GATE_FORTRESS_ENTERED);
    Engine_EventBegin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -8);
    Audio_PlayCue(SOUND_DOOR_OPEN);
    Map_CopyCells(53, 4, 2, 2, 41, 4);
    Engine_EventWait(10);
    Map_CopyCells(53, 6, 2, 2, 41, 4);
    Engine_EventWait(10);
    Engine_EventRequestExit(GATE_EXIT_TO_FORTRESS);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventEnd();
}

void Party_CheckAhead(void)
{
    Engine_LeaderCheckAhead();
}

void Reveal_PlayTreasureCue(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
        Audio_PlayCue(SOUND_TREASURE_FOUND);
    }
}

/* The switch grinds the hidden passage open, a block of cells at a time. */
void HiddenSwitch_Pull(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) != 0) {
        return;
    }
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        return;
    }
    Actor_SetPosition(ACTOR_SWITCH_GLINT, 0, 0);
    Audio_PlayCue(SOUND_HIDDEN_PASSAGE_OPEN);
    Engine_TaskWait(1);
    Map_CopyCells(32, 45, 3, 4, 1, 14);
    Map_CopyCells(35, 45, 3, 4, 33, 14);
    Map_CopyCells(38, 45, 3, 4, 1, 46);
    Engine_TaskWait(10);
    Map_CopyCells(41, 45, 3, 4, 1, 14);
    Map_CopyCells(44, 45, 3, 4, 33, 14);
    Map_CopyCells(47, 45, 3, 4, 1, 46);
    Engine_TaskWait(10);
    Map_CopyCells(50, 45, 3, 4, 1, 14);
    Map_CopyCells(53, 45, 3, 4, 33, 14);
    Map_CopyCells(56, 45, 3, 4, 1, 46);
    Engine_TaskWait(10);
    Map_CopyCells(32, 49, 3, 4, 1, 14);
    Map_CopyCells(35, 49, 3, 4, 33, 14);
    Map_CopyCells(38, 49, 3, 4, 1, 46);
    Engine_TaskWait(10);
    Map_CopyCells(41, 49, 3, 4, 1, 14);
    Map_CopyCells(44, 49, 3, 4, 33, 14);
    Map_CopyCells(47, 49, 3, 4, 1, 46);
    Engine_TaskWait(10);
    GameFlag_Set(FLAG_LUNPA_PASSAGE_OPEN);
}

/* Uncloaked, the party that nears the gate finds the guards in its way. */
void Guards_BlockGate(void)
{
    if (gGameState.cloaked == 0) {
        Engine_EventBegin();
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 0, 2);
        Actor_ShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | 0, 15);
        Engine_EventWait(30);
        Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 168);
        Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 168);
        Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
        Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
        Engine_ActorStop(ACTOR_LEFT_GUARD);
        Engine_ActorSetAnimation(ACTOR_LEFT_GUARD, 0);
        Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
        Engine_ActorStop(ACTOR_RIGHT_GUARD);
        Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, 0);
        Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
        Engine_EventSetMessage((s32)MsgRunpaGuardsWarnPartyAway);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        GameFlag_Set(FLAG_GATE_GUARDS_BLOCKING);
        Map_CopyCellAttributes(6, 11, 1, 1, 7, 11);
        Map_CopyCellAttributes(6, 11, 1, 1, 8, 11);
        Map_CopyCellAttributes(6, 11, 1, 1, 9, 11);
        Engine_EventEnd();
    }
}

void Scene_DoNothing(void)
{
}

/* The cloaked party finds the gateway closed in front of it. */
void Cloak_Begin(void)
{
    Map_CopyCellAttributes(6, 11, 1, 1, 7, 11);
    Map_CopyCellAttributes(6, 11, 1, 1, 8, 11);
    Map_CopyCellAttributes(6, 11, 1, 1, 9, 11);
    GameFlag_Set(FLAG_GATE_CLOAK_CAST);
}

void Guards_CatchParty(void)
{
    struct FieldActor *leader;
    s32 warning;

    if (GameFlag_IsSet(FLAG_GATE_PARTY_CAUGHT) != 0) {
        return;
    }
    GameFlag_Set(FLAG_GATE_PARTY_CAUGHT);
    Engine_EventBegin();
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Engine_ActorStartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Engine_ActorStartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 2, 60);
    warning = (s32)MsgRunpaGuardsCatchParty;
    Engine_EventSetMessage(warning + CATCH_LEFT_GUARD_CHALLENGES);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x20000, 0x10000);
    Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
    Engine_EventWait(35);
    Engine_EventSetMessage(warning + CATCH_RIGHT_GUARD_WONDERS);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 3, 30);
    Engine_EventSetMessage(warning + CATCH_LEFT_GUARD_REFUSES_ENTRY);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
    Engine_EventWait(25);
    Engine_EventSetMessage(warning + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part.pixel - 1, leader->z.part.pixel);
    Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
    Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
    Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(12);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
    Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
    Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_EventEnd();
    Engine_TaskAddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
}

void Gateway_Reopen(void)
{
    Map_CopyCellAttributes(7, 12, 1, 1, 7, 11);
    Map_CopyCellAttributes(7, 12, 1, 1, 8, 11);
    Map_CopyCellAttributes(7, 12, 1, 1, 9, 11);
}

/* A party still near the gate when Cloak fades is spotted. */
void Cloak_End(void)
{
    struct EventWork *work;
    struct FieldActor *leader;

    work = gEventWork;
    GameFlag_Clear(FLAG_GATE_CLOAK_CAST);
    GameFlag_Clear(FLAG_GATE_GUARDS_BLOCKING);
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (leader->x.fixed > PIXELS(104) && leader->x.fixed < PIXELS(240)
        && leader->z.fixed > PIXELS(160) && leader->z.fixed < PIXELS(248)) {
        Engine_TaskRemoveCallback(Guards_Watch);
#if defined(TBS_EDITION_JA)
        Guards_CatchParty();
#else
        work->raised_trigger = TRIGGER_PARTY_SPOTTED;
#endif
    }
    Gateway_Reopen();
    GameFlag_Clear(FLAG_GATE_PARTY_CAUGHT);
}

/* An uncloaked party that walks up to the gate is spotted. */
void Guards_Watch(void)
{
    struct EventWork *work;
    struct FieldActor *leader;

    work = gEventWork;
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if (gGameState.cloaked == 0 && (u32)(leader->x.fixed - PIXELS(144)) <= PIXELS(32)
        && leader->z.fixed >= PIXELS(168) && leader->z.fixed < PIXELS(176)) {
        Engine_TaskRemoveCallback(Guards_Watch);
        work->raised_trigger = TRIGGER_PARTY_SPOTTED;
    }
}

/* Every sixteenth frame the dragged leader kicks up a puff of dust. */
void Leader_KickUpDust(void)
{
    struct FieldActor *leader;
    s32 angle;
    s32 velocity[3];

    leader = Actor_Get(ACTOR_PARTY_LEADER);
    if ((gFrameCount & 15) == 0) {
        angle = (((u32)Random_Next() * 52) >> 16) * 64 + 230;
        velocity[0] = Engine_MathCos(angle) / 4;
        velocity[1] = 0;
        velocity[2] = Engine_MathSin(angle) / 2;
        Effect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed, velocity[0], velocity[1],
                     velocity[2], 0, NULL);
    }
}

/*
 * The guards drag the party caught inside the fortress down to the gate
 * road and throw it; the leader bounces twice and lies sprawled.
 */
void Party_ThrownOut(void)
{
    Engine_EventBegin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(160), PIXELS(128));
    Actor_SetPosition(ACTOR_LEFT_GUARD, PIXELS(152), PIXELS(112));
    Actor_SetPosition(ACTOR_RIGHT_GUARD, PIXELS(168), PIXELS(112));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    /* The script also turns the village guards, whom the gate does not place. */
    Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Engine_CameraFollowActor(ACTOR_PARTY_LEADER, 0);
    Engine_EventOpenScreen();
    Engine_EventWait(30);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x1cccc, 0xe666);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 288);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 288);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 160, 296);
    Engine_TaskAddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Engine_EventWait(1);
    Audio_PlayCue(SOUND_SCUFFLE);
    Engine_EventWait(20);
    Engine_ActorSetSpritePriority(ACTOR_LEFT_GUARD, 3);
    Engine_ActorSetSpritePriority(ACTOR_RIGHT_GUARD, 3);
    Audio_PlayCue(SOUND_SCUFFLE);
    Engine_EventWait(30);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Audio_PlayCue(SOUND_SCUFFLE);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Engine_TaskRemoveCallback(Leader_KickUpDust);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags |= ACTOR_FALLS;
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(6);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_z = PIXELS(6);
    Engine_EventWait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Engine_EventWait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
    Audio_PlayCue(SOUND_LANDING_THUD);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Engine_TaskAddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Engine_EventWait(2);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(3);
    Engine_EventWait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Engine_EventWait(1);
    }
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Engine_EventWait(10);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Engine_TaskRemoveCallback(Leader_KickUpDust);
    Engine_EventWait(50);
    Engine_EventSetMessage((s32)MsgRunpaGuardForbidsReturn);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 144, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 176, 200);
    Engine_ActorWaitForMove(ACTOR_LEFT_GUARD);
    Engine_ActorWaitForMove(ACTOR_RIGHT_GUARD);
    Engine_ActorSetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Engine_ActorSetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Engine_EventWait(30);
    Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Engine_EventEnd();
}

/* The leader leaving the fortress casts Cloak and slips around the guards. */
void Leader_SneaksOut(void)
{
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_EventWait(20);
    Engine_PsynergyBegin(ABILITY_CLOAK, 1);
    Engine_PsynergySetTarget(ACTOR_PARTY_LEADER, 0);
    Engine_PsynergyRaiseHands();
    Engine_PsynergyPlayEffect(1);
    Engine_PsynergyLowerHands();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 144, 184);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 184);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 200);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 200);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 288);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 288);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_EventEnd();
}

/* The village hides its secrets and floats its Nut; the gate sets where a
 * retreat returns to, brings out a party thrown out or sneaking out, starts
 * the guards watching and forgets what the party did in the fortress. */
s32 Scene_Initialize(void)
{
    struct FieldActor *puddle;

    if (gGameState.scene == (s32)&SceneId_RunpaMura1) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        Reveal_HideSecrets();
        if (GameFlag_IsSet(FLAG_LUNPA_NUT_CAUGHT) == 0) {
            FloatingNut_Initialize(ACTOR_FLOATING_NUT);
        }
        puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
        if (puddle != NULL) {
            Engine_ActorSetSpriteFlags(puddle, 0);
        }
        GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
    }
    if (gGameState.scene == (s32)&SceneId_RunpaMura2) {
        gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
        gGameState.retreat_entrance = GATE_RETREAT_ENTRANCE;
        if (gGameState.entrance == GATE_ENTRANCE_THROWN_OUT
            && GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
            Party_ThrownOut();
        }
        if (gGameState.entrance == GATE_ENTRANCE_SNEAKING_OUT
            && GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
            Leader_SneaksOut();
        }
        if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
            && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
            Engine_TaskAddCallback(Party_WatchForFortress, TASK_PRIORITY_SCENE);
        }
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_IT)
        if (gGameState.cloaked != 0)
            Cloak_Begin();
#endif
        Engine_TaskAddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 1);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 2);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 3);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 4);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 5);
        GameFlag_Clear(FLAG_FORTRESS_VISIT);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 6);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 7);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 8);
        GameFlag_Clear(FLAG_FORTRESS_VISIT + 9);
    }
    return 0;
}

/* The Nut circles its resting point, bobbing and swaying. */
s32 FloatingNut_Update(union FieldObject *object)
{
    struct FloatingNut *nut = (struct FloatingNut *)object;
    struct FieldSprite *sprite = nut->sprite;
    s32 bob;
    s32 first;
    s32 second;

    bob = Engine_MathSin(nut->angle) * 2;
    if (bob > 0) {
        bob = -bob;
    }
    nut->x = nut->rest_x + Engine_MathCos(nut->angle) * 2;
    nut->y = nut->rest_y + bob;
    sprite->rotation = Engine_MathCos(nut->angle + 0x8000) / 8;
    first = Random_Next();
    second = Random_Next();
    nut->angle = nut->angle + (((u32)first << 9 >> 16) + ((u32)second << 9 >> 16)) + 0x400;
    return 0;
}

/* The Nut shows its item icon and hovers out of the party's reach. */
void FloatingNut_Initialize(s32 actor)
{
    struct FloatingNut *nut;
    struct FieldSprite *sprite;
    u8 *icon;

    nut = (struct FloatingNut *)Actor_Get(actor);
    sprite = nut->sprite;
    sprite->priority = 1;
    sprite->full_color = 0;
    sprite->palette = 0;
    sprite->part_count = 0;
    Engine_ActorSetSpriteFlags((struct FieldActor *)nut, 0);
    nut->ready = 0;
    nut->motion_flags = 0;
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        nut->y += PIXELS(32);
    }
    nut->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    nut->free_motion = 1;
    icon = Engine_HeapAllocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
    Engine_ItemLoadIcon(ITEM_NUT);
    Engine_VramLoad(sprite->vram_block, ITEM_ICON_TILE_BYTES, &icon[ITEM_ICON_TILES]);
    Engine_HeapRelease(HEAP_ITEM_ICON);
    nut->rest_x = nut->x;
    nut->angle = 0;
    nut->rest_y = nut->y;
    nut->ready = 1;
    nut->update = FloatingNut_Update;
    nut->status = 0;
}
