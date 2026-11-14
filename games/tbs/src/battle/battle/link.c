/// @file
/// Link battles: the parties and Djinn exchanged, and the turn handshake.
// Part of battle.c.

#include "../battle_internal.h"

// The longest "Enemy" label the opponent's names take before them; the
// European editions also put a space after it.
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||         \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define NAME_LIMIT 7
#define APPEND_NAME_SPACE 1
#else
#define NAME_LIMIT 4
#define APPEND_NAME_SPACE 0
#endif

/// What a link battle sends of each party member, and of the Djinn recovery
/// list, in bytes.
enum LinkPartyPacketSize {
  /// A struct Unit (BATTLE_UNIT_SIZE, 332 bytes) and the 8 after it.
  LINK_PARTY_UNIT_BYTES = 340,
  /// A struct DjinnRecoveryTable (268 bytes) and the 52 after it.
  LINK_PARTY_OFFER_BYTES = 320
};

/// Unused: receives the opponent's party into the enemy units, "Enemy" before
/// each name, then its Djinn recovery list. Returns how many party members
/// came.
s32 LinkBattle_ReceiveParty(void) {
  u16 label[24];
  void *buffer;
  struct Unit *opponent;
  s32 memberCount;
  s32 partySlot;
  s32 labelLength;
  s32 i;

  // XXX (no evidence): what the block is for; each party member is received
  // straight into its enemy unit, and the block is only freed.
  buffer = Heap_AllocEwram(LINK_PARTY_UNIT_BYTES);
  memberCount = 0;
  for (partySlot = 0; partySlot < LINK_PARTY_SIZE; partySlot++) {
    opponent = Unit_GetFar(partySlot + FIRST_ENEMY_UNIT);
    if (Serial_StartReceive(opponent) == -1) {
      break;
    }
    Serial_WaitForReceive();
    if (opponent->side != 0) {
      memberCount += 1;
    }
    wait(2);

    // Measure the label (at most NAME_LIMIT + 1 characters), shift the name
    // right to make room for it, then write it in; the name's end is cut.
    Message_DecodeRawUnboundedFar(MsgEnemyLabel, label);
    for (i = 0; i <= NAME_LIMIT; i++) {
      if (label[i] == 0)
        break;
    }
#if APPEND_NAME_SPACE
    label[i] = ' ';
    i += 1;
#endif
    labelLength = i;
    for (i = UNIT_NAME_LENGTH; i >= labelLength; i--) {
      opponent->name[i] = opponent->name[i - labelLength];
    }
    for (i = 0; i < labelLength; i++) {
      opponent->name[i] = label[i];
    }
    opponent->name[UNIT_NAME_LENGTH] = 0;
  }
  Heap_Release(buffer);

  // The recovery packet follows all three party places.
  buffer = Heap_AllocEwram(LINK_PARTY_OFFER_BYTES);
  if (Serial_StartReceive(Djinn_GetRecoveryTableFar(DJINN_RECOVERY_OPPONENT)) !=
      -1) {
    Serial_WaitForReceive();
    wait(2);
  }
  Heap_Release(buffer);
  return memberCount;
}

/// Unused: sends the party, then the Djinn recovery list, each unit under the
/// id the opponent knows it by.
UNREAD_RESULT LinkBattle_SendParty(void) {
  u8 *buffer;
  struct BattleWork *battle;
  s32 j;
  struct DjinnRecoveryList *list;
  s32 size;
  s32 count;
  s32 i;
  u16 unitIds[8];

  size = LINK_PARTY_UNIT_BYTES;
  buffer = Heap_AllocEwram(size);
  {
    s32 noSlot = 0xff;

    battle = gBattleWork;
    for (i = 7; i >= 0; i--)
      battle->opponentSlots[i] = noSlot;

    // The party members, each marked as the opponent's.
    count = BattleParty_PrepareMembers(unitIds);
    for (i = 0; i < count; i++) {
      Iwram_CopyWords(buffer, Unit_GetFar(unitIds[i]), size);
      ((struct Unit *)buffer)->side = BATTLE_SIDE_PARTY;
      // FIRST_ENEMY_UNIT + i, in a byte.
      battle->opponentSlots[unitIds[i]] = i - FIRST_ENEMY_UNIT;
      if (Serial_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
        break;
      Serial_WaitForSend();
      wait(2);
    }
    // Empty places up to three.
    for (; i < LINK_PARTY_SIZE; i++) {
      ((struct Unit *)buffer)->side = BATTLE_SIDE_NONE;
      if (Serial_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
        break;
      Serial_WaitForSend();
      wait(2);
    }

    // The Djinn recovery list, its units renumbered for the opponent.
    size = LINK_PARTY_OFFER_BYTES;
    Heap_Release(buffer);
    buffer = (u8 *)Heap_AllocEwram(size);
    /* FAKEMATCH(dead): the copy in a once-only block; as the plain call, 2 of
     * 152 instructions differ in tbs-en. */
    do {
      Iwram_CopyWords(buffer, Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY),
                      size);
    } while (0);
    list = &((struct DjinnRecoveryTable *)buffer)->list;
    for (j = 0; j < list->count; j++)
      list->entries[j].unitId = battle->opponentSlots[list->entries[j].unitId];
    if (Serial_StartSend(buffer, LINK_PARTY_OFFER_BYTES) != -1) {
      Serial_WaitForSend();
      wait(1);
      wait(2);
    }
    Heap_Release(buffer);
  }
}

/// Copies a name's first four characters, an underscore for each zero, and
/// returns a pointer to the end. UB: the copy is on the stack; UBFIX keeps it.
UNUSED u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1,
                                               u16 *source) {
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
  s32 count = 3;

  do {
    u32 value = *source;

    *dst = value;
    source++;
    dst++;
    if ((u8)value == 0)
      *current = fill;
    current++;
    count--;
  } while (count >= 0);
  base[4] = 0;
  // BUG: the original buffer is on the stack, gone once this returns.
  // UBFIX keeps the result until the next call in private scratch storage.
  return dst;
}

/// In a link battle, shows the "EXEC" signature and waits up to 30 frames for
/// the other console to show it too. Returns -1 when the link fails, else 0.
s32 LinkBattle_WaitSync(void) {
  struct BattleWork *battle = gBattleWork;
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

/// In a link battle, after each action and each turn, steps both consoles
/// through the same handshake of signatures. Returns -1 when the link fails or
/// the steps disagree, else 0.
s32 LinkBattle_SyncTurn(void) {
  struct BattleWork *battle = gBattleWork;
  u16 *peer;
  u16 *signature;
  s32 missed = 0;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches: no instruction addresses any of the 20 bytes it reserves (sub sp,
   * #20), and The Lost Age's LinkBattle_SyncTurn declares the same spare[10];
   * as without it, 20 of 232 instructions differ in tbs-en. */
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
UNREAD_RESULT BattleParty_AssignOpponentSlots(void) {
  u16 activeMembers[8];
  struct BattleWork *battleState = gBattleWork;
  s32 partySize = BattleParty_PrepareMembers(activeMembers);
  s32 memberSlot;
  s32 unitId;

  for (memberSlot = 0; memberSlot < partySize; memberSlot++) {
    unitId = activeMembers[memberSlot];
    battleState->opponentSlots[unitId] = memberSlot - FIRST_ENEMY_UNIT;
  }
}

/// Clears the first 16 bytes of the link transfer's payload, the eight
/// signature halfwords LinkBattle_WaitSync and LinkBattle_SyncTurn write.
s32 Battle_ClearSerialTransferPayload(void) {
  return Iwram_ClearWords(gSerialTransfer.payload, 16);
}
