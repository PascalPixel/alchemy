#include "types.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "call.h"
#include "event_inline.h"
extern char SceneId_Ov6ACRoom6;
extern char SceneId_Ov6ACRoom1;

void Sound_WaitForBgmRestore(void);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);
void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f);
void Func_0200154c(u16 *slots, u16 *colours, u16 *a, u16 *b);
void Func_02002e6c(void);
void Func_020031e0(void);
void Func_020031e8(void);
void Func_02003228(void);
void Func_02003278(s32 a);
void Func_02003290(void);

extern u8 MsgYouHaveProven[];
extern u8 gData_0200338c[];
extern u8 gData_0200339c[];
extern u8 gData_020033c8[];
extern u8 gData_020033f4[];

s32 Func_02000b64(void) {
  s32 entrance;
  s32 scene;

  scene = gMainRoutineWork.scene;
  entrance = gMainRoutineWork.entrance;

  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x100;
  Event_SetFlag(0x112);
  Call4((void (*)())Func_0200154c, (s32)gData_0200338c, (s32)gData_0200339c,
        (s32)gData_020033c8, (s32)gData_020033f4);
  if (scene != (s32)&SceneId_Ov6ACRoom6 && scene == (s32)&SceneId_Ov6ACRoom1) {
    Call1((void (*)())Func_02003278, 0);
    *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 428) = 0x204;
    if ((u32)(entrance - 1) <= 1 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      s32 unset = Event_GetFlag(0xa31);

      if (unset == 0 && Event_GetFlag(0xa32) &&
          Event_GetFlag(0xa33) &&
          Event_GetFlag(0xa34) &&
          Event_GetFlag(0xa35)) {
        struct Actor *actor;

        Func_020031e0();
        FieldEvent_ResolvePendingActions(0);
        Event_StartScreenTransition();
        Event_WaitTransitionFrames();
        Event_PlaySound(78);
        FieldCamera_SetSpeed(0x20000, 0x4000);
        FieldCamera_MoveTo(0xe00000, 0x400000, 0xd80000, 1);
        Func_02003228();
        FieldEvent_Wait(20);
        PaletteFade_ApplySource(0x10000, 0);
        PaletteFade_ApplyTarget(0x10003, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        /* FAKEMATCH(temporary): entrance reused for the palette address, which
         * holds its register; as a plain store, 38 of 324 instructions differ
         * in tla-en. */
        entrance = 0x500021e;
        *(u16 *)entrance = 0x7fff;
        System_Wait(1);
        FieldMessage_ShowCentered((s32)MsgYouHaveProven, 0, 4);
        PaletteFade_ApplyTarget(0x10000, 0);
        PaletteFade_Interpolate(20);
        System_Wait(20);
        FieldCamera_SetSpeed(0x10000, 0x2000);
        FieldCamera_MoveTo(0xe00000, 0x100000, 0xd80000, 1);
        Func_02003228();
        FieldEvent_Wait(40);
        actor = Actor_Get(16);
        actor->motionFlags = unset;
        actor->y = 0x100000;
        actor->terrainHeight = 0x100000;
        ((u8 *)actor)[98] = 18;
        actor->unk66 = 0x4000;
        actor->watchedObject = (struct Actor *)unset;
        Func_02002e6c();
        System_Wait(40);
        MapGrid_SetShake(0x20000, 0x20000, 0x10000);
        Func_02001324(216, 216, 65, 65, 13, 13);
        Func_02001324(232, 216, 66, 65, 14, 13);
        Func_02001324(232, 232, 66, 66, 14, 14);
        Func_02001324(216, 200, 65, 64, 13, 12);
        Func_02001324(248, 200, 67, 64, 15, 12);
        Func_02001324(248, 232, 67, 66, 15, 14);
        Func_02001324(216, 232, 65, 66, 13, 14);
        Func_02001324(200, 216, 64, 65, 12, 13);
        Func_02001324(232, 200, 66, 64, 14, 12);
        Func_02001324(248, 216, 67, 65, 15, 13);
        Func_02001324(200, 232, 64, 66, 12, 14);
        Func_02001324(200, 200, 64, 64, 12, 12);
        MapGrid_SetShake(-1, -1, 0xe666);
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        FieldEvent_Wait(40);
        Event_SetFlag(0xa31);
        Event_PlaySound(80);
        Sound_WaitForBgmRestore();
        Func_02003290();
        Func_020031e8();
      }
    }
  }
  return 0;
}

#include "mars_lighthouse_room_m.h"

extern char SceneId_Ov6a8Room2;

