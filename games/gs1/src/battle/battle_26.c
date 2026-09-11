#include "types.h"
#include "scene.h"

/* battle/party/apply_drain.c */
struct BattlePartyTargetState {
    u8 padding[0x1f8];
    u8 target_ids[1];
};

extern struct BattlePartyTargetState gCell;

void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Battle_Check(amount);

    if (target_count > 0) {
        u8 *base = (u8 *)&gCell;
        s32 offset = 252 << 1;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            Battle_Apply(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
}

/* battle/party/apply_status_damage.c */
extern u8 gCell[];

u8 *Runtime_GetObject(s32);
s32 FixedPoint_Ratio(s32, s32);

void BattleFx_ApplyColorToSourceBuffer(void *, s32);
void BattleFx_StartBufferInterpolation(s32);
void Audio_PlayCue(s32);

s32 BattleParty_ApplyStatusDamage(void)
{
    s32 result = 0;
    s32 count = Battle_Check();

    if (result < count) {
        s32 offset = 252;
        u8 *entry;
        s32 remaining;

        offset <<= 1;
        entry = gCell + offset;
        remaining = count;

        do {
            u8 *object = Runtime_GetObject(*entry);
            s32 amount;

            switch ((s8)object[0x131]) {
            case 1:
                amount = -FixedPoint_Ratio(*(s16 *)(object + 0x34) + 10, 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -FixedPoint_Ratio(*(s16 *)(object + 0x34) + 5, 10);
                if (amount == 0)
                    amount = -1;
                if (result <= 1)
                    result = 2;
                break;
            default:
                amount = 0;
                break;
            }

            remaining--;
            Battle_Apply(*entry, amount);
            entry++;
        } while (remaining != 0);
    }

    if (result != 0) {
        BattleFx_ApplyColorToSourceBuffer((void *)0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        Audio_PlayCue(133);
    }

    return result;
}

/* battle/set_object_flag5b_when_mode3.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Battle_SetObjectFlag5bWhenMode3(void)
{
    void *work;
    void *blk;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
        blk = Runtime_AllocateBlock(0x1F, 0x540);
        if ((blk != NULL) && (FIELD_AT_OFFSET(blk, s8 *, 0x53D) != 0)) {
            FIELD_AT_OFFSET(blk, s8 *, 0x53A) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53B) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53C) = 1;
            FIELD_AT_OFFSET(blk, s8 *, 0x53D) = 0;
        }
        FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x1E0), s8 *, 0x5B) = 1;
        Battle_Check();
    }
}

/* battle/clear_object_flag5b_when_mode3.c */
void *Runtime_AllocateBlock(s32 arg0, s32 arg1);

void Battle_ClearObjectFlag5bWhenMode3(void)
{
    void *work;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (FIELD_AT_OFFSET(work, s16 *, 0x19E) == 3) {
        Battle_Check();
        FIELD_AT_OFFSET(FIELD_AT_OFFSET(work, void **, 0x1E0), s8 *, 0x5B) = 0;
    }
}
