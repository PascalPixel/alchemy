#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "sequence_parent_work.h"

extern const u8 gData_02003f52[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fc0[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(s32 id);
void Func_02000038(const void *data);
void Func_02000584(const void *data);
void Func_020012d8(void);
void Func_020018a8(void);
void Func_02001fac(void);
void Func_02003be0(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Func_02003bf0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003c68(s32 id, s32 x, s32 z);
void Func_02003c70(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc0(s32 id);
void Func_02003db8(const void *data);

static __inline__ s32 Flag_Test(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  return GameFlag_Test(flag);
}

static __inline__ void Flag_Set(s32 flag) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  GameFlag_SetBit(flag);
}

static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003bf0(a, b, c, d, e, f);
}

static __inline__ void Actor_SetPosition(struct Actor *object, s32 x,
                                         s32 y, s32 z) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  /* FAKEMATCH: forwarded inline; see scene_call.h. */
  Func_02003c70(id, x, z, angle);
}

void Func_02003164(void) {
  if (Flag_Test(265) == 0) {
    Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags |= 32;
    if (gSequenceParentWork.entrance == 5) {
      Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 2);
      Actor_GetById(gSequenceParentWork.currentOwner)->priorityFlags |= 1;
    }
  }
  Func_02000584(gData_02003f52);
  Func_020012d8();
  Func_02000038(gData_02003fa0);
  if (Flag_Test(0x840) != 0) {
    Func_02003cc0(23);
    Map_CopyAttributes(50, 7, 1, 1, 52, 7);
    Actor_SetPosition(Actor_GetById(23), 210 << 18, 0, 240 << 15);
  } else {
    Func_02003db8(gData_02003fc0);
  }
  Func_020018a8();
  if (Flag_Test(0x86c) != 0) {
    /* FAKEMATCH: the named angle is started before the two coordinates, as the
     * native does; as a literal argument it is started after them (differ 4).
     */
    s32 angle = 0x4000;

    Actor_PlaceFacing(24, 222 << 18, 168 << 16, angle);
    Func_02003be0(68, 33, 105, 6, 1, 1);
    Func_02003be0(69, 33, 105, 4, 1, 1);
    Flag_Set(583);
  }
  if (Flag_Test(0x86d) != 0)
    Func_02003c68(24, 0, 0);
  Func_02001fac();
}
