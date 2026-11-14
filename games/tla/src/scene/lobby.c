/// @file
/// The Battle Arena's lobby, The Broken Seal's arena reworked: two players
/// link, trade their parties' records and fight, or fight the monster record;
/// a visitor takes song requests.

#include "battle/battle.h"
#include "field/field_work.h"
#include "sound_driver_symbols.h"
#include "flags.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "system/serial.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "actor.h"
#include "edition.h"
#include "owner_state.h"
#include "system/main_routine_work.h"
#include "system/heap_release.h"
#include "system/task.h"
#include "system/input.h"
#include "scene_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

void Serial_RemoveIrqHandlers(void);
void Serial_Initialize(void);
void Event_Start(void);
void Event_End(void);
void Sanctum_Run(s32 value);

void LinkLobby_InitializeSerial(void) {
  volatile u16 *ime = &REG_IME;
  u32 saved = *ime;

  *ime = (u16)(u32)ime;
  Serial_RemoveIrqHandlers();
  Serial_Initialize();
  *ime = saved;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

struct LinkPeer {
  s32 values[6];
};

extern struct LinkPeer gLinkPeerSignatures[2];

s32 LinkLobby_PeerSlotMatches(s32 slot) {
  s32 id = -1;

  if ((gLinkStatus & 3) == 3) {
    id = (REG_SIOCNT << 26) >> 30;
    Event_SetFlag(FLAG_AREA_TEMP_03);
  } else {
    Event_ClearFlag(FLAG_AREA_TEMP_03);
  }
  /* FAKEMATCH(dead): a once-only loop, which places the test after the body. */
  while (id >= 0 && Event_GetFlag(FLAG_AREA_TEMP_03)) {
    const s32 *expected = &gLinkLobby_SlotValues[slot];

    if (id != 0)
      Event_SetFlag(FLAG_AREA_TEMP_02);
    else
      Event_ClearFlag(FLAG_AREA_TEMP_02);
    {
      struct LinkPeer *row =
          &gLinkPeerSignatures[Event_GetFlag(FLAG_AREA_TEMP_02) ^ 1];

      if (row->values[gLinkLobby_SlotColumns[slot]] == *expected)
        return 1;
      break;
    }
  }
  return 0;
}

void LinkLobby_WriteSlotValue(s32 slot) {
  s32 *dst = (s32 *)gSerialTransfer.payload;
  const s32 *src = &gLinkLobby_SlotValues[slot];
  dst[gLinkLobby_SlotColumns[slot]] = *src;
}

extern s32 gLinkLobby_WaitFrames;

/// The lobby's task each frame: counts frames without the opponent, clears
/// the peer signatures after 25, and raises the event trigger when both sides
/// are ready or the link is lost. Returns whether the peer answered.
// Ported from ☀️'s LinkLobby_PollPeerReady.
s32 LinkLobby_PollPeerReady(void) {
  struct FieldWork *work;
  s32 result;
  s32 i;
  s32 set;
  s32 z;

  work = Ram_HeapSlots->fieldWork;
  result = 1;
  z = Actor_GetById(4)->z;

  if (z > 0xe00000)
    Event_ClearFlag(FLAG_AREA_TEMP_04);
  if (work->raisedTrigger != 2) {
    LinkLobby_PeerSlotMatches(0);
    set = Event_GetFlag(FLAG_AREA_TEMP_03);
    if (!set) {
      if (++gLinkLobby_WaitFrames > 25) {
        s32 offset = 0;

        i = 3;
      clear:
        Iwram_ClearWordsEntry((u8 *)gLinkPeerSignatures + offset, 20);
        i--;
        offset += 24;
        if (i >= 0)
          goto clear;
        gLinkLobby_WaitFrames = 0;
        LinkLobby_WriteSlotValue(4);
      }
    } else {
      gLinkLobby_WaitFrames = 0;
    }
    if (gLinkLobby_WaitFrames == 0) {
      if (LinkLobby_PeerSlotMatches(0) &&
          (LinkLobby_PeerSlotMatches(1) || LinkLobby_PeerSlotMatches(2))) {
        Event_SetFlag(FLAG_SCENE_TEMP_01);
        if (Event_GetFlag(FLAG_SCENE_TEMP_02))
          work->raisedTrigger = 1;
        result = 1;
      } else {
        Event_ClearFlag(FLAG_SCENE_TEMP_01);
        result = 0;
      }
    }
    if (Event_GetFlag(FLAG_SCENE_TEMP_01) &&
        Event_GetFlag(FLAG_SCENE_TEMP_02) && !Event_GetFlag(FLAG_SCENE_TEMP_00))
      work->raisedTrigger = 1;
  }
  if ((Event_GetFlag(FLAG_SCENE_TEMP_01) ||
       Event_GetFlag(FLAG_SCENE_TEMP_02)) &&
      !Event_GetFlag(FLAG_MONSTER_BATTLE) && !LinkLobby_PeerSlotMatches(0) &&
      gLinkLobby_WaitFrames > 24) {
    work->raisedTrigger = 2;
    Event_SetFlag(FLAG_SCENE_TEMP_05);
    Event_ClearFlag(FLAG_SCENE_TEMP_01);
    Event_ClearFlag(FLAG_SCENE_TEMP_02);
    LinkLobby_WriteSlotValue(4);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_05))
    work->raisedTrigger = 2;
  return result;
}

#include "text/msg_ids.h"

extern s32 gLinkLobby_CallFrames;

s32 LinkLobby_PromptToParticipate(void) {
  s32 set;

  set = Event_GetFlag(FLAG_SCENE_TEMP_03);
  if (set != 0) {
    return set;
  }
  if (++gLinkLobby_CallFrames == 300) {
    gLinkLobby_CallFrames = 0;
    Event_ClearFlag(FLAG_SCENE_TEMP_00);
  }
  set = Event_GetFlag(FLAG_SCENE_TEMP_00);
  if (set != 0) {
    return set;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgIfYouWantToParticipate);
  Event_OpenMessage(8, 0);
  wait(5);
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  Event_End();
}

