/// @file
/// The Battle Arena's lobby, The Broken Seal's arena reworked: two players
/// link, trade their parties' records and fight, or fight the monster record;
/// a visitor takes song requests.

#include "battle/battle.h"
#include "sprite_ids.h"
#include "window/message_argument.h"
#include "field/field_work.h"
#include "system/frame.h"
#include "sound/sound_driver_symbols.h"
#include "flags.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/scene_tables.h"
#include "system/io_reg.h"
#include "field/event_inline.h"
#include "system/serial.h"
#include "system/ram_buffer.h"
#include "system/iwram_call.h"
#include "field/actor.h"
#include "edition.h"
#include "game/owner_state.h"
#include "system/main_routine_work.h"
#include "system/heap_release.h"
#include "system/task.h"
#include "system/input.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

void Serial_RemoveIrqHandlers(void);
void Serial_Initialize(void);
void Event_Start(void);
void Event_End(void);
void Sanctum_Run(s32 value);
void GameFlag_SetByte(s32 flag, s32 value);

/// Stores a byte in the eight game flags from `flag`.
static inline void FieldGameFlag_SetByte(s32 flag, s32 value) {
  GameFlag_SetByte(flag, value);
}

/// Restarts the link with interrupts locked.
void LinkLobby_InitializeSerial(void) {
  u32 saved;

  IRQ_LOCK(saved);
  Serial_RemoveIrqHandlers();
  Serial_Initialize();
  IRQ_UNLOCK(saved);
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

/// The lobby's people, by the rows of gActorPlacements, as The Broken Seal's
/// Battle Arena keeps them where it has the same ones; the party's rows go to
/// their party slots.

enum LinkLobbyActorId {
  ACTOR_ATTENDANT = ACTOR_FIRST_PLACED, ///< arranges every battle
  ACTOR_LINKED_WINS_TELLER,             ///< reports the linked battles won
  ACTOR_HEALER,                         ///< opens a Sanctum's services
  ACTOR_GREETER, ///< "Welcome to the Battle Arena, where..."
  /// "Hm... It comes in like this... Then you do this, and... Whoa, that's
  /// not right..."
  ACTOR_FUMBLING_VISITOR,
  /// "You look a little scrawny...", then "I'll be rooting..."
  ACTOR_ROOTING_VISITOR,
  /// "I like my men macho"; with L or R held, takes a song request.
  ACTOR_MACHO_LOVING_VISITOR,
  ACTOR_DEBUG_HELPER, ///< loads a debug parameter set
  /// Explains the linked finals' limit of three allies, then the party order.
  ACTOR_PARTY_ORDER_VISITOR,
  /// "In the linked finals, you can't delay after your opponent selects a
  /// move."
  ACTOR_COMMAND_TIME_VISITOR,
  /// "So you win against one guy, and then you lose against the next."
  ACTOR_WIN_AND_LOSE_VISITOR,
  /// "You could use Psynergy, Djinn, even summons and items!"
  ACTOR_STRATEGY_VISITOR,
  ACTOR_WARY_VISITOR, ///< "Should I enter or not? I'm so stressed..."
  ACTOR_MONSTER_RULES_VISITOR,
  ACTOR_MONSTER_WINS_TELLER, ///< reports the most Monster Battles won in a row
  /// "Sometimes, your class changes when you unleash a Djinni."
  ACTOR_CLASS_CHANGE_VISITOR,
  ACTOR_SONG_REQUEST_TAKER, ///< "You have a song request?"
  /// "I don't have the guts to fight in the battle arena."
  ACTOR_GUTLESS_VISITOR
};

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

struct LinkPeer {
  s32 values[6];
};

extern struct LinkPeer gLinkPeerSignatures[4];

/// The words of gLinkLobby_SlotValues, by slot, as The Broken Seal's Battle
/// Arena numbers them. They are the signature, then the state a console
/// publishes.
enum LobbySlot {
  LOBBY_SLOT_SIGNATURE = 0,
  LOBBY_SLOT_ASKED = 1,     ///< has asked for a battle
  LOBBY_SLOT_IN_CIRCLE = 2, ///< stands in the circle
  LOBBY_SLOT_UNUSED = 3,
  LOBBY_SLOT_IDLE = 4,
  /// Replaces the signature when no partner answers the attendant and during
  /// a monster series.
  LOBBY_SLOT_ALONE = 5
};

/// The lobby's raised triggers (FieldWork.raisedTrigger).
enum LobbyRaisedTrigger {
  LOBBY_EVENT_NONE = 0,
  LOBBY_EVENT_PROMPT = 1,      ///< LinkLobby_PromptToParticipate
  LOBBY_EVENT_DISCONNECTED = 2 ///< LinkLobby_ReportDisconnection
};

/// Frames without a link before the lobby gives the partner up.
#define LOBBY_PEER_WAIT_FRAMES 25

/// The first of eight flags, one per character from Isaac, each set while that
/// character is in the battle party (LinkLobby_Initialize).
#define FLAG_LINK_LOBBY_IN_PARTY_FIRST FLAG_SCENE_TEMP_F0

/// Returns 1 when the partner's payload holds slot's word in slot's column, 0
/// when it does not or there is no link.
s32 LinkLobby_PeerSlotMatches(s32 slot) {
  s32 id = -1;

  if ((gLinkStatus & 3) == 3) {
    id = (REG_SIOCNT << 26) >> 30;
    Event_SetFlag(FLAG_AREA_TEMP_03);
  } else {
    Event_ClearFlag(FLAG_AREA_TEMP_03);
  }
  // At most one pass: a match returns 1, anything else breaks out.
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

/// Publishes slot's word in its column of this console's link payload, where
/// the partner's LinkLobby_PeerSlotMatches reads it.
void LinkLobby_WriteSlotValue(s32 slot) {
  s32 *dst = (s32 *)gSerialTransfer.payload;
  const s32 *src = &gLinkLobby_SlotValues[slot];
  dst[gLinkLobby_SlotColumns[slot]] = *src;
}

s32 gLinkLobby_WaitFrames BSS_DATA;
s32 gLinkLobby_CallFrames BSS_DATA;

/// The lobby's task each frame: counts frames without the opponent, clears
/// the peer signatures after 25, and raises the event trigger when both sides
/// are ready or the link is lost. Returns whether the peer answered.
s32 LinkLobby_PollPeerReady(void) {
  struct FieldWork *work;
  s32 result;
  s32 i;
  s32 set;
  FIXED z;

  work = Ram_HeapSlots->fieldWork;
  result = 1;
  z = Actor_GetById(ACTOR_FELIX)->z.fixed;

  if (z > toFIXED(224))
    Event_ClearFlag(FLAG_AREA_TEMP_04);
  if (work->raisedTrigger != LOBBY_EVENT_DISCONNECTED) {
    LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE);
    set = Event_GetFlag(FLAG_AREA_TEMP_03);
    if (!set) {
      if (++gLinkLobby_WaitFrames > LOBBY_PEER_WAIT_FRAMES) {
        for (i = 0; i < 4; i++)
          Iwram_ClearWordsEntry(&gLinkPeerSignatures[i], 20);
        gLinkLobby_WaitFrames = 0;
        LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
      }
    } else {
      gLinkLobby_WaitFrames = 0;
    }
    if (gLinkLobby_WaitFrames == 0) {
      if (LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) &&
          (LinkLobby_PeerSlotMatches(LOBBY_SLOT_ASKED) ||
           LinkLobby_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))) {
        Event_SetFlag(FLAG_SCENE_TEMP_01);
        if (Event_GetFlag(FLAG_SCENE_TEMP_02))
          work->raisedTrigger = LOBBY_EVENT_PROMPT;
        result = 1;
      } else {
        Event_ClearFlag(FLAG_SCENE_TEMP_01);
        result = 0;
      }
    }
    if (Event_GetFlag(FLAG_SCENE_TEMP_01) &&
        Event_GetFlag(FLAG_SCENE_TEMP_02) && !Event_GetFlag(FLAG_SCENE_TEMP_00))
      work->raisedTrigger = LOBBY_EVENT_PROMPT;
  }
  if ((Event_GetFlag(FLAG_SCENE_TEMP_01) ||
       Event_GetFlag(FLAG_SCENE_TEMP_02)) &&
      !Event_GetFlag(FLAG_MONSTER_BATTLE) &&
      !LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) &&
      gLinkLobby_WaitFrames > 24) {
    work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
    Event_SetFlag(FLAG_SCENE_TEMP_05);
    Event_ClearFlag(FLAG_SCENE_TEMP_01);
    Event_ClearFlag(FLAG_SCENE_TEMP_02);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  if (Event_GetFlag(FLAG_SCENE_TEMP_05))
    work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
  return result;
}

