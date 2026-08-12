#include "types.h"

#define U8_AT(p, o)  (*(u8 *)((u8 *)(p) + (o)))
#define S32_AT(p, o) (*(s32 *)((u8 *)(p) + (o)))
#define WORK_080f7f78 (*(u8 **)0x02004c00)

struct MatchNode_080f7f78 {
    struct MatchNode_080f7f78 *next;
    s32 unknown_04;
    s32 position;
};

void Func_080f7db4(void);
void Func_080f7e60(s32, s32, const void *);
void Func_080f7f30(s32);

#define FIND_BEST_MATCH_080f7f78(position_)                              \
    do {                                                                 \
        s32 *match_ring = (s32 *)(WORK_080f7f78 + 0x3404);               \
        s32 match_symbol = match_ring[(position_)];                     \
        struct MatchNode_080f7f78 *match_candidate;                     \
                                                                         \
        S32_AT(WORK_080f7f78, 0x4430) = 1;                              \
        if (match_symbol != -1) {                                       \
            match_candidate =                                             \
                ((struct MatchNode_080f7f78 **)(WORK_080f7f78 + 0x3000))[ \
                    match_symbol];                                      \
            if ((position_) + 0x110 <= 0x3ff) {                         \
                while (match_candidate != 0) {                          \
                    s32 previous = match_candidate->position;           \
                    s32 distance = ((position_) - previous) & 0x3ff;    \
                                                                         \
                    if ((u32)(distance - 1) <= 0x3e) {                  \
                        s32 length = 1;                                \
                                                                         \
                        while (length <= 0x10f &&                       \
                               match_ring[(position_) + length] ==      \
                                   match_ring[previous + length])       \
                            length++;                                  \
                                                                         \
                        if (S32_AT(WORK_080f7f78, 0x4430) < length) {   \
                            S32_AT(WORK_080f7f78, 0x442c) = distance;    \
                            S32_AT(WORK_080f7f78, 0x4430) = length;      \
                            if (length == 0x110)                        \
                                break;                                   \
                        }                                                \
                    }                                                    \
                    match_candidate = match_candidate->next;           \
                }                                                        \
            } else {                                                     \
                while (match_candidate != 0) {                          \
                    s32 previous = match_candidate->position;           \
                    s32 distance = ((position_) - previous) & 0x3ff;    \
                                                                         \
                    if ((u32)(distance - 1) <= 0x3e) {                  \
                        s32 length = 1;                                \
                                                                         \
                        while (length <= 0x10f &&                       \
                               match_ring[((position_) + length) &      \
                                          0x3ff] ==                      \
                                   match_ring[(previous + length) &     \
                                              0x3ff])                   \
                            length++;                                  \
                                                                         \
                        if (S32_AT(WORK_080f7f78, 0x4430) < length) {   \
                            S32_AT(WORK_080f7f78, 0x442c) = distance;    \
                            S32_AT(WORK_080f7f78, 0x4430) = length;      \
                            if (length == 0x110)                        \
                                break;                                   \
                        }                                                \
                    }                                                    \
                    match_candidate = match_candidate->next;           \
                }                                                        \
            }                                                            \
        }                                                                \
    } while (0)

