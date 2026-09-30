/* Object motion, scene tables and the actors' setup. */
#include "CHOJO.H"
#include "IWRAM_CALL.H"
#include "TYPES.H"
#include "CALL.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"
#include "FIELD_EFFECT.H"

extern u8 VinasuChojo_PairActionScript[];
void VinasuChojo_ShowMessage();
void FieldScene_RunScene3c9_02001280();
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

/* FAKEMATCH: calls cast through unprototyped function pointers load their
 * arguments in the reference's order.
 * FAKEMATCH: a one-halfword struct keeps the zero in a pool word held in
 * r8, as the reference does. */
struct Half {
    u16 v;
};

extern u8 MsgVinasuRobin[];
extern u8 MsgVinasuRobinHandedOverShamansRod[];
extern const s32 SceneAction_EntryGroup[];
extern const s32 SceneAction_EntryPair[];
void BattleFx_SetWeightedResult();
void Event_SetPair1d4(u16 first, u16 second);
void Party_SetFields1ceAnd1d0(u16 first, u16 second);
s32 PartyInventory_Remove();
s32 PartyInventory_FindOwner();
void FieldScene_ForwardValue81fc();
s32 Scheduler_RemoveCallback();
void FieldScene_RunStep6(void);

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

LAYOUT_OFFSET_GUARD(PairSprite_Detail, struct PairSprite, detail, 0x28);
LAYOUT_OFFSET_GUARD(PairObject_Parent, union PairObject, link.parent, 0x68);
extern struct PairWork *Data_03001f30;

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

extern struct WorldMapVramBlock ResourceTableEntries[];
void Resource_ResetEntry(s32 block);
s32 AnimationObjects_SelectAnimation(struct FieldSprite *sprite, s32 animation);

/* The OAM view with attribute 1 ending in the two-bit size field. */
struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

void VinasuChojo_FaceActor(s32 actor, s32 facing);

s32 OverlayObject_StepScaleByCounter(Spr *s)
{
    s16 *p = (s16 *)((u8 *)s + 100);

    switch (*p) {
    case 6:
        s->unk18 += -0x2000;
        s->unk1c += 0x1000;
        break;
    case 4:
        s->unk18 += 0x1000;
        s->unk1c += -0x800;
        break;
    case 2:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        break;
    case 0:
        s->unk18 += 0x800;
        s->unk1c += -0x400;
        *p = IwramUnsignedRemainder(Engine_RandomNext(), 80) + 80;
        break;
    }
    {
        s32 t = s->unk64;
        t = t - 1;
        s->unk64 = t;
    }
    return 1;
}

/*
 * Moves an effect like BattleFx_UpdateObjectMotionScaleAndLinkedAngle, and slows its horizontal velocity by a
 * twenty-second across and a twentieth in depth each frame.
 */
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

void OverlayObject_DecayRecordField1e(Spr_02000400 *s)
{
    Obj_02000400 *o = s->obj;
    u16 h = o->unk1e;

    if ((s32)((h + 0xffff) << 16) < 0) {
        {
            s32 t = h + (s32)0xfffff600;
            o->unk1e = t;
        }
    }
}

