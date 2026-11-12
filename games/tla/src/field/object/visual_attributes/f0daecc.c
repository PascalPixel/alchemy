/// @file
/// Routines first written as f0daecc.c; none has a twin in The Broken Seal.
// Part of visual_attributes.c.

#include "../visual_attributes_internal.h"

/* VIEW(f0daecc): tag struct ChainSlot as struct ChainSlot__f0daecc; visual_attributes.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:6916: structure has no member named `root'. */
struct ChainSlot__f0daecc {
  s16 objectId;
  u8 unk02[2];
  s32 swing;
  s32 spacing;
  u8 unk0c[4];
  s32 phase;
  s32 active;
  struct ChainLink *root;
};

struct ChainObject {
  u8 unk00[8];
  s32 x;
  s32 y;
  s32 z;
  u8 unk14[0x3c];
  struct {
    u8 unk00[0x28];
    s16 *entries[4];
  } *sprite;
  u8 unk54;
  u8 motionFlags;
  u8 unk56[3];
  u8 collisionFlags;
};

struct Actor *Actor_GetById(u32 objectId);
struct WorkA4Slot *Func_080dae70(s32 id);

#define CELL_MASK 0xfff00000 // 16-pixel cells

void Func_080daecc(s32 objectId, s32 grow) {
  struct ChainObject *object;
  struct MapWork *map;
  struct ChainSlot__f0daecc *slot;
  struct ChainLink *link;
  struct ChainLink **tail;
  struct MapCell *cells;
  struct MapCell *cell;
  s32 i;
  s32 x;
  s32 top;
  s32 z;
  s32 y;
  s32 frame;
  s32 last;
  s32 kind;
  s32 running;

  object = Actor_GetById(objectId);
  running = Ram_HeapSlots->unk84[8];
  kind = 372;
  map = Ram_HeapSlots->mapWork;
  if (running == 0) {
    if (object->sprite != 0 && object->sprite->entries[0] != 0)
      kind = *object->sprite->entries[0];
    Func_080da9a8(kind);
  }
  if (Func_080dae70(objectId) != 0)
    return;
  slot = Func_080dae3c();
  if (slot == 0)
    return;

  // Clear the collision codes of the five cells above the object.
  x = object->x >> 20;
  z = (object->z - object->y) >> 20;
  cells = map->layers[2].cells;
  for (i = 0; i < 5; i++) {
    z++;
    cell = cells;
    cell += x + z * 128;
    cell->collisionCode = 0;
  }

  x = (object->x & CELL_MASK) + (8 << 16);
  y = object->y & CELL_MASK;
  top = y + (8 << 16);
  z = (object->z & CELL_MASK) + (16 << 16);
  Actor_SetMode((struct Actor *)object, 0);
  object->motionFlags = 0;
  object->collisionFlags = 0;
  object->z += 16 << 16;
  object->y -= 64 << 16;
  Actor_SetSingleChildFlagsFar(object, 0);

  // Hang the links.
  last = 6;
  tail = &slot->root;
  for (i = 0; i <= last && (link = Func_080daea8()) != 0; i++) {
    link->x = x;
    link->y = y + (15 << 16) - i * (14 << 16);
    link->z = z;
    if (i == 0)
      link->kind = 1;
    else if (i == 1)
      link->kind = 2;
    else if (i == last)
      link->kind = 5;
    else
      link->kind = i % 2 + 3;
    *tail = link;
    tail = &link->next;
  }

  if (grow == 0) {
    for (i = 0, link = slot->root; i <= last && link != 0; i++)
      link = link->next;
  } else {
    for (frame = 1; frame <= 28; frame++) {
      i = 0;
      link = slot->root;
      if (i <= last && link != 0) {
        s32 drop = -(frame << 15);

        do {
          link->x = x;
          link->y = top - drop / 2;
          link->z = z;
          drop += frame << 16;
          i++;
          link = link->next;
        } while (i <= last && link != 0);
      }
      wait(1);
    }
  }
  slot->active = 1;
  slot->phase = 0;
  slot->swing = 0x100;
  slot->objectId = objectId;
  slot->spacing = 14 << 16;
}


