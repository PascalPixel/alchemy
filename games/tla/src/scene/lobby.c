// The Battle Arena's lobby, The Broken Seal's arena reworked, reached from the File Screen's Battle
// mode. The party waits in a stone hall: the attendant stands behind the counter left of the arched
// door to the arena, a strongman behind the counter right of it keeps the tally of linked wins, the
// board above the door shows in three digits the most won in a row, and a healer waits on the
// carpeted floor above. With a second Game Boy Advance linked, the two players step onto the
// diamond mark before the arched door, trade their parties and fight; alone, the attendant offers
// Monster Battles. Holding L or R while talking to the woman who likes her men macho opens a song
// request, and a few visitors stand below the hall, out of reach: among them a developer's helper
// who loads debug parameters.

#include "battle/battle.h"
#include "window/window.h"
#include "field/field_state.h"
#include "sound/sound_driver_symbols.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "macros.h"
#include "field/script_inline.h"
#include "text/msg_ids.h"
#include "shop/shop.h"

// A MainRoutineStateWork.party byte with no member in it, all ones.
#define PARTY_SLOT_EMPTY 255

// The map's tileset animation of the iron gate in the north wall, run for eight ticks as the party
// walks to it from the diamond mark.
enum
{
	BATTLE_ARENA_GATE_ANIMATION = 5
};

// The slot a linked battle is requested with, in ENCOUNTER_ZONE_VENUS_DJINN: Granite's; the battle
// takes its enemies from the opponent's units exchanged before it.
enum
{
	LINK_BATTLE_ENCOUNTER_SLOT = 1
};

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

void               syGameFlagSetByte(Sint32 flag, Sint32 value);

// Stores a byte in the eight game flags from `flags`.
static inline void scFieldGameFlagSetByte(Sint32 flag, Sint32 value)
{
	syGameFlagSetByte(flag, value);
}

// Restarts the link with interrupts locked.
void scLinkLobbyInitSerial(void)
{
	Uint32 saved;

	IRQ_LOCK(saved);
	sySerialRemoveIRQHandlers();
	sySerialInit();
	IRQ_UNLOCK(saved);
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

// The lobby's people, by the rows of ActorPlacements, as The Broken Seal's Battle Arena keeps them
// where it has the same ones; the party's rows go to their party slots. The fumbling visitor, the
// debug helper, the visitor who wins and loses, the one who tells the Monster Battle rules and the
// song request taker stand at z 376 to 384, below the hall's south wall and past the camera's box,
// where the player can neither walk nor see them.
typedef enum LINK_LOBBY_ACTOR_ID
{
	ACTOR_ATTENDANT = ACTOR_FIRST_PLACED, // arranges every battle
	ACTOR_LINKED_WINS_TELLER,             // reports the linked battles won
	ACTOR_HEALER,                         // opens a Sanctum's services
	ACTOR_GREETER,                        // "Welcome to the Battle Arena, where..."
	// "Hm... It comes in like this... Then you do this, and... Whoa, that's not right..."
	ACTOR_PRACTICING_VISITOR,
	// "You look a little scrawny...", then "I'll be rooting..."
	ACTOR_ROOTING_VISITOR,
	// "I like my men macho"; with L or R held, takes a song request.
	ACTOR_MACHO_LOVING_VISITOR,
	ACTOR_DEBUG_HELPER, // loads a debug parameter set
	// Explains the linked finals' limit of three allies, then the party order.
	ACTOR_PARTY_ORDER_VISITOR,
	// "In the linked finals, you can't delay after your opponent selects a move."
	ACTOR_COMMAND_TIME_VISITOR,
	// "So you win against one guy, and then you lose against the next."
	ACTOR_WIN_AND_LOSE_VISITOR,
	// "You could use Psynergy, Djinn, even summons and items!"
	ACTOR_STRATEGY_VISITOR,
	ACTOR_WARY_VISITOR, // "Should I enter or not? I'm so stressed..."
	ACTOR_MONSTER_RULES_VISITOR,
	// reports the most Monster Battles won in a row
	ACTOR_MONSTER_WINS_TELLER,
	// "Sometimes, your class changes when you unleash a Djinni."
	ACTOR_CLASS_CHANGE_VISITOR,
	ACTOR_SONG_REQUEST_VISITOR, // "You have a song request?"
	// "I don't have the guts to fight in the battle arena."
	ACTOR_GUTLESS_VISITOR
} LINK_LOBBY_ACTOR_ID;

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

// One linked console's published words, as the link exchange last received them: its signature,
// then its state (LinkLobbySlotColumns gives each LOBBY_SLOT's word).
typedef struct LINK_PEER
{
	Sint32 values[6];
} LINK_PEER;

// The words of LinkLobbySlotValues, by slot, as The Broken Seal's Battle Arena numbers them. They
// are the signature, then the state a console publishes.
typedef enum LOBBY_SLOT
{
	LOBBY_SLOT_SIGNATURE = 0,
	LOBBY_SLOT_ASKED     = 1, // has asked for a battle
	LOBBY_SLOT_IN_CIRCLE = 2, // stands in the circle
	LOBBY_SLOT_UNUSED    = 3,
	LOBBY_SLOT_IDLE      = 4,
	// Replaces the signature when no partner answers the attendant and during a monster series.
	LOBBY_SLOT_ALONE     = 5
} LOBBY_SLOT;

// The hall's metatiles, as map.txt has them.
typedef enum LOBBY_METATILE_BEHAVIOR
{
	// The hall's south doorway, metatile (13, 18): the way out.
	METATILE_BEHAVIOR_WAY_OUT    = 1,
	// The diamond mark before the arched door to the arena, metatile (13, 11).
	METATILE_BEHAVIOR_ARENA_MARK = 10
} LOBBY_METATILE_BEHAVIOR;

// The lobby's raised cues (FIELD_STATE.raisedCue).
typedef enum LOBBY_RAISED_CUE
{
	LOBBY_CUE_NONE         = 0,
	LOBBY_CUE_PROMPT       = 1, // scLinkLobbyPromptToParticipate
	LOBBY_CUE_DISCONNECTED = 2  // scLinkLobbyReportLinkDisconnected
} LOBBY_RAISED_CUE;

// Ticks without a link (scLinkLobbyPollPeerReady) or without the partner's request
// (scLinkLobbyEnterBattle) before the lobby gives the partner up.
#define LOBBY_PEER_WAIT_TICKS                25

// Ticks, five seconds, before the attendant asks the player again to step into the circle.
#define LOBBY_PROMPT_REPEAT_TICKS            300

// The attendant has asked the player to take part. Every LOBBY_PROMPT_REPEAT_TICKS calls of
// scLinkLobbyPromptToParticipate clear it so she asks again.
#define FLAG_LINK_LOBBY_ATTENDANT_ASKED      FLAG_SCENE_TEMP_00
// The linked partner has asked for a battle. Its signature matches and it stands asked or in the
// circle.
#define FLAG_LINK_LOBBY_PARTNER_ASKED        FLAG_SCENE_TEMP_01
// The player has applied for a battle ("Would you like to apply?").
#define FLAG_LINK_LOBBY_PLAYER_APPLIED       FLAG_SCENE_TEMP_02
// The party is on its way into the battle.
#define FLAG_LINK_LOBBY_ENTERING_BATTLE      FLAG_SCENE_TEMP_03
// ACTOR_PARTY_ORDER_VISITOR has given the linked finals' limit of three allies, so the next talk
// explains how to change the party's order.
#define FLAG_LINK_LOBBY_PARTY_ORDER_NEXT     FLAG_SCENE_TEMP_04
// The link was lost and the attendant has yet to say so.
#define FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED FLAG_SCENE_TEMP_05
// A visitor has spoken once this visit, so the rooting visitor goes on to "I'll be rooting..."
// (scLinkLobbyTalkToVisitor).
#define FLAG_LINK_LOBBY_VISITOR_SPOKE        FLAG_SCENE_TEMP_06
// The first of eight flags, one per character from Isaac, each set while that character is in the
// battle party (scLinkLobbyInit).
#define FLAG_LINK_LOBBY_IN_PARTY_FIRST       FLAG_SCENE_TEMP_F0
// The attendant has explained the Battle Arena once. Talking to her again offers to apply.
#define FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED  FLAG_AREA_TEMP_00
// This console is the link's child (multiplayer id 1), which receives the parties first.
#define FLAG_LINK_LOBBY_LINK_CHILD           FLAG_AREA_TEMP_02
// The link is up. Both consoles were heard (LOBBY_LINK_READY_MASK).
#define FLAG_LINK_LOBBY_LINK_UP              FLAG_AREA_TEMP_03
// A linked battle has ended since the leader last walked south of z 224, so the party and the
// visitors speak of it.
#define FLAG_LINK_LOBBY_LINK_BATTLE_ENDED    FLAG_AREA_TEMP_04
// The last linked battle was won.
#define FLAG_LINK_LOBBY_LINK_BATTLE_WON      FLAG_AREA_TEMP_05

// LinkStatusWork bits 0 and 1: both consoles were heard this tick.
#define LOBBY_LINK_READY_MASK                3

// The highest level bonus a monster series reaches (FLAG_MONSTER_BATTLE_WINS, raised by every third
// win in a row), which battle_formation.c adds to the next monster's level over the party's
// average.
enum
{
	LEVEL_BONUS_MAX = 90
};

// The attendant's last offer of the next monster after a monster battle won, "Unbelievable!!! Would
// you like to fight the next monster?" (MsgLobbyNotBadWouldYouLike + 14); each line before it
// praises less, and the one after it offers a stronger monster.
enum
{
	MONSTER_OFFER_UNBELIEVABLE = 14
};

// The lobby's entrances after a battle, which no exit leads to: back from a monster battle won or
// lost, and from a linked battle won (or broken off) or lost.
enum
{
	LOBBY_AFTER_MONSTER_WIN  = 8,
	LOBBY_AFTER_MONSTER_LOSS = 9,
	LOBBY_AFTER_LINK_WIN     = 10,
	LOBBY_AFTER_LINK_LOSS    = 11
};

// Returns 1 when the partner's payload holds slot's word in slot's column, 0 when it does not or
// there is no link.
Bool scLinkLobbyPeerSlotMatches(Sint32 slot)
{
	Sint32 id = -1;

	if ((LinkStatusWork & LOBBY_LINK_READY_MASK) == LOBBY_LINK_READY_MASK)
	{
		// 0 for the parent, 1 for the child.
		id = SIO_ID_OF(REG_SIOCNT);
		fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_UP);
	}
	else
	{
		fdScriptClearFlag(FLAG_LINK_LOBBY_LINK_UP);
	}
	// At most one pass: a match returns 1, anything else breaks out.
	while (id >= 0 && fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_UP))
	{
		const Sint32 *expected = &LinkLobbySlotValues[slot];

		if (id != 0)
			fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_CHILD);
		else
			fdScriptClearFlag(FLAG_LINK_LOBBY_LINK_CHILD);
		{
			LINK_PEER *row =
				(LINK_PEER *)LinkPeerSignatures[fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_CHILD) ^ 1];

			if (row->values[LinkLobbySlotColumns[slot]] == *expected)
				return TRUE;
			break;
		}
	}
	return FALSE;
}

