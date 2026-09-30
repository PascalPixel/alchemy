#include "NIWA.H"
#include "TYPES.H"
#include "CALL.H"

extern u8 gBiribinoNiwaPlacements[];
extern const struct ScenePlacement gBiribinoNiwaPlacementsOther[];

extern u8 MsgBiribinoUponCloserInspectionSeemsDried[];
extern u8 MsgFieldPeeredWell[];

extern const struct SceneEvent gBiribinoNiwaEvents[];
extern const struct SceneEvent gBiribinoNiwaEventsOther[];

extern u8 MsgBiribinoDoThinkCanBecomeAs[];
extern u8 MsgBiribinoTellingMeImResponsibleFor[];
extern u8 MsgBiribinoHaveYouSeenBarricadeWe[];

extern u8 MsgBiribinoLordMccoyDifficult[];
extern u8 MsgBiribinoLordOnlyMeet[];
extern u8 MsgBiribinoMasterQuiteCranky[];
extern u8 MsgBiribinoWhere[];
extern struct EventWork *gEventWork;

/* Main-image code reached through the overlay's import veneers, declared
 * old-style: the scene passes its arguments as plain words. */
s32 Engine_GameFlagIsSet();
void Engine_EventBegin();
void Engine_EventEnd();
void Engine_EventWait();
void Engine_EventSetMessage();
void Engine_EventShowMessage();
void Engine_EventShowMessageAndWait();
s32 Engine_EventOpenMessage();
s32 Engine_EventChooseYesNo();
void Engine_ActorSetSpeed();
void Engine_ActorShowEmote();
void Engine_ActorWalkToAndWait();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorRunRepeatedMotion();
void Engine_ActorFaceDirection();
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait();
void Engine_ObjectSetTargetAndCallback();
void FieldScene_OpenGate();

/* The guard's action table, laid out after the code. */
extern u8 BiribinoNiwa_GuardScript[];

void BiribinoNiwa_ApplyEntryState(void);

void Engine_GameFlagClear();
void InitializeOrbitingSceneEntity();
void BiribinoNiwa_RunGardenScene();

extern u8 MsgBiribinoNotTrueWitnesses[];
extern u8 MsgBiribinoOhItS[];
extern u8 MsgBiribinoUnderArrest[];
void Engine_CameraMoveTo();
void Engine_MapRedraw();
void Engine_TaskWait();
void Engine_EventOpenScreen();
void Engine_CameraSetSpeed();
void Engine_ActorSetAttachedEffect();
void Engine_ActorWalkTo();
void Engine_ActorWaitForMove();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_EventRequestExit();

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

/* The actors placed in the garden, patched in place by flags 0x84f and
   0x845: the overlay image is writable. */
const struct ScenePlacement *Scene_GetPlacements(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        if (GameFlag_IsSet(0x84f) != 0)
            gBiribinoNiwaPlacements[118] = 1;
        if (GameFlag_IsSet(0x845) != 0)
            gBiribinoNiwaPlacements[70] = 0;
        return (const struct ScenePlacement *)gBiribinoNiwaPlacements;
    }
    return gBiribinoNiwaPlacementsOther;
}

void FieldScene_RunStepWithValueFd2(void)
{
    Event_Begin();
    Actor_SetPosition(0xD, 0, 0);
    GameFlag_Set(0xFD2);
    Item_ShowFound(ITEM_NUT, 3);
    Party_GiveItem(ITEM_NUT, 0);
    Event_End();
}

void FieldScene_RunStepWithValue29de(void)
{
    Event_Begin();
    Message_ShowCentered((s32)MsgFieldPeeredWell, 1);
    Message_ShowCentered((s32)MsgBiribinoUponCloserInspectionSeemsDried, 1);
    Event_End();
}

/* What the garden answers. */
const struct SceneEvent *Scene_GetEvents(void)
{
    if (gGameState.scene == (s32)&SceneId_BiribinoNiwa) {
        return gBiribinoNiwaEvents;
    }
    return gBiribinoNiwaEventsOther;
}

void SceneDialogue_AskAboutBarricade(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoHaveYouSeenBarricadeWe);
    Event_AskYesNo(9, 0);
    Event_End();
}

void SceneDialogue_AskIfResponsible(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoTellingMeImResponsibleFor);
    Event_AskYesNo(10, 0);
    Event_End();
}

void SceneDialogue_AskIfFineWarrior(void)
{
    Event_Begin();
    Event_SetMessage((s32)MsgBiribinoDoThinkCanBecomeAs);
    Event_AskYesNo(11, 0);
    Event_End();
}

