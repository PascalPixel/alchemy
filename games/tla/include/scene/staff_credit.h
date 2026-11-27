#ifndef ALCHEMY_SCENE_STAFF_CREDIT_H
#define ALCHEMY_SCENE_STAFF_CREDIT_H

// What the world map and the staff credits share: the picture layer they draw over the map, and the
// placed actors' functions both overlays link.

#include "types.h"
#include "system.h"

struct ACTOR;

// Queues a halfword register write for the next VBlank: the picture layer's own call of
// syIOWriteQueueStore16, an inline over it as script.h's are.
static inline void scQueueIOWrite16(Uint32 addr, Uint32 value)
{
    syIOWriteQueueStore16(addr, value);
}

// The picture layer drawn over the map on background 0: its tiles start at tile 128 of character
// block 0, its map is screen block 4, and it takes background palette 14.
#define PICTURE_FIRST_TILE 128
#define PICTURE_TILES      (VRAM + PICTURE_FIRST_TILE * TILE_SIZE_4BPP)
#define PICTURE_MAP        (VRAM + 4 * BG_SCREEN_SIZE)
#define PICTURE_PALETTE    14
// One frame of a picture's map: a screen's 20 rows of 32 tiles.
#define PICTURE_MAP_BYTES  (BG_SCREEN_WIDTH * DISPLAY_TILE_HEIGHT * 2)
// The lighthouse pictures' frames in their buffer: tiles first, then a frame every PICTURE_STRIDE
// bytes from PICTURE_FIRST.
#define PICTURE_FIRST      4224
#define PICTURE_STRIDE     1536
// Queues frame `frame` of a lighthouse picture, from the caller's buffer, to the picture layer's
// map at the next VBlank (scWorldMapPlayLighthouseTransition and the credits' unused copy of it).
#define PUSH_PICTURE(frame)                                                                        \
    IO_WRITE_QUEUE_PUSH(PICTURE_MAP,                                                               \
                        (Uint32)(buf + PICTURE_FIRST + (frame) * PICTURE_STRIDE),                  \
                        DMA_COPY_WORDS(PICTURE_MAP_BYTES / 4))

// The PP gauge shown over the leader on the world map: a 32x16 sprite of eight tiles.
#define GAUGE_SIZE   (8 * TILE_SIZE_4BPP)
// The levels of PP the gauge can show.
#define GAUGE_LEVELS 30

// What the gauge shows, kept as kind plus level: nothing, the level, or the level blinking low.
enum
{
    GAUGE_EMPTY = 0,
    GAUGE_SHOWN = 100,
    GAUGE_LOW   = 200
};

extern Uint32 StaffCreditGlowColorModes[];
void          scStaffCreditSpawnGrowingPair(struct ACTOR *actor);

void          scWorldMapUpdatePlacedActors(void);

void          scWorldMapStartPlacedActors(void);

void          scWorldMapSpawnSparkPair(struct ACTOR *actor);
void          scWorldMapSpawnParticle(struct ACTOR *actor);

#endif
