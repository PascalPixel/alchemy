/// @file
/// The Battle Arena's lobby, The Broken Seal's arena reworked, reached from the
/// File Screen's Battle mode. The party waits in a stone hall: the attendant
/// stands behind the counter left of the arched door to the arena, a strongman
/// behind the counter right of it keeps the tally of linked wins, the board
/// above the door shows in three digits the most won in a row, and a healer
/// waits on the carpeted floor above. With a second Game Boy Advance linked,
/// the two players step onto the diamond mark before the arched door, trade
/// their parties and fight; alone, the attendant offers Monster Battles.
/// Holding L or R while talking to the woman who likes her men macho opens a
/// song request, and a few visitors stand below the hall, out of reach: among
/// them a developer's helper who loads debug parameters.

#include "battle/battle.h"
#include "sprite_ids.h"
#include "window/message_argument.h"
#include "window/window.h"
#include "field/field_state.h"
#include "system/frame.h"
#include "sound/sound_driver_symbols.h"
#include "flags.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "system/save.h"
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
#include "game/class_djinn.h"
#include "system/main_routine_state.h"
#include "system/heap_release.h"
#include "system/task.h"
#include "system/input.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "shop/shop.h"

/// A gMainRoutineState.party byte with no member in it, all ones.
#define PARTY_SLOT_EMPTY 255

/// The map's tileset animation of the iron gate in the north wall, run for
/// eight frames as the party walks to it from the diamond mark.
enum { BATTLE_ARENA_GATE_ANIMATION = 5 };

/// The slot a linked battle is requested with, in ENCOUNTER_ZONE_VENUS_DJINN:
/// Granite's; the battle takes its enemies from the opponent's units exchanged
/// before it.
enum { LINK_BATTLE_ENCOUNTER_SLOT = 1 };

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

void Serial_RemoveIrqHandlers(void);
void Serial_Initialize(void);
void Event_Start(void);
void Event_End(void);
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
/// their party slots. The fumbling visitor, the debug helper, the visitor who
/// wins and loses, the one who tells the Monster Battle rules and the song
/// request taker stand at z 376 to 384, below the hall's south wall and past
/// the camera's box, where the player can neither walk nor see them.
enum LinkLobbyActorId {
  ACTOR_ATTENDANT = ACTOR_FIRST_PLACED, ///< arranges every battle
  ACTOR_LINKED_WINS_TELLER,             ///< reports the linked battles won
  ACTOR_HEALER,                         ///< opens a Sanctum's services
  ACTOR_GREETER, ///< "Welcome to the Battle Arena, where..."
  /// "Hm... It comes in like this... Then you do this, and... Whoa, that's
  /// not right..."
  ACTOR_PRACTICING_VISITOR,
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
  ACTOR_SONG_REQUEST_VISITOR, ///< "You have a song request?"
  /// "I don't have the guts to fight in the battle arena."
  ACTOR_GUTLESS_VISITOR
};

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// One linked console's published words, as the link exchange last received
/// them: its signature, then its state (gLinkLobby_SlotColumns gives each
/// LobbySlot's word).
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

/// The hall's cells, as map.txt has them.
enum LobbyCellBehavior {
  /// The hall's south doorway, cell (13, 18): the way out.
  CELL_BEHAVIOR_WAY_OUT = 1,
  /// The diamond mark before the arched door to the arena, cell (13, 11).
  CELL_BEHAVIOR_ARENA_MARK = 10
};

/// The lobby's raised triggers (FieldState.raisedTrigger).
enum LobbyRaisedTrigger {
  LOBBY_EVENT_NONE = 0,
  LOBBY_EVENT_PROMPT = 1,      ///< LinkLobby_PromptToParticipate
  LOBBY_EVENT_DISCONNECTED = 2 ///< LinkLobby_ReportLinkDisconnected
};

/// Frames without a link (LinkLobby_PollPeerReady) or without the partner's
/// request (LinkLobby_EnterBattle) before the lobby gives the partner up.
#define LOBBY_PEER_WAIT_FRAMES 25

/// Frames, five seconds, before the attendant asks the player again to step
/// into the circle.
#define LOBBY_PROMPT_REPEAT_FRAMES 300

/// The attendant has asked the player to take part. Every
/// LOBBY_PROMPT_REPEAT_FRAMES calls of LinkLobby_PromptToParticipate clear it
/// so she asks again.
#define FLAG_LINK_LOBBY_ATTENDANT_ASKED FLAG_SCENE_TEMP_00
/// The linked partner has asked for a battle. Its signature matches and it
/// stands asked or in the circle.
#define FLAG_LINK_LOBBY_PARTNER_ASKED FLAG_SCENE_TEMP_01
/// The player has applied for a battle ("Would you like to apply?").
#define FLAG_LINK_LOBBY_PLAYER_APPLIED FLAG_SCENE_TEMP_02
/// The party is on its way into the battle.
#define FLAG_LINK_LOBBY_ENTERING_BATTLE FLAG_SCENE_TEMP_03
/// ACTOR_PARTY_ORDER_VISITOR has given the linked finals' limit of three
/// allies, so the next talk explains how to change the party's order.
#define FLAG_LINK_LOBBY_PARTY_ORDER_NEXT FLAG_SCENE_TEMP_04
/// The link was lost and the attendant has yet to say so.
#define FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED FLAG_SCENE_TEMP_05
/// A visitor has spoken once this visit, so the rooting visitor goes on to
/// "I'll be rooting..." (LinkLobby_TalkToVisitor).
#define FLAG_LINK_LOBBY_VISITOR_SPOKE FLAG_SCENE_TEMP_06
/// The first of eight flags, one per character from Isaac, each set while that
/// character is in the battle party (LinkLobby_Initialize).
#define FLAG_LINK_LOBBY_IN_PARTY_FIRST FLAG_SCENE_TEMP_F0
/// The attendant has explained the Battle Arena once. Talking to her again
/// offers to apply.
#define FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED FLAG_AREA_TEMP_00
/// This console is the link's child (multiplayer id 1), which receives the
/// parties first.
#define FLAG_LINK_LOBBY_LINK_CHILD FLAG_AREA_TEMP_02
/// The link is up. Both consoles were heard (LOBBY_LINK_READY_MASK).
#define FLAG_LINK_LOBBY_LINK_UP FLAG_AREA_TEMP_03
/// A linked battle has ended since the leader last walked south of z 224, so
/// the party and the visitors speak of it.
#define FLAG_LINK_LOBBY_LINK_BATTLE_ENDED FLAG_AREA_TEMP_04
/// The last linked battle was won.
#define FLAG_LINK_LOBBY_LINK_BATTLE_WON FLAG_AREA_TEMP_05

