#include "GROUP_DEPARTURE.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "CALL.H"
#include "FIELD_EFFECT.H"

struct Pulse {
    u8 unknown_00[0x64];
    s16 timer;
    s16 beat;
};

extern u8 MsgHaidiaICantMoveGetHelp[];
extern u8 MsgHaidiaIllGo[];
extern u8 MsgHaidiaRobin[];

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
extern struct PairWork *gEffectWork;

struct WorldMapVramBlock {
    u16 base;
    u16 offset;
};

extern struct WorldMapVramBlock gVramBlockCache[];
s32 Object_InitializeMode(struct FieldSprite *sprite, s32 animation);
void Resource_ResetEntry(s32 block);
void OverlayObject_UpdateArcFromParent(union FieldObject *object);
void SceneEffect_UpdateArcOverAnchor(union FieldObject *object);

/* The OAM view with attribute 1 ending in the two-bit size field. */
struct WorldMapOam {
    u8 unknown_00[4];
    u16 attr0;
    u16 x : 9;
    u16 affine_index : 5;
    u16 size : 2;
};

extern u8 MsgHaidiaDoorWontOpen[];
extern u8 MsgHaidiaSChestValuables[];

void HaidiaArashi_FlashLightning(void)
{
    u32 i;
    s32 record;

    Engine_TaskWait(20);
    GameFlag_Set(0x166);
    Map_SetLayerEntryFlag(0);
    Map_SetLayerEntryFlag(1);
    Map_SetLayerEntryFlag(2);
    Map_SetLayerEntryFlag(3);
    Map_SetLayerEntryFlag(4);
    Map_SetLayerEntryFlag(5);
    ColorBuffer_ApplyTarget(0x10003, 1);
    ColorBuffer_ApplyTarget(0x10000, 2);
    Engine_ColorBufferInterpolate(1);
    Engine_TaskWait(120);
    ColorBuffer_ApplyTarget(0, 0);
    Engine_ColorBufferInterpolate(60);
    Engine_TaskWait(60);
    GameFlag_Clear(0x166);
    Map_ClearLayerEntryFlag(0);
    Map_ClearLayerEntryFlag(1);
    Map_ClearLayerEntryFlag(2);
    Map_ClearLayerEntryFlag(3);
    Map_ClearLayerEntryFlag(4);
    Map_ClearLayerEntryFlag(5);
}

/* Keep actor 27's glow on actor 19: step the screen work values as 19's timer runs down, and on odd frames pulse the glow's scale by 19's beat. */
void HaidiaArashi_UpdatePulsingGlow(void)
{
    struct FieldActor *source;
    struct FieldActor *glow;
    struct FieldSprite *sprite;
    s16 *timer;

    source = Object_GetById(19);
    glow = Object_GetById(27);
    sprite = glow->sprite;
    timer = &((struct Pulse *)source)->timer;
    if (*timer != 0) {
        if (*timer == 60) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x30000, 0x30000, 0x10000);
        }
        if (*timer == 40) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x40000, 0x40000, 0x10000);
        }
        if (*timer == 30) {
            Call3(Engine_WorkSetValuesIfNonNegative, 0x20000, 0x20000, 0x10000);
        }
        if (*timer == 20) {
            Call3(Engine_WorkSetValuesIfNonNegative, -1, -1, 0xe666);
        }
        (*timer)--;
    }
    glow->x.fixed = source->x.fixed;
    glow->z.fixed = source->z.fixed;
    glow->target_x = glow->x.fixed;
    ((u8 *)sprite)[35] = 10;
    if (gFrameCount & 1) {
        switch (((struct Pulse *)source)->beat) {
        case 1:
        case 5:
            glow->scale_x += 0xa3d;
            glow->scale_y += 0xa3d;
            break;
        case 4:
            glow->scale_x += 0x51e;
            glow->scale_y += 0x51e;
            break;
        case 2:
        case 3:
        case 6:
        case 7:
        case 8:
        case 9:
            glow->scale_x += -0x7ae;
            glow->scale_y += -0x7ae;
            break;
        }
        sprite->scale = glow->scale_x;
    } else {
        sprite->scale = 0;
    }
}