/// The attendant asks the player into the circle once both consoles agree, and
/// again after 300 calls.
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
  Event_SetMessageId((s32)MsgLobbyIfYouWantToParticipate);
  // "If you want to participate, please step into the circle!"
  Event_OpenMessage(ACTOR_ATTENDANT, 0);
  wait(5);
  Event_SetFlag(FLAG_SCENE_TEMP_00);
  Event_End();
}

/// The attendant says the link was disconnected.
s32 LinkLobby_ReportDisconnection(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  Event_ClearFlag(FLAG_SCENE_TEMP_00);
  Event_ClearFlag(FLAG_SCENE_TEMP_03);
  Event_Start();
  Event_ResolvePendingActions(0);
  work->raisedTrigger = LOBBY_EVENT_NONE;
  Event_SetMessageId((s32)MsgLobbyLinkWasDisconnected);
  // "The link was disconnected."
  Event_OpenMessage(ACTOR_ATTENDANT, 0);
  Event_ClearFlag(FLAG_SCENE_TEMP_05);
  Event_End();
}

#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LOBBY_PREFIX_LIMIT 7
#else
#define LOBBY_PREFIX_LIMIT 4
#endif

// Both sizes are whole 20-byte blocks, since the serial runtime sends 20 bytes
// a block and ends a send only when its count reaches exactly 0.
/// The bytes sent for each party member: a Unit (332 bytes) and the eight
/// bytes after it, 17 blocks of 20.
#define LOBBY_UNIT_RECORD_BYTES (17 * 20)
/// The bytes received for the Djinn recovery table after the party members,
/// 17 blocks of 20.
#define LOBBY_RECOVERY_RECORD_BYTES (17 * 20)
/// The party members a linked battle takes. A smaller party sends absent
/// records.
#define LOBBY_UNIT_RECORD_COUNT 3
/// gLinkStatus bits 0 and 1: both consoles were heard this frame.
#define LOBBY_LINK_READY_MASK 3
/// A record fails on the 25th frame it waits without a link or past the
/// timeout.
#define LOBBY_TRANSFER_RETRY_LIMIT 24
/// Frames, about ten seconds, for the three party member records, and again
/// for the recovery table.
#define LOBBY_SEND_TIMEOUT_FRAMES 600
/// Frames, about fifteen seconds, for all four records received.
#define LOBBY_RECEIVE_TIMEOUT_FRAMES 900

