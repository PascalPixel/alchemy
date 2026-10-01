#include "TYPES.H"
#include "RESOURCE.H"

void VramBlock_LoadResourceFar(s32 entry_no, s32 mode, s32 resource_id);
extern char ResourceId_UiIconTiles;
s32 RenderOutput_CreateFar(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);

extern u8 MsgAgilityLabel[];
extern u8 MsgPanelStatLabel[];
extern u8 MsgPanelDefenseLabel[];
void UiText_DrawCharacterAtOffsetFar();
void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();

void UiText_DrawStatComparison(s32 alt, s32 base, s32 work)
{
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelStatLabel, work, 0, 32);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 60), 3, work, 16, 40);
    if (*(u16 *)(alt + 60) != *(u16 *)(base + 60)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(alt + 60), 3, work, 64, 40);
        if (*(u16 *)(alt + 60) > *(u16 *)(base + 60)) {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 36, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgPanelDefenseLabel, work, 0, 48);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 62), 3, work, 16, 56);
    if (*(u16 *)(alt + 62) != *(u16 *)(base + 62)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(alt + 62), 3, work, 64, 56);
        if (*(u16 *)(alt + 62) > *(u16 *)(base + 62)) {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 52, 1);
        }
    }
    UiText_DrawCharacterAtOffsetFar((s32)MsgAgilityLabel, work, 0, 64);
    UiText_DrawNumberAtOffsetFar(*(u16 *)(base + 64), 3, work, 16, 72);
    if (*(u16 *)(alt + 64) != *(u16 *)(base + 64)) {
        UiText_DrawNumberAtOffsetFar(*(u16 *)(alt + 64), 3, work, 64, 72);
        if (*(u16 *)(alt + 64) > *(u16 *)(base + 64)) {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 0);
        } else {
            UiIcon_CreateStatChangeArrow(work, 44, 68, 1);
        }
    }
}

/* Loads the icon tiles resource into a free entry and creates an icon
   output from it; ☀️ passes the tiles' address where ⚓️ passes the resource. */
s32 UiIcon_CreateWithResourceVariant(s32 first, s32 second, s32 third)
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
    icon = RenderOutput_CreateFar(slot, 0x40000000, first, second, third);
  }
  return icon;
}
