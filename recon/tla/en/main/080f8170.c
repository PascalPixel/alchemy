// Measured 2026-10-26 under the Makefile's options (-mthumb-slow-loads), differ
// 8 of 494 in tla-en. The native spills its arguments to the stack before it
// builds the heap slots' address; this builds the address first (row 9). An
// empty asm after `created = 0;` measured 4. No single empty asm or one-pass
// block reaches 0.
// First draft of Func_080f8170, written from the listing and ☀️'s twin
// Menu_DrawOwnerStatusPanel (games/tbs/src/menu/menu/common.c): the item
// menu's status panel for a party member (window at tile 0,5, 13 by 12): the
// name, up to two ailments or the level, and below them what the mode's panel
// kind asks for. ⚓️ adds kind 9, the class an equipment change would give
// (with the equipped item, its preview removal or its equipping undone after).
// Message numbers stay numbers: the Poison to Haunt labels (4359 to 4362),
// Cannot equip (4176), Learned and Will learn (4178, 4177) have no catalog
// names yet.
// Differ 14 of 494 (2026-10-19). Messages must be symbols: as numbers they are
// split into movs and shifts where the native loads them (164). Kind 9 needs
// its gotos in this order (18 the other way). Left, all order: the native
// stores slot before mode in the prologue, loads mode before it builds the
// 0x100 mask (twice), and builds the copy's size after IwramCopyWords' entry.
// Adopting it needs the seven unnamed messages named in every edition's
// catalog, whose numbers differ (04359 is Poison in English, another message in
// German).
#include "types.h"
#include "battle_unit.h"
#include "item.h"
#include "render_input.h"
#include "ram_buffer.h"
#include "iwram_call.h"

#define STATUS_PANEL_LOWER_ONLY 0x100
#define STATUS_PANEL_KIND_MASK 0xff
#define INVENTORY_EQUIPPED 0x200
#define ITEM_ID_MASK 0x1ff
#define OWNER_ACTION_ID_MASK 0x3fff

enum StatusPanelKind {
  STATUS_PANEL_EXPERIENCE = 0,
  STATUS_PANEL_EQUIP = 2,
  STATUS_PANEL_LEARN = 4,
  STATUS_PANEL_CLASS = 6,
  STATUS_PANEL_STATS = 8,
  STATUS_PANEL_EQUIP_CLASS = 9
};

// Messages are linker symbols; the unnamed ones by their numbers.
extern char MsgClassName, MsgExpLabel, MsgPanelStatLabel, MsgAbilityNothing;
extern char Msg04359, Msg04360, Msg04361, Msg04362;
extern char Msg04176, Msg04177, Msg04178;
#define MESSAGE_CLASS_NAME ((s32)&MsgClassName)
#define MESSAGE_EXP_LABEL ((s32)&MsgExpLabel)
#define MESSAGE_PANEL_STAT_LABEL ((s32)&MsgPanelStatLabel)
#define MESSAGE_ABILITY_NAME ((s32)&MsgAbilityNothing)
#define MESSAGE_POISON_LABEL ((s32)&Msg04359)
#define MESSAGE_VENOM_LABEL ((s32)&Msg04360)
#define MESSAGE_CURSE_LABEL ((s32)&Msg04361)
#define MESSAGE_HAUNT_LABEL ((s32)&Msg04362)
#define MESSAGE_CANNOT_EQUIP ((s32)&Msg04176)
#define MESSAGE_WILL_LEARN ((s32)&Msg04177)
#define MESSAGE_LEARNED ((s32)&Msg04178)

struct InventoryMenuState {
  u8 unk000[0x28];
  struct RenderInput *statusWindow;
  u8 unk02c[0x144];
  void *cursor;
  u8 unk174[0xec];
  s8 equipPreview;
};

extern const u8 gStatusPanel_LvString[];

struct BattleUnit *Unit_Get(s32 owner);
s32 Window_UpdateOrCreate(struct RenderInput **handle, s32 x, s32 y, s32 w,
                            s32 h, s32 mode);
void WaitFrames(s32 frames);
void Window_ClearInteriorTilesFar(struct RenderInput *window, s32 x, s32 y,
                                    s32 width, s32 height);
