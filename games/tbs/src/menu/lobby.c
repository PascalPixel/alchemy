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
/// The bytes sent for each member: a BattleUnit (BATTLE_UNIT_SIZE, 0x14c)
/// and the eight bytes after it. XXX: likely rounded up to whole 20-byte
/// blocks, because the serial runtime stores each block whole and 0x154 is
/// 17 of them.
#define LOBBY_UNIT_RECORD_BYTES 0x154
/// The bytes sent for the Djinn recovery table after the members: the table
/// (struct DjinnRecoveryTable, 268 bytes) and the 52 bytes after it, 16
/// blocks of 20, two more than the table needs.
#define LOBBY_RECOVERY_RECORD_BYTES 0x140
/// The members a linked battle takes; a smaller party sends absent records.
#define LOBBY_UNIT_RECORD_COUNT 3
/// gLinkStatus bits 0 and 1: both consoles were heard this frame, as the
/// battle code's LINK_BOTH_CONNECTED.
#define LOBBY_LINK_READY_MASK 3
/// A record fails on the 25th frame it waits without a link or past the
/// timeout.
#define LOBBY_TRANSFER_RETRY_LIMIT 24
/// Frames, about ten seconds, for the three member records, and again for
/// the recovery table.
#define LOBBY_SEND_TIMEOUT_FRAMES 600
/// Frames, about fifteen seconds, for all four records received.
#define LOBBY_RECEIVE_TIMEOUT_FRAMES 900
/// Frames without a link (LinkLobby_PollPeerReady) or without the partner's
/// request (LinkLobby_EnterBattle) before the lobby gives the partner up.
#define LOBBY_PEER_WAIT_FRAMES 25

// The lobby's flags, besides FLAG_MONSTER_BATTLE, FLAG_LINK_BATTLE and the two
// byte flags flags.h names:
// FLAG_SCENE_TEMP_00: the attendant has asked the player to take part; every
//   300th call of LinkLobby_PromptToParticipate clears it, to ask again.
// FLAG_SCENE_TEMP_01: the linked partner has asked for a battle: its
//   signature matches, and its state is slot 1 (asked) or 2 (in the circle).
// FLAG_SCENE_TEMP_02: the player has agreed to a battle.
// FLAG_SCENE_TEMP_03: the party is on its way into the battle.
// FLAG_SCENE_TEMP_04: the attendant's next explanation is the party order.
// FLAG_SCENE_TEMP_05: the link was lost, and the attendant has yet to say so.
// FLAG_SCENE_TEMP_F0 + n: character n is in the battle party, set on every
//   arrival (LinkLobby_RunRoundResult).
// 0x172: a linked battle has been arranged (with FLAG_LINK_BATTLE): only the
//   first three members take part (LinkLobby_PartyContains).
// The place flags it uses (flags.h):
// FLAG_PLACE_TEMP_00: the attendant has explained once; talking again offers to
// apply. FLAG_PLACE_TEMP_02: this console is the link's child (multiplayer id
// 1), which receives
//   the parties first. LinkLobby_PeerSlotMatches sets FLAG_PLACE_TEMP_02 and
//   FLAG_PLACE_TEMP_03.
// FLAG_PLACE_TEMP_03: the link is up (LOBBY_LINK_READY_MASK).
// FLAG_PLACE_TEMP_04: a linked battle has ended since the leader last walked
// south of z
//   224; FLAG_PLACE_TEMP_05: it was won. The party's and visitors' lines read
//   them.
// FLAG_PLACE_TEMP_E8, flag 1000: the last linked battle broke off
// (Battle_RunEncounter). In a
//   monster series, byte flag 1000 (flags 1000 to 1007) instead counts the
//   victories, 0 to 2, towards a stronger monster.
//
// gGameState.linkTallies, as this file keeps them: 0 monster battles won,
// 1 monster battles lost, 2 the most monster battles won in a row, 3 those
// won in the current series, 4 linked battles won, 5 linked battles lost,
// 6 the most linked battles won in a row (the wall board), 7 those won in a
// row now.

#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

