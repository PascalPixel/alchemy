#ifndef ALCHEMY_PERSPECTIVE_H
#define ALCHEMY_PERSPECTIVE_H

#include "types.h"
#include "layout_guard.h"

struct PerspectiveWork
{
	s32	 *origin;
	s32	  shake_x, shake_y, shake_decay;
	u8	 *script;
	u16	  fade;
	u8	  fade_step;
	u8	  unknown_017[0xe4 - 0x17];
	s32	  view_x;
	s32	  view_y;
	s32	  scale_x;
	s32	  scale_y;
	s32	  limit_x;
	s32	  limit_y;
	s8	  animation_active;
	u8	  unknown_0fd[3];
	u16	  window_top;
	u16	  window_bottom;
	u16	  shown_top;
	u16	  shown_bottom;
	u16	  window_pending;
	u8	  unknown_10a[6];
	void *tiles;
	u8	  unknown_114[4];
	u16	  pitch;
	u16	  yaw;
	u32	 *resources;
	u8	  unknown_120[0x18];
	u16	  tile_ids[256];
	u16	  quarter_tiles[8];
	s32	  far_plane;
	s32	  distance;
	s32	  previous_zoom;
	s32	  zoom;
	u16	  zoom_transition_frames;
	s16	  zoom_transition_step;
};

LAYOUT_SIZE_GUARD(PerspectiveWork_Size, struct PerspectiveWork, 0x35c);
LAYOUT_OFFSET_GUARD(PerspectiveWork_View, struct PerspectiveWork, view_x, 0xe4);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Pitch, struct PerspectiveWork, pitch, 0x118);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Active, struct PerspectiveWork, animation_active, 0xfc);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Resources, struct PerspectiveWork, resources, 0x11c);
LAYOUT_OFFSET_GUARD(PerspectiveWork_TileIds, struct PerspectiveWork, tile_ids, 0x138);
LAYOUT_OFFSET_GUARD(PerspectiveWork_QuarterTiles, struct PerspectiveWork, quarter_tiles, 0x338);
LAYOUT_OFFSET_GUARD(PerspectiveWork_FarPlane, struct PerspectiveWork, far_plane, 0x348);
LAYOUT_OFFSET_GUARD(PerspectiveWork_Zoom, struct PerspectiveWork, zoom, 0x354);
LAYOUT_OFFSET_GUARD(PerspectiveWork_PreviousZoom, struct PerspectiveWork, previous_zoom, 0x350);
LAYOUT_OFFSET_GUARD(
	PerspectiveWork_TransitionFrames, struct PerspectiveWork, zoom_transition_frames, 0x358);
LAYOUT_OFFSET_GUARD(
	PerspectiveWork_TransitionStep, struct PerspectiveWork, zoom_transition_step, 0x35a);

#endif
