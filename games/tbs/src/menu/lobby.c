#include "unit/import.h"
#include "unit/menu/lobby.h"
#include "unit/object/object.h"
#include "unit/object/objtable.h"
#include "unit/object/evbasic.h"
#include "unit/object/evactor.h"
#include "unit/battle/battle.h"
#include "unit/battle/turnend.h"
#include "unit/field/mapview.h"
#include "unit/game/chara.h"
#include "unit/game/flag.h"
#include "unit/game/party.h"
#include "unit/window/window.h"
#include "unit/window/wintile.h"
#include "unit/window/output.h"
#include "unit/window/save.h"
#include "unit/runtime/runtime.h"
#include "unit/runtime/sched.h"
#include "unit/runtime/heap.h"
#include "unit/runtime/serial.h"
#include "battle.h"
#include "keystate.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "field_event.h"
#include "scene.h"
#include "io_reg.h"
#include "serial_runtime.h"
#include "iwram_call.h"
#include "character.h"
#include "mem.h"
#include "call.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"
#include "scene_ids.h"
#include "game_state.h"
#include "irqlock.h"
#include "field_scene.h"
#include "frame.h"
#include "global_cells.h"

#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

const s32 LinkLobby_SlotValues[] = {
#if defined(TBS_EDITION_DE)
	PEER_TAG('C', 'S', 'G', 'G'), PEER_TAG('3', '0', '1', '2'),
	PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
	PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'G', 'S', 'C')
#elif defined(TBS_EDITION_ES)
	PEER_TAG('C', 'S', 'S', 'G'), PEER_TAG('3', '0', '1', '2'),
	PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
	PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'S', 'S', 'C')
#elif defined(TBS_EDITION_FR)
	PEER_TAG('C', 'S', 'G', 'M'), PEER_TAG('3', '0', '1', '2'),
	PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
	PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('C', 'S', 'G', 'M')
#elif defined(TBS_EDITION_IT)
	PEER_TAG('S', 'G', 'I', 'C'), PEER_TAG('0', '1', '2', '3'),
	PEER_TAG('A', 'B', 'C', '1'), PEER_TAG('C', 'D', 'E', '2'),
	PEER_TAG('E', 'F', 'G', '3'), PEER_TAG('C', 'I', 'G', 'S')
#else
#if EDITION_INTERNATIONAL
	PEER_TAG('S', 'G', 'M', 'C'),
#else
	PEER_TAG('C', 'M', 'G', 'S'),
#endif
	PEER_TAG('0', '1', '2', '3'), PEER_TAG('A', 'B', 'C', '1'),
	PEER_TAG('C', 'D', 'E', '2'), PEER_TAG('E', 'F', 'G', '3'),
	PEER_TAG('S', 'G', 'M', 'C')
#endif
};

#undef PEER_TAG

struct LinkPeer
{
	s32 values[6];
};

static s32 gLinkLobbyWaitFrames;

u8 *LinkLobby_GetEntrances(void)
{
	return (u8 *)gLinkLobbyEntrances;
}

s32 SceneData_ReturnZero(void)
{
	return 0;
}

static s32 LinkLobby_InitializeSerial(void)
{
	u32 saved;

	/* Returns no value: the ROM has the s32 epilogue (pop {r1}; bx r1) and no result. */

	IRQ_LOCK(saved);
	SerialRuntime_RemoveIrqHandlers();
	SerialRuntime_Initialize();
	IRQ_UNLOCK(saved);
}

u8 *LinkLobby_GetExits(void)
{
	return (u8 *)gLinkLobbyExits;
}

s32 LinkLobby_SelectPlacements(void)
{
	s16 v = gGameState.entrance;

	if (v == 11 || v == 9)
	{
		return (s32)gLinkLobbyBattlePlacements;
	}
	return (s32)gLinkLobbyPlacements;
}

static s32 LinkLobby_PeerSlotMatches(s32 slot)
{
	s32 id = -1;

	if ((gLinkStatus & 3) == 3)
	{
		id = (REG_SIOCNT << 26) >> 30;
		Engine_GameFlagSet(0x303);
	}
	else
	{
		Engine_GameFlagClear(0x303);
	}
	/* FAKEMATCH: a loop that runs at most once places the test after the body as the ROM does. */
	while (id >= 0 && Engine_GameFlagIsSet(0x303))
	{
		const s32 *expected = &LinkLobby_SlotValues[slot];

		if (id != 0)
			Engine_GameFlagSet(0x302);
		else
			Engine_GameFlagClear(0x302);
		{
			struct LinkPeer *row = (struct LinkPeer *)gLinkPeerSignatures[Engine_GameFlagIsSet(0x302) ^ 1];

			if (row->values[LinkLobby_SlotColumns[slot]] == *expected)
				return 1;
			break;
		}
	}
	return 0;
}

static void LinkLobby_WriteSlotValue(s32 slot)
{
	s32 *dst = (s32 *)gSerialTransfer.payload;
	s32 *src = &LinkLobby_SlotValues[slot];
	dst[LinkLobby_SlotColumns[slot]] = *src;
}

