#include "types.h"

/*
 * Advance one mover towards its target point.
 *
 * The target is disabled by the sentinel 0x80000000 in the target x, and the
 * owner returns immediately when it sees it.  Otherwise it takes the offset
 * to the target and, when the s8 flag at +0x41 is set, tests whether the
 * mover has arrived: the offsets are converted from 16.16 to whole units by
 * the usual truncating `if (v < 0) v += 0xffff; v >>= 16`, squared, and the
 * sum handed to the IWRAM square root at 0x030001d8.  A root below 128 is
 * refined - the two 16.16 squares are formed by the IWRAM multiply at
 * 0x03000118 and their sum passed to Func_080045d4 - and a refined distance
 * of 0x80000 or less counts as arrival, which hands the mover to
 * Func_0809ba5c and returns.
 *
 * When the mover has not arrived, Func_080044d0 gives the heading to the
 * target.  If the s8 flag at +0x42 is set the heading is rate-limited: the
 * signed halfword difference from the current heading is clamped to the turn
 * limit at +0x32, and a difference that is both negative and past the limit
 * turns by the negated limit rather than the limit itself.  Speed then
 * accelerates by the step at +0x24 up to the cap at +0x20, and the new
 * heading and speed move the mover through cosine at 0x0800231c and sine at
 * 0x08002322 scaled by the IWRAM multiply.
 *
 * Neither relocated helper is reached by a `bl` naming it and neither is
 * retained assembly: `bl Func_080072f0` at 0x0809b946 is the runtime's
 * `_call_via_r3` thunk inside the table at 0x080072e4, so it is a call
 * through the pooled 0x030001d8; and the four `mov ip, pc ; bx rN`
 * sequences at 0x0809b95c, 0x0809b968, 0x0809ba00 and 0x0809ba14 call the
 * pooled 0x03000118 inline, `mov ip, pc` setting the return address to the
 * halfword after the `bx` so control resumes in the same frame - the reading
 * already established for 0x0800ebec.  Both are written as ordinary calls
 * through function pointers.
 *
 * The epilogue at 0x0809ba1e ends `pop {r0} ; bx r0`, so the owner is void.
 * One direct caller, 0x0809b84e.
 */

/* 0x030001d8, the IWRAM square-root helper. */
typedef s32 (*SquareRoot_0809b8f4)(s32 value);

/* 0x03000118, the IWRAM-relocated ARM fixed-point multiply. */
typedef s32 (*Multiply_0809b8f4)(s32 a, s32 b);

s32 Func_080045d4(s32 sum_of_squares);
s32 Func_080044d0(s32 y, s32 x);
s32 Func_0800231c(s32 angle);
s32 Func_08002322(s32 angle);
void Func_0809ba5c(void *mover, s32 target_x, s32 target_y);

struct Mover_0809b8f4 {
    u8 unknown_00[4];
    s32 x;
    s32 y;
    s32 target_x;
    s32 target_y;
    u8 unknown_14[8];
    s32 speed;
    s32 speed_cap;
    s32 acceleration;
    u8 unknown_28[8];
    u16 heading;
    s16 turn_limit;
    u8 unknown_34[0xd];
    s8 check_arrival;
    s8 limit_turn;
};

void Func_0809b8f4(struct Mover_0809b8f4 *mover)
{
    SquareRoot_0809b8f4 square_root = (SquareRoot_0809b8f4)0x030001d8;
    Multiply_0809b8f4 multiply = (Multiply_0809b8f4)0x03000118;
    s32 offset_x;
    s32 offset_y;
    s32 whole_x;
    s32 whole_y;
    s32 distance;
    s16 heading;
    u16 previous;
    s16 difference;
    s16 magnitude;
    s16 limit;
    s32 speed;

    if (mover->target_x == (s32)0x80000000) {
        return;
    }

    offset_x = mover->target_x - mover->x;
    offset_y = mover->target_y - mover->y;

    if (mover->check_arrival != 0) {
        whole_x = offset_x;
        if (offset_x < 0) {
            whole_x = offset_x + 0xffff;
        }
        whole_x >>= 16;
        whole_y = offset_y;
        if (offset_y < 0) {
            whole_y = offset_y + 0xffff;
        }
        whole_y >>= 16;

        distance = square_root(whole_x * whole_x + whole_y * whole_y);
        if ((s16)distance < 128) {
            distance = Func_080045d4(multiply(offset_x, offset_x)
                                     + multiply(offset_y, offset_y));
        }
        if (distance <= 0x80000) {
            Func_0809ba5c(mover, mover->target_x, mover->target_y);
            return;
        }
    }

    heading = (s16)Func_080044d0(offset_y, offset_x);
    if (mover->limit_turn != 0) {
        previous = mover->heading;
        difference = (s16)((s32)heading - (s32)previous);
        magnitude = difference;
        if (difference < 0) {
            magnitude = -difference;
        }
        limit = mover->turn_limit;
        if (magnitude >= limit) {
            if (difference < 0) {
                if (-difference > limit) {
                    difference = (s16)(-(s32)(u16)limit);
                }
            } else if (difference > limit) {
                difference = limit;
            }
            heading = (s16)((s32)difference + (s32)previous);
        }
    }

    mover->heading = (u16)heading;

    speed = mover->speed + mover->acceleration;
    if (speed > mover->speed_cap) {
        speed = mover->speed_cap;
    }
    mover->speed = speed;

    heading = (s16)mover->heading;
    mover->x += multiply(Func_0800231c(heading), speed);
    mover->y += multiply(Func_08002322(heading), speed);
}
