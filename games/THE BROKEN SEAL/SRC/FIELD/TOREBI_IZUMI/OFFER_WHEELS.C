#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "SCENE_IDS.H"
extern u8 MsgTorebiBadComeBack[];
extern u8 MsgTorebiBadLikeUse[];
extern u8 MsgTorebiComeAgain[];
extern u8 MsgTorebiGetRidItems[];
extern u8 MsgTorebiStepRightEnjoy[];
extern u8 MsgTorebiStepRightTry[];

s32 PartyInventory_CountItem(s32 item);
void ObjectTable_Snapshot(void);
void UiWork_PushValueSlot(s32 value, s32 digits);
s32 PartyInventory_CountFreeSlots(void);
void Event_SetPair1c0AndSetValue170(s32 a, s32 b);
void Party_SetFields1ceAnd1d0(s32 scene, s32 entrance);

/* The lucky wheels barker: counts the game tickets (item 228), offers a
 * game, warns when the bag is nearly full (six or fewer free slots) and
 * starts the lucky wheels when the player agrees. */
void TorebiIzumi_OfferLuckyWheels(s32 spoken)
{
    s32 tickets = PartyInventory_CountItem(228);
    s32 room;

    ObjectTable_Snapshot();
    if (spoken == 0) {
        s32 message = (s32)MsgTorebiStepRightTry;

        Engine_EventSetMessage(message);
        Engine_EventShowMessage(8, 0);
        if (tickets == 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        Engine_EventSetMessage(message + 2);
        UiWork_PushValueSlot(tickets, 5);
        Engine_EventOpenMessage(8, 0);
        if (Engine_EventChooseYesNo(0, 0) != 0) {
            Engine_EventShowMessage(8, 0);
            return;
        }
        room = PartyInventory_CountFreeSlots();
        if (room == 0) {
            Engine_EventSetMessage(message + 4);
            Engine_EventOpenMessage(8, 0);
        } else {
            if (room > 6) {
                goto play;
            }
            Engine_EventSetMessage(message + 5);
            Engine_EventOpenMessage(8, 0);
        }
        if (room > 6 || Engine_EventChooseYesNo(0, 0) == 0) {
            goto play;
        }
        Engine_EventSetMessage((s32)MsgTorebiGetRidItems);
        Engine_EventShowMessage(8, 0);
        return;
    }
    if (tickets == 0) {
        Engine_EventSetMessage((s32)MsgTorebiBadComeBack);
        Engine_EventShowMessage(8, 0);
        return;
    }
    Engine_EventSetMessage((s32)MsgTorebiBadLikeUse);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) != 0) {
        Engine_EventSetMessage((s32)MsgTorebiComeAgain);
        Engine_EventShowMessage(8, 0);
        return;
    }
play:
    Engine_EventSetMessage((s32)MsgTorebiStepRightEnjoy);
    Engine_EventShowMessage(8, 0);
    Event_SetPair1c0AndSetValue170(508, 0);
    Party_SetFields1ceAnd1d0((s32)&SceneId_TorebiIzumi1, 12);
}
