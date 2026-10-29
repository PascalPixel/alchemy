/// @file
/// The Battle Arena overlay: starting linked and monster battles, the exchange
/// of the two parties, the attendant and the records of wins.

#include "module/import.h"
#include "module/scene/battle_arena.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/battle/battle.h"
#include "module/field/field.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/system/system.h"
#include "battle.h"
#include "input.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "types.h"
#include "edition.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "types.h"
#include "event.h"
#include "event_inline.h"
#include "io_reg.h"
#include "serial.h"
#include "iwram_call.h"
#include "character.h"
#include "heap_release.h"
#include "tbs_edition.h"
#include "text/msg_ids.h"
#include "task.h"
#include "scene_ids.h"
#include "main_routine_work.h"
#include "battle_side.h"
#include "irq_lock.h"
#include "sound_driver_symbols.h"
#include "scene_tables.h"
#include "frame.h"
#include "flags.h"
#include "heap_slots.h"
#include "sounds.h"
#include "heap.h"

/// The attendant has asked the player to take part; every 300th call of
/// BattleArena_PromptToParticipate clears it so she asks again.
/// ★ Named from the game: "Would you like to apply?" (MsgLobbyPleaseSpeakWhen)
#define FLAG_BATTLE_ARENA_ATTENDANT_ASKED FLAG_SCENE_TEMP_00
/// The linked partner has asked for a battle: its signature matches and it
/// stands asked or in the circle.
/// ★ Named from the game: "Your opponent has asked for a battle. Do you
/// accept?" (MsgLobbyPleaseSpeakWhen)
#define FLAG_BATTLE_ARENA_PARTNER_ASKED FLAG_SCENE_TEMP_01
/// The player has agreed to a battle.
/// ★ Named from the game: "When you finish your preparations, please step into
/// the circle!" (MsgLobbyPleaseSpeakWhen)
#define FLAG_BATTLE_ARENA_PLAYER_AGREED FLAG_SCENE_TEMP_02
/// The party is on its way into the battle.
/// ★ Named from the game: "Good luck!" (MsgLobbyGoodLuck)
#define FLAG_BATTLE_ARENA_ENTERING_BATTLE FLAG_SCENE_TEMP_03
/// ACTOR_PARTY_ORDER_VISITOR has given the linked finals' limit of three
/// allies, so the next talk explains how to change the party's order.
/// ★ Named from the game: "You can change the order of your party in the status
/// screen." (MsgLobbyChangeOrderParty)
#define FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT FLAG_SCENE_TEMP_04
/// The link was lost and the attendant has yet to say so.
/// ★ Named from the game: "The link was disconnected."
/// (MsgLobbyLinkDisconnected)
#define FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED FLAG_SCENE_TEMP_05
/// The first of eight flags, one per character from Isaac, each set while that
/// character is in the battle party (BattleArena_RunRoundResult).
#define FLAG_BATTLE_ARENA_IN_PARTY_FIRST FLAG_SCENE_TEMP_F0
/// The attendant has explained the Battle Arena once; talking to her again
/// offers to apply.
/// ★ Named from the game: "Please speak to me when you want to fight in a
/// battle." (MsgLobbyPleaseSpeakWhen)
#define FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED FLAG_AREA_TEMP_00
/// This console is the link's child (multiplayer id 1), which receives the
/// parties first.
/// ★ Named from the game: "Test your skills in Linked Battles and Monster
/// Battles!" (MsgFieldWelcomeToBattleArenaWherePeople)
#define FLAG_BATTLE_ARENA_LINK_CHILD FLAG_AREA_TEMP_02
/// The link is up: both consoles were heard (LOBBY_LINK_READY_MASK).
/// ★ Named from the game: "Your opponent has arrived!"
/// (MsgLobbyOpponentArrived)
#define FLAG_BATTLE_ARENA_LINK_UP FLAG_AREA_TEMP_03
/// A linked battle has ended since the leader last walked south of z 224, so
/// the party and the visitors speak of it.
/// ★ Named from the game: "Battle results were saved."
/// (MsgLobbyBattleResultsSaved)
#define FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED FLAG_AREA_TEMP_04
/// The last linked battle was won.
/// ★ Named from the game: "Yeah! We won!" (MsgLobbyWonToldCountOn)
#define FLAG_BATTLE_ARENA_LINK_BATTLE_WON FLAG_AREA_TEMP_05

// Link transfers use three unit records followed by one recovery-table record.
// Both sizes are whole 20-byte blocks, since the serial runtime sends 20 bytes
// a block and ends a send only when its count reaches exactly 0.
/// The bytes sent for each party member: a Unit (BATTLE_UNIT_SIZE, 0x14c) and
/// the eight bytes after it, 17 blocks of 20.
#define LOBBY_UNIT_RECORD_BYTES 0x154
/// The bytes sent for the Djinn recovery table after the party members: the
/// table (struct DjinnRecoveryTable, 268 bytes) and the 52 bytes after it, 16
/// blocks of 20, two more than the table needs.
#define LOBBY_RECOVERY_RECORD_BYTES 0x140
/// The party members a linked battle takes; a smaller party sends absent
/// records.
#define LOBBY_UNIT_RECORD_COUNT 3
/// gLinkStatus bits 0 and 1: both consoles were heard this frame, as the
/// battle code's LINK_BOTH_CONNECTED.
#define LOBBY_LINK_READY_MASK 3
/// A record fails on the 25th frame it waits without a link or past the
/// timeout.
#define LOBBY_TRANSFER_RETRY_LIMIT 24
/// Frames, about ten seconds, for the three party member records, and again for
/// the recovery table.
#define LOBBY_SEND_TIMEOUT_FRAMES 600
/// Frames, about fifteen seconds, for all four records received.
#define LOBBY_RECEIVE_TIMEOUT_FRAMES 900
/// Frames without a link (BattleArena_PollPeerReady) or without the partner's
/// request (BattleArena_EnterBattle) before the Battle Arena gives the partner
/// up.
#define LOBBY_PEER_WAIT_FRAMES 25

// The Battle Arena's flags, besides its own above, FLAG_MONSTER_BATTLE,
// FLAG_LINK_BATTLE and the two byte flags flags.h names:
// - FLAG_LINK_BATTLE_ARRANGED: a linked battle has been arranged (with
//   FLAG_LINK_BATTLE): only the first three party members take part
//   (BattleArena_PartyContains).
// - FLAG_AREA_TEMP_E8: the last linked battle broke off (Battle_Run). In a
//   monster series, the byte flag at FLAG_AREA_TEMP_E8 instead counts the
//   victories, 0 to 2, towards a stronger monster.

