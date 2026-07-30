#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S8_AT(p, o)  (*(s8 *)((u8 *)(p) + (o)))
#define U16_AT(p, o) (*(u16 *)((u8 *)(p) + (o)))
#define S16_AT(p, o) (*(s16 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct Position_0801a98c {
    s32 x;
    s32 y;
};

s32 Func_080b50b0(s32, struct Position_0801a98c *);
s32 Func_080770c0(s32);
void Func_08003dec(void *, s32);
void *Func_0801b36c(void *);
s32 Func_08003d28(void *);
void Func_0801aeec(void *, s32);
s32 Func_08003fa4(s32, s32, const void *);

static void SetSpriteX_0801a98c(void *sprite, s32 x)
{
    U16_AT(sprite, 6) = (U16_AT(sprite, 6) & 0xfe00) | (x & 0x1ff);
}

#define CONFIGURE_VISIBILITY(runtime, entity, sprite)                       \
    do {                                                                    \
        if (Func_080770c0(0x103) != 0) {                                   \
            if (U16_AT((runtime), 0x2e2) == 1)                             \
                U8_AT((sprite), 5) = (U8_AT((sprite), 5) & ~0x0c) | 4;     \
            else                                                            \
                U8_AT((sprite), 5) &= (u8)~0x0c;                           \
            if (U16_AT((entity), 0x0a) == 1)                               \
                U8_AT((sprite), 5) = (U8_AT((sprite), 5) & ~0x0c) | 4;     \
        }                                                                   \
    } while (0)

