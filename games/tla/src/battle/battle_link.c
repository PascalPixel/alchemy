/// @file
/// Link battles: the parties and Djinn exchanged, and the turn handshake.
// Part of battle.c.

#include "battle_internal.h"
#include "system/heap_slots.h"
#include "battle/battle.h"

#define LINK_PARTY_UNIT_BYTES 340
#define LINK_PARTY_OFFER_BYTES 340

/// Unused: receives the opponent's party into the enemy units, "Enemy" before
/// each name, then its Djinn recovery list. Returns how many party members
/// came.
s32 LinkBattle_ReceiveParty(void) {
  u16 name_text[16];
  void *buffer;
  u8 *name_entry;
  s32 named_count;
  s32 index;
  s32 len;
  s32 i;

  buffer = Heap_AllocEwram(LINK_PARTY_UNIT_BYTES);
  named_count = 0;
  index = 0;
  while (index <= 2) {
    name_entry = Unit_Get(index + FIRST_ENEMY_UNIT);
    if (Serial_StartReceive(name_entry) == -1) {
      break;
    }
    Serial_WaitForReceive();
    if (name_entry[298] != 0) {
      named_count += 1;
    }
    wait(2);
    Message_DecodeRawUnboundedFar((s32)MsgEnemyLabel, name_text);
    i = 0;
    if (name_text[i] != 0) {
      do {
        i += 1;
        if (i > NAME_PREFIX_LAST_INDEX) {
          break;
        }
      } while (name_text[i] != 0);
    }
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
    name_text[i] = ' ';
    len = i + 1;
#else
    len = i;
#endif
    for (i = UNIT_NAME_LENGTH; i >= len; i--) {
      name_entry[i] = name_entry[i - len];
    }
    for (i = 0; i < len; i++) {
      name_entry[i] = name_text[i];
    }
    name_entry[UNIT_NAME_LENGTH] = 0;
    index += 1;
  }
  Heap_Release(buffer);
  buffer = Heap_AllocEwram(LINK_PARTY_OFFER_BYTES);
  if (Serial_StartReceive(Djinn_GetRecoveryTableFar(DJINN_RECOVERY_OPPONENT)) !=
      -1) {
    Serial_WaitForReceive();
    wait(2);
  }
  Heap_Release(buffer);
  return named_count;
}

/// Unused: sends the party, then the Djinn recovery list, each unit under the
/// id the opponent knows it by.
s32 LinkBattle_SendParty(void) {
  u8 *buffer;
  struct BattleWork *work;
  s32 j;
  struct DjinnRecoveryList *list;
  s32 size;
  s32 count;
  s32 i;
  s32 mark;
  u16 owners[8];

  size = LINK_PARTY_UNIT_BYTES;
  buffer = (u8 *)Heap_AllocEwram(size);
  mark = OPPONENT_SLOT_NONE;
  work = Ram_HeapSlots->battleWork;
  for (i = 7; i >= 0; i--)
    work->opponentSlots[i] = mark;
  count = BattleParty_PrepareMembers(owners);
  for (i = 0; i < count; i++) {
    Iwram_CopyWordsEntry(buffer, Unit_Get(owners[i]), size);
    ((struct Unit *)buffer)->side = BATTLE_SIDE_PARTY;
    work->opponentSlots[owners[i]] = i - FIRST_ENEMY_UNIT;
    if (Serial_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    Serial_WaitForSend();
    wait(2);
  }
  for (; i <= 2; i++) {
    ((struct Unit *)buffer)->side = BATTLE_SIDE_NONE;
    if (Serial_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    Serial_WaitForSend();
    wait(2);
  }
  size = LINK_PARTY_OFFER_BYTES;
  Heap_Release(buffer);
  buffer = (u8 *)Heap_AllocEwram(size);
  IWRAM_COPY_WORDS(buffer, Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY),
                   size);
  list = &((struct DjinnRecoveryTable *)buffer)->list;
  for (j = 0; j < list->count;) {
    list->entries[j].unitId = work->opponentSlots[list->entries[j].unitId];
    j++;
  }
  if (Serial_StartSend(buffer, LINK_PARTY_OFFER_BYTES) != -1) {
    Serial_WaitForSend();
    wait(1);
    wait(2);
  }
  Heap_Release(buffer);
}

/// Copies a name's first four characters, an underscore for each zero, and
/// returns a pointer to the end. UB: the copy is on the stack; UBFIX keeps it.
u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unusedA, u32 unusedB, u16 *source) {
#ifdef UBFIX
  static u8 sBuffer[5];
  u8 *base = sBuffer;
#else
  u8 buffer[5];
  u8 *base = buffer;
#endif
  u32 fill = '_';
  u8 *current = base;
  u8 *dst = current;
  s32 count;

  for (count = 0; count < 4; count++) {
    u32 value = *source;

    *dst = value;
    source++;
    dst++;
    if ((u8)value == 0)
      *current = fill;
    current++;
  }
  base[4] = 0;
  // BUG: the original buffer is on the stack, gone once this returns.
  // UBFIX keeps the result until the next call in private scratch storage.
  return dst;
}

