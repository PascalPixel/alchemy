// Draft, differ 10 of 197, skeleton gap 3 (tla-en, 2026-11-01)
// Worst of the three functions here, measured with --symbol: Func_02000990.
// Func_020005e4 is differ 6 of 83, skeleton gap 1; Func_02000964 matches.
//
// Func_02000990, a rolling ball's update. All three left gaps are sched2 ties,
// and the instructions are all there:
// - the ball's facing is copied to the leader after the store at offset 104
//   in the native. A held s32 facing local, read first, then the store, then
//   leader->facing = facing gives differ 4 of 197, gap 2 with the local below,
//   but is a forced temporary (S2) and is not used here.
// - the native ends the sprite priority copy with orrs r3, r2; a held local,
//   s32 priority = BYTE_AT(leader->sprite, 9) & 12, gives differ 6, gap 3.
// - ldr r5, [pool] for Iwram_MulQ16Entry before ldr r1, [speed]: the load of speed
//   depends on the Math_Cosine call, the pool load does not, so sched2 takes
//   the pool load first. A local function pointer, or speed read as WORD_AT
//   or const, changes nothing.
// The priority copy is written through s8 at byte 9, not the FieldSprite
// bitfield: that makes the store char-aliased, so the x and z loads after it
// keep their order and movs r2, #2 comes first, as in the native. A u8 view, or
// the bitfield through a union, does not.
//
// Func_020005e4, a palette cycle: gcse's PRE hoists the colours' address out
// of the goto loop, so mov r7, sp is one slot early; twin notes in
// resource_6a4_c_0200079c.c list what was tried. Locals for the slot and the
// table reach differ 4, tagged there as FAKEMATCH(temporary); no local here.
#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "field_event.h"
#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define S8_AT(base, offset) (*(s8 *)((u8 *)(base) + (offset)))
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
void Task_Add();
struct FieldActor *Actor_GetById();
s32 Map_GetTerrainHeight();
void Func_02001e28();
void Func_02001ef8();
void Func_02001e48();
void Func_02001ed0();
void Actor_SetSingleChildFlags();
void wait();
void Camera_MoveWithinMapBounds();
struct FieldActor *Func_02001ea8();
void Func_02001dd8();
void Func_02001f08();
void Func_02001de0();
void Camera_FollowActor();
void Func_02001e50();

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
  colors[(phase + i) % 5] = *(u16 *)((u8 *)gData_0200271c + offset);
  i++;
  offset += 2;
  if (i <= 4)
    goto next;
  Dma_Set(dst, (void *)0x05000184, 0x84000002, REG_DMA3);
  gData_020026f0++;
}

extern u8 gData_020026f8[];
extern s32 gData_02002718;
void Actor_SetAnimationSpeed();
s32 Func_02001ee8(s32 x, s32 z, s32 layer);
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

void Func_02000990(struct FieldActor *ball) {
  u8 *work;
  struct FieldActor *leader;
  s32 pos[3];
  s32 d;
  s32 near;
  s32 t;
  s32 zero;

  work = Ram_HeapSlots->eventWork;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (*(s16 *)(work + 358) != 0 || *(s16 *)(work + 346) != 0 ||
      *(s16 *)(work + 350) != 0 || *(s16 *)(work + 360) != 0) {
    Actor_SetAnimationSpeed(ball, 0);
    return;
  }
  Actor_SetAnimationSpeed(ball, 16);
  t = ball->timer + 1;
  ball->timer = t;
  if ((t & 31) == 31)
    Func_02001f08(231);
  S8_AT(ball->sprite, 9) =
      (S8_AT(leader->sprite, 9) & 12) | (S8_AT(ball->sprite, 9) & ~12);
  if (Func_02001ee8(ball->x, ball->z, 2) == 255)
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
      leader->facing = ball->facing;
      WORD_AT(leader, 104) = (s32)ball;
      leader->priorityFlags &= 254;
      ((struct EventHold *)work)->hold = 200;
      gMainRoutineWork.renderMode = 0;
      BYTE_AT(ball, 98) = 1;
      BYTE_AT(ball, 99) = 1;
    }
  }
  ball->x += Iwram_MulQ16(Math_Cosine(ball->facing), ball->speed);
  ball->z += Iwram_MulQ16(Math_Sine(ball->facing), ball->speed);
  return;
stop:
  zero = 0;
  BYTE_AT(ball, 99) = zero;
  WORD_AT(ball, 108) = zero;
  Func_02001db8(ball, gData_020026f8);
  Func_02001f08(228);
  gData_02002718 = zero;
}
