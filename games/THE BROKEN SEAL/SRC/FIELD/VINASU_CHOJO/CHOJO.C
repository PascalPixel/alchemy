#include "BATTLE.H"
#include "DEBUG.H"
#include "FIELD.H"
#include "GAME.H"
#include "GRAPHICS.H"
#include "MENU.H"
#include "MINIGAME.H"
#include "SOUND.H"
#include "SYSTEM.H"
#include "RESOURCE.H"
#include "CHOJO.H"
#include "IWRAM_CALL.H"
#include "TYPES.H"
#include "VRAM_BLOCK.H"
#include "CALLBACK_SCHEDULER.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SERVICE.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"
#include "B5_CONTEXT.H"
#include "text/MSG_IDS.H"

void VinasuChojo_ShowPartyMember();
void Engine_TaskWait();
void Engine_MapRedraw();
void Engine_ActorSetSpriteFlags();
void Engine_EventWait();
void Engine_EventBegin();
void Engine_EventEnd();
s32 Engine_EventChooseYesNo();
void Engine_ActorSetSpeed();
void Object_SetActionCallbackAndRefreshById();
void Engine_ActorSetDestination();
void Engine_ActorMoveToAndWait();
void Engine_ActorWalkTo();
void Engine_ActorWalkToAndWait();
void Engine_ActorWaitForMove();
void Engine_ActorSetPosition();
void Engine_ActorSetAnimation();
void Engine_ActorSetAnimationAndWait();
void ObjectMotion_WaitForAnimationChange();
void Engine_ActorJump();
void Engine_ActorStartRepeatedMotion();
void Engine_ActorRunRepeatedMotion();
void Engine_EventSetMessage();
s32 Engine_EventOpenMessage();
void Engine_EventShowMessageAndWait();
void Engine_ActorSetSpritePriority();
void Engine_ActorShowEmote();
void Engine_CameraSetSpeed();
void Engine_CameraMoveTo();
void Engine_CameraWaitForMove();
void Engine_EventOpenScreen();
void Engine_EventCloseScreen();
void Engine_EventWaitForScreen();
void Engine_AudioPlayCue();

void BattleFx_SetWeightedResult();
void Event_SetDefeatDestination(s32 scene, s32 entrance);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);
s32 PartyInventory_Remove();
s32 PartyInventory_FindOwner();
void VinasuChojo_CleanupSceneObjects();

struct StagedVerticalEffect;
void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect);
void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect);

struct PairDetail {
    u8 unknown_00[22];
    u8 field_16;
};

struct PairSprite {
    struct FieldSprite sprite;
    struct PairDetail *detail;
};

union PairObject {
    union FieldObject object;
    s32 words[28];
    struct {
        u8 unknown_00[0x68];
        union PairObject *parent;
    } link;
};

struct PairWork {
    u8 unknown_00[70];
    u16 vram_block;
};

extern struct PairWork *gEffectWork;

s32 AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);

struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

void VinasuChojo_FaceActor(s32 actor, s32 facing);

s32 OverlayObject_StepScaleByCounter(struct FieldActor *actor)
{
    s16 *counter = (s16 *)&actor->timer;

    switch (*counter) {
    case 6:
        actor->scale_x += -0x2000;
        actor->scale_y += 0x1000;
        break;
    case 4:
        actor->scale_x += 0x1000;
        actor->scale_y += -0x800;
        break;
    case 2:
        actor->scale_x += 0x800;
        actor->scale_y += -0x400;
        break;
    case 0:
        actor->scale_x += 0x800;
        actor->scale_y += -0x400;
        *counter = Math_RemainderUnsigned(Engine_RandomNext(), 80) + 80;
        break;
    }
    {
        s32 t = actor->timer;
        t = t - 1;
        actor->timer = t;
    }
    return 1;
}

void Effect_MoveWithDrag(union FieldObject *object)
{
    s32 velocity_x = object->effect.velocity_x;
    s32 velocity_z;

    object->effect.x += velocity_x;
    object->effect.y += object->effect.velocity_y;
    velocity_z = object->effect.velocity_z;
    object->effect.z += velocity_z;
    object->effect.velocity_x = velocity_x - velocity_x / 22;
    object->effect.velocity_z = velocity_z - velocity_z / 20;
    object->effect.scale_x += object->effect.scale_rate_x;
    object->effect.scale_y += object->effect.scale_rate_y;
    object->effect.sprite->rotation += object->effect.spin;
}

void VinasuChojo_StepSpriteRotation(struct FieldActor *actor)
{
    struct FieldSprite *sprite = actor->sprite;
    u16 rotation = sprite->rotation;

    if ((s32)((rotation + 0xffff) << 16) < 0) {
        {
            s32 t = rotation - 0xa00;
            sprite->rotation = t;
        }
    }
}

s32 OverlayObject_UpdateHeadingTimer(struct FieldActor *actor)
{
    u16 *q = &actor->work;
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Engine_RandomNext() << 15)) >> 16;
            actor->facing = actor->facing + t;
        }
        {
            s32 n = ((u32)(Engine_RandomNext() * 80)) >> 16;
            *q = n;
            if (n == 0) {
                goto out;
            }
            c = n;
        }
    }
    *q = c - 1;
out:
    return 1;
}

const struct SceneEntrance *Scene_GetEntrances(void)
{
    return gVinasuChojoEntrances;
}

const struct SceneRegion *Scene_GetRegions(void)
{
    return 0;
}

const u32 *Scene_GetExits(void)
{
    return gVinasuChojoExits;
}

const struct ScenePlacement *Scene_GetPlacements(void)
{
    return gVinasuChojoPlacements;
}

s32 MeasureFixedPointPositionDistance(s32 *first_position, s32 *second_position)
{
    s32 delta_x = (*first_position++ - *second_position++) >> 16;
    s32 delta_y = (*first_position++ - *second_position++) >> 16;
    s32 delta_z = (*first_position - *second_position) >> 16;
    s32 delta_x_squared = delta_x *delta_x;
    s32 delta_y_squared = delta_y *delta_y;
    s32 delta_z_squared = delta_z *delta_z;

    return Iwram_Sqrt(delta_x_squared + delta_y_squared + delta_z_squared);
}

s32 SceneActor_FindNearestSlotOfKindF2(void)
{
    struct EventWork *work = gEventWork;
    struct FieldActor **p;
    s32 best = 0;
    struct FieldActor *ref;
    s32 limit;
    u32 i;

    limit = 640;
    ref = Object_GetById(ACTOR_PARTY_LEADER);
    i = ACTOR_FIRST_PLACED;
    p = work->placed_actors;
    do {
        struct FieldActor *actor = *p++;
        if (actor != 0) {
            if (*((Spr_020004bc *)actor)->obj->unk28 == 0xf2) {
                s32 dist = MeasureFixedPointPositionDistance(&ref->x.fixed, &actor->x.fixed);
                if (dist < limit) {
                    limit = dist;
                    best = i;
                }
            }
        }
        i++;
    } while (i <= 65);
    return best;
}

