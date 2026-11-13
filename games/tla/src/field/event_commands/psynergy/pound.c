/// @file
/// Two routines not yet understood.
// Part of psynergy.c.

#include "../psynergy_internal.h"

/* VIEW(f0e5d5c): tag struct ShineParticle as struct ShineParticle__f0e5d5c;
 * f0e2c38.c has another definition; tried with the definition the rest of the
 * object has, it does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:2599: structure has no member
 * named `velocityX'. */
struct ShineParticle__f0e5d5c {
  s32 x;
  s32 y;
  s32 unk08;
  s32 velocityX;
  s32 velocityY;
  s32 unk14;
  s32 timer;
};

/* VIEW(f0e5d5c): tag struct ShineWork as struct ShineWork__f0e5d5c; f0e2c38.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:2567: structure has no member
 * named `glow'. */
struct ShineWork__f0e5d5c {
  u8 tiles[0x1000];
  struct ShineParticle__f0e5d5c particles[32];
  struct EffectSprite sprites[32];
  struct EffectSprite glow;
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  s8 fading;
};

/* VIEW(f0e5d5c): tag struct FxScene as struct FxScene__f0e5d5c; f0e2c38.c has
 * another definition; tried with the definition the rest of the object has, it
 * does not compile in tla-ja:
 * games/tla/src/field/event_commands/psynergy.c:2485: structure has no member
 * named `facing'. */
struct FxScene__f0e5d5c {
  u16 facing;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
  u8 unk21[0x7a3];
  s16 fading;
  s16 frame;
};

extern char ResourceId_GlowDisc, ResourceId_GoldStar;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void FieldPsynergy_FadeToCastColor(void);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_ReleaseFar(struct Actor *object);
void Func_080dba5c(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Decompress_TaggedLz(const void *source, void *destination);
s32 VramCache_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 VramCache_FreeEntry(u32 slot);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void wait(s32 frames);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z);
void BattleEffect_SinkAndShrinkObject(void);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
s32 Func_080ce31c(s32 kind);
void FieldPsynergy_FadeFromCastColor(void);
void Func_080e5a64(void);

