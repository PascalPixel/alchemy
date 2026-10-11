#include "battle.h"
#include "field.h"
#include "menu.h"
#include "system.h"
#include "fieldobj.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "create_configured_overlay_object.h"
#include "debug.h"
#include "game.h"
#include "graphics.h"
#include "minigame.h"
#include "sound.h"
#include "edition.h"
#include "call.h"
#include "btlsys.h"
#include "item.h"
#include "inventory.h"
#include "text/msg_ids.h"

void GomaIriguchi_GiveShamansRod();
void GomaIriguchi_RunSpinningLeap();
void GomaIriguchi_SetEntranceFlag();
void OverlayObject_TurnStateByEighth();
void OverlayObject_WaitForHeight();

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void SetEffectRecordMode(struct FieldActor *work, s32 mode)
{
    work->sprite->priority = mode;
}

void *OverlayObject_PrepareObject(s32 first, s32 second, s32 third, s32 fourth)
{
    struct FieldActor *obj;
    struct FieldSprite *rec;

    obj = Engine_ObjectCreate(fourth, first, second, third);
    if (obj != NULL) {
        rec = obj->sprite;
        rec->priority = 0;
        obj->motion_flags = 0;
        obj->collision_flags = 8;
        Engine_ActorSetSpriteFlags(obj, 0);
        ObjectGroup_SetChildValue(obj, 0xE);
        Engine_ObjectSetBlendMode(obj, 1);
        return obj;
    }
    return NULL;
}

void *OverlayObject_CreateConfiguredObject(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
#include "create_configured_overlay_object_body.inc"
}

struct EffectRecord {
    u8 pad[9];
    u8 flags_lo : 2;
    u8 mode : 2;
    u8 flags_hi : 4;
};

void BattleFx_PlayQueuedSound();

void BattleFx_SetQueuedSoundAndPlay();
void Event_PrepareObjectAndApplyValue();

extern u8 *gEffectWork;
void BattleFx_LoadActionEffectResources();
void BattleFx_SetupObjectPair();
void Field_DispatchTypeHandler();
void EventObject_Initialize();
void EffectRuntime_StopCurrentObject();
void Effect_Spawn(s32 x, s32 y, s32 z, s32 dx, s32 dy, s32 dz, s32 lift, void *params);

struct Actor {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 pad14[10];
    s16 angle;
    u8 pad20[2];
    u8 flags34;
    u8 pad23[5];
    s32 speed;
    u8 pad2c[12];
    s32 hover;
    u8 pad3c[12];
    s32 accel;
    u8 pad4c[4];
    struct Actor *sprite;
    u8 pad54;
    u8 layer;
    u8 pad56[22];
    s32 callback;
};

struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectParams {
    s32 count;
    s32 kind;
    s32 spread;
    s32 rise;
    s32 rise2;
    s32 spin;
    u16 tile;
    u16 pad1a;
    u8 pad1c[12];
};

s32 Inventory_AddItem(s32 group, s32 value);

struct Member_387 {
    u8 pad[216];
    u16 items[15];
};

struct Item_387 {
    u8 pad[2];
    u16 flags;
    u8 pad4[8];
    u8 equippable;
};

s32 Inventory_AddItem(s32 member, s32 item);

void FieldScene_RequestAndWaitFrames(s32 selector, s32 frames)
{
    Engine_EventShowMessage(selector, 0);
    Engine_EventWait(frames);
}

void *GomaIriguchi_GetEntrances(void)
{
    return GomaIriguchi_SceneTableA;
}

int SceneData_ReturnZero(void)
{
    return 0;
}

void *GomaIriguchi_GetExits(void)
{
    return GomaIriguchi_SceneTableB;
}

void *GomaIriguchi_GetPlacements(void)
{
    return GomaIriguchi_SceneTableC;
}

void FieldScene_RunOpeningAuxiliarySequence(void)
{
    struct FieldActor *record;
    s32 v3;

    record = Object_GetById(9);
    v3 = record->x.fixed / 0x100000;
    GameFlag_Clear(0x861);
    GameFlag_Clear(0x862);
    if (v3 == 15) {
        Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
    } else if (v3 == 16) {
        Map_CopyCellAttributes(48, 18, 1, 2, v3, 18);
        GameFlag_Set(0x861);
    } else {
        Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
        GameFlag_Set(0x862);
    }
}

