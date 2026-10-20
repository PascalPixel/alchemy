/// @file
/// The Battle Arena overlay: the lobby where linked and monster battles
/// start, the exchange of the two parties, the attendant and the records of
/// wins.

#include "unit/import.h"
#include "unit/menu/lobby.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/battle/battle.h"
#include "unit/field/field.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "unit/runtime/runtime.h"
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
#include "field_service.h"
#include "scene.h"
#include "io_reg.h"
#include "serial_runtime.h"
#include "iwram_call.h"
#include "character.h"
#include "mem.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"
#include "scene_ids.h"
#include "game_state.h"
#include "battle_party.h"
#include "irqlock.h"
#include "field_scene.h"
#include "frame.h"
#include "flags.h"
#include "global_cells.h"
#include "songs.h"

// Link transfers use three unit records followed by one recovery-table record.
#define LOBBY_UNIT_RECORD_BYTES 0x154
#define LOBBY_RECOVERY_RECORD_BYTES 0x140
#define LOBBY_UNIT_RECORD_COUNT 3
#define LOBBY_LINK_READY_MASK 3
#define LOBBY_TRANSFER_RETRY_LIMIT 24
#define LOBBY_SEND_TIMEOUT_FRAMES 600
#define LOBBY_RECEIVE_TIMEOUT_FRAMES 900
#define LOBBY_PEER_WAIT_FRAMES 25

// The lobby's temporary flags:
// FLAG_SCENE_TEMP_00: the attendant has asked the player to take part; every
//   300th call of LinkLobby_PromptToParticipate clears it, to ask again.
// FLAG_SCENE_TEMP_01: a linked partner is there (its signatures match).
// FLAG_SCENE_TEMP_02: the player has agreed to a battle.
// FLAG_SCENE_TEMP_03: the party is on its way into the battle.
// FLAG_SCENE_TEMP_04: the attendant's next explanation is the party order.
// FLAG_SCENE_TEMP_05: the link was lost, and the attendant has yet to say so.

#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

// clang-format off
const s32 gLinkLobby_SlotValues[] = {
#if (LANGUAGE == LANGUAGE_GERMAN)
    PEER_TAG('C', 'S', 'G', 'G'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'G', 'S', 'C')
#elif (LANGUAGE == LANGUAGE_SPANISH)
    PEER_TAG('C', 'S', 'S', 'G'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'S', 'S', 'C')
#elif (LANGUAGE == LANGUAGE_FRENCH)
    PEER_TAG('C', 'S', 'G', 'M'), PEER_TAG('3', '0', '1', '2'),
    PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
    PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('C', 'S', 'G', 'M')
#elif (LANGUAGE == LANGUAGE_ITALIAN)
    PEER_TAG('S', 'G', 'I', 'C'), PEER_TAG('0', '1', '2', '3'),
    PEER_TAG('A', 'B', 'C', '1'), PEER_TAG('C', 'D', 'E', '2'),
    PEER_TAG('E', 'F', 'G', '3'), PEER_TAG('C', 'I', 'G', 'S')
#else
#if (LANGUAGE != LANGUAGE_JAPANESE)
    PEER_TAG('S', 'G', 'M', 'C'),
#else
    PEER_TAG('C', 'M', 'G', 'S'),
#endif
    PEER_TAG('0', '1', '2', '3'), PEER_TAG('A', 'B', 'C', '1'),
    PEER_TAG('C', 'D', 'E', '2'), PEER_TAG('E', 'F', 'G', '3'),
    PEER_TAG('S', 'G', 'M', 'C')
#endif
};
// clang-format on

#undef PEER_TAG

/// The partner console's signature as the lobby reads it: a word for each
/// slot column.
struct LinkPeer {
  s32 values[6];
};

static s32 sLinkLobbyWaitFrames;

/// What a lobby handler's early exit returns. The ROM's s32 handlers return
/// nothing there, leaving whatever the last call put in r0 (UB); UBFIX
/// returns 0, as the function ends that fall off do.
#ifdef UBFIX
#define LOBBY_NO_RESULT 0
#else
#define LOBBY_NO_RESULT
#endif

u8 *LinkLobby_GetEntrances(void) { return (u8 *)gLinkLobbyEntrances; }

s32 SceneData_ReturnZero(void) { return 0; }

/// Reinitializes the serial runtime while interrupts are locked.
/// The original s32 entry produces no defined result; its callers ignore it.
static s32 LinkLobby_InitializeSerial(void) {
  u32 savedIrqState;

  // Returns no value: the ROM has the s32 epilogue (pop {r1}; bx r1) and no
  // result.

  IRQ_LOCK(savedIrqState);
  SerialRuntime_RemoveIrqHandlers();
  SerialRuntime_Initialize();
  IRQ_UNLOCK(savedIrqState);
#ifdef UBFIX
  return 0;
#endif
}

u8 *LinkLobby_GetExits(void) { return (u8 *)gLinkLobbyExits; }

/// Selects the post-link-battle placements for entrances nine and eleven.
s32 LinkLobby_SelectPlacements(void) {
  s16 entrance = gGameState.entrance;

  if (entrance == 11 || entrance == 9) {
    return (s32)gLinkLobbyBattlePlacements;
  }
  return (s32)gLinkLobbyPlacements;
}

/// Checks one language-specific peer signature slot and updates link-side
/// flags.
static s32 LinkLobby_PeerSlotMatches(s32 slot) {
  s32 serialPlayerId = -1;

  if ((gLinkStatus & LOBBY_LINK_READY_MASK) == LOBBY_LINK_READY_MASK) {
    serialPlayerId = (REG_SIOCNT << 26) >> 30;
    Engine_GameFlagSet(0x303);
  } else {
    Engine_GameFlagClear(0x303);
  }
  /* FAKEMATCH(one-pass): a loop left by break after its first pass, which
   * places the test after the body as the ROM does; as an if around the body,
   * 12 of 58 instructions differ in tbs-en. */
  while (serialPlayerId >= 0 && Engine_GameFlagIsSet(0x303)) {
    const s32 *expected = &gLinkLobby_SlotValues[slot];

    if (serialPlayerId != 0)
      Engine_GameFlagSet(0x302);
    else
      Engine_GameFlagClear(0x302);
    {
      struct LinkPeer *peer = (struct LinkPeer *)
          gLinkPeerSignatures[Engine_GameFlagIsSet(0x302) ^ 1];

      if (peer->values[gLinkLobby_SlotColumns[slot]] == *expected)
        return 1;
      break;
    }
  }
  return 0;
}

