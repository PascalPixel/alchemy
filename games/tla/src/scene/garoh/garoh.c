/// @file
/// Scene code of overlay 0x65f, in ROM order.

#include "../garoh.h"
#include "event_inline.h"
#include "map.h"
#include "field.h"
#include "module/field/event_commands.h"
#include "module/scene/garoh.h"
#include "module/scene/scene.h"
#include "scene_ids.h"
#include "text/msg_ids.h"

void Func_02000280(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct MapLayer *to = &map->layers[1];
  struct MapLayer *from = &map->layers[0];

  to->unk10 = from->unk10;
  to->unk14 = from->unk14;
  to->unk28 = from->unk28;
  to->unk2a = from->unk2a;
}

#include "types.h"
#include "flags.h"
#include "event_work.h"
#include "ram_buffer.h"
#include "dma.h"
#include "io_reg.h"
#include "palette_buffers.h"
#include "frame.h"
#include "field.h"

void Func_020002a4(void) {
  // Declared first: x then wins its allocation tie with sine.
  s32 x;
  // Math_Sine is an 8-byte far-call veneer: its target address is the word
  // after its first instruction.
  s32 (*sine)(s32) = *(s32 (**)(s32))(((u32)Math_Sine & ~3) + 4);
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  /* FAKEMATCH(pin): line held in r5; as a plain local, 70 of 110 instructions
   * differ in tla-en. */
  register u16 *line asm("r5") = Data_02006340;

  if (((struct EventWork *)Ram_HeapSlots->eventWork)->pendingFirst == 0) {
    u32 frame;
    s32 z, a, b;
    s32 stepA, stepB;
    s32 i;
    volatile u16 *dma;

    *(s32 *)&layer->unk08[20] = 0;
    frame = *(u8 *)&gFrameCount;
    i = 0;
    /* FAKEMATCH(barrier): asm("" : "+r"(i)) keeps the loop's top test; as i = 0
     * alone, 48 of 110 instructions differ in tla-en. */
    asm("" : "+r"(i));
    z = *(s16 *)((u8 *)layer + 6);
    x = *(s16 *)((u8 *)layer + 2);
    // Each step held in a local for its whole life: as z * 0x3300 and
    // z * 0x1320 the starting phases are built differently.
    stepA = 0x3300;
    stepB = 0x1320;
    a = z * stepA + (frame << 10);
    b = z * stepB - (frame << 10);
    for (; i != 160; i++) {
      /* FAKEMATCH(pin): s1 held in r8; as a plain local, 57 of 110 instructions
       * differ in tla-en. */
      register s32 s1 asm("r8") = sine(a);
      s32 s2 = sine(b);
      *line++ = ((s1 * 3 + s2 * 7) >> 19) + x;
      a += 0x3300;
      b += 0x1320;
    }
    line = Data_02006340;
    REG_BG2HOFS = line[0];
    dma = REG_DMA0;
    dma[5] &= 0xc5ff;
    dma[5] &= 0x7fff;
    (void)dma[5];
    Dma_Set(line + 1, (void *)&REG_BG2HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
            (volatile u32 *)dma);
  }
}

extern const u16 gData_020054ee[];
extern const u16 gData_020054b8[];

// Build BG palette 1 colors 8-15 from the table.
#define DEFINE_GAROH_LOAD_PALETTE(name, data1, n1, n2)                         \
  void name(void) {                                                            \
    struct BattleEffectBuffers *buffers = Ram_HeapSlots->block32;              \
    const u8 *table = (const u8 *)data1;                                       \
    u32 i = 0;                                                                 \
    u16 *dst = &buffers->palette[n1];                                          \
    s32 blue = 4;                                                              \
    s32 red = 0;                                                               \
    volatile u16 *color;                                                       \
    s32 green;                                                                 \
                                                                               \
  loop:                                                                        \
    color = BG_PLTT + n1 + i;                                                  \
    green = blue - 2;                                                          \
    *color = *(const u16 *)(table + blue) << 10 |                              \
             *(const u16 *)(table + green) << 5 | *(const u16 *)(table + red); \
    *dst++ = *color;                                                           \
    i++;                                                                       \
    blue += 6;                                                                 \
    red += 6;                                                                  \
    if (i < n2)                                                                \
      goto loop;                                                               \
  }