struct Unk_080db0b0Sprite {
  u8 unk00[18];
  u16 unk12;
};

// The half turn on the leader's side of the anchor: 0 on its right, 1 on its
// left.
#define SIDE_TURN(side) ((side) * (TRIG_TURN / 2))

void Func_080db0b0(s32 id) {
  s32 rootX, rootY;
  s32 motionFlags;
  struct Unk_080db0b0Sprite *heldSprite;
  s32 side = 0;
  struct Actor *leader = Actor_GetById(Party_GetLeaderActor());
  struct Actor *anchor = Actor_GetById(id);
  /* FAKEMATCH(wrapper): the slot read through a one-member union, which may
   * alias every store, so its reads stay after the stores before them; as
   * struct WorkA4Slot *, 12 of 378 instructions differ in tla-en. */
  union {
    struct WorkA4Slot s;
  } *slot;
  struct Unk_080db0b0Sprite *sprite;
  s32 swing;
  s32 start;
  s32 dir;
  u16 letGo;
  s32 radius;
  s32 steps;
  s32 angle;
  s32 sine;
  s32 i;

  if (leader == NULL || anchor == NULL)
    return;
  motionFlags = leader->motionFlags;
  heldSprite = leader->sprite;
  if (leader->x < anchor->x)
    side = 1;
  slot = (void *)Func_080dae70(id);
  if (slot == NULL)
    return;

  start = slot->s.swing;
  if ((slot->s.phase ^ SIDE_TURN(side)) & (TRIG_TURN / 2)) {
    for (swing = start; swing >= 0; swing -= 64) {
      slot->s.swing = swing;
      wait(1);
    }
    start = 0;
  }
  for (swing = start; swing <= 1536; swing += 64) {
    slot->s.swing = swing;
    slot->s.phase = SIDE_TURN(side) + TRIG_TURN / 4;
    wait(1);
  }
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side ^ 1) + TRIG_TURN / 4) {
    slot->s.swing = 1536;
    wait(1);
  }

  sprite = leader->sprite;
  dir = 1;
  if (side) {
    letGo = KEY_DOWN | KEY_LEFT | KEY_B;
    dir -= 2;
  } else
    letGo = KEY_DOWN | KEY_RIGHT | KEY_B;
  slot->s.active = 1;
  Actor_SetMode(leader, 32);
  while (1) {
    slot->s.phase = SIDE_TURN(side ^ 1) + TRIG_TURN / 4;
    switch (gFrameCount & 63) {
    case 0:
    case 16:
      slot->s.swing = 1568;
      slot->s.spacing = Q16_ONE * 29 / 2;
      sprite->unk12 = dir << 10;
      break;
    case 1:
    case 2:
    case 17:
    case 18:
      slot->s.swing += 16;
      slot->s.spacing += Q16_ONE / 4;
      sprite->unk12 += dir << 9;
      break;
    default:
      if (slot->s.swing > 1536)
        slot->s.swing -= 32;
      else
        slot->s.swing = 1536;
      if (slot->s.spacing > Q16_ONE * 14)
        slot->s.spacing -= Q16_ONE / 32;
      if (sprite->unk12 != 0)
        sprite->unk12 -= dir << 7;
      break;
    }
    if (gInput.pressed & letGo) {
      sprite->unk12 = 0;
      slot->s.active = 1;
      return;
    }
    if (gInput.pressed != 0)
      break;
    wait(1);
  }

  sprite->unk12 = 0;
  slot->s.active = 0;
  Actor_SetMode(leader, 6);
  wait(6);
  Sound_PlayFar(152);
  Actor_SetMode(leader, 7);
  leader->speedLimit = Q16_ONE * 3;
  leader->acceleration = Q16_ONE * 2;
  leader->velocityY = Q16_ONE * 4;
  leader->motionFlags &= 0x7e;
  Actor_SetSingleChildFlagsFar(leader, 0);
  wait(5);
  Actor_SetMode(leader, 16);
  leader->motionFlags = 0;
  rootX = slot->s.root->next->x;
  rootY = slot->s.root->next->y;
  radius = Q16_ONE * 65;
  steps = 8;
  while ((slot->s.phase & 0xff00) != SIDE_TURN(side) + TRIG_TURN * 7 / 32) {
    if (steps != 0) {
      radius += Q16_ONE * 5 / 16;
      slot->s.spacing += Q16_ONE / 16;
      steps--;
    }
    slot->s.swing = 2048;
    slot->s.active = 1;
    // A once-only block: the sine is taken before Iwram_MulQ16 is loaded.
    do {
      sine = Trig_Sin(slot->s.phase - 4000);
    } while (0);
    angle = Iwram_MulQ16(sine, slot->s.swing + slot->s.swing * 5 / 2);
    leader->x = rootX - Iwram_MulQ16(Trig_Sin(angle), radius);
    leader->y = rootY - Iwram_MulQ16(Trig_Cos(angle), radius);
    leader->targetX = leader->x;
    leader->targetY = leader->y;
    heldSprite->unk12 = angle;
    wait(1);
  }
  slot->s.active = 1;
  Actor_SetMode(leader, 7);
  leader->motionFlags = motionFlags & ~1;
  leader->velocityY = Q16_ONE * 4;
  for (i = 7; i >= 0; i--) {
    slot->s.spacing -= Q16_ONE / 16;
    wait(1);
  }
  slot->s.spacing = Q16_ONE * 14;
  heldSprite->unk12 = 0;
  Actor_SetMode(leader, 6);
  Actor_SetSingleChildFlagsFar(leader, 1);
  leader->motionFlags = motionFlags;
  wait(6);
  slot->s.active = 1;
}

