/* 0800a494: ARM draft (Object_UpdateAll) for the game ARM route (K1,
   agbcc/camelot_arm). Route decision (2026-10-07), this draft under agscc
   -marm with the game options: differ 468; with -mno-apcs-frame 463; with
   -mno-apcs-frame -ffixed-fp 461.
   alchemy diff --camelot-arm, upstream's whole arm_adjust_cost
   (2026-10-06, session 6): listing 307, draft 300, common 245, differ
   117; the same draft under stock agbcc_arm: common 250, differ 107
   (stock still fits this function better). 140 to 117 (session 6, aimed by --heat): the steering test as
   x != 0 || z != 0 (136; the listing compares sl, then cmpeq r9); the
   angle kept in a u16 local, angle = ArcTan2(z, x), so the listing's
   lsl #16 and rsb ..., lsr #16 zero-extension survives (129); the turn
   in its own local rather than dx, which takes r0 as listed (117; ratio
   reused: 117). Trials (session 6): case 17's natural order (obj->y -
   target) ^ (y - target), as the listing computes it: 138; a third
   smull alternative tying RdHi to Rm (the bounce site's smull r2, r3,
   r3, r2), with or without RdLo tied to Rs: 117, not kept; the asm
   volatile, holding each velocity store before the next smull as
   listed: 131 from 140, not kept (an ordering device); alchemy permute
   --camelot-arm --heat, 6 minutes, 8 jobs, 13,345 candidates: none
   beat 117 (weighted 1970). What remains:
   those stores sunk past the next smull (rows 125-150 and the speed
   limit's shared tail), register choices for the reached flag, the
   script cursor and the target branch's calls, and the six veneer
   instructions. Earlier: listing 307, draft 299, common 233, differ
   140; stock agbcc_arm 130. Upstream's call rule (a true dependence into
   a CALL_INSN costs 1) costs this draft 16: under the anti/output half
   alone, which the trials below were scored with, it was common 241,
   differ 124. Trial (session 5): alchemy permute --camelot-arm
   --function Object_UpdateAll, 4 minutes, 8 jobs, random rewrites: no
   candidate beat 140 (weighted 2370; 52 register, 13 reordered, 1
   inserted, 9 deleted). 146 to 124 (2026-10-06, session 4): the
   target branch calls IwramSqrt and IwramRatioMulQ14 through two locals,
   sq = IwramSqrt; rm = IwramRatioMulQ14, at the branch's start: the
   listing's five ldr rN, =fn; mov lr, pc; bx rN (gcc_arm defines
   NO_FUNCTION_CSE, so a direct call is always a bl), while the else
   branch's two calls and ArcTan2 stay direct bl to the three veneers, as
   listed. What remains near them: the ldr's register (r3 for r1) at two
   calls, and the velocity stores at 36 and 44 scheduled after their
   loads (MulQ16's asm made volatile to hold them: 131). 161 to 146 (2026-10-06, session 4): the
   command skip loop as goto test; do { cursor++; test: n = cursor; cmd =
   word[n]; } while (cmd > 63), with s16 n: the listing's b to the test,
   no copied exit test, obj->script reloaded after each strh of the
   cursor, ldrsh r2 before ldr r3 and cmd in r3, all as listed. The
   same goto form with the index inline: 162; cmd reused for the index:
   152; the index unextended: 315; for (;;) with n and a break, while
   with n in a comma expression, or for (; test; cursor++): 161 to 162,
   each with the exit test copied above the loop and obj->script
   hoisted. 195 to 172 (2026-10-06, session 2): the
   smull's constraints as two alternatives, the high word tied to the
   second input or early-clobbered ("=&r,&r"(lo), "=r,&r"(hi), "r,r"(a),
   "1,r"(b)), as Transform_UpdateVertices takes them. 172 to 161: case
   17's sign test as (y - target_y) ^ (obj->y - target_y), found by alchemy
   permute --camelot-arm (5 minutes, nothing else below 172); the same
   order in case 16: 169, in case 18: 171.
   Stock agscc --arm: listing 307, draft 294, common 55, differ 491.
   Fixed agbcc_arm -O2 library setting: 307/296/common202/differ199.
   Shared ScriptMotionObject layout and separate ratio/velocity values come
   from the exact Thumb counterpart; ARM still updates only eight objects
   and keeps its own horizontal motion, deceleration and frozen behavior.
   Natural source trials, agscc/agbcc differences: baseline621/517;
   separate ratio/vx/vz609/499; ordinary for loop608/491; ground local same.
   Explicit high/low product expression591/547 was reverted.
   2026-10-06 trials, agscc/agbcc: IwramSqrt and IwramRatioMulQ14 called
   by name (the listing's bl veneers and register calls) 596/480; command
   loop as do/while around a while 596/479, inner for form 597/480;
   smull into separately allocated low and high words 514/278 (the native
   never pairs them, as a 64-bit product would); low shift first in the
   OR 494/208; fixed-address calls in the target branch only 500/269,
   reverted; falling and bouncing as two tests of y against the ground,
   the second reloading velocity_y, 491/199; one command loop with the
   cursor step in an else 483/346, reverted. Remaining: the target branch
   calls IwramSqrt and IwramRatioMulQ14 through a register, the native
   reloads obj->script for each command and enters the cursor loop by a
   jump to its test, the arrival cases 16 and 18 share one tail, and the
   three ldr r4/bx r4 call veneers after the function (six instructions)
   have no source form. This draft does not identify its compiler. */
