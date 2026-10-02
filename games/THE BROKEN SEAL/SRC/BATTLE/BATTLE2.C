#include "TYPES.H"
#include "RUNTIME_MEM.H"
#include "SCENE.H"
#include "IWRAM_CALL.H"
#include "FIXED_MATH.H"
#include "GLOBAL_CELLS.H"
#include "BATTLE_PARTY.H"
#include "BATTLE_ESCAPE.H"
#include "BATTLE_RUNTIME.H"

s32 Trig_Cos(s32);
extern s32 gFrameCount;

void Render_ResetTransformState(void);
void SceneTransform_ApplyPosition(void *);
void SceneTransform_ApplyYaw(s32);
void SceneTransform_ApplyPitch(s32);
void Camera_StoreSceneParameters(u32, u32, u32);
void BattleCamera_SetRange(s32, s32, s32, s32, s32);

/* Alternate scene-camera setup used by the later field presentation. */
struct SceneCameraState {
    u8 filler0[12];
    s32 field0c;
    s32 field10;
    s32 field14;
    s32 field18;
    s32 field1c;
    s32 field20;
    u8 filler24[16];
    s16 field34;
    s16 field36;
};

struct SceneCameraAuxiliary {
    s32 field00;
    u8 filler04[12];
    s32 field10;
    s32 field14;
};

struct SceneCameraRuntime {
    struct SceneCameraState *state;
    u8 filler04[124];
    struct SceneCameraAuxiliary *secondary;
};

struct SceneCameraTransfer {
    s32 first;
    s32 second;
    s32 third;
};

struct SceneCameraObject {
    u32 field00;
    u32 field04;
    u32 field08;
    u32 field0c;
    s32 anchor;
};

extern struct SceneCameraRuntime gCameraWork;
extern struct SceneCameraObject gProjection;

s32 Battle_CollectPartyCommandsFar(void *entries, u16 *excluded_units, s32 excluded_count);
void Runtime_BumpFree(void *ptr);
extern u8 Data_03001e74[];
s32 BattleParty_ListActorIds(s32 groups, u16 *ids);

void Owner_RecalculateStatsFar(u16 id);

/* battle/escape/check_success.c */
struct BattleEscapeState {
    u8 reserved_00[0x45];
    u8 guaranteed;
    u8 failed_attempts;
};

u32 Random16(void);

/* LCG: seed = seed * 0x41c64e6d + 0x3039, returns bits 8-23. */
extern u8 gGameState[];

/* battle/presentation/list/units.c */

struct BattlePresentationUnitEntry {
    u16 unit_id;
    u16 unknown_02;
    u16 value;
    s16 width;
    s16 mode;
    s16 height;
    u8 unknown_0c[4];
};

void Palette_UpdatePulseBrightness(void)
{
    u16 *palette;
    s32 i;
    s32 mask;

    palette = (u16 *)0x050001C0;
    mask = 31;
    i = 15;
    do {
        u32 color;
        s32 amount;
        s32 tick;
        s32 angle;
        s32 red;
        s32 green;
        s32 blue;

        tick = gFrameCount;
        angle = (tick * 3) << 10;
        color = palette[16];
        amount = (0x10000 - Trig_Cos(angle)) / 0x2AAA;

        red = (color >> 10) & mask;
        green = (color >> 5) & mask;
        blue = color & mask;
        red += amount;
        green += amount;
        blue += amount;

        if ((u32)red > 31)
            red = 31;
        if ((u32)green > 31)
            green = 31;
        if ((u32)blue > 31)
            blue = 31;

        *palette++ = (red << 10) | (green << 5) | blue;
        i--;
    } while (i >= 0);
}

