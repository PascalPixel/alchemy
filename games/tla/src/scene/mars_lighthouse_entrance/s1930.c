#include "room.h"

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 gData_0200503c[];
extern const u8 gData_0200547c[];
extern const u8 gData_020054cc[];
extern const u8 gData_02005528[];
extern const u8 gData_02005578[];
extern const u8 gData_020055d4[];
extern const u8 gData_02005644[];
extern const u8 gData_0200610c[];
extern const u8 gData_02006118[];
extern const u8 gData_0200613c[];
extern const u8 gData_02006250[];
extern const u8 gData_020063b8[];
extern const u8 gData_020064fc[];
extern const u8 gData_02006628[];
extern const u8 gData_02006778[];
extern s16 gData_02006868;
extern s16 gData_020068a0;
extern u16 gData_020068a2;

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
s32 Resource_FindFreeEntry(void);
void Func_02000d18(void);
void Func_02000fe0(u8 *cell);
void Func_020012e4(struct Actor *object);
s32 Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);
void Func_02002c2c(void);
void Func_02004c88(s32 scene, s32 a1);
void Func_02004d98(void);

s32 Func_02001930(void) {
  if (Flag_IsSet(0xa30))
    Func_02004c88(gMainRoutineWork.map +
                      ((s32)&SceneId_Ov6ACRoom3 - (s32)&SceneId_Ov6a8Room2),
                  0x5000000);
  return 0;
}

void Func_0200196c(void) { Func_02002f4c(10, 19, 20, 0, gData_0200547c); }

void Func_02001988(void) { Func_02002f4c(12, 19, 20, 0, gData_020054cc); }

void Func_020019a4(void) { Func_02002f4c(9, 18, 20, 0x4000, gData_02005528); }

void Func_020019c4(void) { Func_02002f4c(9, 13, 14, 0, gData_02005578); }

void Func_020019e0(void) { Func_02002f4c(11, 13, 14, 0x4000, gData_020055d4); }

void Func_02001a00(void) { Func_02002f4c(13, 21, 22, 0, gData_02005644); }

extern const u8 gData_020056a0[];

void Func_02001f90(struct Actor *object);

s32 Func_02001a1c(struct Actor *a, struct Actor *b) {
  if (ABS(a->x - b->x) < 0x80000 && ABS(a->y - b->y) < 0x80000 &&
      ABS(a->z - b->z) < 0x80000)
    return 1;
  return 0;
}

s32 Func_02001a80(struct Actor *a, struct Actor *b) {
  s32 dy;

  if (ABS(a->x - b->x) < 0xc0000) {
    dy = a->y - b->y;
    if (dy > -0x80000 && dy < 0x180000 && ABS(a->z - b->z) < 0xc0000)
      return 1;
  }
  return 0;
}

#include "field/event_work.h"

struct RideSlot {
  u16 id;
  u16 blocked;
};

struct RideWork {
  u8 unk00[4];
  u16 landed;
  u16 blocked;
  u8 unk08[2];
  u16 value0a;
  u8 unk0c[0x48];
  struct RideSlot slots[4];
  u8 unk64[4];
  u16 value68;
  u16 direction;
};

extern u8 gData_020023c4[];

void Func_02001ae0(struct Actor *object) {
  struct RideWork *work = (struct RideWork *)(gData_020023c4 + 0x188);
  struct Actor *actor = Actor_Get(gMainRoutineWork.currentOwner);
  struct EventWork *event = Ram_HeapSlots->eventWork;
  s32 dx = object->bounce;
  s32 dz;
  s32 i;
  s32 direction;
  s32 id;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct SpriteParts *)object->animation)->unk12 += object->action;
  if (actor->y == actor->terrainHeight && Func_02001a1c(object, actor) &&
      gMainRoutineWork.renderMode == 0) {
    if (object->unk4c != 0) {
      work->value68 = 0;
      {
        u16 *cell = &work->direction;

        if (object->unk4c > 0) {
          direction = 1;
          *cell = direction;
        } else {
          direction = 0xffff;
          *cell = direction;
        }
      }
    }
    object->priorityFlags = 2;
    {
      s32 speed = object->unk4c;

      object->unk4c = 0;
      object->z -= speed;
      object->gravity += speed >> 2;
    }
    work->landed = 1;
    event->exit = work->value0a;
  }
  for (i = 0; i < 4 && (id = work->slots[i].id) != 0; i++) {
    actor = Actor_Get(id);

    if (actor->y == actor->terrainHeight && Func_02001a80(object, actor)) {
      if (work->slots[i].blocked == 0) {
        s32 speed;

        object->priorityFlags = 2;
        speed = object->unk4c;
        object->unk4c = 0;
        object->z -= speed;
        object->gravity += speed >> 2;
      } else {
        work->blocked = 1;
      }
    }
  }
}
