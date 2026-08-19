#include "types.h"
#include "battle_command.h"
#include "battle_event.h"
#include "battle_runtime.h"
#include "battle_types.h"
#include "runtime_1e74.h"

/* 行動1件の対象解決。コピーを取り、命中とダメージ種別を決めて効果を出す。 */

/* 戦闘ワークとコピー記述子 */
extern void *Data_03001e74;                       /* 戦闘ワークへのポインタセル */
#define BattleWorkPtr Data_03001e74
extern u8 Data_03001388[];                        /* ユニット退避コピーの記述子 */
#define UnitCopyDesc Data_03001388

/*
 * 距離減衰テーブル。target_offsets の絶対値で引く百分率。
 * 中心から離れるほど効果が落ちる。
 */
extern u8 Data_080c2ab8[];                        /* 命中率 */
#define HitFalloff Data_080c2ab8
extern s32 Data_080c2ac0[];                       /* PP喪失 「PPを うしなった！」 */
#define PpLossFalloff Data_080c2ac0
extern s32 Data_080c2ad8[];                       /* HP回復 */
#define HpHealFalloff Data_080c2ad8
extern s32 Data_080c2af0[];                       /* PPダメージ */
#define PpDmgFalloff Data_080c2af0
extern s32 Data_080c2b08[];                       /* HPダメージ 種別5 */
#define HpDmgFalloff5 Data_080c2b08
extern s32 Data_080c2b20[];                       /* HPダメージ 種別8 */
#define HpDmgFalloff8 Data_080c2b20
extern s32 Data_080c2b38[];                       /* HPダメージ 種別6 */
#define HpDmgFalloff6 Data_080c2b38
extern s32 Data_080c2b50[];                       /* PP回復 */
#define PpHealFalloff Data_080c2b50
extern s32 Data_080c2b68[];                       /* HPダメージ 種別3 */
#define HpDmgFalloff Data_080c2b68

/* システム */
u32 Func_08004938(s32 size);                      /* ワークをバンプ確保 (exact/08004938.c) */
#define Sys_Alloc Func_08004938
void Func_08002df0(void *buffer);                 /* Sys_Alloc の解放 */
#define Sys_Free Func_08002df0
void Func_080072f0(void *dst, void *src, s32 size, void *desc); /* 記述子付きコピー */
#define Mem_Copy Func_080072f0
void Func_08015130(s32 mode);                     /* ワーク+65 のモード適用。役割未確定 */
#define Sys_SetMode Func_08015130

/* 算術。ゼロ除算を避ける共有ルーチン */
s32 Func_080022ec(s32 numerator, s32 denominator);
#define Math_Div Func_080022ec
s32 Func_080022f4(s32 numerator, s32 denominator);
#define Math_Mod Func_080022f4

/* 戦闘計算 */
s32 Func_08077178(s32 actor, s32 target, s32 range, s32 effect, s32 table);
#define Battle_HitCheck Func_08077178             /* 命中判定。table は HitFalloff の行 */
s32 Func_08077180(s32 power, s32 scale, s32 unused, s32 bonus);
#define Battle_CalcAttack Func_08077180           /* 武器攻撃のダメージ */
s32 Func_08077188(s32 power, s32 bonus, s32 scale);
#define Battle_CalcPower Func_08077188            /* 威力ベースのダメージ */
s32 Func_08077190(s32 power, s32 scale, s32 factor);
#define Battle_CalcRestore Func_08077190          /* 回復量 */
void Func_08077120(s32 unit, s32 amount);         /* 吸収した分を行動側へ */
#define BattleUnit_Drain Func_08077120
void Func_08077140(s32 slot, s32 unit, s32 mask); /* 召喚ユニットの配置 */
#define BattleUnit_Assign Func_08077140
s32 Func_080bbae8(s32 effect);                    /* 倒れていても効く効果か */
#define BattleEffect_OnDead Func_080bbae8

/* 召喚（スタンバイ）。effect 50/51 の径路 */
s32 Func_080b7514(void);                          /* 空きスロットを探す */
#define Summon_FindSlot Func_080b7514
s32 Func_080c1fa8(s32 work0);                     /* ワーク先頭語から召喚クラスを引く */
#define Summon_ClassId Func_080c1fa8
s32 Func_080b6cdc(s32 class_id);                  /* クラスが有効か */
#define Summon_ClassValid Func_080b6cdc
s32 Func_080c1df4(s32 class_id, s32 n);           /* チャージ取得。bit15 は要リセット */
#define Summon_TakeCharge Func_080c1df4
void Func_080c1f50(s32 class_id);                 /* チャージのリセット */
#define Summon_ResetCharge Func_080c1f50
void Func_080b7548(void);                         /* 配置後の更新 */
#define Summon_Refresh Func_080b7548

/* スプライト側 */
void *Func_080b7dd0(s32 unit);                    /* 表示オブジェクトを引く */
#define Actor_GetObject Func_080b7dd0
void Func_080b6f44(void *obj, s32 unit, s32 x, s32 y); /* マスへ配置 */
#define Actor_Place Func_080b6f44
void Func_080b6c90(void);                         /* 配置の確定 */
#define Actor_Commit Func_080b6c90
s32 Func_080b6ae0(s16 *entries);                  /* 表示スロット一覧。個数を返す */
#define Actor_ListSlots Func_080b6ae0
void Func_080b8000(s32 slot);                     /* スロットの再描画 */
#define Actor_RefreshSlot Func_080b8000

/* ダメージ種別 = nibble + 1。各ケースの挙動から命名。 */
enum {
    DK_PP_DMG = 0,      /* PPへの直接ダメージ */
    DK_HP_HEAL = 2,     /* HP回復 */
    DK_HP_DMG = 3,      /* 威力ベースのHPダメージ */
    DK_ATTACK = 4,      /* 武器攻撃 */
    DK_ATTACK_X = 5,    /* 武器攻撃 威力乗算/10 */
    DK_HP_DMG_6 = 6,    /* 加算HPダメージ 減衰6系 */
    DK_HP_DMG_7 = 7,    /* 加算HPダメージ 減衰5系 */
    DK_HP_DMG_9 = 9,    /* 加算HPダメージ 減衰8系 */
    DK_PP_DRAIN = 11,   /* PP吸収系(EFX_DRAIN_PPでnibble=10) */
    DK_PP_HEAL = 12     /* PP回復 */
};

