#ifndef ALCHEMY_JUPITER_LIGHTHOUSE_AERIE_H
#define ALCHEMY_JUPITER_LIGHTHOUSE_AERIE_H

#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "system/frame.h"
#include "field/cell_target.h"

#define BYTE_AT(base, offset) (*(u8 *)((u8 *)(base) + (offset)))
#define WORD_AT(base, offset) (*(s32 *)((u8 *)(base) + (offset)))

struct EffectOptions;

struct JupiterLighthouseAerieSpark {
  s32 word[10];
};

extern u8 gDecodeFillByte;

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);
void Import_HangingVinePlace(s32 id, s32 a);

#endif
