// Draft, differ 615 of 609: a field Psynergy that sprays from the caster, its
// look chosen by the target (an object of kind 418 whose definition's sixth
// byte is 4, 1 or 3 picks one of resources 0x1dd to 0x1df, else 0x1dc), with
// a colour transition from Data_080f0f90. Cast at an object, it aims at the
// nearest object of kinds 410, 304, 386 or 418 that Func_080cdac0 reaches
// (mode 1); otherwise mode 2, or 3 when the event work's byte at 0xd38 is
// set. Mode 1 and the unaimed mode 0 only show a fizzle (object 366 or 367).
// Otherwise sixteen drops and sixteen puffs of smoke are set up and
// Func_080e38c8 runs as a frame callback for up to 80 frames, until B or a
// drop reaches an object: then the object's stage-6 event runs and, when it
// has a stage-2 event, its index is stored at 0xd30 of the event work. The
// spray stops and waits up to 60 frames for the drops to land.
// Same calls, offsets and control flow; left: register allocation (the
// native keeps the work in r8, the event work and caster on the stack, and
// its frame is 32 bytes against 36), which shifts most rows. The resource
// flames are FieldEffect_RedFlameBurst to BlueFlameBurst (0x1dc to 0x1df),
// the smoke FieldEffect_DustPuffA (0x1e1).
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "effect_sprite.h"
#include "input.h"

struct SprayParticle {
  s32 x;
  s32 y;
  s32 z;
  s32 velocityX;
  s32 velocityY;
  s32 velocityZ;
  s32 timer;
};

struct SprayWork {
  u8 tiles[0x1000];
  struct EffectSprite drops[16];
  struct SprayParticle dropMotion[16];
  struct EffectSprite puffs[16];
  struct SprayParticle puffMotion[16];
  s32 origin[3];
  u16 dropTile;
  u16 puffTile;
  u16 angle;
  u16 targetAngle;
  s16 hitObject;
  s8 hit;
  s8 emitting;
  s32 speed;
  s32 lifetime;
  s32 gravity;
  s8 mode;
};

