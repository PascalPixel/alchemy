// Draft, differ 24 of 724, skeleton gap 24 (tla-en, 2026-11-01): an event command run on an object (targetId): 48
// runes (FieldEffect_Runes) gather on it in a 16-by-3 grid while pebbles
// (FieldEffect_Pebbles) fly up from it for 40 frames; the object takes mode
// 2, its update becomes Func_080e9e8c and it plays Data_080f1040; it is then
// shrunk away (cue 134, a burst of 25 pebbles), the runes flash in a wave,
// the controlled owner turns to face it and takes mode 28, and the runes
// circle (Func_080e9f68, as a frame callback, with the work's state and
// origin set here), drift to the owner, close in, and the owner plays
// gData_080f3614 for 60 frames. The owner is set back to full size.
// Same instructions and registers everywhere; what is left is order in six
// places, 12 rows each way: the pool load of gFrameCount sits before the zero
// stores (the native has it after); add r7, r9 comes a call later and movs r6
// a row later than the native; the sparkTile strh comes after the pebble
// pointers (the native stores it first, before r5 is rebuilt); case 2 negates
// the -1 before storing state; and the two origin stores order their movs one
// row apart. Those stores are the one soft spot: without the startX and
// startZ locals the rows order right, but pointer and value swap r2 and r3
// (differ +16). Tried for the rest, no gain: every placement of the mote
// pointer, every order of the second loop setup with up or down counting, the
// zero stores as statements, a colour temporary, pointer locals for the origin
// and moving fields, the outer loop as while or goto (same code).
// What fixed the frame and allocation, to keep: declaration order is the
// stack order, state to nextSpark; one shared i, whose life makes it r8; rune
// scoped to the flash loop (r0) and delay to case 4 (r3), which leave r4 to
// wave and keep i from it.
#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
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

void *HeapSlot_Alloc(s32 slot, s32 size);
void HeapSlot_Release(s32 slot);
s32 Party_GetLeaderActor(void);
void Field_StartColorTransition(s32 frames, s32 color);
void Camera_WorldToScreen(s32 *position);
void *Resource_GetTableEntry(s32 resourceId);
s32 Resource_DecodeType01(const void *source, void *destination);
s32 Resource_FindFreeEntry(void);
s32 VramCache_Load(s32 slot, s32 size, const void *data);
void Resource_ResetEntry(s32 slot);
u32 Random16(void);
void wait(s32 frames);
void Sound_PlayFar(s32 cue);
void Event_Wait(s32 frames);
void Actor_StartScriptFar(struct Actor *object,
                                  const void *script);
void Task_Add(void (*func)(void), s32 order);
void Task_Remove(void (*func)(void));
void Actor_ArmCallback(s32 id, s32 value, s32 flags);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void BattleEffect_PrepareBufferInterpolation(void);
void Func_080e9e8c(void);
void Func_080e9f68(void);

void Func_080ea14c(s32 targetId) {
  struct MoteWork *work = HeapSlot_Alloc(92, 0x1d60);
  s32 ownerId = Party_GetLeaderActor();
  struct Actor *owner = ObjectTable_Get(ownerId);
  struct Actor *target = ObjectTable_Get(targetId);
  s32 state;
  s32 moteEntry;
  s32 sparkEntry;
  s32 ownerX;
  s32 ownerZ;
  s32 targetX;
  s32 targetZ;
  s32 sparks = 0;
  s32 nextSpark = 0;
  s32 pos[3];
  struct EffectSprite *sprite;
  struct Mote *mote;
  s32 moteTile;
  s32 sparkTile;
  s32 timer;
  s32 wave;
  s32 delay;
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
  i = 0;
  sprite = work->moteSprites;
  for (; i < 48; i++) {
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
  wait(1);
  Sound_PlayFar(139);
  Actor_SetMode(target, 2);
  Event_Wait(60);
  target->action = 0;
  target->update = Func_080e9e8c;
  Sound_PlayFar(142);
  Event_Wait(80);
  Actor_StartScriptFar(target, Data_080f1040);
  work->moving = 0;
  Task_Add(Func_080e9f68, 0x480);
  state = 0;
  timer = 0;
  do {
    switch (state) {
    case 0:
      work->state = 0;
      if (timer <= 39) {
        if ((timer & 7) == 0)
          Sound_PlayFar(133);
        sparks = 1;
      }
      if (timer == 40) {
        Sound_PlayFar(134);
        sparks = 25;
        Actor_ResetMotion(target);
        target->scaleX = 0;
      }
      if (timer == 80) {
        state++;
        timer = -1;
      }
      break;
    case 1:
      for (i = 0; i < 48; i++) {
        struct EffectSprite *rune;
        wave = i % 16 + i / 16;
        wave = timer - wave;
        rune = &work->moteSprites[i];
        if (wave == 0)
          rune->oam.index += 32;
        if (wave == 3)
          rune->oam.index += -32;
        if (wave == 8)
          rune->oam.index += 32;
        if (wave == 10)
          rune->oam.index += -32;
        if (wave == 16)
          rune->oam.index += 32;
        if (wave == 18)
          rune->oam.index += -32;
      }
      if (timer == 0)
        Sound_PlayFar(246);
      if (timer == 56) {
        state++;
        timer = -1;
      }
      break;
    case 2:
      if (timer == 0) {
        Actor_ArmCallback(ownerId, 0x4000, 0);
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
        Actor_SetMode(owner, 28);
      if (timer == 10) {
        state++;
        timer = -1;
      }
      break;
    case 4:
      if (timer == 0) {
        mote = work->motes;
        delay = 0;
        for (i = 47; i >= 0; i--) {
          mote->timer = delay;
          delay -= 3;
          mote++;
        }
        work->radius = 0x3c0000;
        work->angle = 0xc000;
        {
          s32 startX = 0x820000;
          s32 startZ = 0x1e0000;
          work->originX = startX;
          work->originZ = startZ;
        }
        work->moving = 1;
        work->spacing = 0x40000;
        work->state = 2;
        if (timer == 0)
          Sound_PlayFar(187);
      }
      if (timer == 48)
        Sound_PlayFar(187);
      if (timer == 96)
        Sound_PlayFar(187);
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
        Actor_StartScriptFar(owner, gData_080f3614);
      if ((timer & 3) == 0)
        Sound_PlayFar(167);
      work->state = 3;
      if (timer == 60)
        state = 999;
      break;
    }
    for (i = 0; i < sparks; i++) {
      angle = Random16();
      mote = &work->sparks[nextSpark];
      mote->timer = 0;
      mote->x = target->x;
      mote->z = target->y + 0xc0000;
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
    wait(1);
    timer++;
  } while (state != 999);
  Actor_ResetMotion(owner);
  owner->scaleX = 0x10000;
  owner->scaleY = 0x10000;
  Event_Wait(10);
  Task_Remove(Func_080e9f68);
  BattleEffect_PrepareBufferInterpolation();
  Resource_ResetEntry(moteEntry);
  Resource_ResetEntry(sparkEntry);
  HeapSlot_Release(92);
}