void ActorPresentation_SelectActorTwentySevenState(void)
{
    struct Actor *actor = Actor_Get(27);
    u32 flags = gFrameCount;
    u8 *presentation = actor->presentation;

    if (flags & 1) {
        u8 *state = presentation + 35;
        *state = 2;
    } else {
        u8 *state = presentation + 35;
        *state = 64;
    }
}

void FieldScene_RunScene372_02003e48(void)
{
    u32 i;
    s32 rec7;
    s32 rec8;
    s32 record;

    rec8 = Object_GetById(ACTOR_PARTY_LEADER);
    rec7 = Object_GetById(8);
    Engine_EventBegin();
    if (GameFlag_IsSet(0x305) != 0) {
        Engine_ActorStop(8);
        Engine_EventWait(10);
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Engine_ActorSetAnimation(8, 7);
        } else {
            Engine_ActorSetAnimation(8, 8);
        }
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaICantMoveGetHelp);
        Event_ShowMessage(8, 0);
        Engine_ActorEnableActionCallback(8, (s32)HaidiaArashi_ActorEightScript);
        Engine_ActorSetAnimation(8, 6);
    } else {
        Engine_ActorStop(8);
        *(s32 *)(rec7 + 24) = 0x10000;
        *(s32 *)(rec7 + 28) = 0x10000;
        Actor_FaceDirection(8, 0x1000, 0);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Engine_ActorSetAnimation(8, 7);
        } else {
            Engine_ActorSetAnimation(8, 8);
        }
        Engine_EventWait(20);
        Engine_EventSetMessage((s32)MsgHaidiaIllGo);
        Event_ShowMessageAndWait(8, 0, 20);
        Engine_ActorSetAnimation(8, 1);
        Engine_ActorJump(8, 4, 0);
        Engine_EventWait(80);
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(40);
        if (*(s16 *)(rec8 + 6) >= 0) {
            Engine_ActorSetAnimation(8, 7);
        } else {
            Engine_ActorSetAnimation(8, 8);
        }
        Engine_EventWait(2);
        Engine_ActorJump(8, 2, 0);
        Engine_EventWait(60);
        Engine_ActorRunRepeatedMotion(8, 2);
        Engine_EventWait(20);
        Event_ShowMessage(8, 0);
        Engine_ActorEnableActionCallback(8, (s32)HaidiaArashi_ActorEightScript);
        Engine_ActorSetAnimation(8, 6);
        GameFlag_Set(0x305);
    }
    Engine_EventEnd();
}

void FieldScene_ConfigureActorTwentyTwoScene(void)
{
    u32 i;
    u8 *record;

    Engine_EventBegin();
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_ActorStop(ACTOR_ID);
    Scheduler_RemoveCallback((s32)OverlayObject_CopyRecordField1ToSlots22And8);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e0, 0x570);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    Actor_FaceDirection(ACTOR_ID, 0x3000, 20);
    Actor_Get(ACTOR_ID)->priority_flags |= ACTOR_PRIORITY_AUTOMATIC;
    Actor_SetPosition(ACTOR_ID, 0xf90000, 0x4d80000);
    Engine_TaskWait(1);
    Engine_EventSetMessage((s32)MsgHaidiaRobin);
    Event_ShowMessage(0x1016, 0);
    Actor_SetPosition(ACTOR_ID, 0xac0000, 0x4fe0000);
    Engine_TaskWait(1);
    Camera_SetSpeed(0x40000, 0x8000);
    Camera_MoveTo(0xa20000, 0, 0x5050000, 1);
    Engine_CameraWaitForMove();
    Engine_EventWait(40);
    Engine_ActorSetAnimationAndWait(ACTOR_ID, 4);
    Event_ShowMessageAndWait(0x1016, 0, 10);
    Actor_FaceDirection(ACTOR_ID, 0xc000, 20);
    Engine_ActorRunRepeatedMotion(ACTOR_ID, 2);
    Event_ShowMessageAndWait(0x1016, 0, 10);
    Actor_FaceDirection(ACTOR_ID, 0x1000, 20);
    Engine_ActorSetAnimationAndWait(ACTOR_ID, 3);
    Actor_SetSpeed(ACTOR_ID, 0x20000, 0x10000);
    Actor_WalkToAndWait(ACTOR_ID, 165, 0x514);
    Actor_WalkToAndWait(ACTOR_ID, 195, 0x598);
    GameFlag_Set(0x842);
}

