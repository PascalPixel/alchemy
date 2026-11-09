#ifndef TLA_ANKOHL_RUINS_H
#define TLA_ANKOHL_RUINS_H

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "field_sprite.h"
#include "event_inline.h"

/// The overlay's view of the event work heap slot.
struct AnkohlRuinsEventWork {
  u8 unk000[0x154];
  s16 unk154; ///< 66 when Func_020010f4's step is refused; 40 lets it run
  u8 unk156[0x14];
  s16 alarm;
  u8 unk16c[0x60];
  /// Moved with the leader while the raft (object 9) carries them.
  struct Actor *unk1cc;
};

s32 Random16Far(void);

extern s16 gData_02003780;
extern s16 gData_02003782;
extern s16 gData_02003784;
extern s32 gData_02003788;
extern s32 gData_0200378c;

void Func_02000b30(s32 id, s32 cue);
void Func_0200160c(void);
void Func_020019a0(void);
void Func_02001f7c(void);
s32 Func_02002dcc(void (*callback)(void));
void Func_02002e44(void);
void Func_02002e4c(void);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline s32 AnkohlRuins_FlagCheck(s32 flag) {
  return GameFlag_Test(flag);
}

static inline void AnkohlRuins_Call2e24(s32 a, s32 b, s32 c) {
  Map_SetScreenShake(a, b, c);
}

static inline void AnkohlRuins_CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  Camera_MoveWithinMapBounds(x, y, z, mode);
}

static inline void AnkohlRuins_Call2df4(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

static inline void AnkohlRuins_Call2dfc(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f) {
  Map_CopyCellAttributeRect(a, b, c, d, e, f);
}

void Actor_SetSingleChildFlags(struct Actor *object,
                                        s32 flags);
void Func_02002e34(struct Actor *object, s32 a);
void Func_02002eb4(void);

#endif
