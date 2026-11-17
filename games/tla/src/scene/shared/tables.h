/// @file
/// The packed push step the scenes' push tables are written in.
#ifndef ALCHEMY_SCENE_SHARED_TABLES_H
#define ALCHEMY_SCENE_SHARED_TABLES_H

/// One step of the leader as he pushes, packed as one word: x pixels in the
/// upper halfword, z in the lower. Packed through u32, so a negative step is
/// its two's-complement bits.
#define PUSH_STEP(x, z) (((u32)(x) << 16) | ((u32)(z) & 0xffff))

#endif
