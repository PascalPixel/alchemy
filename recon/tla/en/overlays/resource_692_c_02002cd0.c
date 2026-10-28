// Draft, differ 343 of 402: a Magma Rock scene between objects a and b: sound 78,
// the camera to a, a shake and flash (sound 141), a rises on its action script
// (sound 194); sixteen blended objects of type 672 burst from a with sound
// 246 while the blend flickers; sixteen more spiral in around b (sound 195),
// b starting its own script at the fourth; then fifteen rise in a column above
// b (sound 220); the blend fades, the shaking stops, both take their closing
// scripts with sound 574, b takes the rising-effect update (Func_02002c48) for
// 120 frames and the scene ends with sound 390.
#include "../../../../games/tla/src/scene/magma_rock_inside/local.h"
#include "io_reg.h"

extern const u8 gData_02005590[];
extern const s32 gData_02005544[];
extern const s32 gData_02005560[];
extern const u8 gData_020055b4[];
extern const s32 gData_02005570[];
extern const u8 gData_020055d8[];
extern const u8 gData_02005608[];

void Func_02004f30(s32 sound);
void Func_02004e10(s32 a, s32 b);
void Camera_MoveWithinMapBounds(s32 x, s32 y, s32 z, s32 mode);
void Func_02004d10(s32 a, s32 b, s32 c);
void Func_02004e48(s32 a, s32 b);
void Func_02004e40(s32 a, s32 b);
void Func_02004e50(s32 frames);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Func_02004dc0(s32 id, s32 value);
u32 Random16Far(void);
void Vector_AddPolarOffsetFar(s32 distance, s32 angle, s32 *vector);
void Func_02004ea0(void);
void Func_02002c48(struct Actor *object);

void Func_02002cd0(s32 a, s32 b) {
  struct Actor *first = Actor_GetById(a);
  struct Actor *second = Actor_GetById(b);
  struct Actor *object;
  s32 i;

  Func_02004f30(78);
  FieldEvent_Wait(30);
  Func_02004e10(0x8000, 0x1000);
  Camera_MoveWithinMapBounds(((s16 *)&first->x)[1] << 16,
                             ((s16 *)&first->y)[1] << 16,
                             ((s16 *)&first->z)[1] << 16, 1);
  Func_02004f30(141);
  Func_02004d10(0x20000, 0x20000, 0x10000);
  Func_02004e48(0x10000, 0);
  Func_02004e40(0x10003, 1);
  Func_02004e50(60);
  first->motionFlags = 0;
  first->acceleration = 0x3333;
  first->speedLimit = 0x3333;
  Actor_SetMovementTypeById(a, gData_02005590);
  Func_02004f30(194);
  FieldEvent_Wait(45);
  Func_02004dc0(a, 256);
  REG_BLDALPHA = 0x1008;
  REG_BLDCNT = 0x3f10;
  for (i = 0; i != 16; i++) {
    object = Import_ActorCreate(672, first->x, first->y, first->z);
    Func_02004f30(246);
    object->x += ((Random16Far() & 31) - 16) << 16;
    object->y += ((Random16Far() & 15) - 8) << 16;
    object->speedLimit = 0x80000;
    object->acceleration = 0x3333;
    object->motionFlags = 0;
    Actor_SetPartAttribute(object, 10);
    Scene_SetActorSpritePriority(object, 1);
    Func_02004c98(object, 7);
    Actor_SetSingleChildFlags(object, 0);
    Animation_SetStateFlags(object, 1);
    Func_02004ca0(object, gData_02005544);
    REG_BLDALPHA = 0x1004;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
    REG_BLDALPHA = 0x1010;
    wait(2);
    REG_BLDALPHA = 0x100a;
    wait(2);
  }
  REG_BLDALPHA = 0x1008;
  wait(30);
  for (i = 0; i != 16; i++) {
    object = Import_ActorCreate(672, second->x, second->y, second->z);
    Func_02004f30(195);
    object->action = (Random16Far() & 0x7fff) + 0x8000;
    object->unk66 = (((i & 1) << 1) - 1) << ((Random16Far() & 3) + 9);
    object->motionFlags = 0;
    object->unk4c = 0x7a0000;
    object->watchedObject = second;
    ((u8 *)object)[0x62] = ((16 - i) << 3) + 15;
    Actor_SetPartAttribute(object, 10);
    Scene_SetActorSpritePriority(object, 1);
    Func_02004c98(object, 7);
    Actor_SetSingleChildFlags(object, 0);
    Animation_SetStateFlags(object, 1);
    Vector_AddPolarOffsetFar(object->unk4c, object->action, &object->x);
    Func_02004ca0(object, gData_02005560);
    if (i == 3) {
      Func_02004dc0(b, 256);
      second->motionFlags = 0;
      second->acceleration = 0x3333;
      second->speedLimit = 0x3333;
      Actor_SetMovementTypeById(b, gData_020055b4);
    }
    wait(8);
  }
  Func_02004f30(220);
  wait(16);
  for (i = 2; i != 32; i += 2) {
    object = Import_ActorCreate(672, second->x,
                                 second->y + (i << 16) - 0x80000, second->z);
    object->action = Random16Far();
    object->unk66 = 0x800;
    ((u8 *)object)[0x62] = i;
    ((u8 *)object)[0x63] = 1;
    object->bounce = object->x;
    object->unk4c = 0;
    object->gravity = object->z;
    object->watchedObject = second;
    object->motionFlags = 0;
    object->priorityFlags = 1;
    Actor_SetPartAttribute(object, 10);
    Func_02004c98(object, 7);
    Actor_SetSingleChildFlags(object, 0);
    Animation_SetStateFlags(object, 1);
    Func_02004ca0(object, gData_02005570);
  }
  Func_02004f30(220);
  wait(50);
  Func_02004e40(0x7fff, 0);
  Func_02004e50(8);
  wait(16);
  Func_02004d10(0, 0, 0);
  Func_02004dc0(a, 0);
  Func_02004dc0(b, 0);
  Func_02004f30(574);
  Func_02004e40(0x10000, 0);
  Func_02004e50(80);
  Actor_SetMovementTypeById(a, gData_020055d8);
  Actor_SetMovementTypeById(b, gData_02005608);
  second->update = (void (*)(void))Func_02002c48;
  wait(120);
  second->update = 0;
  Func_02004f30(390);
  Func_02004ea0();
  wait(1);
}
