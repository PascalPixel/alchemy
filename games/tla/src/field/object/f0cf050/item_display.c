/// @file
/// Showing a found item, and Reveal's sparkles on hidden things.
// Part of f0cf050.c.

#include "../f0cf050_internal.h"

struct ParticleEmitter {
  u8 padding[8];
  struct ParticlePosition position;
  u8 padding2[20];
  s32 travelOffset;
  u8 padding3[41];
  u8 active;
};

/* VIEW(emitter): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position); f0cf050 has void Vector_AddPolarOffset(s32 distance, u16 angle, s32 *position); tried as the definition's void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position), it does not compile in tla-ja: games/tla/src/field/object/f0cf050.c:56: conflicting types for `Vector_AddPolarOffset'. */
extern void Vector_AddPolarOffset__emitter(s32 radius, s32 angle, s32 *position) __asm__("Vector_AddPolarOffset");
struct ParticleEffectObject *Actor_Spawn(s32, s32, s32, s32);

void Marker_MoveLeaderAwayFromEmitter(struct ParticleEmitter *emitter) {
  struct ParticlePosition position;
  struct ParticleEffectObject *object;
  u32 randomAngle;

  if (emitter->travelOffset >= -255 && emitter->travelOffset <= 255)
    emitter->active = 0;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.x = emitter->position.x;
  position.y = emitter->position.y;
  position.z = emitter->position.z;
  randomAngle = Random16();
  Vector_AddPolarOffset__emitter(randomAngle << 4, Random16(), &position);
  object = Actor_Spawn(0x2a1, position.x, position.y, position.z);
  if (object != 0) {
    s32 mask;
    u8 flags;

    Actor_StartScriptFar((struct Actor *)object,
                         (u32)gBattleEffect_ParticleScript);
    Actor_SetMode(object, 0);
    mask = 13;
    flags = object->child->flags;
    mask = -mask;
    mask &= flags;
    mask |= 4;
    object->child->flags = mask;
  }
}

struct RevealWork {
  u8 unk000[0xca4];
  s8 revealActive;
};

/// Returns 1 while Reveal is active, else 0.
// As ☀️'s Field_IsRevealActive.
s32 Field_IsRevealActive(struct ScriptInterpreter *interpreter) {
  s32 revealState;
  u32 isActive = 0;
  struct RevealWork *work = Ram_HeapSlots->eventWork;

  if (work != NULL) {
    revealState = work->revealActive;
    isActive = (0 - revealState) | revealState;
    isActive >>= 31;
  }
  return isActive;
}


struct EmitterWork {
  u8 unk000[0x14];
  struct Actor *actors[(0xca4 - 0x14) / 4];
  s8 revealActive;
};

struct EffectVisual {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[2];
  u16 tile : 10;
  u16 attributes : 6;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};

extern const u8 gData_080effd8[];
extern const u8 Data_080f01b8[];
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Func_08038248(s32 item);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
/* VIEW(emitter): routine BattleEffect_SpawnRandomParticleAtPosition as void BattleEffect_SpawnRandomParticleAtPosition(void); the definition has void BattleEffect_SpawnRandomParticleAtPosition(const struct ParticleSource *source); tried as the definition's void BattleEffect_SpawnRandomParticleAtPosition(const struct ParticleSource *source), which the object already declares, it does not compile in tla-ja: games/tla/src/field/object/f0cf050.c:168: `BattleEffect_SpawnRandomParticleAtPosition' undeclared (first use in this function). */
extern void BattleEffect_SpawnRandomParticleAtPosition__emitter(void) __asm__("BattleEffect_SpawnRandomParticleAtPosition");

/// Shows the picture of item above actor (in front of the controlled owner,
/// on the centre of the cell), clearing Reveal's sparkles first.
struct Actor *BattleEffect_StartRandomParticleEmitter(s32 actor, s32 item) {
  struct EmitterWork *work = Ram_HeapSlots->eventWork;
  struct Actor *picture = work->actors[actor];
  struct Actor *object;
  struct EffectVisual *visual;
  s32 position[3];
  u8 *buffer;
  s32 tile;
  s32 i;
  s32 zero;
  s32 mask;

