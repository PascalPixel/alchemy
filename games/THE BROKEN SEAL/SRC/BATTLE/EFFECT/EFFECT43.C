#include "RESOURCE.H"
#include "GAME_STATE.H"
#include "ANIMSPR.H"
#include "TYPES.H"
#include "FX_SCENE.H"
#include "CALLBACK_SCHEDULER.H"
#include "EFFECT_SLOT.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"
#include "DMA.H"

extern char MsgMonstersAttackLess;



struct ArcEffectObject {
    u8 unknown_00[6];
    u16 angle;
    u8 unknown_08[0x48];
    struct AnimationObject *records;
    u8 unknown_54[0x10];
    s16 phase;
    s16 step;
    u8 unknown_68[4];
    void *callback;
};

extern struct BattleFxScene *gEffectWork;
extern const u8 BattleFx_ArcSparkTiles[];
void WaitFrames(s32);
void ObjectDispatch_SetSingleChildField26Far(struct ArcEffectObject *, s32);
void Animation_ApplyChildValuesFar(struct ArcEffectObject *, s32);
void UiText_DrawMessage(s32, s32);
s32 GameFlag_TestFar(s32);
void Audio_PlayCue(s32);
void BattleFx_UpdateEffect16State(void);
void BattleFx_UpdatePairedArcSpawner(void);

/* battle/effects/runtime/update_slot.c */
void EffectSlot_UpdateMotion(struct EffectSlot *effect);


void Object_ApplyProjectedPlacementFar(struct AnimationObject *sprite, s32 *position, s32 *scale, s32 mode);
u16 ArcTan2(s32 x, s32 y);
s32 FixedSqrt(s32 value);

void AnimationObjects_SelectAnimationFar(void *, s32);

void *Func_08009030(s32 kind);
u32 Random16(void);


void BattleFx_DrawScaledObject(struct EffectSlot *object);

/* Battle effect 16, the paired arc: load the spark tiles, start the arc
   spawner, pulse the caster's glow twenty times, then run the state
   callback and restore the caster. */
void RunBattleEffect16(void)
{
    struct BattleFxScene *scene;
    struct ArcEffectObject *object;
    struct AnimationObject *group;
    struct AnimationEntry *entry;
    u32 angle;
    s32 slot;
    s32 count;
    s32 index;

    scene = gEffectWork;
    object = scene->main_object;
    group = object->records;
    entry = group->entries[0];
    angle = object->angle;
    slot = Resource_FindFreeEntry();
    {
        s16 *tile_slot = &scene->arc_tile_slot;
        s32 zero = 0;

        *tile_slot = slot;
        VramBlock_LoadCached((s16)slot, 0x100, BattleFx_ArcSparkTiles);
        gGameState.unknown_244 = 0x09600000;
        gGameState.unknown_248[0] = GameFlag_TestFar(0x145);
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
        entry->param = 7;
        group->dirty = 1;
        WaitFrames(2);
        group->dirty = 1;
        entry->param = 0;
        group->flags = 1;
        WaitFrames(3);
    }
    object->callback = NULL;
    object->angle = angle;
    Scheduler_AddOrUpdateCallback((s32)(BattleFx_UpdateEffect16State), 0xc80);
    WaitFrames(15);
    Audio_PlayCue(0xae);
    WaitFrames(55);
    Scheduler_RemoveCallback((u32)(BattleFx_UpdateEffect16State));
    index = 147;
    if (*(s16 *)((u8 *)&gGameState + index * sizeof(s32)) != 0)
        ObjectDispatch_SetSingleChildField26Far(object, 2);
    else
        ObjectDispatch_SetSingleChildField26Far(object, 1);
    Animation_ApplyChildValuesFar(object, 0);
    Resource_ResetEntry(scene->arc_tile_slot);
    UiText_DrawMessage((s32)&MsgMonstersAttackLess, 1);
}

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

/* Draws an effect object at its position, scaled by its own and its
   sprite's factors, when it lies near the screen; flag 4 mirrors it about
   the floor line. */
void BattleFx_DrawScaledObject(struct EffectSlot *object)
{
    struct AnimationObject *sprite;
    s32 offset;
    s32 scale[2];
    s32 position[4];

    sprite = object->object;
    offset = 0;
    if (object->flags & 4)
        offset = 0x1fc0000 - object->z;
    scale[0] = Iwram_MulQ16(object->scale_x, sprite->scale);
    scale[1] = Iwram_MulQ16(object->scale_y, sprite->scale);
    position[0] = object->x;
    position[1] = offset;
    position[2] = object->z + offset;
    position[3] = 0;
    if (object->x > -0x200000 && object->x < 0x1100000 && object->z > -0x200000 && object->z < 0xe00000)
        Object_ApplyProjectedPlacementFar(sprite, position, scale, 0);
}

