#include "TYPES.H"
#include "RESOURCE.H"
#include "RENDER_INPUT.H"
#include "BATTLE_UNIT.H"

void VramBlock_LoadResourceFar(s32 entry_no, s32 mode, s32 resource_id);
extern char ResourceId_UiIconTiles;

extern u8 MsgAgilityLabel[];
extern u8 MsgPanelStatLabel[];
extern u8 MsgPanelDefenseLabel[];
void UiText_DrawCharacterAtOffsetFar();
void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();

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

/* Loads the icon tiles resource into a free entry and creates an icon
   output from it; ☀️ passes the tiles' address where ⚓️ passes the resource. */
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
    VramBlock_LoadResourceFar(slot, copy_mode = resource_mode, (s32)&ResourceId_UiIconTiles);
    icon = (s32)RenderOutput_CreateFar(slot, 0x40000000, window, x, y);
  }
  return icon;
}
