/// @file
/// Scene code of overlay 0x67b, in ROM order, from s0f8c's offset up to the next object the overlay links.

#include "local.h"
#include "map.h"
#include "scene_tables.h"
#include "input.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

struct AquaRockEventWork {
  u8 unk000[0x16a];
  s16 field16a;
};

void Func_02001e00(void);

static inline u8 CellAttribute(struct MapCell *cells, s32 row, s32 col) {
  cells += row * 128 + col;
  return cells->attributeB;
}

void Func_02000f8c(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct AquaRockEventWork *work = Ram_HeapSlots->eventWork;
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 col;
  s32 row;

  if (gMainRoutineWork.renderMode == 2) {
    col = object->x >> 20;
    row = (object->z - object->y - 0xc0000) >> 20;
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      work->field16a = 44;
  } else {
    col = object->x >> 20;
    row = (object->z - object->y) >> 20;
    if (CellAttribute(map->layers[1].cells, row, col) != 0)
      Func_02001e00();
  }
}

void Func_0802b450(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001f60(s32 id);

void Func_02001010(void) {
  Func_02002960_(62, 41, 1, 2, 33, 41);
  Func_0802b450(125, 32, 3, 13, 96, 32);
  Func_0802b450(61, 96, 3, 13, 32, 96);
  Actor_SetPartAttribute(Actor_GetById(12), 5);
  Func_02001f60(12);
  GameFlag_SetBit(0xa6b);
}

void Func_080d489c(s32 id, s32 value);

void Func_0200106c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->gravity;
  s32 i;
  struct MainRoutineWork *state;

  Func_02002998();
  Event_ResolvePendingActions(0);
  Func_080d489c(gMainRoutineWork.currentOwner, 257);
  Func_02002910(object, 40);
  object->motionFlags = 3;
  for (i = 0; i < 180; i++) {
    wait(1);
    if (object->y <= object->terrainHeight)
      break;
  }
  state = &gMainRoutineWork;
  object->gravity = saved;
  Func_080d489c(state->currentOwner, 0);
  Actor_SetSingleChildFlags(object, 1);
  Func_02002910(object, 1);
  state->renderMode = 0;
  Func_020029a0();
}

void Func_020010f4(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Func_02002960_(42, 32, 1, 1, 44, 32);
  GameFlag_SetBit(0x221);
}

void Func_0200112c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Func_02002960_(48, 32, 1, 1, 46, 32);
  GameFlag_SetBit(0x222);
}

void Func_02001164(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);

  object->priorityFlags = 0;
  object->motionFlags = 0;
  Func_02002960_(44, 44, 1, 1, 46, 44);
  GameFlag_SetBit(0x223);
}

void Func_0200119c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 column = object->x >> 20;

  if (column == 53) {
    wait(30);
    object->z += 0x10000;
    GameFlag_SetBit(0x951);
    Map_CopyCellAttributeRect(54, 58, 1, 1, column, 52);
  }
}

void Func_020011e0(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 column = object->x >> 20;

  if (column == 53) {
    wait(30);
    object->z += 0x10000;
    GameFlag_SetBit(0x952);
    Map_CopyCellAttributeRect(54, 58, 1, 1, column, 58);
  }
}

void Func_02002834(s32 id);
void Func_020025a8(s32 id);
void BattleEffect_StartBufferInterpolation(s32 frames);
void Camera_SetSpeedParameters(s32 a, s32 b);
void Func_02002a10(void);
void Func_02000b10(struct Actor *object);
void Func_02000b58(struct Actor *object);
void Func_02001d98(struct Actor *object);

/// Sets flag 0xa69 after four rounds of a random colour value, runs the
/// blend scripts, then moves the camera to object 15, starts its update and
/// plays cue 623 six times.
void Func_02001224(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 i;

  Func_02002998();
  Event_ResolvePendingActions(0);
  object->update = (void (*)(void))Func_02000b10;
  wait(30);
  Func_02002834(8);
  Sound_Play(220);
  for (i = 3; i >= 0; i--) {
    BattleEffect_ApplyColorToTargetBuffer(Random16Far() & 0x7fff, 0);
    BattleEffect_StartBufferInterpolation(15);
    wait(10);
    BattleEffect_ApplyColorToTargetBuffer(0x10000, 0);
    BattleEffect_StartBufferInterpolation(10);
    wait(10);
  }
  BattleEffect_ApplyColorToTargetBuffer(0x7fff, 0);
  BattleEffect_StartBufferInterpolation(20);
  Event_Wait(20);
  GameFlag_SetBit(0xa69);
  Map_CopyCellAttributeRect(15, 48, 1, 1, 16, 48);
  Map_CopyCellAttributeRect(17, 50, 1, 1, 16, 50);
  object->update = (void (*)(void))Func_02000b58;
  Func_020025a8(8);
  Func_0802b450(64, 68, 5, 13, 14, 94);
  Map_CopyCellAttributeRect(64, 68, 5, 8, 14, 34);
  Event_Wait(30);
  Func_02002a20_(0x10000, 0);
  BattleEffect_StartBufferInterpolation(30);
  Event_Wait(15);
  Camera_SetSpeedParameters(0x10000, 0x2000);
  Camera_MoveWithinMapBounds(PIXELS(248), -PIXELS(32), PIXELS(760), 1);
  Func_02002a10();
  Actor_SetHorizontalPositionWithTerrain(15, PIXELS(264), PIXELS(792));
  Actor_SetSingleChildFlags(Actor_GetById(15), 0);
  Actor_GetById(15)->collisionFlags = 0;
  Actor_GetById(15)->priorityFlags |= 2;
  Actor_GetById(15)->update = (void (*)(void))Func_02001d98;
  Event_Wait(30);
  for (i = 5; i >= 0; i--) {
    Sound_Play(623);
    wait(10);
  }
  Event_Wait(30);
  Func_020029a0();
}