/* Steer an effect slot toward its target: snap to it once within 8.0,
   otherwise turn by at most the slot's turn step, accelerate up to
   the maximum speed and move along the heading. */
void EffectSlot_UpdateMotion(struct EffectSlot *effect)
{
    s32 dx;
    s32 dz;
    s32 distance;
    s32 ix;
    s32 iz;
    s32 speed;
    s16 angle;
    s32 heading;
    s32 facing;
    s16 turn;

    if (effect->target_x == EFFECT_NO_TARGET)
        return;
    dx = effect->target_x - effect->x;
    dz = effect->target_z - effect->z;
    if (effect->stop_at_target != 0) {
        ix = dx / 0x10000;
        iz = dz / 0x10000;
        distance = Iwram_Sqrt(ix * ix + iz * iz) << 16;
        if (distance < 0x800000)
            distance = FixedSqrt(Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz));
        if (distance <= 0x80000) {
            EffectSlot_SetPosition(effect, effect->target_x, effect->target_z);
            return;
        }
    }
    angle = ArcTan2(dz, dx);
    if (effect->flag42 != 0) {
        heading = effect->heading;
        turn = angle - heading;
        if ((turn >= 0 ? turn : -turn) >= effect->max_turn_step) {
            if (turn < 0) {
                if (-turn > effect->max_turn_step)
                    turn = -effect->max_turn_step;
            } else if (turn > effect->max_turn_step) {
                turn = effect->max_turn_step;
            }
            angle = turn + heading;
        }
    }
    facing = (u16)angle;
    effect->heading = facing;
    speed = effect->speed + effect->acceleration;
    if (speed > effect->max_speed)
        speed = effect->max_speed;
    angle = (s16)facing;
    effect->speed = speed;
    effect->x += Iwram_MulQ16(Trig_Cos(angle), speed);
    effect->z += Iwram_MulQ16(Trig_Sin(angle), speed);
}

u32 BattleFx_HasReachedTarget(struct EffectSlot *effect)
{
    u32 value;

    if (effect->stop_at_target == 0) {
        return 0;
    }
    value = (u32)effect->target_x ^ 0x80000000;
    return ((0u - value) | value) >> 31;
}

void EffectSlot_SetPosition(struct EffectSlot *effect, s32 x, s32 z)
{
    effect->target_x = EFFECT_NO_TARGET;
    effect->target_z = EFFECT_NO_TARGET;
    effect->x = x;
    effect->z = z;
    effect->speed = 0;
}

void EffectSlot_SetObjectMode(struct EffectSlot *effect, s32 mode)
{
    AnimationObjects_SelectAnimationFar(effect->object, mode);
}

void EffectSlot_SetCallback(struct EffectSlot *effect, EffectCallback callback)
{
    effect->callback = callback;
    effect->callback_delay = 0;
    effect->age = 0;
    effect->state = 0;
}

/* Effect slot: clear the slot, attach a new object of `kind`, place it at
   x, z with unit scale and acceleration, and start it active in mode 1. */
void EffectSlot_Initialize(struct EffectSlot *effect, s32 kind, s32 x, s32 z)
{
    s32 unit;
    s8 *object;
    volatile u32 zero;

    zero = 0;
    Dma_Set((const void *)&zero, effect, 0x85000000 | (sizeof *effect / 4), (volatile u32 *)0x040000d4);
    object = Func_08009030(kind);
    effect->object = object;
    if (object != NULL)
        object[9] &= ~0x0c;
    EffectSlot_SetPosition(effect, x, z);
    /* FAKEMATCH: 1.0 is held in a local, set before max_speed, so the object
       reload is scheduled ahead of the three unit stores */
    unit = 0x10000;
    effect->max_speed = 0x20000;
    effect->acceleration = effect->scale_y = effect->scale_x = unit;
    effect->origin_x = x;
    effect->origin_z = z;
    ((s8 *)effect->object)[38] = 0;
    effect->stop_at_target = 1;
    effect->flag42 = 1;
    effect->update_motion = 1;
    effect->render = 1;
    effect->active = 1;
    effect->random_value = Random16();
    effect->flags = 4;
    EffectSlot_SetObjectMode(effect, 1);
}

void BattleFx_ClearOwnedSlot(struct EffectSlot *slot)
{
    volatile u32 zero;
    if (slot->object)
        ResourceObject_ReleaseFar((struct ResourceObjectWork *)slot->object);
    zero = 0;
    Dma_Set(&zero, slot, 0x85000000 | (sizeof *slot / 4), (volatile u32 *)0x040000d4);
}
