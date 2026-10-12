#ifndef TLA_OVERLAY_664_OV664_H
#define TLA_OVERLAY_664_OV664_H

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "party_state.h"
#include "call.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct Ov664MapLayer {
    u8 unknown_00[0x30];
    u8 *cells;
    u8 unknown_34[4];
};

struct Ov664MapWork {
    u8 unknown_000[0x108];
    struct Ov664MapLayer layers[3];
};

extern char SceneId_Ov664Room1;
extern char SceneId_Ov664Room2;
extern char SceneId_Ov664Room3;
extern char SceneId_Ov664Room4;
extern char SceneId_Ov664Room5;
extern char SceneId_Ov664Room6;
extern char SceneId_Ov664Room7;
extern char SceneId_Ov664Room8;
extern char SceneId_Ov664Room9;
extern u8 gMapCellBuffer[];
extern u32 gFrameCount;

struct FieldActor *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 value);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
void Animation_SetStateFlags(struct FieldActor *object, s32 value);
struct FieldActor *Object_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void Battle_WaitMode0(s32 frames);
void UiText_ShowPositionedMessageAndWait(const u8 *message, s32 mode);
void Func_02005d5c(struct FieldActor *object, s32 value);
void Func_02005d6c(struct FieldActor *object, const u8 *data);
void Func_02005d9c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005dac(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 value);
void Func_02005f34(s32 a, s32 b);
void Func_02005f94(s32 value);

#endif
