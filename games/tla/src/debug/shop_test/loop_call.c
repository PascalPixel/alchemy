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

void Func_02000394(void) {
  s32 pos[2];

  Func_02000e60(&pos[1], &pos[0]);
}

void Func_020003a4(void) {
  s32 i;

  for (i = 0; i < 8; i++)
    Func_02000d20(i);
}