/* テキスト番号。assets/text/message_archive.json の英文から命名。 */
/* 番号はリンク時定数。プールされる絶対シンボルとして参照する。 */
extern char Value_0000081d;
extern char Value_0000081e;
extern char Value_00000820;
extern char Value_00000821;
extern char Value_00000822;
extern char Value_00000823;
extern char Value_00000824;
extern char Value_00000825;
extern char Value_00000826;
extern char Value_00000827;
extern char Value_00000829;
extern char Value_0000082a;
extern char Value_00000831;
extern char Value_00000834;
extern char Value_0000084c;
extern char Value_0000084f;
extern char Value_00000850;
extern char Value_00000854;
extern char Value_0000085e;
extern char Value_0000085f;
extern char Value_00000860;
extern char Value_00000861;
extern char Value_00000862;
extern char Value_00000863;
extern char Value_00000864;
extern char Value_00000865;
extern char Value_00000866;
extern char Value_00000867;
extern char Value_00000868;
extern char Value_00000869;
extern char Value_0000086a;
extern char Value_0000086b;
extern char Value_0000086c;
extern char Value_0000086d;
extern char Value_0000086f;
extern char Value_00000870;
extern char Value_00000872;
extern char Value_00000873;
extern char Value_00000874;
extern char Value_00000875;
extern char Value_00000876;
extern char Value_00000877;
extern char Value_00000878;
extern char Value_0000087d;
extern char Value_0000087e;
extern char Value_0000087f;
extern char Value_00000881;
extern char Value_00000882;
extern char Value_00000883;
extern char Value_00000884;
extern char Value_0000088b;
extern char Value_0000088c;
extern char Value_0000088d;
extern char Value_0000088f;
extern char Value_00000894;
extern char Value_00000896;
extern char Value_000008f3;
extern char Value_000008f4;
extern char Value_000008f5;
extern char Value_000008f6;
#define MSG_HP_RECOVER ((s32)&Value_0000081d)                   /* 「HPが N かいふくした！」 */
#define MSG_PP_RECOVER ((s32)&Value_0000081e)                   /* 「PPが N かいふくした！」 */
#define MSG_HP_FULL ((s32)&Value_00000820)                   /* 「HPがぜんかいふくした！」 */
#define MSG_PP_FULL ((s32)&Value_00000821)                   /* 「PPがぜんかいふくした！」 */
#define MSG_CRITICAL ((s32)&Value_00000822)                   /* 「かいしんのいちげき！」 */
#define MSG_BITTER_BLOW ((s32)&Value_00000823)                   /* 「つうこんのいちげき！」 */
#define MSG_FELLED ((s32)&Value_00000824)                   /* 「あいてを たおした！」 */
#define MSG_GOES_DOWN ((s32)&Value_00000825)                   /* 「たおれてしまった…」 */
#define MSG_DMG_E ((s32)&Value_00000826)                   /* 「Nの ダメージ！」敵側 */
#define MSG_DMG_P ((s32)&Value_00000827)                   /* 「Nの ダメージ！」味方側 */
#define MSG_PP_LOSS_E ((s32)&Value_00000829)                   /* 「PPを Nうしなった！」敵側 */
#define MSG_PP_LOSS_P ((s32)&Value_0000082a)                   /* 「PPを Nうしなった！」味方側 */
#define MSG_DMG_EMPH_E ((s32)&Value_00000831)                   /* ±affinity で 。/！/！！！ 敵側 */
#define MSG_DMG_EMPH_P ((s32)&Value_00000834)                   /* ±affinity で 。/！/！！！ 味方側 */
#define MSG_SPIRIT_DRAIN ((s32)&Value_0000084c)                   /* 「せいしんを すいとられた！」 */
#define MSG_KO_DOWN ((s32)&Value_0000084f)                   /* 「たおれた！」 */
#define MSG_SUFFOCATE ((s32)&Value_00000850)                   /* 「いきができない！」 */
#define MSG_NO_EFFECT ((s32)&Value_00000854)                   /* 「しかし こうかがなかった！」 */
#define MSG_LEECH_GAIN ((s32)&Value_0000085e)                   /* 「さらに PPを Nかいふく！」 */
#define MSG_LEECH_TAKE ((s32)&Value_0000085f) /* 「PPを Nすいとられた！」 */
#define MSG_WAKES ((s32)&Value_00000883) /* 「めをさました！」 */                   /* 「PPを Nすいとられた！」 */
#define MSG_ATK_DOWN ((s32)&Value_00000860)                   /* 「こうげきが Nさがった！」 */
#define MSG_ATK_UP ((s32)&Value_00000861)                   /* 「こうげきが Nあがった！」 */
#define MSG_DEF_DOWN ((s32)&Value_00000862)                   /* 「ぼうぎょが Nさがった！」 */
#define MSG_DEF_UP ((s32)&Value_00000863)                   /* 「ぼうぎょが Nあがった！」 */
#define MSG_REVIVED ((s32)&Value_00000864)                   /* 「いきかえった！」 */
#define MSG_RES_DOWN ((s32)&Value_00000865)                   /* 「たいせいが Nさがった！」 */
#define MSG_RES_UP ((s32)&Value_00000866)                   /* 「たいせいが Nあがった！」 */
#define MSG_POISONED ((s32)&Value_00000867)                   /* 「どくに おかされた！」 */
#define MSG_DELUSION ((s32)&Value_00000868)                   /* 「げんわくに つつまれた！」 */
#define MSG_CONFUSED ((s32)&Value_00000869)                   /* 「こんらんした！」 */
#define MSG_CHARMED ((s32)&Value_0000086a)                   /* 「こころを うばわれた！」 */
#define MSG_STUNNED ((s32)&Value_0000086b)                   /* 「スタンした！」 */
#define MSG_ASLEEP ((s32)&Value_0000086c)                   /* 「ねむってしまった！」 */
#define MSG_PSY_BLOCK ((s32)&Value_0000086d)                   /* 「エナジーを ふうじられた！」 */
#define MSG_REFRAIN ((s32)&Value_0000086f)                   /* 「リフレインに はばまれた！」 */
#define MSG_REFLECT ((s32)&Value_00000870)                   /* 「リフレクの こうか！」 */
#define MSG_EVIL_SPIRIT ((s32)&Value_00000872)                   /* 「あくりょうに とりつかれた！」 */
#define MSG_DEATH_CURSE ((s32)&Value_00000873)                   /* 「しのせんこくを うけた！」 */
#define MSG_VENOM ((s32)&Value_00000874)                   /* 「もうどくに おかされた！」 */
#define MSG_DEATH_COUNT ((s32)&Value_00000875)                   /* 「しにがみが てまねく… N」 */
#define MSG_PSY_SEAL ((s32)&Value_00000876)                   /* 「エナジーを シールされた！」 */
#define MSG_AGI_UP ((s32)&Value_00000877)                   /* 「すばやさが Nあがった！」 */
#define MSG_AGI_DOWN ((s32)&Value_00000878)                   /* 「すばやさが Nさがった！」 */
#define MSG_READIES ((s32)&Value_0000087d)                   /* 「みがまえた！」 */
#define MSG_CHALLENGE ((s32)&Value_0000087e)                   /* 「たたかいに もえてきた！」 */
#define MSG_IMMOBILE ((s32)&Value_0000087f)                   /* 「うごけなくなった！」 */
#define MSG_AURA ((s32)&Value_00000881)                   /* 「まもりのオーラに つつまれた！」 */
#define MSG_AURA_2 ((s32)&Value_00000882)                   /* 「まもりのオーラに つつまれた！」強 */
#define MSG_CURE_POISON ((s32)&Value_00000884)                   /* 「どくが きえた！」 */
#define MSG_CURE_DELUSION ((s32)&Value_0000088b)                   /* 「めが みえるようになった！」 */
#define MSG_CURE_SEAL ((s32)&Value_0000088c)                   /* 「シールが とけた！」 */
#define MSG_CURE_STUN ((s32)&Value_0000088d)                   /* 「スタンが なおった！」 */
#define MSG_CURE_SPIRIT ((s32)&Value_0000088f)                   /* 「しにがみが はなれた！」 */
#define MSG_CURE_CURSE ((s32)&Value_00000894)                   /* 「しのせんこくを ふりはらった！」 */
#define MSG_BUFFS_RESET ((s32)&Value_00000896)                   /* 「のうりょくが もとにもどった！」 */
#define MSG_SPLIT_OFF ((s32)&Value_000008f3)                   /* 「ぶんれつした！」 */
#define MSG_SPLIT_FAIL ((s32)&Value_000008f4)                   /* 「ぶんれつに しっぱいした！」 */
#define MSG_APPEARS ((s32)&Value_000008f5)                   /* 「あらわれた！！」 */
#define MSG_NO_ONE_CAME ((s32)&Value_000008f6)                   /* 「しかし だれもこなかった！」 */

