/// @file
/// Link battles: the parties and Djinn exchanged, and the turn handshake.
// Part of battle.c.

#include "../battle_internal.h"
#include "heap_slots.h"
#include "module/battle/battle.h"

s32 LinkBattle_ReceiveParty(void) {
  u16 name_text[16];
  void *buffer;
  u8 *name_entry;
  s32 named_count;
  s32 index;
  s32 len;
  s32 i;

  buffer = Heap_AllocEwram(340);
  named_count = 0;
  index = 0;
  while (index <= 2) {
    name_entry = Unit_Get(index + 128);
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
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) ||                      \
    (LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
    name_text[i] = ' ';
    len = i + 1;
#else
    len = i;
#endif
    for (i = 14; i >= len; i--) {
      name_entry[i] = name_entry[i - len];
    }
    for (i = 0; i < len; i++) {
      name_entry[i] = name_text[i];
    }
    name_entry[14] = 0;
    index += 1;
  }
  Heap_Release(buffer);
  buffer = Heap_AllocEwram(340);
  if (Serial_StartReceive(Resource_FarCall005(1)) != -1) {
    Serial_WaitForReceive();
    wait(2);
  }
  Heap_Release(buffer);
  return named_count;
}

#define LINK_PARTY_UNIT_BYTES 340
#define LINK_PARTY_OFFER_BYTES 340

