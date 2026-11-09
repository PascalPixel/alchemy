#include "types.h"
#include "event_inline.h"

void Func_02000e40(s32 kind, s32 value);
void Func_02000e48(void);
void Func_02000e50(s32 kind, s32 value);
void Func_02000e60(s32 *a, s32 *b);
void Func_02000d20(s32 index);
void Func_02000d38(s32 value);
void Func_02000dd0(s32 value);
void Func_02000d28(void);
void Func_02000d30(void);

void Func_02000aac(void) { Func_02000d28(); }

void Func_02000ab4(void) { Func_02000d30(); }

void Func_02000abc(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  if (mode == 1)
    Map_CopyCellsRect(a, b, c, d, e, f);
}
