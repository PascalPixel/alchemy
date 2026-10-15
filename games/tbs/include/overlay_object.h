#ifndef ALCHEMY_OVERLAY_OBJECT_H
#define ALCHEMY_OVERLAY_OBJECT_H

#include "types.h"

struct OverlayObjectRecord
{
	u8 unk00[9];
	u8 flags;
};

struct OverlayObject
{
	u8							unk00[6];
	u16							angle;
	s32							coordinate08;
	u8							unk0c[4];
	s32							coordinate10;
	u8							unk14[0x1c];
	s32							field30;
	u8							unk34[4];
	s32							coordinate38;
	u8							unk3c[4];
	s32							coordinate40;
	u8							unk44[0x0c];
	struct OverlayObjectRecord *record;
	u8							unk54;
	u8							unk55;
	u8							unk56[3];
	u8							unk59;
	u8							steeringFlags;
	u8							unk5b[9];
	u16							angle64;
	u8							unk66[2];
	struct OverlayObject	   *linkedObject;
};

#define OVERLAY_OBJECT_OFFSET(field) ((u32) & (((struct OverlayObject *)0)->field))
typedef char OverlayObjectRecord_flags_offset
	[((u32) & (((struct OverlayObjectRecord *)0)->flags)) == 0x09 ? 1 : -1];
typedef char OvObj_record_offset[OVERLAY_OBJECT_OFFSET(record) == 0x50 ? 1 : -1];
typedef char OvObj_angle_offset[OVERLAY_OBJECT_OFFSET(angle) == 0x06 ? 1 : -1];
typedef char OvObj_coordinate_08_offset[OVERLAY_OBJECT_OFFSET(coordinate08) == 0x08 ? 1 : -1];
typedef char OvObj_coordinate_10_offset[OVERLAY_OBJECT_OFFSET(coordinate10) == 0x10 ? 1 : -1];
typedef char OvObj_field_30_offset[OVERLAY_OBJECT_OFFSET(field30) == 0x30 ? 1 : -1];
typedef char OvObj_coordinate_38_offset[OVERLAY_OBJECT_OFFSET(coordinate38) == 0x38 ? 1 : -1];
typedef char OvObj_coordinate_40_offset[OVERLAY_OBJECT_OFFSET(coordinate40) == 0x40 ? 1 : -1];
typedef char OvObj_unknown_55_offset[OVERLAY_OBJECT_OFFSET(unk55) == 0x55 ? 1 : -1];
typedef char OvObj_unknown_59_offset[OVERLAY_OBJECT_OFFSET(unk59) == 0x59 ? 1 : -1];
typedef char OvObj_steering_flags_offset[OVERLAY_OBJECT_OFFSET(steeringFlags) == 0x5a ? 1 : -1];
typedef char OvObj_angle_64_offset[OVERLAY_OBJECT_OFFSET(angle64) == 0x64 ? 1 : -1];
typedef char OvObj_linked_object_offset[OVERLAY_OBJECT_OFFSET(linkedObject) == 0x68 ? 1 : -1];
#undef OVERLAY_OBJECT_OFFSET

#endif