static s32 LinkLobby_PollPeerReady(void)
{
	struct EventWork *work;
	s32				  result;
	s32				  i;

	work = gEventWork;
	result = 1;
	if (Object_GetById(0)->z.fixed > 0xe00000)
	{
		Engine_GameFlagClear(0x304);
	}
	if (work->raised_trigger != 2)
	{
		LinkLobby_PeerSlotMatches(0);
		if (!Engine_GameFlagIsSet(0x303))
		{
			if (++gLinkLobbyWaitFrames > 25)
			{
				for (i = 0; i < 4; i++)
				{
					Iwram_ClearWords(&gLinkPeerSignatures[i], 20);
				}
				gLinkLobbyWaitFrames = 0;
				LinkLobby_WriteSlotValue(4);
			}
		}
		else
		{
			gLinkLobbyWaitFrames = 0;
		}
		if (gLinkLobbyWaitFrames == 0)
		{
			if (LinkLobby_PeerSlotMatches(0)
				&& (LinkLobby_PeerSlotMatches(1) || LinkLobby_PeerSlotMatches(2)))
			{
				Engine_GameFlagSet(0x201);
				if (Engine_GameFlagIsSet(0x202))
				{
					work->raised_trigger = 1;
				}
				result = 1;
			}
			else
			{
				Engine_GameFlagClear(0x201);
				result = 0;
			}
		}
		if (Engine_GameFlagIsSet(0x201) && Engine_GameFlagIsSet(0x202)
			&& !Engine_GameFlagIsSet(0x200))
		{
			work->raised_trigger = 1;
		}
	}
	if ((GameFlag_IsSet(0x201) || GameFlag_IsSet(0x202)) && !GameFlag_IsSet(0x173)
		&& !LinkLobby_PeerSlotMatches(0) && gLinkLobbyWaitFrames > 24)
	{
		work->raised_trigger = 2;
		GameFlag_Set(0x205);
		GameFlag_Clear(0x201);
		GameFlag_Clear(0x202);
		LinkLobby_WriteSlotValue(4);
	}
	if (GameFlag_IsSet(0x205))
	{
		work->raised_trigger = 2;
	}
	return result;
}

u32 gLinkLobbyCallFrames;

static s32 EV01_RAISED(void)
{
	s32 set;

	set = Engine_GameFlagIsSet(0x203);
	if (set != 0)
	{
		return set;
	}
	if (++gLinkLobbyCallFrames == 300)
	{
		gLinkLobbyCallFrames = 0;
		Engine_GameFlagClear(0x200);
	}
	set = GameFlag_IsSet(0x200);
	if (set != 0)
	{
		return set;
	}
	Engine_EventBegin();
	Engine_EventSetMessage((s32)MsgLobbyWantParticipatePlease);
	Engine_EventOpenMessage(8, 0);
	Engine_TaskWait(5);
	Engine_GameFlagSet(0x200);
	Engine_EventEnd();
}

static s32 EV02_RAISED(void)
{
	struct EventWork *work = gEventWork;

	LinkLobby_WriteSlotValue(4);
	Engine_GameFlagClear(512);
	Engine_GameFlagClear(0x203);
	Engine_EventBegin();
	work->raised_trigger = 0;
	Engine_EventSetMessage((s32)MsgLobbyLinkDisconnected);
	Engine_EventOpenMessage(8, 0);
	Engine_GameFlagClear(0x205);
	Engine_EventEnd();
}

static s32 LinkLobby_ReceivePartyRecords(void)
{
	/* FAKEMATCH: gSerialReceivedSize is volatile (serial_runtime.h), so every read here is a load of its own. Measured on tbs-en: without the qualifier differ 69, 186 instructions for 189. */
	u16 buf[24];
	u32 size;
	s32 heap;
	s32 result;
	s32 slot;
	s32 timeout;
	s32 tries;
	u8 *rec;
	s32 ret;
	s32 n;
	s32 i;

	size = 0x154;
	heap = (s32)Runtime_BumpAllocateAlternatePool(size);
	result = 0;
	timeout = 900;
	slot = 0;
	goto next;
wait1:
	if (gSerialReceivedSize > size)
	{
		result = -1;
		goto done;
	}
	Engine_TaskWait(1);
	if (--timeout < 0 || (gLinkStatus & 3) != 3)
	{
		if (++tries > 24)
		{
			result = -1;
			goto done;
		}
	}
test1:
	if (SerialRuntime_GetActiveTransfers() != 0)
	{
		goto wait1;
	}
	if (gSerialReceivedSize != size)
	{
		result = -1;
		goto done;
	}
	if (rec[0x12a] != 0)
	{
		result++;
	}
	Engine_TaskWait(2);
	Call2(Ui_AdjustValueWithoutLimit, (s32)MsgEnemyLabel, (s32)buf);
	i = 0;
	if (buf[i] != 0)
	{
		do
		{
			i++;
			if (i > LOBBY_PREFIX_LIMIT)
			{
				break;
			}
		} while (buf[i] != 0);
	}
#if defined(TBS_EDITION_DE) || defined(TBS_EDITION_ES) || defined(TBS_EDITION_FR) \
	|| defined(TBS_EDITION_IT)
	buf[i] = ' ';
	n = i + 1;
#else
	n = i;
#endif
	for (i = 14; i >= n; i--)
	{
		rec[i] = rec[i - n];
	}
	for (i = 0; i < n; i++)
	{
		rec[i] = buf[i];
	}
	rec[14] = 0;
	slot++;
next:
	if (slot > 2)
	{
		goto second;
	}
	rec = UnitGet(slot + 128);
	tries = 0;
	if ((ret = SerialRuntime_BeginTransferB(rec)) == -1)
	{
		goto failed;
	}
	goto test1;
wait2:
	ret = gSerialReceivedSize;
	if (ret > 0x140)
	{
		result = -1;
		goto done;
	}
	Engine_TaskWait(1);
	if (--timeout < 0 || (gLinkStatus & 3) != 3)
	{
		if (++tries > 24)
		{
			result = -1;
			goto done;
		}
	}
test2:
	if (SerialRuntime_GetActiveTransfers() != 0)
	{
		goto wait2;
	}
	if (gSerialReceivedSize != 0x140)
	{
		result = -1;
	}
	else
	{
		Engine_TaskWait(2);
	}
	goto done;
second:
	Runtime_BumpFree(heap);
	size = 0x140;
	heap = (s32)Runtime_BumpAllocateAlternatePool(size);
	tries = 0;
	if ((ret = SerialRuntime_BeginTransferB(Trade_GetOfferState(1))) != -1)
	{
		goto test2;
	}
failed:
	result = ret;
done:
	Runtime_BumpFree(heap);
	return result;
}