DEFINE_GAROH_LOAD_PALETTE(Func_02000390, gData_020054ee, 24, 8)

// Build BG palette 6 colors 1-9 from the table.
DEFINE_GAROH_LOAD_PALETTE(Func_020003d8, gData_020054b8, 97, 9)

s32 Func_02000420(struct Actor *object) {
  Actor_SetSingleChildFlags(object, 0);
  return 0;
}

#include "battle.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "scene_tables.h"

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "main_routine_work.h"
#include "event_work.h"

extern u32 gData_020056dc[];
extern u32 gData_020056f4[];
extern u32 gData_020059dc[];
extern s32 gData_02005a54[];
extern u32 gData_02005b5c[];
extern s32 gData_02005c1c[];

extern const struct GarohPicture gData_02005de0[];

/// Return the table for the current room.
const void *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65FRoom1) {
    if (Flag_IsSet(FLAG_GAROH_SECRET_SHARED))
      return gData_02005874;
    return gData_020056f4;
  }
  if (scene == (s32)&SceneId_Ov65FRoom2)
    return gData_02005b5c;
  if (scene == (s32)&SceneId_Ov65FRoom3)
    return gData_020059dc;
  if (scene == (s32)&SceneId_Ov65FRoom4)
    return gData_02005a54;
  if (scene == (s32)&SceneId_Ov65FRoom5)
    return gData_02005c1c;
  return gData_020056dc;
}

/// 6A6's Func_02000e54 for this overlay: clears the motion of every object
/// from 8 to 63, shows the picture the event work's index names and moves the
/// party's current owner to it.
void Func_020004cc(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  s32 index;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  // A goto loop: the bytes rebuild the zero on every pass, which a for
  // loop hoists (differ 12).
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
  index = *(s16 *)(work + 0x154);
  Event_PlaySound(158);
  index--;
  MapGrid_AnimateCells(gData_02005de0[index].data, gData_02005de0[index].a,
                       gData_02005de0[index].b);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 2);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  {
    s32 owner = gMainRoutineWork.currentOwner;

    if (index == 5)
      Actor_SetDestinationOffset(owner, 0, -4);
    else
      Actor_CenterAndWalk(owner, 2, -4);
  }
  FieldEvent_Wait(6);
  FieldEvent_SetExitRequest(*(s16 *)(work + 0x154));
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