/// The actors, numbering the rows of gBattleArenaActorPlacements
/// and gBattleArenaBattleActorPlacements, which list the same people in
/// the same order; the rows of Garet, Ivan, Mia and Jenna take those
/// characters' own ids.
enum BattleArenaActorId {
  /// The attendant, who arranges every battle; ACTOR_GREETER calls her the
  /// girl at the counter.
  /// ★ Named from the game: "Please speak to me when you want to fight in a
  /// battle." (MsgLobbyPleaseSpeakWhen)
  ACTOR_ATTENDANT = ACTOR_FIRST_PLACED,
  /// Reports the linked battles won, and answers for ACTOR_DEBUG_HELPER.
  /// ★ Named from the game: "I will tell you your total wins when you finish
  /// the linked finals." (MsgLobbyTellTotalWins)
  ACTOR_LINKED_WINS_TELLER,
  /// Opens a Sanctum's services (BattleArena_OpenSanctum); sprite 0x79, the
  /// healer of every Sanctum in sanctum.c.
  /// ★ Named from the game: "Healer" (MsgDebugGlyphName)
  ACTOR_HEALER,
  /// ★ Named from the game: "Welcome to Battle Arena, where people craving
  /// battle meet!" (MsgFieldWelcomeToBattleArenaWherePeople)
  ACTOR_GREETER,
  /// ★ Named from the game: "I am a traveling warrior. I came here to become
  /// stronger..." (MsgLobbyTravelingWarrior)
  ACTOR_TRAVELING_WARRIOR,
  /// ★ Named from the game: "Up to three allies can fight in Linked Battle
  /// finals." (MsgLobbyThreeAlliesFight)
  ACTOR_FINALS_VISITOR,
  /// ★ Named from the game: "Is this the Battle Arena the old man spoke of?"
  /// (MsgLobbyBattleArenaOld)
  ACTOR_OLD_MANS_ARENA_VISITOR,
  /// Loads a debug parameter set (BattleArena_ChooseDebugParameterSet).
  /// ★ Named from the game: "Choose your parameters here."
  /// (MsgLobbyChooseParameters)
  ACTOR_DEBUG_HELPER,
  /// Explains the linked finals' limit of three allies, then the party order.
  /// ★ Named from the game: "You can change the order of your party in the
  /// status screen." (MsgLobbyChangeOrderParty)
  ACTOR_PARTY_ORDER_VISITOR,
  /// ★ Named from the game: "In the linked finals, you can't delay after your
  /// opponent selects a move." (MsgFieldInTheLinkedFinalsYouCant)
  ACTOR_COMMAND_TIME_VISITOR,
  /// ★ Named from the game: "To use Summon quickly, Djinn must be on standby."
  /// (MsgFieldToUseSummonQuicklyDjinnMust)
  ACTOR_SUMMON_VISITOR,
  /// ★ Named from the game: "Do you put Djinn on standby before a battle, or
  /// do you set them?" (MsgFieldDoYouPutDjinnOnStandby)
  ACTOR_SET_DJINN_VISITOR,
  /// ★ Named from the game: "Remove Djinn you rarely use so you can summon
  /// quickly." (MsgFieldRemoveDjinnYouRarelyUseSo)
  ACTOR_STANDBY_DJINN_VISITOR,
  /// ★ Named from the game: "Monster Battle rules say you all must fight as a
  /// team against a monster." (MsgFieldMonsterBattleRulesSayYouAll)
  ACTOR_MONSTER_RULES_VISITOR,
  /// Reports the most Monster Battles won in a row.
  /// ★ Named from the game: "I'll be counting your victories!"
  /// (MsgLobbyCantWaitSee)
  ACTOR_MONSTER_WINS_TELLER,
  /// ★ Named from the game: "All the warriors are impressive, so I get edgy
  /// before battles begin." (MsgFieldAllTheWarriorsAreImpressiveSo)
  ACTOR_EDGY_VISITOR,
  /// ★ Named from the game: "I could enter, but I'd just get hurt if I did..."
  /// (MsgFieldICouldEnterButIdJust)
  ACTOR_WARY_VISITOR
};

/// The words of gBattleArena_SlotValues, by slot: the signature, then the state
/// a console publishes.
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

/// The Battle Arena's raised triggers (FieldWork.raisedTrigger).
enum BattleArenaRaisedTrigger {
  LOBBY_EVENT_NONE = 0,
  LOBBY_EVENT_PROMPT = 1,      ///< BattleArena_PromptToParticipate
  LOBBY_EVENT_DISCONNECTED = 2 ///< BattleArena_ReportDisconnection
};

