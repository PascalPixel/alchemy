/* @file */
/* The Battle Arena, the hall Isaac walks into when Battle is chosen on the */
/* File Screen with a save that has a Djinni. He comes in from the south, up a
 */
/* short passage. Talking to the attendant behind the west counter arranges a */
/* battle: with a monster when no second Game Boy Advance answers, or over the
 */
/* link cable with a friend. Stepping onto the diamond mark in the middle of */
/* the hall then sends the party through the iron gate in the north wall into */
/* the arena. Afterwards the party is back here: the muscleman behind the east
 */
/* counter keeps the linked battles' record and the old man beside the gate */
/* the monster battles', and the board over the gate shows the longest run of */
/* linked wins. A healer in the room up the east stairs offers a Sanctum's */
/* services, Garet, Ivan, Mia and Jenna wait in that room, and visitors in the
 */
/* hall and the room up the west stairs talk about the rules. */
/* */
/* Most of the code is the link: the two consoles publish where each player */
/* stands in the arranging, and trade their parties before the battle. */

#include "flags.h"
#include "battle/battle.h"
#include "battle/battle_side.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "graphics/graphics.h"
#include "sdk/sound_driver_symbols.h"
#include "text/msg_ids.h"

/* The cells of the hall that start an event when stepped on. */
typedef enum BATTLE_ARENA_CELL_BEHAVIOR {
	/* The passage out to the south, cell (13, 19). */
	CELL_BEHAVIOR_BATTLE_ARENA_WAY_OUT = 1,
	/* The diamond mark in the middle of the hall, cell (13, 12). */
	CELL_BEHAVIOR_BATTLE_ARENA_CIRCLE = 10,
} BATTLE_ARENA_CELL_BEHAVIOR;

/* The map's tileset animation of the iron gate in the north wall, run for */
/* eight frames as the party walks to it from the diamond mark. */
enum { BATTLE_ARENA_GATE_ANIMATION = 5 };

s32 scInit(void);
extern u8 BattleArena_SlotColumns[];
extern SCENE_ENTRANCE BattleArenaEntrances[];
extern u32 BattleArenaExits[];
extern ACTOR_PLACE BattleArenaActorPlacements[];
extern ACTOR_PLACE BattleArenaBattleActorPlacements[];
extern TRIGGER BattleArenaTriggers[];

/* The attendant has asked the player to take part. Every */
/* BATTLE_ARENA_ASK_AGAIN_CALLS-th call of scBattleArenaAskToParticipate clears
 */
/* it so she asks again. */
#define FLAG_BATTLE_ARENA_ATTENDANT_ASKED FLAG_SCENE_TEMP_00
/* The opponent has asked for a battle: its signature matches, and it stands */
/* asked or in the circle. */
#define FLAG_BATTLE_ARENA_OPPONENT_ASKED FLAG_SCENE_TEMP_01
/* The player has applied for a battle ("Would you like to apply?"). */
#define FLAG_BATTLE_ARENA_PLAYER_APPLIED FLAG_SCENE_TEMP_02
/* The party is on its way into the battle. */
#define FLAG_BATTLE_ARENA_ENTERING_BATTLE FLAG_SCENE_TEMP_03
/* ACTOR_PARTY_ORDER_VISITOR has given the linked finals' limit of three */
/* allies, so the next talk explains how to change the party's order. */
#define FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT FLAG_SCENE_TEMP_04
/* The link was lost and the attendant has yet to say so. */
#define FLAG_BATTLE_ARENA_LINK_DISCONNECTED FLAG_SCENE_TEMP_05
/* The first of eight flags, one per character from Isaac, each set while that
 */
/* character is in the battle party (scInit). */
#define FLAG_BATTLE_ARENA_IN_PARTY_FIRST FLAG_SCENE_TEMP_F0
/* The attendant has explained the Battle Arena once. Talking to her again */
/* offers to apply. */
#define FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED FLAG_AREA_TEMP_00
/* This console is the link's child (multiplayer id 1), which receives the */
/* parties first. */
#define FLAG_BATTLE_ARENA_LINK_CHILD FLAG_AREA_TEMP_02
/* The link is up. Both consoles were heard (BATTLE_ARENA_LINK_READY_MASK). */
#define FLAG_BATTLE_ARENA_LINK_UP FLAG_AREA_TEMP_03
/* A linked battle has ended since the leader last walked south of z 224, so */
/* the party and the visitors speak of it. */
#define FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED FLAG_AREA_TEMP_04
/* The last linked battle was won. */
#define FLAG_BATTLE_ARENA_LINK_BATTLE_WON FLAG_AREA_TEMP_05

/* The most monster battles won in a row that count: each adds a level to the */
/* next monster's over the party's average (battle_formation.c). */
enum { LEVEL_BONUS_MAX = 90 };

/* Link transfers use three unit records followed by one recovery-table record.
 */
/* Both sizes are whole 20-byte blocks, since the serial runtime sends 20 bytes
 */
/* a block and ends a send only when its count reaches exactly 0. */
/* The bytes sent for each party member. They are a Unit (BATTLE_UNIT_SIZE, 332
 */
/* bytes) and the eight bytes after it, 17 blocks of 20. */
#define BATTLE_ARENA_UNIT_RECORD_BYTES (17 * 20)
/* The bytes sent for the Djinn recovery table after the party members. They */
/* are the table (DJINN_RECOVERY_TABLE, 268 bytes) and the 52 bytes after
 */
/* it, 16 blocks of 20, two more than the table needs. */
#define BATTLE_ARENA_RECOVERY_RECORD_BYTES (16 * 20)
/* The party members a linked battle takes. A smaller party sends absent */
/* records. */
#define BATTLE_ARENA_UNIT_RECORD_COUNT 3
/* LinkStatusWork bits 0 and 1. Both consoles were heard this frame, as the */
/* battle code's LINK_BOTH_CONNECTED. */
#define BATTLE_ARENA_LINK_READY_MASK 3
/* A record fails on the 25th frame it waits without a link or past the */
/* timeout. */
#define BATTLE_ARENA_TRANSFER_RETRY_LIMIT 24
/* Frames, about ten seconds, for the three party member records, and again for
 */
/* the recovery table. */
#define BATTLE_ARENA_SEND_TIMEOUT_FRAMES 600
/* Frames, about fifteen seconds, for all four records received. */
#define BATTLE_ARENA_RECEIVE_TIMEOUT_FRAMES 900
/* Frames without a link (scBattleArenaWatchOpponent) or without the opponent's
 */
/* request (scBattleArenaEnterBattle) before the Battle Arena gives the */
/* opponent up. */
#define BATTLE_ARENA_PEER_WAIT_FRAMES 25
/* The calls of scBattleArenaAskToParticipate after which the attendant asks */
/* again. */
#define BATTLE_ARENA_ASK_AGAIN_CALLS 300

/* The wall board's digits, which show the most linked battles won in a row, */
/* and the most they show. */
#define WALL_BOARD_DIGITS 3
#define WALL_BOARD_MAX	  999
/* The attendant's offers of another monster battle follow */
/* MsgLobbyNotBadNextMonster, two to each word of praise: a stronger monster at
 */
/* an odd offset, the next one at the even offset after it. The highest praise
 */
/* offers the next monster at this offset ("Unbelievable!!! Would you like to */
/* fight the next monster?") and a stronger one at the offset after. */
#define MONSTER_OFFER_LAST_NEXT 14

/* The Battle Arena's flags, besides its own above, FLAG_MONSTER_BATTLE, */
/* FLAG_LINK_BATTLE and the two byte flags flags.h names: */
/* - FLAG_LINK_BATTLE_ARRANGED: a linked battle has been arranged (with */
/*   FLAG_LINK_BATTLE): only the first three party members take part */
/*   (scBattleArenaPartyContains). */
/* - FLAG_AREA_TEMP_E8: the last linked battle broke off (btRun). In a */
/*   monster series, the byte flag at FLAG_AREA_TEMP_E8 instead counts the */
/*   victories, 0 to 2, towards a stronger monster. */

/* The actors, numbering the rows of BattleArenaActorPlacements and */
/* BattleArenaBattleActorPlacements, which list the same people in the same */
/* order. The rows of Garet, Ivan, Mia and Jenna take those characters' own */
/* ids. */
typedef enum BATTLE_ARENA_ACTOR_ID {
	/* The attendant, who arranges every battle. ACTOR_GREETER calls her the
	   girl */
	/* at the counter. */
	ACTOR_ATTENDANT = ACTOR_FIRST_PLACED,
	/* Reports the linked battles won, and answers for */
	/* ACTOR_DEBUG_PARAMETERS_MAN. */
	ACTOR_LINKED_WINS_TELLER,
	/* Opens a Sanctum's services (scBattleArenaOpenSanctum). The sprite is */
	/* SPRITE_MAN_IN_TALL_HAT, the healer of every Sanctum in sanctum.c. */
	ACTOR_HEALER,
	ACTOR_GREETER,
	ACTOR_TRAVELING_WARRIOR,
	ACTOR_FINALS_VISITOR,
	ACTOR_OLD_MANS_ARENA_VISITOR,
	/* Loads a debug parameter set (scBattleArenaChooseDebugParmSet). */
	ACTOR_DEBUG_PARAMETERS_MAN,
	/* Explains the linked finals' limit of three allies, then the party order.
	 */
	ACTOR_PARTY_ORDER_VISITOR,
	ACTOR_COMMAND_TIME_VISITOR,
	ACTOR_SUMMON_VISITOR,
	ACTOR_SET_DJINN_VISITOR,
	ACTOR_STANDBY_DJINN_VISITOR,
	ACTOR_MONSTER_RULES_VISITOR,
	/* Reports the most Monster Battles won in a row. */
	ACTOR_MONSTER_WINS_TELLER,
	ACTOR_EDGY_VISITOR,
	ACTOR_WARY_VISITOR
} BATTLE_ARENA_ACTOR_ID;

