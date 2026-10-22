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
  /// A struct BattleUnit (BATTLE_UNIT_SIZE, 332 bytes) and the 8 after it.
  LINK_PARTY_UNIT_BYTES = 340,
  /// A struct DjinnRecoveryTable (268 bytes) and the 52 after it.
  LINK_PARTY_OFFER_BYTES = 320
};

/// Link battle: receives the opponent's party into the enemy units, one
/// member at a time, and puts the "Enemy" label (MsgEnemyLabel) before each
/// name; then receives the opponent's Djinn recovery list. Stops early if a
/// transfer cannot start. Returns how many of the received units are
/// members (a unit sent with side 0 is an empty place). Nothing in the
/// source calls it. Overwrites enemy units 128 to 130 and the opponent's
/// recovery table: each 340-byte packet runs 8 bytes into the next enemy
/// unit, which the next packet rewrites, the third's into unit 131.
s32 LinkBattle_ReceiveParty(void) {
  u16 label[24];
  void *buffer;
  struct BattleUnit *opponent;
  s32 memberCount;
  s32 partySlot;
  s32 labelLength;
  s32 i;

  // The block is only freed: each member is received straight into its
  // enemy unit, as in the link lobby's twin of this function
  // (LinkLobby_ReceivePartyRecords, menu/lobby.c). XXX: likely left from a
  // receive into a buffer, because LinkBattle_SendParty does send from one.
  buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_UNIT_BYTES);
  memberCount = 0;
  partySlot = 0;
  while (partySlot < LINK_PARTY_SIZE) {
    opponent = Unit_GetFar(partySlot + FIRST_ENEMY_UNIT);
    if (SerialRuntime_StartReceive(opponent) == -1) {
      break;
    }
    SerialRuntime_WaitForReceive();
    if (opponent->side != 0) {
      memberCount += 1;
    }
    WaitFrames(2);

    // Measure the label (at most NAME_LIMIT + 1 characters), shift the name
    // right to make room for it, then write it in; the name's end is cut.
    Ui_AdjustValueWithoutLimitFar(MsgEnemyLabel, label);
    i = 0;
    if (label[i] != 0) {
      do {
        i += 1;
        if (i > NAME_LIMIT) {
          break;
        }
      } while (label[i] != 0);
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
    partySlot += 1;
  }
  MemFree(buffer);

  // The recovery packet follows all three party places.
  buffer = Runtime_BumpAllocateAlternatePool(LINK_PARTY_OFFER_BYTES);
  if (SerialRuntime_StartReceive(
          Djinn_GetRecoveryTableFar(DJINN_RECOVERY_OPPONENT)) != -1) {
    SerialRuntime_WaitForReceive();
    WaitFrames(2);
  }
  MemFree(buffer);
  return memberCount;
}

