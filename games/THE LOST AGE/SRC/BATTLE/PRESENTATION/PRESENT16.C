#include "TYPES.H"
#include "OWNER_STATE.H"
#include "SYSTEM.H"
#include "BATTLE_PARTY.H"

#define FIELD(base, type, offset) (*(type)((u8 *)(base) + (offset)))

extern char MsgActorDefends;
extern char MsgCannotEscape;
s32 BattleObject_IsValidId(s32);
void UiWork_ClearValueNameTablesFar(void);
void UiText_DrawQuantity(s32 value, s32 mode);
void UiText_ShowMessageAndWaitCoreFar(s32 message_id);
void BattleMotion_SetupEscapeObject(s32 unit);

/* The party tries to run: fifteen times in sixteen each living member turns
   to flee in turn; otherwise they cannot get away. */
s32 BattleEscape_PlayRun(void *action)
{
    s16 party_members[14];
    s32 party_size;
    s32 animated;
    s32 member_slot;

    (void)action;
    if (((u32)(Random16() << 4) >> 16) != 0) {
        party_size = BattleParty_ListLivingUnits(BATTLE_SIDE_PARTY, (u16 *)party_members);
        animated = 0;
        if (party_size != 0) {
            member_slot = 0;
            do {
                BattleMotion_SetupEscapeObject(party_members[member_slot]);
                animated++;
                WaitFrames(8);
                member_slot++;
            } while (animated != party_size);
        }
        WaitFrames(22);
        return 1;
    }
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgCannotEscape);
    return 0;
}

s32 BattlePres_ShowMessageWhenField38Positive(s16 *script)
{
    s32 object_id;
    s32 result;
    void *object;

    object_id = *script;
    object = Owner_GetState(object_id);
    if (BattleObject_IsValidId(object_id) < 0) {
        return -1;
    }
    result = 0;
    if (FIELD(object, s16 *, 0x38) <= 0) {
        return result;
    }
    UiWork_ClearValueNameTablesFar();
    UiText_DrawQuantity(object_id, 1);
    UiText_ShowMessageAndWaitCoreFar((s32)&MsgActorDefends);
    return 0;
}
