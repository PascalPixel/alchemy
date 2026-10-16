#include "types.h"
#include "battle_effect.h"
#include "battle_type.h"

struct ItemOwner {
  u8 padding000[0x10];
  u16 stat10;
  u16 stat12;
  u8 padding014[4];
  u16 stat18;
  u16 stat1a;
  u16 stat1c;
  u8 stat1e;
  u8 padding01f[0x15];
  s16 maxHp;
  s16 maxPp;
  s16 hp;
  s16 pp;
  u8 padding03c[0x9c];
  u16 items[15];
  u8 padding0f6[0x3b];
  s8 poison;
};

struct ItemData {
  u8 padding00[0x0c];
  u8 kind;
  u8 padding0d[0x1b];
  u16 useAbility;
};

struct ItemUseWork {
  u8 padding000[0x1c8];
  u16 entries[32];
  u16 targets[8];
  u8 entryCount;
  u8 targetCount;
  u8 padding21a[0x40];
  s16 resultCode;
};

extern struct ItemUseWork *gMenuWork;

struct ItemOwner *UnitGetFar(s32);
struct ItemData *Item_Get(s32);
u8 Inventory_RemoveFar(s32, s32);
u32 ItemMenu_Collect(struct ItemOwner *, u16 *, s32);
s32 BattleEffect_ApplyToTargets(s32, s32, s32, s32);

s32 Item_Use(s32 slot, s32 ownerId, s32 targetId) {
  struct ItemUseWork *work;
  s32 result;
  s32 itemId;
  struct ItemOwner *owner;
  struct ItemData *item;

  owner = UnitGetFar(ownerId);
  work = gMenuWork;
  itemId = 0x1ff & owner->items[slot];
  item = Item_Get(itemId);
  result = BattleEffect_ApplyToTargets(0x3fff & item->useAbility, ownerId,
                                       targetId, 1);
  if (result != -1) {
    item = Item_Get(owner->items[slot]);
    if (item->kind == 1) {
      Inventory_RemoveFar(ownerId, slot);
      work->entryCount = ItemMenu_Collect(owner, work->entries, 0);
    }
    if (item->kind == 4) {
      if (itemId == 0xb8)
        itemId = 0xb9;
      owner->items[slot] = itemId;
    }
    result = 0;
  }
  return result;
}
