#include "BATTLE_SUMMON.H"
#include "FIXED_MATH.H"

/* 召喚チャージ管理。クラスごとに使用中チャンネルのビットを持ち、 */
/* 取得・解放・名前印のリセットを行う。 */

#if defined(TBS_EDITION_JA)
#define CH_CNT 26
#else
#define CH_CNT 9
#endif

struct SummonChargeState {
    u8 unknown_00[0x10];
    u16 class_ids[6];   /* 0x10 */
    u32 used_masks[6];  /* 0x1c */
    s8 channels[6];     /* 0x34 */
    u8 unknown_3a[6];
    u8 count;           /* 0x40 */
};

struct BattleActorDefinition {
    u8 name[14];
    u8 unknown_0e[282];
    u8 class_id;
    u8 unavailable;
};

extern struct SummonChargeState *gBattleWork;

struct BattleActorDefinition *Owner_GetStateFar(s32 actor_id);

/* 番号表を線形探索し、既存なら次の空きビットを剰余で回して確保、 */
/* 無ければ表末尾に新規登録する。 */

s32 Summon_TakeCharge(s32 no, s32 n)
{
    struct SummonChargeState *w;
    s32 num;
    s32 i;
    s32 retry;
    s32 ch;

    w = gBattleWork;
    num = w->count;
    for (i = 0; i < num; i++) {
        if (w->class_ids[i] == no)
            break;
    }
    if (i != num) {
        retry = 0;
        if (w->channels[i] < 0) {
            w->channels[i] = 1;
            w->used_masks[i] = 3;
            return 0x8001;
        }
        for (; retry <= 31; retry++) {
            ch = Math_Mod(w->channels[i] + 1, CH_CNT);
            w->channels[i] = ch;
            if ((w->used_masks[i] & (1 << (s8)ch)) == 0)
                break;
        }
        w->used_masks[i] |= 1 << w->channels[i];
        return w->channels[i];
    }
    if (num <= 4) {
        w->channels[num] = -1;
        w->class_ids[num] = no;
        w->used_masks[num] = 0;
        w->count = num + 1;
        return CH_CNT;
    }
    return -1;
}
