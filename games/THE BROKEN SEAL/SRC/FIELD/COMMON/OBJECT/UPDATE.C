#include "TYPES.H"
#include "SCRIPT_MOTION.H"

#define TARGET_UNSET ((s32)0x80000000)

typedef s32 (*ScriptCommandFn)(struct ScriptMotionObject *);

extern struct ScriptMotionObject *gObjectSlots;
extern ScriptCommandFn ScriptObject_CommandTable[];

s32 IwramSqrt(s32);
s32 IwramRatioMulQ14(s32, s32);
s32 ArcTan2FarArm(s32, s32);
s32 IwramSqrtFarArm(s32);
s32 IwramRatioMulQ14FarArm(s32, s32);

static __inline__ s32 MulQ16(s32 a, s32 b)
{
    s32 lo;
    s32 hi;

    /* FAKEMATCH: unpaired smull halves (s64 479, asm 278); volatile keeps stores before the next smull (107 to 24) */
    __asm__ __volatile__("smull %0, %1, %2, %3" : "=r"(lo), "=r"(hi) : "r"(a), "r"(b));
    return ((u32)lo >> 16) | (hi << 16);
}

static __inline__ s32 MulQ16R3(s32 a, s32 b)
{
    /* FAKEMATCH: bounce in r3, not folded into the smull as a memory input (r1; 20 to 12 with lo and hi below) */
    register s32 ra __asm__("r3") = a;
    /* FAKEMATCH: bounce smull low half r2 (r3 without it) */
    register s32 lo __asm__("r2");
    /* FAKEMATCH: bounce smull high half r3 (r1 without it) */
    register s32 hi __asm__("r3");

    /* FAKEMATCH: unpaired smull halves, as MulQ16 */
    __asm__ __volatile__("smull %0, %1, %2, %3" : "=r"(lo), "=r"(hi) : "r"(ra), "r"(b));
    return ((u32)lo >> 16) | (hi << 16);
}

static __inline__ s32 MulQ16R1(s32 a, s32 b)
{
    /* FAKEMATCH: acceleration smull low half r1, as when sched1 hoists the velocity load over it (r2 behind the barrier; 28 to 20) */
    register s32 lo __asm__("r1");
    s32 hi;

    /* FAKEMATCH: unpaired smull halves, as MulQ16 */
    __asm__ __volatile__("smull %0, %1, %2, %3" : "=r"(lo), "=r"(hi) : "r"(a), "r"(b));
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
    s32 w;
    /* FAKEMATCH: z's asm load set once (birthing in sched1), in sl (43 unfixed: r0, then mov sl, r0) */
    register s32 z0 __asm__("sl");
    u32 f;

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

        /* FAKEMATCH: ldrh after wait's cmp, not before it (12 to 8): stock sched charges a compiled load 2 on its anti dependence to the branch, an asm 1 */
        __asm__("ldrh %0, %1" : "=r"(w) : "m"(obj->wait));
        if (obj->wait != 0) {
            obj->wait = w - 1;
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
        /* FAKEMATCH: frozen through an asm (cost 1) drops the cmp's sched2 priority from 4 to z's 2; its memory clobber keeps x and y ahead in sched1 (100 compiled or unclobbered) */
        __asm__("ldrb %0, %1" : "=r"(f) : "m"(obj->frozen) : "memory");
        /* FAKEMATCH: z loaded after the frozen test's cmp, not before it (8 to 6): an asm set once, copied into z in both arms */
        __asm__("ldr %0, %1" : "=r"(z0) : "m"(obj->z));
        if (f == 0) {
            z = z0;
            reached = 0;
            if (obj->target_x != TARGET_UNSET) {
                sq = IwramSqrt;
                rm = IwramRatioMulQ14;
#ifdef MODERN
                // this code runs copied into RAM: modern GCC would call the
                // routines directly, out of reach, so keep the addresses
                __asm__("" : "+r"(sq), "+r"(rm));
#endif
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
                    vx = MulQ16R1(ratio, dx);
                    /* FAKEMATCH: velocity load after the orr, not hoisted above it (24 to 28 alone; an empty barrier 40) */
                    __asm__ __volatile__("" : : "r"(vx));
                    obj->velocity_x += vx;
                    vz = MulQ16R1(ratio, dz);
                    /* FAKEMATCH: velocity load after the orr, as above */
                    __asm__ __volatile__("" : : "r"(vz));
                    obj->velocity_z += vz;
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
                len = IwramSqrtFarArm(MulQ16(dx, dx) + MulQ16(dz, dz)) << 8;
                if (len != 0) {
                    rem = len - obj->acceleration;
                    if (rem < 0) {
                        obj->velocity_x = reached;
                        obj->velocity_z = reached;
                    } else {
                        ratio = IwramRatioMulQ14FarArm(len, rem);
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
                    obj->velocity_y = -MulQ16R3(obj->vertical.bounce, obj->velocity_y);
                    if ((obj->velocity_y < 0 ? -obj->velocity_y : obj->velocity_y) <= obj->gravity)
                        obj->velocity_y = 0;
                }
            }
        } else {
            z = z0;
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
            if (x != 0 || z != 0) {
                angle = ArcTan2FarArm(z, x);
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