static s32 SceneData_CopyUpToThreeEntries(u16 *dest)
{
	s32 cnt = Party_CountActiveOwners();
	if (cnt > 3)
		cnt = 3;
	if (cnt > 0)
	{
		s16		 *p = (s16 *)&gGameState;
		const u8 *src;
		s32		  n;
		p += 252;
		src = (const u8 *)p;
		n = cnt;
		do
		{
			u8 c = *src++;
			if (dest != 0)
			{
				*dest = c;
				dest++;
			}
			n--;
		} while (n != 0);
	}
	if (dest != 0)
		*dest = 0x00ff;
	return cnt;
}

struct LinkOffer
{
	u8 item;
	u8 count;
	u8 owner;
	u8 flags;
};

struct LinkOfferList
{
	struct LinkOffer entries[64];
	s32				 count;
};

static s32 LinkLobby_SendPartyRecords(void)
{
	s32					  result;
	u8					 *table;
	u16					  owners[8];
	u8					  slots[8];
	s32					  ret;
	u8					 *heap;
	s32					  count;
	s32					  timeout;
	s32					  tries;
	s32					  i;
	s32					  j;
	s32					  k;
	u32					  size;
	struct LinkOfferList *list;

	size = 0x154;
	heap = Runtime_BumpAllocateAlternatePool(size);
	result = 0;
	timeout = 600;
	count = SceneData_CopyUpToThreeEntries(owners);
	table = slots;
	for (i = 7; i >= 0; i--)
	{
		table[i] = 0;
	}
	for (i = 0; i < count; i++)
	{
		/* FAKEMATCH: a one-pass boundary keeps the send failure inline and gives the record copy's argument order; without it loop.c moves the failure out of the loop. */
		do
		{
			Iwram_CopyWords(heap, UnitGet(owners[i]), size);
			heap[0x12a] = 2;
			table[owners[i]] = i - 128;
			ret = SerialRuntime_BeginTransferA(heap, size);
			tries = 0;
			if (ret == -1)
			{
				result = ret;
				goto done;
			}
		} while (0);
		while (SerialRuntime_GetActiveTransfers() != 0)
		{
			Engine_TaskWait(1);
			if (--timeout < 0 || (gLinkStatus & 3) != 3)
			{
				/* FAKEMATCH: a one-pass boundary keeps this failure inline, so its -1 is reloaded here and jump2 merges it into the offer's; moved out of the loop, it shifts every later reload register. */
				do
				{
					if (++tries > 24)
					{
						result = -1;
						goto done;
					}
				} while (0);
			}
		}
		Engine_TaskWait(2);
	}
	goto next;
wait:
	Engine_TaskWait(1);
	if (--timeout < 0 || (gLinkStatus & 3) != 3)
	{
		if (++tries > 24)
		{
			result = -1;
			goto done;
		}
	}
test:
	if (SerialRuntime_GetActiveTransfers() != 0)
	{
		goto wait;
	}
	Engine_TaskWait(2);
	i++;
next:
	if (i <= 2)
	{
		heap[0x12a] = tries = 0;
		if ((ret = SerialRuntime_BeginTransferA(heap, size)) != -1)
		{
			goto test;
		}
		result = ret;
		goto done;
	}
	Runtime_BumpFree(heap);
	size = 0x140;
	heap = Runtime_BumpAllocateAlternatePool(size);
	/* FAKEMATCH: a one-pass boundary gives the offer copy's argument order and keeps the list base's copy first after the call. */
	do
	{
		Iwram_CopyWords(heap, Trade_GetOfferState(0), size);
	} while (0);
	{
		s32 timeout;
		s32 tries;

		list = (struct LinkOfferList *)(heap + 8);
		tries = 0;
		timeout = 600;
		for (j = 0; j < list->count; j++)
		{
			list->entries[j].owner = slots[list->entries[j].owner];
			if (list->entries[j].owner == 0)
			{
				for (k = j; k < list->count - 1; k++)
				{
					list->entries[k] = list->entries[k + 1];
				}
				list->count--;
				j--;
			}
		}
		if ((ret = SerialRuntime_BeginTransferA(heap, size)) == -1)
		{
			result = ret;
			goto done;
		}
		goto test3;
	wait3:
		Engine_TaskWait(1);
		if (--timeout < 0 || (gLinkStatus & 3) != 3)
		{
			if (++tries > 24)
			{
				result = -1;
				goto done;
			}
		}
	test3:
		if (SerialRuntime_GetActiveTransfers() != 0)
		{
			goto wait3;
		}
		Engine_TaskWait(1);
		Engine_TaskWait(2);
	}
done:
	Runtime_BumpFree(heap);
	return result;
}

static s32 LinkLobby_ExchangePartyRecords(void)
{
	s32 result;
	s32 first;
	s32 flag;

	/* FAKEMATCH: separate initialization from the following flag query. */
	do
	{
		result = 0;
	} while (0);
	flag = Engine_GameFlagIsSet(0x302);
	gLinkExchangeState = result;
	if (!flag)
	{
		Engine_TaskWait(5);
		result = LinkLobby_SendPartyRecords();
		if (result < 0)
			goto fail;
		Engine_TaskWait(5);
		first = result = LinkLobby_ReceivePartyRecords();
		if (result < 0)
			goto check;
	}
	else
	{
		first = result = LinkLobby_ReceivePartyRecords();
		if (result < 0)
			goto fail;
		Engine_TaskWait(10);
		result = LinkLobby_SendPartyRecords();
		if (result < 0)
			goto fail;
	}
	GameFlag_SetByte(0x3f0, first);
	result = first;
check:
	if (first < 0)
	{
	fail:
		SerialRuntime_ResetTransferState();
	}
	return result;
}