void BiribinoNiwa_RunGardenEvent(void)
{
    u8 *record;

    Engine_EventBegin();
    if (Value1(Engine_GameFlagIsSet, 0x84a) != 0) {
        if (Engine_GameFlagIsSet(0x304) != 0) {
            if (Value1(Engine_GameFlagIsSet, 0x201) == 0) {
                Engine_EventSetMessage((s32)MsgBiribinoWhere);
                Engine_EventShowMessageAndWait(12, 0, 10);
                Call3(Engine_ActorShowEmote, 12, 0x107, 40);
                Engine_EventShowMessageAndWait(12, 0, 10);
                Engine_ActorRunRepeatedMotion(12, 2);
                Engine_GameFlagSet(0x201);
            }
            Engine_EventSetMessage((s32)MsgBiribinoMasterQuiteCranky);
            Engine_EventShowMessage(12, 0);
            goto L_0200041c;
        }
        Engine_EventSetMessage((s32)MsgBiribinoLordMccoyDifficult);
        Engine_EventShowMessage(12, 0);
    } else {
        Engine_EventSetMessage((s32)MsgBiribinoLordOnlyMeet);
        Engine_EventOpenMessage(12, 0);
        if (Engine_EventChooseYesNo(0, 0) != 0) {
            goto L_02000408;
        }
        *(u16 *)((*(s32 *)&gEventWork + 0x1d8)) += 1;
        Engine_EventShowMessageAndWait(12, 0, 10);
        record = ((u8 * (*)())Object_GetById)(0);
        if (*(s32 *)((s32)record + 16) <= 0x10dffff) {
            Call3(Engine_ActorSetSpeed, 12, 0xcccc, 0x6666);
            Call3(Engine_ActorWalkToAndWait, 0, 0x15a, 0x112);
            Call3(Engine_ActorWalkToAndWait, 0, 0x148, 0x11a);
            Call3(Engine_ActorFaceDirection, 0, 0xc000, 0);
        }
        Call3(Engine_ActorFaceDirection, 11, 0x1000, 0);
        Call3(Engine_ActorFaceDirection, 12, 0x7000, 20);
        Call3(Engine_ActorShowEmote, 11, 0x102, 20);
        Engine_ActorStartRepeatedMotion(11, 1);
        Engine_EventShowMessageAndWait(11, 0, 10);
        Call3(Engine_ActorShowEmote, 12, 0x108, 60);
        Engine_ActorStartRepeatedMotion(12, 1);
        Engine_EventShowMessageAndWait(12, 0, 20);
        Engine_ActorSetAnimation(11, 3);
        Engine_ActorSetAnimationAndWait(12, 3);
        Call3(Engine_ActorFaceDirection, 11, 0x3000, 0);
        Call3(Engine_ActorFaceDirection, 12, 0x5000, 10);
        Engine_ActorStartRepeatedMotion(11, 1);
        Engine_EventShowMessageAndWait(11, 0, 20);
        Call3(Engine_ActorFaceDirection, 11, 0xf000, 0);
        Engine_ActorSetSpeed(12, 0x10000, 0x8000);
        *(u8 *)(((u8 * (*)())Object_GetById)(12) + 90) &= 254;
        Call3(Engine_ActorWalkToAndWait, 12, 0x15a, 0x107);
        Engine_EventWait(1);
        {
            u8 *record = ((u8 * (*)())Object_GetById)(12);
            /* FAKEMATCH: the flag byte is read through a volatile access. */
            u8 value = *(volatile u8 *)&record[90];
        
            record[90] = (u8)(value | 1);
        }
        Call3(Engine_ActorSetSpeed, 11, 0x9999, 0x4ccc);
        Call3(Engine_ActorWalkToAndWait, 11, 0x148, 0x107);
        Call3(Engine_ActorWalkToAndWait, 11, 0x148, 252);
        Call3(Engine_ActorFaceDirection, 11, 0xc000, 10);
        FieldScene_OpenGate();
        Call3(Engine_ActorWalkToAndWait, 11, 0x148, 246);
        ((s32 (*)())Engine_ActorSetPosition)(11, 0, 0);
        /* FAKEMATCH: the do/while keeps this call after the argument setup
         * of the previous one. */
        do {
            Engine_GameFlagSet(0x84a);
        } while (0);
    }
    Call3(Engine_ObjectSetTargetAndCallback, 12, 0x10000, (s32)BiribinoNiwa_GuardScript);
    goto L_0200041c;
    L_02000408:;
    Engine_EventShowMessage(12, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x3000, 10);
    L_0200041c:;
    Engine_EventEnd();
}