/// gMainRoutineWork.linkTallies, as the lobby keeps them.
enum LinkTally {
  LINK_TALLY_MONSTER_WINS = 0,
  LINK_TALLY_MONSTER_LOSSES = 1,
  LINK_TALLY_MONSTER_BEST_STREAK = 2, ///< the most won in a row
  LINK_TALLY_MONSTER_STREAK = 3,      ///< those won in the current series
  LINK_TALLY_LINKED_WINS = 4,
  LINK_TALLY_LINKED_LOSSES = 5,
  /// The most linked battles won in a row, on the wall board.
  LINK_TALLY_LINKED_BEST_STREAK = 6,
  LINK_TALLY_LINKED_STREAK = 7 ///< those won in a row now
};

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
  struct Unit *unit;

  size = LOBBY_UNIT_RECORD_BYTES;
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = LOBBY_RECEIVE_TIMEOUT_FRAMES;
  for (slot = 0;; slot++) {
    if (slot >= LOBBY_UNIT_RECORD_COUNT)
      goto second;
    unit = Unit_Get(slot + 128);
    transfer = Serial_StartReceive(unit);
    tries = 0;
    if (transfer == -1)
      break;
    while (Serial_GetActiveTransfers() != 0) {
      if (gSerialReceivedSize > size) {
        result = -1;
        goto done;
      }
      wait(1);
      if (--timeout < 0 ||
          (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
        if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
          result = -1;
          goto done;
        }
      }
    }
    if (gSerialReceivedSize != size) {
      result = -1;
      goto done;
    }
    if (unit->side != BATTLE_SIDE_NONE)
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
    for (i = UNIT_NAME_SIZE - 1; i >= length; i--)
      unit->name[i] = unit->name[i - length];
    for (i = 0; i < length; i++)
      unit->name[i] = namePrefix[i];
    unit->name[UNIT_NAME_SIZE - 1] = 0;
  }
  goto failed;
  for (;;) {
  second:
    Heap_Release(heap);
    size = LOBBY_RECOVERY_RECORD_BYTES;
    heap = Heap_AllocEwram(size);
    transfer = Serial_StartReceive(Djinn_GetRecoveryTable(1));
    tries = 0;
    if (transfer == -1)
      break;
    while (Serial_GetActiveTransfers() != 0) {
      s32 limit = LOBBY_RECOVERY_RECORD_BYTES;

      if (gSerialReceivedSize > limit) {
        result = -1;
        goto done;
      }
      wait(1);
      if (--timeout < 0 ||
          (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
        if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
          result = -1;
          goto done;
        }
      }
    }
    if (gSerialReceivedSize != LOBBY_RECOVERY_RECORD_BYTES)
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
    u8 owner = party->party[i];

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
  struct DjinnRecoveryList *list;

  size = LOBBY_UNIT_RECORD_BYTES;
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = LOBBY_SEND_TIMEOUT_FRAMES;
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
      ((struct Unit *)heap)->side = BATTLE_SIDE_PARTY;
      peerSlots[memberIds[i]] = i - 128;
      if ((transferResult = Serial_StartSend(heap, size)) == -1) {
        result = transferResult;
        goto done;
      }
      goto testMember;
    waitMember:
      wait(1);
      if (--timeout < 0 ||
          (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
        if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
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
  if (--timeout < 0 ||
      (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
    if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
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
  if (i < LOBBY_UNIT_RECORD_COUNT) {
    ((struct Unit *)heap)->side = tries = 0;
    if ((transferResult = Serial_StartSend(heap, size)) != -1)
      goto test;
    result = transferResult;
    goto done;
  }
  Heap_Release(heap);
  size = LOBBY_RECOVERY_RECORD_BYTES;
  heap = Heap_AllocEwram(size);
  IWRAM_COPY_WORDS(heap, Djinn_GetRecoveryTable(0), size);
  {
    s32 timeout;
    s32 tries;

    list = &((struct DjinnRecoveryTable *)heap)->list;
    tries = 0;
    timeout = LOBBY_SEND_TIMEOUT_FRAMES;
    for (j = 0; j < list->count; j++) {
      list->entries[j].unitId = peerSlots[list->entries[j].unitId];
      if (list->entries[j].unitId == 0) {
        for (k = j; k < list->count - 1; k++)
          list->entries[k] = list->entries[k + 1];
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
    if (--timeout < 0 ||
        (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
      if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
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
static __inline__ void Lobby_ResetTransferState(void) {
  struct SerialTransferState *state;
  u32 saved;

  state = &gSerialTransfer;
  IRQ_LOCK(saved);
  state = &gSerialTransfer;
  state->status = 0x80;
  *(u8 *)&state->peerFlags = 0;
  gSerialSendSource = 0;
  *(u8 *)&state->flags = 0;
  gSerialSendSize = 0;
  gSerialReceiveDestination = 0;
  gSerialReceivedSize = 0;
  IRQ_UNLOCK(saved);
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
  FieldGameFlag_SetByte(FLAG_LINK_PARTY_COUNT, first);
  result = first;
check:
  if (first < 0) {
  fail:
    Lobby_ResetTransferState();
  }
  return result;
}

void TilesetAnim_Resume(s32 layer);
void TilesetAnim_Pause(s32 layer);
void Field_RequestBattleFromEncounterTable(s32 a, s32 b);
struct Window *Message_OpenWindow(const u8 *message, s32 x, s32 y, s32 flags);

/// Touching the lobby counter: waits for the opponent, walks the lead to the
/// arena door, swaps party records and starts the link battle.
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
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IN_CIRCLE);
    if (!LinkLobby_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))
      window = Message_OpenWindow(MsgLobbyAwaitingYourOpponent, 5, 4, 1);
    while (!LinkLobby_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE)) {
      wait(1);
      stop = 0;
      if (!Event_GetFlag(FLAG_SCENE_TEMP_01))
        stop = 1;
      if (Event_GetFlag(FLAG_SCENE_TEMP_05))
        stop = 1;
      if (!LinkLobby_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE) &&
          !LinkLobby_PeerSlotMatches(LOBBY_SLOT_ASKED)) {
        if (++cnt > 25)
          stop = 1;
      } else {
        cnt = 0;
      }
      if (stop) {
        work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
        Event_SetFlag(FLAG_SCENE_TEMP_05);
        Event_ClearFlag(FLAG_SCENE_TEMP_01);
        Event_ClearFlag(FLAG_SCENE_TEMP_02);
        LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
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
    buf = HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, 0x7c8);
    Task_Remove(LinkLobby_PollPeerReady);
    TilesetAnim_Resume(5);
    wait(8);
    TilesetAnim_Pause(5);
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
      Event_SetMessageId((s32)MsgLobbyGoodLuck);
      // "Good luck!"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      wait(45);
      Actor_SetSpeed(ACTOR_FELIX, 1);
      Actor_WalkTo(ACTOR_FELIX, 216, 184);
      Actor_WaitForMoveById(ACTOR_FELIX);
      Actor_WalkTo(ACTOR_FELIX, 216, 168);
      Actor_WaitForMoveById(ACTOR_FELIX);
    } else {
      Actor_SetSpeed(ACTOR_FELIX, 1);
      Actor_WalkTo(ACTOR_FELIX, 216, 200);
      Actor_WaitForMoveById(ACTOR_FELIX);
      Actor_SetSpeed(ACTOR_FELIX, 0.1);
      Actor_WalkTo(ACTOR_FELIX, 216, 168);
      if (LinkLobby_ExchangePartyRecords() < 0) {
        Actor_SetSpeed(ACTOR_FELIX, 1);
        Actor_WalkTo(ACTOR_FELIX, 216, 200);
        TilesetAnim_Resume(5);
        wait(8);
        TilesetAnim_Pause(5);
        Actor_WaitForMoveById(ACTOR_FELIX);
        HeapSlot_Release(HEAP_SLOT_ENEMY_UNITS);
        LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
        LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
        Task_Add((TaskFunc)LinkLobby_PollPeerReady, TASK_PASS_VBLANK);
        Task_SetFlags(LinkLobby_PollPeerReady, 1);
        Event_ClearFlag(FLAG_SCENE_TEMP_01);
        Event_ClearFlag(FLAG_SCENE_TEMP_02);
        Event_ClearFlag(FLAG_AREA_TEMP_03);
        Event_ClearFlag(FLAG_SCENE_TEMP_03);
        Event_ClearFlag(FLAG_SCENE_TEMP_00);
        work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
        Event_End();
        return;
      }
      Actor_SetSpeed(ACTOR_FELIX, 0.5);
      Actor_WaitForMoveById(ACTOR_FELIX);
    }
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Event_SetAfterBattleDestination(SCENE_LOBBY, 8);
      Event_SetDefeatDestination(SCENE_LOBBY, 9);
    } else {
      Event_SetAfterBattleDestination(SCENE_LOBBY, 10);
      Event_SetDefeatDestination(SCENE_LOBBY, 11);
    }
    gMainRoutineWork.battleType = BATTLE_TYPE_LINK;
    Field_RequestBattleFromEncounterTable(1, 1);
    // EXEC fills the payload's first four halfwords, where the link battle's
    // command entry looks for it in the partner's.
    table = (u16 *)gSerialTransfer.payload;
    table[0] = 'E';
    table[1] = 'X';
    table[2] = 'E';
    table[3] = 'C';
    {
      /* FAKEMATCH(temporary): the bound in a local, and the destination
       * indexed afresh each byte. */
      u32 last = 1991;

      i = 0;
      src = buf;
      do {
        u8 *dst = Ram_MapCellBuffer + 0x8000 + i;
        u8 b = *src;

        i++;
        src++;
        *dst = b;
      } while (i <= last);
    }
    HeapSlot_Release(HEAP_SLOT_ENEMY_UNITS);
  }
  Event_End();
}

/// The lobby clerk: offers the link battle, or reports on the one waiting.
s32 LinkLobby_ChooseBattleMode(void) {
  s32 answer;
  s32 msg;
  s32 line;

  msg = (s32)MsgLobbyPleaseSpeakToMeWhen;
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
  if (LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0)
    wait(1);
  if (LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0) {
    LinkLobby_WriteSlotValue(LOBBY_SLOT_ALONE);
    LinkLobby_InitializeSerial();
    if (!Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Event_SetMessageId(msg + 5);
      // "You do not have an opponent. Would you like to battle a monster?"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      answer = Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0);
      if (answer == 0) {
        FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, 0);
        Event_SetFlag(FLAG_MONSTER_BATTLE);
        Event_ClearFlag(FLAG_LINK_BATTLE_ARRANGED);
        Event_ClearFlag(FLAG_LINK_BATTLE);
        Event_SetFlag(FLAG_SCENE_TEMP_02);
        line = msg + 7;
        gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = answer;
        goto message;
      }
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
      Event_SetFlag(FLAG_LINK_BATTLE);
      LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      line = msg + 6;
      goto message;
    } else {
      line = msg + 3;
      goto message;
    }
  } else {
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      Event_SetMessageId((s32)MsgLobbyYourOpponentHasArrived);
      // "Your opponent has arrived!"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      Event_ClearFlag(FLAG_SCENE_TEMP_02);
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
    }
    if (Event_GetFlag(FLAG_SCENE_TEMP_02)) {
      line = msg + 3;
    message:
      Event_SetMessageId(line);
    open_message:
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
    } else if (!Event_GetFlag(FLAG_SCENE_TEMP_01) &&
               !Event_GetFlag(FLAG_AREA_TEMP_00)) {
      Event_SetMessageId(msg);
      // "Please speak to me when you want to fight in a battle."
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      Event_SetFlag(FLAG_AREA_TEMP_00);
    } else {
      Event_SetFlag(FLAG_AREA_TEMP_00);
      if (Event_GetFlag(FLAG_SCENE_TEMP_01))
        Event_SetMessageId(msg + 2);
      else
        Event_SetMessageId(msg + 1);
      // "Would you like to apply?"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
        if (LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE)) {
          Event_SetFlag(FLAG_LINK_BATTLE);
          Event_SetFlag(FLAG_LINK_BATTLE_ARRANGED);
          if (Event_GetFlag(FLAG_SCENE_TEMP_01))
            Event_SetMessageId(msg + 3);
          else
            Event_SetMessageId(msg + 4);
          LinkLobby_WriteSlotValue(LOBBY_SLOT_ASKED);
          Event_SetFlag(FLAG_SCENE_TEMP_02);
          goto open_message;
        } else {
          Event_SetFlag(FLAG_SCENE_TEMP_05);
        }
      } else {
        Event_SetMessageId(msg);
        // "Please speak to me when you want to fight in a battle."
        Event_OpenMessage(ACTOR_ATTENDANT, 0);
      }
    }
  }
  Event_End();
}