/* Each of the 15 placement calls below takes the same 6-argument shape:
 * two coordinate-like values, two more coordinate-like values, and a
 * trailing pair of small counts. The final call takes no arguments. */
void FieldScene_BuildPlacementGrid(void)
{
    u32 i;
    u8 *record;

    Map_CopyCellsTo(16, 96, 11, 73, 6, 3); /* main:08009180 */
    Map_CopyCellsTo(16, 96, 34, 68, 14, 10); /* main:08009180 */
    Map_CopyCellsTo(16, 96, 64, 68, 7, 7); /* main:08009180 */
    Map_CopyCellsTo(9, 95, 11, 73, 6, 3); /* main:08009180 */
    Map_CopyCellsTo(40, 94, 34, 68, 14, 10); /* main:08009180 */
    Map_CopyCellsTo(54, 94, 64, 68, 8, 7); /* main:08009180 */
    Map_CopyCellsTo(72, 75, 72, 76, 1, 1); /* main:08009180 */
    Map_CopyCellsTo(72, 75, 74, 76, 1, 1); /* main:08009180 */
    Map_CopyCellAttributes(7, 75, 1, 1, 6, 75); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 3, 1, 8, 71); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 2, 1, 9, 72); /* main:080091c0 */
    Map_CopyCellAttributes(8, 70, 2, 1, 9, 73); /* main:080091c0 */
    Map_CopyCellAttributes(11, 66, 1, 1, 8, 73); /* main:080091c0 */
    Map_CopyCellAttributes(12, 66, 1, 4, 11, 73); /* main:080091c0 */
    Map_CopyCellAttributes(25, 0, 1, 1, 6, 74); /* main:080091c0 */
    /* No-argument call that closes out the sequence started above. */
    Engine_MapRedraw(); /* main:08009128 */
}

void SceneState_SetWords1c0And1c8AndRun(void)
{

    u8 *state;

    Engine_EventBegin();
    state = (u8 *)gEventWork;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    GameFlag_Set(0x87c);
    BattleFx_SetWeightedResult(12, 2);
    GameFlag_Set(0x900);
    Engine_EventEnd();
}

void SceneState_SetWorkWordsAndFlag87f(void)
{

    u8 *state;

    Engine_EventBegin();
    state = (u8 *)gEventWork;
    *(s32 *)(state + 0x1C0) = 0x200;
    *(s32 *)(state + 0x1C8) = 64;
    GameFlag_Set(0x87f);
    BattleFx_SetWeightedResult(12, 3);
    GameFlag_Set(0x900);
    Engine_EventEnd();
}

void SceneActor_SetModeByFrameBit1(s32 o)
{
    s32 v;

    if ((*(volatile s32 *)&gFrameCount & 2) != 0) {
        Engine_ObjectSetPartPalettes(o, 7);
    } else {
        Engine_ObjectSetPartPalettes(o, 0);
    }
    {
        volatile s32 *q = (volatile s32 *)&gFrameCount;
        v = (HaidiaArashi_ShakeShift << 3) + 16;
        if (Math_RemainderUnsigned(*q, v) == 0) {
            HaidiaArashi_SpawnEffectPair((union PairObject *)o);
        }
    }
}

void OverlayObject_UpdateRandomSlotByFrame(s32 obj)
{
    volatile s32 *fc = (volatile s32 *)&gFrameCount;
    s32 t;
    s32 n;

    if ((*fc & 1) != 0) {
        t = (s32)((u32)*fc >> 1);
        Engine_ObjectSetPartPalettes(obj, Math_RemainderUnsigned(t, 6));
    }
    n = (HaidiaArashi_ShakeShift << 3) + 16;
    if (Math_RemainderUnsigned(*fc, n) == 0) {
        HaidiaArashi_SpawnEffectPair((union PairObject *)obj);
    }
}

