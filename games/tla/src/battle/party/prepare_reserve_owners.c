#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "owner_state.h"
#include "battle_system.h"
#include "battle_type.h"
#include "battle_work.h"
#include "adventure.h"
#include "ram_buffer.h"

s32 BattleParty_PrepareReserveOwners(u16 *owners) {
  s32 count;
  s32 index;

  count = Party_CountActiveOwnersFar();
  if (count > 4)
    count -= 4;
  else
    count = 0;
  if (Ram_HeapSlots->battleWork->twoSided != 0)
    count = 0;

  for (index = 0; index < count; index++) {
    s32 owner = gAdventure.activeOwners[index + 4];

    if (owners != 0)
      *owners++ = owner;
    ((struct BattleUnit *)Unit_Get(owner))->side = 2;
  }

  if (owners != 0)
    *owners = 0xff;
  return count;
}