static s32 EV10_TOUCH(void)
{
	struct EventWork *work;
	s32				  failed;
	s32				  window;
	s32				  cnt;
	s32				  stop;
	u8				 *buf;
	u8				 *src;
	u8				 *dst;
	u16				 *tbl;
	u32				  i;

	failed = 0;
	window = 0;
	work = gEventWork;
	cnt = 0;
	if (Engine_GameFlagIsSet(0x173))
	{
		Engine_EventBegin();
	}
	else
	{
		/* These exits return no value from the s32 handler. */
		if (!Engine_GameFlagIsSet(0x200))
			return;
		if (Engine_GameFlagIsSet(0x205))
			return;
		Engine_EventBegin();
		Engine_GameFlagSet(0x203);
		LinkLobby_WriteSlotValue(2);
		if (!LinkLobby_PeerSlotMatches(2))
			window = UiText_OpenMessageWindow((s32)MsgLobbyAwaitingOpponent, 5, 4, 1);
		while (!LinkLobby_PeerSlotMatches(2))
		{
			Engine_TaskWait(1);
			stop = 0;
			if (!Engine_GameFlagIsSet(0x201))
				stop = 1;
			if (Engine_GameFlagIsSet(0x205))
				stop = 1;
			if (!LinkLobby_PeerSlotMatches(2) && !LinkLobby_PeerSlotMatches(1))
			{
				if (++cnt > 25)
					stop = 1;
			}
			else
			{
				cnt = 0;
			}
			if (stop)
			{
				work->raised_trigger = 2;
				Engine_GameFlagSet(0x205);
				Engine_GameFlagClear(0x201);
				Engine_GameFlagClear(0x202);
				LinkLobby_WriteSlotValue(4);
				failed = 1;
				Engine_GameFlagClear(0x200);
				break;
			}
		}
		if (window)
			UiWork_Finalize(window, 1);
		Engine_TaskWait(5);
	}
	if (!failed)
	{
		buf = Runtime_AllocateBlock(54, 0x7c8);
		Engine_TaskRemoveCallback(LinkLobby_PollPeerReady);
		Map_ClearLayerEntryFlag(5);
		Engine_TaskWait(8);
		Map_SetLayerEntryFlag(5);
		if (Engine_GameFlagIsSet(0x173))
		{
			Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
			Engine_EventSetMessage((s32)MsgLobbyGoodLuck);
			Engine_EventOpenMessage(8, 0);
			Engine_TaskWait(45);
			Actor_SetSpeed(0, 0x10000, 0x8000);
			Engine_ActorWalkTo(0, 216, 184);
			Engine_ActorWaitForMove(0);
			Engine_ActorWalkTo(0, 216, 168);
			Engine_ActorWaitForMove(0);
		}
		else
		{
			Actor_SetSpeed(0, 0x10000, 0x8000);
			Engine_ActorWalkTo(0, 216, 200);
			Engine_ActorWaitForMove(0);
			Actor_SetSpeed(0, 0x1999, 0xccc);
			Engine_ActorWalkTo(0, 216, 168);
			if (LinkLobby_ExchangePartyRecords() < 0)
			{
				Actor_SetSpeed(0, 0x10000, 0x8000);
				Engine_ActorWalkTo(0, 216, 200);
				Map_ClearLayerEntryFlag(5);
				Engine_TaskWait(8);
				Map_SetLayerEntryFlag(5);
				Engine_ActorWaitForMove(0);
				Runtime_ReleaseHeapBlock(54);
				LinkLobby_WriteSlotValue(0);
				LinkLobby_WriteSlotValue(4);
				Engine_TaskAddCallback(LinkLobby_PollPeerReady, 0xc80);
				Scheduler_SetCallbackMask(LinkLobby_PollPeerReady, 1);
				Engine_GameFlagClear(0x201);
				Engine_GameFlagClear(0x202);
				Engine_GameFlagClear(0x303);
				Engine_GameFlagClear(0x203);
				Engine_GameFlagClear(0x200);
				work->raised_trigger = 2;
				goto done;
			}
			Actor_SetSpeed(0, 0x8000, 0x4000);
			Engine_ActorWaitForMove(0);
		}
		if (Engine_GameFlagIsSet(0x173))
		{
			Party_SetFields1ceAnd1d0((s32)&SceneId_LinkLobby, 8);
			Event_SetDefeatDestination((s32)&SceneId_LinkLobby, 9);
		}
		else
		{
			Party_SetFields1ceAnd1d0((s32)&SceneId_LinkLobby, 10);
			Event_SetDefeatDestination((s32)&SceneId_LinkLobby, 11);
		}
		Battle_SetStart(4);
		BattleFx_SetWeightedResult(1, 1);
		tbl = (u16 *)gSerialTransfer.payload;
		tbl[0] = 0x45;
		tbl[1] = 0x58;
		tbl[2] = 0x45;
		tbl[3] = 0x43;
		i = 0;
		src = buf;
		dst = (u8 *)gMapCellBuffer + 0x8000;
		do
		{
			i++;
			*dst++ = *src++;
		} while (i <= 0x7c7);
		Runtime_ReleaseHeapBlock(54);
	}
done:
	Engine_EventEnd();
}

