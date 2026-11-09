#include "types.h"
#include "system/iwram_call.h"
#include "event_inline.h"

struct Vector3 {
  s32 x;
  s32 y;
  s32 z;
};

struct Work {
  u8 pad0[8];
  struct Vector3 position;
  s32 unk14;
  u8 pad18[0x18];
  s32 unk30;
  s32 unk34;
  u8 pad38[0x0c];
  s32 unk44;
  s32 unk48;
  s32 unk4c;
  u8 pad50[5];
  u8 unk55;
  u8 pad56[0x0c];
  u8 unk62;
  u8 pad63;
  s16 unk64;
  u16 unk66;
};

void Animation_SetStateFlags();
s32 ArcTan2();
void Func_020006f8();
void Func_020032e0();
void Func_020032e8();
void Actor_SetSingleChildFlags();
void Scene_SetActorSpritePriority();
u32 Random16Far();
extern u8 gData_02003684[];

void Func_0200088c(struct Vector3 *from, struct Vector3 *to, s16 part) {
  struct Work *actor;
  s32 x, z, distance;

  actor = ((struct Work *)Import_ActorCreate(0x2a0, from->x, from->y, from->z));
  x = (from->x - to->x) >> 16;
  z = (from->z - to->z) >> 16;
  actor->unk64 = ArcTan2(from->z - to->z, from->x - to->x);
  actor->unk66 = (((Random16Far() & 2) - 1) * (Random16Far() & 15)) << 8;
  actor->unk62 = part;
  actor->unk55 = 0;
  actor->unk44 = to->x;
  actor->unk48 = to->z;
  distance = Iwram_Sqrt(x * x + z * z) << 16;
  actor->unk4c = distance;
  actor->unk30 = distance / 16;
  actor->unk34 = from->y;
  actor->unk14 = (from->y - to->y) / 16;
  Animation_SetStateFlags(actor, 1);
  Actor_SetPartAttribute(actor, (u16)part);
  Scene_SetActorSpritePriority(actor, 2);
  Func_020032e0(actor, 7);
  Actor_SetSingleChildFlags(actor, 0);
  Vector_AddPolarOffsetFar(actor->unk4c, actor->unk64, &actor->position);
  Func_020032e8(actor, gData_02003684);
  Func_020006f8(actor);
}
