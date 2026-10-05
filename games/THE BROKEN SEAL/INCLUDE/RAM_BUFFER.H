#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "TYPES.H"

/* Fixed RAM buffers addressed as constants. */
#define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */
#define Ram_DecodeBuffer ((u8 *)0x0201c000) /* gDecodeBuffer */
#define Ram_MapBlocks ((u8 *)0x02020000) /* gMapBlocks */
#define Ram_MapCollision ((u8 *)0x0202c000) /* gMapCollision */
#define Ram_BgTileBuffer ((u8 *)0x02038000) /* gBgTileBuffer */

/* Actor sprite slots, 16 KB apiece. */
#define Ram_ActorSpriteSlots ((u8 *)0x02018000) /* gActorSpriteSlots */

/* Sound setup copies the Thumb mixer from ROM to this even IWRAM address. */
#define Ram_SoundMixer ((u8 *)0x03007000) /* SoundDriver_Mixer */

/* The IWRAM heap ends at this word. */
#define Ram_IwramHeapEnd ((u8 *)0x03007800) /* gCartridgeResetMarker */

/* WaitFrames preserves the caller's stack below this boundary, then moves here. */
#define Ram_FrameWaitStackTop ((u8 *)0x03007a00) /* gFrameWaitStackTop */

/* Resident work cells beside the heap slots; the German edition places them
 * 16 bytes later. */
#if defined(TBS_EDITION_DE)
#define Ram_BgScroll ((u8 *)0x03001ae0) /* gBgScroll */
#define Ram_VramBlockCache ((u8 *)0x03001b20) /* gVramBlockCache */
#define Ram_MapWork ((void **)0x03001e80) /* gMapWork */
#define Ram_CameraWork ((void **)0x03001e90) /* gCameraWork */
#define Ram_EventWork ((void **)0x03001ecc) /* gEventWork */
#define Ram_DisplayWork ((void **)0x03001edc) /* gDisplayTransition */
#define Ram_WorkSlot ((void **)0x03001e60) /* gWorkSlot */
#define Ram_ActorEffectWork ((void **)0x03001ef0) /* gActorEffectWork */
#define Ram_Disp ((void **)0x03001f08) /* gDisp */
#define Ram_EffectWork ((void **)0x03001f40) /* gEffectWork */
#else
#define Ram_BgScroll ((u8 *)0x03001ad0) /* gBgScroll */
#define Ram_VramBlockCache ((u8 *)0x03001b10) /* gVramBlockCache */
#define Ram_MapWork ((void **)0x03001e70) /* gMapWork */
#define Ram_CameraWork ((void **)0x03001e80) /* gCameraWork */
#define Ram_EventWork ((void **)0x03001ebc) /* gEventWork */
#define Ram_DisplayWork ((void **)0x03001ecc) /* gDisplayTransition */
#define Ram_WorkSlot ((void **)0x03001e50) /* gWorkSlot */
#define Ram_ActorEffectWork ((void **)0x03001ee0) /* gActorEffectWork */
#define Ram_Disp ((void **)0x03001ef8) /* gDisp */
#define Ram_EffectWork ((void **)0x03001f30) /* gEffectWork */
#endif

/* Distance from the event work pointer to the adjacent scene map state pointer. */
#define Ram_SceneMapStateOffset 0xc /* Data_03001ec8 - gEventWork */

#endif