void Func_02000580(s32 actor) {
  s32 msg = (s32)MsgGaroh02;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_020005cc(s32 actor) {
  s32 msg = (s32)MsgGaroh06;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000618(s32 actor) {
  s32 msg = (s32)MsgGaroh18;

  FieldEvent_SetMessageId(msg);
  Event_OpenMessage(actor, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(FieldParty_GetLeaderActor(), 0) ==
      0) {
    FieldEvent_Wait(10);
    FieldEvent_SetMessageId(msg + 1);
  } else {
    FieldEvent_Wait(20);
    FieldEvent_SetMessageId(msg + 2);
  }
  Event_ShowMessage(actor, 0);
}

void Func_02000664(s32 actor) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_SetMessageId((s32)MsgGaroh17);
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Event_End();
}

void Func_02000698(s32 actor) {
  s32 flag;
  s32 msg;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  flag = Event_GetFlag(0x300);
  msg = (s32)MsgGaroh04;
  if (flag) {
    FieldEvent_SetMessageId(msg);
  } else {
    FieldEvent_SetMessageId(msg);
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
    FieldEvent_ShowMessageThenWait(actor, 0, 10);
  }
  FieldEvent_ShowMessageThenWait(actor, 0, 10);
  Event_End();
}

void Func_020006ec(void) {
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldActor_Stop(13);
  Actor_SetAnimation(13, 1);
  FieldEvent_Wait(10);
  Actor_FaceActor(13, 4, 0);
  if (Event_GetFlag(0x301)) {
    FieldEvent_SetMessageId((s32)MsgGaroh21);
  } else {
    FieldEvent_SetMessageId((s32)MsgGaroh20);
    FieldEvent_ShowMessageThenWait(13, 0, 10);
    {
      s32 a = 0x101;
      s32 b = 15;

      FieldActor_ShowEmote(4, a, b);
    }
    {
      s32 a = 0x106;
      s32 b = 10;

      FieldActor_ShowEmote(13, a, b);
    }
    FieldEvent_ShowMessageThenWait(13, 0, 5);
    {
      s32 a = 0x108;
      s32 b = 15;

      FieldActor_ShowEmote(13, a, b);
    }
    FieldEvent_ShowMessageThenWait(13, 0, 5);
  }
  FieldEvent_ShowMessageThenWait(13, 0, 10);
  Event_SetFlag(0x301);
  Actor_SetScript(13, 2);
  Event_End();
}

void Func_02000808(void) {
  s32 flag;
  s32 speed;

  void Func_020007a0(s32 id) {
    BattleEffect_LoadActionEffectResources(144, 1);
    FieldPsynergy_SetupActorPair(id, 0);
    FieldPsynergy_RunHandler(1);
    BattleEffect_StopMainActor();
    BattleEffect_CleanupSceneObjects();
  }

  void Func_020007d4(s32 id) {
    BattleEffect_LoadActionEffectResources(144, 1);
    FieldPsynergy_SetupActorPair(id, 0);
    FieldPsynergy_RunHandler(3);
    BattleEffect_StopMainActor();
    BattleEffect_CleanupSceneObjects();
  }

  flag = Event_GetFlag(0x320);
  if (flag == 0) {
    Event_SetFlag(0x320);
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Actor_SetSingleChildFlags(Actor_Get(22), 0);
    speed = 0x4000;
    Actor_SetPositionAndFacing(22, PIXELS(360), PIXELS(199), speed);
    FieldCamera_MoveTo(PIXELS(359), -1, PIXELS(220), 1);
    FieldCamera_WaitForArrival();
    Func_020007a0(22);
    FieldEvent_Wait(30);
    Actor_SetPositionAndFacing(21, PIXELS(360), PIXELS(199), speed);
    Actor_WalkByAndWait(21, 0, 16);
    Func_020007d4(22);
    FieldEvent_SetMessageId((s32)MsgGaroh12);
    Actor_FaceDirectionAndWait(21, 160 << 7);
    Actor_RunRepeatedMotion(21, 2);
    FieldEvent_Wait(20);
    Actor_FaceDirection(4, 224 << 8, 0);
    FieldEvent_ShowMessageThenWait(21, 0, 10);
    FieldActor_ShowEmote(21, 0x102, 50);
    Actor_Get(21)->actionFlags &= ~1;
    Actor_FaceDirection(21, speed, 0);
    FieldEvent_Wait(30);
    Func_020007a0(22);
    FieldEvent_Wait(20);
    Actor_WalkByAndWait(21, 0, -16);
    Actor_Get(21)->motionFlags = flag;
    Actor_WalkByAndWait(21, 0, -16);
    Actor_SetPosition(21, 0, 0);
    FieldEvent_Wait(10);
    Func_020007d4(22);
    FieldEvent_Wait(60);
    Actor_SetPosition(22, 0, 0);
    Event_End();
  }
}

extern s32 gData_02005e10[];
extern s32 gData_02005e34[];
extern s32 gData_02005f78[];
extern s32 gData_020060d4[];
extern s32 gData_020061d0[];
extern s32 gData_02006278[];

extern s32 gData_0200632c;

void Func_02000960(void) {
  struct EventWork *event = Ram_HeapSlots->eventWork;

  Event_PlaySound(123);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 521;
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Flag_Set(0x121);
  FieldEvent_SetExitRequest(event->exit);
}

void Func_020009a4(void) {
  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED)) {
    if (!Flag_IsSet(0x91e))
      Func_02002694();
  } else {
    if (!Flag_IsSet(0x91f))
      Func_02003878();
  }
}

