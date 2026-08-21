/* このファイルは自己完結。型、構造体、呼び先の宣言をすべてここに持つ。 */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;


enum {
    BATTLE_TARGET_CAPACITY = 14,
    BATTLE_PLAN_SIZE = 0x64,
    BATTLE_ACTION_SIZE = 0x10,
    BATTLE_UNIT_SIZE = 0x14c
};

/*
 * Work item assembled by the battle-command resolver and consumed once for
 * each selected target.  The five fourteen-entry arrays deliberately remain
 * separate: callers populate and consume them with different signedness.
 */
struct BattlePlan {
    u8 actor_id;                                  /* 0x00 */
    s8 target_count;                              /* 0x01 */
    u8 target_ids[BATTLE_TARGET_CAPACITY];        /* 0x02 */
    s8 target_offsets[BATTLE_TARGET_CAPACITY];    /* 0x10 */
    s8 target_adjustments[BATTLE_TARGET_CAPACITY]; /* 0x1e */
    s8 target_modifiers[BATTLE_TARGET_CAPACITY];  /* 0x2c */
    s8 target_results[BATTLE_TARGET_CAPACITY];    /* 0x3a */
    s16 command;                                  /* 0x48 */
    u8 unknown_4a[2];
    s32 action_id;                  /* 0x4c */
    s32 range_index;                /* 0x50 */
    s32 outcome;                    /* 0x54 */
    u32 presentation_flags;         /* 0x58 */
    s32 failure;                    /* 0x5c */
    u32 pending_amount_60;          /* 0x60 */
};

/* Static action definition returned by Func_08077080. */
struct BattleAction {
    u8 target_mode;                 /* 0x00 */
    u8 target_flags;                /* 0x01 */
    u8 damage_class;                /* 0x02 */
    u8 effect;                      /* 0x03 */
    u8 unknown_04[4];
    u8 range;                       /* 0x08 */
    u8 pp_cost;                     /* 0x09 */
    u16 power;                      /* 0x0a */
    u8 type_0c;                     /* 0x0c */
    u8 unknown_0d[3];
};

/*
 * Live combatant record returned by Func_08077008.  Only fields established
 * by multiple battle owners are named.  The unresolved condition bytes keep
 * offset-based names until their meanings are independently witnessed.
 */
struct BattleUnit {
    u8 unknown_000[0x34];
    s16 max_hp;                     /* 0x034 */
    s16 max_pp;                     /* 0x036 */
    s16 hp;                         /* 0x038 */
    s16 pp;                         /* 0x03a */
    u16 attack;                     /* 0x03c */
    u16 defense;                    /* 0x03e */
    u16 agility;                    /* 0x040 */
    u8 luck;                        /* 0x042 */
    u8 action_entry_count;          /* 0x043 */
    u8 unknown_044[0x94];
    u16 inventory[15];              /* 0x0d8 */
    u8 unknown_0f6[0x32];
    u8 class_id;                    /* 0x128 */
    u8 class_bonus_disabled;        /* 0x129 */
    u8 status_12a;                  /* 0x12a */
    /* Nonzero levels reduce incoming damage to 1/2 or 1/10. */
    u8 guard_level;                 /* 0x12b */
    u8 status_12c;                  /* 0x12c */
    u8 status_12d;                  /* 0x12d */
    u8 status_12e;                  /* 0x12e */
    u8 status_12f;                  /* 0x12f */
    u8 restraint;                   /* 0x130 */
    s8 poison;                      /* 0x131 */
    u8 attack_modifier_turns;       /* 0x132 */
    s8 attack_modifier;             /* 0x133 */
    u8 defense_modifier_turns;      /* 0x134 */
    s8 defense_modifier;            /* 0x135 */
    u8 res_modifier_turns;          /* 0x136 */
    s8 res_modifier;                /* 0x137 */
    u8 delusion;                    /* 0x138 */
    s8 confusion;                   /* 0x139 */
    u8 charm;                       /* 0x13a */
    u8 stun;                        /* 0x13b */
    u8 sleep;                       /* 0x13c */
    u8 psy_seal;                    /* 0x13d */
    u8 refrain;                     /* 0x13e */
    s8 reflect;                     /* 0x13f */
    u8 evil_spirit;                 /* 0x140 */
    u8 death_count;                 /* 0x141 */
    u8 unknown_142[2];
    u8 ready_pose;                  /* 0x144 「みがまえた！」で2 */
    u8 cannot_move;                 /* 0x145 */
    u8 agility_modifier_turns;      /* 0x146 */
    s8 agility_modifier;            /* 0x147 */
    s8 battle_end_state;            /* 0x148 */
    u8 unknown_149[3];
};

typedef char BattlePlan_Size[sizeof(struct BattlePlan) == (BATTLE_PLAN_SIZE) ? 1 : -1];
typedef char BattlePlan_TargetIds[(u32)&(((struct BattlePlan *)0)->target_ids) == (0x02) ? 1 : -1];
typedef char BattlePlan_TargetAdjustments[(u32)&(((struct BattlePlan *)0)->target_adjustments) == (0x1e) ? 1 : -1];
typedef char BattlePlan_ActionId[(u32)&(((struct BattlePlan *)0)->action_id) == (0x4c) ? 1 : -1];
typedef char BattlePlan_PendingAmount60[(u32)&(((struct BattlePlan *)0)->pending_amount_60) == (0x60) ? 1 : -1];
typedef char BattleAction_Size[sizeof(struct BattleAction) == (BATTLE_ACTION_SIZE) ? 1 : -1];
typedef char BattleAction_Effect[(u32)&(((struct BattleAction *)0)->effect) == (0x03) ? 1 : -1];
typedef char BattleAction_Power[(u32)&(((struct BattleAction *)0)->power) == (0x0a) ? 1 : -1];
typedef char BattleUnit_Size[sizeof(struct BattleUnit) == (BATTLE_UNIT_SIZE) ? 1 : -1];
typedef char BattleUnit_Hp[(u32)&(((struct BattleUnit *)0)->hp) == (0x038) ? 1 : -1];
typedef char BattleUnit_GuardLevel[(u32)&(((struct BattleUnit *)0)->guard_level) == (0x12b) ? 1 : -1];
typedef char BattleUnit_Status140[(u32)&(((struct BattleUnit *)0)->evil_spirit) == (0x140) ? 1 : -1];