/// Starts an object's chain (Func_080daecc). An object with no motion flags is
/// first moved -16 pixels in z and 64 pixels in y; one with motion
/// flags starts it only when force is set.
void Func_080db444(s32 id, s32 force) {
  struct Actor *object = Actor_GetById(id);
  s32 y;
  s32 zero;

  if (object != NULL) {
    if (object->motionFlags == 0) {
      zero = 0;
      object->z += 0xfff00000;
      y = object->y;
      object->y = y + 0x400000;
      Func_080daecc(id, zero);
    } else if (force) {
      Func_080daecc(id, 0);
    }
  }
}

// Field Psynergy: the dispatchers that run each animation


/// The Psynergy animations (BattleFxScene.animation) the dispatchers name.
// The Broken Seal has 1-16; ⚓️ adds 17-30.
enum FieldPsynergyAnimation {
  FIELD_FX_PUSH = 1,  ///< pushes the target the way the pad points
  FIELD_FX_WAVE = 2,  ///< the hue wave from the caster to the target
  FIELD_FX_PAUSE = 9, ///< stops an object in its tracks
};

void BattleEffect_Run(void);
void BattleEffect_DispatchRequestKind(void);
void BattleEffect_ClearChildValueOnMismatch(void);
void FieldEvent_RunTypeHandler(void);