// Publishes slot's word in its column of this console's link payload, where the partner's
// scLinkLobbyPeerSlotMatches reads it.
void scLinkLobbyWriteSlotValue(Sint32 slot)
{
	Sint32       *dest               = (Sint32 *)SerialTransfer.payload;
	const Sint32 *src                = &LinkLobbySlotValues[slot];
	dest[LinkLobbySlotColumns[slot]] = *src;
}

Sint32 LinkLobbyWaitTicks BSS_DATA; // ticks without the partner
Sint32 LinkLobbyCallTicks BSS_DATA;

// The lobby's event each tick: counts ticks without the opponent, clears the peer signatures after
// 25, and raises the script cue when both sides are ready or the link is lost. Returns whether the
// peer answered.
Sint32                    scLinkLobbyPollPeerReady(void)
{
	FIELD_STATE *state;
	Bool         result;
	Sint32       i;
	Sint32       set;
	FIXED        z;

	state  = RamHeapSlots->fieldState;
	result = TRUE;
	z      = fdActorGetById(ACTOR_FELIX)->z.fixed;

	if (z > toFIXED(224))
		fdScriptClearFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
	if (state->raisedCue != LOBBY_CUE_DISCONNECTED)
	{
		scLinkLobbyPeerSlotMatches(LOBBY_SLOT_SIGNATURE);
		set = fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_UP);
		if (!set)
		{
			if (++LinkLobbyWaitTicks > LOBBY_PEER_WAIT_TICKS)
			{
				for (i = 0; i < 4; i++)
					syWordsClearEntry(LinkPeerSignatures[i], sizeof(SerialTransfer.payload));
				LinkLobbyWaitTicks = 0;
				scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
			}
		}
		else
		{
			LinkLobbyWaitTicks = 0;
		}
		if (LinkLobbyWaitTicks == 0)
		{
			if (scLinkLobbyPeerSlotMatches(LOBBY_SLOT_SIGNATURE)
			    && (scLinkLobbyPeerSlotMatches(LOBBY_SLOT_ASKED)
			        || scLinkLobbyPeerSlotMatches(LOBBY_SLOT_IN_CIRCLE)))
			{
				fdScriptSetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
				if (fdScriptGetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED))
					state->raisedCue = LOBBY_CUE_PROMPT;
				result = TRUE;
			}
			else
			{
				fdScriptClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
				result = FALSE;
			}
		}
		if (fdScriptGetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED)
		    && fdScriptGetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED)
		    && !fdScriptGetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED))
			state->raisedCue = LOBBY_CUE_PROMPT;
	}
	if ((fdScriptGetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED)
	     || fdScriptGetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED))
	    && !fdScriptGetFlag(FLAG_MONSTER_BATTLE)
	    && !scLinkLobbyPeerSlotMatches(LOBBY_SLOT_SIGNATURE)
	    && LinkLobbyWaitTicks >= LOBBY_PEER_WAIT_TICKS)
	{
		state->raisedCue = LOBBY_CUE_DISCONNECTED;
		fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
		fdScriptClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
		fdScriptClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
	}
	if (fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED))
		state->raisedCue = LOBBY_CUE_DISCONNECTED;
	return result;
}

// Once both linked players have applied for a battle, the attendant behind the left counter says,
// "If you want to participate, please step into the circle!", and asks again if the player waits.
Sint32 scLinkLobbyPromptToParticipate(void)
{
	Sint32 set;

	set = fdScriptGetFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
	if (set != 0)
	{
		return set;
	}
	if (++LinkLobbyCallTicks == LOBBY_PROMPT_REPEAT_TICKS)
	{
		LinkLobbyCallTicks = 0;
		fdScriptClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
	}
	set = fdScriptGetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
	if (set != 0)
	{
		return set;
	}
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgLobbyIfYouWantToParticipate);
	// "If you want to participate, please step into the circle!"
	fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
	sySynch(5);
	fdScriptSetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
	fdScriptEnd();
}

// The attendant behind the left counter says, "The link was disconnected."
Sint32 scLinkLobbyReportLinkDisconnected(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
	fdScriptClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
	fdScriptClearFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	state->raisedCue = LOBBY_CUE_NONE;
	fdScriptSetMessageId((Sint32)MsgLobbyLinkWasDisconnected);
	// "The link was disconnected."
	fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
	fdScriptClearFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
	fdScriptEnd();
}

// Both sizes are whole 20-byte blocks, since the serial runtime sends 20 bytes a block and ends a
// send only when its count reaches exactly 0. The bytes sent for each party member: a Unit (332
// bytes) and the eight bytes after it, 17 blocks of 20.
#define LOBBY_UNIT_RECORD_BYTES     (17 * 20)
// The bytes received for the Djinn recovery table after the party members, 17 blocks of 20.
#define LOBBY_RECOVERY_RECORD_BYTES (17 * 20)
// The party members a linked battle takes. A smaller party sends absent records.
#define LOBBY_UNIT_RECORD_COUNT     3
// A record fails on the 25th tick it waits without a link or past the timeout.
#define LOBBY_TRANSFER_RETRY_LIMIT  24
// Ticks, about ten seconds, for the three party member records, and again for the recovery table.
#define LOBBY_SEND_TIMEOUT_TICKS    600
// Ticks, about fifteen seconds, for all four records received.
#define LOBBY_RECEIVE_TIMEOUT_TICKS 900

