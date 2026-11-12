#include "types.h"
#include "task.h"
#include "ram_buffer.h"
#include "actor.h"
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
  struct Actor *mainObject;
  struct Actor *child;
  s16 firstObjectId;
  s16 secondObjectId;
  u16 action;
  s16 fieldKind;
  s8 enabled;
};

struct FxEvent {
  u8 unk00[20];
  struct Actor *objects[80];
  u8 unk154[0xd30 - 0x154];
  u8 unkd30;
  u8 unkd31[7];
  s8 unkd38;
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

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Func_08020330(struct Actor *object);
void Field_StartColorTransition(s32 frames, s32 color);
s32 Func_080cdac0(s32 effect, s32 from, s32 to);
struct ObjectMetadata *Resource_GetMetadataRecordFarFar(s32 id);
struct Actor *Actor_Spawn(s32 kind, s32 x, s32 y, s32 z);
void Actor_Destroy(struct Actor *object);
void wait(s32 frames);
void BattleEffect_SetObjectUpdateMode(struct Actor *object, s32 mode);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
#include "resource.h"
s32 BattleEffect_GetObjectDepth(struct Actor *object);
s32 BattleEffect_GetObjectPriority(struct Actor *object);
void *Func_080ce458(u32 kind, s32 entry, s32 arg);
void Field_RunPsynergyEvent(void *event, s32 a, s32 b);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e38c8(void);

#define OBJECT_DEFINITION(object)                                              \
  (((struct ObjectAnimation *)(object)->sprite)->definition)

void Func_080e3d04(void) {
  struct SprayWork *work;
  struct FxEvent *event;
  struct FxScene *scene;
  struct Actor *caster;
  struct Actor *object;
  struct EffectSprite *sprite;
  union {
    struct Actor *object;
    struct SprayParticle *particle;
  } cursor;
  void *pending;
  s32 pos[3];
  s32 kind;
  s32 look;
  s32 best;
  s32 distance;
  s32 height;
  s32 id;
  s32 resourceId;
  s32 dropSlot;
  s32 puffSlot;
  s32 tile;
  s32 i;
  s32 j;
  s32 k;
  s32 mode;

  work = HeapSlot_Alloc(92, sizeof(struct SprayWork));
  scene = Ram_HeapSlots->effectWork;
  event = Ram_HeapSlots->eventWork;
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
      cursor.object = event->objects[i];
      id = OBJECT_DEFINITION(cursor.object)[0];
      if (cursor.object != NULL && cursor.object->script != NULL &&
          (id == 410 || id == 304 || id == 386 || id == 418)) {
        distance = Func_080cdac0(scene->fieldKind, scene->firstObjectId, i);
        if (distance != -1 && best > distance) {
          height = Resource_GetMetadataRecordFarFar(
                       OBJECT_DEFINITION(cursor.object)[0])
                       ->height
                   << 16;
          work->mode = 1;
          work->origin[0] = cursor.object->x;
          work->origin[1] = cursor.object->y + height;
          work->origin[2] = cursor.object->z;
          scene->secondObjectId = i;
          scene->child = cursor.object;
          best = distance;
        }
      }
    }
  } else {
    if (event->unkd38 == 0)
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

  mode = work->mode;
  if (mode == 1) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Actor_Spawn(366, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = 0;
    object->scaleX = 0x14000;
    object->scaleY = 0x14000;
    Actor_SetMode(object, 2);
    Sound_PlayFar(186);
    wait(50);
    Actor_Destroy(object);
    wait(10);
    goto end;
  }
  if (mode == 0) {
    pos[0] = work->origin[0];
    pos[1] = work->origin[1];
    pos[2] = work->origin[2];
    object = Actor_Spawn(367, pos[0], pos[1], pos[2]);
    if (object == NULL)
      goto end;
    ((u8 *)object)[85] = mode;
    Actor_SetMode(object, 2);
    Sound_PlayFar(186);
    wait(45);
    Actor_Destroy(object);
    wait(10);
    goto end;
  }

  Sound_PlayFar(207);
  BattleEffect_SetObjectUpdateMode(scene->mainObject, 0);
  work->angle = scene->angle;
  work->targetAngle = scene->angle;
  switch (look) {
  case 0:
    resourceId = (s32)&ResourceId_RedFlameBurst;
    break;
  case 1:
    resourceId = (s32)&ResourceId_VioletFlameBurst;
    break;
  case 2:
    resourceId = (s32)&ResourceId_YellowFlameBurst;
    break;
  case 3:
    resourceId = (s32)&ResourceId_BlueFlameBurst;
    break;
  default:
    resourceId = (s32)&ResourceId_RedFlameBurst;
    break;
  }
  Resource_DecodeType01(Resource_GetTableEntry(resourceId), work);
  dropSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(dropSlot, 0x1000, work);
  work->dropTile = tile;
  cursor.particle = work->dropMotion;
  for (j = 0; j < 16; j++) {
    sprite = &work->drops[j];

    Func_080eaf98(sprite, 15, 24, 0x80000000, tile);
    sprite->depth = BattleEffect_GetObjectDepth(caster) + 10;
    sprite->oam.priority = BattleEffect_GetObjectPriority(caster);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    cursor.particle->timer = -1;
    cursor.particle++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_DustPuffA),
                        work);
  puffSlot = Resource_FindFreeEntry();
  tile = VramCache_Load(puffSlot, 0x800, work);
  work->puffTile = tile;
  cursor.particle = work->puffMotion;
  for (j = 0; j < 16; j++) {
    sprite = &work->puffs[j];

    Func_080eaf98(sprite, 8, 15, 0x40000000, tile);
    sprite->scaleY = sprite->scaleX = 0xc000;
    sprite->depth = 250;
    sprite->oam.priority = 0;
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    cursor.particle->timer = -1;
    cursor.particle++;
  }
  wait(1);
  work->emitting = 1;
  Task_Add(Func_080e38c8, 0x480);
  for (k = 0; k < 80; k++) {
    wait(1);
    if (work->hit != 0) {
      pending = Func_080ce458(0x60000005, scene->fieldKind,
                              (s16)(work->hitObject | 0x100));
      if (pending != NULL)
        Field_RunPsynergyEvent(pending, scene->firstObjectId, work->hitObject);
      if (Func_080ce458(0x20000005, scene->fieldKind,
                        (s16)(work->hitObject | 0x100)) != NULL) {
        event->unkd30 = work->hitObject;
        break;
      }
    }
    if (gInput.pressed & 2)
      break;
  }
  Sound_PlayFar(395);
  work->emitting = 0;
  for (k = 0; k < 60; k++) {
    cursor.particle = work->dropMotion;
    for (j = 0; j < 16 && cursor.particle->timer == -1; j++, cursor.particle++)
      ;
    if (j == 16)
      break;
    wait(1);
  }
  Task_Remove(Func_080e38c8);
  Resource_ResetEntry(dropSlot);
  Resource_ResetEntry(puffSlot);
