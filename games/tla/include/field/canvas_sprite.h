#ifndef ALCHEMY_CANVAS_SPRITE_H
#define ALCHEMY_CANVAS_SPRITE_H

/* The canvas sprite of a field Psynergy: a 64 by 64 sprite whose pixels an
   effect draws into a 4 KB canvas. fdBattleEffectStartCanvasSpr starts it,
   fdBattleEffectDrawCanvasSpr draws the sprite each tick,
   fdBattleEffectUploadCanvas copies the canvas into its tiles on command, and
   fdBattleEffectEndCanvasSpr ends it. */

#include "types.h"
#include "static_assert.h"
#include "field/effect_sprite.h"

/* The canvas: 64x64 pixels of 8 bits, in 8x8 tiles. */
#define CANVAS_WIDTH		64
#define CANVAS_SPRITE_BYTES 4096

/* How the sprite is drawn. */
#define CANVAS_SPRITE_16_COLORS	 1 /* rebuilt each tick, in OBJ palette 14 */
#define CANVAS_SPRITE_256_COLORS 2 /* rebuilt each tick, in 256 colours */
#define CANVAS_SPRITE_KEPT		 3 /* drawn as the effect left it */

/* The OBJ palette a 16-colour canvas is drawn in. The effects that draw in 256
   colours keep their own colours in it and the next, colours 224 to 255. */
#define CANVAS_PALETTE 14

/* What fdBattleEffectUploadCanvas does with the canvas in the next tick. */
#define CANVAS_UPLOAD_AND_CLEAR				 1
#define CANVAS_UPLOAD						 2
#define CANVAS_UPLOAD_AND_DARKEN_ONE_LEVEL	 3
#define CANVAS_UPLOAD_AND_DARKEN_TWO_LEVELS	 4
#define CANVAS_UPLOAD_AND_DARKEN_FOUR_LEVELS 5
#define CANVAS_UPLOAD_TO_BG					 11

/* A field Psynergy's canvas sprite: its effect sprite, the tick it is on and
   how long it has drawn, and where, how large, at what angle and depth it is
   drawn. */
typedef struct CANVAS_SPR_STATE {
	EFFECT_SPR spr;
	Uint8	   unusedAfterSpr[120]; /* no code names it */
	Sint32	   drawnFrm;
	Sint32	   frm;
	Sint32	   x; /* whole pixels */
	Sint32	   y;
	Sint32	   scaleX;
	Sint32	   scaleY;
	Sint32	   ang;
	Uint16	   tile;
	Uint8	   depth;
	Uint8	   pri;
	Sint8	   mode;
	Sint8	   command;
	Uint8	   unusedAfterCommand[2]; /* no code names it */
} CANVAS_SPR_STATE;

STATIC_ASSERT(sizeof(CANVAS_SPR_STATE) == 196);
STATIC_ASSERT(offsetof(CANVAS_SPR_STATE, frm) == 164);
STATIC_ASSERT(offsetof(CANVAS_SPR_STATE, tile) == 188);

void fdBattleEffectUploadCanvas(void);
void fdBattleEffectDrawCanvasSpr(void);
void fdBattleEffectStartCanvasSpr(void);
void fdBattleEffectEndCanvasSpr(void);

#endif
