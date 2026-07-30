#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define PTR_AT(p, o) (*(void **)((u8 *)(p) + (o)))

struct MatchNode_080f7f78 {
    struct MatchNode_080f7f78 *next;
    s32 unknown_04;
    s32 position;
};

void Func_080f7db4(void);
void Func_080f7e60(s32, s32, const void *);
void Func_080f7f30(s32);

static s32 *Ring_080f7f78(u8 *work)
{
    return (s32 *)(work + 0x3404);
}

static struct MatchNode_080f7f78 **Heads_080f7f78(u8 *work)
{
    return (struct MatchNode_080f7f78 **)(work + 0x3000);
}

static void FindBestMatch_080f7f78(u8 *work, s32 position)
{
    s32 *ring = Ring_080f7f78(work);
    s32 symbol = ring[position];
    struct MatchNode_080f7f78 *candidate;

    S32_AT(work, 0x4430) = 1;
    if (symbol == -1)
        return;

    candidate = Heads_080f7f78(work)[symbol];
    while (candidate != 0) {
        s32 previous = candidate->position;
        s32 distance = (position - previous) & 0x3ff;

        if ((u32)(distance - 1) <= 0x3e) {
            s32 length = 1;

            while (length <= 0x10f &&
                   ring[(position + length) & 0x3ff] ==
                       ring[(previous + length) & 0x3ff])
                length++;

            if (S32_AT(work, 0x4430) < length) {
                S32_AT(work, 0x442c) = distance;
                S32_AT(work, 0x4430) = length;
                if (length == 0x110)
                    return;
            }
        }
        candidate = candidate->next;
    }
}

s32 Func_080f7f78(const void *input, s32 output, s32 mode)
{
    u8 *work;
    s32 defer_search = 0;

    *(u8 **)0x02004c00 = (u8 *)0x02010000;
    Func_080f7db4();
    work = *(u8 **)0x02004c00;

    S32_AT(work, 0x4440) = mode;
    S32_AT(work, 0x4434) = 0;
    S32_AT(work, 0x4438) = 0;
    S32_AT(work, 0x443c) = 0;
    S32_AT(work, 0x3400) = 0x80;
    U8_AT(work, 0x4408) = 0;
    S32_AT(work, 0x4404) = 1;

    Func_080f7e60(0, 0x2a0, input);
    while (Ring_080f7f78(work)[S32_AT(work, 0x4434)] != -1) {
        s32 cursor = S32_AT(work, 0x4434);

        if (!defer_search) {
            s32 saved_distance;
            s32 saved_length;

            FindBestMatch_080f7f78(work, cursor);
            saved_distance = S32_AT(work, 0x442c);
            saved_length = S32_AT(work, 0x4430);

            if (saved_length > 1) {
                s32 next_position = (cursor + 1) & 0x3ff;
                FindBestMatch_080f7f78(work, next_position);

                if (S32_AT(work, 0x4430) > 2) {
                    s32 next_score = S32_AT(work, 0x4430) + 1;
                    s32 later_position = (cursor + saved_length) & 0x3ff;
                    FindBestMatch_080f7f78(work, later_position);
                    if (next_score >= S32_AT(work, 0x4430) + saved_length) {
                        saved_length = 1;
                        defer_search = 1;
                    }
                }
            }
            S32_AT(work, 0x442c) = saved_distance;
            S32_AT(work, 0x4430) = saved_length;
        }

        if (S32_AT(work, 0x4430) > 1) {
            s32 length = S32_AT(work, 0x4430);
            s32 distance = S32_AT(work, 0x442c);
            s32 out = S32_AT(work, 0x4404);

            defer_search = 0;
            U8_AT(work, 0x4408) |= (u8)S32_AT(work, 0x3400);
            if (length <= 0x10) {
                u16 token = (distance & 0xff) |
                            ((distance << 4) & 0xf000) |
                            (((length - 1) << 8) & 0x0f00);
                U8_AT(work, 0x4408 + out++) = token >> 8;
                U8_AT(work, 0x4408 + out++) = token;
            } else {
                u16 token =
                    (distance & 0xff) | ((distance << 4) & 0xf000);
                U8_AT(work, 0x4408 + out++) = token >> 8;
                U8_AT(work, 0x4408 + out++) = token;
                U8_AT(work, 0x4408 + out++) = length - 0x11;
            }
            S32_AT(work, 0x4404) = out;
        } else {
            s32 out = S32_AT(work, 0x4404);
            U8_AT(work, 0x4408 + out) =
                (u8)Ring_080f7f78(work)[S32_AT(work, 0x4434)];
            S32_AT(work, 0x4404) = out + 1;
            S32_AT(work, 0x4430) = 1;
        }

        Func_080f7e60(
            S32_AT(work, 0x4434) + 0x2a0,
            S32_AT(work, 0x4430), input);
        S32_AT(work, 0x4434) =
            (S32_AT(work, 0x4434) + S32_AT(work, 0x4430)) & 0x3ff;
        S32_AT(work, 0x3400) >>= 1;

        if (S32_AT(work, 0x3400) == 0) {
            Func_080f7f30(output);
            S32_AT(work, 0x3400) = 0x80;
            U8_AT(work, 0x4408) = 0;
            S32_AT(work, 0x4404) = 1;
        }
    }

    U8_AT(work, 0x4408) |= (u8)S32_AT(work, 0x3400);
    U8_AT(work, 0x4408 + S32_AT(work, 0x4404)++) = 0;
    U8_AT(work, 0x4408 + S32_AT(work, 0x4404)++) = 0;
    Func_080f7f30(output);
    return S32_AT(work, 0x443c);
}