void FieldEffect_RunMove(void);
void Func_080ddea8(void);
void Func_080df6f4(void);
void Func_080de21c(void);
void FieldEffect_RunDouse(void);
void FieldEffect_RunWhirlwind(void);
void FieldEffect_RunFrost(void);
void FieldEffect_RunPly(void);
void FieldEffect_RunGrowth(void);
void Func_080e03cc(void);
void FieldEffect_RunRetreat(void);
void BattleEffect_RunEffect16(void);
void Func_080decb8(void);
void Func_080dd820(void);
void Func_080e09c0(void);
void Func_080e1f2c(void);
void Func_080e3074(void);
void Func_080e3d04(void);
void Func_080e572c(void);
void Func_080e5d5c(void);
void Func_080e68d0(void);
void Func_080e9090(void);
void BattleEffect_RunViewSway(void);
void Func_080e9af4(void);
void Func_080e4a34(void);
void BattleEffect_RunEffect30(void);
void Func_080e7844(void);
void Func_080e82cc(void);
void BattleEffect_ResumeObject(s32 id);
s32 BattleEffect_FindNearestObject(s32 self, s32 animation);
s32 BattleEffect_FilterObjectIdByFlags(s32 id);
void *Scene_FindActorTalkTrigger(s32 id);
void BattleEffect_SetupObjectPair(s32 userId, s32 targetId);
void BattleEffect_MarkChildAndRunFallbackTransition(s32 id);
void BattleEffect_PauseObject(s32 id);
void Func_080e0134(void);
void Func_080dd950(void);
void Func_080dc978(s32 sourceId, s32 targetId);
void Func_080dd054(s32 targetId);
void FieldEffect_RunLiftOnTarget(s32 targetId);
void BattleEffect_SteerLiftedTarget(s32 targetId);
void FieldEffect_RunForceOnTarget(s32 targetId);
void FieldEffect_RunDouseOnTarget(s32 targetId);
void FieldEffect_RunFrostOnTarget(s32 targetId);
void Func_080dda30(s32 targetId);
void FieldEffect_RunPlyOnTarget(s32 targetId);
/* VIEW(f0daecc): routine FieldEffect_RunWhirlwindOnTarget as void FieldEffect_RunWhirlwindOnTarget(s32 targetId); battle.h has void FieldEffect_RunWhirlwindOnTarget(void); tried as the definition's void FieldEffect_RunWhirlwindOnTarget(void), which the object already declares, it does not compile in tla-ja: games/tla/src/field/object/visual_attributes.c:7369: too many arguments to function `FieldEffect_RunWhirlwindOnTarget'. */
extern void FieldEffect_RunWhirlwindOnTarget__f0daecc(s32 targetId) __asm__("FieldEffect_RunWhirlwindOnTarget");
void BattleEffect_CallEffect13(s32 targetId);
void Func_080e0978(s32 targetId);
void BattleEffect_CallEffect23(s32 targetId);
void BattleEffect_CallEffect26(s32 targetId);
void BattleEffect_CallSpinEffect(s32 targetId);
void BattleEffect_CallEffect20(s32 targetId);
void Func_080e8db0(s32 targetId);
void Func_080e92b4(s32 targetId);
void BattleEffect_CallEffect24(s32 targetId);
void Func_080e7818(s32 targetId);
void BattleEffect_CallEffect22(s32 targetId);
void BattleEffect_CallEffect21(s32 targetId);
void BattleEffect_CallEffect18(s32 targetId);

/// Runs the animation's handler for kind: 0 runs it with no target chosen, 1
/// on the chosen target (secondObjectId), 2 finishes it and 3 runs the kind's
/// extra step.
void Field_DispatchTypeHandler(s32 kind) {
  if (kind == 0)
    BattleEffect_Run();
  else if (kind == 1)
    BattleEffect_DispatchRequestKind();
  else if (kind == 2)
    BattleEffect_ClearChildValueOnMismatch();
  else if (kind == 3)
    FieldEvent_RunTypeHandler();
}

