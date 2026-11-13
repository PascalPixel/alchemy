// ---- games/tla/src/scene/lobby/lobby.c ----
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

#include "io_reg.h"
#include "event_inline.h"

void Serial_RemoveIrqHandlers(void);
void Serial_Initialize(void);
void Event_Start(void);
void Event_End(void);
void Func_02001abc(s32 value);

void Func_02000044(void) {
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

#include "serial.h"

struct LinkPeer {
  s32 values[6];
};

extern struct LinkPeer gLinkPeerSignatures[2];

s32 Func_0200006c(s32 slot) {
  s32 id = -1;

  if ((gLinkStatus & 3) == 3) {
    id = (REG_SIOCNT << 26) >> 30;
    GameFlag_SetBit(0x303);
  } else {
    GameFlag_ClearBit(0x303);
  }
  /* FAKEMATCH(dead): a once-only loop, which places the test after the body;
   * as a plain if with no break, the test comes first, 43 of 67 instructions
   * differ in tla-en. */
  while (id >= 0 && GameFlag_Test(0x303)) {
    const s32 *expected = &gLinkLobby_SlotValues[slot];

    if (id != 0)
      GameFlag_SetBit(0x302);
    else
      GameFlag_ClearBit(0x302);
    {
      struct LinkPeer *row = &gLinkPeerSignatures[GameFlag_Test(0x302) ^ 1];

      if (row->values[gLinkLobby_SlotColumns[slot]] == *expected)
        return 1;
      break;
    }
  }
  return 0;
}

#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"

void LinkLobby_WriteSlotValue(s32 slot) {
  s32 *dst = (s32 *)gSerialTransfer.payload;
  const s32 *src = &gLinkLobby_SlotValues[slot];
  dst[gLinkLobby_SlotColumns[slot]] = *src;
}

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
};

extern s32 gData_020022b8;

/// The lobby's task each frame: counts frames without the opponent, clears
/// the peer signatures after 25, and raises the event trigger when both sides
/// are ready or the link is lost. Returns whether the peer answered.
// Ported from ☀️'s LinkLobby_PollPeerReady.
s32 Func_02000134(void) {
  struct LobbyEventWork *work;
  s32 result;
  s32 i;
  s32 set;
  s32 z;

  work = Ram_HeapSlots->eventWork;
  result = 1;
  z = Actor_GetById(4)->z;

  if (z > 0xe00000)
    Event_ClearFlag(0x304);
  if (work->raisedTrigger != 2) {
    Func_0200006c(0);
    set = Event_GetFlag(0x303);
    if (!set) {
      if (++gData_020022b8 > 25) {
        s32 offset = 0;

        // A goto loop counting down with the stride stepped by hand; as ☀️'s
        // for loop, loop.c also keeps the table and the routine's addresses
        // in registers (differ 76).
        i = 3;
      clear:
        Iwram_ClearWords((u8 *)gLinkPeerSignatures + offset, 20);
        i--;
        offset += 24;
        if (i >= 0)
          goto clear;
        gData_020022b8 = 0;
        LinkLobby_WriteSlotValue(4);
      }
    } else {
      gData_020022b8 = 0;
    }
    if (gData_020022b8 == 0) {
      if (Func_0200006c(0) && (Func_0200006c(1) || Func_0200006c(2))) {
        Event_SetFlag(0x201);
        if (Event_GetFlag(0x202))
          work->raisedTrigger = 1;
        result = 1;
      } else {
        Event_ClearFlag(0x201);
        result = 0;
      }
    }
    if (Event_GetFlag(0x201) && Event_GetFlag(0x202) && !Event_GetFlag(0x200))
      work->raisedTrigger = 1;
  }
  if ((Event_GetFlag(0x201) || Event_GetFlag(0x202)) && !Event_GetFlag(0x173) &&
      !Func_0200006c(0) && gData_020022b8 > 24) {
    work->raisedTrigger = 2;
    Event_SetFlag(0x205);
    Event_ClearFlag(0x201);
    Event_ClearFlag(0x202);
    LinkLobby_WriteSlotValue(4);
  }
  if (Event_GetFlag(0x205))
    work->raisedTrigger = 2;
  return result;
}

#include "text/msg_ids.h"

extern s32 gData_020022bc;

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

s32 LinkLobby_PromptToParticipate(void) {
  s32 set;

  set = GameFlag_Test(0x203);
  if (set != 0) {
    return set;
  }
  if (++gData_020022bc == 300) {
    gData_020022bc = 0;
    GameFlag_ClearBit(0x200);
  }
  set = Flag_Test(0x200);
  if (set != 0) {
    return set;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgIfYouWantToParticipate);
  FieldMessage_ShowAtActor(8, 0);
  wait(5);
  GameFlag_SetBit(0x200);
  Event_End();
}

