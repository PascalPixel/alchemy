#ifndef TLA_OVERLAY_661_SCENE_H
#define TLA_OVERLAY_661_SCENE_H

#include "types.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "scene_ids.h"

struct SceneEventWork {
    u8 unknown_000[0x16a];
    u16 unknown_16a;
    u8 unknown_16c[0x40];
    s32 screen_effect;
};

extern const u8 Data_02004768[];
extern const u8 Data_0200478e[];
extern const u8 Data_020047d2[];
extern const u8 Data_020047ec[];
extern const u8 Data_02004800[];
extern s32 Data_02004808;

struct FieldActor *Object_GetById(s32 id);
void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void UiText_ShowPositionedMessageAndWait(s32 message, s32 position);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void Object_SetWideSprite(s32 actor, s32 value);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);
void Scheduler_RemoveCallbackFar(void (*callback)(void));
void Field_DispatchTypeHandler(s32 type);

void Func_020006ac(void);
void Func_02001594(void);
void Func_020015ec(s32 show);
void Func_0200163c(void);
void Func_02001650(s32 show, s32 first, u32 count);
void Func_02001e58(void);
void Func_02002010(void);
void Func_02002c6c(void);
void Func_02002ce0(s32 id);
void Func_02002d10(void);
void Func_02002d54(void);
void Func_02002d6c(struct FieldActor *object, s32 x, s32 z, s32 mode);
void Func_020032c4(void);
void Func_02003820(const void *script);
void Func_0200388c(const void *script, s32 a, s32 b);
void Func_02003ff0(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004000(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02004040(void);
void Func_02004048(void);
void Func_02004090(s32 actor, s32 x, s32 z);
void Func_020040c8(s32 message);
s32 Func_020040d8(s32 speaker, s32 flags);
void Func_020040f8(s32 actor);
void Func_02004148(s32 a, s32 b);
void Func_02004178(s32 a, s32 b, s32 c);
void Func_02004180(s32 value);
void Func_02004190(s32 a, s32 b);
void Func_02004198(s32 a, s32 b);
void Func_020041a0(void);
void Func_020041a8(void);
void Func_020041b0(void);
void Func_020041b8(s32 skip);
void Func_020041c8(s32 cue);
s32 Func_020041e0(struct FieldActor *object);

#define SCENE_TASK_ADD_LATE(callback, priority) \
    do { \
        s32 priority_; \
        priority_ = (priority); \
        /* FAKEMATCH: a barrier keeps the callback load after the priority. */ \
        __asm__ volatile("" : : "r"(priority_)); \
        Scheduler_AddOrUpdateCallback((callback), priority_); \
    } while (0)

static inline void Scene_Call4178(s32 a, s32 b, s32 c)
{
    /* FAKEMATCH: scene calls reach the engine through one inline layer. */
    Func_02004178(a, b, c);
}

#endif