/* Opcodes consumed by the event dispatcher at 0x080bd898. */
enum BattleEventOpcode {
    BATTLE_EVENT_UNIT = 0,
    BATTLE_EVENT_VALUE = 1,
    BATTLE_EVENT_ITEM = 2,
    BATTLE_EVENT_ACTION = 3,
    BATTLE_EVENT_TEXT = 4,
    BATTLE_EVENT_TEXT_CONTINUE = 5,
    BATTLE_EVENT_MARK = 6,
    BATTLE_EVENT_RESET = 7,
    BATTLE_EVENT_ACTOR_BEGIN = 8,
    BATTLE_EVENT_ACTOR_RESOLVE = 9,
    BATTLE_EVENT_REFRESH = 10,
    BATTLE_EVENT_ACTOR_FINISH = 11,
    BATTLE_EVENT_ACTOR_EFFECT = 12,
    BATTLE_EVENT_SCRIPT_UPDATE = 13,
    BATTLE_EVENT_SOUND = 14
};

u32 Func_080bbabc(u32 opcode, u32 operand);

/* Battle-facing views of the polymorphic runtime lookup functions. */
struct BattleUnit *Func_08077008(s32 unit_id);
void Func_08077010(s32 unit_id);
struct BattleAction *Func_08077080(s32 action_id);
s32 Func_080770c0(s32 flag_id);
void Func_08077128(s32 unit_id);
s32 Func_080771a0(void);
s32 Func_080772b8(s32 effect_id);


/* 行動1件の対象解決。コピーを取り、命中とダメージ種別を決めて効果を出す。 */

/* 戦闘ワークとコピー記述子 */
/* 名前の対応表。マクロは使わず、評価上の意味だけをここに残す。
 * Func_080bbabc      BattleEvent_Push
 * Func_08077008      BattleUnit_Ge
 * Func_08077010      BattleUnit_Recalculate
 * Func_08077128      BattleUnit_UpdateRatios
 * Func_08077080      BattleAction_Ge
 * Func_080770c0      BattleFlag_Tes
 * Func_080771a0      BattleRandom_Nex
 * Func_080772b8      BattleEffect_Classify
 * Data_03001e74      BattleWorkPtr
 * Data_03001388      UnitCopyDesc
 * Data_080c2ab8      HitFalloff
 * Data_080c2ac0      PpLossFalloff
 * Data_080c2ad8      HpHealFalloff
 * Data_080c2af0      PpDmgFalloff
 * Data_080c2b08      HpDmgFalloff5
 * Data_080c2b20      HpDmgFalloff8
 * Data_080c2b38      HpDmgFalloff6
 * Data_080c2b50      PpHealFalloff
 * Data_080c2b68      HpDmgFalloff
 * Func_08004938      Sys_Alloc
 * Func_08002df0      Sys_Free
 * Func_080072f0      Mem_Copy
 * Func_08015130      Sys_SetMode
 * Func_080022ec      Math_Div
 * Func_080022f4      Math_Mod
 * Func_08077178      Battle_HitCheck        命中判定。table は HitFalloff の行
 * Func_08077180      Battle_CalcAttack      武器攻撃のダメージ
 * Func_08077188      Battle_CalcPower       威力ベースのダメージ
 * Func_08077190      Battle_CalcRestore     回復量
 * Func_08077120      BattleUnit_Drain
 * Func_08077140      BattleUnit_Assign
 * Func_080bbae8      BattleEffect_OnDead
 * Func_080b7514      Summon_FindSlo
 * Func_080c1fa8      Summon_ClassId
 * Func_080b6cdc      Summon_ClassValid
 * Func_080c1df4      Summon_TakeCharge
 * Func_080c1f50      Summon_ResetCharge
 * Func_080b7548      Summon_Refresh
 * Func_080b7dd0      Actor_GetObjec
 * Func_080b6f44      Actor_Place
 * Func_080b6c90      Actor_Commi
 * Func_080b6ae0      Actor_ListSlots
 * Func_080b8000      Actor_RefreshSlo
 * Value_0000081d     MSG_HP_RECOVER         「HPが N かいふくした！」
 * Value_0000081e     MSG_PP_RECOVER         「PPが N かいふくした！」
 * Value_00000820     MSG_HP_FULL            「HPがぜんかいふくした！」
 * Value_00000821     MSG_PP_FULL            「PPがぜんかいふくした！」
 * Value_00000822     MSG_CRITICAL           「かいしんのいちげき！」
 * Value_00000823     MSG_BITTER_BLOW        「つうこんのいちげき！」
 * Value_00000824     MSG_FELLED             「あいてを たおした！」
 * Value_00000825     MSG_GOES_DOWN          「たおれてしまった…」
 * Value_00000826     MSG_DMG_E              「Nの ダメージ！」敵側
 * Value_00000827     MSG_DMG_P              「Nの ダメージ！」味方側
 * Value_00000829     MSG_PP_LOSS_E          「PPを Nうしなった！」敵側
 * Value_0000082a     MSG_PP_LOSS_P          「PPを Nうしなった！」味方側
 * Value_00000831     MSG_DMG_EMPH_E         ±affinity で 。/！/！！！ 敵側
 * Value_00000834     MSG_DMG_EMPH_P         ±affinity で 。/！/！！！ 味方側
 * Value_0000084c     MSG_SPIRIT_DRAIN       「せいしんを すいとられた！」
 * Value_0000084f     MSG_KO_DOWN            「たおれた！」
 * Value_00000850     MSG_SUFFOCATE          「いきができない！」
 * Value_00000854     MSG_NO_EFFECT          「しかし こうかがなかった！」
 * Value_0000085e     MSG_LEECH_GAIN         「さらに PPを Nかいふく！」
 * Value_0000085f     MSG_LEECH_TAKE         「PPを Nすいとられた！」
 * Value_00000883     MSG_WAKES              「めをさました！」 */                   /* 「PPを Nすいとられた！」
 * Value_00000860     MSG_ATK_DOWN           「こうげきが Nさがった！」
 * Value_00000861     MSG_ATK_UP             「こうげきが Nあがった！」
 * Value_00000862     MSG_DEF_DOWN           「ぼうぎょが Nさがった！」
 * Value_00000863     MSG_DEF_UP             「ぼうぎょが Nあがった！」
 * Value_00000864     MSG_REVIVED            「いきかえった！」
 * Value_00000865     MSG_RES_DOWN           「たいせいが Nさがった！」
 * Value_00000866     MSG_RES_UP             「たいせいが Nあがった！」
 * Value_00000867     MSG_POISONED           「どくに おかされた！」
 * Value_00000868     MSG_DELUSION           「げんわくに つつまれた！」
 * Value_00000869     MSG_CONFUSED           「こんらんした！」
 * Value_0000086a     MSG_CHARMED            「こころを うばわれた！」
 * Value_0000086b     MSG_STUNNED            「スタンした！」
 * Value_0000086c     MSG_ASLEEP             「ねむってしまった！」
 * Value_0000086d     MSG_PSY_BLOCK          「エナジーを ふうじられた！」
 * Value_0000086f     MSG_REFRAIN            「リフレインに はばまれた！」
 * Value_00000870     MSG_REFLECT            「リフレクの こうか！」
 * Value_00000872     MSG_EVIL_SPIRIT        「あくりょうに とりつかれた！」
 * Value_00000873     MSG_DEATH_CURSE        「しのせんこくを うけた！」
 * Value_00000874     MSG_VENOM              「もうどくに おかされた！」
 * Value_00000875     MSG_DEATH_COUNT        「しにがみが てまねく… N」
 * Value_00000876     MSG_PSY_SEAL           「エナジーを シールされた！」
 * Value_00000877     MSG_AGI_UP             「すばやさが Nあがった！」
 * Value_00000878     MSG_AGI_DOWN           「すばやさが Nさがった！」
 * Value_0000087d     MSG_READIES            「みがまえた！」
 * Value_0000087e     MSG_CHALLENGE          「たたかいに もえてきた！」
 * Value_0000087f     MSG_IMMOBILE           「うごけなくなった！」
 * Value_00000881     MSG_AURA               「まもりのオーラに つつまれた！」
 * Value_00000882     MSG_AURA_2             「まもりのオーラに つつまれた！」強
 * Value_00000884     MSG_CURE_POISON        「どくが きえた！」
 * Value_0000088b     MSG_CURE_DELUSION      「めが みえるようになった！」
 * Value_0000088c     MSG_CURE_SEAL          「シールが とけた！」
 * Value_0000088d     MSG_CURE_STUN          「スタンが なおった！」
 * Value_0000088f     MSG_CURE_SPIRIT        「しにがみが はなれた！」
 * Value_00000894     MSG_CURE_CURSE         「しのせんこくを ふりはらった！」
 * Value_00000896     MSG_BUFFS_RESET        「のうりょくが もとにもどった！」
 * Value_000008f3     MSG_SPLIT_OFF          「ぶんれつした！」
 * Value_000008f4     MSG_SPLIT_FAIL         「ぶんれつに しっぱいした！」
 * Value_000008f5     MSG_APPEARS            「あらわれた！！」
 * Value_000008f6     MSG_NO_ONE_CAME        「しかし だれもこなかった！」
 */

