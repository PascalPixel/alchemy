#ifndef TLA_JUPITER_AERIE_H
#define TLA_JUPITER_AERIE_H

#include "types.h"
#include "ram_buffer.h"
#include "party_state.h"
#include "object_runtime.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct JupiterAerieSpark {
    s32 word[10];
};

struct JupiterAerieWork {
    u8 unk00[2];
    s16 flag;
    u8 unk04[2];
    s16 x;
    s16 z;
    s16 count;
    s16 current;
    u8 unk0e[18];
};

struct JupiterAerieEntry {
    s16 x;
    s16 z;
    s16 flag;
    u8 unk06[2];
};

extern u8 gDecodeFillByte;
extern s32 gFrameCount;
extern s32 Data_02003314;
extern s32 Data_02003318;
extern u16 Data_02003340[];
extern u8 Data_02003488[];

void Animation_ApplyChildValues(struct ObjectRuntime *object, s32 value);
struct ObjectRuntime *Object_GetById(s32 id);
void ObjectMotion_SetSpeedParameters(s32 actor, s32 speed, s32 acceleration);
void ObjectMotion_SetPositionAndCommit(s32 actor, s32 x, s32 z);
void WaitFrames(s32 frames);
void Battle_WaitMode0(s32 frames);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
s32 GameFlag_GetByte(s32 flag);
void GameFlag_SetByte(s32 flag, s32 value);
u32 Random16Far(void);
void Func_02000858(s32 mode);
void Func_02000e80(void);
void Func_02001010(struct ObjectRuntime *object);
void Func_02001e78(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d, struct JupiterAerieSpark *spark);
void Func_020023b0(void);
void Func_02002438(struct JupiterAerieWork *work, struct JupiterAerieEntry *entry);
void Func_020024a8(struct JupiterAerieWork *work, struct JupiterAerieEntry *entry);
void Func_02002640(void);
void Func_02002a18(void);
void Func_02002a40(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
struct ObjectRuntime *Func_02002af8(void);
void Func_02002b28(s32 id, s32 a);
void Func_02002b68(s32 cue);

#endif
