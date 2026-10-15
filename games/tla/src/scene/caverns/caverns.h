#ifndef TLA_CAVERNS_H
#define TLA_CAVERNS_H

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "party_state.h"
#include "call.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define HALF_AT(base, offset) (*(s16 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct CavernsMapLayer {
    u8 unknown_00[0x30];
    u8 *cells;
    u8 unknown_34[4];
};

struct CavernsMapWork {
    u8 unknown_000[0x108];
    struct CavernsMapLayer layers[3];
};

extern char SceneId_Ov6a9Room1;
extern char SceneId_Ov6a9Room2;
extern char SceneId_Ov6a9Room3;
extern char SceneId_Ov6a9Room4;
extern u8 gMapCellBuffer[];
extern const u8 Data_02001220[];
extern const u8 Data_02001264[];
extern const u8 Data_02001328[];
extern const u8 Data_02001358[];
extern const u8 Data_020013b8[];
extern const u8 Data_02001430[];
extern const u8 Data_0200149c[];
extern const u8 Data_020014d8[];
extern const u8 Data_020014fc[];

struct FieldActor *Engine_ObjectCreate(s32 type, s32 x, s32 y, s32 z);
void Object_SetSpritePriority(struct FieldActor *object, s32 priority);
void ObjectDispatch_SetSingleChildFlags(struct FieldActor *object, s32 value);
void Object_SetPartAttribute(struct FieldActor *object, s32 value);
struct FieldActor *Object_GetById(s32 id);
void Battle_WaitMode0(s32 frames);
void WaitFrames(s32 frames);
void ObjectMotion_ArmCallback(s32 id, s32 angle, s32 flags);
void ObjectMotion_OffsetPositionAndResetMotion(s32 id, s32 dx, s32 dz);
void ObjectMotion_CommitCurrentPositionAndActivate(s32 id);
void ObjectMotion_SetPositionAndCommit(s32 id, s32 x, s32 z);
void Func_02000454(const u8 *data);
void Func_02000d10(void);
void Func_02000ea0(void);
void Func_02001074(struct FieldActor *object, s32 value);
void Func_02001094(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_020010ac(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
s32 Func_020010b4(struct FieldActor *object, s16 *position);
void Func_020010dc(void);
void Func_020010e4(void);
void Func_0200110c(s32 id, s32 x, s32 z);
void Func_02001134(s32 value);
void Func_0200113c(s32 value);
void Func_0200114c(void);
void Func_02001154(s32 a, s32 b, s32 c, s32 d);
void Func_02001164(s32 cue);
void Func_02000ba8(void);
void Func_02000f50(void);
void Func_02000f98(void);
s32 Func_02000c38(struct FieldActor *object, s16 *position);

#endif
