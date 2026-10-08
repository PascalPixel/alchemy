#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "niwa.h"
#include "types.h"
#include "call.h"
#include "field_event.h"
#include "text/msg_ids.h"





extern struct EventWork *gEventWork;

void Engine_ObjectSetTargetAndCallback();
void FieldScene_OpenGate();



void InitializeOrbitingSceneEntity();
void BiribinoNiwa_RunGardenScene();


s32 SceneActor_UpdateFacingTowardTarget(struct FacingObject *object)
{
    s32 delta;
    u16 old;
    s32 tgt;
    struct FacingObject *target;

    target = object->facing_target;
    if (target != NULL) {
        object->facing_flags = (u8)(0xFE & object->facing_flags);
        tgt = (u16)ArcTan2(target->position_z - object->position_z, target->position_x - object->position_x);
        old = object->facing;
        delta = (s16)(tgt - old);
        if (delta != 0) {
            if (delta > 0x1000) {
                delta = 0x1000;
            }
            if (delta < -0x1000) {
                delta = -0x1000;
            }
            object->facing = (u16)(old + delta);
        }
    }
    return 1;
}

u8 *SceneData_GetScriptTable(void)
{
    return Placement_Scripts;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

u8 *SceneData_GetMessageTable(void)
{
    return Placement_Messages;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        if (Engine_GameFlagIsSet(0x84f) != 0)
            gBiribinoNiwaPlacements[118] = 1;
        if (Engine_GameFlagIsSet(0x845) != 0)
            gBiribinoNiwaPlacements[70] = 0;
        return (const struct ScenePlacement *)gBiribinoNiwaPlacements;
    }
    return gBiribinoNiwaPlacementsOther;
}

void FieldScene_RunStepWithValueFd2(void)
{
    Engine_EventBegin();
    Engine_ActorSetPosition(0xD, 0, 0);
    Engine_GameFlagSet(0xFD2);
    Engine_ItemShowFound(ITEM_NUT, 3);
    Engine_PartyGiveItem(ITEM_NUT, 0);
    Engine_EventEnd();
}

void FieldScene_RunStepWithValue29de(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgFieldPeeredWell, 1);
    Engine_MessageShowCentered((s32)MsgBiribinoUponCloserInspectionSeemsDried, 1);
    Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        return gBiribinoNiwaEvents;
    }
    return gBiribinoNiwaEventsOther;
}

void SceneDialogue_AskAboutBarricade(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoHaveYouSeenBarricadeWe);
    Engine_EventAskYesNo(9, 0);
    Engine_EventEnd();
}

void SceneDialogue_AskIfResponsible(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoTellingMeImResponsibleFor);
    Engine_EventAskYesNo(10, 0);
    Engine_EventEnd();
}

void SceneDialogue_AskIfFineWarrior(void)
{
    Engine_EventBegin();
    Engine_EventSetMessage((s32)MsgBiribinoDoThinkCanBecomeAs);
    Engine_EventAskYesNo(11, 0);
    Engine_EventEnd();
}