s32 LinkLobby_ReportDisconnection(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  LinkLobby_WriteSlotValue(4);
  Event_ClearFlag(FLAG_SCENE_TEMP_00);
  Event_ClearFlag(FLAG_SCENE_TEMP_03);
  Event_Start();
  Event_ResolvePendingActions(0);
  work->raisedTrigger = 0;
  Event_SetMessageId((s32)MsgTheLinkWas);
  Event_OpenMessage(8, 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_05);
  Event_End();
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LOBBY_PREFIX_LIMIT 7
#else
#define LOBBY_PREFIX_LIMIT 4
#endif

/// The Broken Seal's BattleArena_ReceivePartyRecords: takes the other player's
/// party records over the link.
s32 LinkLobby_ReceivePartyRecords(void) {
  u16 namePrefix[24];
  s32 result;
  u32 size;
  void *heap;
  s32 timeout;
  s32 slot;
  s32 tries;
  s32 transfer;
  s32 i;
  s32 length;
  u8 *record;

  size = 0x154;
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = 900;
  for (slot = 0;; slot++) {
    if (slot > 2)
      goto second;
    record = Unit_Get(slot + 128);
    transfer = Serial_StartReceive(record);
    tries = 0;
    if (transfer == -1)
      break;
    while (Serial_GetActiveTransfers() != 0) {
      if (gSerialReceivedSize > size) {
        result = -1;
        goto done;
      }
      wait(1);
      if (--timeout < 0 || (gLinkStatus & 3) != 3) {
        if (++tries > 24) {
          result = -1;
          goto done;
        }
      }
    }
    if (gSerialReceivedSize != size) {
      result = -1;
      goto done;
    }
    if (record[0x12a] != 0)
      result++;
    wait(2);
    FieldMessage_DecodeRawUnbounded((s32)MsgEnemyLabel, namePrefix);
    for (i = 0; i <= LOBBY_PREFIX_LIMIT && namePrefix[i] != 0; i++)
      ;
#if LOBBY_PREFIX_LIMIT == 7
    namePrefix[i] = 32;
    length = i + 1;
#else
    length = i;
#endif
    for (i = 14; i >= length; i--)
      record[i] = record[i - length];
    for (i = 0; i < length; i++)
      record[i] = namePrefix[i];
    record[14] = 0;
  }
  goto failed;
  for (;;) {
  second:
    Heap_Release(heap);
    size = 0x154;
    heap = Heap_AllocEwram(size);
    transfer = Serial_StartReceive(Djinn_GetRecoveryTable(1));
    tries = 0;
    if (transfer == -1)
      break;
    while (Serial_GetActiveTransfers() != 0) {
      s32 limit = 0x154;

      if (gSerialReceivedSize > limit) {
        result = -1;
        goto done;
      }
      wait(1);
      if (--timeout < 0 || (gLinkStatus & 3) != 3) {
        if (++tries > 24) {
          result = -1;
          goto done;
        }
      }
    }
    if (gSerialReceivedSize != 0x154)
      result = -1;
    else
      wait(2);
    goto done;
  }
failed:
  result = transfer;
done:
  Heap_Release(heap);
  return result;
}

/// Writes up to three active party owners to out, ending the list with 0xff.
s32 LinkLobby_CopyPartyOwnerIds(u16 *out) {
  struct MainRoutineWork *party;
  s32 count = Party_CountMembers();
  s32 i;

  if (count > 3)
    count = 3;
  i = 0;
  if (i >= count)
    goto done;
  party = &gMainRoutineWork;
next:
  {
    u8 owner = party->activeOwners[i];

    if (out != NULL)
      *out++ = owner;
  }
  i++;
  if (i < count)
    goto next;
done:
  if (out != NULL)
    *out = 0xff;
  return count;
}

/// The list inside a sent recovery table, as ☀️'s DjinnRecoveryList.
struct OfferList {
  struct TradeOffer offers[72];
  s32 count;
};

/// The recovery table as sent: the list from its eighth byte.
struct OfferTable {
  u8 unk00[8];
  struct OfferList list;
};

s32 Serial_StartSend(void *data, s32 size);

/// The Broken Seal's BattleArena_SendPartyRecords: sends the party's records to
/// the other player.
s32 LinkLobby_SendPartyRecords(void) {
  s32 result;
  u16 memberIds[8];
  u8 peerSlots[8];
  s32 transferResult;
  u8 *heap;
  s32 memberCount;
  s32 timeout;
  s32 tries;
  s32 i;
  s32 j;
  s32 k;
  u32 size;
  struct OfferList *list;

  size = 0x154;
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = 600;
  memberCount = LinkLobby_CopyPartyOwnerIds(memberIds);
  for (i = 0; i <= 7; i++)
    peerSlots[i] = 0;
  i = 0;
  if (i >= memberCount)
    goto next;
  {
  send:
    {
      s32 tries = 0;

      Iwram_CopyWordsEntry(heap, Unit_Get(memberIds[i]), size);
      heap[0x12a] = 2;
      peerSlots[memberIds[i]] = i - 128;
      if ((transferResult = Serial_StartSend(heap, size)) == -1) {
        result = transferResult;
        goto done;
      }
      goto testMember;
    waitMember:
      wait(1);
      if (--timeout < 0 || (gLinkStatus & 3) != 3) {
        if (++tries > 24) {
          result = -1;
          goto done;
        }
      }
    testMember:
      if (Serial_GetActiveTransfers() != 0)
        goto waitMember;
      wait(2);
      i++;
      if (i < memberCount)
        goto send;
      /* FAKEMATCH(dead): an empty once-only block between the test and the
       * jump. */
      do {
      } while (0);
      goto next;
    }
  }
wait:
  wait(1);
  if (--timeout < 0 || (gLinkStatus & 3) != 3) {
    if (++tries > 24) {
      result = -1;
      goto done;
    }
  }
test:
  if (Serial_GetActiveTransfers() != 0)
    goto wait;
  wait(2);
  i++;
next:
  if (i <= 2) {
    heap[0x12a] = tries = 0;
    if ((transferResult = Serial_StartSend(heap, size)) != -1)
      goto test;
    result = transferResult;
    goto done;
  }
  Heap_Release(heap);
  size = 0x154;
  heap = Heap_AllocEwram(size);
  /* FAKEMATCH(dead): the statement in a once-only block. */
  do {
    Iwram_CopyWordsEntry(heap, Djinn_GetRecoveryTable(0), size);
  } while (0);
  {
    s32 timeout;
    s32 tries;

    list = &((struct OfferTable *)heap)->list;
    tries = 0;
    timeout = 600;
    for (j = 0; j < list->count; j++) {
      list->offers[j].owner = peerSlots[list->offers[j].owner];
      if (list->offers[j].owner == 0) {
        for (k = j; k < list->count - 1; k++)
          list->offers[k] = list->offers[k + 1];
        list->count--;
        j--;
      }
    }
    if ((transferResult = Serial_StartSend(heap, size)) == -1) {
      result = transferResult;
      goto done;
    }
    goto testOffers;
  waitOffers:
    wait(1);
    if (--timeout < 0 || (gLinkStatus & 3) != 3) {
      if (++tries > 24) {
        result = -1;
        goto done;
      }
    }
  testOffers:
    if (Serial_GetActiveTransfers() != 0)
      goto waitOffers;
    wait(1);
    wait(2);
  }
done:
  Heap_Release(heap);
  return result;
}

/// Resets the transfer state, as LinkBattle_ResetTransferState does.
// In The Lost Age's store order.
static __inline__ void Lobby_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  state = &gSerialTransfer;
  ime = &REG_IME;
  /* FAKEMATCH(dead): the IME exchange in a once-only block. */
  do {
    saved = *ime;
    *ime = (u16)ime;
  } while (0);
  zero = 0;
  state = &gSerialTransfer;
  state->status = 0x80;
  *(u8 *)&state->peerFlags = zero;
  gSerialSendSource = zero;
  *(u8 *)&state->flags = zero;
  gSerialSendSize = zero;
  gSerialReceiveDest = zero;
  gSerialReceivedSize = zero;
  *ime = saved;
}