/// Runs the field Psynergy animation the scene names, with no target chosen
/// yet: each kind runs its effect to the end.
void BattleEffect_Run(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->fieldKind;
  s32 targetId = scene->secondObjectId;
  s32 objectId;

  if (animation == FIELD_FX_PUSH)
    FieldEffect_RunMove();
  else if (animation == 7)
    Func_080ddea8();
  else if (animation == 11)
    Func_080df6f4();
  else if (animation == 4)
    Func_080de21c();
  else if (animation == 5)
    FieldEffect_RunDouse();
  else if (animation == 14)
    FieldEffect_RunWhirlwind();
  else if (animation == 6)
    FieldEffect_RunFrost();
  else if (animation == 3)
    FieldEffect_RunPly();
  else if (animation == 12)
    FieldEffect_RunGrowth();
  else if (animation == 13)
    Func_080e03cc();
  else if (animation == 15)
    FieldEffect_RunRetreat();
  else if (animation == 16)
    BattleEffect_RunEffect16();
  else if (animation == 10)
    Func_080decb8();
  else if (animation == 8)
    Func_080dd820();
  else if (animation == 17)
    Func_080e09c0();
  else if (animation == 29)
    Func_080e1f2c();
  else if (animation == 25)
    Func_080e3074();
  else if (animation == 26)
    Func_080e3d04();
  else if (animation == 21)
    Func_080e572c();
  else if (animation == 18)
    Func_080e5d5c();
  else if (animation == 20)
    Func_080e68d0();
  else if (animation == 19)
    Func_080e9090();
  else if (animation == 27)
    BattleEffect_RunViewSway();
  else if (animation == 24)
    Func_080e9af4();
  else if (animation == 23)
    Func_080e4a34();
  else if (animation == 30)
    BattleEffect_RunEffect30();
  else if (animation == 22)
    Func_080e7844();
  else if (animation == 28)
    Func_080e82cc();
  else if (animation == FIELD_FX_PAUSE) {
    // Let the object stopped last time go, then stop the nearest one that
    // has an event to run; with none, play the fallback transition.
    struct MainRoutineWork *party = &gMainRoutineWork;
    s32 paused;

    paused = party->pausedObject;
    if (paused != -1) {
      BattleEffect_ResumeObject(paused);
      party->pausedObject = -1;
    }
    objectId = BattleEffect_FindNearestObject(scene->firstObjectId, animation);
    objectId = BattleEffect_FilterObjectIdByFlags(objectId);
    if (Scene_FindActorTalkTrigger(objectId) != 0) {
      BattleEffect_SetupObjectPair(scene->firstObjectId, objectId);
      BattleEffect_MarkChildAndRunFallbackTransition(objectId);
      BattleEffect_PauseObject(objectId);
      party->pausedObject = objectId;
    } else {
      Func_080e0134();
    }
  } else if (animation == FIELD_FX_WAVE) {
    // A wave still running from the last cast is reset first.
    if (*(s8 *)(event + 0xca4) != 0)
      Func_080dd950();
    Func_080dc978(scene->firstObjectId, targetId);
  }
}

/// Runs the field Psynergy animation the scene names on the target the player
/// chose (BattleFxScene.secondObjectId).
void BattleEffect_DispatchRequestKind(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;
  u8 *event = Ram_HeapSlots->eventWork;
  s32 animation = scene->fieldKind;
  s32 targetId = scene->secondObjectId;

  scene->enabled = 0;
  if (animation == FIELD_FX_PUSH)
    Func_080dd054(targetId);
  else if (animation == 7)
    FieldEffect_RunLiftOnTarget(targetId);
  else if (animation == 11)
    BattleEffect_SteerLiftedTarget(targetId);
  else if (animation == 4)
    FieldEffect_RunForceOnTarget(targetId);
  else if (animation == 5)
    FieldEffect_RunDouseOnTarget(targetId);
  else if (animation == 6)
    FieldEffect_RunFrostOnTarget(targetId);
  else if (animation == 12)
    Func_080dda30(targetId);
  else if (animation == 3)
    FieldEffect_RunPlyOnTarget(targetId);
  else if (animation == 14)
    FieldEffect_RunWhirlwindOnTarget__f0daecc(targetId);
  else if (animation == 13)
    BattleEffect_CallEffect13(targetId);
  else if (animation == 16)
    BattleEffect_RunEffect16();
  else if (animation == 17)
    Func_080e0978(targetId);
  else if (animation == 10)
    Func_080decb8();
  else if (animation == 15)
    FieldEffect_RunRetreat();
  else if (animation == 8)
    Func_080dd820();
  else if (animation == 23)
    BattleEffect_CallEffect23(targetId);
  else if (animation == 26)
    BattleEffect_CallEffect26(targetId);
  else if (animation == 25)
    BattleEffect_CallSpinEffect(targetId);
  else if (animation == 20)
    BattleEffect_CallEffect20(targetId);
  else if (animation == 19)
    Func_080e8db0(targetId);
  else if (animation == 27)
    Func_080e92b4(targetId);
  else if (animation == 24)
    BattleEffect_CallEffect24(targetId);
  else if (animation == 30)
    Func_080e7818(targetId);
  else if (animation == 22)
    BattleEffect_CallEffect22(targetId);
  else if (animation == 21)
    BattleEffect_CallEffect21(targetId);
  else if (animation == 18)
    BattleEffect_CallEffect18(targetId);
  else if (animation == 28)
    Func_080e82cc();
  else if (animation == FIELD_FX_WAVE) {
    // A wave still running from the last cast is reset first; a target that
    // is not the paused object stops walking while the wave runs.
    struct MainRoutineWork *party;

    if (*(s8 *)(event + 0xca4) != 0)
      Func_080dd950();
    party = &gMainRoutineWork;
    if (party->pausedObject != scene->secondObjectId)
      ((struct Actor *)scene->child)->movementState = 1;
    Func_080dc978(scene->firstObjectId, targetId);
  } else if (animation == FIELD_FX_PAUSE) {
    // Let the object stopped last time go, then stop the target.
    struct MainRoutineWork *party = &gMainRoutineWork;

    if (party->pausedObject != -1) {
      BattleEffect_ResumeObject(party->pausedObject);
      party->pausedObject = -1;
    }
    BattleEffect_PauseObject(targetId);
    party->pausedObject = targetId;
    BattleEffect_MarkChildAndRunFallbackTransition(targetId);
  }
}


