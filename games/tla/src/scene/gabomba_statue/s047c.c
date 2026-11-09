#include "types.h"

#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_inline.h"

struct PuzzleCell {
  u8 kind;
  u8 blocked;
  u8 state;
  u8 occupied;
};

struct SceneState {
  struct PuzzleCell cells[88];
  u8 unk160[4];
  s16 firstId;
  s16 endId;
  u16 tileEntry;
  s8 redrawA;
  s8 redrawB;
  s16 originX;
  s16 originZ;
};

struct EventWork1cc {
  u8 unk000[0x1cc];
  struct Actor *object;
};

struct IoReg16 {
  u16 value;
};

#define REG_BLDALPHA ((struct IoReg16 *)0x04000052)
#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

extern struct SceneState gSceneState;
extern s32 gData_02005af0;
extern const s8 gData_02005940[];
extern const u8 gData_02005948[];

void *Heap_Alloc(s32 size);
void Heap_Release(void *block);
s32 Resource_FindFreeEntry(void);
struct Actor *Func_020005b4(struct Actor *origin, s32 id);
void Func_02001950(s32 show, s32 first, u32 count);
void Func_02002394(void);
void Func_02004380(s32 x, s32 z, s32 a);
void Func_020055cc(const u8 *data, void *buffer);
void Func_0200561c(void *animation, s32 a);
void Func_02005624(struct Actor *object, s32 mode);
void Func_020056b4(struct Actor *object);
void Func_020056bc(void);
void Func_02005824(void);
void Func_0200582c(s32 index, s32 a, s32 b);
void Func_0200583c(s32 index, s32 flag, s32 a, s32 b);

s32 Func_0200047c(struct Actor *object) {
  struct Actor *owner = object->watchedObject;

  if (owner != NULL) {
    u8 *phase = &OBJECT_BYTE(object, 98);

    object->x = owner->x;
    object->y = owner->y + 0x100000;
    object->z = owner->z;
    object->angle = *phase << 8;
    FieldVector_AddPolarOffset(0x60000, *phase << 8, &object->x);
    *phase += 16;
  }
  return 1;
}
