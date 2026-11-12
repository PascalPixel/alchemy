/// @file
/// Scene code of overlay 0x6a8, in ROM order, from s385c's offset up to the next object the overlay links.

#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "ram_buffer.h"

void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);
void Func_020043c4(struct Actor *object, s32 mode);
extern u32 gInput;
extern const s16 gData_02004740[];

void Func_0200385c(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->action != 0) {
    object->action--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(0x106))
    angle = -1;
  else
    angle = gData_02004740[(gInput >> 4) & 15];
  if (angle == -1) {
    Func_020043c4(object, 9);
    return;
  }
  turn = (s16)(angle - object->angle);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->angle += turn;
  Func_020043c4(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

void Func_02004594(void);
void Func_020045e4(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void FlagSet(s32 flag) { GameFlag_SetBit(flag); }

void Func_020038e8(void) {
  void *work = Ram_HeapSlots->eventWork;

  if (FIELD_AT(work, s16, 0xca2) != 0) {
    FlagSet(0x106);
    Func_02004594();
    Func_020045e4();
    GameFlag_ClearBit(0x106);
  }
}
