#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "serial.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "actor.h"
#include "event_inline.h"

void LinkLobby_WriteSlotValue(s32 slot) {
  s32 *dst = (s32 *)gSerialTransfer.payload;
  const s32 *src = &gLinkLobby_SlotValues[slot];
  dst[gLinkLobby_SlotColumns[slot]] = *src;
}

struct LobbyEventWork {
  u8 unk000[0x16a];
  s16 raisedTrigger;
};

struct LinkPeerSignature {
  u8 bytes[24];
};

extern struct LinkPeerSignature gLinkPeerSignatures[];
extern s32 gData_020022b8;

s32 Func_0200006c(s32 slot);

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

void Func_02001a14(void);
void Func_02001a1c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ s32 Flag_Test(s32 flag) { return GameFlag_Test(flag); }

s32 Func_020002d8(void) {
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
  Func_02001a14();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgIfYouWantToParticipate);
  FieldMessage_ShowAtActor(8, 0);
  wait(5);
  GameFlag_SetBit(0x200);
  Func_02001a1c();
}

s32 Func_02000344(void) {
  struct LobbyEventWork *work = Ram_HeapSlots->eventWork;

  LinkLobby_WriteSlotValue(4);
  GameFlag_ClearBit(0x200);
  GameFlag_ClearBit(0x203);
  Func_02001a14();
  Event_ResolvePendingActions(0);
  work->raisedTrigger = 0;
  Event_SetMessageId((s32)MsgTheLinkWas);
  FieldMessage_ShowAtActor(8, 0);
  GameFlag_ClearBit(0x205);
  Func_02001a1c();
}

#include "edition.h"
#include "owner_state.h"

s32 Func_0200193c(void);

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
    while (Func_0200193c() != 0) {
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
    while (Func_0200193c() != 0) {
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
      if (Func_0200193c() != 0)
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
  if (Func_0200193c() != 0)
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
    if (Func_0200193c() != 0)
      goto waitOffers;
    wait(1);
    wait(2);
  }
done:
  Heap_Release(heap);
  return result;
}

#include "io_reg.h"

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
   * come first, 8 of 62 instructions of Func_020007b8 differ in tla-en. */
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

s32 Func_020007b8(void) {
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
void Func_02001a8c(s32 a, s32 b);
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
    Func_02001a14();
    Event_ResolvePendingActions(0);
  } else {
    if (!Event_GetFlag(0x200))
      return;
    if (Event_GetFlag(0x205))
      return;
    Func_02001a14();
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
      if (Func_020007b8() < 0) {
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
        Func_02001a1c();
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
    Func_02001a8c(1, 1);
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
  Func_02001a1c();
}

void Func_02000044(void);

/// The lobby clerk: offers the link battle, or reports on the one waiting.
s32 Func_02000bb0(void) {
  s32 answer;
  s32 msg;
  s32 line;

  msg = (s32)MsgPleaseSpeakTo;
  Func_02001a14();
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
  Func_02001a1c();
}

void MessageArgument_ClearAll(void);
void Func_020019e4(s32 value, s32 digits);

// Tells the leader's win count, total or in a row by the way the lead faces.
s32 Func_02000da0(s32 actor) {
  u32 facing = Actor_GetById(4)->angle;
  u16 *count;
  s32 base;

  Func_02001a14();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(actor, gMainRoutineWork.currentOwner, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    base = (s32)MsgItLooksLikeThisIs;
    count = &gMainRoutineWork.linkTallies[4];
    if (*count != 0) {
      MessageArgument_ClearAll();
      Func_020019e4(*count, 5);
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
      Func_020019e4(*count, 5);
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

s32 Func_02000e4c(s32 id) {
  s32 window;
  s32 choice;
  s32 shown;

  Func_02001a14();
  Event_ResolvePendingActions(0);
  Actor_SetAngleToward(id, gMainRoutineWork.currentOwner, 0);
  Event_SetMessageId((s32)MsgChooseYourParameters);
  Event_ShowMessageAndWait(id, 0);
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
  Func_02001a1c();
}

void Func_0200191c(void);
void Func_02001914(void);
void Func_02001abc(s32 value);

void Func_02000f50(s32 value) {
  Func_02001a14();
  Event_ResolvePendingActions(0);
  Func_02001abc(value);
  Func_02001a1c();
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
