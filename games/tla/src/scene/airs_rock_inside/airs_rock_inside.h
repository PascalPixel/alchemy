#ifndef TLA_AIRS_ROCK_INSIDE_H
#define TLA_AIRS_ROCK_INSIDE_H

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "adventure.h"
#include "call.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct AirsRockInsideMapLayer {
  u8 unk00[0x30];
  u8 *cells;
  u8 unk34[4];
};

struct AirsRockInsideMapWork {
  u8 unk000[0x108];
  struct AirsRockInsideMapLayer layers[3];
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
extern char SceneId_Ov664Room10;
extern u8 gMapCellBuffer[];
extern u32 gFrameCount;

struct FieldActor *Import_ActorCreate(s32 type, s32 x, s32 y, s32 z);
void Scene_SetActorSpritePriority(struct FieldActor *object, s32 priority);
void Actor_SetSingleChildFlags(struct FieldActor *object, s32 value);
void Actor_SetPartAttribute(struct FieldActor *object, s32 value);
void Animation_SetStateFlags(struct FieldActor *object, s32 value);
struct FieldActor *Actor_GetById(s32 id);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void FieldMessage_Show(const u8 *message, s32 mode);
void Func_02005d5c(struct FieldActor *object, s32 value);
void Func_02005d6c(struct FieldActor *object, const u8 *data);
void Func_02005d9c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005dac(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Func_02005e14(void);
void Func_02005e1c(void);
void Func_02005f2c(s32 value);
void Func_02005f34(s32 a, s32 b);
void Func_02005f94(s32 value);

// The cell stampers keep their last cell and the block they copy.
extern s32 gData_02006c0c;
extern s32 gData_020073dc;
extern s32 gData_020073e0;
extern s32 gData_020073e4;
extern s32 gData_020073e8;

s32 Func_02005f84(void);
void Func_02005db4(s32 srcX, s32 src_z, s32 width, s32 height, s32 destX,
                   s32 destZ);

static inline void CopyCells(s32 srcX, s32 srcZ, s32 width, s32 height,
                             s32 destX, s32 destZ) {
  /* FAKEMATCH: forwarding inline puts every argument in a register before the
   * stack arguments are stored; called directly, 24 instructions differ in
   * Func_02002980 in tla-en. */
  Func_02005db4(srcX, srcZ, width, height, destX, destZ);
}

#endif