void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position);

struct CompositeAnimation {
  u8 unk00[0x11];
  u8 displayFlags;
  u8 unk12[7];
  u8 dirty;
  u8 unk1a;
  u8 count;
  u8 unk1c[0x0c];
  struct AnimationEntry *entries[4];
};

void Func_080debd8(void);
void FieldEvent_ShowStatusMessage(void);
void Func_080e4730(void);
void Func_080e9db4(void);
void Func_080e87c4(void);

void BattleEffect_FinishSceneAndReleaseHeapBlock(void);

/// unk26a from main_routine_work.h.
struct PartyPauseView {
  u8 unk000[0x26a];
  s16 pausedObject;
};

void BattleEffect_ClearChildValueOnMismatch(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  if (scene->fieldKind == 2) {
    struct PartyPauseView *party;

    BattleEffect_FinishSceneAndReleaseHeapBlock();
    party = (struct PartyPauseView *)&gMainRoutineWork;
    if (party->pausedObject != scene->secondObjectId)
      ((struct Actor *)scene->child)->movementState = 0;
  }
}

/// Additional processing for each type.
void FieldEvent_RunTypeHandler(void) {
  struct BattleFxScene *scene = Ram_HeapSlots->effectWork;

  switch (scene->fieldKind) {
  case 8:
    Func_080dd950();
    break;
  case 10:
    Func_080debd8();
    break;
  case 16:
    FieldEvent_ShowStatusMessage();
    break;
  case 23:
    Func_080e4730();
    break;
  case 24:
    Func_080e9db4();
    break;
  case 28:
    Func_080e87c4();
    break;
  }
}

void Actor_ApplyRandomChildValues(struct Actor *owner) {
  s32 state;
  struct CompositeAnimation *target;
  s32 initialCount;
  s32 count;
  struct AnimationEntry **entry;
  struct AnimationEntry *current;
  u32 frame;
  s32 value;

  state = owner->animationKind;
  if (state == 1) {
    target = owner->sprite;
    if (target != 0 && (target->displayFlags & state) == 0) {
      initialCount = target->count;
      if (initialCount != 0) {
        entry = target->entries;
        frame = gFrameCount;
        count = initialCount;
        do {
          current = *entry++;
          value = frame % 6;
          count--;
          current->param = value;
        } while (count != 0);
      }
      target->dirty = 1;
    }
  }
}

void Motion_SetTargetPositionFromMagnitudeAngle(struct Actor *object,
                                                s32 magnitude, s32 angle) {
  s32 values[3];

  if (object != 0) {
    SET_VECTOR(values, object->x, object->y, object->z);
    Vector_AddPolarOffset(magnitude, angle, values);
    Actor_SetTargetPosition(object, values[0], values[1], values[2]);
  }
}

struct DepthPart {
  u8 unk00[9];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 unk09_4 : 4;
};

struct DepthMapBase {
  u8 unk00[4];
  s32 base;
};

struct DepthScroll {
  s32 unk00;
  s32 offset;
};

struct DepthMap {
  struct DepthMapBase *base;
  u8 unk04[0xe0];
  struct DepthScroll scroll;
};

