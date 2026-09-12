#include "scene.h"
#include "effect_runtime.h"
#include "global_cells.h"
#include "types.h"
#include "battle_effect_runtime.h"
#include "object_lookup.h"
#include "fixed_math.h"
#include "object_effect.h"

/* battle/effects/random_particle/play_cue_and_start_emitter_on_target.c */
s32 WaitFrames(s32);
s32 Object_SetMode(s32, s32);

s32 Object_GetById(u32);
s32 Audio_PlayCue(s32);

s32 BattleFx_PlayCueAndStartEmitterOnTarget(s32 effect, s32 target, s32 mode)
{
    s32 object;
    s32 result;

    object = Object_GetById(target);
    result = 0;
    if (object != 0) {
        Audio_PlayCue(0x7C);
        Object_SetMode(object, 4);
        WaitFrames(0xC);
        result = Battle_Apply(effect, mode);
    }
    return result;
}

/* battle/runtime/reset_effect_counter.c */
/* battle/runtime/reset_effect_counter.c */
void Battle_ResetEffectCounter(void)
{
  void *runtime;
  void **cell;
  u8 *counter;
  int zero;
  cell = (void **)ADDR_03001EBC;
  runtime = *cell;
  counter = ((u8 *)runtime) + 0xCB6;
  zero = 0;
  *((s16 *)counter) = zero;
  if ((*((s16 *)(((u8 *)runtime) + 0xCB8))) != 0)
  {
    Battle_Check(0x2090);
  }
}

/* battle/effects/misc/find_matching_event.c */
/* battle/effects/misc/find_matching_event.c */
u8 *Ability_GetData(s32);

struct BattleEffectEventRecord {
    s32 flags;
    u16 metadata;
    s16 action_id;
    u8 unknown_08[4];
};

struct BattleEffectCharacter {
    u8 unknown_00[12];
    u8 group;
};

struct BattleEffectRuntime {
    u8 unknown_00[16];
    struct BattleEffectEventRecord *events;
};

struct BattleEffectValueRecord {
    u8 unknown_00[6];
    u16 value;
};

s32 BattleFx_FindMatchingEvent(s32 requested_flags, s32 group, void *result)
{
    struct BattleEffectRuntime *runtime =
        (struct BattleEffectRuntime *)gWork;
    struct BattleEffectEventRecord *event = runtime->events;
    s32 reference = ((struct BattleEffectValueRecord *)get(
        gCell.object_id))->value;
    s32 selected = BattleFx_SelectNearbyTargetObject(gCell.object_id, group);
    s32 alternate;
    s32 ignore_flags = 0;

    *(s32 *)result = selected;
    alternate = get_focused_object_collision();
    if (requested_flags == 0x70000005)
        ignore_flags = 1;

    while (event->flags != -1) {
        s32 high_value = (s16)event->metadata & 0xf000;
        s16 has_reference = event->metadata & 0x0800;
        s32 low_value = event->metadata & 0xff;

        if ((event->flags & 0x0f) == 5 &&
            GameFlag_IsConditionActive(event->action_id) != 0 &&
            (has_reference == 0 ||
             (u16)(high_value - reference + 0x17ff) <= 0x2ffe) &&
            ((struct BattleEffectCharacter *)(void *)Ability_GetData(
                ((u8 *)&event->flags)[1]))->group == group &&
            (ignore_flags ||
             (event->flags & 0x7000000f) == requested_flags)) {
            if ((event->flags & 0x80) != 0)
                return (s32)event;
            if ((event->flags & 0x10) != 0) {
                if (low_value == selected)
                    return (s32)event;
            } else if (low_value == alternate) {
                return (s32)event;
            }
        }
        event++;
    }
    return 0;
}

/* battle/effects/misc/execute_packed_ability_effect.c */
/* gCell is struct BattleWork (battle_effect_runtime.h); its
 * object_id field sits at the same 0x1f4 offset this owner reads as
 * selected_object, matching main:0808e23c's use of the same shared symbol. */

u8 *Ability_GetData(s32);

void BattleFx_SetupObjectPair(s32, s32);

void FieldEvent_RunTypeHandler(void);
void EffectRuntime_StopCurrentObject(void);

