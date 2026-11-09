#include "types.h"
#include "event_inline.h"

struct Probe {
  s32 unk00[6];
};

s32 Func_02001f60(void);
s32 Func_02000c44(struct Probe *probe);
void Func_02000ec8(struct Probe probe);

void Func_02000054(void) {
  if (Func_02001f60())
    FieldEvent_SetExitRequest(13);
}

void Func_02000068(void) {
  struct Probe probe;

  if (Func_02000c44(&probe))
    Func_02000ec8(probe);
}

void Func_02000090(void) {}
