/// @file
/// The Battle Arena overlay: starting linked and monster battles, the exchange
/// of the two parties, the attendant and the records of wins.

#include "unit/import.h"
#include "unit/menu/lobby.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/psynergy_effects.h"
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
#include "script_layer.h"
#include "field_service.h"
#include "field_offset.h"
#include "io_reg.h"
#include "serial_runtime.h"
#include "iwram_call.h"
#include "character.h"
#include "mem.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "task.h"
#include "map_ids.h"
#include "game_state.h"
#include "battle_party.h"
#include "irqlock.h"
#include "map_tables.h"
#include "frame.h"
#include "flags.h"
#include "heap_slots.h"
#include "songs.h"
#include "heap.h"

// Link transfers use three unit records followed by one recovery-table record.
/// The bytes sent for each member: a Unit (BATTLE_UNIT_SIZE, 0x14c)
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
/// Frames without a link (BattleArena_PollPeerReady) or without the partner's
/// request (BattleArena_EnterBattle) before the Battle Arena gives the partner
/// up.
#define LOBBY_PEER_WAIT_FRAMES 25

// The Battle Arena's flags, besides FLAG_MONSTER_BATTLE, FLAG_LINK_BATTLE
// and the two byte flags flags.h names:
// FLAG_MAP_TEMP_00: the attendant has asked the player to take part; every
//   300th call of BattleArena_PromptToParticipate clears it, to ask again.
// FLAG_MAP_TEMP_01: the linked partner has asked for a battle: its
//   signature matches, and its state is LOBBY_SLOT_ASKED or
//   LOBBY_SLOT_IN_CIRCLE.
// FLAG_MAP_TEMP_02: the player has agreed to a battle.
// FLAG_MAP_TEMP_03: the party is on its way into the battle.
// FLAG_MAP_TEMP_04: the attendant's next explanation is the party order.
// FLAG_MAP_TEMP_05: the link was lost, and the attendant has yet to say so.
// FLAG_MAP_TEMP_F0 + n: character n is in the battle party, set on every
//   arrival (BattleArena_RunRoundResult).
// FLAG_LINK_BATTLE_ARRANGED: a linked battle has been arranged (with
// FLAG_LINK_BATTLE): only the
//   first three members take part (BattleArena_PartyContains).
// The map section flags it uses (flags.h):
// FLAG_MAPSEC_TEMP_00: the attendant has explained once; talking again offers
// to apply. FLAG_MAPSEC_TEMP_02: this console is the link's child (multiplayer
// id 1), which receives
//   the parties first. BattleArena_PeerSlotMatches sets FLAG_MAPSEC_TEMP_02 and
//   FLAG_MAPSEC_TEMP_03.
// FLAG_MAPSEC_TEMP_03: the link is up (LOBBY_LINK_READY_MASK).
// FLAG_MAPSEC_TEMP_04: a linked battle has ended since the leader last walked
// south of z
//   224; FLAG_MAPSEC_TEMP_05: it was won. The party's and visitors' lines read
//   them.
// FLAG_MAPSEC_TEMP_E8: the last linked battle broke off (Battle_Run).
//   In a monster series, the byte flag at FLAG_MAPSEC_TEMP_E8 instead counts
//   the victories, 0 to 2, towards a stronger monster.

/// The object events, numbering the rows of gBattleArenaObjectEventTemplates
/// and gBattleArenaBattleObjectEventTemplates, which list the same people in
/// the same order; the rows of Garet, Ivan, Mia and Jenna take those
/// characters' own ids.
enum BattleArenaLocalId {
  /// The attendant, who arranges every battle; LOCALID_GREETER calls her the
  /// girl at the counter.
  /// ★ Named from the game: "Please speak to me when you want to fight in a
  /// battle." (MsgLobbyPleaseSpeakWhen)
  LOCALID_ATTENDANT = LOCALID_FIRST_PLACED,
  /// Reports the linked battles won, and answers for LOCALID_DEBUG_HELPER.
  /// ★ Named from the game: "I will tell you your total wins when you finish
  /// the linked finals." (MsgLobbyTellTotalWins)
  LOCALID_LINKED_WINS_TELLER,
  /// Opens a Sanctum's services (BattleArena_OpenSanctum); sprite 0x79, the
  /// healer of every Sanctum in sanctum.c.
  /// ★ Named from the game: "Healer" (MsgDebugGlyphName)
  LOCALID_HEALER,
  /// ★ Named from the game: "Welcome to Battle Arena, where people craving
  /// battle meet!" (MsgFieldWelcomeToBattleArenaWherePeople)
  LOCALID_GREETER,
  /// ★ Named from the game: "I am a traveling warrior. I came here to become
  /// stronger..." (MsgLobbyTravelingWarrior)
  LOCALID_TRAVELING_WARRIOR,
  /// ★ Named from the game: "Up to three allies can fight in Linked Battle
  /// finals." (MsgLobbyThreeAlliesFight)
  LOCALID_FINALS_VISITOR,
  /// ★ Named from the game: "Is this the Battle Arena the old man spoke of?"
  /// (MsgLobbyBattleArenaOld)
  LOCALID_OLD_MANS_ARENA_VISITOR,
  /// Loads a debug party (BattleArena_ChooseDebugPartyPreset).
  /// ★ Named from the game: "Choose your parameters here."
  /// (MsgLobbyChooseParameters)
  LOCALID_DEBUG_HELPER,
  /// Explains the linked finals' limit of three allies, then the party order.
  /// ★ Named from the game: "You can change the order of your party in the
  /// status screen." (MsgLobbyChangeOrderParty)
  LOCALID_PARTY_ORDER_VISITOR,
  /// ★ Named from the game: "In the linked finals, you can't delay after your
  /// opponent selects a move." (MsgFieldInTheLinkedFinalsYouCant)
  LOCALID_COMMAND_TIME_VISITOR,
  /// ★ Named from the game: "To use Summon quickly, Djinn must be on standby."
  /// (MsgFieldToUseSummonQuicklyDjinnMust)
  LOCALID_SUMMON_VISITOR,
  /// ★ Named from the game: "Do you put Djinn on standby before a battle, or
  /// do you set them?" (MsgFieldDoYouPutDjinnOnStandby)
  LOCALID_SET_DJINN_VISITOR,
  /// ★ Named from the game: "Remove Djinn you rarely use so you can summon
  /// quickly." (MsgFieldRemoveDjinnYouRarelyUseSo)
  LOCALID_STANDBY_DJINN_VISITOR,
  /// ★ Named from the game: "Monster Battle rules say you all must fight as a
  /// team against a monster." (MsgFieldMonsterBattleRulesSayYouAll)
  LOCALID_MONSTER_RULES_VISITOR,
  /// Reports the most Monster Battles won in a row.
  /// ★ Named from the game: "I'll be counting your victories!"
  /// (MsgLobbyCantWaitSee)
  LOCALID_MONSTER_WINS_TELLER,
  /// ★ Named from the game: "All the warriors are impressive, so I get edgy
  /// before battles begin." (MsgFieldAllTheWarriorsAreImpressiveSo)
  LOCALID_EDGY_VISITOR,
  /// ★ Named from the game: "I could enter, but I'd just get hurt if I did..."
  /// (MsgFieldICouldEnterButIdJust)
  LOCALID_WARY_VISITOR
};

/// The words of gBattleArena_SlotValues, by slot: the signature, then the state
/// a console publishes.
enum BattleArenaSlot {
  LOBBY_SLOT_SIGNATURE = 0,
  LOBBY_SLOT_ASKED = 1,     ///< has asked for a battle
  LOBBY_SLOT_IN_CIRCLE = 2, ///< stands in the circle
  LOBBY_SLOT_UNUSED = 3,
  LOBBY_SLOT_IDLE = 4,
  /// Replaces the signature when no partner answers the attendant and during
  /// a monster series.
  LOBBY_SLOT_ALONE = 5
};

