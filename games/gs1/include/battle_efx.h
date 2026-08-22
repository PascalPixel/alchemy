#ifndef ALCHEMY_BATTLE_EFX_H
#define ALCHEMY_BATTLE_EFX_H

/* target_flags 下位4ビットのダメージ種別。 */
enum {
    DK_PP_DMG = -1,     /* PPへの直接ダメージ */
    DK_HP_HEAL = 1,     /* HP回復 */
    DK_HP_DMG = 2,      /* 威力ベースのHPダメージ */
    DK_ATTACK = 3,      /* 武器攻撃 */
    DK_ATTACK_X = 4,    /* 武器攻撃 威力乗算/10 */
    DK_HP_DMG_6 = 5,    /* 加算HPダメージ 減衰6系 */
    DK_HP_DMG_7 = 6,    /* 加算HPダメージ 減衰5系 */
    DK_HP_DMG_9 = 8,    /* 加算HPダメージ 減衰8系 */
    DK_PP_DRAIN = 10,   /* PP吸収系 */
    DK_PP_HEAL = 11     /* PP回復 */
};

/* 挙動から付けた識別名で、原作の識別子の主張ではない。 */
enum {
    EFX_CURE_POISON = 3,
    EFX_CURE_PART = 4,
    EFX_REVIVE_FULL = 5,
    EFX_ATK_UP2 = 6,
    EFX_ATK_UP1 = 7,
    EFX_ATK_DOWN2 = 8,
    EFX_ATK_DOWN1 = 9,
    EFX_DEF_UP2 = 10,
    EFX_DEF_UP1 = 11,
    EFX_DEF_DOWN2 = 12,
    EFX_DEF_DOWN1 = 13,
    EFX_RES_UP2 = 14,
    EFX_RES_UP1 = 15,
    EFX_RES_DOWN2 = 16,
    EFX_RES_DOWN1 = 17,
    EFX_POISON = 18,
    EFX_VENOM = 19,
    EFX_DELUSION = 20,
    EFX_CONFUSE = 21,
    EFX_CHARM = 22,
    EFX_STUN = 23,
    EFX_SLEEP = 24,
    EFX_PSY_BLOCK = 25,
    EFX_EVIL_SPIRIT = 26,
    EFX_INSTANT_DOWN = 27,
    EFX_DEATH_CURSE = 28,
    EFX_REFRAIN = 29,
    EFX_REFLECT = 30,
    EFX_DRAIN_HP = 31,
    EFX_DRAIN_PP = 32,
    EFX_BUFF_CLEAR = 33,
    EFX_LETHAL = 34,
    EFX_HALF_DEF = 35,
    EFX_TEXT_NONE = 45,
    EFX_GUARD1 = 46,
    EFX_GUARD2 = 47,
    EFX_STANDBY = 50,
    EFX_STANDBY_WORK = 51,
    EFX_IMMOBILIZE = 53,
    EFX_CHALLENGE = 54,
    EFX_ACTOR_FLASH = 55,
    EFX_REVIVE_HALF = 56,
    EFX_REVIVE_80 = 57,
    EFX_AGI_SET_DOWN4 = 58,
    EFX_AGI_SET_UP8 = 59,
    EFX_DRAIN_HP_HALF = 60,
    EFX_HEAL_60 = 61,
    EFX_HEAL_30 = 62,
    EFX_PP_RESTORE_7 = 63,
    EFX_CURE_ALL = 64,
    EFX_READY = 66,
    EFX_PSY_SEAL = 67,
    EFX_PP_LEECH = 69
};

#endif