// The Broken Seal's BattleArenaReceivePartyRecords: takes the other player's party records over the
// link.
Sint32 scLinkLobbyReceivePartyRecords(void)
{
	Uint16 namePrefix[24];
	Sint32 result;
	Uint32 size;
	void  *heap;
	Sint32 timeout;
	Sint32 slot;
	Sint32 tries;
	Sint32 transfer;
	Sint32 i;
	Sint32 length;
	UNIT  *unit;

	size    = LOBBY_UNIT_RECORD_BYTES;
	heap    = syHeapAllocateEWRAM(size);
	result  = 0;
	timeout = LOBBY_RECEIVE_TIMEOUT_TICKS;
	slot    = 0;
	// Three records, each started at recordNext and waited for at recordTest, then the Djinn
	// recovery table at table. Each receive waits a tick at a time; more than size bytes is an
	// overrun and fewer once it ends a short record, and once the timeout has run out, or while the
	// link is not ready, every tick is a try, failing past LOBBY_TRANSFER_RETRY_LIMIT.
	goto recordNext;
recordWait:
	if (SerialReceivedSize > size)
	{
		result = -1;
		goto done;
	}
	sySynch(1);
	if (--timeout < 0 || (LinkStatusWork & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK)
	{
		if (++tries > LOBBY_TRANSFER_RETRY_LIMIT)
		{
			result = -1;
			goto done;
		}
	}
recordTest:
	if (sySerialGetActiveTransfers() != 0)
		goto recordWait;
	if (SerialReceivedSize != size)
	{
		result = -1;
		goto done;
	}
	if (unit->side != BATTLE_SIDE_NONE)
		result++;
	sySynch(2);
	fdMessageDecodeRawUnbounded((Sint32)MsgEnemyLabel, namePrefix);
	for (i = 0; i <= BATTLE_ARENA_PREFIX_LAST && namePrefix[i] != 0; i++)
		;
#if BATTLE_ARENA_PREFIX_LAST == 7
	namePrefix[i] = ' ';
	length        = i + 1;
#else
	length = i;
#endif
	for (i = UNIT_NAME_SIZE - 1; i >= length; i--)
		unit->name[i] = unit->name[i - length];
	for (i = 0; i < length; i++)
		unit->name[i] = namePrefix[i];
	unit->name[UNIT_NAME_SIZE - 1] = 0;
	slot++;
recordNext:
	if (slot >= LOBBY_UNIT_RECORD_COUNT)
		goto table;
	unit     = syUnitGet(slot + FIRST_ENEMY_UNIT);
	transfer = sySerialStartReceive(unit);
	tries    = 0;
	if (transfer == -1)
		goto failed;
	goto recordTest;
tableWait:
{
	Sint32 limit = LOBBY_RECOVERY_RECORD_BYTES;

	if (SerialReceivedSize > limit)
	{
		result = -1;
		goto done;
	}
}
	sySynch(1);
	if (--timeout < 0 || (LinkStatusWork & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK)
	{
		if (++tries > LOBBY_TRANSFER_RETRY_LIMIT)
		{
			result = -1;
			goto done;
		}
	}
tableTest:
	if (sySerialGetActiveTransfers() != 0)
		goto tableWait;
	if (SerialReceivedSize != LOBBY_RECOVERY_RECORD_BYTES)
		result = -1;
	else
		sySynch(2);
	goto done;
table:
	syHeapRelease(heap);
	size     = LOBBY_RECOVERY_RECORD_BYTES;
	heap     = syHeapAllocateEWRAM(size);
	transfer = sySerialStartReceive(gmDjinnGetRecoveryTable(1));
	tries    = 0;
	if (transfer != -1)
		goto tableTest;
failed:
	result = transfer;
done:
	syHeapRelease(heap);
	return result;
}

// Writes up to three active party owners to out, ending the list with PARTY_SLOT_EMPTY.
Sint32 scLinkLobbyCopyPartyOwnerIds(Uint16 *out)
{
	Sint32 count = gmPartyCountMembers();
	Sint32 i;

	if (count > 3)
		count = 3;
	for (i = 0; i < count; i++)
	{
		Uint8 owner = MainRoutineStateWork.party[i];

		if (out != NULL)
			*out++ = owner;
	}
	if (out != NULL)
		*out = PARTY_SLOT_EMPTY;
	return count;
}

// The Broken Seal's BattleArenaSendPartyRecords: sends the party's records to the other player.
Sint32 scLinkLobbySendPartyRecords(void)
{
	Sint32               result;
	Uint16               memberIds[8];
	Uint8                peerSlots[8];
	Sint32               transferResult;
	Uint8               *heap;
	Sint32               memberCount;
	Sint32               timeout;
	Sint32               tries;
	Sint32               i;
	Sint32               j;
	Sint32               k;
	Uint32               size;
	DJINN_RECOVERY_LIST *list;

	size        = LOBBY_UNIT_RECORD_BYTES;
	heap        = syHeapAllocateEWRAM(size);
	result      = 0;
	timeout     = LOBBY_SEND_TIMEOUT_TICKS;
	memberCount = scLinkLobbyCopyPartyOwnerIds(memberIds);
	for (i = 0; i <= 7; i++)
		peerSlots[i] = 0;
	// Each send waits a tick at a time to finish. Once the timeout has run out, or while the link
	// is not ready, every tick is a try, and past LOBBY_TRANSFER_RETRY_LIMIT tries the exchange
	// fails. A send that cannot start leaves the block.
	do
	{
		for (i = 0; i < memberCount; i++)
		{
			Sint32 tries = 0;

			syWordsCopyEntry(heap, syUnitGet(memberIds[i]), size);
			((UNIT *)heap)->side    = BATTLE_SIDE_PARTY;
			peerSlots[memberIds[i]] = FIRST_ENEMY_UNIT + i;
			if ((transferResult = sySerialStartSend(heap, size)) == -1)
			{
				result = transferResult;
				goto done;
			}
			while (sySerialGetActiveTransfers() != 0)
			{
				sySynch(1);
				if (--timeout < 0
				    || (LinkStatusWork & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK)
				{
					if (++tries > LOBBY_TRANSFER_RETRY_LIMIT)
					{
						result = -1;
						goto done;
					}
				}
			}
			sySynch(2);
		}
		// Pad to three records with the last copy marked absent.
		for (; i < LOBBY_UNIT_RECORD_COUNT; i++)
		{
			((UNIT *)heap)->side = tries = 0;
			if ((transferResult = sySerialStartSend(heap, size)) == -1)
				break;
			while (sySerialGetActiveTransfers() != 0)
			{
				sySynch(1);
				if (--timeout < 0
				    || (LinkStatusWork & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK)
				{
					if (++tries > LOBBY_TRANSFER_RETRY_LIMIT)
					{
						result = -1;
						goto done;
					}
				}
			}
			sySynch(2);
		}
		if (i < LOBBY_UNIT_RECORD_COUNT)
		{
			result = transferResult;
			break;
		}
		// Then the recovery table.
		syHeapRelease(heap);
		size = LOBBY_RECOVERY_RECORD_BYTES;
		heap = syHeapAllocateEWRAM(size);
		IWRAM_COPY_WORDS(heap, gmDjinnGetRecoveryTable(0), size);
		{
			Sint32 timeout;
			Sint32 tries;

			list    = &((DJINN_RECOVERY_TABLE *)heap)->list;
			tries   = 0;
			timeout = LOBBY_SEND_TIMEOUT_TICKS;
			// Give each Djinni on the list its party member's number on the partner's side, and
			// drop those of party members not sent.
			for (j = 0; j < list->count; j++)
			{
				list->entries[j].unitId = peerSlots[list->entries[j].unitId];
				if (list->entries[j].unitId == 0)
				{
					for (k = j; k < list->count - 1; k++)
						list->entries[k] = list->entries[k + 1];
					list->count--;
					j--;
				}
			}
			if ((transferResult = sySerialStartSend(heap, size)) == -1)
			{
				result = transferResult;
				break;
			}
			while (sySerialGetActiveTransfers() != 0)
			{
				sySynch(1);
				if (--timeout < 0
				    || (LinkStatusWork & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK)
				{
					if (++tries > LOBBY_TRANSFER_RETRY_LIMIT)
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
	syHeapRelease(heap);
	return result;
}

// Resets the transfer state, as syLinkBattleResetTransferState does.
static __inline__ void scLobbyResetTransferState(void)
{
	SERIAL_TRANSFER_STATE *state;
	Uint32                 saved;

	state = &SerialTransfer;
	IRQ_LOCK(saved);
	state->status               = SERIAL_STATUS_RUNNING;
	*(Uint8 *)&state->peerFlags = 0;
	SerialSendSource            = 0;
	*(Uint8 *)&state->flags     = 0;
	SerialSendSize              = 0;
	SerialReceiveDestination    = 0;
	SerialReceivedSize          = 0;
	IRQ_UNLOCK(saved);
}

// Exchanges the two parties. The parent sends first, and the child (area flag
// FLAG_BATTLE_ARENA_LINK_CHILD) receives first.
//
// On success stores the partner's party member count in FLAG_LINK_PARTY_COUNT and returns it. On
// failure resets the transfer state and returns a negative result.
Sint32 scLinkLobbyExchangePartyRecords(void)
{
	Sint32 result;
	Sint32 first;
	Sint32 flag;

	// FAKEMATCH(dead): the result's zero in a once-only block.
	do
	{
		result = 0;
	} while (0);
	flag              = fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_CHILD);
	LinkExchangeState = result;
	if (!flag)
	{
		sySynch(5);
		result = scLinkLobbySendPartyRecords();
		if (result < 0)
			goto fail;
		sySynch(5);
		first = result = scLinkLobbyReceivePartyRecords();
		if (result < 0)
			goto check;
	}
	else
	{
		first = result = scLinkLobbyReceivePartyRecords();
		if (result < 0)
			goto fail;
		sySynch(10);
		result = scLinkLobbySendPartyRecords();
		if (result < 0)
			goto fail;
	}
	scFieldGameFlagSetByte(FLAG_LINK_PARTY_COUNT, first);
	result = first;
check:
	if (first < 0)
	{
	fail:
		scLobbyResetTransferState();
	}
	return result;
}

// Felix steps onto the diamond mark before the arched door to the arena, metatile (13, 11), once he
// has applied: "Awaiting your opponent..." shows until the other player steps on theirs, the
// attendant says "Good luck!", Felix walks to the door, the two parties are traded over the link
// and the battle starts. After choosing a Monster Battle, the mark starts it at once.
Sint32 scLinkLobbyEnterBattle(void)
{
	FIELD_STATE *state;
	Bool         failed;
	WINDOW      *window;
	Sint32       ticksUnanswered;
	Bool         stop;
	Uint8       *savedUnits;
	Uint16      *table;
	Uint32       i;

	failed          = FALSE;
	window          = 0;
	state           = RamHeapSlots->fieldState;
	ticksUnanswered = 0;
	if (fdScriptGetFlag(FLAG_MONSTER_BATTLE))
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
	}
	else
	{
		if (!fdScriptGetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED))
			return;
		if (fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED))
			return;
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_IN_CIRCLE);
		if (!scLinkLobbyPeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))
			// "Awaiting your opponent..."
			window = mnMessageOpenWindow(
				(Sint32)MsgLobbyAwaitingYourOpponent, 5, 4, MESSAGE_WINDOW_ZOOM);
		while (!scLinkLobbyPeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))
		{
			sySynch(1);
			stop = FALSE;
			if (!fdScriptGetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED))
				stop = TRUE;
			if (fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED))
				stop = TRUE;
			if (!scLinkLobbyPeerSlotMatches(LOBBY_SLOT_IN_CIRCLE)
			    && !scLinkLobbyPeerSlotMatches(LOBBY_SLOT_ASKED))
			{
				if (++ticksUnanswered > LOBBY_PEER_WAIT_TICKS)
					stop = TRUE;
			}
			else
			{
				ticksUnanswered = 0;
			}
			if (stop)
			{
				state->raisedCue = LOBBY_CUE_DISCONNECTED;
				fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
				fdScriptClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
				fdScriptClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
				scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
				failed = TRUE;
				fdScriptClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
				break;
			}
		}
		if (window)
			mnWindowClose(window, 1);
		sySynch(5);
	}
	if (!failed)
	{
		// The exchange below receives the opponent's party members and Djinn into units
		// FIRST_ENEMY_UNIT on; what those units held is kept here meanwhile.
		savedUnits = syHeapSlotAllocateEWRAM(HEAP_SLOT_ENEMY_UNITS, 6 * BATTLE_UNIT_SIZE);
		syCloseEvent(scLinkLobbyPollPeerReady);
		fdTilesetAnimationResume(BATTLE_ARENA_GATE_ANIMATION);
		sySynch(8);
		fdTilesetAnimationPause(BATTLE_ARENA_GATE_ANIMATION);
		if (fdScriptGetFlag(FLAG_MONSTER_BATTLE))
		{
			fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
			fdScriptSetMessageId((Sint32)MsgLobbyGoodLuck);
			// "Good luck!"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			sySynch(45);
			fdActorSetSpeed(ACTOR_FELIX, 1);
			fdActorWalkTo(ACTOR_FELIX, 216, 184);
			fdActorWaitForMoveById(ACTOR_FELIX);
			fdActorWalkTo(ACTOR_FELIX, 216, 168);
			fdActorWaitForMoveById(ACTOR_FELIX);
		}
		else
		{
			fdActorSetSpeed(ACTOR_FELIX, 1);
			fdActorWalkTo(ACTOR_FELIX, 216, 200);
			fdActorWaitForMoveById(ACTOR_FELIX);
			fdActorSetSpeed(ACTOR_FELIX, 0.1);
			fdActorWalkTo(ACTOR_FELIX, 216, 168);
			if (scLinkLobbyExchangePartyRecords() < 0)
			{
				fdActorSetSpeed(ACTOR_FELIX, 1);
				fdActorWalkTo(ACTOR_FELIX, 216, 200);
				fdTilesetAnimationResume(BATTLE_ARENA_GATE_ANIMATION);
				sySynch(8);
				fdTilesetAnimationPause(BATTLE_ARENA_GATE_ANIMATION);
				fdActorWaitForMoveById(ACTOR_FELIX);
				syHeapSlotRelease(HEAP_SLOT_ENEMY_UNITS);
				scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
				scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
				sySetEvent((EVENT_FUNC)scLinkLobbyPollPeerReady, EVENT_PASS_TICK);
				sySetEventFlags(scLinkLobbyPollPeerReady, 1);
				fdScriptClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
				fdScriptClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
				fdScriptClearFlag(FLAG_LINK_LOBBY_LINK_UP);
				fdScriptClearFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
				fdScriptClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
				state->raisedCue = LOBBY_CUE_DISCONNECTED;
				fdScriptEnd();
				return;
			}
			fdActorSetSpeed(ACTOR_FELIX, 0.5);
			fdActorWaitForMoveById(ACTOR_FELIX);
		}
		if (fdScriptGetFlag(FLAG_MONSTER_BATTLE))
		{
			fdScriptSetAfterBattleDestination(SCENE_LOBBY, LOBBY_AFTER_MONSTER_WIN);
			fdScriptSetDefeatDestination(SCENE_LOBBY, LOBBY_AFTER_MONSTER_LOSS);
		}
		else
		{
			fdScriptSetAfterBattleDestination(SCENE_LOBBY, LOBBY_AFTER_LINK_WIN);
			fdScriptSetDefeatDestination(SCENE_LOBBY, LOBBY_AFTER_LINK_LOSS);
		}
		MainRoutineStateWork.battleType = BATTLE_TYPE_LINK;
		fdRequestBattleFromEncounterTable(ENCOUNTER_ZONE_VENUS_DJINN, LINK_BATTLE_ENCOUNTER_SLOT);
		// EXEC fills the payload's first four halfwords, where the link battle's command entry
		// looks for it in the partner's.
		table    = (Uint16 *)SerialTransfer.payload;
		table[0] = 'E';
		table[1] = 'X';
		table[2] = 'E';
		table[3] = 'C';
		// The six units go to the battle's actor sprite slots, where the battle takes them from.
		for (i = 0; i < 6 * BATTLE_UNIT_SIZE; i++)
			(RamMapMetatileBuffer + 32 * 1024)[i] = savedUnits[i];
		syHeapSlotRelease(HEAP_SLOT_ENEMY_UNITS);
	}
	fdScriptEnd();
}