static s32 EV08_TALK(void)
{
	/* Returns no value: r0 is whatever the last call left. */

	s32 answer;
	s32 msg;
	s32 line;

	msg = (s32)MsgLobbyPleaseSpeakWhen;
	Engine_EventBegin();
	Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
	if (LinkLobby_PeerSlotMatches(0) == 0)
	{
		Engine_TaskWait(1);
	}
	if (LinkLobby_PeerSlotMatches(0) == 0)
	{
		LinkLobby_WriteSlotValue(5);
		LinkLobby_InitializeSerial();
		if (!GameFlag_IsSet(0x173))
		{
			Engine_EventSetMessage(msg + 5);
			Engine_EventOpenMessage(8, 0);
			answer = Engine_EventChooseYesNo(0, 0);
			if (answer == 0)
			{
				GameFlag_SetByte(1000, 0);
				Engine_GameFlagSet(0x173);
				Engine_GameFlagClear(0x172);
				Engine_GameFlagClear(0x16c);
				Engine_GameFlagSet(0x202);
				line = msg + 7;
				gGameState.link_tallies[3] = answer;
				goto message;
			}
			Engine_GameFlagClear(0x173);
			Engine_GameFlagSet(0x16c);
			LinkLobby_WriteSlotValue(0);
			line = msg + 6;
			goto message;
		}
		else
		{
			line = msg + 3;
			goto message;
		}
	}
	else
	{
		if (GameFlag_IsSet(0x173))
		{
			LinkLobby_WriteSlotValue(0);
			Engine_EventSetMessage((s32)MsgLobbyOpponentArrived);
			Engine_EventOpenMessage(8, 0);
			Engine_GameFlagClear(0x202);
			Engine_GameFlagClear(0x173);
		}
		if (Engine_GameFlagIsSet(0x202))
		{
			line = msg + 3;
			/* FAKEMATCH: the first branch's answers jump into this block's message tail, and an accepted challenge back into its opening, which keeps the reference's single copy of each. */
		message:
			Engine_EventSetMessage(line);
		open_message:
			Engine_EventOpenMessage(8, 0);
			goto done;
		}
		else if (!Engine_GameFlagIsSet(0x201) && !GameFlag_IsSet(0x300))
		{
			Engine_EventSetMessage(msg);
			Engine_EventOpenMessage(8, 0);
			Engine_GameFlagSet(0x300);
		}
		else
		{
			Engine_GameFlagSet(0x300);
			if (Engine_GameFlagIsSet(0x201))
			{
				Engine_EventSetMessage(msg + 2);
			}
			else
			{
				Engine_EventSetMessage(msg + 1);
			}
			Engine_EventOpenMessage(8, 0);
			if (Engine_EventChooseYesNo(0, 0) == 0)
			{
				if (LinkLobby_PeerSlotMatches(0))
				{
					Engine_GameFlagSet(0x16c);
					Engine_GameFlagSet(0x172);
					if (Engine_GameFlagIsSet(0x201))
					{
						Engine_EventSetMessage(msg + 3);
					}
					else
					{
						Engine_EventSetMessage(msg + 4);
					}
					LinkLobby_WriteSlotValue(1);
					Engine_GameFlagSet(0x202);
					goto open_message;
				}
				else
				{
					Engine_GameFlagSet(0x205);
				}
			}
			else
			{
				Engine_EventSetMessage(msg);
				Engine_EventOpenMessage(8, 0);
			}
		}
	}
done:
	Engine_EventEnd();
}

static s32 EV09_TALK(s32 actor)
{
	u32	 facing = Object_GetById(0)->facing;
	u16 *count;
	s32	 base;

	Engine_EventBegin();
	Engine_ActorFaceActor(actor, gGameState.selected_actor, 0);
	if ((facing - 0xa001) <= 0x3ffe)
	{
		base = (s32)MsgLobbyLooksLikeFirst;
		count = &gGameState.link_tallies[4];
		if (*count == 0)
		{
			Engine_EventSetMessage((s32)MsgLobbyTellTotalWins);
			Engine_EventShowMessage(actor, 0);
			goto done;
		}
	}
	else
	{
		base = (s32)MsgLobbyLooksLikeFirstTime;
		count = &gGameState.link_tallies[7];
		if (*count == 0)
			goto none;
	}
	UiWork_ClearValueNameTables();
	UiText_DrawQuantity(*count, 5);
	Engine_EventSetMessage(base + 1);
	Engine_EventShowMessage(actor, 0);
	goto done;
none:
	Engine_EventSetMessage((s32)MsgLobbyTellConsecutiveWins);
	Engine_EventShowMessage(actor, 0);
done:;
}

static s32 EV15_TALK(s32 id)
{
	s32 window;
	s32 choice;
	s32 shown;

	Engine_EventBegin();
	Engine_ActorFaceActor(id, gGameState.selected_actor, 0);
	Engine_EventSetMessage((s32)MsgLobbyChooseParameters);
	Engine_EventShowMessage(id, 0);
	window = Engine_DebugCreateWindow(0, 0, 6, 4, 2);
	choice = 0;
	shown = -1;
	for (;;)
	{
		if (choice != shown)
		{
			RenderOutput_PrepareForRedraw(window);
			UiText_DrawNumber(choice, 3, window, 0, 0);
			shown = choice;
		}
		if (gKeysRepeat & KEY_LEFT)
		{
			choice--;
		}
		if (gKeysRepeat & KEY_RIGHT)
		{
			choice++;
		}
		if (choice < 0)
		{
			choice = 0;
		}
		if (gKeyState & KEY_A)
		{
			break;
		}
		if (gKeyState & KEY_B)
		{
			choice = -1;
			break;
		}
		Engine_TaskWait(1);
	}
	UiWork_Finalize(window, 1);
	{
		s32 ok;
		s32 message;

		if (choice >= 0)
		{
			ok = DebugParty_LoadPreset(choice);
		}
		else
		{
			message = (s32)MsgLobbyTease;
			goto show;
		}
		/* FAKEMATCH: the refusal jumps into the 0x98b branch so the two share one message call, as the reference lays them out. */
		if (ok != 0)
		{
			message = (s32)MsgLobbyNoParameterAsk;
		show:
			Engine_EventSetMessage(message);
			Engine_EventShowMessage(9, 0);
		}
		else
		{
			Engine_EventSetMessage((s32)MsgLobbySettingsMadeRest);
			Engine_EventShowMessage(9, 0);
		}
	}
	Engine_TaskWait(10);
	Engine_EventEnd();
}

