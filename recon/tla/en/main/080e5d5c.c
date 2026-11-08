// Differ 30 of 639 (tla-en, 2026-11-08, -mtune=gba); the pins tagged, measured then.
// 2026-11-06: allocation only (lift r4/r0, the 1 for j++,
// one fused add); a 5-minute permute reached 28 only by changing a signature.
// Also measured (2026-11-06, second pass): speed unpinned with its 0x28000 added
// as its own statement (speed += 0x28000) gives the native's copy-then-add in
// r6 without the pin, but the j loop's 1 then sits in r1 and wait(1) loads its
// own, one instruction more that shifts every pool (43); a 5-minute permute
// from there found nothing lower. The lift and j++ rows are reload's
// round-robin register choices (lift through r2 and r4 here, r4 and r0
// natively), one step out of phase with the native's.
// Earlier: near miss at 30 (2026-11-05) with untagged pins ("r9", "r6"), empty asms,
// one-pass blocks and locals found by search; each needs its S2 tag on adoption.
// Draft, differ 99 of 639, skeleton gap 11 (tla-en, 2026-11-01)
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
// Frame, stack slots and control flow agree. Left: scene and the counters
// i, j trade r9 and r10 (nearly every remaining pair). The allocator ranks
// scene (34 refs, life 404) above j (18, 184), where the listing needs j
// first; scene with 3 fewer refs or j with 2 more would flip it, but no
// spelling found has that without changing the instructions. Tried: the
// counters in other groupings (loops 1 and 2, or all in one: the second
// i = 0 goes to CSE and the frame grows), count-up loops, sprite as a pointer
// or set later. Smaller, partly from that swap: lift's reload takes r2 and r4
// for r4 and r0, the 1 for j++ sits in r1 so wait(1) reloads it, which also
// moves the pool branch by one instruction.
// Found by allocation: size is a whole-function constant left unallocated;
// glow is reassigned in the frame loop; rise is its own counter; the
// particles are indexed by j, so loop.c makes the givs after j = 0.
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
  /* FAKEMATCH(pin): the glow tile held in r9; as a plain local, 90 of 639
   * instructions differ in tla-en. */
  register s32 glowTile asm("r9");
  s32 spot;
  s32 size = 0x800;
  s32 rise;
  /* FAKEMATCH(pin): the speed held in r6; as a plain local, 43 of 639
   * instructions differ in tla-en. */
  register s32 speed asm("r6");
  s32 angle;
  s32 frame;
  s32 i;
  s32 j;

  lift = 0x200000;
  if (child == NULL) {
    scene->x = caster->x;
    scene->y = caster->y;
    scene->z = caster->z;
    Vector_AddPolarOffset(lift, scene->facing, &scene->x);
  } else if (child->motionFlags & 4) {
    scene->y = child->terrainHeight;
    lift = child->y - child->terrainHeight + 0x200000;
  }
  if (scene->enabled != 0) {
    // As x != -1 this splits the -1 and loses the listing's mvns.
    spot = ~Func_080cda84(scene->fieldKind, caster, &scene->x);
    inReach = (u32)(-spot | spot) >> 31;
    if (inReach == 1) {
      for (i = 0; i < 80; i++) {
        object = event->objects[i];
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
  // Default: the zero the canvas and glow fields below reuse.
  i = 0;
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
  for (j = 0; j < 32; j++) {
    sprite = &work->sprites[j];
    speed = Random16() * 18 / 10 + 0x28000;
    angle = j * 0x1555 + (Random16() >> 5);
    pos[0] = scene->x;
    pos[1] = scene->y;
    pos[2] = scene->z;
    Vector_AddPolarOffset(0x40000, angle, pos);
    Camera_WorldToScreen(pos);
    work->particles[j].x = pos[0];
    work->particles[j].y = pos[2];
    work->particles[j].velocityX = Iwram_MulQ16(Trig_Cos(angle), speed);
    work->particles[j].velocityY = Iwram_MulQ16(Trig_Sin(angle), speed);
    work->particles[j].timer = -j;
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
      Field_RunPsynergyEvent(pending, work->fading, frame);
      frame++;
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