extern void *Data_03001e74;                       /* 戦闘ワークへのポインタセル */
extern u8 Data_03001388[];                        /* ユニット退避コピーの記述子 */

/*
 * 距離減衰テーブル。target_offsets の絶対値で引く百分率。
 * 中心から離れるほど効果が落ちる。
 */
extern u8 Data_080c2ab8[];                        /* 命中率 */
extern s32 Data_080c2ac0[];                       /* PP喪失 「PPを うしなった！」 */
extern s32 Data_080c2ad8[];                       /* HP回復 */
extern s32 Data_080c2af0[];                       /* PPダメージ */
extern s32 Data_080c2b08[];                       /* HPダメージ 種別5 */
extern s32 Data_080c2b20[];                       /* HPダメージ 種別8 */
extern s32 Data_080c2b38[];                       /* HPダメージ 種別6 */
extern s32 Data_080c2b50[];                       /* PP回復 */
extern s32 Data_080c2b68[];                       /* HPダメージ 種別3 */

/* システム */
u32 Func_08004938(s32 size);                      /* ワークをバンプ確保 (exact/08004938.c) */
void Func_08002df0(void *buffer);                 /* Sys_Alloc の解放 */
void Func_080072f0(void *dst, void *src, s32 size, void *desc); /* 記述子付きコピー */
void Func_08015130(s32 mode);                     /* ワーク+65 のモード適用。役割未確定 */

/* 算術。ゼロ除算を避ける共有ルーチン */
s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080022f4(s32 numerator, s32 denominator);

/* 戦闘計算 */
s32 Func_08077178(s32 actor, s32 target, s32 range, s32 effect, s32 table);
s32 Func_08077180(s32 power, s32 scale, s32 unused, s32 bonus);
s32 Func_08077188(s32 power, s32 bonus, s32 scale);
s32 Func_08077190(s32 power, s32 scale, s32 factor);
s16 Func_08077120(s32 unit, s32 amount);          /* 吸収した分を行動側へ */
s32 Func_08077140(s32 slot, s32 unit, s32 mask);  /* 召喚ユニットの配置 */
s32 Func_080bbae8(s32 effect);                    /* 倒れていても効く効果か */

/* 召喚（スタンバイ）。effect 50/51 の径路 */
s32 Func_080b7514(void);                          /* 空きスロットを探す */
s32 Func_080c1fa8(s32 work0);                     /* ワーク先頭語から召喚クラスを引く */
s32 Func_080b6cdc(s32 class_id);                  /* クラスが有効か */
s32 Func_080c1df4(s32 class_id, s32 n);           /* チャージ取得。bit15 は要リセット */
s32 Func_080c1f50(s32 class_id);                  /* チャージのリセット */
void Func_080b7548(void);                         /* 配置後の更新 */

/* スプライト側 */
void *Func_080b7dd0(s32 unit);                    /* 表示オブジェクトを引く */
void Func_080b6f44(void *obj, s32 unit, s32 x, s32 y); /* マスへ配置 */
void Func_080b6c90(void);                         /* 配置の確定 */
s32 Func_080b6ae0(s16 *entries);                  /* 表示スロット一覧。個数を返す */
void Func_080b8000(s32 slot);                     /* スロットの再描画 */