/* 効果番号。挙動から付けた慎重な名前で、原作の識別子の主張ではない。 */
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
    EFX_PP_LEECH = 69,
};

/* 属性テーブルはユニット+36 の s16 対。威力テーブルは +72。 */
#define ELEM_AT(unit, range) (*(s16 *)((u8 *)(unit) + 38 + (range) * 4))
#define S8OF(v) (*(s8 *)&(v))

#define CLAMP_MOD(v)                                                           \
    {                                                                          \
        if ((v) < -4)                                                          \
            (v) = -4;                                                          \
        if ((v) > 4)                                                           \
            (v) = 4;                                                           \
    }

#define TAKE_BONUS()                                                           \
    {                                                                          \
        if (range != 4) {                                                      \
            s32 off;                                                           \
                                                                               \
            off = range * 4 + 72;                                              \
            bonus = power - ((s16 *)((u8 *)target + off))[1];                  \
        }                                                                      \
    }

#define APPLY_GUARD()                                                          \
    {                                                                          \
        guard = S8OF(target->guard_level);                                     \
        if (guard != 0) {                                                      \
            if (guard == 1)                                                    \
                dmg /= 2;                                                      \
            else                                                               \
                dmg = Math_Div(dmg, 10);                                  \
        }                                                                      \
    }

#define TEXT_SIDE(player, enemy)                                               \
{                                                                              \
    if ((u32)target_id <= 7)                                                   \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (player));                         \
    else                                                                       \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (enemy));                          \
}

#define TAKE_PWR()                                                             \
{                                                                              \
    pwr = action->power;                                                       \
    if (pwr == 0)                                                              \
        break;                                                                 \
}

#define APPLY_HP_HIT(ko_lo, ko_hi)                                             \
{                                                                              \
    if (target->hp <= dmg) {                                                   \
        dealt = target->hp;                                                    \
        target->hp = 0;                                                        \
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);                  \
        BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                           \
        TEXT_SIDE(ko_lo, ko_hi);                                               \
    } else {                                                                   \
        dealt = dmg;                                                           \
        target->hp = (s16)(target->hp - dmg);                                  \
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);                   \
    }                                                                          \
}

#define APPLY_PP_HIT()                                                         \
{                                                                              \
    if (target->pp <= dmg) {                                                   \
        dealt = target->pp;                                                    \
        target->pp = 0;                                                        \
    } else {                                                                   \
        dealt = dmg;                                                           \
        target->pp = (s16)(target->pp - dmg);                                  \
    }                                                                          \
}

