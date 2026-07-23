typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

struct Object {
    u8 pad0[6];
    u16 x;
    u16 y;
    u8 padA[10];
    s8 byte14;
    u8 pad15;
    u16 attr16;
};

struct State {
    struct Object *object;
    u8 pad4[4];
    s16 targetX;
    s16 targetY;
};

void Func_080b0958(struct State *state) {
    struct Object *object;
    s32 difference;
    s32 amount;
    s32 upper;
    s32 lower;
    s32 raw;
    s32 result;
    s32 rawUpper;

    object = state->object;
    if (object != 0) {
        difference = object->x - state->targetX;
        amount = difference / 4;
        if (amount < 0)
            amount = -amount;
        if (difference > 0) {
            if (amount != 0)
                amount = object->x - amount;
            else
                amount = object->x + 0xFFFF;
            goto updateX;
        } else if (difference < 0) {
            if (amount != 0)
                amount = object->x + amount;
            else
                amount = object->x + 1;
updateX:
            *(volatile u16 *)&object->x = amount;
            lower = 0x1FF;
            raw = *(volatile u16 *)&object->x;
            lower &= raw;
            rawUpper = object->attr16;
            upper = 0xFFFFFE00;
            upper &= rawUpper;
            result = upper | lower;
            object->attr16 = result;
        }

        difference = object->y - state->targetY;
        amount = difference / 4;
        if (amount < 0)
            amount = -amount;
        if (difference > 0) {
            if (amount != 0)
                amount = object->y - amount;
            else
                amount = object->y + 0xFFFF;
            goto updateY;
        } else if (difference < 0) {
            if (amount != 0)
                amount = object->y + amount;
            else
                amount = object->y + 1;
updateY:
            *(volatile u16 *)&object->y = amount;
            object->byte14 = *(volatile u16 *)&object->y;
        }
    }
}
