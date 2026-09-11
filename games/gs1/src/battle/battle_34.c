#include "types.h"
#include "scene.h"

/* battle/unit/lookup_weapon_value_by_class.c */
u8 *Runtime_GetObject(s32);

extern u16 gRom[];
extern u16 gRom2[];
extern u16 gRom3[];
extern u16 gRom4[];
extern u16 gRom5[];

s32 BattleUnit_LookupWeaponValueByClass(s32 id)
{
    u8 *state;
    s32 entry;
    s32 result;

    state = Runtime_GetObject(id);
    entry = Battle_Apply(id, 1);
    result = 0;
    if (entry >= 0) {
        s32 ofs;
        s32 sel;

        ofs = entry * 2 + 216;
        sel = Battle_Do(*(u16 *)(state + ofs) & 0x1FF);
        switch (state[296]) {
        case 0:
            result = gRom[sel];
            break;
        case 1:
            result = gRom2[sel];
            break;
        case 2:
            result = gRom3[sel];
            break;
        case 3:
            result = gRom4[sel];
            break;
        case 4:
            break;
        case 5:
            result = gRom5[sel];
            break;
        }
    }
    return result;
}

/* battle/presentation/actor/spawn.c */
typedef struct {
    /* 0x00 */ void *object;
    /* 0x04 */ u16 sprite;
    /* 0x06 */ u16 anim;
    /* 0x08 */ u16 field_08;
    /* 0x0a */ u16 field_0a;
    /* 0x0c */ s32 x;
    /* 0x10 */ s32 y;
    /* 0x14 */ s32 flag;
    /* 0x18 */ s32 scale;
    /* 0x1c */ s32 field_1c;
    /* 0x20 */ s32 field_20;
    /* 0x24 */ s32 field_24;
    /* 0x28 */ u16 field_28;
    /* 0x2a */ u16 field_2a;
} Actor;

typedef struct {
    /* 0x000 */ u8 field_000[296];
    /* 0x128 */ unsigned int class_id : 8;
    /* 0x129 */ unsigned int kind : 8;
} Unit;

typedef struct {
    /* 0x00 */ u8 field_00[6];
    /* 0x06 */ u16 field_06;
    /* 0x08 */ u8 field_08[16];
    /* 0x18 */ s32 field_18;
    /* 0x1c */ s32 field_1c;
    /* 0x20 */ u8 field_20[53];
    /* 0x55 */ u8 field_55;
    /* 0x56 */ u8 field_56[3];
    /* 0x59 */ u8 field_59;
} Object;

Object *Actor_Run(s32, s32, s32);

Unit *Actor_unk2_4(s32);

extern const u8 gRom[];