void MessageArgument_ClearAll(void);
void MessageArgument_Push(s32 value, s32 digits);

/// Tells the leader's win count, total or in a row by the way the lead faces.
s32 LinkLobby_ReportWinTallies(s32 actor) {
  u32 facing = Actor_GetById(ACTOR_FELIX)->facing;
  u16 *count;
  s32 base;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actor, gMainRoutineWork.leaderActorId, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    base = (s32)MsgLobbyItLooksLike;
    count = &gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_WINS];
    if (*count != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*count, MESSAGE_ARGUMENT_NUMBER);
      Event_SetMessageId(base + 1);
      // "You've won ... Linked Battle. Keep up the good work!"
      Event_ShowMessage(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgLobbyIWillTell);
      // "I will tell you your total wins when you finish the linked finals."
      Event_ShowMessage(actor, 0);
    }
  } else {
    base = (s32)MsgItLooksLikeThisIsYour;
    count = &gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK];
    if (*count != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*count, MESSAGE_ARGUMENT_NUMBER);
      Event_SetMessageId(base + 1);
      // "You've won ... Linked Battle. Check the board on the wall for..."
      Event_ShowMessage(actor, 0);
    } else {
      Event_SetMessageId((s32)MsgLobbyIWillTellYouYourConsecutive);
      // "I will tell you your consecutive wins when you finish the finals."
      Event_ShowMessage(actor, 0);
    }
  }
}

