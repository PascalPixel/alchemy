#ifndef TLA_CHAMPA_CLIFF_CAVES_SCENE_H
#define TLA_CHAMPA_CLIFF_CAVES_SCENE_H

#include "types.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "scene_call.h"
#include "text/msg_ids.h"

struct WarpTarget {
  s32 scene;
  u16 x;
  u16 z;
};

struct RiseObject {
  u8 unk00[0x18];
  s32 scale;
  s32 scaleCopy;
  u8 unk20[0x4c];
  s32 update;
};

extern const struct WarpTarget gData_02003d10;

struct Actor *Actor_GetById(u32 objectId);
void Actor_SetSpeedParameters(s32 id, s32 speed, s32 acceleration);
void Actor_CenterOnColumn(u32 objectId, s32 action, s32 z_offset);
void Actor_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);

void Func_0200016c(const struct WarpTarget *warp, s32 door, s32 mode);
void Map_PlayMapEdit(s32 scene, s32 x, s32 z);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0802b450(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_0200345c(void);
void Func_02003464(void);
void Func_02003474(s32 a, s32 b, s32 c);
void Actor_ArmCallbackAndRefresh(s32 actor, s32 angle);
void Func_02003624(struct Actor *object, struct Actor *target);
void Sound_Play(s32 cue);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_CenterOnColumn_(s32 id, s32 column, s32 offset) {
  Actor_CenterOnColumn(id, column, offset);
}

static __inline__ void Actor_OffsetPositionAndResetMotion_(s32 id, s32 dx,
                                                           s32 dz) {
  Actor_OffsetPositionAndResetMotion(id, dx, dz);
}

static __inline__ void Func_02003534_(s32 actor, s32 angle) {
  Actor_ArmCallbackAndRefresh(actor, angle);
}

#endif
