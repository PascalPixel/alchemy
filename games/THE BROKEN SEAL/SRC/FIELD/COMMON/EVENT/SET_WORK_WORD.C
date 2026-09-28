#include "TYPES.H"

/* The event work as this setter sees it: one word at +0x10 that scripts
   store through the far-call table. */
struct EventWordWork {
    u8 unknown_00[0x10];
    s32 word_10;
};

extern struct EventWordWork *gEventWork;

void Event_SetWorkWord10(s32 value)
{
    gEventWork->word_10 = value;
}