void InitializeActorZeroMotion(void)
{
    struct FieldActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;

    actor = Object_GetById(ACTOR_PARTY_LEADER);
    angle = (actor->facing + 0x1000) & 0xe000;
    flags = actor->motion_flags;
    position[0] = (actor->x.fixed & 0xfff00000) + 0x80000;
    position[1] = actor->y.fixed;
    position[2] = (actor->z.fixed & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, angle, position);
    if (Object_CheckMovementCollision(actor, position) == 0) {
        Engine_GameFlagClear(592);
        VinasuChojo_ConfigureGroupMotion();
        Object_SetMode(actor, 6);
        Engine_TaskWait(6);
        Object_SetMode(actor, 7);
        actor->speed = 0x30000;
        actor->acceleration = 0x20000;
        Engine_AudioPlayCue(152);
        actor->velocity_y = 0x40000;
        actor->motion_flags &= 0x7e;
        Engine_ActorSetSpriteFlags(actor, 0);
        Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER, position[0] >> 16, position[2] >> 16);
        Object_SetMode(actor, 6);
        Engine_ActorSetSpriteFlags(actor, 1);
        actor->motion_flags = flags;
    }
}

void SceneEffect_SpawnAndBobWithActorZero(void)
{
    struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
    struct FieldActor *pillar;
    s32 id;
    s32 k;

    Engine_EventBegin();
    id = SceneActor_FindNearestSlotOfKindF2();
    VinasuChojo_NearestActor = id;
    if (id != 0) {
        Engine_GameFlagSet(592);
        pillar = Object_GetById(VinasuChojo_NearestActor);
        pillar->motion_flags = 0;
        leader->motion_flags &= 0xfe;
        pillar->y.fixed += -0x30000;
        leader->y.fixed += -0x30000;
        leader->terrain_height += -0x30000;
        Engine_TaskWait(2);
        pillar->y.fixed += -0x20000;
        leader->y.fixed += -0x20000;
        leader->terrain_height += -0x20000;
        Engine_TaskWait(10);
        k = 0x20000;
        pillar->y.fixed += k;
        leader->y.fixed += k;
        leader->terrain_height += k;
        Engine_TaskWait(4);
        pillar->y.fixed += k;
        leader->y.fixed += k;
        leader->terrain_height += k;
        Engine_TaskWait(4);
        pillar->y.fixed += 0x10000;
        leader->y.fixed += 0x10000;
        leader->terrain_height += 0x10000;
    }
    Engine_EventEnd();
}

void VinasuChojo_ConfigureGroupMotion(void)
{
    s32 flags;

    Object_GetById(0)->motion_flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    flags = 4;
    Object_GetById(12)->motion_flags = flags;
    Object_GetById(13)->motion_flags = flags;
    Object_GetById(14)->motion_flags = flags;
    Object_GetById(15)->motion_flags = flags;
    Object_GetById(16)->motion_flags = flags;
    Object_GetById(17)->motion_flags = flags;
}

const struct SceneEvent *Scene_GetEvents(void)
{
    return gVinasuChojoEvents;
}