/// Exchanges the two parties. The parent sends first, and the child (area flag
/// FLAG_BATTLE_ARENA_LINK_CHILD) receives first.
///
/// On success stores the partner's party member count in FLAG_LINK_PARTY_COUNT
/// and returns it. On failure resets the transfer state and returns a negative
/// result.
s32 LinkLobby_ExchangePartyRecords(void) {
  s32 result;
  s32 first;
  s32 flag;

  /* FAKEMATCH(dead): the result's zero in a once-only block. */
  do {
    result = 0;
  } while (0);
  flag = Event_GetFlag(FLAG_AREA_TEMP_02);
  gLinkExchangeState = result;
  if (!flag) {
    wait(5);
    result = LinkLobby_SendPartyRecords();
    if (result < 0)
      goto fail;
    wait(5);
    first = result = LinkLobby_ReceivePartyRecords();
    if (result < 0)
      goto check;
  } else {
    first = result = LinkLobby_ReceivePartyRecords();
    if (result < 0)
      goto fail;
    wait(10);
    result = LinkLobby_SendPartyRecords();
    if (result < 0)
      goto fail;
  }
  GameFlag_SetByte(0x3f0, first);
  result = first;
check:
  if (first < 0) {
  fail:
    Lobby_ResetTransferState();
  }
  return result;
}

void Map_ClearLayerEntryFlag(s32 layer);
void Map_SetLayerEntryFlag(s32 layer);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
struct Window *Message_OpenWindow(const u8 *message, s32 x, s32 y, s32 flags);

extern char SceneId_Lobby;

// Touching the lobby counter: waits for the opponent, walks the lead to the
// arena door, swaps party records and starts the link battle.
s32 LinkLobby_EnterBattle(void) {
  struct FieldWork *work;
  s32 failed;
  struct Window *window;
  s32 cnt;
  s32 stop;
  u8 *buf;
  u8 *src;
  u16 *table;
  u32 i;
  s32 scene;
  s32 set;

  failed = 0;
  window = 0;
  work = Ram_HeapSlots->fieldWork;
  cnt = 0;
  if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
    Event_Start();
    Event_ResolvePendingActions(0);
  } else {
    if (!Event_GetFlag(FLAG_SCENE_TEMP_00))
      return;
    if (Event_GetFlag(FLAG_SCENE_TEMP_05))
      return;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetFlag(FLAG_SCENE_TEMP_03);
    LinkLobby_WriteSlotValue(2);
    if (!LinkLobby_PeerSlotMatches(2))
      window = Message_OpenWindow(MsgAwaitingYourOpponent, 5, 4, 1);
    while (!LinkLobby_PeerSlotMatches(2)) {
      wait(1);
      stop = 0;
      if (!Event_GetFlag(FLAG_SCENE_TEMP_01))
        stop = 1;
      if (Event_GetFlag(FLAG_SCENE_TEMP_05))
        stop = 1;
      if (!LinkLobby_PeerSlotMatches(2) && !LinkLobby_PeerSlotMatches(1)) {
        if (++cnt > 25)
          stop = 1;
      } else {
        cnt = 0;
      }
      if (stop) {
        work->raisedTrigger = 2;
        Event_SetFlag(FLAG_SCENE_TEMP_05);
        Event_ClearFlag(FLAG_SCENE_TEMP_01);
        Event_ClearFlag(FLAG_SCENE_TEMP_02);
        LinkLobby_WriteSlotValue(4);
        failed = 1;
        Event_ClearFlag(FLAG_SCENE_TEMP_00);
        break;
      }
    }
    if (window)
      Window_Close(window, 1);
    wait(5);
  }
  if (!failed) {
    buf = HeapSlot_AllocEwram(216, 0x7c8);
    Task_Remove(LinkLobby_PollPeerReady);
    Map_ClearLayerEntryFlag(5);
    wait(8);
    Map_SetLayerEntryFlag(5);
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
      Event_SetMessageId((s32)MsgGoodLuck2);
      Event_OpenMessage(8, 0);
      wait(45);
      Actor_SetSpeed(4, 1);
      Actor_WalkTo(4, 216, 184);
      Actor_WaitForMoveById(4);
      Actor_WalkTo(4, 216, 168);
      Actor_WaitForMoveById(4);
    } else {
      Actor_SetSpeed(4, 1);
      Actor_WalkTo(4, 216, 200);
      Actor_WaitForMoveById(4);
      Actor_SetSpeed(4, 0.1);
      Actor_WalkTo(4, 216, 168);
      if (LinkLobby_ExchangePartyRecords() < 0) {
        Actor_SetSpeed(4, 1);
        Actor_WalkTo(4, 216, 200);
        Map_ClearLayerEntryFlag(5);
        wait(8);
        Map_SetLayerEntryFlag(5);
        Actor_WaitForMoveById(4);
        HeapSlot_Release(216);
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        Task_Add((TaskFunc)LinkLobby_PollPeerReady, TASK_PASS_VBLANK);
        Task_SetFlags(LinkLobby_PollPeerReady, 1);
        Event_ClearFlag(FLAG_SCENE_TEMP_01);
        Event_ClearFlag(FLAG_SCENE_TEMP_02);
        Event_ClearFlag(FLAG_AREA_TEMP_03);
        Event_ClearFlag(FLAG_SCENE_TEMP_03);
        Event_ClearFlag(FLAG_SCENE_TEMP_00);
        work->raisedTrigger = 2;
        Event_End();
        return;
      }
      Actor_SetSpeed(4, 0.5);
      Actor_WaitForMoveById(4);
    }
    /* FAKEMATCH(temporary): the flag test's result kept while the scene symbol
     * is loaded once for both branches. */
    set = Event_GetFlag(FLAG_MONSTER_BATTLE);
    scene = (s32)&SceneId_Lobby;
    if (set) {
      Event_SetAfterBattleDestination(scene, 8);
      Party_SetFields1f2And1f4(scene, 9);
    } else {
      Event_SetAfterBattleDestination(scene, 10);
      Party_SetFields1f2And1f4(scene, 11);
    }
    gMainRoutineWork.battleRule24b = 4;
    Field_RequestBattleFromEncounterTable(1, 1);
    table = (u16 *)gSerialTransfer.payload;
    table[0] = 0x45;
    table[1] = 0x58;
    table[2] = 0x45;
    table[3] = 0x43;
    {
      u32 last = 0x7c7;

      i = 0;
      src = buf;
    copy:
      {
        u8 *dst = Ram_MapCellBuffer + 0x8000 + i;
        u8 b = *src;

        i++;
        src++;
        *dst = b;
      }
      if (i <= last)
        goto copy;
    }
    HeapSlot_Release(216);
  }
  Event_End();
}

