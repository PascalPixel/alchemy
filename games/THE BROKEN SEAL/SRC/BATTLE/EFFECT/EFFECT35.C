#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "FIELDOBJ.H"
/* Battle effect: spawn the pair of scaled objects that follow the linked
   object in mirrored arcs, one for each scaled-arc update callback. */
#include "TYPES.H"
#include "GAME_STATE.H"
#include "FX_SCENE.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_SLOT.H"
#include "GLOBAL_CELLS.H"
#include "DMA.H"
#include "SCENE_IDS.H"
#include "OBJECT_LOOKUP.H"
#include "SYSTEM.H"




/* The existing packed arc-constructor view is kept only at its measured
   alias boundary; other callbacks consume the shared object owners. */
struct ArcChild {
    u8 pad00[22];
    u8 frame;
};

struct ArcSprite {
    u8 pad00[4];
    u16 y:8;
    u16 affine:1;
    u16 double_size:1;
    u16 mode:2;
    u16 mosaic:1;
    u16 color:1;
    u16 shape:2;
    u16 x:9;
    u16 unused:3;
    u16 hflip:1;
    u16 vflip:1;
    u16 size:2;
    u16 tile:10;
    u16 priority:2;
    u16 palette:4;
    u8 pad0a[18];
    u8 slot;
    u8 active:1;
    u8 display_flags:7;
    u8 pad1e[8];
    u8 flags;
    u8 pad27;
    struct ArcChild *child;
};

struct ArcObject {
    u8 pad00[8];
    s32 x;
    s32 y;
    s32 z;
    s32 terrain_height;
    s32 scale_x;
    s32 scale_y;
    u8 pad20[0x23 - 0x20];
    u8 priority_flags;
    u8 pad24[0x50 - 0x24];
    struct ArcSprite *sprite;
    u8 pad54[1];
    u8 motion_flags;
    u8 pad56[0x64 - 0x56];
    u16 counter;
    u8 pad66[2];
    struct ArcObject *linked_object;
    void (*update)(struct FieldActor *);
};

struct ResourceTableEntry {
    u16 value;
    u16 unknown:5;
    u16 tile:10;
    u16 last:1;
};

extern struct BattleFxScene *gEffectWork;
extern struct ResourceTableEntry ResourceTableEntries[];
struct FieldActor *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
s32 AnimationObjects_SelectAnimationFar(struct AnimationObject *sprite, s32 animation);
void Resource_ResetEntry(u32 index);
void BattleFx_UpdateScaledArcObjectA(struct FieldActor *obj);
void BattleFx_UpdateScaledArcObjectB(struct FieldActor *obj);

extern u32 gFrameCount;
s32 __umodsi3(s32, s32);
void Animation_ApplyChildValuesFar(s32, s32);

struct EffectOrigin {
    u8 padding00[4];
    s32 x;
    s32 y;
    s32 z;
};

struct BattleSceneState {
    u8 padding000[0xcc0];
    u16 unknown_cc0;
    u8 padding0cc2[4];
    s8 running;
};

extern u8 gWorkSlot[];

extern const u8 Data_0809c410[];
void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
struct BattleAction *BattleAction_Get(s32 action);
s32 ResourceTable_CountFreeBlocks(void);
void BattleFx_SetupObjectPair(s32 first, s32 second);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void BattleFx_UpdateAllEffectSlots(void);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))
void Vector_AddPolarOffset(s32, s32, void *);

void BattleFx_SpawnScaledArcObjects(struct FieldActor *link);

void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked)
{
    /* FAKEMATCH: the existing scalar-coordinate and packed OAM constructor
       view preserves the native alias ordering and 312-byte extent; the
       shared coordinate/part views change register allocation and add four bytes. */
    struct ArcObject *link = (struct ArcObject *)linked;
    struct BattleFxScene *scene = gEffectWork;
    struct ArcObject *source = (struct ArcObject *)scene->main_object;
    struct ArcObject *objects[2];
    struct ArcObject *object;
    struct ArcSprite *sprite;
    s32 i;

    for (i = 0; i <= 1; i++) {
        object = (struct ArcObject *)Object_CreateFar(26, link->x, link->y, link->z);
        objects[i] = object;
        if (object == 0)
            continue;
        object->terrain_height = link->terrain_height;
        sprite = object->sprite;
        object->motion_flags = 0;
        object->counter = 0;
        object->linked_object = link;
        object->scale_x = object->scale_y = 0x1999;
        if (sprite == 0)
            continue;
        AnimationObjects_SelectAnimationFar((struct AnimationObject *)sprite, 0);
        sprite->flags = 0;
        Resource_ResetEntry(sprite->slot);
        sprite->slot = scene->tile_slot;
        sprite->active = 1;
        sprite->tile = ResourceTableEntries[sprite->slot].tile;
        sprite->color = 0;
        sprite->shape = 1;
        sprite->size = 2;
        sprite->child->frame = 0;
    }
    objects[0]->update = BattleFx_UpdateScaledArcObjectB;
    objects[0]->sprite->priority = 0;
    objects[1]->update = BattleFx_UpdateScaledArcObjectA;
    objects[1]->sprite->priority = source->sprite->priority;
    objects[1]->priority_flags = 2;
}

void BattleFx_FlickerObjectAndTick(s32 arg0)
{
    /* FAKEMATCH: the existing volatile frame-counter reads are retained;
       ordinary reads merge across this callback and change native scheduling. */
    if (((*(volatile s32 *)&gFrameCount) & 2) != 0) {
        Animation_ApplyChildValuesFar(arg0, 7);
    } else {
        Animation_ApplyChildValuesFar(arg0, 0);
    }
    if (((*(volatile s32 *)&gFrameCount) & 15) == 0) {
        BattleFx_SpawnScaledArcObjects(arg0);
    }
}

