#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_scene.h"
#include "input.h"
#include "party_state.h"
#include "text/msg_ids.h"

extern u8 gData_02001300[];

void Func_02000f34(s32 *, s32 *);
void Func_02000e04(s32);
void *Func_02000dec(s32, s32, s32, s32);
void Func_02000df4(s32, s32);
void Func_02000f1c(s32, s32);
void Func_02000f2c(s32, s32);
s32 Func_02000eac(s32, s32, s32);
void Func_02000e94(s32);
void Func_02000e9c(s32, s32);
void Func_02000dac(s32, s32, s32, s32, s32, s32);
void Func_02000ecc(s32, s32, s32, s32, s32, s32, s32);
void UiWork_FinalizePendingCore(void);
void UiText_OpenMessageWindow(s32, s32, s32, s32);
s32 UiWork_IsComplete(void);
void UiText_ShowPositionedMessageAndWait(s32, s32);
void UnitCalc(s32);
s32 Unit_AddPp(s32, s32);
void GameFlag_SetBit(s32);
struct ObjectRuntime *Object_GetById(s32);
void ObjectMotion_Launch(s32, s32, s32);
void Battle_WaitMode0(s32);
void ObjectMotion_SetPositionAndReset(s32, s32, s32);
void ObjectMotion_ArmCallback(s32, s32, s32);
void ObjectMotion_SetAngleToward(s32, s32, s32);

#define INPUT_HELD (*(volatile u32 *)&gInput.held)

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

void *Func_0200004c(void) { return gData_02001300; }

void Func_02000054(void) {}

void Func_02000058(void) {
  s32 pos[2];

  Func_02000f34(&pos[1], &pos[0]);
}

void Func_02000068(void) { Func_02000e04(0); }

void Func_02000074(void) {}

void Func_02000078(s32 message) {
  UiWork_FinalizePendingCore();
  UiText_OpenMessageWindow(message, 5, 0, 0x22);
  while (UiWork_IsComplete() == 0)
    WaitFrames(1);
  WaitFrames(1);
}

