#include "types.h"
#include "flag_call.h"
#include "party_state.h"
#include "scene_ids.h"
#include "field_event.h"

// Import stubs: the event runtime's begin and end, and the pending
// actions' resolution.
void Func_02000fcc(void);
void Func_02000fd4(void);
void Func_0200104c(s32 skip);
void Func_0200043c(void *cells, s32 index);
extern u32 gData_0200117c[];
extern u32 gData_020011cc[];

/// For each of cells 0 to 3 of gData_0200117c whose flag (0x303 to 0x306) is
/// still clear, calls Func_0200043c and sets the flag; the event runtime is
/// begun before the first and ended after the last.
void Func_020005ac(void) {
  s16 count = 0;

  if (!Flag_IsSet(0x303)) {
    Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_0200117c, 0);
    Flag_Set(0x303);
    count = 1;
  }
  if (!Flag_IsSet(0x304)) {
    if (count == 0)
      Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_0200117c, 1);
    Flag_Set(0x304);
    count++;
  }
  if (!Flag_IsSet(0x305)) {
    if (count == 0)
      Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_0200117c, 2);
    Flag_Set(0x305);
    count++;
  }
  if (!Flag_IsSet(0x306)) {
    if (count == 0)
      Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_0200117c, 3);
    Flag_Set(0x306);
    count++;
  }
  if (count != 0)
    Func_02000fd4();
}

/// For each of cells 0 to 2 of gData_020011cc whose flag (0x30a to 0x30c) is
/// still clear, calls Func_0200043c and sets the flag; the event runtime is
/// begun before the first and ended after the last.
void Func_02000684(void) {
  s16 count = 0;

  if (!Flag_IsSet(0x30a)) {
    Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_020011cc, 0);
    Flag_Set(0x30a);
    count = 1;
  }
  if (!Flag_IsSet(0x30b)) {
    if (count == 0)
      Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_020011cc, 1);
    Flag_Set(0x30b);
    count++;
  }
  if (!Flag_IsSet(0x30c)) {
    if (count == 0)
      Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_020011cc, 2);
    Flag_Set(0x30c);
    count++;
  }
  if (count != 0)
    Func_02000fd4();
}

void Func_02000728(void) {
  if (!Flag_IsSet(0x307)) {
    Func_02000fcc();
    Func_0200104c(0);
    Func_0200043c(gData_0200117c, 4);
    Flag_Set(0x307);
    Func_02000fd4();
  }
}

struct FieldActor *Object_GetById(s32 id);

/// Called through the scene table with an object id: in each of the first
/// four rooms, sets a flag when object 8 to 11 is at a given x; object 11 at
/// a given x and z in the second room runs Func_02000728 instead.
void Func_02000760(s32 unused, s32 id) {
  struct FieldActor *object = Object_GetById(id);

  if (gPartyState.scene == (s32)&SceneId_Ov6afRoom1) {
    if (id == 8 && object->x >> 20 == 16)
      GameFlag_SetBit(0x300);
    if (id == 9 && object->x >> 20 == 20)
      GameFlag_SetBit(0x301);
    if (id == 10 && object->x >> 20 == 27)
      GameFlag_SetBit(0x302);
  }
  /* FAKEMATCH: the memory barriers before the second, third and fourth room
   * tests keep GCSE's PRE from moving their party state loads onto the edges
   * after the calls, so each test loads it again as the native does (with any
   * two of the three: differ 29). */
  __asm__ volatile("" : : : "memory");
  if (gPartyState.scene == (s32)&SceneId_Ov6afRoom2) {
    if (id == 10 && object->x >> 19 == 33)
      GameFlag_SetBit(0x308);
    if (id == 11 && object->x >> 19 == 85 && object->z >> 19 == 83)
      Func_02000728();
  }
  __asm__ volatile("" : : : "memory");
  if (gPartyState.scene == (s32)&SceneId_Ov6afRoom3) {
    if (id == 10 && object->x >> 19 == 83)
      GameFlag_SetBit(0x309);
  }
  __asm__ volatile("" : : : "memory");
  if (gPartyState.scene == (s32)&SceneId_Ov6afRoom4) {
    if (id == 8 && object->x >> 19 == 87)
      GameFlag_SetBit(0x30d);
  }
}
