#include "GLOBAL_CELLS.H"
#include "ENTRY_SETUP.H"
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_EFFECT.H"
#include "FIELD_SERVICE.H"

struct ScrollLayer {
    u8 unknown_00[8];
    s32 x;
};

struct MapWork {
    u8 unknown_00[0x164];
    struct ScrollLayer layer;
};

void FieldScene_RunThreeCallSequence(void)
{

    Engine_EventBegin();
    RunStagedActorTransition();
    Engine_EventEnd();
}

/* The far end of the bridge: the first time the leader steps on it, the
 * bridge rolls back the other way in a spray of dust. */
void VinasuHeya_RetractBridge(void)
{
    struct EffectOptions options;
    struct ScrollLayer *layer;
    struct FieldActor *leader;
    /* The leader's tile column, then the bridge column being drawn. */
    s32 x;
    s32 z;
    s32 countdown;
    u32 i;
    s32 dust_x;

    layer = &((struct MapWork *)gMapWork[0])->layer;
    leader = Object_GetById(0);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    leader->y.fixed = 0;
    if (x >= 532 && x <= 539 && z >= 324 && z < 332) {
        leader->y.fixed = -0x20000;
        if (!Engine_GameFlagIsSet(0x306)) {
            Engine_EventBegin();
            Engine_MapCopyCellsTo(63, 29, 33, 20, 1, 1);
            Engine_AudioPlayCue(161);
            Engine_MapCopyCellsTo(44, 83, 44, 80, 3, 3);
            Engine_EventWait(30);
            Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Engine_AudioPlayCue(239);
            Engine_EventWait(20);
            dust_x = 0x2680000;
            x = 0;
            countdown = 60;
            for (i = 0; i <= 319; i++, countdown--) {
                layer->x += 0x3333;
                if (dust_x >= 0x2400000 && i > 40) {
                    dust_x += -0x3333;
                    options.priority = 2;
                    options.start_scale_x = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                    options.start_scale_y = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
                    Effect_Spawn(dust_x, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16), 0, 0x8a0000, &options);
                    if (countdown == 0) {
                        countdown = 40;
                        x += 4;
                        Engine_MapCopyCellsTo(x, 56, 36, 17, 3, 4);
                    }
                }
                Engine_TaskWait(1);
            }
            layer->x += 0x8000;
            layer->x = layer->x / 0x10000 << 16;
            Engine_AudioPlayCue(288);
            Engine_AudioPlayCue(188);
            Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapRenderWaitForValues();
            gEventWork->start_transition = 0x202;
            Engine_EventRequestExit(18);
            Engine_EventEnd();
        }
    }
}

/* Stepping onto the bridge end drops the leader a little; the first time,
 * the bridge rolls out across the chasm in a spray of dust. */
void VinasuHeya_ExtendBridge(void)
{
    struct EffectOptions options;
    struct ScrollLayer *layer;
    struct FieldActor *leader;
    /* The leader's tile column, then the bridge column being drawn. */
    s32 x;
    s32 z;
    s32 countdown;
    u32 i;
    s32 dust_x;

    layer = &((struct MapWork *)gMapWork[0])->layer;
    leader = Object_GetById(0);
    x = leader->x.part.pixel;
    z = leader->z.part.pixel;
    leader->y.fixed = 0;
    if (x >= 788 && x <= 795 && z >= 324 && z < 332) {
        leader->y.fixed = -0x20000;
        if (!Engine_GameFlagIsSet(0x307)) {
            Engine_EventBegin();
            Engine_MapCopyCellsTo(63, 29, 49, 20, 1, 1);
            Engine_AudioPlayCue(161);
            Engine_EventWait(30);
            Engine_WorkSetValuesIfNonNegative(0x10000, 0x10000, 0x10000);
            Engine_AudioPlayCue(239);
            Engine_EventWait(20);
            dust_x = 0x2c00000;
            x = 61;
            countdown = 60;
            for (i = 0; i <= 319; i++, countdown--) {
                layer->x += -0x3333;
                dust_x += 0x3333;
                if (dust_x >= 0x2c80000 && dust_x < 0x2f00000) {
                    options.priority = 2;
                    options.start_scale_x = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                    options.start_scale_y = ((u32)(Engine_RandomNext() * 3) >> 16) * 0x3333 + 0xcccc;
                    options.spin = ((u32)(Engine_RandomNext() << 12) >> 16) + 0xf800;
                    Effect_Spawn(dust_x, 0, 0x1200000, 0, -(((gFrameCount & 1) * 3) << 16), 0, 0x8a0000, &options);
                    if (countdown == 0) {
                        countdown = 40;
                        x -= 4;
                        Engine_MapCopyCellsTo(x, 56, 44, 17, 3, 4);
                    }
                }
                Engine_TaskWait(1);
            }
            layer->x += 0x8000;
            layer->x = layer->x / 0x10000 << 16;
            Engine_AudioPlayCue(288);
            Engine_AudioPlayCue(188);
            Engine_WorkSetValuesIfNonNegative(-1, -1, 0xe666);
            Engine_MapRenderWaitForValues();
            gEventWork->start_transition = 0x202;
            Engine_EventRequestExit(19);
            Engine_EventEnd();
        }
    }
}

/* The pillar switches and the two effects that open the way. */
struct SwitchCell {
    u32 x;
    u32 z;
};

struct SwitchEffect {
    s32 active;
    u8 unknown_04[0x5f];
    u8 finished;
};

extern struct SwitchCell Data_02005164[8];
extern s32 Data_0200577c[];
extern s32 Data_020057c8[];
extern s32 Data_02005ac8[];
extern u16 Data_02005d3c[];

