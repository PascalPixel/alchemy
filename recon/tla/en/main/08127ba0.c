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

s32 Summon_ReleaseCharge(s32 actor_id)
{
    struct SummonChargeState *state = gBattleWork;
    struct BattleActorDefinition *actor;
    s32 count = state->count;
    s32 index;
    s32 name_length;
    s32 bit;
    s32 class_id;

    actor = Owner_GetStateFar(actor_id);
    if (actor->unavailable != 0)
        return;

    class_id = actor->class_id;
    for (index = 0; index < count; index++) {
        if (state->class_ids[index] == class_id)
            break;
    }
    if (index == count || state->used_masks[index] == 0)
        return;

    for (name_length = 0; name_length <= 13; name_length++) {
        if (actor->name[name_length] == 0)
            break;
    }

    bit = 32;
    if (name_length > 0)
        bit = actor->name[name_length - 1] - LIST_MARKER_CHAR;
    state->used_masks[index] &= ~(1 << bit);
}
