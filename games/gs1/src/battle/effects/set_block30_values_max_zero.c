#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "effect_0809b11c.h"
#include "fixed_math.h"

/* battle/effects/set/fx_set_block30_values_max_zero.c */
/* battle/effects/set/set_block30_values_max_zero.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct Fields_08095214 {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void BattleFx_SetBlock30ValuesMaxZero(void)
{
    struct Fields_08095214 *state = (struct Fields_08095214 *)Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 0x7FFF;
    state->field_1f82 = 0;
}

/* battle/effects/set/set_block30_values_12_zero.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct Fields_08095240 {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void BattleFx_SetBlock30Values12Zero(void)
{
    struct Fields_08095240 *state = (struct Fields_08095240 *)Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 12;
    state->field_1f82 = 0;
}

/* battle/effects/set/set_block30_values_128_one.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

struct Fields_08095268 {
    u8 filler[0x1F80];
    u16 field_1f80;
    u16 field_1f82;
};

void BattleFx_SetBlock30Values128One(void)
{
    struct Fields_08095268 *state = (struct Fields_08095268 *)Runtime_AllocateBlock(30, 0x1F88);
    state->field_1f80 = 128;
    state->field_1f82 = 1;
}

/* object/motion/aim/move_halfway_toward_target.c */
struct Object_08095348 {
    u8 filler0[8];
    s32 x;
    s32 y;
    s32 z;
    u8 filler14[0x54];
    struct Object_08095348 *target;
};

void Motion_MoveHalfwayTowardTarget(struct Object_08095348 *object)
{
    struct Object_08095348 *target = object->target;

    object->x += (target->x - object->x) / 2;
    object->y += (target->y - object->y) / 2;
    object->z += (target->z - object->z) / 2;
}

/* battle/effects/fx_run_page_effect_for_slot.c */
/* battle/effects/run/run_page_effect_for_slot.c */
struct MenuState {
    u8 padding_000[20];
    s32 slots[4];
    u8 padding_024[414 - 36];
    s16 mode;
};

s32 GameFlag_IsSet(s32);
void GameFlag_Set(void *);

void Battle_Reset(void);

void BattleFx_FinishHeavyImpact(s32);

void BattleFx_RunPageEffectForSlot(s32 slot, s32 page, void *entries)
{
    struct MenuState *state;
    s32 selection;

    state = *(struct MenuState **)ADDR_03001EBC;
    if (GameFlag_IsSet(366) != 0) {
        selection = 0;
        GameFlag_Set((u8 *)entries + page * 20 + 48);
    } else {
        selection = Battle_Apply(page, entries);
    }

    if (selection >= 0) {
        Battle_Reset();
        Battle_RunEffectForSlot();
        if (slot != -1) {
            if (state->mode == 3)
                BattleFx_RunAngledApproachPhases();

            if (page == 0)
                Battle_Do(slot);
            else if (page == 1)
                Runtime_ReleaseHeapBlock(slot);
            else if (page == 2)
                BattleFx_RunPhasedRadialParticleSequence(slot);
            else if (page == 3)
                BattleFx_FinishHeavyImpact(slot);

            state->slots[slot] = 0;
            if (state->mode == 3)
                Battle_unk3_4();
        }

        Battle_Place(selection, page, entries);
        Battle_ClearObjectFlag5bWhenMode3();
        BattleFx_FinishAction();
    }
}

/* battle/effects/runtime/upd/update_all_slots.c */
void BattleFx_UpdateAllSlots(void)
{
    s32 slot;
    s32 remaining_slots;

    slot = *(s32 *)ADDR_03001F30 + 0x58;
    remaining_slots = 0x17;
    do {
        remaining_slots -= 1;
        EffectSlot_Update((struct EffectSlot *)slot);
        slot += 0x48;
    } while (remaining_slots >= 0);
}

/* battle/effects/runtime/misc/clear_active_slots_and_schedule_updates.c */
void ScheduleCallback(void *);

void WaitFrames(s32);
extern u8 gRomEffectForSlot;

void BattleFx_ClearActiveSlotsAndScheduleUpdates(void)
{
    u8 *slot = *(u8 **)ADDR_03001F30;
    u8 *active_flag;
    s32 slot_index;

    ScheduleCallback(&gRomEffectForSlot);
    active_flag = slot + 157;
    slot += 88;
    for (slot_index = 23; slot_index >= 0; slot_index--) {
        u32 is_active = *(volatile u8 *)active_flag;
        active_flag += 72;
        if ((is_active << 24) != 0) {
            Battle_Do(slot);
        }
        slot += 72;
    }
    Runtime_ReleaseHeapBlock(56);
    WaitFrames(1);
}

