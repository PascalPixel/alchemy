#ifndef ALHAFRAN_CAVE_LOCAL_H
#define ALHAFRAN_CAVE_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "event_inline.h"
#include "field/field.h"

extern char SceneId_Ov66aRoom1;
extern char SceneId_Ov66aRoom2;
extern u8 MsgOhStoppedBy;

struct AlhafranCaveMapCell {
  u8 unk0[2];
  u8 state;
  u8 unk3;
};

struct AlhafranCaveMapLayer {
  struct AlhafranCaveMapCell *cells;
  u8 unk04[0x34];
};

struct AlhafranCaveMapWork {
  u8 unk000[0x17];
  u8 low17 : 2;
  u8 mode17 : 2;
  u8 high17 : 4;
  u8 unk018[0x120];
  struct AlhafranCaveMapLayer layers[1];
};

struct AlhafranCaveEventWork {
  u8 unk000[0x1ac];
  s32 value1ac;
  u8 unk1b0[0xb9a];
  u16 valueD4a;
  u16 valueD4c;
};

struct AlhafranCaveEffectWork {
  u8 unk00[0x1a];
  s16 objectId;
};

struct AlhafranCaveSwitch {
  u16 id;
  s16 kind;
  s16 flag;
};

void Actor_ApplyRandomChildValues(void);
void Func_020013f0(struct Actor *object, s32 mode);
void Func_02001420(s32 x, s32 z, s32 a, s32 b);
void Func_02001580(const void *data);
s32 Func_02001588(void);
void Func_020015d8(void);
void Func_020015f0(const void *data, s32 x, s32 y, s32 z);

void Func_020002f8(void);
void Func_0200051c(void);
void Func_0200129c(const u16 *entry, s32 mode, s32 frame);
void Func_02001320(s32 id, u32 kind, s32 flag, s32 layer);

extern s16 gData_02001608[];
extern const u16 gData_020016e4[];
extern u8 gData_02001778[];
extern u8 gData_02001868[];
extern u8 gData_02001a18[];
extern u8 gData_02001a90[];
extern u8 gData_02001be0[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Func_02001420_(s32 x, s32 z, s32 a, s32 b) {
  Func_02001420(x, z, a, b);
}

static __inline__ void Func_020015d0_(s32 x, s32 z, s32 a, s32 b) {
  Func_080dbdd4(x, z, a, b);
}

static __inline__ void Func_02001400_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

static __inline__ void Func_02001408_(s32 a, s32 b, s32 c, s32 d, s32 e,
                                      s32 f) {
  Func_0802b450(a, b, c, d, e, f);
}

#endif
