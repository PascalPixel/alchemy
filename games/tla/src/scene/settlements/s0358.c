#include "types.h"
#include "main_routine_work.h"
#include "local.h"
#include "io_reg.h"
#include "version.h"

extern char SceneId_Ov690Room1;
extern char SceneId_Ov690Room2;
extern char SceneId_Ov690Room4;
extern char SceneId_Ov690Room7;
extern char SceneId_Ov690Room8;
extern char SceneId_Ov690Room9;
extern char SceneId_Ov690Room10;

extern u8 gData_020024ac[];
extern u8 gData_020024dc[];
extern u8 gData_02002524[];
extern u8 gData_02002554[];
extern u8 gData_020025cc[];
extern u8 gData_02002614[];
extern u8 gData_020027ac[];
extern u8 gData_020027dc[];
extern u8 gData_02002854[];

u8 *Func_02000358(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov690Room1)
    return gData_020024dc;
  if (scene == (s32)&SceneId_Ov690Room2)
    return gData_02002524;
  if (scene == (s32)&SceneId_Ov690Room4)
    return gData_02002554;
  if (scene == (s32)&SceneId_Ov690Room5)
    return gData_020025cc;
  if (scene == (s32)&SceneId_Ov690Room7)
    return gData_020027dc;
  if (scene == (s32)&SceneId_Ov690Room8)
    return gData_02002854;
  if (scene == (s32)&SceneId_Ov690Room9)
    return gData_02002614;
  if (scene == (s32)&SceneId_Ov690Room10)
    return gData_020027ac;
  return gData_020024ac;
}

void Func_02000404(void) {
  s32 mode;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetPosition(11, 0, 0);
  Event_SetFlag(0xf5d);
  Func_02002130(193, 3);
  mode = 0;
  Party_GiveItem(193, mode);
  Func_02002070();
}

void Func_0200043c(s32 actor) {
  s32 msg = (s32)&MsgSettlements01;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) == 0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000488(void) { Window(5, 70, 9, 7, 5, 43, 1); }

void Func_020004a8(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (x == 5 && z == 17) {
    FieldActor_Jump(gMainRoutineWork.currentOwner, 2, 0);
    SetPosCommit(gMainRoutineWork.currentOwner, 88, 296);
  }
  Func_020021a0(5, 70, 9, 7, 5, 43, 1);
  MapGrid_CopyCellsTo(4, 25, 5, 17, 1, 1);
  MapGrid_CopyCellAttributes(4, 25, 1, 1, 5, 17);
  Event_SetFlag(0xa60);
}

void Func_0200052c(void) {
  s32 next = *(s16 *)((u8 *)Ram_HeapSlots->eventWork + 0x154);

  Actor_FaceDirection(4, FACING_NORTH, 0);
  FieldEvent_Wait(10);
  Actor_Get(4)->actionFlags &= 0xfe;
  CopyCells(96, 14, 72, 14, 2, 1);
  Actor_SetDestinationOffset(4, 0, 4);
  FieldActor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_Get(4), 0);
  Actor_SetAnimation(4, 13);
  Actor_SetDestinationOffset(4, 0, 16);
  FieldActor_WaitForMoveById(4);
  Actor_SetAnimation(4, 10);
  FieldEvent_Wait(10);
  Event_PlaySound(123);
  FieldEvent_SetExitRequest(next);
  Func_02002070();
}

void Func_020005cc(void) {
  s32 id;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  CommitPos(4, 0, -14);
  id = 4;
  Actor_WalkByAndWait(id, 16, 0);
  Func_0200052c();
}

void Func_020005f4(void) {
  s32 id, x, z;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  CommitPos(4, -2, -6);
  x = 14;
  z = 8;
  id = 4;
  /* FAKEMATCH(barrier): the empty asm on the actor id, which then comes
   * between the distances and their negation; as Actor_WalkByAndWait(4, -x,
   * -z), 2 of 18 instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_WalkByAndWait(id, -x, -z);
  Func_0200052c();
}

void Func_02000624(void) {
  s32 id, a;

  Func_02002068();
  FieldEvent_ResolvePendingActions(0);
  a = 192;
  id = 4;
  /* FAKEMATCH(barrier): the empty asm on the actor id, which then comes
   * between the angle byte and its shift; as Actor_FaceDirection(4, a << 8,
   * 0), 2 of 11 instructions differ in tla-en. */
  __asm__ volatile("" : "+r"(id));
  Actor_FaceDirection(id, a << 8, 0);
  Func_0200052c();
}

