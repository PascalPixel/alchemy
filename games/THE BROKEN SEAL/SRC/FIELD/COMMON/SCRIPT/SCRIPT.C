#include "EDITION.H"
#include "SCRIPT.H"
#include "TYPES.H"
#include "SCENE.H"
#include "GLOBAL_CELLS.H"
#include "SCRIPT_OBJECT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"
#include "SCRIPT_OBJECT_ENTRY.H"
#include "IWRAM_CALL.H"
#include "FIELDRUN.H"
#include "EVENT_RUNTIME.H"
#include "GAME_STATE.H"

u32 Random16(void);
void Vector_AddPolarOffset(s32 radius, s32 angle, struct FieldPosition *position);
s32 Func_080120dc(struct ObjectRuntime *object, struct FieldPosition *position);
u16 ArcTan2(s32 y, s32 x);

/*
 * Script command: wander to a random point. The three arguments are the
 * base distance, the random extra distance and the leash radius around the
 * object's home cell. Up to seven headings within a quarter turn either
 * side of the facing are tried; each must be free of objects, and the point
 * a further half tile on, turned an eighth either side, must be walkable
 * and inside the leash. When none fits, the object turns round.
 */
s32 Object_Wander(struct ScriptObjectRuntime *object)
{
    struct FieldPosition pos;
    struct FieldPosition probe;
    s32 base;
    const s32 *args;
    s32 range;
    s32 limit;
    s32 radius;
    s32 heading;
    s32 tries;
    s32 dx;
    s32 dz;

    args = &object->script[(s16)object->script_cursor + 1];
    base = *args++;
    range = *args++;
    limit = *args / 0x10000;
    /* FAKEMATCH: the seven-trial retry entry and single-pass distance
       block retain the native 32-byte frame; the ordinary for/early-return
       form uses 36 bytes and changes the division/load registers. */
    tries = 0;
    limit = limit * limit;
retry:
    tries++;
    if (tries <= 7) {
        pos.x = object->x;
        pos.y = object->y;
        pos.z = object->z;
        radius = base + Iwram_MulQ16(Random16(), range);
        heading = object->script_value + (Random16() >> 2) - (Random16() >> 2);
        Vector_AddPolarOffset(radius, heading, &pos);
        if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
            goto retry;
        if (Func_080120dc((struct ObjectRuntime *)object, &pos) != 0)
            goto retry;
        radius += 0x80000;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading, &probe);
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
        if (Func_080120dc((struct ObjectRuntime *)object, &probe) != 0)
            goto retry;
        probe.x = object->x;
        probe.y = object->y;
        probe.z = object->z;
        Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
        if (Func_080120dc((struct ObjectRuntime *)object, &probe) != 0)
            goto retry;
        do {
            dx = pos.x / 0x10000 - object->home_x;
            dz = pos.z / 0x10000 - object->home_z;
        } while (0);
        if (dx * dx + dz * dz > limit)
            goto retry;
        goto found;
    }
    object->script_value += 0x8000;
    object->turned_back = 1;
    return 0;