/// The lobby clerk: offers the link battle, or reports on the one waiting.
s32 LinkLobby_ChooseBattleMode(void) {
  s32 answer;
  s32 msg;
  s32 line;

  msg = (s32)MsgPleaseSpeakTo;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
  if (LinkLobby_PeerSlotMatches(0) == 0)
    wait(1);
  if (LinkLobby_PeerSlotMatches(0) == 0) {
    LinkLobby_WriteSlotValue(5);
    LinkLobby_InitializeSerial();
    if (!Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Event_SetMessageId(msg + 5);
      Event_OpenMessage(8, 0);
      answer = Event_AskYesNoWithActorAnimation(4, 0);
      if (answer == 0) {
        GameFlag_SetByte(1000, 0);
        Event_SetFlag(FLAG_MONSTER_BATTLE);
        Event_ClearFlag(0x172);
        Event_ClearFlag(FLAG_LINK_BATTLE);
        Event_SetFlag(FLAG_SCENE_TEMP_02);
        line = msg + 7;
        gMainRoutineWork.linkTallies[3] = answer;
        goto message;
      }
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
      Event_SetFlag(FLAG_LINK_BATTLE);
      LinkLobby_WriteSlotValue(0);
      line = msg + 6;
      goto message;
    } else {
      line = msg + 3;
      goto message;
    }
  } else {
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      LinkLobby_WriteSlotValue(0);
      Event_SetMessageId((s32)MsgYourOpponentHas);
      Event_OpenMessage(8, 0);
      Event_ClearFlag(FLAG_SCENE_TEMP_02);
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
    }
    if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
      line = msg + 3;
    message:
      Event_SetMessageId(line);
    open_message:
      Event_OpenMessage(8, 0);
    } else if (!Event_GetFlag(FLAG_SCENE_TEMP_01) &&
               !Event_GetFlag(FLAG_AREA_TEMP_00)) {
      Event_SetMessageId(msg);
      Event_OpenMessage(8, 0);
      Event_SetFlag(FLAG_AREA_TEMP_00);
    } else {
      Event_SetFlag(FLAG_AREA_TEMP_00);
      if (Event_GetFlag(FLAG_SCENE_TEMP_01))
        Event_SetMessageId(msg + 2);
      else
        Event_SetMessageId(msg + 1);
      Event_OpenMessage(8, 0);
      if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
        if (LinkLobby_PeerSlotMatches(0)) {
          Event_SetFlag(FLAG_LINK_BATTLE);
          Event_SetFlag(0x172);
          if (Event_GetFlag(FLAG_SCENE_TEMP_01))
            Event_SetMessageId(msg + 3);
          else
            Event_SetMessageId(msg + 4);
          LinkLobby_WriteSlotValue(1);
          Event_SetFlag(FLAG_SCENE_TEMP_02);
          goto open_message;
        } else {
          Event_SetFlag(FLAG_SCENE_TEMP_05);
        }
      } else {
        Event_SetMessageId(msg);
        Event_OpenMessage(8, 0);
      }
    }
  }
  Event_End();
}

void MessageArgument_ClearAll(void);
void MessageArgument_Push(s32 value, s32 digits);

// Tells the leader's win count, total or in a row by the way the lead faces.
s32 LinkLobby_ReportWinTallies(s32 actor) {
  u32 facing = Actor_GetById(4)->facing;
  u16 *count;
  s32 base;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actor, gMainRoutineWork.currentOwner, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    base = (s32)MsgItLooksLikeThisIs;
    count = &gMainRoutineWork.linkTallies[4];
    if (*count != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*count, 5);
      Event_SetMessageId(base + 1);
      Event_ShowMessage(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgIWillTell);
      Event_ShowMessage(actor, 0);
    }
  } else {
    base = (s32)MsgItLooksLikeThisIsYour;
    count = &gMainRoutineWork.linkTallies[7];
    if (*count != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*count, 5);
      Event_SetMessageId(base + 1);
      Event_ShowMessage(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgIWillTellYou);
      Event_ShowMessage(actor, 0);
    }
  }
}

s32 Window_Create();
void WindowSprite_PrepareForRedraw();
void Text_DrawNumber();
s32 DebugParty_LoadPreset();

