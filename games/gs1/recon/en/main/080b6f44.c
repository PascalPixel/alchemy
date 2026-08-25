#include "types.h"

#define BYTE(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

void *Func_080090c8(s32, s32, s32);
void Func_08009098(void *, const void *);
void *Func_08077008(s32);
s32 Func_08077078(void *, s32);
s32 Func_080b6d30(s32);
s32 Func_080b6eb4(s32);
s32 Func_080c2384(s32);
s32 Func_080c23a0(s32);
s32 Func_080044d0(s32, s32);
extern const u8 Data_080c5938[];

void Func_080b6f44(void *actor, s32 unit, s32 x, s32 y)
{
    s32 saved_y;
    s32 fixed_x;
    s32 fixed_y;
    s32 actor_flag;
    s32 sprite;
    s32 scale;
    s32 existing_sprite;
    void *unit_record;
    void *object;
    u8 class_id;

    saved_y = y;
    fixed_x = x << 16;
    fixed_y = y << 16;
    object = Func_080090c8(0xf000, fixed_x, 0);
    unit_record = Func_08077008(unit);
    actor_flag = 0;
    existing_sprite = Func_080b6d30(unit);
    class_id = BYTE(unit_record, 296);

    if (BYTE(unit_record, 297) == 0) {
        sprite = Func_080c2384(class_id);
        if (existing_sprite == 0)
            actor_flag = Func_080c23a0(class_id);
        else
            sprite = existing_sprite;
    } else {
        switch (class_id) {
        case 1: sprite = 301; break;
        case 2: sprite = 302; break;
        case 3: sprite = 303; break;
        case 5: sprite = 305; break;
        default: sprite = 300; break;
        }
        if ((u32)unit > 7)
            actor_flag = 1;
    }

    scale = 0x10000;
    switch (class_id) {
    case 18: case 19: case 20: case 21: case 30:
    case 68: case 69: case 70: case 92:
    case 122: case 123: case 124: case 125: case 126:
        scale = 0xe666; break;
    case 29: case 84:
        scale = 0x10000; break;
    case 47: case 48: case 110: case 136: case 137: case 144:
        scale = 0x13333; break;
    case 49: case 128: case 129:
        scale = 0x16666; break;
    case 52: case 85: case 105:
        scale = 0x14000; break;
    case 78: case 131:
        scale = 0x19999; break;
    case 89: case 146:
        scale = 0x18ccc; break;
    case 94: case 138: case 141: case 145:
    case 148: case 150: case 151: case 152: case 153:
    case 154: case 155: case 156: case 157:
        scale = 0x18000; break;
    case 98:
        scale = 0x14ccc; break;
    case 121:
        scale = 0x1b333; break;
    case 130:
        scale = 0x13333; break;
    case 132: case 133:
        scale = 0x10ccc; break;
    case 134: case 135:
        scale = 0x11999; break;
    case 147: case 149:
        scale = 0x1cccc; break;
    default:
        break;
    }

    WORD(actor, 0) = (s32)object;
    HALF(actor, 4) = sprite;
    HALF(actor, 6) = Func_080b6eb4(unit);
    HALF(actor, 8) = 0;
    HALF(actor, 10) = 0x1fe;
    WORD(actor, 12) = fixed_x;
    WORD(actor, 16) = fixed_y;
    WORD(actor, 20) = actor_flag;
    WORD(actor, 24) = scale;
    WORD(actor, 32) = 0;
    WORD(actor, 36) = 0;
    HALF(actor, 40) = 0;
    HALF(actor, 42) = 0;

    if (class_id <= 1 && Func_08077078(unit_record, 1) == 15) {
        HALF(actor, 4) = class_id == 0 ? 480 : 482;
        HALF(actor, 6) = 0;
    }

    HALF(object, 6) = Func_080044d0(
        (saved_y + (saved_y < 0 ? 7 : 0)) >> 3,
        x) + 0x8000;
    BYTE(object, 89) = 3;
    BYTE(object, 85) = 2;
    scale = BYTE(unit_record, 297) == 0 ? 0xe666 : 0x10000;
    WORD(object, 24) = scale;
    WORD(object, 28) = scale;
    Func_08009098(object, Data_080c5938);
}
