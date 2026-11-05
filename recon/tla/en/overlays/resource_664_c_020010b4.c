// Draft, differ 13 of 211, skeleton gap 5 (tla-en, 2026-11-01)
// The update Func_02001290 gives its shots: while the event work is busy it
// stops; on a cell of kind 212 it ends (cue 539, its end script
// gData_020061cc); otherwise, the first time it reaches the leader (within 16
// pixels on the same height when the leader climbs, else within 8 on the same
// level) and flag 0x104 is clear, it carries the leader along (counter 0x16a
// 200, movement 0); it moves 1.5 pixels a frame along its angle, upward when
// its angle is 0xc000.
// Left, two pairs; the other differ rows are branch distances that follow them.
// 1. Before Func_02005f8c the native puts "adds r0, r6" between "lsls r1" and
// "adds r1, #255"; here r1's chain finishes first. sched2 ties the two on
// priority and forward dependents, and the r1 insn has the lower LUID. Tried
// without change: s32 return, u32 and u16 cue, no prototype, a cue local, s32
// return on Func_02005d6c. Moving the call after the flag stores worsens it.
// 2. The native copies the angle into r0 before "cmp r3, r2", and its else arm
// has no load; here the then arm copies and the else arm reloads. A local (s32
// or u16, one arm or both) or a word compare gives one pseudo: ldrh r0 and
// cmp r0 (differ 19 or 41). The copy sits just before the branch, which is
// where gcse's code hoisting would put it, but this is inferred and untested,
// and that pass is a compiler question, out of scope for a draft.
// What closed the distance from 129: speed as a local, so 0x18000 is rebuilt
// at each call and the shot takes r6; the position pointers taken before the
// height test; near = 0 before them; leader->angle before watchedObject; shot's
// two bytes as struct members with no pointer (raw bytes stay at 15).
// struct Shot overlaps Actor's callback at 0x60, so what those bytes are is
// still open.
// The counter's 200 goes through the EventWork member, so it needs no s32.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "field/event_work.h"

extern u8 gData_020061cc[];
struct Shot {
  u8 unk00[0x62];
  u8 unk62; ///< set once it has hit the leader
  u8 unk63; ///< set with unk62, cleared when it ends
};

struct Actor *Actor_GetById(u32 id);
void Actor_SetAnimationSpeed(struct Actor *object, s32 speed);
s32 Func_02000690(s32 layer, s32 x, s32 z);
void Func_02005f8c(struct Actor *object, s32 cue);
void Func_02005d6c(struct Actor *object, const u8 *script);
s32 Func_02000d18(s32 *a, s32 *b);
s32 Func_02000cec(s32 *a, s32 *b);
s32 GameFlag_Test(s32 flag);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

void Func_020010b4(struct Actor *shot) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  u8 *work = Ram_HeapSlots->eventWork;
  s32 near;
  s32 *leaderPosition;
  s32 *shotPosition;
  s32 speed = 0x18000;

  if (*(s16 *)(work + 0x166) != 0 || *(s16 *)(work + 0x15a) != 0 ||
      *(s16 *)(work + 0x15e) != 0 || *(s16 *)(work + 0x168) != 0 ||
      *(u16 *)(work + 0x1b2) == 0) {
    Actor_SetAnimationSpeed(shot, 0);
    return;
  }
  Actor_SetAnimationSpeed(shot, 16);
  if (Func_02000690(2, shot->x >> 20, (shot->z - shot->y) >> 20) >> 8 == 212) {
    Func_02005f8c(shot, 539);
    ((struct Shot *)shot)->unk63 = 0;
    shot->update = 0;
    Func_02005d6c(shot, gData_020061cc);
    return;
  }
  if (((struct Shot *)shot)->unk62 == 0) {
    near = 0;
    leaderPosition = &leader->x;
    shotPosition = &shot->x;
    if (gMainRoutineWork.renderMode == 2) {
      if (Func_02000d18(leaderPosition, shotPosition) <= 16 &&
          leader->z >> 20 == shot->z >> 20)
        near = 1;
    } else {
      if (Func_02000cec(leaderPosition, shotPosition) <= 8 &&
          leader->y >> 20 == shot->y >> 20)
        near = 1;
    }
    if (near && !GameFlag_Test(0x104)) {
      leader->angle = shot->angle;
      leader->watchedObject = shot;
      leader->priorityFlags &= ~1;
      ((struct FieldSprite *)leader->animation)->priority =
          ((struct FieldSprite *)shot->animation)->priority;
      ((struct EventWork *)work)->unk16a = 200;
      gMainRoutineWork.renderMode = 0;
      ((struct Shot *)shot)->unk62 = 1;
      ((struct Shot *)shot)->unk63 = 1;
    }
  }
  if (shot->angle == 0xc000) {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->y -= Iwram_MulQ16(Math_Sine(shot->angle), speed);
  } else {
    shot->x += Iwram_MulQ16(Math_Cosine(shot->angle), speed);
    shot->z += Iwram_MulQ16(Math_Sine(shot->angle), speed);
  }
}