s32 Scene_Initialize(void)
{
    s32 v;
    u32 i;
    s32 z;
    s16 mode;
    Spr_0200071c *obj;

    Engine_GameFlagSet(324);
    Engine_TaskWait(1);
    Engine_GameFlagSet(272);
    Engine_ActorSetSpriteFlags(Object_GetById(8), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(9), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
    Engine_ActorSetSpriteFlags(Object_GetById(11), 0);
    v = (s32)0xffff0000;
    Object_GetById(10)->scale_x = v;
    Object_GetById(11)->scale_x = v;
    i = 12;
    z = 0;
    do {
        obj = Object_GetById(i);
        Engine_ActorSetSpriteFlags(Object_GetById(i), z);
        Engine_ActorSetSpritePriority(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = gGameState.entrance;

    switch (mode) {
    case 1:
        if (Engine_GameFlagIsSet(0x109) == 0) {
            Scene_RunScriptedActorPresentation();
        }
        break;
    case 2:
        Scene_RunPairedActorEffectSequence();
        break;
    case 3:
        FieldScene_RunThreeStepsInBracket();
        break;
    case 0x5d:
        FieldScene_RunBracketedSceneWithFlag282();
        break;
    case 4:
        VinasuChojo_RunLeapEntrance();
        break;
    case 9:
        Engine_EventBegin();
        if (Engine_GameFlagIsSet(0x345) != 0) {
            Inventory_AddItem(0, 65);
        } else if (Engine_GameFlagIsSet(0x346) != 0) {
            Inventory_AddItem(1, 65);
        } else if (Engine_GameFlagIsSet(0x347) != 0) {
            Inventory_AddItem(2, 65);
        } else {
            Inventory_AddItem(3, 65);
        }
        Engine_EventRequestExit(9);
        break;
    }

    if (Engine_GameFlagIsSet(0x109) != 0) {
        s32 slot = SceneActor_FindNearestSlotOfKindF2();

        if (slot != 0) {
            Spr_0200071c *p = (Spr_0200071c *)Object_GetById(slot);
            if (p != 0) {
                p->unk55 = 0;
            }
        }
    }
    return z;
}

void VinasuChojo_ShowMessage(s32 speaker)
{
    Engine_EventShowMessage(speaker, 0);
    Engine_EventWait(10);
}

void VinasuChojo_FaceActor(s32 actor, s32 facing)
{
    Engine_ActorFaceDirection(actor, facing, 10);
}

void Scene_RunScriptedActorPresentation(void)
{
    struct FieldActor *companion;
    struct FieldActor *leader;

    companion = Object_GetById(18);
    Engine_EventBegin();
    VinasuChojo_ShowPartyMember(1, 0);
    VinasuChojo_ShowPartyMember(2, 0);
    VinasuChojo_ShowPartyMember(3, 0);
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    companion->motion_flags = 0;
    companion->priority_flags |= ACTOR_PRIORITY_UNDERFOOT;
    Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
    Engine_ActorSetSpritePriority(18, 1);
    Engine_ActorSetPosition(18, PIXELS(580), PIXELS(338));
    leader = Object_GetById(ACTOR_PARTY_LEADER);
    leader->motion_flags = 0;
    Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetPosition(ACTOR_PARTY_LEADER, PIXELS(581), PIXELS(288));
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Actor_SetSpeed(18, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Object_GetById(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetDestination(18, 580, 221);
    Actor_MoveToAndWait(0, 581, 171);
    Actor_SetDestination(18, 530, 211);
    Actor_MoveToAndWait(0, 531, 161);
    Actor_SetDestination(18, 520, 191);
    Engine_ActorMoveToAndWait(0, 521, 141);
    Engine_ActorSetSpriteFlags(Object_GetById(18), 1);
    Actor_SetDestination(18, 515, 171);
    Engine_ActorMoveToAndWait(0, 516, 121);
    Engine_AudioPlayCue(0x120);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 6);
    ObjectMotion_WaitForAnimationChange(0);
    leader->x.fixed = PIXELS(516);
    leader->y.fixed = PIXELS(8);
    leader->z.fixed = PIXELS(148);
    leader->facing = FACING_WEST;
    leader->motion_flags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    Engine_AudioPlayCue(152);
    leader->velocity_y = 0x40000;
    Engine_AudioPlayCue(152);
    Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PARTY_LEADER), 1);
    Engine_ActorMoveToAndWait(0, 504, 148);
    Engine_EventWait(10);
    Object_GetById(ACTOR_PARTY_LEADER)->steering_flags |= ACTOR_FACES_MOTION;
    {
        u16 shown = FACING_SOUTH;

        leader->y.fixed = -PIXELS(32);
        leader->facing = shown;
    }
    Engine_EventWait(20);
    Engine_AudioPlayCue(0x134);
    Engine_ActorMoveToAndWait(18, 524, 191);
    Engine_ActorSetSpriteFlags(Object_GetById(18), 0);
    Actor_MoveToAndWait(18, 530, 211);
    Actor_MoveToAndWait(18, 580, 221);
    Engine_ActorSetDestination(18, 580, 338);
    Object_GetById(ACTOR_PARTY_LEADER)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    VinasuChojo_ShowPartyMember(1, 1);
    VinasuChojo_ShowPartyMember(2, 1);
    VinasuChojo_ShowPartyMember(3, 1);
    Actor_WalkTo(ACTOR_PARTY_LEADER, 492, 164);
    Engine_ActorWalkTo(1, 514, 164);
    Actor_WalkTo(IWAN, 492, 140);
    Engine_ActorWalkToAndWait(3, 514, 140);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(GERALD, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(IWAN, FACING_SOUTH, 0);
    Engine_ActorFaceDirection(MEARI, FACING_SOUTH, 0);
    Engine_ActorWaitForMove(18);
    Engine_ActorSetPosition(18, 0, 0);
    Engine_AudioPlayCue(0x121);
    Engine_EventSetMessage((s32)MsgVinasuLongLastTime);
    VinasuChojo_ShowMessage(0x4013);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Engine_ActorFaceDirection(GERALD, FACING_WEST, 0);
    Engine_ActorFaceDirection(IWAN, FACING_WEST, 0);
    VinasuChojo_FaceActor(MEARI, FACING_WEST);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Engine_CameraSetSpeed(0x4cccc, 0x9999);
    Engine_CameraMoveTo(0x1300000, 0x200000, 0x9e0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
    Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Engine_AudioPlayCue(61);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x4013);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 40);
    VinasuChojo_ShowMessage(0x2014);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x100, 0);
    Engine_ActorShowEmote(20, 0x100, 20);
    Actor_FaceDirection(6, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 20);
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Camera_MoveTo(0x1260000, -1, 0xc20000, 1);
    Engine_EventWait(20);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 272, 200);
    Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(0x2014);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x103, 20);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Engine_ActorSetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x101, 40);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 60);
    Engine_EventShowMessageAndWait(19, 0, 40);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x106, 40);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_WEST);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorFaceDirection(6, FACING_EAST, 0);
    Actor_ShowEmote(21, 0x103, 40);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 40);
    Engine_ActorRunRepeatedMotion(21, 1);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 0);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTH + FACING_STEP);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x108, 20);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 40);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
    Engine_EventShowMessageAndWait(0x2014, 0, 40);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 0);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x105, 80);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 60);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorSetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Engine_ActorStartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 60);
    Engine_EventShowMessageAndWait(0x2014, 0, 20);
    VinasuChojo_FaceActor(21, FACING_NORTHWEST + FACING_STEP);
    VinasuChojo_ShowMessage(21);
    VinasuChojo_FaceActor(6, FACING_SOUTHEAST + FACING_STEP);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_EventWait(20);
    VinasuChojo_FaceActor(21, FACING_NORTH + FACING_STEP);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorFaceDirection(6, FACING_EAST, 0);
    Engine_ActorRunRepeatedMotion(21, 1);
    VinasuChojo_ShowMessage(21);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x101, 0);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 80);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 0);
    Engine_CameraSetSpeed(0x6666, 0xccc);
    Camera_MoveTo(0x1260000, -1, 0xb40000, 1);
    Actor_WalkToAndWait(21, 262, 176);
    Engine_ActorFaceDirection(21, FACING_WEST, 40);
    Engine_ActorFaceDirection(21, FACING_EAST, 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(0x8015);
    Engine_ActorShowEmote(19, 0x100, 20);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(0x8015);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 40);
    Event_ShowMessageAndWait(0xa014, 0, 20);
    Actor_ShowEmote(21, 0x105, 20);
    VinasuChojo_ShowMessage(0x8015);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x103, 20);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x101, 40);
    VinasuChojo_ShowMessage(0x8015);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0xa014);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 60);
    Engine_ActorFaceDirection(21, FACING_WEST, 20);
    Engine_EventShowMessageAndWait(0xa015, 0, 40);
    Actor_ShowEmote(6, 0x105, 120);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 60);
    VinasuChojo_ShowMessage(0xa014);
    Engine_ActorFaceDirection(21, FACING_EAST, 40);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventShowMessageAndWait(0x8015, 0, 20);
    VinasuChojo_ShowMessage(0xa014);
    Engine_ActorShowEmote(21, 0x100, 40);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(6, 0x105, 40);
    Engine_ActorShowEmote(20, 0x108, 40);
    VinasuChojo_ShowMessage(0xa014);
    Engine_ActorShowEmote(19, 0x103, 20);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    VinasuChojo_ShowMessage(0xa014);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1f80000, -0x180000, 0xa80000, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(GERALD, 1);
    VinasuChojo_ShowMessage(0x8001);
    Engine_ActorShowEmote(3, 0x101, 40);
    VinasuChojo_ShowMessage(MEARI);
    Engine_ActorSetAnimationAndWait(IWAN, 3);
    Engine_EventShowMessageAndWait(0x1002, 0, 40);
    Engine_ActorJump(GERALD, 2, 20);
    VinasuChojo_ShowMessage(0x8001);
    Engine_EventOpenMessage(0x8001, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Engine_ActorFaceDirection(IWAN, FACING_SOUTH, 0);
    Actor_FaceDirection(MEARI, FACING_SOUTHEAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        gEventWork->message += 1;
    }
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(GERALD);
    Engine_ActorFaceDirection(IWAN, FACING_EAST, 0);
    Engine_ActorFaceDirection(MEARI, FACING_WEST, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    Engine_ActorEnableActionCallback(GERALD, VinasuChojo_PairActionScript);
    Engine_ActorEnableActionCallback(IWAN, VinasuChojo_PairActionScript);
    Object_SetActionCallbackAndRefreshById(MEARI, VinasuChojo_PairActionScript);
    Engine_EventWait(20);
    Engine_EventEnd();
}

void VinasuChojo_ShowPartyMember(s32 actor, s32 shown)
{
    s32 record;

    if (shown != 0) {
        Actor_SetChildValue(actor, 0);
        record = Actor_Get(actor);
        Engine_ActorSetSpriteFlags(record, 1);
        Actor_SetSpeed(actor, 0xcccc, 0x6666);
    } else {
        Actor_SetChildValue(actor, 15);
        record = Actor_Get(actor);
        Engine_ActorSetSpriteFlags(record, 0);
    }
}

void Scene_RunActorEntrySequence(void)
{
    s32 owner;
    struct FieldActor *object;
    s32 pending;
    s32 frame;
    s32 message;

    Engine_EventBegin();
    Engine_EventGetViewCenter()->motion_flags = 0;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(PIXELS(332), PIXELS(32), PIXELS(180), 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 340, 184);
    VinasuChojo_FaceActor(ACTOR_PARTY_LEADER, FACING_WEST);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventSetMessage((s32)MsgVinasuRobin);
    VinasuChojo_ShowMessage(0x9015);
    Engine_EventGetViewCenter()->motion_flags = 0;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 1);
    Actor_SetSpeed(GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(IWAN, 0x10000, 0x8000);
    Actor_SetSpeed(MEARI, 0x16666, 0xb333);
    object = Object_GetById(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(GERALD, object->x.fixed, object->z.fixed);
    }
    object = Object_GetById(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(IWAN, object->x.fixed, object->z.fixed);
    }
    object = Object_GetById(ACTOR_PARTY_LEADER);
    if (object != 0) {
        Actor_SetPosition(MEARI, object->x.fixed, object->z.fixed);
    }
    Actor_WalkTo(GERALD, 328, 168);
    Actor_WalkTo(IWAN, 340, 196);
    Actor_WalkToAndWait(MEARI, 326, 204);
    Engine_ActorSetAnimation(GERALD, 1);
    Engine_ActorSetAnimation(IWAN, 1);
    Engine_ActorFaceDirection(GERALD, FACING_WEST, 0);
    Engine_ActorFaceDirection(IWAN, FACING_WEST, 0);
    VinasuChojo_FaceActor(MEARI, FACING_WEST);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 40);
    Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_WEST);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    VinasuChojo_ShowMessage(21);
    Engine_ActorStartRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(21, 0, 20);
    Engine_ActorFaceDirection(21, FACING_NORTH + FACING_STEP, 40);
    VinasuChojo_ShowMessage(21);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_FaceActor(21, FACING_EAST);
    Event_ShowMessageAndWait(21, 0, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_WEST);
    VinasuChojo_ShowMessage(0x2014);
    Actor_SetAttachedEffect(MEARI, 0x102);
    Engine_ActorStartRepeatedMotion(MEARI, 2);
    VinasuChojo_ShowMessage(MEARI);
    Engine_ActorRunRepeatedMotion(IWAN, 1);
    VinasuChojo_ShowMessage(IWAN);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x106, 40);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 20);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
    Engine_ActorFaceDirection(21, FACING_WEST, 0);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x102, 40);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 20);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Engine_ActorFaceDirection(GERALD, FACING_WEST, 0);
    Engine_ActorFaceDirection(IWAN, FACING_WEST, 0);
    Engine_ActorFaceDirection(MEARI, FACING_WEST, 0);
    Engine_ActorFaceDirection(21, FACING_EAST, 20);
    Actor_ShowEmote(6, 0x101, 40);
    VinasuChojo_ShowMessage(6);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 20);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(6, 3);
    VinasuChojo_ShowMessage(6);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2014);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 260, 186);
    Engine_ActorFaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_WalkToAndWait(6, 276, 192);
    object = Object_GetById(ACTOR_SECOND_OF_PAIR);
    object->facing = FACING_SOUTH + FACING_STEP;
    Engine_TaskWait(1);
    Engine_ActorStartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorJump(6, 2, 20);
    Actor_SetSpeed(6, 0x26666, 0x13333);
    Actor_WalkToAndWait(6, 260, 186);
    Engine_ActorFaceDirection(21, FACING_WEST, 0);
    Actor_WalkToAndWait(6, 248, 172);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
    Engine_ActorFaceDirection(21, FACING_WEST, 0);
    Engine_ActorFaceDirection(6, FACING_EAST, 20);
    Engine_ActorSetAnimationAndWait(6, 3);
    Engine_EventWait(40);
    SceneState_ApplyPair140And0();
    Engine_TaskWait(1);
    frame = 0;
    do {
        object = Object_GetById(6);
        SceneEffect_UpdateObjectByFrameParity((s32)object);
        frame = (frame + 1);
        Engine_TaskWait(1);
    } while ((u32)frame <= 39);
    Engine_TaskAddCallback(FieldScene_RunStep6, TASK_PRIORITY_SCENE);
    Engine_EventWait(80);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 20);
    Engine_ActorFaceDirection(21, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 0);
    VinasuChojo_FaceActor(21, FACING_WEST);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x101, 40);
    Event_ShowMessageAndWait(0x2014, 0, 20);
    Actor_ShowEmote(6, 0x105, 80);
    Engine_ActorStartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    VinasuChojo_ShowMessage(0x2013);
    Scheduler_RemoveCallback((u32)FieldScene_RunStep6);
    Engine_TaskWait(1);
    Actor_SetChildValue(6, 0);
    Engine_TaskWait(10);
    VinasuChojo_CleanupSceneObjects();
    Engine_ActorJump(6, 2, 40);
    VinasuChojo_ShowMessage(6);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 20);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorRunRepeatedMotion(6, 2);
    Object_GetById(6)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(6, 250, 176);
    Engine_EventWait(1);
    Object_GetById(6)->steering_flags |= ACTOR_FACES_MOTION;
    Actor_ShowEmote(21, 0x103, 20);
    Engine_ActorFaceDirection(21, FACING_EAST, 20);
    VinasuChojo_ShowMessage(21);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_EventWait(40);
    Actor_SetSpeed(6, 0x9999, 0x4ccc);
    Object_GetById(6)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(6, 248, 172);
    Engine_EventWait(1);
    Object_GetById(6)->steering_flags |= ACTOR_FACES_MOTION;
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(6, 3);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorSetAnimationAndWait(6, 3);
    VinasuChojo_ShowMessage(6);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x105, 0);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 60);
    Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2014);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 40);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x105, 100);
    Engine_ActorRunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Engine_EventWait(20);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
    Event_OpenMessage(ACTOR_FIRST_OF_PAIR, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
        pending = 1;
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
        gEventWork->message += 1;
        pending = 0;
    }
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    if (pending != 0) {
        gEventWork->message += 1;
    }
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 20);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
    Event_OpenMessage(ACTOR_FIRST_OF_PAIR, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTH, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Engine_ActorSetAnimation(IWAN, 3);
        pending = 1;
    } else {
        Engine_EventWait(20);
        Engine_ActorSetAnimation(IWAN, 4);
        gEventWork->message += 1;
        pending = 0;
    }
    VinasuChojo_ShowMessage(IWAN);
    if (pending != 0) {
        gEventWork->message += 1;
    }
    Engine_ActorRunRepeatedMotion(GERALD, 2);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTH);
    VinasuChojo_ShowMessage(GERALD);
    Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    VinasuChojo_FaceActor(MEARI, FACING_WEST);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    Event_ShowMessageAndWait(ACTOR_FIRST_OF_PAIR, 0, 20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GERALD, 3);
    Engine_ActorSetAnimation(IWAN, 3);
    Engine_ActorSetAnimationAndWait(MEARI, 3);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 3);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    Engine_EventWait(40);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_WEST);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorRunRepeatedMotion(21, 2);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(21, 0, 40);
    VinasuChojo_ShowMessage(0x2014);
    Actor_ShowEmote(21, 0x103, 60);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_WEST);
    Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x105, 60);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventWait(20);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 288, 192);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 0);
    Actor_WalkToAndWait(21, 310, 192);
    Actor_WalkToAndWait(21, 328, 186);
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(21, 2);

    message = (s32)MsgVinasuRobinHandedOverShamansRod;
    Engine_MessageShowCentered(message, 1);
    Actor_WalkToAndWait(21, 310, 192);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_WEST, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_WEST, 0);
    Actor_WalkToAndWait(21, 288, 192);
    Actor_WalkToAndWait(21, 262, 176);
    Engine_ActorFaceDirection(21, FACING_EAST, 40);
    Engine_EventSetMessage((message + 1));
    VinasuChojo_ShowMessage(21);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2014);
    Engine_ActorSetAnimationAndWait(21, 3);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
    VinasuChojo_FaceActor(21, FACING_WEST);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_ActorSetAnimationAndWait(6, 3);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 260, 186);
    Engine_ActorFaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_WalkToAndWait(6, 276, 192);
    Audio_PlayCue(19);
    object = Object_GetById(ACTOR_SECOND_OF_PAIR);
    object->facing = FACING_SOUTH + FACING_STEP;
    Engine_TaskWait(1);
    Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_ActorFaceDirection(21, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(6, FACING_NORTH + FACING_STEP, 20);
    Engine_ActorJump(MEARI, 2, 20);
    VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
    VinasuChojo_ShowMessage(MEARI);
    Engine_ActorFaceDirection(21, FACING_EAST, 0);
    Engine_ActorFaceDirection(6, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 80);
    Engine_ActorFaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_EAST, 40);
    Audio_PlayCue(29);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Engine_ActorFaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(6, FACING_NORTHWEST + FACING_STEP, 20);
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    VinasuChojo_ShowMessage(IWAN);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Engine_ActorFaceDirection(21, FACING_EAST, 0);
    VinasuChojo_FaceActor(6, FACING_NORTH + FACING_STEP);
    Engine_ActorSetAnimation(MEARI, 3);
    VinasuChojo_ShowMessage(MEARI);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x100, 20);
    Engine_ActorFaceDirection(ACTOR_FIRST_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 20);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_ShowEmote(6, 0x102, 0);
    Actor_ShowEmote(21, 0x102, 60);
    Engine_ActorFaceDirection(6, FACING_EAST, 0);
    VinasuChojo_FaceActor(21, FACING_EAST);
    Actor_ShowEmote(GERALD, 0x103, 40);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    VinasuChojo_ShowMessage(GERALD);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
    Engine_ActorSetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
    Engine_ActorSetAnimation(ACTOR_FIRST_OF_PAIR, 4);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Engine_ActorFaceDirection(6, FACING_NORTH + FACING_STEP, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(MEARI, 0x101, 60);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP);
    Engine_ActorSetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_SetAttachedEffect(IWAN, 0x102);
    Event_ShowMessageAndWait(IWAN, 0, 20);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
    Engine_ActorSetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    Engine_ActorStartRepeatedMotion(IWAN, 2);
    Engine_ActorRunRepeatedMotion(MEARI, 2);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
    Engine_ActorRunRepeatedMotion(MEARI, 1);
    VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
    VinasuChojo_ShowMessage(MEARI);
    Engine_ActorFaceDirection(GERALD, FACING_WEST, 20);
    Actor_ShowEmote(GERALD, 0x103, 20);
    Engine_ActorStartRepeatedMotion(GERALD, 2);
    VinasuChojo_ShowMessage(GERALD);
    Engine_ActorFaceDirection(6, FACING_EAST, 0);
    Engine_ActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Engine_ActorFaceDirection(IWAN, FACING_WEST, 40);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x103, 40);
    Engine_ActorStartRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Engine_ActorRunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    owner = PartyInventory_FindOwner(65);
    GameFlag_Set(owner + 0x345);
    PartyInventory_Remove(65);
    Object_GetById(ACTOR_PARTY_LEADER)->steering_flags &= ~ACTOR_FACES_MOTION;
    Object_GetById(GERALD)->steering_flags &= ~ACTOR_FACES_MOTION;
    Object_GetById(IWAN)->steering_flags &= ~ACTOR_FACES_MOTION;
    Object_GetById(MEARI)->steering_flags &= ~ACTOR_FACES_MOTION;
    Object_GetById(ACTOR_SECOND_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
    Object_GetById(ACTOR_FIRST_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
    Object_GetById(21)->steering_flags &= ~ACTOR_FACES_MOTION;
    Object_GetById(6)->steering_flags &= ~ACTOR_FACES_MOTION;
    Engine_ActorEnableActionCallback(ACTOR_PARTY_LEADER, SceneAction_EntryGroup);
    Engine_ActorEnableActionCallback(GERALD, SceneAction_EntryGroup);
    Engine_ActorEnableActionCallback(IWAN, SceneAction_EntryGroup);
    Engine_ActorEnableActionCallback(MEARI, SceneAction_EntryGroup);
    Engine_ActorEnableActionCallback(ACTOR_SECOND_OF_PAIR, SceneAction_EntryPair);
    Engine_ActorEnableActionCallback(ACTOR_FIRST_OF_PAIR, SceneAction_EntryPair);
    Engine_ActorEnableActionCallback(21, SceneAction_EntryGroup);
    Object_SetActionCallbackAndRefreshById(6, SceneAction_EntryGroup);
    /* FAKEMATCH: a plain store is scheduled into the last action call's argument setup; the one-pass block keeps it after the call. Measured 2026-10-04. */
    do {
        gGameState.battle_start = 3;
    } while (0);
    {
        s32 actor = (s32)&SceneId_VinasuChojo;

        Call2(Party_SetFields1ceAnd1d0, actor, 2);
        Call2(Event_SetDefeatDestination, actor, 9);
    }
    BattleFx_SetWeightedResult(98, 1);
    GameFlag_Set(0x350);
}

void SceneEffect_UpdateObjectByFrameParity(s32 a)
{
    if (*(s32 *)&gFrameCount & 2) {
        Engine_ObjectSetPartPalettes(a, 7);
    } else {
        Engine_ObjectSetPartPalettes(a, 0);
    }
    if (Math_RemainderUnsigned(*(s32 *)&gFrameCount, 15) == 0) {
        VinasuChojo_SpawnLinkedPairEffects(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    if (gFrameCount & 1) {
        Engine_ObjectSetPartPalettes(a, Math_RemainderUnsigned(gFrameCount >> 1, 6));
    }
    if (Math_RemainderUnsigned(gFrameCount, 15) == 0) {
        VinasuChojo_SpawnLinkedPairEffects(a);
    }
}

void Effect_AnimateVerticalPositive(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Engine_MathSin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] + offset * 5 + 0x80000;
    }
}

void Effect_AnimateVerticalNegative(struct StagedVerticalEffect *effect)
{
    s32 *anchor = effect->f68;
    s32 frame = ++effect->f64;
    if (frame > 31) {
        Engine_ObjectDispatchRelease((s32)effect);
    } else {
        s32 amplitude = Engine_MathSin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}

void VinasuChojo_SpawnLinkedPairEffects(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = gEffectWork;
    s32 i;

    Engine_AudioPlayCue(131);
    for (i = 0; i < 2; ++i) {
        child = (union PairObject *)Engine_ObjectCreate(26,
            parent->object.actor.x.fixed, parent->object.actor.y.fixed,
            parent->object.actor.z.fixed);
        pair[i] = child;
        if (child != NULL) {
            child->words[5] = parent->words[5];
            part = (struct PairSprite *)child->object.actor.sprite;
            child->object.actor.motion_flags = 0;
            child->object.effect.spin = 0;
            child->link.parent = parent;
            if (part != NULL) {
                sprite = &part->sprite;
                AnimationObjects_SelectAnimation(sprite, 0);
                sprite->flags = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: a plain byte access; the struct field store leaves a dead QImode zero that takes r3 from the +85 address. */
                *(u8 *)&sprite->unknown_1d |= 1;
                sprite->tile = (gVramBlockCache[sprite->vram_block].offset >> 5) & 0x3ff;
                sprite->full_color = 0;
                sprite->shape = 1;
                ((struct WorldMapOam *)sprite)->size = 2;
                part->detail->field_16 = 0;
            }
        }
    }
    {
        union PairObject *p = pair[0];
        struct FieldSprite *sp = p->object.actor.sprite;

        p->object.actor.update = (void (*)(union FieldObject *))Effect_AnimateVerticalNegative;
        sp->priority = parent->object.actor.sprite->priority;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = parent->object.actor.sprite->priority;
        p->update = (void (*)(union FieldObject *))Effect_AnimateVerticalPositive;
        p->priority_flags = 2;
    }
}

void SceneState_ApplyPair140And0(void)
{
    Engine_PsynergyBegin(140, 0);
}

void VinasuChojo_CleanupSceneObjects(s32 a)
{
    BattleEffect_CleanupSceneObjects(a);
}

void FieldScene_RunStep6(void)
{
    SceneState_ForwardByRuntimeWordBits((s32)Object_GetById(6));
}

void SceneEffect_SpawnParticlesAboveActor(void);
void Owner_RefreshRatiosOnFlag(void);

static inline void VinasuChojo_AimBurst(s32 velocity[3], s32 angle)
{
    s32 sine;
    s32 cosine;

    velocity[0] = Engine_MathCos(angle);
    velocity[1] = 0;
    sine = Engine_MathSin(angle);
    cosine = velocity[0];
    cosine += cosine * 2;
    sine *= 2;
    velocity[2] = sine;
    velocity[0] = cosine;
}

void Scene_RunPairedActorEffectSequence(void)
{
    struct EffectOptions second_burst;
    s32 velocity[3];
    struct EffectOptions first_burst;
    struct FieldActor *actor;
    struct FieldActor *second;
    struct FieldActor *first;
    union FieldObject *star = NULL;
    struct FieldActor *bird;
    struct FieldSprite *sprite;
    u8 *buffer;
    u32 i;

    Event_Begin();
    Map_CopyCellAttributes(17, 10, 4, 2, 17, 8);
    Actor_Get(GERALD)->facing = FACING_WEST;
    Actor_SetPosition(GERALD, PIXELS(328), PIXELS(168));
    Actor_Get(IWAN)->facing = FACING_WEST;
    Actor_SetPosition(IWAN, PIXELS(340), PIXELS(196));
    Actor_Get(MEARI)->facing = FACING_WEST;
    Actor_SetPosition(MEARI, PIXELS(326), PIXELS(204));
    Actor_Get(6)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(6, PIXELS(268), PIXELS(154));
    Actor_Get(21)->facing = FACING_SOUTHEAST + FACING_STEP;
    Actor_SetPosition(21, PIXELS(268), PIXELS(164));
    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->timer = 10;
    actor->facing = FACING_NORTH + FACING_STEP;
    Actor_SetPosition(ACTOR_FIRST_OF_PAIR, PIXELS(294), PIXELS(212));
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 9);
    Actor_EnableActionCallback(ACTOR_FIRST_OF_PAIR, VinasuChojo_ScaleStepAction);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_OF_PAIR), 0);
    actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
    actor->timer = 10;
    actor->facing = FACING_EAST;
    Actor_SetPosition(ACTOR_SECOND_OF_PAIR, PIXELS(286), PIXELS(192));
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 7);
    Actor_EnableActionCallback(ACTOR_SECOND_OF_PAIR, VinasuChojo_ScaleStepAction);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_OF_PAIR), 0);
    Event_GetViewCenter()->motion_flags = 0;
    Camera_MoveTo(PIXELS(304), PIXELS(32), PIXELS(180), 0);
    Task_Wait(1);
    Map_Redraw();
    Task_Wait(1);
    Event_OpenScreen();
    Event_WaitForScreen();
    Event_Wait(80);

    Actor_Jump(GERALD, 2, 20);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTHEAST);
    Event_SetMessage((s32)MsgVinasuBeatEm);
    VinasuChojo_ShowMessage(0x1001);
    VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
    VinasuChojo_ShowMessage(MEARI);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    VinasuChojo_FaceActor(IWAN, FACING_NORTHWEST);
    Actor_RunRepeatedMotion(21, 2);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_Get(21)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(21, 0x118, 164);
    Event_Wait(1);
    Actor_Get(21)->steering_flags |= ACTOR_FACES_MOTION;
    Event_Wait(20);
    Actor_ShowEmote(IWAN, 0x102, 40);
    VinasuChojo_ShowMessage(IWAN);
    Actor_SetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(GERALD, 0x103, 20);
    Actor_RunRepeatedMotion(GERALD, 1);
    VinasuChojo_ShowMessage(GERALD);
    Actor_RunRepeatedMotion(21, 1);
    VinasuChojo_FaceActor(21, FACING_EAST);
    VinasuChojo_ShowMessage(21);
    Actor_StartRepeatedMotion(MEARI, 2);
    VinasuChojo_ShowMessage(MEARI);
    Actor_SetAnimation(MEARI, 4);
    VinasuChojo_ShowMessage(MEARI);
    Actor_ShowEmote(21, 0x105, 40);
    VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
    VinasuChojo_ShowMessage(21);
    Actor_Jump(IWAN, 2, 20);
    VinasuChojo_ShowMessage(IWAN);
    Actor_SetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Actor_RunRepeatedMotion(GERALD, 1);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTHEAST);
    Event_OpenMessage(GERALD, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHEAST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        i = TRUE;
    } else {
        gEventWork->message++;
        i = FALSE;
    }
    Task_Wait(20);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
    VinasuChojo_FaceActor(21, FACING_EAST);
    Event_ShowMessage(21, 0);
    Audio_PlayCue(17);
    Event_Wait(40);
    if (i) {
        gEventWork->message++;
    }

    Actor_Stop(ACTOR_FIRST_OF_PAIR);
    Actor_Stop(ACTOR_SECOND_OF_PAIR);
    Task_Wait(1);
    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    actor = Actor_Get(ACTOR_SECOND_OF_PAIR);
    actor->scale_x = 0x10000;
    actor->scale_y = 0x10000;
    Task_Wait(1);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 40);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 1);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_OF_PAIR), 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_FIRST_OF_PAIR, 0x3333, 0x1999);
    Actor_WalkToAndWait(ACTOR_FIRST_OF_PAIR, 0x12c, 206);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 2);
    Event_Wait(20);
    Actor_Get(ACTOR_FIRST_OF_PAIR)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 8);
    Event_Wait(20);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 1);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_OF_PAIR), 1);
    Actor_Jump(ACTOR_SECOND_OF_PAIR, 4, 40);
    Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x3333, 0x1999);
    Actor_Get(ACTOR_SECOND_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_WalkToAndWait(ACTOR_SECOND_OF_PAIR, 0x128, 186);
    Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x1999, 0xccc);
    Actor_WalkToAndWait(ACTOR_SECOND_OF_PAIR, 0x124, 186);
    Actor_Get(ACTOR_SECOND_OF_PAIR)->steering_flags |= ACTOR_FACES_MOTION;
    Actor_Get(ACTOR_SECOND_OF_PAIR)->scale_x = -0x10000;
    Audio_PlayCue(161);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 5);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    Event_Wait(20);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Event_Wait(80);
    Actor_SetAnimationAndWait(MEARI, 4);
    Event_ShowMessageAndWait(MEARI, 0, 20);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    Event_Wait(40);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 20);

    actor = Actor_Get(ACTOR_FIRST_OF_PAIR);
    actor->facing = FACING_SOUTHEAST + FACING_STEP;
    actor->scale_x = 0x10000;
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 1);
    Event_Wait(10);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
    Actor_Jump(ACTOR_FIRST_OF_PAIR, 6, 0);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    Actor_FaceDirection(21, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(6, FACING_EAST, 0);
    star = (union FieldObject *)Object_Create(22, actor->x.fixed, actor->y.fixed + PIXELS(8), actor->z.fixed);
    if (star != NULL) {
        sprite = star->actor.sprite;
        sprite->part_count = 0;
        sprite->full_color = 0;
        sprite->palette = 0;
        sprite->priority = 0;
        star->actor.priority_flags &= ~1;
        star->actor.motion_flags = 0;
        star->actor.unknown_5c = 1;
        star->actor.speed = 0x19999;
        star->actor.acceleration = 0xcccc;
        buffer = Heap_Allocate(17, 0x608);
        Item_LoadIcon(220);
        Vram_Load(sprite->vram_block, 128, buffer + 0x400);
        Heap_Release(17);
    }
    Actor_SetSpritePriority(22, 1);
    bird = Actor_Get(22);
    bird->x.fixed = actor->x.fixed;
    bird->y.fixed = PIXELS(32);
    bird->z.fixed = actor->z.fixed;
    bird->motion_flags = 3;
    bird->speed = 0x19999;
    bird->acceleration = 0xcccc;
    bird->scale_x = 0xc000;
    bird->scale_y = 0xc000;
    if (star != NULL) {
        star->actor.motion_flags = 3;
        star->effect.velocity_y = 0x9999;
        star->effect.velocity_x = 0xcccc;
        star->actor.velocity_y = PIXELS(8);
        FieldObject_SetPosition(&star->actor, PIXELS(308), PIXELS(32), PIXELS(164));
    }
    Actor_MoveToAndWait(22, 0x134, 164);
    Actor_SetPosition(22, 0, 0);
    Actor_SetSpritePriority(21, 0);
    if (star != NULL) {
        Audio_PlayCue(0x135);
        Actor_SetSpriteFlags(&star->actor, 0);
        star->actor.velocity_y = PIXELS(4);
        FieldObject_SetPosition(&star->actor, PIXELS(314), PIXELS(32), PIXELS(137));
        FieldObject_CommitPosition(&star->actor);
        Audio_PlayCue(0x135);
        star->actor.sprite->priority = 1;
        star->actor.velocity_y = PIXELS(6);
        FieldObject_SetPosition(&star->actor, PIXELS(285), PIXELS(32), PIXELS(146));
        FieldObject_CommitPosition(&star->actor);
        Audio_PlayCue(0x135);
        star->actor.velocity_y = PIXELS(5);
        FieldObject_SetPosition(&star->actor, PIXELS(300), PIXELS(32), PIXELS(154));
        FieldObject_CommitPosition(&star->actor);
        Task_Wait(6);
        star->actor.x.fixed = 0;
        star->actor.y.fixed = 0;
        star->actor.z.fixed = 0;
        SceneActor_ParkRecord(&star->actor);
    }
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 30);
    Actor_SetSpritePriority(21, 1);
    Actor_Get(21)->priority_flags |= 1;
    Actor_StartRepeatedMotion(IWAN, 2);
    VinasuChojo_ShowMessage(IWAN);
    Actor_RunRepeatedMotion(MEARI, 1);
    Event_ShowMessageAndWait(MEARI, 0, 40);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    Event_Wait(20);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTH + FACING_STEP, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 0);
    Actor_FaceDirection(6, FACING_SOUTHEAST + FACING_STEP, 0);
    VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
    Actor_ShowEmote(21, 0x101, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
    Actor_ShowEmote(GERALD, 0x101, 0);
    Actor_ShowEmote(IWAN, 0x101, 0);
    Actor_ShowEmote(MEARI, 0x101, 60);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 40);
    Event_OpenMessage(GERALD, 0);
    if (Event_ChooseYesNo(0, 0) == 0) {
        Actor_SetAnimation(GERALD, ANIM_NOD);
        i = TRUE;
    } else {
        gEventWork->message++;
        Actor_SetAnimation(GERALD, ANIM_SHAKE_HEAD);
        i = FALSE;
    }
    VinasuChojo_ShowMessage(GERALD);
    if (i) {
        gEventWork->message++;
    }
    Event_Wait(20);

    Actor_SetSpriteFlags(Actor_Get(24), 0);
    Actor_SetChildValue(24, 7);
    actor = Actor_Get(24);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = PIXELS(64);
    actor->x.fixed = PIXELS(304);
    actor->z.fixed = PIXELS(158);
    Actor_SetSpriteFlags(Actor_Get(25), 0);
    Actor_SetChildValue(25, 7);
    actor = Actor_Get(25);
    actor->scale_y = -0x10000;
    actor->scale_x = 0x1999;
    actor->motion_flags = 0;
    actor->y.fixed = PIXELS(96);
    actor->x.fixed = PIXELS(304);
    actor->z.fixed = PIXELS(158);
    Actor_ShowEmote(21, 0x100, 0);
    Actor_ShowEmote(6, 0x100, 0);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 0);
    Actor_FaceDirection(GERALD, FACING_NORTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 0);
    Actor_FaceDirection(21, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(6, FACING_EAST, 0);
    Actor_EnableActionCallback(24, VinasuChojo_BeamOpenAction);
    Actor_EnableActionCallback(25, VinasuChojo_BeamOpenAction);
    Audio_PlayCue(145);
    Work_SetValuesIfNonNegative(0x60000, 0x60000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(16);
    Task_Wait(20);
    ColorBuffer_ApplyTarget(0x7fff, 0);
    ColorBuffer_Interpolate(24);
    Task_Wait(60);
    Task_AddCallback(SceneEffect_SpawnParticlesAboveActor, TASK_PRIORITY_SCENE);
    Audio_PlayCue(141);
    Work_SetValuesIfNonNegative(0x40000, 0x40000, 0x10000);
    ColorBuffer_ApplyTarget(0x4063ff, 0);
    ColorBuffer_Interpolate(120);
    Actor_EnableActionCallback(24, Data_020060ac);
    Actor_EnableActionCallback(25, Data_020060ac);
    Task_Wait(120);
    Work_SetValuesIfNonNegative(0x30000, 0x30000, 0x10000);
    ColorBuffer_ApplyTarget(0x203210, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    Audio_PlayCue(63);
    Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
    ColorBuffer_ApplyTarget(0x10000, 0);
    ColorBuffer_Interpolate(120);
    Task_Wait(120);
    Work_SetValuesIfNonNegative(0x10000, 0x10000, 0x10000);

    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 1);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 0);
    Actor_Get(ACTOR_SECOND_OF_PAIR)->scale_x = 0x10000;
    Actor_Jump(ACTOR_SECOND_OF_PAIR, 4, 40);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    Event_ShowMessageAndWait(ACTOR_SECOND_OF_PAIR, 0, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 0);
    Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(6, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_RunRepeatedMotion(MEARI, 1);
    VinasuChojo_FaceActor(MEARI, FACING_WEST);
    VinasuChojo_ShowMessage(MEARI);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_EAST);
    Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 4);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x103, 20);
    Actor_StartRepeatedMotion(ACTOR_SECOND_OF_PAIR, 2);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    Actor_ShowEmote(GERALD, 0x100, 20);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTHWEST);
    VinasuChojo_ShowMessage(GERALD);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
    Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    Event_ShowMessageAndWait(ACTOR_FIRST_OF_PAIR, 0, 20);
    Actor_RunRepeatedMotion(GERALD, 1);
    VinasuChojo_ShowMessage(GERALD);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(0x2013);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
    Actor_StartRepeatedMotion(GERALD, 1);
    Actor_StartRepeatedMotion(IWAN, 1);
    Actor_RunRepeatedMotion(MEARI, 1);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    VinasuChojo_FaceActor(MEARI, FACING_NORTHWEST);
    Actor_ShowEmote(21, 0x101, 80);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
    Actor_FaceDirection(GERALD, FACING_SOUTHEAST, 0);
    Actor_FaceDirection(IWAN, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHEAST, 40);
    Actor_RunRepeatedMotion(ACTOR_FIRST_OF_PAIR, 1);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
    Actor_FaceDirection(GERALD, FACING_WEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 20);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
    VinasuChojo_ShowMessage(0x2014);
    Actor_SetAnimationAndWait(21, 3);
    Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 60);
    Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_RunRepeatedMotion(ACTOR_SECOND_OF_PAIR, 1);
    VinasuChojo_ShowMessage(0x2013);
    Actor_FaceDirection(21, FACING_EAST, 40);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP, 40);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP);
    VinasuChojo_ShowMessage(0x2014);
    Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 40);
    Actor_FaceDirection(21, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_SetAnimationAndWait(21, 3);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_EAST, 40);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_ShowEmote(ACTOR_SECOND_OF_PAIR, 0x101, 40);
    VinasuChojo_ShowMessage(0x2013);
    VinasuChojo_FaceActor(21, FACING_SOUTHEAST + FACING_STEP);
    Actor_ShowEmote(21, 0x101, 20);
    VinasuChojo_ShowMessage(21);
    Actor_FaceDirection(ACTOR_FIRST_OF_PAIR, FACING_NORTHWEST + FACING_STEP, 20);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 80);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(ACTOR_SECOND_OF_PAIR, 3);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    Actor_StartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 4);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x102, 60);
    Actor_RunRepeatedMotion(21, 1);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_FIRST_OF_PAIR, 0x108, 40);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 4);
    Event_ShowMessageAndWait(0x2014, 0, 40);
    Actor_RunRepeatedMotion(21, 1);
    Event_Wait(20);
    VinasuChojo_ShowMessage(21);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
    Actor_ShowEmote(GERALD, 0x100, 0);
    Actor_ShowEmote(IWAN, 0x100, 0);
    Actor_ShowEmote(MEARI, 0x100, 0);
    Actor_FaceDirection(IWAN, FACING_NORTHWEST, 0);
    Actor_FaceDirection(MEARI, FACING_NORTHWEST, 20);
    Actor_FaceDirection(ACTOR_SECOND_OF_PAIR, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_SetAnimation(ACTOR_SECOND_OF_PAIR, 3);
    Actor_SetAnimationAndWait(ACTOR_FIRST_OF_PAIR, 3);
    Actor_FaceDirection(21, FACING_NORTHWEST + FACING_STEP, 20);
    VinasuChojo_ShowMessage(0xa015);
    Actor_SetSpeed(6, 0x10000, 0x8000);
    Actor_SetSpeed(21, 0x10000, 0x8000);
    Actor_EnableActionCallback(21, Data_0200622c);
    Event_Wait(20);
    Actor_EnableActionCallback(6, Data_0200622c);
    VinasuChojo_FaceActor(GERALD, FACING_SOUTHWEST);
    Actor_StartRepeatedMotion(GERALD, 2);
    VinasuChojo_ShowMessage(GERALD);
    Audio_PlayCue(17);
    ColorBuffer_ApplyTarget(0x40250d, 1);
    ColorBuffer_Interpolate(40);
    VinasuChojo_FaceActor(ACTOR_FIRST_OF_PAIR, FACING_NORTH + FACING_STEP);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
    Actor_FaceDirection(IWAN, FACING_WEST, 0);
    Actor_FaceDirection(MEARI, FACING_WEST, 0);
    VinasuChojo_FlashScreen();
    Actor_SetChildValue(ACTOR_FIRST_OF_PAIR, 7);
    Actor_SetChildValue(ACTOR_SECOND_OF_PAIR, 7);
    Event_Wait(20);
    Actor_SetChildValue(ACTOR_FIRST_OF_PAIR, 0x100);
    Actor_SetChildValue(ACTOR_SECOND_OF_PAIR, 0x100);
    Event_Wait(20);
    VinasuChojo_FlashScreen();
    Actor_Jump(MEARI, 2, 20);
    VinasuChojo_ShowMessage(MEARI);
    VinasuChojo_FaceActor(ACTOR_SECOND_OF_PAIR, FACING_EAST);
    VinasuChojo_ShowMessage(ACTOR_SECOND_OF_PAIR);
    VinasuChojo_FlashScreen();

    second = Actor_Get(ACTOR_SECOND_OF_PAIR);
    second_burst.palette = 7;
    second_burst.update = Effect_MoveWithDrag;
    second_burst.start_scale_x = 0x10000;
    second_burst.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        VinasuChojo_AimBurst(velocity, i << 12);
        Effect_Spawn(second->x.fixed, second->y.fixed, second->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &second_burst);
    }
    Audio_PlayCue(212);
    Task_Wait(6);
    VinasuChojo_FlashScreen();
    first = Actor_Get(ACTOR_FIRST_OF_PAIR);
    first_burst.palette = 7;
    first_burst.update = Effect_MoveWithDrag;
    first_burst.start_scale_x = 0x10000;
    first_burst.start_scale_y = 0x10000;
    for (i = 0; i <= 16; i++) {
        VinasuChojo_AimBurst(velocity, i << 12);
        Effect_Spawn(first->x.fixed, first->y.fixed, first->z.fixed, velocity[0], velocity[1],
                     velocity[2], EFFECT_USE_UPDATE | EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                     &first_burst);
    }
    Audio_PlayCue(212);
    Actor_Jump(IWAN, 6, 20);
    Audio_PlayCue(54);
    VinasuChojo_ShowMessage(IWAN);
    Actor_SetAnimation(ACTOR_FIRST_OF_PAIR, 4);
    VinasuChojo_ShowMessage(ACTOR_FIRST_OF_PAIR);
    VinasuChojo_FlashScreen();
    Actor_SetSpeed(ACTOR_FIRST_OF_PAIR, 0x3333, 0x1999);
    Actor_SetSpeed(ACTOR_SECOND_OF_PAIR, 0x3333, 0x1999);
    Actor_Get(ACTOR_FIRST_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_Get(ACTOR_SECOND_OF_PAIR)->steering_flags &= ~ACTOR_FACES_MOTION;
    Actor_SetDestination(ACTOR_FIRST_OF_PAIR, 0x126, 196);
    Actor_SetDestination(ACTOR_SECOND_OF_PAIR, 0x126, 196);
    Task_AddCallback(VinasuChojo_UpdateBeamActors, TASK_PRIORITY_SCENE);
    Actor_StartRepeatedMotion(GERALD, 2);
    Event_ShowMessage(GERALD, 0);
    GameFlag_Set(FLAG_PAIR_SLIDING);
    Event_ShowMessage(IWAN, 0);
    GameFlag_Set(FLAG_PAIR_GROWING);
    VinasuChojo_FlashScreen();
    Event_Wait(20);
    VinasuChojo_FlashScreen();
    Event_Wait(20);
    /* FAKEMATCH: the one-pass block keeps the scene id's load after this store, where the reference has it. */
    do {
        gGameState.battle_start = 3;
    } while (0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_VinasuChojo, 3);
    Event_SetDefeatDestination((s32)&SceneId_VinasuChojo, 9);
    BattleFx_SetWeightedResult(98, 0);
    Owner_RefreshRatiosOnFlag();
    GameFlag_Set(0x351);
}
