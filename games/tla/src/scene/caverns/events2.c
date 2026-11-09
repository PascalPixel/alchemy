#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "text/msg_ids.h"
#include "event_inline.h"

void Func_02000d28(void);

void Func_02000d10(void) {
  s32 result0;

  result0 = Event_GetFlag(0x9af);
  if (result0 == 0)
    goto L_02008d24;
  Func_02000d28();
L_02008d24:
  return;
}

void Func_02000d28(void) {
  Map_CopyCellPictureIndicesRect(35, 12, 18, 12, 3, 3);
  MapGrid_CopyCellAttributes(35, 12, 3, 3, 18, 12);
  Map_CopyCellPictureIndicesRect(84, 13, 83, 13, 1, 1);
  return;
}