  if (picture == NULL)
    return NULL;
  position[0] = picture->x;
  position[1] = picture->y;
  position[2] = picture->z;
  if (actor == Party_GetLeaderActor()) {
    Vector_AddPolarOffset__emitter(0x100000, picture->facing, position);
    position[0] = (position[0] & 0xfff00000) + 0x80000;
    position[2] = (position[2] & 0xfff00000) + 0x80000;
  }
  if (work->revealActive != 0) {
    object = Ram_HeapSlots->scriptObjects;
    i = 63;
    do {
      if (object->script != NULL) {
        if (object->update == BattleEffect_SpawnRandomParticleAtPosition__emitter)
          Actor_Destroy(object);
        if (object->script == (s32 *)gBattleEffect_ParticleScript)
          Actor_Destroy(object);
      }
      i--;
      object++;
    } while (i >= 0);
  }
  wait(3);
  picture =
      Func_080200c0(489, position[0], position[1] + 0x100000, position[2]);
  if (picture == NULL)
    return NULL;
  Actor_StartScriptFar(picture, gData_080effd8);
  visual = picture->sprite;
  zero = 0;
  visual->value1a = zero;
  visual->value1b = zero;
  visual->flags05 &= zero - 33;
  mask = ((u8 *)visual)[9] & 0x0f;
  {
    s32 clear = 13;

    clear = -clear;
    mask &= clear;
  }
  mask |= 4;
  ((u8 *)visual)[9] = mask;
  picture->motionFlags = 2;
  picture->terrainHeight = position[1];
  picture->velocityY = 0x20000;
  picture->gravity = 0x4000;
  buffer = HeapSlot_Alloc(68, 0x608);
  Func_08038248(item);
  tile = VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
  visual->tile = tile;
  HeapSlot_Release(68);
  picture->update = Marker_MoveLeaderAwayFromEmitter;
  if (work->revealActive != 0) {
    struct Actor *marker = Ram_HeapSlots->scriptObjects;
    s32 j;

    for (j = 0; j <= 63; j++, marker++) {
      if (marker->script != NULL && marker->script == (s32 *)Data_080f01b8 &&
          marker->x >> 20 == picture->x >> 20 &&
          marker->z >> 20 == picture->z >> 20) {
        Actor_Destroy(marker);
        break;
      }
    }
  }
  return picture;
}

void Actor_ReleaseIfPresent(struct Actor *object) {
  if (object != 0)
    Actor_Destroy(object);
}

/// Nothing in the image calls it.
void Func_080cf34c(void) {}


s32 Party_GetLeaderActor(void);
/* VIEW(rising): routine Marker_MoveLeaderAwayFromEmitter as void Marker_MoveLeaderAwayFromEmitter(void); the definition has void Marker_MoveLeaderAwayFromEmitter(struct ParticleEmitter *emitter). */
extern void Marker_MoveLeaderAwayFromEmitter__rising(void) __asm__("Marker_MoveLeaderAwayFromEmitter");
void Actor_SetSpritePriorityDirectFar(struct Actor *object, s32 priority);
extern const u8 gData_080effd8[];
extern const u8 gData_080f0074[];

/// Places the object 36 pixels above the controlled owner, gives it its
/// sprite priority and, three frames on, the descent script, and turns the
/// owner to face a quarter turn.
void ItemDisplay_RaiseAboveLeader(struct Actor *actor) {
  struct Actor *owner;

  if (actor == 0)
    return;

  {
    struct Actor *found = ObjectTable_Get(Party_GetLeaderActor());
    actor->acceleration = Q16_ONE;
    actor->speedLimit = 0x20000;
    actor->motionFlags = 0;
    owner = found;
  }
  Actor_SetTargetPosition(actor, owner->x, owner->y + 0x240000, owner->z);
  Actor_SetSpritePriorityDirectFar(actor, 1);
  wait(3);
  Actor_SetMode(owner, 28);
  Actor_StartScriptFar(actor, gData_080f0074);
  owner->facing = 0x4000;
}

void BattleEffect_RunActorRiseAndDescent(struct Actor *object, s32 flags) {
  struct Actor *owner;

  if (object != 0) {
    owner = ObjectTable_Get(Party_GetLeaderActor());
    if (flags & 1) {
      Actor_SetSingleChildFlagsFar(object, 0);
      Actor_StartScriptFar(object, gData_080effd8);
      object->velocityY = 0x20000;
      object->gravity = 0x4000;
      object->update = Marker_MoveLeaderAwayFromEmitter__rising;
    }
    if (flags == 3)
      wait(60);
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(object);
    if (flags == 3)
      wait(80);
    Actor_SetMode(owner, 1);
  }
}