s32 BattleFx_ExecutePackedAbilityEffect(s32 packed)
{
    s32 output;
    s32 index;
    s32 mode;
    u8 object;
    void *first;
    void *second;

    index = packed & 0x3FF;
    mode = ((u32)packed >> 10) & 0xF;
    object = Ability_GetData(index)[0xC];
    ObjectTable_Get(gCell.object_id);
    first = (void *)Battle_Place(0x30000005, object, &output);
    second = (void *)Battle_Place(0x20000005, object, &output);
    Battle_ApplyPackedAbilityEffect(index, 0);
    BattleFx_SetupObjectPair(gCell.object_id, output);
    BattleFx_RunEventAction(first, mode, output);
    FieldEvent_RunTypeHandler();
    EffectRuntime_StopCurrentObject();
    BattleFx_RunEventAction(second, mode, output);
    Battle_RunPackedAbilityEffect();
    return 0;
}

/* battle/command/execute_selected_action.c */
struct BattleActionDefinition { u8 pad00[9]; u8 pp_cost; u8 pad0a[2]; u8 target_mode; };
struct BattleUnitRecord { u8 pad00[58]; s16 pp; };
struct BattleTargetCandidate { u8 pad00[4]; u16 flags; };
struct BattleCommandRuntime {
    u8 pad000[0x170]; s16 result_code; u8 pad172[0x2c]; s16 battle_mode;
    u8 pad1a0[0xb26]; u8 resolving_action;
};

/*
 * Declared struct BattleRuntime * to match the shared extern in
 * battle_effect_runtime.h; the local view of the same storage is obtained by
 * a cast below.
 */
extern struct BattleRuntime *gWork;
extern u8 Value_00000920;
/*
 * Returns u8 * to match the prototype shared with the other callers; the raw
 * pointer is cast to the local action-definition view below.
 */
u8 *Ability_GetData(s32);
/*
 * Returns void * because callers view the same record through different
 * structs; each casts the shared pointer to its own view locally.
 */
void *Runtime_GetObject(s32);
void *get(s32);
void Battle_InitializeRenderObject(void); void GameFlag_Clear(s32); s32 GameFlag_IsSet(s32);
void UiText_DrawQuantity(s32, s32); void UiText_DrawMessage(s32, s32);
s32 Object_CallSpawnRoutineAtOrigin(s32); void UiWork_FinalizePending(void);
/* Takes an s32 to match the definition of the packed effect argument. */

/*
 * Matches the shared prototype: s32-returning, with a void * out parameter.
 * Results are cast back to struct BattleTargetCandidate * here.
 */

void GameFlag_Set(s32); s32 BattleFx_SelectNearbyTargetObject(s32, s32); void BattleFx_ClearOutOfBoundsObjects(void);
void Battle_ApplyPackedAbilityEffect(s32, s32); void BattleFx_SetupObjectPair(s32, s32); void initialize(void);
/*
 * Returns s32 to match the shared prototype, although every call site
 * discards the value.
 */
s32 BattleFx_RunEventAction(void *, s32, s32); void BattleFx_DispatchRequestKind(void); void BattleFx_Run(void);
void stop_current_object(void); void BattleFx_ClearChildValueOnMismatch(void); void BattleFx_CleanupSceneObjects(void); void BattleFx_ClearAllObjects(void);