void Func_020000a4(s32 message, s32 count) {
  s32 i;
  void *window;

  window = Func_02000dec(206, 0, 0, 0);
  for (i = 0; i < count; i++) {
    Func_02000df4(1, 1);
    Func_02000df4(5, 2);
    Func_02000df4(0x1e240, 5);
    Func_02000078(message);
    goto check;
  wait:
    if (INPUT_HELD != 0)
      goto next;
    WaitFrames(1);
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
  UiWork_FinalizePendingCore();
  UiWork_Finalize(window, 2);
}

void Func_02000140(void) {
  Func_020000a4((s32)MsgWeaponShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000158(void) {
  Func_020000a4((s32)MsgArmorShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_02000170(void) {
  Func_020000a4((s32)MsgItemShopWelcome,
                MsgArmorShopWelcome - MsgWeaponShopWelcome);
}

void Func_0200018c(void) {
  Func_020000a4((s32)MsgWelcomeWearyWanderers,
                MsgRightWhenI - MsgWelcomeWearyWanderers);
}

void Func_020001a4(s32 value) { Func_02000f1c(1, value); }

void Func_020001b0(s32 value) { Func_02000f1c(2, value); }

void Func_020001bc(s32 value) { Func_02000f1c(3, value); }

void Func_020001c8(s32 value) { Func_02000f1c(24, value); }

void Func_020001d4(s32 value) { Func_02000f2c(0, value); }

void Func_020001e0(void) {}

void Func_020001e4(void) {
  UiText_ShowPositionedMessageAndWait((s32)MsgYouWentUp, 1);
  Unit_LevelUpTo(4, ((u8 *)UnitGet(4))[15] + 10);
  Unit_LevelUpTo(5, ((u8 *)UnitGet(5))[15] + 10);
  Unit_LevelUpTo(6, ((u8 *)UnitGet(6))[15] + 10);
}

void Func_02000224(void) {
  s32 i;

  UiText_ShowPositionedMessageAndWait((s32)MsgYouGotTreasure, 1);
  gPartyState.coins = 777777;
  for (i = 9; i >= 0; i--) {
    Inventory_AddItem(4, 228);
    Inventory_AddItem(4, 229);
  }
  UnitCalc(4);
  UnitCalc(5);
  UnitCalc(6);
}

static __inline__ void Unit_AddHpBy(s32 owner, s32 delta) {
  /* FAKEMATCH: a direct call keeps -100 in r5 for the next call; ⚓️ builds it
   * again before each. */
  Unit_AddHp(owner, delta);
}

void Func_0200026c(void) {
  u8 *state;

  UiText_ShowPositionedMessageAndWait((s32)MsgTheBodyIs, 1);
  Unit_AddHpBy(4, -100);
  Unit_AddHpBy(5, -100);
  Unit_AddHp(6, -33);
  Unit_AddPp(4, -50);
  Unit_AddPp(5, -40);
  Unit_AddPp(6, -35);
  state = UnitGet(4);
  state[0x131] = 1;
  state += 0x140;
  *state = 1;
  state = UnitGet(5);
  state[0x130] = 1;
  state[0x131] = 2;
  UnitCalc(4);
  UnitCalc(5);
  UnitCalc(6);
}

void Func_020002f4(s32 owner, s32 element, s32 djinn, s32 activate) {
  GameFlag_SetBit(element * 20 + djinn + 48);
  Djinn_AddToOwner(owner, element, djinn);
  if (activate == 1)
    Djinn_Activate(owner, element, djinn);
}

void Func_02000330(void) {
  UiText_ShowPositionedMessageAndWait((s32)MsgYouGotA, 1);
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
  UnitCalc(4);
  UnitCalc(5);
  UnitCalc(6);
}

void Func_02000444(void) { Object_GetById(4)->terrainId = 2; }

void Func_02000454(void) { Object_GetById(4)->terrainId = 0; }

static __inline__ void Object_Emote(s32 actor, s32 emote, s32 frames) {
  /* FAKEMATCH: a direct call sets the actor before the emote; ⚓️ adds the
   * emote's 255 after it. */
  Func_02000eac(actor, emote, frames);
}

static __inline__ s32 GameFlag_TestBit(s32 flag) {
  /* FAKEMATCH: a direct call keeps 0x200 in r6 for the later calls; ⚓️ builds
   * it again before each. */
  return GameFlag_Test(flag);
}

static __inline__ void ObjectMotion_SetPosition(s32 actor, s32 x, s32 z) {
  /* FAKEMATCH: a direct call builds the x constant before the actor copy; ⚓️
   * splits it around the copy. */
  ObjectMotion_SetPositionAndReset(actor, x, z);
}

void Func_02000464(s32 unused, s32 actor) {
  Object_Emote(actor, 0x100, 50);
  ObjectMotion_Launch(actor, 6, 15);
  ObjectMotion_Launch(actor, 6, 15);
  Battle_WaitMode0(10);
  if (GameFlag_TestBit(0x200) == 0) {
    Func_02000e94((s32)MsgDailaInteriorsGah);
    Func_02000e9c(actor, 0);
    ObjectMotion_SetPosition(actor, 360, 72);
    ObjectMotion_ArmCallback(actor, 0x4000, 0);
    GameFlag_SetBit(0x200);
  } else {
    Func_02000e94((s32)MsgDailaInteriorsYarg);
    Func_02000e9c(actor, 0);
    ObjectMotion_SetPosition(actor, 392, 104);
    ObjectMotion_ArmCallback(actor, 0x8000, 0);
    GameFlag_ClearBit(0x200);
  }
  Battle_WaitMode0(20);
}

void Func_02000508(s32 unused, s32 actor) {
  ObjectMotion_SetAngleToward(actor, 4, 0);
  Battle_WaitMode0(10);
  Object_Emote(actor, 0x103, 50);
  ObjectMotion_Launch(actor, 6, 15);
  ObjectMotion_Launch(actor, 6, 15);
  Battle_WaitMode0(10);
  Func_02000e94((s32)MsgDailaInteriorsWhatAreYouDoing);
  Func_02000e9c(actor, 0);
  Battle_WaitMode0(10);
}

void Func_0200055c(s32 unused, s32 actor) {
  ObjectMotion_SetAngleToward(actor, 4, 0);
  Battle_WaitMode0(10);
  Object_Emote(actor, 0x103, 50);
  ObjectMotion_Launch(actor, 6, 15);
  ObjectMotion_Launch(actor, 6, 15);
  Battle_WaitMode0(10);
  Func_02000e94((s32)MsgDailaInteriorsWhatAreYouDoing);
  Func_02000e9c(actor, 0);
  Battle_WaitMode0(10);
}

void Func_020005b0(void) {
  s32 frames;
  s32 flags;

  Object_SetModeById(21, 2);
  Battle_WaitMode0(10);
  frames = 25;
  flags = 0;
  Func_02000dac(57, 16, 4, 6, frames, flags);
}

void Func_020005dc(s32 value) {
  s32 frames;
  s32 flags;

  if (value == 1) {
    Object_SetModeById(21, 2);
    frames = 25;
    flags = 0;
    Func_02000dac(57, 16, 4, 6, frames, flags);
  }
}

void Func_02000604(void) {
  s32 a;
  s32 b;
  s32 c;

  a = 2;
  b = 10;
  c = 0;
  Func_02000ecc(34, 26, 5, 4, a, b, c);
}