/* The words of BattleArena_SlotValues, by slot. They are the signature, then */
/* the state a console publishes. */
typedef enum BATTLE_ARENA_SLOT {
	BATTLE_ARENA_SLOT_SIGNATURE = 0,
	BATTLE_ARENA_SLOT_ASKED = 1,	 /* has asked for a battle */
	BATTLE_ARENA_SLOT_IN_CIRCLE = 2, /* stands in the circle */
	BATTLE_ARENA_SLOT_UNUSED = 3,
	BATTLE_ARENA_SLOT_IDLE = 4,
	/* Replaces the signature when no opponent answers the attendant and during
	 */
	/* a monster series. */
	BATTLE_ARENA_SLOT_ALONE = 5
} BATTLE_ARENA_SLOT;

/* The Battle Arena's raised triggers (FIELD_STATE.raisedTrigger). */
typedef enum BATTLE_ARENA_RAISED_TRIGGER {
	BATTLE_ARENA_EVENT_NONE = 0,
	BATTLE_ARENA_EVENT_ASK_TO_PARTICIPATE =
		1, /* scBattleArenaAskToParticipate */
	BATTLE_ARENA_EVENT_DISCONNECTED =
		2 /* scBattleArenaReportLinkDisconnected */
} BATTLE_ARENA_RAISED_TRIGGER;

/* MainRoutineStateWork.linkTallies, as the Battle Arena keeps them. */
typedef enum LINK_TALLY {
	LINK_TALLY_MONSTER_WINS = 0,
	LINK_TALLY_MONSTER_LOSSES = 1,
	LINK_TALLY_MONSTER_BEST_STREAK = 2, /* the most won in a row */
	LINK_TALLY_MONSTER_STREAK = 3,		/* those won in the current series */
	LINK_TALLY_LINKED_WINS = 4,
	LINK_TALLY_LINKED_LOSSES = 5,
	/* The most linked battles won in a row, on the wall board. */
	LINK_TALLY_LINKED_BEST_STREAK = 6,
	LINK_TALLY_LINKED_STREAK = 7 /* those won in a row now */
} LINK_TALLY;

/* Four letters as one word, the first in its low byte. */
#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

/* clang-format off */
/* The words the Battle Arena publishes in its link payload, by slot. */
/* */
/* The opponent compares them with its own (scBattleArenaPeerSlotMatches), by */
/* BATTLE_ARENA_SLOT. The signature is in the first column */
/* (BattleArena_SlotColumns), the states in the second. */
/* */
/* In English and French BATTLE_ARENA_SLOT_ALONE equals the signature and changes */
/* nothing. Elsewhere it is the signature's letters reversed. */
/* */
/* So while a console publishes it (no opponent answered, or a monster series */
/* runs) an opponent's signature test fails, and the attendant does not offer */
/* that console as an opponent. Each return to linking writes the signature */
/* back. */
const s32 BattleArena_SlotValues[] = {
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
/* clang-format on */

#undef PEER_TAG

/* A console's link payload as the Battle Arena reads it from */
/* SerialPeerPayloads. values[0] is its signature word and values[1] its state
 */
/* word (BattleArena_SlotValues, placed by BattleArena_SlotColumns). */
typedef struct LINK_PEER_PAYLOAD {
	s32 values[6]; /* the payload's words; only 0 and 1 are read here */
} LINK_PEER_PAYLOAD;

/* Frames scBattleArenaWatchOpponent has gone without a link, 0 to */
/* BATTLE_ARENA_PEER_WAIT_FRAMES. It is 0 while linked. */
static s32 sBattleArenaWaitFrames;

/* What a Battle Arena handler's early exit returns. */
/* */
/* The handlers return nothing there, leaving whatever the last call returned */
/* (UB). UBFIX returns 0, as the function ends that fall off do. */
#ifdef UBFIX
#define BATTLE_ARENA_NO_RESULT 0
#else
#define BATTLE_ARENA_NO_RESULT
#endif

/* The Battle Arena's entrances, which fdSequenceRun reads through */
/* entry/battle_arena.s. */
u8 *scGetEntrances(void) { return (u8 *)BattleArenaEntrances; }

/* The exit regions entry: the Battle Arena has no exit boxes. */
s32 scGetExitRegions(void) { return 0; }

/* Restarts the link with interrupts locked. */
static s32 scBattleArenaRestartLink(void) {
	u32 savedIrqState;

	IRQ_LOCK(savedIrqState);
	sySerialRemoveIrqHandlers();
	sySerialInit();
	IRQ_UNLOCK(savedIrqState);
#ifdef UBFIX
	return 0;
#endif
}

u8 *scGetExits(void) { return (u8 *)BattleArenaExits; }

/* The actor placements entry. */
s32 scGetActorPlacements(void) {
	s16 entrance = MainRoutineStateWork.entrance;

	if(entrance == BATTLE_ARENA_AFTER_LINK_LOSS ||
	   entrance == BATTLE_ARENA_AFTER_MONSTER_LOSS) {
		return (s32)BattleArenaBattleActorPlacements;
	}
	return (s32)BattleArenaActorPlacements;
}

/* Returns 1 when the opponent's payload holds slot's word in slot's column, 0
 */
/* when it does not or there is no link. */
static s32 scBattleArenaPeerSlotMatches(s32 slot) {
	s32 serialPlayerId = -1;

	if((LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) ==
	   BATTLE_ARENA_LINK_READY_MASK) {
		/* 0 for the parent, 1 for the child. */
		serialPlayerId = SIO_ID_OF(REG_SIOCNT);
		gmFlagSet(FLAG_BATTLE_ARENA_LINK_UP);
	}
	else {
		gmFlagClear(FLAG_BATTLE_ARENA_LINK_UP);
	}
	/* At most one pass: a match returns 1, anything else breaks out. */
	while(serialPlayerId >= 0 && gmFlagGet(FLAG_BATTLE_ARENA_LINK_UP)) {
		const s32 *expected = &BattleArena_SlotValues[slot];

		if(serialPlayerId != 0)
			gmFlagSet(FLAG_BATTLE_ARENA_LINK_CHILD);
		else
			gmFlagClear(FLAG_BATTLE_ARENA_LINK_CHILD);
		{
			LINK_PEER_PAYLOAD *peer =
				(LINK_PEER_PAYLOAD *)
					SerialPeerPayloads[gmFlagGet(FLAG_BATTLE_ARENA_LINK_CHILD) ^
									   1]
						.payload;

			if(peer->values[BattleArena_SlotColumns[slot]] == *expected)
				return 1;
			break;
		}
	}
	return 0;
}

/* Publishes slot's word in its column of this console's link payload, where */
/* the opponent's scBattleArenaPeerSlotMatches reads it. */
static void scBattleArenaWriteSlotValue(s32 slot) {
	s32 *dst = (s32 *)SerialTransfer.payload;
	const s32 *src = &BattleArena_SlotValues[slot];
	dst[BattleArena_SlotColumns[slot]] = *src;
}

/* The task that watches the opponent each frame in the Battle Arena. */
static UNREAD_RESULT scBattleArenaWatchOpponent(void) {
	FIELD_STATE *state;
	s32 result;
	s32 i;

	state = FieldStateWork;
	result = 1;
	/* The leader is south of z 224 (16.16 pixels), towards the way out: the */
	/* party stops talking about the last battle. */
	if(fdActorGet(ACTOR_PARTY_LEADER)->z.fixed > toFIXED(224)) {
		gmFlagClear(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
	}
	/* While BATTLE_ARENA_EVENT_DISCONNECTED waits, skip the opponent tracking.
	 */
	if(state->raisedTrigger != BATTLE_ARENA_EVENT_DISCONNECTED) {
		scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE);
		if(!gmFlagGet(FLAG_BATTLE_ARENA_LINK_UP)) {
			/* Without a link for more than BATTLE_ARENA_PEER_WAIT_FRAMES
			 * frames, */
			/* clear every console's received payload and go idle. */
			if(++sBattleArenaWaitFrames > BATTLE_ARENA_PEER_WAIT_FRAMES) {
				for(i = 0; i < 4; i++) {
					syWordsClear(SerialPeerPayloads[i].payload, 20);
				}
				sBattleArenaWaitFrames = 0;
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
			}
		}
		else {
			sBattleArenaWaitFrames = 0;
		}
		/* While linked: has the opponent asked for a battle, or stepped into
		 * the */
		/* circle? */
		if(sBattleArenaWaitFrames == 0) {
			if(scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE) &&
			   (scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_ASKED) ||
				scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE))) {
				gmFlagSet(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				if(gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED)) {
					state->raisedTrigger =
						BATTLE_ARENA_EVENT_ASK_TO_PARTICIPATE;
				}
				result = 1;
			}
			else {
				gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				result = 0;
			}
		}
		if(gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED) &&
		   gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED) &&
		   !gmFlagGet(FLAG_BATTLE_ARENA_ATTENDANT_ASKED)) {
			state->raisedTrigger = BATTLE_ARENA_EVENT_ASK_TO_PARTICIPATE;
		}
	}
	/* BATTLE_ARENA_PEER_WAIT_FRAMES frames without a link while a linked battle
	 */
	/* is being arranged: report the link lost. */
	if((fdEventGetFlag(FLAG_BATTLE_ARENA_OPPONENT_ASKED) ||
		gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED)) &&
	   !gmFlagGet(FLAG_MONSTER_BATTLE) &&
	   !scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE) &&
	   sBattleArenaWaitFrames >= BATTLE_ARENA_PEER_WAIT_FRAMES) {
		state->raisedTrigger = BATTLE_ARENA_EVENT_DISCONNECTED;
		gmFlagSet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
		gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
		gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	}
	if(gmFlagGet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED)) {
		state->raisedTrigger = BATTLE_ARENA_EVENT_DISCONNECTED;
	}
	RETURN_UNREAD(result);
}