/// In a link battle, exchanges the "EXEC" signal with the other console.
/// Returns zero if both agree within 30 frames, or -1 on timeout or disconnect.
/// Returns zero immediately outside a link battle.
s32 LinkBattle_WaitSync(void) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  u16 *peer;
  u16 *signature;
  s32 missed = 0;
  s32 i;

  if (battle->isLink != 0) {
    u32 other = battle->linkSide ^ 1;

    // gLinkPeerSignatures[other]: a peer's payload holds four bytes before
    // its signature.
    peer = (u16 *)(gSerialPeerPayloads[other] + 4);
    signature = (u16 *)gSerialTransfer.payload;
    if (battle->linkPaused == 0) {
      // The other console is in step once its second pair reads "EC".
      signature[0] = 'E';
      signature[1] = 'X';
      signature[2] = 'E';
      signature[3] = 'C';

      for (i = 0; i < 30; i++) {
        if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
          missed++;
          if (missed > LINK_MAX_MISSED_FRAMES)
            return -1;
        } else {
          missed = 0;
          if (signature[2] == peer[2] && signature[3] == peer[3])
            return 0;
        }
        wait(1);
      }
    }
    return -1;
  }
  return 0;
}

/// In a link battle, synchronizes successive signals after each action and
/// after the end-of-turn processing.
/// "ex" "TU" → "rn" → "EXEC" → "tu" → "RN"
/// Returns -1 if disconnected, if an earlier signal disagrees with the peer,
/// or after 25 consecutive frames without a connection. Returns zero on
/// agreement, or outside a link battle.
s32 LinkBattle_SyncTurn(void) {
  struct BattleWork *battle = Ram_HeapSlots->battleWork;
  u16 *peer;
  u16 *signature;
  s32 missed = 0;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED u16 spare[10];

  if (battle->isLink != 0) {
    peer = gLinkPeerSignatures[battle->linkSide ^ 1];
    signature = (u16 *)gSerialTransfer.payload;
    if (battle->linkPaused != 0)
      return -1;

    // Publish "ex"/"TU" while both sides still agree on the earlier phase.
    signature[0] = 'e';
    signature[1] = 'x';
    signature[4] = 'T';
    signature[5] = 'U';
    wait(1);
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[2] != peer[2] || signature[3] != peer[3])
          return -1;
        if (signature[0] == peer[0] && signature[1] == peer[1] &&
            signature[4] == peer[4] && signature[5] == peer[5])
          break;
      }
      wait(1);
    }

    // Advance to "rn" only after the peer has reached "TU".
    signature[6] = 'r';
    signature[7] = 'n';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[4] != peer[4] || signature[5] != peer[5])
          return -1;
        if (signature[6] == peer[6] && signature[7] == peer[7])
          break;
      }
      wait(1);
    }

    // Confirm "rn" before publishing the execution signature.
    signature[0] = 'E';
    signature[1] = 'X';
    signature[2] = 'E';
    signature[3] = 'C';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[6] != peer[6] || signature[7] != peer[7])
          return -1;
        if (signature[0] == peer[0] && signature[1] == peer[1] &&
            signature[2] == peer[2] && signature[3] == peer[3])
          break;
      }
      wait(1);
    }

    // Confirm the execution signature before acknowledging with "tu".
    signature[4] = 't';
    signature[5] = 'u';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (signature[0] != peer[0] || signature[1] != peer[1] ||
            signature[2] != peer[2] || signature[3] != peer[3])
          return -1;
        if (signature[4] == peer[4] && signature[5] == peer[5])
          break;
      }
      wait(1);
    }

    // "RN": wait for the other console to move on from "rn".
    signature[6] = 'R';
    signature[7] = 'N';
    for (;;) {
      if ((gLinkStatus & LINK_BOTH_CONNECTED) != LINK_BOTH_CONNECTED) {
        if (++missed > LINK_MAX_MISSED_FRAMES)
          return -1;
      } else {
        missed = 0;
        if (peer[6] != 'r' || peer[7] != 'n')
          break;
      }
      wait(1);
    }
  }
  return 0;
}

/// Records the unit id the link opponent knows each party member by.
s32 BattleParty_AssignOpponentSlots(void) {
  u16 activeMembers[8];
  struct BattleWork *battleState = Ram_HeapSlots->battleWork;
  s32 partySize = BattleParty_PrepareMembers(activeMembers);
  s32 memberSlot;
  s32 unitId;

  for (memberSlot = 0; memberSlot < partySize; memberSlot++) {
    unitId = activeMembers[memberSlot];
    battleState->opponentSlots[unitId] = memberSlot - FIRST_ENEMY_UNIT;
  }
}

/// Clears the first 16 words of the link transfer's payload.
void Battle_ClearSerialTransferPayload(void) {
  Iwram_ClearWordsEntry(gSerialTransfer.payload, 16);
}