s32 OverlayObject_UpdateHeadingTimer(Spr_02000424 *s)
{
    u16 *q = (u16 *)((u8 *)s + 0x66);
    s32 c = *q;
    s16 v = *(s16 *)q;

    if (v == 0) {
        {
            s32 t = ((u32)(Random_Next() << 15)) >> 16;
            s->unk06 = s->unk06 + t;
        }
        {
            s32 n = ((u32)(Random_Next() * 80)) >> 16;
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

/* Contiguous unnamed leaf-owner run for resource_3c9. */
s32 SceneData_GetTablee3d4(void)
{
    return (s32)Data_0200e3d4;
}

s32 SceneData_ReturnZero(void)
{
    return 0;
}

s32 SceneData_GetTablee464(void)
{
    return (s32)Data_0200e464;
}

s32 SceneData_GetTablee478(void)
{
    return (s32)Data_0200e478;
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
    u8 *work = *(u8 **)&gEventWork;
    Spr_020004bc **p;
    s32 best = 0;
    Spr_020004bc *ref;
    s32 limit;
    u32 i;

    limit = 640;
    ref = Actor_Get(ACTOR_PARTY_LEADER);
    i = 8;
    p = (Spr_020004bc **)(work + 0x34);
    do {
        Spr_020004bc *spr = *p++;
        if (spr != 0) {
            if (*spr->obj->unk28 == 0xf2) {
                s32 dist = MeasureFixedPointPositionDistance((u8 *)ref + 8, (u8 *)spr + 8);
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
    struct SceneActor *actor;
    s32 position[3];
    s32 angle;
    u8 flags;

    actor = Actor_Get(ACTOR_PARTY_LEADER);
    angle = (actor->angle + 0x1000) & 0xe000;
    flags = actor->flags55;
    position[0] = (actor->x & 0xfff00000) + 0x80000;
    position[1] = actor->y;
    position[2] = (actor->z & 0xfff00000) + 0x80000;
    Vector_AddPolarOffset(0x200000, angle, position);
    if (Object_CheckMovementCollision(actor, position) == 0) {
        GameFlag_Clear(592);
        SceneActor_SetByte55ForActorZeroAnd12To17();
        Object_SetAnimation(actor, 6);
        Task_Wait(6);
        Object_SetAnimation(actor, 7);
        actor->motion30 = 0x30000;
        actor->motion34 = 0x20000;
        Audio_PlayCue(152);
        actor->motion28 = 0x40000;
        actor->flags55 &= 0x7e;
        Actor_SetSpriteFlags(actor, 0);
        Actor_MoveToAndWait(ACTOR_PARTY_LEADER, (s16)(position[0] >> 16),
                         (s16)(position[2] >> 16));
        Object_SetAnimation(actor, 6);
        Actor_SetSpriteFlags(actor, 1);
        actor->flags55 = flags;
    }
}

void SceneEffect_SpawnAndBobWithActorZero(void)
{
    Spr_020005ec *a = Actor_Get(ACTOR_PARTY_LEADER);
    Spr_020005ec *b;
    Spr_020005ec *r;
    s32 k;

    Event_Begin();
    r = SceneActor_FindNearestSlotOfKindF2();
    Data_0200e6e8 = r;
    if (r != 0) {
        GameFlag_Set(592);
        b = Actor_Get(Data_0200e6e8);
        b->unk55 = 0;
        a->unk55 &= 0xfe;
        b->unk0c += (s32)0xfffd0000;
        a->unk0c += (s32)0xfffd0000;
        a->unk14 += (s32)0xfffd0000;
        Task_Wait(2);
        b->unk0c += (s32)0xfffe0000;
        a->unk0c += (s32)0xfffe0000;
        a->unk14 += (s32)0xfffe0000;
        Task_Wait(10);
        k = 0x20000;
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Task_Wait(4);
        b->unk0c += k;
        a->unk0c += k;
        a->unk14 += k;
        Task_Wait(4);
        b->unk0c += 0x10000;
        a->unk0c += 0x10000;
        a->unk14 += 0x10000;
    }
    Event_End();
}

void SceneActor_SetByte55ForActorZeroAnd12To17(void)
{
    s32 val;

    *((u8 *)Object_GetById(0) + 0x55) = 3;
    val = 4;
    *((u8 *)Object_GetById(12) + 0x55) = val;
    *((u8 *)Object_GetById(13) + 0x55) = val;
    *((u8 *)Object_GetById(14) + 0x55) = val;
    *((u8 *)Object_GetById(15) + 0x55) = val;
    *((u8 *)Object_GetById(16) + 0x55) = val;
    *((u8 *)Object_GetById(17) + 0x55) = val;
}

s32 SceneData_GetTableE6ec(void)
{
    return (s32)Data_0200e6ec;
}

s32 FieldScene_InitActorsAndDispatchBySubstate(void)
{
    s32 v;
    u32 i;
    s32 z;
    s16 mode;
    Spr_0200071c *obj;

    GameFlag_Set(324);
    Task_Wait(1);
    GameFlag_Set(272);
    Actor_SetSpriteFlags(Actor_Get(8), 0);
    Actor_SetSpriteFlags(Actor_Get(9), 0);
    Actor_SetSpriteFlags(Actor_Get(10), 0);
    Actor_SetSpriteFlags(Actor_Get(11), 0);
    v = (s32)0xffff0000;
    Object_GetById(10)->scale_x = v;
    Object_GetById(11)->scale_x = v;
    i = 12;
    z = 0;
    do {
        obj = Actor_Get(i);
        Actor_SetSpriteFlags(Actor_Get(i), z);
        Actor_SetSpritePriority(i, 1);
        obj->unk55 = 4;
        obj->unk23 |= 2;
        obj->unk0c = 0x8000;
        i++;
    } while (i <= 17);

    mode = gGameState.entrance;

    switch (mode) {
    case 1:
        if (GameFlag_IsSet(0x109) == 0) {
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
        FieldScene_RunScene3c9_02003924();
        break;
    case 9:
        Event_Begin();
        if (GameFlag_IsSet(0x345) != 0) {
            Inventory_AddItem(0, 65);
        } else if (GameFlag_IsSet(0x346) != 0) {
            Inventory_AddItem(1, 65);
        } else if (GameFlag_IsSet(0x347) != 0) {
            Inventory_AddItem(2, 65);
        } else {
            Inventory_AddItem(3, 65);
        }
        Event_RequestExit(9);
        break;
    }

    if (GameFlag_IsSet(0x109) != 0) {
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
    Event_ShowMessage(speaker, 0);
    Event_Wait(10);
}

void VinasuChojo_FaceActor(s32 actor, s32 facing)
{
    Actor_FaceDirection(actor, facing, 10);
}

void Scene_RunScriptedActorPresentation(void)
{
    u32 i;
    struct Half zero;
    u8 *rec7;
    u8 *rec8;
    u8 *record;
    s32 none;
    s32 v5;
    s32 base6_4013;
    s32 base5_2014;
    s32 v6;
    s32 base6_8015;
    s32 base5_a014;
    s32 work;
    s32 base5_8001;
    s32 pairScript;

    rec7 = (u8 *)Object_GetById(18);
    Engine_EventBegin();
    FieldScene_RunScene3c9_02001280(1, 0);
    FieldScene_RunScene3c9_02001280(2, 0);
    FieldScene_RunScene3c9_02001280(3, 0);
    Engine_CameraMoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    none = 0;
    rec7[85] = none;
    {
        /* FAKEMATCH: the volatile read keeps the flag byte's load in place. */
        u8 value = *(volatile u8 *)&rec7[35];
    
        rec7[35] = (u8)(value | 2);
    }
    record = (u8 *)Object_GetById(18);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Engine_ActorSetSpritePriority(18, 1);
    Engine_ActorSetPosition(18, 0x2440000, 0x1520000);
    rec8 = (u8 *)Object_GetById(0);
    rec8[85] = none;
    Engine_ActorSetSpritePriority(0, 1);
    Engine_ActorSetPosition(0, 0x2450000, 0x1200000);
    Engine_TaskWait(1);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 18, 0x10000, 0x8000);
    Call3(Engine_ActorSetSpeed, 0, 0x10000, 0x8000);
    *(u8 *)((u8 *)Object_GetById(0) + 90) &= 254;
    Call3(Engine_ActorSetDestination, 18, 0x244, 221);
    Call3(Engine_ActorMoveToAndWait, 0, 0x245, 171);
    Call3(Engine_ActorSetDestination, 18, 0x212, 211);
    Call3(Engine_ActorMoveToAndWait, 0, 0x213, 161);
    Call3(Engine_ActorSetDestination, 18, 0x208, 191);
    Engine_ActorMoveToAndWait(0, 0x209, 141);
    record = (u8 *)Object_GetById(18);
    Engine_ActorSetSpriteFlags((s32)record, 1);
    Call3(Engine_ActorSetDestination, 18, 0x203, 171);
    Engine_ActorMoveToAndWait(0, 0x204, 121);
    Engine_AudioPlayCue(0x120);
    Engine_EventWait(20);
    Engine_ActorSetAnimation(0, 6);
    ObjectMotion_WaitForAnimationChange(0);
    *(s32 *)((s32)rec8 + 8) = 0x2040000;
    *(s32 *)((s32)rec8 + 12) = 0x80000;
    *(s32 *)((s32)rec8 + 16) = 0x940000;
    {
        s32 shown = 0x8000;
    
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    rec8[85] = 3;
    zero.v = 0;
    Engine_AudioPlayCue(152);
    *(s32 *)((s32)rec8 + 40) = 0x40000;
    Engine_AudioPlayCue(152);
    record = (u8 *)Object_GetById(0);
    Engine_ActorSetSpriteFlags((s32)record, 1);
    Engine_ActorMoveToAndWait(0, 0x1f8, 148);
    v5 = 1;
    Engine_EventWait(10);
    *(u8 *)((u8 *)Object_GetById(0) + 90) |= v5;
    {
        s32 shown = 0x4000;

        *(s32 *)((s32)rec8 + 12) = -0x200000;
        *(u16 *)((s32)rec8 + 6) = shown;
    }
    Engine_EventWait(20);
    Engine_AudioPlayCue(0x134);
    Engine_ActorMoveToAndWait(18, 0x20c, 191);
    record = (u8 *)Object_GetById(18);
    Engine_ActorSetSpriteFlags((s32)record, 0);
    Call3(Engine_ActorMoveToAndWait, 18, 0x212, 211);
    Call3(Engine_ActorMoveToAndWait, 18, 0x244, 221);
    Engine_ActorSetDestination(18, 0x244, 0x152);
    {
        u8 *record = (u8 *)Object_GetById(0);
        /* FAKEMATCH: the volatile read keeps the flag byte's load in place. */
        u8 value = *(volatile u8 *)&record[35];
    
        record[35] = (u8)(value | v5);
    }
    Call3(Engine_ActorSetSpeed, 0, 0xcccc, 0x6666);
    FieldScene_RunScene3c9_02001280(1, 1);
    FieldScene_RunScene3c9_02001280(2, 1);
    FieldScene_RunScene3c9_02001280(3, 1);
    Call3(Engine_ActorWalkTo, 0, 0x1ec, 164);
    Engine_ActorWalkTo(1, 0x202, 164);
    Call3(Engine_ActorWalkTo, 2, 0x1ec, 140);
    Engine_ActorWalkToAndWait(3, 0x202, 140);
    Engine_ActorSetAnimation(0, 1);
    Engine_ActorSetAnimation(1, 1);
    Engine_ActorSetAnimation(2, 1);
    ((void (*)())Engine_ActorFaceDirection)(0, 0x4000, 0);
    ((void (*)())Engine_ActorFaceDirection)(1, 0x4000, 0);
    ((void (*)())Engine_ActorFaceDirection)(2, 0x4000, 0);
    ((void (*)())Engine_ActorFaceDirection)(3, 0x4000, 0);
    Engine_ActorWaitForMove(18);
    Engine_ActorSetPosition(18, 0, 0);
    base6_4013 = 0x4013;
    Engine_AudioPlayCue(0x121);
    Engine_EventSetMessage(0x2757);
    VinasuChojo_ShowMessage(base6_4013);
    ((void (*)())Engine_ActorFaceDirection)(0, 0x8000, 0);
    ((void (*)())Engine_ActorFaceDirection)(1, 0x8000, 0);
    ((void (*)())Engine_ActorFaceDirection)(2, 0x8000, 0);
    VinasuChojo_FaceActor(3, 0x8000);
    *(u8 *)((s32)Engine_EventGetViewCenter() + 85) = zero.v;
    Engine_CameraSetSpeed(0x4cccc, 0x9999);
    Engine_CameraMoveTo(0x1300000, 0x200000, 0x9e0000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(20);
    base5_2014 = 0x2014;
    ((void (*)())VinasuChojo_FaceActor)(20, 0xd000);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_AudioPlayCue(61);
    VinasuChojo_ShowMessage(base5_2014);
    Engine_ActorSetAnimation(19, 4);
    VinasuChojo_ShowMessage(base6_4013);
    ((void (*)())VinasuChojo_FaceActor)(20, 0xb000);
    Call3(Engine_ActorShowEmote, 20, 0x105, 40);
    VinasuChojo_ShowMessage(base5_2014);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 19, 0x100, 0);
    Engine_ActorShowEmote(20, 0x100, 20);
    Call3(Engine_ActorFaceDirection, 6, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 19, 0x5000, 0);
    Call3(Engine_ActorFaceDirection, 20, 0x5000, 20);
    Engine_CameraSetSpeed(0x19999, 0x3333);
    Call4(Engine_CameraMoveTo, 0x1260000, -1, 0xc20000, 1);
    Engine_EventWait(20);
    Call3(Engine_ActorSetSpeed, 21, 0xcccc, 0x6666);
    Call3(Engine_ActorWalkToAndWait, 21, 0x110, 200);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(base5_2014);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    VinasuChojo_ShowMessage(19);
    Engine_ActorSetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 20, 0x101, 40);
    VinasuChojo_ShowMessage(base5_2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 19, 0x101, 60);
    Engine_EventShowMessageAndWait(19, 0, 40);
    Call3(Engine_ActorShowEmote, 19, 0x106, 40);
    ((void (*)())VinasuChojo_FaceActor)(19, 0x8000);
    VinasuChojo_ShowMessage(0x2013);
    ((void (*)())Engine_ActorFaceDirection)(6, 0, 0);
    Call3(Engine_ActorShowEmote, 21, 0x103, 40);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    ((void (*)())Engine_ActorFaceDirection)(20, 0xb000, 40);
    Engine_ActorRunRepeatedMotion(21, 1);
    v6 = 160;
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorFaceDirection, 20, 0x5000, 0);
    VinasuChojo_FaceActor(19, (v6 << 7));
    Call3(Engine_ActorShowEmote, 19, 0x108, 20);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 20);
    Engine_EventShowMessageAndWait(21, 0, 20);
    ((void (*)())Engine_ActorFaceDirection)(20, 0x8000, 40);
    Engine_ActorSetAnimationAndWait(20, 4);
    Engine_EventShowMessageAndWait(base5_2014, 0, 40);
    Engine_ActorSetAnimationAndWait(21, 3);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Engine_ActorFaceDirection(20, (v6 << 7), 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 20, 0x105, 0);
    Call3(Engine_ActorShowEmote, 19, 0x105, 80);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 19, 0x101, 60);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorSetAnimationAndWait(21, 3);
    VinasuChojo_ShowMessage(21);
    Engine_ActorStartRepeatedMotion(19, 1);
    Engine_ActorRunRepeatedMotion(20, 1);
    Engine_EventWait(20);
    Engine_ActorSetAnimationAndWait(21, 4);
    Engine_EventShowMessageAndWait(21, 0, 20);
    Call3(Engine_ActorShowEmote, 20, 0x105, 60);
    Engine_EventShowMessageAndWait(base5_2014, 0, 20);
    ((void (*)())VinasuChojo_FaceActor)(21, 0xb000);
    VinasuChojo_ShowMessage(21);
    VinasuChojo_FaceActor(6, 0x3000);
    Engine_ActorRunRepeatedMotion(6, 2);
    Engine_EventWait(20);
    ((void (*)())VinasuChojo_FaceActor)(21, 0xd000);
    Engine_ActorSetAnimationAndWait(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    ((void (*)())Engine_ActorFaceDirection)(6, 0, 0);
    Engine_ActorRunRepeatedMotion(21, 1);
    VinasuChojo_ShowMessage(21);
    Engine_ActorSetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(base5_2014);
    Engine_ActorSetAnimationAndWait(21, 4);
    VinasuChojo_ShowMessage(21);
    Call3(Engine_ActorShowEmote, 20, 0x101, 0);
    Call3(Engine_ActorShowEmote, 19, 0x101, 80);
    ((void (*)())Engine_ActorFaceDirection)(19, 0x8000, 0);
    ((void (*)())Engine_ActorFaceDirection)(20, 0x8000, 0);
    Engine_CameraSetSpeed(0x6666, 0xccc);
    Call4(Engine_CameraMoveTo, 0x1260000, -1, 0xb40000, 1);
    Call3(Engine_ActorWalkToAndWait, 21, 0x106, 176);
    base6_8015 = 0x8015;
    ((void (*)())Engine_ActorFaceDirection)(21, 0x8000, 40);
    ((void (*)())Engine_ActorFaceDirection)(21, 0, 20);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(base6_8015);
    Engine_ActorShowEmote(19, 0x100, 20);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorStartRepeatedMotion(21, 2);
    VinasuChojo_ShowMessage(base6_8015);
    Call3(Engine_ActorShowEmote, 20, 0x103, 40);
    Call3(Engine_EventShowMessageAndWait, 0xa014, 0, 20);
    Call3(Engine_ActorShowEmote, 21, 0x105, 20);
    VinasuChojo_ShowMessage(base6_8015);
    Call3(Engine_ActorShowEmote, 19, 0x103, 20);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x101, 40);
    base5_a014 = 0xa014;
    VinasuChojo_ShowMessage(base6_8015);
    Engine_ActorSetAnimationAndWait(20, 4);
    VinasuChojo_ShowMessage(base5_a014);
    Engine_ActorSetAnimationAndWait(19, 3);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 21, 0x103, 60);
    ((void (*)())Engine_ActorFaceDirection)(21, 0x8000, 20);
    Engine_EventShowMessageAndWait(0xa015, 0, 40);
    Call3(Engine_ActorShowEmote, 6, 0x105, 120);
    Call3(Engine_ActorShowEmote, 20, 0x105, 60);
    VinasuChojo_ShowMessage(base5_a014);
    ((void (*)())Engine_ActorFaceDirection)(21, 0, 40);
    Engine_ActorSetAnimation(19, 3);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventShowMessageAndWait(base6_8015, 0, 20);
    VinasuChojo_ShowMessage(base5_a014);
    Engine_ActorShowEmote(21, 0x100, 40);
    Engine_ActorSetAnimation(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    Call3(Engine_ActorShowEmote, 6, 0x105, 40);
    Engine_ActorShowEmote(20, 0x108, 40);
    VinasuChojo_ShowMessage(base5_a014);
    Engine_ActorShowEmote(19, 0x103, 20);
    VinasuChojo_ShowMessage(0x2013);
    Engine_ActorRunRepeatedMotion(21, 1);
    Engine_EventWait(20);
    Engine_ActorStartRepeatedMotion(20, 2);
    VinasuChojo_ShowMessage(base5_a014);
    Engine_ActorSetAnimation(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    work = (s32)&gEventWork;
    *(s32 *)((*(s32 *)work + 0x1c0)) = 0x202;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    Engine_CameraMoveTo(0x1f80000, -0x180000, 0xa80000, 0);
    Engine_TaskWait(1);
    Engine_MapRedraw();
    Engine_TaskWait(1);
    base5_8001 = 0x8001;
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    Engine_ActorRunRepeatedMotion(1, 1);
    VinasuChojo_ShowMessage(base5_8001);
    Engine_ActorShowEmote(3, 0x101, 40);
    VinasuChojo_ShowMessage(3);
    Engine_ActorSetAnimationAndWait(2, 3);
    Engine_EventShowMessageAndWait(0x1002, 0, 40);
    Engine_ActorJump(1, 2, 20);
    VinasuChojo_ShowMessage(base5_8001);
    Engine_EventOpenMessage(base5_8001, 0);
    ((void (*)())Engine_ActorFaceDirection)(0, 0, 0);
    ((void (*)())Engine_ActorFaceDirection)(2, 0x4000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x2000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
        *(u16 *)((*(s32 *)work + 0x1d8)) += 1;
    }
    Engine_EventWait(20);
    VinasuChojo_ShowMessage(1);
    ((void (*)())Engine_ActorFaceDirection)(2, 0, 0);
    Engine_ActorFaceDirection(3, 0x8000, 20);
    Engine_ActorSetAnimation(0, 3);
    Engine_ActorSetAnimation(1, 3);
    Engine_ActorSetAnimation(2, 3);
    Engine_ActorSetAnimationAndWait(3, 3);
    pairScript = (s32)VinasuChojo_PairActionScript;
    ((void (*)())Engine_ActorEnableActionCallback)(1, pairScript);
    Engine_ActorEnableActionCallback(2, pairScript);
    Object_SetActionCallbackAndRefreshById(3, pairScript);
    Engine_EventWait(20);
    Engine_EventEnd();
}

void FieldScene_RunScene3c9_02001280(s32 a0, s32 a1)
{
    u32 i;
    s32 record;

    if (a1 != 0) {
        Actor_SetChildValue(a0, 0);
        record = Actor_Get(a0);
        Actor_SetSpriteFlags(record, 1);
        Actor_SetSpeed(a0, 0xcccc, 0x6666);
    } else {
        Actor_SetChildValue(a0, 15);
        record = Actor_Get(a0);
        Actor_SetSpriteFlags(record, 0);
    }
}

/* The party reaches the summit: Robin speaks, the others gather, and the
   summit's scene is recorded in two scene and entrance pairs of the game
   state, with entrances 2 and 9. */
void Scene_RunActorEntrySequence(void)
{
    s32 itemOwner;
    u8 *object;
    s32 hidden;
    s32 disableMask;
    s32 advanceStep;
    s32 facing;
    s32 finalMask;
    s32 actor20Key;
    s32 frame;
    s32 effectCallback;
    s32 enableMask;
    s32 actor20LateKey;
    s32 message;
    const s32 *groupActions;
    const s32 *pairActions;
    s32 sharedData;

    Event_Begin();
    hidden = 0;
    Engine_EventGetViewCenter()->motion_flags = hidden;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x14c0000, 0x200000, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x154, 184);
    VinasuChojo_FaceActor(0, 0x8000);
    Actor_RunRepeatedMotion(21, 1);
    Event_SetMessage((s32)MsgVinasuRobin);
    VinasuChojo_ShowMessage(0x9015);
    Engine_EventGetViewCenter()->motion_flags = hidden;
    Camera_SetSpeed(0xcccc, 0x1999);
    Camera_MoveTo(0x1300000, 0x200000, 0xb40000, 1);
    Actor_SetSpeed(ACTOR_GERALD, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_IVAN, 0x10000, 0x8000);
    Actor_SetSpeed(ACTOR_MIA, 0x16666, 0xb333);
    object = Object_GetById(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_GERALD, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Object_GetById(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_IVAN, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    object = Object_GetById(0);
    if (object != 0) {
        Actor_SetPosition(ACTOR_MIA, *(s32 *)((s32)object + 8), *(s32 *)((s32)object + 16));
    }
    Actor_WalkTo(ACTOR_GERALD, 0x148, 168);
    Actor_WalkTo(ACTOR_IVAN, 0x154, 196);
    Actor_WalkToAndWait(ACTOR_MIA, 0x146, 204);
    Actor_SetAnimation(ACTOR_GERALD, 1);
    Actor_SetAnimation(ACTOR_IVAN, 1);
    Engine_ActorFaceDirection(1, 0x8000, 0);
    Engine_ActorFaceDirection(2, 0x8000, 0);
    VinasuChojo_FaceActor(3, 0x8000);
    Engine_ActorFaceDirection(20, 0, 0);
    Engine_ActorFaceDirection(19, 0, 40);
    Actor_RunRepeatedMotion(20, 2);
    VinasuChojo_ShowMessage(20);
    VinasuChojo_FaceActor(19, 0x8000);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x103, 20);
    VinasuChojo_ShowMessage(21);
    Actor_StartRepeatedMotion(21, 2);
    Event_ShowMessageAndWait(21, 0, 20);
    Engine_ActorFaceDirection(21, 0xd000, 40);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(21, 4);
    VinasuChojo_FaceActor(21, 0);
    Event_ShowMessageAndWait(21, 0, 20);
    actor20Key = 0x2014;
    Actor_RunRepeatedMotion(20, 1);
    VinasuChojo_FaceActor(20, 0x8000);
    VinasuChojo_ShowMessage(actor20Key);
    Actor_SetAttachedEffect(ACTOR_MIA, 0x102);
    Actor_StartRepeatedMotion(ACTOR_MIA, 2);
    VinasuChojo_ShowMessage(3);
    Actor_RunRepeatedMotion(ACTOR_IVAN, 1);
    VinasuChojo_ShowMessage(2);
    VinasuChojo_FaceActor(19, 0);
    VinasuChojo_ShowMessage(19);
    Actor_ShowEmote(20, 0x106, 40);
    Engine_ActorFaceDirection(20, 0, 20);
    VinasuChojo_ShowMessage(20);
    Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 0);
    Engine_ActorFaceDirection(21, 0x8000, 0);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_ShowEmote(19, 0x102, 40);
    VinasuChojo_ShowMessage(19);
    VinasuChojo_FaceActor(20, 0xb000);
    VinasuChojo_ShowMessage(actor20Key);
    Engine_ActorFaceDirection(20, 0x8000, 20);
    VinasuChojo_ShowMessage(actor20Key);
    Engine_ActorFaceDirection(19, 0x8000, 0);
    Engine_ActorFaceDirection(0, 0x8000, 0);
    Engine_ActorFaceDirection(1, 0x8000, 0);
    Engine_ActorFaceDirection(2, 0x8000, 0);
    Engine_ActorFaceDirection(3, 0x8000, 0);
    Engine_ActorFaceDirection(21, 0, 20);
    Actor_ShowEmote(6, 0x101, 40);
    VinasuChojo_ShowMessage(6);
    Actor_ShowEmote(20, 0x103, 20);
    VinasuChojo_ShowMessage(actor20Key);
    Actor_RunRepeatedMotion(6, 2);
    Event_Wait(20);
    Actor_SetAnimation(6, 3);
    VinasuChojo_ShowMessage(6);
    Actor_SetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(actor20Key);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ActorFaceDirection(21, 0x3000, 0);
    Actor_WalkToAndWait(6, 0x114, 192);
    object = Object_GetById(19);
    {
        s32 shown = 0x5000;

        *(u16 *)((s32)object + 6) = shown;
    }
    Task_Wait(1);
    Actor_StartRepeatedMotion(19, 2);
    VinasuChojo_ShowMessage(0x2013);
    Actor_Jump(6, 2, 20);
    Actor_SetSpeed(6, 0x26666, 0x13333);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ActorFaceDirection(21, 0x8000, 0);
    Actor_WalkToAndWait(6, 248, 172);
    Engine_ActorFaceDirection(19, 0x8000, 0);
    Engine_ActorFaceDirection(21, 0x8000, 0);
    Engine_ActorFaceDirection(6, 0, 20);
    Actor_SetAnimationAndWait(6, 3);
    Event_Wait(40);
    SceneState_ApplyPair140And0();
    Task_Wait(1);
    frame = 0;
    do {
        object = Object_GetById(6);
        SceneEffect_UpdateObjectByFrameParity((s32)object);
        frame = (frame + 1);
        Task_Wait(1);
    } while ((u32)frame <= 39);
    effectCallback = (s32)FieldScene_RunStep6;
    Engine_TaskAddCallback(effectCallback, 0xc80);
    Event_Wait(80);
    Call3(Engine_ActorFaceDirection, 0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 20);
    Engine_ActorFaceDirection(21, 0, 40);
    Call3(Engine_ActorFaceDirection, 0, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 0);
    VinasuChojo_FaceActor(21, 0x8000);
    Actor_ShowEmote(20, 0x101, 40);
    Event_ShowMessageAndWait(0x2014, 0, 20);
    Actor_ShowEmote(6, 0x105, 80);
    Actor_StartRepeatedMotion(19, 2);
    VinasuChojo_ShowMessage(0x2013);
    Scheduler_RemoveCallback(effectCallback);
    Task_Wait(1);
    Actor_SetChildValue(6, 0);
    Task_Wait(10);
    FieldScene_ForwardValue81fc();
    Actor_Jump(6, 2, 40);
    VinasuChojo_ShowMessage(6);
    Actor_ShowEmote(20, 0x103, 20);
    VinasuChojo_ShowMessage(0x2014);
    disableMask = 254;
    Actor_RunRepeatedMotion(6, 2);
    *(u8 *)((u8 *)Object_GetById(6) + 90) &= disableMask;
    Actor_WalkToAndWait(6, 250, 176);
    enableMask = 1;
    Event_Wait(1);
    {
        u8 *actor = Object_GetById(6);
        s32 flags = actor[90];
        flags |= enableMask;
        actor[90] = flags;
    }
    Actor_ShowEmote(21, 0x103, 20);
    Engine_ActorFaceDirection(21, 0, 20);
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(19, 4);
    VinasuChojo_ShowMessage(0x2013);
    Actor_RunRepeatedMotion(6, 2);
    Event_Wait(40);
    Actor_SetSpeed(6, 0x9999, 0x4ccc);
    *(u8 *)((u8 *)Object_GetById(6) + 90) &= disableMask;
    Actor_WalkToAndWait(6, 248, 172);
    Event_Wait(1);
    {
        u8 *object = Object_GetById(6);
        enableMask |= object[90];
        object[90] = enableMask;
    }
    Event_Wait(20);
    Actor_SetAnimationAndWait(6, 3);
    Event_Wait(20);
    Actor_SetAnimationAndWait(19, 3);
    VinasuChojo_ShowMessage(0x2013);
    Actor_SetAnimationAndWait(6, 3);
    VinasuChojo_ShowMessage(6);
    Call3(Engine_ActorFaceDirection, 19, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 20, 0xb000, 20);
    Actor_ShowEmote(19, 0x105, 0);
    Actor_ShowEmote(20, 0x105, 60);
    Actor_SetAnimation(20, 4);
    VinasuChojo_ShowMessage(0x2014);
    Actor_ShowEmote(19, 0x101, 40);
    VinasuChojo_ShowMessage(19);
    Actor_ShowEmote(20, 0x105, 100);
    Actor_RunRepeatedMotion(20, 1);
    Event_Wait(20);
    VinasuChojo_FaceActor(20, 0);
    VinasuChojo_FaceActor(19, 0);
    Event_OpenMessage(20, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 3);
        advanceStep = 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimationAndWait(20, 4);
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        advanceStep = 0;
    }
    VinasuChojo_ShowMessage(20);
    if (advanceStep != 0) {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0x8000, 20);
    Actor_ShowEmote(20, 0x108, 40);
    Event_OpenMessage(20, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0xc000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_IVAN, 3);
        advanceStep = 1;
    } else {
        Event_Wait(20);
        Actor_SetAnimation(ACTOR_IVAN, 4);
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
        advanceStep = 0;
    }
    VinasuChojo_ShowMessage(2);
    if (advanceStep != 0) {
        *(u16 *)((u8 *)gEventWork + 0x1d8) += 1;
    }
    Actor_RunRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_FaceActor(1, 0x4000);
    VinasuChojo_ShowMessage(1);
    Actor_RunRepeatedMotion(19, 1);
    VinasuChojo_ShowMessage(19);
    Call3(Engine_ActorFaceDirection, 1, 0x8000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x8000, 0);
    VinasuChojo_FaceActor(3, 0x8000);
    Actor_SetAnimationAndWait(ACTOR_GERALD, 4);
    Actor_SetAnimationAndWait(20, 3);
    Event_ShowMessageAndWait(20, 0, 20);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, 3);
    Actor_SetAnimation(ACTOR_GERALD, 3);
    Actor_SetAnimation(ACTOR_IVAN, 3);
    Actor_SetAnimationAndWait(ACTOR_MIA, 3);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_SetAnimation(19, 3);
    VinasuChojo_ShowMessage(19);
    Call3(Engine_ActorFaceDirection, 20, 0xb000, 20);
    Actor_SetAnimationAndWait(20, 3);
    actor20LateKey = 0x2014;
    Event_Wait(40);
    VinasuChojo_FaceActor(20, 0x8000);
    VinasuChojo_ShowMessage(actor20LateKey);
    Actor_RunRepeatedMotion(21, 2);
    Event_Wait(20);
    Event_ShowMessageAndWait(21, 0, 40);
    VinasuChojo_ShowMessage(actor20LateKey);
    Actor_ShowEmote(21, 0x103, 60);
    VinasuChojo_FaceActor(19, 0x8000);
    Actor_RunRepeatedMotion(19, 1);
    VinasuChojo_ShowMessage(0x2013);
    Actor_ShowEmote(21, 0x105, 60);
    Actor_SetAnimationAndWait(21, 3);
    Event_Wait(20);
    Actor_SetSpeed(21, 0xcccc, 0x6666);
    Actor_WalkToAndWait(21, 0x120, 192);
    Engine_ActorFaceDirection(19, 0, 0);
    Engine_ActorFaceDirection(20, 0, 0);
    Actor_WalkToAndWait(21, 0x136, 192);
    Actor_WalkToAndWait(21, 0x148, 186);
    Event_Wait(20);
    Actor_RunRepeatedMotion(21, 2);
    /* The message is a link-time name, loaded from the literal pool after
       the two preceding calls, as the game loads it. */
    message = (s32)MsgVinasuRobinHandedOverShamansRod;
    Message_ShowCentered(message, 1);
    Actor_WalkToAndWait(21, 0x136, 192);
    Engine_ActorFaceDirection(19, 0x8000, 0);
    Engine_ActorFaceDirection(20, 0x8000, 0);
    Actor_WalkToAndWait(21, 0x120, 192);
    Actor_WalkToAndWait(21, 0x106, 176);
    Engine_ActorFaceDirection(21, 0, 40);
    Event_SetMessage((message + 1));
    VinasuChojo_ShowMessage(21);
    Actor_SetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(actor20LateKey);
    Actor_SetAnimationAndWait(21, 3);
    VinasuChojo_FaceActor(20, 0);
    VinasuChojo_FaceActor(21, 0x8000);
    Actor_SetAnimationAndWait(21, 3);
    Actor_SetAnimationAndWait(6, 3);
    Actor_SetSpeed(6, 0xcccc, 0x6666);
    Actor_WalkToAndWait(6, 0x104, 186);
    Engine_ActorFaceDirection(21, 0x3000, 0);
    Actor_WalkToAndWait(6, 0x114, 192);
    Audio_PlayCue(19);
    facing = 160;
    object = Object_GetById(19);
    *(u16 *)((s32)object + 6) = (facing << 7);
    Task_Wait(1);
    Actor_RunRepeatedMotion(19, 1);
    VinasuChojo_ShowMessage(19);
    Actor_RunRepeatedMotion(6, 2);
    Engine_ActorFaceDirection(21, 0, 0);
    Engine_ActorFaceDirection(20, (facing << 7), 0);
    Call3(Engine_ActorFaceDirection, 6, 0xd000, 20);
    Actor_Jump(ACTOR_MIA, 2, 20);
    VinasuChojo_FaceActor(3, 0xa000);
    VinasuChojo_ShowMessage(3);
    Engine_ActorFaceDirection(21, 0, 0);
    Engine_ActorFaceDirection(6, 0, 0);
    Engine_ActorFaceDirection(19, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 20, 0xb000, 80);
    Engine_ActorFaceDirection(19, 0, 0);
    Engine_ActorFaceDirection(20, 0, 40);
    Audio_PlayCue(29);
    VinasuChojo_ShowMessage(20);
    Engine_ActorFaceDirection(21, 0x3000, 0);
    Call3(Engine_ActorFaceDirection, 6, 0xb000, 20);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    VinasuChojo_ShowMessage(2);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_SetAnimationAndWait(19, 3);
    VinasuChojo_ShowMessage(19);
    Engine_ActorFaceDirection(21, 0, 0);
    VinasuChojo_FaceActor(6, 0xd000);
    Actor_SetAnimation(ACTOR_MIA, 3);
    VinasuChojo_ShowMessage(3);
    Actor_ShowEmote(20, 0x100, 20);
    Engine_ActorFaceDirection(20, 0x3000, 20);
    VinasuChojo_ShowMessage(20);
    Actor_ShowEmote(6, 0x102, 0);
    Actor_ShowEmote(21, 0x102, 60);
    Engine_ActorFaceDirection(6, 0, 0);
    VinasuChojo_FaceActor(21, 0);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 40);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_ShowMessage(1);
    VinasuChojo_FaceActor(19, 0);
    Actor_SetAnimationAndWait(19, 4);
    VinasuChojo_ShowMessage(19);
    VinasuChojo_FaceActor(20, 0);
    Actor_SetAnimation(20, 4);
    VinasuChojo_ShowMessage(20);
    Engine_ActorFaceDirection(6, 0xd000, 0);
    Actor_ShowEmote(6, 0x101, 0);
    Actor_ShowEmote(ACTOR_MIA, 0x101, 60);
    Event_ShowMessageAndWait(ACTOR_MIA, 0, 20);
    VinasuChojo_FaceActor(19, 0x3000);
    Actor_SetAnimation(19, 4);
    VinasuChojo_ShowMessage(19);
    Actor_SetAttachedEffect(ACTOR_IVAN, 0x102);
    Event_ShowMessageAndWait(ACTOR_IVAN, 0, 20);
    VinasuChojo_FaceActor(19, 0);
    Actor_SetAnimationAndWait(20, 3);
    VinasuChojo_ShowMessage(20);
    Actor_StartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    Actor_StartRepeatedMotion(ACTOR_IVAN, 2);
    Actor_RunRepeatedMotion(ACTOR_MIA, 2);
    Engine_ActorFaceDirection(0, 0xa000, 0);
    Call3(Engine_ActorFaceDirection, 1, 0x2000, 0);
    Call3(Engine_ActorFaceDirection, 2, 0x6000, 0);
    Call3(Engine_ActorFaceDirection, 3, 0xe000, 40);
    Actor_RunRepeatedMotion(ACTOR_MIA, 1);
    VinasuChojo_FaceActor(3, 0xa000);
    VinasuChojo_ShowMessage(3);
    Engine_ActorFaceDirection(1, 0x8000, 20);
    Actor_ShowEmote(ACTOR_GERALD, 0x103, 20);
    Actor_StartRepeatedMotion(ACTOR_GERALD, 2);
    VinasuChojo_ShowMessage(1);
    Engine_ActorFaceDirection(6, 0, 0);
    Engine_ActorFaceDirection(0, 0x8000, 0);
    Engine_ActorFaceDirection(2, 0x8000, 40);
    Actor_ShowEmote(20, 0x103, 40);
    Actor_StartRepeatedMotion(20, 2);
    VinasuChojo_ShowMessage(20);
    Actor_RunRepeatedMotion(19, 1);
    VinasuChojo_ShowMessage(19);
    itemOwner = PartyInventory_FindOwner(65);
    GameFlag_Set(itemOwner + 0x345);
    finalMask = 254;
    PartyInventory_Remove(65);
    *(u8 *)((u8 *)Object_GetById(0) + 90) &= finalMask;
    *(u8 *)((u8 *)Object_GetById(1) + 90) &= finalMask;
    *(u8 *)((u8 *)Object_GetById(2) + 90) &= finalMask;
    *(u8 *)((u8 *)Object_GetById(3) + 90) &= finalMask;
    *(u8 *)((u8 *)Object_GetById(19) + 90) &= finalMask;
    *(u8 *)((u8 *)Object_GetById(20) + 90) &= finalMask;
    *(u8 *)((u8 *)Object_GetById(21) + 90) &= finalMask;
    {
        u8 *actor = Object_GetById(6);
        actor += 90;
        groupActions = SceneAction_EntryGroup;
        finalMask &= *actor;
        *actor = finalMask;
    }
    Actor_EnableActionCallback(ACTOR_PARTY_LEADER, groupActions);
    Actor_EnableActionCallback(ACTOR_GERALD, groupActions);
    Actor_EnableActionCallback(ACTOR_IVAN, groupActions);
    Actor_EnableActionCallback(ACTOR_MIA, groupActions);
    pairActions = SceneAction_EntryPair;
    Actor_EnableActionCallback(19, pairActions);
    Actor_EnableActionCallback(20, pairActions);
    Actor_EnableActionCallback(21, groupActions);
    Object_SetActionCallbackAndRefreshById(6, groupActions);
    /* These tables are shared by the final actor-action assignments. */
    /* FAKEMATCH: an empty do-while around these statements; it only changes instruction scheduling. */
    do {
        sharedData = (s32)&gGameState;
        *(u8 *)((sharedData + 0x22b)) = 3;
    } while (0);
    {
        s32 actor = (s32)&SceneId_VinasuChojo;

        /*
         * Through Call2 rather than called directly: the helper's parameter
         * pseudos fix the order the two argument registers are materialised
         * in, and direct calls here emit them the other way round.
         */
        Call2(Party_SetFields1ceAnd1d0, actor, 2);
        Call2(Event_SetPair1d4, actor, 9);
    }
    BattleFx_SetWeightedResult(98, 1);
    GameFlag_Set(0x350);
}