s32 Window_Create();
void WindowSprite_PrepareForRedraw();
void Text_DrawNumber();
s32 DebugParameterSet_Load();

/// Talking to ACTOR_DEBUG_HELPER loads a debug parameter set.
s32 LinkLobby_ChooseDebugParameterSet(s32 actorId) {
  s32 window;
  s32 choice;
  s32 shown;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actorId, gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId((s32)MsgLobbyChooseYourParametersHere);
  // "Choose your parameters here."
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
      ok = DebugParameterSet_Load(choice);
    } else {
      message = (s32)MsgLobbyWhatIsThisJustTease;
      goto show;
    }
    // The refusal jumps into the unknown-preset branch, so the two share one
    // message call.
    if (ok != 0) {
      message = (s32)MsgLobbyTheresNoSuchParameterAsk;
    show:
      Event_SetMessageId(message);
      // "There's no such parameter. Ask Mr. Sugimoto."
      Event_ShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
    } else {
      Event_SetMessageId((s32)MsgLobbySettingsAreMadeRest);
      // "The settings are made. The rest is up to you."
      Event_ShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
    }
  }
  wait(10);
  Event_End();
}

/// The lobby's healer opens the sanctum.
void LinkLobby_OpenSanctum(s32 actor) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sanctum_Run(actor);
  Event_End();
}

/// Whether owner is one of the first three active owners, or the first four
/// while FLAG_LINK_BATTLE_ARRANGED is clear; an 0xff entry ends the list.
s32 LinkLobby_PartyContains(s32 owner) {
  s32 count = Party_CountMembers();
  s32 limit = 3;
  s32 i;

  if (!Event_GetFlag(FLAG_LINK_BATTLE_ARRANGED))
    limit = 4;
  if (count > limit)
    count = limit;
  for (i = 0; i < count; i++) {
    if (gMainRoutineWork.party[i] == 0xff)
      return 0;
    if (gMainRoutineWork.party[i] == owner)
      return 1;
  }
  return 0;
}

/// Unused: does nothing; nothing calls it.
UNUSED void LinkLobby_DoNothing(void) {}
#undef LOBBY_PREFIX_LIMIT

extern u8 MsgLobbyYouHaveSongRequest[];
extern u8 MsgLobbyComeBackWhenYoureReady[];
extern u16 gLinkLobby_RequestableSounds[];

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
  Actor_FaceActor(actor, gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId((s32)MsgLobbyYouHaveSongRequest);
  // "You have a song request?"
  Event_ShowMessage(actor, 0);
  song = 0;
  all = 0;
  if ((gInput.held & (KEY_START | KEY_UP | KEY_L)) ==
      (KEY_START | KEY_UP | KEY_L))
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
        if (all != 0 || gMainRoutineWork.sendFlag != 0 || gAnyFileCleared != 0)
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
  Event_PlaySound(MUS_LEMURIA_SPRING);
  /* FAKEMATCH(dead): the statement in a once-only block. */
  do {
    Event_SetMessageId((s32)MsgLobbyComeBackWhenYoureReady);
  } while (0);
  // "Come back when you're ready to rock!"
  Event_ShowMessage(actor, 0);
  wait(10);
  Event_End();
}

s32 LinkLobby_PartyContains(s32 id);
void Serial_RemoveIrqHandlers(void);
void MapGrid_CenterOnFollowedPoint(void);
void MapGrid_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX,
                                        s32 destY, s32 width, s32 height);
void MessageArgument_Push(s32 value, s32 digits);
void Save_ProcessSelectedFile(void);
void Event_Start(void);
void Import_EventSetDestination(s32 scene, s32 entrance);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void FieldMessage_Show(s32 no, s32 flags);

/// Talking to a party member waiting in the lobby: what they say follows the
/// last linked battle.
s32 LinkLobby_TalkByProgress(s32 actor) {
  s32 base = (s32)MsgLobbyItsOurTurnLetsGo;
  s32 lead = LinkLobby_PartyContains(ACTOR_FELIX);
  s32 own = LinkLobby_PartyContains(actor);

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actor, gMainRoutineWork.leaderActorId, 0);
  if (Event_GetFlag(FLAG_AREA_TEMP_04)) {
    Event_GetFlag(FLAG_LINK_LOBBY_IN_PARTY_FIRST + ACTOR_FELIX);
    base = Event_GetFlag(actor + FLAG_LINK_LOBBY_IN_PARTY_FIRST);
    if (Event_GetFlag(FLAG_AREA_TEMP_05)) {
      if (base)
        base = (s32)MsgLobbyEverybodyDeservesLittleCredit;
      else
        base = (s32)MsgLobbyEverybodyDidGreatIDidnt;
    } else {
      if (base)
        base = (s32)MsgLobbyDontBlameYourself;
      else
        base = (s32)MsgLobbyYouWinSomeYouLose;
    }
  } else if (lead != 0) {
    if (own == 0)
      base = (s32)MsgOkWeRe;
  } else {
    base = (s32)MsgLobbyIllFightEnoughForBoth;
  }
  Event_SetMessageId(base + actor);
  Event_ShowMessage(actor, 0);
  if (base == (s32)MsgLobbyIllFightEnoughForBoth && actor == 5) {
    FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    Event_Wait(10);
  }
  Event_End();
}

/// Step trigger 1, the way out: stops the link, loads the normal reverb code
/// and goes back to the File Screen (its entrance FILE_SCREEN_FROM_SANCTUM).
s32 LinkLobby_LeaveArena(void) {
  Serial_RemoveIrqHandlers();
  Sound_LoadReverbCode(SOUND_REVERB_NORMAL);
  Import_EventSetDestination(SCENE_CLEAR, 1);
#ifdef UBFIX
  return 0;
#endif
}