static s32 EV10_TALK(s32 arg0)
{
	Engine_EventBegin();
	Engine_SanctumOpen(arg0);
	Engine_EventEnd();
}

static s32 LinkLobby_PartyContains(s32 id)
{
	s32 count;
	s32 max;
	s32 i;

	count = Party_CountActiveOwners();
	max = 3;
	if (Engine_GameFlagIsSet(0x172) == 0)
	{
		max = 4;
	}
	if (count > max)
	{
		count = max;
	}
	for (i = 0; i < count; i++)
	{
		if (gGameState.active_owners[i] == 0xff)
		{
			return 0;
		}
		if (gGameState.active_owners[i] == id)
		{
			return 1;
		}
	}
	return 0;
}

static void State_NoOp(void)
{
}

static s32 LinkLobby_TalkByProgress(s32 actor)
{
	s32 base = (s32)MsgLobbyCantWaitLets;
	s32 lead = LinkLobby_PartyContains(0);
	s32 own = LinkLobby_PartyContains(actor);

	Engine_EventBegin();
	Engine_ActorFaceActor(actor, gGameState.selected_actor, 0);
	if (Engine_GameFlagIsSet(0x304))
	{

		GameFlag_IsSet(0x2f0);
		base = Engine_GameFlagIsSet(actor + 0x2f0);
		if (Engine_GameFlagIsSet(0x305))
		{
			if (base)
				base = (s32)MsgLobbyWonToldCountOn;
			else
				base = (s32)MsgLobbyWonWithoutWell;
		}
		else
		{
			if (base)
				base = (s32)MsgLobbyNotFaultLost;
			else
				base = (s32)MsgLobbyKnewLostBecause;
		}
	}
	else if (lead != 0)
	{
		if (own == 0)
			base = (s32)MsgLobbyGoingFightAlone;
	}
	else
	{
		base = (s32)MsgLobbyTryingGetAway;
	}
	Engine_EventSetMessage(base + actor - 1);
	Engine_EventShowMessage(actor, 0);
	Engine_EventEnd();
}

static s32 EV01_TOUCH(void)
{
	SerialRuntime_RemoveIrqHandlers();
	Sound_LoadPresetParameters(2);
	Event_SetDestination((s32)&SceneId_Clear, 1);
}

/* The handlers below are s32 and return no value: r0 is whatever Engine_EventEnd left. */

static s32 LinkLobby_TalkToAttendant(s32 actor)
{
	s32 message;
	s32 step = 0;

	Engine_EventBegin();
	switch (actor)
	{
	case 12:
		message = (s32)MsgLobbyTravelingWarrior;
		break;
	case 13:
		message = (s32)MsgLobbyThreeAlliesFight;
		break;
	case 14:
	default:
		message = (s32)MsgLobbyBattleArenaOld;
		break;
	}
	Engine_ActorFaceActor(actor, gGameState.selected_actor, 0);
	if (Engine_GameFlagIsSet(0x304))
		step = 2 - (Engine_GameFlagIsSet(0x305) != 0);
	Engine_EventSetMessage(message + step);
	Engine_EventOpenMessage(actor, 0);
	Engine_EventEnd();
}

static s32 EV16_TALK(s32 actor)
{
	s32 message;

	Engine_EventBegin();
	Engine_ActorFaceActor(actor, gGameState.selected_actor, 0);
	if (!GameFlag_IsSet(0x204))
	{
		if (Party_CountActiveOwners() <= 3)
			message = (s32)MsgLobbyThreeAlliesFightLinkedFinals;
		else
			message = (s32)MsgLobbyThreeAlliesFightLinked;
		Engine_GameFlagSet(0x204);
	}
	else
	{
		message = (s32)MsgLobbyChangeOrderParty;
		Engine_GameFlagClear(0x204);
	}
	Engine_EventSetMessage(message);
	Engine_EventOpenMessage(actor, 0);
	Engine_EventEnd();
}

static s32 EV22_TALK(s32 id)
{
	struct GameState *gs;

	Engine_EventBegin();
	gs = &gGameState;
	Engine_ActorFaceActor(id, gs->selected_actor, 0);
	if (gs->link_tallies[2] != 0)
	{
		UiText_DrawQuantity(gs->link_tallies[2], 5);
		Engine_EventSetMessage((s32)MsgLobbyWonNumberConsecutive);
	}
	else
	{
		Engine_EventSetMessage((s32)MsgLobbyCantWaitSee);
	}
	Engine_EventOpenMessage(id, 0);
	Engine_EventEnd();
}

u8 *LinkLobby_GetEvents(void)
{
	return (u8 *)gLinkLobbyEvents;
}

static s32 LinkLobby_SaveBattleResults(void)
{
	s32 window;

	Engine_AudioPlayCue(85);
	window = UiText_OpenMessageWindow((s32)MsgLobbySavingBattleResults, 5, 4, 1);
	while (UiWork_IsComplete() == 0)
		Engine_TaskWait(1);
	SaveState_ProcessSelectedSlot();
	UiWork_Finalize(window, 1);
	Engine_TaskWait(1);
	window = UiText_OpenMessageWindow((s32)MsgLobbyBattleResultsSaved, 5, 4, 1);
	while (UiWork_IsComplete() == 0)
		Engine_TaskWait(1);
	UiWork_Finalize(window, 1);
}

