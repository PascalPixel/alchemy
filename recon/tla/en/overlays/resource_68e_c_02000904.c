// Measured under -Os, 5 November 2026: differ 244 (tla-en); notes below may predate it.
// Draft, differ 160 of 918, skeleton gap 22 (tla-en, 2026-11-01)
// The twin of ☀️'s TolbiSpring_UpdateTossSimulation, keeping its toss pointer.
// What closes it: the ride sits in r7 once deltaX, deltaZ and speed stop
// outranking it in global-alloc, so each is one function-level variable with a
// trail address (r8, r10, r9 in the listing's hit block and tail). Bounds and
// the attraction distance share minZ; its non-constant first set avoids the
// doubled live length of REG_EQUIV. x and z are one local (r5), which lets minX
// take r6 and leaves maxZ the caller save in r4. Reading position and the first
// obstacle x through union SpringWord changes the alias set and stops PRE
// merging the loads the listing repeats after a conditional store.
// What is left, nearly all from one register swap: index is r9 here and r11
// there, speed the reverse. speed is u15 L166 (rank 0.27), index u44 L602
// (0.365); one more ref on speed passes floor_log2 16 and reverses them. Its
// live length is doubled by the 0x30000 first set. On its own speed outranks
// the ride and takes r7 (differ 629). With speed held in r9 as a diagnostic
// only, differ is 42 and these remain:
//   the str at +68 against the kind pool load (sched2);
//   ones in r1 and the result in r0 are swapped: result ranks above the 1;
//   the tail's mov r8, mov sl, mov r9 order;
//   the last block's r0 and r1 reload rotation.
// Tried without gain: constants set in the first block (CSE folds them, note
// stays), speed = 40 then += toss, the constant 0x1999 in maxX (differ 236),
// distance in maxX, a direct result store (differ 196), result declared late,
// all 120 tail orders (this order is best). The old note's -Os lead is
// retired: the weighting is explained by the shared variables above.
#include "iwram_call.h"
#include "types.h"

struct SpringPoint {
  s32 x;
  s32 y;
  s32 z;
};

// Read through this where the listing reloads after a conditional store.
union SpringWord {
  s32 word;
};

struct SpringRide {
  u16 unk00;
  s16 hold;
  struct SpringPoint trail[5];
  struct SpringPoint velocity;
  s32 frames;
};

struct SpringObstacle {
  struct SpringPoint position;
  s16 heading;
  s16 pauseOrPhase;
  s16 speedMode;
  s16 reactionFrames;
  s16 hitCooldownFrames;
};

extern struct SpringRide gData_02001760;
extern s32 gData_020017b0;
extern struct SpringObstacle gData_020017c0[];
extern s32 gData_02001824;
extern s32 gData_02001828;
extern u8 gData_02001746[];
extern u8 gData_02001749[];

struct Actor *Actor_GetById(u32 id);
void Func_020008f0(s32 actor, s32 active);
void Func_020012e8(struct Actor *object, s32 mode);
void Func_02001408(s32 cue);
void Func_02000884(s32 id, s32 *pos, s32 dir, s32 palette, s32 value);
s32 Math_Sine(s32 angle);
s32 Math_Cosine(s32 angle);