s32 LinkLobby_ChooseDebugParameterSet(s32 actorId) {
  s32 window;
  s32 choice;
  s32 shown;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actorId, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgChooseYourParameters);
  Event_ShowMessage(actorId, 0);
  window = Window_Create(0, 0, 6, 4, 2);
  choice = 0;
  shown = -1;
  for (;;) {
    if (choice != shown) {
      WindowSprite_PrepareForRedraw(window);
      Text_DrawNumber(choice, 3, window, 0, 0);
      shown = choice;
    }
    if (gInput.repeat & KEY_LEFT) {
      choice--;
    }
    if (gInput.repeat & KEY_RIGHT) {
      choice++;
    }
    if (choice < 0) {
      choice = 0;
    }
    if (((struct InputState *)&gInput)->pressed & 1) {
      break;
    }
    if (((volatile struct InputState *)&gInput)->pressed & 2) {
      choice = -1;
      break;
    }
    wait(1);
  }
  Window_Close(window, 1);
  {
    s32 ok;
    s32 message;

    if (choice >= 0) {
      ok = DebugParty_LoadPreset(choice);
    } else {
      message = (s32)MsgWhatIsThis;
      goto show;
    }
    // The refusal jumps into the unknown-preset branch, so the two share one
    // message call.
    if (ok != 0) {
      message = (s32)MsgThereSNoSuch;
    show:
      Event_SetMessageId(message);
      Event_ShowMessage(9, 0);
    } else {
      Event_SetMessageId((s32)MsgTheSettingsAre);
      Event_ShowMessage(9, 0);
    }
  }
  wait(10);
  Event_End();
}

/// The lobby's healer opens the sanctum.
void LinkLobby_OpenSanctum(s32 value) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sanctum_Run(value);
  Event_End();
}

/// Whether owner is one of the first three active owners, or the first four
/// while flag 370 is clear; an 0xff entry ends the list.
s32 LinkLobby_PartyContains(s32 owner) {
  s32 count = Party_CountMembers();
  s32 limit = 3;
  s32 i;

  if (!Event_GetFlag(370))
    limit = 4;
  if (count > limit)
    count = limit;
  for (i = 0; i < count; i++) {
    if (gMainRoutineWork.activeOwners[i] == 0xff)
      return 0;
    if (gMainRoutineWork.activeOwners[i] == owner)
      return 1;
  }
  return 0;
}

/// Unused: does nothing; nothing calls it.
UNUSED void LinkLobby_DoNothing(void) {}
#undef LOBBY_PREFIX_LIMIT

extern u8 MsgYouHaveA[];
extern u8 MsgComeBackWhen[];
extern u16 gLinkLobby_RequestableSounds[];
extern s16 gData_02003860;

void Event_Start(void);
void Event_End(void);
s32 SoundFlag_Test(s32 song);
s32 Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void WindowSprite_PrepareForRedraw(s32 window);
void Text_DrawNumber(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Window_Close(s32 window, s32 release);

/// A song request: the player picks a sound by number, only the ones already
/// heard unless all are unlocked (L, R and Select held as it opens), and
/// hears it, until B.
/// "You have a song request?"
void LinkLobby_TakeSongRequest(s32 actor) {
  s32 window;
  s32 song;
  s32 shown;
  s32 step;
  s32 all;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actor, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgYouHaveA);
  Event_ShowMessage(actor, 0);
  song = 0;
  all = 0;
  if ((gInput.held & (KEY_START | KEY_UP | KEY_L)) == 0x248)
    all = 1;
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  for (;;) {
    window = Window_Create(12, 8, 6, 3, 2);
    shown = -1;
    for (;;) {
      if (song != shown) {
        WindowSprite_PrepareForRedraw(window);
        Text_DrawNumber(song, 3, window, 0, 0);
        shown = song;
      }
      step = 0;
      if (gInput.repeat & KEY_LEFT)
        step--;
      if (gInput.repeat & KEY_RIGHT)
        step = 1;
      while (step != 0) {
        song += step;
        if (song < 0)
          song = 96;
        if ((u32)song > 96)
          song = 0;
        if (all != 0 || gMainRoutineWork.unk22d[2] != 0 || gData_02003860 != 0)
          break;
        if (SoundFlag_Test(gLinkLobby_RequestableSounds[song]) != 0)
          goto keys;
      }
    keys:
      if (gInput.pressed & KEY_A)
        break;
      if (gInput.pressed & KEY_B) {
        song = -1;
        break;
      }
      wait(1);
    }
    Window_Close(window, 1);
    if (song < 0)
      break;
    Event_PlaySound(gLinkLobby_RequestableSounds[song]);
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  wait(120);
  Event_PlaySound(MUS_709);
  /* FAKEMATCH(dead): the statement in a once-only block. */
  do {
    Event_SetMessageId((s32)MsgComeBackWhen);
  } while (0);
  Event_ShowMessage(actor, 0);
  wait(10);
  Event_End();
}

s32 LinkLobby_PartyContains(s32 id);
void Serial_RemoveIrqHandlers(void);
void Map_CenterOnFollowedPoint(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void MessageArgument_Push(s32 value, s32 digits);
void Save_ProcessSelectedFile(void);
void Event_Start(void);
/* VIEW(talk): routine Actor_ShowEmoteById as void Actor_ShowEmoteById(s32
 * actor, s32 animation, s32 flags); event.h has s32 Actor_ShowEmoteById(s32
 * actorId, s32 emote, s32 frames); tried as the definition's struct
 * BattleEffectWatchedObject *Actor_ShowEmoteById(s32 resourceId, s32 flags, s32
 * state), it does not compile in tla-ja: games/tla/src/scene/lobby.c:1038:
 * conflicting types for `Actor_ShowEmoteById'. */
extern void Actor_ShowEmoteById__talk(s32 actor, s32 animation,
                                      s32 flags) __asm__("Actor_ShowEmoteById");
s32 Import_EventSetDestination(s32 scene, s32 entrance);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void FieldMessage_Show(s32 no, s32 flags);

s32 LinkLobby_TalkByProgress(s32 actor) {
  s32 base = (s32)MsgItSOur;
  s32 lead = LinkLobby_PartyContains(4);
  s32 own = LinkLobby_PartyContains(actor);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actor, gMainRoutineWork.currentOwner, 0);
  if (Event_GetFlag(FLAG_AREA_TEMP_04)) {
    Event_GetFlag(FLAG_SCENE_TEMP_F4);
    base = Event_GetFlag(actor + 0x2f0);
    if (Event_GetFlag(FLAG_AREA_TEMP_05)) {
      if (base)
        base = (s32)MsgEverybodyDeservesA;
      else
        base = (s32)MsgEverybodyDidGreat;
    } else {
      if (base)
        base = (s32)MsgDonTBlame;
      else
        base = (s32)MsgYouWinSome;
    }
  } else if (lead != 0) {
    if (own == 0)
      base = (s32)MsgOkWeRe;
  } else {
    base = (s32)MsgILlFight;
  }
  Event_SetMessageId(base + actor);
  Event_ShowMessage(actor, 0);
  if (base == (s32)MsgILlFight && actor == 5) {
    Actor_ShowEmoteById__talk(5, 263, 0);
    Event_Wait(10);
  }
  Event_End();
}

/// Step trigger 1, the way out: stops the link, loads the normal reverb code
/// and goes back to the File Screen (its entrance FILE_SCREEN_FROM_SANCTUM).
s32 LinkLobby_LeaveArena(void) {
  Serial_RemoveIrqHandlers();
  Sound_LoadReverbCode(SOUND_REVERB_NORMAL);
  /* FAKEMATCH(dead): the return in a once-only block. */
  do {
    return Import_EventSetDestination((s32)&SceneId_Clear, 1);
  } while (0);
}

/// The visitors' talk, by whether the party has fought here (flag 0x206);
/// actor 14 takes song requests while L or R is held.
s32 LinkLobby_TalkToVisitor(s32 actor) {
  s32 message;

  if (actor == 14 && (gInput.held & KEYS_SHOULDERS)) {
    LinkLobby_TakeSongRequest(14);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SCENE_TEMP_06)) {
    switch (actor) {
    case 13:
      message = (s32)MsgILlBe;
      break;
    case 14:
      message = (s32)MsgILikeMy;
      break;
    default:
      message = (s32)MsgSometimesYourClass;
      break;
    }
  } else {
    switch (actor) {
    case 13:
      message = (s32)MsgYouLookA;
      break;
    case 14:
      message = (s32)MsgILikeMy;
      break;
    default:
      message = (s32)MsgSometimesYourClass;
      break;
    }
    Event_SetFlag(FLAG_SCENE_TEMP_06);
  }
  Event_SetMessageId(message);
  Event_OpenMessage(actor, 0);
  Event_Wait(1);
  if (message == (s32)MsgYouLookA) {
    Actor_StartRepeatedMotion(14, 1);
    Actor_SetAnimation(14, 4);
    Event_Wait(15);
    Actor_FaceActor(14, gMainRoutineWork.currentOwner, 0);
    Event_Wait(5);
    Event_OpenMessage(14, 0);
  }
  Event_End();
}