#define HEAL_CUR(cur, maxv, full_text, part_text)                              \
{                                                                              \
    if ((cur) + dmg > (maxv)) {                                                \
        dmg = (maxv) - (cur);                                                  \
        (cur) = (maxv);                                                        \
    } else {                                                                   \
        (cur) = (s16)((cur) + dmg);                                            \
    }                                                                          \
    BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);                               \
    if ((cur) == (maxv))                                                       \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (full_text));                         \
    else {                                                                     \
        BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);                                \
        BattleEvent_Push(BATTLE_EVENT_TEXT, (part_text));                         \
    }                                                                          \
}

/* ターン数フィールドはオフセット定数で渡す。冒頭の代入が call-saved に住み、
 * プッシュ2回をまたいで生き延びる(306 は即値に畳めず const-prop が残す)。 */
#define ATK_TURNS 0x132
#define DEF_TURNS 0x134
#define RES_TURNS 0x136

#define ADJUST_ATKDEF(field, delta, toff_c, value_expr, text)                  \
{                                                                              \
    s32 toff;                                                                  \
                                                                               \
    toff = (toff_c);                                                           \
    (field) += (delta);                                                        \
    CLAMP_MOD(field);                                                          \
    BattleUnit_Recalculate(target_id);                                         \
    BattleEvent_Push(BATTLE_EVENT_VALUE, (value_expr));                           \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                                  \
    *(u8 *)((u8 *)target + toff) = 7;                                          \
}

#define ADJUST_RES(delta, value_expr, text)                                 \
{                                                                              \
    target->res_modifier += (delta);                                             \
    CLAMP_MOD(target->res_modifier);                                             \
    BattleEvent_Push(BATTLE_EVENT_VALUE, (value_expr));                        \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                               \
    target->res_modifier_turns = 7;                                                    \
}

#define SET_STATUS7(field, text)                                               \
{                                                                              \
    BattleEvent_Push(BATTLE_EVENT_TEXT, (text));                                  \
    (field) = 7;                                                               \
}

/* 共用体経由の参照はエイリアス集合0。gcse が先頭の探査と統合できない。 */
union Cell { s16 v; u16 u; };