end:
  BattleEffect_PrepareBufferInterpolation();
  HeapSlot_Release(92);
}

// Whether a script object stands where it may not.

/// The script objects (Ram_HeapSlots->scriptObjects).
#define SCRIPT_OBJECT_COUNT 64

/// The event work's mode in which the cell test is skipped.
#define EVENT_MODE_NO_CELL_TEST 3

s32 Func_08020308(s32 x, s32 z, s32 layer, s32 height);
s32 Field_CheckSpheresOverlapFar(s32 *a, s32 radiusA, s32 *b, s32 radiusB);

/// Says whether object is blocked: 1 when, outside event mode 3,
/// Func_08020308 reports its cell, ORed with 2 when it overlaps any other
/// collidable script object (each radius less 2 pixels).
s32 Func_080e4244(struct Actor *object) {
  s32 result;
  struct Actor *other;
  s32 i;

  s8 *work = Ram_HeapSlots->eventWork;
  result = 0;
  if (work[394] != EVENT_MODE_NO_CELL_TEST) {
    if (Func_08020308(object->x, object->z, 0, object->terrainHeight) != 0)
      result = 1;
  }
  other = Ram_HeapSlots->scriptObjects;
  for (i = SCRIPT_OBJECT_COUNT - 1; i >= 0; i--, other++) {
    if (other->script != NULL && (other->collisionFlags & 1) &&
        other != object) {
      if (Field_CheckSpheresOverlapFar(&object->x, object->radius - 2,
                                       &other->x, other->radius - 2) >= 0)
        result |= 2;
    }
  }
  return result;
}