// clang-format off
/// The words the lobby publishes in its link payload, by slot; the partner
/// compares them with its own (LinkLobby_PeerSlotMatches). Slot 0 is this
/// game's signature, in column 0 (gLinkLobby_SlotColumns); slots 1 to 4 are
/// its state, in column 1: 1 has asked for a battle, 2 is in the circle, 3 is
/// unused and 4 is idle. Slot 5 replaces the signature when no partner
/// answers the attendant and during a monster series. In English and French
/// it equals slot 0; elsewhere it is slot 0's letters reversed, which the
/// partner's slot 0 test does not match. XXX: likely meant to hide this
/// console from partners meanwhile, because each return to linking writes
/// slot 0 back.
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

/// A console's link payload as the lobby reads it from gLinkPeerSignatures:
/// values[0] is its signature word and values[1] its state word
/// (gLinkLobby_SlotValues, placed by gLinkLobby_SlotColumns).
struct LinkPeer {
  s32 values[6]; ///< the payload's words; only 0 and 1 are read here
};

/// Frames LinkLobby_PollPeerReady has gone without a link, 0 to 25; 0 while
/// linked.
static s32 sLinkLobbyWaitFrames;

/// What a lobby handler's early exit returns. The ROM's s32 handlers return
/// nothing there, leaving whatever the last call put in r0 (UB); UBFIX
/// returns 0, as the function ends that fall off do.
#ifdef UBFIX
#define LOBBY_NO_RESULT 0
#else
#define LOBBY_NO_RESULT
#endif

/// The lobby's entrances, which Field_RunScene reads through entry/lobby.s.
u8 *LinkLobby_GetEntrances(void) { return (u8 *)gLinkLobbyEntrances; }

/// The regions entry: the lobby has no exit boxes.
s32 SceneData_ReturnZero(void) { return 0; }

/// Restarts the link with interrupts locked: removes the serial and timer 3
/// handlers, then runs SerialRuntime_Initialize. Called when no partner
/// answers the attendant and after a linked battle broke off. The original
/// s32 entry produces no defined result; its callers ignore it.
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

/// The placements entry. After a defeat (entrance 9 for a monster battle, 11
/// for a linked one) the party members and four visitors roughly trade places
/// (gLinkLobbyBattlePlacements): Garet, Ivan and Mia or Jenna take three of
/// the visitors' spots, and the visitors stand around where the party stood.
s32 LinkLobby_SelectPlacements(void) {
  s16 entrance = gGameState.entrance;

  if (entrance == 11 || entrance == 9) {
    return (s32)gLinkLobbyBattlePlacements;
  }
  return (s32)gLinkLobbyPlacements;
}

/// Returns 1 when the partner's payload holds slot's word in slot's column,
/// 0 when it does not or there is no link. Also sets or clears place flags
/// FLAG_PLACE_TEMP_03 (the link is up) and FLAG_PLACE_TEMP_02 (this console is
/// the child), and reads the other console's payload by them.
static s32 LinkLobby_PeerSlotMatches(s32 slot) {
  s32 serialPlayerId = -1;

  if ((gLinkStatus & LOBBY_LINK_READY_MASK) == LOBBY_LINK_READY_MASK) {
    // SIOCNT bits 4 and 5: 0 for the parent, 1 for the child.
    serialPlayerId = (REG_SIOCNT << 26) >> 30;
    Engine_GameFlagSet(FLAG_PLACE_TEMP_03);
  } else {
    Engine_GameFlagClear(FLAG_PLACE_TEMP_03);
  }
  /* FAKEMATCH(one-pass): a loop left by break after its first pass, which
   * places the test after the body as the ROM does; as an if around the body,
   * 12 of 58 instructions differ in tbs-en. */
  while (serialPlayerId >= 0 && Engine_GameFlagIsSet(FLAG_PLACE_TEMP_03)) {
    const s32 *expected = &gLinkLobby_SlotValues[slot];

    if (serialPlayerId != 0)
      Engine_GameFlagSet(FLAG_PLACE_TEMP_02);
    else
      Engine_GameFlagClear(FLAG_PLACE_TEMP_02);
    {
      struct LinkPeer *peer = (struct LinkPeer *)
          gLinkPeerSignatures[Engine_GameFlagIsSet(FLAG_PLACE_TEMP_02) ^ 1];

      if (peer->values[gLinkLobby_SlotColumns[slot]] == *expected)
        return 1;
      break;
    }
  }
  return 0;
}

