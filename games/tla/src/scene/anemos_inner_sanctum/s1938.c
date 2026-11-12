#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"

struct AnemosInnerSanctumEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
};

void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02000158(void);
void Func_02000178(void);
void Func_020017b4(void);
void Func_020034a8(void);

void Func_02001938(void) {
  struct Actor *object;

  if (gMainRoutineWork.entrance == 1)
    object = Actor_Get(8);
  else if (gMainRoutineWork.entrance == 3)
    object = Actor_Get(9);
  else
    object = Actor_Get(10);
  if (*(s16 *)((u8 *)Actor_Get(gMainRoutineWork.currentOwner) + 18) <= 247) {
    object->motionFlags = 0;
    object->update = Func_020017b4;
    object->unk4c = Actor_Get(gMainRoutineWork.currentOwner)->x;
    object->acceleration = Actor_Get(gMainRoutineWork.currentOwner)->z;
    Event_SetFlag(0x200);
  }
}

#include "field_sprite.h"
#include "io_reg.h"

struct Actor *OverlayObject_PrepareObject(s32 x, s32 y, s32 z, s32 type);
void Func_02003398(void);
void Func_020033a0(void);
void Func_020032f8(struct Actor *object);

struct SpriteView {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 mode : 2;
  u16 unk4 : 4;
  u8 unk06[14];
  s32 unk14;
};

void Func_020019a8(void) {
  struct Actor *object;
  struct Actor *effect;
  struct FieldSprite *sprite;
  s32 position[3];

  if (gMainRoutineWork.entrance == 1)
    object = Actor_Get(8);
  else if (gMainRoutineWork.entrance == 3)
    object = Actor_Get(9);
  else
    object = Actor_Get(10);
  position[0] = (object->x & 0xfff00000) + 0x80000;
  position[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTarget(object, position[0], 0, position[2]);
  effect =
      OverlayObject_PrepareObject(object->x, 0, position[2] + 0x3b0000, 286);
  effect->scaleX = 0x18000;
  effect->scaleY = 0x18000;
  sprite = effect->animation;
  ((struct SpriteView *)sprite)->mode = 2;
  REG_WINOUT = WINOUT_WIN01_ALL | WINOUT_WINOBJ_BG0 | WINOUT_WINOBJ_BG1 |
               WINOUT_WINOBJ_BG2 | WINOUT_WINOBJ_BG3 | WINOUT_WINOBJ_CLR;
  REG_DISPCNT |= DISPCNT_OBJWIN_ON;
  Event_PlaySound(204);
  object->motionFlags = 3;
  OverlayObject_WaitUntilIdle(object);
  object->x = 0;
  object->y = 0;
  object->z = 0;
  Actor_SetPosition(8, 0, 0);
  Func_020032f8(effect);
  Func_020033a0();
}

void Func_02003380(s32 x, s32 z, s32 a, s32 b);

static __inline__ void Func_02003328_(s32 srcX, s32 srcY, s32 dstX, s32 dstY,
                                      s32 width, s32 height) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which sets the
   * width before the height as the ROM allocates them (1 in r5, 2 in r6); as a
   * direct call, the two constants swap registers, 12 of 98 instructions of
   * Func_02001a94 differ in tla-en. */
  Map_CopyCellPictureIndicesRect(srcX, srcY, dstX, dstY, width, height);
}

