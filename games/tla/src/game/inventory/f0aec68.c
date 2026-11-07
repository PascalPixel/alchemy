/// @file
/// An item's equipment group.

#include "types.h"
#include "item.h"

struct ItemDefinition *Item_GetDirect(s32 item_id);

/// Returns an item's equipment group from its type: 1 for types 1, 7 and 10,
/// 2 for types 2 to 5 and 9, and 0 for any other type.
s32 Item_GetEquipmentGroup(s32 item_id) {
  s32 type = Item_GetDirect(item_id)->type;
  s32 group;

  if (type == 1)
    group = 1;
  else if (type == 2)
    group = 2;
  else if (type == 3)
    group = 2;
  else if (type == 4)
    group = 2;
  else if (type == 5)
    group = 2;
  else if (type == 9)
    group = 2;
  else if (type == 7)
    group = 1;
  else
    group = type == 10;
  return group;
}