#include "iwram_call.h"
#include "dma.h"
#include "io_reg.h"
#include "heap_release.h"

struct CueEvent {
  u8 unk000[358];
  u16 cue;
};

extern u8 gMainRoutineWork[];

s32 Func_080dbcd8(s32 x, s32 z, s32 layer);
s32 Func_080dbd48(s32 x, s32 z, s32 layer);

void Func_080e42d4(void) {
  struct CueEvent *event = Ram_HeapSlots->eventWork;
  u8 *party = gMainRoutineWork;
  struct Actor *object = ObjectTable_Get(*(s32 *)(party + 532));
  s32 reason = 0;

  if (*(s8 *)(party + 707) != 0)
    reason = 1;
  if (Func_080dbcd8(object->x, object->z, object->terrainId) == 0)
    reason = 2;
  if (party[530] == 5 && *(s16 *)((u8 *)object + 102) != 0 &&
      object->timer == 0) {
    s32 x;
    s32 z;
    s32 back;

    if (Func_080dbd48(object->x, object->z, 2) == 0)
      reason = 3;
    x = object->x & 0xfff00000;
    z = object->z & 0xfff00000;
    back = -0x80000;
    /* FAKEMATCH(barrier): back through an empty asm, so its -0x80000 loads
     * before the forward offset is built; without it, 10 of 93 instructions
     * differ in tla-en. */
    asm("" : "+l"(back));
    /* FAKEMATCH(dead): an empty once-only block that keeps the forward offset's
     * add after that load; without it, 4 of 93 instructions differ in tla-en.
     */
    do {
    } while (0);
    if (Func_080dbd48(x + 0x80000, z + back, 2) == 0)
      reason = 4;
  }
  if (reason != 0 && (Func_080e4244(object) & 2) == 0)
    event->cue = 0x208b;
}

/// The heap slot (a byte offset into Ram_HeapSlots) that holds the RAM copy of
/// the ARM picture decoder while a picture is drawn.
#define HEAP_SLOT_PICTURE_DECODER 80

// How a picture is stored.
#define PICTURE_FORMAT_TYPE01 1 ///< one type-01 stream
#define PICTURE_FORMAT_COMPRESSED                                              \
  3 ///< compressed, then run through the decoder

struct PictureSource {
  u8 unk00[5];
  u8 decodeMode;
  u8 unk06;
  u8 format;
  const void **pictures;
  u8 unk0c[10];
  u8 picture;
};

typedef void (*PictureDecoder)(const void *source, void *destination, s32 mode);

extern const u8 BattleEffect_DecodePictureCode[];
extern const u8 BattleEffect_DecodePictureCopyEnd[];
/// The decoder with its tables and work area (ld_script.ld). The block is
/// larger than the copy, which stops before the work area.
extern u8 BattleEffect_DecodePictureBlockSize[];

/// Draws the current picture into a cleared buffer of the given
/// size. The ARM picture decoder runs from RAM: when no copy is loaded yet,
/// this loads one for the call and releases it afterwards.
void Func_080e43a4(struct PictureSource *source, void *buffer, s32 size) {
  s32 decoderLoaded = 1;
  PictureDecoder decode = Ram_HeapSlots->unk4c[1];

  if (decode == NULL) {
    Dma_Set(BattleEffect_DecodePictureCode,
            HeapSlot_Alloc(HEAP_SLOT_PICTURE_DECODER,
                           (s32)BattleEffect_DecodePictureBlockSize),
            DMA_COPY_WORDS((u32)(BattleEffect_DecodePictureCopyEnd -
                                 BattleEffect_DecodePictureCode) >>
                           2),
            REG_DMA3);
    decode = Ram_HeapSlots->unk4c[1];
    decoderLoaded = 0;
  }

  Iwram_ClearWords(buffer, size);
  if (source->format == PICTURE_FORMAT_TYPE01) {
    Resource_DecodeType01(source->pictures[source->picture], buffer);
  } else if (source->format == PICTURE_FORMAT_COMPRESSED) {
    const void *stream =
        Iwram_Decompress(source->pictures[source->picture], buffer);

    if (stream != NULL)
      decode(stream, buffer, 0);
  } else {
    decode(source->pictures[source->picture], buffer, source->decodeMode);
  }

  if (!decoderLoaded)
    HeapSlot_Release(HEAP_SLOT_PICTURE_DECODER);
}