/// Link battle: sends the party, one member at a time and three places in
/// all (an empty place has side 0), then the Djinn recovery list with each
/// owner given the unit id the opponent knows them by. Records that id of
/// each member in gBattleWork->ownerSlots (0xff for none). Nothing in the
/// source calls it.
#ifdef UBFIX
void LinkBattle_SendParty(void) {
#else
s32 LinkBattle_SendParty(void) {
#endif
  u8 *buffer;
  struct BattleWork *battle;
  s32 j;
  struct DjinnRecoveryList *list;
  s32 size;
  s32 count;
  s32 i;
  s32 noSlot;
  u16 owners[8];

  size = LINK_PARTY_UNIT_BYTES;
  buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
  noSlot = 0xff;
  battle = gBattleWork;
  for (i = 7; i >= 0; i--)
    battle->ownerSlots[i] = noSlot;

  // The members, each marked as the opponent's.
  count = BattleParty_PrepareActiveOwners(owners);
  for (i = 0; i < count; i++) {
    Iwram_CopyWords(buffer, Unit_GetFar(owners[i]), size);
    ((struct BattleUnit *)buffer)->side = BATTLE_UNIT_PARTY;
    // FIRST_ENEMY_UNIT + i, in a byte.
    battle->ownerSlots[owners[i]] = i - FIRST_ENEMY_UNIT;
    if (SerialRuntime_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    SerialRuntime_WaitForSend();
    WaitFrames(2);
  }
  // Empty places up to three.
  for (; i < LINK_PARTY_SIZE; i++) {
    ((struct BattleUnit *)buffer)->side = BATTLE_UNIT_ABSENT;
    if (SerialRuntime_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    SerialRuntime_WaitForSend();
    WaitFrames(2);
  }

  // The Djinn recovery list, its owners renumbered for the opponent.
  size = LINK_PARTY_OFFER_BYTES;
  MemFree(buffer);
  buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
  /* FAKEMATCH(one-pass): the copy in a one-pass loop, a scheduling barrier
   * that keeps the list pointer's copy ahead of the count's load; as the plain
   * call, 1 of 152 instructions differ in tbs-en. */
  do {
    Iwram_CopyWords(buffer, Djinn_GetRecoveryTableFar(DJINN_RECOVERY_PARTY),
                    size);
  } while (0);
  list = &((struct DjinnRecoveryTable *)buffer)->list;
  for (j = 0; j < list->count; j++)
    list->entries[j].unitId = battle->ownerSlots[list->entries[j].unitId];
  if (SerialRuntime_StartSend(buffer, LINK_PARTY_OFFER_BYTES) != -1) {
    SerialRuntime_WaitForSend();
    WaitFrames(1);
    WaitFrames(2);
  }
  MemFree(buffer);
}

/// Copies the low bytes of a name's first four characters and a terminator,
/// an underscore in place of each zero, and returns a pointer to the
/// terminator. The copy reads on past the name's end.
/// UBFIX keeps the private scratch buffer valid until the next call; the
/// original returns a pointer to expired stack storage. The first two
/// arguments are ignored.
/// Nothing calls it.
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

/// In a link battle, shows the "EXEC" signature to the other console and
/// waits up to 30 frames for it to show the same. Returns 0 once both do (or
/// outside a link battle), and -1 if the battle is paused, the link is lost
/// for more than LINK_MAX_MISSED_FRAMES frames or the signatures never meet.
/// Called by BattlePresentation_BuildActions. Writes the first eight bytes
/// of gSerialTransfer.payload.
s32 BattlePresentation_WaitSync(void) {
  struct BattleWork *battle = gBattleWork;
  u16 *peer;
  u16 *signature;
  s32 missed = 0;
  s32 i;

  if (battle->twoSided != 0) {
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
        WaitFrames(1);
      }
    }
    return -1;
  }
  return 0;
}

/// In a link battle, after each action and after the end of each round,
/// steps both consoles through the same handshake: "ex" "TU", then "rn",
/// "EXEC", "tu" and finally "RN". Each step shows its signature and waits
/// for the other console to show it too, failing if the other console
/// shows a different earlier step. Returns -1 if the battle is paused, the
/// link is lost for more than LINK_MAX_MISSED_FRAMES frames, or the steps
/// disagree; 0 once in step, or outside a link battle. Called by
/// Battle_RunEncounter. Writes the first sixteen bytes of
/// gSerialTransfer.payload.
s32 BattlePresentation_SyncTurn(void) {
  struct BattleWork *battle = gBattleWork;
  u16 *peer;
  u16 *signature;
  s32 missed = 0;
  /* FAKEMATCH(dead): an unused array for the twenty bytes of frame no
   * instruction touches; without it, 8 of 232 instructions differ in tbs-en. */
  UNUSED u16 spare[10];

  if (battle->twoSided != 0) {
    peer = gLinkPeerSignatures[battle->linkSide ^ 1];
    signature = (u16 *)gSerialTransfer.payload;
    if (battle->linkPaused != 0)
      return -1;

    // Publish "ex"/"TU" while both sides still agree on the earlier phase.
    signature[0] = 'e';
    signature[1] = 'x';
    signature[4] = 'T';
    signature[5] = 'U';
    WaitFrames(1);
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
      WaitFrames(1);
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
      WaitFrames(1);
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
      WaitFrames(1);
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
      WaitFrames(1);
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
      WaitFrames(1);
    }
  }
  return 0;
}

/// Records, for each party member, the unit id the link opponent knows them
/// by (FIRST_ENEMY_UNIT plus their place in the party), in
/// gBattleWork->ownerSlots. Called by Battle_RunEncounter as a link battle
/// is set up.
#ifdef UBFIX
void BattleParty_AssignMemberSlots(void) {
#else
s32 BattleParty_AssignMemberSlots(void) {
#endif
  u16 activeMembers[8];
  struct BattleWork *battleState = gBattleWork;
  s32 partySize = BattleParty_PrepareActiveOwners(activeMembers);
  s32 memberSlot;
  s32 unitId;

  for (memberSlot = 0; memberSlot < partySize; memberSlot++) {
    unitId = activeMembers[memberSlot];
    // FIRST_ENEMY_UNIT + memberSlot, in a byte.
    battleState->ownerSlots[unitId] = memberSlot - FIRST_ENEMY_UNIT;
  }
}

/// Clears the first 16 bytes of the link transfer's payload, the eight
/// signature halfwords BattlePresentation_WaitSync and
/// BattlePresentation_SyncTurn write. Called by Battle_RunEncounter on each way
/// out of the battle but fleeing.
s32 Battle_ClearSerialTransferPayload(void) {
  return Iwram_ClearWords(gSerialTransfer.payload, 16);
}