void BattleFx_AdvanceSpinAngle(void *object)
{
    *(u16 *)((u8 *)object + 6) += 0x2000;
}

/* battle/effects/objects/run_angled_approach_phases.c */
struct Vec {
    s32 x;
    s32 y;
    s32 z;
};

struct Actor {
    u8 pad0[4];
    s32 screenX;
    s32 screenZ;
    u8 pad1[8];
    s32 x;
    s32 z;
    u8 pad2[28];
    s16 timer;
    u8 pad3[2];
    s16 yaw;
    s16 pitch;
    s8 phase;
};

struct Target {
    u8 pad0[8];
    struct Vec pos;
};

extern s32 gCell[];

struct Target *Battle_RunEffectForSlot(s32);

void BattleFx_RunAngledApproachPhases(struct Actor *actor)
{
    struct Target *target;
    struct Vec pos;

    target = Battle_RunEffectForSlot(gCell[125]);
    if (actor->phase == 0) {
        actor->yaw += 1;
        actor->pitch += 1;
        if (actor->timer == 60) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 1) {
        actor->pitch += 1;
        if (actor->timer == 40) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 2) {
        actor->pitch += 1;
        pos.x = target->pos.x;
        pos.y = target->pos.y + 0x140000;
        pos.z = target->pos.z;
        Battle_Do(&pos);
        actor->x += (pos.x - actor->x) / 8;
        actor->z += (pos.z - actor->z) / 8;
        if (actor->timer == 40) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 3) {
        actor->yaw -= 1;
        actor->pitch += 1;
        if (actor->timer == 60) {
            actor->timer = 0;
            actor->phase += 1;
        }
    } else if (actor->phase == 4) {
        Runtime_ReleaseHeapBlock(actor);
    }
    pos.x = actor->x;
    pos.z = actor->z;
    Battle_Place(actor->yaw << 16, actor->pitch << 11, &pos);
    actor->screenX = pos.x;
    actor->screenZ = pos.z;
}

/* battle/effects/heavy_impact/finish.c */
extern s32 gCell[];

void BattleFx_FinishHeavyImpact(s32 arg)
{
    s8 *head;
    struct {
        s32 x;
        s32 y;
        s32 z;
    } pos;
    s32 value;
    s8 *zptr;
    s32 base;
    s32 id;
    s32 count;
    void *ctx;
    void *work;

    id = arg;
    ctx = Object_GetById();
    /* The single-pass block preserves the first call's argument schedule. */
    do
    {
        if (ctx == ((void *)0))
        {
            return;
        }
        Battle_RunFinish();
        base = (*((s32 *)0x03001F30));
        Battle_unk2_4(0x20118C);
        Audio_PlayCue(0xAD);
        Motion_SetVarCbAndRefresh(id, 1);
    }
    while (0);
    Audio_PlayCue(0xAE);
    Motion_SetVarCbAndRefresh(id, 1);
    /* A second boundary gives the third repeated call its observed order. */
    do
    {
        Audio_PlayCue(0xAF);
        Motion_SetVarCbAndRefresh(id, 1);
        WaitFrames(0x14);
        Audio_PlayCue(0x8C);
        (*((s32 *)(((s8 *)ctx) + 0x6C))) = 0x0809592D;
        WaitFrames(0x28);
        Audio_PlayCue(0x99);
        Motion_Launch(id, 0xC, 0x16);
        pos.x = ((s32)(*((s32 *)(((s8 *)ctx) + 8))));
        pos.y = ((s32)(*((s32 *)(((s8 *)ctx) + 0xC))));
        pos.z = ((s32)(*((s32 *)(zptr = (((s8 *)ctx) + 0x10)))));
        NormalizeVector(&pos);
        Object_Destroy(ctx);
        Audio_PlayCue(0xA4);
        work = (base + 0x58);
        count = 0x17;
        do
        {
            Battle_unk4_4(work, 0x11C, pos.x, pos.z);
            EffectSlot_SetCallback(work, 0x08095939);
            /* This boundary keeps the work pointer ahead of the constant. */
            do
            {
            EffectSlot_SetObjectMode(work, 7);
            }
            while (0);
            ObjectGroup_SetChildValueUnlessFifteen(*((s32 *)(head = (((s8 *)work) + 0))), ((u32)(Random16() * 7)) >> 0x10);
            value = (Battle_unk6_2(Random16(), 3) + 0x10000);
            (*((s32 *)(((s8 *)work) + 0x2C))) = value;
            (*((s32 *)(((s8 *)work) + 0x28))) = value;
            count = (count - 1);
            WaitFrames(1);
            work += 0x48;
        }
        while (count >= 0);
    }
    while (0);
    WaitFrames(0x3C);
    Motion_ArmCb(gCell[125], 0x4000, 0);
    WaitFrames(0x14);
    Object_SetMode(Object_GetById(gCell[125]), 0x1C);
    WaitFrames(0x28);
    Audio_PlayCue(0xA4);
    WaitFrames(0x64);
    Shop_InitEffect();
    BattleFx_ClearActiveSlotsAndScheduleUpdates();
}

