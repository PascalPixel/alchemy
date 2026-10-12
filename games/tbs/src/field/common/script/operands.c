#include "unit/field/common/script/field.h"
#include "unit/field/common/script/script.h"
#include "unit/field/common/script/operands.h"
#include "unit/system/scheduler/heap.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "resource.h"
#include "mem.h"
#include "types.h"
#include "animspr.h"
#include "global_cells.h"
#include "dma.h"
#include "ramcode.h"
#include "iwram_call.h"
#include "io_reg.h"
#include "iowrite.h"
#include "heap_state.h"
#include "affine.h"
#include "callback_scheduler.h"
#include "scene.h"
#include "object_dispatch.h"
#include "metadata_lookup.h"
#include "edition.h"
#include "objdisp.h"
#include "object_runtime.h"
#include "btlsys.h"
#include "transform.h"
#include "script.h"
#include "script_motion.h"
#include "script_object_entry.h"
#include "script_object_runtime.h"
#include "fieldobj.h"
#include "fixed_math.h"
#include "keystate.h"
#include "fieldrun.h"
#include "event_runtime.h"
#include "game_state.h"
#include "map.h"
#include "map_scroll.h"
#include "ram_buffer.h"
#include "field_sprite.h"
#include "resource_ids.h"
#include "flag_ids.h"
#include "mapcopy.h"
#include "fixed_point_position.h"
#include "mapframe.h"
#include "btldisp.h"
#include "project.h"
#include "frame.h"
#include "palqueue.h"
#include "curve.h"
#include "btlpty.h"
#include "b5_context.h"
#include "vram_tab.h"
#include "vram_block.h"

enum ScriptOperandOperation
{
	SCRIPT_OPERAND_SET,
	SCRIPT_OPERAND_ADD,
	SCRIPT_OPERAND_COMPARE
};

typedef void (*OperandFunc)(struct ScriptOperands *, s32, s32);

extern OperandFunc Script_OperandHandlerTable[];

// 作業領域の 1 項目を命令の値で書き換える: 代入、加算、または比較 (結果は comparison_result)
// 32 ビットの項目
#define DEFINE_OPERAND_WORD(name, field)                              \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
			return;                                                   \
		}                                                             \
		if (operation == SCRIPT_OPERAND_ADD)                          \
		{                                                             \
			state->field += value;                                    \
			return;                                                   \
		}                                                             \
		state->comparison_result = state->field == value;             \
	}

// アドレスの項目: 加算は value の 4 倍 (4 バイト単位)
#define DEFINE_OPERAND_ADDRESS(name, field)                           \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
			return;                                                   \
		}                                                             \
		if (operation == SCRIPT_OPERAND_ADD)                          \
		{                                                             \
			state->field += value * 4;                                \
			return;                                                   \
		}                                                             \
		state->comparison_result = state->field == value;             \
	}

// 8 ビット、16 ビットの項目: 比較は項目の型に切り詰めた値と行う
#define DEFINE_OPERAND_NARROW(name, field, type)                      \
	void name(struct ScriptOperands *state, s32 operation, s32 value) \
	{                                                                 \
		if (operation == SCRIPT_OPERAND_SET)                          \
		{                                                             \
			state->field = value;                                     \
		}                                                             \
		else if (operation == SCRIPT_OPERAND_ADD)                     \
		{                                                             \
			state->field += value;                                    \
		}                                                             \
		else                                                          \
		{                                                             \
			state->comparison_result = state->field == (type)value;   \
		}                                                             \
	}

DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress, script_address)

void Script_SetOrCompareCursor(struct ScriptOperands *state, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		state->cursor = value;
	}
	else if (operation == SCRIPT_OPERAND_ADD)
	{
		state->cursor = ((s16)state->cursor + value);
	}
	else
	{
		state->comparison_result = (s16)state->cursor == (s16)value;
	}
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareUnsignedHalfword, unsigned_halfword, u16)
DEFINE_OPERAND_WORD(Script_SetOrCompareX, word_08)
DEFINE_OPERAND_WORD(Script_SetOrCompareY, word_0c)
DEFINE_OPERAND_WORD(Script_SetOrCompareZ, word_10)

void Script_SetOrCompareRadius(struct ScriptOperands *state, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		state->halfword_20 = value;
		return;
	}
	if (operation == SCRIPT_OPERAND_ADD)
	{
		state->halfword_20 = (state->halfword_20 + value);
		return;
	}
	state->comparison_result = state->halfword_20 == (s16)value;
}

DEFINE_OPERAND_WORD(Script_SetOrCompareScaleX, word_18)
DEFINE_OPERAND_WORD(Script_SetOrCompareScaleY, word_1c)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityX, word_24)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityY, word_28)
DEFINE_OPERAND_WORD(Script_SetOrCompareVelocityZ, word_2c)
DEFINE_OPERAND_WORD(Script_SetOrCompareSpeedLimit, word_30)
DEFINE_OPERAND_WORD(Script_SetOrCompareAcceleration, word_34)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetX, word_38)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetY, word_3c)
DEFINE_OPERAND_WORD(Script_SetOrCompareTargetZ, word_40)
DEFINE_OPERAND_WORD(Script_SetOrCompareVerticalMotion, word_44)
DEFINE_OPERAND_WORD(Script_SetOrCompareGravity, word_48)
DEFINE_OPERAND_WORD(Script_SetOrCompareTerrainHeight, word_14)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord4c, word_4c)
DEFINE_OPERAND_ADDRESS(Script_SetOrCompareAddress50, address_50)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte54, byte_54, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareMotionFlags, byte_55, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareArrivalAxis, byte_56, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareComparisonResult, comparison_result, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSnapToTarget, byte_58, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareCollisionFlags, byte_59, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSteeringFlags, byte_5a, u8)
DEFINE_OPERAND_NARROW(Script_SetOrComparePaused, byte_5b, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareRepeatCount, byte_5d, u8)

void Script_SetOrCompareHalfword5e(struct ScriptOperands *work, s32 operation, s32 value)
{
	if (operation == SCRIPT_OPERAND_SET)
	{
		work->halfword_5e = value;
		return;
	}
	if (operation == SCRIPT_OPERAND_ADD)
	{
		work->halfword_5e += value;
		return;
	}
	work->comparison_result = (s16)work->halfword_5e == (s16)value;
}

DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword64, signed_halfword_64, s16)
DEFINE_OPERAND_NARROW(Script_SetOrCompareSignedHalfword66, signed_halfword_66, s16)
DEFINE_OPERAND_WORD(Script_SetOrCompareWord68, word_68)
DEFINE_OPERAND_WORD(Script_SetOrCompareUpdateAddress, word_6c)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte62, byte_62, u8)
DEFINE_OPERAND_NARROW(Script_SetOrCompareByte63, byte_63, u8)

// 命令語の次の 2 語 (項目の番号と値) を読み、項目の関数に operation をわたす
static __inline__ s32 ApplyOperand(struct ScriptOperands *work, s32 operation)
{
	s16			index = work->cursor;
	const s32  *command = (const s32 *)work->script_address + index + 1;
	OperandFunc handler = Script_OperandHandlerTable[command[0]];

	if (handler != 0)
		handler(work, operation, command[1]);
	work->cursor += 3;
	return 1;
}

s32 Script_ApplyOperandSet(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_SET);
}

s32 Script_ApplyOperandAdd(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_ADD);
}

s32 Script_ApplyOperandCompare(struct ScriptOperands *work)
{
	return ApplyOperand(work, SCRIPT_OPERAND_COMPARE);
}
