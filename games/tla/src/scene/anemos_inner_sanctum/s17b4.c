#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "frame.h"

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x16a];
  u16 unk16a;
};

extern const s32 gData_02003594[];

struct ObjectRuntime *Object_GetById(s32 id);
struct ObjectRuntime *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
u32 Random16Far(void);
void GameFlag_ClearBit(s32 flag);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Object_SetSpritePriority(struct ObjectRuntime *object, s32 priority);
void Func_020032e0(struct ObjectRuntime *object, s32 value);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object,
                                        s32 flags);
void Func_020032e8(struct ObjectRuntime *object, const s32 *script);
void Func_020034b8(s32 sound);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
s32 Func_02003348(s32 layer, s32 x, s32 z);

static __inline__ void AddPolar(s32 length, s32 angle, s32 *position) {
  /* FAKEMATCH(wrapper): forwarded inline (scene_call.h); as a direct call, 4
   * of 171 instructions differ in tla-en. */
  Vector_AddPolarOffsetFar(length, angle, position);
}

/// The update of an object that moves with the leader: once the leader is past
/// z 248 it settles on its cell and clears flag 0x200; otherwise it repeats the
/// leader's step, raising an effect of type 0x2a0 on each step, and stops where
/// the cell kind under it is 52 or 51, setting the event work's unk16a to 201
/// or 200. unk4c and acceleration hold the leader's last x and z.
void Func_020017b4(struct ObjectRuntime *self) {
  struct ObjectRuntime *leader = Object_GetById(gPartyState.currentOwner);
  s32 next[2];
  s32 ahead[3];
  struct ObjectRuntime *dust;
  s32 kind;

  if (Object_GetById(gPartyState.currentOwner)->z >> 16 > 248) {
    self->update = 0;
    GameFlag_ClearBit(0x200);
    self->x = (self->x & 0xfff00000) + 0x80000;
    self->z = (self->z & 0xfff00000) + 0x80000;
    return;
  }
  ahead[0] = next[0] = self->x - (leader->x - self->unk4c);
  ahead[1] = leader->y;
  ahead[2] = next[1] = self->z + (leader->z - self->acceleration);
  if (self->unk4c != leader->x || self->acceleration != leader->z) {
    dust = Engine_ObjectCreate(0x2a0, self->x, self->y, self->z);
    dust->x += ((Random16Far() & 15) - 8) << 16;
    dust->y += ((Random16Far() & 31) - 4) << 16;
    dust->motionFlags = 0;
    Object_SetPartAttribute(dust, ((u8 *)self)[0x62]);
    Object_SetSpritePriority(dust, 2);
    Func_020032e0(dust, 1);
    ObjectDispatch_SetSingleChildFlags(dust, 0);
    Func_020032e8(dust, gData_02003594);
    self->unk4c = leader->x;
    self->acceleration = leader->z;
    if ((gFrameCount & 3) == 0)
      Func_020034b8(106);
  }
  if (ahead[2] >> 16 > 248)
    return;
  AddPolar(0x70000, 0x8000 - leader->angle, ahead);
  if (Func_02003348(self->terrainId, ahead[0], ahead[2]) == 255)
    return;
  {
    s32 x = next[0];

    self->x = x;
    self->z = next[1];
    /* FAKEMATCH(barrier): an empty asm holding x and the z just stored, so x
     * is copied for the call before terrainId is read; as a call on next[0]
     * and next[1], 2 of 171 instructions differ in tla-en. */
    asm("" : "+l"(x), "+m"(self->z));
    kind = Func_02003348(self->terrainId, x, next[1]);
  }
  if (kind == 52) {
    ((struct AnemosInnerSanctumEventWork *)Ram_HeapSlots->eventWork)->unk16a =
        201;
    self->update = 0;
  } else if (kind == 51) {
    ((struct AnemosInnerSanctumEventWork *)Ram_HeapSlots->eventWork)->unk16a =
        200;
    self->update = 0;
  }
}