/// The Battle Arena's raised events (EventWork.raisedEvent).
enum BattleArenaRaisedEvent {
  LOBBY_EVENT_NONE = 0,
  LOBBY_EVENT_PROMPT = 1,      ///< BattleArena_PromptToParticipate
  LOBBY_EVENT_DISCONNECTED = 2 ///< BattleArena_ReportDisconnection
};

/// gGameState.linkTallies, as the Battle Arena keeps them.
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

#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

// clang-format off
/// The words the Battle Arena publishes in its link payload, by slot; the
/// partner compares them with its own (BattleArena_PeerSlotMatches), by enum
/// BattleArenaSlot. The signature is in the first column (gBattleArena_SlotColumns),
/// the states in the second. In English and French LOBBY_SLOT_ALONE equals the
/// signature; elsewhere it is the signature's letters reversed, which the
/// partner's signature test does not match. XXX: likely meant to hide this
/// console from partners meanwhile, because each return to linking writes
/// the signature back.
const s32 gBattleArena_SlotValues[] = {
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

/// A console's link payload as the Battle Arena reads it from
/// gLinkPeerSignatures: values[0] is its signature word and values[1] its state
/// word (gBattleArena_SlotValues, placed by gBattleArena_SlotColumns).
struct LinkPeer {
  s32 values[6]; ///< the payload's words; only 0 and 1 are read here
};

/// Frames BattleArena_PollPeerReady has gone without a link, 0 to 25; 0 while
/// linked.
static s32 sBattleArenaWaitFrames;

/// What a Battle Arena handler's early exit returns. The ROM's s32 handlers
/// return nothing there, leaving whatever the last call put in r0 (UB); UBFIX
/// returns 0, as the function ends that fall off do.
#ifdef UBFIX
#define LOBBY_NO_RESULT 0
#else
#define LOBBY_NO_RESULT
#endif

/// The Battle Arena's warp destinations, which Map_Run reads through
/// entry/lobby.s.
u8 *BattleArena_GetWarpDests(void) { return (u8 *)gBattleArenaWarpDests; }

/// The warp regions entry: the Battle Arena has no warp boxes.
s32 MapData_ReturnZero(void) { return 0; }

/// Restarts the link with interrupts locked: removes the serial and timer 3
/// handlers, then runs SerialRuntime_Initialize. Called when no partner
/// answers the attendant and after a linked battle broke off. The original
/// s32 entry produces no defined result; its callers ignore it.
static s32 BattleArena_InitializeSerial(void) {
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

u8 *BattleArena_GetWarps(void) { return (u8 *)gBattleArenaWarps; }

/// The object event templates entry. After a defeat (warp destination 9 for a
/// monster battle, 11 for a linked one) the party members and four visitors
/// roughly trade places (gBattleArenaBattleObjectEventTemplates): Garet, Ivan
/// and Mia or Jenna take three of the visitors' spots, and the visitors stand
/// around where the party stood.
s32 BattleArena_SelectObjectEventTemplates(void) {
  s16 warpDest = gGameState.warpDest;

  if (warpDest == 11 || warpDest == 9) {
    return (s32)gBattleArenaBattleObjectEventTemplates;
  }
  return (s32)gBattleArenaObjectEventTemplates;
}

/// Returns 1 when the partner's payload holds slot's word in slot's column,
/// 0 when it does not or there is no link. Also sets or clears map section
/// flags FLAG_MAPSEC_TEMP_03 (the link is up) and FLAG_MAPSEC_TEMP_02 (this
/// console is the child), and reads the other console's payload by them.
static s32 BattleArena_PeerSlotMatches(s32 slot) {
  s32 serialPlayerId = -1;

  if ((gLinkStatus & LOBBY_LINK_READY_MASK) == LOBBY_LINK_READY_MASK) {
    // SIOCNT bits 4 and 5: 0 for the parent, 1 for the child.
    serialPlayerId = (REG_SIOCNT << 26) >> 30;
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_03);
  } else {
    Engine_GameFlagClear(FLAG_MAPSEC_TEMP_03);
  }
  /* FAKEMATCH(one-pass): a loop left by break after its first pass, which
   * places the test after the body as the ROM does; as an if around the body,
   * 12 of 58 instructions differ in tbs-en. */
  while (serialPlayerId >= 0 && Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_03)) {
    const s32 *expected = &gBattleArena_SlotValues[slot];

    if (serialPlayerId != 0)
      Engine_GameFlagSet(FLAG_MAPSEC_TEMP_02);
    else
      Engine_GameFlagClear(FLAG_MAPSEC_TEMP_02);
    {
      struct LinkPeer *peer = (struct LinkPeer *)
          gLinkPeerSignatures[Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_02) ^ 1];

      if (peer->values[gBattleArena_SlotColumns[slot]] == *expected)
        return 1;
      break;
    }
  }
  return 0;
}

/// Publishes slot's word in its column of this console's link payload, where
/// the partner's BattleArena_PeerSlotMatches reads it.
static void BattleArena_WriteSlotValue(s32 slot) {
  s32 *dst = (s32 *)gSerialTransfer.payload;
  const s32 *src = &gBattleArena_SlotValues[slot];
  dst[gBattleArena_SlotColumns[slot]] = *src;
}

// The task runner ignores this result. Keep the native status calculation in
// matching builds: making the function void removes eight instructions. The
// modern build uses the task's actual function type to avoid an
// incompatible indirect call.
#ifdef UBFIX
/// The task that watches the partner each frame in the Battle Arena.
/// It forgets the partner after LOBBY_PEER_WAIT_FRAMES without a link, keeps
/// FLAG_MAP_TEMP_01 current, raises LOBBY_EVENT_PROMPT once both
/// sides agree and LOBBY_EVENT_DISCONNECTED when the
/// link is lost while a battle is being arranged. BattleArena_RunRoundResult
/// schedules it on every arrival; BattleArena_EnterBattle removes it.
static void BattleArena_PollPeerReady(void) {
#else
static s32 BattleArena_PollPeerReady(void) {
#endif
  struct EventWork *work;
  s32 result;
  s32 i;

  work = gFieldWork;
  result = 1;
  // The leader is south of z 224 (16.16 pixels), towards the way out: the
  // party stops talking about the last battle.
  if (ObjectEvent_GetById(LOCALID_PARTY_LEADER)->z.fixed > 0xe00000) {
    Engine_GameFlagClear(FLAG_MAPSEC_TEMP_04);
  }
  // While LOBBY_EVENT_DISCONNECTED waits, skip the partner tracking.
  if (work->raisedEvent != LOBBY_EVENT_DISCONNECTED) {
    BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE);
    if (!Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_03)) {
      // Without a link for 26 frames, clear every console's received payload
      // and go idle.
      if (++sBattleArenaWaitFrames > LOBBY_PEER_WAIT_FRAMES) {
        for (i = 0; i < 4; i++) {
          Iwram_ClearWords(&gLinkPeerSignatures[i], 20);
        }
        sBattleArenaWaitFrames = 0;
        BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
      }
    } else {
      sBattleArenaWaitFrames = 0;
    }
    // While linked: has the partner asked for a battle, or stepped into the
    // circle?
    if (sBattleArenaWaitFrames == 0) {
      if (BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) &&
          (BattleArena_PeerSlotMatches(LOBBY_SLOT_ASKED) ||
           BattleArena_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))) {
        Engine_GameFlagSet(FLAG_MAP_TEMP_01);
        if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_02)) {
          work->raisedEvent = LOBBY_EVENT_PROMPT;
        }
        result = 1;
      } else {
        Engine_GameFlagClear(FLAG_MAP_TEMP_01);
        result = 0;
      }
    }
    if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_01) &&
        Engine_GameFlagIsSet(FLAG_MAP_TEMP_02) &&
        !Engine_GameFlagIsSet(FLAG_MAP_TEMP_00)) {
      work->raisedEvent = LOBBY_EVENT_PROMPT;
    }
  }
  // The 25th frame without a link while a linked battle is being arranged:
  // report the link lost.
  if ((GameFlag_IsSet(FLAG_MAP_TEMP_01) || GameFlag_IsSet(FLAG_MAP_TEMP_02)) &&
      !GameFlag_IsSet(FLAG_MONSTER_BATTLE) &&
      !BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) &&
      sBattleArenaWaitFrames > 24) {
    work->raisedEvent = LOBBY_EVENT_DISCONNECTED;
    GameFlag_Set(FLAG_MAP_TEMP_05);
    GameFlag_Clear(FLAG_MAP_TEMP_01);
    GameFlag_Clear(FLAG_MAP_TEMP_02);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  if (GameFlag_IsSet(FLAG_MAP_TEMP_05)) {
    work->raisedEvent = LOBBY_EVENT_DISCONNECTED;
  }
#ifdef UBFIX
  (void)result;
#else
  return result;
#endif
}