/* VIEW(rising): tag struct EffectVisual as struct EffectVisual__rising; f0cf050.c has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/field/object/f0cf050.c:310: structure has no member named `flags09'. */
struct EffectVisual__rising {
  u8 unk00[5];
  u8 flags05;
  u8 unk06[3];
  u8 flags09;
  u8 unk0a[6];
  u8 tileSlot;
  u8 unk11[9];
  u8 value1a;
  u8 value1b;
};
struct Actor *Func_080200c0(s32 kind, s32 x, s32 y, s32 z);
void Func_08038248(s32 item);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
void Actor_Destroy(struct Actor *object);

void BattleEffect_StartEffectObject22(s32 item, s32 flags) {
  struct Actor *owner = ObjectTable_Get(Party_GetLeaderActor());
  u8 *buffer = HeapSlot_Alloc(68, 0x608);
  struct Actor *object =
      Func_080200c0(489, owner->x, owner->y + 0x240000, owner->z);

  if (object != 0) {
    struct EffectVisual__rising *visual = object->sprite;

    s32 mask;
    s32 zero = 0;

    visual->value1a = zero;
    visual->value1b = zero;
    visual->flags05 &= zero - 33;

    mask = visual->flags09 & 0x0f;
    {
      s32 clear = 13;

      clear = -clear;
      mask &= clear;
    }
    mask |= 4;
    visual->flags09 = mask;

    Func_08038248(item);
    VramCache_Load(visual->tileSlot, 128, buffer + 0x400);
    HeapSlot_Release(68);

    if (flags & 1)
      object->update = Marker_MoveLeaderAwayFromEmitter__rising;
    if (flags & 2)
      ItemDisplay_RaiseAboveLeader(object);

    wait(80);
    Actor_SetMode(owner, 1);
    Actor_Destroy(object);
  }
}


struct ParticleSource {
  u8 padding[8];
  struct ParticlePosition position;
};

/* VIEW(rising): routine Vector_AddPolarOffset as void Vector_AddPolarOffset(s32, s32, struct ParticlePosition *); f0cf050 has void Vector_AddPolarOffset(s32 distance, u16 angle, s32 *position); tried as the definition's void Vector_AddPolarOffset(s32 radius, s32 angle, s32 *position), it does not compile in tla-ja: games/tla/src/field/object/f0cf050.c:353: conflicting types for `Vector_AddPolarOffset'. */
extern void Vector_AddPolarOffset__rising(s32, s32, struct ParticlePosition *) __asm__("Vector_AddPolarOffset");
struct ParticleEffectObject *Actor_Spawn(s32, s32, s32, s32);

void BattleEffect_SpawnRandomParticleAtPosition(
    const struct ParticleSource *source) {
  struct ParticlePosition position;
  struct ParticleEffectObject *object;
  u32 random;

  if ((100 * Random16() >> 16) > 9)
    return;

  position.x = source->position.x;
  position.y = source->position.y;
  position.z = source->position.z;
  random = Random16();
  Vector_AddPolarOffset__rising(random << 4, Random16(), &position);
  object = Actor_Spawn(0x2a1, position.x, position.y, position.z);
  if (object != 0) {
    s32 mask;
    u8 flags;

    Actor_StartScriptFar((struct Actor *)object,
                         (u32)gBattleEffect_ParticleScript);
    Actor_SetMode(object, 0);
    mask = 13;
    flags = object->child->flags;
    mask = -mask;
    mask &= flags;
    mask |= 4;
    object->child->flags = mask;
  }
}

// Places the scene's markers from its scene item rows (a type, an event id, a
// flag and a pad): for each of the first 16 rows whose flag is clear and whose
// event has not run, a marker object goes at its event object, or at the map
// cell of a tile event, when that point is on screen.

struct Actor;

s16 *Func_080ceba8(void);
s32 Func_080cec24(s32 value, s32 *out);
struct Actor *ObjectEvent_GetObject(u32 objectId);

/* VIEW(rising): variable Data_080f01b8 as extern const s32 Data_080f01b8[]; emitter has extern const u8 Data_080f01b8[]. */
extern const s32 Data_080f01b8__rising[] __asm__("Data_080f01b8");

