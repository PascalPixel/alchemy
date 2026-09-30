#include "EFFECT_0809B11C.H"
#include "GLOBAL_CELLS.H"
#include "TYPES.H"
#include "DMA.H"
#include "SCENE_IDS.H"
extern u8 Data_03001f30[];

void BattleFx_UpdateAllEffectSlots(void)
{
    s32 p;
    s32 cnt;

    p = *(s32 *)((u32)&Data_03001f30) + 0x58;
    cnt = 0x17;
    do {
        cnt -= 1;
        EffectSlot_Update((struct EffectSlot *)p);
        p += 0x48;
    } while (cnt >= 0);
}

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

/* Battle effect: prepare the shared effect work for a battle action. The
   work block is cleared when the battle has not set it up already, the action
   and its animation class are recorded, and on the first run the effect
   position, object pair, tile slot and slot-update callback are set up. */

struct ActionEffectWork {
    u8 padding00[0x1c];
    u16 action;
    s16 animation;
    u8 enabled;
    u8 mode;
    u8 visible;
    u8 active;
    u8 padding24[0x21];
    u8 lit;
    u16 tile_slot;
    u16 unknown_48;
    u16 free_blocks;
    s32 x;
    s32 y;
    s32 z;
    u8 padding58[0x6c4];
    u8 slots_enabled;
};

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

struct GameStateActors {
    u8 padding000[0x1da];
    s16 scene;
    u8 padding1dc[0x18];
    s32 object_id;
};

struct BattleActionData {
    u8 padding00[12];
    u8 animation;
};

extern u8 gWorkSlot[];
extern struct GameStateActors gGameState;
extern const u8 Data_0809c410[];

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
struct BattleActionData *BattleAction_Get(s32 action);
s32 ResourceTable_CountFreeBlocks(void);
void BattleFx_SetupObjectPair(s32 first, s32 second);
s32 Resource_FindFreeEntry(void);
s32 VramBlock_LoadCached(s32 slot, s32 size, const void *source);
void BattleFx_UpdateAllEffectSlots(void);
s32 Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);

void BattleFx_LoadActionEffectResources(s32 action, s32 mode)
{
    struct BattleSceneState *scene = *(struct BattleSceneState **)(gWorkSlot + 27 * 4);
    struct EffectOrigin *origin = *(struct EffectOrigin **)(gWorkSlot + 8 * 4);
    struct ActionEffectWork *work;
    s32 running;

    running = scene->running;
    if (running == 0) {
        volatile u32 fill;

        work = Runtime_AllocateHeapBlock(56, 0x720);
        fill = running;
        Dma_Set((const void *)&fill, work, 0x850001c8, (volatile u32 *)0x040000d4);
    } else {
        work = *(struct ActionEffectWork **)(gWorkSlot + 56 * 4);
    }
    work->action = action;
    work->animation = BattleAction_Get(action)->animation;
    running = scene->running;
    if (running != 0)
        return;
    work->free_blocks = 0x200 - ResourceTable_CountFreeBlocks();
    work->mode = mode;
    work->visible = 1;
    work->enabled = 1;
    work->active = 1;
    work->slots_enabled = 1;
    work->x = origin->x;
    work->y = origin->y;
    work->z = origin->z;
    if (gGameState.scene == (s32)&SceneId_MakyuriIriguchi)
        work->lit = 1;
    if (gGameState.scene == (s32)&SceneId_MakyuriHeya2)
        work->lit = 1;
    BattleFx_SetupObjectPair(gGameState.object_id, -1);
    if (work->animation != 8)
        scene->unknown_cc0 = running;
    work->tile_slot = Resource_FindFreeEntry();
    VramBlock_LoadCached((s16)work->tile_slot, 256, Data_0809c410);
    Scheduler_AddOrUpdateCallback(BattleFx_UpdateAllEffectSlots, 0xc80);
}
#endif
