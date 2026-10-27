// Draft, differ 640 of 724: an event sequence run on an object (targetId): 48
// runes (FieldEffect_Runes) gather on it in a 16-by-3 grid while pebbles
// (FieldEffect_Pebbles) fly up from it for 40 frames; the object takes mode
// 2, its update becomes Func_080e9e8c and it plays Data_080f1040; it is then
// shrunk away (cue 134, a burst of 25 pebbles), the runes flash in a wave,
// the controlled owner turns to face it and takes mode 28, and the runes
// circle (Func_080e9f68, as a frame callback, with the work's state and
// origin set here), drift to the owner, close in, and the owner plays
// gData_080f3614 for 60 frames. The owner is set back to full size.
// Same calls, offsets and control flow; left: stack and register allocation
// (the native frame is 64 bytes against 60, with the pebble count and
// cursor at sp 8 and 4 and the work in r9), which shifts most rows.
#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "effect_sprite.h"

struct Mote {
  s32 x;
  s32 z;
  s32 unk08;
  s32 targetX;
  s32 targetZ;
  s32 unk14;
  s32 timer;
};

struct MoteWork {
  u8 tiles[0x800];
  struct Mote sparks[32];
  struct EffectSprite sparkSprites[32];
  struct Mote motes[48];
  struct EffectSprite moteSprites[48];
  s32 originX;
  s32 originZ;
  s32 angle;
  s32 radius;
  s32 spacing;
  s8 state;
  s8 moving;
  u16 sparkTile;
  s32 targetX;
  s32 targetZ;
};

extern u32 gFrameCount;
extern const s32 gData_080f3954[];
extern const u8 Data_080f1040[];
extern const u8 gData_080f3614[];
extern char ResourceId_Runes, ResourceId_Pebbles;

void *Runtime_AllocateHeapBlock(s32 slot, s32 size);
void Runtime_ReleaseHeapBlock(s32 slot);
s32 EventRuntime_GetControlledOwner(void);
void Field_StartColorTransition(s32 frames, s32 color);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
u32 Random16(void);
void WaitTicks(s32 ticks);
void Audio_PlayCue(s32 cue);
void EventRuntime_Wait(s32 frames);
void ObjectDispatch_InitializeFar(struct ObjectRuntime *object,
                                  const void *script);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 order);
void Scheduler_RemoveCallback(void (*callback)(void));
void ObjectMotion_ArmCallback(s32 id, s32 value, s32 flags);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e9e8c(void);
void Func_080e9f68(void);