void FieldPsynergy_Pound(void) {
  struct ShineWork__f0e5d5c *work = HeapSlot_Alloc(92, 0x18b4);
  struct FxScene__f0e5d5c *scene = Ram_HeapSlots->effectWork;
  struct FxEvent *event = Ram_HeapSlots->eventWork;
  struct Actor *child = scene->child;
  struct Actor *caster = scene->mainObject;
  struct Actor *object;
  struct Actor *shine;
  struct CanvasSpriteWork *canvas;
  struct EffectSprite *sprite;
  struct EffectSprite *glow;
  void *pending;
  s32 pos[3];
  s32 starEntry;
  s32 starTile;
  s32 glowEntry;
  s32 inReach;
  s32 lift;
  s32 glowTile;
  s32 spot;
  s32 size = 0x800;
  s32 rise;
  s32 speed;
  s32 angle;
  s32 frame;
  s32 j;
  struct ShineParticle__f0e5d5c *particle;

  lift = 0x200000;
  if (child == NULL) {
    scene->x = caster->x;
    scene->y = caster->y;
    scene->z = caster->z;
    Vector_AddPolarOffset(lift, scene->facing, &scene->x);
  } else if (child->motionFlags & 4) {
    /* FAKEMATCH(dead): lift cleared under a test of the ground's height, a
     * store the line after next overwrites, which sends scene to r4 and the
     * 0x200000 to r0 for the y store; without it, 12 of 639 instructions
     * differ in tla-en. */
    if (child->terrainHeight == 300)
      lift = 0;
    scene->y = child->terrainHeight;
    lift = child->y - child->terrainHeight + 0x200000;
  }
  if (scene->enabled != 0) {
    // As x != -1 this splits the -1 and loses the mvns.
    spot = ~Func_080cda84(scene->fieldKind, caster, &scene->x);
    inReach = (u32)(-spot | spot) >> 31;
    if (inReach == 1) {
      for (j = 0; j < 80; j++) {
        object = event->objects[j];
        if (object != NULL && object->script != NULL &&
            (object->collisionFlags & 1) &&
            Func_080dbe80(&scene->x, 4, &object->x, object->radius - 2) >= 0)
          goto outOfReach;
      }
    }
  } else {
    inReach = 1;
    if (child != NULL && ((u8 *)child)[99] != 0) {
    outOfReach:
      inReach = 0;
    }
  }

  FieldPsynergy_FadeToCastColor();
  Sound_PlayFar(138);
  shine = Actor_Spawn(352, 0, 0, 0);
  work->object = shine;
  if (shine == NULL)
    goto end;
  shine->motionFlags = 0;
  if (*(u16 *)((u8 *)event + 0xd4c) & 2) {
    if ((shine->animationKind & 15) == 1) {
      ((struct EffectSprite *)shine->sprite)->oam.priority = 1;
      ((u8 *)shine)[35] &= 254;
    }
  } else {
    ((u8 *)shine)[35] = 1;
  }
  shine->facing = caster->facing;
  Actor_SelectAnimationFar(shine, 2);
  if (!(*(u16 *)((u8 *)event + 0xd4c) & 1))
    Func_080dba5c();

  Func_080eb824();
  canvas = Ram_HeapSlots->unkec[1];
  canvas->priority = BattleEffect_GetObjectPriority(scene->mainObject);
  canvas->depth = 1;
  canvas->mode = CANVAS_SPRITE_256_COLORS;
  canvas->scaleX = 0;
  canvas->scaleY = 0;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  canvas->x = (s16)(pos[0] >> 16);
  canvas->y = (s16)(pos[2] >> 16);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                      Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;

  // A once-only block: without it the canvas mode load is scheduled one row
  // later (6 differ).
  do {
    Iwram_FillWordsEntry(work, size, 0);
  } while (0);
  glowEntry = VramCache_FindFreeEntry();
  glowTile = VramCache_Load(glowEntry, size, work);
  glow = &work->glow;
  EffectSprite_Init(glow, 32, 32, 0xc0000000, glowTile);
  glow->oam.priority = 0;
  glow->oam.palette256 = 0;
  glow->oam.palette = 0;
  glow->depth = 0;
  glow->scaleY = glow->scaleX = 0xcccc;
  pos[0] = scene->x;
  pos[1] = scene->y;
  pos[2] = scene->z;
  Camera_WorldToScreen(pos);
  glow->x = pos[0];
  glow->y = pos[2];

  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GoldStar), work);
  starEntry = VramCache_FindFreeEntry();
  starTile = VramCache_Load(starEntry, 0x100, work);
  for (j = 0, particle = work->particles; j < 32; j++, particle++) {
    sprite = &work->sprites[j];
    speed = Random16() * 18 / 10;
    speed += 0x28000;
    angle = j * 0x1555 + (Random16() >> 5);
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Vector_AddPolarOffset(0x40000, angle, pos);
    Camera_WorldToScreen(pos);
    particle->x = pos[0];
    particle->y = pos[2];
    particle->velocityX = Iwram_MulQ16(Trig_Cos(angle), speed);
    particle->velocityY = Iwram_MulQ16(Trig_Sin(angle), speed);
    particle->timer = -j;
    EffectSprite_Init(sprite, 8, 8, 0x40000000, starTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    sprite->scaleY = sprite->scaleX = 0xcccc;
  }
  wait(1);
  Func_080dbed0(shine, scene->x, scene->y + lift, scene->z);
  Actor_SelectAnimationFar(shine, 1);
  wait(15);
  rise = 24;
  do {
    shine->y += 0x8000;
    rise--;
    wait(1);
  } while (rise >= 0);
  wait(15);
  work->state = 0;
  work->timer = 0;
  work->sparkles = 0;
  if (!inReach) {
    Task_Add(BattleEffect_SinkAndShrinkObject, 0x480);
    while (work->state != 999)
      wait(1);
    Task_Remove(BattleEffect_SinkAndShrinkObject);
    for (j = 15; j >= 0; j--)
      BattleEffect_SpawnScatterParticle(shine->x, shine->y, shine->z);
    Sound_PlayFar(114);
    wait(1);
  } else {
    pending = Scene_FindPsynergyTrigger(0x50000005, scene->fieldKind,
                                        Func_080ce31c(scene->fieldKind));
    Task_Add(Func_080e5a64, 0x480);
    frame = 0;
    while (work->state != 999) {
      wait(1);
      scene->fading = work->fading;
      scene->frame = frame;
      if (((s8 *)event)[394] != 3 && !(*(u16 *)((u8 *)event + 0xd4c) & 1)) {
        glow = &work->glow;
        EffectSprite_Draw(glow);
      }
      Field_RunPsynergyEvent(pending, work->fading, frame++);
    }
    Task_Remove(Func_080e5a64);
    wait(10);
    Sound_PlayFar(114);
    Func_080dbf94(shine, shine->x, shine->y, shine->z);
  }
  Actor_ReleaseFar(shine);
  VramCache_FreeEntry(starEntry);
  VramCache_FreeEntry(glowEntry);
  BattleEffect_EndCanvasSprite();
end:
  FieldPsynergy_FadeFromCastColor();
  HeapSlot_Release(92);
}

void Func_080e62f4(u8 *source, u8 *destination, s32 phase) {
  s32 width = 32;
  s32 height;
  s32 column;
  s32 row;

  Iwram_ClearWordsEntry(destination, 32 * 32);
  // Set after the clear, which keeps it out of the call's registers.
  height = 32;
  for (column = 0; column < width; column++) {
    s32 wave = (Trig_Sin(column << 13) << 3) + Q16_ONE * 8;

    for (row = 0; row < height; row++) {
      s32 position = phase * 0x19999 - Q16_ONE * 124 + row * (Q16_ONE * 4);
      // In two steps: the wave subtracted, then the shift.
      s32 shift = position - wave;
      shift >>= 16;

      if (shift >= 0 && shift < height)
        destination[(shift << 5) + column] = source[row * width + column];
    }
  }
}
