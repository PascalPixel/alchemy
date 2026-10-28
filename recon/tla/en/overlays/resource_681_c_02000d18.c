// Draft, differ 94 of 188: once any of twelve cells is filled (flag 0x9c4 still
// clear), person 15 walks off, the doorway cells open, four boulders are
// placed and the flag is set.
#include "types.h"
#include "actor.h"
#include "adventure.h"

struct CellPoint {
  s16 x;
  s16 z;
};

extern const struct CellPoint gData_020051d8[12];
extern const u8 gData_020051b8[];
extern const u8 gData_02005118[];
extern const u8 gData_02005168[];

struct Actor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_WaitFrames(s32 frames);
void Actor_SetMovementTypeById(u32 objectId, const u8 *script);
void *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02004ff4(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 Func_02004dec(s32 layer, s32 x, s32 z);
void Func_02004e5c(void);
void Func_02004fcc(s32 mode);
void Func_02004f2c(s32 id, s32 animation, s32 frames);
void Func_02004e04(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004dfc(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005014(s32 cue);
void Func_02004ebc(s32 id, s32 x, s32 z);
void Func_02004fdc(void);
s32 Func_02004f6c(s32 id);

struct BoulderSprite {
  u8 unk00[9];
  u8 unused09 : 2;
  u8 layer : 2;
};

void Func_02000d18(void) {
  struct Actor *guard;
  struct Actor *leader;
  s16 count;
  s16 i;

  Func_02004ff4(104, 20, 1, 8, 117, 20);
  if (GameFlag_Test(0x9c4))
    return;
  count = 0;
  for (i = 0; i <= 11; i++) {
    if (Func_02004dec(2, gData_020051d8[i].x << 20,
                      gData_020051d8[i].z << 20) == 0)
      count++;
  }
  if (count <= 0)
    return;
  guard = Actor_GetById(15);
  leader = Actor_GetById(gAdventure.currentOwner);
  Func_02004e5c();
  Func_02004fcc(0);
  if (leader->z >> 19 > 53 && leader->x >> 19 > 83 && leader->x >> 19 <= 91) {
    leader->speedLimit = 0x20000;
    Func_02004f2c(gAdventure.currentOwner, 0x100, 0);
    Actor_SetMovementTypeById(gAdventure.currentOwner,
                                            gData_020051b8);
  }
  Actor_SetMovementTypeById(15, gData_02005118);
  FieldEvent_WaitFrames(60);
  Actor_SetMovementTypeById(15, gData_02005168);
  FieldEvent_WaitFrames(30);
  Func_02004e04(117, 20, 7, 8, 104, 20);
  Func_02004e04(111, 12, 1, 6, 111, 20);
  Func_02004dfc(49, 84, 8, 8, 40, 84);
  FieldEvent_WaitFrames(30);
  OverlayObject_PrepareObject(175 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1ea0000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1d40000, 286);
  OverlayObject_PrepareObject(181 << 18, 0, 0x1ea0000, 286);
  Func_02005014(204);
  guard->motionFlags = 3;
  ((struct BoulderSprite *)guard->animation)->layer = 3;
  FieldEvent_WaitFrames(10);
  Func_02004ebc(15, 0, 0);
  Func_02004fdc();
  GameFlag_SetBit(0x9c4);
  Func_02004f6c(9);
}
