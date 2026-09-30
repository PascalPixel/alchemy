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
    Event_Begin();
    Actor_SetPosition(ACTOR_FLOATING_NUT, 0, 0);
    GameFlag_Set(FLAG_LUNPA_NUT_CAUGHT);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
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
        Actor_SetSpriteFlags(pillar, 0);
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
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        Map_CopyCells(41, 49, 3, 4, 1, 14);
        Map_CopyCells(44, 49, 3, 4, 33, 14);
        Map_CopyCells(47, 49, 3, 4, 1, 46);
    } else {
        Actor_SetPosition(ACTOR_SWITCH_GLINT, PIXELS(56), PIXELS(268));
        Actor_SetSpriteFlags(Actor_Get(ACTOR_SWITCH_GLINT), 0);
        actor = Actor_Get(ACTOR_SWITCH_GLINT);
        if (actor != NULL) {
            actor->motion_flags = GLINT_MOTION_FLAGS;
            actor->y.fixed = PIXELS(16);
            actor->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
            actor->scale_x = GLINT_SCALE_X;
            actor->scale_y = GLINT_SCALE_Y;
        }
    }
    Task_AddCallback(IcePillar_UpdateDrawOrder, TASK_PRIORITY_SCENE);
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
    Actor_SetAnimation(ACTOR_HIDDEN_PUDDLE, ANIM_STAND);
    puddle = Actor_Get(ACTOR_HIDDEN_PUDDLE);
    puddle->update = NULL;
    Object_SetPartPalettes(Actor_Get(ACTOR_HIDDEN_PUDDLE), 0);
    Task_RemoveCallback(IcePillar_UpdateDrawOrder);
    GameFlag_Set(FLAG_LUNPA_SECRETS_HIDDEN);
}

void Well_Search(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldPeeredWell, 1);
    Message_ShowCentered((s32)MsgRunpaWellFrogs, 1);
    Event_End();
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
    Event_Begin();
    Actor_ShowEmote(ACTOR_WEST_GUARD, EMOTE_IN_FRONT | 2, 60);
    Event_SetMessage((s32)MsgRunpaWestGuardAsksAboutEntering);
    Event_AskYesNo(ACTOR_WEST_GUARD, 0);
    Event_End();
}

/*
 * Once the guards suspect Kalay, the east guard questions the party; each
 * reply draws a different line from him.
 */
void EastGuard_Talk(void)
{
    Event_Begin();
    if (GameFlag_IsSet(FLAG_GUARDS_SUSPECT_KALAY) == 0) {
        Event_SetMessage((s32)MsgRunpaEastGuardDemandsAuthorization);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaEastGuardAsksIfFrom);
        Event_OpenMessage(ACTOR_EAST_GUARD, 0);
        if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 0) {
            gEventWork->message++;
            Event_OpenMessage(ACTOR_EAST_GUARD, 0);
            if (Event_ChooseYesNo(ACTOR_PARTY_LEADER, 0) == 1) {
                gEventWork->message++;
            }
        }
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
    Event_End();
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
        Event_SetMessage(thoughts);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    } else {
        Event_SetMessage((s32)MsgRunpaEastGuardThinksMerchantHarmless);
        Event_ShowMessage(ACTOR_EAST_GUARD, 0);
    }
}

void VillagerA_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerAAsksHowLong);
    Event_AskYesNo(ACTOR_VILLAGER_A, 0);
    Event_End();
}

void VillagerC_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerCAsksAboutKidnapping);
    Event_AskYesNo(ACTOR_VILLAGER_C, 0);
    Event_End();
}

void VillagerG_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerGAsksAboutDonpa);
    Event_AskYesNo(ACTOR_VILLAGER_G, 0);
    Event_End();
}

void VillagerB_Shivers(void)
{
    Event_Begin();
    Actor_RunRepeatedMotion(ACTOR_VILLAGER_B, 3);
    Event_SetMessage((s32)MsgRunpaVillagerBShivers);
    Event_ShowMessage(ACTOR_VILLAGER_B, 0);
    Event_End();
}

void VillagerD_Talk(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgRunpaVillagerDAsksAboutCommotion);
    Event_AskYesNo(ACTOR_VILLAGER_D, 0);
    Event_End();
}