/// The visitors' talk, by whether the party has fought here
/// (FLAG_SCENE_TEMP_06); the macho-loving visitor takes song requests while L
/// or R is held.
s32 LinkLobby_TalkToVisitor(s32 actor) {
  s32 message;

  if (actor == ACTOR_MACHO_LOVING_VISITOR && (gInput.held & KEYS_SHOULDERS)) {
    LinkLobby_TakeSongRequest(ACTOR_MACHO_LOVING_VISITOR);
    return;
  }
  Event_Start();
  Event_ResolvePendingActions(0);
  if (Event_GetFlag(FLAG_SCENE_TEMP_06)) {
    switch (actor) {
    case ACTOR_ROOTING_VISITOR:
      message = (s32)MsgLobbyIllBeRooting;
      break;
    case ACTOR_MACHO_LOVING_VISITOR:
      message = (s32)MsgLobbyILikeMyMenMacho;
      break;
    default:
      message = (s32)MsgLobbySometimesYourClassChangesWhen;
      break;
    }
  } else {
    switch (actor) {
    case ACTOR_ROOTING_VISITOR:
      message = (s32)MsgLobbyYouLookLittleScrawny;
      break;
    case ACTOR_MACHO_LOVING_VISITOR:
      message = (s32)MsgLobbyILikeMyMenMacho;
      break;
    default:
      message = (s32)MsgLobbySometimesYourClassChangesWhen;
      break;
    }
    Event_SetFlag(FLAG_SCENE_TEMP_06);
  }
  Event_SetMessageId(message);
  // "Sometimes, your class changes when you unleash a Djinni. You need to..."
  Event_OpenMessage(actor, 0);
  Event_Wait(1);
  if (message == (s32)MsgLobbyYouLookLittleScrawny) {
    Actor_StartRepeatedMotion(ACTOR_MACHO_LOVING_VISITOR, 1);
    Actor_SetAnimation(ACTOR_MACHO_LOVING_VISITOR, 4);
    Event_Wait(15);
    Actor_FaceActor(ACTOR_MACHO_LOVING_VISITOR, gMainRoutineWork.leaderActorId,
                    0);
    Event_Wait(5);
    Event_OpenMessage(ACTOR_MACHO_LOVING_VISITOR, 0);
  }
  Event_End();
}

/// Talking to ACTOR_PARTY_ORDER_VISITOR, who explains how up to three allies
/// are chosen for a linked battle, by the party's size.
s32 LinkLobby_ExplainPartyOrder(s32 actor) {
  s32 message;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_FaceActor(actor, gMainRoutineWork.leaderActorId, 0);
  if (!Event_GetFlag(FLAG_SCENE_TEMP_04)) {
    if (Party_CountMembers() == 8)
      message = (s32)MsgLobbyUpToThreeAlliesCan;
    else if (Party_CountMembers() <= 3)
      message = (s32)MsgLobbyUpToThreeAlliesCanFight;
    else
      message = (s32)MsgLobbyUpToThreeAlliesCanFightInLinked;
    Event_SetFlag(FLAG_SCENE_TEMP_04);
  } else {
    message = (s32)MsgLobbyYouCanChangeOrder;
    Event_ClearFlag(FLAG_SCENE_TEMP_04);
  }
  Event_SetMessageId(message);
  // "You can change the order of your party in the status screen. But you..."
  Event_OpenMessage(actor, 0);
  Event_End();
}

/// Talking to ACTOR_MONSTER_WINS_TELLER, who reports the most Monster Battles
/// won in a row, or before any, that he will count them.
s32 LinkLobby_ReportMonsterRecord(s32 id) {
  struct MainRoutineWork *gs;

  Event_Start();
  Event_ResolvePendingActions(0);
  gs = &gMainRoutineWork;
  Actor_FaceActor(id, gs->leaderActorId, 0);
  if (gs->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] != 0) {
    MessageArgument_Push(gs->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK],
                         MESSAGE_ARGUMENT_NUMBER);
    Event_SetMessageId((s32)MsgLobbyYouveWonConsecutiveMonsterBattle);
  } else {
    Event_SetMessageId((s32)MsgLobbyICantWaitToSee);
  }
  // "I can't wait to see how many Monster Battles you win. I'll be..."
  Event_OpenMessage(id, 0);
  Event_End();
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 LinkLobby_PartyContains(s32 id);
void MapGrid_CenterOnFollowedPoint(void);
void MessageArgument_Push(s32 value, s32 digits);
void Save_ProcessSelectedFile(void);
void Event_End(void);

/// Saves and reports the link battle's results.
s32 LinkLobby_SaveBattleResults(void) {
  Save_ProcessSelectedFile();
  FieldMessage_Show(MsgLobbyBattleResultsWereSaved, 1);
}

/// Saves and reports the monster battle's results.
s32 LinkLobby_SaveMonsterBattleResults(void) {
  Save_ProcessSelectedFile();
  FieldMessage_Show(MsgLobbyMonsterBattleResultsWereSaved, 1);
}

/// Draws value, at most 999, as three digits from right to left.
void LinkLobby_DrawThreeDigitValue(s32 value) {
  s32 i;

  if (value > 999)
    value = 999;
  for (i = 0; i <= 2; i++) {
    MapGrid_CopyCellsTo(27, value % 10, 16 - i, 8, 1, 1);
    value /= 10;
  }
  MapGrid_CenterOnFollowedPoint();
}

void Event_End(void);

