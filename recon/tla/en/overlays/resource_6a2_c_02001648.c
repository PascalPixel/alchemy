// Draft, differ 60 of 131: overlay 6a2's copy of the function drafted in
// resource_698_c_020014dc.c (overlay 698's Func_020014dc), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "io_reg.h"

struct CellTargetEntry;

struct CellTarget {
  s16 firstFlag;
  s16 flagByte;
  s16 kind;
  s16 x;
  s16 z;
  s16 count;
  s16 current;
  s16 shown;
  s16 tile;
};

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02001550(void);
void Func_02001ca0(s32 sound);
void Func_02001bb0(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void FieldEvent_WaitFrames(s32 frames);
void WaitFrames(s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, u32 flags);
void Object_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
#include "iowrite.h"

void Func_02001648(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Func_02001550();
  Func_02001ca0(229);
  Func_02001bb0(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_WaitFrames(12);
  leader->angle = 0x4000;
  leader->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  x = 0;
  table->shown = 1;
  Object_SetModeById(gPartyState.currentOwner, 28);
  FieldEvent_WaitFrames(16);
  do {
    if (x == 5)
      Func_02001ca0(204);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    x++;
    WaitFrames(1);
  } while (x <= 39);
  Object_GetById(gPartyState.currentOwner)->animationKind = 0;
  table->shown = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