/// Publishes a protocol slot value in the outgoing signature payload.
static void LinkLobby_WriteSlotValue(s32 slot) {
  s32 *dst = (s32 *)gSerialTransfer.payload;
  const s32 *src = &gLinkLobby_SlotValues[slot];
  dst[gLinkLobby_SlotColumns[slot]] = *src;
}

// The scheduler ignores this result. Keep the native status calculation in
// matching builds: making the function void removes eight instructions. The
// modern build uses the scheduler's actual callback type to avoid an
// incompatible indirect call.
#ifdef UBFIX
/// Polls peer readiness, raises arrival/disconnect events and refreshes
/// signatures.
static void LinkLobby_PollPeerReady(void) {
#else
static s32 LinkLobby_PollPeerReady(void) {
#endif
  struct EventWork *work;
  s32 result;
  s32 i;

  work = gEventWork;
  result = 1;
  if (Object_GetById(0)->z.fixed > 0xe00000) {
    Engine_GameFlagClear(0x304);
  }
  if (work->raisedTrigger != 2) {
    LinkLobby_PeerSlotMatches(0);
    if (!Engine_GameFlagIsSet(0x303)) {
      if (++sLinkLobbyWaitFrames > LOBBY_PEER_WAIT_FRAMES) {
        for (i = 0; i < 4; i++) {
          Iwram_ClearWords(&gLinkPeerSignatures[i], 20);
        }
        sLinkLobbyWaitFrames = 0;
        LinkLobby_WriteSlotValue(4);
      }
    } else {
      sLinkLobbyWaitFrames = 0;
    }
    if (sLinkLobbyWaitFrames == 0) {
      if (LinkLobby_PeerSlotMatches(0) &&
          (LinkLobby_PeerSlotMatches(1) || LinkLobby_PeerSlotMatches(2))) {
        Engine_GameFlagSet(FLAG_SCENE_TEMP_01);
        if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02)) {
          work->raisedTrigger = 1;
        }
        result = 1;
      } else {
        Engine_GameFlagClear(FLAG_SCENE_TEMP_01);
        result = 0;
      }
    }
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) &&
        Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02) &&
        !Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00)) {
      work->raisedTrigger = 1;
    }
  }
  if ((GameFlag_IsSet(FLAG_SCENE_TEMP_01) ||
       GameFlag_IsSet(FLAG_SCENE_TEMP_02)) &&
      !GameFlag_IsSet(FLAG_MONSTER_BATTLE) && !LinkLobby_PeerSlotMatches(0) &&
      sLinkLobbyWaitFrames > 24) {
    work->raisedTrigger = 2;
    GameFlag_Set(FLAG_SCENE_TEMP_05);
    GameFlag_Clear(FLAG_SCENE_TEMP_01);
    GameFlag_Clear(FLAG_SCENE_TEMP_02);
    LinkLobby_WriteSlotValue(4);
  }
  if (GameFlag_IsSet(FLAG_SCENE_TEMP_05)) {
    work->raisedTrigger = 2;
  }
#ifdef UBFIX
  (void)result;
#else
  return result;
#endif
}

u32 gLinkLobbyCallFrames;

