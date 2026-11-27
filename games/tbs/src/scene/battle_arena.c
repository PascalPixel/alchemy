// The Battle Arena, the hall Isaac walks into when Battle is chosen on the File Screen with a save
// that has a Djinni. He comes in from the south, up a short passage. Talking to the attendant
// behind the west counter arranges a battle: with a monster when no second Game Boy Advance
// answers, or over the link cable with a friend. Stepping onto the diamond mark in the middle of
// the hall then sends the party through the iron gate in the north wall into the arena. Afterwards
// the party is back here: the muscleman behind the east counter keeps the linked battles' record
// and the old man beside the gate the monster battles', and the board over the gate shows the
// longest run of linked wins. A healer in the room up the east stairs offers a Sanctum's services,
// Garet, Ivan, Mia and Jenna wait in that room, and visitors in the hall and the room up the west
// stairs talk about the rules.
//
// Most of the code is the link: the two consoles publish where each player stands in the arranging,
// and trade their parties before the battle.

#include "flags.h"
#include "battle/battle.h"
#include "battle/battle_side.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "graphics/graphics.h"
#include "sound/sound_driver_symbols.h"
#include "text/msg_ids.h"

// The metatiles of the hall that start an event when stepped on.
typedef enum BATTLE_ARENA_METATILE_BEHAVIOR
{
	// The passage out to the south, metatile (13, 19).
	METATILE_BEHAVIOR_BATTLE_ARENA_WAY_OUT = 1,
	// The diamond mark in the middle of the hall, metatile (13, 12).
	METATILE_BEHAVIOR_BATTLE_ARENA_CIRCLE  = 10,
} BATTLE_ARENA_METATILE_BEHAVIOR;

// The map's tileset animation of the iron gate in the north wall, run for eight ticks as the party
// walks to it from the diamond mark.
enum
{
	BATTLE_ARENA_GATE_ANIMATION = 5
};

Sint32                scInit(void);
extern Uint8          BattleArenaSlotColumns[];
extern SCENE_ENTRANCE BattleArenaEntrances[];
extern Uint32         BattleArenaExits[];
extern ACTOR_PLACE    BattleArenaActorPlacements[];
extern ACTOR_PLACE    BattleArenaBattleActorPlacements[];
extern CUE            BattleArenaCues[];

// The attendant has asked the player to take part. Every BATTLE_ARENA_ASK_AGAIN_CALLS-th call of
// scBattleArenaAskToParticipate clears it so she asks again.
#define FLAG_BATTLE_ARENA_ATTENDANT_ASKED     FLAG_SCENE_TEMP_00
// The opponent has asked for a battle: its signature matches, and it stands asked or in the circle.
#define FLAG_BATTLE_ARENA_OPPONENT_ASKED      FLAG_SCENE_TEMP_01
// The player has applied for a battle ("Would you like to apply?").
#define FLAG_BATTLE_ARENA_PLAYER_APPLIED      FLAG_SCENE_TEMP_02
// The party is on its way into the battle.
#define FLAG_BATTLE_ARENA_ENTERING_BATTLE     FLAG_SCENE_TEMP_03
// ACTOR_PARTY_ORDER_VISITOR has given the linked finals' limit of three allies, so the next talk
// explains how to change the party's order.
#define FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT    FLAG_SCENE_TEMP_04
// The link was lost and the attendant has yet to say so.
#define FLAG_BATTLE_ARENA_LINK_DISCONNECTED   FLAG_SCENE_TEMP_05
// The first of eight flags, one per character from Isaac, each set while that character is in the
// battle party (scInit).
#define FLAG_BATTLE_ARENA_IN_PARTY_FIRST      FLAG_SCENE_TEMP_F0
// The attendant has explained the Battle Arena once. Talking to her again offers to apply.
#define FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED FLAG_AREA_TEMP_00
// This console is the link's child (multiplayer id 1), which receives the parties first.
#define FLAG_BATTLE_ARENA_LINK_CHILD          FLAG_AREA_TEMP_02
// The link is up. Both consoles were heard (BATTLE_ARENA_LINK_READY_MASK).
#define FLAG_BATTLE_ARENA_LINK_UP             FLAG_AREA_TEMP_03
// A linked battle has ended since the leader last walked south of z 224, so the party and the
// visitors speak of it.
#define FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED   FLAG_AREA_TEMP_04
// The last linked battle was won.
#define FLAG_BATTLE_ARENA_LINK_BATTLE_WON     FLAG_AREA_TEMP_05

// The most monster battles won in a row that count: each adds a level to the next monster's over
// the party's average (battle_formation.c).
enum
{
	LEVEL_BONUS_MAX = 90
};

// Link transfers use three unit records followed by one recovery-table record. Both sizes are whole
// 20-byte blocks, since the serial runtime sends 20 bytes a block and ends a send only when its
// count reaches exactly 0. The bytes sent for each party member. They are a Unit (BATTLE_UNIT_SIZE,
// 332 bytes) and the eight bytes after it, 17 blocks of 20.
#define BATTLE_ARENA_UNIT_RECORD_BYTES     (17 * 20)
// The bytes sent for the Djinn recovery table after the party members. They are the table
// (DJINN_RECOVERY_TABLE, 268 bytes) and the 52 bytes after it, 16 blocks of 20, two more than the
// table needs.
#define BATTLE_ARENA_RECOVERY_RECORD_BYTES (16 * 20)
// The party members a linked battle takes. A smaller party sends absent records.
#define BATTLE_ARENA_UNIT_RECORD_COUNT     3
// LinkStatusWork bits 0 and 1. Both consoles were heard this tick, as the battle code's
// LINK_BOTH_CONNECTED.
#define BATTLE_ARENA_LINK_READY_MASK       3
// A record fails on the 25th tick it waits without a link or past the timeout.
#define BATTLE_ARENA_TRANSFER_RETRY_LIMIT  24
// Ticks, about ten seconds, for the three party member records, and again for the recovery table.
#define BATTLE_ARENA_SEND_TIMEOUT_TICKS    600
// Ticks, about fifteen seconds, for all four records received.
#define BATTLE_ARENA_RECEIVE_TIMEOUT_TICKS 900
// Ticks without a link (scBattleArenaWatchOpponent) or without the opponent's request
// (scBattleArenaEnterBattle) before the Battle Arena gives the opponent up.
#define BATTLE_ARENA_PEER_WAIT_TICKS       25
// The calls of scBattleArenaAskToParticipate after which the attendant asks again.
#define BATTLE_ARENA_ASK_AGAIN_CALLS       300

// The wall board's digits, which show the most linked battles won in a row, and the most they show.
#define WALL_BOARD_DIGITS                  3
#define WALL_BOARD_MAX                     999
// The attendant's offers of another monster battle follow MsgLobbyNotBadNextMonster, two to each
// word of praise: a stronger monster at an odd offset, the next one at the even offset after it.
// The highest praise offers the next monster at this offset ("Unbelievable!!! Would you like to
// fight the next monster?") and a stronger one at the offset after.
#define MONSTER_OFFER_LAST_NEXT            14

// The Battle Arena's flags, besides its own above, FLAG_MONSTER_BATTLE, FLAG_LINK_BATTLE and the
// two byte flags flags.h names:
// - FLAG_LINK_BATTLE_ARRANGED: a linked battle has been arranged (with
//       FLAG_LINK_BATTLE): only the first three party members take part
//       (scBattleArenaPartyContains).
// - FLAG_AREA_TEMP_E8: the last linked battle broke off (btRun). In a
//       monster series, the byte flag at FLAG_AREA_TEMP_E8 instead counts the
//       victories, 0 to 2, towards a stronger monster.

// The actors, numbering the rows of BattleArenaActorPlacements and
// BattleArenaBattleActorPlacements, which list the same people in the same order. The rows of
// Garet, Ivan, Mia and Jenna take those characters' own ids.
typedef enum BATTLE_ARENA_ACTOR_ID
{
	// The attendant, who arranges every battle. ACTOR_GREETER calls her the girl at the counter.
	ACTOR_ATTENDANT = ACTOR_FIRST_PLACED,
	// Reports the linked battles won, and answers for ACTOR_DEBUG_PARAMETERS_MAN.
	ACTOR_LINKED_WINS_TELLER,
	// Opens a Sanctum's services (scBattleArenaOpenSanctum). The sprite is SPRITE_MAN_IN_TALL_HAT,
	// the healer of every Sanctum in sanctum.c.
	ACTOR_HEALER,
	ACTOR_GREETER,
	ACTOR_TRAVELING_WARRIOR,
	ACTOR_FINALS_VISITOR,
	ACTOR_OLD_MANS_ARENA_VISITOR,
	// Loads a debug parameter set (scBattleArenaChooseDebugParamSet).
	ACTOR_DEBUG_PARAMETERS_MAN,
	// Explains the linked finals' limit of three allies, then the party order.
	ACTOR_PARTY_ORDER_VISITOR,
	ACTOR_COMMAND_TIME_VISITOR,
	ACTOR_SUMMON_VISITOR,
	ACTOR_SET_DJINN_VISITOR,
	ACTOR_STANDBY_DJINN_VISITOR,
	ACTOR_MONSTER_RULES_VISITOR,
	// Reports the most Monster Battles won in a row.
	ACTOR_MONSTER_WINS_TELLER,
	ACTOR_EDGY_VISITOR,
	ACTOR_WARY_VISITOR
} BATTLE_ARENA_ACTOR_ID;

