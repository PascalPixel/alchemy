#include "TYPES.H"
#include "FIELD_EVENT.H"
extern u8 MsgRariberoTakenHostageFaran[];
extern u8 MsgRariberoWentOceanLook[];

void Shop_Run(s32, s32);

/* The Lalivero shopkeeper: spoken to across the counter she opens shop 32;
 * otherwise she talks about the kidnapping, or about the rough sea once flag
 * 0x9a7 is set. The kidnapping question is loaded once and its two answers
 * are the lines after it. */
void RariberoHeya_RunItemShop(s32 keeper)
{
    struct FieldActor *leader = Engine_ActorGet(0);

    /* FAKEMATCH: the halfword cast of the masked facing keeps the
     * reference's compare. */
    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Shop_Run(32, keeper);
    } else if (Engine_GameFlagIsSet(0x9a7)) {
        Engine_EventSetMessage((s32)MsgRariberoWentOceanLook);
        Engine_EventShowMessage(keeper, 0);
    } else {
        s32 message = (s32)MsgRariberoTakenHostageFaran;

        Engine_EventSetMessage(message);
        Engine_EventOpenMessage(keeper, 0);
        if (Engine_EventChooseYesNo(0, 0) == 0) {
            Engine_EventWait(10);
            Engine_EventSetMessage(message + 1);
        } else {
            Engine_EventSetMessage(message + 2);
        }
        Engine_EventShowMessage(keeper, 0);
    }
}