const u8 *Func_020013b8(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov67bRoom1)
    return gData_02003220;
  if (scene == (s32)&SceneId_Ov67bRoom2)
    return gData_020032d4;
  return gData_02003214;
}

void Func_020013f8(void) {
  struct Actor *object = Actor_GetById(Party_GetLeaderActor());

  if (object != 0 && (object->animationKind & 15) == 1) {
    struct FieldSprite *sprite;
    s32 priority;

    sprite = object->animation;
    priority = gMainRoutineWork.renderMode == 2 ? 2 : 1;
    sprite->priority = priority;
    sprite->partPriority = priority;
    object->priorityFlags &= 0xfe;
  }
}

void Func_02001d30(struct Actor *object);
void Actor_SetPositionAndCommit(s32 id, s32 x, s32 z);

void Func_02001460(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->motionFlags;
  s32 i;
  s32 angle;
  s32 up;
  struct MainRoutineWork *state;

  Sound_Play(612);
  Func_02002998();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = -0x100000;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  for (i = 15; i >= 0; i--) {
    object->y += 0x10000;
    angle = object->angle;
    angle += 0x2000;
    object->angle = angle;
    wait(1);
  }
  Func_02001d30(object);
  object->motionFlags = saved;
  object->velocityY = 0x80000;
  Actor_SetSingleChildFlags(object, 1);
  up = gInput.held & 0x40;
  state = &gMainRoutineWork;
  if (up)
    Actor_SetPositionAndCommit(state->currentOwner, 264, 760);
  else
    Actor_SetPositionAndCommit(state->currentOwner, 264, 824);
  Func_020029a0();
}

void Func_0200153c(void) {
  struct Actor *object = Actor_GetById(gMainRoutineWork.currentOwner);
  s32 saved = object->motionFlags;
  s32 i;
  s32 angle;

  Func_02002998();
  Event_ResolvePendingActions(0);
  Event_StartScreenTransition();
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  object->y = -0x400000;
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x20000, 0x10000);
  for (i = 23; i >= 0; i--) {
    object->y += 0x20000;
    angle = object->angle;
    angle += 0x2000;
    object->angle = angle;
    wait(1);
  }
  Func_02001d30(object);
  object->motionFlags = saved;
  object->velocityY = 0xc0000;
  Actor_SetSingleChildFlags(object, 1);
  Actor_SetPositionAndCommit(gMainRoutineWork.currentOwner, 680, 912);
  Func_020029a0();
}

void Func_020015e8(struct Actor *object) {
  struct FieldSprite *sprite = object->animation;
  s16 *amplitude;

  object->action += 0x2000;
  amplitude = (s16 *)&object->unk66;
  *amplitude += 32;
  if (*amplitude > 0x200)
    *amplitude = 0x200;
  sprite->rotation = Iwram_MulQ16(Math_Sine(object->action), *amplitude);
}

void Func_02001630(struct Actor *object) {
  struct FieldSprite *sprite = object->animation;
  s16 *amplitude;

  object->action += 0x2000;
  amplitude = (s16 *)&object->unk66;
  *amplitude -= 16;
  if (*amplitude < 0) {
    *amplitude = 0;
  }
  sprite->rotation = Iwram_MulQ16(Math_Sine(object->action), *amplitude);
}

void Func_02001670(struct Actor *object) {
  struct FieldSprite *sprite = object->animation;

  object->action += 0x100;
  sprite->rotation = Math_Sine(object->action) / 64 + 0x600;
}

/// Scatters objects 17 to 21 under cue 184, lines them up 32 pixels apart
/// at z 824, then settles them under cue 298 and leaves object 21 rocking;
/// sets flags 0x109 and 0x955 and clears 0x12f.
void Func_0200169c(void) {
  s32 i;

  Func_02002998();
  Event_ResolvePendingActions(0);
  Camera_MoveWithinMapBounds_(-1, -1, -1, 0);
  Actor_SetHorizontalPositionWithTerrain(gMainRoutineWork.currentOwner, 0, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Sound_Play(184);
  // Two goto loops: the ROM loads the zero and the update address on each pass,
  // which loop.c hoists out of for loops (differ 18).
  i = 17;
scatter:
  {
    struct Actor *object = Actor_GetById(i);

    object->speedLimit = 0x4000;
    object->acceleration = 0x400;
    object->update = (void (*)(void))Func_020015e8;
    object->action = Random16Far();
    object->unk66 = 0;
  }
  if (++i <= 21)
    goto scatter;
  Event_Wait(60);
  Map_CopyCellsRect(64, 69, 11, 4, 87, 53);
  Actor_ResetAndSetPosition_(17, 392, 824);
  Actor_ResetAndSetPosition_(18, 424, 824);
  Actor_ResetAndSetPosition_(19, 456, 824);
  Actor_ResetAndSetPosition_(20, 488, 824);
  Actor_ResetAndSetPosition(21, 520, 824);
  Event_Wait(240);
  Map_CopyCellsRect(64, 66, 11, 7, 87, 50);
  i = 17;
settle:
  Actor_GetById(i)->update = (void (*)(void))Func_02001630;
  if (++i <= 21)
    goto settle;
  Sound_Play(298);
  Event_Wait(60);
  {
    struct Actor *object = Actor_GetById(21);

    object->update = (void (*)(void))Func_02001670;
    object->action = 0;
  }
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  GameFlag_SetBit(0x109);
  GameFlag_ClearBit(0x12f);
  Import_EventSetExitRequest(77);
  Func_020029a0();
  GameFlag_SetBit(0x955);
}
