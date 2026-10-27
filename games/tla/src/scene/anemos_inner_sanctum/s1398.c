#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x154];
  s16 unk154;
};

extern char SceneId_Ov6B0Room4;
extern const s32 gData_02003574[];

struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void FieldEvent_WaitTicks(s32 ticks);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_020032e0(struct ObjectRuntime *object, s32 value);
void Func_020032e8(struct ObjectRuntime *object, const s32 *script);
void Func_02003328(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003398(void);
void Func_020033a0(void);
void Func_02003420(s32 a, s32 b);
void Func_02003430(void);
void Func_02003438(s32 id, s32 a);
void Func_02003488(s32 a);
void Func_020034b8(s32 sound);

static __inline__ struct ObjectRuntime *Create(s32 type, s32 x, s32 y, s32 z) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 4
   * of 159 instructions differ in tla-en. */
  return Engine_ObjectCreate(type, x, y, z);
}

/// An event handler from the sanctum's event table (data.c): changes the
/// cells at the leader's place, spawns an effect of type 0x15d there and sets
/// the flag the event work's unk154 selects; once flags 0x200 to 0x206 are all
/// set, it moves object 8 in room 4 (else object 9) to mode 2 and sets flag
/// 0x207.
void Func_02001398(void) {
  struct AnemosInnerSanctumEventWork *work = Ram_HeapSlots->eventWork;
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  struct ObjectRuntime *object;
  s32 all;
  s32 i;
  s32 id;
  s32 x;
  s32 z;
  s32 scene;

  scene = gPartyState.scene;
  x = leader->x;
  z = leader->z;
  if (scene == (s32)&SceneId_Ov6B0Room4)
    Func_02003328(17, 71, x >> 20, (z >> 20) + 64, 1, 1);
  else
    Func_02003328(54, 87, x >> 20, (z >> 20) + 64, 1, 1);
  Func_020034b8(106);
  object = Create(0x15d, ((leader->x >> 20) << 20) + 0x80000, 0,
                  ((leader->z >> 20) << 20) + 0x80000);
  object->priorityFlags |= 2;
  object->motionFlags = 0;
  Object_SetPartAttribute(object, 4);
  object->scaleY = 0x16000;
  object->scaleX = 0x16000;
  Object_SetSpritePriority(object, 2);
  Func_020032e0(object, 3);
  ObjectDispatch_SetSingleChildFlags(object, 0);
  Func_020032e8(object, gData_02003574);
  GameFlag_SetBit(work->unk154 + 0x1ce);
  i = 1;
  all = GameFlag_Test(0x200);
next:
  all &= GameFlag_Test(0x200 + i);
  if (++i != 7)
    goto next;
  if (all) {
    Func_02003398();
    Func_02003488(0);
    FieldEvent_WaitTicks(10);
    Func_02003420(0xcccc, 0x1999);
    id = 9;
    if (gPartyState.scene == (s32)&SceneId_Ov6B0Room4)
      id = 8;
    Func_02003438(id, 1);
    Func_02003430();
    FieldEvent_WaitTicks(10);
    Func_020034b8(0x246);
    Object_SetModeById(id, 2);
    GameFlag_SetBit(0x207);
    Object_GetById(id)->motionFlags = 4;
    FieldEvent_WaitTicks(80);
    Func_020033a0();
  }
}