/* target_flags 下位4ビットのダメージ種別。各ケースの挙動から命名。 */
enum {
    DK_PP_DMG = -1,     /* PPへの直接ダメージ */
    DK_HP_HEAL = 1,     /* HP回復 */
    DK_HP_DMG = 2,      /* 威力ベースのHPダメージ */
    DK_ATTACK = 3,      /* 武器攻撃 */
    DK_ATTACK_X = 4,    /* 武器攻撃 威力乗算/10 */
    DK_HP_DMG_6 = 5,    /* 加算HPダメージ 減衰6系 */
    DK_HP_DMG_7 = 6,    /* 加算HPダメージ 減衰5系 */
    DK_HP_DMG_9 = 8,    /* 加算HPダメージ 減衰8系 */
    DK_PP_DRAIN = 10,   /* PP吸収系(EFX_DRAIN_PPでnibble=10) */
    DK_PP_HEAL = 11     /* PP回復 */
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
struct AffinityPair {
    s16 low;
    s16 high;
};





/* 文面を変数に決めてから1回だけ積む形。呼び先を挟んで統合できない共有尾は
 * ソース側の変数だった(crossjump は bl を跨いで一致を探せない)。 */






/* ターン数フィールドはオフセット定数で渡す。冒頭の代入が call-saved に住み、
 * プッシュ2回をまたいで生き延びる(306 は即値に畳めず const-prop が残す)。 */




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
    s32 g1;
    s32 power;
    s16 hp0;
    s32 kind;
    s16 *cmd;
    s32 target_id;
    s8 *rm;
    s32 range;
    struct BattleUnit *target;
    s32 n;
    s32 value;
    s32 dmg;
    s32 pass;
    s32 scale;
    s8 *am;
    s32 guard;
    s16 saved[8];
    s32 count;
    s32 tmp;
    s32 size;

    bonus = 0;
    work = Data_03001e74;
    half = 0;
    dealt = 0;
    crush = 0;
    skip = 0;
    affinity = 0;
    size = sizeof(struct BattleUnit);
    copy = (struct BattleUnit *)Func_08004938(size);

    actor_id = plan->actor_id;
    target_id = plan->target_ids[slot];
    action_id = plan->action_id;
    range = plan->range_index;
    adjust = plan->target_adjustments[slot];
    modifier = plan->target_modifiers[slot];

    action = Func_08077080(action_id);
    actor = Func_08077008(actor_id);
    target = Func_08077008(target_id);
    Func_080072f0(copy, target, size, Data_03001388);

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

        value = (*(s16 *)((u8 *)(target) + 38 + (range) * 2 * 2));
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
            if (value <= *(s16 *)(((u8 *)(tbl)) + off)) {
                do {
                    i++;
                    if (i > 3)
                        break;
                } while (value <= ((struct AffinityPair *)((u8 *)target + 36))[i].high);
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
        if (chance > (Func_080771a0() & 0xffff))
            Func_080bbabc(BATTLE_EVENT_SCRIPT_UPDATE, 5);
    }

