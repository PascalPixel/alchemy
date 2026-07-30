typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

struct EffectModifier {
    u8 kind;
    s8 value;
    u8 unknown_2[2];
};

struct EffectDefinition {
    u8 unknown_00[2];
    u8 kind;
    u8 flags;
    u8 unknown_04[4];
    s16 primary_value;
    s8 secondary_value;
    u8 unknown_0b;
    u8 category;
    u8 unknown_0d[0x0b];
    struct EffectModifier modifiers[4];
};

struct EffectDefinition *Func_08077018(s32 effect);
void Func_08015080(s32 text, s32 window, s32 x, s32 y);
void Func_08015098(const void *resource, s32 window, s32 x, s32 y);
void Func_080150b0(s32 value, s32 width, s32 window, s32 x, s32 y);
void Func_08015120(s32 value, s32 width);
void Func_08015280(s32 window, s32 text, s32 x, s32 y, s32 width);
void Func_080a4db4(s16 value, s32 width, s32 window, s32 x, s32 y);

void Func_080a4924(s32 window, s32 encoded_effect)
{
    struct EffectDefinition *effect =
        Func_08077018(encoded_effect & 0x1ff);
    volatile s32 branch_barrier = 0;
    s32 row = 0;
    s32 section_started = 0;
    s32 i;

    if (effect->kind != 0) {
        s32 has_summary =
            effect->primary_value != 0 || effect->secondary_value != 0;

        for (i = 0; i < 4 && !has_summary; i++) {
            if (effect->modifiers[i].kind != 0 || effect->category == 3)
                has_summary = 1;
        }

        if (has_summary) {
            Func_08015080(0xb6d, window, 0x10, 0);
            row++;
        }

        if (effect->primary_value != 0) {
            s32 y = row * 8;
            Func_08015080(0xaf7, window, 0, y);
            Func_080a4db4(effect->primary_value, 3, window, 0x40, y);
            row++;
        }

        if (effect->secondary_value != 0) {
            s32 y = row * 8;
            Func_08015080(0xaf8, window, 0, y);
            Func_080a4db4(effect->secondary_value, 3, window, 0x40, y);
            row++;
        }
    }

    for (i = 0; i < 4; i++) {
        struct EffectModifier *modifier = &effect->modifiers[i];
        s32 y;

        if (modifier->kind == 0)
            continue;
        y = row * 8;

        switch (modifier->kind) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 26:
            Func_08015080(modifier->kind + 0xb3b, window, 0, y);
            Func_080a4db4(modifier->value, 3, window, 0x40, y);
            break;

        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
            Func_08015280(
                window, ((modifier->kind - 15) & 3) + 1, 0, row, 2);
            Func_08015080(modifier->kind + 0xb3b, window, 8, y);
            Func_080a4db4(modifier->value, 3, window, 0x40, y);
            break;

        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14: {
            s32 digit;

            Func_08015080(modifier->kind + 0xb3b, window, 0, y);
            Func_08015098((const void *)0x080af21c, window, 0x40, y);
            if (modifier->value > 9) {
                Func_080150b0(1, 1, window, 0x48, y);
                Func_08015098((const void *)0x080af220, window, 0x50, y);
                digit = modifier->value - 10;
            } else {
                Func_080150b0(0, 1, window, 0x48, y);
                Func_08015098((const void *)0x080af220, window, 0x50, y);
                digit = modifier->value;
            }
            Func_080150b0(digit, 1, window, 0x58, y);
            break;
        }

        case 23:
        case 25:
        case 27:
            Func_08015080(modifier->kind + 0xb3b, window, 0, y);
            break;
        }
        row++;
    }

    if (effect->flags & 1) {
        Func_08015080(0xb76, window, 0, row * 8);
        row++;
    }

    if (effect->category == 3) {
        Func_08015080(0xb65, window, 0, row * 8);
        row++;
        section_started = 1;
    }

    if (effect->category != 0 && effect->category != 4) {
        if (!section_started) {
            Func_08015080(0xb6e, window, 0x10, row * 8);
            row++;
        }
        switch (effect->category) {
        case 1:
            Func_08015080(0xb63, window, 0, row * 8);
            row++;
            break;
        case 2:
            if (encoded_effect & 0x400) {
                Func_08015080(0xb73, window, 0, row * 8);
                branch_barrier = 1;
                row++;
                Func_08015080(0xb74, window, 0, row * 8);
                branch_barrier = 2;
                row++;
            } else {
                Func_08015080(0xb71, window, 0, row * 8);
                branch_barrier = 3;
                row++;
                Func_08015080(0xb72, window, 0, row * 8);
                branch_barrier = 4;
                row++;
            }
            break;
        }
    }

    if (effect->flags & 0x10) {
        if (row != 0)
            row++;
        Func_08015080(0xb6f, window, 0x10, row * 8);
        Func_08015120(((encoded_effect & 0xf800) >> 11) + 1, 5);
        row++;
        Func_08015080(0xb70, window, 0, row * 8);
        row++;
    }

    if (row == 0) {
        if (effect->flags & 4) {
            Func_08015080(0xb69, window, 0, 0);
            branch_barrier = 5;
        } else if (effect->flags & 8) {
            Func_08015080(0xb6a, window, 0, 0);
            branch_barrier = 6;
        } else {
            Func_08015080(0xb6c, window, 0, 0);
            branch_barrier = 7;
        }
    }
    (void)branch_barrier;
}
