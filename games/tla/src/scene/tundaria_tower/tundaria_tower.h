#ifndef TLA_TUNDARIA_TOWER_H
#define TLA_TUNDARIA_TOWER_H

#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/frame.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "event_work.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/field_effect.h"

#include "scene_tables.h"

struct TundariaTowerSparkWork {
  u8 unk0000[0x1604];
  s16 resource;
};

struct TundariaTowerPosition {
  s32 x;
  s32 y;
  s32 z;
};

void GameFlag_Set(s32 flag);
void GameFlag_Clear(s32 flag);
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value);
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void Func_020006d4(union FieldObject *object);
void Func_02000708(struct TundariaTowerPosition *position, s32 count,
                   s32 scale);
void Func_02002c1c(s32 a, s32 b, s32 owner);
void Func_02002cc0(void);
void MapGrid_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e,
                                        s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003050(void);
void Actor_SetHorizontalPositionWithTerrain(s32 id, s32 x, s32 z);
void Func_020030c0(s32 id);
void Import_LeaderStopMoving(void);
void Import_EventResolveAllPendingActions(void);
void Event_ResolvePendingActions(s32 a);
void Func_02003170(const s16 *script);

extern u8 MsgThereSSomething[];
extern s16 gData_02003264[];
extern s16 gData_0200326e[];
extern u16 gData_02003cd0;

void Func_02002e00(void);
void Rocks_Spawn(s32 x, s32 y, s32 z, s32 frames);
void Func_0200137c(void);
extern struct Trigger gData_020037f8[];
extern struct Trigger gData_0200381c[];
extern struct Trigger gData_0200393c[];
extern struct Trigger gData_020039e4[];
extern struct Trigger gData_02003a5c[];
extern struct Trigger gData_02003b28[];
extern struct Trigger gData_02003c90[];

void Func_02000840(struct Actor *object);
void Func_02002ebc(void);

void MapGrid_SetScreenShake(s32 a, s32 b, s32 c);
void BattleEffect_ApplyColorToTargetBuffer(s32 color, s32 b);
void BattleEffect_ApplyColorToSourceBuffer(s32 color, s32 b);

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static inline void TundariaTower_FadeFrom(s32 color, s32 b) {
  BattleEffect_ApplyColorToSourceBuffer(color, b);
}

static inline void TundariaTower_SetWork(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

#endif
