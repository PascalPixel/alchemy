// Near misses of overlay 6aa: Func_02001254 differ 36 (the listing keeps one
// counter address in r5 and loads the other twice), Func_020012e4 differ 22
// (the listing reloads the counter sign-extended for the last test and builds
// its zero in r4 before the first).
#include "../../../../games/tla/src/scene/mars_lighthouse_entrance/room.h"

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
extern u8 gMapCellBuffer[];

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
s32 Resource_FindFreeEntry(void);
void VramCache_Load(s32 entry, s32 size, void *source);
void Task_Add(void (*func)(void), s32 priority);
void Actor_SetMovementTypeById(s32 id, const void *script);
void Func_02000d18(void);
void Func_02000fe0(u8 *cell);
void Func_020012e4(struct Actor *object);
void Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);
void Func_02002c2c(void);
void Func_02004c88(s32 scene, s32 a1);
void Func_02004d98(void);

void Func_02001254(void) {
  if (gData_020068a0 < 60) {
    if (gData_02006868 == 0) {
      Func_0200246c(0, 3);
      gData_02006868 = 1;
    }
  } else {
    if (gData_02006868 == 1) {
      Func_0200246c(0, 67);
      gData_02006868 = 0;
    }
  }
  if (++gData_020068a0 > 300)
    gData_020068a0 = 0;
}

void Func_020012e4(struct Actor *object) {
  s16 *count = &object->action;
  s32 zero;

  ((struct SpriteParts *)object->animation)->unk12 += 0x800;
  (*count)++;
  zero = 0;
  if (*count == 8)
    object->velocityY = 0x40000;
  if (*count > 9) {
    object->x += 0x40000;
    object->scaleX += 0xfffff800;
    object->scaleY += 0xfffff800;
  }
  if (*count == 32) {
    object->x = zero;
    object->z = zero;
    object->update = (void (*)(void))zero;
  }
}
