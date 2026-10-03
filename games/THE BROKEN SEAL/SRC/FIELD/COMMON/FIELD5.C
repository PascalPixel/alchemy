#include "BATTLE_EFFECT_COUNTERS.H"
#include "TYPES.H"
#include "MAP.H"
#include "EVENT_RUNTIME.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"
#include "GAME_STATE.H"
#include "FIELDRUN.H"

void Vector_AddPolarOffset(s32, u32, struct FieldPosition *);
extern struct EventRuntime *gEventWork;

s32 GameFlag_IsConditionActive(s32 condition);
void Audio_PlayCue(s32 sound_id);
void Battle_InitializeRenderObject(void);
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

extern u8 gMapCellBuffer[];

struct StepOwner {
    u8 unknown_000[0x38];
    s16 hp;                         /* 0x038 */
    u8 unknown_03a[0x131 - 0x3a];
    u8 unknown_131;                 /* 0x131 */
};

enum StepTileEvent {
    STEP_EVENT_FIRST = 1,
    STEP_EVENT_LAST = 239,
    STEP_TERRAIN_FIRST = 240,
    STEP_TERRAIN_LAST = 241,
    STEP_EVENT_HAZARD = 250,
    STEP_WARP_FIRST = 252,
    STEP_WARP_LAST = 254
};

#define STEP_MODE_WORLD_MAP 3
#define STEP_NO_TARGET ((s32)0x80000000)
#define STEP_FLAG_DOUBLE_SPEED 0x167
#define STEP_CUE_EFFECT 139
s32 Party_CountActiveOwnersFar(void);
struct BattleUnit *Owner_GetStateFar(s32 owner);
s32 GameFlag_TestFar(s32 flag);
void UpdateMapRegionAtPosition(s32 x, s32 y, s32 z);
s32 BattleFx_ApplyLookupResult(void *motion, s32 speed);
s32 EffectRuntime_LookupByTableEntry(s32 kind, s32 speed);
void BattleParty_ApplyDrain(s32 steps, s32 unused);
s32 Func_080b50f8(void);
void Audio_PlayCue(s32 cue);
void Event_ClearInvalidPackedValues(void);
s32 BattleParty_ApplyStatusDamage(void);
void BattleFx_ConfigureLinkedObject(s32 actor, s32 event);
void BattleParty_ApplyHealthDelta(s32 amount, s32 flags);

void BattleFx_ResetCounters(void)
{
  short cleared_value;
  struct BattleEffectCounterState *state;
  cleared_value = 0;
  state = Data_03001ebc;
  state->counters[0] = cleared_value;
  state->counters[1] = 0;
  state->counters[2] = 0;
  state->counters[3] = 0;
  state->counters[4] = 0;
  state->counters[5] = 0;
  state->counters[6] = 0;
  state->counters[7] = 0;
  state->counters[8] = 0;
  state->counters[9] = 0;
  state->counters[10] = 0;
  state->counters[11] = 0;
}

s32 BattleFx_SumCounters(void)
{
    struct BattleEffectCounterState *state = Data_03001ebc;

    return state->counters[0] + state->counters[1] + state->counters[2]
         + state->counters[3] + state->counters[4] + state->counters[5]
         + state->counters[6] + state->counters[7] + state->counters[6]
         + state->counters[8] + state->counters[9] + state->counters[10]
         + state->counters[11];
}

