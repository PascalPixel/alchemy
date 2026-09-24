#include "TYPES.H"

typedef void (*FillFn)(void *dst, s32 size, s32 value);

static __inline__ void FillWords(void *dst, s32 size, s32 value)
{
    ((FillFn)0x03000168)(dst, size, value);
}

/* Builds the battle presentation tilemap: 256 blank words, 128 border words,
   240 words of sequential tile pairs from 0x0201, then 640 border words. */
void BattlePresentation_BuildTilemap(s32 *destination)
{
    s32 entry;
    u32 index;

    FillWords(destination, 0x100, -1);
    destination += 0x40;
    FillWords(destination, 0x80, 0x03ff03ff);
    entry = 0x02010200;
    destination += 0x20;
    index = 0;
    do {
        index++;
        *destination++ = entry;
        entry += 0x00020002;
    } while (index <= 239);
    FillWords(destination, 0x280, 0x03ff03ff);
}