void BattleFx_CycleObjectValueByCounter(s32 arg0)
{
    /* FAKEMATCH: the existing volatile frame-counter reads are retained;
       ordinary reads merge across this callback and change native scheduling. */
    if (((*(volatile s32 *)&gFrameCount) & 1) != 0) {
        s32 value = __umodsi3((s32)((unsigned int)(*(volatile s32 *)&gFrameCount) >> 1), 6);

        Animation_ApplyChildValuesFar(arg0, value);
    }
    if (((*(volatile s32 *)&gFrameCount) & 15) == 0) {
        BattleFx_SpawnScaledArcObjects(arg0);
    }
}

void BattleFx_UpdateAllEffectSlots(void)
{
    s32 p;
    s32 cnt;

    p = (s32)gEffectWork->slots;
    cnt = 0x17;
    do {
        cnt -= 1;
        EffectSlot_Update((struct EffectSlot *)p);
        p += sizeof(struct EffectSlot);
    } while (cnt >= 0);
}

/* Battle effect: prepare the shared effect work for a battle action. The
   work block is cleared when the battle has not set it up already, the action
   and its animation class are recorded, and on the first run the effect
   position, object pair, tile slot and slot-update callback are set up. */
void BattleFx_LoadActionEffectResources(s32 action, s32 mode)
{
    struct BattleSceneState *scene = *(struct BattleSceneState **)(gWorkSlot + 27 * 4);
    struct EffectOrigin *origin = *(struct EffectOrigin **)(gWorkSlot + 8 * 4);
    struct BattleFxScene *work;
    s32 running;

    running = scene->running;
    if (running == 0) {
        volatile u32 fill;

        work = Runtime_AllocateHeapBlock(56, sizeof *work);
        fill = running;
        Dma_Set((const void *)&fill, work, 0x85000000 | (sizeof *work / 4), (volatile u32 *)0x040000d4);
    } else {
        work = *(struct BattleFxScene **)(gWorkSlot + 56 * 4);
    }
    work->action = action;
    work->animation = BattleAction_Get(action)->type_0c;
    running = scene->running;
    if (running != 0)
        return;
    work->free_blocks = 0x200 - ResourceTable_CountFreeBlocks();
    work->mode = mode;
    work->visible = 1;
    work->enabled = 1;
    work->active = 1;
    work->message_mode = 1;
    work->saved_x = origin->x;
    work->saved_y = origin->y;
    work->saved_z = origin->z;
    if ((s16)gGameState.map == (s32)&SceneId_MakyuriIriguchi)
        work->lit = 1;
    if ((s16)gGameState.map == (s32)&SceneId_MakyuriHeya2)
        work->lit = 1;
    BattleFx_SetupObjectPair(gGameState.selected_actor, -1);
    if (work->animation != 8)
        scene->unknown_cc0 = running;
    work->tile_slot = Resource_FindFreeEntry();
    VramBlock_LoadCached((s16)work->tile_slot, 256, Data_0809c410);
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateAllEffectSlots), 0xc80);
}

void BattleFx_SetupObjectPair(s32 first_object_id, s32 second_object_id)
{
    void *first_object; void *second_object; s32 facing_quadrant; void *state;
    state = gEffectWork;
    FIELD_AT_OFFSET(state, s16, 0x18) = first_object_id;
    first_object = ObjectTable_Get((s16)first_object_id);
    FIELD_AT_OFFSET(state, s16, 0x1A) = second_object_id;
    FIELD_AT_OFFSET(state, s32 *, 0x10) = (s32)first_object;
    second_object = ObjectTable_Get((s16)second_object_id);
    facing_quadrant = (FIELD_AT_OFFSET(first_object, u16, 6) + 0x2000) & 0xC000;
    FIELD_AT_OFFSET(state, s32 *, 0x14) = (s32)second_object;
    FIELD_AT_OFFSET(state, s32 *, 0) = facing_quadrant;
    if (second_object != 0) {
        FIELD_AT_OFFSET(state, s32 *, 0x38) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0x6C);
        FIELD_AT_OFFSET(state, s32 *, 0x3C) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0);
        {
            u8 object_variant = (u8)FIELD_AT_OFFSET(FIELD_AT_OFFSET(FIELD_AT_OFFSET(second_object, void **, 0x50), void **, 0x28), u8, 5);
            FIELD_AT_OFFSET(state, u8, 0x44) = object_variant;
        }
        FIELD_AT_OFFSET(state, s32 *, 4) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 8);
        FIELD_AT_OFFSET(state, s32 *, 0xC) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0x10);
        FIELD_AT_OFFSET(state, s32 *, 8) = (s32)FIELD_AT_OFFSET(second_object, s32 *, 0xC);
        return;
    }
    FIELD_AT_OFFSET(state, s32 *, 4) = (s32)FIELD_AT_OFFSET(first_object, s32 *, 8);
    FIELD_AT_OFFSET(state, s32 *, 0xC) = (s32)FIELD_AT_OFFSET(first_object, s32 *, 0x10);
    FIELD_AT_OFFSET(state, s32 *, 8) = (s32)FIELD_AT_OFFSET(first_object, s32 *, 0xC);
    Vector_AddPolarOffset(0x100000, facing_quadrant, (u8 *)state + 4);
}

void EffectRuntime_StopCurrentObject(void)
{
    struct BattleFxScene *work = gEffectWork;
    u8 *object = work->main_object;

    *(s32 *)(object + 0x6c) = 0;
    Animation_ApplyChildValuesFar(object, 0);
    WaitFrames(1);
}