/// gLinkStatus bits 0 and 1: both consoles were heard this frame.
#define LOBBY_LINK_READY_MASK 3

/// The highest level bonus a monster series reaches (FLAG_MONSTER_BATTLE_WINS,
/// raised by every third win in a row), which battle_formation.c adds to the
/// next monster's level over the party's average.
enum { LEVEL_BONUS_MAX = 90 };

/// The attendant's last offer of the next monster after a monster battle won,
/// "Unbelievable!!! Would you like to fight the next monster?"
/// (MsgLobbyNotBadWouldYouLike + 14); each line before it praises less, and
/// the one after it offers a stronger monster.
enum { MONSTER_OFFER_UNBELIEVABLE = 14 };

/// The lobby's entrances after a battle, which no exit leads to: back from a
/// monster battle won or lost, and from a linked battle won (or broken off) or
/// lost.
enum {
  LOBBY_AFTER_MONSTER_WIN = 8,
  LOBBY_AFTER_MONSTER_LOSS = 9,
  LOBBY_AFTER_LINK_WIN = 10,
  LOBBY_AFTER_LINK_LOSS = 11
};

/// Returns 1 when the partner's payload holds slot's word in slot's column, 0
/// when it does not or there is no link.
s32 LinkLobby_PeerSlotMatches(s32 slot) {
  s32 id = -1;

  if ((gLinkStatus & LOBBY_LINK_READY_MASK) == LOBBY_LINK_READY_MASK) {
    // 0 for the parent, 1 for the child.
    id = SIO_ID_OF(REG_SIOCNT);
    Event_SetFlag(FLAG_LINK_LOBBY_LINK_UP);
  } else {
    Event_ClearFlag(FLAG_LINK_LOBBY_LINK_UP);
  }
  // At most one pass: a match returns 1, anything else breaks out.
  while (id >= 0 && Event_GetFlag(FLAG_LINK_LOBBY_LINK_UP)) {
    const s32 *expected = &gLinkLobby_SlotValues[slot];

    if (id != 0)
      Event_SetFlag(FLAG_LINK_LOBBY_LINK_CHILD);
    else
      Event_ClearFlag(FLAG_LINK_LOBBY_LINK_CHILD);
    {
      struct LinkPeer *row =
          &gLinkPeerSignatures[Event_GetFlag(FLAG_LINK_LOBBY_LINK_CHILD) ^ 1];

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

s32 gLinkLobby_WaitFrames BSS_DATA; ///< frames without the partner
s32 gLinkLobby_CallFrames BSS_DATA;

/// The lobby's task each frame: counts frames without the opponent, clears
/// the peer signatures after 25, and raises the event trigger when both sides
/// are ready or the link is lost. Returns whether the peer answered.
s32 LinkLobby_PollPeerReady(void) {
  struct FieldState *state;
  s32 result;
  s32 i;
  s32 set;
  FIXED z;

  state = Ram_HeapSlots->fieldState;
  result = 1;
  z = Actor_GetById(ACTOR_FELIX)->z.fixed;

  if (z > toFIXED(224))
    Event_ClearFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
  if (state->raisedTrigger != LOBBY_EVENT_DISCONNECTED) {
    LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE);
    set = Event_GetFlag(FLAG_LINK_LOBBY_LINK_UP);
    if (!set) {
      if (++gLinkLobby_WaitFrames > LOBBY_PEER_WAIT_FRAMES) {
        for (i = 0; i < 4; i++)
          Iwram_ClearWordsEntry(&gLinkPeerSignatures[i],
                                sizeof(gSerialTransfer.payload));
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
        Event_SetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
        if (Event_GetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED))
          state->raisedTrigger = LOBBY_EVENT_PROMPT;
        result = 1;
      } else {
        Event_ClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
        result = 0;
      }
    }
    if (Event_GetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED) &&
        Event_GetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED) &&
        !Event_GetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED))
      state->raisedTrigger = LOBBY_EVENT_PROMPT;
  }
  if ((Event_GetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED) ||
       Event_GetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED)) &&
      !Event_GetFlag(FLAG_MONSTER_BATTLE) &&
      !LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE) &&
      gLinkLobby_WaitFrames >= LOBBY_PEER_WAIT_FRAMES) {
    state->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
    Event_SetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
    Event_ClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
    Event_ClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  if (Event_GetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED))
    state->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
  return result;
}

/// Once both linked players have applied for a battle, the attendant behind the
/// left counter says, "If you want to participate, please step into the
/// circle!", and asks again if the player waits.
s32 LinkLobby_PromptToParticipate(void) {
  s32 set;

  set = Event_GetFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
  if (set != 0) {
    return set;
  }
  if (++gLinkLobby_CallFrames == LOBBY_PROMPT_REPEAT_FRAMES) {
    gLinkLobby_CallFrames = 0;
    Event_ClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
  }
  set = Event_GetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
  if (set != 0) {
    return set;
  }
  Event_Start();
  Event_EndLastingPsynergy(0);
  Event_SetMessageId((s32)MsgLobbyIfYouWantToParticipate);
  // "If you want to participate, please step into the circle!"
  Event_OpenMessage(ACTOR_ATTENDANT, 0);
  wait(5);
  Event_SetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
  Event_End();
}