u8 GetFocusedObjectCollision(void)
{
    u32 runtime_slot_address;
    struct EventRuntime *runtime;
    struct ObjectRuntime *object;
    struct MapState *map;
    struct FieldPosition position;
    u8 *tile;
    u32 offset;
    s32 x;
    s32 y;

    runtime_slot_address = (u32)&gEventWork;
    runtime = gEventWork;
    offset = ((u32)gGameState.selected_actor * 4) + 0x14;
    object = *(struct ObjectRuntime **)((u8 *)runtime + offset);

    map = *(struct MapState **)(runtime_slot_address - 76);

    if (object == 0)
        return 0;

    position.x = object->x;
    position.y = object->y;
    position.z = object->z;
    Vector_AddPolarOffset(0x100000, object->angle, &position);

    if (runtime->mode_19e == 3) {
        u32 tile_x;
        u32 tile_y;

        x = position.x;
        if (x < 0)
            x += 0x1fffff;
        tile_x = (x >> 21) & 31;

        y = position.z;
        if (y < 0)
            y += 0x1fffff;
        tile_y = (y >> 21) & 31;

        tile = (u8 *)gMapBlocks + ((tile_x + (tile_y << 5)) << 2);
    } else {
        tile = (u8 *)map->layers[0].cells;

        x = position.x;
        if (x < 0)
            x += 0xfffff;
        {
            u32 tile_x = x >> 20;

            y = position.z;
            if (y < 0)
                y += 0xfffff;

            tile = (u8 *)((u32)tile +
                ((tile_x + ((u32)(y >> 20) << 7)) << 2));
        }
    }

    return tile[2];
}

void UpdateMapRegionAtPosition(s32 position_x, s32 position_y, s32 position_z)
{
    s32 selected_value;
    struct EventRuntime *runtime;
    s32 z;
    s32 y;
    s32 x;
    s16 condition;
    s16 min_z;
    s16 max_y;
    s16 min_y;
    s32 max_z;
    s16 max_x;
    s16 min_x;
    const struct SceneRegion *region;

    x = position_x;
    y = position_y;
    z = position_z;
    region = gOverlayArea.regions();
    if (region != 0 &&
        (runtime = gEventWork, min_x = region->min_x, min_x != -1)) {
loop:
        min_y = region->min_y;
        min_z = region->min_z;
        max_x = region->max_x;
        max_y = region->max_y;
        max_z = region->max_z;
        condition = region->condition;
        selected_value = region->value;
        if (GameFlag_IsConditionActive(condition)!= 0 &&
            y >= REGION_FIXED(min_y)&&
            y < REGION_FIXED(max_y)&&
            x >= REGION_FIXED(min_x)&&
            x < REGION_FIXED(max_x)&&
            z >= REGION_FIXED(min_z)&&
            z < REGION_FIXED(max_z)) {
            runtime->value_170 = (u16)selected_value;
            Audio_PlayCue(123);
            Battle_InitializeRenderObject();
            return;
        }
        runtime = gEventWork;
        region++;
        min_x = region->min_x;
        if (min_x != -1)
            goto loop;
    }
}

/*
 * Runs what one step of the party leader costs and triggers on the field:
 * reads the tile under the new position (its event byte opens map regions
 * and records the step and warp events), advances the encounter step
 * counter by the leader's speed, runs the step effects every whole count,
 * steps the poison and Avoid timers, and when the step effects knocked a
 * party member out, records who fell.
 */