/// Calls of BattleArena_PromptToParticipate towards asking again, at 300;
/// cleared on every arrival.
u32 gBattleArenaCallFrames;

/// LOBBY_EVENT_PROMPT: the attendant asks the player into the circle once both
/// consoles agree, and again after 300 calls. The line is "If you want to
/// participate, please step into the circle!". Returns the flag's value when
/// FLAG_MAP_TEMP_03 or _00 is set and nothing is shown; the path that
/// shows the line returns nothing in the ROM (UB).
static s32 BattleArena_PromptToParticipate(void) {
  s32 set;

  set = Engine_GameFlagIsSet(FLAG_MAP_TEMP_03);
  if (set != 0) {
    return set;
  }
  if (++gBattleArenaCallFrames == 300) {
    gBattleArenaCallFrames = 0;
    Engine_GameFlagClear(FLAG_MAP_TEMP_00);
  }
  set = GameFlag_IsSet(FLAG_MAP_TEMP_00);
  if (set != 0) {
    return set;
  }
  Engine_ScriptStart();
  Engine_ScriptSetMessage(MsgLobbyWantParticipatePlease);
  Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
  Engine_TaskWait(5);
  Engine_GameFlagSet(FLAG_MAP_TEMP_00);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// LOBBY_EVENT_DISCONNECTED: the attendant says the link was disconnected.
/// Publishes LOBBY_SLOT_IDLE and clears the raised trigger and
/// FLAG_MAP_TEMP_00, _03 and _05. The original handler has no defined return
/// value.
static s32 BattleArena_ReportDisconnection(void) {
  struct EventWork *work = gFieldWork;

  BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  Engine_GameFlagClear(FLAG_MAP_TEMP_00);
  Engine_GameFlagClear(FLAG_MAP_TEMP_03);
  Engine_ScriptStart();
  work->raisedEvent = LOBBY_EVENT_NONE;
  Engine_ScriptSetMessage(MsgLobbyLinkDisconnected);
  Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
  Engine_GameFlagClear(FLAG_MAP_TEMP_05);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Receives the partner's three member records into units 0x80 to 0x82 (the
/// battle's enemies, by Unit_Get) and its Djinn recovery table into the enemy
/// side's (Djinn_GetRecoveryTable(1)). Each name gets the edition's
/// MsgEnemyLabel ("Enemy" in English) in front, within the 14 letters a
/// Unit name holds. Returns how many records hold a member, or -1 when
/// a transfer fails, overruns or runs out of time. The EWRAM pool buffer
/// it allocates and frees is never written: the records are received in
/// place.
static s32 BattleArena_ReceivePartyRecords(void) {
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
  heap = AllocEwram(size);
  result = 0;
  timeout = LOBBY_RECEIVE_TIMEOUT_FRAMES;
  slot = 0;
  // The record loop and both waits are written with goto (MATCHING.md): as a
  // for loop round while loops, 105 of 189 instructions differ in tbs-en.
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
  // Unit's side (0x12a): the sender marks a member 2 and a padding
  // record 0.
  if (record[0x12a] != 0) {
    result++;
  }
  Engine_TaskWait(2);
  // Put the prefix before the name, shifting it right and cutting it to the
  // 14 letters and terminator a Unit name holds.
  FieldMessage_DecodeRawUnbounded(MsgEnemyLabel, namePrefix);
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
  record = (u8 *)Unit_Get(slot + 128);
  tries = 0;
  if ((transferResult = SerialRuntime_StartReceive(record)) == -1) {
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
  heap = AllocEwram(size);
  tries = 0;
  if ((transferResult =
           SerialRuntime_StartReceive(Djinn_GetRecoveryTable(1))) != -1) {
    goto test2;
  }
failed:
  result = transferResult;
done:
  Runtime_BumpFree(heap);
  return result;
}

/// Copies the character ids of the party's first three members
/// (gGameState.party), ends them with 0xff and returns how many. A
/// null destination only counts them.
static s32 BattleArena_CopyPartyOwnerIds(u16 *ownerIds) {
  s32 ownerCount = Party_CountMembers();
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
    source = gGameState.party;
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
/// (BATTLE_SIDE_PARTY), and for a smaller party absent records, copies of
/// the last member marked side 0. Then sends this side's Djinn recovery
/// table, rewritten for the partner: each entry's member becomes the number
/// the partner gives that member (0x80 plus its place), and the entries of
/// members not sent are dropped. Returns 0, or -1 when a send fails or runs
/// out of time. The copies go through an EWRAM pool buffer freed before
/// returning.
static s32 BattleArena_SendPartyRecords(void) {
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
  struct DjinnRecoveryList *list;

  size = LOBBY_UNIT_RECORD_BYTES;
  heap = AllocEwram(size);
  result = 0;
  timeout = LOBBY_SEND_TIMEOUT_FRAMES;
  ownerCount = BattleArena_CopyPartyOwnerIds(ownerIds);
  slotMap = peerSlots;
  for (i = 7; i >= 0; i--) {
    slotMap[i] = 0;
  }
  for (i = 0; i < ownerCount; i++) {
    /* FAKEMATCH(one-pass): the record's send in a one-pass block, which keeps
     * the send failure inside the loop and gives the copy's argument order;
     * as a plain block, 3 of 255 instructions differ in tbs-en. */
    do {
      Iwram_CopyWords(heap, Unit_Get(ownerIds[i]), size);
      heap[0x12a] = 2;
      // As a u8, i - 128 is 0x80 + i: this member's unit on the partner's
      // side.
      slotMap[ownerIds[i]] = i - 128;
      transferResult = SerialRuntime_StartSend(heap, size);
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
  // The padding loop is written with goto (MATCHING.md): as for (; i <= 2;
  // i++) round a while loop, 127 of 254 instructions differ in tbs-en.
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
    if ((transferResult = SerialRuntime_StartSend(heap, size)) != -1) {
      goto test;
    }
    result = transferResult;
    goto done;
  }
  Runtime_BumpFree(heap);
  size = LOBBY_RECOVERY_RECORD_BYTES;
  heap = AllocEwram(size);
  /* FAKEMATCH(one-pass): the recovery table's copy in a one-pass block, which
   * gives the copy's argument order and keeps the list base's copy first after
   * the call; as a plain call, 8 of 255 instructions differ in tbs-en. */
  do {
    Iwram_CopyWords(heap, Djinn_GetRecoveryTable(0), size);
  } while (0);
  {
    s32 timeout;
    s32 tries;

    list = &((struct DjinnRecoveryTable *)heap)->list;
    tries = 0;
    timeout = LOBBY_SEND_TIMEOUT_FRAMES;
    // Give each Djinni on the list its member's number on the partner's
    // side, and drop those of members not sent (peerSlots 0).
    for (j = 0; j < list->count; j++) {
      list->entries[j].unitId = peerSlots[list->entries[j].unitId];
      if (list->entries[j].unitId == 0) {
        for (k = j; k < list->count - 1; k++) {
          list->entries[k] = list->entries[k + 1];
        }
        list->count--;
        j--;
      }
    }
    if ((transferResult = SerialRuntime_StartSend(heap, size)) == -1) {
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

/// Exchanges the two parties: the parent sends first, the child (map section
/// flag FLAG_MAPSEC_TEMP_02) receives first. On success stores the partner's
/// member count in FLAG_LINK_PARTY_COUNT and returns it; on failure resets the
/// transfer state and returns a negative result.
static s32 BattleArena_ExchangePartyRecords(void) {
  s32 result;
  s32 receivedOwnerCount;
  s32 receiveFirst;

  /* FAKEMATCH(one-pass): result's zero in a one-pass block, which loads it
   * into r6 ahead of the call as the ROM does; as a plain statement, 4 of 63
   * instructions differ in tbs-en. */
  do {
    result = 0;
  } while (0);
  receiveFirst = Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_02);
  gLinkExchangeState = result;
  // Every failure reaches the one reset: as nested ifs before a single
  // if (result < 0), 16 of 62 instructions differ in tbs-en.
  if (!receiveFirst) {
    Engine_TaskWait(5);
    result = BattleArena_SendPartyRecords();
    if (result < 0)
      goto fail;
    Engine_TaskWait(5);
    receivedOwnerCount = result = BattleArena_ReceivePartyRecords();
    if (result < 0)
      goto check;
  } else {
    receivedOwnerCount = result = BattleArena_ReceivePartyRecords();
    if (result < 0)
      goto fail;
    Engine_TaskWait(10);
    result = BattleArena_SendPartyRecords();
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

/// Coord event 10, the circle: starts the battle. A monster battle starts at
/// once. A linked one needs the attendant's call (FLAG_MAP_TEMP_00) and no
/// lost link waiting to be reported (FLAG_MAP_TEMP_05); it publishes
/// LOBBY_SLOT_IN_CIRCLE and waits for the partner to step in, giving up when
/// the partner withdraws, the link is lost, or the partner neither asks nor
/// stands in the circle for LOBBY_PEER_WAIT_FRAMES. The leader then walks in
/// while the parties are exchanged; a failed exchange walks back, restarts the
/// Battle Arena's polling and raises LOBBY_EVENT_DISCONNECTED. Sets where the
/// battle returns to (warp destinations 8 and 9 for a monster battle, 10 and 11
/// for a linked one) and requests it. The original returns have no defined
/// status.
static s32 BattleArena_EnterBattle(void) {
  struct EventWork *work;
  s32 failed;
  struct Window *window;
  s32 missedFrames;
  s32 cancelEntry;
  u8 *savedWork;
  u8 *source;
  u8 *destination;
  u16 *signature;
  u32 i;

  failed = 0;
  window = 0;
  work = gFieldWork;
  missedFrames = 0;
  if (Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
    Engine_ScriptStart();
  } else {
    if (!Engine_GameFlagIsSet(FLAG_MAP_TEMP_00))
      return LOBBY_NO_RESULT;
    if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_05))
      return LOBBY_NO_RESULT;
    Engine_ScriptStart();
    Engine_GameFlagSet(FLAG_MAP_TEMP_03);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IN_CIRCLE);
    if (!BattleArena_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))
      window = Message_OpenWindow(MsgLobbyAwaitingOpponent, 5, 4, 1);
    while (!BattleArena_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE)) {
      Engine_TaskWait(1);
      cancelEntry = 0;
      if (!Engine_GameFlagIsSet(FLAG_MAP_TEMP_01))
        cancelEntry = 1;
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_05))
        cancelEntry = 1;
      if (!BattleArena_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE) &&
          !BattleArena_PeerSlotMatches(LOBBY_SLOT_ASKED)) {
        if (++missedFrames > LOBBY_PEER_WAIT_FRAMES)
          cancelEntry = 1;
      } else {
        missedFrames = 0;
      }
      if (cancelEntry) {
        work->raisedEvent = LOBBY_EVENT_DISCONNECTED;
        Engine_GameFlagSet(FLAG_MAP_TEMP_05);
        Engine_GameFlagClear(FLAG_MAP_TEMP_01);
        Engine_GameFlagClear(FLAG_MAP_TEMP_02);
        BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
        failed = 1;
        Engine_GameFlagClear(FLAG_MAP_TEMP_00);
        break;
      }
    }
    if (window)
      Window_Close(window, 1);
    Engine_TaskWait(5);
  }
  // Suspend peer polling while transferring the party and entering battle.
  if (!failed) {
    // HEAP_SLOT_ENEMY_UNITS is gEnemyUnits, the battle's enemy
    // units (LINK_OWNER_STATES_SIZE, battle/battle/encounter.c). While it is
    // allocated, the exchange below receives the partner's members and Djinn
    // into units 0x80 to 0x83. The copy to gActorSpriteSlots
    // (gMapGridCellBuffer
    // + 0x8000) hands them to Battle_Run, which copies them back.
    savedWork = AllocHeapSlotEwram(HEAP_SLOT_ENEMY_UNITS, 0x7c8);
    Engine_TaskRemoveCallback((TaskFunc)BattleArena_PollPeerReady);
    // Run the map's tileset animation 5 for eight frames.
    TilesetAnim_Resume(5);
    Engine_TaskWait(8);
    TilesetAnim_Pause(5);
    if (Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
      Engine_ObjectEventFaceObjectEvent(LOCALID_ATTENDANT,
                                        gGameState.leaderLocalId, 0);
      Engine_ScriptSetMessage(MsgLobbyGoodLuck);
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      Engine_TaskWait(45);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
      Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 216, 184);
      Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
      Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 216, 168);
      Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    } else {
      // Speeds are 16.16 pixels a frame: one to (216, 200), a tenth while
      // the parties are exchanged, then a half.
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
      Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 216, 200);
      Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x1999, 0xccc);
      Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 216, 168);
      // A failed exchange: walk back out and return to the Battle Arena,
      // reporting the link lost (LOBBY_EVENT_DISCONNECTED).
      if (BattleArena_ExchangePartyRecords() < 0) {
        ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x10000, 0x8000);
        Engine_ObjectEventWalkTo(LOCALID_PARTY_LEADER, 216, 200);
        TilesetAnim_Resume(5);
        Engine_TaskWait(8);
        TilesetAnim_Pause(5);
        Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
        FreeHeapSlot(HEAP_SLOT_ENEMY_UNITS);
        BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
        BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
        FieldScheduler_AddCallback((TaskFunc)BattleArena_PollPeerReady, 0xc80);
        Task_SetFlags((TaskFunc)BattleArena_PollPeerReady, 1);
        Engine_GameFlagClear(FLAG_MAP_TEMP_01);
        Engine_GameFlagClear(FLAG_MAP_TEMP_02);
        Engine_GameFlagClear(FLAG_MAPSEC_TEMP_03);
        Engine_GameFlagClear(FLAG_MAP_TEMP_03);
        Engine_GameFlagClear(FLAG_MAP_TEMP_00);
        work->raisedEvent = LOBBY_EVENT_DISCONNECTED;
        Engine_ScriptEnd();
        return LOBBY_NO_RESULT;
      }
      ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, 0x8000, 0x4000);
      Engine_ObjectEventWaitForMove(LOCALID_PARTY_LEADER);
    }
    if (Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
      FieldScript_SetNextDestination((s32)&MapId_BattleArena, 8);
      FieldScript_SetDefeatDestination((s32)&MapId_BattleArena, 9);
    } else {
      FieldScript_SetNextDestination((s32)&MapId_BattleArena, 10);
      FieldScript_SetDefeatDestination((s32)&MapId_BattleArena, 11);
    }
    // Publish EXEC only after the battle and return destinations are ready.
    // It fills the payload's first four halfwords, where the link battle's
    // command entry looks for it in the partner's (party_commands.c).
    Battle_SetType(BATTLE_TYPE_LINK);
    Field_RequestBattleFromEncounterTable(1, 1);
    signature = (u16 *)gSerialTransfer.payload;
    signature[0] = 'E';
    signature[1] = 'X';
    signature[2] = 'E';
    signature[3] = 'C';
    i = 0;
    source = savedWork;
    destination = (u8 *)gMapGridCellBuffer + 0x8000;
    do {
      i++;
      *destination++ = *source++;
    } while (i <= 0x7c7);
    FreeHeapSlot(HEAP_SLOT_ENEMY_UNITS);
  }
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to LOCALID_ATTENDANT, who arranges a battle. With no partner, offers
/// a monster battle. With one, calls off a monster battle in waiting, then asks
/// the player to apply or to accept the partner's request; a yes is published
/// as LOBBY_SLOT_ASKED. Agreeing sets FLAG_MAP_TEMP_02, and for a linked
/// battle FLAG_LINK_BATTLE and FLAG_LINK_BATTLE_ARRANGED. The original
/// handler's return value is ignored.
static s32 BattleArena_ChooseBattleMode(void) {
  // Returns no value: r0 is whatever the last call left.

  s32 answer;
  s32 messageBase;
  s32 messageId;

  // The attendant's lines from MsgLobbyPleaseSpeakWhen: +1 asks the player to
  // apply, +2 to accept the partner's request, +3 and +7 send the player to
  // the circle, +4 says the request is made, +5 offers a monster battle and
  // +6 asks the player to wait for an opponent.
  messageBase = MsgLobbyPleaseSpeakWhen;
  Engine_ScriptStart();
  Engine_ObjectEventFaceObjectEvent(LOCALID_ATTENDANT, gGameState.leaderLocalId,
                                    0);
  // Look for the partner's signature twice, a frame apart.
  if (BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0) {
    Engine_TaskWait(1);
  }
  if (BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0) {
    BattleArena_WriteSlotValue(LOBBY_SLOT_ALONE);
    BattleArena_InitializeSerial();
    if (!GameFlag_IsSet(FLAG_MONSTER_BATTLE)) {
      Engine_ScriptSetMessage(messageBase + 5);
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      answer = Engine_ScriptChooseYesNo(0, 0);
      // Yes starts a monster series: FLAG_MAPSEC_TEMP_E8's byte and
      // LINK_TALLY_MONSTER_STREAK start at 0.
      if (answer == YES) {
        FieldGameFlag_SetByte(FLAG_MAPSEC_TEMP_E8, 0);
        Engine_GameFlagSet(FLAG_MONSTER_BATTLE);
        Engine_GameFlagClear(FLAG_LINK_BATTLE_ARRANGED);
        Engine_GameFlagClear(FLAG_LINK_BATTLE);
        Engine_GameFlagSet(FLAG_MAP_TEMP_02);
        messageId = messageBase + 7;
        gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK] = answer;
        goto message;
      }
      Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
      Engine_GameFlagSet(FLAG_LINK_BATTLE);
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      messageId = messageBase + 6;
      goto message;
    } else {
      messageId = messageBase + 3;
      goto message;
    }
  } else {
    // A partner arrived while a monster battle waited: call it off.
    if (GameFlag_IsSet(FLAG_MONSTER_BATTLE)) {
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      Engine_ScriptSetMessage(MsgLobbyOpponentArrived);
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      Engine_GameFlagClear(FLAG_MAP_TEMP_02);
      Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
    }
    if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_02)) {
      messageId = messageBase + 3;
      /* The first branch's answers jump into this block's message tail, and an
       * accepted challenge back into its opening: the listing keeps one copy of
       * each. Written as if/else arms with their own message calls,
       * cross-jumping folds the copies into the last arm instead (differ 51 to
       * 55). */
    message:
      Engine_ScriptSetMessage(messageId);
    open_message:
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
    } else if (!Engine_GameFlagIsSet(FLAG_MAP_TEMP_01) &&
               !GameFlag_IsSet(FLAG_MAPSEC_TEMP_00)) {
      Engine_ScriptSetMessage(messageBase);
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      Engine_GameFlagSet(FLAG_MAPSEC_TEMP_00);
    } else {
      Engine_GameFlagSet(FLAG_MAPSEC_TEMP_00);
      if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_01)) {
        Engine_ScriptSetMessage(messageBase + 2);
      } else {
        Engine_ScriptSetMessage(messageBase + 1);
      }
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      if (Engine_ScriptChooseYesNo(0, 0) == YES) {
        // Yes, and the partner is still there; else the link is lost.
        if (BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE)) {
          Engine_GameFlagSet(FLAG_LINK_BATTLE);
          Engine_GameFlagSet(FLAG_LINK_BATTLE_ARRANGED);
          if (Engine_GameFlagIsSet(FLAG_MAP_TEMP_01)) {
            Engine_ScriptSetMessage(messageBase + 3);
          } else {
            Engine_ScriptSetMessage(messageBase + 4);
          }
          BattleArena_WriteSlotValue(LOBBY_SLOT_ASKED);
          Engine_GameFlagSet(FLAG_MAP_TEMP_02);
          goto open_message;
        } else {
          Engine_GameFlagSet(FLAG_MAP_TEMP_05);
        }
      } else {
        Engine_ScriptSetMessage(messageBase);
        Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      }
    }
  }
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to LOCALID_LINKED_WINS_TELLER, who reports the linked battles won,
/// the total when the leader faces north and those won in a row otherwise;
/// before the first, the line says it will tell them later. Like the original
/// handler, it never ends its script (no Engine_ScriptEnd) and returns nothing.
static s32 BattleArena_ReportWinTallies(s32 localId) {
  u32 facing = ObjectEvent_GetById(LOCALID_PARTY_LEADER)->facing;
  u16 *tally;
  s32 messageBase;

  Engine_ScriptStart();
  Engine_ObjectEventFaceObjectEvent(localId, gGameState.leaderLocalId, 0);
  // FACING_IS_NORTH's test, written out: strictly between northwest and
  // northeast.
  if ((facing - 0xa001) <= 0x3ffe) {
    messageBase = MsgLobbyLooksLikeFirst;
    tally = &gGameState.linkTallies[LINK_TALLY_LINKED_WINS];
    if (*tally != 0) {
      MessageArgument_ClearAll();
      Text_DrawQuantity(*tally, 5);
      Engine_ScriptSetMessage(messageBase + 1);
      Engine_ScriptShowMessage(localId, 0);
    } else {
      Engine_ScriptSetMessage(MsgLobbyTellTotalWins);
      Engine_ScriptShowMessage(localId, 0);
    }
  } else {
    messageBase = MsgLobbyLooksLikeFirstTime;
    tally = &gGameState.linkTallies[LINK_TALLY_LINKED_STREAK];
    if (*tally != 0) {
      MessageArgument_ClearAll();
      Text_DrawQuantity(*tally, 5);
      Engine_ScriptSetMessage(messageBase + 1);
      Engine_ScriptShowMessage(localId, 0);
    } else {
      Engine_ScriptSetMessage(MsgLobbyTellConsecutiveWins);
      Engine_ScriptShowMessage(localId, 0);
    }
  }
#ifdef UBFIX
  return 0;
#endif
}
/// Talking to LOCALID_DEBUG_HELPER: Left and Right choose a preset number from
/// 0 up, A loads it (DebugParty_LoadPreset) and B cancels.
/// LOCALID_LINKED_WINS_TELLER replies: one line for a cancel, one for a preset
/// past the table's end (the loader's nonzero result), one for a loaded party.
static s32 BattleArena_ChooseDebugPartyPreset(s32 localId) {
  struct Window *window;
  s32 presetIndex;
  s32 displayedPreset;
  s32 loadResult;

  Engine_ScriptStart();
  Engine_ObjectEventFaceObjectEvent(localId, gGameState.leaderLocalId, 0);
  Engine_ScriptSetMessage(MsgLobbyChooseParameters);
  Engine_ScriptShowMessage(localId, 0);
  window = Engine_DebugCreateWindow(0, 0, 6, 4, 2);
  presetIndex = 0;
  displayedPreset = -1;
  for (;;) {
    if (presetIndex != displayedPreset) {
      WindowSprite_PrepareForRedraw(window);
      Text_DrawNumber(presetIndex, 3, window, 0, 0);
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
  Window_Close(window, 1);
  if (presetIndex >= 0) {
    loadResult = DebugParty_LoadPreset(presetIndex);
  }
  if (presetIndex < 0) {
    Engine_ScriptSetMessage(MsgLobbyTease);
    Engine_ScriptShowMessage(LOCALID_LINKED_WINS_TELLER, 0);
  } else if (loadResult != 0) {
    Engine_ScriptSetMessage(MsgLobbyNoParameterAsk);
    Engine_ScriptShowMessage(LOCALID_LINKED_WINS_TELLER, 0);
  } else {
    Engine_ScriptSetMessage(MsgLobbySettingsMadeRest);
    Engine_ScriptShowMessage(LOCALID_LINKED_WINS_TELLER, 0);
  }
  Engine_TaskWait(10);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to LOCALID_HEALER, who serves as a Sanctum's healer
/// (Engine_SanctumOpen).
static s32 BattleArena_OpenSanctum(s32 localId) {
  Engine_ScriptStart();
  Engine_SanctumOpen(localId);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Returns 1 when character ownerId is among the party's first three members
/// while a linked battle is arranged (FLAG_LINK_BATTLE_ARRANGED), or its first
/// four otherwise; 0 when not.
static s32 BattleArena_PartyContains(s32 ownerId) {
  s32 count;
  s32 ownerLimit;
  s32 i;

  count = Party_CountMembers();
  ownerLimit = 3;
  if (Engine_GameFlagIsSet(FLAG_LINK_BATTLE_ARRANGED) == 0) {
    ownerLimit = 4;
  }
  if (count > ownerLimit) {
    count = ownerLimit;
  }
  for (i = 0; i < count; i++) {
    if (gGameState.party[i] == 0xff) {
      return 0;
    }
    if (gGameState.party[i] == ownerId) {
      return 1;
    }
  }
  return 0;
}

/// UNUSED: the original empty Battle Arena callback.
UNUSED static void State_NoOp(void) {}

/// Talking to Garet, Ivan, Mia or Jenna, who speaks of the last linked battle
/// (map section flags FLAG_MAPSEC_TEMP_04 and FLAG_MAPSEC_TEMP_05) and whether
/// they fought in it, or else of whether Isaac and they are in the battle
/// party. Each message family holds one line per character, at local id - 1;
/// Felix's holds "?".
static s32 BattleArena_TalkByProgress(s32 localId) {
  s32 messageBase = MsgLobbyCantWaitLets;
  s32 isaacPresent = BattleArena_PartyContains(0);
  s32 speakerPresent = BattleArena_PartyContains(localId);

  Engine_ScriptStart();
  Engine_ObjectEventFaceObjectEvent(localId, gGameState.leaderLocalId, 0);
  if (Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_04)) {

    // Isaac's flag is tested and the result dropped; messageBase first
    // holds whether the speaker fought (FLAG_MAP_TEMP_F0 + local id).
    GameFlag_IsSet(FLAG_MAP_TEMP_F0);
    messageBase = Engine_GameFlagIsSet(localId + 0x2f0);
    if (Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_05)) {
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
  Engine_ScriptSetMessage(messageBase + localId - 1);
  Engine_ScriptShowMessage(localId, 0);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Coord event 1, the way out: stops the link, loads sound preset 2 and goes
/// back to the main menu (its warp destination
/// MAIN_MENU_FROM_SANCTUM).
static s32 BattleArena_LeaveArena(void) {
  SerialRuntime_RemoveIrqHandlers();
  Sound_LoadPresetParameters(2);
  FieldScript_SetDestination((s32)&MapId_MainMenu, MAIN_MENU_FROM_SANCTUM);
#ifdef UBFIX
  return 0;
#endif
}

// The handlers below are s32 and return no value: r0 is whatever
// Engine_ScriptEnd left.

/// Talking to one of three visitors: LOCALID_TRAVELING_WARRIOR,
/// LOCALID_FINALS_VISITOR, who explains the linked finals, and
/// LOCALID_OLD_MANS_ARENA_VISITOR, who wonders whether this is the old man's
/// Battle Arena. Each says their first line unless a linked battle has ended
/// since the leader last walked south of z 224 (map section flag
/// FLAG_MAPSEC_TEMP_04), then their second after a win or their third after a
/// loss (FLAG_MAPSEC_TEMP_05).
static s32 BattleArena_TalkToVisitor(s32 localId) {
  s32 message;
  s32 step = 0;

  Engine_ScriptStart();
  switch (localId) {
  case LOCALID_TRAVELING_WARRIOR:
    message = MsgLobbyTravelingWarrior;
    break;
  case LOCALID_FINALS_VISITOR:
    message = MsgLobbyThreeAlliesFight;
    break;
  case LOCALID_OLD_MANS_ARENA_VISITOR:
  default:
    message = MsgLobbyBattleArenaOld;
    break;
  }
  Engine_ObjectEventFaceObjectEvent(localId, gGameState.leaderLocalId, 0);
  if (Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_04))
    step = 2 - (Engine_GameFlagIsSet(FLAG_MAPSEC_TEMP_05) != 0);
  Engine_ScriptSetMessage(message + step);
  Engine_ScriptOpenMessage(localId, 0);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to LOCALID_PARTY_ORDER_VISITOR, who alternates, by FLAG_MAP_TEMP_04,
/// between the linked finals' limit of three allies (worded for the party's
/// size) and how to change the party's order.
static s32 BattleArena_ExplainPartyOrder(s32 localId) {
  s32 message;

  Engine_ScriptStart();
  Engine_ObjectEventFaceObjectEvent(localId, gGameState.leaderLocalId, 0);
  if (!GameFlag_IsSet(FLAG_MAP_TEMP_04)) {
    if (Party_CountMembers() <= 3)
      message = MsgLobbyThreeAlliesFightLinkedFinals;
    else
      message = MsgLobbyThreeAlliesFightLinked;
    Engine_GameFlagSet(FLAG_MAP_TEMP_04);
  } else {
    message = MsgLobbyChangeOrderParty;
    Engine_GameFlagClear(FLAG_MAP_TEMP_04);
  }
  Engine_ScriptSetMessage(message);
  Engine_ScriptOpenMessage(localId, 0);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to LOCALID_MONSTER_WINS_TELLER, who reports the most monster battles
/// won in a row (tally 2), or before any, that the speaker will count them.
static s32 BattleArena_ReportMonsterRecord(s32 localId) {
  struct GameState *state;

  Engine_ScriptStart();
  state = &gGameState;
  Engine_ObjectEventFaceObjectEvent(localId, state->leaderLocalId, 0);
  if (state->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] != 0) {
    Text_DrawQuantity(state->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK], 5);
    Engine_ScriptSetMessage(MsgLobbyWonNumberConsecutive);
  } else {
    Engine_ScriptSetMessage(MsgLobbyCantWaitSee);
  }
  Engine_ScriptOpenMessage(localId, 0);
  Engine_ScriptEnd();
#ifdef UBFIX
  return 0;
#endif
}

u8 *BattleArena_GetEvents(void) { return (u8 *)gBattleArenaEvents; }

/// Saves the battle results between saving and saved messages, through
/// Save_ProcessSelectedFile, which writes the first 16 bytes of the save
/// stamp (gSaveStamp) into the current file. BattleArena_RunRoundResult calls
/// it after each linked battle won or lost.
static s32 BattleArena_SaveBattleResults(void) {
  // A shared inline message-pair helper grows each save routine from 0x6c to
  // 0x70 in the native build, so retain the two sequences.
  struct Window *window;

  Engine_AudioPlayCue(MUS_SAVED);
  window = Message_OpenWindow(MsgLobbySavingBattleResults, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  Save_ProcessSelectedFile();
  Window_Close(window, 1);
  Engine_TaskWait(1);
  window = Message_OpenWindow(MsgLobbyBattleResultsSaved, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  Window_Close(window, 1);
#ifdef UBFIX
  return 0;
#endif
}

/// The same with the monster battle messages, after a monster series that set
/// a new record.
static s32 BattleArena_SaveMonsterBattleResults(void) {
  // See BattleArena_SaveBattleResults for the measured shared-helper trial.
  struct Window *window;

  Engine_AudioPlayCue(MUS_SAVED);
  window = Message_OpenWindow(MsgLobbySavingMonsterBattle, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  Save_ProcessSelectedFile();
  Window_Close(window, 1);
  Engine_TaskWait(1);
  window = Message_OpenWindow(MsgLobbyMonsterBattleResults, 5, 4, 1);
  while (UiWork_IsComplete() == 0)
    Engine_TaskWait(1);
  Window_Close(window, 1);
#ifdef UBFIX
  return 0;
#endif
}

/// Shows value, at most 999, on the wall board: three map cells at row 8,
/// columns 14 to 16, each copied from column 27 at the row of its digit.
/// BattleArena_RunRoundResult shows the most linked battles won in a row.
static s32 BattleArena_DrawThreeDigitValue(s32 value) {
  s32 digit;

  if (value > 999)
    value = 999;
  for (digit = 0; digit <= 2; digit++) {
    Engine_MapGridCopyCellsTo(27, value % 10, 16 - digit, 8, 1, 1);
    value = value / 10;
  }
  Engine_MapGridRedraw();
#ifdef UBFIX
  return 0;
#endif
}

/// The overlay's initialize entry, run on every arrival: settles the battle
/// that brought the party here and restarts the Battle Arena's polling. Warp
/// destination 8 is a monster battle won, 9 one lost, 10 a linked battle won
/// (or broken off, FLAG_MAPSEC_TEMP_E8) and 11 one lost; any other warp
/// destination (BATTLE_ARENA_FROM_MAIN_MENU, from the main menu)
/// restarts the link. Updates gGameState.linkTallies, saves after linked
/// battles and new monster records, and restores the party's HP, PP and Djinn
/// unless a monster series goes on. Returns 0.
s32 BattleArena_RunRoundResult(void) {
  s32 i;
  struct EventWork *work;
  void (*callback)(void);

  gBattleArenaCallFrames = 0;
  sBattleArenaWaitFrames = 0;
  gFieldWork->transition = MAP_TRANSITION(TRANSITION_WINDOW, 1);
  Sound_LoadPresetParameters(2);
  BattleArena_DrawThreeDigitValue(
      gGameState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
  MapGrid_CopyCellAttributes(11, 11, 1, 1, 13, 10);
  BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  Engine_TaskWait(1);
  TilesetAnim_Pause(5);

  // The payload's halfwords 4 to 7 hold the link battle's phase word
  // ("VSST", "EDVS", "TUrn"); the Battle Arena puts TALK there.
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
    if (BattleArena_PartyContains(i)) {
      Engine_GameFlagSet(0x2f0 + i);
    }
  }
  // Warp destination 8, a monster battle won: offer the next one.
  if (gGameState.warpDest == 8) {
    s32 wins;
    s32 messageOffset;
    s32 v;

    Engine_ScriptStart();
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    BattleArena_WriteSlotValue(LOBBY_SLOT_ALONE);
    gGameState.linkTallies[LINK_TALLY_MONSTER_WINS]++;
    gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK]++;
    // The offer has two lines a level (FLAG_MONSTER_BATTLE_WINS, -1 when a
    // series starts), the even line capped at 14. FLAG_MAPSEC_TEMP_E8's byte
    // counts the victories 0 to 2; the third raises the level and takes the
    // next, odd line (at most 15), which promises a stronger monster.
    wins = (s8)FieldGameFlag_GetByte(FLAG_MONSTER_BATTLE_WINS);
    messageOffset = wins * 2 + 2;
    if (messageOffset > 14) {
      messageOffset = 14;
    }
    v = FieldGameFlag_GetByte(FLAG_MAPSEC_TEMP_E8);
    if (v == 2) {
      FieldGameFlag_SetByte(FLAG_MAPSEC_TEMP_E8, 0);
      wins++;
      messageOffset++;
    } else {
      FieldGameFlag_SetByte(FLAG_MAPSEC_TEMP_E8, v + 1);
    }
    Engine_ObjectEventFaceObjectEvent(LOCALID_ATTENDANT,
                                      gGameState.leaderLocalId, 0);
    Engine_ScriptSetMessage(MsgLobbyNotBadNextMonster + messageOffset);
    Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
    if (Engine_ScriptChooseYesNo(0, 0) == YES) {
      if (wins > 90) {
        wins = 90;
      }
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, wins);
    } else {
      // No: the series ends, and a new record (tally 2) is saved.
      Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
      Text_DrawQuantity(gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK], 5);
      if (gGameState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
          gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
        gGameState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
            gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK];
        Engine_ScriptSetMessage(MsgLobbyWonNumberBattleBroke);
        Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
        BattleArena_SaveMonsterBattleResults();
      } else {
        Engine_ScriptSetMessage(MsgLobbyWonNumberBattle);
        Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      }
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    }
    Engine_ScriptEnd();
    // Warp destination 9, a monster battle lost: the series ends, and a new
    // record is saved.
  } else if (gGameState.warpDest == 9) {
    gGameState.linkTallies[LINK_TALLY_MONSTER_LOSSES]++;
    Engine_ScriptStart();
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    BattleArena_WriteSlotValue(LOBBY_SLOT_ALONE);
    Engine_ObjectEventFaceObjectEvent(LOCALID_ATTENDANT,
                                      gGameState.leaderLocalId, 0);
    Text_DrawQuantity(gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK], 5);
    if (gGameState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
        gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
      gGameState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
          gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK];
      Engine_ScriptSetMessage(MsgLobbyWonNumberBattleBroke);
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
      BattleArena_SaveMonsterBattleResults();
    } else {
      Engine_ScriptSetMessage(MsgLobbyWonNumberBattleTold);
      Engine_ScriptOpenMessage(LOCALID_ATTENDANT, 0);
    }
    gGameState.linkTallies[LINK_TALLY_MONSTER_STREAK] = 0;
    Engine_GameFlagClear(FLAG_MONSTER_BATTLE);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    Engine_ScriptEnd();
    // Warp destination 10, a linked battle won: count the win and the run; a
    // longer run is the board's new record. Saved.
  } else if (gGameState.warpDest == 10) {
    s32 v;

    Engine_ScriptStart();
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
    // The battle broke off instead (Battle_Run set
    // FLAG_MAPSEC_TEMP_E8): report the link lost and restart it.
    if (GameFlag_IsSet(FLAG_MAPSEC_TEMP_E8)) {
      work = gFieldWork;
      Engine_GameFlagClear(FLAG_MAPSEC_TEMP_E8);
      work->raisedEvent = LOBBY_EVENT_DISCONNECTED;
      Engine_GameFlagClear(FLAG_MAPSEC_TEMP_04);
      Engine_TaskWait(20);
      BattleArena_InitializeSerial();
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
    } else {
      gGameState.linkTallies[LINK_TALLY_LINKED_WINS]++;
      v = gGameState.linkTallies[LINK_TALLY_LINKED_STREAK] + 1;
      gGameState.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
      if (gGameState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] < (u16)v)
        gGameState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] = v;
      BattleArena_DrawThreeDigitValue(
          gGameState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
      BattleArena_SaveBattleResults();
      Engine_GameFlagSet(FLAG_MAPSEC_TEMP_04);
      Engine_GameFlagSet(FLAG_MAPSEC_TEMP_05);
    }
    Engine_ScriptEnd();
    // Warp destination 11, a linked battle lost: count the loss and end the
    // run. Saved.
  } else if (gGameState.warpDest == 11) {
    s32 v;

    Engine_ScriptStart();
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
    if ((v = Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) == 0) {
      gGameState.linkTallies[LINK_TALLY_LINKED_LOSSES]++;
      gGameState.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
      BattleArena_SaveBattleResults();
    }
    Engine_GameFlagSet(FLAG_MAPSEC_TEMP_04);
    Engine_GameFlagClear(FLAG_MAPSEC_TEMP_05);
    Engine_ScriptEnd();
    // Arriving from the main menu: start the link, clear
    // FLAG_LINK_BATTLE_ARRANGED and reset the monster level. Idle sleep is
    // turned off here, with a note when it was on.
  } else {
    SerialRuntime_Initialize();
    Engine_GameFlagClear(FLAG_LINK_BATTLE_ARRANGED);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    if (gGameState.idleSleepEnabled != 0) {
      Engine_ScriptStart();
      Engine_ScriptOpenScreen();
      Engine_ScriptWaitForScreen();
      Engine_ObjectEventFaceObjectEvent(LOCALID_ATTENDANT,
                                        gGameState.leaderLocalId, 0);
      Engine_ScriptSetMessage(MsgLobbyNoteCantUse);
      Engine_ScriptShowMessage(LOCALID_ATTENDANT, 0);
      Engine_ScriptEnd();
    }
    gGameState.idleSleepEnabled = 0;
    gIdleSleepEnabled = 0;
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  // Resume polling and restore the party unless continuing a monster run.
  callback = (TaskFunc)BattleArena_PollPeerReady;
  FieldScheduler_AddCallback(callback, 0xc80);
  Task_SetFlags(callback, 1);
  if (gGameState.warpDest != 8 || !Engine_GameFlagIsSet(FLAG_MONSTER_BATTLE)) {
    Party_RestoreHpPp(1);
    DjinnRecovery_TickTwenty();
  }
  return 0;
}

// clang-format off
/// The payload word each slot of gBattleArena_SlotValues goes in: 0 the
/// signature, 1 the state. The last two entries have no slot.
u8 gBattleArena_SlotColumns[] = {
    0, 1, 1, 1, 1, 0, 0, 0,
};

/// Where the party appears: BATTLE_ARENA_FROM_MAIN_MENU from the main menu,
/// 8 and 9 after a monster battle won or lost, 10 and 11 after a linked one
/// (BattleArena_EnterBattle).
struct MapWarpDest gBattleArenaWarpDests[] = {
    { 11, CONDITION_ALWAYS, 128, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { 10, CONDITION_ALWAYS, 352, 0, 148, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { 8, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
    { 9, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_FROM_MAIN_MENU, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { 0, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { MAP_TABLE_END },
};

u32 gBattleArenaWarps[] = {
    MAP_WARPS_END,
};

struct ObjectEventTemplate gBattleArenaObjectEventTemplates[] = {
    { 0x6a, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(304), 0, PIXELS(168), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(272), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x9d, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(64), 0, PIXELS(92), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(40), 0, PIXELS(120), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x2e, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(256), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x66, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(144), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(144), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x99, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(128), 0, PIXELS(164), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(64), 0, PIXELS(136), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(288), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { GARET, IF_FLAG_SET(0x1), MOVEMENT_TYPE_STAND,
        PIXELS(352), 0, PIXELS(108), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { IVAN, IF_FLAG_SET(0x2), MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(136), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { MIA, IF_FLAG_SET(0x3), MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(136), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { JENNA, IF_FLAG_SET(0x5), MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(136), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};

struct ObjectEventTemplate gBattleArenaBattleObjectEventTemplates[] = {
    { 0x6a, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE },
    { 0x74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(280), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x79, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(304), 0, PIXELS(168), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(192), 0, PIXELS(272), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x9d, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(136), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(64), 0, PIXELS(92), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(40), 0, PIXELS(120), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x2e, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(256), 0, PIXELS(384), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6b, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x66, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x98, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(352), 0, PIXELS(108), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x96, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(352), 0, PIXELS(156), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x99, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(376), 0, PIXELS(136), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(64), 0, PIXELS(136), FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(248), 0, PIXELS(184), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(328), 0, PIXELS(88), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { 0x74, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(72), 0, PIXELS(288), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { GARET, IF_FLAG_SET(0x1), MOVEMENT_TYPE_STAND,
        PIXELS(104), 0, PIXELS(144), FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0 },
    { IVAN, IF_FLAG_SET(0x2), MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(144), FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0 },
    { MIA, IF_FLAG_SET(0x3), MOVEMENT_TYPE_STAND,
        PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { JENNA, IF_FLAG_SET(0x5), MOVEMENT_TYPE_STAND,
        PIXELS(128), 0, PIXELS(124), FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0 },
    { MAP_TABLE_END },
};
// clang-format on

struct MapEvent gBattleArenaEvents[] = {
    {MAP_EVENT_TALK, GARET, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_TalkByProgress)},
    {MAP_EVENT_TALK, IVAN, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_TalkByProgress)},
    {MAP_EVENT_TALK, MIA, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_TalkByProgress)},
    {MAP_EVENT_TALK, JENNA, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_TalkByProgress)},
    {MAP_EVENT_TALK, LOCALID_ATTENDANT, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_ChooseBattleMode)},
    {MAP_EVENT_TALK, LOCALID_LINKED_WINS_TELLER, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_ReportWinTallies)},
    {MAP_EVENT_TALK, LOCALID_HEALER, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_OpenSanctum)},
    {MAP_EVENT_TALK, LOCALID_GREETER, CONDITION_ALWAYS,
     MSGNO(MsgFieldWelcomeToBattleArenaWherePeople)},
    {MAP_EVENT_TALK, LOCALID_TRAVELING_WARRIOR, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_TalkToVisitor)},
    {MAP_EVENT_TALK, LOCALID_FINALS_VISITOR, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_TalkToVisitor)},
    {MAP_EVENT_TALK, LOCALID_OLD_MANS_ARENA_VISITOR, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_TalkToVisitor)},
    {MAP_EVENT_TALK, LOCALID_DEBUG_HELPER, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_ChooseDebugPartyPreset)},
    {MAP_EVENT_TALK, LOCALID_PARTY_ORDER_VISITOR, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_ExplainPartyOrder)},
    {MAP_EVENT_TALK, LOCALID_COMMAND_TIME_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldInTheLinkedFinalsYouCant)},
    {MAP_EVENT_TALK, LOCALID_SUMMON_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldToUseSummonQuicklyDjinnMust)},
    {MAP_EVENT_TALK, LOCALID_SET_DJINN_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldDoYouPutDjinnOnStandby)},
    {MAP_EVENT_TALK, LOCALID_STANDBY_DJINN_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldRemoveDjinnYouRarelyUseSo)},
    {MAP_EVENT_TALK, LOCALID_MONSTER_RULES_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldMonsterBattleRulesSayYouAll)},
    {MAP_EVENT_TALK, LOCALID_MONSTER_WINS_TELLER, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_ReportMonsterRecord)},
    {MAP_EVENT_TALK, LOCALID_EDGY_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldAllTheWarriorsAreImpressiveSo)},
    {MAP_EVENT_TALK, LOCALID_WARY_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldICouldEnterButIdJust)},
    {MAP_EVENT_COORD, 10, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_EnterBattle)},
    {MAP_EVENT_COORD, 1, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_LeaveArena)},
    {MAP_EVENT_RAISED, 1, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_PromptToParticipate)},
    {MAP_EVENT_RAISED, 2, CONDITION_ALWAYS,
     EVENT_SCRIPT(BattleArena_ReportDisconnection)},
    {MAP_EVENTS_END},
};