/// The attendant behind the left counter says, "The link was disconnected."
s32 LinkLobby_ReportLinkDisconnected(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  Event_ClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
  Event_ClearFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
  Event_Start();
  Event_EndLastingPsynergy(0);
  state->raisedTrigger = LOBBY_EVENT_NONE;
  Event_SetMessageId((s32)MsgLobbyLinkWasDisconnected);
  // "The link was disconnected."
  Event_OpenMessage(ACTOR_ATTENDANT, 0);
  Event_ClearFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
  Event_End();
}

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
/// A record fails on the 25th frame it waits without a link or past the
/// timeout.
#define LOBBY_TRANSFER_RETRY_LIMIT 24
/// Frames, about ten seconds, for the three party member records, and again
/// for the recovery table.
#define LOBBY_SEND_TIMEOUT_FRAMES 600
/// Frames, about fifteen seconds, for all four records received.
#define LOBBY_RECEIVE_TIMEOUT_FRAMES 900

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
  slot = 0;
  // Three records, each started at recordNext and waited for at recordTest,
  // then the Djinn recovery table at table. Each receive waits a frame at a
  // time; more than size bytes is an overrun and fewer once it ends a short
  // record, and once the timeout has run out, or while the link is not ready,
  // every frame is a try, failing past LOBBY_TRANSFER_RETRY_LIMIT.
  goto recordNext;
recordWait:
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
recordTest:
  if (Serial_GetActiveTransfers() != 0)
    goto recordWait;
  if (gSerialReceivedSize != size) {
    result = -1;
    goto done;
  }
  if (unit->side != BATTLE_SIDE_NONE)
    result++;
  wait(2);
  FieldMessage_DecodeRawUnbounded((s32)MsgEnemyLabel, namePrefix);
  for (i = 0; i <= BATTLE_ARENA_PREFIX_LAST && namePrefix[i] != 0; i++)
    ;
#if BATTLE_ARENA_PREFIX_LAST == 7
  namePrefix[i] = ' ';
  length = i + 1;
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
  unit = Unit_Get(slot + FIRST_ENEMY_UNIT);
  transfer = Serial_StartReceive(unit);
  tries = 0;
  if (transfer == -1)
    goto failed;
  goto recordTest;
tableWait:
  {
    s32 limit = LOBBY_RECOVERY_RECORD_BYTES;

    if (gSerialReceivedSize > limit) {
      result = -1;
      goto done;
    }
  }
  wait(1);
  if (--timeout < 0 ||
      (gLinkStatus & LOBBY_LINK_READY_MASK) != LOBBY_LINK_READY_MASK) {
    if (++tries > LOBBY_TRANSFER_RETRY_LIMIT) {
      result = -1;
      goto done;
    }
  }
tableTest:
  if (Serial_GetActiveTransfers() != 0)
    goto tableWait;
  if (gSerialReceivedSize != LOBBY_RECOVERY_RECORD_BYTES)
    result = -1;
  else
    wait(2);
  goto done;
table:
  Heap_Release(heap);
  size = LOBBY_RECOVERY_RECORD_BYTES;
  heap = Heap_AllocEwram(size);
  transfer = Serial_StartReceive(Djinn_GetRecoveryTable(1));
  tries = 0;
  if (transfer != -1)
    goto tableTest;
failed:
  result = transfer;
done:
  Heap_Release(heap);
  return result;
}