s32 Func_080bbb0c(struct BattlePlan *plan, s32 slot)
{
    /*
     * 宣言順はスピルスロット順(先頭ほど高位)。参照の割付:
     * offset=fp action=76 actor=72 actor_id=68 action_id=64 bonus=60
     * work=56 half=52 adjust=48 dealt=44 crush=40 hit=36 modifier=32
     * skip=28 nibble=24 affinity=20 copy=16 power=12 (temp)=8 cmd=4
     * target=r7 target_id=sl range=r9 saved=84..
     */
    s32 offset;
    struct BattleAction *action;
    struct BattleUnit *actor;
    s32 actor_id;
    s32 action_id;
    s32 bonus;
    void *work;
    s32 half;
    s32 adjust;
    s32 dealt;
    s32 crush;
    s32 hit;
    s32 modifier;
    s32 skip;
    s32 nibble;
    s32 affinity;
    struct BattleUnit *copy;
    s32 power;
    s32 kind;
    s16 *cmd;
    s32 target_id;
    s32 range;
    struct BattleUnit *target;
    s32 n;
    s16 value;
    s32 dmg;
    s32 pass;
    s32 scale;
    s32 guard;
    s16 saved[8];
    s32 count;
    s32 tmp;
    s32 size;

    bonus = 0;
    work = BattleWorkPtr;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    affinity = 0;
    size = sizeof(struct BattleUnit);
    copy = (struct BattleUnit *)Sys_Alloc(size);

    actor_id = plan->actor_id;
    target_id = plan->target_ids[slot];
    action_id = plan->action_id;
    range = plan->range_index;
    adjust = plan->target_adjustments[slot];
    modifier = plan->target_modifiers[slot];

    action = BattleAction_Get(action_id);
    actor = BattleUnit_Get(actor_id);
    target = BattleUnit_Get(target_id);
    Mem_Copy(copy, target, size, UnitCopyDesc);

    if (action->range != 255) {
        offset = plan->target_offsets[slot];
        if (offset < 0)
            offset = -offset;
    } else {
        offset = 0;
    }

    /* 属性相性。テーブルを二方向に走査して符号を決める。 */
    if (range != 4) {
        s16 *tbl;
        s32 i;

        value = ELEM_AT(target, range);
        tbl = (s16 *)((u8 *)target + 36);
        i = 0;
        if (value >= tbl[1]) {
            s16 *p;

            p = tbl;
            do {
                i++;
                p += 2;
                if (i > 3)
                    break;
            } while (value >= p[1]);
        }
        if (i == 4)
            affinity = -1;

        i = 0;
        {
            s32 off;

            off = 2;
            if (value <= *(s16 *)((u8 *)tbl + off)) {
                s16 *p;

                p = (s16 *)((u8 *)target + 36);
                do {
                    i++;
                    p += 2;
                    if (i > 3)
                        break;
                } while (value <= p[1]);
            }
        }
        if (i == 4)
            affinity = 1;
    }

    /* 攻撃力。元素武器でなければ 100。 */
    if ((u32)plan->range_index <= 3) {
        cmd = &plan->command;
        if (*cmd != 2) {
            s32 off;

            off = plan->range_index * 4 + 72;
            power = *(s16 *)((u8 *)actor + off);
            goto after_power;
        }
    } else
        cmd = &plan->command;
    power = 100;
after_power:

    if (plan->command == 5 && (u32)plan->range_index <= 3 && affinity > 0) {
        s32 chance;

        {
            s32 off;

            off = plan->range_index * 4 + 72;
            chance = power - ((s16 *)((u8 *)target + off))[1];
        }
        chance += 30;
        chance *= 0x28f;
        if (chance > (BattleRandom_Next() & 0xffff))
            BattleEvent_Push(BATTLE_EVENT_SCRIPT_UPDATE, 5);
    }

    nibble = action->target_flags & 15;
    {
        s32 first;

        first = plan->target_results[slot];
        if (first == -1)
            hit = Battle_HitCheck(
                actor_id, target_id, range, action->effect,
                HitFalloff[offset]);
        else
            hit = first;
    }

    if ((u8)(action->effect + 206) <= 1) {
        s32 st;
        s32 rec;

        st = actor->class_id;
        rec = Summon_FindSlot();
        if (action->effect == EFX_STANDBY_WORK)
            st = Summon_ClassId(*(s32 *)work);
        if (hit != 0 && Summon_ClassValid(st) != 0 && rec >= 0) {
            s32 ch;
            s16 *slots;

            ch = Summon_TakeCharge(st, 1);
            if (ch & 0x8000)
                Summon_ResetCharge(st);
            BattleUnit_Assign(rec, st, ch & 0x7fff);
            slots = (s16 *)((u8 *)work + 2);
            {
                s32 off;
                s32 i;
                s32 j;
                s32 jsave;

                off = 100;
                i = 0;
                jsave = 0;
                j = 0;
                if (*(s16 *)((u8 *)slots + off) == 254) {
                    *(s16 *)((u8 *)slots + off) = rec;
                } else {
                    for (;;) {
                        if (*(s16 *)((u8 *)slots + off) == 255) {
                            s32 t;

                            t = jsave + 102;
                            *(s16 *)((u8 *)slots + off) = rec;
                            *(s16 *)((u8 *)slots + t) = 255;
                            break;
                        }
                        i++;
                        off += 2;
                        j += 2;
                        if (i > 5)
                            break;
                        jsave = j;
                        if (((union Cell *)((u8 *)slots + off))->v == 254) {
                            *(s16 *)((u8 *)slots + off) = rec;
                            break;
                        }
                    }
                }
            }
            Summon_Refresh();
            {
                void *obj;
                s32 x;
                s32 y;

                obj = Actor_GetObject(rec);
                x = *(s32 *)((u8 *)obj + 12);
                if (x < 0)
                    x += 0xffff;
                y = *(s32 *)((u8 *)obj + 16);
                x >>= 16;
                if (y < 0)
                    y += 0xffff;
                y >>= 16;
                Actor_Place(obj, rec, x, y);
            }
            Actor_Commit();
            count = Actor_ListSlots(saved);
            if (count > 0) {
                u16 *q;

                q = (u16 *)saved;
                do {
                    count--;
                    Actor_RefreshSlot(*q++);
                } while (count != 0);
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, rec);
            if (action_id != 0x1f7)
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SPLIT_OFF);
            else
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_APPEARS);
        } else if (action_id == 0x1f7) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SPLIT_FAIL);
        } else {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_NO_ONE_CAME);
        }
    }

    if (hit != 0) {
        s32 efx;

        efx = action->effect;
        if (efx == EFX_IMMOBILIZE) {
            s16 *tbl;

            hit = 0;
            if (*(s16 *)((u8 *)work + 748) == target_id) {
                hit = 1;
            } else {
                n = 0;
                while ((u32)++n <= 19) {
                    s32 idx;

                    idx = (n << 4) + 748;
                    if (*(s16 *)((u8 *)work + idx) == target_id) {
                        hit = 1;
                        break;
                    }
                }
            }
        } else if (efx == EFX_HALF_DEF) {
            half = 1;
        } else if (efx == EFX_LETHAL) {
            crush = 1;
        } else if (efx == EFX_INSTANT_DOWN) {
            skip = 1;
        } else if (efx == EFX_ACTOR_FLASH) {
            if (actor->hp != 0)
                BattleEvent_Push(BATTLE_EVENT_ACTOR_EFFECT, actor_id);
        } else if (efx == EFX_DRAIN_PP) {
            if (target->pp != 0)
                nibble = 10;
            else
                hit = 0;
        }
    }

    /* ダメージ種別。HP が残っているか、分類が非ゼロなら種別スイッチへ。 */
    if (skip == 0
        && (target->hp != 0 || BattleEffect_Classify(action->effect) != 0)
        && (u32)(nibble + 1) <= 12) {
        s32 pp;
        s32 cur;

        /*
         * ダメージ種別 nibble+1:
         *  0 PPダメージ(別系)  2 HP回復  3 HPダメージ  4/5 武器攻撃
         *  6/7/9 加算攻撃(アイテム・distance テーブル別)  11 PP吸収系  12 PP回復
         *  1/8/10 は空。
         */
        switch (nibble + 1) {
        case DK_ATTACK:
        case DK_ATTACK_X:
        {
            s32 def;
            s32 apwr;

            def = target->defense;
            scale = def;
            cur = target->hp;
            if (half != 0)
                scale = (u32)def >> 1;
            pass = 1;
            do {
                TAKE_BONUS();
                if (pass == 0)
                    bonus = 0;
                apwr = action->power;
                if (nibble == 4)
                    dmg = Math_Div(
                        Battle_CalcAttack(actor->attack, scale, 0, bonus)
                            * apwr,
                        10);
                else
                    dmg = Battle_CalcAttack(actor->attack, scale, apwr,
                                            bonus);
                dmg *= adjust;
                if (modifier != 0) {
                    if (modifier == 1)
                        dmg = dmg * 5 / 4;
                    else
                        dmg = dmg * 3 / 2;
                }
                dmg += (u8)Math_Mod(((u8 *)target)[15], 5) + 6;
                if (pass == 0) {
                    BattleEvent_Push(BATTLE_EVENT_MARK, 0);
                    {
                        s32 text;

                        if ((u32)target_id <= 7)
                            text = MSG_BITTER_BLOW;
                        else
                            text = MSG_CRITICAL;
                        BattleEvent_Push(BATTLE_EVENT_TEXT_CONTINUE, text);
                    }
                }
                dmg += BattleRandom_Next() & 3;
                APPLY_GUARD();
                if (dmg <= 0)
                    dmg = 1;
                if (crush != 0) {
                    if (dmg < cur - 1)
                        dmg = cur - 1;
                    if (dmg <= 0)
                        dmg = 1;
                }
                if (BattleFlag_Test(366) != 0 && *cmd == 5 && cur <= dmg) {
                    dmg = cur - 1;
                }
                pass++;
            } while (pass <= 1);
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            cur -= dmg;
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            TEXT_SIDE(MSG_DMG_EMPH_P + affinity, MSG_DMG_EMPH_E + affinity);
            if (cur <= 0) {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE(MSG_GOES_DOWN, MSG_FELLED);
            } else
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case DK_PP_DRAIN:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            TAKE_BONUS();
            dmg = action->power;
            dmg = Battle_CalcPower(dmg, bonus, 256);
            dmg = Math_Div(PpLossFalloff[offset] * dmg, 100);
            dmg *= adjust;
            APPLY_GUARD();
            if (action->effect == EFX_DRAIN_PP && dmg > pp)
                dmg = pp;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            pp -= dmg;
            TEXT_SIDE(MSG_PP_LOSS_P, MSG_PP_LOSS_E);
            if (pp <= 0)
                pp = 0;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->pp - pp;
            target->pp = pp;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case DK_HP_HEAL:
        {
            s32 pwr;

            TAKE_PWR();
            cur = target->hp;
            dmg = Battle_CalcRestore(pwr, range == 4 ? 100 : power, 256);
            dmg = Math_Div(HpHealFalloff[offset] * dmg, 100);
            dmg *= adjust;
            dmg += BattleRandom_Next() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            if (cur == target->max_hp)
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_FULL);
            else {
                BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_RECOVER);
            }
            goto hp_tail;
        }

        case DK_PP_DMG:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            TAKE_BONUS();
            dmg = action->power;
            dmg = Battle_CalcPower(dmg, bonus, 256);
            dmg = Math_Div(PpDmgFalloff[offset] * dmg, 100);
            dmg *= adjust;
            APPLY_GUARD();
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            pp -= dmg;
            TEXT_SIDE(MSG_DMG_P, MSG_DMG_E);
            if (pp <= 0)
                pp = 0;
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            goto pp_store;
        }

        case DK_HP_DMG_6:
        case DK_HP_DMG_7:
        case DK_HP_DMG_9:
        {
            if (action->power == 0)
                break;
            pass = 1;
            cur = target->hp;
            do {
                TAKE_BONUS();
                if (pass == 0)
                    bonus = 0;
                dmg = action->power;
                if (*cmd == 6) {
                    s32 item;

                    item = action_id - 380;
                    if ((u32)item <= 21) {
                        switch (item) {
                        case 0:
                        case 6:
                        case 12:
                        case 18:
                            kind = 3;
                            break;
                        case 1:
                        case 7:
                        case 13:
                        case 19:
                            kind = 6;
                            break;
                        case 2:
                        case 8:
                        case 14:
                        case 20:
                            kind = 9;
                            break;
                        case 3:
                        case 9:
                        case 15:
                        case 21:
                            kind = 12;
                            break;
                        }
                        dmg += Math_Div(kind * target->max_hp, 100);
                    }
                }
                dmg = Battle_CalcPower(dmg, bonus, 256);
                dmg *= adjust;
                switch (nibble) {
                case 5:
                    dmg = Math_Div(HpDmgFalloff5[offset] * dmg, 100);
                    break;
                case 6:
                    dmg = Math_Div(HpDmgFalloff6[offset] * dmg, 100);
                    break;
                case 8:
                    dmg = Math_Div(HpDmgFalloff8[offset] * dmg, 100);
                    break;
                }
                dmg += BattleRandom_Next() & 3;
                APPLY_GUARD();
                if (BattleFlag_Test(366) != 0 && *cmd == 6 && cur > dmg) {
                    dmg = cur;
                }
                pass++;
            } while (pass <= 1);
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            cur -= dmg;
            TEXT_SIDE(MSG_DMG_EMPH_P + affinity, MSG_DMG_EMPH_E + affinity);
            if (cur <= 0) {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE(MSG_GOES_DOWN, MSG_FELLED);
            } else
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            goto hp_tail;
        }

        case 12:
        {
            s32 pwr;

            TAKE_PWR();
            pp = target->pp;
            dmg = Battle_CalcRestore(pwr, range == 4 ? 100 : power, 256);
            dmg = Math_Div(PpHealFalloff[offset] * dmg, 100);
            dmg *= adjust;
            pp += dmg;
            if (pp > target->max_pp) {
                pp = target->max_pp;
                dmg = pp - target->pp;
            }
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            if (pp == target->max_pp)
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_FULL);
            else {
                BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
                BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_RECOVER);
            }
