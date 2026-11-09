#include "types.h"
#include "task.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct SpringPoint {
  s32 x;
  s32 y;
  s32 z;
};

struct SpringObstacle {
  struct SpringPoint position;
  s16 heading;
  s16 pauseOrPhase;
  s16 speedMode;
  s16 reactionFrames;
  s16 hitCooldownFrames;
};

struct SpringRide {
  u16 unk00;
  u16 hold;
  struct SpringPoint trail[5];
  struct SpringPoint velocity;
  s32 frames;
};

extern struct SpringRide gData_02001760;
extern s32 gData_020017b0;
extern struct SpringObstacle gData_020017c0[];
extern s32 gData_02001820;
extern s32 gData_02001824;
extern s32 gData_02001828;
extern u8 gData_0200174c[];
extern u8 gData_02001750[];
extern u16 gData_02001754[];

void Func_020008f0(s32 actor, s32 active);
void Func_02000904(void);
void Func_020012e8(struct Actor *object, s32 mode);

/// Places the four actors from their tables, resets the ride and installs
/// its task.
void Func_020010f4(void) {
  struct SpringRide *ride = &gData_02001760;
  s32 i = 0;
  u8 *xtbl;
  u16 *htbl;
  struct SpringObstacle *rec;
  u8 *ztbl;
  s32 zero;

  htbl = gData_02001754;
  ztbl = gData_02001750;
  xtbl = gData_0200174c;
  rec = gData_020017c0;

  do {
    zero = 0;
    rec->position.x = *xtbl << 16;
    /* FAKEMATCH(barrier): the x store ends the first group, so the z table's
     * read follows it at once; as plain stores of 0 with no barrier, 2 of 52
     * instructions differ in tla-en. */
    __asm__("");
    rec->position.z = *ztbl << 16;
    rec->position.y = zero;
    rec->heading = *htbl;
    rec->pauseOrPhase = zero;
    rec->speedMode = zero;
    rec->reactionFrames = zero;
    rec->hitCooldownFrames = zero;
    i++;
    xtbl++;
    ztbl++;
    htbl++;
    rec++;
  } while (i != 4);

  ride->trail[0].x = 0xffe20000;
  ride->trail[0].y = 0;
  ride->trail[0].z = 0x640000;
  ride->velocity.x = 0;
  ride->velocity.y = 0;
  ride->velocity.z = 0;
  ride->frames = 0;

  Func_020012e8(Actor_Get(20), 2);
  Func_020012e8(Actor_Get(21), 2);

  Task_AddFrame((TaskFunc)Func_02000904, TASK_PASS_VBLANK + 3);
}

/// Runs the spring ride from one side until it ends, and returns its result.
s32 Func_02001184(s32 side) {
  struct SpringRide *ride;

  ride = &gData_02001760;
  ride->trail[0].y = 0;
  ride->trail[1].y = 0;
  ride->trail[2].y = 0;
  ride->trail[3].y = 0;
  gData_020017b0 = side;
  gData_02001824 = 0;
  ride->hold = 0xffff;
  for (gData_02001820 = 0;; gData_02001820++) {
  if (gData_02001820 == 50)
    Event_PlaySound(300);
  /* FAKEMATCH: the volatile read keeps the frame's reload where both paths
   * meet; plain, PRE copies it onto the cue's path (differ 2). */
  if (gData_02001820 == 16) {
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 39);
    ride->hold = 0;
    ride->velocity.x = 0x14ccc;
    ride->velocity.y = 0x40000;
    ride->velocity.z = -0x20000;
    ride->trail[0].x = 0x780000;
    ride->trail[0].y = 0x100000;
    ride->trail[0].z = 0x980000;
    ride->frames = 300;
    if (gData_020017b0 == 1) {
      Func_020012e8(Actor_Get(16), 3);
      Func_020012e8(Actor_Get(17), 0);
      Func_020008f0(15, 1);
      Func_020008f0(14, 1);
      Func_020008f0(13, 1);
    } else {
      Func_020012e8(Actor_Get(11), 3);
      Func_020012e8(Actor_Get(12), 0);
      Func_020008f0(10, 1);
      Func_020008f0(9, 1);
      Func_020008f0(8, 1);
    }
  }
  System_Wait(1);
  if (gData_02001824 == 1)
    break;
  }
  return gData_02001828;
}