/// Publishes slot's word in its column of this console's link payload, where
/// the partner's LinkLobby_PeerSlotMatches reads it.
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
/// The scheduler callback that watches the partner each frame in the lobby.
/// It forgets the partner after LOBBY_PEER_WAIT_FRAMES without a link, keeps
/// FLAG_SCENE_TEMP_01 current, raises event 1 (LinkLobby_PromptToParticipate)
/// once both sides agree and event 2 (LinkLobby_ReportDisconnection) when the
/// link is lost while a battle is being arranged. LinkLobby_RunRoundResult
/// schedules it on every arrival; LinkLobby_EnterBattle removes it.
static void LinkLobby_PollPeerReady(void) {
#else
static s32 LinkLobby_PollPeerReady(void) {
#endif
  struct EventWork *work;
  s32 result;
  s32 i;

  work = gEventWork;
  result = 1;
  // The leader is south of z 224 (16.16 pixels), towards the way out: the
  // party stops talking about the last battle.
  if (Object_GetById(0)->z.fixed > 0xe00000) {
    Engine_GameFlagClear(FLAG_PLACE_TEMP_04);
  }
  // While raised event 2 (the lost link) waits, skip the partner tracking.
  if (work->raisedTrigger != 2) {
    LinkLobby_PeerSlotMatches(0);
    if (!Engine_GameFlagIsSet(FLAG_PLACE_TEMP_03)) {
      // Without a link for 26 frames, clear every console's received payload
      // and go idle.
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
    // While linked: has the partner asked for a battle, or stepped into the
    // circle?
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
  // The 25th frame without a link while a linked battle is being arranged:
  // report the link lost.
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

/// Calls of LinkLobby_PromptToParticipate towards asking again, at 300;
/// cleared on every arrival.
u32 gLinkLobbyCallFrames;

/// Raised event 1: the attendant asks the player into the circle once both
/// consoles agree, and again after 300 calls. The line is "If you want to
/// participate, please step into the circle!". Returns the flag's value when
/// FLAG_SCENE_TEMP_03 or _00 is set and nothing is shown; the path that
/// shows the line returns nothing in the ROM (UB).
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

/// Raised event 2: the attendant says the link was disconnected. Publishes
/// slot 4 (idle) and clears the raised trigger and FLAG_SCENE_TEMP_00, _03
/// and _05. The original handler has no defined return value.
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

/// Receives the partner's three member records into units 0x80 to 0x82 (the
/// battle's enemies, by UnitGet) and its Djinn recovery table into the enemy
/// side's (Djinn_GetRecoveryTable(1)). Each name gets the edition's
/// MsgEnemyLabel ("Enemy" in English) in front, within the 14 letters a
/// BattleUnit name holds. Returns how many records hold a member, or -1 when
/// a transfer fails, overruns or runs out of time. The alternate-pool buffer
/// it allocates and frees is never written: the records are received in
/// place.
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
  // BattleUnit's side (0x12a): the sender marks a member 2 and a padding
  // record 0.
  if (record[0x12a] != 0) {
    result++;
  }
  Engine_TaskWait(2);
  // Put the prefix before the name, shifting it right and cutting it to the
  // 14 letters and terminator a BattleUnit name holds.
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
  // German, Spanish, French and Italian put a space after the prefix.
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
  // The partner's members become this battle's enemies 0x80 to 0x82.
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
  // Then the recovery table, into the enemy side's (unit 0x83's record).
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

/// Copies the character ids of the party's first three members
/// (gGameState.activeOwners), ends them with 0xff and returns how many. A
/// null destination only counts them.
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

/// Sends the party's first three members' records, marked side 2
/// (BATTLE_UNIT_PARTY), and for a smaller party absent records, copies of
/// the last member marked side 0. Then sends this side's Djinn recovery
/// table, rewritten for the partner: each entry's owner becomes the number
/// the partner gives that member (0x80 plus its place), and the entries of
/// members not sent are dropped. Returns 0, or -1 when a send fails or runs
/// out of time. The copies go through an alternate-pool buffer freed before
/// returning.
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
      // As a u8, i - 128 is 0x80 + i: this member's unit on the partner's
      // side.
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
  // Pad to three records with the last copy marked absent (side 0).
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
    // Give each Djinni on the list its owner's number on the partner's side,
    // and drop those of members not sent (peerSlots 0).
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

/// Exchanges the two parties: the parent sends first, the child (place flag
/// FLAG_PLACE_TEMP_02) receives first. On success stores the partner's member
/// count in FLAG_LINK_PARTY_COUNT and returns it; on failure resets the
/// transfer state and returns a negative result.
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
  receiveFirst = Engine_GameFlagIsSet(FLAG_PLACE_TEMP_02);
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

/// Touch event 10, the circle: starts the battle. A monster battle starts at
/// once. A linked one needs the attendant's call (FLAG_SCENE_TEMP_00) and no
/// lost link waiting to be reported (FLAG_SCENE_TEMP_05); it publishes slot 2
/// and waits for the partner to step in, giving up when the partner
/// withdraws, the link is lost, or the partner neither asks nor stands in the
/// circle for LOBBY_PEER_WAIT_FRAMES. The leader then walks in while the
/// parties are exchanged; a failed exchange walks back, restarts the lobby's
/// polling and raises event 2. Sets where the battle returns to (entrances 8
/// and 9 for a monster battle, 10 and 11 for a linked one) and requests it. The
/// original returns have no defined status.
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
    // Heap slot 54 is gBattleOwnerStates, the battle's enemy units
    // (LINK_OWNER_STATES_SIZE, battle/battle/encounter.c). While it is
    // allocated, the exchange below receives the partner's members and Djinn
    // into units 0x80 to 0x83. The copy to gActorSpriteSlots (gMapCellBuffer
    // + 0x8000) hands them to Battle_RunEncounter, which copies them back.
    savedWork = Runtime_AllocateBlock(54, 0x7c8);
    Engine_TaskRemoveCallback((SchedulerCallback)LinkLobby_PollPeerReady);
    // Run the map's tile animation channel 5 for eight frames.
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
      // Speeds are 16.16 pixels a frame: one to (216, 200), a tenth while
      // the parties are exchanged, then a half.
      Actor_SetSpeed(0, 0x10000, 0x8000);
      Engine_ActorWalkTo(0, 216, 200);
      Engine_ActorWaitForMove(0);
      Actor_SetSpeed(0, 0x1999, 0xccc);
      Engine_ActorWalkTo(0, 216, 168);
      // A failed exchange: walk back out and return to the lobby, reporting
      // the link lost (event 2).
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
        Engine_GameFlagClear(FLAG_PLACE_TEMP_03);
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
    // It fills the payload's first four halfwords, where the link battle's
    // command entry looks for it in the partner's (party_commands.c).
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

/// Talk event 8, the attendant: arranges a battle. With no partner, offers a
/// monster battle. With one, calls off a monster battle in waiting, then asks
/// the player to apply or to accept the partner's request; a yes is
/// published as slot 1. Agreeing sets FLAG_SCENE_TEMP_02, and for a linked
/// battle FLAG_LINK_BATTLE and flag 0x172. The original handler's return
/// value is ignored.
static s32 LinkLobby_ChooseBattleMode(void) {
  // Returns no value: r0 is whatever the last call left.

  s32 answer;
  s32 messageBase;
  s32 messageId;

  // The attendant's lines from MsgLobbyPleaseSpeakWhen: +1 asks the player to
  // apply, +2 to accept the partner's request, +3 and +7 send the player to
  // the circle, +4 says the request is made, +5 offers a monster battle and
  // +6 asks the player to wait for an opponent.
  messageBase = MsgLobbyPleaseSpeakWhen;
  Engine_EventBegin();
  Engine_ActorFaceActor(8, gGameState.selectedActor, 0);
  // Look for the partner's signature twice, a frame apart.
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
      // Yes (0) starts a monster series: byte flag 1000 and the series'
      // victories (tally 3) start at 0.
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
    // A partner arrived while a monster battle waited: call it off.
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
               !GameFlag_IsSet(FLAG_PLACE_TEMP_00)) {
      Engine_EventSetMessage(messageBase);
      Engine_EventOpenMessage(8, 0);
      Engine_GameFlagSet(FLAG_PLACE_TEMP_00);
    } else {
      Engine_GameFlagSet(FLAG_PLACE_TEMP_00);
      if (Engine_GameFlagIsSet(FLAG_SCENE_TEMP_01)) {
        Engine_EventSetMessage(messageBase + 2);
      } else {
        Engine_EventSetMessage(messageBase + 1);
      }
      Engine_EventOpenMessage(8, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        // Yes, and the partner is still there; else the link is lost.
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

/// Talk event 9: reports the linked battles won, the total when the leader
/// faces north and those won in a row otherwise; before the first, the line
/// says it will tell them later. Like the original handler, it never ends
/// its event (no Engine_EventEnd) and returns nothing.
static s32 LinkLobby_ReportWinTallies(s32 actor) {
  u32 facing = Object_GetById(0)->facing;
  u16 *tally;
  s32 messageBase;

  Engine_EventBegin();
  Engine_ActorFaceActor(actor, gGameState.selectedActor, 0);
  // FACING_IS_NORTH's test, written out: strictly between northwest and
  // northeast.
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
/// Talk event 15, a debug helper: Left and Right choose a preset number from
/// 0 up, A loads it (DebugParty_LoadPreset) and B cancels. Actor 9 replies:
/// one line for a cancel, one for a preset past the table's end (the loader's
/// nonzero result), one for a loaded party.
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

/// Talk event 10: the actor serves as a Sanctum's priest (Engine_SanctumOpen).
static s32 LinkLobby_OpenSanctum(s32 actorId) {
  Engine_EventBegin();
  Engine_SanctumOpen(actorId);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Returns 1 when character ownerId is among the party's first three members
/// while a linked battle is arranged (flag 0x172), or its first four
/// otherwise; 0 when not.
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

/// Talk events 1, 2, 3 and 5: Garet, Ivan, Mia or Jenna speaks of the last
/// linked battle (place flags FLAG_PLACE_TEMP_04 and FLAG_PLACE_TEMP_05) and
/// whether they fought in it, or else of whether Isaac and they are in the
/// battle party. Each message family holds one line per character, at actor -
/// 1; Felix's holds "?".
static s32 LinkLobby_TalkByProgress(s32 actor) {
  s32 messageBase = MsgLobbyCantWaitLets;
  s32 isaacPresent = LinkLobby_PartyContains(0);
  s32 speakerPresent = LinkLobby_PartyContains(actor);

  Engine_EventBegin();
  Engine_ActorFaceActor(actor, gGameState.selectedActor, 0);
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_04)) {

    // Isaac's flag is tested and the result dropped; messageBase first
    // holds whether the speaker fought (FLAG_SCENE_TEMP_F0 + actor).
    GameFlag_IsSet(FLAG_SCENE_TEMP_F0);
    messageBase = Engine_GameFlagIsSet(actor + 0x2f0);
    if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_05)) {
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

/// Touch event 1, the way out: stops the link, loads sound preset 2 and goes
/// back to the file select screen (its entrance 1).
static s32 LinkLobby_LeaveArena(void) {
  SerialRuntime_RemoveIrqHandlers();
  Sound_LoadPresetParameters(2);
  Event_SetDestination((s32)&SceneId_Clear, CLEAR_FROM_SANCTUM);
#ifdef UBFIX
  return 0;
#endif
}

// The handlers below are s32 and return no value: r0 is whatever
// Engine_EventEnd left.

/// Talk events 12 to 14, three visitors: a traveling warrior, one who
/// explains the linked finals, and one who wonders whether this is the old
/// man's Battle Arena. Each says their first line unless a linked battle has
/// ended since the leader last walked south of z 224 (place flag
/// FLAG_PLACE_TEMP_04), then their second after a win or their third after a
/// loss (FLAG_PLACE_TEMP_05).
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
  if (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_04))
    step = 2 - (Engine_GameFlagIsSet(FLAG_PLACE_TEMP_05) != 0);
  Engine_EventSetMessage(message + step);
  Engine_EventOpenMessage(actor, 0);
  Engine_EventEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Talk event 16: alternates, by FLAG_SCENE_TEMP_04, between the linked
/// finals' limit of three allies (worded for the party's size) and how to
/// change the party's order.
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

/// Talk event 22: reports the most monster battles won in a row (tally 2),
/// or before any, that the speaker will count them.
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

/// Saves the battle results between saving and saved messages, through
/// SaveState_ProcessSelectedSlot, which writes the first 16 bytes of the save
/// stamp (gSaveStamp) into the current file. LinkLobby_RunRoundResult calls it
/// after each linked battle won or lost.
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

/// The same with the monster battle messages, after a monster series that set
/// a new record.
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

/// Shows value, at most 999, on the wall board: three map cells at row 8,
/// columns 14 to 16, each copied from column 27 at the row of its digit.
/// LinkLobby_RunRoundResult shows the most linked battles won in a row.
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

/// The overlay's initialize entry, run on every arrival: settles the battle
/// that brought the party here and restarts the lobby's polling. Entrance 8
/// is a monster battle won, 9 one lost, 10 a linked battle won (or broken
/// off, flag 1000) and 11 one lost; any other entrance (the file select
/// screen uses 1) restarts the link. Updates gGameState.linkTallies,
/// saves after linked battles and new monster records, and restores the
/// party's HP, PP and Djinn unless a monster series goes on. Returns 0.
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

  // The payload's halfwords 4 to 7 hold the link battle's phase word
  // ("VSST", "EDVS", "TUrn"); the lobby puts TALK there.
  {
    u16 *name = (u16 *)gSerialTransfer.payload;

    name[4] = 'T';
    name[5] = 'A';
    name[6] = 'L';
    name[7] = 'K';
  }
  // Note who is in the battle party, for the members' lines.
  for (i = 0; i < 8; i++) {
    Engine_GameFlagClear(0x2f0 + i);
    if (LinkLobby_PartyContains(i)) {
      Engine_GameFlagSet(0x2f0 + i);
    }
  }
  // Entrance 8, a monster battle won: offer the next one.
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
    // The offer has two lines a level (FLAG_MONSTER_BATTLE_WINS, -1 when a
    // series starts), the even line capped at 14. Byte flag 1000 counts the
    // victories 0 to 2; the third raises the level and takes the next, odd
    // line (at most 15), which promises a stronger monster.
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
      // No: the series ends, and a new record (tally 2) is saved.
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
    // Entrance 9, a monster battle lost: the series ends, and a new record
    // is saved.
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
    // Entrance 10, a linked battle won: count the win and the run; a longer
    // run is the board's new record. Saved.
  } else if (gGameState.entrance == 10) {
    s32 v;

    Engine_EventBegin();
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    LinkLobby_WriteSlotValue(0);
    LinkLobby_WriteSlotValue(4);
    // The battle broke off instead (Battle_RunEncounter set flag 1000):
    // report the link lost and restart it.
    if (GameFlag_IsSet(1000)) {
      eventWork = gEventWork;
      Engine_GameFlagClear(1000);
      eventWork->raisedTrigger = 2;
      Engine_GameFlagClear(FLAG_PLACE_TEMP_04);
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
      Engine_GameFlagSet(FLAG_PLACE_TEMP_04);
      Engine_GameFlagSet(FLAG_PLACE_TEMP_05);
    }
    Engine_EventEnd();
    // Entrance 11, a linked battle lost: count the loss and end the run.
    // Saved.
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
    Engine_GameFlagSet(FLAG_PLACE_TEMP_04);
    Engine_GameFlagClear(FLAG_PLACE_TEMP_05);
    Engine_EventEnd();
    // Arriving from the file select screen: start the link, clear flag 0x172
    // and reset the monster level. Idle sleep is turned off here, with a note
    // when it was on.
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
/// The payload word each slot of gLinkLobby_SlotValues goes in: 0 the
/// signature, 1 the state. The last two entries have no slot.
u8 gLinkLobby_SlotColumns[] = {
    0, 1, 1, 1, 1, 0, 0, 0,
};

/// Where the party appears: 1 from the file select screen, 8 and 9 after a
/// monster battle won or lost, 10 and 11 after a linked one
/// (LinkLobby_EnterBattle).
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
    SCENE_EXITS_END,
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
