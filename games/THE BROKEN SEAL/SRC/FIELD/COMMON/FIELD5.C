#include "BATTLE_EFFECT_COUNTERS.H"
#include "TYPES.H"
#include "MAP.H"
#include "EVENT_RUNTIME.H"
#include "IWRAM_CALL.H"
#include "RAM_BUFFER.H"

struct MapFocusObject {
    u8 padding00[6];
    u16 kind;
    s32 x;
    s32 unknown_0c;
    s32 y;
};

struct MapFocusPosition {
    s32 x;
    s32 unknown_04;
    s32 y;
};

void Vector_AddPolarOffset(s32, u32, struct MapFocusPosition *);
extern struct EventPairWork1d6 gGameState;
extern struct EventRuntime *gEventWork;

struct MapRegion {
    s16 min_x;
    s16 min_y;
    s16 min_z;
    s16 max_x;
    s16 max_y;
    s16 max_z;
    s16 condition;
    s16 value;
};

typedef struct MapRegion *(*MapRegionProvider)(void);

struct MapRegionServices {
    u8 padding000[0x2c];
    MapRegionProvider region_provider;
};

extern struct MapRegionServices gOverlayArea;
s32 GameFlag_IsConditionActive(s32 condition);
void Audio_PlayCue(s32 sound_id);
void Battle_InitializeRenderObject(void);
#define REGION_FIXED(value) ((s32)((u32)(s32)(value) << 16))

extern u8 gMapCellBuffer[];

struct StepTile {
    u8 unknown_0[2];
    u8 event;                       /* 0x02 */
    u8 unknown_3;
};

struct StepLayer {
    struct StepTile *tiles;         /* 0x00 */
    u8 unknown_04[0x2c];
};

struct StepMapWork {
    u8 unknown_000[0x130];
    struct StepLayer layers[3];     /* 0x130 */
};

struct StepActor {
    u8 unknown_00[8];
    u8 motion[0x20];                /* 0x08 */
    s32 unknown_28;                 /* 0x28 */
    u8 unknown_2c[4];
    s32 speed;                      /* 0x30 */
    u8 unknown_34[4];
    s32 unknown_38;                 /* 0x38 */
};

struct StepWork {
    u8 unknown_000[0x14];
    struct StepActor *actors[1];    /* 0x014 */
    u8 unknown_018[0x16c - 0x18];
    s16 step_event;                 /* 0x16c */
    s16 warp_event;                 /* 0x16e */
    u8 unknown_170[0xc];
    u16 unknown_17c;                /* 0x17c */
    s16 unknown_17e;                /* 0x17e */
    u8 unknown_180[2];
    u16 unknown_182;                /* 0x182 */
    s16 fallen_count;               /* 0x184 */
    s16 standing_count;             /* 0x186 */
    u16 fallen[11];                 /* 0x188 */
    s16 mode;                       /* 0x19e */
    u8 encounter_full[4];           /* 0x1a0 */
    u8 unknown_1a4[0xc];
    s32 encounter_rate;             /* 0x1b0 */
    s32 encounter_steps;            /* 0x1b4 */
    struct StepTile *tile;          /* 0x1b8 */
    struct StepTile *previous_tile; /* 0x1bc */
};

