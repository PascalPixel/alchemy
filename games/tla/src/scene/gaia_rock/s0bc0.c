#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "iwram_call.h"
#include "event_inline.h"

extern const u8 gData_020016cc[];
extern const u8 gData_02001688[];

void Func_02000e74(void);
void Func_02000b2c(s32 id, s32 x, s32 y, s32 z);
void Func_02000e2c(struct Actor *object, const u8 *script);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Func_02000e24(struct Actor *object, s32 mode);
void Func_02000ecc(void);
void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Func_02000e7c(void);

void Func_02000bc0(s32 id, s32 flag) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 gravity = leader->gravity;
  struct Actor *pillar;
  s32 i;

  Func_02000e74();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  pillar = Actor_Get(id);
  Func_02000b2c(id, leader->x, leader->y, leader->z);
  Func_02000e2c(pillar, gData_020016cc);
  i = 15;
dust:
  {
    struct Actor *dust =
        FieldActor_Create(255, leader->x, leader->y + 0x100000, leader->z);

    if (dust != 0) {
      s32 angle;
      s32 speed;
      u32 size;
      s32 scale;

      Func_02000e2c(dust, gData_02001688);
      angle = Random16Far();
      speed = Random16Far();
      size = Random16Far();
      scale = (size >> 1) + 0x2000;
      dust->velocityY = Iwram_MulQ16(speed * 8, Math_Cosine(angle));
      dust->velocityX = Iwram_MulQ16(speed * 8, Math_Sin(angle));
      dust->acceleration = 0;
      dust->scaleX = scale;
      dust->scaleY = scale;
      dust->gravity = 0x547a;
      dust->bounce = 0x8000;
    }
  }
  if (--i >= 0)
    goto dust;
  Event_PlaySound(0x24e);
  Event_PlaySound(217);
  MapGrid_SetShake(0x20000, 0x20000, 0xe666);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Func_02000e24(leader, 40);
  FieldEvent_Wait(40);
  Event_PlaySound(204);
  leader->motionFlags = 3;
  id = 0;
  leader->gravity = 0x547a;
  goto fall;
next:
  id++;
fall:
  if (id <= 179) {
    System_Wait(1);
    if (leader->y > leader->terrainHeight)
      goto next;
  }
  Event_PlaySound(188);
  leader->gravity = gravity;
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0);
  Func_02000e24(leader, 38);
  MapGrid_SetShake(0x10000, 0x40000, 0x10000);
  System_Wait(10);
  MapGrid_SetShake(-1, -1, 0xe666);
  System_Wait(20);
  FieldCamera_MoveTo(leader->x, leader->y, leader->z, 1);
  Func_02000ecc();
  FieldEvent_Wait(20);
  Actor_SetSingleChildFlags(leader, 1);
  Func_02000e24(leader, 1);
  leader->velocityY = 0x40000;
  System_Wait(20);
  gMainRoutineWork.renderMode = 0;
  Func_02000e7c();
  Event_SetFlag(flag);
}
