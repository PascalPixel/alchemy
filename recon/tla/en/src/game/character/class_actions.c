#include "types.h"
#include "owner_state.h"
#include "item.h"

struct ClassAbility {
  u16 ability;
  u8 level;
  u8 unk03;
};

struct ClassAbilities {
  u8 unk00[0x10];
  struct ClassAbility abilities[16];
};

enum { OWNER_ACTION_CLASS = 0x8000, OWNER_ACTION_ITEM = 0x4000 };

s32 Owner_UpdateClassActions(s32 owner) {
  struct BattleUnit *st;
  u16 *ability;
  struct ClassAbilities *cls;
  struct ItemDefinition *def;
  s32 i;
  s32 j;
  u16 mask;
  struct ClassAbility *cg;

  st = Unit_Get(owner);
  ability = (u16 *)st->actionSlots;
  cls = (struct ClassAbilities *)Class_GetDefinition(st->classIndex);
  if (st->classIndex == 0)
    return 0;
  mask = OWNER_ACTION_CLASS | OWNER_ACTION_ITEM;
  for (j = 0; j < 32; j++) {
    if (ability[j * 2] & mask)
      ability[j * 2] = 0;
  }
  for (i = 31, j = 31; i >= 0;) {
    if (ability[i * 2] == 0) {
      i--;
    } else {
      ability[j * 2] = ability[i * 2];
      i--;
      j--;
    }
  }
  for (; j >= 0; j--)
    ability[j * 2] = 0;

  mask = OWNER_ACTION_CLASS;
  for (j = 0; j < 16; j++) {
    cg = &cls->abilities[j];
    if (cls->abilities[j].ability == 0)
      continue;
    if (st->level < cls->abilities[j].level)
      continue;
    i = 0;
    while (i < 32 && ability[i * 2] != cls->abilities[j].ability)
      i++;
    if (i != 32)
      continue;
    for (i = 0; i < 32; i++) {
      if (ability[i * 2] == 0) {
        ability[i * 2] = cg->ability | OWNER_ACTION_CLASS;
        break;
      }
    }
    if (i == 32)
      break;
  }

  for (j = 0; j < 15; j++) {
    if (st->inventory[j] == 0)
      continue;
    if (!(st->inventory[j] & INVENTORY_EQUIPPED))
      continue;
    def = Item_GetDirect(st->inventory[j]);
    if (def->useType != 3)
      continue;
    for (i = 0; i < 32; i++) {
      if ((ability[i * 2] & OWNER_ACTION_ID_MASK) == def->actionId)
        break;
    }
    if (i != 32)
      continue;
    for (i = 0; i < 32; i++) {
      if (ability[i * 2] == 0) {
        ability[i * 2] = def->actionId | OWNER_ACTION_ITEM;
        break;
      }
    }
    if (i == 32)
      break;
  }

  for (i = 0, j = 0; i < 32;) {
    if (ability[i * 2] == 0) {
      i++;
    } else {
      ability[j * 2] = ability[i * 2];
      i++;
      j++;
    }
  }
  for (; j < 32; j++)
    ability[j * 2] = 0;
  return 0;
}
