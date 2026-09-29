#include "VILLAGE.H"
extern u8 MsgRunpaGeraldRefusesToReturn[];
extern u8 MsgRunpaGuardsWarnPartyAway[];

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