void Func_020000c0(void);
void Func_020003c4(void);
s32 Func_02000058();
void Func_02000934(s32 entrance);
void Func_02002118(void);
void Func_02003150(s32 value);
void Func_020031b8(s32 scene, s32 a1);
void Func_02003270(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void Func_020032b0(s32 a);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void SetBlend(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g) {
  Func_02003270(a, b, c, d, e, f, g);
}

s32 Func_02000e84(void) {
  s16 scene = gMainRoutineWork.scene;
  s32 entrance = gMainRoutineWork.entrance;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov6ACRoom6 || scene == (s32)&SceneId_Ov6ACRoom2 ||
      scene == (s32)&SceneId_Ov6ACRoom3 || scene == (s32)&SceneId_Ov6ACRoom4 ||
      scene == (s32)&SceneId_Ov6ACRoom5) {
    Func_020032b0(1);
    SetBlend(1, 0x10000, 0x40000, 0x20000, 0x10000, 0x8000, 0x4000);
  }
  if (scene == (s32)&SceneId_Ov6ACRoom6) {
    switch (entrance) {
    case 1:
      Actor_SetSpriteColor(Actor_Get(8), 1);
      Func_02000058(9, 4);
      Func_02003150(4);
      Func_02003150(5);
      break;
    case 2:
      Actor_SetSpriteColor(Actor_Get(8), 3);
      Func_02000058(9, 5);
      Func_02003150(6);
      Func_02003150(7);
      break;
    case 3:
      Actor_SetSpriteColor(Actor_Get(8), 0);
      Func_02000058(9, 2);
      Func_02003150(0);
      Func_02003150(1);
      break;
    case 4:
      Actor_SetSpriteColor(Actor_Get(8), 4);
      Func_02000058(9, 3);
      Func_02003150(2);
      Func_02003150(3);
      break;
    }
    object = Actor_Get(10);
    object->motionFlags = 0;
    object->z = 0x680000;
    FieldTask_AddFrame(Func_020000c0, 0x480);
  } else if (scene == (s32)&SceneId_Ov6ACRoom1) {
    if (entrance == 1 || entrance == 2 || entrance == 8 || entrance == 10 ||
        entrance == 12 || entrance == 14) {
      Func_02002118();
      if (Event_GetFlag(0xa31)) {
        MapGrid_CopyCellAttributes(30, 12, 4, 3, 12, 12);
        MapGrid_CopyCellsLayered(64, 64, 4, 3, 76, 12);
      }
      Func_02000934(entrance);
      {
        s32 priority = 0x480;

        FieldTask_AddFrame(Func_020003c4, priority);
      }
      Func_02000058(12, 4);
      Func_02000058(13, 5);
      Func_02000058(14, 2);
      Func_02000058(15, 3);
      if (!Event_GetFlag(0xa32))
        Actor_SetPosition(12, 0, 0);
      if (!Event_GetFlag(0xa33))
        Actor_SetPosition(13, 0, 0);
      if (!Event_GetFlag(0xa34))
        Actor_SetPosition(14, 0, 0);
      if (!Event_GetFlag(0xa35))
        Actor_SetPosition(15, 0, 0);
    }
  }
  if (Event_GetFlag(0xa30)) {
    // A second name of the one scene id: the ROM keeps a literal for each
    // use, and with one name they share one.
    extern char SceneId_Ov6ACRoom3Second __asm__("SceneId_Ov6ACRoom3");

    Func_020031b8(gMainRoutineWork.map +
                      ((s32)&SceneId_Ov6ACRoom3Second - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  }
  return 0;
}

#include "version.h"
#include "scene_tables.h"
#include "io_reg.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_0200080c(void);

void Func_020010e4(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgMankindWithThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_02001154(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgFishWithCold, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_020011c4(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgDragonsBurningWith, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_02001234(void) {
  u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

  Func_020031e0();
  Event_ResolvePendingActions(0);
  /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
   * call; as a direct call, 8 of 37 instructions differ in tla-en. */
  PaletteFade_ApplySource(0x10000, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  *color = 0x7fff;
  wait(1);
  Func_0803ce64((s32)MsgBirdOnThe, 0, 0);
  BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
  BattleEffect_StartBufferInterpolation(20);
  wait(20);
  Func_020031e8();
}

void Func_020012a4(s32 x, s32 z, s32 kind) {
  struct Actor *object;

  /* FAKEMATCH(temporary): z shifted ahead of the call; as z << 16 in its
   * arguments, 2 of 53 instructions differ in tla-en. */
  z <<= 16;
  object = (struct Actor *)Import_ActorCreate(403, x << 16, 0, z);

  if (object != 0) {
    Actor_SetSingleChildFlags(object, 0);
    Func_02003118(object, 3);
    if (kind != 0) {
      ((struct MarsLighthouseRoomMAnimation *)object->animation)->layer = 2;
      Func_02003120(object, gData_0200387c);
    } else {
      ((struct MarsLighthouseRoomMAnimation *)object->animation)->layer = 1;
      object->velocityX = (((u32)(Random16Far() * 8) >> 16) - 4) << 16;
      object->velocityY = PIXELS(4);
      Func_02003120(object, gData_02003900);
    }
  }
}

void Func_02001324(s32 x, s32 z, s32 c, s32 d, s32 e, s32 f) {
  u32 i;
  s32 px;
  s32 pz;

  Sound_Play(144);
  Func_020012a4(x, z + 12, 1);
  for (i = 0; i < 3; i++) {
    px = x + ((u32)(Random16Far() * 12) >> 16) - 6;
    pz = z + ((u32)(Random16Far() * 12) >> 16) - 6;
    Func_020012a4(px, pz, 0);
  }
  wait(4);
  Map_CopyCellPictureIndicesRect(c, d, e + 64, f, 1, 1);
}

/// No one holding item 222: MsgIfYouDo, once (flag 0xa2f). Someone holding it
/// (and flag 0xa30 set, from patch 1): MsgYouWhoHold and the
/// message after it, the camera moves on and Func_0200080c runs, then flag
/// 0xa2e is set. Each opens as Func_020010e4 does.
void Func_020013a0(void) {
  if (PartyInventory_FindOwner(222) == -1) {
    if (!Event_GetFlag(0xa2f)) {
      u16 *color = (volatile u16 *)&OBJ_PLTT_COLOR(0, 15);

      Func_020031e0();
      Event_ResolvePendingActions(0);
      FieldCamera_SetSpeed(0x20000, 0x4000);
      FieldCamera_MoveTo(PIXELS(224), PIXELS(64), PIXELS(216), 1);
      Func_02003228();
      FieldEvent_Wait(20);
      /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
       * call; as a direct call, 12 of 155 instructions differ in tla-en. */
      PaletteFade_ApplySource(0x10000, 0);
      BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
      BattleEffect_StartBufferInterpolation(20);
      wait(20);
      *color = 0x7fff;
      wait(1);
      Func_0803ce64((s32)MsgIfYouDo, 0, 4);
      BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
      BattleEffect_StartBufferInterpolation(20);
      wait(20);
      Event_SetFlag(0xa2f);
      Func_020031e8();
    }
  }
#if !TLA_PATCH_1
  else {
#else
  else if (Event_GetFlag(0xa30)) {
#endif
    u16 *color = (u16 *)&OBJ_PLTT_COLOR(0, 15);

    Func_020031e0();
    Event_ResolvePendingActions(0);
    FieldCamera_SetSpeed(0x20000, 0x4000);
    /* FAKEMATCH(wrapper): forwarded so this branch builds its own positions;
     * as a direct call, 16 of 155 instructions differ in tla-en. */
    FieldCamera_MoveTo(PIXELS(224), PIXELS(64), PIXELS(216), 1);
    Func_02003228();
    FieldEvent_Wait(20);
    /* FAKEMATCH(wrapper): forwarded so 0x10000 is built again for the next
     * call, as in the branch above; as a direct call at this site, 22 of 155
     * instructions differ in tla-en. */
    PaletteFade_ApplySource(0x10000, 0);
    BattleEffect_ApplyColorToTargetBuffer(0x10003, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(20);
    *color = 0x7fff;
    wait(1);
    Func_0803ce64((s32)MsgYouWhoHold, 0, 4);
    wait(10);
    Func_0803ce64((s32)MsgYouWhoHold + 1, 0, 3);
    FieldCamera_MoveTo(PIXELS(224), PIXELS(88), PIXELS(216), 1);
    Func_02003228();
    FieldEvent_Wait(20);
    Sound_Play(560);
    Func_0200080c();
    FieldEvent_Wait(20);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(20);
    wait(40);
    Event_SetFlag(0xa2e);
    Func_020031e8();
  }
}

#include "dma.h"

extern u16 *Data_0200392c;
extern u16 *Data_02003930;
extern u16 *Data_0200391c;

void Func_0200183c(void);

/// Records the four palette lists, writes each listed palette entry from its
/// red, green and blue, then copies two palettes from block32 and starts the
/// blend.
void Func_0200154c(u16 *slots, u16 *colours, u16 *a, u16 *b) {
  u8 *saved = Ram_HeapSlots->block32;

  gData_02003928 = slots;
  Data_0200392c = colours;
  Data_02003930 = a;
  Data_0200391c = b;
  if (*slots != 0xffff && *colours != 0xffff) {
  next:
    {
      int red = *colours++;
      int index = *slots++;
      int green = *colours++;
      int blue = *colours++;
      int value;

      /* FAKEMATCH(dead): the colour's build and store in a once-only
       * block, so both lists are stepped right after their loads and the
       * next entry's marker loads before 0xffff is built; as plain
       * statements, 22 of 80 instructions differ in tla-en. */
      do {
        value = (blue << 10) | (green << 5) | red;
        ((u16 *)BG_PLTT)[index] = value;
      } while (0);
    }
    if (*slots != 0xffff && *colours != 0xffff)
      goto next;
  }
  Dma_Set((const void *)BG_PLTT, saved, DMA_COPY_WORDS(112), REG_DMA3);
  Dma_Set((const void *)OBJ_PLTT, saved + 448, DMA_COPY_WORDS(112), REG_DMA3);
  PaletteFade_ApplyTarget(0x10000, 0);
  if ((s8)gMainRoutineWork.unk2c2 == 2)
    Func_0200183c();
}

void Func_02001608(void) {
  gData_02003938 = 0;
  gData_02003934 = Func_020016e8(gData_02003928);
  gData_02003924 = 15;
  gData_02003920 = 16;
  gData_02003918 = 1;
  Task_AddFrame(Func_0200170c, 0x480);
}
