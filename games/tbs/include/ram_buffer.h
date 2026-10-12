#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

#include "types.h"

#define Ram_MapCellBuffer FIXED_ADDRESS((u8 *), 0x02010000, gMapCellBuffer)
#define Ram_DecodeBuffer FIXED_ADDRESS((u8 *), 0x0201c000, gDecodeBuffer)
#define Ram_MapBlocks FIXED_ADDRESS((u8 *), 0x02020000, gMapBlocks)
#define Ram_MapCollision FIXED_ADDRESS((u8 *), 0x0202c000, gMapCollision)
#define Ram_BgTileBuffer FIXED_ADDRESS((u8 *), 0x02038000, gBgTileBuffer)

#define Ram_ActorSpriteSlots FIXED_ADDRESS((u8 *), 0x02018000, gActorSpriteSlots)

#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03007000, SoundDriver_Mixer)

#define Ram_IwramHeapEnd FIXED_ADDRESS((u8 *), 0x03007800, gCartridgeResetMarker)

#define Ram_FrameWaitStackTop FIXED_ADDRESS((u8 *), 0x03007a00, gFrameWaitStackTop)

#if defined(TBS_EDITION_DE)
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ae0, gBgScroll)
#define Ram_VramBlockCache FIXED_ADDRESS((u8 *), 0x03001b20, gVramBlockCache)
#define Ram_MapWork FIXED_ADDRESS((void **), 0x03001e80, gMapWork)
#define Ram_CameraWork FIXED_ADDRESS((void **), 0x03001e90, gCameraWork)
#define Ram_EventWork FIXED_ADDRESS((void **), 0x03001ecc, gEventWork)
#define Ram_DisplayWork FIXED_ADDRESS((void **), 0x03001edc, gDisplayTransition)
#define Ram_WorkSlot FIXED_ADDRESS((void **), 0x03001e60, gWorkSlot)
#define Ram_ActorEffectWork FIXED_ADDRESS((void **), 0x03001ef0, gActorEffectWork)
#define Ram_Disp FIXED_ADDRESS((void **), 0x03001f08, gDisp)
#define Ram_EffectWork FIXED_ADDRESS((void **), 0x03001f40, gEffectWork)
#else
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ad0, gBgScroll)
#define Ram_VramBlockCache FIXED_ADDRESS((u8 *), 0x03001b10, gVramBlockCache)
#define Ram_MapWork FIXED_ADDRESS((void **), 0x03001e70, gMapWork)
#define Ram_CameraWork FIXED_ADDRESS((void **), 0x03001e80, gCameraWork)
#define Ram_EventWork FIXED_ADDRESS((void **), 0x03001ebc, gEventWork)
#define Ram_DisplayWork FIXED_ADDRESS((void **), 0x03001ecc, gDisplayTransition)
#define Ram_WorkSlot FIXED_ADDRESS((void **), 0x03001e50, gWorkSlot)
#define Ram_ActorEffectWork FIXED_ADDRESS((void **), 0x03001ee0, gActorEffectWork)
#define Ram_Disp FIXED_ADDRESS((void **), 0x03001ef8, gDisp)
#define Ram_EffectWork FIXED_ADDRESS((void **), 0x03001f30, gEffectWork)
#endif

#define Ram_SceneMapStateOffset FIXED_OFFSET(0xc, gFieldBlendWork, gEventWork)

// RAM の名前：置き場はリンカが決め、上の固定アドレスと照らし合わせる
extern void *gEffectWork; // ワークのスロット 56：場面ごとに別の構造体を指す
extern volatile u16 gLinkStatus;

#endif