// The words of BattleArenaSlotValues, by slot. They are the signature, then the state a console
// publishes.
typedef enum BATTLE_ARENA_SLOT
{
	BATTLE_ARENA_SLOT_SIGNATURE = 0,
	BATTLE_ARENA_SLOT_ASKED     = 1, // has asked for a battle
	BATTLE_ARENA_SLOT_IN_CIRCLE = 2, // stands in the circle
	BATTLE_ARENA_SLOT_UNUSED    = 3,
	BATTLE_ARENA_SLOT_IDLE      = 4,
	// Replaces the signature when no opponent answers the attendant and during a monster series.
	BATTLE_ARENA_SLOT_ALONE     = 5
} BATTLE_ARENA_SLOT;

// The Battle Arena's raised cues (FIELD_STATE.raisedCue).
typedef enum BATTLE_ARENA_RAISED_CUE
{
	BATTLE_ARENA_CUE_NONE               = 0,
	BATTLE_ARENA_CUE_ASK_TO_PARTICIPATE = 1, // scBattleArenaAskToParticipate
	BATTLE_ARENA_CUE_DISCONNECTED       = 2  // scBattleArenaReportLinkDisconnected
} BATTLE_ARENA_RAISED_CUE;

// MainRoutineStateWork.linkTallies, as the Battle Arena keeps them.
typedef enum LINK_TALLY
{
	LINK_TALLY_MONSTER_WINS        = 0,
	LINK_TALLY_MONSTER_LOSSES      = 1,
	LINK_TALLY_MONSTER_BEST_STREAK = 2, // the most won in a row
	LINK_TALLY_MONSTER_STREAK      = 3, // those won in the current series
	LINK_TALLY_LINKED_WINS         = 4,
	LINK_TALLY_LINKED_LOSSES       = 5,
	// The most linked battles won in a row, on the wall board.
	LINK_TALLY_LINKED_BEST_STREAK  = 6,
	LINK_TALLY_LINKED_STREAK       = 7 // those won in a row now
} LINK_TALLY;

// Four letters as one word, the first in its low byte.
#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

// clang-format off
// The words the Battle Arena publishes in its link payload, by slot.
//
// The opponent compares them with its own (scBattleArenaPeerSlotMatches), by BATTLE_ARENA_SLOT. The signature is in the first column (BattleArenaSlotColumns), the states in the second.
//
// In English and French BATTLE_ARENA_SLOT_ALONE equals the signature and changes nothing. Elsewhere it is the signature's letters reversed.
//
// So while a console publishes it (no opponent answered, or a monster series runs) an opponent's signature test fails, and the attendant does not offer that console as an opponent. Each return to linking writes the signature back.
const Sint32 BattleArenaSlotValues[] = {
#if LANGUAGE == LANGUAGE_GERMAN
	PEER_TAG('C', 'S', 'G', 'G'), PEER_TAG('3', '0', '1', '2'),
	PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
	PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'G', 'S', 'C')
#elif LANGUAGE == LANGUAGE_SPANISH
	PEER_TAG('C', 'S', 'S', 'G'), PEER_TAG('3', '0', '1', '2'),
	PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
	PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('G', 'S', 'S', 'C')
#elif LANGUAGE == LANGUAGE_FRENCH
	PEER_TAG('C', 'S', 'G', 'M'), PEER_TAG('3', '0', '1', '2'),
	PEER_TAG('1', 'A', 'B', 'C'), PEER_TAG('2', 'C', 'D', 'E'),
	PEER_TAG('3', 'E', 'F', 'G'), PEER_TAG('C', 'S', 'G', 'M')
#elif LANGUAGE == LANGUAGE_ITALIAN
	PEER_TAG('S', 'G', 'I', 'C'), PEER_TAG('0', '1', '2', '3'),
	PEER_TAG('A', 'B', 'C', '1'), PEER_TAG('C', 'D', 'E', '2'),
	PEER_TAG('E', 'F', 'G', '3'), PEER_TAG('C', 'I', 'G', 'S')
#else
#if LANGUAGE != LANGUAGE_JAPANESE
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

// A console's link payload as the Battle Arena reads it from SerialPeerPayloads. values[0] is its
// signature word and values[1] its state word (BattleArenaSlotValues, placed by
// BattleArenaSlotColumns).
typedef struct LINK_PEER_PAYLOAD
{
	Sint32 values[6]; // the payload's words; only 0 and 1 are read here
} LINK_PEER_PAYLOAD;

// Ticks scBattleArenaWatchOpponent has gone without a link, 0 to BATTLE_ARENA_PEER_WAIT_TICKS. It
// is 0 while linked.
static Sint32 BattleArenaWaitTicks;

// What a Battle Arena handler's early exit returns.
//
// The handlers return nothing there, leaving whatever the last call returned (UB). UBFIX returns 0,
// as the function ends that fall off do.
#if FIX_FUNCTION_SIGNATURES
#	define BATTLE_ARENA_NO_RESULT 0
#else
#	define BATTLE_ARENA_NO_RESULT
#endif

// The Battle Arena's entrances, which fdModeRun reads through entry/battle_arena.s.
Uint8 *scGetEntrances(void)
{
	return (Uint8 *)BattleArenaEntrances;
}

// The exit regions entry: the Battle Arena has no exit boxes.
Sint32 scGetExitRegions(void)
{
	return 0;
}

// Restarts the link with interrupts locked.
static Sint32 scBattleArenaRestartLink(void)
{
	Uint32 savedIrqState;

	IRQ_LOCK(savedIrqState);
	sySerialRemoveIRQHandlers();
	sySerialInit();
	IRQ_UNLOCK(savedIrqState);
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

Uint8 *scGetExits(void)
{
	return (Uint8 *)BattleArenaExits;
}

// The actor placements entry.
Sint32 scGetActorPlacements(void)
{
	Sint16 entrance = MainRoutineStateWork.entrance;

	if (entrance == BATTLE_ARENA_AFTER_LINK_LOSS || entrance == BATTLE_ARENA_AFTER_MONSTER_LOSS)
	{
		return (Sint32)BattleArenaBattleActorPlacements;
	}
	return (Sint32)BattleArenaActorPlacements;
}

// Returns 1 when the opponent's payload holds slot's word in slot's column, 0 when it does not or
// there is no link.
static Bool scBattleArenaPeerSlotMatches(Sint32 slot)
{
	Sint32 serialPlayerId = -1;

	if ((LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) == BATTLE_ARENA_LINK_READY_MASK)
	{
		// 0 for the parent, 1 for the child.
		serialPlayerId = SIO_ID_OF(REG_SIOCNT);
		gmFlagSet(FLAG_BATTLE_ARENA_LINK_UP);
	}
	else
	{
		gmFlagClear(FLAG_BATTLE_ARENA_LINK_UP);
	}
	// At most one pass: a match returns 1, anything else breaks out.
	while (serialPlayerId >= 0 && gmFlagGet(FLAG_BATTLE_ARENA_LINK_UP))
	{
		const Sint32 *expected = &BattleArenaSlotValues[slot];

		if (serialPlayerId != 0)
			gmFlagSet(FLAG_BATTLE_ARENA_LINK_CHILD);
		else
			gmFlagClear(FLAG_BATTLE_ARENA_LINK_CHILD);
		{
			LINK_PEER_PAYLOAD *peer =
				(LINK_PEER_PAYLOAD *)SerialPeerPayloads[gmFlagGet(FLAG_BATTLE_ARENA_LINK_CHILD) ^ 1]
					.payload;

			if (peer->values[BattleArenaSlotColumns[slot]] == *expected)
				return TRUE;
			break;
		}
	}
	return FALSE;
}

// Publishes slot's word in its column of this console's link payload, where the opponent's
// scBattleArenaPeerSlotMatches reads it.
static void scBattleArenaWriteSlotValue(Sint32 slot)
{
	Sint32       *dest                 = (Sint32 *)SerialTransfer.payload;
	const Sint32 *src                  = &BattleArenaSlotValues[slot];
	dest[BattleArenaSlotColumns[slot]] = *src;
}

// The event that watches the opponent each tick in the Battle Arena.
static UNREAD_RESULT scBattleArenaWatchOpponent(void)
{
	FIELD_STATE *state;
	Sint32       result;
	Sint32       i;

	state  = FieldStateWork;
	result = 1;
	// The leader is south of z 224 (16.16 pixels), towards the way out: the party stops talking
	// about the last battle.
	if (fdActorGet(ACTOR_PARTY_LEADER)->z.fixed > toFIXED(224))
	{
		gmFlagClear(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
	}
	// While BATTLE_ARENA_CUE_DISCONNECTED waits, skip the opponent tracking.
	if (state->raisedCue != BATTLE_ARENA_CUE_DISCONNECTED)
	{
		scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE);
		if (!gmFlagGet(FLAG_BATTLE_ARENA_LINK_UP))
		{
			// Without a link for more than BATTLE_ARENA_PEER_WAIT_TICKS ticks, clear every
			// console's received payload and go idle.
			if (++BattleArenaWaitTicks > BATTLE_ARENA_PEER_WAIT_TICKS)
			{
				for (i = 0; i < 4; i++)
				{
					syWordsClear(SerialPeerPayloads[i].payload, 20);
				}
				BattleArenaWaitTicks = 0;
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
			}
		}
		else
		{
			BattleArenaWaitTicks = 0;
		}
		// While linked: has the opponent asked for a battle, or stepped into the circle?
		if (BattleArenaWaitTicks == 0)
		{
			if (scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE)
			    && (scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_ASKED)
			        || scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE)))
			{
				gmFlagSet(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				if (gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED))
				{
					state->raisedCue = BATTLE_ARENA_CUE_ASK_TO_PARTICIPATE;
				}
				result = 1;
			}
			else
			{
				gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				result = 0;
			}
		}
		if (gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED)
		    && gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED)
		    && !gmFlagGet(FLAG_BATTLE_ARENA_ATTENDANT_ASKED))
		{
			state->raisedCue = BATTLE_ARENA_CUE_ASK_TO_PARTICIPATE;
		}
	}
	// BATTLE_ARENA_PEER_WAIT_TICKS ticks without a link while a linked battle is being arranged:
	// report the link lost.
	if ((fdScriptGetFlag(FLAG_BATTLE_ARENA_OPPONENT_ASKED)
	     || gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED))
	    && !gmFlagGet(FLAG_MONSTER_BATTLE)
	    && !scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE)
	    && BattleArenaWaitTicks >= BATTLE_ARENA_PEER_WAIT_TICKS)
	{
		state->raisedCue = BATTLE_ARENA_CUE_DISCONNECTED;
		gmFlagSet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
		gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
		gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	}
	if (gmFlagGet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED))
	{
		state->raisedCue = BATTLE_ARENA_CUE_DISCONNECTED;
	}
	RETURN_UNREAD(result);
}

