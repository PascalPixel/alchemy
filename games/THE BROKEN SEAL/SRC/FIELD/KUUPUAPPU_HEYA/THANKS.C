#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "FIELD_SCENE.H"

extern u8 MsgKuupuappuHeadingOutBeyondGomaRange[];
extern u8 MsgKuupuappuYouWereSuchGreatHelp[];

enum {
    ITEM_WATER_OF_LIFE = 189
};

void SceneActor_SetModeZeroAndValue(s32 a, s32 b);
void SceneEffect_ApplyThreeValuesAndFinish();
void SceneActor_SetPairZeroAndValue();
s32 PartyInventory_HasSpace();

/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}

/* Actor 16 thanks the party once with a Water of Life, when the party has
 * room for it, and then asks where they are heading. */
void FieldScene_RunScene383SequenceC(void)
{
    Event_Begin();
    if (GameFlag_IsSet(0x857) == 0) {
        Event_SetMessage((s32)MsgKuupuappuYouWereSuchGreatHelp);
        SceneActor_SetModeZeroAndValue(16, 20);
        SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
        SceneActor_SetModeZeroAndValue(16, 30);
        Actor_FaceDirection(16, 0, 0);
        Event_Wait(30);
        Actor_RunRepeatedMotion(16, 2);
        Event_Wait(30);
        SceneActor_SetPairZeroAndValue(0, 16, 20);
        SceneEffect_ApplyThreeValuesAndFinish(16, 3, 20);
        bump_step(1);
        if (PartyInventory_HasSpace() == 0) {
            Event_SetMessage(((s32)MsgKuupuappuYouWereSuchGreatHelp + 3));
            SceneActor_SetModeZeroAndValue(16, 20);
            Event_End();
            goto done;
        }
        GameFlag_Set(0x857);
        Party_GiveItem(ITEM_WATER_OF_LIFE, 0);
    }
    Event_SetMessage((s32)MsgKuupuappuHeadingOutBeyondGomaRange);
    Event_OpenMessage(16, 0);
    Event_Wait(20);
    if (Event_ChooseYesNo(0, 0) != 0) {
        bump_step(1);
    }
    Event_ShowMessage(16, 0);
    Event_End();
done:;
}
