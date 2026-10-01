#include "TYPES.H"
#include "RESOURCE.H"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 MsgAgilityLabel[];
extern u8 MsgPanelStatLabel[];
extern u8 MsgPanelDefenseLabel[];
void UiText_DrawCharacterAtOffsetFar();
void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();

extern s32 VramBlock_LoadCached();
extern s32 RenderOutput_CreateFar();
extern u8 UiIcon_ResourceTiles[];
s32 VramBlock_LoadCached(s32 entry_no, s32 mode, s32 data);
s32 RenderOutput_CreateFar(s32 entry_no, s32 flags, s32 first, s32 second, s32 third);
#define FIELD_AT_OFFSET(base, type, offset)     (*(type *)((u8 *)(base) + (offset)))

struct UiIconObject { u8 unknown_00[0x16]; u16 value_16 : 9; u16 unknown_16b : 7; };

/* menu/psynergy_menu/call_icon_routine_with_value.c */
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);

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

s32 UiIcon_CreateWithResource(s32 first, s32 unused, s32 second, s32 third)
{
    s32 entry_no;
    s32 result;
    result = 0;
    entry_no = Resource_FindFreeEntry();
    if (entry_no != 0) {
        VramBlock_LoadCached(entry_no, 0x80, UiIcon_ResourceTiles);
        result = RenderOutput_CreateFar(entry_no, 0x40000000, first, second, third);
    }
    return result;
}

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
    VramBlock_LoadCached(slot, copy_mode = resource_mode, UiIcon_ResourceTiles);
    icon = RenderOutput_CreateFar(slot, 0x40000000, first, second, third);
  }
  return icon;
}

void UiIcon_PrepareObject(void *object)
{
    if (object != NULL) {
        FIELD_AT_OFFSET(object, s8, 5) = 1;
        ((struct UiIconObject *) object)->value_16 = FIELD_AT_OFFSET(object, u16, 6);
        FIELD_AT_OFFSET(object, s8, 0x14) = FIELD_AT_OFFSET(object, u16, 8);
        FIELD_AT_OFFSET(object, s8, 0x17) = -0x3F & FIELD_AT_OFFSET(object, s8, 0x17);
        FIELD_AT_OFFSET(object, s8, 0x15) = -4 & FIELD_AT_OFFSET(object, s8, 0x15);
    }
}

void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Ui_LoadCharacterEntryForSlotFar(0, arg1, 0);
}
