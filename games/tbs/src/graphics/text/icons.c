#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "resource.h"
#include "vram_block.h"
#include "render_input.h"
#include "battle_unit.h"
#include "ui.h"
#include "text/msg_ids.h"

void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();


void UiText_DrawStatComparison(const struct BtlUnit *alt, const struct BtlUnit *base, s32 work)
{
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelStatLabel, work, 0, 32);
    UiText_DrawNumberAtOffsetFar(base->atk, 3, work, 16, 40);
    if (alt->atk != base->atk) {
        UiText_DrawNumberAtOffsetFar(alt->atk, 3, work, 64, 40);
        if (alt->atk > base->atk) {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
    UiText_DrawNumberAtOffsetFar(base->def, 3, work, 16, 56);
    if (alt->def != base->def) {
        UiText_DrawNumberAtOffsetFar(alt->def, 3, work, 64, 56);
        if (alt->def > base->def) {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
    UiText_DrawNumberAtOffsetFar(base->agi, 3, work, 16, 72);
    if (alt->agi != base->agi) {
        UiText_DrawNumberAtOffsetFar(alt->agi, 3, work, 64, 72);
        if (alt->agi > base->agi) {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 1);
        }
    }
}

s32 UiIcon_CreateWithResource(struct RenderInput *window, s32 unused, s32 x, s32 y)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Resource_FindFreeEntry();
    if (entry_no != 0) {
        VramBlock_LoadCached(entry_no, 0x80, UiIcon_ResourceTiles);
        result = (s32)RenderOutput_CreateFar(entry_no, 0x40000000, window, x, y);
    }
    return result;
}

s32 UiIcon_CreateWithResourceVariant(struct RenderInput *window, s32 x, s32 y)
{
  s32 slot;
  unsigned char copy_mode;
  int resource_mode;
  s32 icon;
  icon = 0;
  icon = 0;
  slot = Resource_FindFreeEntry();
  resource_mode = 0x80;
  if (slot != 0)
  {
    VramBlock_LoadCached(slot, copy_mode = resource_mode, UiIcon_ResourceTiles);
    icon = (s32)RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
  }
  return icon;
}

struct UiIconAttributes {
    u8 y;
    u8 affine_mode : 2;
    u8 flags : 6;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
};

void UiIcon_PrepareObject(struct RenderOutput *object)
{
    if (object != NULL) {
        object->active = 1;
        ((struct UiIconAttributes *)&object->packed)->x = object->x;
        ((struct UiIconAttributes *)&object->packed)->y = object->y;
        ((struct UiIconAttributes *)&object->packed)->matrix = 0;
        ((struct UiIconAttributes *)&object->packed)->affine_mode = 0;
    }
}
