// Draft of Menu_SelectSaveSlotAction: differ 2 of 84 (tla-en, 2026-10-18).
// Its ☀️ twin is games/tbs/src/window/battle_command.c's function
// of the same name: it scans the save records, lists the actions the slot
// group allows (plus two optional entries behind EWRAM flags), runs the
// selection and maps the position to an action. The Lost Age's second
// optional entry is 31 behind gData_02005350, the menu is 7 wide, and with
// the first option off, action 4 becomes 5.
//
// Left: instruction order only (the table's pool load sits one place later,
// after the first add of group * 6). Rewritten from The Broken Seal's form,
// which used its own names and option entry.
#include "types.h"

extern s16 gData_020036d8;
extern s16 gData_02005350;
extern s8 gMenu_SaveSlotActionByPosition[];

s32 Func_080432a4(void);
void AffineEffect_InitializeWork(void);
void Menu_AppendResourceEntry(s32 resource);
void Menu_CenterResourceEntries(s32 x, s32 width, s32 y);
s32 Menu_RunResourceSelectionLoop(s32 initial);
void Menu_EndResourceSelection(void);

s32 Menu_SelectSaveSlotAction(void) {
  s32 type;
  s32 ret;
  s32 initial;
  u32 group;

  group = 0;
  initial = 0;
  type = Func_080432a4();
  if (type < 0)
    return -1;
  if (type == 0)
    return 0;
  if (type == 3)
    group = 1;
  else if (type == 103)
    group = 2;
  else if (type > 100)
    group = 3;
  else
    initial = 1;
  AffineEffect_InitializeWork();
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(21);
  if (group <= 1)
    Menu_AppendResourceEntry(22);
  if (group == 0 || group == 3)
    Menu_AppendResourceEntry(23);
  Menu_AppendResourceEntry(24);
  if (gData_020036d8 != 0)
    Menu_AppendResourceEntry(29);
  if (gData_02005350 != 0)
    Menu_AppendResourceEntry(31);
  Menu_CenterResourceEntries(17, 7, 0);
  ret = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (ret >= 0)
    ret = gMenu_SaveSlotActionByPosition[ret + group * 6];
  if (gData_020036d8 == 0 && ret == 4)
    ret = 5;
  return ret;
}
