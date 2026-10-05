#include "TYPES.H"
#include "RESOURCE.H"
#include "VRAM_BLOCK.H"
#include "RENDER_INPUT.H"
#include "BATTLE_UNIT.H"
#include "UI.H"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 MsgAgilityLabel[];
extern u8 MsgPanelStatLabel[];
extern u8 MsgPanelDefenseLabel[];
void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();

extern u8 UiIcon_ResourceTiles[];

void UiText_DrawStatComparison(const struct BattleUnit *alt, const struct BattleUnit *base, s32 work)
{
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelStatLabel, work, 0, 32);
    UiText_DrawNumberAtOffsetFar(base->attack, 3, work, 16, 40);
    if (alt->attack != base->attack) {
        UiText_DrawNumberAtOffsetFar(alt->attack, 3, work, 64, 40);
        if (alt->attack > base->attack) {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
    UiText_DrawNumberAtOffsetFar(base->defense, 3, work, 16, 56);
    if (alt->defense != base->defense) {
        UiText_DrawNumberAtOffsetFar(alt->defense, 3, work, 64, 56);
        if (alt->defense > base->defense) {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
    UiText_DrawNumberAtOffsetFar(base->agility, 3, work, 16, 72);
    if (alt->agility != base->agility) {
        UiText_DrawNumberAtOffsetFar(alt->agility, 3, work, 64, 72);
        if (alt->agility > base->agility) {
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

/* The object's packed OAM halfwords, separate from its logical position. */
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
        ((struct UiIconAttributes *)&object->packed)->x = (u16)object->x;
        ((struct UiIconAttributes *)&object->packed)->y = (u16)object->y;
        ((struct UiIconAttributes *)&object->packed)->matrix = 0;
        ((struct UiIconAttributes *)&object->packed)->affine_mode = 0;
    }
}