    nibble = action->target_flags & 15;
    {
        s32 first;

        first = plan->target_results[slot];
        if (first == -1)
            hit = Func_08077178(
                actor_id, target_id, range, action->effect,
                Data_080c2ab8[offset]);
        else
            hit = first;
    }
    if ((u8)(action->effect + 206) <= 1) {
        s32 st;
        s32 rec;

        st = actor->class_id;
        rec = Func_080b7514();
        if (action->effect == EFX_STANDBY_WORK)
            st = Func_080c1fa8(*(s32 *)work);
        if (hit != 0 && Func_080b6cdc(st) != 0 && rec >= 0) {
            s32 ch;
            s16 *slots;

            ch = Func_080c1df4(st, 1);
            if (ch & 0x8000)
                Func_080c1f50(st);
            Func_08077140(rec, st, ch & 0x7fff);
            slots = (s16 *)(((u8 *)(work)) + 2);
            {
                s32 off;
                s32 i;
                s32 j;
                s32 jsave;

                off = 100;
                i = 0;
                jsave = 0;
                if (*(s16 *)(((u8 *)(slots)) + off) == 254) {
                    *(s16 *)(((u8 *)(slots)) + off) = rec;
                } else {
                    s32 woff;
                    u8 *base;

                    j = 0;
                    woff = 100;
                    for (;;) {
                        base = (u8 *)slots;
                        if (*(s16 *)(woff + (s32)base) == 255) {
                            s32 t;

                            *(s16 *)(woff + base) = rec;
                            t = jsave + 102;
                            *(s16 *)(base + t) = 255;
                            break;
                        }
                        i++;
                        woff += 2;
                        j += 2;
                        if (i > 5)
                            break;
                        jsave = j;
                        if (*(s16 *)(woff + (s32)base) == 254) {
                            *(s16 *)(woff + (s32)base) = rec;
                            break;
                        }
                    }
                }
            }
            Func_080b7548();
            {
                void *obj;
                s32 x;
                s32 y;

                obj = Func_080b7dd0(rec);
                x = *(s32 *)(((u8 *)(obj)) + 12);
                if (x < 0)
                    x += 0xffff;
                y = *(s32 *)(((u8 *)(obj)) + 16);
                x >>= 16;
                if (y < 0)
                    y += 0xffff;
                y >>= 16;
                Func_080b6f44(obj, rec, x, y);
            }
            Func_080b6c90();
            {
                s32 listed;

                listed = Func_080b6ae0(saved);
                if (listed > 0) {
                    u16 *q;

                    q = (u16 *)saved;
                    count = listed;
                    do {
                        Func_080b8000(*q++);
                        count--;
                    } while (count != 0);
                }
            }
            Func_080bbabc(BATTLE_EVENT_UNIT, rec);
            if (action_id != 0x1f7)
                Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_000008f5));
            else
                Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_000008f3));
        } else if (action_id == 0x1f7) {
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_000008f4));
        } else {
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_000008f6));
        }
    }

    if (hit != 0) {
        s32 efx;

        efx = action->effect;
        if (action->effect == EFX_IMMOBILIZE) {
            s32 hidx;

            hit = 0;
            hidx = 748;
            if (*(s16 *)(((u8 *)(work)) + hidx) == target_id) {
                hit = 1;
            } else {
                n = 0;
            scan_next:
                n++;
                if ((u32)n <= 19) {
                    s32 idx;

                    idx = ((n << 1) << 3) + 748;
                    if (*(s16 *)(((u8 *)(work)) + idx) == target_id)
                        hit = 1;
                    else
                        goto scan_next;
                }
            }
        } else if (action->effect == EFX_HALF_DEF) {
            half = 1;
        } else if (action->effect == EFX_LETHAL) {
            crush = 1;
        } else if (action->effect == EFX_INSTANT_DOWN) {
            skip = 1;
        } else if (action->effect == EFX_ACTOR_FLASH) {
            if (actor->hp != 0)
                Func_080bbabc(BATTLE_EVENT_ACTOR_EFFECT, actor_id);
        } else if (action->effect == EFX_DRAIN_PP) {
            if (target->pp != 0)
                nibble = 10;
            else
                hit = 0;
        }
    }

    /* ダメージ種別。HP が残っているか、分類が非ゼロなら種別スイッチへ。 */
    if (skip == 0
        && (target->hp != 0 || Func_080772b8(action->effect) != 0)) {
        s32 pp;
        s32 cur;

        /*
         * -1 PPダメージ(別系)  1 HP回復  2 HPダメージ  3/4 武器攻撃
         *  5/6/8 加算攻撃(アイテム・distance テーブル別)  10 PP吸収系  11 PP回復
         *  0/7/9 は空。
         */
        switch (nibble) {
        case DK_ATTACK:
        case DK_ATTACK_X:
        {
            s32 def;
            s32 apwr;

            hp0 = target->hp;
            def = target->defense;
            scale = def;
            cur = hp0;
            if (half != 0)
                scale = (u32)def >> 1;
            pass = 1;
            do {
                {
                    if (range != 4) {
                        s32 off;

                        off = range * 4 + 72;
                        bonus = power - ((s16 *)((u8 *)target + off))[1];
                    }
                }
                if (pass == 0)
                    bonus = 0;
                apwr = action->power;
                if (nibble == 4)
                    dmg = Func_080022ec(
                        Func_08077180(actor->attack, scale, 0, bonus)
                            * apwr,
                        10);
                else
                    dmg = Func_08077180(actor->attack, scale, apwr,
                                            bonus);
                dmg *= adjust;
                if (modifier != 0) {
                    if (modifier == 1)
                        dmg = dmg * 5 / 4;
                    else
                        dmg = dmg * 3 / 2;
                    dmg += (u8)Func_080022f4(((u8 *)target)[15], 5) + 6;
                    if (pass == 0) {
                        Func_080bbabc(BATTLE_EVENT_MARK, 0);
                        {
                            s32 text;

                            text = ((s32)&Value_00000822);
                            if ((u32)target_id <= 7)
                                text += 1;
                            Func_080bbabc(BATTLE_EVENT_TEXT_CONTINUE, text);
                        }
                    }
                }
                dmg += Func_080771a0() & 3;
                {
                    guard = (*(s8 *)&(target->guard_level));
                    if (guard != 0) {
                        if (guard == 1)
                            dmg /= 2;
                        else
                            dmg = Func_080022ec(dmg, 10);
                    }
                }
                if (dmg <= 0)
                    dmg = 1;
                if (crush != 0) {
                    if (dmg < cur - 1) {
                        dmg = cur - 1;
                        if (dmg <= 0)
                            dmg = 1;
                    }
                }
                if (Func_080770c0(366) != 0 && *cmd == 5 && cur <= dmg) {
                    dmg = cur - 1;
                }
                pass++;
            } while (pass <= 1);
            Func_080bbabc(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            cur -= dmg;
            Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = ((s32)&Value_00000834) + affinity;
                else
                    text = ((s32)&Value_00000831) + affinity;
                Func_080bbabc(BATTLE_EVENT_TEXT, text);
            }
            if (cur <= 0) {
                Func_080bbabc(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                {
                    if ((u32)target_id <= 7)
                        Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000825)));
                    else
                        Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000824)));
                }
            } else
                Func_080bbabc(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Func_08077128(target_id);
            break;
        }

        case DK_PP_DRAIN:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            if (range != 4) {
                s32 off;

                off = range * 4;
                off = off + 72;
                bonus = power - ((s16 *)((u8 *)target + off))[1];
            }
            dmg = action->power;
            dmg = Func_08077188(dmg, bonus, 256);
            dmg = Func_080022ec(dmg * Data_080c2ac0[offset], 100);
            dmg *= adjust;
            {
                guard = (*(s8 *)&(target->guard_level));
                if (guard != 0) {
                    if (guard == 1)
                        dmg /= 2;
                    else
                        dmg = Func_080022ec(dmg, 10);
                }
            }
            if (action->effect == EFX_DRAIN_PP && dmg > pp)
                dmg = pp;
            Func_080bbabc(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = ((s32)&Value_0000082a);
                else
                    text = ((s32)&Value_00000829);
                Func_080bbabc(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            Func_080bbabc(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->pp - pp;
            target->pp = pp;
            Func_08077128(target_id);
            break;
        }

        case DK_HP_HEAL:
        {
            if (action->power == 0)
                break;
            cur = target->hp;
            dmg = action->power;
            dmg = Func_08077190(dmg, range == 4 ? 100 : power, 256);
            dmg = Func_080022ec(dmg * Data_080c2ad8[offset], 100);
            dmg *= adjust;
            dmg += Func_080771a0() & 3;
            cur += dmg;
            if (cur > target->max_hp) {
                cur = target->max_hp;
                dmg = cur - target->hp;
            }
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            if (cur == target->max_hp)
                Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000820));
            else {
                Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
                Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000081d));
            }
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Func_08077128(target_id);
            break;
        }

        case DK_PP_DMG:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            {
                if (range != 4) {
                    s32 off;

                    off = range * 4 + 72;
                    bonus = power - ((s16 *)((u8 *)target + off))[1];
                }
            }
            dmg = action->power;
            dmg = Func_08077188(dmg, bonus, 256);
            dmg = Func_080022ec(dmg * Data_080c2af0[offset], 100);
            dmg *= adjust;
            {
                guard = (*(s8 *)&(target->guard_level));
                if (guard != 0) {
                    if (guard == 1)
                        dmg /= 2;
                    else
                        dmg = Func_080022ec(dmg, 10);
                }
            }
            Func_080bbabc(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = ((s32)&Value_00000827);
                else
                    text = ((s32)&Value_00000826);
                Func_080bbabc(BATTLE_EVENT_TEXT, text);
                pp -= dmg;
            }
            if (pp <= 0)
                pp = 0;
            Func_080bbabc(BATTLE_EVENT_ACTOR_FINISH, target_id);
            goto pp_store;
        }

        case DK_HP_DMG_6:
        case DK_HP_DMG_7:
        case DK_HP_DMG_9:
        {
            if (action->power == 0)
                break;
            cur = target->hp;
            pass = 1;
            do {
                {
                    if (range != 4) {
                        s32 off;

                        off = range * 4 + 72;
                        bonus = power - ((s16 *)((u8 *)target + off))[1];
                    }
                }
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
                    }
                    dmg += Func_080022ec(target->max_hp * kind, 100);
                }
                dmg = Func_08077188(dmg, bonus, 256);
                dmg *= adjust;
                switch (nibble & 15) {
                case 5:
                    dmg = Func_080022ec(Data_080c2b08[offset] * dmg, 100);
                    break;
                case 8:
                    dmg = Func_080022ec(dmg * Data_080c2b20[offset], 100);
                    break;
                case 6:
                    dmg = Func_080022ec(dmg * Data_080c2b38[offset], 100);
                    break;
                }
                dmg += Func_080771a0() & 3;
                {
                    guard = (*(s8 *)&(target->guard_level));
                    if (guard != 0) {
                        if (guard == 1)
                            dmg /= 2;
                        else
                            dmg = Func_080022ec(dmg, 10);
                    }
                }
                if (Func_080770c0(366) != 0 && *cmd == 6 && cur > dmg) {
                    dmg = cur;
                }
                pass++;
            } while (pass <= 1);
            Func_080bbabc(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = ((s32)&Value_00000834) + affinity;
                else
                    text = ((s32)&Value_00000831) + affinity;
                Func_080bbabc(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }
            if (cur <= 0) {
                Func_080bbabc(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
                {
                    s32 text;

                    cur = 0;
                    if ((u32)target_id <= 7)
                        text = ((s32)&Value_00000825);
                    else
                        text = ((s32)&Value_00000824);
                    Func_080bbabc(BATTLE_EVENT_TEXT, text);
                }
            } else
                Func_080bbabc(BATTLE_EVENT_ACTOR_FINISH, target_id);
            dealt = target->hp - cur;
            target->hp = (s16)cur;
            Func_08077128(target_id);
            break;
        }

        case DK_PP_HEAL:
        {
            if (action->power == 0)
                break;
            pp = target->pp;
            dmg = action->power;
            dmg = Func_08077190(dmg, range == 4 ? 100 : power, 256);
            dmg = Func_080022ec(dmg * Data_080c2b50[offset], 100);
            dmg *= adjust;
            pp += dmg;
            if (pp > target->max_pp) {
                pp = target->max_pp;
                dmg = pp - target->pp;
            }
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            if (pp == target->max_pp)
                Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000821));
            else {
                Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
                Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000081e));
            }