found:
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
    object->script_cursor += 4;
    return 1;
}

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
    struct FieldPosition pos;
    struct FieldPosition probe;
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
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
        goto roam;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Func_080120dc((struct ObjectRuntime *)object, &pos) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    radius += 0x80000;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading, &probe);
    if (Func_080120dc((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x2000, &probe);
    if (Func_080120dc((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x2000, &probe);
    if (Func_080120dc((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading + 0x4000, &probe);
    if (Func_080120dc((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    probe.x = object->x;
    probe.y = object->y;
    probe.z = object->z;
    Vector_AddPolarOffset(radius, heading - 0x4000, &probe);
    if (Func_080120dc((struct ObjectRuntime *)object, &probe) != 0)
        goto roam;
    dx = pos.x / 0x10000 - object->home_x;
    dz = pos.z / 0x10000 - object->home_z;
    if (dx * dx + dz * dz > limit)
        goto roam;
    object->flags_59 |= 2;
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
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
    if (ScriptObject_CheckOverlap((struct ScriptObjectEntry *)object, (s32 *)&pos) != 0)
        goto back;
    pos.x = object->x;
    pos.y = object->y;
    pos.z = object->z;
    Vector_AddPolarOffset(radius, heading, &pos);
    if (Func_080120dc((struct ObjectRuntime *)object, &pos) != 0)
        goto back;
    object->flags_59 &= ~2;
    Object_SetMoveTarget((struct ObjectRuntime *)object, pos.x, pos.y, pos.z);
done:
    object->script_cursor += 4;
    return 1;
}

enum ScriptOperandOperation {
    SCRIPT_OPERAND_SET,
    SCRIPT_OPERAND_ADD,
    SCRIPT_OPERAND_COMPARE
};

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);
extern OperandFunc Script_OperandHandlerTable[];

/* field/store_assigned_key_value.c */
s32 GameFlag_TestFar(s32);

static __inline__ void StoreHalfword(s16 *address, s32 value)
{
    *address = value;
}

extern volatile u32 gKeyState;

void Script_SetOrCompareAddress(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->script_address = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->script_address += (u32)value * 4;
        return;
    }
    state->comparison_result = state->script_address == (u32)value;
}

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->cursor = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->cursor =
            (u16)((u32)(s32)(s16)state->cursor + (u32)value);
    } else {
        state->comparison_result = (s16)state->cursor == (s16)value;
    }
}

void Script_SetOrCompareUnsignedHalfword(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->unsigned_halfword = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->unsigned_halfword =
            (u16)((u32)state->unsigned_halfword + (u32)value);
    } else {
        state->comparison_result = state->unsigned_halfword == (u16)value;
    }
}

void Script_SetOrCompareWord08(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_08 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_08 += value;
        return;
    }
    state->comparison_result = state->word_08 == value;
}

void Script_SetOrCompareWord0c(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_0c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_0c += value;
        return;
    }
    state->comparison_result = state->word_0c == value;
}

void Script_SetOrCompareWord10(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_10 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_10 += value;
        return;
    }
    state->comparison_result = state->word_10 == value;
}

void Script_SetOrCompareHalfword20(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->halfword_20 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->halfword_20 = (u16)((u32)state->halfword_20 + (u32)value);
        return;
    }
    state->comparison_result = state->halfword_20 == (s16)value;
}

void Script_SetOrCompareWord18(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_18 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_18 += value;
        return;
    }
    state->comparison_result = state->word_18 == value;
}

void Script_SetOrCompareWord1c(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_1c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_1c += value;
        return;
    }
    state->comparison_result = state->word_1c == value;
}

void Script_SetOrCompareWord24(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_24 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_24 += value;
        return;
    }
    state->comparison_result = state->word_24 == value;
}

void Script_SetOrCompareWord28(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_28 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_28 += value;
        return;
    }
    state->comparison_result = state->word_28 == value;
}

void Script_SetOrCompareWord2c(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_2c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_2c += value;
        return;
    }
    state->comparison_result = state->word_2c == value;
}

void Script_SetOrCompareWord30(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_30 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_30 += value;
        return;
    }
    state->comparison_result = state->word_30 == value;
}

void Script_SetOrCompareWord34(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_34 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_34 += value;
        return;
    }
    state->comparison_result = state->word_34 == value;
}

void Script_SetOrCompareWord38(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_38 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_38 += value;
        return;
    }
    state->comparison_result = state->word_38 == value;
}

void Script_SetOrCompareWord3c(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_3c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_3c += value;
        return;
    }
    state->comparison_result = state->word_3c == value;
}

void Script_SetOrCompareWord40(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_40 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_40 += value;
        return;
    }
    state->comparison_result = state->word_40 == value;
}

void Script_SetOrCompareWord44(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_44 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_44 += value;
        return;
    }
    state->comparison_result = state->word_44 == value;
}

void Script_SetOrCompareWord48(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_48 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_48 += value;
        return;
    }
    state->comparison_result = state->word_48 == value;
}

void Script_SetOrCompareWord14(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_14 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_14 += value;
        return;
    }
    state->comparison_result = state->word_14 == value;
}

void Script_SetOrCompareWord4c(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->word_4c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->word_4c += value;
        return;
    }
    state->comparison_result = state->word_4c == value;
}