void Field_ProcessStep(s32 layer, s32 x, s32 y, s32 z)
{
    /* FAKEMATCH: retain the existing StepOwner read/byte-store prefix. The
       full BattleUnit view moved the party-byte load before its index shift.
       Signed lvalue/array views reduced frame 44 to 40; a second used base
       grew it to 48. Scalar s16 interpretations keep the actual frame. */
    struct FieldStepWork *work = (struct FieldStepWork *)Data_03001ebc;
    /* FAKEMATCH: the map work cell is reached as gEventWork[-19]; the ROM loads
       0x03001ebc once and subtracts 76 to reach 0x03001e70. */
    struct MapState *map = ((struct MapState **)&Data_03001ebc)[-19];
    s32 selected = gGameState.selected_actor;
    struct ObjectRuntime *actor = work->actors[selected];
    s16 hp[8];
    struct MapCell *tile;
    struct StepOwner *owner;
    u32 count;
    s32 fell;
    u32 i;
    u32 event;
    s32 speed;
    s32 steps;
    s32 n;
    s32 sum;
    s32 full;

    fell = 0;
    if (work == NULL)
        return;

    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++)
        hp[i] = Owner_GetStateFar(gGameState.active_owners[i])->hp;

    if (work->mode == STEP_MODE_WORLD_MAP) {
        tile = &((struct MapCell *)Ram_MapBlocks)[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
    } else {
        if ((u32)layer <= 2)
            tile = map->layers[layer].cells;
        else
            tile = (struct MapCell *)gMapCellBuffer;
        tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
    }
    event = tile->collision_code;
    work->previous_tile = work->tile;
    work->tile = tile;
    if (event != 0)
        UpdateMapRegionAtPosition(x, y, z);
    if (event >= STEP_EVENT_FIRST && event <= STEP_EVENT_LAST)
        work->step_event = event;
    if (event >= STEP_WARP_FIRST && event <= STEP_WARP_LAST)
        work->warp_event = event;

    if (gGameState.movement_mode == 0 && actor != NULL && actor->target_x != STEP_NO_TARGET) {
        speed = actor->speed_limit;
        if (GameFlag_TestFar(STEP_FLAG_DOUBLE_SPEED))
            speed <<= 1;
        if (work->mode == STEP_MODE_WORLD_MAP) {
            full = 1;
            work->unknown_17c = BattleFx_ApplyLookupResult(&actor->x, speed);
        } else if (event == STEP_TERRAIN_FIRST || event == STEP_TERRAIN_LAST) {
            full = work->encounter_full[event - (STEP_TERRAIN_FIRST - 1)];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(event - (STEP_TERRAIN_FIRST - 1), speed);
        } else {
            full = work->encounter_full[0];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(0, speed);
        }
        steps = Iwram_MulQ16(work->encounter_rate, actor->speed_limit);
        if (!full)
            steps /= 2;
        sum = work->encounter_steps + steps;
        work->encounter_steps = sum;
        if (sum > 0xffff) {
            n = sum / 0x10000;
            work->encounter_steps = sum & 0xffff;
            BattleParty_ApplyDrain(n, 0);
            if (Func_080b50f8())
                Audio_PlayCue(STEP_CUE_EFFECT);
            Event_ClearInvalidPackedValues();
            fell = BattleParty_ApplyStatusDamage();
        }
        if (((s16)gGameState.unknown_22e) == 0 && event == STEP_EVENT_HAZARD) {
            if (work->previous_tile->collision_code == STEP_EVENT_HAZARD)
                gGameState.unknown_232 += actor->speed_limit / 0x10000;
            else
                gGameState.unknown_232 = ((s16)gGameState.unknown_22c) / 2;
        }
        if (gGameState.unknown_244 != 0 && gGameState.unknown_23e != 2) {
            gGameState.unknown_244 -= actor->speed_limit;
            if (gGameState.unknown_244 <= 0) {
                gGameState.unknown_244 = 1;
                if (work->unknown_17e == 0)
                    work->unknown_17e = 0x2096;
            }
        }
    }

    if (((s16)gGameState.unknown_22e) == 1) {
        gGameState.unknown_232++;
        if (gGameState.unknown_232 == ((s16)gGameState.unknown_22c) / 2)
            BattleFx_ConfigureLinkedObject(selected, 0x101);
        if (gGameState.unknown_232 == ((s16)gGameState.unknown_22c))
            BattleFx_ConfigureLinkedObject(selected, 0x100);
    }
    if (gGameState.unknown_232 >= ((s16)gGameState.unknown_22c)) {
        n = ((s16)gGameState.unknown_230);
        gGameState.unknown_232 = 0;
        BattleParty_ApplyHealthDelta(-(n & 255), n & 0x100);
        fell++;
    }

    if (fell) {
        work->fallen_count = 0;
        work->standing_count = 0;
        actor->velocity_y = 0x40000;
        BattleFx_ConfigureLinkedObject(selected, 0x102);
        for (i = 0; i < count; i++) {
            owner = (struct StepOwner *)Owner_GetStateFar(gGameState.active_owners[i]);
            if (owner->hp > 0) {
                work->standing_count++;
            } else if (hp[i] != 0) {
                work->fallen[work->fallen_count++] = gGameState.active_owners[i];
                work->unknown_182 = 0xffff;
                owner->unknown_131 = 0;
            }
        }
    }
}