pp_store:
            target->pp = (s16)pp;
            Func_08077128(target_id);
            break;
        }

        case 0:
        case 7:
        case 9:
            /* 空スロット */
            break;

        case DK_HP_DMG:
            if (hit != 0) {
            if (action->power == 0)
                break;
            cur = target->hp;
            {
                if (range != 4) {
                    s32 off;

                    off = range * 4 + 72;
                    bonus = power - ((s16 *)((u8 *)target + off))[1];
                }
            }
            dmg = action->power;
            dmg = Func_08077188(dmg, bonus, 256);
            dmg *= adjust;
            dmg = Func_080022ec(dmg * Data_080c2b68[offset], 100);
            {
                guard = (*(s8 *)&(target->guard_level));
                if (guard != 0) {
                    if (guard == 1)
                        dmg /= 2;
                    else
                        dmg = Func_080022ec(dmg, 10);
                }
            }
            Func_080bbabc(BATTLE_EVENT_ACTOR_BEGIN, target_id);
            Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            {
                s32 text;

                if ((u32)target_id <= 7)
                    text = ((s32)&Value_00000827);
                else
                    text = ((s32)&Value_00000826);
                Func_080bbabc(BATTLE_EVENT_TEXT, text);
                cur -= dmg;
            }
            if (cur <= 0) {
                Func_080bbabc(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
                Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
                cur = 0;
                {
                    if ((u32)target_id <= 7)
                        Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000825)));
                    else
                        Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000824)));
                }
            } else
                Func_080bbabc(BATTLE_EVENT_ACTOR_FINISH, target_id);
