// Draft, differ 4 of 639 (tla-en, 2026-11-08)
// 9 November, still 4. The else-if branch's registers follow once the NULL
// branch's &scene->x goes through a local, with the plain test (no inReach
// device): a block-local at = &scene->x set after the z store gives the
// else-if exactly but reloads scene into r3 for scene->facing where the
// listing reuses r1 (9); carried through shine, (s32 *)(shine = (void *)
// &scene->x), every row matches but the add, built as movs r2, #4 / add r2, r9
// where the listing has mov r2, r9 / adds r2, #4: shine sits in fp, so reload
// takes the commuted alternative (4, the same count as the kept form). Through
// starEntry or glowEntry the store to its slot stays (7); through sprite,
// pending, particle, glow, object, canvas or the other locals 11 to 451; at at
// function scope 9; facing read first 12.
// Devices pass (2026-11-09), none below 4: the mask or the flags held in each
// other local (glowEntry, starEntry, starTile 4; glowTile, spot 18; j 11;
// rise, frame, angle, speed, size 34 to 180), as `flags & (v = 4)`,
// `(v = 4) & flags`, `flags & (v = 4, 4)` or `(v = flags) & 4` (16).
// Left: the airborne test's two registers. The listing loads motionFlags into
// r2 and builds the 4 in r3 (ldrb r2 / movs r3, #4 / ands r3, r2); the kept
// form, the mask held in inReach, gives ldrb r3 / movs r2, #4. Every edition
// measures the same (tla-ja, de, es, fr and it: 4; de, es and fr by --symbol).
// The plain test (child->motionFlags & 4) gives the listing's ands but 12: the
// else-if branch's two reloads, scene's base for the scene->y store and the
// 0x200000 for lift, take r2 and r4 where the listing has r4 and r0. They are
// reload's round-robin choices (spill registers r0 to r4), one step behind the
// listing's: the listing makes one more round-robin reload than this draft
// between the scene->x store in the NULL branch (r1, both) and the scene->y
// store. Confirmed: the NULL branch passing (s32 *)(glowEntry = (s32)&scene->x)
// makes that reload (adds r2, #4 then needs a spill register) and the else-if
// branch then matches exactly, but its store to glowEntry stays (7). What the
// listing's extra reload is was not found; the motionFlags mask in a spilled
// local (inReach, glowEntry or starEntry) is the nearest. Tried without gain:
// the test as a bitfield, (flags >> 2) & 1, a u8 local, casts, a dedicated mask
// local (16), the test stored in inReach, spot or glowEntry, nested ifs,
// child != NULL first (44), lift as lift += ..., split, reordered or through a
// long long (4, other rows), the NULL branch's stores in one statement, a
// caster copy, a centre pointer for &scene->x (33), and two permute runs over
// the opening (10 and 12 minutes; best 4, by the same reused local).
// 2026-11-08, third pass (28 to 4): both pins and the zero default i = 0 are
// gone. The particles are stepped by a pointer the loop advances after j, so
// j++ takes its 1 in r0 before the particle's 28 and wait(1) reuses it; with
// that, speed += 0x28000 as its own statement gives the copy-then-add in r6
// unpinned. The objects loop counting with j puts j above scene in the
// allocator's order (j to r10, scene to r9) without the r9 pin. Pins measured
// on that form: none needed (an r9 pin on the glow tile now costs 68).
// Earlier passes (2026-11-01 to 11-08) reached 28 only with pins on the glow
// tile (r9) and speed (r6).
// A field Psynergy that raises a glowing object
// (object 352) before the caster or at the target: the scene sits 32 pixels
// before the caster, or on the target's ground when it is airborne. With a
// target in reach and no solid object at the spot (Func_080cda84,
// Func_080dbe80), the shine plays: the glow disc on the canvas, a glow
// sprite and 32 gold stars (FieldEffect_GoldStar) bursting from a ring, the
// object rising 24 frames, then Func_080e5a64 runs as a frame callback, the
// effect's stage-5 event running each frame and the glow drawn unless the
// event mode is 3 or the event work's flags at 0xd4c say otherwise, until it
// parks at 999; then the object bursts (Func_080dbf94). Otherwise the
// object sinks and shrinks (BattleEffect_SinkAndShrinkObject) and scatters
// sixteen particles. Cue 114 ends both.
// Found by allocation: size is a whole-function constant left unallocated;
// glow is reassigned in the frame loop; rise is its own counter; j counts the
// objects, stars and particles alike.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "effect_sprite.h"
#include "canvas_sprite.h"
#include "iwram_call.h"

struct ShineParticle {
  s32 x;
  s32 y;
  s32 unk08;
  s32 velocityX;
  s32 velocityY;
  s32 unk14;
  s32 timer;
};

