#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"

extern const u8 gData_02003f52[];
extern const u8 gData_02003fa0[];
extern const u8 gData_02003fc0[];

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
struct Actor *Actor_GetById(u32 objectId);
void Func_02000038(const void *data);
void Func_02000584(const void *data);
void Func_020012d8(void);
void Func_020018a8(void);
void Func_02001fac(void);
void Map_CopyCellPictureIndicesRect(s32 srcX, s32 srcY, s32 destX, s32 destY, s32 width,
                   s32 height);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Actor_SetHorizontalPositionAndAngleWithTerrain(s32 id, s32 x, s32 z, s32 angle);
void Func_02003cc0(s32 id);
void Func_02003db8(const void *data);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyAttributes(s32 a, s32 b, s32 c, s32 d, s32 e,
                                          s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Actor_SetPosition(struct Actor *object, s32 x,
                                         s32 y, s32 z) {
  Actor_SetPositionAndResetMotion(object, x, y, z);
}

static __inline__ void Actor_PlaceFacing(s32 id, s32 x, s32 z, s32 angle) {
  Actor_SetHorizontalPositionAndAngleWithTerrain(id, x, z, angle);
}

void Func_02003164(void) {
  if (GameFlag_Test(265) == 0) {
    Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 32;
    if (gMainRoutineWork.entrance == 5) {
      Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
      Actor_GetById(gMainRoutineWork.currentOwner)->priorityFlags |= 1;
    }
  }
  Func_02000584(gData_02003f52);
  Func_020012d8();
  Func_02000038(gData_02003fa0);
  if (GameFlag_Test(0x840) != 0) {
    Func_02003cc0(23);
    Map_CopyAttributes(50, 7, 1, 1, 52, 7);
    Actor_SetPosition(Actor_GetById(23), 210 << 18, 0, 240 << 15);
  } else {
    Func_02003db8(gData_02003fc0);
  }
  Func_020018a8();
  if (GameFlag_Test(0x86c) != 0) {
    /* FAKEMATCH(temporary): the angle held in a local is started before the two
     * coordinates, as the native does; as the literal argument, started after
     * them, 4 of 114 instructions differ in tla-en. */
    s32 angle = 0x4000;

    Actor_PlaceFacing(24, 222 << 18, 168 << 16, angle);
    Map_CopyCellPictureIndicesRect(68, 33, 105, 6, 1, 1);
    Map_CopyCellPictureIndicesRect(69, 33, 105, 4, 1, 1);
    GameFlag_SetBit(583);
  }
  if (GameFlag_Test(0x86d) != 0)
    Actor_SetHorizontalPositionWithTerrain(24, 0, 0);
  Func_02001fac();
}
