#include "BATTLE_SUMMON.H"
#include "TBS_EDITION.H"
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

s32 Summon_ResetCharge(s32 class_id)
{
    u8 *summon;
    s32 object_index;
    s32 slot;
    s32 next_slot;
    u8 occupied;
    u8 marker;

    object_index = 0;
    marker = LIST_MARKER_CHAR;
scan_objects:
    summon = (u8 *)Owner_GetStateFar(object_index + 0x80);
    occupied = summon[298];
    if (occupied != 1) goto next_object;
    if (summon[296] != class_id) goto next_object;
    slot = 0;
    if (summon[0] != 0) goto scan_slots;
    summon[0] = marker;
    summon[occupied] = slot;
    return;
scan_slots:
    slot++;
    if (slot > 13) return;
    occupied = summon[slot];
    if (occupied != 0) goto scan_slots;
    next_slot = slot + 1;
    summon[slot] = marker;
    summon[next_slot] = occupied;
    return;
next_object:
    object_index++;
    if (object_index <= 5) goto scan_objects;
}
