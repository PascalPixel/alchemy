#include "types.h"
#include "call.h"
struct SceneActor {
  u8 pad0[16];
  s32 unk_10;
  u8 pad14[35 - 20];
  u8 unk_23;
  u8 pad24[64 - 36];
  s32 unk_40;
  u8 pad44[85 - 68];
  u8 unk_55;
};
struct SceneWork {
  u8 pad0[12];
  s32 unk_0c;
};
struct FieldWork {
  u8 pad0[0x178];
  struct SceneWork unk_178;
};
struct Iwram {
  u8 pad0[32];
  struct FieldWork *field;
};
void Camera_FollowActor(s32 id, s32 flag);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005884(s32, s32, s32);
void Func_020059f4(s32);
void Actor_SetSpritePriority(u32, s32);
struct SceneActor *Actor_GetById(s32);
void wait(s32);
extern s32 Data_02006a1c;
extern s32 gData_020059fc[];
extern s32 gData_02005a0c[];
extern s32 gData_02005a14[];
extern s32 gData_02005a1c[];
extern s32 gData_020069dc[];

void Func_02003a6c(s32 mode) {
  s32 speed;
  u32 count;
  s32 *list;
  s32 *p;
  s32 *q;
  u32 i;
  u32 j;
  struct SceneWork *work;

  work = &((struct Iwram *)0x3000000)->field->unk_178;
  speed = 0x1999;
  count = 0;
  Data_02006a1c = work->unk_0c;
  switch (mode) {
  case 0:
  case 3:
    list = gData_020059fc;
    break;
  case 1:
    list = gData_02005a0c;
    break;
  case 2:
    list = gData_02005a14;
    break;
  default:
    list = gData_02005a1c;
    break;
  }

  for (i = 0; i <= 4 && list[i] != 64; i++) {
    Actor_GetById(list[i])->unk_55 = 0;
    Actor_SetSpritePriority(list[i], 1);
    count++;
  }

  for (i = 0; i <= 5; i++) {
    Actor_GetById(gData_020069dc[i])->unk_23 |= 32;
    Actor_SetSpritePriority(gData_020069dc[i], 0);
  }

  Func_020059f4(223);

  for (i = 0; i <= 227; i++) {
    work->unk_0c -= speed;
    for (j = 0; j < count; j++) {
      struct SceneActor *actor;

      Actor_GetById(list[j])->unk_10 += speed;
      actor = Actor_GetById(list[j]);
      actor->unk_40 = Actor_GetById(list[j])->unk_10;
    }
    if ((i & 3) == 3)
      speed += 0x1999;
    if (speed > 0x17fff)
      speed = 0x18000;
    wait(1);
    if (mode == 0) {
      if (i == 40)
        Camera_MoveWithinMapBounds(128 << 18, 208 << 15, 152 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 1) {
      if (i == 40)
        Camera_MoveWithinMapBounds(128 << 18, 208 << 15, 176 << 17, 1);
      if (i == 120)
        break;
    } else if (mode == 3) {
      if (i == 40)
        Camera_FollowActor(0, 1);
      if (i == 120)
        break;
    }
  }

  for (i = 0; i <= 5; i++) {
    Actor_GetById(gData_020069dc[i])->unk_23 &= 223;
    Actor_SetSpritePriority(gData_020069dc[i], 1);
  }

  for (i = 0; i < count; i++) {
    Func_02005884(list[i], 0, 0);
    Actor_GetById(list[i])->unk_55 = 0;
  }
}
