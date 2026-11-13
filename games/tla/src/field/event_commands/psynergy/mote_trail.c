/// @file
/// Three routines not yet understood.
// Part of psynergy.c.

#include "../psynergy_internal.h"

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
s32 Decompress_TaggedLz(const void *source, void *destination);
s32 VramCache_FindFreeEntry(void);
s32 VramCache_Load(u32 slot, u32 size, const void *source);
s32 VramCache_FreeEntry(u32 slot);
u32 Random16(void);
void wait(s32 frames);
void Event_Wait(s32 frames);
void Actor_StartScriptFar(struct Actor *object, const void *script);
void Actor_ArmCallback(s32 id, s32 value, s32 flags);
void Vector_AddPolarOffset(s32 distance, s32 angle, s32 *position);
void FieldPsynergy_FadeFromCastColor(void);
/* VIEW(f0ea14c): routine Func_080e9e8c as void Func_080e9e8c(void); the
 * definition has void Func_080e9e8c(struct Actor *object). */
extern void Func_080e9e8c__f0ea14c(void) __asm__("Func_080e9e8c");
void Func_080e9f68(void);

void Func_080ea14c(s32 targetId) {
  struct MoteWork *work = HeapSlot_Alloc(92, sizeof(struct MoteWork));
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
  s32 startX = 130 * Q16_ONE;
  s32 startZ = 30 * Q16_ONE;

  Field_StartColorTransition(8, gData_080f3954[gFrameCount & 7]);
  pos[0] = owner->x;
  pos[1] = owner->y + 24 * Q16_ONE;
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
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Runes), work);
  moteEntry = VramCache_FindFreeEntry();
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
    mote->targetX = ((i % 16) * 3 << 18) + 30 * Q16_ONE;
    mote->targetZ = ((i / 16) * 3 << 19) + 30 * Q16_ONE;
    mote->timer = -i;
    sprite++;
    mote++;
  }
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_Pebbles), work);
  sparkEntry = VramCache_FindFreeEntry();
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
  target->timer = 0;
  target->update = Func_080e9e8c__f0ea14c;
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
        work->radius = 60 * Q16_ONE;
        work->angle = 0xc000;
        work->originX = startX;
        work->originZ = startZ;
        work->moving = 1;
        work->spacing = 4 * Q16_ONE;
        work->state = 2;
      }
      if (timer == 0)
        Sound_PlayFar(187);
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
      work->originX = timer * (ownerX - startX) / 30 + startX;
      work->originZ = timer * (ownerZ - startZ) / 30 + startZ;
      if (timer == 30) {
        state++;
        timer = -1;
      }
      break;
    case 6:
      if (timer == 0)
        work->spacing = 0;
      work->radius += -2 * Q16_ONE;
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
      mote->z = target->y + 12 * Q16_ONE;
      mote->unk08 = target->z;
      Vector_AddPolarOffset(Random16() * 4 + 2 * Q16_ONE, angle, &mote->x);
      mote->targetX = 0;
      mote->targetZ = Q16_ONE * 9 / 2;
      mote->unk14 = 0;
      Vector_AddPolarOffset(Random16() * 2 + 2 * Q16_ONE, angle,
                            &work->sparks[nextSpark].targetX);
      nextSpark = (nextSpark + 1) & 31;
    }
    sparks = 0;
    wait(1);
    timer++;
  } while (state != 999);
  Actor_ResetMotion(owner);
  owner->scaleX = Q16_ONE;
  owner->scaleY = Q16_ONE;
  Event_Wait(10);
  Task_Remove(Func_080e9f68);
  FieldPsynergy_FadeFromCastColor();
  VramCache_FreeEntry(moteEntry);
  VramCache_FreeEntry(sparkEntry);
  HeapSlot_Release(92);
}

#include "field/field.h"

struct ActorFlag {
  u16 actor;
  s16 flag;
};

struct Actor *Actor_GetById(u32 objectId);
s32 Func_080eab70(struct Actor *object);
s32 GameFlag_Test(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Func_080eaa14(struct ActorFlag *list);

void Func_080ea7c0(struct ActorFlag *actors) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  struct ActorFlag *list = actors;
  struct Actor *leader;
  s32 height;

  for (; actors->actor != 0xffff; actors++) {
    struct Actor *object = Actor_GetById(actors->actor);
    u16 flag = actors->flag;
    s32 slot = Func_080eab70(object);
    struct MapCell *cell = &map->layers[object->terrainId].cells[slot];

    object->motionFlags = 0;
    object->collisionFlags &= ~1;
    Actor_SetSingleChildFlagsFar(object, 0);
    cell->collisionCode = 0xff;
    if (GameFlag_Test(flag)) {
      Actor_SetMode(object, 0);
      GameFlag_ClearBit(flag);
    } else {
      Actor_SetMode(object, 1);
    }
  }
  Func_080eaa14(list);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  height = MapGrid_GetTerrainHeightFar(leader->terrainId, leader->x, leader->z);
  if (leader->y < height)
    leader->y = leader->terrainHeight = height;
}

struct MenuFlagWork {
  u8 unk000[0x506];
  u8 busy;
};

struct DescriptorEntry {
  s16 id;
  u8 unk02[6];
};

extern void *gOverlayArea[];
extern struct DescriptorEntry gData_080eff28[];

void BattleEffect_StartTiledCanvas(s32 mode);
void BattleEffect_EndCanvasLayer(void);
void Func_0814ea58(void *effect, s32 mode);
void PartyStatus_CreateWindow(s32 flags);
s32 MapGrid_GetTerrainHeightAboveFar(s32 x, s32 z, s32 layer);
s32 Func_080dbde8(s32 x, s32 z, s32 layer);
u8 Func_080dbdf4(s32 x, s32 z, s32 layer, s32 value);
void Shop_InitializeWork(void);
s32 Inn_RoomPrice(s32 innId);
void Shop_ReleaseWork(void);

void Func_080ea8a8(s32 x, s32 z, s32 layer) {
  if (MapGrid_GetTerrainHeightAboveFar(x, z, layer) == 0)
    Func_080dbdf4(x, z, layer, Func_080dbde8(x, z, layer) | 0x80);
}
