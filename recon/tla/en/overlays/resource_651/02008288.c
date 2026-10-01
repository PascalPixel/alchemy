/* NONMATCHING: resource_651 at 0x02008288 and 0x02008340, for
 * FIELD/DERI_MURA, stay listing.
 *
 * Remaining difference: one instruction's place in each. The ROM loads the
 * leader's facing before the 0x2000 it adds (ldrh r3, [r0, #6]; movs r2,
 * #128); approved agscc with TLA's build flags sets the constant first.
 * alchemy permute found no
 * rewrite that moves it in 74,224 candidates. Every other byte matches.
 */

#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "PARTY_STATE.H"

extern u8 MsgDeriTaviRikiWorry[];
extern u8 MsgDeriTaviRikiHome[];

/* Across the counter the shopkeeper sells; from anywhere else he talks. */
void DeriMura_TalkShopkeeper(s32 keeper)
{
    struct FieldActor *leader = Engine_ActorGet(gPartyState.current_owner);

    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Engine_ShopOpen(3, keeper);
    } else {
        Engine_EventBegin();
        Engine_EventPrepareSpeakers(0);
        Engine_EventSetMessage((s32)MsgDeriTaviRikiWorry);
        Engine_EventShowMessage(keeper, 0);
        Engine_EventEnd();
    }
}

/* The shopkeeper once Tavi and Riki are home. */
void DeriMura_TalkShopkeeperRelieved(s32 keeper)
{
    struct FieldActor *leader = Engine_ActorGet(gPartyState.current_owner);

    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Engine_ShopOpen(3, keeper);
    } else {
        Engine_EventBegin();
        Engine_EventPrepareSpeakers(0);
        Engine_EventSetMessage((s32)MsgDeriTaviRikiHome);
        Engine_EventShowMessage(keeper, 0);
        Engine_EventEnd();
    }
}