s32 BattleCommand_ExecuteSelectedAction(u32 encodedAction)
{
    s32 actionId = encodedAction & 0x3ff;
    struct BattleCommandRuntime *runtime = (struct BattleCommandRuntime *)gWork;
    struct BattleTargetCandidate *primary;
    struct BattleTargetCandidate *secondary;
    struct BattleTargetCandidate *tertiary;
    s32 actor;
    s32 targetId;
    s32 specialResult;
    s32 targetMode;
    s32 cost;
    s32 status;

    targetMode = ((struct BattleActionDefinition *)(void *)Ability_GetData(actionId))->target_mode;
    actor = (encodedAction >> 10) & 15;
    get(gCell.object_id);
    specialResult = 0;
    Battle_InitializeRenderObject();
    GameFlag_Clear(0x145);
    if (actor == 15) actor = 0;

    if (GameFlag_IsSet(0x17e)) {
        UiText_DrawQuantity(actor, 1); UiText_DrawQuantity(actionId, 4); UiText_DrawMessage(0x91f, 1);
        return 0;
    }
    if (runtime->battle_mode == 3 && actionId == 0x90) {
        UiText_DrawQuantity(actor, 1); UiText_DrawQuantity(0x90, 4); UiText_DrawMessage(0x91f, 1);
        return 0;
    }
    if (actionId == 0x95) {
        if (GameFlag_IsSet(0x144)) {
            UiText_DrawQuantity(actor, 1); UiText_DrawQuantity(0x95, 4); UiText_DrawMessage(0x921, 1);
            return 0;
        }
        UiText_DrawQuantity(0x95, 4); UiText_DrawMessage((s32)&Value_00000920, 13);
        status = Object_CallSpawnRoutineAtOrigin(1); UiWork_FinalizePending();
        if (status != 0) return 0;
        {
            u16 *work = (u16 *)&gCell;
            s32 a, b;
            a = work[288];
            work[224] = a;
            b = work[289];
            work[225] = b;
        }
        runtime->result_code = 999;
        specialResult = 1;
    }
    if (encodedAction & 0x2000) return Battle_CheckExecuteSelectedAction(encodedAction);

    if (actor <= 7) {
        cost = ((struct BattleActionDefinition *)(void *)Ability_GetData(actionId))->pp_cost;
        if (((struct BattleUnitRecord *)Runtime_GetObject(actor))->pp < cost) {
            UiText_DrawQuantity(actor, 1); UiText_DrawQuantity(actionId, 4); UiText_DrawMessage(0x91e, 1);
            if (specialResult)runtime->result_code = 0;
            return 0;
        }
        Battle_ApplyExecuteSelectedAction(actor, -cost);
    }

    primary = (struct BattleTargetCandidate *)Battle_Place(0x10000005, targetMode, &targetId);
    secondary = (struct BattleTargetCandidate *)Battle_Place(5, targetMode, &targetId);
    tertiary = (struct BattleTargetCandidate *)Battle_Place(0x50000005, targetMode, &targetId);
    targetId = -1;
    GameFlag_Set(0x140); GameFlag_Set(0x141);
    if (primary || secondary || tertiary) {
        targetId = BattleFx_SelectNearbyTargetObject(gCell.object_id, targetMode);
        if (secondary && (secondary->flags & 0x400)) {
            GameFlag_Clear(0x140); GameFlag_Clear(0x141);
        }
    } else GameFlag_Clear(0x141);

    if (runtime->battle_mode == 3) BattleFx_ClearOutOfBoundsObjects();
    Battle_ApplyPackedAbilityEffect(actionId, 0); runtime->resolving_action = 1;
    BattleFx_SetupObjectPair(gCell.object_id, targetId); initialize();
    BattleFx_RunEventAction(primary, actor, targetId);
    if (GameFlag_IsSet(0x140)) {
        if (GameFlag_IsSet(0x141)) BattleFx_DispatchRequestKind(); else BattleFx_Run();
    }
    stop_current_object(); BattleFx_RunEventAction(secondary, actor, targetId);
    if (GameFlag_IsSet(0x140)) BattleFx_ClearChildValueOnMismatch();
    GameFlag_Clear(0x140); GameFlag_Clear(0x141); runtime->resolving_action = 0;
    BattleFx_CleanupSceneObjects();
    if (runtime->battle_mode == 3) BattleFx_ClearAllObjects();
    return 0;
}

/* battle/effects/misc/has_matching_event5.c */
s32 BattleFx_HasMatchingEvent5(s32 effectId)
{
    s32 local;
    s32 result = Battle_Place(0x70000005, (u16)effectId, &local);
    return (u32)((-result) | result) >> 31;
}

/* battle/effects/misc/has_trigger.c */
u32 BattleFx_HasTrigger(u16 effectId)
{
    s32 result;

    result = Battle_CheckHasTrigger(effectId);
    return (u32)((0 - result) | result) >> 0x1F;
}

/* object/motion/pos/snap_to_terrain.c */
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

void Motion_SnapToTerrain(void *object)
{
    s32 angle;

    angle = Obj_Place(0, FIELD_AT_OFFSET(object, s32 *, 8), FIELD_AT_OFFSET(object, s32 *, 0x10));
    FIELD_AT_OFFSET(object, s32 *, 0xC) = angle;
    FIELD_AT_OFFSET(object, s32 *, 0x14) = angle;
}

/* effect_runtime/find_slot_by_object.c */
/* effect_runtime/find_slot_by_object.c */
/* effect_runtime/find_slot_by_object.c */
s32 EffectRuntime_FindSlotByObject(s32 object)
{
    s32 index;
    s32 found;
    struct EffectRuntimeSlot *slot;

    slot = EFFECT_RUNTIME->slots;
    found = -1;
    index = 0;
    if (slot->active == object) {
        found = 0;
    } else {
loop_2:
        index += 1;
        slot++;
        if (index <= 9) {
            if (slot->active == object) {
                found = index;
            } else {
                goto loop_2;
            }
        }
    }
    return found;
}