// Felix talks to the attendant behind the counter left of the arched door: with no other player
// linked, "You do not have an opponent. Would you like to battle a monster?"; with one, she
// explains the Battle Arena the first time and then asks, "Would you like to apply?"
Sint32 scLinkLobbyChooseBattleMode(void)
{
	Sint32 answer;
	Sint32 msg;
	Sint32 line;

	msg = (Sint32)MsgLobbyPleaseSpeakToMeWhen;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
	if (scLinkLobbyPeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0)
		sySynch(1);
	if (scLinkLobbyPeerSlotMatches(LOBBY_SLOT_SIGNATURE) == 0)
	{
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_ALONE);
		scLinkLobbyInitSerial();
		if (!fdScriptGetFlag(FLAG_MONSTER_BATTLE))
		{
			fdScriptSetMessageId(msg + 5);
			// "You do not have an opponent. Would you like to battle a monster?"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			answer = fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0);
			// Yes starts a monster series: FLAG_AREA_TEMP_E8's byte, which counts the victories
			// towards a stronger monster, and LINK_TALLY_MONSTER_STREAK start at 0.
			if (answer == 0)
			{
				scFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, 0);
				fdScriptSetFlag(FLAG_MONSTER_BATTLE);
				fdScriptClearFlag(FLAG_LINK_BATTLE_ARRANGED);
				fdScriptClearFlag(FLAG_LINK_BATTLE);
				fdScriptSetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
				// "When you finish your preparations, please step into the circle!"
				line                                                        = msg + 7;
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = answer;
				goto message;
			}
			fdScriptClearFlag(FLAG_MONSTER_BATTLE);
			fdScriptSetFlag(FLAG_LINK_BATTLE);
			scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
			// "Please wait until an opponent arrives, then."
			line = msg + 6;
			goto message;
		}
		else
		{
			// "When you finish your preparations, please step into the circle!"
			line = msg + 3;
			goto message;
		}
	}
	else
	{
		if (fdScriptGetFlag(FLAG_MONSTER_BATTLE))
		{
			scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
			fdScriptSetMessageId((Sint32)MsgLobbyYourOpponentHasArrived);
			// "Your opponent has arrived!"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			fdScriptClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
			fdScriptClearFlag(FLAG_MONSTER_BATTLE);
		}
		if (fdScriptGetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED))
		{
			// "When you finish your preparations, please step into the circle!"
			line = msg + 3;
		message:
			fdScriptSetMessageId(line);
		open_message:
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
		}
		else if (!fdScriptGetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED)
		         && !fdScriptGetFlag(FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED))
		{
			fdScriptSetMessageId(msg);
			// "Please speak to me when you want to fight in a battle."
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			fdScriptSetFlag(FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED);
		}
		else
		{
			fdScriptSetFlag(FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED);
			if (fdScriptGetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED))
				fdScriptSetMessageId(msg + 2);
			else
				fdScriptSetMessageId(msg + 1);
			// "Your opponent has asked for a battle. Do you accept?", or with no request yet "Would
			// you like to apply?"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			if (fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0)
			{
				if (scLinkLobbyPeerSlotMatches(LOBBY_SLOT_SIGNATURE))
				{
					fdScriptSetFlag(FLAG_LINK_BATTLE);
					fdScriptSetFlag(FLAG_LINK_BATTLE_ARRANGED);
					// "When you finish your preparations, please step into the circle!", or with no
					// request yet "You have asked for a battle. We will call your name if your
					// opponent accepts. Please wait."
					if (fdScriptGetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED))
						fdScriptSetMessageId(msg + 3);
					else
						fdScriptSetMessageId(msg + 4);
					scLinkLobbyWriteSlotValue(LOBBY_SLOT_ASKED);
					fdScriptSetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
					goto open_message;
				}
				else
				{
					fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
				}
			}
			else
			{
				fdScriptSetMessageId(msg);
				// "Please speak to me when you want to fight in a battle."
				fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			}
		}
	}
	fdScriptEnd();
}

