#include "types.h"
#include "ram_buffer.h"
#include "sequence_parent_work.h"
#include "event_work.h"

extern char SceneId_SuhallaGate1;
extern char SceneId_SuhallaGate2;
extern char SceneId_SuhallaGate3;

void Func_02000110(void);
void Func_02000168(void);
void Func_02000264(void);

s32 Func_0200028c(void) {
  struct EventWork *work;
  s16 scene;

  work = Ram_HeapSlots->eventWork;
  work->screenEffect = 256;
  scene = gSequenceParentWork.scene;
  if (scene == (s32)&SceneId_SuhallaGate1)
    Func_02000110();
  else if (scene == (s32)&SceneId_SuhallaGate2)
    Func_02000168();
  else if (scene == (s32)&SceneId_SuhallaGate3)
    Func_02000264();
  return 0;
}