/// Writes up to three active party owners to out, ending the list with
/// PARTY_SLOT_EMPTY.
s32 LinkLobby_CopyPartyOwnerIds(u16 *out) {
  s32 count = Party_CountMembers();
  s32 i;

  if (count > 3)
    count = 3;
  for (i = 0; i < count; i++) {
    u8 owner = gMainRoutineState.party[i];

    if (out != NULL)
      *out++ = owner;
  }
  if (out != NULL)
    *out = PARTY_SLOT_EMPTY;
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
  // Each send waits a frame at a time to finish. Once the timeout has run out,
  // or while the link is not ready, every frame is a try, and past
  // LOBBY_TRANSFER_RETRY_LIMIT tries the exchange fails. A send that cannot
  // start leaves the block.
  do {
    for (i = 0; i < memberCount; i++) {
      s32 tries = 0;

      Iwram_CopyWordsEntry(heap, Unit_Get(memberIds[i]), size);
      ((struct Unit *)heap)->side = BATTLE_SIDE_PARTY;
      peerSlots[memberIds[i]] = FIRST_ENEMY_UNIT + i;
      if ((transferResult = Serial_StartSend(heap, size)) == -1) {
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
    // Pad to three records with the last copy marked absent.
    for (; i < LOBBY_UNIT_RECORD_COUNT; i++) {
      ((struct Unit *)heap)->side = tries = 0;
      if ((transferResult = Serial_StartSend(heap, size)) == -1)
        break;
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
    if (i < LOBBY_UNIT_RECORD_COUNT) {
      result = transferResult;
      break;
    }
    // Then the recovery table.
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
      // Give each Djinni on the list its party member's number on the
      // partner's side, and drop those of party members not sent.
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
        break;
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
      wait(1);
      wait(2);
    }
  } while (0);
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
  state->status = SERIAL_STATUS_RUNNING;
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
  flag = Event_GetFlag(FLAG_LINK_LOBBY_LINK_CHILD);
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

void TilesetAnim_Resume(s32 channel);
void TilesetAnim_Pause(s32 channel);
void Field_RequestBattleFromEncounterTable(s32 table, s32 slot);

/// Felix steps onto the diamond mark before the arched door to the arena, cell
/// (13, 11), once he has applied: "Awaiting your opponent..." shows until the
/// other player steps on theirs, the attendant says "Good luck!", Felix walks
/// to the door, the two parties are traded over the link and the battle
/// starts. After choosing a Monster Battle, the mark starts it at once.
s32 LinkLobby_EnterBattle(void) {
  struct FieldState *state;
  s32 failed;
  struct Window *window;
  s32 framesUnanswered;
  s32 stop;
  u8 *savedUnits;
  u16 *table;
  u32 i;

  failed = 0;
  window = 0;
  state = Ram_HeapSlots->fieldState;
  framesUnanswered = 0;
  if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
    Event_Start();
    Event_EndLastingPsynergy(0);
  } else {
    if (!Event_GetFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED))
      return;
    if (Event_GetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED))
      return;
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_SetFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IN_CIRCLE);
    if (!LinkLobby_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE))
      // "Awaiting your opponent..."
      window = Message_OpenWindow((s32)MsgLobbyAwaitingYourOpponent, 5, 4,
                                  MESSAGE_WINDOW_ZOOM);
    while (!LinkLobby_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE)) {
      wait(1);
      stop = 0;
      if (!Event_GetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED))
        stop = 1;
      if (Event_GetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED))
        stop = 1;
      if (!LinkLobby_PeerSlotMatches(LOBBY_SLOT_IN_CIRCLE) &&
          !LinkLobby_PeerSlotMatches(LOBBY_SLOT_ASKED)) {
        if (++framesUnanswered > LOBBY_PEER_WAIT_FRAMES)
          stop = 1;
      } else {
        framesUnanswered = 0;
      }
      if (stop) {
        state->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
        Event_SetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
        Event_ClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
        Event_ClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
        LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
        failed = 1;
        Event_ClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
        break;
      }
    }
    if (window)
      Window_Close(window, 1);
    wait(5);
  }
  if (!failed) {
    // The exchange below receives the opponent's party members and Djinn into
    // units FIRST_ENEMY_UNIT on; what those units held is kept here meanwhile.
    savedUnits =
        HeapSlot_AllocEwram(HEAP_SLOT_ENEMY_UNITS, 6 * BATTLE_UNIT_SIZE);
    Task_Remove(LinkLobby_PollPeerReady);
    TilesetAnim_Resume(BATTLE_ARENA_GATE_ANIMATION);
    wait(8);
    TilesetAnim_Pause(BATTLE_ARENA_GATE_ANIMATION);
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineState.leaderActorId, 0);
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
        TilesetAnim_Resume(BATTLE_ARENA_GATE_ANIMATION);
        wait(8);
        TilesetAnim_Pause(BATTLE_ARENA_GATE_ANIMATION);
        Actor_WaitForMoveById(ACTOR_FELIX);
        HeapSlot_Release(HEAP_SLOT_ENEMY_UNITS);
        LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
        LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
        Task_Add((TaskFunc)LinkLobby_PollPeerReady, TASK_PASS_FRAME);
        Task_SetFlags(LinkLobby_PollPeerReady, 1);
        Event_ClearFlag(FLAG_LINK_LOBBY_PARTNER_ASKED);
        Event_ClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
        Event_ClearFlag(FLAG_LINK_LOBBY_LINK_UP);
        Event_ClearFlag(FLAG_LINK_LOBBY_ENTERING_BATTLE);
        Event_ClearFlag(FLAG_LINK_LOBBY_ATTENDANT_ASKED);
        state->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
        Event_End();
        return;
      }
      Actor_SetSpeed(ACTOR_FELIX, 0.5);
      Actor_WaitForMoveById(ACTOR_FELIX);
    }
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      Event_SetAfterBattleDestination(SCENE_LOBBY, LOBBY_AFTER_MONSTER_WIN);
      Event_SetDefeatDestination(SCENE_LOBBY, LOBBY_AFTER_MONSTER_LOSS);
    } else {
      Event_SetAfterBattleDestination(SCENE_LOBBY, LOBBY_AFTER_LINK_WIN);
      Event_SetDefeatDestination(SCENE_LOBBY, LOBBY_AFTER_LINK_LOSS);
    }
    gMainRoutineState.battleType = BATTLE_TYPE_LINK;
    Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_VENUS_DJINN,
                                          LINK_BATTLE_ENCOUNTER_SLOT);
    // EXEC fills the payload's first four halfwords, where the link battle's
    // command entry looks for it in the partner's.
    table = (u16 *)gSerialTransfer.payload;
    table[0] = 'E';
    table[1] = 'X';
    table[2] = 'E';
    table[3] = 'C';
    // The six units go to the battle's actor sprite slots, where the battle
    // takes them from.
    for (i = 0; i < 6 * BATTLE_UNIT_SIZE; i++)
      (Ram_MapCellBuffer + 32 * 1024)[i] = savedUnits[i];
    HeapSlot_Release(HEAP_SLOT_ENEMY_UNITS);
  }
  Event_End();
}