/* Calls of scBattleArenaAskToParticipate towards asking again, at */
/* BATTLE_ARENA_ASK_AGAIN_CALLS. It is cleared on every arrival. */
u32 BattleArenaCallFrames;

/* Once both linked players have applied for a battle, the attendant behind the
 */
/* west counter calls out "If you want to participate, please step into the */
/* circle!", and calls it again now and then while the player keeps her */
/* waiting. */
/* */
/* `scBattleArenaWatchOpponent` raises it */
/* (BATTLE_ARENA_EVENT_ASK_TO_PARTICIPATE) while both have applied, and every */
/* BATTLE_ARENA_ASK_AGAIN_CALLS calls the flag that keeps her quiet is cleared.
 */
/* */
/* Returns the flag's value when FLAG_BATTLE_ARENA_ENTERING_BATTLE or */
/* FLAG_BATTLE_ARENA_ATTENDANT_ASKED is set and nothing is shown. The path that
 */
/* shows the line returns nothing (UB). */
static s32 scBattleArenaAskToParticipate(void) {
	s32 set;

	set = gmFlagGet(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
	if(set != 0) {
		return set;
	}
	if(++BattleArenaCallFrames == BATTLE_ARENA_ASK_AGAIN_CALLS) {
		BattleArenaCallFrames = 0;
		gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	}
	set = fdEventGetFlag(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	if(set != 0) {
		return set;
	}
	fdEventStart();
	fdEventSetMessageId(MsgLobbyWantParticipatePlease);
	/* "If you want to participate, please step into the circle!" */
	fdEventOpenMessage(ACTOR_ATTENDANT, 0);
	sySynch(5);
	gmFlagSet(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* When the link cable's connection is lost while a linked battle is being */
/* arranged, the attendant behind the west counter says "The link was */
/* disconnected.", and the arranging starts over. */
/* */
/* `scBattleArenaWatchOpponent` and `scBattleArenaEnterBattle` raise it */
/* (BATTLE_ARENA_EVENT_DISCONNECTED). */
static s32 scBattleArenaReportLinkDisconnected(void) {
	FIELD_STATE *state = FieldStateWork;

	scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
	gmFlagClear(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
	fdEventStart();
	state->raisedTrigger = BATTLE_ARENA_EVENT_NONE;
	fdEventSetMessageId(MsgLobbyLinkDisconnected);
	/* "The link was disconnected." */
	fdEventOpenMessage(ACTOR_ATTENDANT, 0);
	gmFlagClear(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Receives the opponent's three party member records into units */
/* FIRST_ENEMY_UNIT to FIRST_ENEMY_UNIT + 2 (the battle's enemies, by */
/* gmUnitGet) and its Djinn recovery table into the enemy side's */
/* (gmDjinnGetRecoveryTable(DJINN_RECOVERY_OPPONENT)). */
/* */
/* Returns how many records hold a party member, or -1 when a transfer fails, */
/* overruns or runs out of time. */
static s32 scBattleArenaReceivePartyRecords(void) {
	u16 namePrefix[24];
	u32 size;
	void *buffer;
	s32 result;
	s32 slot;
	s32 timeout;
	s32 tries;
	UNIT *unit;
	s32 started;
	s32 prefixLength;
	s32 i;

	size = BATTLE_ARENA_UNIT_RECORD_BYTES;
	buffer = syHeapAllocEwram(size);
	result = 0;
	timeout = BATTLE_ARENA_RECEIVE_TIMEOUT_FRAMES;
	slot = 0;
	/* Three records, each started at recordNext and waited for at recordTest,
	 */
	/* then the recovery table at recovery. Each receive waits a frame at a
	 * time; */
	/* more than size bytes is an overrun and fewer once it ends a short record,
	 */
	/* and once the timeout has run out, or while the link is not ready, every
	 */
	/* frame is a try, failing past BATTLE_ARENA_TRANSFER_RETRY_LIMIT. */
	goto recordNext;
recordWait:
	if(SerialReceivedSize > size) {
		result = -1;
		goto done;
	}
	sySynch(1);
	if(--timeout < 0 || (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) !=
							BATTLE_ARENA_LINK_READY_MASK) {
		if(++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT) {
			result = -1;
			goto done;
		}
	}
recordTest:
	if(sySerialGetActiveTransfers() != 0) {
		goto recordWait;
	}
	if(SerialReceivedSize != size) {
		result = -1;
		goto done;
	}
	/* The sender marks a party member BATTLE_SIDE_PARTY and a padding record */
	/* BATTLE_SIDE_NONE. */
	if(unit->side != BATTLE_SIDE_NONE) {
		result++;
	}
	sySynch(2);
	mnFieldMessageDecodeRawUnbounded(MsgEnemyLabel, namePrefix);
	/* The prefix's length, at most BATTLE_ARENA_PREFIX_LAST + 1. */
	for(i = 0; i <= BATTLE_ARENA_PREFIX_LAST && namePrefix[i] != 0; i++)
		;
	/* German, Spanish, French and Italian put a space after the prefix. */
#if LANGUAGE == LANGUAGE_GERMAN || LANGUAGE == LANGUAGE_SPANISH ||             \
	LANGUAGE == LANGUAGE_FRENCH || LANGUAGE == LANGUAGE_ITALIAN
	namePrefix[i] = ' ';
	prefixLength = i + 1;
#else
	prefixLength = i;
#endif
	for(i = UNIT_NAME_SIZE - 1; i >= prefixLength; i--) {
		unit->name[i] = unit->name[i - prefixLength];
	}
	for(i = 0; i < prefixLength; i++) {
		unit->name[i] = namePrefix[i];
	}
	unit->name[UNIT_NAME_SIZE - 1] = 0;
	slot++;
recordNext:
	if(slot >= BATTLE_ARENA_UNIT_RECORD_COUNT) {
		goto recovery;
	}
	/* The opponent's party members become this battle's first three enemies. */
	unit = gmUnitGet(slot + FIRST_ENEMY_UNIT);
	tries = 0;
	if((started = sySerialStartReceive(unit)) == -1) {
		goto failed;
	}
	goto recordTest;
recoveryWait:
	started = SerialReceivedSize;
	if(started > BATTLE_ARENA_RECOVERY_RECORD_BYTES) {
		result = -1;
		goto done;
	}
	sySynch(1);
	if(--timeout < 0 || (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) !=
							BATTLE_ARENA_LINK_READY_MASK) {
		if(++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT) {
			result = -1;
			goto done;
		}
	}
recoveryTest:
	if(sySerialGetActiveTransfers() != 0) {
		goto recoveryWait;
	}
	if(SerialReceivedSize != BATTLE_ARENA_RECOVERY_RECORD_BYTES) {
		result = -1;
	}
	else {
		sySynch(2);
	}
	goto done;
recovery:
	/* Then the opponent's Djinn recovery table, as the enemy side's. */
	syHeapRelease(buffer);
	size = BATTLE_ARENA_RECOVERY_RECORD_BYTES;
	buffer = syHeapAllocEwram(size);
	tries = 0;
	if((started = sySerialStartReceive(
			gmDjinnGetRecoveryTable(DJINN_RECOVERY_OPPONENT))) != -1) {
		goto recoveryTest;
	}
failed:
	result = started;
done:
	syHeapRelease(buffer);
	return result;
}

/* Copies the character ids of the first three party members */
/* (MainRoutineStateWork.party), ends them with 0xff and returns how many. A */
/* null destination only counts them. */
static s32 scBattleArenaCopyPartyOwnerIds(u16 *memberIds) {
	s32 memberCount = gmPartyCountMembers();
	if(memberCount > BATTLE_ARENA_UNIT_RECORD_COUNT)
		memberCount = BATTLE_ARENA_UNIT_RECORD_COUNT;
	if(memberCount > 0) {
#ifndef UBFIX
		s16 *partyAddress = (s16 *)&MainRoutineStateWork;
#endif
		const u8 *source;
		s32 remaining;
#ifdef UBFIX
		/* The original short-pointer addition traverses a struct as an array.
		 */
		source = MainRoutineStateWork.party;
#else
		partyAddress += offsetof(MAIN_ROUTINE_STATE, party) / sizeof(s16);
		source = (const u8 *)partyAddress;
#endif
		remaining = memberCount;
		do {
			u8 memberId = *source++;
			if(memberIds != 0) {
				*memberIds = memberId;
				memberIds++;
			}
			remaining--;
		} while(remaining != 0);
	}
	if(memberIds != 0)
		*memberIds = BATTLE_UNIT_LIST_END;
	return memberCount;
}

/* Sends the first three party members' records, marked side 2 */
/* (BATTLE_SIDE_PARTY). */
/* */
/* Returns 0, or -1 when a send fails or runs out of time. The copies go */
/* through an EWRAM pool buffer freed before returning. */
static s32 scBattleArenaSendPartyRecords(void) {
	s32 result;
	u16 memberIds[8];
	u8 peerSlots[8];
	s32 started;
	u8 *buffer;
	s32 memberCount;
	s32 timeout;
	s32 tries;
	s32 i;
	s32 j;
	s32 k;
	u32 size;
	DJINN_RECOVERY_LIST *list;

	size = BATTLE_ARENA_UNIT_RECORD_BYTES;
	buffer = syHeapAllocEwram(size);
	result = 0;
	timeout = BATTLE_ARENA_SEND_TIMEOUT_FRAMES;
	memberCount = scBattleArenaCopyPartyOwnerIds(memberIds);
	for(i = 7; i >= 0; i--) {
		peerSlots[i] = 0;
	}
	/* Each send waits a frame at a time to finish. Once the timeout has run
	 * out, */
	/* or while the link is not ready, every frame is a try, and past */
	/* BATTLE_ARENA_TRANSFER_RETRY_LIMIT tries the exchange fails. A send that
	 */
	/* does not start breaks out to the release; the waits inside leave by done.
	 */
	do {
		for(i = 0; i < memberCount; i++) {
			syWordsCopy(buffer, gmUnitGet(memberIds[i]), size);
			((UNIT *)buffer)->side = BATTLE_SIDE_PARTY;
			/* As a u8, i - FIRST_ENEMY_UNIT wraps round to FIRST_ENEMY_UNIT +
			 * i, */
			/* FIRST_ENEMY_UNIT being half of 256: this party member's unit on
			 * the */
			/* opponent's side. */
			peerSlots[memberIds[i]] = i - FIRST_ENEMY_UNIT;
			started = sySerialStartSend(buffer, size);
			tries = 0;
			if(started == -1) {
				result = started;
				goto done;
			}
			while(sySerialGetActiveTransfers() != 0) {
				sySynch(1);
				if(--timeout < 0 ||
				   (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) !=
					   BATTLE_ARENA_LINK_READY_MASK) {
					if(++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT) {
						result = -1;
						goto done;
					}
				}
			}
			sySynch(2);
		}
		/* Pad to three records with the last copy marked absent
		 * (BATTLE_SIDE_NONE). */
		for(; i < BATTLE_ARENA_UNIT_RECORD_COUNT; i++) {
			((UNIT *)buffer)->side = tries = 0;
			if((started = sySerialStartSend(buffer, size)) == -1)
				break;
			while(sySerialGetActiveTransfers() != 0) {
				sySynch(1);
				if(--timeout < 0 ||
				   (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) !=
					   BATTLE_ARENA_LINK_READY_MASK) {
					if(++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT) {
						result = -1;
						goto done;
					}
				}
			}
			sySynch(2);
		}
		if(i < BATTLE_ARENA_UNIT_RECORD_COUNT) {
			result = started;
			break;
		}
		syHeapRelease(buffer);

		/* Then the recovery table. */
		size = BATTLE_ARENA_RECOVERY_RECORD_BYTES;
		buffer = syHeapAllocEwram(size);
		IWRAM_COPY_WORDS(buffer, gmDjinnGetRecoveryTable(DJINN_RECOVERY_PARTY),
						 size);
		{
			s32 timeout;
			s32 tries;

			list = &((DJINN_RECOVERY_TABLE *)buffer)->list;
			tries = 0;
			timeout = BATTLE_ARENA_SEND_TIMEOUT_FRAMES;
			/* Give each Djinni on the list its party member's number on the */
			/* opponent's side, and drop those of party members not sent
			 * (peerSlots */
			/* 0). */
			for(j = 0; j < list->cnt; j++) {
				list->entries[j].unitId = peerSlots[list->entries[j].unitId];
				if(list->entries[j].unitId == 0) {
					for(k = j; k < list->cnt - 1; k++) {
						list->entries[k] = list->entries[k + 1];
					}
					list->cnt--;
					j--;
				}
			}
			if((started = sySerialStartSend(buffer, size)) == -1) {
				result = started;
				break;
			}
			while(sySerialGetActiveTransfers() != 0) {
				sySynch(1);
				if(--timeout < 0 ||
				   (LinkStatusWork & BATTLE_ARENA_LINK_READY_MASK) !=
					   BATTLE_ARENA_LINK_READY_MASK) {
					if(++tries > BATTLE_ARENA_TRANSFER_RETRY_LIMIT) {
						result = -1;
						goto done;
					}
				}
			}
			sySynch(1);
			sySynch(2);
		}
	} while(0);
done:
	syHeapRelease(buffer);
	return result;
}

/* Exchanges the two parties. The parent sends first, and the child (area flag
 */
/* FLAG_BATTLE_ARENA_LINK_CHILD) receives first. */
static s32 scBattleArenaExchangePartyRecords(void) {
	s32 result;
	s32 receivedMemberCount;
	s32 receiveFirst;

	result = 0;
	/* FAKEMATCH(dead): an empty once-only loop after result's zero loads it
	 * ahead of the call. */
	do {
	} while(0);
	receiveFirst = gmFlagGet(FLAG_BATTLE_ARENA_LINK_CHILD);
	LinkExchangeState = result;
	if(!receiveFirst) {
		sySynch(5);
		result = scBattleArenaSendPartyRecords();
		if(result < 0)
			goto fail;
		sySynch(5);
		receivedMemberCount = result = scBattleArenaReceivePartyRecords();
		if(result < 0)
			goto check;
	}
	else {
		receivedMemberCount = result = scBattleArenaReceivePartyRecords();
		if(result < 0)
			goto fail;
		sySynch(10);
		result = scBattleArenaSendPartyRecords();
		if(result < 0)
			goto fail;
	}
	syFieldGameFlagSetByte(FLAG_LINK_PARTY_COUNT, receivedMemberCount);
	result = receivedMemberCount;
check:
	/* Any failure resets the link; the count received is never negative here.
	 */
	if(receivedMemberCount < 0) {
	fail:
		sySerialResetTransferState();
	}
	return result;
}

/* Stepping onto the diamond mark in the middle of the hall, cell (13, 12), */
/* once a battle is arranged starts it. For a monster battle the attendant says
 */
/* "Good luck!" and Isaac walks north through the iron gate, which lifts, into
 */
/* the arena. For a linked battle "Awaiting your opponent..." shows until the */
/* friend steps onto the mark too, and Isaac walks slowly to the gate while */
/* the two parties are traded over the cable. */
/* */
/* Before a battle is arranged, nothing happens on the mark. A lost link or a */
/* failed trade walks him back out and lets the attendant say so. */
static s32 scBattleArenaEnterBattle(void) {
	FIELD_STATE *state;
	s32 failed;
	WINDOW *window;
	s32 missedFrames;
	s32 cancelEntry;
	u8 *savedState;
	u8 *source;
	u8 *destination;
	u16 *signature;
	u32 i;

	failed = 0;
	window = 0;
	state = FieldStateWork;
	missedFrames = 0;
	if(gmFlagGet(FLAG_MONSTER_BATTLE)) {
		fdEventStart();
	}
	else {
		if(!gmFlagGet(FLAG_BATTLE_ARENA_ATTENDANT_ASKED))
			return BATTLE_ARENA_NO_RESULT;
		if(gmFlagGet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED))
			return BATTLE_ARENA_NO_RESULT;
		fdEventStart();
		gmFlagSet(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IN_CIRCLE);
		if(!scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE))
			window = mnMessageOpenWindow(MsgLobbyAwaitingOpponent, 5, 4,
										 MESSAGE_WINDOW_ZOOM);
		while(!scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE)) {
			sySynch(1);
			cancelEntry = 0;
			if(!gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED))
				cancelEntry = 1;
			if(gmFlagGet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED))
				cancelEntry = 1;
			if(!scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_IN_CIRCLE) &&
			   !scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_ASKED)) {
				if(++missedFrames > BATTLE_ARENA_PEER_WAIT_FRAMES)
					cancelEntry = 1;
			}
			else {
				missedFrames = 0;
			}
			if(cancelEntry) {
				state->raisedTrigger = BATTLE_ARENA_EVENT_DISCONNECTED;
				gmFlagSet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
				gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
				failed = 1;
				gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
				break;
			}
		}
		if(window)
			mnWindowClose(window, 1);
		sySynch(5);
	}
	/* Stop watching the opponent while the parties are traded and the battle */
	/* starts. */
	if(!failed) {
		/* HEAP_SLOT_ENEMY_UNITS is EnemyUnits, the battle's enemy */
		/* units (ENEMY_UNITS_SIZE, battle/battle_run.c). While it is */
		/* allocated, the exchange below receives the opponent's party members
		 * and */
		/* Djinn into units FIRST_ENEMY_UNIT to FIRST_ENEMY_UNIT + 3. The copy
		 * to */
		/* ActorSpriteSlots (32 KB into MapGridCellBuffer) hands them to */
		/* btRun, which copies them back. */
		savedState =
			syHeapSlotAllocEwram(HEAP_SLOT_ENEMY_UNITS, 6 * BATTLE_UNIT_SIZE);
		syCloseEvent((TASK_FUNC)scBattleArenaWatchOpponent);
		fdTilesetAnimResume(BATTLE_ARENA_GATE_ANIMATION);
		sySynch(8);
		fdTilesetAnimPause(BATTLE_ARENA_GATE_ANIMATION);
		if(gmFlagGet(FLAG_MONSTER_BATTLE)) {
			fdActorFaceActor(ACTOR_ATTENDANT,
							 MainRoutineStateWork.leaderActorId, 0);
			fdEventSetMessageId(MsgLobbyGoodLuck);
			/* "Good luck!" */
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			sySynch(45);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 184);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 168);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		}
		else {
			/* Isaac walks to the gate at a crawl while the parties are traded,
			 * and */
			/* at half pace once they are. */
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 200);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.1);
			fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 168);
			/* A failed exchange: walk back out and return to the Battle Arena,
			 */
			/* reporting the link lost (BATTLE_ARENA_EVENT_DISCONNECTED). */
			if(scBattleArenaExchangePartyRecords() < 0) {
				fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
				fdActorWalkTo(ACTOR_PARTY_LEADER, 216, 200);
				fdTilesetAnimResume(BATTLE_ARENA_GATE_ANIMATION);
				sySynch(8);
				fdTilesetAnimPause(BATTLE_ARENA_GATE_ANIMATION);
				fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
				syHeapSlotRelease(HEAP_SLOT_ENEMY_UNITS);
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
				scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
				syTaskAddFrame((TASK_FUNC)scBattleArenaWatchOpponent,
							   TASK_PASS_FRAME);
				syTaskSetFlags((TASK_FUNC)scBattleArenaWatchOpponent,
							   TASK_FLAG_KEEP_RUNNING);
				gmFlagClear(FLAG_BATTLE_ARENA_OPPONENT_ASKED);
				gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
				gmFlagClear(FLAG_BATTLE_ARENA_LINK_UP);
				gmFlagClear(FLAG_BATTLE_ARENA_ENTERING_BATTLE);
				gmFlagClear(FLAG_BATTLE_ARENA_ATTENDANT_ASKED);
				state->raisedTrigger = BATTLE_ARENA_EVENT_DISCONNECTED;
				fdEventEnd();
				return BATTLE_ARENA_NO_RESULT;
			}
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
			fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
		}
		if(gmFlagGet(FLAG_MONSTER_BATTLE)) {
			fdEventSetAfterBattleDestination(SCENE_BATTLE_ARENA,
											 BATTLE_ARENA_AFTER_MONSTER_WIN);
			fdEventSetDefeatDestination(SCENE_BATTLE_ARENA,
										BATTLE_ARENA_AFTER_MONSTER_LOSS);
		}
		else {
			fdEventSetAfterBattleDestination(SCENE_BATTLE_ARENA,
											 BATTLE_ARENA_AFTER_LINK_WIN);
			fdEventSetDefeatDestination(SCENE_BATTLE_ARENA,
										BATTLE_ARENA_AFTER_LINK_LOSS);
		}
		/* Publish EXEC only after the battle and return destinations are ready.
		 */
		/* It fills the payload's first four halfwords, where the link battle's
		 */
		/* command entry looks for it in the opponent's */
		/* (screens_battle_party_commands.c). */
		btSetType(BATTLE_TYPE_LINK);
		fdRequestBattleFromEncounterTable(1, 1);
		signature = (u16 *)SerialTransfer.payload;
		signature[0] = 'E';
		signature[1] = 'X';
		signature[2] = 'E';
		signature[3] = 'C';
		i = 0;
		source = savedState;
		destination = ActorSpriteSlots;
		for(; i < 6 * BATTLE_UNIT_SIZE; i++) {
			*destination++ = *source++;
		}
		syHeapSlotRelease(HEAP_SLOT_ENEMY_UNITS);
	}
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Talking to the attendant, the woman behind the west counter, arranges a */
/* battle. With no second Game Boy Advance on the cable she says "You do not */
/* have an opponent. Would you like to battle a monster?", and Yes sends the */
/* party to the diamond mark. With a friend linked she explains the Battle */
/* Arena once, then offers to ask the friend for a battle, or to accept the */
/* friend's request. */
static s32 scBattleArenaChooseBattleMode(void) {
	/* UB: returns no value, so its result is whatever the last call returned.
	 */

	s32 answer;
	s32 messageBase;
	s32 messageId;

	/* The attendant's lines follow "Please speak to me when you want to fight
	 * in */
	/* a battle." (MsgLobbyPleaseSpeakWhen), as quoted where each is chosen. */
	messageBase = MsgLobbyPleaseSpeakWhen;
	fdEventStart();
	fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
	if(scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE) == 0) {
		sySynch(1);
	}
	if(scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE) == 0) {
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ALONE);
		scBattleArenaRestartLink();
		if(!fdEventGetFlag(FLAG_MONSTER_BATTLE)) {
			fdEventSetMessageId(messageBase + 5);
			/* "You do not have an opponent. Would you like to battle a
			 * monster?" */
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			answer = fdEventAskYesNoWithActorAnimation(0, 0);
			/* Yes starts a monster series: FLAG_AREA_TEMP_E8's byte and */
			/* LINK_TALLY_MONSTER_STREAK start at 0. */
			if(answer == YES) {
				syFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, 0);
				gmFlagSet(FLAG_MONSTER_BATTLE);
				gmFlagClear(FLAG_LINK_BATTLE_ARRANGED);
				gmFlagClear(FLAG_LINK_BATTLE);
				gmFlagSet(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
				/* "When you finish your preparations, please step into the
				 * circle!" */
				messageId = messageBase + 7;
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK] =
					answer;
				goto message;
			}
			gmFlagClear(FLAG_MONSTER_BATTLE);
			gmFlagSet(FLAG_LINK_BATTLE);
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
			/* "Please wait until an opponent arrives, then." */
			messageId = messageBase + 6;
			goto message;
		}
		else {
			/* "When you finish your preparations, please step into the circle!"
			 */
			messageId = messageBase + 3;
			goto message;
		}
	}
	else {
		/* An opponent arrived while a monster battle waited: call it off. */
		if(fdEventGetFlag(FLAG_MONSTER_BATTLE)) {
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
			fdEventSetMessageId(MsgLobbyOpponentArrived);
			/* "Your opponent has arrived!" */
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			gmFlagClear(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
			gmFlagClear(FLAG_MONSTER_BATTLE);
		}
		if(gmFlagGet(FLAG_BATTLE_ARENA_PLAYER_APPLIED)) {
			/* "When you finish your preparations, please step into the circle!"
			 */
			messageId = messageBase + 3;
			/* The lone player's answers above show their line here too, and an
			 */
			/* accepted request below comes back to open its own. */
		message:
			fdEventSetMessageId(messageId);
		open_message:
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
		}
		else if(!gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED) &&
				!fdEventGetFlag(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED)) {
			fdEventSetMessageId(messageBase);
			/* "Please speak to me when you want to fight in a battle." */
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			gmFlagSet(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED);
		}
		else {
			gmFlagSet(FLAG_BATTLE_ARENA_ATTENDANT_EXPLAINED);
			if(gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED)) {
				/* "Your opponent has asked for a battle. Do you accept?" */
				fdEventSetMessageId(messageBase + 2);
			}
			else {
				/* "Would you like to apply?" */
				fdEventSetMessageId(messageBase + 1);
			}
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			if(fdEventAskYesNoWithActorAnimation(0, 0) == YES) {
				/* Yes, and the opponent is still there; else the link is lost.
				 */
				if(scBattleArenaPeerSlotMatches(BATTLE_ARENA_SLOT_SIGNATURE)) {
					gmFlagSet(FLAG_LINK_BATTLE);
					gmFlagSet(FLAG_LINK_BATTLE_ARRANGED);
					if(gmFlagGet(FLAG_BATTLE_ARENA_OPPONENT_ASKED)) {
						/* "When you finish your preparations, please step into
						 * the circle!" */
						fdEventSetMessageId(messageBase + 3);
					}
					else {
						/* "You have asked for a battle. We will call your name
						 * if your..." */
						fdEventSetMessageId(messageBase + 4);
					}
					scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ASKED);
					gmFlagSet(FLAG_BATTLE_ARENA_PLAYER_APPLIED);
					goto open_message;
				}
				else {
					gmFlagSet(FLAG_BATTLE_ARENA_LINK_DISCONNECTED);
				}
			}
			else {
				fdEventSetMessageId(messageBase);
				/* "Please speak to me when you want to fight in a battle." */
				fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			}
		}
	}
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Talking to the muscleman behind the east counter gives the linked battles' */
/* record. From the front of the counter he tells the linked battles won in */
/* all; from the side, the ones won in a row, and points to the board over the
 */
