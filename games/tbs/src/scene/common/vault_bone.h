#include "types.h"
#include "fieldobj.h"
#include "field_event.h"

static void EV41_SEARCH(void)
{
	Engine_EventBegin();
	if (PartyInventory_HasSpace() == 0)
	{
		Engine_ActorSetAnimationAndWait(18, 4);
		Engine_EventWait(20);
		Engine_EventSetMessage(MsgVaultWowHaveManyThingsArent);
		Engine_EventShowMessage(18, 0);
	}
	else
	{
		Engine_ItemShowFound(ITEM_BONE, 3);
		Engine_PartyGiveItem(ITEM_BONE, 0);
	}
	Engine_EventEnd();
}
