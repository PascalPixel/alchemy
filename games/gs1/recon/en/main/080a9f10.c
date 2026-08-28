#include "types.h"
#include "battle_efx.h"
#include "battle_types.h"

struct Object_080a9f10 {
    u8 padding_000[0x10];
    u16 stat_10;
    u16 stat_12;
    u8 padding_014[4];
    u16 stat_18;
    u16 stat_1a;
    u16 stat_1c;
    u8 stat_1e;
    u8 padding_01f[0x15];
    s16 max_hp;
    s16 max_pp;
    s16 hp;
    s16 pp;
    u8 padding_03c[0xf5];
    s8 poison;
};

struct Runtime_080a9f10 {
    u8 padding_000[0x208];
    u16 targets[8];
    u8 padding_218;
    u8 target_count;
    u8 padding_21a[0x40];
    s16 result_code;
};

extern struct Runtime_080a9f10 *Data_03001f2c;

struct Object_080a9f10 *Func_08077008(s32);
void Func_08077010(s32);
struct BattleAction *Func_08077080(s32);
void Func_08077128(s32);
s32 Func_08077190(s32, s32, s32);
s32 Func_08004458(void);
void Func_08015120(s32, s32);
s32 Func_080022ec(s32, s32);

s32 Func_080a9f10(
    s32 effect_id,
    s32 source_id,
    s32 target_id,
    s32 fixed_scale)
{
    struct BattleAction *effect;
    struct Runtime_080a9f10 *runtime;
    struct Object_080a9f10 *target;
    struct Object_080a9f10 *source;
    s32 later_target;
    s32 changed;
    u32 index;
    s16 scale;
    s32 random_adjust;
    s32 random_nonone;
    s32 amount;
    s32 result_code;

    effect = Func_08077080(effect_id);
    runtime = Data_03001f2c;
    changed = 0;
    result_code = 0;
    later_target = 0;

    if (target_id != 9)
        target = Func_08077008(target_id);
    else
        target = Func_08077008(0);

    index = 0;
    if (runtime->target_count != 0) {
        do {
            if (effect->range == 0xff) {
                target_id = runtime->targets[index];
                target = Func_08077008(target_id);
            }

            amount = effect->power;
            switch (effect->target_flags & 0xf) {
            case BATTLE_DAMAGE_HP_HEAL:
                if (fixed_scale == 0) {
                    if (effect->damage_class != 4) {
                        s32 stat_offset;

                        source = Func_08077008(source_id);
                        stat_offset = effect->damage_class * 4 + 0x48;
                        scale = *(s16 *)((u8 *)source + stat_offset);
                    } else
                        scale = 100;
                    amount = Func_08077190(amount, scale, 0x100);
                }

                if (target->hp <= 0) {
                    if (later_target == 0)
                        result_code = 2;
                } else {
                    if (target->hp == target->max_hp) {
                        if (later_target == 0)
                            result_code = 4;
                    } else {
                        target->hp += amount;
                        if (target->hp > target->max_hp) {
                            amount -= target->hp - target->max_hp;
                            target->hp = target->max_hp;
                            if (later_target == 0)
                                result_code = 0;
                        } else if (later_target == 0) {
                            result_code = 1;
                        }
                        Func_08077128(target_id);
                        changed = 1;
                        if (effect->range == 0xff) {
                            later_target = 1;
                            result_code = 3;
                        }
                    }
                }
                break;

            case 9:
                random_adjust = ((u32)Func_08004458() * 4) >> 16;
                if (random_adjust == 0) {
                    random_adjust = -1;
                } else {
                    random_nonone = 1 ^ random_adjust;
                    random_adjust =
                        (u32)((-random_nonone) | random_nonone) >> 31;
                    random_adjust = 1 - random_adjust;
                }

                switch (effect_id & 0x3fff) {
                case 0x104:
                    target->stat_10 += amount + random_adjust;
                    result_code = 0x10;
                    changed = 1;
                    break;
                case 0x105:
                    target->stat_12 += amount + random_adjust;
                    result_code = 0x11;
                    changed = 1;
                    break;
                case 0x108:
                    target->stat_1c += amount + random_adjust;
                    result_code = 0x12;
                    changed = 1;
                    break;
                case 0x109:
                    target->stat_1e += amount;
                    result_code = 0x13;
                    changed = 1;
                    break;
                case 0x106:
                    target->stat_18 += amount + random_adjust;
                    Func_08015120(3, 5);
                    result_code = 0x14;
                    changed = 1;
                    break;
                case 0x107:
                    target->stat_1a += amount + random_adjust;
                    Func_08015120(4, 5);
                    result_code = 0x15;
                    changed = 1;
                    break;
                }
                break;

            case BATTLE_DAMAGE_PP_HEAL:
                if (target->pp == target->max_pp) {
                    if (later_target == 0)
                        result_code = 7;
                } else {
                    target->pp += amount;
                    if (target->pp > target->max_pp) {
                        amount -= target->pp - target->max_pp;
                        target->pp = target->max_pp;
                        if (later_target == 0)
                            result_code = 5;
                    } else if (later_target == 0) {
                        result_code = 6;
                    }
                    Func_08077128(target_id);
                    changed = 1;
                    if (effect->range == 0xff) {
                        later_target = 1;
                        result_code = 8;
                    }
                }
                break;

            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 10:
                break;
            }

            switch (effect->effect) {
            case 1:
                if (target->hp <= 0 || target->hp == target->max_hp) {
                    if (later_target == 0)
                        result_code = 2;
                } else {
                    target->hp += amount;
                    if (target->hp > target->max_hp) {
                        target->hp = target->max_hp;
                        if (later_target == 0)
                            result_code = 0;
                    } else if (later_target == 0) {
                        result_code = 1;
                    }
                    Func_08077128(target_id);
                    changed = 1;
                }
                break;

            case 2:
                if (target->pp == target->max_pp) {
                    if (later_target == 0)
                        result_code = 7;
                } else {
                    target->pp += amount;
                    if (target->pp > target->max_pp) {
                        target->pp = target->max_pp;
                        if (later_target == 0)
                            result_code = 5;
                    } else if (later_target == 0) {
                        result_code = 6;
                    }
                    Func_08077128(target_id);
                    changed = 1;
                }
                break;

            case EFX_REVIVE_FULL:
                if (target->hp == 0) {
                    target->hp = target->max_hp;
                    Func_08077128(target_id);
                    changed = 1;
                    if (later_target == 0)
                        result_code = 0xc;
                } else if (later_target == 0) {
                    result_code = 0xd;
                }
                break;

            case EFX_REVIVE_HALF:
                if (target->hp == 0) {
                    target->hp = target->max_hp / 2;
                    Func_08077128(target_id);
                    if (later_target == 0)
                        result_code = 0xc;
                } else if (later_target == 0) {
                    result_code = 0xd;
                }
                break;

            case EFX_REVIVE_80:
                if (target->hp == 0) {
                    target->hp = Func_080022ec(
                        target->max_hp * 7, 10);
                    Func_08077128(target_id);
                    if (later_target == 0)
                        result_code = 0xc;
                } else if (later_target == 0) {
                    result_code = 0xd;
                }
                break;

            case EFX_CURE_POISON:
                if (target->poison != 0) {
                    target->poison = 0;
                    changed = 1;
                    if (later_target == 0)
                        result_code = 0xa;
                } else if (later_target == 0) {
                    result_code = 0xb;
                }
                break;
            }

            if (effect->range != 0xff)
                break;
            index = (u8)(index + 1);
        } while (index < runtime->target_count);
    }

    if (changed == 0) {
        runtime->result_code = result_code;
        return -1;
    }

    index = 0;
    if (index < runtime->target_count) {
        do {
            Func_08077010(runtime->targets[index]);
            index = (u8)(index + 1);
        } while (index < runtime->target_count);
    }
    runtime->result_code = result_code;
    return 0;
}