/* gate for the best run. Before any linked battle he only says he will tell */
/* the player once the linked finals are over. */
static s32 scBattleArenaTellLinkedWins(s32 actorId) {
	u32 facing = fdActorGet(ACTOR_PARTY_LEADER)->facing;
	u16 *tally;
	s32 messageBase;

	fdEventStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if(FACING_IS_NORTH(facing)) {
		messageBase = MsgLobbyLooksLikeFirst;
		tally = &MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_WINS];
		if(*tally != 0) {
			mnMessageArgumentClearAll();
			mnMessageArgumentPush(*tally, MESSAGE_ARGUMENT_NUMBER);
			fdEventSetMessageId(messageBase + 1);
			/* "You've won ... Linked Battle. Keep up the good work!" */
			fdEventShowMessage(actorId, 0);
		}
		else {
			fdEventSetMessageId(MsgLobbyTellTotalWins);
			/* "I will tell you your total wins when you finish the linked..."
			 */
			fdEventShowMessage(actorId, 0);
		}
	}
	else {
		messageBase = MsgLobbyLooksLikeFirstTime;
		tally = &MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK];
		if(*tally != 0) {
			mnMessageArgumentClearAll();
			mnMessageArgumentPush(*tally, MESSAGE_ARGUMENT_NUMBER);
			fdEventSetMessageId(messageBase + 1);
			/* "You've won ... Linked Battle. Check the board on the wall..." */
			fdEventShowMessage(actorId, 0);
		}
		else {
			fdEventSetMessageId(MsgLobbyTellConsecutiveWins);
			/* "I will tell you your consecutive wins when you finish the..." */
			fdEventShowMessage(actorId, 0);
		}
	}
