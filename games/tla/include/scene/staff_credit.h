#ifndef ALCHEMY_SCENE_STAFF_CREDIT_H
#define ALCHEMY_SCENE_STAFF_CREDIT_H

/// @file
/// What the world map and the staff credits share: the picture layer they
/// draw over the map, and the placed actors' routines both overlays link.

#include "types.h"
#include "system/io_reg.h"
#include "system/io_write_queue.h"

struct Actor;

/// Queues a halfword register write for the next VBlank: the picture layer's
/// own call of IoWriteQueue_Store16, an inline over it as event.h's are.
static inline void Scene_QueueIoWrite16(u32 address, u32 value) {
  IoWriteQueue_Store16(address, value);
}

/// The picture layer drawn over the map on background 0: its tiles start at
/// tile 128 of character block 0, its map is screen block 4, and it takes
/// background palette 14.
#define PICTURE_FIRST_TILE 128
#define PICTURE_TILES (VRAM + PICTURE_FIRST_TILE * TILE_SIZE_4BPP)
#define PICTURE_MAP (VRAM + 4 * BG_SCREEN_SIZE)
#define PICTURE_PALETTE 14
/// One frame of a picture's map: a screen's 20 rows of 32 tiles.
#define PICTURE_FRAME_SIZE (BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT * 2)
/// The lighthouse pictures' frames in their buffer: tiles first, then a frame
/// every PICTURE_FRAME_STRIDE bytes from PICTURE_FIRST_FRAME.
#define PICTURE_FIRST_FRAME 4224
#define PICTURE_FRAME_STRIDE 1536
/// Queues frame `frame` of a lighthouse picture, from the caller's buffer, to
/// the picture layer's map at the next VBlank
/// (WorldMap_PlayLighthouseTransition and the credits' unused copy of it).
#define PUSH_PICTURE(frame)                                                    \
  IO_WRITE_QUEUE_PUSH(                                                         \
      PICTURE_MAP,                                                             \
      (u32)(buffer + PICTURE_FIRST_FRAME + (frame) * PICTURE_FRAME_STRIDE),    \
      DMA_COPY_WORDS(PICTURE_FRAME_SIZE / 4))

/// The PP gauge shown over the leader on the world map: a 32x16 sprite of
/// eight tiles.
#define GAUGE_SIZE (8 * TILE_SIZE_4BPP)
/// The gauge's levels of PP, and what it shows: empty, its level, or its
/// level flashing when PP is low.
#define GAUGE_LEVELS 30
/// What the gauge shows, kept as kind plus level: nothing, the level, or the
/// level blinking low.
enum { GAUGE_EMPTY = 0, GAUGE_SHOWN = 100, GAUGE_LOW = 200 };

extern u32 gStaffCredit_GlowColorModes[];
void StaffCredit_SpawnGrowingPair(struct Actor *actor);

void WorldMap_UpdatePlacedActors(void);

void WorldMap_StartPlacedActors(void);

void WorldMap_SpawnSparkPair(struct Actor *actor);
void WorldMap_SpawnParticle(struct Actor *actor);

#endif
