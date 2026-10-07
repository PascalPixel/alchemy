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
#include "VRAM_BLOCK.H"
#include "RUNTIME_MEM.H"
#include "ANIMSPR.H"
#include "OBJECT_RUNTIME.H"
#include "SCRIPT_MOTION.H"
#include "OBJDISP.H"
#include "FIELDOBJ.H"
#include "BATTLE_RUNTIME.H"
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
#include "IO_REG.H"
#include "HEAP_STATE.H"

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
    u8 pad20[35 - 32];
    u8 priority_flags;
    u8 pad24[80 - 36];
    struct ArcSprite *sprite;
    u8 pad54[1];
    u8 motion_flags;
    u8 pad56[100 - 86];
    u16 counter;
    u8 pad66[2];
    struct ArcObject *linked_object;
    void (*update)(struct FieldActor *);
};

extern struct BattleFxScene *gEffectWork;
struct FieldActor *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);

extern volatile u32 gFrameCount;
void Animation_ApplyChildValuesFar(struct DispatchObject *object, u32 value);

struct EffectOrigin {
    u8 padding00[4];
    s32 x;
    s32 y;
    s32 z;
};

struct BattleSceneState {
    u8 padding000[3264];
    u16 unknown_cc0;
    u8 padding0cc2[4];
    s8 running;
};



void Vector_AddPolarOffset(s32, s32, void *);

void BattleFx_SpawnScaledArcObjects(struct FieldActor *link);

void BattleFx_SpawnScaledArcObjects(struct FieldActor *linked)
{
    /* FAKEMATCH: scalar coordinates and packed OAM preserve alias order and registers; shared views add four bytes. */
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
        sprite->tile = (gVramBlockCache[sprite->slot].offset >> 5) & 0x3ff;
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
    if ((gFrameCount & 2) != 0) {
        Animation_ApplyChildValuesFar((struct DispatchObject *)arg0, 7);
    } else {
        Animation_ApplyChildValuesFar((struct DispatchObject *)arg0, 0);
    }
    if ((gFrameCount & 15) == 0) {
        BattleFx_SpawnScaledArcObjects(arg0);
    }
}

void BattleFx_CycleObjectValueByCounter(s32 arg0)
{
    if ((gFrameCount & 1) != 0) {
        s32 value = (gFrameCount >> 1) % 6;

        Animation_ApplyChildValuesFar((struct DispatchObject *)arg0, value);
    }
    if ((gFrameCount & 15) == 0) {
        BattleFx_SpawnScaledArcObjects(arg0);
    }
}

void BattleFx_UpdateAllEffectSlots(void)
{
    s32 p;
    s32 cnt;

    p = (s32)gEffectWork->slots;
    cnt = 23;
    do {
        cnt -= 1;
        EffectSlot_Update((struct EffectSlot *)p);
        p += sizeof(struct EffectSlot);
    } while (cnt >= 0);
}

void BattleFx_LoadActionEffectResources(s32 action, s32 mode)
{
    struct BattleSceneState *scene = *(gWorkSlot + HEAP_SLOT_EVENT);
    struct EffectOrigin *origin = *(gWorkSlot + 8);
    struct BattleFxScene *work;
    s32 running;

    running = scene->running;
    if (running == 0) {
        volatile u32 fill;

        work = Runtime_AllocateHeapBlock(56, sizeof *work);
        fill = running;
        Dma_Set((const void *)&fill, work, DMA_ENABLE32 | DMA_32BIT | DMA_SRC_FIXED32 | (sizeof *work / 4), REG_DMA3);
    } else {
        work = *(gWorkSlot + 56);
    }
    work->action = action;
    work->animation = BattleAction_Get(action)->type_0c;
    running = scene->running;
    if (running != 0)
        return;
    work->free_blocks = 512 - ResourceTable_CountFreeBlocks();
    work->mode = mode;
    work->visible = 1;
    work->enabled = 1;
    work->active = 1;
    work->message_mode = 1;
    work->saved_x = origin->x;
    work->saved_y = origin->y;
    work->saved_z = origin->z;
    if (gGameState.map == (s32)&SceneId_MakyuriIriguchi)
        work->lit = 1;
    if (gGameState.map == (s32)&SceneId_MakyuriHeya2)
        work->lit = 1;
    BattleFx_SetupObjectPair(gGameState.selected_actor, -1);
    if (work->animation != 8)
        scene->unknown_cc0 = running;
    work->tile_slot = Resource_FindFreeEntry();
    VramBlock_LoadCached((s16)work->tile_slot, 256, Data_0809c410);
    Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllEffectSlots, 3200);
}

void BattleFx_SetupObjectPair(s32 first_object_id, s32 second_object_id)
{
    struct ScriptMotionObject *first_object;
    struct ScriptMotionObject *second_object;
    s32 facing_quadrant;
    struct BattleFxScene *state;

    state = gEffectWork;
    state->first_object_id = first_object_id;
    first_object = ObjectTable_Get((s16)first_object_id);
    state->second_object_id = second_object_id;
    state->main_object = first_object;
    second_object = ObjectTable_Get((s16)second_object_id);
    facing_quadrant = (first_object->facing + 0x2000) & 0xc000;
    state->child = second_object;
    state->angle = facing_quadrant;
    if (second_object != NULL) {
        state->saved_callback = second_object->hook;
        state->saved_script = (const s32 *)second_object->script;
        {
            struct AnimationObject *animation =
                ((struct ObjectRuntime *)second_object)->animation;
            u8 palette = animation->entries[0]->param;

            state->saved_palette = palette;
        }
        state->x = second_object->x;
        state->z = second_object->z;
        state->y = second_object->y;
        return;
    }
    state->x = first_object->x;
    state->z = first_object->z;
    state->y = first_object->y;
    Vector_AddPolarOffset(0x100000, facing_quadrant, &state->x);
}

void EffectRuntime_StopCurrentObject(void)
{
    struct BattleFxScene *work = gEffectWork;
    struct ScriptMotionObject *object = work->main_object;

    object->hook = NULL;
    Animation_ApplyChildValuesFar((struct DispatchObject *)object, 0);
    WaitFrames(1);
}