#ifdef UBFIX
	return 0;
#endif
}

/* A developers' leftover: the white-bearded man in green stands south of the */
/* hall's wall, where the player can never walk. Talking to him shows "Choose */
/* your parameters here." and a number picked with Left and Right; A loads that
 */
/* numbered set of levels and equipment for testing (`btDebugParmSetLoad`) */
/* and B gives up. The muscleman behind the east counter answers: "The settings
 */
/* are made. The rest is up to you.", "There's no such parameter. Ask Mr. */
/* Sugimoto." or "What is this, just a tease?". */
static s32 scBattleArenaChooseDebugParmSet(s32 actorId) {
	WINDOW *window;
	s32 setIndex;
	s32 displayedSet;
	s32 loadResult;

	fdEventStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	fdEventSetMessageId(MsgLobbyChooseParameters);
	/* "Choose your parameters here." */
	fdEventShowMessage(actorId, 0);
	window = mnWindowCreate(0, 0, 6, 4, WINDOW_FLAG_INSTANT);
	setIndex = 0;
	displayedSet = -1;
	for(;;) {
		if(setIndex != displayedSet) {
			mnWindowSprPrepareForRedraw(window);
			mnTextDrawNum(setIndex, 3, window, 0, 0);
			displayedSet = setIndex;
		}
		if(KeysRepeat & KEY_LEFT) {
			setIndex--;
		}
		if(KeysRepeat & KEY_RIGHT) {
			setIndex++;
		}
		if(setIndex < 0) {
			setIndex = 0;
		}
		if(KeysPressed & KEY_A) {
			break;
		}
		if(KeysPressed & KEY_B) {
			setIndex = -1;
			break;
		}
		sySynch(1);
	}
	mnWindowClose(window, 1);
	if(setIndex >= 0) {
		loadResult = btDebugParmSetLoad(setIndex);
	}
	if(setIndex < 0) {
		fdEventSetMessageId(MsgLobbyTease);
		/* "What is this, just a tease?" */
		fdEventShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	else if(loadResult != 0) {
		fdEventSetMessageId(MsgLobbyNoParameterAsk);
		/* "There's no such parameter. Ask Mr. Sugimoto." */
		fdEventShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	else {
		fdEventSetMessageId(MsgLobbySettingsMadeRest);
		/* "The settings are made. The rest is up to you." */
		fdEventShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	sySynch(10);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Talking to the healer in the tall hat, in the room up the east stairs, */
/* offers a Sanctum's services, the same as in any town (`mnSanctumRun`). */
static s32 scBattleArenaOpenSanctum(s32 actorId) {
	fdEventStart();
	mnSanctumRun(actorId);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Returns 1 when character memberId is among the first three party members */
/* while a linked battle is arranged (FLAG_LINK_BATTLE_ARRANGED), or its first
 */
/* four otherwise. Returns 0 when not. */
static s32 scBattleArenaPartyContains(s32 memberId) {
	s32 count;
	s32 memberLimit;
	s32 i;

	count = gmPartyCountMembers();
	memberLimit = 3;
	if(gmFlagGet(FLAG_LINK_BATTLE_ARRANGED) == 0) {
		memberLimit = 4;
	}
	if(count > memberLimit) {
		count = memberLimit;
	}
	for(i = 0; i < count; i++) {
		if(MainRoutineStateWork.party[i] == BATTLE_UNIT_LIST_END) {
			return 0;
		}
		if(MainRoutineStateWork.party[i] == memberId) {
			return 1;
		}
	}
	return 0;
}

/* UNUSED: an empty function the Battle Arena's code keeps. */
UNUSED static void scBattleArenaEmpty(void) {}

/* Talking to Garet, Ivan, Mia or Jenna in the room up the east stairs gets a */
/* line about the coming battle that depends on who will fight it. Those in the
 */
/* party cheer ("I can't wait, Isaac! Let's go get 'em!"), one left out asks */
/* whether Isaac means to fight alone or to sit out, and after a linked battle
 */
/* each speaks of the win or the loss, whether they fought or watched. */
/* */
/* The linked battle takes only the first three of the party */
/* (`scBattleArenaPartyContains`). The line is the speaker's own in each list,
 */
/* one message after the other for Garet, Ivan, Mia and Jenna. */
static s32 scBattleArenaTalkByProgress(s32 actorId) {
	s32 messageBase = MsgLobbyCantWaitLets;
	s32 isaacPresent = scBattleArenaPartyContains(ISAAC);
	s32 speakerPresent = scBattleArenaPartyContains(actorId);

	fdEventStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if(gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED)) {
		/* Isaac's flag is tested and the result dropped; messageBase first
		 * holds */
		/* whether the speaker fought (FLAG_BATTLE_ARENA_IN_PARTY_FIRST + actor
		 * id). */
		fdEventGetFlag(FLAG_BATTLE_ARENA_IN_PARTY_FIRST);
		messageBase = gmFlagGet(actorId + FLAG_BATTLE_ARENA_IN_PARTY_FIRST);
		if(gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_WON)) {
			if(messageBase)
				messageBase = MsgLobbyWonToldCountOn;
			else
				messageBase = MsgLobbyWonWithoutWell;
		}
		else {
			if(messageBase)
				messageBase = MsgLobbyNotFaultLost;
			else
				messageBase = MsgLobbyKnewLostBecause;
		}
	}
	else if(isaacPresent != 0) {
		if(speakerPresent == 0)
			messageBase = MsgLobbyGoingFightAlone;
	}
	else {
		messageBase = MsgLobbyTryingGetAway;
	}
	fdEventSetMessageId(messageBase + actorId - 1);
	fdEventShowMessage(actorId, 0);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Walking south out of the hall, down the passage he came in by, cell (13, */
/* 19), leaves the Battle Arena and goes back to the File Screen. */
/* */
/* The link is stopped and the sound's normal echo restored first. */
static s32 scBattleArenaLeaveArena(void) {
	sySerialRemoveIrqHandlers();
	sySndLoadReverbCode(SOUND_REVERB_NORMAL);
	fdEventSetDestination(SCENE_FILE_SCREEN, FILE_SCREEN_FROM_SANCTUM);
#ifdef UBFIX
	return 0;
#endif
}

/* UB: the handlers below return no value, so their result is whatever */
/* fdEventEnd returned. */

/* Talking to one of three visitors speaks of the Battle Arena: the swordsman */
/* in the room up the west stairs is a traveling warrior who warns not to */
/* underestimate the monsters, the old man with a cane there says up to three */
/* allies fight in the linked finals, and the old woman beside him wonders if */
/* this is the arena the old man dreamed of. After a linked battle each praises
 */
/* the win or consoles the loss. */
static s32 scBattleArenaTalkToVisitor(s32 actorId) {
	s32 message;
	s32 step = 0;

	fdEventStart();
	switch(actorId) {
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
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if(gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED))
		step = 2 - (gmFlagGet(FLAG_BATTLE_ARENA_LINK_BATTLE_WON) != 0);
	fdEventSetMessageId(message + step);
	fdEventOpenMessage(actorId, 0);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Talking to the fair-haired boy on the west side of the hall gives two tips */
/* in turn. First, up to three allies can fight in the linked finals, so with */
/* four in the party the rightmost has to pass; then, the party's order can be
 */
/* changed in the status screen. */
static s32 scBattleArenaExplainPartyOrder(s32 actorId) {
	s32 message;

	fdEventStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if(!fdEventGetFlag(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT)) {
		if(gmPartyCountMembers() <= 3)
			message = MsgLobbyThreeAlliesFightLinkedFinals;
		else
			message = MsgLobbyThreeAlliesFightLinked;
		gmFlagSet(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT);
	}
	else {
		message = MsgLobbyChangeOrderParty;
		gmFlagClear(FLAG_BATTLE_ARENA_PARTY_ORDER_NEXT);
	}
	fdEventSetMessageId(message);
	/* "You can change the order of your party in the status screen...." */
	fdEventOpenMessage(actorId, 0);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

/* Talking to the old man with a cane beside the gate gives the most monster */
/* battles won in a row ("You've won 3 consecutive Monster Battles."), or */
/* before any, that he will be counting the player's victories. */
static s32 scBattleArenaReportMonsterRecord(s32 actorId) {

	fdEventStart();
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	if(MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] != 0) {
		mnMessageArgumentPush(
			MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK],
			MESSAGE_ARGUMENT_NUMBER);
		fdEventSetMessageId(MsgLobbyWonNumberConsecutive);
	}
	else {
		fdEventSetMessageId(MsgLobbyCantWaitSee);
	}
	fdEventOpenMessage(actorId, 0);
	fdEventEnd();
#ifdef UBFIX
	return 0;
#endif
}

u8 *scGetTriggers(void) { return (u8 *)BattleArenaTriggers; }

/* Saves the file after a linked battle, between "Saving battle results." and */
/* "Battle results were saved.". */
static s32 scBattleArenaSaveBattleResults(void) {
	WINDOW *window;

	sySndPlay(MUS_SAVED);
	window = mnMessageOpenWindow(MsgLobbySavingBattleResults, 5, 4,
								 MESSAGE_WINDOW_ZOOM);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	mnSaveWriteLinkTallies();
	mnWindowClose(window, 1);
	sySynch(1);
	window = mnMessageOpenWindow(MsgLobbyBattleResultsSaved, 5, 4,
								 MESSAGE_WINDOW_ZOOM);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	mnWindowClose(window, 1);
#ifdef UBFIX
	return 0;
#endif
}

/* Saves the file after a series of monster battles that set a new record, */
/* between "Saving Monster Battle results." and "Monster Battle results were */
/* saved.". It is scBattleArenaSaveBattleResults with the Monster Battle's two
 */
/* lines; the game keeps each save as a function of its own, with no */
/* parameter for its messages. */
static s32 scBattleArenaSaveMonsterBattleResults(void) {
	WINDOW *window;

	sySndPlay(MUS_SAVED);
	window = mnMessageOpenWindow(MsgLobbySavingMonsterBattle, 5, 4,
								 MESSAGE_WINDOW_ZOOM);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	mnSaveWriteLinkTallies();
	mnWindowClose(window, 1);
	sySynch(1);
	window = mnMessageOpenWindow(MsgLobbyMonsterBattleResults, 5, 4,
								 MESSAGE_WINDOW_ZOOM);
	while(mnMessageAllTyped() == 0)
		sySynch(1);
	mnWindowClose(window, 1);
#ifdef UBFIX
	return 0;
#endif
}

/* Shows value, at most WALL_BOARD_MAX, on the board over the gate, cells (14,
 */
/* 8) to (16, 8). The digits' pictures stand in map column 27, digit d in row */
/* d. */
static s32 scBattleArenaDrawThreeDigitValue(s32 value) {
	s32 digit;

	if(value > WALL_BOARD_MAX)
		value = WALL_BOARD_MAX;
	for(digit = 0; digit < WALL_BOARD_DIGITS; digit++) {
		fdMapGridCopyCellsTo(27, value % 10, 16 - digit, 8, 1, 1);
		value = value / 10;
	}
	fdMapGridCenterOnFollowedPoint();
#ifdef UBFIX
	return 0;
#endif
}

/* Each time the party comes into the hall, from the File Screen or back from */
/* the arena, the battle that brought it here is settled. After a monster */
/* battle won, the attendant offers the next monster ("Not bad! Would you like
 */
/* to fight the next monster?"), with ever higher praise; No, or a loss, ends */
/* the series with the number won, and a new record is saved. A linked battle */
/* is counted, the board over the gate updated and the file saved. From the */
/* File Screen the link is started. */
s32 scInit(void) {
	s32 i;
	FIELD_STATE *state;
	void (*func)(void);

	BattleArenaCallFrames = 0;
	sBattleArenaWaitFrames = 0;
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	sySndLoadReverbCode(SOUND_REVERB_NORMAL);
	scBattleArenaDrawThreeDigitValue(
		MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
	fdMapGridCopyCellAttributes(11, 11, 1, 1, 13, 10);
	scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	sySynch(1);
	fdTilesetAnimPause(BATTLE_ARENA_GATE_ANIMATION);

	/* The payload's halfwords 4 to 7 hold the link battle's phase word */
	/* ("VSST", "EDVS", "TUrn"); the Battle Arena puts TALK there. */
	{
		u16 *name = (u16 *)SerialTransfer.payload;

		name[4] = 'T';
		name[5] = 'A';
		name[6] = 'L';
		name[7] = 'K';
	}
	/* Note who is in the battle party, for the party members' lines. */
	for(i = 0; i < 8; i++) {
		gmFlagClear(FLAG_BATTLE_ARENA_IN_PARTY_FIRST + i);
		if(scBattleArenaPartyContains(i)) {
			gmFlagSet(FLAG_BATTLE_ARENA_IN_PARTY_FIRST + i);
		}
	}
	/* A monster battle won: offer the next one. */
	if(MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_MONSTER_WIN) {
		s32 wins;
		s32 messageOffset;
		s32 v;

		fdEventStart();
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ALONE);
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_WINS]++;
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK]++;
		/* The offer has two lines a level (FLAG_MONSTER_BATTLE_WINS, -1 when a
		 */
		/* series starts), the next monster's line going no further than */
		/* MONSTER_OFFER_LAST_NEXT. FLAG_AREA_TEMP_E8's byte counts the
		 * victories 0 */
		/* to 2; the third raises the level and takes the line after, which */
		/* promises a stronger monster. */
		wins = (s8)syFieldGameFlagGetByte(FLAG_MONSTER_BATTLE_WINS);
		messageOffset = wins * 2 + 2;
		if(messageOffset > MONSTER_OFFER_LAST_NEXT) {
			messageOffset = MONSTER_OFFER_LAST_NEXT;
		}
		v = syFieldGameFlagGetByte(FLAG_AREA_TEMP_E8);
		if(v == 2) {
			syFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, 0);
			wins++;
			messageOffset++;
		}
		else {
			syFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, v + 1);
		}
		fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId,
						 0);
		fdEventSetMessageId(MsgLobbyNotBadNextMonster + messageOffset);
		fdEventOpenMessage(ACTOR_ATTENDANT, 0);
		if(fdEventAskYesNoWithActorAnimation(0, 0) == YES) {
			if(wins > LEVEL_BONUS_MAX) {
				wins = LEVEL_BONUS_MAX;
			}
			syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, wins);
		}
		else {
			/* No: the series ends, and a new best run is saved. */
			gmFlagClear(FLAG_MONSTER_BATTLE);
			syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
			mnMessageArgumentPush(
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
				MESSAGE_ARGUMENT_NUMBER);
			if(MainRoutineStateWork
				   .linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
			   MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
				MainRoutineStateWork
					.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
					MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
				fdEventSetMessageId(MsgLobbyWonNumberBattleBroke);
				/* "You won ... battle. You broke the record!" */
				fdEventOpenMessage(ACTOR_ATTENDANT, 0);
				scBattleArenaSaveMonsterBattleResults();
			}
			else {
				fdEventSetMessageId(MsgLobbyWonNumberBattle);
				/* "You won ... battle. Please speak to me when you wish to..."
				 */
				fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			}
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		}
		fdEventEnd();
		/* A monster battle lost: the series ends, and a new best run is saved.
		 */
	}
	else if(MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_MONSTER_LOSS) {
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_LOSSES]++;
		fdEventStart();
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_ALONE);
		fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId,
						 0);
		mnMessageArgumentPush(
			MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
			MESSAGE_ARGUMENT_NUMBER);
		if(MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
		   MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
			MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
			fdEventSetMessageId(MsgLobbyWonNumberBattleBroke);
			/* "You won ... battle. You broke the record!" */
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
			scBattleArenaSaveMonsterBattleResults();
		}
		else {
			fdEventSetMessageId(MsgLobbyWonNumberBattleTold);
			/* "You won ... battle. I told you our monsters were tough!..." */
			fdEventOpenMessage(ACTOR_ATTENDANT, 0);
		}
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = 0;
		gmFlagClear(FLAG_MONSTER_BATTLE);
		syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		fdEventEnd();
		/* A linked battle won: count the win and the run; a longer run is the
		 */
		/* board's new record. Saved. */
	}
	else if(MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_LINK_WIN) {
		s32 v;

		fdEventStart();
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
		/* The battle broke off instead (btRun set FLAG_AREA_TEMP_E8): report */
		/* the link lost and restart it. */
		if(fdEventGetFlag(FLAG_AREA_TEMP_E8)) {
			state = FieldStateWork;
			gmFlagClear(FLAG_AREA_TEMP_E8);
			state->raisedTrigger = BATTLE_ARENA_EVENT_DISCONNECTED;
			gmFlagClear(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
			sySynch(20);
			scBattleArenaRestartLink();
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
			scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
		}
		else {
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_WINS]++;
			v = MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] + 1;
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
			if(MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] <
			   (u16)v)
				MainRoutineStateWork
					.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] = v;
			scBattleArenaDrawThreeDigitValue(
				MainRoutineStateWork
					.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
			scBattleArenaSaveBattleResults();
			gmFlagSet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
			gmFlagSet(FLAG_BATTLE_ARENA_LINK_BATTLE_WON);
		}
		fdEventEnd();
		/* A linked battle lost: count the loss and end the run. Saved. */
	}
	else if(MainRoutineStateWork.entrance == BATTLE_ARENA_AFTER_LINK_LOSS) {
		s32 v;

		fdEventStart();
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
		if((v = gmFlagGet(FLAG_MONSTER_BATTLE)) == 0) {
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_LOSSES]++;
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
			scBattleArenaSaveBattleResults();
		}
		gmFlagSet(FLAG_BATTLE_ARENA_LINK_BATTLE_ENDED);
		gmFlagClear(FLAG_BATTLE_ARENA_LINK_BATTLE_WON);
		fdEventEnd();
		/* Arriving from the File Screen: start the link, clear */
		/* FLAG_LINK_BATTLE_ARRANGED and reset the monster level. Idle sleep is
		 */
		/* turned off here, with a note when it was on. */
	}
	else {
		sySerialInit();
		gmFlagClear(FLAG_LINK_BATTLE_ARRANGED);
		syFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
		if(MainRoutineStateWork.idleSleepEnabled != 0) {
			fdEventStart();
			fdEventStartScrTransition();
			fdEventWaitTransitionFrames();
			fdActorFaceActor(ACTOR_ATTENDANT,
							 MainRoutineStateWork.leaderActorId, 0);
			fdEventSetMessageId(MsgLobbyNoteCantUse);
			/* "Note: you can't use the sleep function in battle stages!" */
			fdEventShowMessage(ACTOR_ATTENDANT, 0);
			fdEventEnd();
		}
		MainRoutineStateWork.idleSleepEnabled = 0;
		IdleSleepEnabled = 0;
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_SIGNATURE);
		scBattleArenaWriteSlotValue(BATTLE_ARENA_SLOT_IDLE);
	}
	func = (TASK_FUNC)scBattleArenaWatchOpponent;
	syTaskAddFrame(func, TASK_PASS_FRAME);
	syTaskSetFlags(func, TASK_FLAG_KEEP_RUNNING);
	if(MainRoutineStateWork.entrance != BATTLE_ARENA_AFTER_MONSTER_WIN ||
	   !gmFlagGet(FLAG_MONSTER_BATTLE)) {
		gmPartyRestoreHpPp(1);
		btDjinnRecoveryCountDownTwenty();
	}
	return 0;
}