/// gMainRoutineWork.linkTallies, as the Battle Arena keeps them.
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
/// LobbySlot. The signature is in the first column (gBattleArena_SlotColumns),
/// the states in the second. In English and French LOBBY_SLOT_ALONE equals the
/// signature and changes nothing; elsewhere it is the signature's letters
/// reversed, so while a console publishes it (no partner answered, or a
/// monster series runs) a partner's signature test fails and the attendant
/// does not offer that console as an opponent, until each return to linking
/// writes the signature back.
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
struct LinkPeerPayload {
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

/// The Battle Arena's entrances, which FieldSequence_Run reads through
/// entry/battle_arena.s.
u8 *BattleArena_GetEntrances(void) { return (u8 *)gBattleArenaEntrances; }

/// The exit regions entry: the Battle Arena has no exit boxes.
s32 Scene_GetExitRegions(void) { return 0; }

/// Restarts the link with interrupts locked: removes the serial and timer 3
/// handlers, then runs Serial_Initialize. Called when no partner
/// answers the attendant and after a linked battle broke off. The original
/// s32 entry produces no defined result; its callers ignore it.
static s32 BattleArena_InitializeSerial(void) {
  u32 savedIrqState;

  // Returns no value: the ROM has the s32 epilogue (pop {r1}; bx r1) and no
  // result.

  IRQ_LOCK(savedIrqState);
  Serial_RemoveIrqHandlers();
  Serial_Initialize();
  IRQ_UNLOCK(savedIrqState);
#ifdef UBFIX
  return 0;
#endif
}

u8 *BattleArena_GetExits(void) { return (u8 *)gBattleArenaExits; }

/// The actor placements entry. After a defeat
/// (BATTLE_ARENA_AFTER_MONSTER_LOSS or BATTLE_ARENA_AFTER_LINK_LOSS) the party
/// party members and four visitors roughly trade places
/// (gBattleArenaBattleActorPlacements): Garet, Ivan and Mia or Jenna take
/// three of the visitors' spots, and the visitors stand around where the party
/// stood.
s32 BattleArena_SelectActorPlacements(void) {
  s16 entrance = gMainRoutineWork.entrance;

  if (entrance == BATTLE_ARENA_AFTER_LINK_LOSS ||
      entrance == BATTLE_ARENA_AFTER_MONSTER_LOSS) {
    return (s32)gBattleArenaBattleActorPlacements;
  }
  return (s32)gBattleArenaActorPlacements;
}

/// Returns 1 when the partner's payload holds slot's word in slot's column, 0
/// when it does not or there is no link. Also sets or clears area flags
/// FLAG_BATTLE_ARENA_LINK_UP (the link is up) and FLAG_BATTLE_ARENA_LINK_CHILD
/// (this console is the child), and reads the other console's payload by them.
static s32 BattleArena_PeerSlotMatches(s32 slot) {
  s32 serialPlayerId = -1;

  if ((gLinkStatus & LOBBY_LINK_READY_MASK) == LOBBY_LINK_READY_MASK) {
    // SIOCNT bits 4 and 5: 0 for the parent, 1 for the child.
    serialPlayerId = (REG_SIOCNT << 26) >> 30;
    GameFlag_Set(FLAG_BATTLE_ARENA_LINK_UP);
  } else {
    GameFlag_Clear(FLAG_BATTLE_ARENA_LINK_UP);
  }
  /* FAKEMATCH(one-pass): a loop left by break after its first pass, which
   * places the test after the body as the ROM does; as an if around the body,
   * 12 of 58 instructions differ in tbs-en. */
  while (serialPlayerId >= 0 && GameFlag_Get(FLAG_BATTLE_ARENA_LINK_UP)) {
    const s32 *expected = &gBattleArena_SlotValues[slot];

    if (serialPlayerId != 0)
      GameFlag_Set(FLAG_BATTLE_ARENA_LINK_CHILD);
    else
      GameFlag_Clear(FLAG_BATTLE_ARENA_LINK_CHILD);
    {
      struct LinkPeerPayload *peer = (struct LinkPeerPayload *)
          gLinkPeerSignatures[GameFlag_Get(FLAG_BATTLE_ARENA_LINK_CHILD) ^ 1];

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
/// The task that watches the partner each frame in the Battle Arena. It forgets
/// the partner after LOBBY_PEER_WAIT_FRAMES without a link, keeps
/// FLAG_BATTLE_ARENA_PARTNER_ASKED current, raises LOBBY_EVENT_PROMPT once both
/// sides agree and LOBBY_EVENT_DISCONNECTED when the link is lost while a
/// battle is being arranged. BattleArena_RunRoundResult schedules it on every
/// arrival; BattleArena_EnterBattle removes it.
static void BattleArena_PollPeerReady(void) {
#else
static s32 BattleArena_PollPeerReady(void) {
#endif
  struct FieldWork *work;
  s32 result;
  s32 i;

  work = gFieldWork;
  result = 1;
  // The leader is south of z 224 (16.16 pixels), towards the way out: the
  // party stops talking about the last battle.
  if (Actor_GetById(ACTOR_PARTY_LEADER)->z.fixed > 0xe00000) {
    GameFlag_Clear(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
  }
  // While LOBBY_EVENT_DISCONNECTED waits, skip the partner tracking.
  if (work->raisedTrigger != LOBBY_EVENT_DISCONNECTED) {
    BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE);
    if (!GameFlag_Get(FLAG_BATTLE_ARENA_LINK_UP)) {
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
        GameFlag_Set(FLAG_BATTLE_ARENA_PARTNER_ASKED);
        if (GameFlag_Get(FLAG_BATTLE_ARENA_PLAYER_AGREED)) {
          work->raisedTrigger = LOBBY_EVENT_PROMPT;
        }
        result = 1;
      } else {
        GameFlag_Clear(FLAG_BATTLE_ARENA_PARTNER_ASKED);
        result = 0;
      }
    }
    if (GameFlag_Get(FLAG_BATTLE_ARENA_PARTNER_ASKED) &&
        GameFlag_Get(FLAG_BATTLE_ARENA_PLAYER_AGREED) &&
        !GameFlag_Get(FLAG_BATTLE_ARENA_ATTENDANT_ASKED)) {
      work->raisedTrigger = LOBBY_EVENT_PROMPT;
    }
  }
  // The 25th frame without a link while a linked battle is being arranged:
  // report the link lost.
  if ((Event_GetFlag(FLAG_BATTLE_ARENA_PARTNER_ASKED) ||
       GameFlag_Get(FLAG_BATTLE_ARENA_PLAYER_AGREED)) &&
      !GameFlag_Get(FLAG_MONSTER_BATTLE) &&
      !BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) &&
      sBattleArenaWaitFrames > 24) {
    work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
    GameFlag_Set(FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED);
    GameFlag_Clear(FLAG_BATTLE_ARENA_PARTNER_ASKED);
    GameFlag_Clear(FLAG_BATTLE_ARENA_PLAYER_AGREED);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  if (GameFlag_Get(FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED)) {
    work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
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
/// FLAG_BATTLE_ARENA_ENTERING_BATTLE or FLAG_BATTLE_ARENA_ATTENDANT_ASKED is
/// set and nothing is shown; the path that shows the line returns nothing in
/// the ROM (UB).
static s32 BattleArena_PromptToParticipate(void) {
  s32 set;

  set = GameFlag_Get(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
  if (set != 0) {
    return set;
  }
  if (++gBattleArenaCallFrames == 300) {
    gBattleArenaCallFrames = 0;
    GameFlag_Clear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
  }
  set = Event_GetFlag(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
  if (set != 0) {
    return set;
  }
  Event_Start();
  Event_SetMessageId(MsgLobbyWantParticipatePlease);
  FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
  wait(5);
  GameFlag_Set(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// LOBBY_EVENT_DISCONNECTED: the attendant says the link was disconnected.
/// Publishes LOBBY_SLOT_IDLE and clears the raised trigger and
/// FLAG_BATTLE_ARENA_ATTENDANT_ASKED, FLAG_BATTLE_ARENA_ENTERING_BATTLE and
/// FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED. The original handler has no defined
/// return value.
static s32 BattleArena_ReportDisconnection(void) {
  struct FieldWork *work = gFieldWork;

  BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  GameFlag_Clear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
  GameFlag_Clear(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
  Event_Start();
  work->raisedTrigger = LOBBY_EVENT_NONE;
  Event_SetMessageId(MsgLobbyLinkDisconnected);
  FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
  GameFlag_Clear(FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Receives the partner's three party member records into units 0x80 to 0x82
/// (the battle's enemies, by Unit_Get) and its Djinn recovery table into the
/// enemy side's (Djinn_GetRecoveryTable(1)). Each name gets the edition's
/// MsgEnemyLabel ("Enemy" in English) in front, within the 14 letters a
/// Unit name holds. Returns how many records hold a party member, or -1 when
/// a transfer fails, overruns or runs out of time. The EWRAM pool buffer
/// it allocates and frees is never written: the records are received in
/// place.
static s32 BattleArena_ReceivePartyRecords(void) {
  /* FAKEMATCH(volatile): gSerialReceivedSize is volatile (serial.h),
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
  heap = Heap_AllocEwram(size);
  result = 0;
  timeout = LOBBY_RECEIVE_TIMEOUT_FRAMES;
  slot = 0;
  // The record loop and both waits are written with goto (MATCHING.md): as a
  // for loop round while loops, 105 of 189 instructions differ in tbs-en.
  goto next;
waitRecord:
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
testRecord:
  if (Serial_GetActiveTransfers() != 0) {
    goto waitRecord;
  }
  if (gSerialReceivedSize != size) {
    result = -1;
    goto done;
  }
  // Unit's side (0x12a): the sender marks a party member 2 and a padding
  // record 0.
  if (record[0x12a] != 0) {
    result++;
  }
  wait(2);
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
  // The partner's party members become this battle's enemies 0x80 to 0x82.
  record = (u8 *)Unit_Get(slot + 128);
  tries = 0;
  if ((transferResult = Serial_StartReceive(record)) == -1) {
    goto failed;
  }
  goto testRecord;
waitRecovery:
  transferResult = gSerialReceivedSize;
  if (transferResult > LOBBY_RECOVERY_RECORD_BYTES) {
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
testRecovery:
  if (Serial_GetActiveTransfers() != 0) {
    goto waitRecovery;
  }
  if (gSerialReceivedSize != LOBBY_RECOVERY_RECORD_BYTES) {
    result = -1;
  } else {
    wait(2);
  }
  goto done;
second:
  // Then the recovery table, into the enemy side's (unit 0x83's record).
  Heap_Release(heap);
  size = LOBBY_RECOVERY_RECORD_BYTES;
  heap = Heap_AllocEwram(size);
  tries = 0;
  if ((transferResult = Serial_StartReceive(Djinn_GetRecoveryTable(1))) != -1) {
    goto testRecovery;
  }
failed:
  result = transferResult;
done:
  Heap_Release(heap);
  return result;
}

/// Copies the character ids of the first three party members
/// (gMainRoutineWork.party), ends them with 0xff and returns how many. A
/// null destination only counts them.
static s32 BattleArena_CopyPartyOwnerIds(u16 *memberIds) {
  s32 memberCount = Party_CountMembers();
  if (memberCount > LOBBY_UNIT_RECORD_COUNT)
    memberCount = LOBBY_UNIT_RECORD_COUNT;
  if (memberCount > 0) {
#ifndef UBFIX
    s16 *partyAddress = (s16 *)&gMainRoutineWork;
#endif
    const u8 *source;
    s32 remaining;
#ifdef UBFIX
    // The original short-pointer addition traverses a struct as an array.
    // The actual member avoids that UB; used plainly, it removes four native
    // bytes (0x44 to 0x40), so matching builds retain the old address form.
    source = gMainRoutineWork.party;
#else
    partyAddress += 252;
    source = (const u8 *)partyAddress;
#endif
    remaining = memberCount;
    do {
      u8 memberId = *source++;
      if (memberIds != 0) {
        *memberIds = memberId;
        memberIds++;
      }
      remaining--;
    } while (remaining != 0);
  }
  if (memberIds != 0)
    *memberIds = 0x00ff;
  return memberCount;
}

/// Sends the first three party members' records, marked side 2
/// (BATTLE_SIDE_PARTY), and for a smaller party absent records, copies of
/// the last party member marked side 0. Then sends this side's Djinn recovery
/// table, rewritten for the partner: each entry's party member becomes the
/// number the partner gives that party member (0x80 plus its place), and the
/// entries of party members not sent are dropped. Returns 0, or -1 when a send
/// fails or runs out of time. The copies go through an EWRAM pool buffer freed
/// before returning.
static s32 BattleArena_SendPartyRecords(void) {
  // Measured plain form: replacing only the final wait and test polling
  // region with while(active transfers) adds 8 bytes in tbs-en.
  s32 result;
  u8 *slotMap;
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
  memberCount = BattleArena_CopyPartyOwnerIds(memberIds);
  slotMap = peerSlots;
  for (i = 7; i >= 0; i--) {
    slotMap[i] = 0;
  }
  for (i = 0; i < memberCount; i++) {
    Iwram_CopyWords(heap, Unit_Get(memberIds[i]), size);
    heap[0x12a] = 2;
    // As a u8, i - 128 is 0x80 + i: this party member's unit on the partner's
    // side.
    slotMap[memberIds[i]] = i - 128;
    transferResult = Serial_StartSend(heap, size);
    tries = 0;
    if (transferResult == -1) {
      result = transferResult;
      goto done;
    }
    while (Serial_GetActiveTransfers() != 0) {
      wait(1);
      if (--timeout < 0 ||
          (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
        if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
          result = -1;
          goto done;
        }
      }
    }
    wait(2);
  }
  // The padding loop is written with goto (MATCHING.md): as for (; i <= 2;
  // i++) round a while loop, 127 of 254 instructions differ in tbs-en.
  goto next;
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
  if (Serial_GetActiveTransfers() != 0) {
    goto wait;
  }
  wait(2);
  i++;
next:
  // Pad to three records with the last copy marked absent (side 0).
  if (i <= 2) {
    heap[0x12a] = tries = 0;
    if ((transferResult = Serial_StartSend(heap, size)) != -1) {
      goto test;
    }
    result = transferResult;
    goto done;
  }
  Heap_Release(heap);
  size = LOBBY_RECOVERY_RECORD_BYTES;
  heap = Heap_AllocEwram(size);
  /* FAKEMATCH(one-pass): the recovery table's copy in a one-pass block, which
   * gives the copy's argument order and keeps the list base's copy first after
   * the call; as a plain call, 6 of 254 instructions differ in tbs-en. */
  do {
    Iwram_CopyWords(heap, Djinn_GetRecoveryTable(0), size);
  } while (0);
  {
    s32 timeout;
    s32 tries;

    list = &((struct DjinnRecoveryTable *)heap)->list;
    tries = 0;
    timeout = LOBBY_SEND_TIMEOUT_FRAMES;
    // Give each Djinni on the list its party member's number on the partner's
    // side, and drop those of party members not sent (peerSlots 0).
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
    if ((transferResult = Serial_StartSend(heap, size)) == -1) {
      result = transferResult;
      goto done;
    }
    goto testOffer;
  waitOffer:
    wait(1);
    if (--timeout < 0 ||
        (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
      if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
        result = -1;
        goto done;
      }
    }
  testOffer:
    if (Serial_GetActiveTransfers() != 0) {
      goto waitOffer;
    }
    wait(1);
    wait(2);
  }
  /* FAKEMATCH(one-pass): the shared exit in a one-pass block, so loop.c finds
   * no barrier at done to move the record loop's two failures to; as a plain
   * label, 62 of 254 instructions differ in tbs-en. */
  do {
  done:
    Heap_Release(heap);
    return result;
  } while (0);
}

/// Exchanges the two parties: the parent sends first, the child (area
/// flag FLAG_BATTLE_ARENA_LINK_CHILD) receives first. On success stores the
/// partner's party member count in FLAG_LINK_PARTY_COUNT and returns it; on
/// failure resets the transfer state and returns a negative result.
static s32 BattleArena_ExchangePartyRecords(void) {
  s32 result;
  s32 receivedMemberCount;
  s32 receiveFirst;

  result = 0;
  /* FAKEMATCH(one-pass): an empty once-only loop after result's zero, which
   * loads it into r6 ahead of the call as the ROM does; as a plain statement, 4
   * of 63 instructions differ in tbs-en. */
  do {
  } while (0);
  receiveFirst = GameFlag_Get(FLAG_BATTLE_ARENA_LINK_CHILD);
  gLinkExchangeState = result;
  // Every failure reaches the one reset: as nested ifs before a single
  // if (result < 0), 16 of 62 instructions differ in tbs-en.
  if (!receiveFirst) {
    wait(5);
    result = BattleArena_SendPartyRecords();
    if (result < 0)
      goto fail;
    wait(5);
    receivedMemberCount = result = BattleArena_ReceivePartyRecords();
    if (result < 0)
      goto check;
  } else {
    receivedMemberCount = result = BattleArena_ReceivePartyRecords();
    if (result < 0)
      goto fail;
    wait(10);
    result = BattleArena_SendPartyRecords();
    if (result < 0)
      goto fail;
  }
  FieldGameFlag_SetByte(FLAG_LINK_PARTY_COUNT, receivedMemberCount);
  result = receivedMemberCount;
check:
  if (receivedMemberCount < 0) {
  fail:
    Serial_ResetTransferState();
  }
  return result;
}

/// Step trigger 10, the circle: starts the battle. A monster battle starts at
/// once. A linked one needs the attendant's call
/// (FLAG_BATTLE_ARENA_ATTENDANT_ASKED) and no lost link waiting to be reported
/// (FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED); it publishes LOBBY_SLOT_IN_CIRCLE
/// and waits for the partner to step in, giving up when the partner withdraws,
/// the link is lost, or the partner neither asks nor stands in the circle for
/// LOBBY_PEER_WAIT_FRAMES. The leader then walks in while the parties are
/// exchanged; a failed exchange walks back, restarts the Battle Arena's polling
/// and raises LOBBY_EVENT_DISCONNECTED. Sets where the battle returns to (the
/// BATTLE_ARENA_AFTER_ entrances) and requests it. The original returns
/// have no defined status.
static s32 BattleArena_EnterBattle(void) {
  struct FieldWork *work;
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
  if (GameFlag_Get(FLAG_MONSTER_BATTLE)) {
    Event_Start();
  } else {
    if (!GameFlag_Get(FLAG_BATTLE_ARENA_ATTENDANT_ASKED))
      return LOBBY_NO_RESULT;
    if (GameFlag_Get(FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED))
      return LOBBY_NO_RESULT;
    Event_Start();
    GameFlag_Set(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IN_CIRCLE);
    if (!BattleArena_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))
      window = Message_OpenWindow(MsgLobbyAwaitingOpponent, 5, 4, 1);
    while (!BattleArena_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE)) {
      wait(1);
      cancelEntry = 0;
      if (!GameFlag_Get(FLAG_BATTLE_ARENA_PARTNER_ASKED))
        cancelEntry = 1;
      if (GameFlag_Get(FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED))
        cancelEntry = 1;
      if (!BattleArena_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE) &&
          !BattleArena_PeerSlotMatches(LOBBY_SLOT_ASKED)) {
        if (++missedFrames > LOBBY_PEER_WAIT_FRAMES)
          cancelEntry = 1;
      } else {
        missedFrames = 0;
      }
      if (cancelEntry) {
        work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
        GameFlag_Set(FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED);
        GameFlag_Clear(FLAG_BATTLE_ARENA_PARTNER_ASKED);
        GameFlag_Clear(FLAG_BATTLE_ARENA_PLAYER_AGREED);
        BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
        failed = 1;
        GameFlag_Clear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
        break;
      }
    }
    if (window)
      Window_Close(window, 1);
    wait(5);
  }
  // Suspend peer polling while transferring the party and entering battle.
  if (!failed) {
    // HEAP_SLOT_ENEMY_UNITS is gEnemyUnits, the battle's enemy
    // units (LINK_OWNER_STATES_SIZE, battle/battle/run.c). While it is
    // allocated, the exchange below receives the partner's party members and
    // Djinn into units 0x80 to 0x83. The copy to gActorSpriteSlots
    // (gMapGridCellBuffer
    // + 0x8000) hands them to Battle_Run, which copies them back.
    savedWork = HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, 0x7c8);
    Task_Remove((TaskFunc)BattleArena_PollPeerReady);
    // Run the map's tileset animation 5 for eight frames.
    TilesetAnim_Resume(5);
    wait(8);
    TilesetAnim_Pause(5);
    if (GameFlag_Get(FLAG_MONSTER_BATTLE)) {
      Actor_SetAngleToward(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
      Event_SetMessageId(MsgLobbyGoodLuck);
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      wait(45);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, Q16_ONE, 0x8000);
      Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 216, 184);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 216, 168);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    } else {
      // Speeds are 16.16 pixels a frame: one to (216, 200), a tenth while
      // the parties are exchanged, then a half.
      Actor_SetSpeed(ACTOR_PARTY_LEADER, Q16_ONE, 0x8000);
      Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 216, 200);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x1999, 0xccc);
      Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 216, 168);
      // A failed exchange: walk back out and return to the Battle Arena,
      // reporting the link lost (LOBBY_EVENT_DISCONNECTED).
      if (BattleArena_ExchangePartyRecords() < 0) {
        Actor_SetSpeed(ACTOR_PARTY_LEADER, Q16_ONE, 0x8000);
        Actor_ResetAndSetPositionInMode2(ACTOR_PARTY_LEADER, 216, 200);
        TilesetAnim_Resume(5);
        wait(8);
        TilesetAnim_Pause(5);
        Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
        HeapSlot_Release(HEAP_SLOT_ENEMY_UNITS);
        BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
        BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
        Task_AddFrame((TaskFunc)BattleArena_PollPeerReady, TASK_PASS_FRAME);
        Task_SetFlags((TaskFunc)BattleArena_PollPeerReady, 1);
        GameFlag_Clear(FLAG_BATTLE_ARENA_PARTNER_ASKED);
        GameFlag_Clear(FLAG_BATTLE_ARENA_PLAYER_AGREED);
        GameFlag_Clear(FLAG_BATTLE_ARENA_LINK_UP);
        GameFlag_Clear(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
        GameFlag_Clear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
        work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
        Event_End();
        return LOBBY_NO_RESULT;
      }
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
      Actor_WaitForMoveById(ACTOR_PARTY_LEADER);
    }
    if (GameFlag_Get(FLAG_MONSTER_BATTLE)) {
      Event_SetAfterBattleDestination((s32)&SceneId_BattleArena,
                                      BATTLE_ARENA_AFTER_MONSTER_WIN);
      Event_SetDefeatDestination((s32)&SceneId_BattleArena,
                                 BATTLE_ARENA_AFTER_MONSTER_LOSS);
    } else {
      Event_SetAfterBattleDestination((s32)&SceneId_BattleArena,
                                      BATTLE_ARENA_AFTER_LINK_WIN);
      Event_SetDefeatDestination((s32)&SceneId_BattleArena,
                                 BATTLE_ARENA_AFTER_LINK_LOSS);
    }
    // Publish EXEC only after the battle and return destinations are ready.
    // It fills the payload's first four halfwords, where the link battle's
    // command entry looks for it in the partner's (battle_party_commands.c).
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
    HeapSlot_Release(HEAP_SLOT_ENEMY_UNITS);
  }
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to ACTOR_ATTENDANT, who arranges a battle. With no partner, offers
/// a monster battle. With one, calls off a monster battle in waiting, then asks
/// the player to apply or to accept the partner's request; a yes is published
/// as LOBBY_SLOT_ASKED. Agreeing sets FLAG_BATTLE_ARENA_PLAYER_AGREED, and for
/// a linked battle FLAG_LINK_BATTLE and FLAG_LINK_BATTLE_ARRANGED. The original
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
  Event_Start();
  Actor_SetAngleToward(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
  // Look for the partner's signature twice, a frame apart.
  if (BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0) {
    wait(1);
  }
  if (BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0) {
    BattleArena_WriteSlotValue(LOBBY_SLOT_ALONE);
    BattleArena_InitializeSerial();
    if (!Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Event_SetMessageId(messageBase + 5);
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      answer = Event_AskYesNoWithActorAnimation(0, 0);
      // Yes starts a monster series: FLAG_AREA_TEMP_E8's byte and
      // LINK_TALLY_MONSTER_STREAK start at 0.
      if (answer == YES) {
        FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, 0);
        GameFlag_Set(FLAG_MONSTER_BATTLE);
        GameFlag_Clear(FLAG_LINK_BATTLE_ARRANGED);
        GameFlag_Clear(FLAG_LINK_BATTLE);
        GameFlag_Set(FLAG_BATTLE_ARENA_PLAYER_AGREED);
        messageId = messageBase + 7;
        gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = answer;
        goto message;
      }
      GameFlag_Clear(FLAG_MONSTER_BATTLE);
      GameFlag_Set(FLAG_LINK_BATTLE);
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      messageId = messageBase + 6;
      goto message;
    } else {
      messageId = messageBase + 3;
      goto message;
    }
  } else {
    // A partner arrived while a monster battle waited: call it off.
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      Event_SetMessageId(MsgLobbyOpponentArrived);
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      GameFlag_Clear(FLAG_BATTLE_ARENA_PLAYER_AGREED);
      GameFlag_Clear(FLAG_MONSTER_BATTLE);
    }
    if (GameFlag_Get(FLAG_BATTLE_ARENA_PLAYER_AGREED)) {
      messageId = messageBase + 3;
      /* The first branch's answers jump into this block's message tail, and an
       * accepted challenge back into its opening: the listing keeps one copy of
       * each. Written as if/else arms with their own message calls,
       * cross-jumping folds the copies into the last arm instead (differ 51 to
       * 55). */
    message:
      Event_SetMessageId(messageId);
    open_message:
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
    } else if (!GameFlag_Get(FLAG_BATTLE_ARENA_PARTNER_ASKED) &&
               !Event_GetFlag(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED)) {
      Event_SetMessageId(messageBase);
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      GameFlag_Set(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED);
    } else {
      GameFlag_Set(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED);
      if (GameFlag_Get(FLAG_BATTLE_ARENA_PARTNER_ASKED)) {
        Event_SetMessageId(messageBase + 2);
      } else {
        Event_SetMessageId(messageBase + 1);
      }
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      if (Event_AskYesNoWithActorAnimation(0, 0) == YES) {
        // Yes, and the partner is still there; else the link is lost.
        if (BattleArena_PeerSlotMatches(LOBBY_SLOT_SIGNATURE)) {
          GameFlag_Set(FLAG_LINK_BATTLE);
          GameFlag_Set(FLAG_LINK_BATTLE_ARRANGED);
          if (GameFlag_Get(FLAG_BATTLE_ARENA_PARTNER_ASKED)) {
            Event_SetMessageId(messageBase + 3);
          } else {
            Event_SetMessageId(messageBase + 4);
          }
          BattleArena_WriteSlotValue(LOBBY_SLOT_ASKED);
          GameFlag_Set(FLAG_BATTLE_ARENA_PLAYER_AGREED);
          goto open_message;
        } else {
          GameFlag_Set(FLAG_BATTLE_ARENA_LINK_LOST_UNREPORTED);
        }
      } else {
        Event_SetMessageId(messageBase);
        FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      }
    }
  }
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to ACTOR_LINKED_WINS_TELLER, who reports the linked battles won,
/// the total when the leader faces north and those won in a row otherwise;
/// before the first, the line says it will tell them later. Like the original
/// handler, it never ends its event (no Event_End) and returns nothing.
static s32 BattleArena_ReportWinTallies(s32 actorId) {
  u32 facing = Actor_GetById(ACTOR_PARTY_LEADER)->facing;
  u16 *tally;
  s32 messageBase;

  Event_Start();
  Actor_SetAngleToward(actorId, gMainRoutineWork.leaderActorId, 0);
  // FACING_IS_NORTH's test, written out: strictly between northwest and
  // northeast.
  if ((facing - 0xa001) <= 0x3ffe) {
    messageBase = MsgLobbyLooksLikeFirst;
    tally = &gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_WINS];
    if (*tally != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*tally, MESSAGE_ARGUMENT_NUMBER);
      Event_SetMessageId(messageBase + 1);
      Event_ShowMessageAndWait(actorId, 0);
    } else {
      Event_SetMessageId(MsgLobbyTellTotalWins);
      Event_ShowMessageAndWait(actorId, 0);
    }
  } else {
    messageBase = MsgLobbyLooksLikeFirstTime;
    tally = &gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK];
    if (*tally != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(*tally, MESSAGE_ARGUMENT_NUMBER);
      Event_SetMessageId(messageBase + 1);
      Event_ShowMessageAndWait(actorId, 0);
    } else {
      Event_SetMessageId(MsgLobbyTellConsecutiveWins);
      Event_ShowMessageAndWait(actorId, 0);
    }
  }
#ifdef UBFIX
  return 0;
#endif
}
/// Talking to ACTOR_DEBUG_HELPER: Left and Right choose a parameter set number
/// from 0 up, A loads it (DebugParameterSet_Load) and B cancels.
/// ACTOR_LINKED_WINS_TELLER replies: one line for a cancel, one for a
/// parameter set past the table's end (the loader's nonzero result), one for
/// a loaded party.
static s32 BattleArena_ChooseDebugParameterSet(s32 actorId) {
  struct Window *window;
  s32 setIndex;
  s32 displayedSet;
  s32 loadResult;

  Event_Start();
  Actor_SetAngleToward(actorId, gMainRoutineWork.leaderActorId, 0);
  Event_SetMessageId(MsgLobbyChooseParameters);
  Event_ShowMessageAndWait(actorId, 0);
  window = Window_Create(0, 0, 6, 4, WINDOW_FLAG_INSTANT);
  setIndex = 0;
  displayedSet = -1;
  for (;;) {
    if (setIndex != displayedSet) {
      WindowSprite_PrepareForRedraw(window);
      Text_DrawNumber(setIndex, 3, window, 0, 0);
      displayedSet = setIndex;
    }
    if (gKeysRepeat & KEY_LEFT) {
      setIndex--;
    }
    if (gKeysRepeat & KEY_RIGHT) {
      setIndex++;
    }
    if (setIndex < 0) {
      setIndex = 0;
    }
    if (gKeysPressed & KEY_A) {
      break;
    }
    if (gKeysPressed & KEY_B) {
      setIndex = -1;
      break;
    }
    wait(1);
  }
  Window_Close(window, 1);
  if (setIndex >= 0) {
    loadResult = DebugParameterSet_Load(setIndex);
  }
  if (setIndex < 0) {
    Event_SetMessageId(MsgLobbyTease);
    Event_ShowMessageAndWait(ACTOR_LINKED_WINS_TELLER, 0);
  } else if (loadResult != 0) {
    Event_SetMessageId(MsgLobbyNoParameterAsk);
    Event_ShowMessageAndWait(ACTOR_LINKED_WINS_TELLER, 0);
  } else {
    Event_SetMessageId(MsgLobbySettingsMadeRest);
    Event_ShowMessageAndWait(ACTOR_LINKED_WINS_TELLER, 0);
  }
  wait(10);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to ACTOR_HEALER, who serves as a Sanctum's healer
/// (Sanctum_Run).
static s32 BattleArena_OpenSanctum(s32 actorId) {
  Event_Start();
  Sanctum_Run(actorId);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Returns 1 when character memberId is among the first three party members
/// while a linked battle is arranged (FLAG_LINK_BATTLE_ARRANGED), or its first
/// four otherwise; 0 when not.
static s32 BattleArena_PartyContains(s32 memberId) {
  s32 count;
  s32 memberLimit;
  s32 i;

  count = Party_CountMembers();
  memberLimit = 3;
  if (GameFlag_Get(FLAG_LINK_BATTLE_ARRANGED) == 0) {
    memberLimit = 4;
  }
  if (count > memberLimit) {
    count = memberLimit;
  }
  for (i = 0; i < count; i++) {
    if (gMainRoutineWork.party[i] == 0xff) {
      return 0;
    }
    if (gMainRoutineWork.party[i] == memberId) {
      return 1;
    }
  }
  return 0;
}

/// UNUSED: the original empty Battle Arena callback.
UNUSED static void State_NoOp(void) {}

/// Talking to Garet, Ivan, Mia or Jenna, who speaks of the last linked battle
/// (area flags FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED and
/// FLAG_BATTLE_ARENA_LINK_BATTLE_WON) and whether they fought in it, or else of
/// whether Isaac and they are in the battle party. Each message family holds
/// one line per character, at actor id - 1; Felix's holds "?".
static s32 BattleArena_TalkByProgress(s32 actorId) {
  s32 messageBase = MsgLobbyCantWaitLets;
  s32 isaacPresent = BattleArena_PartyContains(0);
  s32 speakerPresent = BattleArena_PartyContains(actorId);

  Event_Start();
  Actor_SetAngleToward(actorId, gMainRoutineWork.leaderActorId, 0);
  if (GameFlag_Get(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED)) {

    // Isaac's flag is tested and the result dropped; messageBase first holds
    // whether the speaker fought (FLAG_BATTLE_ARENA_IN_PARTY_FIRST + actor id).
    Event_GetFlag(FLAG_BATTLE_ARENA_IN_PARTY_FIRST);
    messageBase = GameFlag_Get(actorId + FLAG_BATTLE_ARENA_IN_PARTY_FIRST);
    if (GameFlag_Get(FLAG_BATTLE_ARENA_LINK_BATTLE_WON)) {
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
  Event_SetMessageId(messageBase + actorId - 1);
  Event_ShowMessageAndWait(actorId, 0);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Step trigger 1, the way out: stops the link, loads the normal reverb code
/// and goes back to the File Screen (its entrance FILE_SCREEN_FROM_SANCTUM).
static s32 BattleArena_LeaveArena(void) {
  Serial_RemoveIrqHandlers();
  Sound_LoadReverbCode(SOUND_REVERB_NORMAL);
  Event_SetDestination((s32)&SceneId_FileScreen, FILE_SCREEN_FROM_SANCTUM);
#ifdef UBFIX
  return 0;
#endif
}

// The handlers below are s32 and return no value: r0 is whatever
// Event_End left.

/// Talking to one of three visitors: ACTOR_TRAVELING_WARRIOR,
/// ACTOR_FINALS_VISITOR, who explains the linked finals, and
/// ACTOR_OLD_MANS_ARENA_VISITOR, who wonders whether this is the old man's
/// Battle Arena. Each says their first line unless a linked battle has ended
/// since the leader last walked south of z 224 (area flag
/// FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED), then their second after a win or their
/// third after a loss (FLAG_BATTLE_ARENA_LINK_BATTLE_WON).
static s32 BattleArena_TalkToVisitor(s32 actorId) {
  s32 message;
  s32 step = 0;

  Event_Start();
  switch (actorId) {
  case ACTOR_TRAVELING_WARRIOR:
    message = MsgLobbyTravelingWarrior;
    break;
  case ACTOR_FINALS_VISITOR:
    message = MsgLobbyThreeAlliesFight;
    break;
  case ACTOR_OLD_MANS_ARENA_VISITOR:
  default:
    message = MsgLobbyBattleArenaOld;
    break;
  }
  Actor_SetAngleToward(actorId, gMainRoutineWork.leaderActorId, 0);
  if (GameFlag_Get(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED))
    step = 2 - (GameFlag_Get(FLAG_BATTLE_ARENA_LINK_BATTLE_WON) != 0);
  Event_SetMessageId(message + step);
  FieldMessage_ShowAtActor(actorId, 0);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to ACTOR_PARTY_ORDER_VISITOR, who alternates, by
/// FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT, between the linked finals' limit of
/// three allies (worded for the party's size) and how to change the party's
/// order.
static s32 BattleArena_ExplainPartyOrder(s32 actorId) {
  s32 message;

  Event_Start();
  Actor_SetAngleToward(actorId, gMainRoutineWork.leaderActorId, 0);
  if (!Event_GetFlag(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT)) {
    if (Party_CountMembers() <= 3)
      message = MsgLobbyThreeAlliesFightLinkedFinals;
    else
      message = MsgLobbyThreeAlliesFightLinked;
    GameFlag_Set(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT);
  } else {
    message = MsgLobbyChangeOrderParty;
    GameFlag_Clear(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT);
  }
  Event_SetMessageId(message);
  FieldMessage_ShowAtActor(actorId, 0);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

/// Talking to ACTOR_MONSTER_WINS_TELLER, who reports the most monster battles
/// won in a row (tally 2), or before any, that the speaker will count them.
static s32 BattleArena_ReportMonsterRecord(s32 actorId) {
  struct MainRoutineWork *state;

  Event_Start();
  state = &gMainRoutineWork;
  Actor_SetAngleToward(actorId, state->leaderActorId, 0);
  if (state->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] != 0) {
    MessageArgument_Push(state->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK],
                         MESSAGE_ARGUMENT_NUMBER);
    Event_SetMessageId(MsgLobbyWonNumberConsecutive);
  } else {
    Event_SetMessageId(MsgLobbyCantWaitSee);
  }
  FieldMessage_ShowAtActor(actorId, 0);
  Event_End();
#ifdef UBFIX
  return 0;
#endif
}

u8 *BattleArena_GetTriggers(void) { return (u8 *)gBattleArenaTriggers; }

/// Saves the battle results between saving and saved messages, through
/// Save_ProcessSelectedFile, which writes the first 16 bytes of the save
/// stamp (gBattleArenaRecord) into the current file. BattleArena_RunRoundResult
/// calls it after each linked battle won or lost.
static s32 BattleArena_SaveBattleResults(void) {
  // A shared inline message-pair helper grows each save routine from 0x6c to
  // 0x70 in the native build, so retain the two copies.
  struct Window *window;

  Sound_Play(MUS_SAVED);
  window = Message_OpenWindow(MsgLobbySavingBattleResults, 5, 4, 1);
  while (Message_AllTyped() == 0)
    wait(1);
  Save_ProcessSelectedFile();
  Window_Close(window, 1);
  wait(1);
  window = Message_OpenWindow(MsgLobbyBattleResultsSaved, 5, 4, 1);
  while (Message_AllTyped() == 0)
    wait(1);
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

  Sound_Play(MUS_SAVED);
  window = Message_OpenWindow(MsgLobbySavingMonsterBattle, 5, 4, 1);
  while (Message_AllTyped() == 0)
    wait(1);
  Save_ProcessSelectedFile();
  Window_Close(window, 1);
  wait(1);
  window = Message_OpenWindow(MsgLobbyMonsterBattleResults, 5, 4, 1);
  while (Message_AllTyped() == 0)
    wait(1);
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
    MapGrid_CopyCellPictureIndicesRect(27, value % 10, 16 - digit, 8, 1, 1);
    value = value / 10;
  }
  MapGrid_CenterOnFollowedPoint();
#ifdef UBFIX
  return 0;
#endif
}

/// The overlay's initialize entry, run on every arrival: settles the battle
/// that brought the party here and restarts the Battle Arena's polling. A
/// linked battle broken off (FLAG_AREA_TEMP_E8) arrives as
/// BATTLE_ARENA_AFTER_LINK_WIN; any other entrance than the
/// BATTLE_ARENA_AFTER_ ones (BATTLE_ARENA_FROM_FILE_SCREEN, from the File
/// Screen) restarts the link. Updates gMainRoutineWork.linkTallies, saves
/// after linked battles and new monster records, and restores the party's HP,
/// PP and Djinn unless a monster series goes on. Returns 0.
s32 BattleArena_RunRoundResult(void) {
  s32 i;
  struct FieldWork *work;
  void (*func)(void);

  gBattleArenaCallFrames = 0;
  sBattleArenaWaitFrames = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Sound_LoadReverbCode(SOUND_REVERB_NORMAL);
  BattleArena_DrawThreeDigitValue(
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
  MapGrid_CopyCellAttributes(11, 11, 1, 1, 13, 10);
  BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  wait(1);
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
  // Note who is in the battle party, for the party members' lines.
  for (i = 0; i < 8; i++) {
    GameFlag_Clear(FLAG_BATTLE_ARENA_IN_PARTY_FIRST + i);
    if (BattleArena_PartyContains(i)) {
      GameFlag_Set(FLAG_BATTLE_ARENA_IN_PARTY_FIRST + i);
    }
  }
  // A monster battle won: offer the next one.
  if (gMainRoutineWork.entrance == BATTLE_ARENA_AFTER_MONSTER_WIN) {
    s32 wins;
    s32 messageOffset;
    s32 v;

    Event_Start();
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    BattleArena_WriteSlotValue(LOBBY_SLOT_ALONE);
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_WINS]++;
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK]++;
    // The offer has two lines a level (FLAG_MONSTER_BATTLE_WINS, -1 when a
    // series starts), the even line capped at 14. FLAG_AREA_TEMP_E8's byte
    // counts the victories 0 to 2; the third raises the level and takes the
    // next, odd line (at most 15), which promises a stronger monster.
    wins = (s8)FieldGameFlag_GetByte(FLAG_MONSTER_BATTLE_WINS);
    messageOffset = wins * 2 + 2;
    if (messageOffset > 14) {
      messageOffset = 14;
    }
    v = FieldGameFlag_GetByte(FLAG_AREA_TEMP_E8);
    if (v == 2) {
      FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, 0);
      wins++;
      messageOffset++;
    } else {
      FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, v + 1);
    }
    Actor_SetAngleToward(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
    Event_SetMessageId(MsgLobbyNotBadNextMonster + messageOffset);
    FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(0, 0) == YES) {
      if (wins > 90) {
        wins = 90;
      }
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, wins);
    } else {
      // No: the series ends, and a new record (tally 2) is saved.
      GameFlag_Clear(FLAG_MONSTER_BATTLE);
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
      MessageArgument_Push(
          gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
          MESSAGE_ARGUMENT_NUMBER);
      if (gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
          gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
        gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
            gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
        Event_SetMessageId(MsgLobbyWonNumberBattleBroke);
        FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
        BattleArena_SaveMonsterBattleResults();
      } else {
        Event_SetMessageId(MsgLobbyWonNumberBattle);
        FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      }
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    }
    Event_End();
    // A monster battle lost: the series ends, and a new
    // record is saved.
  } else if (gMainRoutineWork.entrance == BATTLE_ARENA_AFTER_MONSTER_LOSS) {
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_LOSSES]++;
    Event_Start();
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    BattleArena_WriteSlotValue(LOBBY_SLOT_ALONE);
    Actor_SetAngleToward(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
    MessageArgument_Push(
        gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
        MESSAGE_ARGUMENT_NUMBER);
    if (gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
        gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
      gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
          gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
      Event_SetMessageId(MsgLobbyWonNumberBattleBroke);
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
      BattleArena_SaveMonsterBattleResults();
    } else {
      Event_SetMessageId(MsgLobbyWonNumberBattleTold);
      FieldMessage_ShowAtActor(ACTOR_ATTENDANT, 0);
    }
    gMainRoutineWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = 0;
    GameFlag_Clear(FLAG_MONSTER_BATTLE);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    Event_End();
    // A linked battle won: count the win and the run; a
    // longer run is the board's new record. Saved.
  } else if (gMainRoutineWork.entrance == BATTLE_ARENA_AFTER_LINK_WIN) {
    s32 v;

    Event_Start();
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
    // The battle broke off instead (Battle_Run set
    // FLAG_AREA_TEMP_E8): report the link lost and restart it.
    if (Event_GetFlag(FLAG_AREA_TEMP_E8)) {
      work = gFieldWork;
      GameFlag_Clear(FLAG_AREA_TEMP_E8);
      work->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
      GameFlag_Clear(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
      wait(20);
      BattleArena_InitializeSerial();
      BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
    } else {
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_WINS]++;
      v = gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK] + 1;
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
      if (gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] < (u16)v)
        gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] = v;
      BattleArena_DrawThreeDigitValue(
          gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
      BattleArena_SaveBattleResults();
      GameFlag_Set(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
      GameFlag_Set(FLAG_BATTLE_ARENA_LINK_BATTLE_WON);
    }
    Event_End();
    // A linked battle lost: count the loss and end the
    // run. Saved.
  } else if (gMainRoutineWork.entrance == BATTLE_ARENA_AFTER_LINK_LOSS) {
    s32 v;

    Event_Start();
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
    if ((v = GameFlag_Get(FLAG_MONSTER_BATTLE)) == 0) {
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_LOSSES]++;
      gMainRoutineWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
      BattleArena_SaveBattleResults();
    }
    GameFlag_Set(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
    GameFlag_Clear(FLAG_BATTLE_ARENA_LINK_BATTLE_WON);
    Event_End();
    // Arriving from the File Screen: start the link, clear
    // FLAG_LINK_BATTLE_ARRANGED and reset the monster level. Idle sleep is
    // turned off here, with a note when it was on.
  } else {
    Serial_Initialize();
    GameFlag_Clear(FLAG_LINK_BATTLE_ARRANGED);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    if (gMainRoutineWork.idleSleepEnabled != 0) {
      Event_Start();
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Actor_SetAngleToward(ACTOR_ATTENDANT, gMainRoutineWork.leaderActorId, 0);
      Event_SetMessageId(MsgLobbyNoteCantUse);
      Event_ShowMessageAndWait(ACTOR_ATTENDANT, 0);
      Event_End();
    }
    gMainRoutineWork.idleSleepEnabled = 0;
    gIdleSleepEnabled = 0;
    BattleArena_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    BattleArena_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  // Resume polling and restore the party unless continuing a monster run.
  func = (TaskFunc)BattleArena_PollPeerReady;
  Task_AddFrame(func, TASK_PASS_FRAME);
  Task_SetFlags(func, 1);
  if (gMainRoutineWork.entrance != BATTLE_ARENA_AFTER_MONSTER_WIN ||
      !GameFlag_Get(FLAG_MONSTER_BATTLE)) {
    Party_RestoreHpPp(1);
    DjinnRecovery_CountDownTwenty();
  }
  return 0;
}

// clang-format off
/// The payload word each slot of gBattleArena_SlotValues goes in: 0 the
/// signature, 1 the state. The last two entries have no slot.
u8 gBattleArena_SlotColumns[] = {
    0, 1, 1, 1, 1, 0, 0, 0,
};

/// Where the party appears: BATTLE_ARENA_FROM_FILE_SCREEN from the File Screen,
/// the BATTLE_ARENA_AFTER_ ones after a battle (BattleArena_EnterBattle).
struct SceneEntrance gBattleArenaEntrances[] = {
    { BATTLE_ARENA_AFTER_LINK_LOSS, CONDITION_ALWAYS, 128, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_AFTER_LINK_WIN, CONDITION_ALWAYS, 352, 0, 148, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_AFTER_MONSTER_WIN, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_AFTER_MONSTER_LOSS, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_FROM_FILE_SCREEN, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { 0, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gBattleArenaExits[] = {
    SCENE_EXITS_END,
};

struct ActorPlacement gBattleArenaActorPlacements[] = {
    { 0x6a, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
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
    { SCENE_TABLE_END },
};

struct ActorPlacement gBattleArenaBattleActorPlacements[] = {
    { 0x6a, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        PIXELS(152), 0, PIXELS(204), FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE },
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
    { SCENE_TABLE_END },
};
// clang-format on

struct Trigger gBattleArenaTriggers[] = {
    {TRIGGER_TALK, GARET, CONDITION_ALWAYS, EVENT(BattleArena_TalkByProgress)},
    {TRIGGER_TALK, IVAN, CONDITION_ALWAYS, EVENT(BattleArena_TalkByProgress)},
    {TRIGGER_TALK, MIA, CONDITION_ALWAYS, EVENT(BattleArena_TalkByProgress)},
    {TRIGGER_TALK, JENNA, CONDITION_ALWAYS, EVENT(BattleArena_TalkByProgress)},
    {TRIGGER_TALK, ACTOR_ATTENDANT, CONDITION_ALWAYS,
     EVENT(BattleArena_ChooseBattleMode)},
    {TRIGGER_TALK, ACTOR_LINKED_WINS_TELLER, CONDITION_ALWAYS,
     EVENT(BattleArena_ReportWinTallies)},
    {TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
     EVENT(BattleArena_OpenSanctum)},
    {TRIGGER_TALK, ACTOR_GREETER, CONDITION_ALWAYS,
     MSGNO(MsgFieldWelcomeToBattleArenaWherePeople)},
    {TRIGGER_TALK, ACTOR_TRAVELING_WARRIOR, CONDITION_ALWAYS,
     EVENT(BattleArena_TalkToVisitor)},
    {TRIGGER_TALK, ACTOR_FINALS_VISITOR, CONDITION_ALWAYS,
     EVENT(BattleArena_TalkToVisitor)},
    {TRIGGER_TALK, ACTOR_OLD_MANS_ARENA_VISITOR, CONDITION_ALWAYS,
     EVENT(BattleArena_TalkToVisitor)},
    {TRIGGER_TALK, ACTOR_DEBUG_HELPER, CONDITION_ALWAYS,
     EVENT(BattleArena_ChooseDebugParameterSet)},
    {TRIGGER_TALK, ACTOR_PARTY_ORDER_VISITOR, CONDITION_ALWAYS,
     EVENT(BattleArena_ExplainPartyOrder)},
    {TRIGGER_TALK, ACTOR_COMMAND_TIME_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldInTheLinkedFinalsYouCant)},
    {TRIGGER_TALK, ACTOR_SUMMON_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldToUseSummonQuicklyDjinnMust)},
    {TRIGGER_TALK, ACTOR_SET_DJINN_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldDoYouPutDjinnOnStandby)},
    {TRIGGER_TALK, ACTOR_STANDBY_DJINN_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldRemoveDjinnYouRarelyUseSo)},
    {TRIGGER_TALK, ACTOR_MONSTER_RULES_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldMonsterBattleRulesSayYouAll)},
    {TRIGGER_TALK, ACTOR_MONSTER_WINS_TELLER, CONDITION_ALWAYS,
     EVENT(BattleArena_ReportMonsterRecord)},
    {TRIGGER_TALK, ACTOR_EDGY_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldAllTheWarriorsAreImpressiveSo)},
    {TRIGGER_TALK, ACTOR_WARY_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldICouldEnterButIdJust)},
    {TRIGGER_STEP, 10, CONDITION_ALWAYS, EVENT(BattleArena_EnterBattle)},
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(BattleArena_LeaveArena)},
    {TRIGGER_RAISED, 1, CONDITION_ALWAYS,
     EVENT(BattleArena_PromptToParticipate)},
    {TRIGGER_RAISED, 2, CONDITION_ALWAYS,
     EVENT(BattleArena_ReportDisconnection)},
    {TRIGGERS_END},
};
