#include "sea_of_time.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "scene_ids.h"

extern const u8 gData_020022d0[];
extern const u8 gData_0200242c[];
extern const u8 gData_0200251c[];

void Func_0200141c(s32 x, s32 z, s32 actor) {
  s32 cx = x >> 20;
  s32 cz = z >> 20;

  Func_02000388(1, cx - 1, cz - 1, 0);
  Func_02000388(1, cx, cz - 1, 0);
  Func_02000388(1, cx - 1, cz, 0);
  Func_02000388(1, cx, cz, 0);
  Func_02001fdc(actor, 0, 0);
}

void Func_02001480(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Func_02002044(0);
  object = Actor_GetById(9);
  Func_0200141c(object->x, Actor_GetById(9)->z, 9);
  GameFlag_SetBit(0xa90);
  Func_02001fb4();
}

void Func_020014b4(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Func_02002044(0);
  object = Actor_GetById(10);
  Func_0200141c(object->x, Actor_GetById(10)->z, 10);
  GameFlag_SetBit(0xa91);
  Func_02001fb4();
}

void Func_020014ec(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Func_02002044(0);
  object = Actor_GetById(11);
  Func_0200141c(object->x, Actor_GetById(11)->z, 11);
  GameFlag_SetBit(0xa92);
  Func_02001fb4();
}

void Func_02001524(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Func_02002044(0);
  object = Actor_GetById(12);
  Func_0200141c(object->x, Actor_GetById(12)->z, 12);
  GameFlag_SetBit(0xa93);
  Func_02001fb4();
}

void Func_0200155c(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Func_02002044(0);
  object = Actor_GetById(13);
  Func_0200141c(object->x, Actor_GetById(13)->z, 13);
  GameFlag_SetBit(0xa94);
  Func_02001fb4();
}

void Func_02001594(void) {
  struct SeaOfTimeObject *object;

  Func_02001fac();
  Func_02002044(0);
  object = Actor_GetById(9);
  Func_0200141c(object->x, Actor_GetById(9)->z, 9);
  GameFlag_SetBit(0xa95);
  Func_02001fb4();
}

void Func_020015cc(void) {
  Func_02001fac();
  Func_02002044(0);
  Func_02002004(4);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fb4();
}

void Func_020015ec(void) {
  Func_02001fac();
  Func_02002044(0);
  Func_02002004(5);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_02001fb4();
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov687Room1)
    return (const struct Trigger *)gData_020022d0;
  if (gMainRoutineWork.entrance <= 4)
    return (const struct Trigger *)gData_0200242c;
  return (const struct Trigger *)gData_0200251c;
}