struct StepState {
    u8 unknown_000[0x1f2];
    u8 unknown_1f2;                 /* 0x1f2 */
    u8 unknown_1f3;
    s32 selected_actor;             /* 0x1f4 */
    u8 party[0x34];                 /* 0x1f8 */
    s16 unknown_22c;                /* 0x22c */
    s16 unknown_22e;                /* 0x22e */
    s16 unknown_230;                /* 0x230 */
    s16 unknown_232;                /* 0x232 */
    u8 unknown_234[0xa];
    s16 unknown_23e;                /* 0x23e */
    u8 unknown_240[4];
    s32 unknown_244;                /* 0x244 */
};

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
extern struct StepState Data_02000240;
s32 Party_CountActiveOwnersFar(void);
struct StepOwner *Owner_GetStateFar(s32 owner);
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
    struct MapFocusObject *object;
    struct MapState *map;
    struct MapFocusPosition position;
    u8 *tile;
    u32 offset;
    s32 x;
    s32 y;

    runtime_slot_address = (u32)&gEventWork;
    runtime = gEventWork;
    offset = (gGameState.object_id * 4) + 0x14;
    object = *(struct MapFocusObject **)((u8 *)runtime + offset);

    map = *(struct MapState **)(runtime_slot_address - 76);

    if (object == 0)
        return 0;

    position.x = object->x;
    position.unknown_04 = object->unknown_0c;
    position.y = object->y;
    Vector_AddPolarOffset(0x100000, object->kind, &position);

    if (runtime->mode_19e == 3) {
        u32 tile_x;
        u32 tile_y;

        x = position.x;
        if (x < 0)
            x += 0x1fffff;
        tile_x = (x >> 21) & 31;

        y = position.y;
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

            y = position.y;
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
    struct MapRegion *region;

    x = position_x;
    y = position_y;
    z = position_z;
    region = gOverlayArea.region_provider();
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
    struct StepWork *work = Data_03001ebc;
    /* FAKEMATCH: the map work cell is reached as gEventWork[-19]; the ROM loads
       0x03001ebc once and subtracts 76 to reach 0x03001e70. */
    struct StepMapWork *map = ((struct StepMapWork **)&Data_03001ebc)[-19];
    s32 selected = Data_02000240.selected_actor;
    struct StepActor *actor = work->actors[selected];
    s16 hp[8];
    struct StepTile *tile;
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
        hp[i] = Owner_GetStateFar(Data_02000240.party[i])->hp;

    if (work->mode == STEP_MODE_WORLD_MAP) {
        tile = &((struct StepTile *)Ram_MapBlocks)[((x / 0x200000) & 31) + (((z / 0x200000) & 31) << 5)];
    } else {
        if ((u32)layer <= 2)
            tile = map->layers[layer].tiles;
        else
            tile = (struct StepTile *)gMapCellBuffer;
        tile = &tile[(x / 0x100000) + ((z / 0x100000) << 7)];
    }
    event = tile->event;
    work->previous_tile = work->tile;
    work->tile = tile;
    if (event != 0)
        UpdateMapRegionAtPosition(x, y, z);
    if (event >= STEP_EVENT_FIRST && event <= STEP_EVENT_LAST)
        work->step_event = event;
    if (event >= STEP_WARP_FIRST && event <= STEP_WARP_LAST)
        work->warp_event = event;

    if (Data_02000240.unknown_1f2 == 0 && actor != NULL && actor->unknown_38 != STEP_NO_TARGET) {
        speed = actor->speed;
        if (GameFlag_TestFar(STEP_FLAG_DOUBLE_SPEED))
            speed <<= 1;
        if (work->mode == STEP_MODE_WORLD_MAP) {
            full = 1;
            work->unknown_17c = BattleFx_ApplyLookupResult(actor->motion, speed);
        } else if (event == STEP_TERRAIN_FIRST || event == STEP_TERRAIN_LAST) {
            full = work->encounter_full[event - (STEP_TERRAIN_FIRST - 1)];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(event - (STEP_TERRAIN_FIRST - 1), speed);
        } else {
            full = work->encounter_full[0];
            work->unknown_17c = EffectRuntime_LookupByTableEntry(0, speed);
        }
        steps = Iwram_MulQ16(work->encounter_rate, actor->speed);
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
        if (Data_02000240.unknown_22e == 0 && event == STEP_EVENT_HAZARD) {
            if (work->previous_tile->event == STEP_EVENT_HAZARD)
                Data_02000240.unknown_232 += actor->speed / 0x10000;
            else
                Data_02000240.unknown_232 = Data_02000240.unknown_22c / 2;
        }
        if (Data_02000240.unknown_244 != 0 && Data_02000240.unknown_23e != 2) {
            Data_02000240.unknown_244 -= actor->speed;
            if (Data_02000240.unknown_244 <= 0) {
                Data_02000240.unknown_244 = 1;
                if (work->unknown_17e == 0)
                    work->unknown_17e = 0x2096;
            }
        }
    }

    if (Data_02000240.unknown_22e == 1) {
        Data_02000240.unknown_232++;
        if (Data_02000240.unknown_232 == Data_02000240.unknown_22c / 2)
            BattleFx_ConfigureLinkedObject(selected, 0x101);
        if (Data_02000240.unknown_232 == Data_02000240.unknown_22c)
            BattleFx_ConfigureLinkedObject(selected, 0x100);
    }
    if (Data_02000240.unknown_232 >= Data_02000240.unknown_22c) {
        n = Data_02000240.unknown_230;
        Data_02000240.unknown_232 = 0;
        BattleParty_ApplyHealthDelta(-(n & 255), n & 0x100);
        fell++;
    }

    if (fell) {
        work->fallen_count = 0;
        work->standing_count = 0;
        actor->unknown_28 = 0x40000;
        BattleFx_ConfigureLinkedObject(selected, 0x102);
        for (i = 0; i < count; i++) {
            owner = Owner_GetStateFar(Data_02000240.party[i]);
            if (owner->hp > 0) {
                work->standing_count++;
            } else if (hp[i] != 0) {
                work->fallen[work->fallen_count++] = Data_02000240.party[i];
                work->unknown_182 = 0xffff;
                owner->unknown_131 = 0;
            }
        }
    }
}