void LeftGuard_Talk(void)
{
    s32 recognition;

    if (gGameState.cloaked != 0) {
        Event_SetMessage((s32)MsgRunpaLeftGuardHearsSomeone);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0
               && GameFlag_IsSet(FLAG_LUNPA_CAVE_REUNION_SEEN) == 0) {
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
        recognition = (s32)MsgRunpaLeftGuardRecognizesHammet;
        Event_SetMessage(recognition + RECOGNITION_SEEN_THAT_MAN);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage(recognition + RECOGNITION_IMPOSSIBLE);
        GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
    } else {
        Event_SetMessage((s32)MsgRunpaLeftGuardResentsDodonpa);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

void RightGuard_Talk(void)
{
    if (gGameState.cloaked != 0) {
        Event_SetMessage((s32)MsgRunpaRightGuardFeelsCreepy);
    } else if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage((s32)MsgRunpaRightGuardWondersHow);
    } else {
        Event_SetMessage((s32)MsgRunpaRightGuardBoasts);
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
            Psynergy_Cancel();
            Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
            Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 1, 60);
            Event_SetMessage((s32)MsgRunpaLeftGuardRecognizesHammet);
            GameFlag_Set(FLAG_GATE_GUARD_SAW_HAMMET);
        } else {
            Event_SetMessage((s32)MsgRunpaLeftGuardGlimpsedMerchant);
        }
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
        Event_SetMessage((s32)MsgRunpaLeftGuardDismissesThought);
    } else {
        Event_SetMessage((s32)MsgRunpaLeftGuardThoughts);
    }
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
}

void RightGuard_MindRead(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_TRADE_REOPENED) != 0) {
        Event_SetMessage((s32)MsgRunpaRightGuardReopenedThoughts);
    } else {
        Event_SetMessage((s32)MsgRunpaYoudShadowSneak);
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
    Event_Begin();
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_SetMessage((s32)MsgRunpaGeraldRefusesToReturn);
    Event_ShowMessage(ACTOR_GERALD, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2, 100);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, 0, 12);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    GameFlag_Clear(FLAG_GATE_TURNING_BACK);
    Event_End();
}

/* The door opens, the leader steps through, and the scene changes. */
void Door_Enter(void)
{
    struct EventWork *work;
    struct FieldActor *actor;
    u32 id;
    s32 index;

    work = gEventWork;
    Event_Begin();
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
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    if (index != DOOR_TEMPLE) {
        Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
        Event_Wait(10);
    }
    Event_RequestExit(work->touched_trigger);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

/* The opened passage can be walked into only while Reveal lasts. */
void HiddenPassage_Enter(void)
{
    if (GameFlag_IsSet(FLAG_LUNPA_PASSAGE_OPEN) != 0) {
        if (GameFlag_IsSet(FLAG_LUNPA_SECRETS_HIDDEN) == 0) {
            Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
            Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
            Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
            Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -8);
            Event_Wait(13);
            Event_RequestExit(LUNPA_EXIT_TO_JAIL);
        }
    }
}

void FortressGate_Enter(void)
{
    GameFlag_Set(FLAG_GATE_FORTRESS_ENTERED);
    Event_Begin();
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags = 0;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
    Actor_WalkBy(ACTOR_PARTY_LEADER, 0, -8);
    Audio_PlayCue(SOUND_DOOR_OPEN);
    Map_CopyCells(53, 4, 2, 2, 41, 4);
    Event_Wait(10);
    Map_CopyCells(53, 6, 2, 2, 41, 4);
    Event_Wait(10);
    Event_RequestExit(GATE_EXIT_TO_FORTRESS);
    Event_CloseScreen();
    Event_WaitForScreen();
    Event_End();
}

