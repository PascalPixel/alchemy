// Near miss, differ 2 of 72 (tla-en, 2026-10-30): one reordered pair, a
// scheduler tie. The status panel's list of the five status names, each with
// its icon, or MsgStatusNameNormal when none is set. The native spills the icon
// column (str r3, [sp, #0]) before it builds the count's zero (movs r1, #0);
// this builds the zero first. In sched2 both are ready together at priority 8
// and in the same class; rank_for_schedule then prefers the zero for having
// more dependents (mov fp, r1 and the ldrsh that clobbers r1; the store has
// only that ldrsh). The store would lead only if its dependence into the
// sign-extending ldrsh cost 2 cycles, which -mthumb-slow-loads gives a plain
// load but not ldrsh: the cost model, not a source form.
// Measured before: declaring the column before the count 13, a barrier between
// them 15, a one-pass block around the column 17 and around both 10; no single
// empty asm or one-pass block reaches 0. Measured 2026-10-30: the count's zero
// assigned between the two coordinates 13 (reload then builds -16 from that
// zero), coordinates before the count 38 to 40, the row before the column 14,
// count as u32, `!count` or a do-while loop 2, count as s16 51 or u8 21, the
// icon row as a loop-invariant base plus y 10 or 53; the permuter (4 minutes,
// 11,569 candidates) found nothing better.
#include "types.h"

struct MenuPanel {
  u8 unk00[12];
  s16 x;
  s16 y;
};

extern char MsgStatusNameNormal[];
extern char MsgStatusNameDown[];

void MenuIcon_SetSlot(s32 index, u32 x, s32 mode);
void Text_DrawCharacterAtOffsetFar(s32 message, void *window, s32 x, s32 y);

// Lists usable entries with icons and names, or "none" when no entries qualify.
void Func_081054e0(struct MenuPanel *panel, s32 x, s32 y, const u8 *enabled) {
  s32 count = 0;
  s32 icon_x = panel->x * 8 + x + 8;
  s32 icon_y = panel->y * 8 + y + 8;
  s32 i;

  for (i = 0; i <= 4; i++) {
    if (enabled[i] != 0) {
      MenuIcon_SetSlot(i, icon_x, icon_y);
      Text_DrawCharacterAtOffsetFar((s32)MsgStatusNameDown + i, panel, x + 16,
                                      y);
      y += 16;
      icon_y += 16;
      count++;
    } else {
      MenuIcon_SetSlot(i, -16, -16);
    }
  }
  if (count == 0)
    Text_DrawCharacterAtOffsetFar((s32)MsgStatusNameNormal, panel, x, y);
}