dealt = target->hp - cur;
            target->hp = (s16)cur;
            Func_08077128(target_id);
            break;
            }
            Func_080bbabc(BATTLE_EVENT_ACTOR_FINISH, target_id);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000854));
            break;

        }
    }

    /* 付加効果 */
    Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
    if (Func_080772b8(action->effect) == 0 && target->hp == 0
        && Func_080bbae8(action->effect) == 0)
        goto done;
    if (hit == 0)
        goto done;
    if ((u32)(action->effect - 3) > 66)
        goto done;

    switch (action->effect) {
    case EFX_CURE_ALL:
        if (target->delusion != 0) {
            target->delusion = 0;
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000088b));
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->stun != 0) {
            target->stun = 0;
            Func_080bbabc(BATTLE_EVENT_RESET, 0);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000088d));
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000088c));
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->death_count != 0) {
            target->death_count = 0;
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000894));
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->evil_spirit != 0) {
            target->evil_spirit = 0;
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000088f));
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
        }
        if (target->poison != 0) {
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000884));
            target->poison = 0;
        }
        Func_080bbabc(BATTLE_EVENT_RESET, 0);
        break;

    case EFX_CURE_PART:
        if (target->delusion != 0) {
            target->delusion = 0;
            Func_080bbabc(BATTLE_EVENT_RESET, 0);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000088b));
        }
        if (target->stun != 0) {
            target->stun = 0;
            Func_080bbabc(BATTLE_EVENT_RESET, 0);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000088d));
        }
        target->sleep = 0;
        if (target->psy_seal != 0) {
            target->psy_seal = 0;
            Func_080bbabc(BATTLE_EVENT_RESET, 0);
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000088c));
        }
        if (target->death_count == 0)
            break;
        target->death_count = 0;
        Func_080bbabc(BATTLE_EVENT_RESET, 0);
        Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000894));
        break;

    case EFX_HEAL_60:
    case EFX_HEAL_30:
    {
        s32 old;
        s32 maxu;
        s32 maxv;
        s32 heal;
        u16 *stat_ptr;

        stat_ptr = (u16 *)&target->hp;
        old = *stat_ptr--;
        heal = *(s16 *)(stat_ptr + 1);
        stat_ptr--;
        maxu = *stat_ptr--;
        maxv = *(s16 *)(stat_ptr + 1);
        if (action->effect == EFX_HEAL_60)
            heal += Func_080022ec(maxv * 60, 100);
        else
            heal += Func_080022ec(maxv * 30, 100);
        if (heal > (s16)maxu)
            heal = (s16)maxu;
        tmp = heal - (s16)old;
        if (tmp == 0 && nibble != 1)
            break;
        if (heal == (s16)maxu)
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000820));
        else {
            Func_080bbabc(BATTLE_EVENT_VALUE, tmp);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000081d));
        }
        target->hp = (s16)heal;
        Func_08077128(target_id);
        break;
    }

    case EFX_PP_RESTORE_7:
    {
        s32 old;
        s32 maxv;
        s32 heal;

        heal = target->pp;
        maxv = target->max_pp;
        old = heal;
        heal += Func_080022ec(maxv * 7, 100);
        if (heal > maxv)
            heal = maxv;
        tmp = heal - old;
        if (tmp == 0 && nibble != 11)
            break;
        if (heal == maxv)
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000821));
        else {
            Func_080bbabc(BATTLE_EVENT_VALUE, tmp);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000081e));
        }
        target->pp = (s16)heal;
        Func_08077128(target_id);
        break;
    }

    case EFX_AGI_SET_UP8:
        (*(s8 *)&(target->agility_modifier)) = 8;
        target->agility_modifier_turns = 5;
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, target->agility - copy->agility);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000877));
        break;

    case EFX_AGI_SET_DOWN4:
        am = &(*(s8 *)&(target->agility_modifier));
    {
        u8 v;

        v = -4;
        *am = v;
    }
        target->agility_modifier_turns = 5;
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, copy->agility - target->agility);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000878));
        break;

    case EFX_ATK_DOWN1:
        target->attack_modifier += -1;
        {
            if ((target->attack_modifier) < -4)
                (target->attack_modifier) = -4;
            if ((target->attack_modifier) > 4)
                (target->attack_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000860));
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_DOWN2:
        target->attack_modifier += -2;
        {
            if ((target->attack_modifier) < -4)
                (target->attack_modifier) = -4;
            if ((target->attack_modifier) > 4)
                (target->attack_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, copy->attack - target->attack);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000860));
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_UP1:
        target->attack_modifier += 1;
        {
            if ((target->attack_modifier) < -4)
                (target->attack_modifier) = -4;
            if ((target->attack_modifier) > 4)
                (target->attack_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000861));
        target->attack_modifier_turns = 7;
        break;

    case EFX_ATK_UP2:
        target->attack_modifier += 2;
        {
            if ((target->attack_modifier) < -4)
                (target->attack_modifier) = -4;
            if ((target->attack_modifier) > 4)
                (target->attack_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, target->attack - copy->attack);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000861));
        target->attack_modifier_turns = 7;
        break;

    case EFX_DEF_DOWN1:
        target->defense_modifier += -1;
        {
            if ((target->defense_modifier) < -4)
                (target->defense_modifier) = -4;
            if ((target->defense_modifier) > 4)
                (target->defense_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000862));
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_DOWN2:
        target->defense_modifier += -2;
        {
            if ((target->defense_modifier) < -4)
                (target->defense_modifier) = -4;
            if ((target->defense_modifier) > 4)
                (target->defense_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, copy->defense - target->defense);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000862));
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_UP1:
        target->defense_modifier += 1;
        {
            if ((target->defense_modifier) < -4)
                (target->defense_modifier) = -4;
            if ((target->defense_modifier) > 4)
                (target->defense_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000863));
        target->defense_modifier_turns = 7;
        break;

    case EFX_DEF_UP2:
        target->defense_modifier += 2;
        {
            if ((target->defense_modifier) < -4)
                (target->defense_modifier) = -4;
            if ((target->defense_modifier) > 4)
                (target->defense_modifier) = 4;
        }
        Func_08077010(target_id);
        Func_080bbabc(BATTLE_EVENT_VALUE, target->defense - copy->defense);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000863));
        target->defense_modifier_turns = 7;
        break;


    case EFX_REVIVE_FULL:
        if (target->hp != 0)
            break;
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000864));
        target->hp = target->max_hp;
        Func_08077128(target_id);
        break;

    case EFX_REVIVE_HALF:
        if (target->hp != 0)
            break;
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000864));
        target->hp = (s16)(target->max_hp / 2);
        Func_08077128(target_id);
        break;

    case EFX_REVIVE_80:
        if (target->hp != 0)
            break;
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000864));
        target->hp = (s16)Func_080022ec(target->max_hp * 8, 10);
        Func_08077128(target_id);
        break;

    case EFX_CURE_POISON:
        if (target->poison != 0)
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000884));
        target->poison = 0;
        break;

    case EFX_RES_DOWN1:
        {
            target->res_modifier += (-1);
            {
                if ((target->res_modifier) < -4)
                    (target->res_modifier) = -4;
                if ((target->res_modifier) > 4)
                    (target->res_modifier) = 4;
            }
            Func_080bbabc(BATTLE_EVENT_VALUE, ((copy->res_modifier - target->res_modifier) * 20));
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000865)));
            target->res_modifier_turns = 7;
        }
        break;

    case EFX_RES_DOWN2:
    {
        target->res_modifier += -2;
        if (-4 > target->res_modifier)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        Func_080bbabc(BATTLE_EVENT_VALUE, (copy->res_modifier - target->res_modifier) * 20);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000865));
        target->res_modifier_turns = 7;
    }
        break;

    case EFX_RES_UP1:
        {
            target->res_modifier += (1);
            {
                if ((target->res_modifier) < -4)
                    (target->res_modifier) = -4;
                if ((target->res_modifier) > 4)
                    (target->res_modifier) = 4;
            }
            Func_080bbabc(BATTLE_EVENT_VALUE, ((target->res_modifier - copy->res_modifier) * 20));
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000866)));
            target->res_modifier_turns = 7;
        }
        break;

    case EFX_RES_UP2:
    {
        target->res_modifier += 2;
        if (*(rm = &(*(s8 *)&(target->res_modifier))) < -4)
            target->res_modifier = -4;
        if (target->res_modifier > 4)
            target->res_modifier = 4;
        Func_080bbabc(BATTLE_EVENT_VALUE, (target->res_modifier - copy->res_modifier) * 20);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000866));
        target->res_modifier_turns = 7;
    }
        break;

    case EFX_POISON:
        if (target->poison != 0)
            break;
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000867));
        target->poison = 1;
        break;

    case EFX_VENOM:
        if (target->poison > 1)
            break;
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000874));
        target->poison = 2;
        break;

    case EFX_DELUSION:
        {
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000868)));
            (target->delusion) = 7;
        }
        break;

    case EFX_CONFUSE:
        {
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000869)));
            (target->confusion) = 7;
        }
        break;

    case EFX_CHARM:
        {
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000086a)));
            (target->charm) = 7;
        }
        break;

    case EFX_STUN:
        {
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000086b)));
            (target->stun) = 7;
        }
        break;

    case EFX_SLEEP:
        {
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000086c)));
            (target->sleep) = 7;
        }
        break;

    case EFX_PSY_BLOCK:
        {
            if ((u32)target_id <= 7)
                Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000086d)));
            else
                Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000876)));
        }
        target->psy_seal |= 7;
        break;

    case EFX_PSY_SEAL:
        {
            if ((u32)target_id <= 7)
                Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000086d)));
            else
                Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000876)));
        }
        target->psy_seal |= 16;
        break;

    case EFX_INSTANT_DOWN:
        Func_080bbabc(BATTLE_EVENT_ACTOR_RESOLVE, target_id);
        if (target->status_12a == 2)
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000084f));
        else if (action_id == 219)
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000850));
        else
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000084c));
        target->hp = 0;
        Func_08077128(target_id);
        break;

    case EFX_REFRAIN:
        {
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000086f)));
            (target->refrain) = 7;
        }
        break;

    case EFX_REFLECT:
        {
            Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_00000870)));
            (target->reflect) = 7;
        }
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
        Func_080bbabc(BATTLE_EVENT_RESET, 0);
        Func_080bbabc(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_hp)
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000820));
        else {
            Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000081d));
        }
        actor->hp = (s16)heal;
        Func_08077128(actor_id);
        break;
    }

    case EFX_DRAIN_PP:
    {
        s32 heal;
        s32 amt;

        heal = actor->pp;
        amt = dealt;
        heal += amt;
        if (heal > actor->max_pp) {
            heal = actor->max_pp;
            amt = heal - actor->pp;
        }
        Func_080bbabc(BATTLE_EVENT_RESET, 0);
        Func_080bbabc(BATTLE_EVENT_UNIT, actor_id);
        if (heal == actor->max_pp)
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000821));
        else {
            Func_080bbabc(BATTLE_EVENT_VALUE, amt);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000081e));
        }
        actor->pp = (s16)heal;
        Func_08077128(actor_id);
        break;
    }

    case EFX_PP_LEECH:
        dmg = Func_080022ec(dealt, 10);
        if (target->pp < dmg)
            dmg = target->pp;
        if (actor->pp + dmg > actor->max_pp)
            dmg = actor->max_pp - actor->pp;
        if (dmg == 0)
            break;
        Func_080bbabc(BATTLE_EVENT_VALUE, dmg);
        {
            if ((u32)target_id <= 7)
                Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000085f)));
            else
                Func_080bbabc(BATTLE_EVENT_TEXT, (((s32)&Value_0000085e)));
        }
        Func_08077120(actor_id, dmg);
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
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000896));
        break;

    case EFX_EVIL_SPIRIT:
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000872));
        target->evil_spirit = 1;
        break;

    case EFX_DEATH_CURSE:
        if (target->death_count == 0) {
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000873));
            target->death_count = 7;
            break;
        }
        if (target->death_count <= 1)
            break;
        target->death_count -= 1;
        Func_080bbabc(BATTLE_EVENT_VALUE, target->death_count);
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000875));
        break;

    case EFX_READY:
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000087d));
        ((u8 *)target)[0x144] = 2;  /* ready_pose。構造体表記だと共有尾が壊れる */
        break;

    case EFX_CHALLENGE:
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000087e));
        target->battle_end_state = 1;
        if ((u32)target_id <= 7)
            ((u8 *)work)[67] |= 2;
        break;

    case EFX_IMMOBILIZE:
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_0000087f));
        target->cannot_move = 1;
        break;

    case EFX_GUARD1:
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000881));
        if ((*(s8 *)&(target->guard_level)) > 0)
            break;
        g1 = 1;
        (*(s8 *)&(target->guard_level)) = g1;
        break;

    case EFX_GUARD2:
        Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000882));
        if ((*(s8 *)&(target->guard_level)) > 1)
            break;
        (*(s8 *)&(target->guard_level)) = 2;
        break;

    case EFX_TEXT_NONE:
        Func_080bbabc(BATTLE_EVENT_TEXT, (u32)-1);
        break;

    default:
        break;
    }

done:
    /* 終了処理 */
    Func_080bbabc(BATTLE_EVENT_RESET, 0);
    if (target->hp != 0) {
        if (target->sleep != 0)
        if (target->sleep <= 6
            && dealt > 0 && (3 & Func_080771a0()) == 0) {
            target->sleep = 0;
            Func_080bbabc(BATTLE_EVENT_UNIT, target_id);
            Func_080bbabc(BATTLE_EVENT_TEXT, ((s32)&Value_00000883));
        }
    }
    Func_08002df0(copy);
    Func_08077010(target_id);
    Func_08015130(((u8 *)Data_03001e74)[65]);
    if (target->hp != 0)
        Func_080bbabc(BATTLE_EVENT_ACTOR_FINISH, target_id);
    if (actor->evil_spirit != 0 && (Func_080771a0() & 3) == 0 && dealt > 0) {
        s32 share;

        share = dealt >> 2;
        if (share == 0)
            share = 1;
        plan->pending_amount_60 += share;
    }
}
