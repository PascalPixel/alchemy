#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "flag_call.h"
#include "task.h"
#include "scene_tables.h"

struct TreasureIsleEventWork {
  u8 unk000[0x1ac];
  s32 route;
};

extern char SceneId_Ov6afRoom1;
extern char SceneId_Ov6afRoom2;
extern char SceneId_Ov6afRoom3;
extern char SceneId_Ov6afRoom4;
extern u32 gData_0200117c[];
extern u32 gData_020011cc[];

struct Actor *Actor_GetById(s32 id);
s32 Task_Add(void (*func)(void), s32 priority);
void Func_020003fc(void *cells, s32 index);
void Func_02000ba0(void);
void Func_02000fec(s32 id, s32 x, s32 z);
void Func_0200105c(void);
s32 Func_02001064(s32 a, s32 b, s32 c);

static __inline__ void PlaceObject(s32 id, s32 x, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as direct calls, 12
   * of 241 instructions differ in tla-en. */
  Func_02000fec(id, x, z);
}

/// Treasure Isle's setup for each of its four rooms: places the objects whose
/// flags are set, schedules Func_02000ba0 in rooms 2 and 3 and opens again
/// the cells already opened there (Func_020003fc), and in room 4 raises the
/// part priority of objects 9 to 16.
s32 Func_02000cf4(void) {
  s32 i;

  ((struct TreasureIsleEventWork *)Ram_HeapSlots->eventWork)->route = 0x204;
  Func_0200105c();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom1) {
    Flag_Set(0x300);
    Flag_Set(0x301);
    Flag_Set(0x302);
    if (Flag_IsSet(0x300))
      PlaceObject(8, PIXELS(264), PIXELS(440));
    if (Flag_IsSet(0x301))
      PlaceObject(9, PIXELS(328), PIXELS(440));
    if (Flag_IsSet(0x302))
      PlaceObject(10, PIXELS(440), PIXELS(344));
  }
  /* FAKEMATCH(barrier): an empty asm with a memory clobber before each later
   * room test, so GCSE does not carry gMainRoutineWork's address from one test to
   * the next; without this one, 27 of 241 instructions differ in tla-en. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom2) {
    Task_Add(Func_02000ba0, TASK_PASS_VBLANK);
    Func_02001064(0, 8, 9);
    if (Flag_IsSet(0x308))
      PlaceObject(10, PIXELS(264), PIXELS(184));
    if (Flag_IsSet(0x303))
      Func_020003fc(gData_0200117c, 0);
    if (Flag_IsSet(0x304))
      Func_020003fc(gData_0200117c, 1);
    if (Flag_IsSet(0x305))
      Func_020003fc(gData_0200117c, 2);
    if (Flag_IsSet(0x306))
      Func_020003fc(gData_0200117c, 3);
    if (Flag_IsSet(0x307))
      Func_020003fc(gData_0200117c, 4);
  }
  /* FAKEMATCH(barrier): as above; without it, 35 of 241 instructions differ in
   * tla-en. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom3) {
    Task_Add(Func_02000ba0, TASK_PASS_VBLANK);
    Func_02001064(0, 8, 9);
    if (Flag_IsSet(0x309))
      PlaceObject(10, PIXELS(664), PIXELS(248));
    if (Flag_IsSet(0x30a))
      Func_020003fc(gData_020011cc, 0);
    if (Flag_IsSet(0x30b))
      Func_020003fc(gData_020011cc, 1);
    if (Flag_IsSet(0x30c))
      Func_020003fc(gData_020011cc, 2);
  }
  /* FAKEMATCH(barrier): as above; without it, 39 of 241 instructions differ in
   * tla-en. */
  __asm__("" : : : "memory");
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6afRoom4) {
    if (Flag_IsSet(0x30d))
      PlaceObject(8, PIXELS(696), PIXELS(600));
    /* FAKEMATCH(signature): Func_02001064 declared returning s32, though it
     * reaches Func_080d9e98, whose draft returns nothing; as void, these calls
     * set r0 first and 4 of 241 instructions differ in tla-en. */
    Func_02001064(0, 17, 18);
    Func_02001064(1, 19, 20);
    Func_02001064(2, 21, 22);
    i = 0;
  next:
    {
      /* FAKEMATCH(pin): the flags byte's address pinned to r2, which leaves r1
       * to the 12, as the native has them; unpinned, 12 of 241 instructions
       * differ in tla-en. */
      register u8 *flags asm("r2") =
          (u8 *)Actor_GetById(i + 9)->animation + 37;

      *flags |= 12;
    }
    if (++i <= 7)
      goto next;
  }
  return 0;
}
