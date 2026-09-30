/* The Lunpa fortress: the patrolling guards and the village path triggers. */
#include "FORTRESS.H"

void ConfigureSceneActor9(void)
{
    Event_Begin();
    Actor_Stop(9);
    Actor_SetDestinationOffset(9, 0, 0);
    Actor_SetAnimation(9, 0);
    Actor_FaceActor(9, ACTOR_PARTY_LEADER, 0);
    Actor_ShowEmote(9, 256, 0);
    RunActorScriptedSequenceA(10);
    Event_End();
}

s32 AreSceneActorsInPassingLane(void)
{
    SceneActor *player = Object_GetById(0);
    SceneActor *passing_actor = Object_GetById(17);
    s32 ox = player->x;
    s32 pz;
    s32 px;
    s32 oxx, pzz, pxx;

    if (ox < 0) {
        ox += 0xfffff;
    }
    oxx = ox >> 20;
    pz = passing_actor->z;
    if (pz < 0) {
        pz += 0xfffff;
    }
    px = passing_actor->x;
    pzz = pz >> 20;
    if (px < 0) {
        px += 0xfffff;
    }
    pxx = px >> 20;
    if (oxx == 52 && pxx == 57 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    if (oxx == 57 && pxx == 52 && pzz > 34 && pzz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorSeventeenInteraction(void)
{

    struct ObjectRuntime *actor = Object_GetById(17);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    Engine_EventGetViewCenter(actor);
    if (gFrameCount & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 0x3400000 - actor->x;
            work[9] = 0x2400000 - actor->z;
        }
        if (!AreSceneActorsInPassingLane()) {
            IsSceneActorWithinTriggerBox(17);
            if (IsSceneActorWithinFourSteps(17) && gGameState.cloaked != 0) {
                SetSceneValue(&scene[191], 0x2092);
                return;
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(17)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 92);
            }
        }
    }
}

void ActivateSceneActor17(void)
{
    RunActorScriptedSequenceA(17);
    Event_End();
}

s32 IsPlayerInSecondaryTriggerArea(void)
{
    SceneActor *player = Object_GetById(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if ((u32)(xx - 41) <= 3 && zz > 25 && zz <= 28) {
        return 1;
    }
    if (xx == 41 && zz > 37 && zz <= 41) {
        return 1;
    }
    if ((u32)(xx - 54) <= 2 && zz > 30 && zz <= 40) {
        return 1;
    }
    return 0;
}

void FieldScene_UpdateActorEighteenInteraction(void)
{

    struct ObjectRuntime *actor = Object_GetById(18);
    s32 *work = (s32 *)(*(u8 **)gCam + 0x164);
    s16 *scene = *(s16 **)(gCam + 0x4c);

    if (gFrameCount & 1) {
        work[6] = 1;
        work[7] = 1;
    } else {
        work[6] = -1;
        work[7] = -1;
    }
    if (GameFlag_IsSet(0x106) || scene[191] != 0 || scene[192] != 0) {
        actor->movement_state = 1;
    } else if (!GameFlag_IsSet(0x214)) {
        actor->movement_state = 0;
        if (!GameFlag_IsSet(0x214) && actor->movement_state == 0) {
            work[8] = 0x2f00000 - actor->x;
            work[9] = 0x1f00000 - actor->z;
        }
        if (!IsPlayerInSecondaryTriggerArea()) {
            if (IsSceneActorWithinFourSteps(18) && gGameState.cloaked != 0) {
                SetSceneValue(&scene[191], 0x2092);
                return;
            }
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(18)) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214)) {
                SetSceneValue(&scene[193], 93);
            }
        }
    }
}

void ActivateSceneActor18(void)
{
    RunActorScriptedSequenceA(18);
    Event_End();
}

s32 IsPlayerOutsideSceneRectangle(void)
{
    SceneActor *player = Object_GetById(0);
    s32 zz = player->z / 0x100000;
    s32 xx = player->x / 0x100000;

    if (xx > 45 && zz > 14 && xx <= 64 && zz <= 16) {
        return 0;
    }
    return 1;
}

void FieldScene_RunScene3bfSequenceA(void)
{

    struct EventWork *work;

    work = gEventWork;
    if (GameFlag_IsSet(0x214) == 0) {
        if (IsPlayerOutsideSceneRectangle() == 0) {
            if (gGameState.cloaked == 0) {
                if (IsActorInteractionAvailable(17) != 0) {
                    GameFlag_Set(0x215);
                    GameFlag_Set(0x214);
                }
            }
            if (GameFlag_IsSet(0x214) != 0) {
                work->raised_trigger = 94;
            }
        }
    }
}