s32 LinkLobby_PollPeerReady(void);
void LinkLobby_InitializeSerial(void);
void Serial_Initialize(void);
void TilesetAnim_Pause(s32 entry);
void Party_RestoreHpPp(s32 value);
void Sound_LoadReverbCode(s32 preset);
void LinkLobby_WriteSlotValue(s32 slot);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
/// Sets the lobby up as The Broken Seal's BattleArena_Initialize sets the arena
/// up.
s32 LinkLobby_Initialize(void) {
  s32 i;
  s32 wins;
  u32 score;
  struct FieldWork *work;
  void *func;

  Event_SetFlag(FLAG_IN_BATTLE_ARENA);
  gMainRoutineWork.firstShortcut = 0;
  gMainRoutineWork.secondShortcut = 0;
  gLinkLobby_CallFrames = 0;
  gLinkLobby_WaitFrames = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Sound_LoadReverbCode(2);
  LinkLobby_DrawThreeDigitValue(
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
  MapGrid_CopyCellAttributes(11, 11, 1, 1, 13, 10);
  LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  wait(1);
  TilesetAnim_Pause(5);
  {
    u16 *name = (u16 *)gSerialTransfer.payload;

    name[4] = 'T';
    name[5] = 'A';
    name[6] = 'L';
    name[7] = 'K';
  }
  for (i = 0; i < 8; i++) {
    s32 flag = FLAG_LINK_LOBBY_IN_PARTY_FIRST + i;

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
    LinkLobby_WriteSlotValue(LOBBY_SLOT_ALONE);
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_WINS]++;
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK]++;
    wins = (s8)GameFlag_GetByte(FLAG_MONSTER_BATTLE_WINS);
    msg = wins * 2 + 2;
    if (msg > 14)
      msg = 14;
    v = FieldGameFlag_GetByte(FLAG_AREA_TEMP_E8);
    if (v == 2) {
      FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, 0);
      wins++;
      msg++;
    } else {
      FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, v + 1);
    }
    Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
    Event_SetMessageId((s32)MsgLobbyNotBadWouldYouLike + msg);
    Event_OpenMessage(ACTOR_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      if (wins > 90)
        wins = 90;
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, wins);
    } else {
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
      // score holds the row's address, then the score read through it.
      score = (u32)&gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
      MessageArgument_Push(*(u16 *)score, MESSAGE_ARGUMENT_NUMBER);
      score = *(u16 *)score;
      if (gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
          score) {
        gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] = score;
        Event_SetMessageId((s32)MsgLobbyYouWonBattleYouBroke);
        // "You won ... battle. You broke the record!"
        Event_OpenMessage(ACTOR_ATTENDANT, 0);
        LinkLobby_SaveMonsterBattleResults();
      } else {
        Event_SetMessageId((s32)MsgLobbyYouWonBattlePleaseSpeak);
        // "You won ... battle. Please speak to me when you wish to battle..."
        Event_OpenMessage(ACTOR_ATTENDANT, 0);
      }
      LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    }
    Event_End();
  } else if (gMainRoutineWork.entrance == 9) {
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_LOSSES]++;
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(LOBBY_SLOT_ALONE);
    Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
    // score holds the row's address, then the score read through it.
    score = (u32)&gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
    MessageArgument_Push(*(u16 *)score, MESSAGE_ARGUMENT_NUMBER);
    score = *(u16 *)score;
    if (gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] < score) {
      gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] = score;
      Event_SetMessageId((s32)MsgLobbyYouWonBattleYouBroke);
      // "You won ... battle. You broke the record!"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      LinkLobby_SaveMonsterBattleResults();
    } else {
      Event_SetMessageId((s32)MsgLobbyYouWonBattleITold);
      // "You won ... battle. I told you our monsters were tough! Please..."
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
    }
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = 0;
    Event_ClearFlag(FLAG_MONSTER_BATTLE);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    Event_End();
  } else if (gMainRoutineWork.entrance == 10) {
    s32 v;

    Event_Start();
    Event_ResolvePendingActions(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
    if (Event_GetFlag(FLAG_AREA_TEMP_E8)) {
      work = Ram_HeapSlots->fieldWork;
      Event_ClearFlag(FLAG_AREA_TEMP_E8);
      work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
      Event_ClearFlag(FLAG_AREA_TEMP_04);
      wait(20);
      LinkLobby_InitializeSerial();
      LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
    } else {
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_WINS]++;
      v = gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK] + 1;
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
      score = (u32)&gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK];
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
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
    if ((v = Event_GetFlag(FLAG_MONSTER_BATTLE)) == 0) {
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_LOSSES]++;
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
      LinkLobby_SaveBattleResults();
    }
    Event_SetFlag(FLAG_AREA_TEMP_04);
    Event_ClearFlag(FLAG_AREA_TEMP_05);
    Event_End();
  } else {
    Serial_Initialize();
    Event_ClearFlag(FLAG_LINK_BATTLE_ARRANGED);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    if (gMainRoutineWork.idleSleepEnabled != 0) {
      Event_Start();
      Event_ResolvePendingActions(0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
      Event_SetMessageId((s32)MsgLobbyNoteYouCantUseSleep);
      // "Note: you can't use the sleep function in battle stages!"
      Event_ShowMessage(ACTOR_ATTENDANT, 0);
      Event_End();
    }
    gMainRoutineWork.idleSleepEnabled = 0;
    gIdleSleepEnabled = 0;
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  func = LinkLobby_PollPeerReady;
  Task_Add(func, TASK_PASS_VBLANK);
  Task_SetFlags(func, 1);
  if (gMainRoutineWork.entrance != 8 || !Event_GetFlag(FLAG_MONSTER_BATTLE)) {
    Party_RestoreHpPp(1);
    DjinnRecovery_CountDownTwenty();
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// A word of four characters, the first in its lowest byte, as The Broken
/// Seal's Battle Arena writes them.
#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

/// The words the lobby publishes in its link payload, by enum LobbySlot; the
/// partner compares them with its own (LinkLobby_PeerSlotMatches).
// clang-format off
s32 gLinkLobby_SlotValues[] = {
#if (LANGUAGE == LANGUAGE_JAPANESE)
    PEER_TAG('C', 'M', 'g', '2'), PEER_TAG('0', '1', '2', '3'),
    PEER_TAG('A', 'B', 'C', '1'), PEER_TAG('C', 'D', 'E', '2'),
    PEER_TAG('E', 'F', 'G', '3'), PEER_TAG('S', 'G', 'M', 'C')
#elif (LANGUAGE == LANGUAGE_GERMAN)
    PEER_TAG('C', 's', '5', 'G'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'G', 'S', 'C')
#elif (LANGUAGE == LANGUAGE_SPANISH)
    PEER_TAG('C', '7', 's', 'G'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'S', 'S', 'C')
#elif (LANGUAGE == LANGUAGE_FRENCH)
    PEER_TAG('c', 'S', 'G', '2'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('M', 'G', 'S', 'C')
#elif (LANGUAGE == LANGUAGE_ITALIAN)
    PEER_TAG('s', 'G', 'I', '1'), PEER_TAG('0', '1', '2', '3'),
    PEER_TAG('A', 'B', 'C', '1'), PEER_TAG('C', 'D', 'E', '2'),
    PEER_TAG('E', 'F', 'G', '3'), PEER_TAG('C', 'I', 'G', 'S')
#else
    PEER_TAG('s', 'G', '1', 'C'), PEER_TAG('0', '1', '2', '3'),
    PEER_TAG('A', 'B', 'C', '1'), PEER_TAG('C', 'D', 'E', '2'),
    PEER_TAG('E', 'F', 'G', '3'), PEER_TAG('S', 'G', 'M', 'C')
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
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 204, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 272, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 380, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_STRIPED_TOP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 252, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 284, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 216, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 216, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 384, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 184, 0, 256, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 376, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 376, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, 8, 360, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_ISAAC)), MOVEMENT_TYPE_STAND, 324, 0, 216, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_GARET)), MOVEMENT_TYPE_STAND, 296, 0, 168, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_IVAN)), MOVEMENT_TYPE_STAND, 352, 0, 152, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_MIA)), MOVEMENT_TYPE_STAND, 352, 0, 108, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_SHEBA)), MOVEMENT_TYPE_STAND, 376, 0, 136, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_PIERS)), MOVEMENT_TYPE_STAND, 328, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_JENNA)), MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