static s32 LinkLobby_SaveMonsterBattleResults(void)
{
	s32 window;

	Engine_AudioPlayCue(85);
	window = UiText_OpenMessageWindow((s32)MsgLobbySavingMonsterBattle, 5, 4, 1);
	while (UiWork_IsComplete() == 0)
		Engine_TaskWait(1);
	SaveState_ProcessSelectedSlot();
	UiWork_Finalize(window, 1);
	Engine_TaskWait(1);
	window = UiText_OpenMessageWindow((s32)MsgLobbyMonsterBattleResults, 5, 4, 1);
	while (UiWork_IsComplete() == 0)
		Engine_TaskWait(1);
	UiWork_Finalize(window, 1);
}

static s32 LinkLobby_DrawThreeDigitValue(s32 value)
{
	s32 digit;

	if (value > 999)
		value = 999;
	for (digit = 0; digit <= 2; digit++)
	{
		Engine_MapCopyCellsTo(27, value % 10, 16 - digit, 8, 1, 1);
		value = value / 10;
	}
	Engine_MapRedraw();
}

s32 LinkLobby_RunRoundResult(void)
{
	s32				  i;
	u32				  score;
	struct EventWork *ew;
	void (*callback)(void);

	gLinkLobbyCallFrames = 0;
	gLinkLobbyWaitFrames = 0;
	gEventWork->start_transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	Sound_LoadPresetParameters(2);
	LinkLobby_DrawThreeDigitValue(gGameState.link_tallies[6]);
	Map_CopyCellAttributes(11, 11, 1, 1, 13, 10);
	LinkLobby_WriteSlotValue(4);
	Engine_TaskWait(1);
	Map_SetLayerEntryFlag(5);

	{
		u16 *name = (u16 *)gSerialTransfer.payload;

		name[4] = 'T';
		name[5] = 'A';
		name[6] = 'L';
		name[7] = 'K';
	}
	for (i = 0; i < 8; i++)
	{
		Engine_GameFlagClear(0x2f0 + i);
		if (LinkLobby_PartyContains(i))
		{
			Engine_GameFlagSet(0x2f0 + i);
		}
	}
	if (gGameState.entrance == 8)
	{
		s32 wins;
		s32 msg;
		s32 v;

		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		LinkLobby_WriteSlotValue(5);
		{
			struct GameState *state = &gGameState;

			state->link_tallies[0]++;
			state->link_tallies[3]++;
		}
		wins = (s8)GameFlag_GetByte(0x3f8);
		msg = wins * 2 + 2;
		if (msg > 14)
		{
			msg = 14;
		}
		v = Value1(GameFlag_GetByte, 1000);
		if (v == 2)
		{
			GameFlag_SetByte(1000, 0);
			wins++;
			msg++;
		}
		else
		{
			Call2(GameFlag_SetByte, 1000, v + 1);
		}
		{
			struct GameState *state = &gGameState;

			Engine_ActorFaceActor(8, state->selected_actor, 0);
			Engine_EventSetMessage((s32)MsgLobbyNotBadNextMonster + msg);
			Engine_EventOpenMessage(8, 0);
			if (Engine_EventChooseYesNo(0, 0) == 0)
			{
				if (wins > 90)
				{
					wins = 90;
				}
				GameFlag_SetByte(0x3f8, wins);
			}
			else
			{

				Engine_GameFlagClear(0x173);
				GameFlag_SetByte(0x3f8, -1);
				UiText_DrawQuantity(state->link_tallies[3], 5);
				score = state->link_tallies[3];
				if (state->link_tallies[2] < score)
				{
					state->link_tallies[2] = score;
					Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleBroke);
					Engine_EventOpenMessage(8, 0);
					LinkLobby_SaveMonsterBattleResults();
				}
				else
				{
					Engine_EventSetMessage((s32)MsgLobbyWonNumberBattle);
					Engine_EventOpenMessage(8, 0);
				}
				LinkLobby_WriteSlotValue(0);
			}
		}
		Engine_EventEnd();
	}
	else if (gGameState.entrance == 9)
	{

		gGameState.link_tallies[1]++;
		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		LinkLobby_WriteSlotValue(5);
		Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
		/* FAKEMATCH: one scalar holds the row address, then its score. */
		score = (u32)&gGameState.link_tallies[3];
		UiText_DrawQuantity(*(u16 *)score, 5);
		score = *(u16 *)score;
		if (gGameState.link_tallies[2] < score)
		{
			gGameState.link_tallies[2] = score;
			Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleBroke);
			Engine_EventOpenMessage(8, 0);
			LinkLobby_SaveMonsterBattleResults();
		}
		else
		{
			Engine_EventSetMessage((s32)MsgLobbyWonNumberBattleTold);
			Engine_EventOpenMessage(8, 0);
		}
		gGameState.link_tallies[3] = 0;
		Engine_GameFlagClear(0x173);
		GameFlag_SetByte(0x3f8, -1);
		LinkLobby_WriteSlotValue(0);
		Engine_EventEnd();
	}
	else if (gGameState.entrance == 10)
	{
		s32 v;

		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		LinkLobby_WriteSlotValue(0);
		LinkLobby_WriteSlotValue(4);
		if (GameFlag_IsSet(1000))
		{
			ew = gEventWork;
			Engine_GameFlagClear(1000);
			ew->raised_trigger = 2;
			Engine_GameFlagClear(0x304);
			Engine_TaskWait(20);
			LinkLobby_InitializeSerial();
			LinkLobby_WriteSlotValue(0);
			LinkLobby_WriteSlotValue(4);
		}
		else
		{
			gGameState.link_tallies[4]++;
			v = gGameState.link_tallies[7] + 1;
			gGameState.link_tallies[7] = v;
			/* FAKEMATCH: reuse the other score branches' address slot. */
			score = (u32)&gGameState.link_tallies[6];
			if (*(u16 *)score < (u16)v)
			{
				*(u16 *)score = v;
			}
			LinkLobby_DrawThreeDigitValue(*(u16 *)score);
			LinkLobby_SaveBattleResults();
			Engine_GameFlagSet(0x304);
			Engine_GameFlagSet(0x305);
		}
		Engine_EventEnd();
	}
	else if (gGameState.entrance == 11)
	{
		s32 v;

		Engine_EventBegin();
		Engine_EventOpenScreen();
		Engine_EventWaitForScreen();
		LinkLobby_WriteSlotValue(0);
		LinkLobby_WriteSlotValue(4);
		if ((v = Engine_GameFlagIsSet(0x173)) == 0)
		{
			gGameState.link_tallies[5]++;
			gGameState.link_tallies[7] = v;
			LinkLobby_SaveBattleResults();
		}
		Engine_GameFlagSet(0x304);
		Engine_GameFlagClear(0x305);
		Engine_EventEnd();
	}
	else
	{
		SerialRuntime_Initialize();
		Engine_GameFlagClear(0x172);
		GameFlag_SetByte(0x3f8, -1);
		if (gGameState.saved_options != 0)
		{
			Engine_EventBegin();
			Engine_EventOpenScreen();
			Engine_EventWaitForScreen();
			Engine_ActorFaceActor(8, gGameState.selected_actor, 0);
			Engine_EventSetMessage((s32)MsgLobbyNoteCantUse);
			Engine_EventShowMessage(8, 0);
			Engine_EventEnd();
		}
		gGameState.saved_options = 0;
		gOptionMirror = 0;
		LinkLobby_WriteSlotValue(0);
		LinkLobby_WriteSlotValue(4);
	}
	callback = LinkLobby_PollPeerReady;
	Engine_TaskAddCallback(callback, 0xc80);
	Scheduler_SetCallbackMask(callback, 1);
	if (gGameState.entrance != 8 || !Engine_GameFlagIsSet(0x173))
	{
		Owner_RefreshActiveRatios(1);
		BattlePlacement_UpdateTimedEntriesTwentyTimes();
	}
	return 0;
}