s32 Render_ProjectPoint(const void *point, s32 *screen);
void Func_080eb01c(void *work);

s32 Func_080db9a8(struct Actor *object, s32 kind) {
  if (kind == 0xc000)
    return ((struct DepthPart *)object->sprite)->priority;
  return 0;
}

/// Returns the 2-bit OBJ priority of the object's animation record.
s32 BattleEffect_GetObjectPriority(struct Actor *object) {
  return ((struct DepthPart *)object->sprite)->priority;
}

/// Returns the object's sort depth: from its projected z on the world map
/// (mode 3), otherwise from its y and z against the map's scroll.
s32 BattleEffect_GetObjectDepth(struct Actor *object) {
  s32 point[3];
  s32 screen[3];
  s32 depth;
  s32 *out;

  if (*((s8 *)Ram_HeapSlots->eventWork + 394) == 3) {
    point[0] = object->x;
    out = screen;
    point[1] = object->y;
    point[2] = object->z;
    Render_ProjectPoint(point, out);
    depth = (0x200 - out[2]) / 2 + 128;
    if (depth <= 0)
      depth = 1;
  } else {
    struct DepthMap *map = (struct DepthMap *)Ram_HeapSlots->mapWork;
    struct DepthScroll *scroll = &map->scroll;
    s32 bias = scroll->offset & 0xffff0000;
    s32 base = map->base->base;

    depth = ((object->y - base + object->z - base - bias) >> 16) + 62;
  }
  return depth;
}

/// Per-frame callback: draws the effect scene's sprite (effect work +0x7c8).
void BattleEffect_DrawSceneSprite(void) {
  Func_080eb01c((u8 *)Ram_HeapSlots->effectWork + 0x7c8);
}


struct DepthSprite {
  u8 unk00[5];
  u8 unk05_0 : 5;
  u8 hidden : 1;
  u8 unk05_6 : 2;
  u8 unk06[3];
  u8 unk09_0 : 2;
  u8 priority : 2;
  u8 layer : 4;
  u8 unk0a[2];
  s32 x;
  s32 y;
  u8 unk14[4];
  s32 scale;
  u8 unk1c[2];
  u16 frame;
  u8 unk20[8];
};

struct DepthScene {
  u16 mode;
  u8 unk02[2];
  s32 x;
  u8 unk08[8];
  struct Actor *mainObject;
  u8 unk14[0x7b4];
  struct DepthSprite sprite;
  s16 vramSlot;
  s16 active;
};

void Func_080eaf98(struct DepthSprite *sprite, s32 a, s32 b, u32 c, s32 tiles);

void Func_080dba5c(void) {
  struct DepthScene *scene = Ram_HeapSlots->effectWork;
  struct Actor *mainObject;
  struct DepthSprite *sprite;
  s32 position[3];
  void *buffer;
  s32 slot;
  s32 tiles;

  mainObject = scene->mainObject;
  if (scene->mode == 0xc000) {
    scene->active = 1;
    buffer = Heap_Alloc(0x200);
    // An empty once-only loop keeps the zero kept in sl after the fill's
    // arguments; without it, 12 of 103 instructions differ in tla-en.
    do {
    } while (0);
    Iwram_FillWords(buffer, 0x200, 0);
    slot = Resource_FindFreeEntry();
    tiles = VramCache_Load(slot, 0x200, buffer);
    scene->vramSlot = slot;
    Heap_Release(buffer);
    sprite = &scene->sprite;
    Func_080eaf98(sprite, 16, 31, 0x80000000, tiles);
    sprite->frame = 0;
    sprite->priority = 0;
    sprite->hidden = 0;
    sprite->layer = 0;
    sprite->scale = 0x20000;
    position[0] = scene->x;
    position[1] = mainObject->y + 0x1c0000;
    position[2] = mainObject->z;
    Camera_WorldToScreen(position);
    sprite->x = position[0];
    sprite->y = position[2];
    Task_Add(BattleEffect_DrawSceneSprite, TASK_PASS_VBLANK);
  }
}
#undef CELL_MASK
#undef SIDE_TURN