/// Return the table for the current room.
const void *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65FRoom1) {
    if (Flag_IsSet(FLAG_GAROH_SECRET_SHARED))
      return gData_02005f78;
    return gData_02005e34;
  }
  if (scene == (s32)&SceneId_Ov65FRoom2)
    return gData_020060d4;
  if (scene == (s32)&SceneId_Ov65FRoom3)
    return gData_0200611c;
  if (scene == (s32)&SceneId_Ov65FRoom4)
    return gData_020061d0;
  if (scene == (s32)&SceneId_Ov65FRoom5)
    return gData_02006278;
  return gData_02005e10;
}

void Func_02000a68(void) {
  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED) && Flag_IsSet(FLAG_GAROH_NIGHT)) {
    gData_0200632c++;
    if ((gData_0200632c & 0x3ff) == 0)
      Event_PlaySound(0x20d);
  }
}

void Func_02000d30(void);

s32 Scene_Initialize(void) {
  struct EventWork *work;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom4) {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
  } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom5) {
    FieldActor_SetSpritePriority(13, 1);
    FieldActor_SetSpritePriority(14, 1);
    if (Event_GetFlag(0x91f)) {
      s32 angle = 0x4000;

      Actor_SetPositionAndFacing(10, PIXELS(328), PIXELS(162), angle);
    }
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
    Func_020052f4(0);
    if (gMainRoutineWork.entrance == 6) {
      work = Ram_HeapSlots->eventWork;
      work->screenEffect = 0x209;
      Event_ClearFlag(0x120);
      Event_ClearFlag(0x121);
      ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x204;
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
    }
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
    if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom1) {
      Event_SetFlag(0x919);
      Func_02000be4();
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom2) {
      Func_02000d30();
    } else if (gMainRoutineWork.scene == (s32)&SceneId_Ov65FRoom3) {
      Func_02000e00();
      if (Event_GetFlag(0x919)) {
        gMainRoutineWork.savedScene = (s32)&SceneId_Ov660Room3;
        gMainRoutineWork.savedEntrance = 2;
      }
    }
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000be4(void) {
  FieldTask_AddFrame(Func_02000a68, 0x480);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  if (Event_GetFlag(FLAG_GAROH_SECRET_SHARED) == 0 ||
      Event_GetFlag(FLAG_GAROH_NIGHT) != 0) {
    Event_SetFlag(FLAG_GAROH_NIGHT);
    PaletteFade_ApplyTarget(0x10007, 1);
    if (Event_GetFlag(0x311))
      PaletteFade_ApplyTarget(0x10003, 2);
    PaletteFade_Interpolate(1);
    Event_WaitTransitionFrames();
    Func_0200538c();
    {
      u8 *palette = *(u8 **)((u8 *)Ram_HeapSlots + 0x80);

      *(volatile u16 *)&BG_PLTT_COLOR(1, 14) = 0x7fff;
      *(u16 *)(palette + 60) = *(volatile u16 *)&BG_PLTT_COLOR(1, 14);
    }
    PaletteFade_ApplyTarget(0x10000, 1);
  } else {
    MapGrid_CopyCellsTo(10, 32, 10, 10, 1, 1);
    MapGrid_CopyCellsTo(12, 32, 25, 22, 1, 1);
    MapGrid_CopyCellsTo(11, 33, 8, 22, 1, 1);
    MapGrid_CopyCellsTo(74, 32, 74, 10, 1, 1);
    MapGrid_CopyCellsTo(76, 32, 89, 22, 1, 1);
    MapGrid_CopyCellsTo(75, 33, 72, 22, 1, 1);
  }
  if (gMainRoutineWork.entrance == 12) {
    struct EventWork *work = Ram_HeapSlots->eventWork;

    work->screenEffect = 0x209;
    Event_ClearFlag(0x120);
    Event_ClearFlag(0x121);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0;
  }
}