void RunActor17SceneStep(void)
{
    RunActorScriptedSequenceA(17);
    Event_End();
}

void TriggerSceneStage95FromActor12(void)
{

    u8 *scene_state = ((u8*)gEventWork);

    if (IsActorInteractionAvailable(12) != 0 && gGameState.cloaked == 0) {
        s16 *scene_stage;
        s32 next_stage;

        Engine_TaskRemoveCallback(TriggerSceneStage95FromActor12);
        scene_stage = (s16 *)(scene_state + 386);
        next_stage = 95;
        *scene_stage = next_stage;
    }
}

void FieldScene_RunScene3bfSequenceB(void)
{

    struct EventWork *work;

    work = gEventWork;
    if (GameFlag_IsSet(0x225) == 0) {
        if (IsActorInteractionAvailable(13) != 0) {
            if (gGameState.cloaked == 0) {
                GameFlag_Set(0x225);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
                work->raised_trigger = 96;
            }
        }
    }
}

void FieldScene_RunScene3bfSequenceC(void)
{

    struct EventWork *work;

    work = gEventWork;
    if (GameFlag_IsSet(0x225) == 0) {
        if (IsActorInteractionAvailable(21) != 0) {
            if (gGameState.cloaked == 0) {
                GameFlag_Set(0x225);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceC);
                Engine_TaskRemoveCallback((s32)FieldScene_RunScene3bfSequenceB);
                work->raised_trigger = 96;
            }
        }
    }
}

s32 IsSceneActorVerticallyNearPlayer(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Object_GetById(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 z_distance = actor_z - player_z;

    if (z_distance >= -6 && z_distance <= 6 && actor_x - 1 < player_x && actor_x + 1 > player_x) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorHorizontallyNearPlayer(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Object_GetById(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;

    if (x_distance < -6 || x_distance > 6) {
        return 0;
    }
    if (actor_z - 2 < player_z && actor_z + 2 > player_z) {
        return 1;
    }
    return 0;
}

s32 IsActorInteractionAvailable(s32 actor_id)
{
    if (IsSceneActorWithinTriggerBox(actor_id) == 0) {
        return 0;
    }
    if (IsSceneActorVerticallyNearPlayer(actor_id)!= 0) {
        return 1;
    }
    {
        s32 result = IsSceneActorHorizontallyNearPlayer(actor_id);

        /* branchless "result != 0" */
        return (u32)(result | -result) >> 31;
    }
}

s32 IsSceneActorWithinFourSteps(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Object_GetById(0);
    s32 actor_z = scene_actor->z / 0x100000;
    s32 actor_x = scene_actor->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    actor_z += 1;
    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance + z_distance <= 4) {
        return 1;
    }
    return 0;
}

s32 IsSceneActorWithinTriggerBox(s32 actor_id)
{
    SceneActor *scene_actor = Object_GetById(actor_id);
    SceneActor *player = Engine_EventGetViewCenter();
    s32 actor_x = scene_actor->x / 0x100000;
    s32 actor_z = scene_actor->z / 0x100000;
    s32 player_x = player->x / 0x100000;
    s32 player_z = player->z / 0x100000;
    s32 x_distance = actor_x - player_x;
    s32 z_distance;

    if (x_distance < 0) {
        x_distance = -x_distance;
    }
    z_distance = actor_z - player_z;
    if (z_distance < 0) {
        z_distance = -z_distance;
    }
    if (x_distance > 7 || z_distance > 5) {
        return 0;
    }
    return 1;
}

void TriggerScene41AtVillagePath(void)
{

    SceneActor *player = Object_GetById(0);

    if (GameFlag_IsSet(859) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 43 && player_z > 28 && player_z <= 31) {
            s16 *q = (s16 *)(((u8*)gEventWork) + 364);
            s32 v = 41;

            *q = v;
            FieldScene_UpdateObjectPairC();
        }
    }
}

void TriggerScene40AtVillagePath(void)
{

    DirectionalSceneActor *player = Object_GetById(0);

    if (GameFlag_IsSet(856) == 0) {
        s32 player_x = player->x / 0x100000;
        s32 player_z = player->z / 0x100000;

        if (player_x == 16 && player_z > 55 && player_z <= 58
            && (player->dir == 0xc000 || player->dir == 0x4000)) {
            s16 *q = (s16 *)(((u8*)gEventWork) + 364);
            s32 v = 40;

            *q = v;
            FieldScene_UpdateTableBObjectPair();
        }
    }
}