/// By the entrance taken, sets that entrance's flag, moves its actor to the
/// middle of its cell and copies a cell picture column twice beside it.
void Func_02001a94(void) {
  struct Actor *object;
  s32 column;
  s32 target[3];

  if (gMainRoutineWork.entrance == 1) {
    object = Actor_Get(8);
    Event_SetFlag(0x301);
    column = 11;
  } else if (gMainRoutineWork.entrance == 3) {
    object = Actor_Get(9);
    Event_SetFlag(0x302);
    column = 26;
  } else {
    object = Actor_Get(10);
    Event_SetFlag(0x303);
    column = 53;
  }
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  target[0] = (object->x & 0xfff00000) + 0x80000;
  target[2] = (object->z & 0xfff00000) + 0x80000;
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_SetMoveTarget(object, target[0], 0, target[2]);
  Actor_WaitForMove(object);
  Event_PlaySound(188);
  Func_02003328_(18, 4, column, 4, 1, 2);
  System_Wait(5);
  MapGrid_CopyCellsTo(18, 7, column, 4, 1, 2);
  System_Wait(5);
  Func_02003380(column << 20, 0x580000, 0, 0);
  Func_020033a0();
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

extern u8 MsgSummonInheritedEarthDarkness[];

void Func_02001b88(void) {
  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  PaletteFade_ApplySource(0x10000, 0);
  PaletteFade_ApplyTarget(0x10003, 1);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  FieldMessage_ShowCentered((s32)MsgSummonInheritedEarthDarkness, 0, 6);
  PaletteFade_ApplyTarget(0x10000, 0);
  PaletteFade_Interpolate(0x3c);
  FieldEvent_Wait(60);
  Event_SetFlag(0xa8c);
  Func_020033a0();
}

extern char SceneId_Ov6B0Room1;
extern char SceneId_Ov6B0Room2;
extern char SceneId_Ov6B0Room3;
extern char SceneId_Ov6B0Room4;
extern char SceneId_Ov6B0Room5;
extern const u8 gData_02003dc0[];
extern const u8 gData_02003dcc[];
extern const u8 gData_02003e38[];
extern const u8 gData_02003fac[];
extern const u8 gData_02004150[];
extern const u8 gData_020042c4[];

const u8 *Func_02001bec(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room1)
    return gData_02003dcc;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room2)
    return gData_02003e38;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room3)
    return gData_02003fac;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room4)
    return gData_02004150;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room5)
    return gData_020042c4;
  return gData_02003dc0;
}

extern char SceneId_Ov694Room1;

struct MainRoutineWork2c2 {
  u8 unk000[0x2c2];
  u8 value;
};

void Func_020000f4(void);
void Func_02003448(s32 scene, s32 entrance);
void Func_020034b0(void);

void Func_02001c60(void) {
  Func_020000f4();
  Func_020034b0();
  {
    struct MainRoutineWork2c2 *party =
        (struct MainRoutineWork2c2 *)&gMainRoutineWork;

    party->value = 2;
  }
  Func_02003448((s32)&SceneId_Ov694Room1, 99);
  FieldEvent_Wait(40);
  Func_02000158();
}

#include "scene_ids.h"

struct EventWork154 {
  u8 unk000[0x154];
  s16 value;
};

struct AnemosInnerSanctumPicture {
  const void *data;
  u16 a;
  u16 b;
};

extern const s16 gData_020043e4[];
extern const struct AnemosInnerSanctumPicture gData_02004410[];

void Event_FinishScreenTransition(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02003320_(const void *data, s32 a, s32 b) {
  Map_PlayMapEdit(data, a, b);
}

/// The scene opening: clears the actors' motion flags, plays the entrance's
/// map edit and steps the leader in.
void Func_02001c94(void) {
  struct EventWork154 *work = Ram_HeapSlots->eventWork;
  s32 i;

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop, as in overlay 6a6's Func_02000e54 (its twin): it carries no
  // loop notes.
  i = 8;
loop:
  {
    struct Actor *object = Actor_Get(i);

    if (object != NULL)
      object->motionFlags = 0;
  }
  i++;
  if ((u32)i <= 63)
    goto loop;
  Event_PlaySound(158);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov6B0Room1) {
    MapGrid_AnimateCells(gData_020043e4, 33, 7);
  } else {
    s32 index = work->value / 2 - 1;

    Func_02003320_(gData_02004410[index].data, gData_02004410[index].a,
                   gData_02004410[index].b);
  }
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, 0, -4);
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(work->value);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Func_020033a0();
}

