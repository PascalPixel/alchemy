#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "system.h"
#include "mem.h"
#include "owner_state.h"
#include "serial_runtime.h"
#include "tla_edition.h"
#include "text/msg_ids.h"
#include "battle_work.h"
#include "iwram_call.h"
#include "edition.h"

s32 LinkBattle_ReceiveParty(void) {
  u16 name_text[16];
  void *buffer;
  u8 *name_entry;
  s32 named_count;
  s32 index;
  s32 len;
  s32 i;

  buffer = Runtime_BumpAllocateAlternatePool(340);
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
    WaitFrames(2);
    Ui_AdjustValueWithoutLimitFar((s32)MsgEnemyLabel, name_text);
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
  MemFree(buffer);
  buffer = Runtime_BumpAllocateAlternatePool(340);
  if (Serial_StartReceive(Resource_FarCall005(1)) != -1) {
    Serial_WaitForReceive();
    WaitFrames(2);
  }
  MemFree(buffer);
  return named_count;
}

#define LINK_PARTY_UNIT_BYTES 340
#define LINK_PARTY_OFFER_BYTES 340

struct LinkOfferEntry {
  u8 unk00[2];
  u8 unitId;
  u8 unk03;
};

struct LinkOfferList {
  struct LinkOfferEntry entries[72];
  s32 count;
};

struct LinkOfferTable {
  u8 unk00[8];
  struct LinkOfferList list;
};

struct LinkUnit {
  u8 unk000[0x12a];
  u8 side;
};

s32 BattleParty_PrepareActiveOwners(u16 *unit_ids);

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
  buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
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
    WaitFrames(2);
  }
  for (; i <= 2; i++) {
    ((struct LinkUnit *)buffer)->side = 0;
    if (Serial_StartSend(buffer, LINK_PARTY_UNIT_BYTES) == -1)
      break;
    Serial_WaitForSend();
    WaitFrames(2);
  }
  size = LINK_PARTY_OFFER_BYTES;
  MemFree(buffer);
  buffer = (u8 *)Runtime_BumpAllocateAlternatePool(size);
  /* FAKEMATCH: a one-pass loop is a sched2 barrier, so the list pointer's copy
   * is emitted before the count's address as in the reference (differ 4 without
   * it), as The Broken Seal's LinkBattle_SendParty. */
  do {
    Iwram_CopyWords(buffer, Djinn_GetRecoveryTableFar(0), size);
  } while (0);
  list = &((struct LinkOfferTable *)buffer)->list;
  for (j = 0; j < list->count;) {
    list->entries[j].unitId = work->ownerSlots[list->entries[j].unitId];
    /* FAKEMATCH: the empty one-pass block ends the scheduling region after the
     * step, so the entry pointer advances before the count is reloaded as in
     * the reference (differ 2 without it). */
    j++;
    do {
    } while (0);
  }
  if (Serial_StartSend(buffer, LINK_PARTY_OFFER_BYTES) != -1) {
    Serial_WaitForSend();
    WaitFrames(1);
    WaitFrames(2);
  }
  MemFree(buffer);
}
