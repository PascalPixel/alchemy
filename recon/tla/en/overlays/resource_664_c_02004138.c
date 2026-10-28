// Draft, differ 333 of 402: a burst of power between two objects in Air's Rock
// (a = idA, b = idB): the camera centres on a (cues 78, 141) while the
// screen zooms and fades; a starts its action script (gData_020060e0) and,
// blended over the background, sixteen sparks (object type 0x2a0) fly out
// of it while the blend flickers; then sixteen sparks swirl round b at
// widening distances (gData_020060b0), b starting its own script at the
// fourth, and fifteen more rise in a column over b (gData_020060c0); the
// screen fades back, both objects take their closing scripts and b runs
// Func_020040b0 for 120 frames.
// Left: a rough reading: the native keeps a in r7, idA in r9, idB in r11 and b in r10 with a 12-byte stack frame (the draft spills idA to the stack), keeps the blend register's address in r6 and orders the sparks' field stores otherwise.
#include "types.h"
#include "object_runtime.h"
#include "call.h"

#define REG_BLDCNT (*(volatile u16 *)0x04000050)
#define REG_BLDALPHA (*(volatile u16 *)0x04000052)

extern const u8 gData_020060e0[], gData_02006094[], gData_020060b0[],
    gData_02006104[], gData_020060c0[], gData_02006128[], gData_02006158[];
struct ObjectRuntime *Object_GetById(u32 id);
void Func_02005f94(s32 cue);
void FieldEvent_WaitFrames(s32 frames);
void Func_02005ed4(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02005dcc(s32 a, s32 b, s32 c);
void Func_02005f0c(s32 a, s32 b);
s32 Func_02005f04(s32 a, s32 b);
s32 Func_02005f14(s32 frames);
void ObjectMotion_EnableActionAndSetCallback(s32 actor, const void *callback);
void Func_02005e94(s32 actor, s32 a);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Func_02005d5c(struct ObjectRuntime *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        u32 value);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_02005d6c(struct ObjectRuntime *object, const u8 *script);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *position);
void WaitFrames(s32 frames);
void Func_020040b0(void);
void Func_02005f3c(void);

void Func_02004138(s32 idA, s32 idB) {
  struct ObjectRuntime *a = Object_GetById(idA);
  struct ObjectRuntime *b = Object_GetById(idB);
  struct ObjectRuntime *spark;
  s32 i;

  Func_02005f94(78);
  FieldEvent_WaitFrames(30);
  Call2(Func_02005ed4, 0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&a->x)[1] << 16, ((s16 *)&a->y)[1] << 16,
                             ((s16 *)&a->z)[1] << 16, 1);
  Func_02005f94(141);
  Call3(Func_02005dcc, 0x20000, 0x20000, 0x10000);
  Call2(Func_02005f0c, 0x10000, 0);
  Value2(Func_02005f04, 0x10003, 1);
  Value1(Func_02005f14, 60);
  a->motionFlags = 0;
  a->acceleration = 0x3333;
  a->speedLimit = 0x3333;
  ObjectMotion_EnableActionAndSetCallback(idA, gData_020060e0);
  Func_02005f94(194);
  FieldEvent_WaitFrames(45);
  Func_02005e94(idA, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  for (i = 0; i != 16; i++) {
    spark = Engine_ObjectCreate(0x2a0, a->x, a->y, a->z);
    Func_02005f94(246);
    spark->x += ((Random16Far() & 31) - 16) << 16;
    spark->y += ((Random16Far() & 15) - 8) << 16;
    spark->speedLimit = 0x80000;
    spark->acceleration = 0x3333;
    spark->motionFlags = 0;
    Object_SetPartAttribute(spark, 2);
    Object_SetSpritePriority(spark, 1);
    Func_02005d5c(spark, 7);
    ObjectDispatch_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02005d6c(spark, gData_02006094);
    REG_BLDALPHA = 0x1004;
    WaitFrames(2);
    REG_BLDALPHA = 0x100a;
    WaitFrames(2);
    REG_BLDALPHA = 0x1010;
    WaitFrames(2);
    REG_BLDALPHA = 0x100a;
    WaitFrames(2);
  }
  REG_BLDALPHA = 0x1008;
  WaitFrames(30);
  for (i = 0; i != 16; i++) {
    spark = Engine_ObjectCreate(0x2a0, b->x, b->y, b->z);
    Func_02005f94(195);
    spark->action = (Random16Far() & 0x7fff) + 0x8000;
    spark->unk66 = ((i & 1) * 2 - 1) << ((Random16Far() & 3) + 9);
    spark->motionFlags = 0;
    spark->unk4c = 0x7a0000;
    spark->linkedObject = b;
    ((u8 *)spark)[98] = (16 - i) * 8 + 15;
    Object_SetPartAttribute(spark, 2);
    Object_SetSpritePriority(spark, 1);
    Func_02005d5c(spark, 7);
    ObjectDispatch_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Vector_AddPolarOffsetFar(spark->unk4c, spark->action, &spark->x);
    Func_02005d6c(spark, gData_020060b0);
    if (i == 3) {
      Func_02005e94(idB, 0x100);
      b->motionFlags = 0;
      b->acceleration = 0x3333;
      b->speedLimit = 0x3333;
      ObjectMotion_EnableActionAndSetCallback(idB, gData_02006104);
    }
    WaitFrames(8);
  }
  Func_02005f94(220);
  WaitFrames(16);
  for (i = 2; i != 32; i += 2) {
    spark = Engine_ObjectCreate(0x2a0, b->x, (i << 16) + b->y - 0x80000, b->z);
    spark->action = Random16Far();
    spark->unk66 = 0x800;
    ((u8 *)spark)[98] = i;
    ((u8 *)spark)[99] = 1;
    spark->bounce = spark->x;
    spark->unk4c = 0;
    spark->gravity = spark->z;
    spark->linkedObject = b;
    spark->motionFlags = 0;
    spark->priorityFlags = 1;
    Object_SetPartAttribute(spark, 2);
    Func_02005d5c(spark, 7);
    ObjectDispatch_SetSingleChildFlags(spark, 0);
    Animation_SetStateFlags(spark, 1);
    Func_02005d6c(spark, gData_020060c0);
  }
  Func_02005f94(220);
  WaitFrames(50);
  Value2(Func_02005f04, 0x7fff, 0);
  Value1(Func_02005f14, 8);
  WaitFrames(16);
  Call3(Func_02005dcc, 0, 0, 0);
  Func_02005e94(idA, 0);
  Func_02005e94(idB, 0);
  Func_02005f94(574);
  Value2(Func_02005f04, 0x10000, 0);
  Value1(Func_02005f14, 80);
  ObjectMotion_EnableActionAndSetCallback(idA, gData_02006128);
  ObjectMotion_EnableActionAndSetCallback(idB, gData_02006158);
  b->update = Func_020040b0;
  WaitFrames(120);
  b->update = 0;
  Func_02005f94(390);
  Func_02005f3c();
  WaitFrames(1);
}