void Party_CheckAhead(void)
{
    Leader_CheckAhead();
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
    Task_Wait(1);
    Map_CopyCells(32, 45, 3, 4, 1, 14);
    Map_CopyCells(35, 45, 3, 4, 33, 14);
    Map_CopyCells(38, 45, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(41, 45, 3, 4, 1, 14);
    Map_CopyCells(44, 45, 3, 4, 33, 14);
    Map_CopyCells(47, 45, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(50, 45, 3, 4, 1, 14);
    Map_CopyCells(53, 45, 3, 4, 33, 14);
    Map_CopyCells(56, 45, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(32, 49, 3, 4, 1, 14);
    Map_CopyCells(35, 49, 3, 4, 33, 14);
    Map_CopyCells(38, 49, 3, 4, 1, 46);
    Task_Wait(10);
    Map_CopyCells(41, 49, 3, 4, 1, 14);
    Map_CopyCells(44, 49, 3, 4, 33, 14);
    Map_CopyCells(47, 49, 3, 4, 1, 46);
    Task_Wait(10);
    GameFlag_Set(FLAG_LUNPA_PASSAGE_OPEN);
}

/* Uncloaked, the party that nears the gate finds the guards in its way. */
void Guards_BlockGate(void)
{
    if (gGameState.cloaked == 0) {
        Event_Begin();
        Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 0, 2);
        Actor_ShowEmote(ACTOR_RIGHT_GUARD, EMOTE_IN_FRONT | 0, 15);
        Event_Wait(30);
        Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 168);
        Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 168);
        Actor_WaitForMove(ACTOR_LEFT_GUARD);
        Actor_WaitForMove(ACTOR_RIGHT_GUARD);
        Actor_Stop(ACTOR_LEFT_GUARD);
        Actor_SetAnimation(ACTOR_LEFT_GUARD, 0);
        Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
        Actor_Stop(ACTOR_RIGHT_GUARD);
        Actor_SetAnimation(ACTOR_RIGHT_GUARD, 0);
        Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
        Event_SetMessage((s32)MsgRunpaGuardsWarnPartyAway);
        Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
        GameFlag_Set(FLAG_GATE_GUARDS_BLOCKING);
        Map_CopyCellAttributes(6, 11, 1, 1, 7, 11);
        Map_CopyCellAttributes(6, 11, 1, 1, 8, 11);
        Map_CopyCellAttributes(6, 11, 1, 1, 9, 11);
        Event_End();
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
    Event_Begin();
    leader = Actor_Get(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_StartRepeatedMotion(ACTOR_LEFT_GUARD, 1);
    Actor_StartRepeatedMotion(ACTOR_RIGHT_GUARD, 1);
    Event_Wait(20);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 2, 60);
    warning = (s32)MsgRunpaGuardsCatchParty;
    Event_SetMessage(warning + CATCH_LEFT_GUARD_CHALLENGES);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x20000, 0x10000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x20000, 0x10000);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_SHAKE_HEAD);
    Event_Wait(35);
    Event_SetMessage(warning + CATCH_RIGHT_GUARD_WONDERS);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_ShowEmote(ACTOR_LEFT_GUARD, EMOTE_IN_FRONT | 3, 30);
    Event_SetMessage(warning + CATCH_LEFT_GUARD_REFUSES_ENTRY);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_NOD);
    Event_Wait(25);
    Event_SetMessage(warning + CATCH_RIGHT_GUARD_SENDS_PARTY_OFF);
    Event_ShowMessage(ACTOR_RIGHT_GUARD, 0);
    Actor_WalkTo(ACTOR_LEFT_GUARD, leader->x.part.pixel - 1, leader->z.part.pixel);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 216);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 200);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_FaceActor(ACTOR_LEFT_GUARD, ACTOR_PARTY_LEADER, 0);
    Actor_FaceActor(ACTOR_RIGHT_GUARD, ACTOR_PARTY_LEADER, 0);
    Event_Wait(12);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 160, 272);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 256);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 256);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_End();
    Task_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
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
        Task_RemoveCallback(Guards_Watch);
        work->raised_trigger = TRIGGER_PARTY_SPOTTED;
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
        Task_RemoveCallback(Guards_Watch);
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
        velocity[0] = Math_Cos(angle) / 4;
        velocity[1] = 0;
        velocity[2] = Math_Sin(angle) / 2;
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
    Event_Begin();
    Actor_SetPosition(ACTOR_PARTY_LEADER, PIXELS(160), PIXELS(128));
    Actor_SetPosition(ACTOR_LEFT_GUARD, PIXELS(152), PIXELS(112));
    Actor_SetPosition(ACTOR_RIGHT_GUARD, PIXELS(168), PIXELS(112));
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    /* The script also turns the village guards, whom the gate does not place. */
    Actor_FaceDirection(ACTOR_WEST_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_EAST_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Camera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Event_OpenScreen();
    Event_Wait(30);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x1cccc, 0xe666);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x1cccc, 0xe666);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 152, 288);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 168, 288);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Actor_SetDestination(ACTOR_PARTY_LEADER, 160, 296);
    Task_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Event_Wait(1);
    Audio_PlayCue(SOUND_SCUFFLE);
    Event_Wait(20);
    Actor_SetSpritePriority(ACTOR_LEFT_GUARD, 3);
    Actor_SetSpritePriority(ACTOR_RIGHT_GUARD, 3);
    Audio_PlayCue(SOUND_SCUFFLE);
    Event_Wait(30);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
    Audio_PlayCue(SOUND_SCUFFLE);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Task_RemoveCallback(Leader_KickUpDust);
    Actor_Get(ACTOR_PARTY_LEADER)->motion_flags |= ACTOR_FALLS;
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(6);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_z = PIXELS(6);
    Event_Wait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Event_Wait(1);
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_SPRAWLED);
    Audio_PlayCue(SOUND_LANDING_THUD);
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Task_AddCallback(Leader_KickUpDust, TASK_PRIORITY_SCENE);
    Event_Wait(2);
    Actor_Get(ACTOR_PARTY_LEADER)->velocity_y = PIXELS(3);
    Event_Wait(1);
    while (Actor_Get(ACTOR_PARTY_LEADER)->y.fixed != 0) {
        Event_Wait(1);
    }
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | 2);
    Event_Wait(10);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Task_RemoveCallback(Leader_KickUpDust);
    Event_Wait(50);
    Event_SetMessage((s32)MsgRunpaGuardForbidsReturn);
    Event_ShowMessage(ACTOR_LEFT_GUARD, 0);
    Actor_Get(ACTOR_LEFT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_Get(ACTOR_RIGHT_GUARD)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpeed(ACTOR_LEFT_GUARD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_RIGHT_GUARD, 0x10000, 0x8000);
    Actor_WalkTo(ACTOR_LEFT_GUARD, 144, 200);
    Actor_WalkTo(ACTOR_RIGHT_GUARD, 176, 200);
    Actor_WaitForMove(ACTOR_LEFT_GUARD);
    Actor_WaitForMove(ACTOR_RIGHT_GUARD);
    Actor_SetAnimation(ACTOR_LEFT_GUARD, ANIM_STAND);
    Actor_SetAnimation(ACTOR_RIGHT_GUARD, ANIM_STAND);
    Event_Wait(30);
    Actor_FaceDirection(ACTOR_LEFT_GUARD, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_RIGHT_GUARD, FACING_SOUTH + FACING_STEP, 0);
    Event_End();
}