void Func_080ea14c(s32 targetId) {
  struct MoteWork *work = Runtime_AllocateHeapBlock(92, 0x1d60);
  s32 ownerId = EventRuntime_GetControlledOwner();
  struct ObjectRuntime *owner = ObjectTable_Get(ownerId);
  struct ObjectRuntime *target = ObjectTable_Get(targetId);
  struct EffectSprite *sprite;
  struct Mote *mote;
  s32 pos[3];
  s32 sparks = 0;
  s32 nextSpark = 0;
  s32 ownerX;
  s32 ownerZ;
  s32 targetX;
  s32 targetZ;
  s32 moteEntry;
  s32 sparkEntry;
  s32 moteTile;
  s32 sparkTile;
  s32 state;
  s32 timer;
  s32 wave;
  s32 angle;
  s32 i;

  Field_StartColorTransition(8, gData_080f3954[gFrameCount & 7]);
  pos[0] = owner->x;
  pos[1] = owner->y + 0x180000;
  pos[2] = owner->z;
  Camera_WorldToScreen(pos);
  ownerX = pos[0];
  ownerZ = pos[2];
  work->targetX = ownerX;
  work->targetZ = ownerZ;
  pos[0] = target->x;
  pos[1] = target->y;
  pos[2] = target->z;
  Camera_WorldToScreen(pos);
  targetX = pos[0];
  targetZ = pos[2];
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Runes), work);
  moteEntry = Resource_FindFreeEntry();
  moteTile = VramCache_Load(moteEntry, 0x800, work);
  mote = work->motes;
  sprite = work->moteSprites;
  for (i = 0; i < 48; i++) {
    Func_080eaf98(sprite, 4, 4, 0, moteTile + ((Random16() << 4) >> 16) * 2);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 1;
    sprite->depth = 240;
    mote->x = targetX;
    mote->z = targetZ;
    mote->targetX = ((i % 16) * 3 << 18) + 0x1e0000;
    mote->targetZ = ((i / 16) * 3 << 19) + 0x1e0000;
    mote->timer = -i;
    sprite++;
    mote++;
  }
  Resource_DecodeType01(Resource_GetTableEntry((s32)&ResourceId_Pebbles), work);
  sparkEntry = Resource_FindFreeEntry();
  sparkTile = VramCache_Load(sparkEntry, 0x800, work);
  work->sparkTile = sparkTile;
  mote = work->sparks;
  sprite = work->sparkSprites;
  for (i = 31; i >= 0; i--) {
    Func_080eaf98(sprite, 8, 8, 0x40000000, sparkTile);
    sprite->oam.palette256 = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    sprite->depth = 240;
    mote->timer = -1;
    sprite++;
    mote++;
  }
  WaitTicks(1);
  Audio_PlayCue(139);
  Object_SetMode(target, 2);
  EventRuntime_Wait(60);
  target->action = 0;
  target->update = Func_080e9e8c;
  Audio_PlayCue(142);
  EventRuntime_Wait(80);
  ObjectDispatch_InitializeFar(target, Data_080f1040);
  work->moving = 0;
  Scheduler_AddOrUpdateCallback(Func_080e9f68, 0x480);
  state = 0;
  timer = 0;
  do {
    switch (state) {
    case 0:
      work->state = 0;
      if (timer <= 39) {
        if ((timer & 7) == 0)
          Audio_PlayCue(133);
        sparks = 1;
      }
      if (timer == 40) {
        Audio_PlayCue(134);
        sparks = 25;
        Object_ResetMotion(target);
        target->scaleX = 0;
      }
      if (timer == 80) {
        state++;
        timer = -1;
      }
      break;
    case 1:
      sprite = work->moteSprites;
      for (i = 0; i < 48; i++) {
        wave = timer - (i % 16 + i / 16);
        if (wave == 0)
          sprite->oam.index += 32;
        if (wave == 3)
          sprite->oam.index += -32;
        if (wave == 8)
          sprite->oam.index += 32;
        if (wave == 10)
          sprite->oam.index += -32;
        if (wave == 16)
          sprite->oam.index += 32;
        if (wave == 18)
          sprite->oam.index += -32;
        sprite++;
      }
      if (timer == 0)
        Audio_PlayCue(246);
      if (timer == 56) {
        state++;
        timer = -1;
      }
      break;
    case 2:
      if (timer == 0) {
        ObjectMotion_ArmCallback(ownerId, 0x4000, 0);
        break;
      }
      if (owner->script[owner->step] == 17) {
        state++;
        timer = -1;
      }
      if (timer == 60) {
        state++;
        timer = -1;
      }
      break;
    case 3:
      if (timer == 0)
        Object_SetMode(owner, 28);
      if (timer == 10) {
        state++;
        timer = -1;
      }
      break;
    case 4:
      if (timer == 0) {
        mote = work->motes;
        wave = 0;
        for (i = 47; i >= 0; i--) {
          mote->timer = wave;
          wave -= 3;
          mote++;
        }
        work->radius = 0x3c0000;
        work->angle = 0xc000;
        work->originX = 0x820000;
        work->originZ = 0x1e0000;
        work->moving = 1;
        work->spacing = 0x40000;
        work->state = 2;
        if (timer == 0)
          Audio_PlayCue(187);
      }
      if (timer == 48)
        Audio_PlayCue(187);
      if (timer == 96)
        Audio_PlayCue(187);
      if (timer == 100) {
        state++;
        timer = -1;
      }
      break;
    case 5:
      work->originX = timer * (ownerX - 0x820000) / 30 + 0x820000;
      work->originZ = timer * (ownerZ - 0x1e0000) / 30 + 0x1e0000;
      if (timer == 30) {
        state++;
        timer = -1;
      }
      break;
    case 6:
      if (timer == 0)
        work->spacing = 0;
      work->radius += -0x20000;
      if (work->radius < 0) {
        work->radius = 0;
        state++;
        timer = -1;
      }
      break;
    case 7:
      if (timer == 0)
        ObjectDispatch_InitializeFar(owner, gData_080f3614);
      if ((timer & 3) == 0)
        Audio_PlayCue(167);
      work->state = 3;
      if (timer == 60)
        state = 999;
      break;
    }
    for (i = sparks; i != 0; i--) {
      angle = Random16();
      mote = &work->sparks[nextSpark];
      mote->timer = 0;
      mote->x = target->x;
      mote->z = target->y + 0x30000;
      mote->unk08 = target->z;
      Vector_AddPolarOffset(Random16() * 4 + 0x20000, angle, &mote->x);
      mote->targetX = 0;
      mote->targetZ = 0x48000;
      mote->unk14 = 0;
      Vector_AddPolarOffset(Random16() * 2 + 0x20000, angle,
                            &work->sparks[nextSpark].targetX);
      nextSpark = (nextSpark + 1) & 31;
    }
    sparks = 0;
    WaitTicks(1);
    timer++;
  } while (state != 999);
  Object_ResetMotion(owner);
  owner->scaleX = 0x10000;
  owner->scaleY = 0x10000;
  EventRuntime_Wait(10);
  Scheduler_RemoveCallback(Func_080e9f68);
  BattleEffect_PrepareBufferInterpolation();
  Resource_ResetEntry(moteEntry);
  Resource_ResetEntry(sparkEntry);
  Runtime_ReleaseHeapBlock(92);
}
