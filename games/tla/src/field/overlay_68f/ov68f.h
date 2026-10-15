#ifndef TLA_OVERLAY_68F_OV68F_H
#define TLA_OVERLAY_68F_OV68F_H

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "object_runtime.h"
#include "party_state.h"
#include "scene_ids.h"
#include "event_work.h"
#include "resource.h"
#include "mem.h"
#include "call.h"
#include "field_effect.h"

struct SceneEvent;

struct Ov68fEffectParams {
    u8 unknown_00[0x18];
    u16 kind;
    u8 unknown_1a[0xe];
};

struct Ov68fSparkWork {
    u8 unknown_0000[0x1604];
    s16 resource;
};

struct Ov68fPosition {
    s32 x;
    s32 y;
    s32 z;
};

struct ObjectRuntime *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
s32 Random16Far(void);
void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 flags);
void OverlayObject_WaitUntilIdle(struct ObjectRuntime *object);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
s32 MapCell_SetAttributes(u32 layer, s32 x, s32 z, s32 value);
void Func_020000b8(s32 x, s32 y, s32 z, s32 unused, s32 speed, s32 phase, s32 life,
                   struct Ov68fEffectParams *params);
void Func_02000708(struct Ov68fPosition *position, s32 count, s32 speed);
void Func_02002c1c(s32 a, s32 b, s32 owner);
void Func_02002cc0(void);
void Func_02002fc0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02002fe8(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02003048(void);
void Func_02003050(void);
void Func_02003088(s32 id, s32 x, s32 z);
void Func_020030c0(s32 id);
void Func_02003100(void);
void Func_02003140(void);
void Func_02003148(s32 a);
void Func_02003170(const u8 *script);

extern const u8 MsgThereSSomething[];
extern const u8 Data_02003264[];
extern const u8 Data_0200326e[];
extern u16 Data_02003cd0;

void Func_02002e00(void);
void Func_02002d44(s32 x, s32 y, s32 z, s32 frames);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Func_0200137c(void);
extern const u8 Data_020037f8[];
extern const u8 Data_0200381c[];
extern const u8 Data_0200393c[];
extern const u8 Data_020039e4[];
extern const u8 Data_02003a5c[];
extern const u8 Data_02003b28[];
extern const u8 Data_02003c90[];

void Func_02000840(struct ObjectRuntime *object);
void Func_02002ebc(void);

void Func_02003000(s32 a, s32 b, s32 c);
void Func_02003108(s32 color, s32 b);
void Func_02003110(s32 color, s32 b);
void ObjectMotion_ResetAndSetPosition(s32 actor, s32 x, s32 z);

static inline void Ov68f_FadeTo(s32 color, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer; as direct calls their constants are shared in registers. */
    Func_02003108(color, b);
}

static inline void Ov68f_FadeFrom(s32 color, s32 b)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer; as direct calls their constants are shared in registers. */
    Func_02003110(color, b);
}

static inline void Ov68f_ObjPlace(s32 actor, s32 x, s32 z)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer; as a direct call the actor loads before the coordinates. */
    ObjectMotion_ResetAndSetPosition(actor, x, z);
}

static inline void Ov68f_SetWork(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer; as direct calls their constants are shared in registers. */
    Func_02003000(a, b, c);
}

#endif
