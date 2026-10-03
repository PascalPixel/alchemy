#include "RUNTIME_MEM.H"
#include "FIELDOBJ.H"
#include "TYPES.H"
#include "SCENE.H"
#include "BATTLE_TYPES.H"
#include "BATTLE_RUNTIME.H"
#include "PARTY_STATE.H"
#include "SOUND_IDS.H"
#include "FIXED_MATH.H"
#include "EVENT_RUNTIME.H"
#include "OBJECT_RUNTIME.H"

s32 Party_CountActiveOwnersFar(void);
s16 Owner_AdjustFirstValueFar(s32 owner, s32 amount);
s16 Owner_AdjustSecondValueFar(s32 owner, s32 amount);
void BattleFx_ApplyColorToSourceBuffer(s32 color, s32 mode);
void BattleFx_StartBufferInterpolation(s32 frames);
void Audio_PlayCue(s32 cue);

#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
s32 Map_ResumeAnimationFar(void);

s32 Map_LoadAreaGraphicsFar(void);

/* Party-wide HP changes in battle: drains, direct or percentage deltas, and
   the poison and venom damage applied at the end of a round. */
void BattleParty_ApplyDrain(s32 amount)
{
    s32 target_count = Party_CountActiveOwnersFar();

    if (target_count > 0) {
        /* FAKEMATCH: the existing base-plus-offset cursor retains the address
           setup and operand order; a direct array address folds into the pool. */
        u8 *base = (u8 *)&gGameState;
        s32 offset = (u32)&((struct GameState *)0)->active_owners;
        u8 *target_id = base + offset;
        s32 remaining = target_count;

        do {
            Owner_AdjustSecondValueFar(*target_id++, amount);
            remaining--;
        } while (remaining != 0);
    }
}

/* Passes a health change to every active party member: amount itself, or
 * when scaled amount percent of the member's maximum HP, falling back to the
 * magnitude of amount when that rounds to zero. A loss first flashes the
 * screen and plays the heavy impact cue when it exceeds ten, a lighter hit
 * cue otherwise; a gain plays the recovery cue. */
void BattleParty_ApplyHealthDelta(s32 amount, s32 scaled)
{
    s32 count;
    s32 i;
    s32 value;
    struct BattleUnit *unit;

    if (amount < 0) {
        BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        if (amount < -10)
            Audio_PlayCue(SOUND_HEAVY_IMPACT);
        else
            Audio_PlayCue(133);
    } else {
        Audio_PlayCue(SOUND_RECOVERY);
    }

    count = Party_CountActiveOwnersFar();
    for (i = 0; i < count; i++) {
        unit = Owner_GetStateFar(gGameState.active_owners[i]);
        if (!scaled) {
            value = amount;
        } else {
            value = unit->max_hp * amount / 100;
            if (value == 0) {
                value = amount;
                if (value < 0)
                    value = -value;
            }
        }
        Owner_AdjustFirstValueFar(gGameState.active_owners[i], value);
    }
}

s32 BattleParty_ApplyStatusDamage(void)
{
    s32 result = 0;
    s32 count = Party_CountActiveOwnersFar();

    if (result < count) {
        /* FAKEMATCH: preserve the existing member-offset cursor setup; the
           direct array address changes the pool addend and saved registers. */
        s32 offset = (u32)&((struct GameState *)0)->active_owners / 2;
        u8 *entry;
        s32 remaining;

        offset <<= 1;
        entry = (u8 *)&gGameState + offset;
        remaining = count;

        do {
            struct BattleUnit *unit = Owner_GetStateFar(*entry);
            s32 amount;

            switch (unit->poison) {
            case 1:
                amount = -((unit->max_hp + 10) / 20);
                if (amount == 0)
                    amount = -1;
                if (result <= 0)
                    result = 1;
                break;
            case 2:
                amount = -((unit->max_hp + 5) / 10);
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
            Owner_AdjustFirstValueFar(*entry, amount);
            entry++;
        } while (remaining != 0);
    }

    if (result != 0) {
        BattleFx_ApplyColorToSourceBuffer(0x1ff, 0);
        BattleFx_StartBufferInterpolation(4);
        Audio_PlayCue(133);
    }

    return result;
}

void Battle_SetObjectFlag5bWhenMode3(void)
{
    struct EventRuntime *work;
    void *blk;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (work->mode_19e == 3) {
        blk = Runtime_AllocateBlock(0x1F, 0x540);
        if ((blk != NULL) && (FIELD_AT_OFFSET(blk, s8 *, 0x53D) != 0)) {
            FIELD_AT_OFFSET(blk, s8 *, 0x53A) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53B) = 0;
            FIELD_AT_OFFSET(blk, s8 *, 0x53C) = 1;
            FIELD_AT_OFFSET(blk, s8 *, 0x53D) = 0;
        }
        ((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 1;
        Map_ResumeAnimationFar();
    }
}

void Battle_ClearObjectFlag5bWhenMode3(void)
{
    struct EventRuntime *work;

    work = Runtime_AllocateBlock(0x1B, 0xCCC);
    if (work->mode_19e == 3) {
        Map_LoadAreaGraphicsFar();
        ((struct ObjectRuntime *)((struct EventWork *)work)->view_center)->movement_state = 0;
    }
}