/// Places a marker at each waiting scene item event that is on screen.
void Func_080cf554(void) {
  // The row type is s16, as the rows are: with it the copied head test stays
  // apart from the bottom one.
  s16 *entry = Func_080ceba8();
  u8 *map = (u8 *)Ram_HeapSlots->mapWork;
  s32 *origin = (s32 *)(map + 228);
  s32 originX = origin[0] & 0xffff0000;
  s32 originZ = origin[1] & 0xffff0000;
  s32 result[3];
  // volatile: headerWord lives at sp+4, stored once; as a plain s32, 86 of
  // 193 instructions differ in tla-en.
  volatile s32 headerWord;
  struct Actor *object;
  s16 type;
  s32 id;
  s32 flag;
  s32 i;
  s32 x;
  s32 y;
  s32 z;
  s32 dx;
  s32 dz;

  // DEAD: the map header's second word is never read; the ROM keeps its load
  // and store.
  headerWord = (*(s32 **)map)[1];
  if (entry == NULL)
    return;
  for (i = 0; i <= 15 && (type = *entry++) != 0 && !(type & 0x4000); i++) {
    id = entry[0];
    flag = entry[1];
    entry += 3;
    if (type != 128 && flag != -1 && GameFlag_Test(flag))
      continue;
    if (Func_080cec24(id, result))
      continue;
    object = ObjectEvent_GetObject(i + 64);
    if (object == NULL)
      continue;
    // A tile event's object holds its map cell, marked by this value.
    if (object->unk4c == 0x31415927) {
      x = object->timer << 16;
      z = (s16)object->unk66 << 16;
      y = MapGrid_GetTerrainHeightFar(0, x, z);
    } else {
      if ((u16)(type - 128) <= 1)
        continue;
      if (type == 130 || type == 132 || type == 133 || type == 131)
        continue;
      x = object->x;
      y = object->y;
      z = object->z;
    }
    // Both differences are taken before either is shifted; shifted as each is
    // taken, 2 of 193 instructions differ in tla-en.
    dx = x - originX;
    dz = z - originZ - y;
    dx >>= 16;
    dz >>= 16;
    if ((u32)(dx + 15) > 270 || (u32)(dz + 47) > 238)
      continue;
    if (flag == -1 || GameFlag_Test(flag))
      continue;
    object = Func_080200c0(673, x, y, z);
    if (object != NULL) {
      object->motionFlags = 0;
      Actor_StartScriptFar((struct Actor *)object, Data_080f01b8__rising);
      Actor_SetSingleChildFlagsFar((struct Actor *)object, 0);
      Actor_SetMode(object, 1);
      ((struct FieldSprite *)object->sprite)->priority = 1;
    }
  }
}


/// Each frame, before drawing: turns windows 0 and 1 on, writes the first four
/// values of the shown page into WININ, WINOUT, WIN0H and WIN1H, and has DMA 0
/// copy the rest into WIN0H and WIN1H one word each HBlank.
// As ☀️'s FieldEffect_StartWindowHBlankDma does.
void BattleEffect_StartWindowHBlankDma(void) {
  struct ScreenTransitionState *work;
  u16 *src;
  volatile u16 *channel;
  s32 in;
  s32 out;
  s32 other;

  work = Ram_HeapSlots->screenTransition;
  src = work->pages[work->page];
  channel = REG_DMA0;
  // Stop the previous transfer before priming the first line's registers.
  channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
  channel[5] &= ~DMA_ENABLE;
  (void)channel[5];
  REG_DISPCNT |= DISPCNT_WIN0_ON | DISPCNT_WIN1_ON;
  // The load in a once-only block comes before the register address is
  // built; as the plain load, 2 of 68 instructions differ in tla-en.
  do {
    in = *src;
  } while (0);
  REG_WININ = in;
  src++;
  /* FAKEMATCH(temporary): WINOUT's value read into out before the register
   * address is built; as REG_WINOUT = *src, 10 of 68 instructions differ in
   * tla-en. */
  out = *src;
  REG_WINOUT = out;
  src++;
  REG_WIN0H = *src;
  src++;
  /* FAKEMATCH(temporary): WIN1H's value read into other before the register
   * address is built; as REG_WIN1H = *src, 8 of 68 instructions differ in
   * tla-en. */
  other = *src;
  REG_WIN1H = other;
  src++;
  // other is reused for both vertical ranges; WIN_RANGE(0, 160) at each
  // store differs.
  other = WIN_RANGE(0, 160);
  REG_WIN0V = other;
  REG_WIN1V = other;
  // HBlank repeats one word: both horizontal window bounds for the next line.
  Dma_Set(
      src, (void *)&REG_WIN0H,
      (DMA_ENABLE | DMA_START_HBLANK | DMA_REPEAT | DMA_32BIT | DMA_DEST_RELOAD)
              << 16 |
          1,
      (volatile u32 *)channel);
}