void OverlayObject_ApplyIwramWord1e40(s32 o)
{
    volatile s32 *p = (s32 *)&gFrameCount;
    s32 t;

    if ((*p & 1) != 0) {
        t = (s32)((u32)*p >> 1);
        Engine_ObjectSetPartPalettes(o, Math_RemainderUnsigned(t, 6));
    }
}

void SceneEffect_UpdateArcOverAnchor(union FieldObject *object)
{
    Obj *o = (Obj *)object;
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Engine_ObjectDispatchRelease(o);
    } else {
        d = Engine_MathSin(t << 10);
        o->f18 = d;
        o->f1c = d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 + ((k << 2) + k) + 0x80000;
    }
}

void OverlayObject_UpdateArcFromParent(union FieldObject *object)
{
    Obj *o = (Obj *)object;
    Obj *b;
    s32 t;
    s32 d;
    s32 k;

    b = o->f68;
    o->f64++;
    t = (s16)o->f64;
    t = (s16)o->f64;
    if (t > 31) {
        Engine_ObjectDispatchRelease(o);
    } else {
        d = Engine_MathSin(t << 10);
        o->f18 = d;
        o->f1c = -d;
        o->f08 = b->f08;
        k = 0x10000;
        o->f0c += k;
        k -= d;
        o->f10 = b->f10 - ((k << 2) + k) + 0x100000;
    }
}

/* Stormy Haidia: spawns the linked pair of effect objects above the parent actor, with a cue, and gives both the parent's sprite priority. */
void HaidiaArashi_SpawnEffectPair(union PairObject *parent)
{
    union PairObject *pair[2];
    union PairObject *child;
    struct PairSprite *part;
    struct FieldSprite *sprite;
    struct PairWork *work = gEffectWork;
    s32 i;

    Engine_AudioPlayCue(152);
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
                Object_InitializeMode(sprite, 0);
                sprite->flags = 0;
                Resource_ResetEntry(sprite->vram_block);
                sprite->vram_block = work->vram_block;
                /* FAKEMATCH: a plain byte access; the struct field store
                 * leaves a dead QImode zero that takes r3 from the +85
                 * address. */
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

        p->object.actor.update = OverlayObject_UpdateArcFromParent;
        sp->priority = parent->object.actor.sprite->priority;
    }
    {
        struct FieldActor *p = &pair[1]->object.actor;
        struct FieldSprite *sp = p->sprite;

        sp->priority = parent->object.actor.sprite->priority;
        p->update = SceneEffect_UpdateArcOverAnchor;
        p->priority_flags = 2;
    }
}

void SceneState_SetValue140Mode0(void)
{

    Engine_PsynergyBegin(140, 0);
}

void FieldScene_RunSingleStep(void)
{
    BattleEffect_CleanupSceneObjects();
}

void FieldScene_RunFourPairedSteps(void)
{
    OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(32));
    OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(33));
    OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(30));
    if (HaidiaArashi_ShakeDone == 0) {
        OverlayObject_UpdateRandomSlotByFrame((s32)Actor_Get(29));
    }
}

void SceneState_SetValue19ThenCall(void)
{

    OverlayObject_ApplyIwramWord1e40((s32)Actor_Get(19));
}

void OverlayObject_CopyRecordField1ToSlots22And8(void)
{
    Ent *src;
    Ent *dst;
    Ent *dst2;

    src = ((Rec *)Object_GetById(0))->f50;
    dst = ((Rec *)Object_GetById(22))->f50;
    dst->f = src->f;
    dst2 = ((Rec *)Object_GetById(8))->f50;
    dst2->f = src->f;
}

void SceneState_SetValueEe4(void)
{

    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgHaidiaDoorWontOpen, 1);
    Engine_EventEnd();
}

void FieldScene_ShowChestValuables(void)
{
    Engine_EventBegin();
    Engine_MessageShowCentered((s32)MsgHaidiaSChestValuables, 1);
    Engine_EventEnd();
}
