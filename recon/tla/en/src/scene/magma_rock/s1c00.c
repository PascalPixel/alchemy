#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "call.h"

struct MagmaRockEffectOptions {
  s32 kind;
  s32 palette;
  s32 scaleX;
  s32 scaleY;
  u8 unk10[8];
  u16 id;
  u8 unk1a[2];
  const void *data;
  s32 unk20;
  void (*update)();
};

extern u32 gFrameCount;

struct Actor *Actor_GetById(s32 id);
void Func_02003aec(void);
void Func_02003bcc(s32 value);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_02003c54(s32 cue);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetActionById(s32 id, s32 action);
u32 Random16Far(void);
void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   struct MagmaRockEffectOptions *options);
void wait(s32 frames);
void Camera_MoveWithinMapBounds(s32 a, s32 b, s32 c, s32 d);
void FieldEvent_Wait(s32 frames);
void Actor_SetSingleChildFlags(struct Actor *object, u32 value);
void Func_02003ac4(s32 a, s32 b, s32 c);
void Func_02003acc(void);
void Func_02003b74(s32 a, s32 b);
void Func_02003b6c(void);
void Func_02003af4(void);

void Func_02001c00(void) {
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct MagmaRockEffectOptions options;
  s32 position[3];
  struct MagmaRockEffectOptions *opt;
  s32 *offset;
  s32 i;
  u32 phase;

  Func_02003aec();
  Func_02003bcc(0);
  GameFlag_SetBit(0x260);
  Func_02003c54(133);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 6);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 10);
  Actor_SetActionById(gMainRoutineWork.currentOwner, 32);
  opt = &options;
  opt->kind = 1;
  opt->palette = 14;
  opt->scaleX = SPLIT_CONSTANT(0xcccc);
  opt->scaleY = SPLIT_CONSTANT(0xcccc);
  i = 0;
first:
  {
    s32 r = Random16Far();

    offset = position;
    offset[0] = ((r & 15) << 16) - 0x80000;
  }
  offset[1] = (Random16Far() & 15) << 16;
  Func_0200015c(leader->x + offset[0], leader->y + offset[1], leader->z, 0,
                0x13333, 0, 0xb0000, opt);
  wait(5);
  i++;
  if (i != 8)
    goto first;
  Call4((void (*)())Camera_MoveWithinMapBounds, -1, -1, -1, 0);
  gMainRoutineWork.renderMode = 0;
  leader->motionFlags = 3;
  leader->gravity = 0xcccc;
  leader->bounce = 0;
  FieldEvent_Wait(1);
  leader->angle = 0xc000;
  i = 60;
second:
  phase = gFrameCount & 1;
  if (phase == 0) {
    offset[0] = ((Random16Far() & 15) << 16) - 0x80000;
    offset[1] = (Random16Far() & 15) << 16;
    Func_0200015c(leader->x + offset[0], leader->y + offset[1], leader->z, 0,
                  0x13333, phase, 0xb0000, opt);
  }
  wait(1);
  if (leader->velocityY != 0) {
    i--;
    if (i != 0)
      goto second;
  }
  Func_02003c54(188);
  Actor_SetSingleChildFlags(leader, 1);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 22);
  Call3((void (*)())Func_02003ac4, 0x50000, 0x80000, 0x10000);
  Call3((void (*)())Func_02003ac4, -1, -1, SPLIT_CONSTANT(0xe666));
  Func_02003acc();
  Func_02003b74(gMainRoutineWork.currentOwner, 1);
  Func_02003b6c();
  Actor_SetModeById(gMainRoutineWork.currentOwner, 4);
  Actor_SetPartAttribute(Actor_GetById(gMainRoutineWork.currentOwner), 0);
  Actor_WaitForAnimationChange(gMainRoutineWork.currentOwner);
  GameFlag_ClearBit(0x260);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  Func_02003af4();
}
