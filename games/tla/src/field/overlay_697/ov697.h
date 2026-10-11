#ifndef TLA_OVERLAY_697_OV697_H
#define TLA_OVERLAY_697_OV697_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"
#include "field_scene.h"
#include "scene_ids.h"

#define OBJECT_BYTE(object, offset) (((u8 *)(object))[offset])

struct Ov697MapLayer {
    u8 unknown_00[0x30];
    u8 *cells;
    u8 unknown_34[4];
};

struct Ov697MapWork {
    u8 unknown_000[0x108];
    struct Ov697MapLayer layers[3];
};

struct Ov697State {
    u8 unknown_00[0x10];
    s16 pattern;
    u8 unknown_12[2];
    s16 field_14;
    u8 unknown_16[4];
    s16 field_1a;
    s16 field_1c;
};

extern u8 Data_020023c4[];

#define gOv697State ((struct Ov697State *)(Data_020023c4 + 0x88))

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void WaitFrames(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

void Func_0200062c(s32 id);
void Func_02000660(s32 id);
void Func_0200070c(s32 id, s32 value);
void Func_0200078c(s32 id);
void Func_020009ac(s32 a, s32 b);
void Func_02000a18(s32 id, s32 a, s32 b);
void Func_02000b7c(s32 id, s32 other, s32 flag, s32 side);
void Func_02000e40(s32 id, s32 dx, s32 dz, s32 axis);
void Func_02001104(s32 id, s32 a, s32 frame);
void Func_02004418(const void *data);
void Func_020045ac(const void *data, s32 a);
void Func_020057c8(struct ObjectRuntime *object, s32 a);
void Func_020057d0(struct ObjectRuntime *object, const void *script);
struct ObjectRuntime *Func_020057d8(s32 type, s32 x, s32 y, s32 z);
void Func_02005818(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005820(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005918(s32 id, s32 a, s32 b);
void Func_020059a0(s32 a, s32 b);
s32 Func_020059b8(s32 id);
void Func_020059c0(void);
void Func_020059c8(void);
s32 Func_02005a30(void);
void Func_02005a38(void);

#endif