void FieldScene_RunScene387SequenceC(void)
{
    struct FieldActor *actor;
    s32 tile_x;

    actor = Object_GetById(10);
    tile_x = actor->x.fixed / 0x100000;
    if (tile_x == 23) {
        Engine_EventWait(10);
        Object_GetById(10)->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
        Object_GetById(10)->motion_flags = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(10), 0);
        Engine_MapCopyCellAttributes(54, 17, 1, 1, tile_x, 17);
        Engine_GameFlagSet(0x863);
    }
}

void FieldScene_RunScene387SequenceD(void)
{
    struct EventWork *p5;
    s32 v5;

    p5 = gEventWork;
    Engine_EventBegin();
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 8);
    Engine_EventWait(20);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x3333, 0x1999);
    Actor_SetSpeed(9, 0x3333, 0x1999);
    Event_PlayCue(185);
    v5 = (11 - (p5->touched_trigger << 1)) << 4;
    Actor_SetDestinationOffset(ACTOR_PARTY_LEADER, v5, 0);
    Actor_SetDestinationOffset(9, v5, 0);
    Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
    Engine_ActorWaitForMove(9);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
    FieldScene_RunOpeningAuxiliarySequence();
    BattleFx_PlayQueuedSound();
    Engine_EventEnd();
}

void Resource387_NoOpCallbackA(void)
{
}

void Resource387_NoOpCallbackB(void)
{
}

void FieldScene_RunStepWithValue866(void)
{
    Engine_EventBegin();
    Engine_GameFlagSet(0x866);
    Engine_EventEnd();
}

void *GomaIriguchi_GetEvents(void)
{
    return GomaIriguchi_SceneTableD;
}

s32 GomaIriguchi_RestoreEntryState(void)
{
    u32 i;
    u8 *record;
    s32 v5;

    Engine_GameFlagSet(0x144);
    Engine_EventWait(10);
    BattleFx_SetQueuedSoundAndPlay(170);
    Engine_ActorSetAnimation(11, 2);
    Object_GetById(11)->priority_flags = ACTOR_PRIORITY_UNDERFOOT;
    {
        struct FieldActor *record = Object_GetById(8);

        record->collision_flags = 16 | record->collision_flags;
    }
    {
        struct FieldActor *record = Object_GetById(15);

        record->collision_flags = 8 | record->collision_flags;
    }
    if (Engine_GameFlagIsSet(0x865) != 0) {
        Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
    }
    if (Engine_GameFlagIsSet(0x860) != 0) {
        Engine_ActorSetPosition(8, 0x880000, 0xc40000);
        Object_GetById(8)->priority_flags |= 2;
        v5 = 12;
        Engine_ActorSetAnimation(8, 2);
        Engine_MapCopyCellAttributes(39, 12, 3, 1, 8, v5);
        Engine_MapCopyCellAttributes(43, 11, 3, 1, v5, 11);
    }
    if (Engine_GameFlagIsSet(0x861) != 0) {
        Actor_SetPosition(9, 0x1080000, 0x1380000);
        Map_CopyCellAttributes(48, 18, 1, 2, 16, 18);
    } else {
        if (Engine_GameFlagIsSet(0x862) != 0) {
            Actor_SetPosition(9, 0x1180000, 0x1380000);
            Map_CopyCellAttributes(47, 18, 1, 2, 16, 18);
        }
    }
    if (Engine_GameFlagIsSet(0x863) != 0) {
        Engine_ActorSetPosition(10, 0x1780000, 0x1180000);
        ((struct Flags35 *)Object_GetById(10))->flags = 2;
        v5 = 0;
        Object_GetById(10)->motion_flags = v5;
        record = (s32)Object_GetById(10);
        Engine_ActorSetSpriteFlags((s32)record, 0);
        Map_CopyCellAttributes(54, 17, 1, 1, 23, 17);
    }
    return 0;
}

