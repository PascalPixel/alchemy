#include "types.h"
#include "edition.h"
#include "menu.h"
#include "tla_edition.h"

extern s16 gData_020036d8;
extern s16 gData_02005350;
extern s8 gMenu_SaveSlotActionByPosition[];

s32 Func_080432a4(void);
void Menu_CenterResourceEntries(s32, s32, s32);

/// Offers the actions the save records allow (plus two optional entries
/// behind EWRAM flags) and maps the chosen position to an action. The second
/// optional entry is 31, and with the first option off, action 4 becomes 5.
// As ☀️'s Menu_SelectSaveSlotAction. A module of its own: in the Japanese
// edition it follows Menu_AnimateSelectionToEntry, which is still scaffolded
// there.
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
  Menu_CenterResourceEntries(17, SAVE_SLOT_MENU_WIDTH, 0);
  ret = Menu_RunResourceSelectionLoop(initial);
  Menu_EndResourceSelection();
  if (ret >= 0) {
    u32 twice = group * 2;

    /* FAKEMATCH: the one-pass block issues the table's pool load between the
       shift and the add; as ret + group * 6, 2 instructions differ in
       tla-en. */
    do {
    } while (0);
    ret = gMenu_SaveSlotActionByPosition[ret + (twice + group) * 2];
  }
  if (gData_020036d8 == 0 && ret == 4)
    ret = 5;
  return ret;
}
