#ifndef TLA_OVERLAY_6AD_ROOM_H
#define TLA_OVERLAY_6AD_ROOM_H

#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "field_event.h"
#include "flag_call.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define S16_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define U16_AT(base, offset) (*(u16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct SceneWork {
    u8 unknown_000[0x166];
    u16 unknown_166;
    u8 unknown_168[0x44];
    s32 unknown_1ac;
    u8 unknown_1b0[0xb88];
    u8 unknown_d38;
};

struct Box {
    s32 word[6];
};

#define SCENE_WORK ((struct SceneWork *)Ram_HeapSlots->event_work)

struct FieldActor *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 flags);
void Animation_ApplyChildValues(struct FieldActor *object, s32 value);
void Object_SetModeById(u32 id, s32 mode);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
void Motion_CamBounds(s32 x, s32 y, s32 z, s32 use_setter);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
s32 Map_GetTerrainHeight(s32 terrain, s32 x, s32 z);
void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);
u32 Random16Far(void);

void Func_020027ac(s32 index, s32 value);
s32 Func_020027dc(s32 index);
s32 Func_02004a8c(struct Box *box);
void Func_02004d10(struct Box box);
void Func_02005490(struct FieldActor *object, const void *script);
struct FieldActor *Func_02005498(s32 kind, s32 x, s32 y, s32 z);
void Func_020054e0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020054e8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_020054f0(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
void Func_02005560(void);
void Func_02005568(void);
void Func_02005598(s32 id, s32 x, s32 z);
void Func_020055d0(s32 a0, s32 a1);
void Func_020055e0(void);
void Func_020055f0(s32 a0);
s32 Func_020055f8(s32 id);
void Func_02005600(void);
void Func_02005608(void);
void Func_02005648(s32 a0);
void Func_02005660(void);
struct FieldActor *Func_02005680(s32 *position, s32 mode);
void Func_02005688(void);
void Func_020056c0(s32 cue);

void Func_020001f4(s32 unused, s32 id);
void Func_020002ec(void);
void Func_020003b4(void);
void Func_0200055c(s32 id);
void Func_02001448(void);

static inline void Func_020054e0_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants; direct calls share registers or load the stack arguments one at a time */
    Func_020054e0(a0, a1, a2, a3, a4, a5);
}

static inline void Func_020054e8_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants; direct calls share registers or load the stack arguments one at a time */
    Func_020054e8(a0, a1, a2, a3, a4, a5);
}

static inline void Func_020054f0_(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants; direct calls share registers or load the stack arguments one at a time */
    Func_020054f0(a0, a1, a2, a3, a4, a5);
}

static inline void Func_020027ac_(s32 index, s32 value)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants; direct calls share registers or load the stack arguments one at a time */
    Func_020027ac(index, value);
}

static inline void Func_02005598_(s32 id, s32 x, s32 z)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants; direct calls share registers or load the stack arguments one at a time */
    Func_02005598(id, x, z);
}

static inline void Vector_AddPolar(s32 length, s32 angle, s32 *position)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants; direct calls share registers or load the stack arguments one at a time */
    Vector_AddPolarOffsetFar(length, angle, position);
}

static inline void Camera_SetBounds(s32 x, s32 y, s32 z, s32 use_setter)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters keep each call's own constants; direct calls share registers or load the stack arguments one at a time */
    Motion_CamBounds(x, y, z, use_setter);
}

#endif