/* battle/effects/objects/set_alternating_words.c */
extern u32 gIw;
extern u32 gRomSetAlternatingWords[];

void BattleFx_SetObjectAlternatingWords(u8 *object)
{
    u32 *table = gRomSetAlternatingWords;
    u32 index = (gIw >> 2) & 1;
    u32 value = index[table];
    *(u32 *)(object + 0x18) = value;
    *(u32 *)(object + 0x1C) = value;
}

/* battle/effects/obj/advance_object_field6_with_ramp.c */
void BattleFx_AdvanceObjectField6WithRamp(void *obj)
{
    u32 step;

    step = FIELD_AT_OFFSET(obj, s16 *, 0x64) * 0x50;
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + step + 0x1000);
    if (step < 0x1000U) {
        FIELD_AT_OFFSET(obj, s16 *, 0x64) = (s16)((u16)FIELD_AT_OFFSET(obj, s16 *, 0x64) + 1);
    }
}

/* battle/effects/obj/shrink_object_and_destroy_slow.c */
void Object_Destroy();

void BattleFx_ShrinkObjectAndDestroySlow(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) + 0xFFFFFE40;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0x1C) + 0xFFFFFE40);
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + 0x2000);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x3000) {
        Object_Destroy();
    }
}

/* battle/effects/obj/shrink_object_and_destroy_fast.c */
/* battle/effects/obj/shrink_object_and_destroy_fast.c */
void Object_Destroy();

void BattleFx_ShrinkObjectAndDestroyFast(void *obj)
{
    s32 scale;

    scale = FIELD_AT_OFFSET(obj, s32 *, 0x18) + 0xFFFFFC00;
    FIELD_AT_OFFSET(obj, s32 *, 0x1C) = (s32)(FIELD_AT_OFFSET(obj, s32 *, 0x1C) + 0xFFFFFC00);
    FIELD_AT_OFFSET(obj, u16 *, 6) = (u16)(FIELD_AT_OFFSET(obj, u16 *, 6) + 0x2000);
    FIELD_AT_OFFSET(obj, s32 *, 0x18) = scale;
    if (scale < 0x3000) {
        Object_Destroy();
    }
}

/* battle/effects/objects/update_descending_orbit.c */
struct Triple08095fcc {
    s32 x;
    s32 y;
    s32 z;
};

struct Object08095fcc {
    u8 unknown_00[8];
    s32 x;
    s32 y;
    s32 z;
    u8 unknown_14[80];
    u16 timer;
    s16 angle;
};

extern s32 gCell[];

s32 Object_GetById(u32);
void RotateVectorByMagnitude(s32, s32, void *);
void Object_Destroy();

void BattleFx_UpdateDescendingOrbitObject(struct Object08095fcc *arg)
{
    struct Triple08095fcc local;
    struct Object08095fcc *other;
    s32 raw;
    s16 value;
    s32 y;

    other = (struct Object08095fcc *)Object_GetById(gCell[125]);
    raw = arg->timer - 1;
    arg->timer = raw;
    value = arg->timer;
    local.x = other->x;
    local.z = other->z;
    RotateVectorByMagnitude(value * 0x6666,
                  (value << 11) + arg->angle,
                  &local);
    arg->x = local.x;
    arg->z = local.z;
    y = arg->y + 0xFFFF0000;
    arg->y = y;
    if (y < other->y + 0x140000)
        Object_Destroy(arg);
}

/* battle/effects/particles/update_radial_spread.c */
struct Output_08096048 {
    s32 x;
    s32 y;
    s32 z;
};

struct PositionSource_08096048 {
    u8 padding00[8];
    struct Output_08096048 position;
};