/* effect_runtime/set_mode5_and_play_cue.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(s32, s32);
s32 EffectRuntime_FindSlotByObject();
s32 Audio_PlayCue(s32);

void EffectRuntime_SetMode5AndPlayCue(void)
{
    s32 slot;
    s32 object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 5);
        }
        Audio_PlayCue(0x7D);
        WaitFrames(0xC);
    }
}

/* effect_runtime/set_mode7_and_launch.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(void *, s32);

s32 EffectRuntime_FindSlotByObject();
s32 Audio_PlayCue(s32);

void EffectRuntime_SetMode7AndLaunch(void)
{
    s32 slot;
    void *object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        WaitFrames(0x12);
        if (object != NULL) {
            Object_SetMode(object, 7);
        }
        Audio_PlayCue(0x92);
        if (object != NULL) {
            FIELD_AT_OFFSET(object, s32 *, 0x28) = 0x80000;
            Sys_Apply(object, 1);
        }
    }
}

/* effect_runtime/set_mode4_and_play_cue.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 WaitFrames(s32);
s32 Object_SetMode(s32, s32);
s32 EffectRuntime_FindSlotByObject();
s32 Audio_PlayCue(s32);

void EffectRuntime_SetMode4AndPlayCue(void)
{
    s32 slot;
    s32 object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 4);
        }
        Audio_PlayCue(0x7C);
        WaitFrames(0xC);
    }
}

/* effect_runtime/set_mode2.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 Object_SetMode(s32, s32);
s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_SetMode2(void)
{
    s32 slot;
    s32 object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != 0) {
            Object_SetMode(object, 2);
        }
    }
}

/* effect_runtime/get_current_object.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 EffectRuntime_FindSlotByObject();

s32 EffectRuntime_GetCurrentObject(void)
{
    s32 slot;

    slot = EffectRuntime_FindSlotByObject();
    if (slot == -1) {
        return 0;
    }
    { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; return *(s32 *)((s32)base + (s32)offset + (s32)0x11C); }
}

/* effect_runtime/clear_current_flags.c */
#define FIELD_AT_OFFSET(base, type, offset) \
    (*(type)((u8 *)(base) + (offset)))

s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_ClearCurrentFlags(void)
{
    s32 slot;
    void *object;

    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        { s32 base = *(s32 *)ADDR_03001EBC; s32 offset = slot * 8; object = *(void **)((s32)base + (s32)offset + (s32)0x11C); }
        if (object != NULL) {
            FIELD_AT_OFFSET(object, s8 *, 0x54) = 0;
        }
    }
}

/* effect_runtime/set_current_position.c */
s32 EffectRuntime_FindSlotByObject();

void EffectRuntime_SetCurrentPosition(s32 unused, s32 x, s32 y)
{
    s32 slot;
    s32 angle;
    s32 objectX;
    s32 objectY;
    void *slotData;
    void *slotBase;
    void *object;

    objectY = y;
    objectX = x;
    slot = EffectRuntime_FindSlotByObject();
    if (slot != -1) {
        slotBase = *(s32 *)ADDR_03001EBC + (slot * 8);
        slotData = slotBase + 0x11C;
        object = FIELD_AT_OFFSET(slotBase, void **, 0x11C);
        if (object != NULL) {
            if (objectX == -1) {
                objectX = (FIELD_AT_OFFSET(slotData, u8 *, 6) << 0x14) + 0x80000;
            }
            if (objectY == -1) {
                objectY = (FIELD_AT_OFFSET(slotData, u8 *, 7) << 0x14) + 0x80000;
            }
            FIELD_AT_OFFSET(object, s32 *, 8) = objectX;
            FIELD_AT_OFFSET(object, s32 *, 0x10) = objectY;
            angle = Sys_Place(0, objectX, objectY);
            FIELD_AT_OFFSET(object, s32 *, 0x14) = angle;
            FIELD_AT_OFFSET(object, s32 *, 0xC) = angle;
        }
    }
}

/* battle/effects/fx_emit_random_particle.c */
/* battle/effects/particles/emit_random.c */
#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

s32 ArcTan2(s32, s32);
void RotateVectorByMagnitude(s32, u16, void *);

