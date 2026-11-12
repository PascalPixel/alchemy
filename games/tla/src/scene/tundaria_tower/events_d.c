#include "tundaria_tower.h"
#include "iwram_call.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "scene_tables.h"
#include "types.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_0200137c(void) {
  if ((Actor_GetById(gMainRoutineWork.currentOwner)->y >> 19) != 0)
    MapCell_SetAttributes(0, 14, 17, 0);
  else
    MapCell_SetAttributes(0, 14, 17, 0x3200);
}

struct TundariaTowerEventWork {
  u8 unk000[428];
  s32 screenEffect;
};

extern const u8 gData_02003340[];
extern const u8 gData_0200335a[];
union FieldObject;
extern union FieldObject *Data_02003ccc;
void Func_020010d4(void);
void Func_02000548(void);
void Func_0200058c(void);
void Func_02000404(const u8 *data);
void Func_020011e0(void);
void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Animation_SetStateFlags(struct Actor *object, s32 a);
void Func_02003168(const u8 *data);

s32 Func_020013b8(void) {
  struct Actor *object;
  s32 scene;
  s32 flag;

  ((struct TundariaTowerEventWork *)Ram_HeapSlots->eventWork)->screenEffect =
      0x204;
  scene = gMainRoutineWork.scene;
  if (scene == (s32)&SceneId_Ov68fRoom1) {
  } else if (scene == (s32)&SceneId_Ov68fRoom2) {
    switch (gMainRoutineWork.entrance) {
    case 1:
    case 2:
    case 3:
    case 4:
      if (GameFlag_Test(0x942))
        Func_02000548();
      if (GameFlag_Test(0x943))
        Func_0200058c();
      break;
    case 11:
      flag = GameFlag_Test(0x945);
      if (flag == 0) {
        object = Actor_GetById(8);
        Actor_SetSingleChildFlags(Actor_GetById(8), 0);
        object->collisionFlags = 0;
        object->motionFlags = 0;
        object->terrainHeight = PIXELS(8);
        object->y = PIXELS(8);
        Actor_SetItemIcon(object, 0x1b9);
        if (GameFlag_Test(0x944)) {
          MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
          MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
          Actor_SetPosition(68, PIXELS(344), PIXELS(372));
          Actor_GetById(68)->update = (void (*)(void))Func_02000840;
          Actor_SetPosition(8, 0, 0);
        }
      } else if (GameFlag_Test(0x944)) {
        MapGrid_CopyCellsTo(31, 85, 20, 85, 3, 4);
        MapGrid_CopyCellAttributes(11, 22, 3, 2, 20, 22);
        Actor_SetPosition(8, 0, 0);
      }
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom4) {
    switch (gMainRoutineWork.entrance) {
    case 6:
    case 7:
      if (!GameFlag_Test(100)) {
        if (!GameFlag_Test(0x200)) {
          Actor_SetPosition(8, PIXELS(344), PIXELS(536));
          MapCell_SetAttributes(0, 21, 33, 0x3300);
          MapCell_SetAttributes(0, 22, 33, 0xff00);
        } else if (!GameFlag_Test(0x201)) {
          MapCell_SetAttributes(0, 26, 33, 0xff00);
        }
        Actor_WatchAndSetScript(8, gMainRoutineWork.currentOwner);
      }
      if (!GameFlag_Test(0x109) && gMainRoutineWork.entrance == 7)
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom5) {
    Func_02000404(gData_02003340);
  } else if (scene == (s32)&SceneId_Ov68fRoom6) {
    switch (gMainRoutineWork.entrance) {
    case 3:
      object = FieldActor_Create(0x11e, PIXELS(472), 0, PIXELS(200));
      if (object != NULL) {
        Call2((void (*)())Scene_SetActorSpritePriority, (s32)object, 1);
        object->motionFlags = 0;
        Actor_SetSingleChildFlags(object, 0);
        Actor_SetPartAttribute(object, 14);
        Animation_SetStateFlags(object, 1);
      }
      break;
    case 1:
    case 2:
      Func_02000404(gData_0200335a);
      if (GameFlag_Test(0x947)) {
        Actor_SetPosition(13, PIXELS(216), PIXELS(136));
        if (GameFlag_Test(0x946))
          Actor_SetPosition(12, PIXELS(200), PIXELS(136));
      }
      Func_02003168(gData_02003264);
      Actor_SetSpritePriority(12, 2);
      break;
    case 4:
    case 5:
      if (GameFlag_Test(0x94b)) {
        MapGrid_CopyCellsTo(18, 97, 6, 97, 3, 4);
        MapGrid_CopyCellsTo(82, 34, 70, 34, 3, 2);
        MapGrid_CopyCellAttributes(7, 36, 1, 1, 7, 35);
      }
      {
        union FieldObject **slot = &Data_02003ccc;
        struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

        effect->unk4c = 144 << 18;
        effect->update = Func_020010d4;
        *slot = (union FieldObject *)effect;
        effect->motionFlags = 0;
        Scene_SetActorSpritePriority(effect, 3);
      }
      break;
    case 6:
    case 7: {
      union FieldObject **slot = &Data_02003ccc;
      struct Actor *effect = ((struct Actor *)Import_ActorCreate(4, 0, 0, 0));

      effect->unk4c = 156 << 18;
      effect->update = Func_020010d4;
      *slot = (union FieldObject *)effect;
      effect->motionFlags = 0;
      Scene_SetActorSpritePriority(effect, 3);
    } break;
    case 10:
      if (!GameFlag_Test(0x109))
        Func_020011e0();
      break;
    }
  } else if (scene == (s32)&SceneId_Ov68fRoom7) {
    GameFlag_SetBit(0x144);
    if (GameFlag_Test(0x948)) {
      MapGrid_CopyCellsTo(13, 26, 13, 16, 3, 3);
      MapGrid_CopyCellAttributes(13, 26, 3, 3, 13, 16);
    } else {
      Task_Add(Func_0200137c, 0x480);
    }
    Func_02003168(gData_0200326e);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

extern u32 gInput;
extern s16 gData_02003272[];
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

void Func_020017dc(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->timer != 0) {
    object->timer--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (GameFlag_Test(0x106))
    angle = -1;
  else
    angle = gData_02003272[(gInput >> 4) & 15];
  if (angle == -1) {
    Func_02002f80(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}