s32 LinkLobby_ExplainPartyOrder(s32 actor) {
  s32 message;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actor, gMainRoutineWork.currentOwner, 0);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_04)) {
    if (Party_CountMembers() == 8)
      message = (s32)MsgUpToThree;
    else if (Party_CountMembers() <= 3)
      message = (s32)MsgUpToThreeAllies;
    else
      message = (s32)MsgUpToThreeAlliesCan;
    Event_SetFlag(FLAG_SCENE_TEMP_04);
  } else {
    message = (s32)MsgYouCanChange;
    Event_ClearFlag(FLAG_SCENE_TEMP_04);
  }
  Event_SetMessageId(message);
  Event_OpenMessage(actor, 0);
  Event_End();
}

s32 LinkLobby_ReportMonsterRecord(s32 id) {
  struct MainRoutineWork *gs;

  Event_Start();
  Event_ResolvePendingActions(0);
  gs = &gMainRoutineWork;
  Actor_FaceActor(id, gs->currentOwner, 0);
  if (gs->linkTallies[2] != 0) {
    MessageArgument_Push(gs->linkTallies[2], 5);
    Event_SetMessageId((s32)MsgYouVeWon);
  } else {
    Event_SetMessageId((s32)MsgICanTWaitTo);
  }
  Event_OpenMessage(id, 0);
  Event_End();
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 LinkLobby_PartyContains(s32 id);
void Map_CenterOnFollowedPoint(void);
void MessageArgument_Push(s32 value, s32 digits);
void Save_ProcessSelectedFile(void);
void Event_End(void);
s32 Import_EventSetDestination(s32 scene, s32 entrance);

/// Saves and reports the link battle's results.
s32 LinkLobby_SaveBattleResults(void) {
  Save_ProcessSelectedFile();
  FieldMessage_Show(MsgBattleResultsWere, 1);
}

/// Saves and reports the monster battle's results.
s32 LinkLobby_SaveMonsterBattleResults(void) {
  Save_ProcessSelectedFile();
  FieldMessage_Show(MsgMonsterBattleResults, 1);
}

/// Draws value, at most 999, as three digits from right to left.
void LinkLobby_DrawThreeDigitValue(s32 value) {
  s32 i;

  if (value > 999)
    value = 999;
  i = 0;
next:
  MapGrid_CopyCellsTo(27, value % 10, 16 - i, 8, 1, 1);
  value /= 10;
  i++;
  if (i <= 2)
    goto next;
  Map_CenterOnFollowedPoint();
}

void Event_End(void);

extern s32 gLinkLobby_WaitFrames;
extern s32 gLinkLobby_CallFrames;
extern u16 gData_02003a74[];
extern u8 gAutoSleepEnabled;

s32 LinkLobby_PollPeerReady(void);
void LinkLobby_InitializeSerial(void);
void Serial_Initialize(void);
void Map_SetLayerEntryFlag(s32 entry);
void Party_RestoreHpPp(s32 value);
void Sound_LoadReverbCode(s32 preset);
void LinkLobby_WriteSlotValue(s32 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void GameFlag_SetByte(s32 flag, s32 value);

static inline void Flag_SetByte(s32 flag, s32 value) {
  /* FAKEMATCH(wrapper): Flag_SetByte forwarded inline. */
  GameFlag_SetByte(flag, value);
}

/// Sets the lobby up as The Broken Seal's BattleArena_Initialize sets the arena
/// up.
s32 LinkLobby_Initialize(void) {
  s32 i;
  s32 wins;
  u32 score;
  struct FieldWork *work;
  void *func;

  Event_SetFlag(0x17e);
  gMainRoutineWork.packedEvents[0] = 0;
  gMainRoutineWork.packedEvents[1] = 0;
  gLinkLobby_CallFrames = 0;
  gLinkLobby_WaitFrames = 0;
  gFieldWork->transition = 0x201;
  Sound_LoadReverbCode(2);
  LinkLobby_DrawThreeDigitValue(gMainRoutineWork.linkTallies[6]);
  MapGrid_CopyCellAttributes(11, 11, 1, 1, 13, 10);
  LinkLobby_WriteSlotValue(4);
  wait(1);
  Map_SetLayerEntryFlag(5);
  {
    u16 *name = gData_02003a74;

    name[4] = 'T';
    name[5] = 'A';
    name[6] = 'L';
    name[7] = 'K';
  }
  for (i = 0; i < 8; i++) {
    s32 flag = 0x2f0 + i;

    Event_ClearFlag(flag);
    if (LinkLobby_PartyContains(i))
      Event_SetFlag(flag);
  }
  if (gMainRoutineWork.entrance == 8) {
    s32 msg;
    s32 v;

    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(5);
    gMainRoutineWork.linkTallies[0]++;
    gMainRoutineWork.linkTallies[3]++;
    wins = (s8)GameFlag_GetByte(0x3f8);
    msg = wins * 2 + 2;
    if (msg > 14)
      msg = 14;
    v = FieldGameFlag_GetByte(1000);
    if (v == 2) {
      GameFlag_SetByte(1000, 0);
      wins++;
      msg++;
    } else {
      Flag_SetByte(1000, v + 1);
    }
    Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId((s32)MsgNotBadWould + msg);
    Event_OpenMessage(8, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      if (wins > 90)
        wins = 90;
      GameFlag_SetByte(0x3f8, wins);
    } else {
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
      GameFlag_SetByte(0x3f8, -1);
      // score holds the row's address, then the score read through it.
      score = (u32)&gMainRoutineWork.linkTallies[3];
      MessageArgument_Push(*(u16 *)score, 5);
      score = *(u16 *)score;
      if (gMainRoutineWork.linkTallies[2] < score) {
        gMainRoutineWork.linkTallies[2] = score;
        Event_SetMessageId((s32)MsgYouWonBattle);
        Event_OpenMessage(8, 0);
        LinkLobby_SaveMonsterBattleResults();
      } else {
        Event_SetMessageId((s32)MsgYouWonBattlePlease);
        Event_OpenMessage(8, 0);
      }
      LinkLobby_WriteSlotValue(0);
    }
    Event_End();
  } else if (gMainRoutineWork.entrance == 9) {
    gMainRoutineWork.linkTallies[1]++;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(5);
    Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
    // score holds the row's address, then the score read through it.
    score = (u32)&gMainRoutineWork.linkTallies[3];
    MessageArgument_Push(*(u16 *)score, 5);
    score = *(u16 *)score;
    if (gMainRoutineWork.linkTallies[2] < score) {
      gMainRoutineWork.linkTallies[2] = score;
      Event_SetMessageId((s32)MsgYouWonBattle);
      Event_OpenMessage(8, 0);
      LinkLobby_SaveMonsterBattleResults();
    } else {
      Event_SetMessageId((s32)MsgYouWonBattleI);
      Event_OpenMessage(8, 0);
    }
    gMainRoutineWork.linkTallies[3] = 0;
    Event_ClearFlag(FLAG_MONSTER_BATTLE);
    GameFlag_SetByte(0x3f8, -1);
    LinkLobby_WriteSlotValue(0);
    Event_End();
  } else if (gMainRoutineWork.entrance == 10) {
    s32 v;

    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if (Event_GetFlag(FLAG_AREA_TEMP_E8)) {
      work = Ram_HeapSlots->fieldWork;
      Event_ClearFlag(FLAG_AREA_TEMP_E8);
      work->raisedTrigger = 2;
      Event_ClearFlag(FLAG_AREA_TEMP_04);
      wait(20);
      LinkLobby_InitializeSerial();
      LinkLobby_WriteSlotValue(0);
      LinkLobby_WriteSlotValue(4);
    } else {
      gMainRoutineWork.linkTallies[4]++;
      v = gMainRoutineWork.linkTallies[7] + 1;
      gMainRoutineWork.linkTallies[7] = v;
      score = (u32)&gMainRoutineWork.linkTallies[6];
      if (*(u16 *)score < (u16)v)
        *(u16 *)score = v;
      LinkLobby_DrawThreeDigitValue(*(u16 *)score);
      LinkLobby_SaveBattleResults();
      Event_SetFlag(FLAG_AREA_TEMP_04);
      Event_SetFlag(FLAG_AREA_TEMP_05);
    }
    Event_End();
  } else if (gMainRoutineWork.entrance == 11) {
    s32 v;

    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if ((v = Event_GetFlag(FLAG_MONSTER_BATTLE)) == 0) {
      gMainRoutineWork.linkTallies[5]++;
      gMainRoutineWork.linkTallies[7] = v;
      LinkLobby_SaveBattleResults();
    }
    Event_SetFlag(FLAG_AREA_TEMP_04);
    Event_ClearFlag(FLAG_AREA_TEMP_05);
    Event_End();
  } else {
    Serial_Initialize();
    Event_ClearFlag(0x172);
    GameFlag_SetByte(0x3f8, -1);
    if (gMainRoutineWork.unk244[6] != 0) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Actor_FaceActor(8, gMainRoutineWork.currentOwner, 0);
      Event_SetMessageId((s32)MsgNoteYouCan);
      Event_ShowMessage(8, 0);
      Event_End();
    }
    gMainRoutineWork.unk244[6] = 0;
    gAutoSleepEnabled = 0;
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
  }
  func = LinkLobby_PollPeerReady;
  Task_Add(func, 0x480);
  Task_SetFlags(func, 1);
  if (gMainRoutineWork.entrance != 8 || !Event_GetFlag(FLAG_MONSTER_BATTLE)) {
    Party_RestoreHpPp(1);
    DjinnRecovery_CountDownTwenty();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Edition-specific link handshake values (each word is four ASCII characters).
// clang-format off
s32 gLinkLobby_SlotValues[] = {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    0x32674d43, 0x33323130, 0x31434241, 0x32454443, 0x33474645, 0x434d4753,
#elif (LANGUAGE == LANGUAGE_GERMAN)
    0x47357343, 0x32313033, 0x43424131, 0x45444332, 0x47464533, 0x43534747,
#elif (LANGUAGE == LANGUAGE_SPANISH)
    0x47733743, 0x32313033, 0x43424131, 0x45444332, 0x47464533, 0x43535347,
#elif (LANGUAGE == LANGUAGE_FRENCH)
    0x32475363, 0x32313033, 0x43424131, 0x45444332, 0x47464533, 0x4353474d,
#elif (LANGUAGE == LANGUAGE_ITALIAN)
    0x31494773, 0x33323130, 0x31434241, 0x32454443, 0x33474645, 0x53474943,
#else
    0x43314773, 0x33323130, 0x31434241, 0x32454443, 0x33474645, 0x434d4753,
#endif
};

u8 gLinkLobby_SlotColumns[] = {
    0, 1, 1, 1, 1, 0,
};

u16 gLinkLobby_RequestableSounds[] = {
    709, 68, 74, 727, 1, 2, 3, 4,
    5, 6, 7, 8, 9, 10, 11, 12,
    13, 14, 15, 16, 17, 18, 20, 21,
    22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 34, 35, 36, 37, 38, 39,
    40, 41, 42, 43, 44, 55, 76, 49,
    50, 51, 52, 56, 57, 53, 54, 58,
    59, 60, 61, 62, 63, 64, 65, 70,
    71, 72, 73, 67, 75, 700, 701, 702,
    703, 704, 705, 706, 707, 708, 710, 720,
    721, 722, 723, 724, 725, 726, 728, 729,
    730, 741, 742, 743, 750, 751, 752, 753,
    754,
};

struct SceneEntrance gSceneEntrances[] = {
    {10, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {11, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {8, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
    {9, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
    {1, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {0, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(190, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 1),
    ACTOR_PLACEMENT(187, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 204, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(201, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(192, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 272, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(344, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 380, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(86, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 252, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(120, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 284, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(242, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(191, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 216, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 216, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(232, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 384, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(234, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 256, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(196, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(188, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(223, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(187, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 376, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(192, CONDITION_ALWAYS, 8, 360, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(0, 0x1000, MOVEMENT_TYPE_STAND, 324, 0, 216, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(1, 0x1001, MOVEMENT_TYPE_STAND, 296, 0, 168, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(2, 0x1002, MOVEMENT_TYPE_STAND, 352, 0, 152, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(3, 0x1003, MOVEMENT_TYPE_STAND, 352, 0, 108, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(6, 0x1006, MOVEMENT_TYPE_STAND, 376, 0, 136, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(7, 0x1007, MOVEMENT_TYPE_STAND, 328, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(5, 0x1005, MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// Unused: a placement table nothing reads.
u32 gLinkLobby_UnusedPlacements[] = {
    0xffff00be, 1, 0x980000, 0, 0xcc0000, 0x1004000,
    0xffff00bb, 1, 0x1180000, 0, 0xcc0000, 0x14000,
    0xffff00c9, 1, 0x1300000, 0, 0xa80000, 0x10000,
    0xffff00c0, 1, 0xc00000, 0, 0x1100000, 0x10000,
    344, 1, 0x1480000, 0, 0x880000, 0x10000,
    0xffff0056, 1, 0x400000, 0, 0x5c0000, 0x14000,
    0xffff0078, 1, 0x280000, 0, 0x740000, 0x10000,
    0xffff00f2, 1, 0x1000000, 0, 0x1800000, 0x14000,
    0xffff00bf, 1, 0x680000, 0, 0xf80000, 0x14000,
    0xffff00ba, 1, 0x1480000, 0, 0xf80000, 0x14000,
    0x100e8, 1, 0x1600000, 0, 0x6c0000, 0x14000,
    0x200e7, 1, 0x1600000, 0, 0x9c0000, 0x1c000,
    0x300ea, 1, 0x1780000, 0, 0x880000, 0x18000,
    0xffff00c4, 1, 0x400000, 0, 0x880000, 0x1c000,
    0xffff00bc, 1, 0xf80000, 0, 0xb80000, 0x14000,
    0xffff00df, 1, 0x1480000, 0, 0x580000, 0x14000,
    0xffff00bb, 1, 0x480000, 0, 0x1200000, 0x10000,
    0xffff00c0, 8, 0x1680000, 0, 0x1280000, 0x10000,
    0x10000000, 1, 0x1480000, 0, 0x880000, 0x10000,
    0x10010001, 1, 0x1600000, 0, 0x6c0000, 0x14000,
    0x10020002, 1, 0x1600000, 0, 0x9c0000, 0x1c000,
    0x10030003, 1, 0x1780000, 0, 0x880000, 0x18000,
    0x10060006, 1, 0x980000, 0, 0x900000, 0x18000,
    0x10070007, 1, 0x680000, 0, 0x900000, 0x10000,
    0x10050005, 1, 0x800000, 0, 0x7c0000, 0x14000,
    0xffff, 0, 0, 0, 0, 0,
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_TALK, 0, CONDITION_ALWAYS, EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, 1, CONDITION_ALWAYS, EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, 2, CONDITION_ALWAYS, EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, 3, CONDITION_ALWAYS, EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, 5, CONDITION_ALWAYS, EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, 6, CONDITION_ALWAYS, EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, 7, CONDITION_ALWAYS, EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(LinkLobby_ChooseBattleMode)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(LinkLobby_ReportWinTallies)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, EVENT(LinkLobby_OpenSanctum)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgWelcomeToBattle)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgHmItComes)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, EVENT(LinkLobby_TalkToVisitor)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(LinkLobby_TalkToVisitor)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     EVENT(LinkLobby_ChooseDebugParameterSet)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(LinkLobby_ExplainPartyOrder)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgInTheLinked)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgSoYouWin)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgYouCouldUse)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgShouldIEnter)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgMonsterBattleRules)},
    {TRIGGER_TALK, 22, CONDITION_ALWAYS, EVENT(LinkLobby_ReportMonsterRecord)},
    {TRIGGER_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgSometimesYourClass)},
    {TRIGGER_TALK, 24, CONDITION_ALWAYS, EVENT(LinkLobby_TakeSongRequest)},
    {TRIGGER_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgIDonTHaveThe)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(LinkLobby_EnterBattle)},
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(LinkLobby_LeaveArena)},
    {TRIGGER_RAISED, 1, CONDITION_ALWAYS, EVENT(LinkLobby_PromptToParticipate)},
    {TRIGGER_RAISED, 2, CONDITION_ALWAYS, EVENT(LinkLobby_ReportDisconnection)},
    {TRIGGER_TABLE_END},
};
