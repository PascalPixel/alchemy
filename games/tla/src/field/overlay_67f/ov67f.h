#ifndef TLA_OVERLAY_67F_H
#define TLA_OVERLAY_67F_H

#include "types.h"
#include "object_runtime.h"
#include "party_state.h"
#include "ram_buffer.h"
#include "input.h"
#include "call.h"
#include "text/msg_ids.h"

struct Ov67fEventWork {
    u8 unknown_000[0x154];
    s16 exit;
    u8 unknown_156[0x56];
    s32 field_1ac;
    u8 unknown_1b0[0xb0c];
    struct ObjectRuntime *object_cbc;
};

struct Ov67fAnimation {
    u8 unknown_00[9];
    u8 unused_09 : 2;
    u8 layer : 2;
    u8 unknown_0a[8];
    u16 angle;
};

struct Ov67fMapLayer {
    u8 unknown_00[0x30];
    u8 *cells;
    u8 unknown_34[4];
};

struct Ov67fMapWork {
    u8 unknown_000[0x108];
    struct Ov67fMapLayer layers[3];
};

struct Ov67fPlacement {
    u16 id;
    u16 unknown_02;
    s16 flag;
};

struct Ov67fEffectWork {
    u8 unknown_00[26];
    s16 object_id;
};

extern u32 gFrameCount;
extern u8 gMapCellBuffer[];
extern char SceneId_Ov67FRoom1;
extern char SceneId_Ov67FRoom2;

void WaitFrames(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);

void Func_020000a4(const void *data, s32 a, s32 b);
void Func_02000bc0(s32 a, s32 b, s32 owner);
s32 Func_02000eac(u32 layer, s32 x, s32 y);
s32 Func_02000ee8(u32 layer, s32 x, s32 y, s32 value);
void Func_02000f30(struct ObjectRuntime *object);
void Func_02001428(struct ObjectRuntime *object, s32 value);
void Func_02001450(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001460(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001488(void);
void Func_02001490(void);
void Func_020014c0(s32 id);
void Func_020014e8(s32 value);
void Func_02001500(const void *data);
void Func_02001518(const void *data);
void Func_02001520(void);

extern u8 Data_02001654[];
extern u8 Data_02001658[];
extern u8 Data_02001674[];
extern u8 Data_02001688[];
extern u8 Data_02001696[];
extern u8 Data_020016d0[];
extern u8 Data_020016f0[];
extern u8 Data_02001754[];
extern u8 Data_0200176c[];
extern u8 Data_02001814[];
extern u8 Data_020018ec[];
extern u8 Data_020019ac[];

void Func_0200100c(u16 *list);
void Func_02000974(s32 mode);
void Func_02000aa8(void);
void Func_02001448(void);
void Func_02001470(s32 a, s32 b, s32 c);
s32 Func_02001508(struct ObjectRuntime *object);
void Func_02001510(const void *data);
void Func_020014e0(s32 value);
void Func_020014f8(const void *data);
void Func_02001538(s32 cue);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Object_AttachWorkTargetToObject(s32 id, s32 flag);

#endif