/// Unused: a placement table nothing reads.
struct ActorPlacement gLinkLobby_UnusedPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 204, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 304, 0, 168, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 272, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, FLAG_IN_PARTY(ACTOR_ISAAC), MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_STRIPED_TOP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 92, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IZUMO_WOMAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 116, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 256, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, FLAG_IN_PARTY(ACTOR_GARET), MOVEMENT_TYPE_STAND, 352, 0, 108, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, FLAG_IN_PARTY(ACTOR_IVAN), MOVEMENT_TYPE_STAND, 352, 0, 156, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, FLAG_IN_PARTY(ACTOR_MIA), MOVEMENT_TYPE_STAND, 376, 0, 136, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_SQUASH, 360, 0, 296, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ISAAC, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_ISAAC)), MOVEMENT_TYPE_STAND, 328, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_GARET, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_GARET)), MOVEMENT_TYPE_STAND, 352, 0, 108, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_IVAN, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_IVAN)), MOVEMENT_TYPE_STAND, 352, 0, 156, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MIA, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_MIA)), MOVEMENT_TYPE_STAND, 376, 0, 136, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_SHEBA)), MOVEMENT_TYPE_STAND, 152, 0, 144, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_PIERS)), MOVEMENT_TYPE_STAND, 104, 0, 144, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, IF_FLAG_SET(FLAG_IN_PARTY(ACTOR_JENNA)), MOVEMENT_TYPE_STAND, 128, 0, 124, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_TALK, ACTOR_ISAAC, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_GARET, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_IVAN, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_MIA, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_JENNA, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_SHEBA, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_PIERS, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_ATTENDANT, CONDITION_ALWAYS,
     EVENT(LinkLobby_ChooseBattleMode)},
    {TRIGGER_TALK, ACTOR_LINKED_WINS_TELLER, CONDITION_ALWAYS,
     EVENT(LinkLobby_ReportWinTallies)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
     EVENT(LinkLobby_OpenSanctum)},
    {TRIGGER_TALK, ACTOR_GREETER, CONDITION_ALWAYS,
     MSGNO(MsgLobbyWelcomeToBattleArenaWhere)},
    {TRIGGER_TALK, ACTOR_FUMBLING_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbyHmItComesInLike)},
    {TRIGGER_TALK, ACTOR_ROOTING_VISITOR, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkToVisitor)},
    {TRIGGER_TALK, ACTOR_MACHO_LOVING_VISITOR, CONDITION_ALWAYS,
     EVENT(LinkLobby_TalkToVisitor)},
    {TRIGGER_TALK, ACTOR_DEBUG_HELPER, CONDITION_ALWAYS,
     EVENT(LinkLobby_ChooseDebugParameterSet)},
    {TRIGGER_TALK, ACTOR_PARTY_ORDER_VISITOR, CONDITION_ALWAYS,
     EVENT(LinkLobby_ExplainPartyOrder)},
    {TRIGGER_TALK, ACTOR_COMMAND_TIME_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbyInLinkedFinalsYouCant)},
    {TRIGGER_TALK, ACTOR_WIN_AND_LOSE_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbySoYouWinAgainstOne)},
    {TRIGGER_TALK, ACTOR_STRATEGY_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbyYouCouldUsePsynergyDjinn)},
    {TRIGGER_TALK, ACTOR_WARY_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbyShouldIEnterOrNot)},
    {TRIGGER_TALK, ACTOR_MONSTER_RULES_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbyMonsterBattleRulesSay)},
    {TRIGGER_TALK, ACTOR_MONSTER_WINS_TELLER, CONDITION_ALWAYS,
     EVENT(LinkLobby_ReportMonsterRecord)},
    {TRIGGER_TALK, ACTOR_CLASS_CHANGE_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbySometimesYourClassChangesWhen)},
    {TRIGGER_TALK, ACTOR_SONG_REQUEST_TAKER, CONDITION_ALWAYS,
     EVENT(LinkLobby_TakeSongRequest)},
    {TRIGGER_TALK, ACTOR_GUTLESS_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbyIDontHaveGuts)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(LinkLobby_EnterBattle)},
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(LinkLobby_LeaveArena)},
    {TRIGGER_RAISED, LOBBY_EVENT_PROMPT, CONDITION_ALWAYS,
     EVENT(LinkLobby_PromptToParticipate)},
    {TRIGGER_RAISED, LOBBY_EVENT_DISCONNECTED, CONDITION_ALWAYS,
     EVENT(LinkLobby_ReportDisconnection)},
    {TRIGGER_TABLE_END},
};
