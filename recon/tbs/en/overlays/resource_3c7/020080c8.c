/* Draft of resource_3c7 0x020080c8..0x0200815c (148 bytes with pool),
 * RariberoHeya_RunItemShop; the listing keeps the rows. Remaining
 * difference: its messages have catalogue names now and its bytes match the
 * ROM, but it calls Engine_ShopOpen and Engine_UiWorkWaitThenFinalizeCapacity,
 * which the overlay's imports do not define. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgRariberoTakenHostageFaran[];
extern u8 MsgRariberoWentOceanLook[];

/* MsgRariberoTakenHostageFaran ("... was taken hostage, and Faran went after
 * them?") is loaded once; the two answers are derived from it at run time. */

/* The Lalivero shopkeeper: spoken to across the counter she opens shop 32;
 * otherwise she talks about the kidnapping, or about the rough sea once flag
 * 0x9a7 is set. */
void RariberoHeya_RunItemShop(s32 keeper)
{
    struct FieldActor *leader = Engine_ActorGet(0);

    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Engine_ShopOpen(32, keeper);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoWentOceanLook);
        Engine_EventShowMessage(keeper, 0);
    } else {
        s32 message = (s32)MsgRariberoTakenHostageFaran;

        Engine_EventSetMessage(message);
        Engine_EventOpenMessage(keeper, 0);
        if (Engine_UiWorkWaitThenFinalizeCapacity(0, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(message + 1);
        } else {
            Engine_EventSetMessage(message + 2);
        }
        Engine_EventShowMessage(keeper, 0);
    }
}
