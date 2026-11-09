#include "room.h"
#include "scene_tables.h"

void Func_02000670(void) {
  ((struct SceneWork *)Ram_HeapSlots->eventWork)->unk166 = 0;
  Event_ClearFlag(0x106);
  Event_ClearFlag(0x104);
  Func_02005560();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0x20000, 0x4000);
  Camera_SetBounds(PIXELS(248), -1, PIXELS(712), 1);
  Func_020055e0();
  System_Wait(20);
  FieldCamera_SetSpeed(0x50000, 0xa000);
  FieldCamera_MoveTo(PIXELS(248), -1, PIXELS(872), 1);
  Func_020055e0();
  FieldEvent_Wait(120);
  Func_02005568();
}

void Func_020006f4(s32 burst) {
  if (!Flag_IsSet(0xa46)) {
    Func_020054e0_(29, 60, 3, 4, 14, 50);
    MapGrid_CopyCellsLayered(93, 60, 3, 4, 78, 50);
    if (burst && !Flag_IsSet(0xa43))
      Func_0200055c(14);
    Flag_Set(0xa43);
  }
}

void Func_02000754(void) { Import_EventSetExitRequest(2); }

void Func_02000760(void) { Import_EventSetExitRequest(3); }

void Func_0200076c(void) { Import_EventSetExitRequest(4); }

void Func_02000778(void) { Import_EventSetExitRequest(5); }

void Func_02000784(void) { Import_EventSetExitRequest(6); }

void Func_02000790(void) { Import_EventSetExitRequest(7); }

void Func_0200079c(void) { Import_EventSetExitRequest(8); }

void Func_020007a8(void) { Import_EventSetExitRequest(9); }

void Func_020007b4(void) { Import_EventSetExitRequest(10); }

void Func_020007c0(void) { Import_EventSetExitRequest(11); }

void Func_020007cc(void) { Import_EventSetExitRequest(12); }

void Func_020007d8(void) { Import_EventSetExitRequest(13); }

void Func_020007e4(void) { Import_EventSetExitRequest(14); }

void Func_020007f0(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (object->x >> 20 == 13) {
    object->motionFlags = 3;
    if (object->terrainHeight !=
        Map_GetTerrainHeight(BYTE_AT(object, 34), object->x, object->z)) {
      s32 frames;

      System_Wait(2);
      for (frames = 0; frames < 30 && object->y > object->terrainHeight;
           frames++)
        System_Wait(1);
      Event_PlaySound(133);
    }
    MapGrid_CopyCellsLayered(71, 24, 7, 1, 71, 5);
    MapGrid_CopyCellAttributes(7, 24, 7, 1, 7, 7);
    MapGrid_CopyCellAttributes(7, 88, 7, 1, 7, 71);
    Event_PlaySound(167);
    Actor_SetPosition(id, 0, 0);
    Flag_Set(0x358);
  }
}

void Func_020008a0(s32 unused, s32 id) {
  if (Actor_Get(id)->x >> 20 == 7)
    Flag_Set(0xa37);
}

void Func_020008bc(void) {
  Func_020054e0_(17, 24, 3, 1, 17, 13);
  Func_020054e8_(20, 24, 1, 1, 18, 77);
  Func_020054e0_(62, 20, 1, 2, 54, 20);
}

void Func_02000900(void) {
  Func_020054e0_(17, 25, 3, 1, 17, 13);
  Func_020054e8_(20, 25, 1, 1, 18, 77);
  Func_020054e0_(62, 22, 1, 2, 54, 20);
}

void Func_02000944(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 frame;

  /* FAKEMATCH: taking the object's address keeps the motion flags' address
   * inside the loop; without it GCSE hoists it out (differ 24). */
  (void)&object;
  Func_02005608();
  Actor_SetSingleChildFlags(object, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 28);
  FieldEvent_Wait(16);
  frame = 0;
step:
  if (frame == 5)
    Event_PlaySound(204);
  object->motionFlags = 0;
  object->scaleX += (-0x400);
  object->scaleY += -0x300;
  object->y += -0x9999;
  System_Wait(1);
  if (++frame <= 39)
    goto step;
  Actor_Get(gMainRoutineWork.currentOwner)->animationKind = 0;
  SCENE_WORK->unk1ac = 521;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(16);
}