/// Felix talks to the attendant behind the counter left of the arched door:
/// with no other player linked, "You do not have an opponent. Would you like
/// to battle a monster?"; with one, she explains the Battle Arena the first
/// time and then asks, "Would you like to apply?"
s32 LinkLobby_ChooseBattleMode(void) {
  s32 answer;
  s32 msg;
  s32 line;

  msg = (s32)MsgLobbyPleaseSpeakToMeWhen;
  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineState.leaderActorId, 0);
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
      // Yes starts a monster series: FLAG_AREA_TEMP_E8's byte, which counts
      // the victories towards a stronger monster, and LINK_TALLY_MONSTER_STREAK
      // start at 0.
      if (answer == 0) {
        FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, 0);
        Event_SetFlag(FLAG_MONSTER_BATTLE);
        Event_ClearFlag(FLAG_LINK_BATTLE_ARRANGED);
        Event_ClearFlag(FLAG_LINK_BATTLE);
        Event_SetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
        // "When you finish your preparations, please step into the circle!"
        line = msg + 7;
        gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK] = answer;
        goto message;
      }
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
      Event_SetFlag(FLAG_LINK_BATTLE);
      LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      // "Please wait until an opponent arrives, then."
      line = msg + 6;
      goto message;
    } else {
      // "When you finish your preparations, please step into the circle!"
      line = msg + 3;
      goto message;
    }
  } else {
    if (Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      Event_SetMessageId((s32)MsgLobbyYourOpponentHasArrived);
      // "Your opponent has arrived!"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      Event_ClearFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
    }
    if (Event_GetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED)) {
      // "When you finish your preparations, please step into the circle!"
      line = msg + 3;
    message:
      Event_SetMessageId(line);
    open_message:
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
    } else if (!Event_GetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED) &&
               !Event_GetFlag(FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED)) {
      Event_SetMessageId(msg);
      // "Please speak to me when you want to fight in a battle."
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      Event_SetFlag(FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED);
    } else {
      Event_SetFlag(FLAG_LINK_LOBBY_ATTENDANT_EXPLAINED);
      if (Event_GetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED))
        Event_SetMessageId(msg + 2);
      else
        Event_SetMessageId(msg + 1);
      // "Your opponent has asked for a battle. Do you accept?", or with no
      // request yet "Would you like to apply?"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
        if (LinkLobby_PeerSlotMatches(LOBBY_SLOT_SIGNATURE)) {
          Event_SetFlag(FLAG_LINK_BATTLE);
          Event_SetFlag(FLAG_LINK_BATTLE_ARRANGED);
          // "When you finish your preparations, please step into the
          // circle!", or with no request yet "You have asked for a battle. We
          // will call your name if your opponent accepts. Please wait."
          if (Event_GetFlag(FLAG_LINK_LOBBY_PARTNER_ASKED))
            Event_SetMessageId(msg + 3);
          else
            Event_SetMessageId(msg + 4);
          LinkLobby_WriteSlotValue(LOBBY_SLOT_ASKED);
          Event_SetFlag(FLAG_LINK_LOBBY_PLAYER_APPLIED);
          goto open_message;
        } else {
          Event_SetFlag(FLAG_LINK_LOBBY_LINK_LOST_UNREPORTED);
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

/// Felix talks to the man behind the counter right of the arched door: across
/// the counter, from the south, he tells the linked battles won in all; from
/// any other side, the most won in a row, which the board on the wall shows.
s32 LinkLobby_ReportWinTallies(s32 actor) {
  u32 facing = Actor_GetById(ACTOR_FELIX)->facing;
  s32 base;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceActor(actor, gMainRoutineState.leaderActorId, 0);
  if (facing > FACING_NORTHWEST && facing < FACING_NORTHEAST) {
    base = (s32)MsgLobbyItLooksLike;
    if (gMainRoutineState.linkTallies[LINK_TALLY_LINKED_WINS] != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(
          gMainRoutineState.linkTallies[LINK_TALLY_LINKED_WINS],
          MESSAGE_ARGUMENT_NUMBER);
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
    if (gMainRoutineState.linkTallies[LINK_TALLY_LINKED_STREAK] != 0) {
      MessageArgument_ClearAll();
      MessageArgument_Push(
          gMainRoutineState.linkTallies[LINK_TALLY_LINKED_STREAK],
          MESSAGE_ARGUMENT_NUMBER);
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

void WindowSprite_PrepareForRedraw(struct Window *window);
void Text_DrawNumber(s32 value, s32 digits, struct Window *window, s32 x,
                     s32 y);
s32 DebugParameterSet_Load();

/// A developer's leftover: the white-bearded man in green placed below the
/// hall, where the player cannot walk, says "Choose your parameters here." and
/// lets the player pick a debug parameter set by number with Left and Right.
/// The man at the right counter answers, "The settings are made. The rest is up
/// to you.", or for a set that does not exist, "There's no such parameter. Ask
/// Mr. Sugimoto."
s32 LinkLobby_ChooseDebugParameterSet(s32 actorId) {
  struct Window *window;
  s32 choice;
  s32 shown;
  s32 ok;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceActor(actorId, gMainRoutineState.leaderActorId, 0);
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
    if (gInput.pressed & KEY_A) {
      break;
    }
    if (gInput.pressed & KEY_B) {
      choice = -1;
      break;
    }
    wait(1);
  }
  Window_Close(window, 1);
  if (choice >= 0) {
    ok = DebugParameterSet_Load(choice);
  } else {
    Event_SetMessageId((s32)MsgLobbyWhatIsThisJustTease);
    // "What is this, just a tease?"
    Event_ShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
    wait(10);
    Event_End();
    return;
  }
  if (ok != 0) {
    Event_SetMessageId((s32)MsgLobbyTheresNoSuchParameterAsk);
    // "There's no such parameter. Ask Mr. Sugimoto."
    Event_ShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
  } else {
    Event_SetMessageId((s32)MsgLobbySettingsAreMadeRest);
    // "The settings are made. The rest is up to you."
    Event_ShowMessage(ACTOR_LINKED_WINS_TELLER, 0);
  }
  wait(10);
  Event_End();
}

/// Felix talks to the healer on the carpeted floor above the hall: she offers a
/// Sanctum's services.
void LinkLobby_OpenSanctum(s32 actor) {
  Event_Start();
  Event_EndLastingPsynergy(0);
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
    if (gMainRoutineState.party[i] == PARTY_SLOT_EMPTY)
      return 0;
    if (gMainRoutineState.party[i] == owner)
      return 1;
  }
  return 0;
}

/// Unused: does nothing; nothing calls it.
UNUSED void LinkLobby_DoNothing(void) {}

extern u8 MsgLobbyYouHaveSongRequest[];
extern u8 MsgLobbyComeBackWhenYoureReady[];
/// The sounds the song request offers, numbered from 0 in its window.
#define SONG_REQUEST_SOUNDS 97
extern u16 gLinkLobby_SongRequestSounds[SONG_REQUEST_SOUNDS];

s32 SoundFlag_Test(s32 song);

/// "You have a song request?": the player picks a piece of music or a sound by
/// number with Left and Right and hears it with A, until B ends it with "Come
/// back when you're ready to rock!"
///
/// Only the ones already heard in the game can be picked, unless Start, Up and
/// L are held as the request opens, or this game or a file on the cartridge has
/// been cleared. The woman in red takes requests while L or R is held
/// (LinkLobby_TalkToVisitor); ACTOR_SONG_REQUEST_VISITOR, a strongman, stands
/// below the hall where the player cannot walk.
void LinkLobby_TakeSongRequest(s32 actor) {
  struct Window *window;
  s32 song;
  s32 shown;
  s32 step;
  s32 all;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceActor(actor, gMainRoutineState.leaderActorId, 0);
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
      for (; step != 0;) {
        song += step;
        if (song < 0)
          song = SONG_REQUEST_SOUNDS - 1;
        if ((u32)song > SONG_REQUEST_SOUNDS - 1)
          song = 0;
        if (all != 0 || gMainRoutineState.sendFlag != 0 || gAnyFileCleared != 0)
          break;
        if (SoundFlag_Test(gLinkLobby_SongRequestSounds[song]) != 0)
          break;
      }
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
    Event_PlaySound(gLinkLobby_SongRequestSounds[song]);
  }
  Event_PlaySound(SOUND_FADE_OUT_BGM);
  wait(120);
  Event_PlaySound(MUS_TRY_YOUR_LUCK);
  /* FAKEMATCH(dead): the statement in a once-only block. */
  do {
    Event_SetMessageId((s32)MsgLobbyComeBackWhenYoureReady);
  } while (0);
  // "Come back when you're ready to rock!"
  Event_ShowMessage(actor, 0);
  wait(10);
  Event_End();
}

void MapGrid_CenterOnFollowedPoint(void);
void MapGrid_CopyCellTilesIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY,
                                      s32 width, s32 height);
void Save_WriteLinkTallies(void);
/// Leaves for an entrance of a scene (Event_SetDestination, through the
/// overlay's own import row).
void Import_EventSetDestination(s32 scene, s32 entrance);
void Actor_FaceActorById(s32 actor, s32 target, s32 frames);
void FieldMessage_Show(s32 no, s32 flags);

/// Felix talks to a member of his party waiting in the hall: what they say
/// follows whether they will fight, and how the last linked battle went.
s32 LinkLobby_TalkByProgress(s32 actor) {
  s32 base = (s32)MsgLobbyItsOurTurnLetsGo;
  s32 lead = LinkLobby_PartyContains(ACTOR_FELIX);
  s32 own = LinkLobby_PartyContains(actor);

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceActor(actor, gMainRoutineState.leaderActorId, 0);
  if (Event_GetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED)) {
    // DEAD: Felix's own flag is read and dropped; the ROM keeps the call.
    Event_GetFlag(FLAG_LINK_LOBBY_IN_PARTY_FIRST + ACTOR_FELIX);
    base = Event_GetFlag(actor + FLAG_LINK_LOBBY_IN_PARTY_FIRST);
    if (Event_GetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_WON)) {
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
  if (base == (s32)MsgLobbyIllFightEnoughForBoth && actor == ACTOR_JENNA) {
    FieldActor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
    Event_Wait(10);
  }
  Event_End();
}

/// Felix walks out of the hall's south doorway, cell (13, 18): the link ends
/// and the game returns to the File Screen.
///
/// The serial handlers come off and the sound's normal reverb is loaded again
/// before the scene leaves for SCENE_CLEAR's entrance 1.
s32 LinkLobby_LeaveArena(void) {
  Serial_RemoveIrqHandlers();
  Sound_LoadReverbCode(SOUND_REVERB_NORMAL);
  Import_EventSetDestination(SCENE_CLEAR, 1);
#ifdef UBFIX
  return 0;
#endif
}

/// Felix talks to the woman in the striped top at the west of the hall, "You
/// look a little scrawny, but guys like you are often surprisingly strong...",
/// and the woman in red beside her turns to him: "Fighting a guy with a build
/// like that would really wear me out! Whew!" Once a visitor has spoken this
/// visit, she says "I'll be rooting..." instead. Talking to the woman in red
/// gives "I like my men macho", or with L or R held, a song request.
s32 LinkLobby_TalkToVisitor(s32 actor) {
  s32 message;

  if (actor == ACTOR_MACHO_LOVING_VISITOR && (gInput.held & KEYS_SHOULDERS)) {
    LinkLobby_TakeSongRequest(ACTOR_MACHO_LOVING_VISITOR);
    return;
  }
  Event_Start();
  Event_EndLastingPsynergy(0);
  if (Event_GetFlag(FLAG_LINK_LOBBY_VISITOR_SPOKE)) {
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
    Event_SetFlag(FLAG_LINK_LOBBY_VISITOR_SPOKE);
  }
  Event_SetMessageId(message);
  // "Sometimes, your class changes when you unleash a Djinni. You need to..."
  Event_OpenMessage(actor, 0);
  Event_Wait(1);
  if (message == (s32)MsgLobbyYouLookLittleScrawny) {
    Actor_StartRepeatedMotion(ACTOR_MACHO_LOVING_VISITOR, 1);
    Actor_SetAnimation(ACTOR_MACHO_LOVING_VISITOR, 4);
    Event_Wait(15);
    Actor_FaceActor(ACTOR_MACHO_LOVING_VISITOR, gMainRoutineState.leaderActorId,
                    0);
    Event_Wait(5);
    Event_OpenMessage(ACTOR_MACHO_LOVING_VISITOR, 0);
  }
  Event_End();
}

/// Felix talks to the fair-haired boy on the raised floor at the west of the
/// hall: he explains that up to three allies fight in a linked battle, in
/// words that follow the party's size, and the next time, how to change the
/// party's order on the status screen.
s32 LinkLobby_ExplainPartyOrder(s32 actor) {
  s32 message;

  Event_Start();
  Event_EndLastingPsynergy(0);
  Actor_FaceActor(actor, gMainRoutineState.leaderActorId, 0);
  if (!Event_GetFlag(FLAG_LINK_LOBBY_PARTY_ORDER_NEXT)) {
    if (Party_CountMembers() == 8)
      message = (s32)MsgLobbyUpToThreeAlliesCan;
    else if (Party_CountMembers() <= 3)
      message = (s32)MsgLobbyUpToThreeAlliesCanFight;
    else
      message = (s32)MsgLobbyUpToThreeAlliesCanFightInLinked;
    Event_SetFlag(FLAG_LINK_LOBBY_PARTY_ORDER_NEXT);
  } else {
    message = (s32)MsgLobbyYouCanChangeOrder;
    Event_ClearFlag(FLAG_LINK_LOBBY_PARTY_ORDER_NEXT);
  }
  Event_SetMessageId(message);
  // "You can change the order of your party in the status screen. But you..."
  Event_OpenMessage(actor, 0);
  Event_End();
}

/// Felix talks to the old man with a cane right of the arched door: he tells
/// the most Monster Battles won in a row, or before any, "I can't wait to see
/// how many Monster Battles you win."
s32 LinkLobby_ReportMonsterRecord(s32 id) {
  struct MainRoutineState *gs;

  Event_Start();
  Event_EndLastingPsynergy(0);
  gs = &gMainRoutineState;
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

/// Saves and reports the link battle's results.
s32 LinkLobby_SaveBattleResults(void) {
  Save_WriteLinkTallies();
  FieldMessage_Show(MsgLobbyBattleResultsWereSaved, 1);
}

/// Saves and reports the monster battle's results.
s32 LinkLobby_SaveMonsterBattleResults(void) {
  Save_WriteLinkTallies();
  FieldMessage_Show(MsgLobbyMonsterBattleResultsWereSaved, 1);
}

/// The most the wall board's three digits show.
#define WALL_BOARD_MAX 999

/// Shows value, at most WALL_BOARD_MAX, on the wall board, as three digits
/// from right to left.
void LinkLobby_DrawThreeDigitValue(s32 value) {
  s32 digit;

  if (value > WALL_BOARD_MAX)
    value = WALL_BOARD_MAX;
  for (digit = 0; digit <= 2; digit++) {
    MapGrid_CopyCellsTo(27, value % 10, 16 - digit, 8, 1, 1);
    value = value / 10;
  }
  MapGrid_CenterOnFollowedPoint();
}

void Party_RestoreHpPp(s32 value);
void Sound_LoadReverbCode(s32 preset);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

/// Sets the lobby up as The Broken Seal's BattleArena_Initialize sets the arena
/// up.
s32 LinkLobby_Initialize(void) {
  s32 i;
  s32 wins;

  Event_SetFlag(FLAG_IN_BATTLE_ARENA);
  gMainRoutineState.firstShortcut = 0;
  gMainRoutineState.secondShortcut = 0;
  gLinkLobby_CallFrames = 0;
  gLinkLobby_WaitFrames = 0;
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
  Sound_LoadReverbCode(SOUND_REVERB_NORMAL);
  LinkLobby_DrawThreeDigitValue(
      gMainRoutineState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
  MapGrid_CopyCellAttributes(11, 11, 1, 1, 13, 10);
  LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  wait(1);
  TilesetAnim_Pause(BATTLE_ARENA_GATE_ANIMATION);
  {
    /* FAKEMATCH(temporary): name holds the payload's address, viewed as
     * halfwords. */
    u16 *name = (u16 *)gSerialTransfer.payload;

    name[4] = 'T';
    name[5] = 'A';
    name[6] = 'L';
    name[7] = 'K';
  }
  for (i = 0; i < 8; i++) {
    Event_ClearFlag(FLAG_LINK_LOBBY_IN_PARTY_FIRST + i);
    if (LinkLobby_PartyContains(i))
      Event_SetFlag(FLAG_LINK_LOBBY_IN_PARTY_FIRST + i);
  }
  if (gMainRoutineState.entrance == LOBBY_AFTER_MONSTER_WIN) {
    s32 msg;
    s32 v;

    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(LOBBY_SLOT_ALONE);
    gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_WINS]++;
    gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK]++;
    // The offer has two lines a level (FLAG_MONSTER_BATTLE_WINS, -1 when a
    // series starts), the even line capped at MONSTER_OFFER_UNBELIEVABLE.
    // FLAG_AREA_TEMP_E8's byte counts the victories 0 to 2; the third raises
    // the level and takes the next, odd line, which promises a stronger
    // monster.
    wins = (s8)GameFlag_GetByte(FLAG_MONSTER_BATTLE_WINS);
    msg = wins * 2 + 2;
    if (msg > MONSTER_OFFER_UNBELIEVABLE)
      msg = MONSTER_OFFER_UNBELIEVABLE;
    v = FieldGameFlag_GetByte(FLAG_AREA_TEMP_E8);
    if (v == 2) {
      FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, 0);
      wins++;
      msg++;
    } else {
      FieldGameFlag_SetByte(FLAG_AREA_TEMP_E8, v + 1);
    }
    Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineState.leaderActorId, 0);
    Event_SetMessageId((s32)MsgLobbyNotBadWouldYouLike + msg);
    Event_OpenMessage(ACTOR_ATTENDANT, 0);
    if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
      if (wins > LEVEL_BONUS_MAX)
        wins = LEVEL_BONUS_MAX;
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, wins);
    } else {
      Event_ClearFlag(FLAG_MONSTER_BATTLE);
      FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
      MessageArgument_Push(
          gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK],
          MESSAGE_ARGUMENT_NUMBER);
      if (gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
          gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
        gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
            gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK];
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
  } else if (gMainRoutineState.entrance == LOBBY_AFTER_MONSTER_LOSS) {
    gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_LOSSES]++;
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(LOBBY_SLOT_ALONE);
    Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineState.leaderActorId, 0);
    MessageArgument_Push(
        gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK],
        MESSAGE_ARGUMENT_NUMBER);
    if (gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] <
        gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK]) {
      gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_BEST_STREAK] =
          gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK];
      Event_SetMessageId((s32)MsgLobbyYouWonBattleYouBroke);
      // "You won ... battle. You broke the record!"
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
      LinkLobby_SaveMonsterBattleResults();
    } else {
      Event_SetMessageId((s32)MsgLobbyYouWonBattleITold);
      // "You won ... battle. I told you our monsters were tough! Please..."
      Event_OpenMessage(ACTOR_ATTENDANT, 0);
    }
    gMainRoutineState.linkTallies[LINK_TALLY_MONSTER_STREAK] = 0;
    Event_ClearFlag(FLAG_MONSTER_BATTLE);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    Event_End();
  } else if (gMainRoutineState.entrance == LOBBY_AFTER_LINK_WIN) {
    s32 v;

    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
    // FLAG_AREA_TEMP_E8: the linked battle broke off rather than being won.
    if (Event_GetFlag(FLAG_AREA_TEMP_E8)) {
      struct FieldState *state = Ram_HeapSlots->fieldState;

      Event_ClearFlag(FLAG_AREA_TEMP_E8);
      state->raisedTrigger = LOBBY_EVENT_DISCONNECTED;
      Event_ClearFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
      wait(20);
      LinkLobby_InitializeSerial();
      LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
      LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
    } else {
      gMainRoutineState.linkTallies[LINK_TALLY_LINKED_WINS]++;
      v = gMainRoutineState.linkTallies[LINK_TALLY_LINKED_STREAK] + 1;
      gMainRoutineState.linkTallies[LINK_TALLY_LINKED_STREAK] = v;
      if (gMainRoutineState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] < (u16)v)
        gMainRoutineState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK] = v;
      LinkLobby_DrawThreeDigitValue(
          gMainRoutineState.linkTallies[LINK_TALLY_LINKED_BEST_STREAK]);
      LinkLobby_SaveBattleResults();
      Event_SetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
      Event_SetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_WON);
    }
    Event_End();
  } else if (gMainRoutineState.entrance == LOBBY_AFTER_LINK_LOSS) {
    Event_Start();
    Event_EndLastingPsynergy(0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
    if (!Event_GetFlag(FLAG_MONSTER_BATTLE)) {
      gMainRoutineState.linkTallies[LINK_TALLY_LINKED_LOSSES]++;
      gMainRoutineState.linkTallies[LINK_TALLY_LINKED_STREAK] = 0;
      LinkLobby_SaveBattleResults();
    }
    Event_SetFlag(FLAG_LINK_LOBBY_LINK_BATTLE_ENDED);
    Event_ClearFlag(FLAG_LINK_LOBBY_LINK_BATTLE_WON);
    Event_End();
  } else {
    Serial_Initialize();
    Event_ClearFlag(FLAG_LINK_BATTLE_ARRANGED);
    FieldGameFlag_SetByte(FLAG_MONSTER_BATTLE_WINS, -1);
    if (gMainRoutineState.idleSleepEnabled != 0) {
      Event_Start();
      Event_EndLastingPsynergy(0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Actor_FaceActor(ACTOR_ATTENDANT, gMainRoutineState.leaderActorId, 0);
      Event_SetMessageId((s32)MsgLobbyNoteYouCantUseSleep);
      // "Note: you can't use the sleep function in battle stages!"
      Event_ShowMessage(ACTOR_ATTENDANT, 0);
      Event_End();
    }
    gMainRoutineState.idleSleepEnabled = 0;
    gIdleSleepEnabled = 0;
    LinkLobby_WriteSlotValue(LOBBY_SLOT_SIGNATURE);
    LinkLobby_WriteSlotValue(LOBBY_SLOT_IDLE);
  }
  Task_Add((TaskFunc)LinkLobby_PollPeerReady, TASK_PASS_FRAME);
  Task_SetFlags((TaskFunc)LinkLobby_PollPeerReady, 1);
  if (gMainRoutineState.entrance != LOBBY_AFTER_MONSTER_WIN ||
      !Event_GetFlag(FLAG_MONSTER_BATTLE)) {
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

/// The music and sounds the song request offers, numbered from 0 in its window.
u16 gLinkLobby_SongRequestSounds[] = {
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

struct SceneEntrance gSceneEntrances[] = {
    {LOBBY_AFTER_LINK_WIN, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {LOBBY_AFTER_LINK_LOSS, CONDITION_ALWAYS, 336, 0, 164, FACING_NORTH, 0, 16, 0, 416, 310, 0},
    {LOBBY_AFTER_MONSTER_WIN, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
    {LOBBY_AFTER_MONSTER_LOSS, CONDITION_ALWAYS, 216, 0, 200, FACING_WEST, 0, 16, 0, 416, 310, 0},
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
    {TRIGGER_TALK, ACTOR_PRACTICING_VISITOR, CONDITION_ALWAYS,
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
    {TRIGGER_TALK, ACTOR_SONG_REQUEST_VISITOR, CONDITION_ALWAYS,
     EVENT(LinkLobby_TakeSongRequest)},
    {TRIGGER_TALK, ACTOR_GUTLESS_VISITOR, CONDITION_ALWAYS,
     MSGNO(MsgLobbyIDontHaveGuts)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ARENA_MARK, CONDITION_ALWAYS,
     EVENT(LinkLobby_EnterBattle)},
    {TRIGGER_STEP, CELL_BEHAVIOR_WAY_OUT, CONDITION_ALWAYS,
     EVENT(LinkLobby_LeaveArena)},
    {TRIGGER_RAISED, LOBBY_EVENT_PROMPT, CONDITION_ALWAYS,
     EVENT(LinkLobby_PromptToParticipate)},
    {TRIGGER_RAISED, LOBBY_EVENT_DISCONNECTED, CONDITION_ALWAYS,
     EVENT(LinkLobby_ReportLinkDisconnected)},
    {TRIGGER_TABLE_END},
};