struct ShineWork {
  u8 tiles[0x1000];
  struct ShineParticle particles[32];
  struct EffectSprite sprites[32];
  struct EffectSprite glow;
  struct Actor *object;
  s16 state;
  s16 timer;
  s8 sparkles;
  s8 fading;
};

struct FxScene {
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

struct FxEvent {
  u8 unk00[20];
  struct Actor *objects[80];
};

extern char ResourceId_GlowDisc, ResourceId_GoldStar;

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
s32 Func_080cda84(s32 id, struct Actor *object, s32 *position);
s32 Func_080dbe80(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
void BattleEffect_InitializeSharedScene(void);
void Sound_PlayFar(s32 cue);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_SetMode(struct Actor *object, s32 mode);
void Actor_Destroy(struct Actor *object);
void Func_080dba5c(void);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
u32 Random16(void);
s32 Trig_Sin(s32 angle);
s32 Trig_Cos(s32 angle);
void wait(s32 frames);
void Func_080dbed0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_080dbf94(struct Actor *object, s32 x, s32 y, s32 z);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void BattleEffect_SinkAndShrinkObject(void);
struct Actor *BattleEffect_SpawnScatterParticle(s32 x, s32 y, s32 z);
s32 Func_080ce31c(s32 kind);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void Field_RunPsynergyEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e5a64(void);

void Func_080e5d5c(void) {
  struct ShineWork *work = HeapSlot_Alloc(92, 0x18b4);
  struct FxScene *scene = Ram_HeapSlots->effectWork;
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
  struct ShineParticle *particle;

  lift = 0x200000;
  /* FAKEMATCH(temporary): the mask 4 held in inReach; as child->motionFlags & 4,
   * 12 of 639 instructions differ in tla-en. */
  if (child == NULL) {
    scene->x = caster->x;
    scene->y = caster->y;
    scene->z = caster->z;
    Vector_AddPolarOffset(lift, scene->facing, &scene->x);
  } else if (child->motionFlags & (inReach = 4)) {
    scene->y = child->terrainHeight;
    lift = child->y - child->terrainHeight + 0x200000;
  }
  if (scene->enabled != 0) {
    // As x != -1 this splits the -1 and loses the listing's mvns.
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

  BattleEffect_InitializeSharedScene();
  Sound_PlayFar(138);
  shine = Actor_Spawn(352, 0, 0, 0);
  work->object = shine;
  if (shine == NULL)
    goto end;
  shine->motionFlags = 0;
  if (*(u16 *)((u8 *)event + 0xd4c) & 2) {
    if ((shine->animationKind & 15) == 1) {
      ((struct EffectSprite *)shine->animation)->oam.priority = 1;
      ((u8 *)shine)[35] &= 254;
    }
  } else {
    ((u8 *)shine)[35] = 1;
  }
  shine->angle = caster->angle;
  Actor_SetMode(shine, 2);
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
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GlowDisc),
                        Ram_HeapSlots->battleFxWork[1]);
  canvas->command = CANVAS_UPLOAD;

  // A once-only block: without it the canvas mode load is scheduled one row
  // later (6 differ).
  do {
    Iwram_FillWords(work, size, 0);
  } while (0);
  glowEntry = Resource_FindFreeEntry();
  glowTile = VramCache_Load(glowEntry, size, work);
  glow = &work->glow;
  Func_080eaf98(glow, 32, 32, 0xc0000000, glowTile);
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

  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_GoldStar),
                        work);
  starEntry = Resource_FindFreeEntry();
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
    Func_080eaf98(sprite, 8, 8, 0x40000000, starTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    sprite->scaleY = sprite->scaleX = 0xcccc;
  }
  wait(1);
  Func_080dbed0(shine, scene->x, scene->y + lift, scene->z);
  Actor_SetMode(shine, 1);
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
    pending = Func_080ce458(0x50000005, scene->fieldKind,
                            Func_080ce31c(scene->fieldKind));
    Task_Add(Func_080e5a64, 0x480);
    frame = 0;
    while (work->state != 999) {
      wait(1);
      scene->fading = work->fading;
      scene->frame = frame;
      if (((s8 *)event)[394] != 3 && !(*(u16 *)((u8 *)event + 0xd4c) & 1)) {
        glow = &work->glow;
        Func_080eb01c(glow);
      }
      Field_RunPsynergyEvent(pending, work->fading, frame++);
    }
    Task_Remove(Func_080e5a64);
    wait(10);
    Sound_PlayFar(114);
    Func_080dbf94(shine, shine->x, shine->y, shine->z);
  }
  Actor_Destroy(shine);
  Resource_ResetEntry(starEntry);
  Resource_ResetEntry(glowEntry);
  BattleEffect_EndCanvasSprite();
end:
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}