/* The leader leaving the fortress casts Cloak and slips around the guards. */
void Leader_SneaksOut(void)
{
    Event_Begin();
    Event_OpenScreen();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 152, 168);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_Wait(20);
    Psynergy_Begin(ABILITY_CLOAK, 1);
    Psynergy_SetTarget(ACTOR_PARTY_LEADER, 0);
    Psynergy_RaiseHands();
    Psynergy_PlayEffect(1);
    Psynergy_LowerHands();
    Actor_WalkTo(ACTOR_PARTY_LEADER, 144, 184);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 184);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 200);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 200);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 72, 288);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 88, 288);
    Actor_WaitForMove(ACTOR_PARTY_LEADER);
    Event_End();
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
            Actor_SetSpriteFlags(puddle, 0);
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
            Task_AddCallback(Party_WatchForFortress, TASK_PRIORITY_SCENE);
        }
        Task_AddCallback(Guards_Watch, TASK_PRIORITY_SCENE);
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

    bob = Math_Sin(nut->angle) * 2;
    if (bob > 0) {
        bob = -bob;
    }
    nut->x = nut->rest_x + Math_Cos(nut->angle) * 2;
    nut->y = nut->rest_y + bob;
    sprite->rotation = Math_Cos(nut->angle + 0x8000) / 8;
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
    Actor_SetSpriteFlags((struct FieldActor *)nut, 0);
    nut->ready = 0;
    nut->motion_flags = 0;
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
        nut->y += PIXELS(32);
    }
    nut->priority_flags &= ~ACTOR_PRIORITY_AUTOMATIC;
    nut->free_motion = 1;
    icon = Heap_Allocate(HEAP_ITEM_ICON, ITEM_ICON_BUFFER_SIZE);
    Item_LoadIcon(ITEM_NUT);
    Vram_Load(sprite->vram_block, ITEM_ICON_TILE_BYTES, &icon[ITEM_ICON_TILES]);
    Heap_Release(HEAP_ITEM_ICON);
    nut->rest_x = nut->x;
    nut->angle = 0;
    nut->rest_y = nut->y;
    nut->ready = 1;
    nut->update = FloatingNut_Update;
    nut->status = 0;
}
