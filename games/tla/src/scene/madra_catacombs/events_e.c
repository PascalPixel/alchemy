#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 returnEntrance;
};

struct EventWork1ac {
  u8 unk000[0x1ac];
  s32 flag;
};

struct AnimationRecord18 {
  u8 unk00[0x12];
  u16 frame;
};

extern u8 MsgFieldEyesOfTruth;
extern const u8 gData_020013c0[];
extern const u8 gData_020013c4[];
extern const u8 gData_020013ca[];
extern const u8 gData_0200121c[];

void Actor_SetSingleChildFlags(struct Actor *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_0200110c(void);
void Func_02001114(void);
void Func_0200113c(s32 id, s32 value);
void Func_0200115c(s32 id);
s32 Func_02001184(s32 owner);
void Func_020011b4(void);
void Func_020011bc(s32 a, s32 b, s32 c, s32 d);
void Func_020011c4(const void *data);
void Func_020011dc(const void *data);
s32 Func_020011fc(void);
void Func_02001204(void);
void Func_02000038(const void *data);
void Func_0200014c(const void *data);
void Func_02000270(struct Actor *object);
void Func_020008d0(struct Actor *object);
void Func_02000958(void);
void Func_02000a28(void);
void Func_02000c94(void);
void Func_02000cf4(void);

void Func_02000dcc(void) {
  s32 a;
  s32 b;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *object;

  GameFlag_ClearBit(0x203);
  Actor_SetSpritePriority(64, 3);
  Func_020011b4();
  b = 0x102;
  a = 1;
  /* FAKEMATCH(barrier): a and b held at an empty asm, so the first argument is
   * set before the second is finished; as no asm, differ 2 (of M not
   * recorded). */
  __asm__("" : "+r"(a), "+r"(b));
  Func_020011bc(a, b + 255, 9, 10);
  Func_0200115c(11);
  if (GameFlag_Test(0x8c8) != 0) {
    MapGrid_CopyCellAttributes(41, 13, 2, 1, 39, 13);
    object = Actor_GetById(11);
    x = 162;
    z = 216;
    x <<= 18;
    y = 0;
    /* FAKEMATCH(barrier): x, y and z held at an empty asm, the second of the
     * pair above, so y is cleared before z is shifted; as no asm, not
     * measured. */
    __asm__("" : "+r"(x), "+r"(y), "+r"(z));
    Actor_SetPositionAndResetMotion(object, x, y, z << 16);
  }
  if (GameFlag_Test(0x8ca) != 0)
    Actor_SetPosition(12, PIXELS(712), PIXELS(424));
  Func_020011dc(gData_020013c0);
  if (GameFlag_Test(0x9ef) != 0)
    Actor_SetPosition(8, PIXELS(568), PIXELS(280));
  Actor_GetById(8)->priorityFlags &= 0xfe;
  Func_02000038(gData_020013ca);
  Func_020011c4(gData_020013c4);
  if (GameFlag_Test(0x8cb) != 0) {
    Map_CopyCellPictureIndicesRect(75, 66, 97, 39, 2, 2);
    MapGrid_CopyCellAttributes(33, 37, 2, 2, 33, 39);
    wait(1);
  }
  if (GameFlag_Test(0x8ab) != 0) {
    MapGrid_CopyCellsTo(82, 74, 116, 16, 5, 3);
    MapGrid_CopyCellAttributes(52, 11, 5, 1, 52, 18);
    if (gMainRoutineWork.entrance == 3)
      Func_02000c94();
    else
      wait(10);
  }
}

void Func_02000f2c(void) {
  Func_020011b4();
  Func_020011bc(0, 0x200, 8, 9);
  if (GameFlag_Test(0x8ab) != 0) {
    Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    MapGrid_CopyCellsTo(68, 70, 113, 11, 5, 3);
    MapGrid_CopyCellsTo(50, 74, 50, 75, 2, 1);
    MapGrid_CopyCellAttributes(49, 9, 5, 1, 49, 13);
    if (gMainRoutineWork.entrance == 8)
      Func_02000cf4();
    else
      wait(1);
  }
}

void Func_02000fbc(void) {
  if (GameFlag_Test(0x8cc) == 0)
    Actor_SetHorizontalPositionWithTerrain(65, 0, 0);
}

void Func_02000fd8(void) {
  struct Actor *object;

  if (GameFlag_Test(0x8cc) != 0) {
    Actor_SetHorizontalPositionWithTerrain(64, 0, 0);
  } else {
    object = Actor_GetById(64);
    object->collisionFlags = 8;
    object->motionFlags = 2;
    object->y = PIXELS(32);
    object->terrainHeight = PIXELS(32);
    object->update = Func_02000a28;
    Actor_GetById(8)->scaleY = 0xcccc;
  }
}
