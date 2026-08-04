#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))

u8 *Func_08077080(s32);
u8 *Func_08077008(s32);
u16 Func_08077190(u16, s16, s32);
void Func_08077128(s32);
u32 Func_08004458(void);
void Func_08015120(s32, s32);
s32 Func_080022ec(s32, s32);
void Func_08077010(s32);

static void SetResult_080a9f10(s16 *result, s32 multi_target, s32 value)
{
    if (!multi_target)
        *result = value;
}

s32 Func_080a9f10(s32 effect_id,
                  s32 source_character,
                  s32 target_character,
                  s32 fixed_amount)
{
    u8 *definition = Func_08077080(effect_id);
    u8 *party = *(u8 **)0x03001f2c;
    s32 selected_target = target_character == 9 ? 0 : target_character;
    u8 *target = Func_08077008(selected_target);
    u16 amount = U16_AT(definition, 0x0a);
    s16 result = 0;
    s32 changed = 0;
    s32 multi_target = 0;
    u32 party_index = 0;

    do {
        s32 kind;

        if (definition[8] == 0xff) {
            selected_target = U16_AT(party, 0x208 + party_index * 2);
            target = Func_08077008(selected_target);
        }

        kind = (definition[1] & 15) - 1;
        switch (kind) {
        case 0: {
            s16 current_hp;
            s16 maximum_hp;

            if (!fixed_amount) {
                s16 source_stat;
                if (definition[2] != 4)
                    source_stat =
                        *(s32 *)(Func_08077008(source_character) +
                                 0x48 + definition[2] * 4);
                else
                    source_stat = 100;
                amount = Func_08077190(amount, source_stat, 0x100);
            }

            current_hp = S16_AT(target, 0x38);
            maximum_hp = S16_AT(target, 0x34);
            if (current_hp <= 0) {
                SetResult_080a9f10(&result, multi_target, 2);
            } else if (current_hp == maximum_hp) {
                SetResult_080a9f10(&result, multi_target, 4);
            } else {
                s32 restored = current_hp + amount;
                S16_AT(target, 0x38) = restored;
                if (restored > maximum_hp) {
                    amount -= restored - maximum_hp;
                    S16_AT(target, 0x38) = maximum_hp;
                    SetResult_080a9f10(&result, multi_target, 0);
                } else {
                    SetResult_080a9f10(&result, multi_target, 1);
                }
                Func_08077128(selected_target);
                changed = 1;
                if (definition[8] == 0xff) {
                    multi_target = 1;
                    result = 3;
                }
            }
            break;
        }

        case 8: {
            s32 random = (Func_08004458() * 4) >> 16;
            s32 adjustment = random == 0 ? -1 : random == 1 ? 0 : 1;

            switch (effect_id & 0x3fff) {
            case 0x104:
                U16_AT(target, 0x10) += amount + adjustment;
                result = 0x10;
                changed = 1;
                break;
            case 0x105:
                U16_AT(target, 0x12) += amount + adjustment;
                result = 0x11;
                changed = 1;
                break;
            case 0x108:
                U16_AT(target, 0x1c) += amount + adjustment;
                result = 0x12;
                changed = 1;
                break;
            case 0x109:
                U8_AT(target, 0x1e) += amount;
                result = 0x13;
                changed = 1;
                break;
            case 0x106:
                U16_AT(target, 0x18) += amount + adjustment;
                Func_08015120(3, 5);
                result = 0x14;
                changed = 1;
                break;
            case 0x107:
                U16_AT(target, 0x1a) += amount + adjustment;
                Func_08015120(4, 5);
                result = 0x15;
                changed = 1;
                break;
            }
            break;
        }

        case 10: {
            s16 current_pp = S16_AT(target, 0x3a);
            s16 maximum_pp = S16_AT(target, 0x36);
            if (current_pp == maximum_pp) {
                SetResult_080a9f10(&result, multi_target, 7);
            } else {
                s32 restored = current_pp + amount;
                S16_AT(target, 0x3a) = restored;
                if (restored > maximum_pp) {
                    amount -= restored - maximum_pp;
                    S16_AT(target, 0x3a) = maximum_pp;
                    SetResult_080a9f10(&result, multi_target, 5);
                } else {
                    SetResult_080a9f10(&result, multi_target, 6);
                }
                Func_08077128(selected_target);
                changed = 1;
                if (definition[8] == 0xff) {
                    multi_target = 1;
                    result = 8;
                }
            }
            break;
        }
        }

        switch (definition[3] - 1) {
        case 0: {
            s16 current_hp = S16_AT(target, 0x38);
            s16 maximum_hp = S16_AT(target, 0x34);
            if (current_hp <= 0 || current_hp == maximum_hp) {
                SetResult_080a9f10(&result, multi_target, 2);
            } else {
                s32 restored = current_hp + amount;
                S16_AT(target, 0x38) = restored;
                if (restored > maximum_hp) {
                    S16_AT(target, 0x38) = maximum_hp;
                    SetResult_080a9f10(&result, multi_target, 0);
                } else {
                    SetResult_080a9f10(&result, multi_target, 1);
                }
                Func_08077128(selected_target);
                changed = 1;
            }
            break;
        }
        case 1: {
            s16 maximum_pp = S16_AT(target, 0x36);
            s32 restored;
            if (S16_AT(target, 0x3a) == maximum_pp) {
                SetResult_080a9f10(&result, multi_target, 7);
                break;
            }
            restored = S16_AT(target, 0x3a) + amount;
            S16_AT(target, 0x3a) = restored;
            if (restored > maximum_pp) {
                S16_AT(target, 0x3a) = maximum_pp;
                SetResult_080a9f10(&result, multi_target, 5);
            } else {
                SetResult_080a9f10(&result, multi_target, 6);
            }
            Func_08077128(selected_target);
            changed = 1;
            break;
        }
        case 4:
            if (S16_AT(target, 0x38) == 0) {
                S16_AT(target, 0x38) = S16_AT(target, 0x34);
                Func_08077128(selected_target);
                changed = 1;
                SetResult_080a9f10(&result, multi_target, 0x0c);
            } else {
                SetResult_080a9f10(&result, multi_target, 0x0d);
            }
            break;
        case 0x37:
            if (S16_AT(target, 0x38) == 0) {
                s16 maximum = S16_AT(target, 0x34);
                S16_AT(target, 0x38) =
                    (maximum + (maximum < 0)) >> 1;
                Func_08077128(selected_target);
                SetResult_080a9f10(&result, multi_target, 0x0c);
            } else {
                SetResult_080a9f10(&result, multi_target, 0x0d);
            }
            break;
        case 0x38:
            if (S16_AT(target, 0x38) == 0) {
                S16_AT(target, 0x38) =
                    Func_080022ec(S16_AT(target, 0x34) * 7, 10);
                Func_08077128(selected_target);
                SetResult_080a9f10(&result, multi_target, 0x0c);
            } else {
                SetResult_080a9f10(&result, multi_target, 0x0d);
            }
            break;
        case 2:
            if (S8_AT(target, 0x131) != 0) {
                S8_AT(target, 0x131) = 0;
                changed = 1;
                SetResult_080a9f10(&result, multi_target, 0x0a);
            } else {
                SetResult_080a9f10(&result, multi_target, 0x0b);
            }
            break;
        }

        if (definition[8] != 0xff)
            break;
        party_index = (u8)(party_index + 1);
    } while (party_index < U8_AT(party, 0x219));

    S16_AT(party, 0x25a) = result;
    if (!changed)
        return -1;

    party_index = 0;
    while (party_index < U8_AT(party, 0x219)) {
        Func_08077010(U16_AT(party, 0x208 + party_index * 2));
        party_index = (u8)(party_index + 1);
    }
    return 0;
}