void Func_02000d30(void) {
  s32 t4;

  Task_AddFrame(Func_02000a68, 0x480);
  Func_0200535c();
  Func_02005364(0, 10, 11);
  t4 = Event_GetFlag(FLAG_GAROH_NIGHT);
  if (t4 == 0)
    goto L_02008da6;
  if (gMainRoutineWork.entrance != 12)
    goto L_02008d72;
  PaletteFade_ApplyTarget(0x2048c9, 1);
  goto L_02008d7e;
L_02008d72:
  PaletteFade_ApplyTarget(0x10007, 1);
L_02008d7e:
  PaletteFade_Interpolate(1);
  Event_WaitTransitionFrames();
  Func_0200538c();
  Func_020003d8();
  PaletteFade_ApplyTarget(0x10000, 1);
  Task_AddFrame((s32)Func_020002a4, 0xc80);
  goto L_02008dea;
L_02008da6:
  MapGrid_CopyCellsTo(20, 7, 9, 7, 5, 5);
  MapGrid_CopyCellsTo(20, 71, 9, 71, 5, 5);
  MapGrid_CopyCellsTo(84, 7, 73, 7, 5, 5);
  Func_02000390();
  PaletteFade_ApplyTarget(0x10000, 1);
  Func_02000280();
L_02008dea:
}

extern u8 gData_02005450[];

void Func_02000e00(void) {
  FieldTask_AddFrame(Func_02000a68, 1152);
  if (Flag_IsSet(FLAG_GAROH_NIGHT)) {
    if (Flag_IsSet(0x318))
      PaletteFade_ApplyTarget(0x10003, 1);
    else
      PaletteFade_ApplyTarget(0x10007, 1);
  }
  Func_0200536c(gData_02005450);
  PaletteFade_Interpolate(1);
  Event_WaitTransitionFrames();
}

void Func_02000e58(void) { Func_02005374(gData_02005450); }

void Func_02000e68(void) { Func_02005374(gData_02005450); }

void Func_02000e78(s32 id, struct Actor *object) {
  s32 i;
  s32 saved;

  object->motionFlags = 0;
  saved = id;
  for (i = 16; i > 0; i--) {
    object->y -= PIXELS(2);
    FieldEvent_Wait(1);
  }
  Event_PlaySound(136);
  Func_020012a8(saved);
}

#define DEFINE_GAROH_TRIGGER_AT_CELL(name, n1, n2, n3, n4)                     \
  void name(void) {                                                            \
    struct Actor *object = Actor_Get(n1);                                      \
    s32 x = object->x >> 20;                                                   \
    s32 raw;                                                                   \
    s32 z;                                                                     \
                                                                               \
    raw = object->z;                                                           \
    z = raw >> 20;                                                             \
    if (!Flag_IsSet(n2) && x == n3 && z == n4) {                               \
      Func_02000e78(n1, object);                                               \
      Flag_Set(n2);                                                            \
    }                                                                          \
  }

DEFINE_GAROH_TRIGGER_AT_CELL(Func_02000eb0, 8, 0x201, 10, 29)

DEFINE_GAROH_TRIGGER_AT_CELL(Func_02000eec, 9, 0x202, 15, 28)

void Func_02000f28(void) {
  Actor_Get(10);
  Flag_IsSet(0x203);
}