pp_store:
            target->pp = (s16)pp;
            BattleUnit_UpdateRatios(target_id);
            break;
        }

        case 1:
        case 8:
        case 10:
            /* 空スロット */
            break;

        case 3:
            if (hit != 0) {
            if (action->power == 0)
                break;
            cur = target->hp;
            TAKE_BONUS();
            dmg = action->power;
            dmg = Battle_CalcPower(dmg, bonus, 256);
            dmg *= adjust;
            dmg = Math_Div(HpDmgFalloff[offset] * dmg, 100);
            APPLY_GUARD();
            BattleEvent_Push(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            cur -= dmg;
            TEXT_SIDE(MSG_DMG_P, MSG_DMG_E);
            if (cur <= 0) {
                BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                TEXT_SIDE(MSG_GOES_DOWN, MSG_FELLED);
            } else
                BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
hp_tail:
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            BattleUnit_UpdateRatios(target_id);
            break;
            }
            BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_NO_EFFECT);
            break;

        }
    }

    /* 付加効果 */
    BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
    if (BattleEffect_Classify(action->effect) == 0 && target->hp == 0
        && BattleEffect_OnDead(action->effect) == 0)
        goto done;
    if (hit == 0)
        goto done;
    if ((u32)(action->effect - 3) > 66)
        goto done;

    switch (action->effect) {
    case EFX_CURE_ALL:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_DELUSION);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_STUN);
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_SEAL);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->death_count != 0) {
            target->death_count = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_CURSE);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->evil_spirit != 0) {
            target->evil_spirit = 0;
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_SPIRIT);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->poison != 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_POISON);
            target->poison = 0;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        break;

    case EFX_CURE_PART:
        if (target->delusion != 0) {
            target->delusion = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_DELUSION);
        }
        if (target->stun != 0) {
            target->stun = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_STUN);
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            BattleEvent_Push(BATTLE_EVENT_RESET, 0);
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_SEAL);
        }
        if (target->death_count == 0)
            break;
        target->death_count = 0;
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_CURSE);
        break;

    case EFX_HEAL_60:
    case EFX_HEAL_30:
    {
        s32 old;
        s32 maxu;
        s32 maxv;
        s32 heal;

        old = *(volatile u16 *)&target->hp;
        heal = target->hp;
        maxu = *(volatile u16 *)&target->max_hp;
        maxv = target->max_hp;
        if (action->effect == EFX_HEAL_60)
            tmp = maxv * 60;
        else
            tmp = maxv * 30;
        heal += Math_Div(tmp, 100);
        if (heal > (s16)maxu)
            heal = (s16)maxu;
        tmp = heal - (s16)old;
        if (tmp == 0 && nibble != 1)
            break;
        if (heal == (s16)maxu)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_RECOVER);
        }
        target->hp = (s16)heal;
        BattleUnit_UpdateRatios(target_id);
        break;
    }

    case EFX_PP_RESTORE_7:
    {
        s32 old;
        s32 maxv;
        s32 heal;

        maxv = target->max_pp;
        heal = target->pp;
        old = heal;
        heal += Math_Div(maxv * 7, 100);
        if (heal > maxv)
            heal = maxv;
        tmp = heal - old;
        if (tmp == 0 && nibble != 11)
            break;
        if (heal == maxv)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, tmp);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_RECOVER);
        }
        target->pp = (s16)heal;
        BattleUnit_UpdateRatios(target_id);
        break;
    }

    case EFX_AGI_SET_UP8:
        S8OF(target->agility_modifier) = 8;
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->agility - copy->agility);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AGI_UP);
        break;

    case EFX_AGI_SET_DOWN4:
        S8OF(target->agility_modifier) = 252;
        target->agility_modifier_turns = 5;
        BattleUnit_Recalculate(target_id);
        BattleEvent_Push(BATTLE_EVENT_VALUE, copy->agility - target->agility);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AGI_DOWN);
        break;

    case EFX_ATK_DOWN1:
        ADJUST_ATKDEF(target->attack_modifier, -1, ATK_TURNS,
                      copy->attack - target->attack, MSG_ATK_DOWN);
        break;

    case EFX_ATK_DOWN2:
        ADJUST_ATKDEF(target->attack_modifier, -2, ATK_TURNS,
                      copy->attack - target->attack, MSG_ATK_DOWN);
        break;

    case EFX_ATK_UP1:
        ADJUST_ATKDEF(target->attack_modifier, 1, ATK_TURNS,
                      target->attack - copy->attack, MSG_ATK_UP);
        break;

    case EFX_ATK_UP2:
        ADJUST_ATKDEF(target->attack_modifier, 2, ATK_TURNS,
                      target->attack - copy->attack, MSG_ATK_UP);
        break;

    case EFX_DEF_DOWN1:
        ADJUST_ATKDEF(target->defense_modifier, -1, DEF_TURNS,
                      copy->defense - target->defense, MSG_DEF_DOWN);
        break;

    case EFX_DEF_DOWN2:
        ADJUST_ATKDEF(target->defense_modifier, -2, DEF_TURNS,
                      copy->defense - target->defense, MSG_DEF_DOWN);
        break;

    case EFX_DEF_UP1:
        ADJUST_ATKDEF(target->defense_modifier, 1, DEF_TURNS,
                      target->defense - copy->defense, MSG_DEF_UP);
        break;

    case EFX_DEF_UP2:
        ADJUST_ATKDEF(target->defense_modifier, 2, DEF_TURNS,
                      target->defense - copy->defense, MSG_DEF_UP);
        break;

    case EFX_REVIVE_FULL:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REVIVED);
        target->hp = target->max_hp;
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REVIVE_HALF:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REVIVED);
        target->hp = (s16)(target->max_hp / 2);
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REVIVE_80:
        if (target->hp != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_REVIVED);
        target->hp = (s16)Math_Div(target->max_hp * 8, 10);
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_CURE_POISON:
        if (target->poison != 0)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CURE_POISON);
        target->poison = 0;
        break;

    case EFX_RES_DOWN1:
        ADJUST_RES(-1, (copy->res_modifier - target->res_modifier) * 20, MSG_RES_DOWN);
        break;

    case EFX_RES_DOWN2:
        ADJUST_RES(-2, (copy->res_modifier - target->res_modifier) * 20, MSG_RES_DOWN);
        break;

    case EFX_RES_UP1:
        ADJUST_RES(1, (target->res_modifier - copy->res_modifier) * 20, MSG_RES_UP);
        break;

    case EFX_RES_UP2:
        ADJUST_RES(2, (target->res_modifier - copy->res_modifier) * 20, MSG_RES_UP);
        break;

    case EFX_POISON:
        if (target->poison != 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_POISONED);
        target->poison = 1;
        break;

    case EFX_VENOM:
        if (target->poison > 1)
            break;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_VENOM);
        target->poison = 2;
        break;

    case EFX_DELUSION:
        SET_STATUS7(target->delusion, MSG_DELUSION);
        break;

    case EFX_CONFUSE:
        SET_STATUS7(target->confusion, MSG_CONFUSED);
        break;

    case EFX_CHARM:
        SET_STATUS7(target->charm, MSG_CHARMED);
        break;

    case EFX_STUN:
        SET_STATUS7(target->stun, MSG_STUNNED);
        break;

    case EFX_SLEEP:
        SET_STATUS7(target->sleep, MSG_ASLEEP);
        break;

    case EFX_PSY_BLOCK:
        TEXT_SIDE(MSG_PSY_BLOCK, MSG_PSY_SEAL);
        target->psy_seal |= 7;
        break;

    case EFX_PSY_SEAL:
        TEXT_SIDE(MSG_PSY_BLOCK, MSG_PSY_SEAL);
        target->psy_seal |= 16;
        break;

    case EFX_INSTANT_DOWN:
        BattleEvent_Push(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
        if (target->status_12a == 2)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_KO_DOWN);
        else if (action_id == 219)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SUFFOCATE);
        else
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_SPIRIT_DRAIN);
        target->hp = 0;
        BattleUnit_UpdateRatios(target_id);
        break;

    case EFX_REFRAIN:
        SET_STATUS7(target->refrain, MSG_REFRAIN);
        break;

    case EFX_REFLECT:
        SET_STATUS7(target->reflect, MSG_REFLECT);
        break;

    case EFX_DRAIN_HP:
    case EFX_DRAIN_HP_HALF:
    {
        s32 heal;

        heal = actor->hp;
        dmg = dealt;
        if (action->effect == EFX_DRAIN_HP_HALF)
            dmg /= 2;
        heal += dmg;
        if (heal > actor->max_hp) {
            heal = actor->max_hp;
            dmg = heal - actor->hp;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_hp)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_HP_RECOVER);
        }
        actor->hp = (s16)heal;
        BattleUnit_UpdateRatios(actor_id);
        break;
    }

    case EFX_DRAIN_PP:
    {
        s32 heal;

        heal = actor->pp;
        dmg = dealt;
        heal += dmg;
        if (heal > actor->max_pp) {
            heal = actor->max_pp;
            dmg = heal - actor->pp;
        }
        BattleEvent_Push(BATTLE_EVENT_RESET, 0);
        BattleEvent_Push(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_pp)
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_FULL);
        else {
            BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_PP_RECOVER);
        }
        actor->pp = (s16)heal;
        BattleUnit_UpdateRatios(actor_id);
        break;
    }

    case EFX_PP_LEECH:
        dmg = Math_Div(dealt, 10);
        if (target->pp < dmg)
            dmg = target->pp;
        if (actor->pp + dmg > actor->max_pp)
            dmg = actor->max_pp - actor->pp;
        if (dmg == 0)
            break;
        BattleEvent_Push(BATTLE_EVENT_VALUE, dmg);
        TEXT_SIDE(MSG_LEECH_TAKE, MSG_LEECH_GAIN);
        BattleUnit_Drain(actor_id, dmg);
        break;

    case EFX_BUFF_CLEAR:
        if (target->attack_modifier > 0) {
            target->attack_modifier = 0;
            target->attack_modifier_turns = 0;
        }
        if (target->defense_modifier > 0) {
            target->defense_modifier = 0;
            target->defense_modifier_turns = 0;
        }
        if (target->res_modifier > 0) {
            target->res_modifier = 0;
            target->res_modifier_turns = 0;
        }
        if (target->agility_modifier > 0)
            target->agility_modifier = 0;
        target->status_12c = 0;
        target->status_12d = 0;
        target->status_12e = 0;
        target->status_12f = 0;
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_BUFFS_RESET);
        break;

    case EFX_EVIL_SPIRIT:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_EVIL_SPIRIT);
        target->evil_spirit = 1;
        break;

    case EFX_DEATH_CURSE:
        if (target->death_count == 0) {
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEATH_CURSE);
            target->death_count = 7;
            break;
        }
        if (target->death_count <= 1)
            break;
        target->death_count -= 1;
        BattleEvent_Push(BATTLE_EVENT_VALUE, target->death_count);
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_DEATH_COUNT);
        break;

    case EFX_READY:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_READIES);
        ((u8 *)target)[0x144] = 2;  /* ready_pose。構造体表記だと共有尾が壊れる */
        break;

    case EFX_CHALLENGE:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_CHALLENGE);
        target->battle_end_state = 1;
        if ((u32)target_id <= 7)
            ((u8 *)work)[67] |= 2;
        break;

    case EFX_IMMOBILIZE:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_IMMOBILE);
        target->cannot_move = 1;
        break;

    case EFX_GUARD1:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AURA);
        if (S8OF(target->guard_level) > 0)
            break;
        S8OF(target->guard_level) = 1;
        break;

    case EFX_GUARD2:
        BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_AURA_2);
        if (S8OF(target->guard_level) > 1)
            break;
        S8OF(target->guard_level) = 2;
        break;

    case EFX_TEXT_NONE:
        BattleEvent_Push(BATTLE_EVENT_TEXT, (u32)-1);
        break;

    default:
        break;
    }

done:
    /* 終了処理 */
    BattleEvent_Push(BATTLE_EVENT_RESET, 0);
    if (target->hp != 0) {
        if (target->sleep != 0)
        if (target->sleep <= 6
            && dealt > 0 && (BattleRandom_Next() & 3) == 0) {
            target->sleep = 0;
            BattleEvent_Push(BATTLE_EVENT_UNIT, target_id);
            BattleEvent_Push(BATTLE_EVENT_TEXT, MSG_WAKES);
        }
    }
    Sys_Free(copy);
    BattleUnit_Recalculate(target_id);
    Sys_SetMode(((u8 *)BattleWorkPtr)[65]);
    if (target->hp != 0)
        BattleEvent_Push(BATTLE_EVENT_ACTOR_FINISH, target_id);
    if (actor->evil_spirit != 0 && (BattleRandom_Next() & 3) == 0 && dealt > 0) {
        s32 share;

        share = dealt >> 2;
        if (share == 0)
            share = 1;
        plan->pending_amount_60 += share;
    }
}
