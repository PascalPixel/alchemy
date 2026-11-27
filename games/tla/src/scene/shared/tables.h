/* The packed push step the scenes' push tables are written in, and how many
   kinds of thing they push. */
#ifndef ALCHEMY_SCENE_SHARED_TABLES_H
#define ALCHEMY_SCENE_SHARED_TABLES_H

/* One step of the leader as he pushes, packed as one word: x pixels in the
   upper halfword, z in the lower. Packed through Uint32, so a negative step is
   its two's-complement bits. */
#define PUSH_STEP(x, z)	  (((Uint32)(x) << 16) | ((Uint32)(z) & 0xffff))
/* A PUSH_STEP's x, in pixels. */
#define PUSH_STEP_X(step) ((Sint32)(step) >> 16)
/* A PUSH_STEP's z, in pixels. */
#define PUSH_STEP_Z(step) ((Sint16)(step))

/* The kinds of thing the leader can push, each with its rectangle: the rows of
   a scene's PushKinds and PushRectangles, a log, a pipe and a fallen column,
   each lying west to east and north to south. */
#define PUSH_KINDS 6

#endif
