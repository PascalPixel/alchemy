#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

extern s32 gData_020026f0;
extern s32 gData_020026f4;
extern u16 gData_0200271c[];
extern u16 gData_0200273c[];
extern u16 gData_02002740[];
extern u32 gFrameCount;

s32 __modsi3();
s32 GameFlag_Test();
void Scheduler_AddOrUpdateCallback();
struct FieldActor *Object_GetById();
s32 Map_GetTerrainHeight();
void Func_02001e28();
void Func_02001ef8();
void Func_02001e48();
void Func_02001ed0();
void ObjectDispatch_SetSingleChildFlags();
void WaitFrames();
void Camera_MoveWithinMapBounds();
struct FieldActor *Func_02001ea8();
void Func_02001dd8();
void Func_02001f08();
void Func_02001de0();
void Camera_FollowActor();
void Func_02001e50();

// differ 8: native rebuilds the buffer address (mov r7, sp) inside each pass
// and reads the table as [table, offset]; here the address is set before the
// loop.
void Func_020005e4(void) {
  u16 colors[16];
  s32 phase;
  s32 i;
  s32 offset;
  u16 *dst;

  phase = (gData_020026f0 / 4) % 5;
  if (gData_020026f4 != 0) {
    u8 *work = Ram_HeapSlots->eventWork;

    if (*(s16 *)(work + 358) != 0 && (HALF_AT(work, 358) & 0x3ff) != 153)
      return;
    if (*(s8 *)(work + 0xca4) != 0)
      return;
    if (*(s16 *)(work + 350) != 0)
      return;
  }
  dst = colors;
  i = 0;
  offset = 4;
next:
  *(dst + (phase + i) % 5) = *(u16 *)(offset + (u8 *)gData_0200271c);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(dst, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_020026f0++;
}

extern u8 gData_020026f8[];
extern s32 gData_02002718;
void ObjectDispatch_ApplyValueToChildren();
s32 Func_02001ee8();
void Func_02001ed8();
void Func_02001db8();
s32 Math_Cosine();
s32 Math_Sine();

s32 Func_02000964(s32 *a, s32 *b) {
  s32 dx = (a[0] - b[0]) >> 16;
  s32 dz = (a[2] - b[2]) >> 16;

  return Iwram_Sqrt(dx * dx + dz * dz);
}

struct EventHold {
  u8 unk000[362];
  u16 hold;
};

static inline s32 MapAttr(s32 x, s32 z, s32 layer) {
  /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep the argument
   * order */
  return Func_02001ee8(x, z, layer);
}

void Func_02000990(struct FieldActor *ball) {
  u8 *work;
  struct FieldActor *leader;
  s32 pos[3];
  s32 d;
  s32 near;
  s32 t;

  work = Ram_HeapSlots->eventWork;
  leader = Object_GetById(gPartyState.currentOwner);
  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    ObjectDispatch_ApplyValueToChildren(ball, 0);
    return;
  }
  ObjectDispatch_ApplyValueToChildren(ball, 16);
  t = ball->timer + 1;
  ball->timer = t;
  if ((t & 31) == 31)
    Func_02001f08(231);
  ball->sprite->priority = leader->sprite->priority;
  if (MapAttr(ball->x, ball->z, 2) == 255)
    goto stop;
  pos[0] = ball->x;
  pos[1] = ball->y;
  pos[2] = ball->z;
  Func_02001ed8(pos);
  d = pos[0];
  if (d > 0x1100000)
    goto stop;
  if (d < -0x200000)
    goto stop;
  if (BYTE_AT(ball, 98) == 0) {
    d = leader->y - ball->y;
    if (d < 0)
      d = ball->y - leader->y;
    near = 0;
    if (Func_02000964(&leader->x, &ball->x) <= 12 && d < 0xc0000)
      near = 1;
    if (near && !GameFlag_Test(260)) {
      WORD_AT(leader, 104) = (s32)ball;
      leader->facing = ball->facing;
      leader->priorityFlags &= 254;
      ((struct EventHold *)work)->hold = 200;
      gPartyState.renderMode = 0;
      BYTE_AT(ball, 98) = 1;
      BYTE_AT(ball, 99) = 1;
    }
  }
  ball->x += Iwram_MulQ16(Math_Cosine(ball->facing), ball->speed);
  ball->z += Iwram_MulQ16(Math_Sine(ball->facing), ball->speed);
  return;
stop:
  BYTE_AT(ball, 99) = 0;
  WORD_AT(ball, 108) = 0;
  Func_02001db8(ball, gData_020026f8);
  Func_02001f08(228);
  gData_02002718 = 0;
}
