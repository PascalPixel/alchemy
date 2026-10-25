// Draft, differ 286 of 402: the Psynergy Stone's power passing between two
// objects: the first rises glowing while sixteen sparks burst from it with
// the screen flashing, sixteen sparks then spiral down onto the second as it
// rises, sixteen orbiting sparks circle it, the screen whitens and both settle
// back while the second keeps sparkling for two seconds. Overlay 681's
// Func_020015d0 is a near copy.
// Measured against its copies: overlay 681's Func_020015d0 (differ 286 of 402).
#include "types.h"
#include "lib/call.h"
#include "object_runtime.h"
#include "system/io_reg.h"

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern const u8 gData_02004034[];
extern const u8 gData_02003fe8[];
extern const u8 gData_02004004[];
extern const u8 gData_02004058[];
extern const u8 gData_02004014[];
extern const u8 gData_0200407c[];
extern const u8 gData_020040ac[];

struct ObjectRuntime *Object_GetById(s32 id);
void Func_02003ef4(s32 cue);
void FieldEvent_WaitFrames(s32 frames);
void Func_02003e44(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 follow);
void Func_02003d44(s32 x, s32 y, s32 z);
void Func_02003e7c(s32 a, s32 b);
void Func_02003e74(s32 a, s32 b);
void Func_02003e84(s32 frames);
void ObjectMotion_EnableActionAndSetCallback(u32 objectId, const u8 *script);
void Func_02003e04(s32 id, s32 animation);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 attribute);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Func_02003cc4(struct ObjectRuntime *object, s32 mode);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Func_02003ccc(struct ObjectRuntime *object, const u8 *script);
void WaitFrames(s32 frames);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *position);
void Func_02003ebc(void);
void Func_02001598(struct ObjectRuntime *object);

void Func_02001620(s32 sourceId, s32 targetId) {
  struct ObjectRuntime *source = Object_GetById(sourceId);
  struct ObjectRuntime *target = Object_GetById(targetId);
  struct ObjectRuntime *spark;
  s32 i;

  Func_02003ef4(78);
  FieldEvent_WaitFrames(30);
  Call2((void (*)())Func_02003e44, 0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&source->x)[1] << 16,
                             ((s16 *)&source->y)[1] << 16,
                             ((s16 *)&source->z)[1] << 16, 1);
  Func_02003ef4(141);
  Call3((void (*)())Func_02003d44, 0x20000, 0x20000, 0x10000);
  Call2((void (*)())Func_02003e7c, 0x10000, 0);
  Call2((void (*)())Func_02003e74, 0x10003, 1);
  Func_02003e84(60);
  source->motionFlags = 0;
  source->acceleration = 0x3333;
  source->speedLimit = 0x3333;
  ObjectMotion_EnableActionAndSetCallback(sourceId, gData_02004034);
  Func_02003ef4(194);
  FieldEvent_WaitFrames(45);
  Func_02003e04(sourceId, 0x100);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  i = 0;
burst:
  spark = Engine_ObjectCreate(0x2a0, source->x, source->y, source->z);
  Func_02003ef4(246);
  spark->x += ((Random16Far() & 31) - 16) << 16;
  spark->speedLimit = 0x80000;
  spark->y += ((Random16Far() & 15) - 8) << 16;
  spark->acceleration = 0x3333;
  spark->motionFlags = 0;
  Object_SetPartAttribute(spark, 2);
  Object_SetSpritePriority(spark, 1);
  Func_02003cc4(spark, 7);
  ObjectDispatch_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  Func_02003ccc(spark, gData_02003fe8);
  REG_BLDALPHA = 0x1004;
  WaitFrames(2);
  REG_BLDALPHA = 0x100a;
  WaitFrames(2);
  REG_BLDALPHA = 0x1010;
  WaitFrames(2);
  REG_BLDALPHA = 0x100a;
  WaitFrames(2);
  if (++i != 16)
    goto burst;
  REG_BLDALPHA = 0x1008;
  WaitFrames(30);
  i = 0;
spiral:
  spark = Engine_ObjectCreate(0x2a0, target->x, target->y, target->z);
  Func_02003ef4(195);
  spark->action = (Random16Far() & 0x7fff) + 0x8000;
  spark->unk66 = ((i & 1) * 2 - 1) << ((Random16Far() & 3) + 9);
  spark->motionFlags = 0;
  spark->unk4c = 0x7a0000;
  spark->linkedObject = target;
  FIELD_AT(spark, u8, 98) = ((16 - i) << 3) + 15;
  Object_SetPartAttribute(spark, 2);
  Object_SetSpritePriority(spark, 1);
  Func_02003cc4(spark, 7);
  ObjectDispatch_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  Vector_AddPolarOffsetFar(spark->unk4c, spark->action, &spark->x);
  Func_02003ccc(spark, gData_02004004);
  if (i == 3) {
    Func_02003e04(targetId, 0x100);
    target->motionFlags = 0;
    target->acceleration = 0x3333;
    target->speedLimit = 0x3333;
    ObjectMotion_EnableActionAndSetCallback(targetId, gData_02004058);
  }
  WaitFrames(8);
  if (++i != 16)
    goto spiral;
  Func_02003ef4(220);
  WaitFrames(16);
  i = 2;
orbit:
  spark = Engine_ObjectCreate(0x2a0, target->x, (i << 16) + target->y - 0x80000,
                              target->z);
  spark->action = Random16Far();
  spark->unk66 = 0x800;
  FIELD_AT(spark, u8, 98) = i;
  FIELD_AT(spark, u8, 99) = 1;
  spark->bounce = spark->x;
  spark->unk4c = 0;
  spark->gravity = spark->z;
  spark->linkedObject = target;
  spark->motionFlags = 0;
  spark->priorityFlags = 1;
  Object_SetPartAttribute(spark, 2);
  Func_02003cc4(spark, 7);
  ObjectDispatch_SetSingleChildFlags(spark, 0);
  Animation_SetStateFlags(spark, 1);
  Func_02003ccc(spark, gData_02004014);
  i += 2;
  if (i != 32)
    goto orbit;
  Func_02003ef4(220);
  WaitFrames(50);
  Call2((void (*)())Func_02003e74, 0x7fff, 0);
  Func_02003e84(8);
  WaitFrames(16);
  Call3((void (*)())Func_02003d44, 0, 0, 0);
  Func_02003e04(sourceId, 0);
  Func_02003e04(targetId, 0);
  Func_02003ef4(0x23e);
  Call2((void (*)())Func_02003e74, 0x10000, 0);
  Func_02003e84(80);
  ObjectMotion_EnableActionAndSetCallback(sourceId, gData_0200407c);
  ObjectMotion_EnableActionAndSetCallback(targetId, gData_020040ac);
  target->update = (void (*)(void))Func_02001598;
  WaitFrames(120);
  target->update = 0;
  Func_02003ef4(0x186);
  Func_02003ebc();
  WaitFrames(1);
}
