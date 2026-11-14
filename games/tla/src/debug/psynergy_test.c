/// @file
/// Overlay 0x6b8's scene: its code up to Func_02000c4c, then its tables, in ROM
/// order.

#include "battle/battle.h"
#include "flags.h"
#include "sounds.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "system/input.h"
#include "system/main_routine_work.h"
#include "text/msg_ids.h"
#include "field/event_inline.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "call.h"
#include "event_work.h"
#include "fixed_math.h"
#include "system/vector3.h"
#include "inventory.h"
#include "graphics/uiflow.h"
#include "macros.h"

struct Actor;
struct SparkleSprite;

extern s16 gData_02001300[];

void ItemPicker_RunToDrop(s32 *, s32 *);

void Inn_CheckIn(s32, s32);

void Func_0200092c(s32, s32, s32, s32, s32, s32, s32);
void Func_02000950(s32, s32, s32, s32);
void Func_020009c0(s32, s32, s32, s32);
void Func_02000a10(s32, s32, s32, s32, s32);
void Func_02000d4c(struct Actor *);

void Actor_FallOverWithAnimationById(s32 actor, s32 angle, s32 frames);

void Actor_ArriveInRing(s32 actor);

void EffectSprite_Draw(struct SparkleSprite *sprite);

extern s32 gData_02000f44[];

s32 BattleEffect_GetObjectPriority(struct Actor *object);

#include "edition.h"
#include "item_icon.h"

struct Actor;

struct Vector3;

void Event_SetDestination(s32 scene, s32 entrance);

extern u8 gData_02001860[];

void Field_RequestBattleFromEncounterTable(s32, s32);

void Event_PickUpItem(s32, s32, s32);

void Camera_WorldToScreen(struct Vector3 *v);

s32 BattleEffect_GetObjectDepth(struct Actor *object);

#include "module/game/game.h"
#include "module/tla.h"
#include "module/window/window.h"

void NameEntry_EditMemberName(s32);
void Shop_Run(s32, s32);
void TileCopies_CopyTouchedCells(s32, s32, s32, s32, s32, s32, s32);
void Message_OpenWindow(s32, s32, s32, s32);

#define INPUT_HELD (*(volatile u32 *)&gInput.held)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void *Func_0200004c(void) { return gData_02001300; }

void Test_ReceiveFlint(void) {}

void Func_02000058(void) {
  s32 pos[2];

  ItemPicker_RunToDrop(&pos[1], &pos[0]);
}

void Test_ReceiveFizz(void) { NameEntry_EditMemberName(0); }

void Func_02000074(void) {}

void MenuTest_ShowMessageAndWait(s32 message) {
  Message_CloseFinishedWindows();
  Message_OpenWindow(message, 5, 0, 0x22);
  while (Message_AllTyped() == 0)
    System_Wait(1);
  System_Wait(1);
}

void MenuTest_BrowseMessageRange(s32 message, s32 count) {
  s32 i;
  void *window;

  window = Portrait_OpenWindow(206, 0, 0, 0);
  for (i = 0; i < count; i++) {
    MessageArgument_Push(1, 1);
    MessageArgument_Push(5, 2);
    MessageArgument_Push(0x1e240, 5);
    MenuTest_ShowMessageAndWait(message);
    goto check;
  wait:
    if (INPUT_HELD != 0)
      goto next;
    System_Wait(1);
  check:
    if (INPUT_HELD & 2)
      goto done;
    if ((INPUT_HELD & 1) || (INPUT_HELD & 0x80)) {
      message++;
      goto next;
    }
    if (!(INPUT_HELD & 0x40))
      goto wait;
    message--;
  next:;
  }
done:
  Message_CloseFinishedWindows();
  Window_Close(window, 2);
}

