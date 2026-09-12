#include "types.h"

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

Object *Func_080090c8(s32, s32, s32);
void Func_08009098(Object *, const void *);
Unit *Func_08077008(s32);
s32 Func_08077078(Unit *, s32);
s32 Func_080b6d30(s32);
s32 Func_080b6eb4(s32);
s32 Func_080c2384(s32);
s32 Func_080c23a0(s32);
s32 Func_080044d0(s32, s32);
extern const u8 Data_080c5938[];

void Func_080b6f44(Actor *actor, s32 unit, s32 x, s32 y)
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
    object = Func_080090c8(0xf000, fixed_x, 0);
    unit_record = Func_08077008(unit);
    actor_flag = 0;
    existing_sprite = Func_080b6d30(unit);

    if (unit_record->kind == 0) {
        sprite = Func_080c2384(unit_record->class_id);
        if (existing_sprite == 0)
            actor_flag = Func_080c23a0(unit_record->class_id);
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
    anim = Func_080b6eb4(unit);
    actor->field_08 = 0;
    actor->field_20 = 0;
    actor->field_24 = 0;
    actor->field_28 = 0;
    actor->field_2a = 0;
    actor->field_0a = 0x1fe;
    class_id = unit_record->class_id;
    actor->anim = anim;

    if (class_id <= 1 && Func_08077078(unit_record, 1) == 15) {
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
    position = Func_080044d0(row >> 3, x) + 0x8000;
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
    Func_08009098(object, Data_080c5938);
}