void Func_02000904(void) {
  struct SpringRide *toss;
  struct SpringObstacle *obstacle;
  s32 index;
  s32 height;
  s32 edgeInset;
  s32 motionStep;
  s32 speedMode;
  s32 cycleFrame;
  s32 minX;
  s32 maxX;
  s32 minZ;
  s32 maxZ;
  s32 next;
  s32 reaction;
  s32 modeBits;
  s32 *trail0;
  s32 deltaX;
  s32 deltaZ;
  s32 speed;
  s32 *trail4;
  s32 *result;
  s32 position;
  // Unused: gives the stack frame its 12 bytes.
  struct SpringPoint spare;

  toss = &gData_02001760;
  index = 3;
  do {
    toss->trail[index].x = toss->trail[index - 1].x;
    toss->trail[index].y = toss->trail[index - 1].y;
    toss->trail[index].z = toss->trail[index - 1].z;
    index--;
  } while (index != 0);

  if (toss->hold > 31) {
    toss->trail[0].x += toss->velocity.x;
    height = toss->trail[0].y + toss->velocity.y;
    toss->trail[0].y = height;
    toss->trail[0].z += toss->velocity.z;

    if (height <= 0) {
      toss->trail[0].y = 0;
      if (toss->velocity.y != 0) {
        toss->velocity.y = 0;
        if (gData_020017b0 == 1) {
          Func_020012e8(Actor_GetById(17), 1);
        } else {
          Func_020012e8(Actor_GetById(12), 1);
        }
      }

      if (toss->frames > 0) {
        s32 deltaX;
        s32 deltaZ;

        deltaX = 0x780000 - toss->trail[0].x;
        deltaZ = 0x470000 - toss->trail[0].z;
        deltaX >>= 8;
        deltaZ >>= 8;
        minZ = Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
        toss->velocity.x += (0x1999 * deltaX) / minZ;
        toss->velocity.z += (0x1999 * deltaZ) / minZ;
        toss->velocity.x = toss->velocity.x * 253 / 256;
        toss->velocity.z = toss->velocity.z * 253 / 256;
        toss->frames = toss->frames - 1;
      } else {
        toss->velocity.x = toss->velocity.x * 220 / 256;
        toss->velocity.z = toss->velocity.z * 220 / 256;
        if (toss->velocity.x > -1024 && toss->velocity.x < 1024) {
          toss->velocity.x = 0;
        }
        if (toss->velocity.z > -1024 && toss->velocity.z < 1024) {
          toss->velocity.z = 0;
        }
        if (toss->velocity.x == 0 && toss->velocity.z == 0) {
          if (gData_020017b0 == 1) {
            Func_020012e8(Actor_GetById(17), 2);
            Func_020008f0(15, 0);
            Func_020008f0(14, 0);
            Func_020008f0(13, 0);
          } else {
            Func_020012e8(Actor_GetById(12), 2);
            Func_020008f0(10, 0);
            Func_020008f0(9, 0);
            Func_020008f0(8, 0);
          }
          {
            s32 deltaX = (0x780000 - toss->trail[0].x) >> 16;
            s32 deltaZ = (0x470000 - toss->trail[0].z) >> 16;
            s32 distanceSquared = deltaX * deltaX + deltaZ * deltaZ;

            gData_02001824 = 1;
            result = &gData_02001828;
            if (distanceSquared <= 224) {
              *result = 0;
            } else if (distanceSquared <= 624) {
              *result = 1;
            } else if (distanceSquared <= 1088) {
              *result = 2;
            } else if (distanceSquared <= 1680) {
              *result = 3;
            } else {
              *result = 4;
            }
          }
        }
      }

      position = ((union SpringWord *)&toss->trail[0].z)->word;
      minX = 0x300000;
      maxX = 0xc00000;
      minZ = 0x180000;
      maxZ = 0x780000;
      if (position < 0x2a0000) {
        edgeInset = (0x2a0000 - position) * 42 / 18;
        minX += edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX -= edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      if (position > 0x660000) {
        edgeInset = (position * 42 - 0x660000 * 42) / 18;
        minX = 0x300000 + edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX = 0xc00000 - edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      position = ((union SpringWord *)&toss->trail[0].x)->word;
      if (position < 0x5a0000) {
        edgeInset = (0x5a0000 - position) * 18 / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }
      if (position > 0x960000) {
        edgeInset = (position * 18 - 0x960000 * 18) / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }

      if (position < minX) {
        toss->trail[0].x = minX;
        if (toss->velocity.x < 0) {
          toss->velocity.x = -toss->velocity.x / 2;
        }
      }
      if (toss->trail[0].x > maxX) {
        toss->trail[0].x = maxX;
        if (toss->velocity.x > 0) {
          toss->velocity.x = -toss->velocity.x / 2;
        }
      }
      if (((union SpringWord *)&toss->trail[0].z)->word < minZ) {
        toss->trail[0].z = minZ;
        if (toss->velocity.z < 0) {
          toss->velocity.z = -toss->velocity.z / 2;
        }
      }
      if (toss->trail[0].z > maxZ) {
        toss->trail[0].z = maxZ;
        if (toss->velocity.z > 0) {
          toss->velocity.z = -toss->velocity.z / 2;
        }
      }
    } else {
      toss->velocity.y -= 0x4000;
    }
  }

  index = 0;
  do {
    obstacle = &gData_020017c0[index];

    if (obstacle->reactionFrames > 0) {
      obstacle->reactionFrames = obstacle->reactionFrames - 1;
    }
    if (obstacle->hitCooldownFrames > 0) {
      obstacle->hitCooldownFrames = obstacle->hitCooldownFrames - 1;
    }

    reaction = (u16)obstacle->reactionFrames;
    modeBits = (u16)obstacle->speedMode;
    next = index + 1;
    if (index <= 1) {
      speedMode = (s16)modeBits;
      motionStep = 0x10000;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if ((s16)reaction > 0) {
        if (index == 0) {
          Func_020012e8(Actor_GetById(18), 3);
        } else {
          Func_020012e8(Actor_GetById(19), 3);
        }
      } else {
        if (index == 0) {
          Func_020012e8(Actor_GetById(18), 1);
        } else {
          Func_020012e8(Actor_GetById(19), 1);
        }
        if (obstacle->pauseOrPhase == 0) {
          s32 pos;

          pos = obstacle->position.x;
          if (obstacle->heading == 0) {
            obstacle->position.x = pos + motionStep;
          } else {
            obstacle->position.x = pos - motionStep;
          }
          if (((union SpringWord *)&obstacle->position.x)->word <= 0x400000) {
            obstacle->heading = 0;
            if (index == 1) {
              obstacle->pauseOrPhase = 30;
            }
          }
          if (obstacle->position.x > 0xafffff) {
            obstacle->heading = 1;
            if (index == 1) {
              obstacle->pauseOrPhase = 30;
            }
          }
        } else {
          obstacle->pauseOrPhase = obstacle->pauseOrPhase - 1;
        }
      }
    } else if (index == 2) {
      speedMode = (s16)modeBits;
      motionStep = -64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if ((s16)reaction > 0) {
        Func_020012e8(Actor_GetById(20), 3);
      } else {
        Func_020012e8(Actor_GetById(20), 2);
        obstacle->position.x = Math_Sine(obstacle->heading) * 48 + 0x700000;
        obstacle->position.z = Math_Cosine(obstacle->heading) * 40 + 0x480000;
        cycleFrame = (u16)obstacle->heading + motionStep;
        obstacle->heading = cycleFrame;
        obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
      }
    } else {
      cycleFrame = obstacle->pauseOrPhase & 511;
      speedMode = (s16)modeBits;
      motionStep = 64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if ((s16)reaction > 0) {
        Func_020012e8(Actor_GetById(21), 3);
      } else if (cycleFrame <= 383) {
        obstacle->position.x = Math_Sine(obstacle->heading) * 52 + 0x700000;
        obstacle->position.z = Math_Cosine(obstacle->heading) * 24 + 0x480000;
        obstacle->heading = obstacle->heading + motionStep;
        Func_020012e8(Actor_GetById(21), 2);
      } else {
        Func_020012e8(Actor_GetById(21), 3);
      }
      obstacle->pauseOrPhase = obstacle->pauseOrPhase + 1;
    }

    if (obstacle->hitCooldownFrames == 0 && toss->trail[0].y == 0) {
      s32 distanceSquared;

      deltaX = (obstacle->position.x - toss->trail[0].x) >> 16;
      deltaZ = (obstacle->position.z - toss->trail[0].z) >> 16;
      distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
      if (distanceSquared <= 119 && toss->frames > 30) {
        s32 nextMode;

        speed = 0x30000;
        if (index <= 1) {
          s32 velocityX;

          velocityX = toss->velocity.x;
          if (obstacle->heading == 0) {
            if (velocityX < speed) {
              toss->velocity.x = speed;
              toss->frames = toss->frames - 100;
            }
          } else {
            if (velocityX > -speed) {
              toss->velocity.x = -speed;
              toss->frames = toss->frames - 100;
            }
          }
        } else {
          s32 distance;

          distance = Iwram_Sqrt(distanceSquared);
          toss->velocity.x = -deltaX * speed / distance;
          toss->velocity.z = -deltaZ * speed / distance;
          toss->frames = toss->frames - 100;
        }
        Func_02001408(301);
        nextMode = (obstacle->speedMode + 1) % 3;
        obstacle->reactionFrames = 36;
        obstacle->speedMode = nextMode;
        obstacle->hitCooldownFrames = 30;
      }
    }

    switch (index) {
    case 0:
      Func_02000884(18, &obstacle->position.x, 0,
                    gData_02001746[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    case 1:
      Func_02000884(19, &obstacle->position.x, 0,
                    gData_02001746[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    case 2:
      Func_02000884(20, &obstacle->position.x, 0x8000 - obstacle->heading,
                    gData_02001749[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    case 3:
      Func_02000884(21, &obstacle->position.x, 0xffff - obstacle->heading,
                    gData_02001749[obstacle->speedMode],
                    (obstacle->speedMode << 4) + 16);
      break;
    }

    index = next;
  } while (index != 4);

  toss->trail[4].x = toss->trail[0].x;
  toss->trail[4].y = 0;
  toss->trail[4].z = toss->trail[0].z;

  deltaX = (s32)&toss->trail[1].x;
  speed = (s32)&toss->trail[3].x;
  trail0 = &toss->trail[0].x;
  trail4 = &toss->trail[4].x;
  deltaZ = (s32)&toss->trail[2].x;
  if (gData_020017b0 == 1) {
    Func_02000884(17, trail0, 0, 0, 16);
    Func_02000884(16, trail4, 0, 0, 16);
    Func_02000884(15, (s32 *)deltaX, 0, 0, 16);
    Func_02000884(14, (s32 *)deltaZ, 0, 0, 16);
    Func_02000884(13, (s32 *)speed, 0, 0, 16);
    Func_020012e8(Actor_GetById(15), 4);
    Func_020012e8(Actor_GetById(14), 4);
    Func_020012e8(Actor_GetById(13), 4);
  } else {
    Func_02000884(12, trail0, 0, 0, 16);
    Func_02000884(11, trail4, 0, 0, 16);
    Func_02000884(10, (s32 *)deltaX, 0, 0, 16);
    Func_02000884(9, (s32 *)deltaZ, 0, 0, 16);
    Func_02000884(8, (s32 *)speed, 0, 0, 16);
    Func_020012e8(Actor_GetById(10), 4);
    Func_020012e8(Actor_GetById(9), 4);
    Func_020012e8(Actor_GetById(8), 4);
  }

  if (toss->hold != -1) {
    toss->hold = toss->hold + 1;
  }
}
