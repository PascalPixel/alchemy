// Draft of overlay 698's Func_020014dc, in 18 overlays of the 698 family:
// differ 16 of 131. The leader's cell copied with sound 229, the party walks
// off it shrinking while REG_WINOUT shows the sprite window, then the screen
// transition. The frame count is the column variable set to 0 just before
// its loop and counted after the wait, which gives the leader r6 and the count
// r7 as the native has them (54 with the count set before the stores). Left:
// the party state's base and its offset take each other's registers when the
// owner's address is first built, and the table is copied out of r8 before
// the last flag store. Tried: the owner's address in a local (66), the
// declarations split from their initialisers, permute 6 minutes.
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
void Func_020013e4(void);
void Func_02001b44(s32 sound);
void Func_02001a44(s32 srcX, s32 srcZ, s32 width, s32 height, s32 dstX,
                   s32 dstZ);
void FieldEvent_WaitFrames(s32 frames);
void WaitFrames(s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, u32 flags);
void Object_SetModeById(u32 id, s32 mode);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
#include "iowrite.h"

void Func_020014dc(struct CellTarget *table, struct CellTargetEntry *entry) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 x = (leader->x >> 20) | (s16)(table->x & 64);
  s32 z = (leader->z >> 20) | (s16)(table->z & 64);

  Func_020013e4();
  Func_02001b44(229);
  Func_02001a44(table->x, table->z + 2, 1, 1, x, z);
  FieldEvent_WaitFrames(12);
  leader->angle = 0x4000;
  leader->motionFlags = 0;
  ObjectDispatch_SetSingleChildFlags(Object_GetById(gPartyState.currentOwner),
                                     0);
  Io_Write16(0xf1f, &REG_WINOUT);
  REG_DISPCNT |= 0x8000;
  table->shown = 1;
  Object_SetModeById(gPartyState.currentOwner, 28);
  FieldEvent_WaitFrames(16);
  x = 0;
  do {
    if (x == 5)
      Func_02001b44(204);
    leader->scaleX += -0x400;
    leader->scaleY += -0x300;
    leader->y += -0x9999;
    WaitFrames(1);
    x++;
  } while (x <= 39);
  Object_GetById(gPartyState.currentOwner)->animationKind = 0;
  table->shown = 0;
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x209;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
}