struct GlobalData {
    u8 filler[0x1F4];
    u32 value_1F4;
};

extern struct GlobalData gCell;
extern char Value_fff80000;
extern char Value_001ffffe;

void BattleFx_EmitRandomParticle(void)
{
    register s32 rel_x;
    register s32 rel_y;
    register s32 x_offset;
    register s32 y_offset;
    register s32 x_delta;
    register s32 y_delta;
    register s32 object_x;
    register s32 object_y;
    register s32 negative_center;
    register u32 maximum;
    register s32 center;
    register s32 index;
    register u8 *object;
    register u8 *entry;

    object = ObjectTable_Get(gCell.value_1F4);
    entry = *(u8 **)ADDR_03001EBC + 0x11C;
    index = 0;
    if (entry[4] != 0) {
        object_x = FIELD(object, s32, 8);
        object_y = FIELD(object, s32, 0x10);
        negative_center = (s32)&Value_fff80000;
        maximum = (u32)&Value_001ffffe;
        center = 128;
        center <<= 12;
loop:
        x_offset = entry[6] << 20;
        x_delta = object_x - x_offset;
        rel_x = x_delta + negative_center;
        y_offset = entry[7] << 20;
        y_delta = object_y - y_offset;
        rel_y = y_delta + negative_center;
        if ((u32)(x_delta + 0x7FFFF) <= maximum &&
            (u32)(y_delta + 0x7FFFF) <= maximum) {
            FIELD(object, s32, 8) = x_offset + center;
            FIELD(object, s32, 0x10) = y_offset + center;
            RotateVectorByMagnitude(0x140000,
                          (u16)ArcTan2(rel_y, rel_x),
                          object + 8);
            FIELD(object, s32, 0x38) = 0x80000000;
            FIELD(object, s32, 0x3C) = 0x80000000;
            FIELD(object, s32, 0x40) = 0x80000000;
            return;
        }
        index++;
        entry += 8;
        if (index <= 9 && entry[4] != 0)
            goto loop;
    }
}

/* battle/effects/random_particle/emit.c */
struct ParticlePosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ParticleEmitter {
    u8 padding[8];
    struct ParticlePosition position;
    u8 padding2[20];
    s32 travel_offset;
    u8 padding3[41];
    u8 active;
};

struct ParticleChild {
    u8 padding[9];
    u8 flags;
};

struct ParticleEffectObject {
    u8 padding[80];
    struct ParticleChild *child;
};

#define OBJECT_0808EEE4_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char ParticlePosition_size[
    sizeof(struct ParticlePosition) == 0x0c ? 1 : -1
];
typedef char ParticleEmitter_travel_offset_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEmitter, travel_offset) == 0x28 ? 1 : -1
];
typedef char ParticleEmitter_active_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEmitter, active) == 0x55 ? 1 : -1
];
typedef char ParticleEffectObject_child_offset[
    OBJECT_0808EEE4_OFFSET(struct ParticleEffectObject, child) == 0x50 ? 1 : -1
];

extern u32 Random16(void);
extern void RotateVectorByMagnitude(s32, s32, struct ParticlePosition *);
extern void Object_SetCallback(struct ParticleEffectObject *, void *);
extern void Object_SetMode(struct ParticleEffectObject *, s32);
extern const u8 gRom[];

void EmitRandomParticleEffect(struct ParticleEmitter *emitter)
{
    struct ParticlePosition position;
    struct ParticleEffectObject *object;
    u32 random_angle;

    if (emitter->travel_offset >= -255 && emitter->travel_offset <= 255)
        emitter->active = 0;

    if ((100 * Random16() >> 16) > 9)
        return;

    position.x = emitter->position.x;
    position.y = emitter->position.y;
    position.z = emitter->position.z;
    random_angle = Random16();
    RotateVectorByMagnitude(random_angle << 4, Random16(), &position);
    object = (struct ParticleEffectObject *)Object_Spawn(
        0x11D, position.x, position.y, position.z);
    if (object != 0) {
        s32 mask;
        u8 flags;

        Object_SetCallback(object, (void *)gRom);
        Object_SetMode(object, 0);
        mask = 13;
        flags = object->child->flags;
        mask = -mask;
        mask &= flags;
        mask |= 4;
        object->child->flags = mask;
    }
}

/* battle/effects/random_particle/start_emitter.c */
struct EfxSrc {
    u8 pad0[6];
    u16 ang;
    s32 x;
    s32 y;
    s32 z;
};

