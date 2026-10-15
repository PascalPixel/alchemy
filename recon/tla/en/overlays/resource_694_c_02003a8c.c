/* Draft, differ 10 of 220: overlay 694's scene setup, which on entrances 18 and 19 shows the coins
 * won or lost and on entrance 20 hands out the prize list, written from the listing (its ☀️ twin
 * by score, tolbi_spring.c's TorebiIzumi_OpenScene, is a different scene). Left: scheduling only.
 * The native builds -1 after the ldrsb of the list head and of the next prize (movs; ldrsb; movs
 * r5/r2, #1; negs), loads the item for PartyInventory_GiveItem after the mode 0 (movs r1, #0;
 * adds r0, r5), and steps the prize pointer after the 0xc000 wait argument; the draft does each
 * the other way round. */
#include "../../../../games/tla/src/field/overlay_694/ov694.h"
#include "call.h"

struct Ov694Prizes {
    u8 unknown_000[0x14c];
    s8 list[8];
};

extern u8 gSceneState[];

void Func_02001c44(s32 id);
s32 Func_02001a60(s32 topic);
void Func_0200403c(s32 value, s32 digits);
void Func_020041fc(s32 item, s32 count);
void PartyInventory_GiveItem(s32 item, s32 mode);
void AudioCommand_WaitForStateByteClear(void);

void Func_02003a8c(void)
{
    s32 diff;
    s8 *list;
    s8 *prize;
    s32 item;

    ((struct EventWork *)Ram_HeapSlots->event_work)->screen_effect = 0x209;
    Func_02001c44(13);
    Func_02001c44(16);
    Func_02001c44(17);
    Func_02001c44(19);
    Func_02001c44(20);
    Object_SetPartAttribute(Object_GetById(14), 1);
    Object_SetPartAttribute(Object_GetById(15), 2);
    Object_SetPartAttribute(Object_GetById(8), 0);
    Object_SetPartAttribute(Object_GetById(9), 3);
    if (!Value1((s32 (*)())GameFlag_Test, 0x9e5))
        return;
    GameFlag_ClearBit(0x9e5);
    if (gPartyState.entrance == 18)
    {
        diff = gPartyState.coins - *(s32 *)gSceneState;
        Func_0200406c();
        Func_02004204(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        if (diff > 0)
        {
            if (diff > 19999)
                Func_020042b4(93);
            else if (diff > 4999)
                Func_020042b4(92);
            else
                Func_020042b4(91);
            Battle_WaitMode0(20);
            Func_0200412c((s32)MsgScene694No15);
            Func_0200403c(diff, 5);
            Func_02004144(14, 0);
            AudioCommand_WaitForStateByteClear();
        }
        else if (diff < 0)
        {
            Func_0200412c((s32)MsgScene694No16);
            Func_0200403c(-diff, 5);
            Func_02004144(14, 0);
        }
        Func_02004074();
    }
    else if (gPartyState.entrance == 19)
    {
        diff = gPartyState.coins - *(s32 *)gSceneState;
        Func_0200406c();
        Func_02004204(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        if (diff > 0)
        {
            if (diff > 19999)
                Func_020042b4(93);
            else if (diff > 4999)
                Func_020042b4(92);
            else
                Func_020042b4(91);
            Battle_WaitMode0(20);
            Func_0200412c((s32)MsgScene694No15);
            Func_0200403c(diff, 5);
            Func_02004144(14, 0);
            AudioCommand_WaitForStateByteClear();
        }
        else if (diff < 0)
        {
            Func_0200412c((s32)MsgScene694No16);
            Func_0200403c(-diff, 5);
            Func_02004144(14, 0);
        }
        Func_02004074();
    }
    else if (gPartyState.entrance == 20)
    {
        list = ((struct Ov694Prizes *)&gPartyState)->list;
        Func_0200406c();
        Func_02004204(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Battle_WaitMode0(20);
        if (list[0] == -1)
        {
            Func_02001938(1);
        }
        else if (list[0] == -2)
        {
            Func_0200412c((s32)MsgScene694No04);
            Func_02004144(18, 0);
        }
        else
        {
            Func_0200412c((s32)MsgScene694No05);
            Func_02004144(18, 0);
            if (list[0] != -1)
            {
                prize = list;
            next:
                    if (prize == list)
                        Func_0200412c((s32)MsgScene694No11);
                    else
                        Func_0200412c((s32)MsgScene694No14);
                    item = Func_02001a60(*prize);
                    Func_0200403c(item, 2);
                    Func_02004144(18, 0);
                    Func_020041fc(item, 3);
                    PartyInventory_GiveItem(item, 0);
                    prize++;
                    Func_0200415c(0, 0xc000);
                if (*prize != -1)
                    goto next;
            }
            ((struct Ov694Prizes *)&gPartyState)->list[0] = -2;
            Func_0200412c((s32)MsgScene694No17);
            Func_02004144(18, 0);
        }
        Func_02004074();
    }
}