/// Prompts for participation unless entering battle or a prompt is still
/// pending. Retains the original handler's missing result on the message path.
static s32 LinkLobby_PromptToParticipate(void) {
  s32 set;

  set = Engine_GameFlagIsSet(FLAG_SCENE_TEMP_03);
  if (set != 0) {
    return set;
  }
  if (++gLinkLobbyCallFrames == 300) {
    gLinkLobbyCallFrames = 0;
    Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  }
  set = GameFlag_IsSet(FLAG_SCENE_TEMP_00);
  if (set != 0) {
    return set;
  }
  Engine_EventBegin();
  Engine_EventSetMessage(MsgLobbyWantParticipatePlease);
  Engine_EventOpenMessage(8, 0);
  Engine_TaskWait(5);
  Engine_GameFlagSet(FLAG_SCENE_TEMP_00);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Resets the disconnect trigger and displays the attendant's warning.
/// The original handler has no defined return value.
static s32 LinkLobby_ReportDisconnection(void) {
  struct EventWork *work = gEventWork;

  LinkLobby_WriteSlotValue(4);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_03);
  Engine_EventBegin();
  work->raisedTrigger = 0;
  Engine_EventSetMessage(MsgLobbyLinkDisconnected);
  Engine_EventOpenMessage(8, 0);
  Engine_GameFlagClear(FLAG_SCENE_TEMP_05);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Receives three unit records and a recovery table; returns the occupied count
/// or the original transfer failure. All size checks observe fresh serial
/// reads.
static s32 LinkLobby_ReceivePartyRecords(void) {
  /* FAKEMATCH(volatile): gSerialReceivedSize is volatile (serial_runtime.h),
   * so every read here is a load of its own; without the qualifier, 24 of 190
   * instructions differ in tbs-en. */
  u16 namePrefix[24];
  u32 size;
  void *heap;
  s32 result;
  s32 slot;
  s32 timeout;
  s32 tries;
  u8 *record;
  s32 transferResult;
  s32 prefixLength;
  s32 nameIndex;

  size = LOBBY_UNIT_RECORD_BYTES;
  heap = Runtime_BumpAllocateAlternatePool(size);
  result = 0;
  timeout = LOBBY_RECEIVE_TIMEOUT_FRAMES;
  slot = 0;
  goto next;
wait1:
  if (gSerialReceivedSize > size) {
    result = -1;
    goto done;
  }
  Engine_TaskWait(1);
  if (--timeout < 0 ||
      (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
    if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
      result = -1;
      goto done;
    }
  }
test1:
  if (SerialRuntime_GetActiveTransfers() != 0) {
    goto wait1;
  }
  if (gSerialReceivedSize != size) {
    result = -1;
    goto done;
  }
  if (record[0x12a] != 0) {
    result++;
  }
  Engine_TaskWait(2);
  FieldUi_AdjustValueWithoutLimit(MsgEnemyLabel, namePrefix);
  nameIndex = 0;
  if (namePrefix[nameIndex] != 0) {
    do {
      nameIndex++;
      if (nameIndex > LOBBY_PREFIX_LIMIT) {
        break;
      }
    } while (namePrefix[nameIndex] != 0);
  }
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
  namePrefix[nameIndex] = ' ';
  prefixLength = nameIndex + 1;
#else
  prefixLength = nameIndex;
#endif
  for (nameIndex = 14; nameIndex >= prefixLength; nameIndex--) {
    record[nameIndex] = record[nameIndex - prefixLength];
  }
  for (nameIndex = 0; nameIndex < prefixLength; nameIndex++) {
    record[nameIndex] = namePrefix[nameIndex];
  }
  record[14] = 0;
  slot++;
next:
  if (slot > 2) {
    goto second;
  }
  record = (u8 *)UnitGet(slot + 128);
  tries = 0;
  if ((transferResult = SerialRuntime_BeginReceive(record)) == -1) {
    goto failed;
  }
  goto test1;
wait2:
  transferResult = gSerialReceivedSize;
  if (transferResult > LOBBY_RECOVERY_RECORD_BYTES) {
    result = -1;
    goto done;
  }
  Engine_TaskWait(1);
  if (--timeout < 0 ||
      (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
    if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
      result = -1;
      goto done;
    }
  }
test2:
  if (SerialRuntime_GetActiveTransfers() != 0) {
    goto wait2;
  }
  if (gSerialReceivedSize != LOBBY_RECOVERY_RECORD_BYTES) {
    result = -1;
  } else {
    Engine_TaskWait(2);
  }
  goto done;
second:
  Runtime_BumpFree(heap);
  size = LOBBY_RECOVERY_RECORD_BYTES;
  heap = Runtime_BumpAllocateAlternatePool(size);
  tries = 0;
  if ((transferResult =
           SerialRuntime_BeginReceive(Djinn_GetRecoveryTable(1))) != -1) {
    goto test2;
  }
failed:
  result = transferResult;
done:
  Runtime_BumpFree(heap);
  return result;
}

/// Copies at most three active owner IDs, terminates with 0xff, and returns
/// count. Passing a null destination only counts the entries.
static s32 LinkLobby_CopyPartyOwnerIds(u16 *ownerIds) {
  s32 ownerCount = Party_CountActiveOwners();
  if (ownerCount > LOBBY_UNIT_RECORD_COUNT)
    ownerCount = LOBBY_UNIT_RECORD_COUNT;
  if (ownerCount > 0) {
#ifndef UBFIX
    s16 *partyAddress = (s16 *)&gGameState;
#endif
    const u8 *source;
    s32 remaining;
#ifdef UBFIX
    // The original short-pointer addition traverses a struct as an array.
    // The actual member avoids that UB; used plainly, it removes four native
    // bytes (0x44 to 0x40), so matching builds retain the old address form.
    source = gGameState.activeOwners;
#else
    partyAddress += 252;
    source = (const u8 *)partyAddress;
#endif
    remaining = ownerCount;
    do {
      u8 ownerId = *source++;
      if (ownerIds != 0) {
        *ownerIds = ownerId;
        ownerIds++;
      }
      remaining--;
    } while (remaining != 0);
  }
  if (ownerIds != 0)
    *ownerIds = 0x00ff;
  return ownerCount;
}

/// Sends up to three active owners, pads missing records, then sends recovery
/// data. Returns zero on completion or the original serial failure.
static s32 LinkLobby_SendPartyRecords(void) {
  // Measured plain form: replacing only the final offer-transfer wait3/test3
  // polling region with while(active transfers) adds 8 bytes in tbs-en.
  s32 result;
  u8 *slotMap;
  u16 ownerIds[8];
  u8 peerSlots[8];
  s32 transferResult;
  u8 *heap;
  s32 ownerCount;
  s32 timeout;
  s32 tries;
  s32 i;
  s32 j;
  s32 k;
  u32 size;
  struct DjinnRecoveryList *recoveryList;

  size = LOBBY_UNIT_RECORD_BYTES;
  heap = Runtime_BumpAllocateAlternatePool(size);
  result = 0;
  timeout = LOBBY_SEND_TIMEOUT_FRAMES;
  ownerCount = LinkLobby_CopyPartyOwnerIds(ownerIds);
  slotMap = peerSlots;
  for (i = 7; i >= 0; i--) {
    slotMap[i] = 0;
  }
  for (i = 0; i < ownerCount; i++) {
    /* FAKEMATCH(one-pass): the record's send in a one-pass block, which keeps
     * the send failure inside the loop and gives the copy's argument order;
     * as a plain block, 3 of 255 instructions differ in tbs-en. */
    do {
      Iwram_CopyWords(heap, UnitGet(ownerIds[i]), size);
      heap[0x12a] = 2;
      slotMap[ownerIds[i]] = i - 128;
      transferResult = SerialRuntime_BeginSend(heap, size);
      tries = 0;
      if (transferResult == -1) {
        result = transferResult;
        goto done;
      }
    } while (0);
    while (SerialRuntime_GetActiveTransfers() != 0) {
      Engine_TaskWait(1);
      if (--timeout < 0 ||
          (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
        /* FAKEMATCH(one-pass): the retry limit's test in a one-pass block,
         * which keeps this failure inside the loop, its -1 loaded here;
         * without the block, 11 of 255 instructions differ in tbs-en. */
        do {
          if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
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
  if (--timeout < 0 ||
      (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
    if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
      result = -1;
      goto done;
    }
  }
test:
  if (SerialRuntime_GetActiveTransfers() != 0) {
    goto wait;
  }
  Engine_TaskWait(2);
  i++;
next:
  if (i <= 2) {
    heap[0x12a] = tries = 0;
    if ((transferResult = SerialRuntime_BeginSend(heap, size)) != -1) {
      goto test;
    }
    result = transferResult;
    goto done;
  }
  Runtime_BumpFree(heap);
  size = LOBBY_RECOVERY_RECORD_BYTES;
  heap = Runtime_BumpAllocateAlternatePool(size);
  /* FAKEMATCH(one-pass): the recovery table's copy in a one-pass block, which
   * gives the copy's argument order and keeps the list base's copy first after
   * the call; as a plain call, 8 of 255 instructions differ in tbs-en. */
  do {
    Iwram_CopyWords(heap, Djinn_GetRecoveryTable(0), size);
  } while (0);
  {
    s32 timeout;
    s32 tries;

    recoveryList = &((struct DjinnRecoveryTable *)heap)->list;
    tries = 0;
    timeout = LOBBY_SEND_TIMEOUT_FRAMES;
    for (j = 0; j < recoveryList->count; j++) {
      recoveryList->entries[j].unitId =
          peerSlots[recoveryList->entries[j].unitId];
      if (recoveryList->entries[j].unitId == 0) {
        for (k = j; k < recoveryList->count - 1; k++) {
          recoveryList->entries[k] = recoveryList->entries[k + 1];
        }
        recoveryList->count--;
        j--;
      }
    }
    if ((transferResult = SerialRuntime_BeginSend(heap, size)) == -1) {
      result = transferResult;
      goto done;
    }
    goto test3;
  wait3:
    Engine_TaskWait(1);
    if (--timeout < 0 ||
        (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
      if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
        result = -1;
        goto done;
      }
    }
  test3:
    if (SerialRuntime_GetActiveTransfers() != 0) {
      goto wait3;
    }
    Engine_TaskWait(1);
    Engine_TaskWait(2);
  }
done:
  Runtime_BumpFree(heap);
  return result;
}

/// Orders send/receive by link side and records the received owner count.
/// A failed exchange resets the transfer state.
static s32 LinkLobby_ExchangePartyRecords(void) {
  s32 result;
  s32 receivedOwnerCount;
  s32 receiveFirst;

  /* FAKEMATCH(one-pass): result's zero in a one-pass block, which loads it
   * into r6 ahead of the call as the ROM does; as a plain statement, 4 of 63
   * instructions differ in tbs-en. */
  do {
    result = 0;
  } while (0);
  receiveFirst = Engine_GameFlagIsSet(0x302);
  gLinkExchangeState = result;
  if (!receiveFirst) {
    Engine_TaskWait(5);
    result = LinkLobby_SendPartyRecords();
    if (result < 0)
      goto fail;
    Engine_TaskWait(5);
    receivedOwnerCount = result = LinkLobby_ReceivePartyRecords();
    if (result < 0)
      goto check;
  } else {
    receivedOwnerCount = result = LinkLobby_ReceivePartyRecords();
    if (result < 0)
      goto fail;
    Engine_TaskWait(10);
    result = LinkLobby_SendPartyRecords();
    if (result < 0)
      goto fail;
  }
  FieldGameFlag_SetByte(FLAG_LINK_PARTY_COUNT, receivedOwnerCount);
  result = receivedOwnerCount;
check:
  if (receivedOwnerCount < 0) {
  fail:
    SerialRuntime_ResetTransferState();
  }
  return result;
}

/// Waits for the opponent, exchanges parties and starts the arena battle.
/// Disconnects restore the lobby callback and actor position; original returns
/// have no defined status.
static s32 LinkLobby_EnterBattle(void) {
  struct EventWork *work;
  s32 failed;
  struct UiWindow *window;
  s32 missedFrames;
  s32 cancelEntry;
  u8 *savedWork;
  u8 *source;
  u8 *destination;
  u16 *signature;
  u32 i;

  failed = 0;
  window = 0;
  work = gEventWork;
  missedFrames = 0;
  if (Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
    Engine_EventBegin();
  } else {
    if (!Engine_GameFlagIsSet(FLAG_SCENE_TEMP_00))
      return LOBBY_NO_RESULT;
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_05))
      return LOBBY_NO_RESULT;
    Engine_EventBegin();
    Engine_GameFlagSet(FLAG_SCENE_TEMP_03);
    LinkLobby_WriteSlotValue(2);
    if (!LinkLobby_PeerSlotMatches(2))
      window = UiText_OpenMessageWindow(MsgLobbyAwaitingOpponent, 5, 4, 1);
    while (!LinkLobby_PeerSlotMatches(2)) {
      Engine_TaskWait(1);
      cancelEntry = 0;
      if (!Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01))
        cancelEntry = 1;
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_05))
        cancelEntry = 1;
      if (!LinkLobby_PeerSlotMatches(2) && !LinkLobby_PeerSlotMatches(1)) {
        if (++missedFrames > LOBBY_PEER_WAIT_FRAMES)
          cancelEntry = 1;
      } else {
        missedFrames = 0;
      }
      if (cancelEntry) {
        work->raisedTrigger = 2;
        Engine_GameFlagSet(FLAG_SCENE_TEMP_05);
        Engine_GameFlagClear(FLAG_SCENE_TEMP_01);
        Engine_GameFlagClear(FLAG_SCENE_TEMP_02);
        LinkLobby_WriteSlotValue(4);
        failed = 1;
        Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
        break;
      }
    }
    if (window)
      UiWork_Finalize(window, 1);
    Engine_TaskWait(5);
  }
  // Suspend peer polling while transferring the party and entering battle.
  if (!failed) {
    savedWork = Runtime_AllocateBlock(54, 0x7c8);
    Engine_TaskRemoveCallback((SchedulerCallback)LinkLobby_PollPeerReady);
    Map_ClearLayerEntryFlag(5);
    Engine_TaskWait(8);
    Map_SetLayerEntryFlag(5);
    if (Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
      Engine_ActorFaceActor(8, gGameState.selectedActor, 0);
      Engine_EventSetMessage(MsgLobbyGoodLuck);
      Engine_EventOpenMessage(8, 0);
      Engine_TaskWait(45);
      Actor_SetSpeed(0, 0x10000, 0x8000);
      Engine_ActorWalkTo(0, 216, 184);
      Engine_ActorWaitForMove(0);
      Engine_ActorWalkTo(0, 216, 168);
      Engine_ActorWaitForMove(0);
    } else {
      Actor_SetSpeed(0, 0x10000, 0x8000);
      Engine_ActorWalkTo(0, 216, 200);
      Engine_ActorWaitForMove(0);
      Actor_SetSpeed(0, 0x1999, 0xccc);
      Engine_ActorWalkTo(0, 216, 168);
      if (LinkLobby_ExchangePartyRecords() < 0) {
        Actor_SetSpeed(0, 0x10000, 0x8000);
        Engine_ActorWalkTo(0, 216, 200);
        Map_ClearLayerEntryFlag(5);
        Engine_TaskWait(8);
        Map_SetLayerEntryFlag(5);
        Engine_ActorWaitForMove(0);
        Runtime_ReleaseHeapBlock(54);
        LinkLobby_WriteSlotValue(0);
        LinkLobby_WriteSlotValue(4);
        FieldScheduler_AddCallback((SchedulerCallback)LinkLobby_PollPeerReady,
                                   0xc80);
        Scheduler_SetTaskFlags((SchedulerCallback)LinkLobby_PollPeerReady, 1);
        Engine_GameFlagClear(FLAG_SCENE_TEMP_01);
        Engine_GameFlagClear(FLAG_SCENE_TEMP_02);
        Engine_GameFlagClear(0x303);
        Engine_GameFlagClear(FLAG_SCENE_TEMP_03);
        Engine_GameFlagClear(FLAG_SCENE_TEMP_00);
        work->raisedTrigger = 2;
        Engine_EventEnd();
        return LOBBY_NO_RESULT;
      }
      Actor_SetSpeed(0, 0x8000, 0x4000);
      Engine_ActorWaitForMove(0);
    }
    if (Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
      Event_SetNextDestination((s32)&SceneId_LinkLobby, 8);
      Event_SetDefeatDestination((s32)&SceneId_LinkLobby, 9);
    } else {
      Event_SetNextDestination((s32)&SceneId_LinkLobby, 10);
      Event_SetDefeatDestination((s32)&SceneId_LinkLobby, 11);
    }
    // Publish EXEC only after the battle and return destinations are ready.
    Battle_SetStart(4);
    Field_RequestBattleFromEncounterTable(1, 1);
    signature = (u16 *)gSerialTransfer.payload;
    signature[0] = 'E';
    signature[1] = 'X';
    signature[2] = 'E';
    signature[3] = 'C';
    i = 0;
    source = savedWork;
    destination = (u8 *)gMapCellBuffer + 0x8000;
    do {
      i++;
      *destination++ = *source++;
    } while (i <= 0x7c7);
    Runtime_ReleaseHeapBlock(54);
  }
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Offers a linked challenge or a monster battle and publishes the response.
/// Retains the original shared message tails and ignored return value.
static s32 LinkLobby_ChooseBattleMode(void) {
  // Returns no value: r0 is whatever the last call left.

  s32 answer;
  s32 messageBase;
  s32 messageId;

  messageBase = MsgLobbyPleaseSpeakWhen;
  Engine_EventBegin();
  Engine_ActorFaceActor(8, gGameState.selectedActor, 0);
  if (LinkLobby_PeerSlotMatches(0) == 0) {
    Engine_TaskWait(1);
  }
  if (LinkLobby_PeerSlotMatches(0) == 0) {
    LinkLobby_WriteSlotValue(5);
    LinkLobby_InitializeSerial();
    if (!GameFlag_IsSet(FLAG_MONSTER_BATTLE)) {
      Engine_EventSetMessage(messageBase + 5);
      Engine_EventOpenMessage(8, 0);
      answer = Engine_EventChooseYesNo(0, 0);
      if (answer == 0) {
        FieldGameFlag_SetByte(1000, 0);
        Engine_GameFlagSet(FLAG_MONSTER_BATTLE);
        Engine_GameFlagClear(0x172);
        Engine_GameFlagClear(FLAG_LINK_BATTLE);
        Engine_GameFlagSet(FLAG_SCENE_TEMP_02);
        messageId = messageBase + 7;
        gGameState.linkTallies[3] = answer;
        goto message;
      }
      Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
      Engine_GameFlagSet(FLAG_LINK_BATTLE);
      LinkLobby_WriteSlotValue(0);
      messageId = messageBase + 6;
      goto message;
    } else {
      messageId = messageBase + 3;
      goto message;
    }
  } else {
    if (GameFlag_IsSet(FLAG_MONSTER_BATTLE)) {
      LinkLobby_WriteSlotValue(0);
      Engine_EventSetMessage(MsgLobbyOpponentArrived);
      Engine_EventOpenMessage(8, 0);
      Engine_GameFlagClear(FLAG_SCENE_TEMP_02);
      Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
    }
    if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_02)) {
      messageId = messageBase + 3;
      /* The first branch's answers jump into this block's message tail, and an
       * accepted challenge back into its opening: the listing keeps one copy of
       * each. Written as if/else arms with their own message calls,
       * cross-jumping folds the copies into the last arm instead (differ 51 to
       * 55). */
    message:
      Engine_EventSetMessage(messageId);
    open_message:
      Engine_EventOpenMessage(8, 0);
    } else if (!Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01) &&
               !GameFlag_IsSet(0x300)) {
      Engine_EventSetMessage(messageBase);
      Engine_EventOpenMessage(8, 0);
      Engine_GameFlagSet(0x300);
    } else {
      Engine_GameFlagSet(0x300);
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01)) {
        Engine_EventSetMessage(messageBase + 2);
      } else {
        Engine_EventSetMessage(messageBase + 1);
      }
      Engine_EventOpenMessage(8, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        if (LinkLobby_PeerSlotMatches(0)) {
          Engine_GameFlagSet(FLAG_LINK_BATTLE);
          Engine_GameFlagSet(0x172);
          if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01)) {
            Engine_EventSetMessage(messageBase + 3);
          } else {
            Engine_EventSetMessage(messageBase + 4);
          }
          LinkLobby_WriteSlotValue(1);
          Engine_GameFlagSet(FLAG_SCENE_TEMP_02);
          goto open_message;
        } else {
          Engine_GameFlagSet(FLAG_SCENE_TEMP_05);
        }
      } else {
        Engine_EventSetMessage(messageBase);
        Engine_EventOpenMessage(8, 0);
      }
    }
  }
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Reports total or consecutive linked wins according to the leader's facing.
/// Retains the original handler's missing return and event-end behavior.
static s32 LinkLobby_ReportWinTallies(s32 actor) {
  u32 facing = Object_GetById(0)->facing;
  u16 *tally;
  s32 messageBase;

  Engine_EventBegin();
  Engine_ActorFaceActor(actor, gGameState.selectedActor, 0);
  if ((facing - 0xa001) <= 0x3ffe) {
    messageBase = MsgLobbyLooksLikeFirst;
    tally = &gGameState.linkTallies[4];
    if (*tally != 0) {
      UiWork_ClearValueNameTables();
      UiText_DrawQuantity(*tally, 5);
      Engine_EventSetMessage(messageBase + 1);
      Engine_EventShowMessage(actor, 0);
    } else {
      Engine_EventSetMessage(MsgLobbyTellTotalWins);
      Engine_EventShowMessage(actor, 0);
    }
  } else {
    messageBase = MsgLobbyLooksLikeFirstTime;
    tally = &gGameState.linkTallies[7];
    if (*tally != 0) {
      UiWork_ClearValueNameTables();
      UiText_DrawQuantity(*tally, 5);
      Engine_EventSetMessage(messageBase + 1);
      Engine_EventShowMessage(actor, 0);
    } else {
      Engine_EventSetMessage(MsgLobbyTellConsecutiveWins);
      Engine_EventShowMessage(actor, 0);
    }
  }
#ifdef UBFIX
  return 0;
#endif
}
/// Selects a debug party preset; A accepts and B cancels.
/// The preset loader's nonzero result selects the unavailable-preset message.
static s32 LinkLobby_ChooseDebugPartyPreset(s32 actorId) {
  struct UiWindow *window;
  s32 presetIndex;
  s32 displayedPreset;
  s32 loadResult;

  Engine_EventBegin();
  Engine_ActorFaceActor(actorId, gGameState.selectedActor, 0);
  Engine_EventSetMessage(MsgLobbyChooseParameters);
  Engine_EventShowMessage(actorId, 0);
  window = Engine_DebugCreateWindow(0, 0, 6, 4, 2);
  presetIndex = 0;
  displayedPreset = -1;
  for (;;) {
    if (presetIndex != displayedPreset) {
      RenderOutput_PrepareForRedraw(window);
      UiText_DrawNumber(presetIndex, 3, window, 0, 0);
      displayedPreset = presetIndex;
    }
    if (gKeysRepeat & KEY_LEFT) {
      presetIndex--;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      presetIndex++;
    }
    if (presetIndex < 0) {
      presetIndex = 0;
    }
    if (gKeysPressed & KEY_A) {
      break;
    }
    if (gKeysPressed & KEY_B) {
      presetIndex = -1;
      break;
    }
    Engine_TaskWait(1);
  }
  UiWork_Finalize(window, 1);
  if (presetIndex >= 0) {
    loadResult = DebugParty_LoadPreset(presetIndex);
  }
  if (presetIndex < 0) {
    Engine_EventSetMessage(MsgLobbyTease);
    Engine_EventShowMessage(9, 0);
  } else if (loadResult != 0) {
    Engine_EventSetMessage(MsgLobbyNoParameterAsk);
    Engine_EventShowMessage(9, 0);
  } else {
    Engine_EventSetMessage(MsgLobbySettingsMadeRest);
    Engine_EventShowMessage(9, 0);
  }
  Engine_TaskWait(10);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Runs the sanctum service for this actor within an event.
static s32 LinkLobby_OpenSanctum(s32 actorId) {
  Engine_EventBegin();
  Engine_SanctumOpen(actorId);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Tests the first three linked-battle owners, or four outside linked finals.
static s32 LinkLobby_PartyContains(s32 ownerId) {
  s32 count;
  s32 ownerLimit;
  s32 i;

  count = Party_CountActiveOwners();
  ownerLimit = 3;
  if (Engine_GameFlagIsSet(0x172) == 0) {
    ownerLimit = 4;
  }
  if (count > ownerLimit) {
    count = ownerLimit;
  }
  for (i = 0; i < count; i++) {
    if (gGameState.activeOwners[i] == 0xff) {
      return 0;
    }
    if (gGameState.activeOwners[i] == ownerId) {
      return 1;
    }
  }
  return 0;
}

/// UNUSED: the original empty lobby callback.
UNUSED static void State_NoOp(void) {}

/// Chooses each party member's dialogue from participation and round outcome.
static s32 LinkLobby_TalkByProgress(s32 actor) {
  s32 messageBase = MsgLobbyCantWaitLets;
  s32 isaacPresent = LinkLobby_PartyContains(0);
  s32 speakerPresent = LinkLobby_PartyContains(actor);

  Engine_EventBegin();
  Engine_ActorFaceActor(actor, gGameState.selectedActor, 0);
  if (Engine_GameFlagIsSet(0x304)) {

    GameFlag_IsSet(0x2f0);
    messageBase = Engine_GameFlagIsSet(actor + 0x2f0);
    if (Engine_GameFlagIsSet(0x305)) {
      if (messageBase)
        messageBase = MsgLobbyWonToldCountOn;
      else
        messageBase = MsgLobbyWonWithoutWell;
    } else {
      if (messageBase)
        messageBase = MsgLobbyNotFaultLost;
      else
        messageBase = MsgLobbyKnewLostBecause;
    }
  } else if (isaacPresent != 0) {
    if (speakerPresent == 0)
      messageBase = MsgLobbyGoingFightAlone;
  } else {
    messageBase = MsgLobbyTryingGetAway;
  }
  Engine_EventSetMessage(messageBase + actor - 1);
  Engine_EventShowMessage(actor, 0);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Removes the link IRQ handlers, restores sound parameters and leaves the
/// arena.
static s32 LinkLobby_LeaveArena(void) {
  SerialRuntime_RemoveIrqHandlers();
  Sound_LoadPresetParameters(2);
  Event_SetDestination((s32)&SceneId_Clear, 1);
#ifdef UBFIX
  return 0;
#endif
}

// The handlers below are s32 and return no value: r0 is whatever
// Engine_EventEnd left.

/// Selects the attendant's rules or post-battle line from the round outcome.
static s32 LinkLobby_TalkToAttendant(s32 actor) {
  s32 message;
  s32 step = 0;

  Engine_EventBegin();
  switch (actor) {
  case 12:
    message = MsgLobbyTravelingWarrior;
    break;
  case 13:
    message = MsgLobbyThreeAlliesFight;
    break;
  case 14:
  default:
    message = MsgLobbyBattleArenaOld;
    break;
  }
  Engine_ActorFaceActor(actor, gGameState.selectedActor, 0);
  if (Engine_GameFlagIsSet(0x304))
    step = 2 - (Engine_GameFlagIsSet(0x305) != 0);
  Engine_EventSetMessage(message + step);
  Engine_EventOpenMessage(actor, 0);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Alternates between the linked-party size and reorder instructions.
static s32 LinkLobby_ExplainPartyOrder(s32 actor) {
  s32 message;

  Engine_EventBegin();
  Engine_ActorFaceActor(actor, gGameState.selectedActor, 0);
  if (!GameFlag_IsSet(FLAG_SCENE_TEMP_04)) {
    if (Party_CountActiveOwners() <= 3)
      message = MsgLobbyThreeAlliesFightLinkedFinals;
    else
      message = MsgLobbyThreeAlliesFightLinked;
    Engine_GameFlagSet(FLAG_SCENE_TEMP_04);
  } else {
    message = MsgLobbyChangeOrderParty;
    Engine_GameFlagClear(FLAG_SCENE_TEMP_04);
  }
  Engine_EventSetMessage(message);
  Engine_EventOpenMessage(actor, 0);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Reports the stored consecutive-monster-battle record when nonzero.
static s32 LinkLobby_ReportMonsterRecord(s32 actorId) {
  struct GameState *state;

  Engine_EventBegin();
  state = &gGameState;
  Engine_ActorFaceActor(actorId, state->selectedActor, 0);
  if (state->linkTallies[2] != 0) {
    UiText_DrawQuantity(state->linkTallies[2], 5);
    Engine_EventSetMessage(MsgLobbyWonNumberConsecutive);
  } else {
    Engine_EventSetMessage(MsgLobbyCantWaitSee);
  }
  Engine_EventOpenMessage(actorId, 0);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

u8 *LinkLobby_GetEvents(void) { return (u8 *)gLinkLobbyEvents; }

/// Shows saving/saved messages around the linked battle results save.
static s32 LinkLobby_SaveBattleResults(void) {
  // A shared inline message-pair helper grows each save routine from 0x6c to
  // 0x70 in the native build, so retain the two sequences.
  struct UiWindow *window;

  Engine_AudioPlayCue(MUS_085);
  window = UiText_OpenMessageWindow(MsgLobbySavingBattleResults, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  SaveState_ProcessSelectedSlot();
  UiWork_Finalize(window, 1);
  Engine_TaskWait(1);
  window = UiText_OpenMessageWindow(MsgLobbyBattleResultsSaved, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  UiWork_Finalize(window, 1);
#ifdef UBFIX
  return 0;
#endif
}

/// Shows saving/saved messages around the monster battle record save.
static s32 LinkLobby_SaveMonsterBattleResults(void) {
  // See LinkLobby_SaveBattleResults for the measured shared-helper trial.
  struct UiWindow *window;

  Engine_AudioPlayCue(MUS_085);
  window = UiText_OpenMessageWindow(MsgLobbySavingMonsterBattle, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  SaveState_ProcessSelectedSlot();
  UiWork_Finalize(window, 1);
  Engine_TaskWait(1);
  window = UiText_OpenMessageWindow(MsgLobbyMonsterBattleResults, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  UiWork_Finalize(window, 1);
#ifdef UBFIX
  return 0;
#endif
}

/// Draws a right-to-left three-digit value capped at 999, then redraws the map.
static s32 LinkLobby_DrawThreeDigitValue(s32 value) {
  s32 digit;

  if (value > 999)
    value = 999;
  for (digit = 0; digit <= 2; digit++) {
    Engine_MapCopyCellsTo(27, value % 10, 16 - digit, 8, 1, 1);
    value = value / 10;
  }
  Engine_MapRedraw();
#ifdef UBFIX
  return 0;
#endif
}

/// Processes the entry's monster/link result and restores lobby polling.
/// Returns zero after updating records, messages and party recovery.
s32 LinkLobby_RunRoundResult(void) {
  s32 i;
  u32 score;
  struct EventWork *eventWork;
  void (*callback)(void);

  gLinkLobbyCallFrames = 0;
  sLinkLobbyWaitFrames = 0;
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Sound_LoadPresetParameters(2);
  LinkLobby_DrawThreeDigitValue(gGameState.linkTallies[6]);
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
  for (i = 0; i < 8; i++) {
    Engine_GameFlagClear(0x2f0 + i);
    if (LinkLobby_PartyContains(i)) {
      Engine_GameFlagSet(0x2f0 + i);
    }
  }
  // Monster victories advance the challenge; defeats finish the streak.
  if (gGameState.entrance == 8) {
    s32 wins;
    s32 messageOffset;
    s32 v;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    LinkLobby_WriteSlotValue(5);
    gGameState.linkTallies[0]++;
    gGameState.linkTallies[3]++;
    wins = (s8)FieldGameFlag_GetByte(FLAG_MONSTER_BATTLE_WINS);
    messageOffset = wins * 2 + 2;
    if (messageOffset > 14) {
      messageOffset = 14;
    }
    v = FieldGameFlag_GetByte(1000);
    if (v == 2) {
      FieldGameFlag_SetByte(1000, 0);
      wins++;
      messageOffset++;
    } else {
      FieldGameFlag_SetByte(1000, v + 1);
    }
    Engine_ActorFaceActor(8, gGameState.selectedActor, 0);
    Engine_EventSetMessage(MsgLobbyNotBadNextMonster + messageOffset);
    Engine_EventOpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      if (wins > 90) {
        wins = 90;
      }
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, wins);
    } else {
      Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
      UiText_DrawQuantity(gGameState.linkTallies[3], 5);
      score = gGameState.linkTallies[3];
      if (gGameState.linkTallies[2] < score) {
        gGameState.linkTallies[2] = score;
        Engine_EventSetMessage(MsgLobbyWonNumberBattleBroke);
        Engine_EventOpenMessage(8, 0);
        LinkLobby_SaveMonsterBattleResults();
      } else {
        Engine_EventSetMessage(MsgLobbyWonNumberBattle);
        Engine_EventOpenMessage(8, 0);
      }
      LinkLobby_WriteSlotValue(0);
    }
    Engine_EventEnd();
  } else if (gGameState.entrance == 9) {

    gGameState.linkTallies[1]++;
    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    LinkLobby_WriteSlotValue(5);
    Engine_ActorFaceActor(8, gGameState.selectedActor, 0);
    /* FAKEMATCH(temporary): score holds the tally's address, then the tally;
     * as gGameState.linkTallies[3] read at both uses, 19 of 393 instructions
     * differ in tbs-en. */
    score = (u32)&gGameState.linkTallies[3];
    UiText_DrawQuantity(*(u16 *)score, 5);
    score = *(u16 *)score;
    if (gGameState.linkTallies[2] < score) {
      gGameState.linkTallies[2] = score;
      Engine_EventSetMessage(MsgLobbyWonNumberBattleBroke);
      Engine_EventOpenMessage(8, 0);
      LinkLobby_SaveMonsterBattleResults();
    } else {
      Engine_EventSetMessage(MsgLobbyWonNumberBattleTold);
      Engine_EventOpenMessage(8, 0);
    }
    gGameState.linkTallies[3] = 0;
    Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    LinkLobby_WriteSlotValue(0);
    Engine_EventEnd();
    // Linked battles update wins/losses and the consecutive-win record.
  } else if (gGameState.entrance == 10) {
    s32 v;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if (GameFlag_IsSet(1000)) {
      eventWork = gEventWork;
      Engine_GameFlagClear(1000);
      eventWork->raisedTrigger = 2;
      Engine_GameFlagClear(0x304);
      Engine_TaskWait(20);
      LinkLobby_InitializeSerial();
      LinkLobby_WriteSlotValue(0);
      LinkLobby_WriteSlotValue(4);
    } else {
      gGameState.linkTallies[4]++;
      v = gGameState.linkTallies[7] + 1;
      gGameState.linkTallies[7] = v;
      /* FAKEMATCH(temporary): score holds the record's address, as in the
       * other branches; as gGameState.linkTallies[6] at each use, 4 of 393
       * instructions differ in tbs-en. */
      score = (u32)&gGameState.linkTallies[6];
      if (*(u16 *)score < (u16)v) {
        *(u16 *)score = v;
      }
      LinkLobby_DrawThreeDigitValue(*(u16 *)score);
      LinkLobby_SaveBattleResults();
      Engine_GameFlagSet(0x304);
      Engine_GameFlagSet(0x305);
    }
    Engine_EventEnd();
  } else if (gGameState.entrance == 11) {
    s32 v;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    if ((v = Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) == 0) {
      gGameState.linkTallies[5]++;
      gGameState.linkTallies[7] = v;
      LinkLobby_SaveBattleResults();
    }
    Engine_GameFlagSet(0x304);
    Engine_GameFlagClear(0x305);
    Engine_EventEnd();
  } else {
    SerialRuntime_Initialize();
    Engine_GameFlagClear(0x172);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    if (gGameState.idleSleepEnabled != 0) {
      Engine_EventBegin();
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      Engine_ActorFaceActor(8, gGameState.selectedActor, 0);
      Engine_EventSetMessage(MsgLobbyNoteCantUse);
      Engine_EventShowMessage(8, 0);
      Engine_EventEnd();
    }
    gGameState.idleSleepEnabled = 0;
    gIdleSleepEnabled = 0;
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
  }
  // Resume polling and restore the party unless continuing a monster run.
  callback = (SchedulerCallback)LinkLobby_PollPeerReady;
  FieldScheduler_AddCallback(callback, 0xc80);
  Scheduler_SetTaskFlags(callback, 1);
  if (gGameState.entrance != 8 || !Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
    Party_RestoreHpPp(1);
    DjinnRecovery_TickTwenty();
  }
  return 0;
}

// clang-format off
u8 gLinkLobby_SlotColumns[] = {
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
    { GARET, IF_FLAG_SET(0x1), ACTOR_STAND,
        PIXELS(352), 0, PIXELS(108), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { IVAN, IF_FLAG_SET(0x2), ACTOR_STAND,
        PIXELS(328), 0, PIXELS(136), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { MIA, IF_FLAG_SET(0x3), ACTOR_STAND,
        PIXELS(376), 0, PIXELS(136), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { JENNA, IF_FLAG_SET(0x5), ACTOR_STAND,
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
    { GARET, IF_FLAG_SET(0x1), ACTOR_STAND,
        PIXELS(104), 0, PIXELS(144), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { IVAN, IF_FLAG_SET(0x2), ACTOR_STAND,
        PIXELS(152), 0, PIXELS(144), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { MIA, IF_FLAG_SET(0x3), ACTOR_STAND,
        PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { JENNA, IF_FLAG_SET(0x5), ACTOR_STAND,
        PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};
// clang-format on

struct SceneEvent gLinkLobbyEvents[] = {
    {EVENT_TALK, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress)},
    {EVENT_TALK, 2, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress)},
    {EVENT_TALK, 3, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress)},
    {EVENT_TALK, 5, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkByProgress)},
    {EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_ChooseBattleMode)},
    {EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_ReportWinTallies)},
    {EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_OpenSanctum)},
    {EVENT_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldWelcomeToBattleArenaWherePeople)},
    {EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkToAttendant)},
    {EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkToAttendant)},
    {EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_TalkToAttendant)},
    {EVENT_TALK, 15, CONDITION_ALWAYS,
     EVENT_SCRIPT(LinkLobby_ChooseDebugPartyPreset)},
    {EVENT_TALK, 16, CONDITION_ALWAYS,
     EVENT_SCRIPT(LinkLobby_ExplainPartyOrder)},
    {EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgFieldInTheLinkedFinalsYouCant)},
    {EVENT_TALK, 18, CONDITION_ALWAYS,
     MSGNO(MsgFieldToUseSummonQuicklyDjinnMust)},
    {EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgFieldDoYouPutDjinnOnStandby)},
    {EVENT_TALK, 20, CONDITION_ALWAYS,
     MSGNO(MsgFieldRemoveDjinnYouRarelyUseSo)},
    {EVENT_TALK, 21, CONDITION_ALWAYS,
     MSGNO(MsgFieldMonsterBattleRulesSayYouAll)},
    {EVENT_TALK, 22, CONDITION_ALWAYS,
     EVENT_SCRIPT(LinkLobby_ReportMonsterRecord)},
    {EVENT_TALK, 23, CONDITION_ALWAYS,
     MSGNO(MsgFieldAllTheWarriorsAreImpressiveSo)},
    {EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgFieldICouldEnterButIdJust)},
    {EVENT_TOUCH, 10, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_EnterBattle)},
    {EVENT_TOUCH, 1, CONDITION_ALWAYS, EVENT_SCRIPT(LinkLobby_LeaveArena)},
    {EVENT_RAISED, 1, CONDITION_ALWAYS,
     EVENT_SCRIPT(LinkLobby_PromptToParticipate)},
    {EVENT_RAISED, 2, CONDITION_ALWAYS,
     EVENT_SCRIPT(LinkLobby_ReportDisconnection)},
    {SCENE_EVENTS_END},
};