extern s32 gCell[];
extern u32 gIw;

s32 Object_GetById(u32);
u32 Random16(void);
/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
#define Rand Random16
void RotateVectorByMagnitude(s32, s32, void *);
void NormalizeVector(void *);
void Audio_PlayCue(s32);

void BattleFx_UpdateRadialSpread(struct EffectSlot *effect)
{
    struct Output_08096048 position;
    struct PositionSource_08096048 *source;
    s32 state;
    u32 random;

    source = (struct PositionSource_08096048 *)
        Object_GetById(gCell[125]);
    state = effect->state;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y;
        position.z = source->position.z;

        random = Rand() * 10 + 0xa0000;
        RotateVectorByMagnitude(
            random,
            Rand(),
            &position);
        NormalizeVector(&position);

        effect->origin_x = position.x;
        effect->origin_z = position.z;
        effect->x = position.x;
        effect->z = position.z;
        position.x = effect->x;
        position.z = effect->z;

        RotateVectorByMagnitude(0x780000, 0xc000, &position);
        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->acceleration = 0x10000;
        effect->max_speed = 0x50000;
        effect->flag42 = state;
        effect->state++;

        if ((gIw & 1) != 0)
            Audio_PlayCue(0x90);
    } else if (state == 1) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            effect->state--;
    } else if (state == 2) {
        if (EffectSlot_HasReachedTarget(effect) == 0)
            Battle_RunAndDestroyFast(effect);
    }
}

/* battle/effects/fx_update_random_target_particle.c */
/* battle/effects/particles/update_random_target_particle.c */
struct EffectVector {
    s32 x;
    s32 y;
    s32 z;
};

struct EffectPositionSource {
    u8 unknown_00[8];
    struct EffectVector position;
};

extern s32 gCell[];

struct EffectPositionSource *Object_GetById(s32 id);
u32 Random16(void);
void RotateVectorByMagnitude(
    s32 magnitude,
    s32 angle,
    struct EffectVector *position);
void NormalizeVector(struct EffectVector *position);
void Audio_PlayCue(s32 cue);

void BattleFx_UpdateRandomTargetParticle(struct EffectSlot *effect)
{
    struct EffectPositionSource *source;
    struct EffectVector position;
    s8 *state_pointer;
    s32 state;

    source = Object_GetById(gCell[125]);
    state_pointer = &effect->state;
    state = *state_pointer;

    if (state == 0) {
        position.x = source->position.x;
        position.y = source->position.y + Random16() * 5 + 0xf0000;
        position.z = source->position.z;
        NormalizeVector(&position);
        RotateVectorByMagnitude(
            Random16() * 6 + 0x20000,
            Random16(),
            &position);

        effect->target_x = position.x;
        effect->target_z = position.z;
        effect->x = position.x;
        effect->z = position.z - 0x640000;
        effect->acceleration = 0x30000;
        effect->max_speed = Random16() * 3 + 0x30000;
        effect->scale_x = 0x10000;
        effect->scale_y = 0x10000;
        effect->flag42 = state;
        effect->flag41 = 1;
        (*state_pointer)++;
    } else if ((u8)(effect->state - 1) <= 1) {
        if (EffectSlot_HasReachedTarget(effect) == 0) {
            position.x = effect->x;
            position.z = effect->z;
            RotateVectorByMagnitude(0xc0000, Random16(), &position);
            effect->target_x = position.x;
            effect->target_z = position.z;
            effect->flag41 = 0;
            effect->speed = 0x10000;
            effect->acceleration = 0;
            effect->max_speed = Random16() + 0x23333;
            effect->scale_x = 0x8000;
            effect->scale_y = 0x8000;
            Audio_PlayCue(143);

            if (*state_pointer == 1)
                (*state_pointer)--;
            else
                (*state_pointer)++;
            effect->callback_delay = 6;
        }
    } else if (state == 3) {
        Battle_RunAndDestroyFast(effect);
    }
}

/* battle/effects/motion/halve_distance_to_target.c */
struct Object_08096574 {
    u8 padding000[8];
    s32 x;
    s32 y;
    s32 z;
    u8 padding014[84];
    struct Object_08096574 *target;
};

void BattleFx_HalveDistanceToTarget(struct Object_08096574 *object)
{
    struct Object_08096574 *target = object->target;

    object->x += (target->x - object->x) / 2;
    object->y += (target->y - object->y) / 2;
    object->z += (target->z - object->z) / 2;
}
