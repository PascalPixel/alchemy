#ifndef ALCHEMY_BATTLE_SUMMON_H
#define ALCHEMY_BATTLE_SUMMON_H

#include "layout_guard.h"

#include "types.h"
#include "edition.h"
#if !GAME_TLA
#include "farcall.h"
#endif

struct BattleSpriteDefinition
{
	u16 animation;
	u8	sprite_bit0 : 1;
	u8	sprite_bits1 : 4;
	u8	sprite_bits5 : 3;
	u8	layout_bit0 : 1;
	u8	layout_bits1 : 4;
	u8	layout_kind : 3;
	u8	unknown_04[4];
};

LAYOUT_SIZE_GUARD(BattleSpriteDefinition_Size, struct BattleSpriteDefinition, 8);
extern struct BattleSpriteDefinition Summon_EntryTable[];

struct SummonDefinition
{
	u16 name_message_id;
	u8	unknown_02[2];
	u8	djinn_required[4];
};

struct BattleSummonState
{
	u32 available_mask;
};

#if GAME_TLA
const struct SummonDefinition *SummonDefinition_Get(s32 summon_id);
#endif
s32							   EnmSlotGet(void);
s32							   EnmChk(s32 class_id);

const struct SummonDefinition *SummonDefinition_GetNear(u32 summon_id);
s32							   BtlGrpRnd(s32 record_id);
s32							   EnmCntDec(s32 class_id, s32 n);
s32							   EnmCntRel(s32 actor_id);
s32							   EnmCntClr(s32 class_id);
s32							   EnmUpd(void);

#endif