void Func_02000140(void) {
  MenuTest_BrowseMessageRange((s32)MsgWeaponShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000158(void) {
  MenuTest_BrowseMessageRange((s32)MsgArmorShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000170(void) {
  MenuTest_BrowseMessageRange((s32)MsgItemShopWelcome,
                              MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_0200018c(void) {
  MenuTest_BrowseMessageRange((s32)MsgWelcomeWearyWanderers,
                              MsgRightWhenI - MsgWelcomeWearyWanderers);
}

void Func_020001a4(s32 value) { Shop_Run(1, value); }

void Func_020001b0(s32 value) { Shop_Run(2, value); }

void Func_020001bc(s32 value) { Shop_Run(3, value); }

void Func_020001c8(s32 value) { Shop_Run(24, value); }

void Func_020001d4(s32 value) { Inn_CheckIn(0, value); }

void Func_020001e0(void) {}

void Func_020001e4(void) {
  Message_ShowCentered((s32)MsgYouWentUp, 1);
  Unit_LevelUpTo(4, ((u8 *)Unit_Get(4))[15] + 10);
  Unit_LevelUpTo(5, ((u8 *)Unit_Get(5))[15] + 10);
  Unit_LevelUpTo(6, ((u8 *)Unit_Get(6))[15] + 10);
}

void Func_02000224(void) {
  s32 i;

  Message_ShowCentered((s32)MsgYouGotTreasure, 1);
  gMainRoutineWork.coins = 777777;
  for (i = 9; i >= 0; i--) {
    Inventory_AddItem(4, 228);
    Inventory_AddItem(4, 229);
  }
  Unit_Calculate(4);
  Unit_Calculate(5);
  Unit_Calculate(6);
}

static __inline__ void Unit_AddHpBy(s32 owner, s32 delta) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline. */
  Unit_AddHp(owner, delta);
}

void Func_0200026c(void) {
  u8 *state;

  Message_ShowCentered((s32)MsgTheBodyIs, 1);
  Unit_AddHpBy(4, -100);
  FieldUnit_AddHp(5, -100);
  FieldUnit_AddHp(6, -33);
  Unit_AddPp(4, -50);
  Unit_AddPp(5, -40);
  Unit_AddPp(6, -35);
  state = Unit_Get(4);
  state[0x131] = 1;
  state += 0x140;
  *state = 1;
  state = Unit_Get(5);
  state[0x130] = 1;
  state[0x131] = 2;
  Unit_Calculate(4);
  Unit_Calculate(5);
  Unit_Calculate(6);
}

void Func_020002f4(s32 owner, s32 element, s32 djinn, s32 activate) {
  Event_SetFlag(element * 20 + djinn + 48);
  Djinn_Give(owner, element, djinn);
  if (activate == 1)
    Djinn_Set(owner, element, djinn);
}

void Func_02000330(void) {
  Message_ShowCentered((s32)MsgYouGotA, 1);
  Func_020002f4(4, 0, 0, 1);
  Func_020002f4(4, 0, 1, 1);
  Func_020002f4(4, 0, 2, 1);
  Func_020002f4(4, 0, 3, 1);
  Func_020002f4(4, 0, 4, 1);
  Func_020002f4(4, 0, 5, 1);
  Func_020002f4(4, 0, 6, 1);
  Func_020002f4(5, 2, 0, 1);
  Func_020002f4(5, 2, 1, 1);
  Func_020002f4(5, 2, 2, 1);
  Func_020002f4(5, 2, 3, 1);
  Func_020002f4(5, 2, 4, 1);
  Func_020002f4(5, 2, 5, 1);
  Func_020002f4(5, 2, 6, 1);
  Func_020002f4(6, 1, 0, 1);
  Func_020002f4(6, 1, 1, 1);
  Func_020002f4(6, 1, 2, 1);
  Func_020002f4(6, 1, 3, 1);
  Func_020002f4(6, 1, 4, 1);
  Func_020002f4(6, 1, 5, 1);
  Unit_Calculate(4);
  Unit_Calculate(5);
  Unit_Calculate(6);
}

void Func_02000444(void) { Actor_GetById(4)->terrainId = 2; }

void Func_02000454(void) { Actor_GetById(4)->terrainId = 0; }

void Func_02000464(s32 unused, s32 actor) {
  Actor_ShowEmote(actor, 0x100, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  if (Event_GetFlag(0x200) == 0) {
    FieldEvent_SetMessageId((s32)MsgDailaInteriorsGah);
    Event_ShowMessage(actor, 0);
    Actor_WalkToAndWait(actor, 360, 72);
    Actor_FaceDirection(actor, 0x4000, 0);
    Event_SetFlag(0x200);
  } else {
    FieldEvent_SetMessageId((s32)MsgDailaInteriorsYarg);
    Event_ShowMessage(actor, 0);
    Actor_WalkToAndWait(actor, 392, 104);
    Actor_FaceDirection(actor, 0x8000, 0);
    Event_ClearFlag(0x200);
  }
  FieldEvent_Wait(20);
}

void Func_02000508(s32 unused, s32 actor) {
  Actor_FaceActor(actor, 4, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(actor, 0x103, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgDailaInteriorsWhatAreYouDoing);
  Event_ShowMessage(actor, 0);
  FieldEvent_Wait(10);
}

void Func_0200055c(s32 unused, s32 actor) {
  Actor_FaceActor(actor, 4, 0);
  FieldEvent_Wait(10);
  FieldActor_ShowEmote(actor, 0x103, 50);
  FieldActor_Jump(actor, 6, 15);
  FieldActor_Jump(actor, 6, 15);
  FieldEvent_Wait(10);
  FieldEvent_SetMessageId((s32)MsgDailaInteriorsWhatAreYouDoing);
  Event_ShowMessage(actor, 0);
  FieldEvent_Wait(10);
}

void Func_020005b0(void) {
  s32 frames;
  s32 flags;

  Actor_SetAnimation(21, 2);
  FieldEvent_Wait(10);
  frames = 25;
  flags = 0;
  MapGrid_CopyCells(57, 16, 4, 6, frames, flags);
}

void Func_020005dc(s32 value) {
  s32 frames;
  s32 flags;

  if (value == 1) {
    Actor_SetAnimation(21, 2);
    frames = 25;
    flags = 0;
    MapGrid_CopyCells(57, 16, 4, 6, frames, flags);
  }
}

void Func_02000604(void) {
  s32 a;
  s32 b;
  s32 c;

  a = 2;
  b = 10;
  c = 0;
  TileCopies_CopyTouchedCells(34, 26, 5, 4, a, b, c);
}

static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f,
                                     s32 g) {
  TileCopies_CopyTouchedCells(a, b, c, d, e, f, g);
}

void Func_02000624(void) {
  Map_CopyCells(34, 26, 5, 4, 2, 10, 0);
  Event_SetDestination((s32)&SceneId_PsynergyTest, 6);
}

void Func_02000650(void) {
  Event_SetDestination((s32)&SceneId_PsynergyTest, 8);
}

/* VIEW(psynergy_test): variable gData_020017b0 as u8 gData_020017b0[];
 * module/tla.h has s32 gData_020017b0; tried as the rest of the object declares
 * it, Func_02000748 moves: 2 of 22 instructions in tla-ja. */
extern u8 gData_020017b0__psynergy_test[] __asm__("gData_020017b0");

void Func_02000660(void) {
  Func_02000f14(249, 40, 168);
  gMainRoutineWork.battleRule24b = 2;
  Field_RequestBattleFromEncounterTable(14, 0);
}

void Func_02000688(s32 actor) {
  Event_SetFlag(FLAG_DAILA_PSY_CRYSTAL_DUG);
  Func_0200092c(actor, 36, 28, 1, 1, 10, 10);
}

void Func_020006b0(s32 actor) {
  Event_SetFlag(0x301);
  Func_02000950(actor, 18, 184, 184);
}

void Func_020006cc(s32 actor) {
  Event_SetFlag(0x302);
  Func_020009c0(actor, 19, 200, 200);
}

void Func_020006e8(s32 actor) {
  Event_SetFlag(0x303);
  Func_02000a10(actor, 216, 216, 14, 0);
}

void Func_0200070c(void) {
  Event_PickUpItem(19, ((u8 *)Actor_Get(19))[0x62], 0x310);
}

void Func_02000724(void) {
  s32 frames;
  s32 flags;

  frames = 16;
  flags = 10;
  MapGrid_CopyCells(48, 26, 2, 4, frames, flags);
}

void Func_02000740(void) {}

void Func_02000744(void) {}

void Func_02000748(s32 actor) {
  Func_02000c4c(Actor_Get(actor), gData_02001860);
  Func_02000c4c(Actor_Get(gMainRoutineWork.currentOwner),
                gData_020017b0__psynergy_test);
  System_Wait(100);
  Func_02000d4c(Actor_Get(actor));
  Func_02000d4c(Actor_Get(gMainRoutineWork.currentOwner));
}

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

void Actor_FallOverById(s32 actor, s32 angle);
void Actor_DepartInRing(s32 actor);

s32 Func_0200079c(void) {
  u8 *work = Ram_HeapSlots->eventWork;

  *(s32 *)(work + 428) = 0x100;
  work[3383] = 0;
  work[3382] = 0;
  Item_AdjustCounter(5, 1);
  Item_AdjustCounter(5, 1);
  Item_AdjustCounter(5, 1);
  Item_AdjustCounter(6, 1);
  Item_AdjustCounter(6, 1);
  Item_AdjustCounter(7, 1);
  Item_AdjustCounter(106, 1);
  Item_AdjustCounter(108, 1);
  Item_AdjustCounter(109, 1);
  Item_AdjustCounter(113, 1);
  Item_AdjustCounter(123, 1);
  Item_AdjustCounter(130, 1);
  Item_AdjustCounter(140, 1);
  Item_AdjustCounter(151, 1);
  gMainRoutineWork.eventMode = 2;
  Actor_SetAnimation(15, 2);
  Actor_SetAnimation(16, 2);
  // Call3 and Call2 build each call's constants afresh (call.h).
  Call3((void (*)())Actor_FallOverWithAnimationById, 9, 0x8000, 0);
  Call2((void (*)())Actor_FallOverById, 10, 0x8000);
  Actor_FallOverById(12, 0);
  if (Event_GetFlag(FLAG_DAILA_PSY_CRYSTAL_DUG))
    MapGrid_CopyCells(36, 28, 1, 1, 10, 10);
  if (Event_GetFlag(0x301))
    Actor_SetPosition(18, 184, 184);
  if (Event_GetFlag(0x302) && !Event_GetFlag(FLAG_GAROH_NIGHT))
    Actor_SetPosition(19, 200, 200);
  Actor_SetItemIcon(Actor_Get(19), 50);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->entrance == 6)
      Actor_DepartInRing(party->currentOwner);
    if (party->entrance == 8)
      Actor_ArriveInRing(party->currentOwner);
  }
  return 0;
}

void DebugBrowser_Icons();
void Sanctum_Run();

s32 Scene_PrepareMap(void) { return 0; }

void Func_0200091c(void) {
  DebugBrowser_Icons();
  return;
}

void Func_02000924(void) {
  Sanctum_Run();
  return;
}

void Func_0200092c(s32 mode, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  if (mode == 1)
    MapGrid_CopyCells(a, b, c, d, e, f);
}

void Func_02000950(s32 mode, s32 id, s32 x, s32 z) {
  struct Actor *object;
  s32 scale;

  object = Actor_Get(id);
  if (mode == 1) {
    Actor_SetPositionFixed(id, x << 16, z << 16);
    object->scaleX = 0;
    object->scaleY = 0;
  }
  if (mode == 2) {
    while (object->scaleX < Q16_ONE) {
      System_Wait(1);
      scale = object->scaleX;
      scale += 0x51e;
      object->scaleX = scale;
      object->scaleY = scale;
    }
    object->scaleX = Q16_ONE;
    object->scaleY = Q16_ONE;
  }
}

void Func_020009c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
  struct Actor *object0;

  object0 = Actor_Get(arg1);
  if (arg0 != 2)
    goto L_02008a06;
  Actor_SetPositionFixed(arg1, px(arg2), px(arg3));
  object0->y = px(32);
  object0->motionFlags = 3;
  object0->gravity = 0x170a;
  System_Wait(50);
L_02008a06:
  return;
}

/// Mode 1 places object kind 354 at (x, z) in pixels; mode 2 grows object
/// kind 252 there from 0x6666 to full size while it turns, then turns it
/// opposite the leader's facing and passes scene and entrance on to
/// Field_RequestBattleFromEncounterTable.
void Func_02000a10(s32 mode, s32 x, s32 z, s32 scene, s32 entrance) {
  struct Actor *object;

  if (mode == 1) {
    object = FieldActor_Create(354, x << 16, 0, z << 16);
    if (object != NULL) {
      Actor_SetSpriteFlags(object, 0);
      object->priorityFlags = 2;
    }
  }
  if (mode == 2) {
    object = (FieldActor_Create(252, x << 16, 0, z << 16));
    if (object != NULL) {
      s32 scale;

      Actor_SetSpriteFlags(object, 0);
      object->velocityY = px(6);
      object->scaleX = 0x6666;
      object->scaleY = 0x6666;
      while (object->scaleX < Q16_ONE) {
        System_Wait(1);
        scale = object->scaleX + 0x51e;
        {
          /* FAKEMATCH(temporary): the turn is added as a word. */
          s32 angle = object->facing + TRIG_TURN / 16;

          object->facing = angle;
        }
        object->scaleX = scale;
        object->scaleY = scale;
      }
      object->scaleX = Q16_ONE;
      object->scaleY = Q16_ONE;
      object->facing =
          Actor_Get(gMainRoutineWork.currentOwner)->facing + TRIG_TURN / 2;
      Field_RequestBattleFromEncounterTable(scene, entrance);
    }
  }
}

void Actor_SetSpriteColorMode(struct Actor *, u32);

void Func_02000ad0(struct Actor *object) {
  struct Vector3 position;
  s32 x, z, sine;
  s32 i, j;
  struct SparkleWork *work = (struct SparkleWork *)object->watchedObject;
  u8 *count = (u8 *)object + 99;
  struct SparkleSprite *sprite;

  if (*count & 1)
    Actor_SetSpriteColorMode(
        object, (u8)((u32)((struct CounterBits *)count)->rest % 6));
  if (((u8 *)object)[98] == 0) {
    if ((*count & 15) == 0) {
      Event_PlaySound(SE_598);
      work->unka0[work->unka6] = 0;
      work->unka6 ^= 1;
    }
  } else if (((u8 *)object)[98] == 1) {
    if (work->unka0[0] > 31 && work->unka0[1] > 31) {
      Actor_SetSpriteColorMode(object, 0);
      VramCache_FreeEntry(work->vramEntry);
      object->update = 0;
      return;
    }
  }
  (*count)++;
  for (i = 0; i < 2; i++) {
    sine = Math_Sin(work->unka0[i] << 10);
    if (work->unka0[i] >= 0) {
      if (work->unka0[i] <= 31) {
        struct Vector3 *p = &position;

        SET_POSITION(p, object->x, object->y + (work->unka0[i] << 16),
                     object->z);
        Camera_WorldToScreen(p);
        x = p->x;
        j = 0;
        z = p->z;
        sprite = &work->sprites[i * 2];
        for (; j <= 1; j++) {
          sprite->unk0c = x;
          sprite->unk10 = z;
          sprite->unk14 = sine;
          sprite->unk18 = sine;
          if (j == 0) {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) - 1;
          } else {
            sprite->unk1e = BattleEffect_GetObjectDepth(object) + 1;
            sprite->unk10 -= 0x10000;
            sprite->unk18 = -sprite->unk18;
          }
          EffectSprite_Draw(sprite);
          sprite++;
        }
        work->unka0[i]++;
      }
    }
  }
}

// clang-format off
s32 gData_02000f44[] = {
    0, 0, 0, 0, 0, 0,
    0x70000000, (s32)0xf7700000, 0, 0, 0, 0,
    0x77770000, (s32)0xffff7770, -9, 0x7777ffff, 0, 0,
    0, 0, 0x7777, 0x777ffff, 0x7fffffff, (s32)0xffff7777,
    0, 0, 0, 0, 0, 0,
    7, 0x77f, -0x90000, -0x9000, 0x7ffff700, 0x7ffff70,
    0x7fff70, 0x7ffff7, 0x7fff7, 0x7fff7, 0x77ff, 119,
    0, 0, 0, 0, 0, 0,
    -0x890000, 0x77000000, 0, 0, 0, 0,
    0, 0, 0x7fff, 0x7ffff, 0x7ffff7, 0x7ffff70,
    0x7fff700, 0x7ffff700, 0x7fff7000, 0x7fff7000, 0, 0,
    0, 0, 0, 0, 0x40000000, (s32)0xf4400000,
    0, 0, 0, 0, 0x44440000, (s32)0xffff4440,
    -12, 0x4444ffff, 0, 0, 0, 0,
    0x4444, 0x444ffff, 0x4fffffff, (s32)0xffff4444, 0, 0,
    0, 0, 0, 0, 4, 0x44f,
    -0xc0000, -0xc000, 0x4ffff400, 0x4ffff40, 0x4fff40, 0x4ffff4,
    0x4fff4, 0x4fff4, 0x44ff, 68, 0, 0,
    0, 0, 0, 0, -0xbc0000, 0x44000000,
    0, 0, 0, 0, 0, 0,
    0x4fff, 0x4ffff, 0x4ffff4, 0x4ffff40, 0x4fff400, 0x4ffff400,
    0x4fff4000, 0x4fff4000, (s32)0xe9000000, 233, (s32)0xeb000000, 235,
    (s32)0xedeb0000, 0xebed, (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xedeb0000, 0xebed, (s32)0xeb000000, 235, (s32)0xe9000000, 233,
    0xe900, 0xe90000, 0xebe9e9, (s32)0xe9e9eb00, (s32)0xedebeb00, 0xebebed,
    (s32)0xefed0000, 0xedef, (s32)0xefed0000, 0xedef, (s32)0xedebeb00, 0xebebed,
    0xebe9e9, (s32)0xe9e9eb00, 0xe900, 0xe90000, (s32)0xe9000000, 233,
    (s32)0xebe90000, 0xe9eb, (s32)0xedebe900, 0xe9ebed, (s32)0xeeedebe9, (s32)0xe9ebedee,
    (s32)0xeeedebe9, (s32)0xe9ebedee, (s32)0xedebe900, 0xe9ebed, (s32)0xebe90000, 0xe9eb,
    (s32)0xe9000000, 233,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 64, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 112, 0, 56, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 64, 0, 120, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 112, 0, 120, FACING_NORTH, 0, 16, 16, 304, 224, 0},
    {4, CONDITION_ALWAYS, 344, 0, 104, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {5, CONDITION_ALWAYS, 88, 0, 184, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {6, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {7, CONDITION_ALWAYS, 424, 0, 136, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {8, CONDITION_ALWAYS, 184, 0, 200, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(PsynergyTest),
    SCENE_EXIT(20, PsynergyTest, 2),
    SCENE_EXITS_END,
};

s16 gData_02001300[] = {
    1, -1, 1, 0, 0, 40, 0, 0,
    0, 40, 0x4000, 0, 5, -1, 1, 0,
    0, 40, 0, 0, 0, 72, 0x4000, 0,
    2, -1, 1, 0, 0, 40, 0, 0,
    0, 104, 0x4000, 0, 3, -1, 1, 0,
    0, 40, 0, 0, 0, 136, 0x4000, 0,
    205, -1, 1, 0, 0, 88, 0, 0,
    0, 40, 0x4000, 0, 71, -1, 1, 0,
    0, 82, 0, 0, 0, 72, 0x4000, 0,
    202, -1, 1, 0, 0, 88, 0, 0,
    0, 104, 0x4000, 0, 206, -1, 1, 0,
    0, 88, 0, 0, 0, 136, 0x4000, 0,
    245, -1, 1, 0, 0, 136, 0, 0,
    0, 40, 0x4000, 0, 307, -1, 1, 0,
    0, 136, 0, 0, 0, 72, 0x4000, 0,
    299, -1, 1, 0, 0, 136, 0, 0,
    0, 104, 0x4000, 0, 290, -1, 1, 0,
    0, 136, 0, 0, 0, 136, 0x4000, 0,
    364, -1, 1, 0, 0, 360, 0, 0,
    0, 104, 0x4000, 0, 56, -1, 1, 0,
    0, 392, 0, 0, 0, 104, 0x4000, 0,
    290, -1, 1, 0, 0, -64, 0, 0,
    0, -64, 0x4000, 0, 489, -1, 1, 0,
    0, -64, 0, 0, 0, -64, 0x4000, 2,
    252, -1, 1, 0, 0, -64, 0, 0,
    0, -64, 0x4000, 2, 305, -1, 1, 0,
    0, 456, 0, 0, 0, 104, 0x4000, 0,
    365, -1, 1, 0, 0, 392, 0, 0,
    0, 168, 0x4000, 0, -1, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {0, 5, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0, 2, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0, 3, CONDITION_ALWAYS, EVENT(Func_02000748)},
    {0xc400, 8, CONDITION_ALWAYS, EVENT(Func_020001a4)},
    {0x8400, 8, CONDITION_ALWAYS, EVENT(Func_020001b0)},
    {0x400, 8, CONDITION_ALWAYS, EVENT(Func_020001bc)},
    {0x4400, 8, CONDITION_ALWAYS, EVENT(Func_020001c8)},
    {0, 9, CONDITION_ALWAYS, EVENT(Func_020001e0)},
    {0x4400, 10, CONDITION_ALWAYS, EVENT(Func_020001d4)},
    {0, 10, CONDITION_ALWAYS, EVENT(Func_02000924)},
    {0xc400, 11, CONDITION_ALWAYS, EVENT(Func_02000140)},
    {0x8400, 11, CONDITION_ALWAYS, EVENT(Func_02000158)},
    {0x400, 11, CONDITION_ALWAYS, EVENT(Func_02000170)},
    {0x4400, 11, CONDITION_ALWAYS, EVENT(Func_0200018c)},
    {0, 12, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0, 13, CONDITION_ALWAYS, EVENT(Func_0200091c)},
    {0, 14, CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0xc400, 15, CONDITION_ALWAYS, EVENT(Test_ReceiveFizz)},
    {0, 15, CONDITION_ALWAYS, EVENT(Func_02000058)},
    {0, 19, CONDITION_ALWAYS, EVENT(Func_0200070c)},
#if (LANGUAGE ==                                                               \
     LANGUAGE_JAPANESE) /* Rows exclusive to the Japanese edition. */
    {0x8d15, 10, CONDITION_ALWAYS, 5618},
#endif
    {0x8d15, 8, CONDITION_ALWAYS, MSGNO(MsgOooogOw2)},
    {0x8d15, 9, CONDITION_ALWAYS, MSGNO(MsgWhyDoThese)},
    {0x8d15, 10, CONDITION_ALWAYS, MSGNO(MsgStupidBriggs2)},
    {0x8d15, 11, CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {0x8d15, 12, CONDITION_ALWAYS, MSGNO(MsgIJustCame)},
    {1, 20, CONDITION_ALWAYS, 20},
    {3, 10, CONDITION_ALWAYS, EVENT(Func_0200026c)},
    {3, 11, CONDITION_ALWAYS, EVENT(Func_020001e4)},
    {3, 12, CONDITION_ALWAYS, EVENT(Func_02000224)},
    {3, 13, CONDITION_ALWAYS, EVENT(Func_02000330)},
    {19, 100, CONDITION_ALWAYS, 0x20014d},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 14, CONDITION_ALWAYS,
     EVENT(Func_02000074)},
    {PSYNERGY_ON_ACTOR(ABILITY_FORCE), 15, CONDITION_ALWAYS,
     EVENT(Func_02000074)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, CONDITION_ALWAYS,
     EVENT(Func_02000074)},
    {PSYNERGY_ON_ACTOR(ABILITY_CATCH), 15, CONDITION_ALWAYS,
     EVENT(Func_02000074)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 18, CONDITION_ALWAYS,
     EVENT(Test_ReceiveFlint)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), 21,
     CONDITION_ALWAYS, EVENT(Func_020005dc)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
     CONDITION_ALWAYS, EVENT(Func_02000444)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
     EVENT(Func_02000454)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 16,
     CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0x60009a15, 17, CONDITION_ALWAYS, EVENT(Func_02000464)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 17,
     CONDITION_ALWAYS, EVENT(Func_02000508)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 17,
     CONDITION_ALWAYS, EVENT(Test_ReceiveFlint)},
    {0x20009d15, 17, CONDITION_ALWAYS, EVENT(Func_0200055c)},
    {0x20009d15, 21, CONDITION_ALWAYS, EVENT(Func_020005b0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 50, 0x300,
     EVENT(Func_02000688)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 51, 0x301,
     EVENT(Func_020006b0)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 52, 0x302,
     EVENT(Func_020006cc)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 53, 0x303,
     EVENT(Func_020006e8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60,
     CONDITION_ALWAYS, EVENT(Func_02000724)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 55,
     CONDITION_ALWAYS, EVENT(Func_02000604)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 56,
     CONDITION_ALWAYS, EVENT(Func_02000624)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13,
     CONDITION_ALWAYS, EVENT(Func_02000650)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 57,
     CONDITION_ALWAYS, EVENT(Func_02000660)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 13, CONDITION_ALWAYS,
     EVENT(Func_02000740)},
    {PSYNERGY_ON_ACTOR(ABILITY_TREMOR), 15, CONDITION_ALWAYS,
     EVENT(Func_02000740)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 11, CONDITION_ALWAYS,
     EVENT(Func_02000744)},
    {PSYNERGY_ON_ACTOR(ABILITY_GRIND), 15, CONDITION_ALWAYS,
     EVENT(Func_02000744)},
    {TRIGGER_TABLE_END},
};
#undef INPUT_HELD

// Overlay 0x6b8's code from Func_02000c4c on, in ROM order.

/* VIEW(s0c4c): tag struct SparkleSprite as struct SparkleSprite__s0c4c;
 * psynergy_test.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/debug/psynergy_test.c:850: structure has no member named
 * `flag'. */
struct SparkleSprite__s0c4c {
  u8 unk00[5];
  u8 attr5Low : 5;
  u8 flag : 1;
  u8 attr5High : 2;
  u8 unk06[2];
  u8 unk08;
  u8 unk09 : 2;
  u8 priority : 2;
  u8 palette : 4;
  u8 unk0a[0x1e];
};

/* VIEW(s0c4c): tag struct SparkleWork as struct SparkleWork__s0c4c;
 * psynergy_test.c has another definition; tried with the definition the rest of
 * the object has, it does not compile in tla-ja:
 * games/tla/src/debug/psynergy_test.c:841: incompatible types in assignment. */
struct SparkleWork__s0c4c {
  struct SparkleSprite__s0c4c sprites[4];
  u16 unka0;
  u16 unka2;
  s16 vramEntry;
  u16 unka6;
  struct Actor *object;
};

void EffectSprite_Init(struct SparkleSprite__s0c4c *sprite, s32 width,
                       s32 height, u32 attributes, s32 tile);

void Func_02000c4c(struct Actor *object, struct SparkleWork__s0c4c *work) {
  struct SparkleSprite__s0c4c *sprite;
  s32 tile;
  s32 row;
  s32 col;
  s32 offset;

  work->vramEntry = VramCache_FindFreeEntry();
  Vram_Load(work->vramEntry, 256, gData_02000f44);
  work->unka6 = 0;
  work->unka0 = 999;
  work->unka2 = 999;
  work->object = object;
  ((u8 *)object)[0x62] = 0;
  ((u8 *)object)[0x63] = 0;
  object->update = Func_02000ad0;
  tile = gResourceTableEntries[work->vramEntry].offset >> 5;
  object->watchedObject = (struct Actor *)work;
  for (row = 0; row <= 1; row++) {
    col = 1;
    offset = (row * 2) * sizeof(struct SparkleSprite__s0c4c);
    sprite = (struct SparkleSprite__s0c4c *)(offset + (s32)work);
    for (; col >= 0; col--) {
      EffectSprite_Init(sprite, 16, 16, 0x80004000, tile);
      sprite->flag = 0;
      sprite->palette = 0;
      sprite->priority = BattleEffect_GetObjectPriority(object) & 3;
      sprite++;
    }
  }
}

void Func_02000d4c(struct Actor *object) { ((u8 *)object)[98] = 1; }
