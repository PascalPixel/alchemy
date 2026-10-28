// Draft, differ 358 of 493 (was 407): steps onto a listed cell in rooms 1,
// 2, 3 and 5 (SceneId_Ov655Room1 to Room5): either falls through to exit
// work->exit or opens the cell and sets its flag. The first test of flag
// 0x1c1 goes through Value1, as the native builds 0x1c1 afresh. Left:
// register allocation (the native keeps cellX in r8, the leader in r9, the
// record in r6 and the flag in r11, and spills cellZ) and the tables the
// native loads into one register for a shared call tail.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"
#include "map.h"
#include "field_sprite.h"
#include "iwram_call.h"
#include "call.h"

extern u8 gMapShapeGrid[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void wait(s32 frames);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);
struct Actor *Actor_GetById(s32 id);
void Actor_SetPartAttribute(struct Actor *object, s32 value);
void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void ObjectVisual_CopyAttributes(s32 id, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200056c(s32 id);
void Func_020006ec(struct Actor *object, struct Actor *other);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02000c4c(s32 level, s32 layer);
void Func_02003ba0(struct Actor *object, s32 mode);
void Func_02003ba8(struct Actor *object, const void *script);
struct Actor *Func_02003bb0(s32 type, s32 x, s32 y, s32 z);
void Func_02003c00(s32 a, s32 b, s32 c);
s32 Func_02003c10(s32 layer, s32 x, s32 z);
void Func_02003c28(void);
void Func_02003c30(void);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c78(s32 id, s32 target);
void Func_02003cc0(s32 id);
s32 Func_02003d20(void);
s32 Func_02003d80(void *sprite, s32 frame);
s32 Func_02003db0(struct Actor *object);
s32 Func_02003dd0(s32 layer, s32 shape, s32 height);
struct Actor *Func_02003dd8(s32 *position, s32 mode);
void Func_02003df8(s32 cue);
void Effect_AdvanceMotionAtLeaderPriority(struct Actor *object);

/// The effect fields of an Actor, as field_effect.h names them.
struct EffectView {
  u8 unk00[0x30];
  s32 scaleRateX;
  s32 scaleRateY;
  u8 unk38[0x0c];
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  struct FieldSprite *sprite;
  u8 unk54[0x10];
  u16 spin;
  u8 unk66[6];
  void (*update)(struct Actor *object);
};
#include "event_work.h"

extern char SceneId_Ov655Room1;
extern char SceneId_Ov655Room2;
extern char SceneId_Ov655Room3;
extern char SceneId_Ov655Room5;
extern u16 gData_02003fd2[];
extern u16 gData_02003fda[];
extern u16 gData_02003fe2[];
extern u16 gData_02003fea[];
extern u16 gData_02003ff2[];
extern u16 gData_02004012[];
extern u16 gData_02004032[];
extern u16 gData_02004052[];
extern u16 gData_02004068[];
extern u16 gData_02004110[];
extern u16 gData_02004170[];
extern u16 gData_020041b8[];
extern struct Actor *gData_02004420;
extern s32 gData_02005678;
extern s32 gData_0200567c;

void FieldEvent_Wait(s32 frames);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
s32 Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Actor_SetSpritePriority(u32 id, s32 variant);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
struct Actor *Func_02003cf8(void);
void Func_02003d00(s32 exit);
void Func_02003d78(s32 skip);

#define SCENE(name) ((s32)&SceneId_##name)

void Func_02001a30(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 cellX = ((leader->x >> 20) << 1) + 1;
  s32 cellZ = ((leader->z >> 20) << 1) + 1;
  s32 n, flag;
  u16 *record;
  u32 i;

  if (gSequenceParentWork.scene == SCENE(Ov655Room1)) {
    n = work->exit - 11;
    flag = work->exit + 0x235;
    record = &gData_02004068[n * 6];
  } else if (gSequenceParentWork.scene == SCENE(Ov655Room2)) {
    n = work->exit - 14;
    flag = work->exit + 0x232;
    record = &gData_02004110[n * 6];
  } else if (gSequenceParentWork.scene == SCENE(Ov655Room3)) {
    n = work->exit - 12;
    flag = work->exit + 0x234;
    record = &gData_02004170[n * 6];
  } else if (gSequenceParentWork.scene == SCENE(Ov655Room5)) {
    n = work->exit - 18;
    flag = work->exit + 0x22e;
    record = &gData_020041b8[n * 6];
    if (GameFlag_Test(0x33a))
      gData_020041b8[26] = 3;
  } else {
    return;
  }
  Func_02003c28();
  Func_02003d78(0);
  Actor_SetSpeedParameters(gSequenceParentWork.currentOwner, 0x40000, 0x20000);
  Actor_SetPositionAndCommit(gSequenceParentWork.currentOwner, cellX << 3,
                                    cellZ << 3);
  Actor_SetPositionAndResetMotion(leader, cellX << 19, leader->y, cellZ << 19);
  FieldEvent_Wait(4);
  Func_02003cf8()->y += -0x80000;
  if (GameFlag_Test(flag)) {
    s32 x, y, index, toX, toZ;
    s16 scene;

    Func_02003df8(229);
    scene = gSequenceParentWork.scene;
    x = record[0];
    y = record[1];
    if (scene == SCENE(Ov655Room1))
      Func_02003be0(gData_02003fd2[2], gData_02003fd2[3], x, y, 1, 1);
    else if (scene == SCENE(Ov655Room2))
      Func_02003be0(gData_02003fda[2], gData_02003fda[3], x, y, 1, 1);
    else if (scene == SCENE(Ov655Room3))
      Func_02003be0(gData_02003fe2[2], gData_02003fe2[3], x, y, 1, 1);
    else if (scene == SCENE(Ov655Room5))
      Func_02003be0(gData_02003fea[2], gData_02003fea[3], x, y, 1, 1);
    index = record[2];
    toX = record[3];
    toZ = record[4];
    scene = gSequenceParentWork.scene;
    if (scene == SCENE(Ov655Room1)) {
      Func_02003be0(gData_02003ff2[index * 2], gData_02003ff2[index * 2 + 1],
                    toX, toZ, 1, 2);
    } else if (scene == SCENE(Ov655Room2)) {
      Func_02003be0(gData_02004012[index * 2], gData_02004012[index * 2 + 1],
                    toX, toZ, 1, 2);
    } else if (scene == SCENE(Ov655Room3)) {
      Func_02003be0(gData_02004032[index * 2], gData_02004032[index * 2 + 1],
                    toX, toZ, 1, 2);
    } else if (scene == SCENE(Ov655Room5)) {
      if ((n == 0 && Value1((s32 (*)())GameFlag_Test, 0x1c1)) || (n == 11 && GameFlag_Test(0x24c)))
        Func_02003be0(gData_02004052[8], gData_02004052[9], toX, toZ, 1, 2);
      else
        Func_02003be0(gData_02004052[index * 2], gData_02004052[index * 2 + 1],
                      toX, toZ, 1, 2);
    }
    FieldEvent_Wait(12);
    leader->angle = 0x4000;
    leader->motionFlags = 0;
    Actor_SetSingleChildFlags(Actor_GetById(gSequenceParentWork.currentOwner),
                                       0);
    scene = gSequenceParentWork.scene;
    if (scene == SCENE(Ov655Room1)) {
      Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 3);
    } else if (scene == SCENE(Ov655Room2) || scene == SCENE(Ov655Room3)) {
      Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 2);
    } else if (scene == SCENE(Ov655Room5)) {
      if (gData_02004420 != NULL)
        ((struct FieldSprite *)gData_02004420->animation)->priority = 3;
      Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 3);
    }
    Actor_SetModeById(gSequenceParentWork.currentOwner, 28);
    FieldEvent_Wait(16);
    for (i = 0; i <= 39; i++) {
      if (i == 5)
        Func_02003df8(204);
      leader->scaleX += -0x400;
      leader->scaleY += -0x300;
      leader->y += -0x9999;
      wait(1);
    }
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x209;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    Func_02003d00(work->exit);
  } else {
    s32 x, y;
    s16 scene;

    Func_02003df8(161);
    scene = gSequenceParentWork.scene;
    y = record[1];
    x = record[0];
    if (scene == SCENE(Ov655Room1))
      Func_02003be0(gData_02003fd2[0], gData_02003fd2[1], x, y, 1, 1);
    else if (scene == SCENE(Ov655Room2))
      Func_02003be0(gData_02003fda[0], gData_02003fda[1], x, y, 1, 1);
    else if (scene == SCENE(Ov655Room3))
      Func_02003be0(gData_02003fe2[0], gData_02003fe2[1], x, y, 1, 1);
    else if (scene == SCENE(Ov655Room5))
      Func_02003be0(gData_02003fea[0], gData_02003fea[1], x, y, 1, 1);
    FieldEvent_Wait(12);
    GameFlag_SetBit(flag);
    GameFlag_SetBit(0x201);
    scene = gSequenceParentWork.scene;
    if (scene == SCENE(Ov655Room5) || scene == SCENE(Ov655Room3)) {
      gData_0200567c = leader->x >> 20;
      gData_02005678 = leader->z >> 20;
    }
    Func_02003c30();
  }
}