struct FxScene {
  u16 angleFraction;
  u16 angle;
  s32 x;
  s32 y;
  s32 z;
  struct ObjectRuntime *mainObject;
  struct ObjectRuntime *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

struct FxEvent {
  u8 unk00[20];
  struct ObjectRuntime *objects[80];
};

struct ObjectAnimation {
  u8 unk00[40];
  s16 *definition;
};

struct ObjectMetadata {
  u8 unk00[8];
  s8 height;
};

extern const s32 Data_080f0f90[];
extern char ResourceId_RedFlameBurst, ResourceId_VioletFlameBurst,
    ResourceId_YellowFlameBurst, ResourceId_BlueFlameBurst,
    ResourceId_DustPuffA;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 Func_08020330(struct ObjectRuntime *object);
void Field_StartColorTransition(s32 frames, s32 color);
s32 Func_080cdac0(s32 effect, s32 from, s32 to);
struct ObjectMetadata *Resource_GetMetadataRecordFarFar(s32 id);
struct ObjectRuntime *Object_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Object_SetMode(struct ObjectRuntime *object, s32 mode);
void Object_Destroy(struct ObjectRuntime *object);
void Sound_PlayFar(s32 cue);
void WaitFrames(s32 frames);
void BattleEffect_SetObjectUpdateMode(struct ObjectRuntime *object, s32 mode);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
s32 BattleEffect_GetObjectDepth(struct ObjectRuntime *object);
s32 BattleEffect_GetObjectPriority(struct ObjectRuntime *object);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void EventRuntime_RunPendingEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e38c8(void);

#define OBJECT_DEFINITION(object)                                              \
  (((struct ObjectAnimation *)(object)->animation)->definition)

void Func_080e3d04(void) {
  struct SprayWork *work;
  struct FxEvent *event;
  struct FxScene *scene;
  struct ObjectRuntime *caster;
  struct ObjectRuntime *object;
  struct EffectSprite *sprite;
  struct SprayParticle *particle;
  void *pending;
  s32 pos[3];
  s32 kind;
  s32 look;
  s32 best;
  s32 distance;
  s32 height;
  s32 id;
  s32 dropSlot;
  s32 puffSlot;
  s32 tile;
  s32 i;
  s32 j;

  work = Runtime_AllocateHeapBlock(92, sizeof(struct SprayWork));
  event = Ram_HeapSlots->eventWork;
  scene = Ram_HeapSlots->effectWork;
  caster = scene->mainObject;
  look = 0;
  object = scene->child;
  if (Func_08020330(object) == 418) {
    kind = ((u8 *)OBJECT_DEFINITION(object))[5];
    if (kind == 4)
      look = 1;
    if (kind == 1)
      look = 2;
    if (kind == 3)
      look = 3;
  }
  Field_StartColorTransition(8, Data_080f0f90[look]);
  work->mode = 0;
  if (scene->enabled != 0) {
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + 0x100000;
    work->origin[2] = scene->z;
    best = 9999;
    for (i = 0; i < 80; i++) {
      object = event->objects[i];
      id = OBJECT_DEFINITION(object)[0];
      if (object != NULL && object->script != NULL &&
          (id == 410 || id == 304 || id == 386 || id == 418)) {
        distance = Func_080cdac0(scene->fieldKind, scene->firstObjectId, i);
        if (distance != -1 && distance < best) {
          height =
              Resource_GetMetadataRecordFarFar(OBJECT_DEFINITION(object)[0])
                  ->height
              << 16;
          work->mode = 1;
          work->origin[0] = object->x;
          work->origin[1] = object->y + height;
          work->origin[2] = object->z;
          scene->secondObjectId = i;
          scene->child = object;
          best = distance;
        }
      }
    }
  } else {
    if (((s8 *)event)[0xd38] == 0)
      work->mode = 2;
    else
      work->mode = 3;
    if (scene->child != NULL)
      height =
          Resource_GetMetadataRecordFarFar(OBJECT_DEFINITION(scene->child)[0])
              ->height
          << 16;
    else
      height = 0x140000;
    work->origin[0] = scene->x;
    work->origin[1] = scene->y + height;
    work->origin[2] = scene->z;
  }

  if (work->mode == 1) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Object_Spawn(366, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = 0;
    object->scaleX = 0x14000;
    object->scaleY = 0x14000;
    Object_SetMode(object, 2);
    Sound_PlayFar(186);
    WaitFrames(50);
    Object_Destroy(object);
    WaitFrames(10);
    goto end;
  }
  if (work->mode == 0) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Object_Spawn(367, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = work->mode;
    Object_SetMode(object, 2);
    Sound_PlayFar(186);
    WaitFrames(45);
    Object_Destroy(object);
    WaitFrames(10);
    goto end;
  }

  Sound_PlayFar(207);
  BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
  work->angle = scene->angle;
  work->targetAngle = scene->angle;
  switch (look) {
  case 1:
    id = (s32)&ResourceId_VioletFlameBurst;
    break;
  case 2:
    id = (s32)&ResourceId_YellowFlameBurst;
    break;
  case 3:
    id = (s32)&ResourceId_BlueFlameBurst;
    break;
  default:
    id = (s32)&ResourceId_RedFlameBurst;
    break;
  }
  Resource_DecodeType01(Resource_GetTableEntry(id), work);
  dropSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(dropSlot, 0x1000, work);
  work->dropTile = tile;
  particle = work->dropMotion;
  sprite = work->drops;
  for (i = 15; i >= 0; i--) {
    Func_080eaf98(sprite, 15, 24, 0x80000000, tile);
    sprite->depth = BattleEffect_GetObjectDepth(caster) + 10;
    sprite->oam.priority = BattleEffect_GetObjectPriority(caster);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    particle->timer = -1;
    sprite++;
    particle++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  puffSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(puffSlot, 0x800, work);
  work->puffTile = tile;
  particle = work->puffMotion;
  sprite = work->puffs;
  for (i = 15; i >= 0; i--) {
    Func_080eaf98(sprite, 8, 15, 0x40000000, tile);
    sprite->scaleX = sprite->scaleY = 0xc000;
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    particle->timer = -1;
    sprite++;
    particle++;
  }
  WaitFrames(1);
  work->emitting = 1;
  Scheduler_AddOrUpdateCallback(Func_080e38c8, 0x480);
  for (i = 0; i < 80; i++) {
    WaitFrames(1);
    if (work->hit != 0) {
      pending = Func_080ce458(0x60000005, scene->fieldKind,
                              (s16)(work->hitObject | 0x100));
      if (pending != NULL)
        EventRuntime_RunPendingEvent(pending, scene->firstObjectId,
                                     work->hitObject);
      if (Func_080ce458(0x20000005, scene->fieldKind,
                        (s16)(work->hitObject | 0x100)) != NULL) {
        ((u8 *)event)[0xd30] = work->hitObject;
        break;
      }
    }
    if (gInput.pressed & 2)
      break;
  }
  Sound_PlayFar(395);
  work->emitting = 0;
  for (i = 0; i < 60; i++) {
    particle = work->dropMotion;
    for (j = 0; j < 16 && particle->timer == -1; j++)
      particle++;
    if (j == 16)
      break;
    WaitFrames(1);
  }
  Scheduler_RemoveCallback(Func_080e38c8);
  Resource_ResetEntry(dropSlot);
  Resource_ResetEntry(puffSlot);
end:
  BattleEffect_PrepareBufferInterpolation();
  Runtime_ReleaseHeapBlock(92);
}