#include "scene_tables.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_020033e0_(s32 id, s32 x, s32 z) {
  Actor_SetHorizontalPositionWithTerrain(id, x, z);
}

void Func_02001d68(void) {
  struct Actor *object = Actor_Get(9);

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(9, 2);
  object->y = -0x400000;
  Func_020033e0_(9, PIXELS(728), PIXELS(120));
  Actor_SetPosition(gMainRoutineWork.currentOwner, PIXELS(728), PIXELS(120));
  Actor_Get(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  Actor_WaitForMove(object);
  Event_SetFlag(0x300);
  Func_020033a0();
}

void Func_02001df4(void) {
  struct Actor *object = Actor_Get(11);

  Func_02003398();
  FieldEvent_ResolvePendingActions(0);
  object->motionFlags = 0;
  object->y = -0x400000;
  Actor_Get(gMainRoutineWork.currentOwner)->y = -0x300000;
  Event_StartScreenTransition();
  object->acceleration = 0x10000;
  object->speedLimit = 0x10000;
  FieldActor_SetMoveTarget(object, object->x, object->y + 0x300000, object->z);
  Actor_WaitForMove(object);
  Event_SetFlag(0x304);
  Func_020033a0();
}

#include "call.h"
s32 Func_02000038();
s32 Func_02000a78();
s32 Func_02002a74();
s32 Func_02003190();
s32 Func_02003490();
extern u8 gData_020036e4[];
extern u8 gData_020036ea[];
extern u8 gData_020036f6[];
extern u8 gData_0200384c[];
extern u8 gData_0200385c[];
extern u8 gData_02003888[];
extern u8 gData_020038b4[];

s32 Func_02001e64(void) {
  s32 v_r1, v_r6, v_r7, v_r2, v_r5, v_r3, v_r0, t0, t1, t3;
  s32 t6, t9, t12, t15, t27;
  s32 t30, t35, t36, t38, t42;
  s32 t59;
  s32 t60, t61, t64;
  s32 t78, t80;
  s32 t91, t92, t93, t94, t95, t96, t106;
  s32 t126;
  s32 t132, t136;
  s32 v_two;

  v_r2 = (s32)Ram_HeapSlots;
  v_r7 = 0x204;
  v_r6 = (s32)&gMainRoutineWork;
  FIELD_AT(FIELD_AT(v_r2, s32, 108), s32, 428) = v_r7;
  v_r1 = gMainRoutineWork.scene;
  if (v_r1 == (s32)&SceneId_Ov6B0Room1) {
    Call4((void (*)())Func_02000038, (s32)gData_0200384c, (s32)gData_0200385c,
          (s32)gData_02003888, (s32)gData_020038b4);
    t3 = Event_GetFlag(0xa87);
    if (t3 != 0) {
      Call3((void (*)())Func_02000a78, 0x208, 48, 0);
      MapGrid_CopyCellsTo(85, 34, 85, 8, 9, 8);
    }
    t6 = Event_GetFlag(0xa88);
    if (t6 != 0) {
      Call3((void (*)())Func_02000a78, 0x228, 48, 3);
      MapGrid_CopyCellsTo(101, 34, 101, 8, 9, 8);
    }
    t9 = Event_GetFlag(0xa89);
    if (t9 != 0) {
      Call3((void (*)())Func_02000a78, 0x1f8, 40, 10);
      MapGrid_CopyCellsTo(85, 46, 85, 20, 9, 8);
    }
    t12 = Event_GetFlag(0xa8a);
    if (t12 != 0) {
      Call3((void (*)())Func_02000a78, 0x238, 40, 2);
      MapGrid_CopyCellsTo(101, 46, 101, 20, 9, 8);
    }
    t15 = Event_GetFlag(0xa8b);
    if (t15 == 0)
      goto L_0200a6e6;
    MapGrid_CopyCellsTo(54, 7, 33, 7, 1, 2);
    Func_02003380(0x2180000, 0x880000, 0, 0);
    goto L_0200a6e6;
  }
  if (v_r1 != (s32)&SceneId_Ov6B0Room2)
    goto L_0200a1b4;
  FieldActor_SetSpritePriority(8, 1);
  if ((Event_GetFlag(0xa80)) != 0) {
    Actor_SetPosition(8, 0x1280000, 0xf80000);
    FieldActor_SetSpritePriority(8, 2);
  }
  if ((Event_GetFlag(0xa81)) != 0) {
    Actor_SetPosition(10, 0x1b80000, 0x2f80000);
  }
  Call1((void (*)())Func_02003490, (s32)gData_020036e4);
  FIELD_AT(((s32)Actor_Get((FIELD_AT(v_r6, s32, 532)))), u8, 35) |= 64;
  FIELD_AT(((s32)Actor_Get(9)), u8, 85) = 0;
  FIELD_AT(((s32)Actor_Get(9)), u8, 35) |= 128;
  if ((FIELD_AT(v_r6, s16, 482)) != 17)
    goto L_0200a03c;
  if ((Event_GetFlag(0x300)) == 0)
    goto L_0200a03c;
  Actor_SetAnimation(9, 2);
  Actor_SetPosition(9, 0x2d80000, 0x780000);
L_0200a03c:
  if (gMainRoutineWork.entrance == 18 && GameFlag_Test(0x300) == 0)
    Func_02001d68();
  {
    struct MainRoutineWork *work = &gMainRoutineWork;
    if (work->entrance != 10)
      goto L_0200a6e6;
    if ((Event_GetFlag(0x300)) != 0) {
      Actor_SetPosition(9, 0, 0);
    }
    if ((Event_GetFlag(0x207)) != 0) {
      t42 = (s32)Actor_Get(9);
      FIELD_AT(t42, u8, 85) = 4;
    }
    if ((Event_GetFlag(0x200)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 44, 86, 1, 1);
    }
    if ((Event_GetFlag(0x201)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 46, 86, 1, 1);
    }
    if ((Event_GetFlag(0x202)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 43, 87, 1, 1);
    }
    if ((Event_GetFlag(0x203)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 45, 87, 1, 1);
    }
    if ((Event_GetFlag(0x204)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 47, 87, 1, 1);
    }
    if ((Event_GetFlag(0x205)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 44, 88, 1, 1);
    }
    if ((Event_GetFlag(0x206)) != 0) {
      MapGrid_CopyCellsTo(54, 87, 46, 88, 1, 1);
    }
    FIELD_AT(((s32)Actor_Get((work->currentOwner))), u8, 35) |= 32;
  }
  goto L_0200a6e6;
L_0200a1b4:
  if (v_r1 != (s32)&SceneId_Ov6B0Room3)
    goto L_0200a2ce;
  t60 = (s32)Actor_Get(14);
  v_two = 2;
  v_r5 = 1;
  FIELD_AT(t60, u8, 85) = v_two;
  FIELD_AT(t60, s32, 20) = 0xfff20000;
  FIELD_AT(t60, s32, 12) = 0xfff20000;
  FIELD_AT(t60, u8, 35) |= 128;
  FIELD_AT(((s32)Actor_Get((FIELD_AT(v_r6, s32, 532)))), u8, 35) |= 64;
  FIELD_AT(((s32)Actor_Get(9)), u8, 98) = v_r5;
  if ((Event_GetFlag(0xa82)) != 0) {
    Actor_SetPosition(8, 0x380000, 0x1a80000);
  }
  if ((Event_GetFlag(0xa83)) != 0) {
    Actor_SetPosition(10, 0x2180000, 0x1680000);
    MapGrid_CopyCellsTo(51, 20, 31, 20, v_r5, v_two);
    Func_02003380(0x1f00000, 0x1500000, 0, 0);
  }
  if ((Event_GetFlag(0xa84)) != 0) {
    Actor_SetPosition(13, 0x2e80000, 0x1c80000);
  }
  if ((Event_GetFlag(0xa85)) == 0)
    goto L_0200a2c6;
  if ((Event_GetFlag(0x109)) != 0)
    goto L_0200a2b8;
  if ((Event_GetFlag(0xa84)) != 0)
    goto L_0200a2b8;
  t78 = FIELD_AT(((s32)Actor_Get(12)), s16, 10);
  t80 = FIELD_AT(((s32)Actor_Get(12)), s16, 18);
  v_r5 = (t78 << 16);
  Actor_SetPosition(13, v_r5, (t80 << 16));
L_0200a2b8:
  Actor_SetPosition(12, 0x2d80000, 0x1d80000);
L_0200a2c6:
  Call1((void (*)())Func_02003490, (s32)gData_020036ea);
  goto L_0200a6e6;
L_0200a2ce:
  if (v_r1 != (s32)&SceneId_Ov6B0Room4)
    goto L_0200a52c;
  if ((Event_GetFlag(0xa86)) != 0) {
    Actor_SetPosition(9, 0x1080000, 0x1680000);
  }
  Call1((void (*)())Func_02003490, (s32)gData_020036f6);
  v_r3 = (FIELD_AT(v_r6, u16, 482));
  v_r2 = (FIELD_AT(v_r6, s16, 482));
  if (v_r2 != 1)
    goto L_0200a4a2;
  Func_02002a74(33, 0x280, 0x380, 18, 68);
  v_r5 = (Event_GetFlag(0x304));
  if (v_r5 == 0) {
    t91 = FIELD_AT(v_r6, s32, 532);
    t92 = (s32)Actor_Get(t91);
    FIELD_AT(t92, u8, 35) |= 64;
    t94 = (s32)Actor_Get(8);
    FIELD_AT(t94, u8, 85) = v_r5;
    t95 = (s32)Actor_Get(8);
    FIELD_AT(t95, u8, 35) |= 128;
    goto L_0200a3a8;
  }
  Actor_SetPosition(8, 0, 0);
  Event_SetFlag(0x200);
  Event_SetFlag(0x201);
  Event_SetFlag(0x202);
  Event_SetFlag(0x203);
  Event_SetFlag(v_r7);
  Event_SetFlag(0x205);
  Event_SetFlag(0x206);
L_0200a3a8:
  if ((Event_GetFlag(0x207)) != 0) {
    t106 = (s32)Actor_Get(8);
    FIELD_AT(t106, u8, 85) = 4;
  }
  if ((Event_GetFlag(0x200)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 8, 69, 1, 1);
  }
  if ((Event_GetFlag(0x205)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 6, 71, 1, 1);
  }
  if ((Event_GetFlag(0x201)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 5, 75, 1, 1);
  }
  if ((Event_GetFlag(0x202)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 8, 75, 1, 1);
  }
  if ((Event_GetFlag(0x203)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 11, 75, 1, 1);
  }
  if ((Event_GetFlag(0x204)) != 0) {
    MapGrid_CopyCellsTo(17, 71, 8, 73, 1, 1);
  }
  if ((Event_GetFlag(0x206)) == 0)
    goto L_0200a6e6;
  MapGrid_CopyCellsTo(17, 71, 10, 71, 1, 1);
  goto L_0200a6e6;
L_0200a4a2:
  if (v_r2 == 11) {
    Func_02002a74(33, 0x280, 0x380, 18, 68);
    Func_02003190(22, 0, v_r7);
    goto L_0200a6e6;
  }
  if (v_r2 != 12)
    goto L_0200a4f4;
  Func_02002a74(35, 0x280, 0x380, 18, 68);
  if ((Event_GetFlag(0x109)) != 0)
    goto L_0200a6e6;
  Func_02003190(36, -0x24, v_r7);
  goto L_0200a6e6;
L_0200a4f4:
  if ((u32)((v_r3 - 4) << 16) > (u32)0x10000)
    goto L_0200a6e6;
  Func_02002a74(35, 0x280, 0x380, 18, 68);
  goto L_0200a6e6;
L_0200a52c:
  if (v_r1 != (s32)&SceneId_Ov6B0Room5)
    goto L_0200a6e6;
  t126 = FIELD_AT(v_r2, s32, 32);
  v_r5 = t126;
  v_r3 = ((FIELD_AT(v_r6, s16, 482)) - 1);
  if ((u32)v_r3 > (u32)11)
    goto L_0200a6e6;
  switch (v_r3) {
  case 0:
    goto L_0200a606;
  case 1:
    goto L_0200a606;
  case 2:
    goto L_0200a63a;
  case 3:
    goto L_0200a63a;
  case 4:
    goto L_0200a686;
  case 5:
    goto L_0200a686;
  case 6:
    goto L_0200a580;
  case 7:
    goto L_0200a6e6;
  case 8:
    goto L_0200a6e6;
  case 9:
    goto L_0200a6e6;
  case 10:
    goto L_0200a6e6;
  case 11:
    goto L_0200a596;
  }
L_0200a580:
  if ((Event_GetFlag(0x304)) != 0)
    goto L_0200a596;
  Actor_SetPosition(11, 0, 0);
L_0200a596:
  FIELD_AT(((s32)Actor_Get((gMainRoutineWork.currentOwner))), u8, 35) |= 64;
  Actor_SetAnimation(11, 2);
  FIELD_AT(((s32)Actor_Get(11)), u8, 85) = 0;
  FIELD_AT(((s32)Actor_Get(11)), u8, 35) |= 128;
  Actor_SetAnimation(11, 2);
  /* FAKEMATCH(dead): v_r6 set again to the address it already holds; without
   * it, 4 of 913 instructions differ in tla-en. */
  v_r6 = (s32)&gMainRoutineWork;
  if ((Event_GetFlag(0x304)) != 0)
    goto L_0200a5fa;
  if ((FIELD_AT(v_r6, s16, 482)) != 12)
    goto L_0200a5fa;
  Func_02001df4();
  goto L_0200a6e6;
L_0200a5fa:
  FIELD_AT(((s32)Actor_Get(11)), s32, 12) = 0xfff00000;
  goto L_0200a6e6;
L_0200a606:
  if ((Event_GetFlag(0x301)) != 0) {
    v_r2 = 240;
    v_r0 = 8;
    v_r1 = 0x780000;
    goto L_0200a650;
  }
  MapGrid_CopyCellsTo(18, 1, 11, 4, 1, 2);
  v_r1 = 176;
  v_r0 = 0xb80000;
  goto L_0200a67a;
L_0200a63a:
  if ((Event_GetFlag(0x302)) == 0)
    goto L_0200a660;
  v_r2 = 240;
  v_r0 = 9;
  v_r1 = 0x2280000;
L_0200a650:
  Actor_SetPosition(v_r0, v_r1, (v_r2 << 15));
  Event_SetFlag(0x200);
  goto L_0200a6e6;
L_0200a660:
  MapGrid_CopyCellsTo(18, 1, 26, 4, 1, 2);
  v_r1 = 176;
  v_r0 = 0x1a80000;
L_0200a67a:
  Func_02003380(v_r0, (v_r1 << 15), 0, 2);
  goto L_0200a6e6;
L_0200a686:
  if ((Event_GetFlag(0x303)) != 0) {
    Actor_SetPosition(10, 0x3180000, 0x780000);
    Event_SetFlag(0x200);
    goto L_0200a6d0;
  }
  MapGrid_CopyCellsTo(18, 1, 53, 4, 1, 2);
  Func_02003380(0x3580000, 0x580000, 0, 2);
L_0200a6d0:
  FIELD_AT(v_r5, s32, 236) = ((FIELD_AT(v_r5, s32, 236)) + 0x80000);
  FIELD_AT(v_r5, s32, 244) = ((FIELD_AT(v_r5, s32, 244)) + 0x80000);
L_0200a6e6:
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#include "../../field/object/set_sprite_priority_b.c"
