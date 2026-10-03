#include "TYPES.H"
#include "RESOURCE.H"
#include "VRAM_BLOCK.H"
#include "RENDER_INPUT.H"
#include "BATTLE_UNIT.H"

/* Main-image symbols: every pool word inside the ROM or the work RAM. */
extern u8 MsgAgilityLabel[];
extern u8 MsgPanelStatLabel[];
extern u8 MsgPanelDefenseLabel[];
void UiText_DrawCharacterAtOffsetFar();
void UiText_DrawNumberAtOffsetFar();
void UiIcon_CreateStatChangeArrow();

extern u8 UiIcon_ResourceTiles[];

/* menu/psynergy_menu/call_icon_routine_with_value.c */
void Ui_LoadCharacterEntryForSlotFar(s32 a, s32 b, s32 c);

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
    u8 flags;
    u16 x : 9;
    u16 other_x : 7;
};

void UiIcon_PrepareObject(struct RenderOutput *object)
{
    /* FAKEMATCH: keep the existing 9-bit OAM assignment and signed byte
       masks. An ordinary unsigned halfword/mask rewrite grows this function
       by 12 bytes in all six editions and changes its two literal masks. */
    if (object != NULL) {
        object->active = 1;
        ((struct UiIconAttributes *)&object->packed)->x = (u16)object->x;
        *(s8 *)&object->packed = (u16)object->y;
        *((s8 *)&object->packed + 3) = -0x3f & *((s8 *)&object->packed + 3);
        *((s8 *)&object->packed + 1) = -4 & *((s8 *)&object->packed + 1);
    }
}

void PsynergyMenu_CallIconRoutineWithValue(s32 arg0, s32 arg1)
{
    Ui_LoadCharacterEntryForSlotFar(0, arg1, 0);
}
