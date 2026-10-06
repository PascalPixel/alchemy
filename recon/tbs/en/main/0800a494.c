/* 0800a494: ARM draft for the camelot_arm route (CAMELOT_ARM_SOURCES, K1).
   alchemy diff --camelot-arm: listing 307, draft 296, common 204, differ
   195 (stock agbcc_arm 199).
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
    /* FAKEMATCH: unpaired smull halves; agbcc_arm differ 479 as s64, 278 */
    __asm__("smull %0, %1, %2, %3" : "=&r"(lo), "=&r"(hi) : "r"(a), "r"(b));
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
                while ((cmd = (u32)obj->script->word[(s16)obj->script_cursor]) > 63)
                    obj->script_cursor++;
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
                dx = (obj->target_x - x) / 65536;
                dz = (obj->target_z - z) / 65536;
                len = IwramSqrt(dx * dx + dz * dz) << 16;
                if (len < 0x01000000) {
                    dx = obj->target_x - x;
                    dz = obj->target_z - z;
                    len = IwramSqrt(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
                }
                if (len == 0) {
                    x = obj->target_x;
                    z = obj->target_z;
                } else {
                    ratio = IwramRatioMulQ14(len, obj->acceleration);
                    obj->velocity_x += MulQ16(ratio, dx);
                    obj->velocity_z += MulQ16(ratio, dz);
                    vx = obj->velocity_x;
                    vz = obj->velocity_z;
                    len = IwramSqrt(MulQ16(vx, vx) + MulQ16(vz, vz)) << 8;
                    if (len > obj->speed_limit) {
                        ratio = IwramRatioMulQ14(len, obj->speed_limit);
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
                ((obj->y - obj->target_y) ^ (y - obj->target_y)) < 0)
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
            if (z != 0 || x != 0) {
                dx = (s16)((u16)ArcTan2(z, x) - obj->facing);
                if (dx >= 0x1000)
                    dx = 0x1000;
                if (dx < -0x1000)
                    dx = -0x1000;
                obj->facing += dx;
            }
        }
    }
}
