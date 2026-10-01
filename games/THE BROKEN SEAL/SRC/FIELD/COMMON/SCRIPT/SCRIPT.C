#include "EDITION.H"
#include "SCRIPT_OPERANDS.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "IWRAM_CALL.H"

struct WanderPosition {
    s32 x;
    s32 y;
    s32 z;
};

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, struct WanderPosition *position);
s32 ScriptObject_CheckOverlap(struct ScriptObjectRuntime *object, struct WanderPosition *position);
s32 Func_080120dc(struct ScriptObjectRuntime *object, struct WanderPosition *position);
void Object_SetMoveTarget(struct ScriptObjectRuntime *object, s32 x, s32 y, s32 z);
u16 ArcTan2(s32 y, s32 x);

/*
 * Script command: wander about the object's home cell (+0x64, +0x66). The
 * three arguments are the base distance, the random extra distance and the
 * leash radius in whole units. Inside the leash, up to seven headings near
 * the stored heading are tried; the point must be free of objects, and five
 * probes half a tile further on, straight ahead and turned an eighth and a
 * quarter either side, must be walkable, and the point must stay inside the
 * leash. Outside the leash, the object heads back towards home and clears
 * the flag it sets while roaming.
 */
s32 ScriptObject_WanderNearHome(struct ScriptObjectRuntime *object)
{
    struct WanderPosition pos;
    struct WanderPosition probe;
    s32 base;
    const s32 *args;
    s32 range;
    s32 limit;
    s32 radius;
    u16 heading;
    u16 angle;
    s32 tries;
    s32 dx;
    s32 dz;

    args = &object->script[(s16)object->script_cursor + 1];
    base = *args++;
    range = *args++;
    limit = *args / 0x10000;
    angle = (s16)object->script_value;
    limit = limit * limit;
    tries = 0;
    dx = object->x / 0x10000 - object->home_x;
    dz = object->z / 0x10000 - object->home_z;
    if (dx * dx + dz * dz > limit)
        goto home;
roam:
    tries++;
    if (tries > 7)
        goto home;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &pos);
    if (ScriptObject_CheckOverlap(object, &pos) != 0)
        goto roam;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Func_080120dc(object, &pos) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    radius += 0x80000;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x4000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x4000, &probe);
    if (Func_080120dc(object, &probe) != 0)
        goto roam;
    dx = pos.x / 0x10000 - object->home_x;
    dz = pos.z / 0x10000 - object->home_z;
    if (dx * dx + dz * dz > limit)
        goto roam;
    object->flags_59 |= 2;
    Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
    goto done;
home:
    tries = 0;
    angle = ArcTan2(dz, dx) + 0x8000;
back:
    tries++;
    if (tries > 7)
        goto done;
    radius = base + Iwram_MulQ16(Random16(), range);
    heading = angle + (Random16() >> 2) - (Random16() >> 2);
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(0x80000, heading, &pos);
    if (ScriptObject_CheckOverlap(object, &pos) != 0)
        goto back;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Func_080120dc(object, &pos) != 0)
        goto back;
    object->flags_59 &= ~2;
    Object_SetMoveTarget(object, pos.x, pos.y, pos.z);
done:
    object->script_cursor += 4;
    return 1;
}

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Script_OperandHandlerTable[];

/* field/store_assigned_key_value.c */
extern u8 *gWork;
s32 GameFlag_TestFar(s32);

static __inline__ void StoreHalfword(u8 *address, s32 value)
{
    *(s16 *)address = value;
}