// Calls of scBattleArenaAskToParticipate towards asking again, at BATTLE_ARENA_ASK_AGAIN_CALLS. It
// is cleared on every arrival.
Uint32        BattleArenaCallTicks;

// Once both linked players have applied for a battle, the attendant behind the west counter calls
// out "If you want to participate, please step into the circle!", and calls it again now and then
// while the player keeps her waiting.
//
// `scBattleArenaWatchOpponent` raises it (BATTLE_ARENA_CUE_ASK_TO_PARTICIPATE) while both have
// applied, and every BATTLE_ARENA_ASK_AGAIN_CALLS calls the flag that keeps her quiet is cleared.
//
// Returns the flag's value when FLAG_BATTLE_ARENA_ENTERING_BATTLE or
// FLAG_BATTLE_ARENA_ATTENDANT_ASKED is set and nothing is shown. The path that shows the line
// returns nothing (UB).
static Sint32 scBattleArenaAskToParticipate(void)
{
	Sint32 set;

	set = gmFlagGet(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
	if (set != 0)
	{
		return set;
	}
	if (++BattleArenaCallTicks == BATTLE_ARENA_ASK_AGAIN_CALLS)
	{
		BattleArenaCallTicks = 0;
		gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	}
	set = fdScriptGetFlag(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	if (set != 0)
	{
		return set;
	}
	fdScriptStart();
	fdScriptSetMessageId(MsgLobbyWantParticipatePlease);
	// "If you want to participate, please step into the circle!"
	fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
	sySynch(5);
	gmFlagSet(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// When the link cable's connection is lost while a linked battle is being arranged, the attendant
// behind the west counter says "The link was disconnected.", and the arranging starts over.
//
// `scBattleArenaWatchOpponent` and `scBattleArenaEnterBattle` raise it
// (BATTLE_ARENA_CUE_DISCONNECTED).
static Sint32 scBattleArenaReportLinkDisconnected(void)
{
	FIELD_STATE *state = FieldStateWork;

	scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	gmFlagClear(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
	fdScriptStart();
	state->raisedCue = BATTLE_ARENA_CUE_NONE;
	fdScriptSetMessageId(MsgLobbyLinkDisconnected);
	// "The link was disconnected."
	fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
	gmFlagClear(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Receives the opponent's three party member records into units FIRST_ENEMY_UNIT to
// FIRST_ENEMY_UNIT + 2 (the battle's enemies, by gmUnitGet) and its Djinn recovery table into the
// enemy side's (gmDjinnGetRecoveryTable(DJINN_RECOVERY_OPPONENT)).
//
// Returns how many records hold a party member, or -1 when a transfer fails, overruns or runs out
// of time.
static Sint32 scBattleArenaReceivePartyRecords(void)
{
	Uint16 namePrefix[24];
	Uint32 size;
	void  *buf;
	Sint32 result;
	Sint32 slot;
	Sint32 timeout;
	Sint32 tries;
	UNIT  *unit;
	Sint32 started;
	Sint32 prefixLength;
	Sint32 i;

	size    = BATTLE_ARENA_UNIT_RECORD_BYTES;
	buf     = syHeapAllocateEWRAM(size);
	result  = 0;
	timeout = BATTLE_ARENA_RECEIVE_TIMEOUT_TICKS;
	slot    = 0;
	// Three records, each started at recordNext and waited for at recordTest, then the recovery
	// table at recovery. Each receive waits a tick at a time; more than size bytes is an overrun
	// and fewer once it ends a short record, and once the timeout has run out, or while the link is
	// not ready, every tick is a try, failing past BATTLE_ARENA_TRANSFER_RETRY_LIMIT.
	goto recordNext;
recordWait:
	if (SerialReceivedSize > size)
	{
		result = -1;
		goto done;
	}
	sySynch(1);
	if (--timeout < 0
	    || (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) != BATTLE_ARENA_LINK_READY_MASK)
	{
		if (++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT)
		{
			result = -1;
			goto done;
		}
	}
recordTest:
	if (sySerialGetActiveTransfers() != 0)
	{
		goto recordWait;
	}
	if (SerialReceivedSize != size)
	{
		result = -1;
		goto done;
	}
	// The sender marks a party member BATTLE_SIDE_PARTY and a padding record BATTLE_SIDE_NONE.
	if (unit->side != BATTLE_SIDE_NONE)
	{
		result++;
	}
	sySynch(2);
	mnFieldMessageDecodeRawUnbounded(MsgEnemyLabel, namePrefix);
	// The prefix's length, at most BATTLE_ARENA_PREFIX_LAST + 1.
	for (i = 0; i <= BATTLE_ARENA_PREFIX_LAST && namePrefix[i] != 0; i++)
		;
	// German, Spanish, French and Italian put a space after the prefix.
#if SPACED_ENEMY_NAME_PREFIX
	namePrefix[i] = ' ';
	prefixLength  = i + 1;
#else
	prefixLength = i;
#endif
	for (i = UNIT_NAME_SIZE - 1; i >= prefixLength; i--)
	{
		unit->name[i] = unit->name[i - prefixLength];
	}
	for (i = 0; i < prefixLength; i++)
	{
		unit->name[i] = namePrefix[i];
	}
	unit->name[UNIT_NAME_SIZE - 1] = 0;
	slot++;
recordNext:
	if (slot >= BATTLE_ARENA_UNIT_RECORD_COUNT)
	{
		goto recovery;
	}
	// The opponent's party members become this battle's first three enemies.
	unit  = gmUnitGet(slot + FIRST_ENEMY_UNIT);
	tries = 0;
	if ((started = sySerialStartReceive(unit)) == -1)
	{
		goto failed;
	}
	goto recordTest;
recoveryWait:
	started = SerialReceivedSize;
	if (started > BATTLE_ARENA_RECOVERY_RECORD_BYTES)
	{
		result = -1;
		goto done;
	}
	sySynch(1);
	if (--timeout < 0
	    || (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) != BATTLE_ARENA_LINK_READY_MASK)
	{
		if (++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT)
		{
			result = -1;
			goto done;
		}
	}
recoveryTest:
	if (sySerialGetActiveTransfers() != 0)
	{
		goto recoveryWait;
	}
	if (SerialReceivedSize != BATTLE_ARENA_RECOVERY_RECORD_BYTES)
	{
		result = -1;
	}
	else
	{
		sySynch(2);
	}
	goto done;
recovery:
	// Then the opponent's Djinn recovery table, as the enemy side's.
	syHeapRelease(buf);
	size  = BATTLE_ARENA_RECOVERY_RECORD_BYTES;
	buf   = syHeapAllocateEWRAM(size);
	tries = 0;
	if ((started = sySerialStartReceive(gmDjinnGetRecoveryTable(DJINN_RECOVERY_OPPONENT))) != -1)
	{
		goto recoveryTest;
	}
failed:
	result = started;
done:
	syHeapRelease(buf);
	return result;
}

// Copies the character ids of the first three party members (MainRoutineStateWork.party), ends them
// with 0xff and returns how many. A null destination only counts them.
static Sint32 scBattleArenaCopyPartyOwnerIds(Uint16 *memberIds)
{
	Sint32 memberCount = gmPartyCountMembers();
	if (memberCount > BATTLE_ARENA_UNIT_RECORD_COUNT)
		memberCount = BATTLE_ARENA_UNIT_RECORD_COUNT;
	if (memberCount > 0)
	{
#if !FIX_POINTER_OUTSIDE_OBJECT
		Sint16 *partyAddr = (Sint16 *)&MainRoutineStateWork;
#endif
		const Uint8 *src;
		Sint32       remaining;
#if FIX_POINTER_OUTSIDE_OBJECT
		// The original short-pointer addition traverses a struct as an array.
		src = MainRoutineStateWork.party;
#else
		partyAddr += offsetof(MAIN_ROUTINE_STATE, party) / sizeof(Sint16);
		src        = (const Uint8 *)partyAddr;
#endif
		remaining = memberCount;
		do
		{
			Uint8 memberId = *src++;
			if (memberIds != 0)
			{
				*memberIds = memberId;
				memberIds++;
			}
			remaining--;
		} while (remaining != 0);
	}
	if (memberIds != 0)
		*memberIds = BATTLE_UNIT_LIST_END;
	return memberCount;
}

// Sends the first three party members' records, marked side 2 (BATTLE_SIDE_PARTY).
//
// Returns 0, or -1 when a send fails or runs out of time. The copies go through an EWRAM pool
// buffer freed before returning.
static Sint32 scBattleArenaSendPartyRecords(void)
{
	Sint32               result;
	Uint16               memberIds[8];
	Uint8                peerSlots[8];
	Sint32               started;
	Uint8               *buf;
	Sint32               memberCount;
	Sint32               timeout;
	Sint32               tries;
	Sint32               i;
	Sint32               j;
	Sint32               k;
	Uint32               size;
	DJINN_RECOVERY_LIST *list;

	size        = BATTLE_ARENA_UNIT_RECORD_BYTES;
	buf         = syHeapAllocateEWRAM(size);
	result      = 0;
	timeout     = BATTLE_ARENA_SEND_TIMEOUT_TICKS;
	memberCount = scBattleArenaCopyPartyOwnerIds(memberIds);
	for (i = 7; i >= 0; i--)
	{
		peerSlots[i] = 0;
	}
	// Each send waits a tick at a time to finish. Once the timeout has run out, or while the link
	// is not ready, every tick is a try, and past BATTLE_ARENA_TRANSFER_RETRY_LIMIT tries the
	// exchange fails. A send that does not start breaks out to the release; the waits inside leave
	// by done.
	do
	{
		for (i = 0; i < memberCount; i++)
		{
			syWordsCopy(buf, gmUnitGet(memberIds[i]), size);
			((UNIT *)buf)->side     = BATTLE_SIDE_PARTY;
			// As a Uint8, i - FIRST_ENEMY_UNIT wraps round to FIRST_ENEMY_UNIT
			// + i, FIRST_ENEMY_UNIT being half of 256: this party member's unit
			// on the opponent's side.
			peerSlots[memberIds[i]] = i - FIRST_ENEMY_UNIT;
			started                 = sySerialStartSend(buf, size);
			tries                   = 0;
			if (started == -1)
			{
				result = started;
				goto done;
			}
			while (sySerialGetActiveTransfers() != 0)
			{
				sySynch(1);
				if (--timeout < 0
				    || (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK)
				           != BATTLE_ARENA_LINK_READY_MASK)
				{
					if (++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT)
					{
						result = -1;
						goto done;
					}
				}
			}
			sySynch(2);
		}
		// Pad to three records with the last copy marked absent (BATTLE_SIDE_NONE).
		for (; i < BATTLE_ARENA_UNIT_RECORD_COUNT; i++)
		{
			((UNIT *)buf)->side = tries = 0;
			if ((started = sySerialStartSend(buf, size)) == -1)
				break;
			while (sySerialGetActiveTransfers() != 0)
			{
				sySynch(1);
				if (--timeout < 0
				    || (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK)
				           != BATTLE_ARENA_LINK_READY_MASK)
				{
					if (++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT)
					{
						result = -1;
						goto done;
					}
				}
			}
			sySynch(2);
		}
		if (i < BATTLE_ARENA_UNIT_RECORD_COUNT)
		{
			result = started;
			break;
		}
		syHeapRelease(buf);

		// Then the recovery table.
		size = BATTLE_ARENA_RECOVERY_RECORD_BYTES;
		buf  = syHeapAllocateEWRAM(size);
		IWRAM_COPY_WORDS(buf, gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY), size);
		{
			Sint32 timeout;
			Sint32 tries;

			list    = &((DJINN_RECOVERY_TABLE *)buf)->list;
			tries   = 0;
			timeout = BATTLE_ARENA_SEND_TIMEOUT_TICKS;
			// Give each Djinni on the list its party member's number on the opponent's side, and
			// drop those of party members not sent (peerSlots 0).
			for (j = 0; j < list->count; j++)
			{
				list->entries[j].unitId = peerSlots[list->entries[j].unitId];
				if (list->entries[j].unitId == 0)
				{
					for (k = j; k < list->count - 1; k++)
					{
						list->entries[k] = list->entries[k + 1];
					}
					list->count--;
					j--;
				}
			}
			if ((started = sySerialStartSend(buf, size)) == -1)
			{
				result = started;
				break;
			}
			while (sySerialGetActiveTransfers() != 0)
			{
				sySynch(1);
				if (--timeout < 0
				    || (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK)
				           != BATTLE_ARENA_LINK_READY_MASK)
				{
					if (++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT)
					{
						result = -1;
						goto done;
					}
				}
			}
			sySynch(1);
			sySynch(2);
		}
	} while (0);
done:
	syHeapRelease(buf);
	return result;
}

// Exchanges the two parties. The parent sends first, and the child (area flag
// FLAG_BATTLE_ARENA_LINK_CHILD) receives first.
static Sint32 scBattleArenaExchangePartyRecords(void)
{
	Sint32 result;
	Sint32 receivedMemberCount;
	Sint32 receiveFirst;

	result = 0;
	// FAKEMATCH(dead): an empty once-only loop after result's zero loads it
	// * ahead of the call.
	do
	{
	} while (0);
	receiveFirst      = gmFlagGet(FLAG_BATTLE_ARENA_LINK_CHILD);
	LinkExchangeState = result;
	if (!receiveFirst)
	{
		sySynch(5);
		result = scBattleArenaSendPartyRecords();
		if (result < 0)
			goto fail;
		sySynch(5);
		receivedMemberCount = result = scBattleArenaReceivePartyRecords();
		if (result < 0)
			goto check;
	}
	else
	{
		receivedMemberCount = result = scBattleArenaReceivePartyRecords();
		if (result < 0)
			goto fail;
		sySynch(10);
		result = scBattleArenaSendPartyRecords();
		if (result < 0)
			goto fail;
	}
	syFieldGameFlagSetByte(FLAG_LINK_PARTY_COUNT, receivedMemberCount);
	result = receivedMemberCount;
check:
	// Any failure resets the link; the count received is never negative here.
	if (receivedMemberCount < 0)
	{
	fail:
		sySerialResetTransferState();
	}
	return result;
}

// Stepping onto the diamond mark in the middle of the hall, metatile (13, 12), once a battle is
// arranged starts it. For a monster battle the attendant says "Good luck!" and Isaac walks north
// through the iron gate, which lifts, into the arena. For a linked battle "Awaiting your
// opponent..." shows until the friend steps onto the mark too, and Isaac walks slowly to the gate
// while the two parties are traded over the cable.
//
// Before a battle is arranged, nothing happens on the mark. A lost link or a failed trade walks him
// back out and lets the attendant say so.
static Sint32 scBattleArenaEnterBattle(void)
{
	FIELD_STATE *state;
	Bool         failed;
	WINDOW      *window;
	Sint32       missedTicks;
	Bool         cancelEntry;
	Uint8       *savedState;
	Uint8       *src;
	Uint8       *dest;
	Uint16      *signature;
	Uint32       i;

	failed      = FALSE;
	window      = 0;
	state       = FieldStateWork;
	missedTicks = 0;
	if (gmFlagGet(FLAG_MONSTER_BATTLE))
	{
		fdScriptStart();
	}
	else
	{
		if (!gmFlagGet(FLAG_BATTLE_ARENA_ATTENDANT_ASKED))
			return BATTLE_ARENA_NO_RESULT;
		if (gmFlagGet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED))
			return BATTLE_ARENA_NO_RESULT;
		fdScriptStart();
		gmFlagSet(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IN_CIRCLE);
		if (!scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE))
			window = mnMessageOpenWindow(MsgLobbyAwaitingOpponent, 5, 4, MESSAGE_WINDOW_ZOOM);
		while (!scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE))
		{
			sySynch(1);
			cancelEntry = FALSE;
			if (!gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED))
				cancelEntry = TRUE;
			if (gmFlagGet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED))
				cancelEntry = TRUE;
			if (!scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE)
			    && !scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_ASKED))
			{
				if (++missedTicks > BATTLE_ARENA_PEER_WAIT_TICKS)
					cancelEntry = TRUE;
			}
			else
			{
				missedTicks = 0;
			}
			if (cancelEntry)
			{
				state->raisedCue = BATTLE_ARENA_CUE_DISCONNECTED;
				gmFlagSet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
				gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
				failed = TRUE;
				gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
				break;
			}
		}
		if (window)
			mnWindowClose(window, 1);
		sySynch(5);
	}
	// Stop watching the opponent while the parties are traded and the battle starts.
	if (!failed)
	{
		// HEAP_SLOT_ENEMY_UNITS is EnemyUnits, the battle's enemy units (ENEMY_UNITS_SIZE,
		// battle/battle_run.c). While it is allocated, the exchange below receives the opponent's
		// party members and Djinn into units FIRST_ENEMY_UNIT to FIRST_ENEMY_UNIT + 3. The copy to
		// ActorSpriteSlots (32 KB into MapGridMetatileBuffer) hands them to btRun, which copies
		// them back.
		savedState = syHeapSlotAllocateEWRAM(HEAP_SLOT_ENEMY_UNITS, 6 * BATTLE_UNIT_SIZE);
		syCloseEvent((EVENT_FUNC)scBattleArenaWatchOpponent);
		fdTilesetAnimationResume(BATTLE_ARENA_GATE_ANIMATION);
		sySynch(8);
		fdTilesetAnimationPause(BATTLE_ARENA_GATE_ANIMATION);
		if (gmFlagGet(FLAG_MONSTER_BATTLE))
		{
			fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
			fdScriptSetMessageId(MsgLobbyGoodLuck);
			// "Good luck!"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			sySynch(45);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 184);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 168);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		}
		else
		{
			// Isaac walks to the gate at a crawl while the parties are traded, and at half pace
			// once they are.
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 200);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.1);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 168);
			// A failed exchange: walk back out and return to the Battle Arena, reporting the link
			// lost (BATTLE_ARENA_CUE_DISCONNECTED).
			if (scBattleArenaExchangePartyRecords() < 0)
			{
				fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
				fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 200);
				fdTilesetAnimationResume(BATTLE_ARENA_GATE_ANIMATION);
				sySynch(8);
				fdTilesetAnimationPause(BATTLE_ARENA_GATE_ANIMATION);
				fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
				syHeapSlotRelease(HEAP_SLOT_ENEMY_UNITS);
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
				sySetTickEvent((EVENT_FUNC)scBattleArenaWatchOpponent, EVENT_PASS_TICK);
				sySetEventFlags((EVENT_FUNC)scBattleArenaWatchOpponent, EVENT_FLAG_KEEP_RUNNING);
				gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
				gmFlagClear(FLAG_BATTLE_ARENA_LINK_UP);
				gmFlagClear(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
				gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
				state->raisedCue = BATTLE_ARENA_CUE_DISCONNECTED;
				fdScriptEnd();
				return BATTLE_ARENA_NO_RESULT;
			}
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		}
		if (gmFlagGet(FLAG_MONSTER_BATTLE))
		{
			fdScriptSetAfterBattleDestination(SCENE_BATTLE_ARENA, BATTLE_ARENA_AFTER_MONSTER_WIN);
			fdScriptSetDefeatDestination(SCENE_BATTLE_ARENA, BATTLE_ARENA_AFTER_MONSTER_LOSS);
		}
		else
		{
			fdScriptSetAfterBattleDestination(SCENE_BATTLE_ARENA, BATTLE_ARENA_AFTER_LINK_WIN);
			fdScriptSetDefeatDestination(SCENE_BATTLE_ARENA, BATTLE_ARENA_AFTER_LINK_LOSS);
		}
		// Publish EXEC only after the battle and return destinations are ready. It fills the
		// payload's first four halfwords, where the link battle's command entry looks for it in the
		// opponent's (screens_battle_party_commands.c).
		SET_BATTLE_TYPE(BATTLE_TYPE_LINK);
		fdRequestBattleFromEncounterTable(1, 1);
		signature    = (Uint16 *)SerialTransfer.payload;
		signature[0] = 'E';
		signature[1] = 'X';
		signature[2] = 'E';
		signature[3] = 'C';
		i            = 0;
		src          = savedState;
		dest         = ActorSpriteSlots;
		for (; i < 6 * BATTLE_UNIT_SIZE; i++)
		{
			*dest++ = *src++;
		}
		syHeapSlotRelease(HEAP_SLOT_ENEMY_UNITS);
	}
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Talking to the attendant, the woman behind the west counter, arranges a battle. With no second
// Game Boy Advance on the cable she says "You do not have an opponent. Would you like to battle a
// monster?", and Yes sends the party to the diamond mark. With a friend linked she explains the
// Battle Arena once, then offers to ask the friend for a battle, or to accept the friend's request.
static Sint32 scBattleArenaChooseBattleMode(void)
{
	// UB: returns no value, so its result is whatever the last call returned.

	Sint32 answer;
	Sint32 msgBase;
	Sint32 msgId;

	// The attendant's lines follow "Please speak to me when you want to fight in a battle."
	// (MsgLobbyPleaseSpeakWhen), as quoted where each is chosen.
	msgBase = MsgLobbyPleaseSpeakWhen;
	fdScriptStart();
	fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
	if (scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE) == 0)
	{
		sySynch(1);
	}
	if (scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE) == 0)
	{
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ALONE);
		scBattleArenaRestartLink();
		if (!fdScriptGetFlag(FLAG_MONSTER_BATTLE))
		{
			fdScriptSetMessageId(msgBase + 5);
			// "You do not have an opponent. Would you like to battle a monster?"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			answer = fdScriptAskYesNoWithActorAnimation(0, 0);
			// Yes starts a monster series: FLAG_AREA_TEMP_E8's byte and LINK_TALLY_MONSTER_STREAK
			// start at 0.
			if (answer == YES)
			{
				syFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, 0);
				gmFlagSet(FLAG_MONSTER_BATTLE);
				gmFlagClear(FLAG_LINK_BATTLE_ARRANGED);
				gmFlagClear(FLAG_LINK_BATTLE);
				gmFlagSet(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
				// "When you finish your preparations, please step into the circle!"
				msgId                                                       = msgBase + 7;
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = answer;
				goto message;
			}
			gmFlagClear(FLAG_MONSTER_BATTLE);
			gmFlagSet(FLAG_LINK_BATTLE);
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
			// "Please wait until an opponent arrives, then."
			msgId = msgBase + 6;
			goto message;
		}
		else
		{
			// "When you finish your preparations, please step into the circle!"
			msgId = msgBase + 3;
			goto message;
		}
	}
	else
	{
		// An opponent arrived while a monster battle waited: call it off.
		if (fdScriptGetFlag(FLAG_MONSTER_BATTLE))
		{
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
			fdScriptSetMessageId(MsgLobbyOpponentArrived);
			// "Your opponent has arrived!"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
			gmFlagClear(FLAG_MONSTER_BATTLE);
		}
		if (gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED))
		{
			// "When you finish your preparations, please step into the circle!"
			msgId = msgBase + 3;
			// The lone player's answers above show their line here too, and an accepted request
			// below comes back to open its own.
		message:
			fdScriptSetMessageId(msgId);
		open_message:
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
		}
		else if (!gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED)
		         && !fdScriptGetFlag(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED))
		{
			fdScriptSetMessageId(msgBase);
			// "Please speak to me when you want to fight in a battle."
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			gmFlagSet(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED);
		}
		else
		{
			gmFlagSet(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED);
			if (gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED))
			{
				// "Your opponent has asked for a battle. Do you accept?"
				fdScriptSetMessageId(msgBase + 2);
			}
			else
			{
				// "Would you like to apply?"
				fdScriptSetMessageId(msgBase + 1);
			}
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			if (fdScriptAskYesNoWithActorAnimation(0, 0) == YES)
			{
				// Yes, and the opponent is still there; else the link is lost.
				if (scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE))
				{
					gmFlagSet(FLAG_LINK_BATTLE);
					gmFlagSet(FLAG_LINK_BATTLE_ARRANGED);
					if (gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED))
					{
						// "When you finish your preparations, please step into the circle!"
						fdScriptSetMessageId(msgBase + 3);
					}
					else
					{
						// "You have asked for a battle. We will call your name if your..."
						fdScriptSetMessageId(msgBase + 4);
					}
					scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ASKED);
					gmFlagSet(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
					goto open_message;
				}
				else
				{
					gmFlagSet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
				}
			}
			else
			{
				fdScriptSetMessageId(msgBase);
				// "Please speak to me when you want to fight in a battle."
				fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			}
		}
	}
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Talking to the muscleman behind the east counter gives the linked battles' record. From the front
// of the counter he tells the linked battles won in all; from the side, the ones won in a row, and
// points to the board over the gate for the best run. Before any linked battle he only says he will
// tell the player once the linked finals are over.
static Sint32 scBattleArenaTellLinkedWins(Sint32 actorId)
{
	Uint32  facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	Uint16 *tally;
	Sint32  msgBase;

	fdScriptStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if (FACING_IS_NORTH(facing))
	{
		msgBase = MsgLobbyLooksLikeFirst;
		tally   = &MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_WINS];
		if (*tally != 0)
		{
			mnMessageArgumentClearAll();
			mnMessageArgumentPush(*tally, MESSAGE_ARGUMENT_NUMBER);
			fdScriptSetMessageId(msgBase + 1);
			// "You've won ... Linked Battle. Keep up the good work!"
			fdScriptShowMessage(actorId, 0);
		}
		else
		{
			fdScriptSetMessageId(MsgLobbyTellTotalWins);
			// "I will tell you your total wins when you finish the linked..."
			fdScriptShowMessage(actorId, 0);
		}
	}
	else
	{
		msgBase = MsgLobbyLooksLikeFirstTime;
		tally   = &MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK];
		if (*tally != 0)
		{
			mnMessageArgumentClearAll();
			mnMessageArgumentPush(*tally, MESSAGE_ARGUMENT_NUMBER);
			fdScriptSetMessageId(msgBase + 1);
			// "You've won ... Linked Battle. Check the board on the wall..."
			fdScriptShowMessage(actorId, 0);
		}
		else
		{
			fdScriptSetMessageId(MsgLobbyTellConsecutiveWins);
			// "I will tell you your consecutive wins when you finish the..."
			fdScriptShowMessage(actorId, 0);
		}
	}
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// A developers' leftover: the white-bearded man in green stands south of the hall's wall, where the
// player can never walk. Talking to him shows "Choose your parameters here." and a number picked
// with Left and Right; A loads that numbered set of levels and equipment for testing
// (`btDebugParamSetLoad`) and B gives up. The muscleman behind the east counter answers: "The
// settings are made. The rest is up to you.", "There's no such parameter. Ask Mr. Sugimoto." or
// "What is this, just a tease?".
static Sint32 scBattleArenaChooseDebugParamSet(Sint32 actorId)
{
	WINDOW *window;
	Sint32  setIndex;
	Sint32  displayedSet;
	Sint32  loadResult;

	fdScriptStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	fdScriptSetMessageId(MsgLobbyChooseParameters);
	// "Choose your parameters here."
	fdScriptShowMessage(actorId, 0);
	window       = mnWindowCreate(0, 0, 6, 4, WINDOW_FLAG_INSTANT);
	setIndex     = 0;
	displayedSet = -1;
	for (;;)
	{
		if (setIndex != displayedSet)
		{
			mnWindowSpritePrepareForRedraw(window);
			mnTextDrawNumber(setIndex, 3, window, 0, 0);
			displayedSet = setIndex;
		}
		if (PerRepeat & KEY_LEFT)
		{
			setIndex--;
		}
		if (PerRepeat & KEY_RIGHT)
		{
			setIndex++;
		}
		if (setIndex < 0)
		{
			setIndex = 0;
		}
		if (PerPush & KEY_A)
		{
			break;
		}
		if (PerPush & KEY_B)
		{
			setIndex = -1;
			break;
		}
		sySynch(1);
	}
	mnWindowClose(window, 1);
	if (setIndex >= 0)
	{
		loadResult = btDebugParamSetLoad(setIndex);
	}
	if (setIndex < 0)
	{
		fdScriptSetMessageId(MsgLobbyTease);
		// "What is this, just a tease?"
		fdScriptShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	else if (loadResult != 0)
	{
		fdScriptSetMessageId(MsgLobbyNoParameterAsk);
		// "There's no such parameter. Ask Mr. Sugimoto."
		fdScriptShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	else
	{
		fdScriptSetMessageId(MsgLobbySettingsMadeRest);
		// "The settings are made. The rest is up to you."
		fdScriptShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	sySynch(10);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Talking to the healer in the tall hat, in the room up the east stairs, offers a Sanctum's
// services, the same as in any town (`mnSanctumRun`).
static Sint32 scBattleArenaOpenSanctum(Sint32 actorId)
{
	fdScriptStart();
	mnSanctumRun(actorId);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Returns 1 when character memberId is among the first three party members while a linked battle is
// arranged (FLAG_LINK_BATTLE_ARRANGED), or its first four otherwise. Returns 0 when not.
static Bool scBattleArenaPartyContains(Sint32 memberId)
{
	Sint32 count;
	Sint32 memberLimit;
	Sint32 i;

	count       = gmPartyCountMembers();
	memberLimit = 3;
	if (gmFlagGet(FLAG_LINK_BATTLE_ARRANGED) == 0)
	{
		memberLimit = 4;
	}
	if (count > memberLimit)
	{
		count = memberLimit;
	}
	for (i = 0; i < count; i++)
	{
		if (MainRoutineStateWork.party[i] == BATTLE_UNIT_LIST_END)
		{
			return FALSE;
		}
		if (MainRoutineStateWork.party[i] == memberId)
		{
			return TRUE;
		}
	}
	return FALSE;
}

// UNUSED: an empty function the Battle Arena's code keeps.
UNUSED static void scBattleArenaEmpty(void)
{
}

// Talking to Garet, Ivan, Mia or Jenna in the room up the east stairs gets a line about the coming
// battle that depends on who will fight it. Those in the party cheer ("I can't wait, Isaac! Let's
// go get 'em!"), one left out asks whether Isaac means to fight alone or to sit out, and after a
// linked battle each speaks of the win or the loss, whether they fought or watched.
//
// The linked battle takes only the first three of the party (`scBattleArenaPartyContains`). The
// line is the speaker's own in each list, one message after the other for Garet, Ivan, Mia and
// Jenna.
static Sint32 scBattleArenaTalkByProgress(Sint32 actorId)
{
	Sint32 messageBase    = MsgLobbyCantWaitLets;
	Sint32 isaacPresent   = scBattleArenaPartyContains(ISAAC);
	Sint32 speakerPresent = scBattleArenaPartyContains(actorId);

	fdScriptStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if (gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED))
	{
		// Isaac's flag is tested and the result dropped; messageBase first holds whether the
		// speaker fought (FLAG_BATTLE_ARENA_IN_PARTY_FIRST + actor id).
		fdScriptGetFlag(FLAG_BATTLE_ARENA_IN_PARTY_FIRST);
		messageBase = gmFlagGet(actorId + FLAG_BATTLE_ARENA_IN_PARTY_FIRST);
		if (gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_WON))
		{
			if (messageBase)
				messageBase = MsgLobbyWonToldCountOn;
			else
				messageBase = MsgLobbyWonWithoutWell;
		}
		else
		{
			if (messageBase)
				messageBase = MsgLobbyNotFaultLost;
			else
				messageBase = MsgLobbyKnewLostBecause;
		}
	}
	else if (isaacPresent != 0)
	{
		if (speakerPresent == 0)
			messageBase = MsgLobbyGoingFightAlone;
	}
	else
	{
		messageBase = MsgLobbyTryingGetAway;
	}
	fdScriptSetMessageId(messageBase + actorId - 1);
	fdScriptShowMessage(actorId, 0);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Walking south out of the hall, down the passage he came in by, metatile (13, 19), leaves the
// Battle Arena and goes back to the File Screen.
//
// The link is stopped and the sound's normal echo restored first.
static Sint32 scBattleArenaLeaveArena(void)
{
	sySerialRemoveIRQHandlers();
	sySoundLoadReverbCode(SOUND_REVERB_NORMAL);
	fdScriptSetDestination(SCENE_FILE_SCREEN, FILE_SCREEN_FROM_SANCTUM);
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// UB: the handlers below return no value, so their result is whatever fdScriptEnd returned.

// Talking to one of three visitors speaks of the Battle Arena: the swordsman in the room up the
// west stairs is a traveling warrior who warns not to underestimate the monsters, the old man with
// a cane there says up to three allies fight in the linked finals, and the old woman beside him
// wonders if this is the arena the old man dreamed of. After a linked battle each praises the win
// or consoles the loss.
static Sint32 scBattleArenaTalkToVisitor(Sint32 actorId)
{
	Sint32 msg;
	Sint32 step = 0;

	fdScriptStart();
	switch (actorId)
	{
		case ACTOR_TRAVELING_WARRIOR     : msg = MsgLobbyTravelingWarrior; break;
		case ACTOR_FINALS_VISITOR        : msg = MsgLobbyThreeAlliesFight; break;
		case ACTOR_OLD_MANS_ARENA_VISITOR:
		default                          : msg = MsgLobbyBattleArenaOld; break;
	}
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if (gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED))
		step = 2 - (gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_WON) != 0);
	fdScriptSetMessageId(msg + step);
	fdScriptOpenMessage(actorId, 0);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Talking to the fair-haired boy on the west side of the hall gives two tips in turn. First, up to
// three allies can fight in the linked finals, so with four in the party the rightmost has to pass;
// then, the party's order can be changed in the status screen.
static Sint32 scBattleArenaExplainPartyOrder(Sint32 actorId)
{
	Sint32 msg;

	fdScriptStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if (!fdScriptGetFlag(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT))
	{
		if (gmPartyCountMembers() <= 3)
			msg = MsgLobbyThreeAlliesFightLinkedFinals;
		else
			msg = MsgLobbyThreeAlliesFightLinked;
		gmFlagSet(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT);
	}
	else
	{
		msg = MsgLobbyChangeOrderParty;
		gmFlagClear(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT);
	}
	fdScriptSetMessageId(msg);
	// "You can change the order of your party in the status screen...."
	fdScriptOpenMessage(actorId, 0);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Talking to the old man with a cane beside the gate gives the most monster battles won in a row
// ("You've won 3 consecutive Monster Battles."), or before any, that he will be counting the
// player's victories.
static Sint32 scBattleArenaReportMonsterRecord(Sint32 actorId)
{

	fdScriptStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if (MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] != 0)
	{
		mnMessageArgumentPush(MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK],
		                      MESSAGE_ARGUMENT_NUMBER);
		fdScriptSetMessageId(MsgLobbyWonNumberConsecutive);
	}
	else
	{
		fdScriptSetMessageId(MsgLobbyCantWaitSee);
	}
	fdScriptOpenMessage(actorId, 0);
	fdScriptEnd();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

Uint8 *scGetCues(void)
{
	return (Uint8 *)BattleArenaCues;
}

// Saves the file after a linked battle, between "Saving battle results." and "Battle results were
// saved.".
static Sint32 scBattleArenaSaveBattleResults(void)
{
	WINDOW *window;

	sySoundPlay(MUS_SAVED);
	window = mnMessageOpenWindow(MsgLobbySavingBattleResults, 5, 4, MESSAGE_WINDOW_ZOOM);
	while (mnMessageAllTyped() == 0)
		sySynch(1);
	mnSaveWriteLinkTallies();
	mnWindowClose(window, 1);
	sySynch(1);
	window = mnMessageOpenWindow(MsgLobbyBattleResultsSaved, 5, 4, MESSAGE_WINDOW_ZOOM);
	while (mnMessageAllTyped() == 0)
		sySynch(1);
	mnWindowClose(window, 1);
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Saves the file after a series of monster battles that set a new record, between "Saving Monster
// Battle results." and "Monster Battle results were saved.". It is scBattleArenaSaveBattleResults
// with the Monster Battle's two lines; the game keeps each save as a function of its own, with no
// parameter for its messages.
static Sint32 scBattleArenaSaveMonsterBattleResults(void)
{
	WINDOW *window;

	sySoundPlay(MUS_SAVED);
	window = mnMessageOpenWindow(MsgLobbySavingMonsterBattle, 5, 4, MESSAGE_WINDOW_ZOOM);
	while (mnMessageAllTyped() == 0)
		sySynch(1);
	mnSaveWriteLinkTallies();
	mnWindowClose(window, 1);
	sySynch(1);
	window = mnMessageOpenWindow(MsgLobbyMonsterBattleResults, 5, 4, MESSAGE_WINDOW_ZOOM);
	while (mnMessageAllTyped() == 0)
		sySynch(1);
	mnWindowClose(window, 1);
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Shows value, at most WALL_BOARD_MAX, on the board over the gate, metatiles (14, 8) to (16, 8).
// The digits' pictures stand in map column 27, digit d in row d.
static Sint32 scBattleArenaDrawThreeDigitValue(Sint32 value)
{
	Sint32 digit;

	if (value > WALL_BOARD_MAX)
		value = WALL_BOARD_MAX;
	for (digit = 0; digit < WALL_BOARD_DIGITS; digit++)
	{
		fdMapGridCopyMetatilesTo(27, value % 10, 16 - digit, 8, 1, 1);
		value = value / 10;
	}
	fdMapGridCenterOnFollowedPoint();
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Each time the party comes into the hall, from the File Screen or back from the arena, the battle
// that brought it here is settled. After a monster battle won, the attendant offers the next
// monster ("Not bad! Would you like to fight the next monster?"), with ever higher praise; No, or a
// loss, ends the series with the number won, and a new record is saved. A linked battle is counted,
// the board over the gate updated and the file saved. From the File Screen the link is started.
Sint32 scInit(void)
{
	Sint32       i;
	FIELD_STATE *state;
	void         (*callback)(void);

	BattleArenaCallTicks       = 0;
	BattleArenaWaitTicks       = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	sySoundLoadReverbCode(SOUND_REVERB_NORMAL);
	scBattleArenaDrawThreeDigitValue(
		MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
	fdMapGridCopyMetatileAttributes(11, 11, 1, 1, 13, 10);
	scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	sySynch(1);
	fdTilesetAnimationPause(BATTLE_ARENA_GATE_ANIMATION);

	// The payload's halfwords 4 to 7 hold the link battle's phase word ("VSST", "EDVS", "TUrn");
	// the Battle Arena puts TALK there.
	{
		Uint16 *name = (Uint16 *)SerialTransfer.payload;

		name[4]      = 'T';
		name[5]      = 'A';
		name[6]      = 'L';
		name[7]      = 'K';
	}
	// Note who is in the battle party, for the party members' lines.
	for (i = 0; i < 8; i++)
	{
		gmFlagClear(FLAG_BATTLE_ARENA_IN_PARTY_FIRST + i);
		if (scBattleArenaPartyContains(i))
		{
			gmFlagSet(FLAG_BATTLE_ARENA_IN_PARTY_FIRST + i);
		}
	}
	// A monster battle won: offer the next one.
	if (MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_MONSTER_WIN)
	{
		Sint32 windows;
		Sint32 msgOffset;
		Sint32 v;

		fdScriptStart();
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ALONE);
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_WINS]++;
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK]++;
		// The offer has two lines a level (FLAG_MONSTER_BATTLE_WINS, -1 when a series starts), the
		// next monster's line going no further than MONSTER_OFFER_LAST_NEXT. FLAG_AREA_TEMP_E8's
		// byte counts the victories 0 to 2; the third raises the level and takes the line after,
		// which promises a stronger monster.
		windows   = (Sint8)syFieldGameFlagGetByte(FLAG_MONSTER_BATTLE_WINS);
		msgOffset = windows * 2 + 2;
		if (msgOffset > MONSTER_OFFER_LAST_NEXT)
		{
			msgOffset = MONSTER_OFFER_LAST_NEXT;
		}
		v = syFieldGameFlagGetByte(FLAG_AREA_TEMP_E8);
		if (v == 2)
		{
			syFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, 0);
			windows++;
			msgOffset++;
		}
		else
		{
			syFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, v + 1);
		}
		fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
		fdScriptSetMessageId(MsgLobbyNotBadNextMonster + msgOffset);
		fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
		if (fdScriptAskYesNoWithActorAnimation(0, 0) == YES)
		{
			if (windows > LEVEL_BONUS_MAX)
			{
				windows = LEVEL_BONUS_MAX;
			}
			syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, windows);
		}
		else
		{
			// No: the series ends, and a new best run is saved.
			gmFlagClear(FLAG_MONSTER_BATTLE);
			syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
			mnMessageArgumentPush(MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
			                      MESSAGE_ARGUMENT_NUMBER);
			if (MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK]
			    < MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK])
			{
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
					MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
				fdScriptSetMessageId(MsgLobbyWonNumberBattleBroke);
				// "You won ... battle. You broke the record!"
				fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
				scBattleArenaSaveMonsterBattleResults();
			}
			else
			{
				fdScriptSetMessageId(MsgLobbyWonNumberBattle);
				// "You won ... battle. Please speak to me when you wish to..."
				fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			}
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		}
		fdScriptEnd();
		// A monster battle lost: the series ends, and a new best run is saved.
	}
	else if (MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_MONSTER_LOSS)
	{
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_LOSSES]++;
		fdScriptStart();
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ALONE);
		fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
		mnMessageArgumentPush(MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
		                      MESSAGE_ARGUMENT_NUMBER);
		if (MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK]
		    < MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK])
		{
			MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
			fdScriptSetMessageId(MsgLobbyWonNumberBattleBroke);
			// "You won ... battle. You broke the record!"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			scBattleArenaSaveMonsterBattleResults();
		}
		else
		{
			fdScriptSetMessageId(MsgLobbyWonNumberBattleTold);
			// "You won ... battle. I told you our monsters were tough!..."
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
		}
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = 0;
		gmFlagClear(FLAG_MONSTER_BATTLE);
		syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		fdScriptEnd();
		// A linked battle won: count the win and the run; a longer run is the board's new record.
		// Saved.
	}
	else if (MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_LINK_WIN)
	{
		Sint32 v;

		fdScriptStart();
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
		// The battle broke off instead (btRun set FLAG_AREA_TEMP_E8): report the link lost and
		// restart it.
		if (fdScriptGetFlag(FLAG_AREA_TEMP_E8))
		{
			state = FieldStateWork;
			gmFlagClear(FLAG_AREA_TEMP_E8);
			state->raisedCue = BATTLE_ARENA_CUE_DISCONNECTED;
			gmFlagClear(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
			sySynch(20);
			scBattleArenaRestartLink();
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
		}
		else
		{
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_WINS]++;
			v = MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] + 1;
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
			if (MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] < (Uint16)v)
				MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] = v;
			scBattleArenaDrawThreeDigitValue(
				MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
			scBattleArenaSaveBattleResults();
			gmFlagSet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
			gmFlagSet(FLAG_BATTLE_ARENA_LINK_BATTLE_WON);
		}
		fdScriptEnd();
		// A linked battle lost: count the loss and end the run. Saved.
	}
	else if (MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_LINK_LOSS)
	{
		Sint32 v;

		fdScriptStart();
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
		if ((v = gmFlagGet(FLAG_MONSTER_BATTLE)) == 0)
		{
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_LOSSES]++;
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
			scBattleArenaSaveBattleResults();
		}
		gmFlagSet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
		gmFlagClear(FLAG_BATTLE_ARENA_LINK_BATTLE_WON);
		fdScriptEnd();
		// Arriving from the File Screen: start the link, clear FLAG_LINK_BATTLE_ARRANGED and reset
		// the monster level. Idle sleep is turned off here, with a note when it was on.
	}
	else
	{
		sySerialInit();
		gmFlagClear(FLAG_LINK_BATTLE_ARRANGED);
		syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
		if (MainRoutineStateWork.idleSleepEnabled != 0)
		{
			fdScriptStart();
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
			fdScriptSetMessageId(MsgLobbyNoteCantUse);
			// "Note: you can't use the sleep function in battle stages!"
			fdScriptShowMessage(ACTOR_ATTENDANT, 0);
			fdScriptEnd();
		}
		MainRoutineStateWork.idleSleepEnabled = 0;
		IdleSleepEnabled                      = 0;
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	}
	callback = (EVENT_FUNC)scBattleArenaWatchOpponent;
	sySetTickEvent(callback, EVENT_PASS_TICK);
	sySetEventFlags(callback, EVENT_FLAG_KEEP_RUNNING);
	if (MainRoutineStateWork.entrance != BATTLE_ARENA_AFTER_MONSTER_WIN
	    || !gmFlagGet(FLAG_MONSTER_BATTLE))
	{
		gmPartyRestoreHpPp(1);
		btDjinnRecoveryCountDownTwenty();
	}
	return 0;
}