// Felix talks to the man behind the counter right of the arched door: across the counter, from the
// south, he tells the linked battles won in all; from any other side, the most won in a row, which
// the board on the wall shows.
Sint32 scLinkLobbyReportWinTallies(Sint32 actor)
{
	Uint32 facing = fdActorGetById(ACTOR_FELIX)->facing;
	Sint32 base;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceActor(actor, MainRoutineStateWork.leaderActorId, 0);
	if (facing > FACING_NORTHWEST && facing < FACING_NORTHEAST)
	{
		base = (Sint32)MsgLobbyItLooksLike;
		if (MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_WINS] != 0)
		{
			mnMessageArgumentClearAll();
			mnMessageArgumentPush(MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_WINS],
			                      MESSAGE_ARGUMENT_NUMBER);
			fdScriptSetMessageId(base + 1);
			// "You've won ... Linked Battle. Keep up the good work!"
			fdScriptShowMessage(actor, 0);
		}
		else
		{
			fdScriptSetMessageId((Sint32)MsgLobbyIWillTell);
			// "I will tell you your total wins when you finish the linked finals."
			fdScriptShowMessage(actor, 0);
		}
	}
	else
	{
		base = (Sint32)MsgItLooksLikeThisIsYour;
		if (MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] != 0)
		{
			mnMessageArgumentClearAll();
			mnMessageArgumentPush(MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK],
			                      MESSAGE_ARGUMENT_NUMBER);
			fdScriptSetMessageId(base + 1);
			// "You've won ... Linked Battle. Check the board on the wall for..."
			fdScriptShowMessage(actor, 0);
		}
		else
		{
			fdScriptSetMessageId((Sint32)MsgLobbyIWillTellYouYourConsecutive);
			// "I will tell you your consecutive wins when you finish the finals."
			fdScriptShowMessage(actor, 0);
		}
	}
}

// A developer's leftover: the white-bearded man in green placed below the hall, where the player
// cannot walk, says "Choose your parameters here." and lets the player pick a debug parameter set
// by number with Left and Right. The man at the right counter answers, "The settings are made. The
// rest is up to you.", or for a set that does not exist, "There's no such parameter. Ask Mr.
// Sugimoto."
Sint32 scLinkLobbyChooseDebugParamSet(Sint32 actorId)
{
	WINDOW *window;
	Sint32  choice;
	Sint32  shown;
	Sint32  ok;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceActor(actorId, MainRoutineStateWork.leaderActorId, 0);
	fdScriptSetMessageId((Sint32)MsgLobbyChooseYourParametersHere);
	// "Choose your parameters here."
	fdScriptShowMessage(actorId, 0);
	window = mnWindowCreate(0, 0, 6, 4, 2);
	choice = 0;
	shown  = -1;
	for (;;)
	{
		if (choice != shown)
		{
			mnWindowSpritePrepareForRedraw(window);
			mnTextDrawNumber(choice, 3, window, 0, 0);
			shown = choice;
		}
		if (PerWork.repeat & KEY_LEFT)
		{
			choice--;
		}
		if (PerWork.repeat & KEY_RIGHT)
		{
			choice++;
		}
		if (choice < 0)
		{
			choice = 0;
		}
		if (PerWork.push & KEY_A)
		{
			break;
		}
		if (PerWork.push & KEY_B)
		{
			choice = -1;
			break;
		}
		sySynch(1);
	}
	mnWindowClose(window, 1);
	if (choice >= 0)
	{
		ok = btDebugParamSetLoad(choice);
	}
	else
	{
		fdScriptSetMessageId((Sint32)MsgLobbyWhatIsThisJustTease);
		// "What is this, just a tease?"
		fdScriptShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
		sySynch(10);
		fdScriptEnd();
		return;
	}
	if (ok != 0)
	{
		fdScriptSetMessageId((Sint32)MsgLobbyTheresNoSuchParameterAsk);
		// "There's no such parameter. Ask Mr. Sugimoto."
		fdScriptShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	else
	{
		fdScriptSetMessageId((Sint32)MsgLobbySettingsAreMadeRest);
		// "The settings are made. The rest is up to you."
		fdScriptShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
	}
	sySynch(10);
	fdScriptEnd();
}

// Felix talks to the healer on the carpeted floor above the hall: she offers a Sanctum's services.
void scLinkLobbyOpenSanctum(Sint32 actor)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	mnSanctumRun(actor);
	fdScriptEnd();
}

// Whether owner is one of the first three active owners, or the first four while
// FLAG_LINK_BATTLE_ARRANGED is clear; an 0xff entry ends the list.
Bool scLinkLobbyPartyContains(Sint32 owner)
{
	Sint32 count = gmPartyCountMembers();
	Sint32 limit = 3;
	Sint32 i;

	if (!fdScriptGetFlag(FLAG_LINK_BATTLE_ARRANGED))
		limit = 4;
	if (count > limit)
		count = limit;
	for (i = 0; i < count; i++)
	{
		if (MainRoutineStateWork.party[i] == PARTY_SLOT_EMPTY)
			return FALSE;
		if (MainRoutineStateWork.party[i] == owner)
			return TRUE;
	}
	return FALSE;
}

// Unused: does nothing; nothing calls it.
UNUSED void scLinkLobbyNoOperation(void)
{
}

extern Uint8 MsgLobbyYouHaveSongRequest[];
extern Uint8 MsgLobbyComeBackWhenYoureReady[];
// The sounds the song request offers, numbered from 0 in its window.
#define SONG_REQUEST_SOUNDS 97
extern Uint16 LinkLobbySongRequestSounds[SONG_REQUEST_SOUNDS];

// "You have a song request?": the player picks a piece of music or a sound by number with Left and
// Right and hears it with A, until B ends it with "Come back when you're ready to rock!"
//
// Only the ones already heard in the game can be picked, unless Start, Up and L are held as the
// request opens, or this game or a file on the cartridge has been cleared. The woman in red takes
// requests while L or R is held (scLinkLobbyTalkToVisitor); ACTOR_SONG_REQUEST_VISITOR, a
// strongman, stands below the hall where the player cannot walk.
void          scLinkLobbyTakeSongRequest(Sint32 actor)
{
	WINDOW *window;
	Sint32  song;
	Sint32  shown;
	Sint32  step;
	Sint32  all;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceActor(actor, MainRoutineStateWork.leaderActorId, 0);
	fdScriptSetMessageId((Sint32)MsgLobbyYouHaveSongRequest);
	// "You have a song request?"
	fdScriptShowMessage(actor, 0);
	song = 0;
	all  = 0;
	if ((PerWork.data & (KEY_START | KEY_UP | KEY_L)) == (KEY_START | KEY_UP | KEY_L))
		all = 1;
	fdScriptPlaySound(SOUND_FADE_OUT_BGM);
	for (;;)
	{
		window = mnWindowCreate(12, 8, 6, 3, 2);
		shown  = -1;
		for (;;)
		{
			if (song != shown)
			{
				mnWindowSpritePrepareForRedraw(window);
				mnTextDrawNumber(song, 3, window, 0, 0);
				shown = song;
			}
			step = 0;
			if (PerWork.repeat & KEY_LEFT)
				step--;
			if (PerWork.repeat & KEY_RIGHT)
				step = 1;
			for (; step != 0;)
			{
				song += step;
				if (song < 0)
					song = SONG_REQUEST_SOUNDS - 1;
				if ((Uint32)song > SONG_REQUEST_SOUNDS - 1)
					song = 0;
				if (all != 0 || MainRoutineStateWork.sendFlag != 0 || AnyFileCleared != 0)
					break;
				if (sySoundFlagTest(LinkLobbySongRequestSounds[song]) != 0)
					break;
			}
			if (PerWork.push & KEY_A)
				break;
			if (PerWork.push & KEY_B)
			{
				song = -1;
				break;
			}
			sySynch(1);
		}
		mnWindowClose(window, 1);
		if (song >= 0)
		{
			fdScriptPlaySound(LinkLobbySongRequestSounds[song]);
		}
		else
		{
			fdScriptPlaySound(SOUND_FADE_OUT_BGM);
			sySynch(120);
			fdScriptPlaySound(MUS_TRY_YOUR_LUCK);
			fdScriptSetMessageId((Sint32)MsgLobbyComeBackWhenYoureReady);
			// "Come back when you're ready to rock!"
			fdScriptShowMessage(actor, 0);
			break;
		}
	}
	sySynch(10);
	fdScriptEnd();
}

