#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "TYPES.H"

/* Fixed RAM buffers that Camelot's code addressed as constants. GCC folds a
 * constant base with its offsets where it cannot fold a label: it loads each
 * offset address from the pool, where from a label it adds the offsets to one
 * loaded base. Code the ROM shows doing the former reaches its buffer through
 * the entry here; the build checks each entry against the object named beside
 * it. Everywhere else the label serves. */
#define Ram_MapCellBuffer ((u8 *)0x02010000) /* gMapCellBuffer */
#define Ram_DecodeBuffer ((u8 *)0x0201c000) /* gDecodeBuffer */
#define Ram_MapBlocks ((u8 *)0x02020000) /* gMapBlocks */
#define Ram_MapCollision ((u8 *)0x0202c000) /* gMapCollision */
#define Ram_BgTileBuffer ((u8 *)0x02038000) /* gBgTileBuffer */

/* The actor sprite slots, 16 KB apiece: the slot base is a constant the
 * slot index is added to, which a label turns into a different register. */
#define Ram_ActorSpriteSlots ((u8 *)0x02018000) /* gActorSpriteSlots */

/* The IWRAM copy of the sound mixer, which the sound set-up fills from ROM.
 * Its label is a Thumb routine, so only a constant gives the even address
 * the copy writes to without an extra instruction. */
#define Ram_SoundMixer ((u8 *)0x03007000) /* SoundDriver_Mixer */

/* The IWRAM heap ends where this word lies: the allocators compare against
 * its address, which GCC turns into a test against the byte before it. */
#define Ram_IwramHeapEnd ((u8 *)0x03007800) /* Data_03007800 */

/* IWRAM cells beside the heap's slot words, which hold the resident work
 * blocks. GCC reaches a second constant cell by adding to the first one's
 * address. The German edition's IWRAM sits 16 bytes later. */
#if defined(TBS_EDITION_DE)
#define Ram_BgScroll ((u8 *)0x03001ae0) /* gBgScroll */
#define Ram_VramBlockCache ((u8 *)0x03001b20) /* gVramBlockCache */
#define Ram_MapWork ((void **)0x03001e80) /* gMapWork */
#define Ram_CameraWork ((void **)0x03001e90) /* gCameraWork */
#define Ram_EventWork ((void **)0x03001ecc) /* gEventWork */
#define Ram_DisplayWork ((void **)0x03001edc) /* Data_03001ecc */
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
#define Ram_DisplayWork ((void **)0x03001ecc) /* Data_03001ecc */
#define Ram_WorkSlot ((void **)0x03001e50) /* gWorkSlot */
#define Ram_ActorEffectWork ((void **)0x03001ee0) /* gActorEffectWork */
#define Ram_Disp ((void **)0x03001ef8) /* gDisp */
#define Ram_EffectWork ((void **)0x03001f30) /* gEffectWork */
#endif

/* The scene map state pointer sits beside the event work pointer: GCC reads it
 * from the event-work base it already holds, at this checked distance. */
#define Ram_SceneMapStateOffset 0xc /* Data_03001ec8 - gEventWork */

#endif
