#ifndef TLA_JUPITER_LIGHTHOUSE_AERIE_H
#define TLA_JUPITER_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "frame.h"
#include "cell_target.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct EffectOptions;

struct JupiterLighthouseAerieSpark {
  s32 word[10];
};

extern u8 gDecodeFillByte;
extern s32 gData_02003314;
extern s32 gData_02003318;
extern u16 gData_02003340[];
extern struct CellTarget gData_02003488;

void Animation_ApplyChildValues(struct Actor *object, s32 value);
void GameFlag_SetByte(s32 flag, s32 value);
u32 Random16Far(void);
void Func_02000858(s32 mode);
void Func_02000e80(void);
void Func_02001010(struct Actor *object);
void Func_02001e78(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);
void Func_020023b0(void);
void Func_02002438(struct CellTarget *work, struct CellTargetEntry *entry);
void Func_020024a8(struct CellTarget *work, struct CellTargetEntry *entry);
void Func_02002640(void);
void Func_02002a18(void);
void Func_02002b28(s32 id, s32 a);

#endif
