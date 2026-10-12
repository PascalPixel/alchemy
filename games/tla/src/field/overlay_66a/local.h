#ifndef OVERLAY_66A_LOCAL_H
#define OVERLAY_66A_LOCAL_H

#include "types.h"
#include "ram_buffer.h"
#include "object_runtime.h"
#include "party_state.h"

extern char SceneId_Ov66aRoom1;
extern char SceneId_Ov66aRoom2;
extern u8 MsgOhStoppedBy;

struct Ov66aMapCell {
    u8 unknown_0[2];
    u8 state;
    u8 unknown_3;
};

struct Ov66aMapLayer {
    struct Ov66aMapCell *cells;
    u8 unknown_04[0x34];
};

struct Ov66aMapWork {
    u8 unknown_000[0x138];
    struct Ov66aMapLayer layers[1];
};

struct Ov66aEventWork {
    u8 unknown_000[0x1ac];
    s32 value_1ac;
    u8 unknown_1b0[0xb9a];
    u16 value_d4a;
};

struct Ov66aEffectWork {
    u8 unknown_00[0x1a];
    s16 object_id;
};

struct Ov66aSwitch {
    u16 id;
    s16 kind;
    s16 flag;
};

s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectDispatch_SetSingleChildFlags(struct ObjectRuntime *object, s32 flags);
void ObjectGroup_ApplyRandomChildValues(void);
void UiText_OpenMessageAtObject(s32 id, s32 flags);
s32 Inventory_PromptAndSetObjectMode(s32 a, s32 b);
void Func_020013f0(struct ObjectRuntime *object, s32 mode);
void Func_02001400(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001408(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02001420(s32 x, s32 z, s32 a, s32 b);
void Func_020014b8(s32 message);
void Func_020014d0(s32 id, s32 flags);
void Func_02001580(const void *data);
s32 Func_02001588(void);
s32 Func_02001590(void);
s32 Func_020015d0(s32 x, s32 z, s32 a, s32 b);
void Func_020015d8(void);
void Func_020015f0(const void *data, s32 x, s32 y, s32 z);

void Func_020002f8(void);
void Func_0200051c(void);
void Func_0200129c(const u16 *entry, s32 mode, s32 frame);
void Func_02001320(s32 id, u32 kind, s32 flag, s32 layer);

extern s16 Data_02001608[];
extern const u16 Data_020016e4[];
extern u8 Data_02001778[];
extern u8 Data_02001868[];
extern u8 Data_02001a18[];
extern u8 Data_02001a90[];
extern u8 Data_02001be0[];

static __inline__ void Func_02001420_(s32 x, s32 z, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_02001420(x, z, a, b);
}

static __inline__ void Func_020015d0_(s32 x, s32 z, s32 a, s32 b)
{
    /* FAKEMATCH: forwarding through the inline keeps each call's own constants, as ☀️'s scene layer does. */
    Func_020015d0(x, z, a, b);
}

static __inline__ void Func_02001400_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters give each stack argument its own register; a direct call builds both in r3. */
    Func_02001400(a, b, c, d, e, f);
}

static __inline__ void Func_02001408_(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f)
{
    /* FAKEMATCH: as in ☀️'s scene layer, the inline parameters give each stack argument its own register; a direct call builds both in r3. */
    Func_02001408(a, b, c, d, e, f);
}

#endif
