#include "TYPES.H"
#include "EFFECT_0809B11C.H"

#if defined(TBS_EDITION_EN)
/* The other editions keep their code here in their scaffolds for now. */

struct ArcPulseEntry {
    u8 unknown_00[5];
    u8 mode;
};

struct ArcPulseGroup {
    u8 unknown_00[37];
    u8 lit;
    u8 pulse;
    u8 unknown_27;
    struct ArcPulseEntry *entry;
};

struct ArcEffectObject {
    u8 unknown_00[6];
    u16 angle;
    u8 unknown_08[0x48];
    struct ArcPulseGroup *records;
    u8 unknown_54[0x10];
    s16 phase;
    s16 step;
    u8 unknown_68[4];
    void *callback;
};

struct ArcEffectScene {
    u8 unknown_000[16];
    struct ArcEffectObject *object;
    u8 unknown_014[0x706];
    s16 tile_slot;
};

extern struct ArcEffectScene *gEffectWork;
extern s32 gGameState[];
extern const u8 BattleFx_ArcSparkTiles[];

void WaitFrames(s32);
s32 Resource_ResetEntry(s32);
s32 VramBlock_LoadCached(u32, u32, const void *);
s32 Resource_FindFreeEntry(void);
s32 Scheduler_AddOrUpdateCallback(void (*)(void), s32);
void Scheduler_RemoveCallback(void (*)(void));
void ObjectDispatch_SetSingleChildField26Far(struct ArcEffectObject *, s32);
void Animation_ApplyChildValuesFar(struct ArcEffectObject *, s32);
void UiText_DrawMessage(s32, s32);
s32 GameFlag_TestFar(s32);
void Audio_PlayCue(s32);
void BattleFx_UpdateEffect16State(void);
void BattleFx_UpdatePairedArcSpawner(void);

/* Battle effect 16, the paired arc: load the spark tiles, start the arc
   spawner, pulse the caster's glow twenty times, then run the state
   callback and restore the caster. */
void RunBattleEffect16(void)
{
    struct ArcEffectScene *scene;
    struct ArcEffectObject *object;
    struct ArcPulseGroup *group;
    struct ArcPulseEntry *entry;
    u32 angle;
    s32 slot;
    s32 count;
    s32 index;

    scene = gEffectWork;
    object = scene->object;
    group = object->records;
    entry = group->entry;
    angle = object->angle;
    slot = Resource_FindFreeEntry();
    {
        s16 *tile_slot = &scene->tile_slot;
        s32 zero = 0;

        *tile_slot = slot;
        VramBlock_LoadCached((s16)slot, 0x100, BattleFx_ArcSparkTiles);
        gGameState[145] = 0x09600000;
        *(u8 *)&gGameState[146] = GameFlag_TestFar(0x145);
        Animation_ApplyChildValuesFar(object, zero);
        object->callback = BattleFx_UpdatePairedArcSpawner;
        object->phase = zero;
        object->step = zero;
    }
    Audio_PlayCue(0x8c);
    WaitFrames(15);
    object->phase = 1;
    WaitFrames(10);
    for (count = 0; count < 20; count++) {
        entry->mode = 7;
        group->lit = 1;
        WaitFrames(2);
        group->lit = 1;
        entry->mode = 0;
        group->pulse = 1;
        WaitFrames(3);
    }
    object->callback = NULL;
    object->angle = angle;
    Scheduler_AddOrUpdateCallback(BattleFx_UpdateEffect16State, 0xc80);
    WaitFrames(15);
    Audio_PlayCue(0xae);
    WaitFrames(55);
    Scheduler_RemoveCallback(BattleFx_UpdateEffect16State);
    index = 147;
    if (*(s16 *)&gGameState[index] != 0)
        ObjectDispatch_SetSingleChildField26Far(object, 2);
    else
        ObjectDispatch_SetSingleChildField26Far(object, 1);
    Animation_ApplyChildValuesFar(object, 0);
    Resource_ResetEntry(scene->tile_slot);
    UiText_DrawMessage(0x922, 1);
}
#endif

/* battle/effects/runtime/update_slot.c */
void EffectSlot_UpdateMotion(struct EffectSlot *effect);
void BattleFx_DrawScaledObject(struct EffectSlot *effect);

void EffectSlot_Update(struct EffectSlot *effect)
{
    if (effect->active != 0) {
        effect->age++;
        if (effect->callback_delay != 0)
            effect->callback_delay--;
        else if (effect->callback != 0)
            effect->callback(effect);
        if (effect->active != 0) {
            if (effect->update_motion != 0)
                EffectSlot_UpdateMotion(effect);
            if (effect->render != 0)
                BattleFx_DrawScaledObject(effect);
        }
    }
}