struct EfxPos {
    s32 x;
    s32 y;
    s32 z;
};

struct EfxVisual {
    u8 pad0[5];
    u8 flags1;
    u8 pad1[3];
    u8 flags2;
    u8 pad2[18];
    u8 slot;
    u8 pad3[9];
    u8 unk1;
    u8 unk2;
};

struct TileBits {
    u16 tile : 10;
};

struct EfxObj {
    s32 data;
    u8 pad0[0x24];
    u32 travel;
    u8 pad1[0x1c];
    u32 dy;
    u8 pad2[4];
    struct EfxVisual *vis;
    u8 pad3[24];
    void (*proc)(void);
};

extern u8 *EfxWork;
extern const u8 EmitterData[];
extern const u8 ParticleData[];

void RotateVectorByMagnitude(s32 mag, s32 ang, struct EfxPos *pos);
struct EfxObj *Object_CreateFar(s32 kind, s32 x, s32 y, s32 z);
void ObjectDispatch_InitializeFar(struct EfxObj *obj, s32 data);
void Object_Destroy(struct EfxObj *obj);
void *Runtime_AllocateHeapBlock(s32 kind, s32 size);
void WaitFrames(s32 frames);
void ItemIcon_LoadTilesFar(s32 item);
s32 VramBlock_LoadCached(u32 slot, u32 size, const void *src);
void Runtime_ReleaseHeapBlock(s32 kind);
void EmitRandomParticleEffect(void);
void BattleFx_SpawnRandomParticleAtPosition(const void *src);

#define BATTLE_ACTIVE_OFS 0xcb8

struct EfxObj *BattleFx_StartRandomParticleEmitter(s32 obj_id, s32 item)
{
    struct EfxObj *obj;
    /* GCC shape: work holds the state base, then the pool count. */
    s32 work = (s32)EfxWork;
    u32 off = (obj_id * 4) + 0x14;
    /* GCC shape: src_z holds the source pointer, then the spawn Z. */
    s32 src_z = (s32)*(struct EfxSrc **)(work + off);
    struct EfxPos pos;
    s32 x;

    if (src_z == 0)
        return 0;

    pos.x = ((struct EfxSrc *)src_z)->x;
    pos.y = ((struct EfxSrc *)src_z)->y;
    pos.z = ((struct EfxSrc *)src_z)->z;
    RotateVectorByMagnitude(
        0x100000,
        ((struct EfxSrc *)src_z)->ang,
        &pos);
    x = (pos.x & 0xfff00000) + 0x80000;
    src_z = (pos.z & 0xfff00000) + 0x80000;

    if (*(s16 *)(work + BATTLE_ACTIVE_OFS) != 0) {
        struct EfxObj *ent = EfxPool;

        work = 63;

        do {
            if (ent->data != 0) {
                if (ent->proc == (void (*)(void))BattleFx_SpawnRandomParticleAtPosition) {
                    Object_Destroy(ent);
                }
                if (ent->data == (s32)ParticleData) {
                    Object_Destroy(ent);
                }
            }
            work--;
            ent++;
        } while (work >= 0);
    }

    WaitFrames(3);
    obj = Object_CreateFar(22, x, 0x100000, src_z);
    if (obj == 0)
        return 0;

    ObjectDispatch_InitializeFar(obj, (s32)EmitterData);
    {
        struct EfxVisual *vis = obj->vis;
        void *buf;
        s32 mask;
        s32 zero = 0;

        vis->unk1 = zero;
        vis->unk2 = zero;

        vis->flags1 &= zero - 33;

        mask = vis->flags2 & 0x0f;
        {
            s32 clr = 13;
            clr = -clr;
            mask &= clr;
        }
        mask |= 4;
        vis->flags2 = mask;

        obj->travel = 0x20000;
        obj->dy = 0x4000;

        buf = Runtime_AllocateHeapBlock(17, 0x608);
        ItemIcon_LoadTilesFar(item);

        ((struct TileBits *)((u8 *)vis + 8))->tile =
            VramBlock_LoadCached(vis->slot, 128, (u8 *)buf + 0x400);

        Runtime_ReleaseHeapBlock(17);
        obj->proc = (void (*)(void))EmitRandomParticleEffect;
    }

    return obj;
}

/* object/destroy_if_present.c */
s32 Object_Destroy();

void Object_DestroyIfPresent(s32 arg0)
{
    if (arg0 != 0) {
        Object_Destroy();
    }
}
