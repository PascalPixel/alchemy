#ifndef FUNE_HEYA_HEYA_H
#define FUNE_HEYA_HEYA_H

/* Helpers shared by the ship cabins' scene code. */

#include "TYPES.H"

#ifdef ALCHEMY_FIELD_EVENT_H
/* Moves the next dialogue line on by amount messages. */
static __inline__ void bump_step(s32 amount)
{
    gEventWork->message += amount;
}
#endif

#endif
