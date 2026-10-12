/* Near miss, differ 2 of 94 (one reordered row): the native loads *cursor (ldr r6, [r0]) before it copies columns to r7; this draft copies first. Tried 2026-10-12: an empty asm barrier after the two initial loads moves the whole prologue (differ 20); three permuter minutes found nothing. */
#include "types.h"
#include "input.h"

void Audio_PlayCue(s32 cue);

s32 Menu_MoveGridCursor(s32 *cursor, s32 count, s32 columns)
{
    struct InputState *input = &gInput;
    s32 result = 0;
    s32 old = *cursor;
    s32 pos = old;

    if (input->repeat & 0x20) {
        pos--;
        pos = (pos + count) % count;
    }
    if (input->repeat & 0x10) {
        pos++;
        pos = (pos + count) % count;
    }
    if (input->repeat & 0x40) {
        if (pos - columns >= 0)
            pos -= columns;
    }
    if (input->repeat & 0x80) {
        if (pos + columns < (columns + count - 1) / columns * columns)
            pos += columns;
        if (pos > count - 1)
            pos = count - 1;
    }
    if (old != pos) {
        Audio_PlayCue(111);
        result = 1;
    }
    if (old / columns * columns != pos / columns * columns)
        result = 2;
    *cursor = pos;
    return result;
}