void FieldScene_RunScene38e_0200045c(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x200) == 0) {
        FieldScene_OpenGate();
    }
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 2);
    Actor_CenterAndWalk(ACTOR_PARTY_LEADER, 2, -16);
    Event_Wait(16);
    Event_RequestExit(2);
    Event_End();
}

/*
 * The garden's scene start: seats scene entity 8 in its idle presentation
 * and, in the garden itself, runs the scene body.
 */
s32 Scene_Initialize(void)
{
    u8 *work = (u8 *)gEventWork;
    struct SceneEntity *ent;
    struct SceneHandle *h;
    u8 *fp;
    s32 zero;

    *(s32 *)(work + 448) = 256;            /* 128 << 1 */

    ent = (struct SceneEntity *)Object_GetById(8);
    fp = (u8 *)ent + 35;
    /* FAKEMATCH: one register carries the stored zero and then, decremented
       by 13, the ~0x0c handle mask, so the two are not folded into separate
       constants. */
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

/* McCoy's Palace garden entry: place actors 11 and 12 (or send 12 off on
 * its guard script) by the story flags, and start the garden scene when it
 * has not played yet. */
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
        Call3(Engine_ActorSetPosition, 11, 0x1340000, 0x1070000);
        Engine_ActorSetPosition(12, 0x15a0000, 0x1070000);
        if (Engine_GameFlagIsSet(0x84f) == 0) {
            if (Engine_GameFlagIsSet(0x845) == 0) {
                Engine_ActorSetPosition(11, 0, 0);
                {
                    u8 *script = BiribinoNiwa_GuardScript;

                    Value3(Engine_ObjectSetTargetAndCallback, 12, 0x10000, (s32)script);
                }
            }
        }
    }
    if (Engine_GameFlagIsSet(0x845) != 0) {
        Call3(Engine_ActorSetPosition, 10, 0xe00000, 0x1240000);
        Call3(Engine_ActorFaceDirection, 10, 0x4000, 0);
        Engine_ActorFaceDirection(8, 0, 0);
        if (Engine_GameFlagIsSet(0x85e) == 0) {
            BiribinoNiwa_RunGardenScene();
        }
    }
}

/* McCoy's Palace garden: the party walks in and actors 11 and 12 talk it
 * over until the choice is made, then the scene
 * plays out by flag 0x84a and leaves. */
