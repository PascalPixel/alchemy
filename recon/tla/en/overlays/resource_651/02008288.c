
#include "types.h"
#include "field_event.h"
#include "party_state.h"

extern u8 MsgDeriTaviRikiWorry[];
extern u8 MsgDeriTaviRikiHome[];

void DeriMura_TalkShopkeeper(s32 keeper)
{
    struct FieldActor *leader = Engine_ActorGet(gPartyState.current_owner);

    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Engine_ShopOpen(3, keeper);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriTaviRikiWorry);
        Engine_EventShowMessage(keeper, 0);
        Engine_EventEnd();
    }
}

void DeriMura_TalkShopkeeperRelieved(s32 keeper)
{
    struct FieldActor *leader = Engine_ActorGet(gPartyState.current_owner);

    if ((u16)((leader->facing + 0x2000) & 0xc000) == 0xc000) {
        Engine_ShopOpen(3, keeper);
    } else {
        Engine_EventBegin();
        Engine_EventResolvePendingActions(0);
        Engine_EventSetMessage((s32)MsgDeriTaviRikiHome);
        Engine_EventShowMessage(keeper, 0);
        Engine_EventEnd();
    }
}
