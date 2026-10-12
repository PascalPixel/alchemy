#include "unit/menu/shop/shop.h"
#include "unit/menu/shop/buysell.h"
#include "unit/menu/shop/yadoya.h"
#include "unit/system/scheduler/runtime.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "mem.h"
#include "edition.h"
#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "party_state.h"
#include "btlsys.h"
#include "object_runtime.h"
#include "dma.h"
#include "callback_scheduler.h"
#include "resource.h"
#include "shop.h"
#include "ui.h"
#include "tbs_edition.h"
#include "io_reg.h"
#include "text/msg_ids.h"
#include "keystate.h"
#include "node_chain.h"
#include "game_state.h"
#include "sound_ids.h"
#include "item.h"
#include "fixed_math.h"
#include "far_runtime.h"
#include "lib.h"
#include "fixed_point_position.h"
#include "inn_runtime.h"
#include "event_runtime.h"
#include "btlcalc.h"
#include "fieldobj.h"

enum InnMessageId
{
	INN_MESSAGE_WELCOME = 0xd1c,
	INN_MESSAGE_STAY_COMPLETE,
	INN_MESSAGE_NOT_ENOUGH_COINS,
	INN_MESSAGE_GOODBYE,
	INN_MESSAGE_REST_COMPLETE,
};

#if EDITION_INTERNATIONAL
#define MESSAGE_WINDOW_ROWS 12
#else
#define MESSAGE_WINDOW_ROWS 11
#endif

s32 Inn_RoomPrice(s32 mode)
{
	struct ShopRuntime *shop = gMenuWork;
	s32					active = 0;
	s32					factor = Inn_PriceMultipliers[mode];
	s32					index;

	for (index = 0; index < shop->party_member_count; index++)
	{
		if (UnitGetFar(shop->party_member_ids[index])->hp != 0)
			active++;
	}
	return factor * active;
}

s32 Inn_CheckIn(s32 mode, s32 object_id)
{
	struct InnRuntimeState *state;
	struct ObjectRuntime   *object;
	s32						win;
	s32						amount;
	s32						message_base;

	Shop_InitializeCursorWork();
	state = gMenuWork;
	state->active = 1;
	if (mode == 5)
		state->special_active = 1;

	object = Object_GetByIdFar(object_id);
	state->resource_id = ((struct AnimationObject *)object->animation)->entries[0]->anim_id;
	win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);

	amount = Inn_RoomPrice(mode);
	UiWork_PushValueSlotFar(amount, 5);
	message_base = (s32)MsgInnWelcome;
	UiMessage_ShowAndWait(message_base);
	state->window = UiWindow_CreateFar(0, 16, MESSAGE_WINDOW_ROWS, 4, 2);
	Shop_DrawMoney();

	if (UiMessage_ShowChoice(0) != 0)
	{
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_GOODBYE - INN_MESSAGE_WELCOME));
		UiWork_FinalizeFar(state->window, 2);
	}
	else if (amount > (u32)gGameState.coins)
	{
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_NOT_ENOUGH_COINS - INN_MESSAGE_WELCOME));
		UiWork_FinalizeFar(state->window, 2);
	}
	else
	{
		UiWork_FinalizeFar(state->window, 2);
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_STAY_COMPLETE - INN_MESSAGE_WELCOME));
		UiWork_FinalizeFar(win, 2);
		Inn_PlaySleep(amount);

		object = Object_GetByIdFar(object_id);
		state->resource_id = ((struct AnimationObject *)object->animation)->entries[0]->anim_id;
		win = UiWindow_CreateWithSideObjectFar(state->resource_id, 0, 0, 0);
		UiMessage_ShowAndWait(message_base + (INN_MESSAGE_REST_COMPLETE - INN_MESSAGE_WELCOME));
	}

	UiWork_FinalizeFar(win, 2);
	Inn_Cleanup();
	return 0;
}

void Inn_PlaySleep(s32 room_price)
{
	s16					 objects[8];
	s32					 count;
	s32					 index;
	struct BtlUnit		*object;
	struct EventRuntime *state;

	count = Party_ListActiveOwnersFar(objects);
	Party_AdjustCoinsFar(-room_price);

	for (index = 0; index < count; index++)
	{
		object = UnitGetFar(objects[index]);
		if (object->hp != 0)
		{
			object->hp = object->max_hp;
			object->ep = object->max_ep;
			UnitRatioFar(objects[index]);
		}
	}

	state = (struct EventRuntime *)gEventWork;
	state->value_1c0 = 0x209;
	state->value_1c8 = 60;
	WaitFrames(20);
	Event_FinishScreenTransitionFar();
	Event_WaitTransitionFramesFar();
	AudioCommand_PlayFar(86);
	AudioCommand_WaitForStateByteClear();
	WaitFrames(10);
	Event_StartScreenTransitionFar();
	Event_WaitTransitionFramesFar();
	WaitFrames(30);
	((struct EventRuntime *)gEventWork)->value_1c8 = 16;
}