void Func_02000644(void) {
  CopyCells2(0, 3, 1, 1, 3, 15);
  Event_SetFlag(0x230);
}

extern u8 gData_0200289c[];
extern u8 gData_020028f0[];
extern u8 gData_02002974[];
extern u8 gData_020029a4[];
extern u8 gData_02002a70[];
extern u8 gData_02002ad0[];
extern u8 gData_02002b3c[];
extern u8 gData_02002b90[];
extern u8 gData_02002c2c[];

u8 *Func_02000668(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov690Room1)
    return gData_020028f0;
  if (scene == (s32)&SceneId_Ov690Room2)
    return gData_02002974;
  if (scene == (s32)&SceneId_Ov690Room4)
    return gData_020029a4;
  if (scene == (s32)&SceneId_Ov690Room5)
    return gData_02002a70;
  if (scene == (s32)&SceneId_Ov690Room7)
    return gData_02002ad0;
  if (scene == (s32)&SceneId_Ov690Room8)
    return gData_02002b3c;
  if (scene == (s32)&SceneId_Ov690Room9)
    return gData_02002b90;
  if (scene == (s32)&SceneId_Ov690Room10)
    return gData_02002c2c;
  return gData_0200289c;
}

void Func_02000714(struct Actor *object) {
  s32 step;

  object->x += object->action << 8;

  object->y += 0x8000;

  step = 0x51e;
  object->scaleX += step;
  object->scaleY += step;
  object->action += 2;
  if (--*(s32 *)&object->watchedObject == 0)
    Func_02002000(object);
}

