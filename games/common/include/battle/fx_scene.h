#ifndef ALCHEMY_FX_SCENE_H
#define ALCHEMY_FX_SCENE_H

#include "types.h"
#include "layout_guard.h"
#include "effect_slot.h"

struct AnimationObject;

struct ArcEffectObject
{
	u8						unknown_00[6];
	u16						angle;
	u8						unknown_08[0x48];
	struct AnimationObject *records;
	u8						unknown_54[0x10];
	s16						phase;
	s16						step;
	struct FieldActor	   *linked_object;
	void				   *callback;
};

struct BattleFxScene
{
	s32	  angle;
	s32	  x;
	s32	  y;
	s32	  z;
	void *main_object;
	void *child;
	s16	  first_object_id;
	s16	  second_object_id;
	u16	  action;
	s16	  animation;
	s8	  enabled;
	u8	  mode;
	s8	  visible;
	s8	  active;
	void (*finish_callback)(void);
	u8				  unknown_28[0x0c];
	s8				  child_option;
	s8				  child_mode;
	u8				  unknown_36[2];
	void			 *saved_callback;
	const s32		 *saved_script;
	u8				  unknown_40[4];
	u8				  saved_palette;
	u8				  lit;
	u16				  tile_slot;
	u16				  unknown_48;
	u16				  free_blocks;
	s32				  saved_x;
	s32				  saved_y;
	s32				  saved_z;
	struct EffectSlot slots[24];
	u8				  unknown_718[2];
	s16				  arc_tile_slot;
	s8				  message_mode;
	u8				  unknown_71d[3];
};

LAYOUT_SIZE_GUARD(BattleFxScene_Size, struct BattleFxScene, 0x720);
LAYOUT_OFFSET_GUARD(BattleFxScene_MainObject, struct BattleFxScene, main_object, 0x10);
LAYOUT_OFFSET_GUARD(BattleFxScene_Action, struct BattleFxScene, action, 0x1c);
LAYOUT_OFFSET_GUARD(BattleFxScene_SavedCallback, struct BattleFxScene, saved_callback, 0x38);
LAYOUT_OFFSET_GUARD(BattleFxScene_TileSlot, struct BattleFxScene, tile_slot, 0x46);
LAYOUT_OFFSET_GUARD(BattleFxScene_SavedX, struct BattleFxScene, saved_x, 0x4c);
LAYOUT_OFFSET_GUARD(BattleFxScene_Slots, struct BattleFxScene, slots, 0x58);
LAYOUT_OFFSET_GUARD(BattleFxScene_ArcTileSlot, struct BattleFxScene, arc_tile_slot, 0x71a);
LAYOUT_OFFSET_GUARD(BattleFxScene_MessageMode, struct BattleFxScene, message_mode, 0x71c);

static __inline__ s32 BattleFx_Interpolate(s32 to, s32 from, s32 step)
{
	return from + (to - from) * step / 10;
}

#endif