s32 Func_02000344(void) {
  struct LobbyEventWork *work = Ram_HeapSlots->eventWork;

  LinkLobby_WriteSlotValue(4);
  GameFlag_ClearBit(0x200);
  GameFlag_ClearBit(0x203);
  Event_Start();
  Event_ResolvePendingActions(0);
  work->raisedTrigger = 0;
  Event_SetMessageId((s32)MsgTheLinkWas);
  FieldMessage_ShowAtActor(8, 0);
  GameFlag_ClearBit(0x205);
  Event_End();
}

#include "edition.h"
#include "owner_state.h"

s32 Serial_GetActiveTransfers(void);

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LOBBY_PREFIX_LIMIT 7
#else
#define LOBBY_PREFIX_LIMIT 4
#endif

s32 Func_0200039c(void) {
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

#include "main_routine_work.h"

/// Writes up to three active party owners to out, ending the list with 0xff.
s32 Func_02000538(u16 *out) {
  struct MainRoutineWork *party;
  s32 count = Party_CountActiveOwners();
  s32 i;

  if (count > 3)
    count = 3;
  i = 0;
  // A goto loop; as a for, while or do loop, loop.c strength-reduces the owner
  // index (differ 24).
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

#include "heap_release.h"

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

s32 Func_0200192c(void *data, s32 size);

s32 Func_02000578(void) {
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
  memberCount = Func_02000538(memberIds);
  for (i = 0; i <= 7; i++)
    peerSlots[i] = 0;
  i = 0;
  if (i >= memberCount)
    goto next;
  {
  send:
    {
      s32 tries = 0;

      Iwram_CopyWords(heap, Unit_Get(memberIds[i]), size);
      heap[0x12a] = 2;
      peerSlots[memberIds[i]] = i - 128;
      if ((transferResult = Func_0200192c(heap, size)) == -1) {
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
       * jump, which stops cross-jumping joining the test to the guard; as no
       * block, 14 of 264 instructions differ in tla-en. */
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
    if ((transferResult = Func_0200192c(heap, size)) != -1)
      goto test;
    result = transferResult;
    goto done;
  }
  Heap_Release(heap);
  size = 0x154;
  heap = Heap_AllocEwram(size);
  /* FAKEMATCH(dead): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 6 of 264 instructions differ
   * in tla-en. */
  do {
    Iwram_CopyWords(heap, Djinn_GetRecoveryTable(0), size);
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
    if ((transferResult = Func_0200192c(heap, size)) == -1) {
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

/// Resets the transfer state, as BattleLink_ResetTransferState does.
// In The Lost Age's store order.
static __inline__ void Lobby_ResetTransferState(void) {
  struct SerialTransferState *state;
  volatile u16 *ime;
  u32 saved;
  u32 zero;

  state = &gSerialTransfer;
  ime = &REG_IME;
  /* FAKEMATCH(dead): the IME exchange in a once-only block, which keeps it
   * before the zero and status constants; as plain statements, the constants
   * come first, 8 of 62 instructions of LinkLobby_ExchangePartyRecords differ in tla-en. */
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

  /* FAKEMATCH(dead): the result's zero in a once-only block, which keeps it
   * apart from the flag query that follows; as the plain result = 0, 2 of 62
   * instructions differ in tla-en. */
  do {
    result = 0;
  } while (0);
  flag = GameFlag_Test(0x302);
  gLinkExchangeState = result;
  if (!flag) {
    wait(5);
    result = Func_02000578();
    if (result < 0)
      goto fail;
    wait(5);
    first = result = Func_0200039c();
    if (result < 0)
      goto check;
  } else {
    first = result = Func_0200039c();
    if (result < 0)
      goto fail;
    wait(10);
    result = Func_02000578();
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

#include "task.h"

struct Window;

void Func_02001944(void *callback, s32 mask);
void Func_0200198c(s32 layer);
void Func_02001994(s32 layer);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
struct Window *Message_OpenWindow(const u8 *message, s32 x, s32 y, s32 flags);

extern char SceneId_Ov6b5;

// Touching the lobby counter: waits for the opponent, walks the lead to the
// arena door, swaps party records and starts the link battle.
s32 Func_02000864(void) {
  struct LobbyEventWork *work;
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
  work = Ram_HeapSlots->eventWork;
  cnt = 0;
  if (Event_GetFlag(0x173)) {
    Event_Start();
    Event_ResolvePendingActions(0);
  } else {
    if (!Event_GetFlag(0x200))
      return;
    if (Event_GetFlag(0x205))
      return;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetFlag(0x203);
    LinkLobby_WriteSlotValue(2);
    if (!Func_0200006c(2))
      window = Message_OpenWindow(MsgAwaitingYourOpponent, 5, 4, 1);
    while (!Func_0200006c(2)) {
      wait(1);
      stop = 0;
      if (!Event_GetFlag(0x201))
        stop = 1;
      if (Event_GetFlag(0x205))
        stop = 1;
      if (!Func_0200006c(2) && !Func_0200006c(1)) {
        if (++cnt > 25)
          stop = 1;
      } else {
        cnt = 0;
      }
      if (stop) {
        work->raisedTrigger = 2;
        Event_SetFlag(0x205);
        Event_ClearFlag(0x201);
        Event_ClearFlag(0x202);
        LinkLobby_WriteSlotValue(4);
        failed = 1;
        Event_ClearFlag(0x200);
        break;
      }
    }
    if (window)
      Window_Close(window, 1);
    wait(5);
  }
  if (!failed) {
    buf = HeapSlot_AllocEwram(216, 0x7c8);
    Task_RemoveFar(Func_02000134);
    Func_0200198c(5);
    wait(8);
    Func_02001994(5);
    if (Event_GetFlag(0x173)) {
      Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
      Event_SetMessageId((s32)MsgGoodLuck2);
      FieldMessage_ShowAtActor(8, 0);
      wait(45);
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_ResetAndSetPositionInMode2(4, 216, 184);
      Actor_WaitForMoveById(4);
      Actor_ResetAndSetPositionInMode2(4, 216, 168);
      Actor_WaitForMoveById(4);
    } else {
      Actor_SetSpeed(4, 0x10000, 0x8000);
      Actor_ResetAndSetPositionInMode2(4, 216, 200);
      Actor_WaitForMoveById(4);
      Actor_SetSpeed(4, 0x1999, 0xccc);
      Actor_ResetAndSetPositionInMode2(4, 216, 168);
      if (LinkLobby_ExchangePartyRecords() < 0) {
        Actor_SetSpeed(4, 0x10000, 0x8000);
        Actor_ResetAndSetPositionInMode2(4, 216, 200);
        Func_0200198c(5);
        wait(8);
        Func_02001994(5);
        Actor_WaitForMoveById(4);
        HeapSlot_Release(216);
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        Task_Add((TaskFunc)Func_02000134, TASK_PASS_VBLANK);
        Func_02001944(Func_02000134, 1);
        Event_ClearFlag(0x201);
        Event_ClearFlag(0x202);
        Event_ClearFlag(0x303);
        Event_ClearFlag(0x203);
        Event_ClearFlag(0x200);
        work->raisedTrigger = 2;
        Event_End();
        return;
      }
      Actor_SetSpeedParameters(4, 0x8000, 0x4000);
      Actor_WaitForMoveById(4);
    }
    /* FAKEMATCH(temporary): the flag test's result kept while the scene
     * symbol is loaded once for both branches; as if (Event_GetFlag(0x173)), 2
     * of 329 instructions differ in tla-en. */
    set = Event_GetFlag(0x173);
    scene = (s32)&SceneId_Ov6b5;
    if (set) {
      Party_SetFields1eeAnd1f0(scene, 8);
      Party_SetFields1f2And1f4(scene, 9);
    } else {
      Party_SetFields1eeAnd1f0(scene, 10);
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
s32 Func_02000bb0(void) {
  s32 answer;
  s32 msg;
  s32 line;

  msg = (s32)MsgPleaseSpeakTo;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
  if (Func_0200006c(0) == 0)
    wait(1);
  if (Func_0200006c(0) == 0) {
    LinkLobby_WriteSlotValue(5);
    Func_02000044();
    if (!Event_GetFlag(0x173)) {
      Event_SetMessageId(msg + 5);
      FieldMessage_ShowAtActor(8, 0);
      answer = Event_AskYesNoWithActorAnimation(4, 0);
      if (answer == 0) {
        GameFlag_SetByte(1000, 0);
        Event_SetFlag(0x173);
        Event_ClearFlag(0x172);
        Event_ClearFlag(0x16c);
        Event_SetFlag(0x202);
        line = msg + 7;
        gMainRoutineWork.linkTallies[3] = answer;
        goto message;
      }
      Event_ClearFlag(0x173);
      Event_SetFlag(0x16c);
      LinkLobby_WriteSlotValue(0);
      line = msg + 6;
      goto message;
    } else {
      line = msg + 3;
      goto message;
    }
  } else {
    if (Event_GetFlag(0x173)) {
      LinkLobby_WriteSlotValue(0);
      Event_SetMessageId((s32)MsgYourOpponentHas);
      FieldMessage_ShowAtActor(8, 0);
      Event_ClearFlag(0x202);
      Event_ClearFlag(0x173);
    }
    if (Event_GetFlag(0x202)) {
      line = msg + 3;
      // The first branch's answers jump into this block's message tail, and an
      // accepted challenge back into its opening: the native keeps one copy of
      // each, as ☀️'s lobby does.
    message:
      Event_SetMessageId(line);
    open_message:
      FieldMessage_ShowAtActor(8, 0);
    } else if (!Event_GetFlag(0x201) && !Event_GetFlag(0x300)) {
      Event_SetMessageId(msg);
      FieldMessage_ShowAtActor(8, 0);
      Event_SetFlag(0x300);
    } else {
      Event_SetFlag(0x300);
      if (Event_GetFlag(0x201))
        Event_SetMessageId(msg + 2);
      else
        Event_SetMessageId(msg + 1);
      FieldMessage_ShowAtActor(8, 0);
      if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
        if (Func_0200006c(0)) {
          Event_SetFlag(0x16c);
          Event_SetFlag(0x172);
          if (Event_GetFlag(0x201))
            Event_SetMessageId(msg + 3);
          else
            Event_SetMessageId(msg + 4);
          LinkLobby_WriteSlotValue(1);
          Event_SetFlag(0x202);
          goto open_message;
        } else {
          Event_SetFlag(0x205);
        }
      } else {
        Event_SetMessageId(msg);
        FieldMessage_ShowAtActor(8, 0);
      }
    }
  }
  Event_End();
}

void MessageArgument_ClearAll(void);
void MessageArgument_Push(s32 value, s32 digits);

// Tells the leader's win count, total or in a row by the way the lead faces.
s32 Func_02000da0(s32 actor) {
  u32 facing = Actor_GetById(4)->facing;
  u16 *count;
  s32 base;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    base = (s32)MsgItLooksLikeThisIs;
    count = &gMainRoutineWork.linkTallies[4];
    if (*count != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*count, 5);
      Event_SetMessageId(base + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgIWillTell);
      Event_ShowMessageAndWait(actor, 0);
    }
  } else {
    base = (s32)MsgItLooksLikeThisIsYour;
    count = &gMainRoutineWork.linkTallies[7];
    if (*count != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*count, 5);
      Event_SetMessageId(base + 1);
      Event_ShowMessageAndWait(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgIWillTellYou);
      Event_ShowMessageAndWait(actor, 0);
    }
  }
}

#include "input.h"

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
  Actor_SetAngleToward(actorId, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgChooseYourParameters);
  Event_ShowMessageAndWait(actorId, 0);
  window = Window_Create(0, 0, 6, 4, 2);
  choice = 0;
  shown = -1;
  for (;;) {
    if (choice != shown) {
      WindowSprite_PrepareForRedraw(window);
      Text_DrawNumber(choice, 3, window, 0, 0);
      shown = choice;
    }
    if (gInput.repeat & 0x20) {
      choice--;
    }
    if (gInput.repeat & 0x10) {
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
      Event_ShowMessageAndWait(9, 0);
    } else {
      Event_SetMessageId((s32)MsgTheSettingsAre);
      Event_ShowMessageAndWait(9, 0);
    }
  }
  wait(10);
  Event_End();
}

void Func_02000f50(s32 value) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Func_02001abc(value);
  Event_End();
}

/// Whether owner is one of the first three active owners, or the first four
/// while flag 370 is clear; an 0xff entry ends the list.
s32 Func_02000f6c(s32 owner) {
  s32 count = Party_CountActiveOwners();
  s32 limit = 3;
  s32 i;
  struct MainRoutineWork *party;

  if (!GameFlag_Test(370))
    limit = 4;
  if (count > limit)
    count = limit;
  i = 0;
  if (i >= count)
    goto none;
next:
  party = &gMainRoutineWork;
  if (party->activeOwners[i] == 0xff)
    goto none;
  /* FAKEMATCH(barrier): the empty asm with a memory clobber, so the owner byte
   * is read again for the second test; as no asm, 9 of 33 instructions differ
   * in tla-en. */
  __asm__ volatile("" ::: "memory");
  if (gMainRoutineWork.activeOwners[i] == owner)
    return 1;
  i++;
  if (i < count)
    goto next;
none:
  return 0;
}

void Func_02000fb8(void) {}
#undef LOBBY_PREFIX_LIMIT

// ---- games/tla/src/scene/lobby/s0fbc.c ----
extern u8 MsgYouHaveA[];
extern u8 MsgComeBackWhen[];
extern u16 gData_02001b02[];
extern s16 gData_02003860;

void Event_Start(void);
void Event_End(void);
s32 SoundFlag_Test(s32 song);
s32 Window_Create(s32 x, s32 y, s32 width, s32 height, s32 flags);
void WindowSprite_PrepareForRedraw(s32 window);
void Text_DrawNumber(s32 value, s32 digits, s32 window, s32 x, s32 y);
void Window_Close(s32 window, s32 release);

void Func_02000fbc(s32 actor) {
  s32 window;
  s32 song;
  s32 shown;
  s32 step;
  s32 all;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgYouHaveA);
  Event_ShowMessageAndWait(actor, 0);
  song = 0;
  all = 0;
  if ((gInput.held & 0x248) == 0x248)
    all = 1;
  Sound_Play(78);
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
      if (gInput.repeat & 32)
        step--;
      if (gInput.repeat & 16)
        step = 1;
      while (step != 0) {
        song += step;
        if (song < 0)
          song = 96;
        if ((u32)song > 96)
          song = 0;
        if (all != 0 || gMainRoutineWork.unk22d[2] != 0 || gData_02003860 != 0)
          break;
        if (SoundFlag_Test(gData_02001b02[song]) != 0)
          goto keys;
      }
    keys:
      if (gInput.pressed & 1)
        break;
      if (gInput.pressed & 2) {
        song = -1;
        break;
      }
      wait(1);
    }
    Window_Close(window, 1);
    if (song < 0)
      break;
    Sound_Play(gData_02001b02[song]);
  }
  Sound_Play(78);
  wait(120);
  Sound_Play(0x2c5);
  /* FAKEMATCH(dead): the statement in a once-only block, whose end holds
   * the order around it; as the plain statement, 2 of 153 instructions differ
   * in tla-en. */
  do {
    Event_SetMessageId((s32)MsgComeBackWhen);
  } while (0);
  Event_ShowMessageAndWait(actor, 0);
  wait(10);
  Event_End();
}

// ---- games/tla/src/scene/lobby/talk.c ----
#include "scene_ids.h"

s32 Func_02000f6c(s32 id);
void Serial_RemoveIrqHandlers(void);
void Map_CenterOnFollowedPoint(void);
void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                    s32 width, s32 height);
void MessageArgument_Push(s32 value, s32 digits);
void Func_020019ec(void);
void Event_Start(void);
void Event_End(void);
void Event_SetMessageId(s32 message);
void Event_ShowMessageAndWait(s32 actor, s32 flags);
/* VIEW(talk): routine Func_080d47b4 as void Func_080d47b4(s32 actor, s32 animation, s32 flags); event.h has s32 Func_080d47b4(s32 actorId, s32 emote, s32 frames); tried as the definition's struct BattleEffectWatchedObject *Func_080d47b4(s32 resourceId, s32 flags, s32 state), it does not compile in tla-ja: games/tla/src/scene/lobby.c:1069: conflicting types for `Func_080d47b4'. */
extern void Func_080d47b4__talk(s32 actor, s32 animation, s32 flags) __asm__("Func_080d47b4");
s32 Func_02001a84(s32 scene, s32 entrance);
void Event_ResolvePendingActions(s32 skip);
void Actor_SetAngleToward(s32 actor, s32 target, s32 frames);
void FieldMessage_Show(s32 no, s32 flags);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.

s32 Func_02001130(s32 actor) {
  s32 base = (s32)MsgItSOur;
  s32 lead = Func_02000f6c(4);
  s32 own = Func_02000f6c(actor);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  if (GameFlag_Test(0x304)) {
    GameFlag_Test(0x2f4);
    base = GameFlag_Test(actor + 0x2f0);
    if (GameFlag_Test(0x305)) {
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
  Event_ShowMessageAndWait(actor, 0);
  if (base == (s32)MsgILlFight && actor == 5) {
    Func_080d47b4__talk(5, 263, 0);
    Event_Wait(10);
  }
  Event_End();
}

/// Step trigger 1, the way out: stops the link, loads the normal reverb code
/// and goes back to the File Screen (its entrance FILE_SCREEN_FROM_SANCTUM).
s32 LinkLobby_LeaveArena(void) {
  Serial_RemoveIrqHandlers();
  Sound_LoadPresetParameters(2);
  /* FAKEMATCH(dead): the return in a once-only block, which loads the scene
   * number before the 1; as the plain return, the 1 comes first, 2 of 8
   * instructions differ in tla-en. */
  do {
    return Func_02001a84((s32)&SceneId_Ov64a, 1);
  } while (0);
}

s32 Func_0200122c(s32 actor) {
  s32 message;

  if (actor == 14 && (gInput.held & 0x300)) {
    Func_02000fbc(14);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  if (GameFlag_Test(0x206)) {
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
    GameFlag_SetBit(0x206);
  }
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actor, 0);
  Event_Wait(1);
  if (message == (s32)MsgYouLookA) {
    Actor_StartRepeatedMotion(14, 1);
    Actor_SetModeById(14, 4);
    Event_Wait(15);
    Actor_SetAngleToward(14, gMainRoutineWork.currentOwner, 0);
    Event_Wait(5);
    FieldMessage_ShowAtActor(14, 0);
  }
  Event_End();
}

s32 Func_02001304(s32 actor) {
  s32 message;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  if (!Flag_Test(0x204)) {
    if (Party_CountActiveOwners() == 8)
      message = (s32)MsgUpToThree;
    else if (Party_CountActiveOwners() <= 3)
      message = (s32)MsgUpToThreeAllies;
    else
      message = (s32)MsgUpToThreeAlliesCan;
    GameFlag_SetBit(0x204);
  } else {
    message = (s32)MsgYouCanChange;
    GameFlag_ClearBit(0x204);
  }
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actor, 0);
  Event_End();
}

s32 Func_02001388(s32 id) {
  struct MainRoutineWork *gs;

  Event_Start();
  Event_ResolvePendingActions(0);
  gs = &gMainRoutineWork;
  Actor_SetAngleToward(id, gs->currentOwner, 0);
  if (gs->linkTallies[2] != 0) {
    MessageArgument_Push(gs->linkTallies[2], 5);
    Event_SetMessageId((s32)MsgYouVeWon);
  } else {
    Event_SetMessageId((s32)MsgICanTWaitTo);
  }
  FieldMessage_ShowAtActor(id, 0);
  Event_End();
}

// ---- games/tla/src/scene/lobby/scene_events.c ----
const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }


s32 Func_02000f6c(s32 id);
void Serial_RemoveIrqHandlers(void);
void Map_CenterOnFollowedPoint(void);
void MessageArgument_Push(s32 value, s32 digits);
void Func_020019ec(void);
void Event_Start(void);
void Event_End(void);
s32 Func_02001a84(s32 scene, s32 entrance);

s32 Func_020013ec(void) {
  Func_020019ec();
  FieldMessage_Show(MsgBattleResultsWere, 1);
}

s32 Func_02001400(void) {
  Func_020019ec();
  FieldMessage_Show(MsgMonsterBattleResults, 1);
}

/// Draws value, at most 999, as three digits from right to left.
void Func_02001414(s32 value) {
  s32 i;

  if (value > 999)
    value = 999;
  i = 0;
  // A goto loop; as ☀️'s for loop, loop.c keeps the stacked 1 in r7 (differ
  // 12).
next:
  Map_CopyCellPictureIndicesRect(27, value % 10, 16 - i, 8, 1, 1);
  value /= 10;
  i++;
  if (i <= 2)
    goto next;
  Map_CenterOnFollowedPoint();
}


/* VIEW(scene_events): tag struct LobbyEventWork as struct LobbyEventWork__scene_events; lobby.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/lobby.c:1307: structure has no member named `startTransition'. */
struct LobbyEventWork__scene_events {
  u8 unk000[0x16a];
  s16 raisedTrigger;
  u8 unk16c[0x40];
  s32 startTransition;
};

extern s32 gData_020022b8;
extern s32 gData_020022bc;
extern u16 gData_02003a74[];
extern u8 gAutoSleepEnabled;

s32 Func_02000134(void);
void Func_02000044(void);
void Serial_Initialize(void);
void Func_02001944(void *func, s32 mask);
void Func_02001994(s32 entry);
void Func_02001a04(s32 value);
void Sound_LoadPresetParameters(s32 preset);
void LinkLobby_WriteSlotValue(s32 value);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void GameFlag_SetByte(s32 flag, s32 value);

static inline s32 Flag_GetByte(s32 flag) {
  /* FAKEMATCH(wrapper): Flag_GetByte forwarded inline; as direct calls to
   * GameFlag_GetByte, 60 of 438 instructions differ in tla-en. */
  return GameFlag_GetByte(flag);
}

static inline void Flag_SetByte(s32 flag, s32 value) {
  /* FAKEMATCH(wrapper): Flag_SetByte forwarded inline; as direct calls to
   * GameFlag_SetByte, 17 of 438 instructions differ in tla-en. */
  GameFlag_SetByte(flag, value);
}

static inline void Map_CopyCells(s32 x, s32 y, s32 width, s32 height, s32 destX,
                                 s32 destY) {
  /* FAKEMATCH(wrapper): Map_CopyCells forwarded inline; as direct calls to
   * Map_CopyCellAttributeRect, 6 of 438 instructions differ in tla-en. */
  Map_CopyCellAttributeRect(x, y, width, height, destX, destY);
}

s32 LinkLobby_RunRoundResult(void) {
  s32 i;
  s32 wins;
  u32 score;
  struct LobbyEventWork__scene_events *work;
  void *func;

  Event_SetFlag(0x17e);
  gMainRoutineWork.packedEvents[0] = 0;
  gMainRoutineWork.packedEvents[1] = 0;
  gData_020022bc = 0;
  gData_020022b8 = 0;
  ((struct LobbyEventWork__scene_events *)Ram_HeapSlots->eventWork)->startTransition = 0x201;
  Sound_LoadPresetParameters(2);
  Func_02001414(gMainRoutineWork.linkTallies[6]);
  Map_CopyCells(11, 11, 1, 1, 13, 10);
  LinkLobby_WriteSlotValue(4);
  wait(1);
  Func_02001994(5);
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
    if (Func_02000f6c(i))
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
    v = Flag_GetByte(1000);
    if (v == 2) {
      GameFlag_SetByte(1000, 0);
      wins++;
      msg++;
    } else {
      Flag_SetByte(1000, v + 1);
    }
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    Event_SetMessageId((s32)MsgNotBadWould + msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
      if (wins > 90)
        wins = 90;
      GameFlag_SetByte(0x3f8, wins);
    } else {
      Event_ClearFlag(0x173);
      GameFlag_SetByte(0x3f8, -1);
      /* FAKEMATCH(temporary): one scalar holds the row address, then its score;
       * as the row read directly in entrances 8 and 9, 36 of 438 instructions
       * differ in tla-en. */
      score = (u32)&gMainRoutineWork.linkTallies[3];
      MessageArgument_Push(*(u16 *)score, 5);
      score = *(u16 *)score;
      if (gMainRoutineWork.linkTallies[2] < score) {
        gMainRoutineWork.linkTallies[2] = score;
        Event_SetMessageId((s32)MsgYouWonBattle);
        FieldMessage_ShowAtActor(8, 0);
        Func_02001400();
      } else {
        Event_SetMessageId((s32)MsgYouWonBattlePlease);
        FieldMessage_ShowAtActor(8, 0);
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
    Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
    /* FAKEMATCH(temporary): one scalar holds the row address, then its score;
     * as the row read directly in entrances 8 and 9, 36 of 438 instructions
     * differ in tla-en. */
    score = (u32)&gMainRoutineWork.linkTallies[3];
    MessageArgument_Push(*(u16 *)score, 5);
    score = *(u16 *)score;
    if (gMainRoutineWork.linkTallies[2] < score) {
      gMainRoutineWork.linkTallies[2] = score;
      Event_SetMessageId((s32)MsgYouWonBattle);
      FieldMessage_ShowAtActor(8, 0);
      Func_02001400();
    } else {
      Event_SetMessageId((s32)MsgYouWonBattleI);
      FieldMessage_ShowAtActor(8, 0);
    }
    gMainRoutineWork.linkTallies[3] = 0;
    Event_ClearFlag(0x173);
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
    if (Event_GetFlag(1000)) {
      work = Ram_HeapSlots->eventWork;
      Event_ClearFlag(1000);
      work->raisedTrigger = 2;
      Event_ClearFlag(0x304);
      wait(20);
      Func_02000044();
      LinkLobby_WriteSlotValue(0);
      LinkLobby_WriteSlotValue(4);
    } else {
      gMainRoutineWork.linkTallies[4]++;
      v = gMainRoutineWork.linkTallies[7] + 1;
      gMainRoutineWork.linkTallies[7] = v;
      /* FAKEMATCH(temporary): the other score branches' address slot reused; as
       * the row read directly, 8 of 438 instructions differ in tla-en. */
      score = (u32)&gMainRoutineWork.linkTallies[6];
      if (*(u16 *)score < (u16)v)
        *(u16 *)score = v;
      Func_02001414(*(u16 *)score);
      Func_020013ec();
      Event_SetFlag(0x304);
      Event_SetFlag(0x305);
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
    if ((v = Event_GetFlag(0x173)) == 0) {
      gMainRoutineWork.linkTallies[5]++;
      gMainRoutineWork.linkTallies[7] = v;
      Func_020013ec();
    }
    Event_SetFlag(0x304);
    Event_ClearFlag(0x305);
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
      Actor_SetAngleToward(8, gMainRoutineWork.currentOwner, 0);
      Event_SetMessageId((s32)MsgNoteYouCan);
      Event_ShowMessageAndWait(8, 0);
      Event_End();
    }
    gMainRoutineWork.unk244[6] = 0;
    gAutoSleepEnabled = 0;
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
  }
  func = Func_02000134;
  Task_Add(func, 0x480);
  Func_02001944(func, 1);
  if (gMainRoutineWork.entrance != 8 || !Event_GetFlag(0x173)) {
    Func_02001a04(1);
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

u16 gData_02001b02[] = {
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
    {190, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(204), FACING_SOUTH, 0, 1},
    {187, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(204), FACING_SOUTH, 1, 0},
    {201, CONDITION_ALWAYS, 1, PIXELS(152), 0, PIXELS(88), FACING_SOUTH, 1, 0},
    {192, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(272), FACING_WEST, 1, 0},
    {344, CONDITION_ALWAYS, 1, PIXELS(128), 0, PIXELS(380), FACING_SOUTH, 1, 0},
    {86, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(252), FACING_SOUTHEAST, 1, 0},
    {120, CONDITION_ALWAYS, 1, PIXELS(80), 0, PIXELS(284), FACING_EAST, 1, 0},
    {242, CONDITION_ALWAYS, 1, PIXELS(256), 0, PIXELS(384), FACING_SOUTH, 1, 0},
    {191, CONDITION_ALWAYS, 1, PIXELS(112), 0, PIXELS(216), FACING_WEST, 1, 0},
    {186, CONDITION_ALWAYS, 1, PIXELS(96), 0, PIXELS(216), FACING_EAST, 1, 0},
    {232, CONDITION_ALWAYS, 1, PIXELS(104), 0, PIXELS(384), FACING_EAST, 1, 0},
    {231, CONDITION_ALWAYS, 1, PIXELS(64), 0, PIXELS(88), FACING_SOUTH, 1, 0},
    {234, CONDITION_ALWAYS, 2, PIXELS(184), 0, PIXELS(256), FACING_NORTH, 1, 0},
    {196, CONDITION_ALWAYS, 1, PIXELS(64), 0, PIXELS(376), FACING_NORTH, 1, 0},
    {188, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_SOUTH, 1, 0},
    {223, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(136), FACING_NORTH, 1, 0},
    {187, CONDITION_ALWAYS, 1, PIXELS(72), 0, PIXELS(376), FACING_EAST, 1, 0},
    {192, CONDITION_ALWAYS, 8, PIXELS(360), 0, PIXELS(296), FACING_EAST, 1, 0},
    {0, 0x1000, 1, PIXELS(324), 0, PIXELS(216), FACING_SOUTHWEST, 1, 0},
    {1, 0x1001, 1, PIXELS(296), 0, PIXELS(168), FACING_NORTHEAST, 1, 0},
    {2, 0x1002, 1, PIXELS(352), 0, PIXELS(152), FACING_NORTH, 1, 0},
    {3, 0x1003, 1, PIXELS(352), 0, PIXELS(108), FACING_SOUTH, 1, 0},
    {6, 0x1006, 1, PIXELS(376), 0, PIXELS(136), FACING_WEST, 1, 0},
    {7, 0x1007, 1, PIXELS(328), 0, PIXELS(88), FACING_SOUTH, 1, 0},
    {5, 0x1005, 1, PIXELS(328), 0, PIXELS(136), FACING_EAST, 1, 0},
    {SCENE_TABLE_END},
};

u32 gData_02001ee0[] = {
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
    {0, 0, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 1, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 2, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 3, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 5, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 6, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 7, CONDITION_ALWAYS, EVENT(Func_02001130)},
    {0, 8, CONDITION_ALWAYS, EVENT(Func_02000bb0)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_02000da0)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000f50)},
    {0, 11, CONDITION_ALWAYS, MSGNO(MsgWelcomeToBattle)},
    {0, 12, CONDITION_ALWAYS, MSGNO(MsgHmItComes)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_0200122c)},
    {0, 14, CONDITION_ALWAYS, EVENT(Func_0200122c)},
    {0, 15, CONDITION_ALWAYS, EVENT(LinkLobby_ChooseDebugParameterSet)},
    {0, 16, CONDITION_ALWAYS, EVENT(Func_02001304)},
    {0, 17, CONDITION_ALWAYS, MSGNO(MsgInTheLinked)},
    {0, 18, CONDITION_ALWAYS, MSGNO(MsgSoYouWin)},
    {0, 19, CONDITION_ALWAYS, MSGNO(MsgYouCouldUse)},
    {0, 20, CONDITION_ALWAYS, MSGNO(MsgShouldIEnter)},
    {0, 21, CONDITION_ALWAYS, MSGNO(MsgMonsterBattleRules)},
    {0, 22, CONDITION_ALWAYS, EVENT(Func_02001388)},
    {0, 23, CONDITION_ALWAYS, MSGNO(MsgSometimesYourClass)},
    {0, 24, CONDITION_ALWAYS, EVENT(Func_02000fbc)},
    {0, 25, CONDITION_ALWAYS, MSGNO(MsgIDonTHaveThe)},
    {2, 10, CONDITION_ALWAYS, EVENT(Func_02000864)},
    {2, 1, CONDITION_ALWAYS, EVENT(LinkLobby_LeaveArena)},
    {6, 1, CONDITION_ALWAYS, EVENT(LinkLobby_PromptToParticipate)},
    {6, 2, CONDITION_ALWAYS, EVENT(Func_02000344)},
    {TRIGGERS_END},
};