/* Keep the address symbol for the build map while exposing its role to C. */
void Camera_ConfigureScene(s32 pos)
{
    struct SceneCameraState *state = gCameraWork.state;
    struct SceneCameraAuxiliary *secondary = gCameraWork.secondary;
    struct SceneCameraTransfer local;
    u32 result;

    state->field10 = 160 << 11;
    state->field0c = 0;
    state->field14 = 0;
    secondary->field00 = 128 << 7;
    state->field36 = 128 << 7;
    state->field34 = 244 << 8;
    state->field1c = 0;
    state->field20 = 0x02ee0000;
    state->field18 = 0;

    Render_ResetTransformState();
    SceneTransform_ApplyPosition(&state->field0c);
    SceneTransform_ApplyYaw(state->field36);
    SceneTransform_ApplyPitch(state->field34);

    local.first = 0;
    local.second = 0;
    local.third = state->field20;
    Iwram_TransformVector((s32 *)&local, (s32 *)state);

    result = Iwram_RatioMulQ14(0x03c90000, 192 << 8);
    Camera_StoreSceneParameters(0, result, 0x07920000);

    gProjection.anchor = pos + 120;
    secondary->field10 = 1;
    BattleCamera_SetRange(240 << 15, (0x76 - pos) << 16, 0, 128 << 4, 128 << 10);
    secondary->field14 = 1;
    secondary->field10 = 0;
}

void BattleUnit_ClearField12bForGroup(void)
{
    u16 ids[14];
    s32 count;
    s32 index;

    count = BattleParty_ListActorIds(3, ids);
    for (index = 0; index < count; index++) {
        struct BattleUnit *actor;

        actor = Owner_GetStateFar(ids[index]);
        actor->guard_level = 0;
        Owner_RecalculateStatsFar(ids[index]);
    }
}

s32 BattleEscape_CheckSuccess(void)
{
    s32 escaped;
    u8 *failed_attempts;
    s16 living_units[14];
    s32 living_count;
    s32 level_total;
    s32 unit_index;
    s32 chance;
    struct BattleEscapeState *escape_state;

    escaped = 0;
    escape_state = *(struct BattleEscapeState **)((u32)&Data_03001e74);
    if (escape_state->guaranteed == 1) {
        escaped = 1;
    } else {
        failed_attempts = &escape_state->failed_attempts;
        chance = 0x1388 + (escape_state->failed_attempts * 0x7D0);
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_PARTY,
            living_units);
        level_total = 0;
        for (unit_index = escaped; unit_index < living_count; unit_index++) {
            level_total += Owner_GetStateFar(
                (s32)living_units[unit_index])->level;
        }
        chance += level_total * 0x1F4 / living_count;
        living_count = BattleParty_ListLivingUnits(
            BATTLE_SIDE_ENEMIES,
            living_units);
        level_total = 0;
        for (unit_index = 0; unit_index < living_count; unit_index++) {
            level_total += Owner_GetStateFar(
                (s32)living_units[unit_index])->level;
        }
        chance -= level_total * 0x1F4 / living_count;
        if ((chance > 0) &&
            ((u32)((u32)(0x2710 * Random16()) >> 0x10) < (u32)chance)) {
            escaped = 1;
        }
        *failed_attempts += 1;
    }
    if (gGameState[0x22B] == 2) {
        escaped = 0;
    }
    return escaped;
}

s32 BattlePres_BuildUnitEntries(
    struct BattlePresentationUnitEntry *entries)
{
    u16 *excluded_units = (u16 *)Runtime_BumpAllocateAlternatePool(17);
    u16 *unit_ids = (u16 *)Runtime_BumpAllocateAlternatePool(9);
    s32 unit_count = BattleParty_ListLivingUnits(1, unit_ids);
    s32 excluded_count = 0;
    s32 entry_count = 0;
    s32 unit_index;

    for (unit_index = 0; unit_index < unit_count; unit_index++) {
        struct BattleUnit *unit = Owner_GetStateFar(unit_ids[unit_index]);
        s32 copy_index;

        for (copy_index = 0; copy_index < unit->action_entry_count; copy_index++) {
            /* The word read tests confusion, charm and stun together. */
            if (unit->sleep != 0 || (*(u32 *)&unit->delusion & 0xffffff00)) {
                struct BattlePresentationUnitEntry *entry =
                    &entries[entry_count];
                entry->unit_id = unit_ids[unit_index];
                entry->value = unit->agility;
                entry->width = 8;
                entry->mode = 0;
                entry->height = 0x180;
                entry_count++;
            } else {
                excluded_units[excluded_count++] = unit_ids[unit_index];
            }
        }
    }

    {
        s32 appended;

        entries += entry_count;
        appended = Battle_CollectPartyCommandsFar(entries, excluded_units, excluded_count);
        if (appended < 0) {
            unit_count = -1;
        } else {
            unit_count = entry_count + appended;
        }
        Runtime_BumpFree(unit_ids);
        Runtime_BumpFree(excluded_units);
        return unit_count;
    }
}
