/* Draft of resource_3c7 0x020080c8..0x0200815c (148 bytes with pool),
 * RariberoHeya_RunItemShop; the listing keeps the rows. Remaining
 * difference: the reference loads message 0x26e3 from its literal pool as a
 * link-time value and derives the answers from it; the integer message is
 * scheduled differently (156 bytes, 44 differ from +0x0). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

/* Message 9955, "... was taken hostage, and Faran went after them?", as a
 * link-time value, so the two answers are derived from it at run time. */

/* The Lalivero shopkeeper: spoken to across the counter she opens shop 32;
 * otherwise she talks about the kidnapping, or about the rough sea once flag
 * 0x9a7 is set. */
void RariberoHeya_RunItemShop(s32 keeper)
{
    struct FieldActor *leader = Engine_ActorGet(0);

    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Engine_ShopOpen(32, keeper);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage(10480);
        Engine_EventShowMessage(keeper, 0);
    } else {
        s32 message = 0x26e3;

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
