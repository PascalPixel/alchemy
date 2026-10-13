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
/* FAKEMATCH: Ram_WorkSlot stays gWorkSlot's linked address as a constant (build-checked): read through gWorkSlot, canvas.c, palette.c and twopic.c differ in tbs-ja and tbs-en. */
#define Ram_WorkSlot FIXED_ADDRESS((void **), 0x03001e60, gWorkSlot)
#else
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ad0, gBgScroll)
#define Ram_VramBlockCache FIXED_ADDRESS((u8 *), 0x03001b10, gVramBlockCache)
/* FAKEMATCH: Ram_WorkSlot stays gWorkSlot's linked address as a constant (build-checked): read through gWorkSlot, canvas.c, palette.c and twopic.c differ in tbs-ja and tbs-en. */
#define Ram_WorkSlot FIXED_ADDRESS((void **), 0x03001e50, gWorkSlot)
#endif

// ワークのスロット表 gWorkSlot の中の作業ポインタ
#define Ram_MapWork ((void **)&gWorkSlot[8])
#define Ram_CameraWork ((void **)&gWorkSlot[12])
#define Ram_EventWork ((void **)&gWorkSlot[27])
#define Ram_DisplayWork ((void **)&gWorkSlot[31])
#define Ram_ActorEffectWork ((void **)&gWorkSlot[36])
#define Ram_Disp ((void **)&gWorkSlot[42])
#define Ram_EffectWork ((void **)&gWorkSlot[56])

#define Ram_SceneMapStateOffset FIXED_OFFSET(0xc, gFieldBlendWork, gEventWork)

#endif