void Script_SetOrCompareAddress50(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->address_50 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        state->address_50 += (u32)value * 4;
        return;
    }
    state->comparison_result = state->address_50 == (u32)value;
}

void Script_SetOrCompareByte54(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_54 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_54 = (u8)((u32)state->byte_54 + (u32)value);
    } else {
        state->comparison_result = state->byte_54 == (u8)value;
    }
}

void Script_SetOrCompareByte55(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_55 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_55 = (u8)((u32)state->byte_55 + (u32)value);
    } else {
        state->comparison_result = state->byte_55 == (u8)value;
    }
}

void Script_SetOrCompareByte56(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_56 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_56 = (u8)((u32)state->byte_56 + (u32)value);
    } else {
        state->comparison_result = state->byte_56 == (u8)value;
    }
}

void Script_SetOrCompareComparisonResult(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->comparison_result = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->comparison_result =
            (u8)((u32)state->comparison_result + (u32)value);
    } else {
        state->comparison_result = state->comparison_result == (u8)value;
    }
}

void Script_SetOrCompareByte58(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_58 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_58 = (u8)((u32)state->byte_58 + (u32)value);
    } else {
        state->comparison_result = state->byte_58 == (u8)value;
    }
}

void Script_SetOrCompareByte59(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_59 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_59 = (u8)((u32)state->byte_59 + (u32)value);
    } else {
        state->comparison_result = state->byte_59 == (u8)value;
    }
}

void Script_SetOrCompareByte5a(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_5a = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_5a = (u8)((u32)state->byte_5a + (u32)value);
    } else {
        state->comparison_result = state->byte_5a == (u8)value;
    }
}

void Script_SetOrCompareByte5b(struct ScriptOperands *state, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        state->byte_5b = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        state->byte_5b = (u8)((u32)state->byte_5b + (u32)value);
    } else {
        state->comparison_result = state->byte_5b == (u8)value;
    }
}

void Script_SetOrCompareByte5d(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->byte_5d = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->byte_5d += value;
    } else {
        work->comparison_result = work->byte_5d == (u8)value;
    }
}

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->halfword_5e = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        work->halfword_5e = work->halfword_5e + value;
        return;
    }
    work->comparison_result = (s16)work->halfword_5e == (s16)value;
}

void Script_SetOrCompareSignedHalfword64(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->signed_halfword_64 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->signed_halfword_64 =
            (u16)work->signed_halfword_64 + value;
    } else {
        work->comparison_result = work->signed_halfword_64 == (s16)value;
    }
}

void Script_SetOrCompareSignedHalfword66(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->signed_halfword_66 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->signed_halfword_66 =
            (u16)work->signed_halfword_66 + value;
    } else {
        work->comparison_result = work->signed_halfword_66 == (s16)value;
    }
}

void Script_SetOrCompareWord68(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->word_68 = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        work->word_68 = work->word_68 + (u32)value;
        return;
    }
    work->comparison_result = work->word_68 == (u32)value;
}

void Script_SetOrCompareWord6c(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->word_6c = value;
        return;
    }
    if (operation == SCRIPT_OPERAND_ADD) {
        work->word_6c = work->word_6c + (u32)value;
        return;
    }
    work->comparison_result = work->word_6c == (u32)value;
}

void Script_SetOrCompareByte62(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->byte_62 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->byte_62 += value;
    } else {
        work->comparison_result = work->byte_62 == (u8)value;
    }
}

void Script_SetOrCompareByte63(struct ScriptOperands *work, s32 operation, s32 value)
{
    if (operation == SCRIPT_OPERAND_SET) {
        work->byte_63 = value;
    } else if (operation == SCRIPT_OPERAND_ADD) {
        work->byte_63 += value;
    } else {
        work->comparison_result = work->byte_63 == (u8)value;
    }
}

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    const s32 *command = (const s32 *)work->script_address + index + 1;
    OperandFunc handler = Script_OperandHandlerTable[command[0]];

    if (handler != 0)
        handler(work, SCRIPT_OPERAND_SET, command[1]);
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    const s32 *command = (const s32 *)work->script_address + index + 1;
    OperandFunc handler = Script_OperandHandlerTable[command[0]];

    if (handler != 0)
        handler(work, SCRIPT_OPERAND_ADD, command[1]);
    work->cursor += 3;
    return 1;
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
    s16 index = (s16)work->cursor;
    const s32 *command = (const s32 *)work->script_address + index + 1;
    OperandFunc handler = Script_OperandHandlerTable[command[0]];

    if (handler != 0)
        handler(work, SCRIPT_OPERAND_COMPARE, command[1]);
    work->cursor += 3;
    return 1;
}