void FieldScene_RunScene387SequenceA(void)
{
    u32 i;
    struct FieldActor *record;

#if EDITION_INTERNATIONAL
    BattleFx_PlayQueuedSound();
#endif
    Engine_EventBegin();
    Engine_EventWait(30);
    Engine_EventSetMessage((s32)MsgGomaNoUsePsynergy);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
    Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xe000, 20);
    record = Object_GetById(ACTOR_PARTY_LEADER);
    if (record != 0) {
        Actor_SetPosition(GERALD, record->x.fixed, record->z.fixed);
    }
    Actor_WalkToAndWait(GERALD, 0x108, 168);
    Actor_FaceDirection(GERALD, 0x6000, 20);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(GERALD, 4);
    Engine_EventWait(20);
    Event_ShowMessageAndWait(GERALD, 0, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
    Engine_EventWait(20);
    if (GameFlag_IsSet(0x855) == 0) {
        Engine_ActorSetAnimation(GERALD, 2);
        record = Object_GetById(ACTOR_PARTY_LEADER);
        if (record != 0) {
            Actor_SetDestination(GERALD, record->x.part.pixel, record->z.part.pixel);
        }
        Engine_ActorWaitForMove(GERALD);
        Actor_SetPosition(GERALD, 0, 0);
        Engine_EventEnd();
    } else {
        Actor_SetPosition(IWAN, 0x1680000, 0xf80000);
        Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
        Actor_WalkToAndWait(IWAN, 0x110, 248);
        Actor_WalkToAndWait(IWAN, 0x110, 208);
        Actor_FaceDirection(IWAN, 0xa000, 20);
        Engine_ActorRunRepeatedMotion(IWAN, 2);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(IWAN, 0, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
        Actor_FaceDirection(GERALD, 0x4000, 0);
        Engine_EventWait(20);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(GERALD, 0x100, 60);
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        Actor_WalkToAndWait(IWAN, 0x108, 200);
        Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 168);
        Actor_WalkToAndWait(IWAN, 248, 184);
        Engine_ActorWaitForMove(ACTOR_PARTY_LEADER);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
        Actor_FaceDirection(GERALD, 0x6000, 0);
        Actor_WalkToAndWait(IWAN, 232, 184);
        Engine_EventWait(20);
        Actor_ShowEmote(IWAN, 0x105, 60);
        Actor_FaceDirection(IWAN, 0xe000, 20);
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(IWAN, 0, 20);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        Engine_EventWait(20);
        Actor_FaceDirection(IWAN, 0x8000, 20);
        Event_ShowMessageAndWait(IWAN, 0, 120);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
        Actor_ShowEmote(GERALD, 0x105, 60);
        Engine_ActorFaceEachOther(ACTOR_PARTY_LEADER, GERALD, 0);
        Engine_EventWait(60);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x6000, 0);
        Actor_FaceDirection(GERALD, 0x6000, 0);
        Engine_EventWait(60);
        Actor_ShowEmote(IWAN, 0x106, 0);
        Engine_ActorStartRepeatedMotion(IWAN, 1);
        Engine_EventWait(30);
        Event_ShowMessageAndWait(IWAN, 0, 30);
        Actor_FaceDirection(IWAN, 0xe000, 20);
        Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(20);
        Event_ShowMessageAndWait(IWAN, 0, 20);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        Engine_EventWait(20);
        Actor_FaceDirection(IWAN, 0x8000, 20);
        GomaIriguchi_SetEntranceFlag();
        Engine_ActorSetAnimation(IWAN, 1);
        Engine_EventWait(20);
#if EDITION_INTERNATIONAL
        BattleFx_PlayQueuedSound();
#endif
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x100, 0);
        Actor_ShowEmote(GERALD, 0x100, 60);
        Engine_ActorJump(GERALD, 2, 0);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(1, 20);
        Actor_WalkToAndWait(IWAN, 0x108, 184);
        Engine_EventWait(10);
        Actor_FaceActor(IWAN, GERALD, 0);
        Actor_FaceActor(GERALD, IWAN, 0);
        Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x105, 0);
        Actor_ShowEmote(GERALD, 0x105, 60);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(GERALD, 0x101, 0);
        Engine_EventWait(60);
        Actor_FaceDirection(GERALD, 0x8000, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Engine_EventWait(60);
        Actor_FaceDirection(GERALD, 0x4000, 0);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x2000, 0);
        Engine_EventWait(10);
        FieldScene_RequestAndWaitFrames(1, 20);
        Actor_ShowEmote(IWAN, 0x105, 0);
        Engine_EventWait(60);
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
        Actor_FaceActor(GERALD, IWAN, 0);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x102, 0);
        Actor_ShowEmote(GERALD, 0x102, 0);
        Engine_EventWait(60);
        Engine_ActorRunRepeatedMotion(IWAN, 2);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 0);
        Actor_ShowEmote(GERALD, 0x101, 0);
        Engine_EventWait(80);
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
        Engine_ActorStartRepeatedMotion(GERALD, 1);
        Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
        Actor_SetAttachedEffect(GERALD, 0x102);
        Engine_EventWait(60);
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0, 0);
        Actor_FaceDirection(GERALD, 0x8000, 0);
        Engine_EventWait(80);
        Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
        Actor_FaceActor(GERALD, IWAN, 0);
        Engine_EventWait(30);
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 30);
        Engine_ActorSetAnimationAndWait(IWAN, 4);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
        Engine_ActorRunRepeatedMotion(GERALD, 2);
        Engine_EventWait(20);
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 40);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimationAndWait(GERALD, 3);
        Engine_EventWait(20);
        Event_PrepareObjectAndApplyValue(2, 1);
        Engine_EventWait(60);
        GomaIriguchi_GiveShamansRod();
        Engine_ActorRunRepeatedMotion(IWAN, 1);
        Engine_EventWait(20);
        Actor_WalkToAndWait(IWAN, 248, 184);
        Engine_EventWait(20);
        FieldScene_RequestAndWaitFrames(2, 20);
        Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
        Actor_FaceDirection(GERALD, 0x8000, 0);
        Engine_EventWait(120);
        FieldScene_RequestAndWaitFrames(2, 30);
        Actor_FaceActor(ACTOR_PARTY_LEADER, IWAN, 0);
        Actor_FaceActor(GERALD, IWAN, 0);
        Actor_FaceActor(IWAN, ACTOR_PARTY_LEADER, 0);
        Engine_EventWait(20);
        Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
        Engine_ActorSetAnimation(GERALD, 3);
        Engine_ActorSetAnimationAndWait(IWAN, 3);
        Engine_EventWait(50);
        Actor_SetSpeed(GERALD, 0xcccc, 0x6666);
        Actor_SetSpeed(IWAN, 0xcccc, 0x6666);
        Actor_WalkTo(GERALD, 248, 168);
        Actor_WalkToAndWait(IWAN, 248, 168);
        Actor_SetPosition(IWAN, 0, 0);
        Engine_ActorWaitForMove(GERALD);
        Actor_SetPosition(GERALD, 0, 0);
        Map_CopyCellAttributes(74, 11, 1, 1, 73, 11);
        GameFlag_Set(0x865);
        Engine_EventEnd();
    }
}

