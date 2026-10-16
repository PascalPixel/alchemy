#ifndef ALCHEMY_OVERLAY_OBJECT_H
#define ALCHEMY_OVERLAY_OBJECT_H

#include "types.h"
#include "layout_guard.h"

struct OverlayObjectRecord {
  u8 unk00[9];
  u8 flags;
};

struct OverlayObject {
  u8 unk00[6];
  u16 angle;
  s32 coordinate08;
  u8 unk0c[4];
  s32 coordinate10;
  u8 unk14[0x1c];
  s32 field30;
  u8 unk34[4];
  s32 coordinate38;
  u8 unk3c[4];
  s32 coordinate40;
  u8 unk44[0x0c];
  struct OverlayObjectRecord *record;
  u8 unk54;
  u8 unk55;
  u8 unk56[3];
  u8 unk59;
  u8 steeringFlags;
  u8 unk5b[9];
  u16 angle64;
  u8 unk66[2];
  struct OverlayObject *linkedObject;
};

LAYOUT_OFFSET_GUARD(OverlayObjectRecord_flags_offset,
                    struct OverlayObjectRecord, flags, 0x09);
LAYOUT_OFFSET_GUARD(OvObj_record_offset, struct OverlayObject, record, 0x50);
LAYOUT_OFFSET_GUARD(OvObj_angle_offset, struct OverlayObject, angle, 0x06);
LAYOUT_OFFSET_GUARD(OvObj_coordinate_08_offset, struct OverlayObject,
                    coordinate08, 0x08);
LAYOUT_OFFSET_GUARD(OvObj_coordinate_10_offset, struct OverlayObject,
                    coordinate10, 0x10);
LAYOUT_OFFSET_GUARD(OvObj_field_30_offset, struct OverlayObject, field30, 0x30);
LAYOUT_OFFSET_GUARD(OvObj_coordinate_38_offset, struct OverlayObject,
                    coordinate38, 0x38);
LAYOUT_OFFSET_GUARD(OvObj_coordinate_40_offset, struct OverlayObject,
                    coordinate40, 0x40);
LAYOUT_OFFSET_GUARD(OvObj_unknown_55_offset, struct OverlayObject, unk55, 0x55);
LAYOUT_OFFSET_GUARD(OvObj_unknown_59_offset, struct OverlayObject, unk59, 0x59);
LAYOUT_OFFSET_GUARD(OvObj_steering_flags_offset, struct OverlayObject,
                    steeringFlags, 0x5a);
LAYOUT_OFFSET_GUARD(OvObj_angle_64_offset, struct OverlayObject, angle64, 0x64);
LAYOUT_OFFSET_GUARD(OvObj_linked_object_offset, struct OverlayObject,
                    linkedObject, 0x68);

#endif