void SceneEffect_UpdateObjectByFrameParity(s32 a)
{
    if (*(s32 *)&gFrameCount & 2) {
        Object_SetPartPalettes(a, 7);
    } else {
        Object_SetPartPalettes(a, 0);
    }
    if (IwramUnsignedRemainder(*(s32 *)&gFrameCount, 15) == 0) {
        VinasuChojo_SpawnLinkedPairEffects(a);
    }
}

void SceneState_ForwardByRuntimeWordBits(s32 a)
{
    volatile u32 *p = (u32 *)&gFrameCount;

    if (*p & 1) {
        Object_SetPartPalettes(a, IwramUnsignedRemainder(*p >> 1, 6));
    }
    if (IwramUnsignedRemainder(*p, 15) == 0) {
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
        s32 amplitude = Math_Sin(frame << 10);
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
        s32 amplitude = Math_Sin(frame << 10);
        s32 offset;
        effect->f18 = amplitude;
        effect->f1c = -amplitude;
        effect->f8 = anchor[2];
        effect->fc += 0x10000;
        offset = 0x10000 - amplitude;
        effect->f10 = anchor[4] - offset * 5 + 0x100000;
    }
}

/* Spawns the linked pair of effect objects above the parent actor, with a cue, and gives both the parent's sprite priority. */
void VinasuChojo_SpawnLinkedPairEffects(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = Data_03001f30;
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
                /* FAKEMATCH: a plain byte access; the struct field store
                 * leaves a dead QImode zero that takes r3 from the +85
                 * address. */
                *(u8 *)&sprite->unknown_1d |= 1;
                sprite->tile = (ResourceTableEntries[sprite->vram_block].offset >> 5) & 0x3ff;
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
    Psynergy_Begin(140, 0);
}

void FieldScene_ForwardValue81fc(s32 a)
{
    BattleEffect_CleanupSceneObjects(a);
}

void FieldScene_RunStep6(void)
{
    SceneState_ForwardByRuntimeWordBits((s32)Actor_Get(6));
}