s32 Func_080f7f78(const void *input, s32 output, s32 mode)
{
    s32 defer_search = 0;

    *(u8 **)0x02004c00 = (u8 *)0x02010000;
    Func_080f7db4();

    S32_AT(WORK_080f7f78, 0x4440) = mode;
    S32_AT(WORK_080f7f78, 0x4434) = 0;
    S32_AT(WORK_080f7f78, 0x4438) = 0;
    S32_AT(WORK_080f7f78, 0x443c) = 0;
    S32_AT(WORK_080f7f78, 0x3400) = 0x80;
    U8_AT(WORK_080f7f78, 0x4408) = 0;
    S32_AT(WORK_080f7f78, 0x4404) = 1;

    Func_080f7e60(0, 0x2a0, input);
    while (((s32 *)(WORK_080f7f78 + 0x3404))[
               S32_AT(WORK_080f7f78, 0x4434)] != -1) {
        s32 cursor = S32_AT(WORK_080f7f78, 0x4434);

        if (!defer_search) {
            s32 saved_distance;
            s32 saved_length;

            FIND_BEST_MATCH_080f7f78(cursor);
            saved_distance = S32_AT(WORK_080f7f78, 0x442c);
            saved_length = S32_AT(WORK_080f7f78, 0x4430);

            if (saved_length > 1) {
                s32 next_position = (cursor + 1) & 0x3ff;
                FIND_BEST_MATCH_080f7f78(next_position);

                if (S32_AT(WORK_080f7f78, 0x4430) > 2) {
                    s32 next_score = S32_AT(WORK_080f7f78, 0x4430) + 1;
                    s32 later_position = (cursor + saved_length) & 0x3ff;
                    FIND_BEST_MATCH_080f7f78(later_position);
                    if (next_score >=
                        S32_AT(WORK_080f7f78, 0x4430) + saved_length) {
                        saved_length = 1;
                        defer_search = 1;
                    }
                }
            }
            S32_AT(WORK_080f7f78, 0x442c) = saved_distance;
            S32_AT(WORK_080f7f78, 0x4430) = saved_length;
        }

        if (S32_AT(WORK_080f7f78, 0x4430) > 1) {
            s32 length = S32_AT(WORK_080f7f78, 0x4430);
            s32 distance = S32_AT(WORK_080f7f78, 0x442c);
            s32 out = S32_AT(WORK_080f7f78, 0x4404);

            defer_search = 0;
            U8_AT(WORK_080f7f78, 0x4408) |=
                (u8)S32_AT(WORK_080f7f78, 0x3400);
            if (length <= 0x10) {
                u16 token = (distance & 0xff) |
                            ((distance << 4) & 0xf000) |
                            (((length - 1) << 8) & 0x0f00);
                U8_AT(WORK_080f7f78, 0x4408 + out++) = token >> 8;
                U8_AT(WORK_080f7f78, 0x4408 + out++) = token;
            } else {
                u16 token =
                    (distance & 0xff) | ((distance << 4) & 0xf000);
                U8_AT(WORK_080f7f78, 0x4408 + out++) = token >> 8;
                U8_AT(WORK_080f7f78, 0x4408 + out++) = token;
                U8_AT(WORK_080f7f78, 0x4408 + out++) = length - 0x11;
            }
            S32_AT(WORK_080f7f78, 0x4404) = out;
        } else {
            s32 out = S32_AT(WORK_080f7f78, 0x4404);
            U8_AT(WORK_080f7f78, 0x4408 + out) =
                (u8)((s32 *)(WORK_080f7f78 + 0x3404))[
                    S32_AT(WORK_080f7f78, 0x4434)];
            S32_AT(WORK_080f7f78, 0x4404) = out + 1;
            S32_AT(WORK_080f7f78, 0x4430) = 1;
        }

        Func_080f7e60(
            S32_AT(WORK_080f7f78, 0x4434) + 0x2a0,
            S32_AT(WORK_080f7f78, 0x4430), input);
        S32_AT(WORK_080f7f78, 0x4434) =
            (S32_AT(WORK_080f7f78, 0x4434) +
             S32_AT(WORK_080f7f78, 0x4430)) &
            0x3ff;
        S32_AT(WORK_080f7f78, 0x3400) >>= 1;

        if (S32_AT(WORK_080f7f78, 0x3400) == 0) {
            Func_080f7f30(output);
            S32_AT(WORK_080f7f78, 0x3400) = 0x80;
            U8_AT(WORK_080f7f78, 0x4408) = 0;
            S32_AT(WORK_080f7f78, 0x4404) = 1;
        }
    }

    U8_AT(WORK_080f7f78, 0x4408) |=
        (u8)S32_AT(WORK_080f7f78, 0x3400);
    U8_AT(WORK_080f7f78, 0x4408 + S32_AT(WORK_080f7f78, 0x4404)++) = 0;
    U8_AT(WORK_080f7f78, 0x4408 + S32_AT(WORK_080f7f78, 0x4404)++) = 0;
    Func_080f7f30(output);
    return S32_AT(WORK_080f7f78, 0x443c);
}
