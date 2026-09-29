#ifndef TLA_DISPLAY_SCROLL_H
#define TLA_DISPLAY_SCROLL_H

#include "TYPES.H"

/* The row the streamed background scrolls toward; the current row follows
   it a capped step each frame and the new rows are copied in as it moves. */
extern s16 gScrollTarget;

#endif