void Text_DrawStringAtOffsetFar(const u8 *text, struct RenderInput *window,
                                  s32 x, s32 y);
void Text_DrawCharacterAtOffsetFar(s32 message, struct RenderInput *window,
                                     s32 x, s32 y);
void Text_DrawNumberInWindowFar(s32 value, s32 digits,
                                  struct RenderInput *window, s32 x, s32 y);
s32 CharacterMenu_BuildAvailability(u8 *ailments, s32 requested, s32 owner);
void WindowSprite_ClearListFar(struct RenderInput *window);
void *SideObject_CreateFar(s32 owner, s32 x, s32 y, struct RenderInput *window,
                           s32 a, s32 b);
void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
void Unit_UpdateClassFar(s32 owner);
void Inventory_EquipFar(s32 owner, s32 slot);
void Unit_CalculateFar(s32 owner);
void Window_SetTilemapEntryFar(struct RenderInput *window, s32 entry, s32 x,
                                 s32 y, s32 mode);
void Text_DrawStatComparison(struct BattleUnit *unit,
                               struct BattleUnit *backup,
                               struct RenderInput *window);
void StatusPanel_DrawHpPp(struct BattleUnit *unit, struct RenderInput *window);

void Func_080f8170(struct RenderInput *unused, s32 owner, s32 slot, s32 mode) {
  s32 created;
  struct ItemDefinition *def;
  u32 item;
  struct InventoryMenuState *state;
  struct BattleUnit *unit;
  struct RenderInput *window;
  s32 ailmentRows;
  s32 value;
  struct BattleUnit *backup;
  u8 ailments[8];

  created = 0;
  state = (struct InventoryMenuState *)Ram_HeapSlots->menuRuntime;
  unit = Unit_Get(owner);
  item = unit->inventory[slot];
  def = Item_Get(item & ITEM_ID_MASK);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    created =
        Window_UpdateOrCreate(&state->statusWindow, 0, 5, 13, 12, 0x102);
  window = state->statusWindow;

  if (!(mode & STATUS_PANEL_LOWER_ONLY)) {
    if (!created) {
      WaitFrames(1);
      Window_ClearInteriorTilesFar(state->statusWindow, 0, 0, 88, 32);
    }
    Text_DrawStringAtOffsetFar(unit->name, window, 32, 0);
    CharacterMenu_BuildAvailability(ailments, 1, owner);
    ailmentRows = 0;
    if (ailments[1]) {
      Text_DrawCharacterAtOffsetFar(MESSAGE_POISON_LABEL, window, 32,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[2]) {
      Text_DrawCharacterAtOffsetFar(MESSAGE_VENOM_LABEL, window, 32,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[3]) {
      Text_DrawCharacterAtOffsetFar(MESSAGE_CURSE_LABEL, window, 32,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailments[4]) {
      Text_DrawCharacterAtOffsetFar(MESSAGE_HAUNT_LABEL, window, 32,
                                      ailmentRows * 8 + 8);
      ailmentRows++;
    }
    if (ailmentRows < 2) {
      value = unit->level;
      Text_DrawStringAtOffsetFar(gStatusPanel_LvString, window, 40, 16);
      Text_DrawNumberInWindowFar(value, 4, window, 56, 16);
    }
  }

  if (!created) {
    WaitFrames(1);
    Window_ClearInteriorTilesFar(state->statusWindow, 0, 32, 88, 80);
  }
  WindowSprite_ClearListFar(window);
  if (!(mode & STATUS_PANEL_LOWER_ONLY))
    state->cursor = SideObject_CreateFar(owner, 0, 0, window, 0, 0);

  switch (mode & STATUS_PANEL_KIND_MASK) {
  case STATUS_PANEL_EQUIP_CLASS:
    // Only removing an equipped item or equipping another can change the
    // class.
    if (item & INVENTORY_EQUIPPED) {
      if (state->equipPreview)
        goto classChange;
      goto experience;
    }
    if (state->equipPreview)
      goto experience;
  classChange:
    value = unit->classIndex + MESSAGE_CLASS_NAME;
    Text_DrawCharacterAtOffsetFar(value, window, 0, 32);
    if (!Item_CanOwnerEquip(owner, item)) {
      Text_DrawCharacterAtOffsetFar(MESSAGE_CANNOT_EQUIP, window, 16, 48);
      break;
    }
    backup = Heap_Alloc(sizeof(struct BattleUnit));
    Iwram_CopyWords(backup, unit, sizeof(struct BattleUnit));
    if (state->equipPreview) {
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
      Unit_UpdateClassFar(owner);
    } else {
      Inventory_EquipFar(owner, slot);
    }
    Unit_CalculateFar(owner);
    Window_SetTilemapEntryFar(window, 0xf12a, 3, 5, 0);
    value = unit->classIndex + MESSAGE_CLASS_NAME;
    Text_DrawCharacterAtOffsetFar(value, window, 0, 48);
    Iwram_CopyWords(unit, backup, sizeof(struct BattleUnit));
    Heap_Release(backup);
    break;
  case STATUS_PANEL_EXPERIENCE:
  experience:
    value = unit->classIndex + MESSAGE_CLASS_NAME;
    Text_DrawCharacterAtOffsetFar(value, window, 0, 32);
    StatusPanel_DrawHpPp(unit, window);
    value = unit->experience;
    Text_DrawCharacterAtOffsetFar(MESSAGE_EXP_LABEL, window, 0, 64);
    Text_DrawNumberInWindowFar(value, 8, window, 24, 72);
    break;
  case STATUS_PANEL_CLASS:
    value = unit->classIndex + MESSAGE_CLASS_NAME;
    Text_DrawCharacterAtOffsetFar(value, window, 0, 32);
    StatusPanel_DrawHpPp(unit, window);
    break;
  case STATUS_PANEL_EQUIP:
  case STATUS_PANEL_EQUIP + 1:
    if (!Item_CanOwnerEquip(owner, item)) {
      Text_DrawCharacterAtOffsetFar(MESSAGE_CANNOT_EQUIP, window, 0, 48);
      break;
    }
    backup = Heap_Alloc(sizeof(struct BattleUnit));
    Iwram_CopyWords(backup, unit, sizeof(struct BattleUnit));
    if (state->equipPreview)
      unit->inventory[slot] &= ~INVENTORY_EQUIPPED;
    else
      Inventory_EquipFar(owner, slot);
    Unit_CalculateFar(owner);
    Text_DrawStatComparison(unit, backup, window);
    Iwram_CopyWords(unit, backup, sizeof(struct BattleUnit));
    Heap_Release(backup);
    break;
  case STATUS_PANEL_LEARN: {
    s32 psynergy = def->actionId;
    s32 known = 0;
    s32 i;

    for (i = 0; i < 32; i++) {
      if ((unit->actionSlots[i].encodedAction & OWNER_ACTION_ID_MASK) ==
          psynergy) {
        known = 1;
        break;
      }
    }
    if (known) {
      Text_DrawCharacterAtOffsetFar(psynergy + MESSAGE_ABILITY_NAME, window, 0,
                                      48);
      Text_DrawCharacterAtOffsetFar(MESSAGE_LEARNED, window, 0, 56);
    } else {
      Text_DrawCharacterAtOffsetFar(psynergy + MESSAGE_ABILITY_NAME, window, 0,
                                      48);
      Text_DrawCharacterAtOffsetFar(MESSAGE_WILL_LEARN, window, 0, 56);
    }
    break;
  }
  case STATUS_PANEL_STATS: {
    s32 labels = MESSAGE_PANEL_STAT_LABEL;

    Text_DrawCharacterAtOffsetFar(labels, window, 0, 40); // Attack
    value = unit->atk;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 40);
    Text_DrawCharacterAtOffsetFar(labels + 1, window, 0, 48); // Defense
    value = unit->def;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 48);
    Text_DrawCharacterAtOffsetFar(labels + 4, window, 0, 56); // Agility
    value = unit->agi;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 56);
    Text_DrawCharacterAtOffsetFar(labels + 3, window, 0, 64); // Luck
    value = unit->luck;
    Text_DrawNumberInWindowFar(value, 3, window, 64, 64);
    break;
  }
  }
}
