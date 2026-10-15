#ifndef TLA_MARS_ROOM_M_H
#define TLA_MARS_ROOM_M_H

#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "map.h"
#include "input.h"
#include "call.h"
#include "iwram_call.h"
#include "text/msg_ids.h"

struct MarsRoomMEventWork {
    u8 unk000[0x154];
    s16 exit;
    u8 unk156[0x10];
    s16 field166;
    u8 unk168[0x44];
    s32 field1ac;
};

struct MarsRoomMVector {
    s32 x;
    s32 y;
    s32 z;
};

struct MarsRoomMAnimation {
    u8 unk00[9];
    u8 unused09 : 2;
    u8 layer : 2;
    u8 unk0a[6];
    u8 value10;
    u8 flag11 : 1;
    u16 angle;
};

struct MarsRoomMRideWork {
    s32 counter;
    s16 active;
    u8 unk06[6];
    u16 value0c;
    s16 value0e;
    u8 unk10[0x60];
    struct ObjectRuntime *handle;
};

extern u32 gFrameCount;
extern u8 gData_020023c4[];
extern char SceneId_Ov6ACRoom1;
extern char SceneId_Ov6ACRoom2;
extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov6ACRoom4;
extern char SceneId_Ov6ACRoom5;
extern char SceneId_Ov6ACRoom6;

void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void Object_SetPartAttribute(struct ObjectRuntime *object, s32 value);
void Scheduler_AddOrUpdateCallback(void (*callback)(void), s32 priority);

void Func_02003118(struct ObjectRuntime *object, s32 value);
void Func_02003120(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_02003128(s32 kind, s32 x, s32 y, s32 z);

void Func_02001988(void);
void Func_02001aec(struct ObjectRuntime *object);
void Func_02001cc4(struct ObjectRuntime *object, s32 layer);
void Func_02001b64(void);

extern const u8 gData_020032d4[];
extern const u8 gData_020034f0[];
extern const u8 gData_02003508[];
extern const u8 gData_02003598[];

extern u16 gData_02003918;
extern u16 gData_02003920;
extern u16 gData_02003924;
extern u16 *gData_02003928;
extern u16 gData_02003934;
extern u16 gData_02003938;
extern const u8 gData_020036b8[];
extern const u8 gData_020036c8[];
extern const u8 gData_02003708[];
extern const u8 gData_02003858[];
extern const u8 gData_0200387c[];
extern const u8 gData_02003900[];

s32 Func_020016e8(u16 *list);
void Func_0200170c(void);
void Func_0200166c(void);
void Func_0200168c(void);
s32 Func_02001de0(struct ObjectRuntime *a, struct ObjectRuntime *b);
void Func_02001e38(struct ObjectRuntime *object);
void Func_02001ed4(struct ObjectRuntime *object);
void Func_020012a4(s32 x, s32 z, s32 kind);
void Func_02002ad4(void);
void Func_020031e0(void);
void Func_020031e8(void);
void Func_02003280(s32 value);
void Func_02003250(s32 color, s32 mode);
void Func_02003248(s32 color, s32 mode);
void Func_02003258(s32 frames);
void Func_020031c0(s32 message, s32 a, s32 b);
void Func_020032b8(s32 cue);
void Func_020032a0(void);
void Func_02003148(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
u32 Random16Far(void);
s32 Math_Cosine(s32 angle);
void Animation_SetStateFlags(struct ObjectRuntime *object, s32 flags);
void Event_StartScreenTransition(void);
void Event_WaitTransitionFrames(void);

#endif