void Func_02000f3c(void) {
  struct Actor *object = Actor_Get(11);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (!Flag_IsSet(0x204) && x == 15 && z == 25) {
    Func_02000e78(11, object);
    Flag_Set(0x204);
  }
}

void Func_02000f74(void) {
  Func_0200537c();
  Func_02000eb0();
}

void Func_02000f80(void) {
  Func_0200537c();
  Func_02000eec();
}

void Func_02000f8c(void) {
  Func_0200537c();
  Func_02000f28();
}

void Func_02000f98(void) {
  Func_0200537c();
  Func_02000f3c();
}

void Func_02000fa4(void) {
  Func_0200537c();
  Func_02000eb0();
  Func_02000eec();
  Func_02000f28();
  Func_02000f3c();
}

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void CopyRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  MapGrid_CopyCellPictureIndicesRect(a0, a1, a2, a3, a4, a5);
}

static inline void BlendRect(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5) {
  Map_CopyCellAttributeRect(a0, a1, a2, a3, a4, a5);
}

void Func_02000fbc(void) {
  CopyRect(98, 8, 86, 8, 1, 2);
  BlendRect(34, 9, 1, 1, 22, 11);
}

void Func_02000fec(void) {
  CopyRect(25, 47, 11, 33, 1, 3);
  BlendRect(25, 47, 1, 1, 11, 33);
}

void Func_0200101c(void) {
  MapGrid_CopyCellsTo(44, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(108, 5, 83, 5, 3, 2);
  BlendRect(44, 5, 3, 3, 19, 5);
}

void Func_0200105c(void) {
  CopyRect(102, 8, 86, 8, 1, 2);
  BlendRect(22, 10, 1, 1, 22, 11);
}

s32 Func_0200108c(void) { return Flag_IsSet(FLAG_GAROH_NIGHT); }

void Func_02001098(void) {
  if (Flag_IsSet(FLAG_GAROH_NIGHT)) {
    if (Flag_IsSet(0x318))
      PaletteFade_ApplyTarget(0x10003, 1);
    else
      PaletteFade_ApplyTarget(0x10007, 1);
    PaletteFade_Interpolate(16);
  }
}

void Func_020010d4(void) {
  CopyRect(8, 30, 11, 33, 1, 3);
  BlendRect(8, 30, 1, 1, 11, 33);
}

void Func_02001104(void) {
  MapGrid_CopyCellsTo(39, 5, 19, 5, 3, 3);
  MapGrid_CopyCellsTo(103, 5, 83, 5, 3, 2);
  BlendRect(39, 5, 3, 3, 19, 5);
}

void Func_02001144(void) {
  struct GarohEventWork *work;

  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED)) {
    Flag_Set(0x210);
    if (gMainRoutineWork.entrance == 1) {
      PaletteFade_ApplyTarget(0x10003, 1);
      Flag_Set(FLAG_GAROH_NIGHT);
      Flag_Set(0x318);
    } else {
      PaletteFade_ApplyTarget(0x10000, 1);
      Flag_Clear(FLAG_GAROH_NIGHT);
      Flag_Clear(0x318);
    }
    work = Ram_HeapSlots->eventWork;
    if (work->fadeLock == 0)
      PaletteFade_Interpolate(120);
  }
}

void Func_020011c8(void) {
  struct GarohEventWork *work;

  if (!Flag_IsSet(FLAG_GAROH_SECRET_SHARED)) {
    Flag_Set(0x211);
    if (gMainRoutineWork.entrance == 2) {
      PaletteFade_ApplyTarget(0x10003, 1);
      Flag_Set(FLAG_GAROH_NIGHT);
      Flag_Set(0x318);
    } else {
      PaletteFade_ApplyTarget(0x10007, 1);
      Flag_Set(FLAG_GAROH_NIGHT);
      Flag_Clear(0x318);
    }
    work = Ram_HeapSlots->eventWork;
    if (work->fadeLock == 0)
      PaletteFade_Interpolate(120);
  }
}
