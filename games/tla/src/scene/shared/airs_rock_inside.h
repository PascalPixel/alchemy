#ifndef TLA_AIRS_ROCK_INSIDE_H
#define TLA_AIRS_ROCK_INSIDE_H

#include "types.h"
#include "ram_buffer.h"
#include "field_event.h"
#include "iwram_call.h"
#include "main_routine_work.h"
#include "frame.h"

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

void Scene_SetActorSpritePriority(struct Actor *object, s32 priority);
void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);
void Actor_SetPartAttribute(struct Actor *object, s32 attribute);
void Animation_SetStateFlags(struct Actor *object, u32 value);
struct Actor *Actor_GetById(u32 objectId);
s32 GameFlag_Test(s32 flag);
void GameFlag_SetBit(s32 flag);
void GameFlag_ClearBit(s32 flag);
void FieldEvent_Wait(s32 frames);
void FieldMessage_Show(const u8 *message, s32 mode);
void Func_02005d5c(struct Actor *object, s32 value);
void Actor_StartScript(struct Actor *object, const u8 *data);
void Map_CopyCellPictureIndicesRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Map_CopyCellAttributeRect(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void Event_ResolvePendingActions(s32 value);
void BattleParty_ApplyDrain(s32 a, s32 b);
void Sound_Play(s32 value);

// The cell stampers keep their last cell and the block they copy.
extern s32 gData_02006c0c;
extern s32 gData_020073dc;
extern s32 gData_020073e0;
extern s32 gData_020073e4;
extern s32 gData_020073e8;

void Func_02005db4(s32 srcX, s32 src_z, s32 width, s32 height, s32 destX,
                   s32 destZ);

static inline void CopyCells(s32 srcX, s32 srcZ, s32 width, s32 height,
                             s32 destX, s32 destZ) {
  /* FAKEMATCH(wrapper): the call forwarded through an inline, which puts every
   * argument in a register before the stack arguments are stored; as direct
   * calls to Func_02005db4, 24 of 59 instructions of Func_02002980 differ in
   * tla-en (24 of 59 in Func_02002a14, 12 of 59 in Func_02003d40). */
  /* FAKEMATCH(wrapper): CopyCells forwarded inline; as direct calls to
   * Func_02005db4, 12 of 70 instructions of Func_020026c0 differ in tla-en. */
  Func_02005db4(srcX, srcZ, width, height, destX, destZ);
}

#endif