void BiribinoNiwa_RunGardenScene(void)
{

    Engine_EventBegin();
    Call4(Engine_CameraMoveTo, -1, -1, -1, 0);
    Call4(Engine_CameraMoveTo, 0x1400000, -1, 0x1400000, 0);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Call3(Engine_ActorSetPosition, 0, 0x1400000, 0x1740000);
    Engine_EventOpenScreen();
    Engine_CameraSetSpeed(0x3333, 0x666);
    Engine_CameraMoveTo(0x1400000, -1, 0x1220000, 1);
    Call3(Engine_ActorSetSpeed, 0, 0x9999, 0x4ccc);
    Call3(Engine_ActorWalkToAndWait, 0, 0x140, 0x136);
    Call3(Engine_ActorFaceDirection, 11, 0x3000, 10);
    Engine_ActorStartRepeatedMotion(11, 2);
    Call3(Engine_ActorShowEmote, 11, 0x100, 60);
    Engine_EventSetMessage((s32)MsgBiribinoOhItS);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Call3(Engine_ActorFaceDirection, 12, 0x5000, 10);
    Engine_ActorStartRepeatedMotion(12, 2);
    Call3(Engine_ActorShowEmote, 12, 0x100, 60);
    Engine_EventShowMessageAndWait(12, 0, 20);
    Call3(Engine_ActorFaceDirection, 11, 0x1000, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x7000, 40);
    Call3(Engine_ActorFaceDirection, 11, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x5000, 10);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Engine_ActorRunRepeatedMotion(12, 1);
    Engine_EventOpenMessage(12, 0);
    Call3(Engine_ActorFaceDirection, 0, 0xe000, 0);
    while (Engine_EventChooseYesNo(0, 0) != 0) {
        Engine_ActorShowEmote(12, 0x100, 60);
        Engine_EventSetMessage((s32)MsgBiribinoNotTrueWitnesses);
        Engine_EventShowMessageAndWait(12, 0, 10);
        Engine_ActorStartRepeatedMotion(12, 2);
        Engine_EventOpenMessage(12, 0);
    }
    Engine_EventWait(10);
    Call3(Engine_ActorFaceDirection, 11, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 12, 0x5000, 20);
    Engine_ActorSetAnimation(11, 3);
    Engine_ActorSetAnimationAndWait(12, 3);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(11, 1);
    Engine_EventSetMessage((s32)MsgBiribinoUnderArrest);
    Engine_EventShowMessageAndWait(11, 0, 10);
    Call3(Engine_ActorSetSpeed, 11, 0x10000, 0x8000);
    Call3(Engine_ActorWalkToAndWait, 11, 0x13a, 0x118);
    Call3(Engine_ActorFaceDirection, 0, 0xc000, 20);
    Engine_EventShowMessageAndWait(11, 0, 40);
    Call2(Engine_ActorSetAttachedEffect, 0, 0x102);
    Engine_EventWait(60);
    if (Engine_GameFlagIsSet(0x84a) == 0) {
        Engine_ActorSetSpeed(12, 0x10000, 0x8000);
        ((u8 * (*)())Object_GetById)(12)[90] &= 254;
        Engine_ActorWalkToAndWait(12, 0x15a, 0x107);
        Engine_EventWait(1);
        {
            u8 *record = ((u8 * (*)())Object_GetById)(12);
            /* FAKEMATCH: a result temporary, not a compound or-assign: the
             * reference merges the byte into the mask's register, which the
             * two-address ORR does only when the result is its own object. */
            u8 merged = (u8)(record[90] | 1);

            record[90] = merged;
        }
    }
    Call3(Engine_ActorSetSpeed, 11, 0xcccc, 0x6666);
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkTo, 11, 0x148, 0x106);
    Call3(Engine_ActorWalkToAndWait, 0, 0x148, 0x116);
    Engine_ActorSetAnimation(11, 1);
    FieldScene_OpenGate();
    Engine_EventWait(40);
    Call3(Engine_ActorWalkTo, 0, 0x148, 242);
    Call3(Engine_ActorWalkToAndWait, 11, 0x148, 242);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorWaitForMove(0);
    Engine_ActorSetPosition(0, 0, 0);
    *(s32 *)((u8 *)gEventWork + 0x1c0) = 0x201;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_EventRequestExit(10);
    Engine_EventEnd();
}

void FieldScene_OpenGate(void)
{
    Audio_PlayCue(0xBC);
    Map_AnimateCells(Niwa_GateCells, 0x34, 0xB);
    GameFlag_Set(0x200);
}

/*
 * Walks one entity around a lobe of a sine and cosine figure and advances its
 * phase by a random step. The vertical term is forced non-positive, so the
 * path is one lobe rather than a full circle. The two trig calls take
 * different arguments and the two random draws are independent and summed:
 * neither pair is a common subexpression.
 */
s32 SceneEffect_UpdateLobeOrbitEntity(struct SceneEntity_0200090c *entity)
{
    struct SceneHandle_0200090c *handle = entity->handle;
    s32 vertical;
    s32 tilt;
    s32 step;

    vertical = Math_Sin(entity->phase) * 2;
    if (vertical > 0) vertical = -vertical;

    entity->x = entity->origin_x + Math_Cos(entity->phase) * 2;
    entity->y = entity->origin_y + vertical;

    /* A quarter turn on from the position phase. */
    tilt = Math_Cos(entity->phase + 0x8000);
    /* Bias then shift: division by 8 rounded toward zero. */
    if (tilt < 0) tilt += 7;
    handle->field1e = (s16)(tilt >> 3);

    /* The shift pair extracts a field, unsigned; it is not a scale. */
    step = (s32)(((u32)Random_Next() << 9) >> 16)
         + (s32)(((u32)Random_Next() << 9) >> 16);
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
    Actor_SetSpriteFlags(actor, zero);
    actor->active = zero;
    actor->mode = zero;

    if (GameFlag_IsSet(0x109) == 0)
        actor->y += 0x200000;

    actor->flags_23 &= 0xfe;
    actor->visible = 1;

    transfer = (u8 *)Engine_HeapAllocate(17, 0x608);
    Item_LoadIcon(ITEM_NUT);
    transfer += 0x400;
    Vram_Load(sprite->palette, 128, transfer);
    Heap_Release(17);

    actor->orbit_center_x = actor->x;
    actor->orbit_angle = zero;
    actor->orbit_center_y = actor->y;
    actor->active = 1;
    actor->callback = (u32)SceneEffect_UpdateLobeOrbitEntity;
    actor->state = zero;
}