// Leaves for an entrance of a scene (fdScriptSetDestination, through the overlay's own import row).
void   syImportScriptSetDestination(Sint32 scene, Sint32 entrance);

// Felix talks to a member of his party waiting in the hall: what they say follows whether they will
// fight, and how the last linked battle went.
Sint32 scLinkLobbyTalkByProgress(Sint32 actor)
{
	Sint32 base = (Sint32)MsgLobbyItsOurTurnLetsGo;
	Sint32 lead = scLinkLobbyPartyContains(ACTOR_FELIX);
	Sint32 own  = scLinkLobbyPartyContains(actor);

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceActor(actor, MainRoutineStateWork.leaderActorId, 0);
	if (fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED))
	{
		// DEAD: Felix's own flag is read and dropped; the ROM keeps the call.
		fdScriptGetFlag(FLAG_LINK_LOBBY_IN_PARTY_FIRST + ACTOR_FELIX);
		base = fdScriptGetFlag(actor + FLAG_LINK_LOBBY_IN_PARTY_FIRST);
		if (fdScriptGetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_WON))
		{
			if (base)
				base = (Sint32)MsgLobbyEverybodyDeservesLittleCredit;
			else
				base = (Sint32)MsgLobbyEverybodyDidGreatIDidnt;
		}
		else
		{
			if (base)
				base = (Sint32)MsgLobbyDontBlameYourself;
			else
				base = (Sint32)MsgLobbyYouWinSomeYouLose;
		}
	}
	else if (lead != 0)
	{
		if (own == 0)
			base = (Sint32)MsgOkWeRe;
	}
	else
	{
		base = (Sint32)MsgLobbyIllFightEnoughForBoth;
	}
	fdScriptSetMessageId(base + actor);
	fdScriptShowMessage(actor, 0);
	if (base == (Sint32)MsgLobbyIllFightEnoughForBoth && actor == ACTOR_JENNA)
	{
		scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
		fdScriptWait(10);
	}
	fdScriptEnd();
}

// Felix walks out of the hall's south doorway, metatile (13, 18): the link ends and the game
// returns to the File Screen.
//
// The serial handlers come off and the sound's normal reverb is loaded again before the scene
// leaves for SCENE_CLEAR's entrance 1.
Sint32 scLinkLobbyLeaveArena(void)
{
	sySerialRemoveIRQHandlers();
	sySoundLoadReverbCode(SOUND_REVERB_NORMAL);
	syImportScriptSetDestination(SCENE_CLEAR, 1);
#if FIX_FUNCTION_SIGNATURES
	return 0;
#endif
}

// Felix talks to the woman in the striped top at the west of the hall, "You look a little scrawny,
// but guys like you are often surprisingly strong...", and the woman in red beside her turns to
// him: "Fighting a guy with a build like that would really wear me out! Whew!" Once a visitor has
// spoken this visit, she says "I'll be rooting..." instead. Talking to the woman in red gives "I
// like my men macho", or with L or R held, a song request.
Sint32 scLinkLobbyTalkToVisitor(Sint32 actor)
{
	Sint32 msg;

	if (actor == ACTOR_MACHO_LOVING_VISITOR && (PerWork.data & KEYS_SHOULDERS))
	{
		scLinkLobbyTakeSongRequest(ACTOR_MACHO_LOVING_VISITOR);
		return;
	}
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if (fdScriptGetFlag(FLAG_LINK_LOBBY_VISITOR_SPOKE))
	{
		switch (actor)
		{
			case ACTOR_ROOTING_VISITOR     : msg = (Sint32)MsgLobbyIllBeRooting; break;
			case ACTOR_MACHO_LOVING_VISITOR: msg = (Sint32)MsgLobbyILikeMyMenMacho; break;
			default                        : msg = (Sint32)MsgLobbySometimesYourClassChangesWhen; break;
		}
	}
	else
	{
		switch (actor)
		{
			case ACTOR_ROOTING_VISITOR     : msg = (Sint32)MsgLobbyYouLookLittleScrawny; break;
			case ACTOR_MACHO_LOVING_VISITOR: msg = (Sint32)MsgLobbyILikeMyMenMacho; break;
			default                        : msg = (Sint32)MsgLobbySometimesYourClassChangesWhen; break;
		}
		fdScriptSetFlag(FLAG_LINK_LOBBY_VISITOR_SPOKE);
	}
	fdScriptSetMessageId(msg);
	// "Sometimes, your class changes when you unleash a Djinni. You need to..."
	fdScriptOpenMessage(actor, 0);
	fdScriptWait(1);
	if (msg == (Sint32)MsgLobbyYouLookLittleScrawny)
	{
		fdActorStartRepeatedMotion(ACTOR_MACHO_LOVING_VISITOR, 1);
		fdActorSetAnimation(ACTOR_MACHO_LOVING_VISITOR, 4);
		fdScriptWait(15);
		fdActorFaceActor(ACTOR_MACHO_LOVING_VISITOR, MainRoutineStateWork.leaderActorId, 0);
		fdScriptWait(5);
		fdScriptOpenMessage(ACTOR_MACHO_LOVING_VISITOR, 0);
	}
	fdScriptEnd();
}

// Felix talks to the fair-haired boy on the raised floor at the west of the hall: he explains that
// up to three allies fight in a linked battle, in words that follow the party's size, and the next
// time, how to change the party's order on the status screen.
Sint32 scLinkLobbyExplainPartyOrder(Sint32 actor)
{
	Sint32 msg;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceActor(actor, MainRoutineStateWork.leaderActorId, 0);
	if (!fdScriptGetFlag(FLAG_LINK_LOBBY_PARTY_ORDER_NEXT))
	{
		if (gmPartyCountMembers() == 8)
			msg = (Sint32)MsgLobbyUpToThreeAlliesCan;
		else if (gmPartyCountMembers() <= 3)
			msg = (Sint32)MsgLobbyUpToThreeAlliesCanFight;
		else
			msg = (Sint32)MsgLobbyUpToThreeAlliesCanFightInLinked;
		fdScriptSetFlag(FLAG_LINK_LOBBY_PARTY_ORDER_NEXT);
	}
	else
	{
		msg = (Sint32)MsgLobbyYouCanChangeOrder;
		fdScriptClearFlag(FLAG_LINK_LOBBY_PARTY_ORDER_NEXT);
	}
	fdScriptSetMessageId(msg);
	// "You can change the order of your party in the status screen. But you..."
	fdScriptOpenMessage(actor, 0);
	fdScriptEnd();
}

// Felix talks to the old man with a cane right of the arched door: he tells the most Monster
// Battles won in a row, or before any, "I can't wait to see how many Monster Battles you win."
Sint32 scLinkLobbyReportMonsterRecord(Sint32 id)
{
	MAIN_ROUTINE_STATE *gs;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	gs = &MainRoutineStateWork;
	fdActorFaceActor(id, gs->leaderActorId, 0);
	if (gs->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] != 0)
	{
		mnMessageArgumentPush(gs->linkTallies[LINK_TALLY_MONSTER_BEST_STREAK],
		                      MESSAGE_ARGUMENT_NUMBER);
		fdScriptSetMessageId((Sint32)MsgLobbyYouveWonConsecutiveMonsterBattle);
	}
	else
	{
		fdScriptSetMessageId((Sint32)MsgLobbyICantWaitToSee);
	}
	// "I can't wait to see how many Monster Battles you win. I'll be..."
	fdScriptOpenMessage(id, 0);
	fdScriptEnd();
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

// Saves and reports the link battle's results.
Sint32 scLinkLobbySaveBattleResults(void)
{
	mnSaveWriteLinkTallies();
	mnFieldMessageShow(MsgLobbyBattleResultsWereSaved, 1);
}

// Saves and reports the monster battle's results.
Sint32 scLinkLobbySaveMonsterBattleResults(void)
{
	mnSaveWriteLinkTallies();
	mnFieldMessageShow(MsgLobbyMonsterBattleResultsWereSaved, 1);
}