/* clang-format off */
/* The payload word each slot of BattleArena_SlotValues goes in. 0 is the */
/* signature and 1 the state. The last two entries have no slot. */
u8 BattleArena_SlotColumns[] = {
    0, 1, 1, 1, 1, 0, 0, 0,
};

/* Where the party appears. BATTLE_ARENA_FROM_FILE_SCREEN is from the File */
/* Screen, and the BATTLE_ARENA_AFTER_ ones are after a battle */
/* (scBattleArenaEnterBattle). */
SCENE_ENTRANCE BattleArenaEntrances[] = {
    { BATTLE_ARENA_AFTER_LINK_LOSS, CONDITION_ALWAYS, 128, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_AFTER_LINK_WIN, CONDITION_ALWAYS, 352, 0, 148, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_AFTER_MONSTER_WIN, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_AFTER_MONSTER_LOSS, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0 },
    { BATTLE_ARENA_FROM_FILE_SCREEN, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0 },
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 BattleArenaExits[] = {
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
/* clang-format on */

TRIGGER BattleArenaTriggers[] = {
	{TRIGGER_TALK, GARET, CONDITION_ALWAYS, EVENT(scBattleArenaTalkByProgress)},
	{TRIGGER_TALK, IVAN, CONDITION_ALWAYS, EVENT(scBattleArenaTalkByProgress)},
	{TRIGGER_TALK, MIA, CONDITION_ALWAYS, EVENT(scBattleArenaTalkByProgress)},
	{TRIGGER_TALK, JENNA, CONDITION_ALWAYS, EVENT(scBattleArenaTalkByProgress)},
	{TRIGGER_TALK, ACTOR_ATTENDANT, CONDITION_ALWAYS,
	 EVENT(scBattleArenaChooseBattleMode)},
	{TRIGGER_TALK, ACTOR_LINKED_WINS_TELLER, CONDITION_ALWAYS,
	 EVENT(scBattleArenaTellLinkedWins)},
	{TRIGGER_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
	 EVENT(scBattleArenaOpenSanctum)},
	{TRIGGER_TALK, ACTOR_GREETER, CONDITION_ALWAYS,
	 MSGNO(MsgFieldWelcomeToBattleArenaWherePeople)},
	{TRIGGER_TALK, ACTOR_TRAVELING_WARRIOR, CONDITION_ALWAYS,
	 EVENT(scBattleArenaTalkToVisitor)},
	{TRIGGER_TALK, ACTOR_FINALS_VISITOR, CONDITION_ALWAYS,
	 EVENT(scBattleArenaTalkToVisitor)},
	{TRIGGER_TALK, ACTOR_OLD_MANS_ARENA_VISITOR, CONDITION_ALWAYS,
	 EVENT(scBattleArenaTalkToVisitor)},
	{TRIGGER_TALK, ACTOR_DEBUG_PARAMETERS_MAN, CONDITION_ALWAYS,
	 EVENT(scBattleArenaChooseDebugParmSet)},
	{TRIGGER_TALK, ACTOR_PARTY_ORDER_VISITOR, CONDITION_ALWAYS,
	 EVENT(scBattleArenaExplainPartyOrder)},
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
	 EVENT(scBattleArenaReportMonsterRecord)},
	{TRIGGER_TALK, ACTOR_EDGY_VISITOR, CONDITION_ALWAYS,
	 MSGNO(MsgFieldAllTheWarriorsAreImpressiveSo)},
	{TRIGGER_TALK, ACTOR_WARY_VISITOR, CONDITION_ALWAYS,
	 MSGNO(MsgFieldICouldEnterButIdJust)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BATTLE_ARENA_CIRCLE, CONDITION_ALWAYS,
	 EVENT(scBattleArenaEnterBattle)},
	{TRIGGER_STEP, CELL_BEHAVIOR_BATTLE_ARENA_WAY_OUT, CONDITION_ALWAYS,
	 EVENT(scBattleArenaLeaveArena)},
	{TRIGGER_RAISED, BATTLE_ARENA_EVENT_ASK_TO_PARTICIPATE, CONDITION_ALWAYS,
	 EVENT(scBattleArenaAskToParticipate)},
	{TRIGGER_RAISED, BATTLE_ARENA_EVENT_DISCONNECTED, CONDITION_ALWAYS,
	 EVENT(scBattleArenaReportLinkDisconnected)},
	{TRIGGER_TABLE_END},
};