struct FieldActor *OverlayObject_PrepareObjectWithCommand15(s32 x, s32 y, s32 z, s32 kind);
void OverlayObject_WaitUntilIdle(struct FieldActor *object);
struct SwitchEffect *SceneEffect_SpawnEffect284AtCell(s32 x, s32 z, const void *script);
void ObjectDispatch_WaitForValue16(struct SwitchEffect *effect);
void Object_PlaceCurrentWithinCameraBounds(s32 actor, s32 mode);
void Battle_ResetEffectCounter(void);

/*
 * A pushed pillar reaches a switch: it sinks into the floor, and once all
 * four are down the two door effects run and the way opens.
 */
void Scene_RunScene3c8SequenceA(void)
{
    struct FieldActor *effect;
    struct FieldActor *actor;
    struct FieldActor *other;
    struct FieldActor *leader;
    struct FieldActor *a;
    struct FieldActor *b;
    struct FieldActor *c;
    struct FieldActor *d;
    u8 *flags;
    u8 *motion;
    u32 id;
    u32 i;
    u32 slot;
    u32 priority;

    effect = 0;
    leader = Object_GetById(0);
    Engine_EventBegin();
#if defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) || defined(TBS_EDITION_IT)
    Battle_ResetEffectCounter();
#endif
    Engine_MapCopyCellAttributes(69, 48, 4, 2, 5, 48);
    Engine_MapCopyCellAttributes(73, 37, 9, 13, 9, 37);
    for (id = 15; id <= 18; id++) {
        actor = Object_GetById(id);
        flags = &actor->priority_flags;
        if (*flags != 2)
            Engine_MapCopyCellAttributes(72, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);
        else
            Engine_MapCopyCellAttributes(73, 48, 1, 1, actor->x.fixed >> 20, actor->z.fixed >> 20);

        slot = 8;
        for (i = 0; i < 8; i++) {
            if ((actor->x.fixed >> 20) == Data_02005164[i].x
                && (actor->z.fixed >> 20) == Data_02005164[i].z
                && actor->y.fixed >= 0) {
                slot = i;
                break;
            }
        }
        if (slot == 8)
            continue;
        for (i = 15; i <= 18; i++) {
            other = Object_GetById(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) == (other->z.fixed >> 20)) {
                slot = 8;
                break;
            }
        }
        if (slot == 8)
            continue;

        priority = leader->sprite->priority;
        if ((u32)(leader->z.fixed >> 20) <= Data_02005164[slot].z) {
            effect = OverlayObject_PrepareObjectWithCommand15(actor->x.fixed, actor->y.fixed,
                                   actor->z.fixed - 0x40000, 20);
            Engine_ActorSetSpritePriority(0, 3);
        }
        for (i = 15; i <= 18; i++) {
            other = Object_GetById(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20))
                Engine_ActorSetSpritePriority(i, 3);
        }
        Engine_ActorSetSpriteFlags(Object_GetById(id), 0);
        actor->unknown_22 = 0;
        motion = &actor->motion_flags;
        *motion = 3;
        ((union FieldObject *)actor)->effect.velocity_y = 0x1999;
        ((union FieldObject *)actor)->effect.velocity_x = 0;
        Engine_MapCopyCellAttributes(6, 44, 1, 1, Data_02005164[slot].x, Data_02005164[slot].z);
        OverlayObject_WaitUntilIdle(actor);
        Engine_AudioPlayCue(188);
        actor->collision_flags = 0;
        *motion = 0;
        actor->y.fixed = -0x100000;
        Engine_ActorSetSpritePriority(id, 3);
        *flags = 2;
        Engine_MapCopyCellAttributes(73, 48, 1, 1, Data_02005164[slot].x, Data_02005164[slot].z);
        Engine_ActorSetSpritePriority(0, priority);
        Object_GetById(0)->priority_flags |= 1;
        for (i = 15; i <= 18; i++) {
            other = Object_GetById(i);
            if (id != i
                && (actor->x.fixed >> 20) == (other->x.fixed >> 20)
                && (actor->z.fixed >> 20) - 1 == (other->z.fixed >> 20)) {
                Engine_ActorSetSpritePriority(i, 1);
                Object_GetById(i)->priority_flags |= 1;
            }
        }
        Engine_ObjectDispatchRelease(effect);
        if (Engine_GameFlagIsSet(0x308)) {
            Engine_EventEnd();
            return;
        }
        a = Object_GetById(15);
        b = Object_GetById(16);
        c = Object_GetById(17);
        d = Object_GetById(18);
        if ((a->priority_flags & b->priority_flags & c->priority_flags & d->priority_flags) & 2) {
            struct SwitchEffect *first;
            struct SwitchEffect *second;

            Engine_CameraSetSpeed(0x10000, 0x2000);
            Object_PlaceCurrentWithinCameraBounds(14, 1);
            Engine_CameraWaitForMove();
            first = SceneEffect_SpawnEffect284AtCell(136, 0x308, Data_0200577c);
            Engine_EventWait(30);
            Engine_CameraSetSpeed(0x6666, 0xccc);
            Engine_CameraMoveTo(0xd80000, -1, 0x2780000, 1);
            ObjectDispatch_WaitForValue16(first);
            Engine_ObjectSetScript((struct FieldActor *)first, Data_020057c8);
            second = SceneEffect_SpawnEffect284AtCell(216, 0x2f8, Data_02005ac8);
            while (first->active != 0 || second->active != 0) {
                if (first->finished != 0 || second->finished != 0) {
                    Engine_EventWait(30);
                    Engine_MapAnimateCells(Data_02005d3c, 77, 35);
                    Engine_MapCopyCellAttributes(13, 35, 1, 1, 13, 36);
                    Engine_GameFlagSet(0x308);
                    break;
                }
                Engine_TaskWait(1);
            }
        }
    }
    Engine_EventEnd();
}