void BiribinoNiwa_RunGardenEvent(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(0x84a) != 0) {
        if (Engine_GameFlagIsSet(0x304) != 0) {
            if (GameFlag_IsSet(0x201) == 0) {
                Engine_EventSetMessage((s32)MsgBiribinoWhere);
                Engine_EventShowMessageAndWait(12, 0, 10);
                Actor_ShowEmote(12, 0x107, 40);
                Engine_EventShowMessageAndWait(12, 0, 10);
                Engine_ActorRunRepeatedMotion(12, 2);
                Engine_GameFlagSet(0x201);
            }
            Engine_EventSetMessage((s32)MsgBiribinoMasterQuiteCranky);
            Engine_EventShowMessage(12, 0);
            goto done;
        }
        Engine_EventSetMessage((s32)MsgBiribinoLordMccoyDifficult);
        Engine_EventShowMessage(12, 0);
    } else {
        Engine_EventSetMessage((s32)MsgBiribinoLordOnlyMeet);
        Engine_EventOpenMessage(12, 0);
        if (Engine_EventChooseYesNo(0, 0) != 0) {
            goto declined;
        }
        gEventWork->message++;
        Engine_EventShowMessageAndWait(12, 0, 10);
        if (Object_GetById(0)->z.fixed < 0x10e0000) {
            Actor_SetSpeed(12, 0xcccc, 0x6666);
            Actor_WalkToAndWait(0, 0x15a, 0x112);
            Actor_WalkToAndWait(0, 0x148, 0x11a);
            Actor_FaceDirection(0, 0xc000, 0);
        }
        Actor_FaceDirection(11, 0x1000, 0);
        Actor_FaceDirection(12, 0x7000, 20);
        Actor_ShowEmote(11, 0x102, 20);
        Engine_ActorStartRepeatedMotion(11, 1);
        Engine_EventShowMessageAndWait(11, 0, 10);
        Actor_ShowEmote(12, 0x108, 60);
        Engine_ActorStartRepeatedMotion(12, 1);
        Engine_EventShowMessageAndWait(12, 0, 20);
        Engine_ActorSetAnimation(11, 3);
        Engine_ActorSetAnimationAndWait(12, 3);
        Actor_FaceDirection(11, 0x3000, 0);
        Actor_FaceDirection(12, 0x5000, 10);
        Engine_ActorStartRepeatedMotion(11, 1);
        Engine_EventShowMessageAndWait(11, 0, 20);
        Actor_FaceDirection(11, 0xf000, 0);
        Engine_ActorSetSpeed(12, 0x10000, 0x8000);
        Object_GetById(12)->steering_flags &= ~ACTOR_FACES_MOTION;
        Actor_WalkToAndWait(12, 0x15a, 0x107);
        Engine_EventWait(1);
        Object_GetById(12)->steering_flags |= ACTOR_FACES_MOTION;
        Actor_SetSpeed(11, 0x9999, 0x4ccc);
        Actor_WalkToAndWait(11, 0x148, 0x107);
        Actor_WalkToAndWait(11, 0x148, 252);
        Actor_FaceDirection(11, 0xc000, 10);
        FieldScene_OpenGate();
        Actor_WalkToAndWait(11, 0x148, 246);
        /* FAKEMATCH: called without its prototype, the actor id loads after the two zeros. */
        ((s32 (*)())Engine_ActorSetPosition)(11, 0, 0);
        /* FAKEMATCH: the do/while keeps this call after the argument setup of the previous one. */
        do {
            Engine_GameFlagSet(0x84a);
        } while (0);
    }
    Call3(Engine_ObjectSetTargetAndCallback, 12, 0x10000, (s32)BiribinoNiwa_GuardScript);
    goto done;
declined:
    Engine_EventShowMessage(12, 0);
    Actor_FaceDirection(12, 0x3000, 10);
done:
    Engine_EventEnd();
}

void BiribinoNiwa_LeaveThroughGate(void)
{
    Engine_EventBegin();
    if (GameFlag_IsSet(0x200) == 0) {
        FieldScene_OpenGate();
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    Engine_EventWait(16);
    Engine_EventRequestExit(2);
    Engine_EventEnd();
}

s32 Scene_Initialize(void)
{
    struct EventWork *work = gEventWork;
    struct SceneEntity *ent;
    struct SceneHandle *h;
    u8 *fp;
    s32 zero;

    work->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);

    ent = (struct SceneEntity *)Object_GetById(8);
    fp = (u8 *)ent + 35;
    /* FAKEMATCH: one register carries the stored zero and then, decremented by 13, the ~0x0c handle mask, so the two are not folded into separate constants. */
    zero = 0;
    *fp = (u8)zero;

    h = ent->h;
    zero -= 13;
    h->flags09 = (u8)((h->flags09 & zero) | 0x04);

    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        BiribinoNiwa_ApplyEntryState();
    }

    return 0;
}

void BiribinoNiwa_ApplyEntryState(void)
{
    u32 i;
    s32 record;

    if (Engine_GameFlagIsSet(0x109) != 0) {
        Engine_GameFlagClear(0x200);
    }
    if (Engine_GameFlagIsSet(0xfd2) == 0) {
        InitializeOrbitingSceneEntity(13);
    }
    if (Engine_GameFlagIsSet(0x84a) != 0) {
        Actor_SetPosition(11, 0x1340000, 0x1070000);
        Engine_ActorSetPosition(12, 0x15a0000, 0x1070000);
        if (Engine_GameFlagIsSet(0x84f) == 0) {
            if (Engine_GameFlagIsSet(0x845) == 0) {
                Engine_ActorSetPosition(11, 0, 0);
                {
                    u8 *script = BiribinoNiwa_GuardScript;

                    Call3(Engine_ObjectSetTargetAndCallback, 12, 0x10000, (s32)script);
                }
            }
        }
    }
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Actor_SetPosition(10, 0xe00000, 0x1240000);
        Actor_FaceDirection(10, 0x4000, 0);
        Engine_ActorFaceDirection(8, 0, 0);
        if (Engine_GameFlagIsSet(0x85e) == 0) {
            BiribinoNiwa_RunGardenScene();
        }
    }
}