// clang-format off
// The payload word each slot of BattleArenaSlotValues goes in. 0 is the signature and 1 the state. The last two entries have no slot.
Uint8 BattleArenaSlotColumns[] = {
	0, 1, 1, 1, 1, 0, 0, 0,
};

// Where the party appears. BATTLE_ARENA_FROM_FILE_SCREEN is from the File Screen, and the BATTLE_ARENA_AFTER_ ones are after a battle (scBattleArenaEnterBattle).
SCENE_ENTRANCE BattleArenaEntrances[] = {
	{ BATTLE_ARENA_AFTER_LINK_LOSS, CONDITION_ALWAYS, 128, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
	{ BATTLE_ARENA_AFTER_LINK_WIN, CONDITION_ALWAYS, 352, 0, 148, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
	{ BATTLE_ARENA_AFTER_MONSTER_WIN, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
	{ BATTLE_ARENA_AFTER_MONSTER_LOSS, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
	{ BATTLE_ARENA_FROM_FILE_SCREEN, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
	{ SCENE_TABLE_END },
};

Uint32 BattleArenaExits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE BattleArenaActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 204, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		304, 0, 168, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		192, 0, 272, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		128, 0, 124, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		64, 0, 92, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		40, 0, 120, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		256, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 144, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 144, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		128, 0, 164, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		64, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		72, 0, 288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(GARET, IF_FLAG_SET(FLAG_IN_PARTY(GARET)), MOVEMENT_TYPE_STAND,
		352, 0, 108, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(IVAN, IF_FLAG_SET(FLAG_IN_PARTY(IVAN)), MOVEMENT_TYPE_STAND,
		328, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(MIA, IF_FLAG_SET(FLAG_IN_PARTY(MIA)), MOVEMENT_TYPE_STAND,
		376, 0, 136, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(JENNA, IF_FLAG_SET(FLAG_IN_PARTY(JENNA)), MOVEMENT_TYPE_STAND,
		376, 0, 136, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE BattleArenaBattleActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 204, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		304, 0, 168, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		192, 0, 272, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PERSON_WITH_SWORD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 136, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		64, 0, 92, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		40, 0, 120, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_WHITE_BEARD_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		256, 0, 384, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		352, 0, 108, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		352, 0, 156, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 136, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		64, 0, 136, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 184, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 88, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		72, 0, 288, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(GARET, IF_FLAG_SET(FLAG_IN_PARTY(GARET)), MOVEMENT_TYPE_STAND,
		104, 0, 144, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(IVAN, IF_FLAG_SET(FLAG_IN_PARTY(IVAN)), MOVEMENT_TYPE_STAND,
		152, 0, 144, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(MIA, IF_FLAG_SET(FLAG_IN_PARTY(MIA)), MOVEMENT_TYPE_STAND,
		128, 0, 124, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(JENNA, IF_FLAG_SET(FLAG_IN_PARTY(JENNA)), MOVEMENT_TYPE_STAND,
		128, 0, 124, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};
// clang-format on

CUE BattleArenaCues[] = {
	{CUE_TALK, GARET, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaTalkByProgress)},
	{CUE_TALK, IVAN, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaTalkByProgress)},
	{CUE_TALK, MIA, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaTalkByProgress)},
	{CUE_TALK, JENNA, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaTalkByProgress)},
	{CUE_TALK, ACTOR_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaChooseBattleMode)},
	{CUE_TALK, ACTOR_LINKED_WINS_TELLER, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaTellLinkedWins)},
	{CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaOpenSanctum)},
	{CUE_TALK, ACTOR_GREETER, CONDITION_ALWAYS, MSGNO(MsgFieldWelcomeToBattleArenaWherePeople)},
	{CUE_TALK, ACTOR_TRAVELING_WARRIOR, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaTalkToVisitor)},
	{CUE_TALK, ACTOR_FINALS_VISITOR, CONDITION_ALWAYS, SCRIPT_REF(scBattleArenaTalkToVisitor)},
	{CUE_TALK,
     ACTOR_OLD_MANS_ARENA_VISITOR, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaTalkToVisitor)},
	{CUE_TALK,
     ACTOR_DEBUG_PARAMETERS_MAN, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaChooseDebugParamSet)},
	{CUE_TALK,
     ACTOR_PARTY_ORDER_VISITOR, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaExplainPartyOrder)},
	{CUE_TALK,
     ACTOR_COMMAND_TIME_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldInTheLinkedFinalsYouCant)},
	{CUE_TALK, ACTOR_SUMMON_VISITOR, CONDITION_ALWAYS, MSGNO(MsgFieldToUseSummonQuicklyDjinnMust)},
	{CUE_TALK, ACTOR_SET_DJINN_VISITOR, CONDITION_ALWAYS, MSGNO(MsgFieldDoYouPutDjinnOnStandby)},
	{CUE_TALK,
     ACTOR_STANDBY_DJINN_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldRemoveDjinnYouRarelyUseSo)},
	{CUE_TALK,
     ACTOR_MONSTER_RULES_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgFieldMonsterBattleRulesSayYouAll)},
	{CUE_TALK,
     ACTOR_MONSTER_WINS_TELLER, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaReportMonsterRecord)},
	{CUE_TALK, ACTOR_EDGY_VISITOR, CONDITION_ALWAYS, MSGNO(MsgFieldAllTheWarriorsAreImpressiveSo)},
	{CUE_TALK, ACTOR_WARY_VISITOR, CONDITION_ALWAYS, MSGNO(MsgFieldICouldEnterButIdJust)},
	{CUE_STEP,
     METATILE_BEHAVIOR_BATTLE_ARENA_CIRCLE, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaEnterBattle)},
	{CUE_STEP,
     METATILE_BEHAVIOR_BATTLE_ARENA_WAY_OUT, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaLeaveArena)},
	{CUE_RAISED,
     BATTLE_ARENA_CUE_ASK_TO_PARTICIPATE, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaAskToParticipate)},
	{CUE_RAISED,
     BATTLE_ARENA_CUE_DISCONNECTED, CONDITION_ALWAYS,
     SCRIPT_REF(scBattleArenaReportLinkDisconnected)},
	{CUE_TABLE_END},
};
