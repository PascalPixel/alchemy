#include "TYPES.H"

void Djinn_AddToOwner(s32 owner, s32 element, s32 djinn);
void Trade_AddOffer(s32 owner, s32 element, s32 djinn);
void GameFlag_SetBit(s32 flag);

/* Gives a party member a djinni, offers it for trade and raises its found
   flag, 0x30 + element * 20 + djinni. */
void Djinn_AddFoundToOwner(s32 owner, s32 element, s32 djinn)
{
    s32 flag = element * 20 + djinn;

    Djinn_AddToOwner(owner, element, djinn);
    flag += 0x30;
    Trade_AddOffer(owner, element, djinn);
    GameFlag_SetBit(flag);
}