void Overlay387_ConfigureActorEightAtDepth(void)
{
    s32 depth;
    s32 span;
    struct OverlayActorState *state;

    Engine_EventBegin();
    depth = ((struct OverlayActorPosition *)Object_GetById(8))->depth_fixed >> 20;
    if (depth == 11) {
        GomaIriguchi_RunSpinningLeap(8);
        state = Object_GetById(8);
        state->flags |= 2;
        span = 12;
        Engine_MapCopyCellAttributes(39, 12, 3, 1, 8, span);
        Engine_MapCopyCellAttributes(43, 11, 3, 1, span, depth);
        Engine_GameFlagSet(2144);
    }
    Engine_EventEnd();
}

void GomaIriguchi_SetEntranceFlag(void)
{

    u8 *p5;

    p5 = gEffectWork;
    BattleFx_LoadActionEffectResources(78, 1);
    BattleFx_SetupObjectPair(2, 15);
    {
        u8 *f = (u8 *)((s32)p5 + 0x71c);
        s32 t = 8;

        t |= *f;
        *f = t;
    }
    EventObject_Initialize();
    Field_DispatchTypeHandler(1);
    EffectRuntime_StopCurrentObject();
}

void OverlayObject_TurnStateByEighth(u8 *obj)
{
    u8 *state = *(u8 **)(obj + 80);
    s32 v = *(u16 *)(state + 30) - 0x800;

    *(u16 *)(state + 30) = v;
}