s32 LinkBattle_SendParty(void) {
  u8 *buffer;
  struct BattleWork *work;
  s32 j;
  struct LinkOfferList *list;
  s32 size;
  s32 count;
  s32 i;
  s32 mark;
  u16 owners[8];

  size = LINK_PARTY_UNIT_BYTES;
  buffer = (u8 *)Heap_AllocEwram(size);
  mark = 0xff;
  work = Ram_HeapSlots->battleWork;
  for (i = 7; i >= 0; i--)
    work->ownerSlots[i] = mark;
  count = BattleParty_PrepareActiveOwners(owners);
  for (i = 0; i < count; i++) {
    Iwram_CopyWords(buffer, Unit_Get(owners[i]), size);
    ((struct LinkUnit *)buffer)->side = 2;
    work->ownerSlots[owners[i]] = i - 128;
    if (Serial_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    Serial_WaitForSend();
    wait(2);
  }
  for (; i <= 2; i++) {
    ((struct LinkUnit *)buffer)->side = 0;
    if (Serial_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    Serial_WaitForSend();
    wait(2);
  }
  size = LINK_PARTY_OFFER_BYTES;
  Heap_Release(buffer);
  buffer = (u8 *)Heap_AllocEwram(size);
  // The copy in a one-pass loop, a scheduling barrier that keeps the list
  // pointer's copy ahead of the count's load; as the plain call, 2 of 153
  // instructions differ in tla-en.
  do {
    Iwram_CopyWords(buffer, Djinn_GetRecoveryTableFar(0), size);
  } while (0);
  list = &((struct LinkOfferTable *)buffer)->list;
  for (j = 0; j < list->count;) {
    list->entries[j].unitId = work->ownerSlots[list->entries[j].unitId];
    j++;
  }
  if (Serial_StartSend(buffer, LINK_PARTY_OFFER_BYTES) != -1) {
    Serial_WaitForSend();
    wait(1);
    wait(2);
  }
  Heap_Release(buffer);
}

u8 *Text_CopyFiveCharsPaddingUnderscore(u32 unused0, u32 unused1, u16 *source) {
  u8 buffer[5];
  u8 *base = buffer;
  u32 fill = '_';
  u8 *p = base;
  u8 *dst = p;
  s32 count = 3;

  do {
    u32 value = *source;

    source++;
    *dst = value;
    dst++;
    if ((u8)value == 0)
      *p = fill;
    p++;
    count--;
  } while (count >= 0);
  base[4] = 0;
  return dst;
}

/// In a link battle, exchanges the "EXEC" signal with the other console.
/// Returns zero if both agree within 30 frames, or -1 on timeout or disconnect.
/// Returns zero immediately outside a link battle.
s32 LinkBattle_WaitSync(void) {
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 *peer;
  u16 *sig;
  s32 lost = 0;
  s32 i;

  if (work->twoSided != 0) {
    u32 other = work->linkSide ^ 1;

    peer = (u16 *)(gSerialPeerPayloads[other] + 4);
    sig = (u16 *)gSerialTransfer.payload;
    if (work->linkPaused == 0) {
      sig[0] = 'E';
      sig[1] = 'X';
      sig[2] = 'E';
      sig[3] = 'C';

      for (i = 0; i <= 29; i++) {
        if ((gLinkStatus & 3) != 3) // The two consoles lost their connection.
        {
          lost++;
          if (lost > 24)
            return -1;
        } else {
          lost = 0;
          if (sig[2] == peer[2] && sig[3] == peer[3])
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
  struct BattleWork *work = Ram_HeapSlots->battleWork;
  u16 *peer;
  u16 *sig;
  s32 lost = 0;
  // Unused: gives the stack frame its 20 bytes.
  UNUSED u16 spare[10];

  if (work->twoSided != 0) {
    peer = gLinkPeerSignatures[work->linkSide ^ 1];
    sig = (u16 *)gSerialTransfer.payload;
    if (work->linkPaused != 0)
      return -1;

    // "ex" "TU"
    sig[0] = 'e';
    sig[1] = 'x';
    sig[4] = 'T';
    sig[5] = 'U';
    wait(1);
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[2] != peer[2] || sig[3] != peer[3])
          return -1;
        if (sig[0] == peer[0] && sig[1] == peer[1] && sig[4] == peer[4] &&
            sig[5] == peer[5])
          break;
      }
      wait(1);
    }

    // "rn"
    sig[6] = 'r';
    sig[7] = 'n';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[4] != peer[4] || sig[5] != peer[5])
          return -1;
        if (sig[6] == peer[6] && sig[7] == peer[7])
          break;
      }
      wait(1);
    }

    // "EXEC"
    sig[0] = 'E';
    sig[1] = 'X';
    sig[2] = 'E';
    sig[3] = 'C';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[6] != peer[6] || sig[7] != peer[7])
          return -1;
        if (sig[0] == peer[0] && sig[1] == peer[1] && sig[2] == peer[2] &&
            sig[3] == peer[3])
          break;
      }
      wait(1);
    }

    // "tu"
    sig[4] = 't';
    sig[5] = 'u';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (sig[0] != peer[0] || sig[1] != peer[1] || sig[2] != peer[2] ||
            sig[3] != peer[3])
          return -1;
        if (sig[4] == peer[4] && sig[5] == peer[5])
          break;
      }
      wait(1);
    }

    // "RN": wait for the peer to move on from "rn".
    sig[6] = 'R';
    sig[7] = 'N';
    for (;;) {
      if ((gLinkStatus & 3) != 3) {
        if (++lost > 24)
          return -1;
      } else {
        lost = 0;
        if (peer[6] != 'r' || peer[7] != 'n')
          break;
      }
      wait(1);
    }
  }
  return 0;
}

s32 BattleParty_AssignMemberSlots(void) {
  u16 activeMembers[8];
  u8 *battleState = (u8 *)Ram_HeapSlots->battleWork;
  s32 partySize = BattleParty_PrepareActiveOwners(activeMembers);
  s32 memberSlot;
  s32 unitId;

  for (memberSlot = 0; memberSlot < partySize; memberSlot++) {
    unitId = activeMembers[memberSlot];
    unitId += 72;
    battleState[unitId] = (s8)(memberSlot - 128);
  }
}

void BattleStaging_ClearLinkPayload(void) {
  Iwram_ClearWords(gSerialTransfer.payload, 16);
}