#include "TYPES.H"
#include "SCRIPT_MOTION.H"

#define TARGET_UNSET ((s32)0x80000000)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern struct ScriptMotionObject *gObjectSlots;
extern ScriptCommandFn ScriptObject_CommandTable[];

s32 ArcTan2(s32, s32);
s32 IwramSqrt(s32);
s32 IwramRatioMulQ14(s32, s32);

static __inline__ s32 MulQ16(s32 a, s32 b)
{
    s32 lo;
    s32 hi;

    /* The native smull writes its low and high words to unpaired registers
       (r1/r3, r2/r0); a 64-bit product takes a register pair. */
    /* RdHi tied to Rs or early-clobbered: camelot_arm 195 to 172. */
    /* FAKEMATCH: unpaired smull halves; agbcc_arm differ 479 as s64, 278 */
    __asm__("smull %0, %1, %2, %3" : "=&r,&r"(lo), "=r,&r"(hi) : "r,r"(a), "1,r"(b));
    return ((u32)lo >> 16) | (hi << 16);
}

void Object_UpdateAll(void)
{
    struct ScriptMotionObject *obj;
    s32 i;
    s32 x;
    s32 y;
    s32 z;
    s32 dx;
    s32 dz;
    s32 vx;
    s32 vz;
    s32 len;
    s32 ratio;
    s32 rem;
    s32 ground;
    s32 reached;
    u32 cmd;
    s32 (*sq)(s32);
    s32 (*rm)(s32, s32);
    s16 n;
    u16 angle;
    s32 turn;

    obj = gObjectSlots;
    for (i = 0; i < 8; i++, obj++) {
        if (obj->script == NULL)
            continue;
        if (obj->hook != NULL)
            obj->hook(obj);
        if (obj->script == NULL)
            continue;
        if (obj->paused != 0)
            continue;

        if (obj->wait != 0) {
            obj->wait--;
        } else {
            do {
                goto test;
                do {
                    obj->script_cursor++;
                test:
                    n = obj->script_cursor;
                    cmd = (u32)obj->script->word[n];
                } while (cmd > 63);
            } while (ScriptObject_CommandTable[cmd](obj) != 0);
            if (obj->script == NULL)
                continue;
        }

        x = obj->x;
        y = obj->y;
        z = obj->z;

        if (obj->frozen == 0) {
            reached = 0;
            if (obj->target_x != TARGET_UNSET) {
                sq = IwramSqrt;
                rm = IwramRatioMulQ14;
                dx = (obj->target_x - x) / 65536;
                dz = (obj->target_z - z) / 65536;
                len = sq(dx * dx + dz * dz) << 16;
                if (len < 0x01000000) {
                    dx = obj->target_x - x;
                    dz = obj->target_z - z;
                    len = sq(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
                }
                if (len == 0) {
                    x = obj->target_x;
                    z = obj->target_z;
                } else {
                    ratio = rm(len, obj->acceleration);
                    obj->velocity_x += MulQ16(ratio, dx);
                    obj->velocity_z += MulQ16(ratio, dz);
                    vx = obj->velocity_x;
                    vz = obj->velocity_z;
                    len = sq(MulQ16(vx, vx) + MulQ16(vz, vz)) << 8;
                    if (len > obj->speed_limit) {
                        ratio = rm(len, obj->speed_limit);
                        obj->velocity_x = MulQ16(ratio, vx);
                        obj->velocity_z = MulQ16(ratio, vz);
                    }
                }
            } else {
                dx = obj->velocity_x;
                dz = obj->velocity_z;
                len = IwramSqrt(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
                if (len != 0) {
                    rem = len - obj->acceleration;
                    if (rem < 0) {
                        obj->velocity_x = reached;
                        obj->velocity_z = reached;
                    } else {
                        ratio = IwramRatioMulQ14(len, rem);
                        obj->velocity_x = MulQ16(ratio, dx);
                        obj->velocity_z = MulQ16(ratio, dz);
                    }
                } else {
                    obj->velocity_x = reached;
                    obj->velocity_z = reached;
                }
            }

            if ((obj->motion_flags & 2) != 0) {
                ground = obj->terrain_height;
                if (y > ground)
                    obj->velocity_y -= obj->gravity;
                if (y <= ground && obj->velocity_y < 0) {
                    y = ground;
                    obj->velocity_y = -MulQ16(obj->vertical.bounce, obj->velocity_y);
                    if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y) <= obj->gravity)
                        obj->velocity_y = 0;
                }
            }
        }

        x += obj->velocity_x;
        y += obj->velocity_y;
        z += obj->velocity_z;

        switch (obj->arrival_axis) {
        case 16:
            if (x == obj->target_x ||
                ((obj->x - obj->target_x) ^ (x - obj->target_x)) < 0)
                reached = 1;
            break;
        case 17:
            if (y == obj->target_y ||
                ((y - obj->target_y) ^ (obj->y - obj->target_y)) < 0)
                reached = 1;
            break;
        case 18:
            if (z == obj->target_z ||
                ((obj->z - obj->target_z) ^ (z - obj->target_z)) < 0)
                reached = 1;
            break;
        }

        if (reached != 0) {
            if (obj->snap_to_target != 0) {
                x = obj->target_x;
                obj->velocity_x = 0;
                z = obj->target_z;
                obj->velocity_z = 0;
                if (obj->motion_flags == 0) {
                    y = obj->target_y;
                    obj->velocity_y = 0;
                }
            }
            obj->target_x = TARGET_UNSET;
            obj->target_y = TARGET_UNSET;
            obj->target_z = TARGET_UNSET;
            obj->arrival_axis = 0;
        }

        obj->x = x;
        obj->y = y;
        obj->z = z;

        if ((obj->steering_flags & 1) != 0) {
            x = obj->velocity_x;
            z = obj->velocity_z;
            if (x != 0 || z != 0) {
                angle = ArcTan2(z, x);
                turn = (s16)(angle - obj->facing);
                if (turn >= 0x1000)
                    turn = 0x1000;
                if (turn < -0x1000)
                    turn = -0x1000;
                obj->facing += turn;
            }
        }
    }
}