void OverlayObject_WaitForHeight(u8 *obj, s32 height)
{
    s32 cnt = 60;
    while (cnt != 0) {
        Engine_TaskWait(1);
        cnt--;
        if (*(s32 *)(obj + 12) <= height)
            break;
    }
}

void GomaIriguchi_RunSpinningLeap(s32 id)
{
    struct Vec dir;
    struct EffectParams params;
    struct Vec *v;
    struct EffectParams *p;
    struct Actor *actor;
    u32 i;
    s32 x;
    s32 z;

    actor = (struct Actor *)Object_GetById(id);
    actor->layer = 0;
    for (i = 0; i <= 17; i++) {
        Engine_TaskWait(1);
        actor->sprite->angle += -256;
        actor->x -= Engine_MathCos((u16)actor->sprite->angle) / 2;
        actor->hover = 0x80000000;
    }
    actor->callback = (s32)OverlayObject_TurnStateByEighth;
    Actor_SetSpeed(id, 0x30000, 0x18000);
    Engine_ActorSetDestination(id, 160, 192);
    actor->accel = 0xcccc;
    actor->layer = 3;
    actor->flags34 = 0;
    Engine_ActorWaitForMove(id);
    OverlayObject_WaitForHeight((s32)actor, 0x200000);
    Engine_AudioPlayCue(188);
    Work_SetValuesIfNonNegative(0x50000, 0x50000, 0x10000);
    Engine_AudioPlayCue(141);
    Work_SetValuesIfNonNegative(-1, -1, 0xe666);
    for (i = 0; i <= 16; i++) {
        v = &dir;
        v->x = Engine_MathCos(i << 12);
        v->y = 0;
        v->z = Engine_MathSin(i << 12);
        v->x -= v->x / 4;
        v->z -= v->z / 2;
        Effect_Spawn(actor->x, actor->y, actor->z, v->x, v->y, v->z, 0, NULL);
    }
    actor->speed = 0x50000;
    Engine_ActorSetDestination(id, 139, 196);
    Engine_ActorWaitForMove(id);
    OverlayObject_WaitForHeight((s32)actor, 0x200000);
    actor->callback = 0;
    actor->sprite->angle = 0x1000;
    p = &params;
    p->tile = 214;
    p->spread = 0x8000;
    p->rise = 0xcccc;
    p->rise2 = 0x18000;
    p->spin = 0x13333;
    Effect_Spawn(actor->x, actor->y, actor->z, 0, 0, 0, 0x1c0000, p);
    Engine_AudioPlayCue(154);
    Engine_ActorSetAnimation(id, 3);
    Engine_MapWaitWorkValuesBelow256();
}

void SceneActor_ApplyValueAndMatchingSlots(s32 group, s32 value)
{
    u8 *work = UnitGet(group);
    s32 i;
    Inventory_AddItem(group, value);
    for (i = 0; i < 15; i++) {
        if (*(u16 *)(work + 216 + i * 2) == value)
            Inventory_Equip(group, i);
    }
}

void GomaIriguchi_GiveShamansRod(void)
{
    struct Member_387 *member;
    struct Item_387 *item;
    s32 tries;
    s32 i;
    s32 id;

    id = 65;
    member = (struct Member_387 *)UnitGet(2);
    tries = 0;
retry:
    if (++tries > 1000) {
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
        for (i = 0; i <= 14; i++) {
            item = Item_Get(member->items[i]);
            if (((u8 *)item)[3] != 8) {
                Inventory_Discard(2, i);
                break;
            }
        }
        if (i == 15)
#endif
        member->items[14] = 0;
    }
    if (Inventory_AddItem(2, id) == -1) {
        for (i = 0; i <= 14; i++) {
            item = Item_Get(member->items[i]);
            if (((u8 *)item)[2] == 1) {
                Inventory_Discard(2, i);
                goto retry;
            }
        }
        for (i = 0; i <= 14; i++) {
            item = Item_Get(member->items[i]);
            if ((item->flags & 0x8ff) == 0 && item->equippable == 1) {
                Inventory_Discard(2, i);
                goto retry;
            }
        }
        goto retry;
    }
    for (i = 0; i <= 14; i++) {
        if (member->items[i] == id)
            Inventory_Equip(2, i);
    }
}