// The most the wall board's three digits show.
#define WALL_BOARD_MAX 999

// Shows value, at most WALL_BOARD_MAX, on the wall board, as three digits from right to left.
void scLinkLobbyDrawThreeDigitValue(Sint32 value)
{
	Sint32 digit;

	if (value > WALL_BOARD_MAX)
		value = WALL_BOARD_MAX;
	for (digit = 0; digit <= 2; digit++)
	{
		fdMapGridCopyMetatilesTo(27, value % 10, 16 - digit, 8, 1, 1);
		value = value / 10;
	}
	fdMapGridCenterOnFollowedPoint();
}

// Sets the lobby up as The Broken Seal's BattleArenaInitialize sets the arena up.
Sint32 scLinkLobbyInit(void)
{
	Sint32 i;
	Sint32 windows;

	fdScriptSetFlag(FLAG_IN_BATTLE_ARENA);
	MainRoutineStateWork.firstShortcut  = 0;
	MainRoutineStateWork.secondShortcut = 0;
	LinkLobbyCallTicks                  = 0;
	LinkLobbyWaitTicks                  = 0;
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	sySoundLoadReverbCode(SOUND_REVERB_NORMAL);
	scLinkLobbyDrawThreeDigitValue(MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
	fdMapGridCopyMetatileAttributes(11, 11, 1, 1, 13, 10);
	scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
	sySynch(1);
	fdTilesetAnimationPause(BATTLE_ARENA_GATE_ANIMATION);
	{
		// The payload's halfwords, as every signature's writer holds them.
		Uint16 *name = (Uint16 *)SerialTransfer.payload;

		name[4]      = 'T';
		name[5]      = 'A';
		name[6]      = 'L';
		name[7]      = 'K';
	}
	for (i = 0; i < 8; i++)
	{
		fdScriptClearFlag(FLAG_LINK_LOBBY_IN_PARTY_FIRST + i);
		if (scLinkLobbyPartyContains(i))
			fdScriptSetFlag(FLAG_LINK_LOBBY_IN_PARTY_FIRST + i);
	}
	if (MainRoutineStateWork.entrance == LOBBY_AFTER_MONSTER_WIN)
	{
		Sint32 msg;
		Sint32 v;

		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_ALONE);
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_WINS]++;
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK]++;
		// The offer has two lines a level (FLAG_MONSTER_BATTLE_WINS, -1 when a series starts), the
		// even line capped at MONSTER_OFFER_UNBELIEVABLE. FLAG_AREA_TEMP_E8's byte counts the
		// victories 0 to 2; the third raises the level and takes the next, odd line, which promises
		// a stronger monster.
		windows = (Sint8)syGameFlagGetByte(FLAG_MONSTER_BATTLE_WINS);
		msg     = windows * 2 + 2;
		if (msg > MONSTER_OFFER_UNBELIEVABLE)
			msg = MONSTER_OFFER_UNBELIEVABLE;
		v = scFieldGameFlagGetByte(FLAG_AREA_TEMP_E8);
		if (v == 2)
		{
			scFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, 0);
			windows++;
			msg++;
		}
		else
		{
			scFieldGameFlagSetByte(FLAG_AREA_TEMP_E8, v + 1);
		}
		fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
		fdScriptSetMessageId((Sint32)MsgLobbyNotBadWouldYouLike + msg);
		fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
		if (fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0)
		{
			if (windows > LEVEL_BONUS_MAX)
				windows = LEVEL_BONUS_MAX;
			scFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, windows);
		}
		else
		{
			fdScriptClearFlag(FLAG_MONSTER_BATTLE);
			scFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
			mnMessageArgumentPush(MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
			                      MESSAGE_ARGUMENT_NUMBER);
			if (MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK]
			    < MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK])
			{
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
					MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
				fdScriptSetMessageId((Sint32)MsgLobbyYouWonBattleYouBroke);
				// "You won ... battle. You broke the record!"
				fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
				scLinkLobbySaveMonsterBattleResults();
			}
			else
			{
				fdScriptSetMessageId((Sint32)MsgLobbyYouWonBattlePleaseSpeak);
				// "You won ... battle. Please speak to me when you wish to battle..."
				fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			}
			scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
		}
		fdScriptEnd();
	}
	else if (MainRoutineStateWork.entrance == LOBBY_AFTER_MONSTER_LOSS)
	{
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_LOSSES]++;
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_ALONE);
		fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
		mnMessageArgumentPush(MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK],
		                      MESSAGE_ARGUMENT_NUMBER);
		if (MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK]
		    < MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK])
		{
			MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
				MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK];
			fdScriptSetMessageId((Sint32)MsgLobbyYouWonBattleYouBroke);
			// "You won ... battle. You broke the record!"
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
			scLinkLobbySaveMonsterBattleResults();
		}
		else
		{
			fdScriptSetMessageId((Sint32)MsgLobbyYouWonBattleITold);
			// "You won ... battle. I told you our monsters were tough! Please..."
			fdScriptOpenMessage(ACTOR_ATTENDANT, 0);
		}
		MainRoutineStateWork.linkTallies[LINK_TALLY_MONSTER_STREAK] = 0;
		fdScriptClearFlag(FLAG_MONSTER_BATTLE);
		scFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
		fdScriptEnd();
	}
	else if (MainRoutineStateWork.entrance == LOBBY_AFTER_LINK_WIN)
	{
		Sint32 v;

		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
		// FLAG_AREA_TEMP_E8: the linked battle broke off rather than being won.
		if (fdScriptGetFlag(FLAG_AREA_TEMP_E8))
		{
			FIELD_STATE *state = RamHeapSlots->fieldState;

			fdScriptClearFlag(FLAG_AREA_TEMP_E8);
			state->raisedCue = LOBBY_CUE_DISCONNECTED;
			fdScriptClearFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
			sySynch(20);
			scLinkLobbyInitSerial();
			scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
			scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
		}
		else
		{
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_WINS]++;
			v = MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] + 1;
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
			if (MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] < (Uint16)v)
				MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] = v;
			scLinkLobbyDrawThreeDigitValue(
				MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
			scLinkLobbySaveBattleResults();
			fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
			fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_WON);
		}
		fdScriptEnd();
	}
	else if (MainRoutineStateWork.entrance == LOBBY_AFTER_LINK_LOSS)
	{
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
		if (!fdScriptGetFlag(FLAG_MONSTER_BATTLE))
		{
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_LOSSES]++;
			MainRoutineStateWork.linkTallies[LINK_TALLY_LINKED_STREAK] = 0;
			scLinkLobbySaveBattleResults();
		}
		fdScriptSetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
		fdScriptClearFlag(FLAG_LINK_LOBBY_LINK_BATTLE_WON);
		fdScriptEnd();
	}
	else
	{
		sySerialInit();
		fdScriptClearFlag(FLAG_LINK_BATTLE_ARRANGED);
		scFieldGameFlagSetByte(FLAG_MONSTER_BATTLE_WINS, -1);
		if (MainRoutineStateWork.idleSleepEnabled != 0)
		{
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdActorFaceActor(ACTOR_ATTENDANT, MainRoutineStateWork.leaderActorId, 0);
			fdScriptSetMessageId((Sint32)MsgLobbyNoteYouCantUseSleep);
			// "Note: you can't use the sleep function in battle stages!"
			fdScriptShowMessage(ACTOR_ATTENDANT, 0);
			fdScriptEnd();
		}
		MainRoutineStateWork.idleSleepEnabled = 0;
		IdleSleepEnabled                      = 0;
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_SIGNATURE);
		scLinkLobbyWriteSlotValue(LOBBY_SLOT_IDLE);
	}
	sySetEvent((EVENT_FUNC)scLinkLobbyPollPeerReady, EVENT_PASS_TICK);
	sySetEventFlags((EVENT_FUNC)scLinkLobbyPollPeerReady, 1);
	if (MainRoutineStateWork.entrance != LOBBY_AFTER_MONSTER_WIN
	    || !fdScriptGetFlag(FLAG_MONSTER_BATTLE))
	{
		gmPartyRestoreHpPp(1);
		btDjinnRecoveryCountDownTwenty();
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

// A word of four characters, the first in its lowest byte, as The Broken Seal's Battle Arena writes
// them.
#define PEER_TAG(a, b, c, d) ((a) | ((b) << 8) | ((c) << 16) | ((d) << 24))

// The words the lobby publishes in its link payload, by LOBBY_SLOT; the partner compares them with
// its own (scLinkLobbyPeerSlotMatches).
// clang-format off
Sint32 LinkLobbySlotValues[] = {
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

Uint8 LinkLobbySlotColumns[] = {
	0, 1, 1, 1, 1, 0,
};

// The music and sounds the song request offers, numbered from 0 in its window.
Uint16 LinkLobbySongRequestSounds[] = {
	MUS_TRY_YOUR_LUCK,
	MUS_THE_BROKEN_SEAL,
	MUS_THE_ADVENTURE_RESUMES,
	MUS_VENUS_LIGHTHOUSE_ENTRANCE,
	MUS_SUPER_LUCKY_DICE,
	MUS_ALHAFRA,
	MUS_CHAMPA,
	MUS_DAILA,
	MUS_APOJII_ISLANDS,
	MUS_IZUMO_FESTIVAL,
	MUS_IZUMO,
	MUS_THE_WISE_ONES_TRAP,
	MUS_KANDOREAN_TEMPLE_CAVES,
	MUS_KIBOMBO,
	MUS_RITUALS,
	MUS_LEMURIA,
	MUS_MADRA,
	MUS_ISLANDS,
	MUS_GAROH,
	MUS_YALLAM,
	MUS_CHILDRENS_SONG,
	MUS_CHILDRENS_SONG_SHORT,
	MUS_AIRS_ROCK,
	MUS_AQUA_ROCK,
	MUS_MADRA_CATACOMBS,
	MUS_CAVERNS,
	MUS_GAIA_ROCK,
	MUS_INSIDE_THE_GABOMBA_STATUE,
	MUS_JUPITER_LIGHTHOUSE,
	MUS_MAGMA_ROCK,
	MUS_MARS_LIGHTHOUSE,
	MUS_TUNDARIA_TOWER,
	MUS_TAOPO_SWAMP,
	MUS_ANKOHL_RUINS,
	MUS_KARST_AND_AGATIO,
	MUS_BRIGGS_STEALS_SHIP,
	MUS_CONFRONTATION,
	MUS_SHAMAN_VILLAGE,
	MUS_SHEBAS_PAST,
	MUS_ALEX,
	MUS_MAIN_THEME_TITLE,
	MUS_TO_THE_OPEN_SEA,
	MUS_A_JOURNEY_AROUND_THE_WORLD,
	MUS_JOURNEY_TO_SAVE_THE_WORLD,
	MUS_STAFF_CREDIT,
	MUS_TEST_YOUR_SKILLS,
	MUS_TRIAL_ROAD,
	MUS_BATTLE_JENNA,
	MUS_BATTLE_THE_LOST_AGE,
	MUS_BRIGGS_BATTLE,
	MUS_AQUA_HYDRA,
	MUS_BATTLE_ON_THE_SEA,
	MUS_BATTLE_WITH_KARST_AND_AGATIO,
	MUS_BATTLE_WITH_FLAME_DRAGON,
	MUS_BATTLE_WITH_DOOM_DRAGON,
	MUS_VICTORY,
	MUS_DEFEATED,
	MUS_WIND_ADEPTS,
	MUS_FATED_CONNECTION_WITH_SWORN_ENEMIES,
	MUS_PROX,
	MUS_ALCHEMY_UNLEASHED,
	MUS_UNUSED_TITLE_AFTER_PROLOGUE,
	MUS_SEA_OF_TIME,
	MUS_UNUSED_TITLE,
	MUS_PASSWORD_ENTRY,
	MUS_MERCURY_LIGHTHOUSE,
	MUS_EPILOGUE_VILLAGERS,
	MUS_EPILOGUE,
	MUS_TITLE_AFTER_PROLOGUE,
	MUS_VILLAGE_THEME,
	MUS_GONDOWAN_SETTLEMENT,
	MUS_AN_UNSETTLING_FEELING,
	MUS_LEMURIAN_PALACE,
	MUS_SINKING_INTO_SORROW,
	MUS_REUNITING_WITH_HAMA,
	MUS_PORT_TOWN_THEME,
	MUS_COLOSSO_INTRO,
	MUS_COLOSSEUM,
	MUS_TOLBI,
	MUS_ANEMOS_INNER_SANCTUM,
	MUS_CONTIGO,
	MUS_ELEMENTAL_STARS,
	MUS_DUNGEON_THEME,
	MUS_SUHALLA_GATE,
	MUS_YAMPI_DESERT,
	MUS_REMOTE_VILLAGE,
	MUS_DEHKAN_PLATEAU,
	MUS_GABOMBA_CATACOMBS,
	MUS_BABIS_THEME,
	MUS_SET_SAIL_ON_THE_KARAGOL_SEA,
	MUS_IDEJIMA,
	MUS_VENUS_LIGHTHOUSE,
	MUS_BATTLE_THE_BROKEN_SEAL,
	MUS_BATTLE_WITH_STAR_MAGICIAN,
	MUS_BATTLE_WITH_VALUKAR,
	MUS_BATTLE_WITH_SENTINEL,
	MUS_BATTLE_WITH_DULLAHAN,
};

SCENE_ENTRANCE SceneEntrances[] = {
	{LOBBY_AFTER_LINK_WIN, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
	{LOBBY_AFTER_LINK_LOSS, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
	{LOBBY_AFTER_MONSTER_WIN, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
	{LOBBY_AFTER_MONSTER_LOSS, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
	{1, CONDITION_ALWAYS, 216, 0, 288, FACING_NORTH, 0, 16, 0, 416, 310, 0},
	{0, CONDITION_ALWAYS, 216, 0, 208, FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
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

// Unused: a placement table nothing reads.
ACTOR_PLACE LinkLobbyUnusedPlacements[] = {
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

CUE SceneCues[] = {
	{CUE_TALK, ACTOR_ISAAC, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkByProgress)},
	{CUE_TALK, ACTOR_GARET, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkByProgress)},
	{CUE_TALK, ACTOR_IVAN, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkByProgress)},
	{CUE_TALK, ACTOR_MIA, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkByProgress)},
	{CUE_TALK, ACTOR_JENNA, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkByProgress)},
	{CUE_TALK, ACTOR_SHEBA, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkByProgress)},
	{CUE_TALK, ACTOR_PIERS, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkByProgress)},
	{CUE_TALK, ACTOR_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyChooseBattleMode)},
	{CUE_TALK, ACTOR_LINKED_WINS_TELLER, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyReportWinTallies)},
	{CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyOpenSanctum)},
	{CUE_TALK, ACTOR_GREETER, CONDITION_ALWAYS, MSGNO(MsgLobbyWelcomeToBattleArenaWhere)},
	{CUE_TALK, ACTOR_PRACTICING_VISITOR, CONDITION_ALWAYS, MSGNO(MsgLobbyHmItComesInLike)},
	{CUE_TALK, ACTOR_ROOTING_VISITOR, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkToVisitor)},
	{CUE_TALK, ACTOR_MACHO_LOVING_VISITOR, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyTalkToVisitor)},
	{CUE_TALK, ACTOR_DEBUG_HELPER, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyChooseDebugParamSet)},
	{CUE_TALK,
     ACTOR_PARTY_ORDER_VISITOR, CONDITION_ALWAYS,
     SCRIPT_REF(scLinkLobbyExplainPartyOrder)},
	{CUE_TALK, ACTOR_COMMAND_TIME_VISITOR, CONDITION_ALWAYS, MSGNO(MsgLobbyInLinkedFinalsYouCant)},
	{CUE_TALK, ACTOR_WIN_AND_LOSE_VISITOR, CONDITION_ALWAYS, MSGNO(MsgLobbySoYouWinAgainstOne)},
	{CUE_TALK, ACTOR_STRATEGY_VISITOR, CONDITION_ALWAYS, MSGNO(MsgLobbyYouCouldUsePsynergyDjinn)},
	{CUE_TALK, ACTOR_WARY_VISITOR, CONDITION_ALWAYS, MSGNO(MsgLobbyShouldIEnterOrNot)},
	{CUE_TALK, ACTOR_MONSTER_RULES_VISITOR, CONDITION_ALWAYS, MSGNO(MsgLobbyMonsterBattleRulesSay)},
	{CUE_TALK,
     ACTOR_MONSTER_WINS_TELLER, CONDITION_ALWAYS,
     SCRIPT_REF(scLinkLobbyReportMonsterRecord)},
	{CUE_TALK,
     ACTOR_CLASS_CHANGE_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbySometimesYourClassChangesWhen)},
	{CUE_TALK,
     ACTOR_SONG_REQUEST_VISITOR, CONDITION_ALWAYS,
     SCRIPT_REF(scLinkLobbyTakeSongRequest)},
	{CUE_TALK, ACTOR_GUTLESS_VISITOR, CONDITION_ALWAYS, MSGNO(MsgLobbyIDontHaveGuts)},
	{CUE_STEP, METATILE_BEHAVIOR_ARENA_MARK, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyEnterBattle)},
	{CUE_STEP, METATILE_BEHAVIOR_WAY_OUT, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyLeaveArena)},
	{CUE_RAISED, LOBBY_CUE_PROMPT, CONDITION_ALWAYS, SCRIPT_REF(scLinkLobbyPromptToParticipate)},
	{CUE_RAISED,
     LOBBY_CUE_DISCONNECTED, CONDITION_ALWAYS,
     SCRIPT_REF(scLinkLobbyReportLinkDisconnected)},
	{CUE_TABLE_END},
};
