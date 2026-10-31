// Near miss, differ 860 of 918 (tla-en, 2026-10-31): the twin's plain C;
// what remains is GCSE. The native hoists every expression all branches
// compute (the trail addresses before the medal/coin test, the ring result's
// address before the ring tests, the reaction and speed-mode halfwords as
// ldrh before the index tests, sign-extended in each branch, and index + 1
// to the loop body's top, spilled to sp+8) and runs no PRE (loads after a
// conditional store are reloaded). alchemy-gcc's gcse.c does exactly that
// only under -Os: this C compiled with -Os in place of -O2 (analysis only)
// differs from the native in one store's place. At -O2, PRE and no hoisting
// put the toss pointer in r9 instead of r7 and the whole allocation follows.
// Draft: the twin of ☀️'s TolbiSpring_UpdateTossSimulation.
#include "iwram_call.h"
#include "types.h"

struct SpringPoint {
  s32 x;
  s32 y;
  s32 z;
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
        s32 distance;

        deltaX = (0x780000 - toss->trail[0].x) >> 8;
        deltaZ = (0x470000 - toss->trail[0].z) >> 8;
        distance = Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
        toss->velocity.x += (0x1999 * deltaX) / distance;
        toss->velocity.z += (0x1999 * deltaZ) / distance;
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
            if (distanceSquared <= 224) {
              gData_02001828 = 0;
            } else if (distanceSquared <= 624) {
              gData_02001828 = 1;
            } else if (distanceSquared <= 1088) {
              gData_02001828 = 2;
            } else if (distanceSquared <= 1680) {
              gData_02001828 = 3;
            } else {
              gData_02001828 = 4;
            }
          }
        }
      }

      minX = 0x300000;
      maxX = 0xc00000;
      minZ = 0x180000;
      maxZ = 0x780000;
      if (toss->trail[0].z < 0x2a0000) {
        edgeInset = (0x2a0000 - toss->trail[0].z) * 42 / 18;
        minX = 0x300000 + edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX = 0xc00000 - edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      if (toss->trail[0].z > 0x660000) {
        edgeInset = (toss->trail[0].z * 42 - 0x660000 * 42) / 18;
        minX = 0x300000 + edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX = 0xc00000 - edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      if (toss->trail[0].x < 0x5a0000) {
        edgeInset = (0x5a0000 - toss->trail[0].x) * 18 / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }
      if (toss->trail[0].x > 0x960000) {
        edgeInset = (toss->trail[0].x * 18 - 0x960000 * 18) / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }

      if (toss->trail[0].x < minX) {
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
      if (toss->trail[0].z < minZ) {
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

    if (index <= 1) {
      speedMode = obstacle->speedMode;
      motionStep = 0x10000;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
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
          if (obstacle->heading == 0) {
            obstacle->position.x = obstacle->position.x + motionStep;
          } else {
            obstacle->position.x = obstacle->position.x - motionStep;
          }
          if (obstacle->position.x <= 0x400000) {
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
      speedMode = obstacle->speedMode;
      motionStep = -64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
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
      speedMode = obstacle->speedMode;
      motionStep = 64;
      if (speedMode == 1) {
        motionStep = motionStep * 2;
      }
      if (speedMode == 2) {
        motionStep = motionStep * 3;
      }
      if (obstacle->reactionFrames > 0) {
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
      s32 deltaX;
      s32 deltaZ;
      s32 distanceSquared;

      deltaX = (obstacle->position.x - toss->trail[0].x) >> 16;
      deltaZ = (obstacle->position.z - toss->trail[0].z) >> 16;
      distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
      if (distanceSquared <= 119 && toss->frames > 30) {
        s32 speed;
        s32 nextMode;

        speed = 0x30000;
        if (index <= 1) {
          if (obstacle->heading == 0) {
            if (toss->velocity.x < speed) {
              toss->velocity.x = speed;
              toss->frames = toss->frames - 100;
            }
          } else {
            if (toss->velocity.x > -speed) {
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

    index++;
  } while (index != 4);

  toss->trail[4].x = toss->trail[0].x;
  toss->trail[4].y = 0;
  toss->trail[4].z = toss->trail[0].z;

  if (gData_020017b0 == 1) {
    Func_02000884(17, &toss->trail[0].x, 0, 0, 16);
    Func_02000884(16, &toss->trail[4].x, 0, 0, 16);
    Func_02000884(15, &toss->trail[1].x, 0, 0, 16);
    Func_02000884(14, &toss->trail[2].x, 0, 0, 16);
    Func_02000884(13, &toss->trail[3].x, 0, 0, 16);
    Func_020012e8(Actor_GetById(15), 4);
    Func_020012e8(Actor_GetById(14), 4);
    Func_020012e8(Actor_GetById(13), 4);
  } else {
    Func_02000884(12, &toss->trail[0].x, 0, 0, 16);
    Func_02000884(11, &toss->trail[4].x, 0, 0, 16);
    Func_02000884(10, &toss->trail[1].x, 0, 0, 16);
    Func_02000884(9, &toss->trail[2].x, 0, 0, 16);
    Func_02000884(8, &toss->trail[3].x, 0, 0, 16);
    Func_020012e8(Actor_GetById(10), 4);
    Func_020012e8(Actor_GetById(9), 4);
    Func_020012e8(Actor_GetById(8), 4);
  }

  if (toss->hold != -1) {
    toss->hold = toss->hold + 1;
  }
}