void BiribinoNiwa_RunGardenScene(void)
{

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Camera_MoveTo(0x1400000, -1, 0x1400000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Actor_SetPosition(0, 0x1400000, 0x1740000);
    Engine_EventOpenScreen();
    Engine_CameraSetSpeed(0x3333, 0x666);
    Engine_CameraMoveTo(0x1400000, -1, 0x1220000, 1);
    Actor_SetSpeed(0, 0x9999, 0x4ccc);
    Actor_WalkToAndWait(0, 0x140, 0x136);
    Actor_FaceDirection(11, 0x3000, 10);
    Engine_ActorStartRepeatedMotion(11, 2);
    Actor_ShowEmote(11, 0x100, 60);
    Engine_EventSetMessage((s32)MsgBiribinoOhItS);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Actor_FaceDirection(12, 0x5000, 10);
    Engine_ActorStartRepeatedMotion(12, 2);
    Actor_ShowEmote(12, 0x100, 60);
    Engine_EventShowMessageAndWait(12, 0, 20);
    Actor_FaceDirection(11, 0x1000, 0);
    Actor_FaceDirection(12, 0x7000, 40);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x5000, 10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventOpenMessage(12, 0);
    Actor_FaceDirection(0, 0xe000, 0);
    while (Engine_EventChooseYesNo(0, 0) != 0) {
        Engine_ActorShowEmote(12, 0x100, 60);
        Engine_EventSetMessage((s32)MsgBiribinoNotTrueWitnesses);
        Engine_EventShowMessageAndWait(12, 0, 10);
        Engine_ActorStartRepeatedMotion(12, 2);
        Engine_EventOpenMessage(12, 0);
    }
    Engine_EventWait(10);
    Actor_FaceDirection(11, 0x3000, 0);
    Actor_FaceDirection(12, 0x5000, 20);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventSetMessage((s32)MsgBiribinoUnderArrest);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Actor_SetSpeed(11, 0x10000, 0x8000);
    Actor_WalkToAndWait(11, 0x13a, 0x118);
    Actor_FaceDirection(0, 0xc000, 20);
    Engine_EventShowMessageAndWait(11, 0, 40);
    Actor_SetAttachedEffect(0, 0x102);
    Engine_EventWait(60);
    if (Engine_GameFlagIsSet(0x84a) == 0) {
        Engine_ActorSetSpeed(12, 0x10000, 0x8000);
        Object_GetById(12)->steering_flags &= 254;
        Engine_ActorWalkToAndWait(12, 0x15a, 0x107);
        Engine_EventWait(1);
        {
            struct FieldActor *record = Object_GetById(12);

            record->steering_flags = ACTOR_FACES_MOTION | record->steering_flags;
        }
    }
    Actor_SetSpeed(11, 0xcccc, 0x6666);
    Actor_SetSpeed(0, 0xcccc, 0x6666);
    Actor_WalkTo(11, 0x148, 0x106);
    Actor_WalkToAndWait(0, 0x148, 0x116);
    Engine_ActorSetAnimation(11, 1);
    FieldScene_OpenGate();
    Engine_EventWait(40);
    Actor_WalkTo(0, 0x148, 242);
    Actor_WalkToAndWait(11, 0x148, 242);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetPosition(0, 0, 0);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(10);
    Engine_EventEnd();
}

void FieldScene_OpenGate(void)
{
    Engine_AudioPlayCue(0xBC);
    Engine_MapAnimateCells(Niwa_GateCells, 0x34, 0xB);
    Engine_GameFlagSet(0x200);
}

s32 SceneEffect_UpdateLobeOrbitEntity(struct SceneEntity_0200090c *entity)
{
    struct SceneHandle_0200090c *handle = entity->handle;
    s32 vertical;
    s32 tilt;
    s32 step;

    vertical = Engine_MathSin(entity->phase) * 2;
    if (vertical > 0) vertical = -vertical;

    entity->x = entity->origin_x + Engine_MathCos(entity->phase) * 2;
    entity->y = entity->origin_y + vertical;

    tilt = Engine_MathCos(entity->phase + 0x8000);
    if (tilt < 0) tilt += 7;
    handle->field1e = (s16)(tilt >> 3);

    step = (s32)(((u32)Engine_RandomNext() << 9) >> 16)
         + (s32)(((u32)Engine_RandomNext() << 9) >> 16);
    entity->phase = entity->phase + step + 1024;

    return 0;
}

void InitializeOrbitingSceneEntity(s32 id)
{
    OrbitingSceneObject *actor;
    OrbitingSceneObjectSprite *sprite;
    u8 *transfer;
    s32 zero;

    actor = (OrbitingSceneObject *)Object_GetById(id);
    sprite = actor->sprite;
    sprite->flags_09_mode = 1;
    sprite->flags_05_bit_5 = 0;
    sprite->flags_09_high = 0;

    zero = 0;
    sprite->state = zero;
    Engine_ActorSetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (Engine_GameFlagIsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = (u8 *)Engine_HeapAllocate(17, 0x608);
    Engine_ItemLoadIcon(ITEM_NUT);
    transfer += 0x400;
    Engine_VramLoad(sprite->palette, 128, transfer);
    Engine_HeapRelease(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateLobeOrbitEntity;
    actor->state = zero;
}
