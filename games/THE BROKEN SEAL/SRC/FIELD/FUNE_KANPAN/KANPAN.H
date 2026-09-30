#ifndef FUNE_KANPAN_KANPAN_H
#define FUNE_KANPAN_KANPAN_H

/* Helpers shared by the ship deck's scene code. */

#include "TYPES.H"

#ifdef ALCHEMY_FIELD_EVENT_H
/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}
#endif

#endif