extern u16 gGameState[];
extern volatile u32 gKeyState;

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->script_address = value;
        return;
    }
    if (operation == 1) {
        state->script_address += (u32)value * 4;
        return;
    }
    result = 0;
    if (state->script_address == (u32)value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->cursor = value;
    } else if (operation == 1) {
        state->cursor =
            (u16)((u32)(s32)(s16)state->cursor + (u32)value);
    } else {
        result = 0;
        if ((s16)state->cursor == (s16)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->unsigned_halfword = value;
    } else if (operation == 1) {
        state->unsigned_halfword =
            (u16)((u32)state->unsigned_halfword + (u32)value);
    } else {
        result = 0;
        if (state->unsigned_halfword == (u16)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareWord08(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_08 = value;
        return;
    }
    if (operation == 1) {
        state->word_08 += value;
        return;
    }
    result = 0;
    if (state->word_08 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord0c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_0c = value;
        return;
    }
    if (operation == 1) {
        state->word_0c += value;
        return;
    }
    result = 0;
    if (state->word_0c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord10(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_10 = value;
        return;
    }
    if (operation == 1) {
        state->word_10 += value;
        return;
    }
    result = 0;
    if (state->word_10 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareHalfword20(struct ScriptOperands *state, s32 operation, s32 value)
{
  s8 result;
  if (operation == 0)
  {
    state->halfword_20 = value;
    return;
  }
  if (operation == 1)
  {
    state->halfword_20 = (u16)((u32)state->halfword_20 + (u32)value);
    return;
  }
  result = 0;
  if (state->halfword_20 == (s16)value)
  {
    result = 1;
  }
  state->comparison_result = result;
}

void Script_SetOrCompareWord18(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_18 = value;
        return;
    }
    if (operation == 1) {
        state->word_18 += value;
        return;
    }
    result = 0;
    if (state->word_18 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord1c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_1c = value;
        return;
    }
    if (operation == 1) {
        state->word_1c += value;
        return;
    }
    result = 0;
    if (state->word_1c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord24(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_24 = value;
        return;
    }
    if (operation == 1) {
        state->word_24 += value;
        return;
    }
    result = 0;
    if (state->word_24 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord28(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_28 = value;
        return;
    }
    if (operation == 1) {
        state->word_28 += value;
        return;
    }
    result = 0;
    if (state->word_28 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord2c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_2c = value;
        return;
    }
    if (operation == 1) {
        state->word_2c += value;
        return;
    }
    result = 0;
    if (state->word_2c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord30(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_30 = value;
        return;
    }
    if (operation == 1) {
        state->word_30 += value;
        return;
    }
    result = 0;
    if (state->word_30 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord34(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_34 = value;
        return;
    }
    if (operation == 1) {
        state->word_34 += value;
        return;
    }
    result = 0;
    if (state->word_34 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord38(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_38 = value;
        return;
    }
    if (operation == 1) {
        state->word_38 += value;
        return;
    }
    result = 0;
    if (state->word_38 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord3c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_3c = value;
        return;
    }
    if (operation == 1) {
        state->word_3c += value;
        return;
    }
    result = 0;
    if (state->word_3c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord40(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_40 = value;
        return;
    }
    if (operation == 1) {
        state->word_40 += value;
        return;
    }
    result = 0;
    if (state->word_40 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord44(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_44 = value;
        return;
    }
    if (operation == 1) {
        state->word_44 += value;
        return;
    }
    result = 0;
    if (state->word_44 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord48(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_48 = value;
        return;
    }
    if (operation == 1) {
        state->word_48 += value;
        return;
    }
    result = 0;
    if (state->word_48 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord14(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_14 = value;
        return;
    }
    if (operation == 1) {
        state->word_14 += value;
        return;
    }
    result = 0;
    if (state->word_14 == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->word_4c = value;
        return;
    }
    if (operation == 1) {
        state->word_4c += value;
        return;
    }
    result = 0;
    if (state->word_4c == value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        state->address_50 = value;
        return;
    }
    if (operation == 1) {
        state->address_50 += (u32)value * 4;
        return;
    }
    result = 0;
    if (state->address_50 == (u32)value) {
        result = 1;
    }
    state->comparison_result = result;
}

void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->byte_54 = value;
    } else if (operation == 1) {
        state->byte_54 = (u8)((u32)state->byte_54 + (u32)value);
    } else {
        result = 0;
        if (state->byte_54 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte55(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_55 = value;
    } else if (operation == 1) {
        state->byte_55 = (u8)((u32)state->byte_55 + (u32)value);
    } else {
        result = 0;
        if (state->byte_55 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte56(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_56 = value;
    } else if (operation == 1) {
        state->byte_56 = (u8)((u32)state->byte_56 + (u32)value);
    } else {
        result = 0;
        if (state->byte_56 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        state->comparison_result = value;
    } else if (operation == 1) {
        state->comparison_result =
            (u8)((u32)state->comparison_result + (u32)value);
    } else {
        result = 0;
        if (state->comparison_result == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte58(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_58 = value;
    } else if (operation == 1) {
        state->byte_58 = (u8)((u32)state->byte_58 + (u32)value);
    } else {
        result = 0;
        if (state->byte_58 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte59(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_59 = value;
    } else if (operation == 1) {
        state->byte_59 = (u8)((u32)state->byte_59 + (u32)value);
    } else {
        result = 0;
        if (state->byte_59 == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte5a(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_5a = value;
    } else if (operation == 1) {
        state->byte_5a = (u8)((u32)state->byte_5a + (u32)value);
    } else {
        result = 0;
        if (state->byte_5a == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte5b(struct ScriptOperands *state, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        state->byte_5b = value;
    } else if (operation == 1) {
        state->byte_5b = (u8)((u32)state->byte_5b + (u32)value);
    } else {
        result = 0;
        if (state->byte_5b == (u8)value)
            result = 1;
        state->comparison_result = result;
    }
}

void Script_SetOrCompareByte5d(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->byte_5d = value;
    } else if (operation == 1) {
        work->byte_5d += value;
    } else {
        result = 0;
        if (work->byte_5d == (u8)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->halfword_5e = value;
        return;
    }
    if (operation == 1) {
        work->halfword_5e = work->halfword_5e + value;
        return;
    }
    result = 0;
    if ((s16)work->halfword_5e == (s16)value) {
        result = 1;
    }
    work->comparison_result = result;
}

void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->signed_halfword_64 = value;
    } else if (operation == 1) {
        work->signed_halfword_64 =
            (u16)work->signed_halfword_64 + value;
    } else {
        result = 0;
        if (work->signed_halfword_64 == (s16)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;

    if (operation == 0) {
        work->signed_halfword_66 = value;
    } else if (operation == 1) {
        work->signed_halfword_66 =
            (u16)work->signed_halfword_66 + value;
    } else {
        result = 0;
        if (work->signed_halfword_66 == (s16)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareWord68(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->word_68 = value;
        return;
    }
    if (operation == 1) {
        work->word_68 = work->word_68 + (u32)value;
        return;
    }
    result = 0;
    if (work->word_68 == (u32)value) {
        result = 1;
    }
    work->comparison_result = result;
}

void Script_SetOrCompareWord6c(struct ScriptOperands *work, s32 operation, s32 value)
{
    s8 result;

    if (operation == 0) {
        work->word_6c = value;
        return;
    }
    if (operation == 1) {
        work->word_6c = work->word_6c + (u32)value;
        return;
    }
    result = 0;
    if (work->word_6c == (u32)value) {
        result = 1;
    }
    work->comparison_result = result;
}

void Script_SetOrCompareByte62(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        work->byte_62 = value;
    } else if (operation == 1) {
        work->byte_62 += value;
    } else {
        result = 0;
        if (work->byte_62 == (u8)value)
            result = 1;
        work->comparison_result = result;
    }
}

void Script_SetOrCompareByte63(struct ScriptOperands *work, s32 operation, s32 value)
{
    s32 result;
    if (operation == 0) {
        work->byte_63 = value;
    } else if (operation == 1) {
        work->byte_63 += value;
    } else {
        result = 0;
        if (work->byte_63 == (u8)value)
            result = 1;
        work->comparison_result = result;
    }
}

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Script_OperandHandlerTable[*(s32 *)entry];

    if (callback != 0)
        callback(work, 0, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Script_OperandHandlerTable[*(s32 *)entry];

    if (callback != 0)
        callback(work, 1, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    u8 *entry = (u8 *)(work->script_address + index * 4 + 4);
    OperandFunc callback = Script_OperandHandlerTable[*(s32 *)entry];

    if (callback != 0)
        callback(work, 2, *(s32 *)(entry + 4));
    work->cursor += 3;
    return 1;
}

void ObjectDispatch_SetField6c(void *arg0, s32 arg1)
{
    if (arg0 != NULL) {
        *(s32 *)((u8 *)arg0 + 0x6C) = arg1;
    }
}

/* The Japanese edition tests the held keys where the others read the key state. */
#if EDITION_INTERNATIONAL

extern u8 Data_03001c94[];

#define ASSIGNED_KEYS Data_03001c94
#else

extern u8 gKeysHeld[];

#define ASSIGNED_KEYS gKeysHeld
#endif

u32 Field_StoreAssignedKeyValue(u32 value)
{
    u32 no = value >> 14;
    u32 ret = 0x3FFF & value;
    u8 *state = gWork;

    if (GameFlag_TestFar(0x107) != 0) {
        StoreHalfword(state + 0x182, 0xFA);
    } else if (*(s16 *)(state + 0x19E) == 3) {
        if (*(volatile u32 *)((u32)&ASSIGNED_KEYS) & 0x100) {
            StoreHalfword(state + 0x182, 0xFC88);
        } else if (*(volatile u32 *)((u32)&ASSIGNED_KEYS) & 0x200) {
            StoreHalfword(state + 0x182, 0xFC87);
        }
    } else {
        switch (no) {
        case 0:
            StoreHalfword(state + 0x17E, ret);
            break;
        case 1:
            StoreHalfword(state + 0x180, ret);
            break;
        }
    }

    return ret;
}

/* field/check_configured_keys.c */
/* キー入力と設定表の照合。押下キーに対応する番号欄へ1を立てる。
   該当が無ければ表の値を Field_StoreAssignedKeyValue へ渡す。
   キー状態は割り込みで更新されるため、判定ごとに読み直す。 */
s32 Field_CheckConfiguredKeys(void)
{
    u8 *work = gWork;
    s32 ret = 0;

    if (work == NULL) {
        return 0;
    }

    if (gKeyState & gGameState[266]) {
        s16 *q = (s16 *)(work + 185 * 2);
        s32 v = 1;
        *q = v;
        ret = 1;
    } else if (gKeyState & gGameState[264]) {
        s16 *q = (s16 *)(work + 186 * 2);
        s32 v = 1;
        *q = v;
        ret = 1;
    } else if (gKeyState & gGameState[267]) {
        s16 *q = (s16 *)(work + 187 * 2);
        s32 v = 1;
        *q = v;
        ret = 1;
    } else if (gKeyState & gGameState[268]) {
        ret = Field_StoreAssignedKeyValue(gGameState[272]);
    } else if (gKeyState & gGameState[269]) {
        ret = Field_StoreAssignedKeyValue(gGameState[273]);
    }

    return ret;
}

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 arg1, s32 *b, s32 arg3)
{
    s32 dx = (*a++ - *b++) >> 16;
    s32 dy = (*a++ - *b++) >> 16;
    s32 dz = (*a - *b) >> 16;
    s32 radius = arg1 + arg3;
    if (!(dx > 0x400000) && !(dz > 0x400000) &&
        (dx *dx + dy *dy + dz *dz) < radius *radius)
        return 0;
    return -1;
}
