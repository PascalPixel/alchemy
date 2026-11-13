/// @file
/// The packed push step the scenes' push tables are written in.
#ifndef ALCHEMY_SCENE_SHARED_TABLES_H
#define ALCHEMY_SCENE_SHARED_TABLES_H

/// One step of the leader as he pushes: x pixels in the high half, z in the
/// low half, as the push routines read it.
#define PUSH_STEP(x, z) ((x) * 0x10000 + ((z) & 0xffff))

#endif