void Func_02000758(s32 x, s32 y, s32 z) {
  struct Actor *object = FieldActor_Create(0x11d, x, y, z);

  if (object != NULL) {
    s8 *animation;

    object->unk5c[0] = 2;
    object->motionFlags = 0;
    object->action = 0;
    object->priorityFlags &= 0xfe;
    Actor_SetSpriteColor(object, 7);
    animation = object->animation;
    animation[9] = (animation[9] & -13) | 4;
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    *(s32 *)&object->watchedObject = 60;
    object->update = (void (*)(void))Func_02000714;
    Func_02001fe0(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}

void Func_020007d8(void) {
  if ((gFrameCount & 63) == 0)
    Func_02000758(0xf80000, 0x300000, 0x1000000);
}

u32 Random16Far(void);

/// Moves the object around its orbit's centre (words 14 and 15) and steps the
/// angle by a random amount.
// As ☀️'s Scene_UpdateFloatingNut does.
s32 Func_020007fc(s32 *p) {
  s16 *q = (s16 *)p[20];
  s32 a, b;
  s32 d = Math_Sin(p[12]) * 2;

  if (d > 0)
    d = -d;
  p[2] = p[14] + Math_Cosine(p[12]) * 2;
  p[3] = p[15] + d;
  q[9] = Math_Cosine(p[12] + 0x8000) / 8;
  a = Random16Far();
  b = Random16Far();
  {
    u32 x;
    s32 angle;

    /* FAKEMATCH(dead): the empty once-only block keeps the first random
     * value's shift after the second call; without it, 2 of 44 instructions
     * differ in tla-en (with the asm below also removed, none differ). */
    do {
    } while (0);
    x = (u32)a << 9;
    angle = p[12];
    p[12] = angle + ((x >> 16) + (((u32)b << 9) >> 16)) + 0x400;
  }
  return 0;
}

struct OrbitSprite {
  u8 unk00[5];
  u8 flags05Low : 5;
  u8 flags05Bit5 : 1;
  u8 flags05High : 2;
  u8 unk06[3];
  u8 flags09Low : 2;
  u8 flags09Mode : 2;
  u8 flags09High : 4;
  u8 unk0a[6];
  u8 palette;
  u8 unk11[10];
  u8 state;
};

struct OrbitObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[15];
  u8 flags23;
  u8 unk24[12];
  s32 orbitAngle;
  u8 unk34[4];
  s32 orbitCenterX;
  s32 orbitCenterY;
  s32 orbitCenterZ;
  u8 unk44[12];
  struct OrbitSprite *sprite;
  u8 unk54;
  u8 mode;
  u8 state;
  u8 unk57[5];
  u8 active;
  u8 unk5d[4];
  u8 visible;
  u8 unk62[10];
  u32 callback;
};

void Func_02002180(s32 a, s32 b, s32 c, s32 d);
void Func_02002170(void);
u8 *HeapSlot_AllocFar(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);

static __inline__ void Settlements_Func_02002180(s32 a, s32 b, s32 c, s32 d) {
  Func_02002180(a, b, c, d);
}

void Func_02000860(void) {
  struct OrbitObject *actor;
  struct OrbitSprite *sprite;
  u8 *transfer;

  REG_BLDALPHA = 0xc08;
  REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
  {
    s32 priority = 0x480;

    FieldTask_AddFrame(Func_020007d8, priority);
  }
  Func_02002170();
  Func_02002170();
  Settlements_Func_02002180(1, 0x203, 8, 9);
  if (Event_GetFlag(0xf5d)) {
    Actor_SetPosition(11, 0, 0);
    return;
  }
  actor = (struct OrbitObject *)Actor_Get(11);
  sprite = actor->sprite;
  sprite->flags09Mode = 1;
  sprite->flags05Bit5 = 0;
  sprite->flags09High = 0;
  sprite->state = 0;
  Actor_SetSingleChildFlags((struct Actor *)actor, 0);
  actor->active = 0;
  actor->mode = 0;
  if (Event_GetFlag(0x109) == 0)
    actor->y += 0x100000;
  actor->flags23 &= 0xfe;
  actor->visible = 1;
  transfer = HeapSlot_AllocFar(68, 0x608);
  Item_LoadIcon(193);
  transfer += 0x400;
  Vram_Load(sprite->palette, 128, transfer);
  HeapSlot_Release(68);
  actor->orbitCenterX = actor->x;
  actor->orbitAngle = 0;
  actor->orbitCenterY = actor->y;
  actor->active = 1;
  actor->callback = (u32)Func_020007fc;
  actor->state = 0;
}

extern char SceneId_Ov690Room6;

extern const u8 gData_02002272[];

void Func_02000e04(void);
void Func_0200125c(void);
void Func_02002140(s32 a);
void Func_02002188(const void *data);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Place(s32 id, s32 x, s32 z) { Actor_SetHorizontalPositionWithTerrain(id, x, z); }

s32 Func_02000988(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room7)
    Func_02000860();
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room9) {
    Func_02002140(0);
    Func_02002188(gData_0200226c);
    if (GameFlag_Test(0x206))
      Actor_SetHorizontalPositionWithTerrain(10, 0, 0);
    Actor_GetById(10)->priorityFlags |= 2;
    Actor_SetModeById(18, 2);
    Actor_SetModeById(19, 2);
    Actor_SetModeById(21, 2);
    if (GameFlag_Test(0x205))
      Actor_SetModeById(10, 2);
    Func_02000e04();
#if TLA_PATCH_1
    if (GameFlag_Test(0x204)) {
      Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
      Place(21, PIXELS(184), PIXELS(184));
      Actor_GetById(21)->motionFlags = 0;
      CopyCells2(9, 11, 1, 1, 11, 12);
    }
#endif
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room4) {
    if (GameFlag_Test(0x230)) {
      CopyCells2(0, 3, 1, 1, 3, 15);
      Actor_SetModeById(9, 4);
    }
    if (GameFlag_Test(0x109) && Flag_Test(0x220)) {
      GameFlag_ClearBit(0x220);
      Func_0200125c();
    }
    Func_02002188(gData_02002272);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room3)
    Func_02002140(0);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov690Room6)
    Func_02002140(0);
  return 0;
}
