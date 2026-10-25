// Draft, differ 10 of 103 (2026-10-24): a one-pass block around obj =
// &node->obj takes four rows off. Left: the bit-field updates, where the
// native loads bytes 5 and 7 before it builds their masks.
#include "types.h"

struct Object_0801c0dc {
  u8 filler0[5];
  u8 field50 : 2;
  u8 field52 : 2;
  u8 field54 : 1;
  u8 field55 : 1;
  u8 field56 : 2;
  u8 filler6;
  u8 field70 : 1;
  u8 field71 : 5;
  u8 field76 : 2;
  u16 field80 : 10;
  u16 field8a : 2;
  u16 field8c : 4;
};

struct ListNode {
  u8 unk00[8];
  s16 base;
  s16 kind;
  u16 src;
  s16 tile;
  u8 unk10[16];
  s16 end;
  s16 field22;
  u8 unk24[2];
  s16 field26;
  struct Object_0801c0dc obj;
};

void Ui_BuildPairedPatternsToSlot(s32 arg0, s32 arg1, s32 *arg2, s32 *arg3,
                                  s32 arg4);
void UiIcon_BuildItemIconTiles(u32 glyph, s32 includeBase, s32 *sourceIndex,
                               s32 *result, s32 reuseSource);
void Ability_LoadGlyph(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

extern u8 MsgCommandName;
extern u8 MsgItemName;
extern u8 MsgAbilityName;

void MenuSelection_SetupEntry(u32 kind, s32 base, struct ListNode *node,
                              s32 reuse) {
  s32 src;
  s32 tile;
  struct Object_0801c0dc *obj;

  switch (kind) {
  case 1:
  case 6:
    if (reuse != 0)
      src = node->src;
    Ui_BuildPairedPatternsToSlot(base, 0, &src, &tile, reuse);
    node->end = base + (s32)&MsgCommandName;
    break;
  case 2:
    if (reuse != 0)
      src = node->src;
    UiIcon_BuildItemIconTiles(base, 1, &src, &tile, reuse);
    node->end = base + (s32)&MsgItemName;
    break;
  case 4:
    if (reuse != 0)
      src = node->src;
    Ability_LoadGlyph(base, 1, (s32)&src, (s32)&tile, reuse);
    node->end = base + (s32)&MsgAbilityName;
    break;
  }

  node->base = base;
  node->src = src;
  node->tile = tile;
  node->kind = kind;
  node->field22 = 0x100;
  node->field26 = 0x100;

  /* FAKEMATCH(one-pass): the object pointer formed in a once-only block,
   * which ends the scheduling region before its field stores; as
   * obj = &node->obj, 2 of 103 instructions differ in tla-en. */
  do {
    obj = &node->obj;
  } while (0);
  obj->field52 = 0;
  obj->field55 = 0;
  obj->field54 = 0;
  obj->field76 = 1;
  obj->field56 = 0;
  obj->field8c = 0;
  obj->field80 = tile;
  obj->field8a = 0;
}