void Func_0801a98c(void)
{
    u8 *runtime = *(u8 **)0x03001e98;
    u8 *entity;
    u8 *main_object = runtime + 0x2d8;
    u8 *cursor_object = runtime + 0x30c;
    u8 *main_sprite = runtime + 0x300;
    s32 index = 0;

    for (entity = PTR_AT(runtime, 0x348);
         entity != 0;
         entity = PTR_AT(entity, 4), index++) {
        u8 *sprite = entity + 0x28;
        s32 priority = 0xf0;

        U8_AT(sprite, 5) &= (u8)~3;
        U8_AT(sprite, 7) &= (u8)~0x3e;
        SetSpriteX_0801a98c(sprite, U16_AT(entity, 0x10));
        S8_AT(sprite, 4) = U16_AT(entity, 0x12);

        if (U16_AT(runtime, 0x3a0) != 0) {
            S8_AT(sprite, 4) =
                U16_AT(entity, 0x12) + U16_AT(runtime, 0x3a0);
        } else if (S16_AT(entity, 0x10) != S16_AT(entity, 0x18)) {
            s32 next = S16_AT(entity, 0x10) + S16_AT(entity, 0x14);
            s32 target = S16_AT(entity, 0x18);
            s32 step = S16_AT(entity, 0x14);
            if ((step > 0 && next > target) || (step <= 0 && next < target))
                next = target;
            S16_AT(entity, 0x10) = next;
            SetSpriteX_0801a98c(sprite, next);
        } else if (index == U16_AT(runtime, 0x39e)) {
            priority = 0xf1;
            if (U16_AT(main_object, 0x0a) != 0) {
                struct Position_0801a98c position;
                if (Func_080b50b0(U16_AT(entity, 8), &position) != -1) {
                    U16_AT(main_object, 0x18) = position.x;
                    U16_AT(main_object, 0x1a) = position.y;
                    if (S16_AT(main_object, 0x22) == 0) {
                        U16_AT(main_object, 0x10) = position.x;
                        S16_AT(main_object, 0x12) = position.y;
                        S16_AT(main_object, 0x22) = 1;
                    }
                }
            }
        }

        if (S16_AT(entity, 0x22) != 0) {
            CONFIGURE_VISIBILITY(runtime, entity, sprite);
            Func_08003dec(sprite, priority);
        }
    }

    if (U16_AT(cursor_object, 0x0a) != 0) {
        u8 *sprite = cursor_object + 0x28;
        u8 *tracked = Func_0801b36c(runtime);
        s32 phase = (*(u32 *)0x03001800 >> 1) & 15;
        const s8 *wave = (const s8 *)0x08036740;

        U8_AT(sprite, 5) =
            ((U8_AT(sprite, 5) & ~0x0c & ~3 & ~0x10) | 0x20) & 0x3f;
        U8_AT(sprite, 7) = (U8_AT(sprite, 7) & 0x3f & ~0x3e) | 0x80;
        U8_AT(sprite, 9) &= (u8)~0x0c;
        U16_AT(sprite, 8) =
            (U16_AT(sprite, 8) & 0xfc00) |
            (U16_AT(cursor_object, 0x0e) & 0x3ff);
        SetSpriteX_0801a98c(sprite, U16_AT(tracked, 0x10) - 4);
        U8_AT(sprite, 4) =
            U8_AT(tracked, 0x12) + (wave[phase] >> 1) - 4;

        if (S16_AT(cursor_object, 0x22) != S16_AT(cursor_object, 0x26)) {
            U16_AT(runtime, 0x340) = S16_AT(cursor_object, 0x22);
            U16_AT(runtime, 0x342) = S16_AT(cursor_object, 0x22);
            S16_AT(runtime, 0x344) = 0;
            U8_AT(sprite, 7) =
                (U8_AT(sprite, 7) & ~0x3e) |
                ((Func_08003d28(runtime + 0x340) & 0x1f) * 2);
            U8_AT(sprite, 5) |= 3;
            SetSpriteX_0801a98c(sprite, (U16_AT(sprite, 6) & 0x1ff) - 16);
            U8_AT(sprite, 4) -= 16;
            S16_AT(cursor_object, 0x22) += U16_AT(cursor_object, 0x24);
        }
        if (Func_080770c0(0x103) != 0)
            U8_AT(sprite, 5) = (U8_AT(sprite, 5) & ~0x0c) | 4;
        Func_08003dec(sprite, 0xf8);
    }

    Func_0801aeec(runtime, 0);
    Func_0801aeec(runtime, 1);

    for (entity = PTR_AT(runtime, 0x34c);
         entity != 0;
         entity = PTR_AT(entity, 4)) {
        u8 *sprite = entity + 0x28;

        if (S16_AT(entity, 0x10) != S16_AT(entity, 0x18))
            S16_AT(entity, 0x10) += U16_AT(entity, 0x14);
        if (S16_AT(entity, 0x12) != S16_AT(entity, 0x1a))
            S16_AT(entity, 0x12) += U16_AT(entity, 0x16);
        SetSpriteX_0801a98c(sprite, S16_AT(entity, 0x10));
        U8_AT(sprite, 4) = S16_AT(entity, 0x12);

        if (S16_AT(entity, 0x22) != S16_AT(entity, 0x26)) {
            S16_AT(entity, 0x22) += U16_AT(entity, 0x24);
            U16_AT(runtime, 0x340) = S16_AT(entity, 0x22);
            U16_AT(runtime, 0x342) = S16_AT(entity, 0x22);
            S16_AT(runtime, 0x344) = 0;
            U8_AT(sprite, 7) =
                (U8_AT(sprite, 7) & ~0x3e) |
                ((Func_08003d28(runtime + 0x340) & 0x1f) * 2);
            U8_AT(sprite, 5) |= 3;
            SetSpriteX_0801a98c(sprite, (U16_AT(sprite, 6) & 0x1ff) - 8);
            U8_AT(sprite, 4) -= 8;
        } else {
            U8_AT(sprite, 5) &= (u8)~3;
            U8_AT(sprite, 7) &= (u8)~0x3e;
        }
        CONFIGURE_VISIBILITY(runtime, entity, sprite);
        Func_08003dec(sprite, 0xf0);
    }

    if (U16_AT(main_object, 0x0a) != 0) {
        s32 phase = (*(u32 *)0x03001800 >> 2) & 15;
        const s8 *wave = (const s8 *)0x08036740;
        s32 delta;

        U16_AT(main_sprite, 8) =
            (U16_AT(main_sprite, 8) & 0xfc00) |
            (Func_08003fa4(U16_AT(main_object, 0x0c), 0x100,
                           (const void *)(0x080346f8 + (phase << 8))) &
             0x3ff);

        delta = S16_AT(main_object, 0x18) - S16_AT(main_object, 0x10);
        if (delta != 0)
            S16_AT(main_object, 0x10) +=
                (delta >> 1) != 0 ? delta >> 1 : delta;
        delta = S16_AT(main_object, 0x1a) - S16_AT(main_object, 0x12);
        if (delta != 0)
            S16_AT(main_object, 0x12) +=
                (delta >> 1) != 0 ? delta >> 1 : delta;

        U8_AT(main_sprite, 4) =
            wave[phase] + S16_AT(main_object, 0x12) - 0x20;
        SetSpriteX_0801a98c(main_sprite, U16_AT(main_object, 0x10) - 4);
        if (Func_080770c0(0x103) != 0) {
            if (U16_AT(runtime, 0x2e2) == 1)
                U8_AT(main_sprite, 5) =
                    (U8_AT(main_sprite, 5) & ~0x0c) | 4;
            else
                U8_AT(main_sprite, 5) &= (u8)~0x0c;
        }
        Func_08003dec(main_sprite, 0xf8);
    }

    U16_AT(runtime, 0x3a2)++;
}