u8 LinkLobby_SlotColumns[] = {
	0, 1, 1, 1, 1, 0, 0, 0,
};

struct SceneEntrance gLinkLobbyEntrances[] = {
	{ 11, CONDITION_ALWAYS, 128, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
	{ 10, CONDITION_ALWAYS, 352, 0, 148, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
	{ 8, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
	{ 9, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
	{ 1, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
	{ 0, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

u32 gLinkLobbyExits[] = {
	SCENE_EXITS(511),
};

struct ScenePlacement gLinkLobbyPlacements[] = {
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(304), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(272), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(64), 0, PIXELS(92), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(40), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(256), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(144), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x99, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(128), 0, PIXELS(164), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(64), 0, PIXELS(136), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(88), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(288), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ GERALD, IF_FLAG_SET(0x1), ACTOR_STAND,
		PIXELS(352), 0, PIXELS(108), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ IWAN, IF_FLAG_SET(0x2), ACTOR_STAND,
		PIXELS(328), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ MEARI, IF_FLAG_SET(0x3), ACTOR_STAND,
		PIXELS(376), 0, PIXELS(136), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ JASMINE, IF_FLAG_SET(0x5), ACTOR_STAND,
		PIXELS(376), 0, PIXELS(136), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct ScenePlacement gLinkLobbyBattlePlacements[] = {
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(152), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(280), 0, PIXELS(204), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x79, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(304), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(192), 0, PIXELS(272), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(64), 0, PIXELS(92), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(40), 0, PIXELS(120), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(256), 0, PIXELS(384), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6b, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(104), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x66, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x98, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(352), 0, PIXELS(108), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x96, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(352), 0, PIXELS(156), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x99, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(376), 0, PIXELS(136), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x73, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(64), 0, PIXELS(136), FACING_NORTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x67, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(328), 0, PIXELS(88), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ 0x74, CONDITION_ALWAYS, ACTOR_STAND,
		PIXELS(72), 0, PIXELS(288), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ GERALD, IF_FLAG_SET(0x1), ACTOR_STAND,
		PIXELS(104), 0, PIXELS(144), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ IWAN, IF_FLAG_SET(0x2), ACTOR_STAND,
		PIXELS(152), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
	{ MEARI, IF_FLAG_SET(0x3), ACTOR_STAND,
		PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ JASMINE, IF_FLAG_SET(0x5), ACTOR_STAND,
		PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
	{ SCENE_TABLE_END },
};

struct SceneEvent gLinkLobbyEvents[] = {
	{ EVENT_TALK, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress) },
	{ EVENT_TALK, 2, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress) },
	{ EVENT_TALK, 3, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress) },
	{ EVENT_TALK, 5, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress) },
	{ EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK) },
	{ EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
	{ EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
	{ EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFieldWelcomeToBattleArenaWherePeople) },
	{ EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkToAttendant) },
	{ EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkToAttendant) },
	{ EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkToAttendant) },
	{ EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
	{ EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
	{ EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgFieldInTheLinkedFinalsYouCant) },
	{ EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgFieldToUseSummonQuicklyDjinnMust) },
	{ EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgFieldDoYouPutDjinnOnStandby) },
	{ EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgFieldRemoveDjinnYouRarelyUseSo) },
	{ EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgFieldMonsterBattleRulesSayYouAll) },
	{ EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
	{ EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgFieldAllTheWarriorsAreImpressiveSo) },
	{ EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgFieldICouldEnterButIdJust) },
	{ EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
	{ EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_TOUCH) },
	{ EVENT_RAISED, 1, CONDITION_ALWAYS, EVENT_SCRIPT(EV01_RAISED) },
	{ EVENT_RAISED, 2, CONDITION_ALWAYS, EVENT_SCRIPT(EV02_RAISED) },
	{ SCENE_EVENTS_END },
};
