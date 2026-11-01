// Draft, differ 609 of 918, skeleton gap 115 (tla-en, 2026-11-01)
// The twin's plain C, with what the native hoists written as locals: the ride
// reaction and speed-mode halfwords (s32, so they load with ldrh and extend in
// each branch), index + 1, the obstacle's x, velocity.x, the trail addresses,
// the distance-result address. The ride is named directly, not through a
// pointer: a pointer's constant REG_EQUIV doubles its live length and it fell
// to r9, a mov at every access; with the global it costs one pool load per
// region (the skeleton's added ldr) and a caller save of r4 around the calls.
// What remains points at -Os, not at the C: -O2 weights refs by loop depth
// (deltaX and deltaZ outrank the ride pointer), avoids reusing a dying qty in
// local-alloc (flow.c, local-alloc.c test optimize_size), and runs PRE, so
// loads after a conditional store are not reloaded. The native behaves as
// unweighted, reuses r5, hoists and never runs PRE; next spills to sp+8 there
// and loop.c folds it into index here. Tried without gain: a goto loop (gap
// 138), one pointer per region (176, 193), block-scoped limit constants.
// The twin of ☀️'s TolbiSpring_UpdateTossSimulation.
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
  s32 *trail1;
  s32 *trail2;
  s32 *trail3;
  s32 *trail4;
  s32 *result;
  s32 x;
  s32 z;
  // Unused: gives the stack frame its 12 bytes.
  struct SpringPoint spare;

  index = 3;
  do {
    gData_02001760.trail[index].x = gData_02001760.trail[index - 1].x;
    gData_02001760.trail[index].y = gData_02001760.trail[index - 1].y;
    gData_02001760.trail[index].z = gData_02001760.trail[index - 1].z;
    index--;
  } while (index != 0);

  if (gData_02001760.hold > 31) {
    gData_02001760.trail[0].x += gData_02001760.velocity.x;
    height = gData_02001760.trail[0].y + gData_02001760.velocity.y;
    gData_02001760.trail[0].y = height;
    gData_02001760.trail[0].z += gData_02001760.velocity.z;

    if (height <= 0) {
      gData_02001760.trail[0].y = 0;
      if (gData_02001760.velocity.y != 0) {
        gData_02001760.velocity.y = 0;
        if (gData_020017b0 == 1) {
          Func_020012e8(Actor_GetById(17), 1);
        } else {
          Func_020012e8(Actor_GetById(12), 1);
        }
      }

      if (gData_02001760.frames > 0) {
        s32 deltaX;
        s32 deltaZ;
        s32 distance;

        deltaX = 0x780000 - gData_02001760.trail[0].x;
        deltaZ = 0x470000 - gData_02001760.trail[0].z;
        deltaX >>= 8;
        deltaZ >>= 8;
        distance = Iwram_Sqrt(deltaX * deltaX + deltaZ * deltaZ);
        gData_02001760.velocity.x += (0x1999 * deltaX) / distance;
        gData_02001760.velocity.z += (0x1999 * deltaZ) / distance;
        gData_02001760.velocity.x = gData_02001760.velocity.x * 253 / 256;
        gData_02001760.velocity.z = gData_02001760.velocity.z * 253 / 256;
        gData_02001760.frames = gData_02001760.frames - 1;
      } else {
        gData_02001760.velocity.x = gData_02001760.velocity.x * 220 / 256;
        gData_02001760.velocity.z = gData_02001760.velocity.z * 220 / 256;
        if (gData_02001760.velocity.x > -1024 && gData_02001760.velocity.x < 1024) {
          gData_02001760.velocity.x = 0;
        }
        if (gData_02001760.velocity.z > -1024 && gData_02001760.velocity.z < 1024) {
          gData_02001760.velocity.z = 0;
        }
        if (gData_02001760.velocity.x == 0 && gData_02001760.velocity.z == 0) {
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
            s32 deltaX = (0x780000 - gData_02001760.trail[0].x) >> 16;
            s32 deltaZ = (0x470000 - gData_02001760.trail[0].z) >> 16;
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

      z = gData_02001760.trail[0].z;
      minX = 0x300000;
      maxX = 0xc00000;
      minZ = 0x180000;
      maxZ = 0x780000;
      if (z < 0x2a0000) {
        edgeInset = (0x2a0000 - z) * 42 / 18;
        minX += edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX -= edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      if (z > 0x660000) {
        edgeInset = (z * 42 - 0x660000 * 42) / 18;
        minX = 0x300000 + edgeInset;
        if (minX > 0x5a0000) {
          minX = 0x5a0000;
        }
        maxX = 0xc00000 - edgeInset;
        if (maxX < 0x960000) {
          maxX = 0x960000;
        }
      }
      x = gData_02001760.trail[0].x;
      if (x < 0x5a0000) {
        edgeInset = (0x5a0000 - x) * 18 / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }
      if (x > 0x960000) {
        edgeInset = (x * 18 - 0x960000 * 18) / 42;
        minZ = 0x180000 + edgeInset;
        if (minZ > 0x2a0000) {
          minZ = 0x2a0000;
        }
        maxZ = 0x780000 - edgeInset;
        if (maxZ < 0x660000) {
          maxZ = 0x660000;
        }
      }

      if (x < minX) {
        gData_02001760.trail[0].x = minX;
        if (gData_02001760.velocity.x < 0) {
          gData_02001760.velocity.x = -gData_02001760.velocity.x / 2;
        }
      }
      if (gData_02001760.trail[0].x > maxX) {
        gData_02001760.trail[0].x = maxX;
        if (gData_02001760.velocity.x > 0) {
          gData_02001760.velocity.x = -gData_02001760.velocity.x / 2;
        }
      }
      if (gData_02001760.trail[0].z < minZ) {
        gData_02001760.trail[0].z = minZ;
        if (gData_02001760.velocity.z < 0) {
          gData_02001760.velocity.z = -gData_02001760.velocity.z / 2;
        }
      }
      if (gData_02001760.trail[0].z > maxZ) {
        gData_02001760.trail[0].z = maxZ;
        if (gData_02001760.velocity.z > 0) {
          gData_02001760.velocity.z = -gData_02001760.velocity.z / 2;
        }
      }
    } else {
      gData_02001760.velocity.y -= 0x4000;
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

    next = index + 1;
    reaction = (u16)obstacle->reactionFrames;
    modeBits = (u16)obstacle->speedMode;
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

    if (obstacle->hitCooldownFrames == 0 && gData_02001760.trail[0].y == 0) {
      s32 deltaX;
      s32 deltaZ;
      s32 distanceSquared;

      deltaX = (obstacle->position.x - gData_02001760.trail[0].x) >> 16;
      deltaZ = (obstacle->position.z - gData_02001760.trail[0].z) >> 16;
      distanceSquared = deltaX * deltaX + deltaZ * deltaZ;
      if (distanceSquared <= 119 && gData_02001760.frames > 30) {
        s32 speed;
        s32 nextMode;

        speed = 0x30000;
        if (index <= 1) {
          s32 velocityX;

          velocityX = gData_02001760.velocity.x;
          if (obstacle->heading == 0) {
            if (velocityX < speed) {
              gData_02001760.velocity.x = speed;
              gData_02001760.frames = gData_02001760.frames - 100;
            }
          } else {
            if (velocityX > -speed) {
              gData_02001760.velocity.x = -speed;
              gData_02001760.frames = gData_02001760.frames - 100;
            }
          }
        } else {
          s32 distance;

          distance = Iwram_Sqrt(distanceSquared);
          gData_02001760.velocity.x = -deltaX * speed / distance;
          gData_02001760.velocity.z = -deltaZ * speed / distance;
          gData_02001760.frames = gData_02001760.frames - 100;
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

  gData_02001760.trail[4].x = gData_02001760.trail[0].x;
  gData_02001760.trail[4].y = 0;
  gData_02001760.trail[4].z = gData_02001760.trail[0].z;

  trail1 = &gData_02001760.trail[1].x;
  trail2 = &gData_02001760.trail[2].x;
  trail3 = &gData_02001760.trail[3].x;
  trail4 = &gData_02001760.trail[4].x;
  trail0 = &gData_02001760.trail[0].x;
  if (gData_020017b0 == 1) {
    Func_02000884(17, trail0, 0, 0, 16);
    Func_02000884(16, trail4, 0, 0, 16);
    Func_02000884(15, trail1, 0, 0, 16);
    Func_02000884(14, trail2, 0, 0, 16);
    Func_02000884(13, trail3, 0, 0, 16);
    Func_020012e8(Actor_GetById(15), 4);
    Func_020012e8(Actor_GetById(14), 4);
    Func_020012e8(Actor_GetById(13), 4);
  } else {
    Func_02000884(12, trail0, 0, 0, 16);
    Func_02000884(11, trail4, 0, 0, 16);
    Func_02000884(10, trail1, 0, 0, 16);
    Func_02000884(9, trail2, 0, 0, 16);
    Func_02000884(8, trail3, 0, 0, 16);
    Func_020012e8(Actor_GetById(10), 4);
    Func_020012e8(Actor_GetById(9), 4);
    Func_020012e8(Actor_GetById(8), 4);
  }

  if (gData_02001760.hold != -1) {
    gData_02001760.hold = gData_02001760.hold + 1;
  }
}
