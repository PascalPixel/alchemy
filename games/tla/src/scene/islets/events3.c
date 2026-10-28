#include "types.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "ram_buffer.h"
#include "event_work.h"
#include "call.h"
#include "text/msg_ids.h"

void Func_02003e64();
void Func_02003e6c();
void Func_02003f0c();
void Func_02003fa4();
struct Actor *Actor_GetById();

void Func_0200208c(void) {
  ((void (*)())Func_02003e64)();
  Call1((void (*)())Func_02003fa4, 0);
  Call3((void (*)())Func_02003f0c, gSequenceParentWork.currentOwner, 6, 1);
  ((void (*)())Func_02003e6c)();
  return;
}

void Func_020020b4(void) {
  struct Actor *object0;

  object0 = Actor_GetById(gSequenceParentWork.currentOwner);
  object0->terrainId = 2;
  return;
}

void Func_020020d0(void) {
  struct Actor *object0;

  object0 = Actor_GetById(gSequenceParentWork.currentOwner);
  object0->terrainId = 0;
  return;
}