void ObjectDispatch_SetField6c(void *raw, s32 value)
{
    struct ObjectRuntime *object = raw;

    if (object != NULL)
        *(s32 *)object->unknown_6c = value;
}

/* The Japanese edition tests the held keys where the others read the key state. */
#if EDITION_INTERNATIONAL

#define ASSIGNED_KEYS gKeyState
#else

extern u8 gKeysHeld[];

#define ASSIGNED_KEYS gKeysHeld
#endif

u32 Field_StoreAssignedKeyValue(u32 value)
{
    u32 selector = value >> 14;
    u32 request = 0x3FFF & value;
    struct FieldStepWork *work = (struct FieldStepWork *)gWork;

    if (GameFlag_TestFar(0x107) != 0) {
        StoreHalfword((s16 *)&work->unknown_182, 0xFA);
    } else if (work->mode == 3) {
        if (*(volatile u32 *)((u32)&ASSIGNED_KEYS) & 0x100) {
            StoreHalfword((s16 *)&work->unknown_182, 0xFC88);
        } else if (*(volatile u32 *)((u32)&ASSIGNED_KEYS) & 0x200) {
            StoreHalfword((s16 *)&work->unknown_182, 0xFC87);
        }
    } else {
        switch (selector) {
        case 0:
            StoreHalfword(&work->unknown_17e, request);
            break;
        case 1:
            StoreHalfword((s16 *)work->unknown_180, request);
            break;
        }
    }

    return request;
}

/* field/check_configured_keys.c */
/* キー入力と設定表の照合。押下キーに対応する番号欄へ1を立てる。
   該当が無ければ表の値を Field_StoreAssignedKeyValue へ渡す。
   キー状態は割り込みで更新されるため、判定ごとに読み直す。 */
s32 Field_CheckConfiguredKeys(void)
{
    struct EventRuntime *work = gWork;
    /* FAKEMATCH: indexed immediate stores introduce two literal-1 loads
       and grow this service from 168 to 192 bytes. Keep the selected
       halfword address and signed word value local to each store. */
    s32 result = 0;

    if (work == NULL)
        return 0;
    if (gKeyState & gGameState.unknown_214) {
        s16 *flag = (s16 *)work->unknown_172;
        s32 value = 1;

        *flag = value;
        result = 1;
    } else if (gKeyState & gGameState.unknown_210) {
        s16 *flag = (s16 *)(work->unknown_172 + 2);
        s32 value = 1;

        *flag = value;
        result = 1;
    } else if (gKeyState & gGameState.unknown_216) {
        s16 *flag = (s16 *)(work->unknown_172 + 4);
        s32 value = 1;

        *flag = value;
        result = 1;
    } else if (gKeyState & gGameState.first_shortcut_keys) {
        result = Field_StoreAssignedKeyValue(gGameState.first_shortcut);
    } else if (gKeyState & gGameState.second_shortcut_keys) {
        result = Field_StoreAssignedKeyValue(gGameState.second_shortcut);
    }
    return result;
}

s32 Runtime_CheckRadiusOverlap(s32 *a, s32 radius_a, s32 *b, s32 radius_b)
{
    const struct FieldPosition *first = (const struct FieldPosition *)a;
    const struct FieldPosition *second = (const struct FieldPosition *)b;
    const s32 *first_word = &first->x;
    const s32 *second_word = &second->x;
    s32 dx = (*first_word++ - *second_word++) >> 16;
    s32 dy = (*first_word++ - *second_word++) >> 16;
    s32 dz = (*first_word - *second_word) >> 16;
    s32 radius = radius_a + radius_b;

    if (!(dx > 0x400000) && !(dz > 0x400000) &&
        dx * dx + dy * dy + dz * dz < radius * radius)
        return 0;
    return -1;
}
