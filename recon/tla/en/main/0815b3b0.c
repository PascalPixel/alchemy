/* Near miss of Func_0815b3b0 (listing 46 instructions, draft 46, differ 2,
 * one reordered row): the native reads the flag byte at +0x11 (ldrb r2)
 * between the two halves of 0x10000 (movs r3, #128 / lsls r3, r3, #9); here
 * the constant is finished first. In sched2 the shift outranks the load by
 * one, which a load latency of 3 would reverse. Tried: the flag clear before
 * the 0x10000 store (differ 10), the constant through
 * asm("" : "+l"(one)) volatile or not (differ 8 and 2), one-pass blocks
 * (differ 17), an empty asm (differ 8), the flag byte read into a local held
 * by an asm (differ 11, the mask becomes 254), an asm on the sprite pointer
 * (differ 22). */
#include "types.h"
#include "animspr.h"
#include "ram_buffer.h"

struct AnimationEntry *Func_0815b24c(void);
s32 Animation_ApplyChildArgumentFar(void *, s32);

struct EffectSprite {
    u8 unknown_00[0x0c];
    s32 unknown_0c;
    u8 unknown_10;
    u8 unknown_11_0 : 1;
    u8 unknown_11_1 : 7;
    u16 unknown_12;
    u8 kind;
    u8 unknown_15;
    u8 unknown_16;
    u8 unknown_17;
    u8 unknown_18[2];
    u8 unknown_1a;
    u8 unknown_1b;
    u8 unknown_1c[0x0c];
    struct AnimationEntry *entry;
    u8 unknown_2c[0x0c];
};

// Takes the first free effect sprite of the 64 with a new animation entry.
struct EffectSprite *Func_0815b3b0(s32 kind, s32 value)
{
    struct EffectSprite *sprite = Ram_HeapSlots->unknown_0c[1];
    struct EffectSprite *found = NULL;
    s32 i = 0;

    while (i <= 63) {
        if (sprite->kind == 0) {
            found = sprite;
            break;
        }
        i++;
        sprite++;
    }
    sprite->entry = Func_0815b24c();
    sprite->kind = kind;
    sprite->unknown_15 = value;
    sprite->unknown_0c = 0x10000;
    sprite->unknown_11_0 = 0;
    sprite->unknown_17 = 0;
    sprite->unknown_16 = 0;
    sprite->unknown_1b = 1;
    sprite->unknown_1a = 0;
    sprite->unknown_12 = 0;
    Animation_ApplyChildArgumentFar(sprite, 0);
    return found;
}