void Actor_unk3_4(Actor *actor, s32 unit, s32 x, s32 y)
{
    s32 fixed_x;
    s32 fixed_y;
    s32 actor_flag;
    s32 sprite;
    s32 existing_sprite;
    Unit *unit_record;
    Object *object;
    s32 position;
    s32 row;
    s32 anim;
    u8 class_id;

    fixed_x = x << 16;
    fixed_y = y << 16;
    object = Actor_Run(0xf000, fixed_x, 0);
    unit_record = Actor_unk2_4(unit);
    actor_flag = 0;
    existing_sprite = Actor_Check(unit);

    if (unit_record->kind == 0) {
        sprite = Actor_unk2(unit_record->class_id);
        if (existing_sprite == 0)
            actor_flag = Actor_unk3(unit_record->class_id);
        else
            sprite = existing_sprite;
    } else {
        switch (unit_record->class_id) {
        case 1: sprite = 301; break;
        case 3: sprite = 303; break;
        case 2: sprite = 302; break;
        case 5: sprite = 305; break;
        case 0:
        default: sprite = 300; break;
        }
        if ((u32)unit > 7)
            actor_flag = 1;
    }

    actor->scale = 0x10000;
    switch (unit_record->class_id) {
    case 78: actor->scale = 0x19999; break;
    case 89: actor->scale = 0x18ccc; break;
    case 130: actor->scale = 0x13333; break;
    case 131: actor->scale = 0x19999; break;
    case 138: actor->scale = 0x18000; break;
    case 147: actor->scale = 0x1cccc; break;
    case 149: actor->scale = 0x1cccc; break;
    case 29: actor->scale = 0x10000; break;
    case 121: actor->scale = 0x1b333; break;
    case 148: actor->scale = 0x18000; break;
    case 150: actor->scale = 0x18000; break;
    case 151: actor->scale = 0x18000; break;
    case 152: actor->scale = 0x18000; break;
    case 153: actor->scale = 0x18000; break;
    case 154: actor->scale = 0x18000; break;
    case 155: actor->scale = 0x18000; break;
    case 156: actor->scale = 0x18000; break;
    case 157: actor->scale = 0x18000; break;
    case 47: actor->scale = 0x13333; break;
    case 48: actor->scale = 0x13333; break;
    case 49: actor->scale = 0x16666; break;
    case 84: actor->scale = 0x10000; break;
    case 85: actor->scale = 0x14000; break;
    case 128: actor->scale = 0x16666; break;
    case 129: actor->scale = 0x16666; break;
    case 94: actor->scale = 0x18000; break;
    case 98: actor->scale = 0x14ccc; break;
    case 110: actor->scale = 0x13333; break;
    case 132: actor->scale = 0x10ccc; break;
    case 133: actor->scale = 0x10ccc; break;
    case 134: actor->scale = 0x11999; break;
    case 135: actor->scale = 0x11999; break;
    case 136: actor->scale = 0x13333; break;
    case 137: actor->scale = 0x13333; break;
    case 141: actor->scale = 0x18000; break;
    case 144: actor->scale = 0x13333; break;
    case 145: actor->scale = 0x18000; break;
    case 146: actor->scale = 0x18ccc; break;
    case 52: actor->scale = 0x14000; break;
    case 105: actor->scale = 0x14000; break;
    case 18: case 19: case 20: case 21: case 30:
    case 68: case 69: case 70: case 92:
    case 122: case 123: case 124: case 125: case 126:
        actor->scale = 0xe666;
        break;
    }

    actor->object = object;
    actor->x = fixed_x;
    actor->y = fixed_y;
    actor->flag = actor_flag;
    actor->sprite = sprite;
    anim = Actor_unk4(unit);
    actor->field_08 = 0;
    actor->field_20 = 0;
    actor->field_24 = 0;
    actor->field_28 = 0;
    actor->field_2a = 0;
    actor->field_0a = 0x1fe;
    class_id = unit_record->class_id;
    actor->anim = anim;

    if (class_id <= 1 && Actor_Apply(unit_record, 1) == 15) {
        if (unit_record->class_id == 0) {
            sprite = 480;
            actor->sprite = sprite;
        } else {
            sprite = 482;
            actor->sprite = sprite;
        }
        actor->anim = 0;
    }

    row = y;
    if (row < 0)
        row += 7;
    position = Actor_Apply2(row >> 3, x) + 0x8000;
    object->field_06 = position;
    object->field_59 = 3;
    object->field_55 = 2;
    if (unit_record->kind == 0) {
        object->field_18 = 0x14ccc;
        object->field_1c = 0x14ccc;
    } else {
        object->field_18 = 0x10000;
        object->field_1c = 0x10000;
    }
    Actor_Apply3(object, gRom);
}

/* battle/placement/get_step_pair.c */
/* Step table for battle placement: signed bytes in (x, y) pairs. */
extern const s8 gRom[];

/*
 * Read one (x, y) pair from the step table.  Entries are pairs, so the index
 * is doubled and the second component read at index + 1.  A leaf with no
 * prologue; the owner includes the single pool word holding the table base.
 * No call site is known -- the address is reached by a computed value, or by
 * nothing at all.
 */
void BattlePlacement_GetStepPair(s32 index, s32 *x, s32 *y)
{
    index *= 2;
    *x = gRom[index];
    *y = gRom[index + 1];
}
