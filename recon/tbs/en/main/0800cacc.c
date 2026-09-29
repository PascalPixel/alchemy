/*
 * Per-frame update pass over the 64 script objects held in the shared table
 * at 0x03001e64 (the same table ScriptObject_CheckOverlap walks).
 *
 * For each live entry the pass:
 *   1. runs the entry's optional hook at +0x6c,
 *   2. counts down the wait timer at +0x5e, or otherwise executes script
 *      commands out of the 64-entry dispatch table at 0x08013624 until one
 *      of them returns zero,
 *   3. accelerates the entry's velocity toward its target position -- a
 *      three-axis form when the mode byte at +0x55 is zero, a planar (x/z)
 *      form otherwise -- and clamps the resulting speed to +0x30,
 *   4. in the planar form only, applies the terrain / bounce / cycle-table
 *      effects selected by the flag bits of +0x55,
 *   5. integrates the position, optionally rejects the step when it would
 *      overlap another entry, notices when an axis target was passed, and
 *      finally turns the facing angle at +0x06 toward the new velocity.
 *
 * Uncertain, and left as evidenced offsets rather than invented names:
 *  - The reference keeps two extra byte cursors, obj+0x55 and obj+0x56, as
 *    independent induction variables alongside the object pointer. They are
 *    modelled here as the two byte pointers `ctl` and `act`. Between them
 *    they reach obj+0x55, +0x58, +0x59, +0x5a, +0x5b, +0x5e, +0x60 and +0x61,
 *    which straddle several members of struct ScriptObjectRuntime, so they
 *    are spelled as indices off the two cursors rather than as members. Two
 *    of those bytes disagree with the shared header and are reported rather
 *    than fixed here, because this worker may not edit the header:
 *      * the header names obj+0x60 a `void *callback`, but this owner uses
 *        +0x60 as a byte counter (incremented while a step is refused) and
 *        +0x61 as an independent byte guard;
 *      * the header leaves obj+0x6c as unknown_6c, but this owner calls it
 *        as a function pointer taking the object.
 *  - The words at obj+0x44 and obj+0x48 also fall inside an unknown range;
 *    +0x44 doubles as a phase counter for the 0x080131c0 cycle table and
 *    +0x48 as a gravity/rebound magnitude. Reached with FIELD_AT.
 *  - The three fixed IWRAM entries are typed indirect calls, per the project
 *    rule for a `bx rN` call-via slot. 0x030001d8 takes a sum of squares and
 *    returns a length; 0x0300013c returns a scale factor that maps its first
 *    argument onto its second; 0x03000118 is a fixed-point multiply. Their
 *    exact fixed-point conventions are NOT established here.
 *  - Every one of the 31 "align / mov ip, pc / bx rN" inline-call sites in
 *    the reference calls 0x03000118, and only 0x03000118. IWRAM_CALL.H now
 *    supplies that shared typed machine interface. The 0x030001d8 and
 *    0x0300013c calls all go through the ordinary __call_via_rN veneers at
 *    0x080072ec / 0x080072f0 / 0x08007310, exactly as this draft emits them,
 *    and so do the +0x6c hook and the dispatch-table handler.
 *  - Func_08011f54 reads a ground height for a terrain id at a planar
 *    position; it has no project name yet.
 *  - ScriptObject_CheckOverlap is declared here over struct
 *    ScriptObjectRuntime. Its own unit declares it over struct
 *    ScriptObjectEntry, a second view of the same 0x70 record. Converging
 *    the two views is the integrator's call, not this owner's.
 *
 * Every reference branch, loop, switch, call and store is represented and
 * the block order agrees. The remaining differing halfwords are chiefly
 * allocation: the reference keeps the three delta components
 * in r8/fp/r9 and spills both byte cursors for a 48-byte frame, while this
 * previous draft used 44 and let GCC rematerialise `act` from the object
 * pointer. These are unresolved C lifetime/alias questions, not evidence
 * of hand-written assembly or a limitation of ordinary C.
 *
 * 2026-09-26: complete owner [0800cacc,0800d130), 1636 bytes including
 * three own pools. ObjectSystem_Initialize schedules it at priority c8a.
 * Audited all raw blocks, the exact overlap callee, geometry helpers and
 * movement neighbour Object_SetMoveTarget. Fresh baseline is 1576 bytes,
 * 800 differing halfwords / 586 aligned edits, different topology; full
 * normalized difference read. The dispatch loop has a different back edge,
 * and allocator coalescing merges the three-axis clamp and braking tails.
 * H1 transfers the neighbour's named multiply symbol and gives dispatch,
 * cycle and object tables independent typed extern ownership. Predict
 * indexed table loads and independently materialized multiply/divide
 * addresses. Keep local lifetimes and control flow unchanged. One score,
 * full normalized difference, then record and commit; at most three
 * structural hypotheses / 25 minutes. Exact full bytes and compare-all,
 * coverage, tests and verify are required for adoption.
 * H1 result: 1568 bytes, 800 differing halfwords / 586 aligned edits,
 * topology still different. Full normalized difference read. Indexed
 * table loads recovered; no improvement in the broad allocation mismatch.
 * Frame remains 36 rather than 48, and the clamp/braking tails still merge.
 * The multiply-symbol change does not resolve those independent lifetimes.
 * H2 transfers the exact Object_UpdateAllMotion sibling's scalar lifetime
 * pattern: delta locals become braking velocities and position locals
 * become facing inputs. The raw owner also keeps Z in r9 across the
 * delta/velocity phases and saves the ground-probe inputs in sl/r9.
 * Predict distinct three-axis clamp/braking tails and retained high-register
 * delta ownership, plus facing reloads from the position stack slots.
 * Change only those lifetimes, not declaration order or dispatch flow;
 * one score and full difference, then commit before the 23:30 checkpoint.
 * H2 result: 1624 bytes, 792 differing halfwords / 570 aligned edits.
 * Full normalized difference read. The three-axis clamp/braking tails
 * are now separate, and facing reads the position spill slots as predicted.
 * Frame remains 36, act is rematerialized, and deltas stay in low registers.
 * The exact sibling expresses its coarse deltas as signed division by
 * 65536; this draft still manually expands that operation into branches.
 * That is a separate lowering hypothesis, not a declaration permutation.
 * H3 restores those signed coarse-coordinate divisions from the exact
 * movement sibling. Predict short-lived low-register quotient temporaries
 * followed by r8/fp/r9 delta ownership, rather than holding each manually
 * expanded conditional accumulator in a low register. Leave all other
 * arithmetic and flow unchanged. One trial, then stop this owner's batch
 * at three hypotheses and preserve the complete result by 23:30.
 * H3 result: 1596 bytes, 795 differing halfwords / 529 aligned edits.
 * Full normalized difference and the shared clamp tail inspected. Signed
 * division restores the r0 quotient temporary and X in r8, but Y is r9
 * instead of fp and Z remains r7. Two clamp stores share the braking tail
 * again. The topology reader reports uncovered candidate target 0x1ee;
 * the compiler listing identifies that jump as the shared .L121 tail.
 * No CFG-equivalence or exactness claim. Remaining first-order questions
 * are dispatch back-edge ownership, the independent act/motion cursors,
 * and the 36/48-byte stack model. Three trials complete; stop this batch.
 * H1 is 4c8a6b29d, H2 is a440a0f1e. No new credited bytes.
 */

#include "TYPES.H"
#include "GLOBAL_CELLS.H"
#include "IWRAM_CALL.H"
#include "SCRIPT_OBJECT_RUNTIME.H"

#define FIELD_AT(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

/* An axis target holds this sentinel while no target is set. */
#define TARGET_UNSET ((s32)0x80000000)

/* Fixed IWRAM helper entries reached through call-via / ip-link slots. */
#define IwramLength ((s32 (*)(s32))0x030001d8)
#define IwramRatio ((s32 (*)(s32, s32))0x0300013c)

/* The 64-entry script command dispatch table and the cycle table used by the
 * flag-8/flag-4 vertical motion. */
typedef s32 (*ScriptCommandFn)(struct ScriptObjectRuntime *);
typedef void (*ScriptObjectHook)(struct ScriptObjectRuntime *);

extern ScriptCommandFn Data_08013624[];
extern const s32 Data_080131c0[];
extern struct ScriptObjectRuntime *gObjectSlots;
/* FAKEMATCH: preserve independent multiply/divide address ownership, as
 * in the exact Object_SetMoveTarget neighbour. */
#define MulQ16(left, right) Iwram_Call2((left), (right), IwramMulQ16ReturnIp)

s32 Func_08011f54(s32, s32, s32);
s32 ScriptObject_CheckOverlap(struct ScriptObjectRuntime *, s32 *);
s32 FixedSqrt(s32);
s32 ArcTan2(s32, s32);

void Object_UpdateAllThumb(void)
{
    struct ScriptObjectRuntime *obj;
    const s32 *script;
    ScriptObjectHook hook;
    ScriptCommandFn *tbl;
    u8 *ctl;
    u8 *act;
    s32 cnt;
    s32 reached;
    s32 pos[3];
    s32 px;
    s32 py;
    s32 pz;
    s32 dx;
    s32 dy;
    s32 dz;
    s32 vx;
    s32 vy;
    s32 dist;
    s32 len;
    s32 ratio;
    s32 rem;
    s32 ground;
    s32 step;
    s32 half;
    s32 work;
    u32 cmd;
    u32 phase;

    obj = gObjectSlots;
    ctl = &obj->flags;
    act = &obj->unknown_56[0];

    for (cnt = 63; cnt >= 0;
         cnt--, obj++, ctl += 0x70, act += 0x70) {
        reached = 0;

        script = obj->script;
        if (script == NULL)
            continue;

        hook = FIELD_AT(obj, ScriptObjectHook, 0x6c);
        if (hook != NULL) {
            hook(obj);
            script = obj->script;
        }
        if (script == NULL)
            continue;
        if (ctl[6] != 0)
            continue;

        if (*(s16 *)(act + 8) != 0) {
            (*(u16 *)(act + 8))--;
        } else {
            tbl = Data_08013624;
            for (;;) {
                cmd = (u32)script[(s16)obj->script_cursor];
                if (cmd > 63) {
                    obj->script_cursor = obj->script_cursor + 1;
                    script = obj->script;
                    continue;
                }
                if (tbl[cmd](obj) == 0)
                    break;
                script = obj->script;
            }
            if (obj->script == NULL)
                continue;
        }

        px = obj->x;
        py = obj->y;
        pz = obj->z;

        if (ctl[12] == 0) {
            if (ctl[0] == 0) {
                /* Three-axis approach. */
                if (obj->target_x != TARGET_UNSET) {
                    dx = (obj->target_x - px) / 65536;
                    dy = (obj->target_y - py) / 65536;
                    dz = (obj->target_z - pz) / 65536;
                    dist = IwramLength(dx * dx + dy * dy + dz * dz);
                    if (dist == 0) {
                        px = obj->target_x;
                        py = obj->target_y;
                        pz = obj->target_z;
                    } else {
                        ratio = IwramRatio(dist << 16, obj->acceleration);
                        vx = obj->velocity_x + dx * ratio;
                        vy = obj->velocity_y + dy * ratio;
                        dz = obj->velocity_z + dz * ratio;
                        obj->velocity_y = vy;
                        obj->velocity_x = vx;
                        obj->velocity_z = dz;
                        len = FixedSqrt(MulQ16(vx, vx) + MulQ16(vy, vy) +
                                        MulQ16(dz, dz));
                        if (len > obj->speed_limit) {
                            ratio = IwramRatio(len, obj->speed_limit);
                            obj->velocity_x = MulQ16(vx, ratio);
                            obj->velocity_y = MulQ16(vy, ratio);
                            obj->velocity_z = MulQ16(dz, ratio);
                        }
                    }
                } else {
                    dx = obj->velocity_x;
                    dz = obj->velocity_z;
                    dy = obj->velocity_y;
                    len = FixedSqrt(MulQ16(dx, dx) + MulQ16(dy, dy) +
                                    MulQ16(dz, dz));
                    if (len != 0) {
                        rem = len - obj->acceleration;
                        if (rem < 0)
                            rem = 0;
                        ratio = IwramRatio(len, rem);
                        obj->velocity_x = MulQ16(dx, ratio);
                        obj->velocity_y = MulQ16(dy, ratio);
                        obj->velocity_z = MulQ16(dz, ratio);
                    } else {
                        obj->velocity_x = 0;
                        obj->velocity_y = 0;
                        obj->velocity_z = 0;
                    }
                }
            } else {
                /* Planar approach: x and z only. */
                if (obj->target_x != TARGET_UNSET) {
                    dx = (obj->target_x - px) / 65536;
                    dz = (obj->target_z - pz) / 65536;
                    /* The coarse cell length is enough while the object is
                     * far away; near the target the full-precision length is
                     * recomputed instead. */
                    dist = IwramLength(dx * dx + dz * dz) << 16;
                    if (dist <= 0x00ffffff) {
                        dx = obj->target_x - px;
                        dz = obj->target_z - pz;
                        dist = FixedSqrt(MulQ16(dx, dx) + MulQ16(dz, dz));
                    }
                    if (dist == 0) {
                        px = obj->target_x;
                        pz = obj->target_z;
                    } else {
                        ratio = IwramRatio(dist, obj->acceleration);
                        vx = obj->velocity_x + MulQ16(dx, ratio);
                        obj->velocity_x = vx;
                        dz = obj->velocity_z + MulQ16(dz, ratio);
                        obj->velocity_z = dz;
                        len = FixedSqrt(MulQ16(vx, vx) + MulQ16(dz, dz));
                        if (len > obj->speed_limit) {
                            ratio = IwramRatio(len, obj->speed_limit);
                            obj->velocity_x = MulQ16(vx, ratio);
                            obj->velocity_z = MulQ16(dz, ratio);
                        }
                    }
                } else {
                    dx = obj->velocity_x;
                    dz = obj->velocity_z;
                    len = FixedSqrt(MulQ16(dx, dx) + MulQ16(dz, dz));
                    if (len != 0) {
                        rem = len - obj->acceleration;
                        if (rem < 0)
                            rem = 0;
                        ratio = IwramRatio(len, rem);
                        obj->velocity_x = MulQ16(dx, ratio);
                        obj->velocity_z = MulQ16(dz, ratio);
                    } else {
                        obj->velocity_x = 0;
                        obj->velocity_z = 0;
                    }
                }

                /* Flag 0x01: follow the ground under the next planar step and
                 * bleed off speed proportional to the slope climbed. */
                if ((ctl[0] & 1) != 0) {
                    vx = px + obj->velocity_x;
                    dz = pz + obj->velocity_z;
                    ground = Func_08011f54((s32)obj->terrain_id, vx, dz);
                    step = ground - obj->terrain_height;
                    if (ground - py > -0x40000)
                        py += step;
                    if (step < 0)
                        step = -step;
                    half = obj->acceleration / 2;
                    if (step > half)
                        step = half;
                    step = step * 3;
                    if (step != 0 && (ctl[0] & 0x10) == 0) {
                        dx = obj->velocity_x;
                        dy = obj->velocity_y;
                        dz = obj->velocity_z;
                        len = FixedSqrt(MulQ16(dx, dx) + MulQ16(dy, dy) +
                                        MulQ16(dz, dz));
                        if (len != 0) {
                            rem = len - step;
                            if (rem < 0)
                                rem = 0;
                            ratio = IwramRatio(len, rem);
                            obj->velocity_x = MulQ16(dx, ratio);
                            obj->velocity_y = MulQ16(dy, ratio);
                            obj->velocity_z = MulQ16(dz, ratio);
                        }
                    }
                    obj->terrain_height = ground;
                }

                /* Flag 0x02: fall toward the stored ground height, then
                 * rebound with the damping factor at +0x44. */
                if ((ctl[0] & 2) != 0) {
                    ground = obj->terrain_height;
                    if (py > ground) {
                        obj->velocity_y =
                            obj->velocity_y - FIELD_AT(obj, s32, 0x48);
                    } else if (obj->velocity_y < 0) {
                        py = ground;
                        work = MulQ16(obj->velocity_y,
                                        FIELD_AT(obj, s32, 0x44));
                        rem = -work;
                        obj->velocity_y = rem;
                        if (rem < 0)
                            rem = -rem;
                        if (rem <= FIELD_AT(obj, s32, 0x48))
                            obj->velocity_y = 0;
                    }
                }

                /* Flag 0x04: drive the vertical velocity straight off the
                 * cycle table instead, at one of two scales. */
                if ((ctl[0] & 4) != 0) {
                    phase = (u32)(FIELD_AT(obj, s32, 0x44) & 0x3f);
                    if ((ctl[0] & 8) != 0) {
                        work = FIELD_AT(obj, s32, 0x48) *
                               Data_080131c0[phase >> 1];
                        if (work < 0)
                            work += 15;
                        work >>= 4;
                    } else {
                        work = FIELD_AT(obj, s32, 0x48) *
                               Data_080131c0[phase >> 1];
                        if (work < 0)
                            work += 63;
                        work >>= 6;
                    }
                    obj->velocity_y = work;
                    FIELD_AT(obj, s32, 0x44) = FIELD_AT(obj, s32, 0x44) + 1;
                }
            }
        }

        px += obj->velocity_x;
        py += obj->velocity_y;
        pz += obj->velocity_z;

        /* Flag 0x80 at +0x59: refuse a step that would overlap another
         * entry, and count how many frames in a row it was refused. */
        if ((ctl[4] & 0x80) != 0) {
            pos[0] = px;
            pos[1] = py;
            pos[2] = pz;
            if (ScriptObject_CheckOverlap(obj, pos) != 0) {
                ctl[11] = (u8)(ctl[11] + 1);
                continue;
            }
            ctl[11] = 0;
        }

        /* Action kinds 16/17/18 watch one axis and report the frame in which
         * the target is reached or passed. */
        switch (act[0]) {
        case 16:
            if (px == obj->target_x ||
                ((obj->x - obj->target_x) ^ (px - obj->target_x)) < 0)
                reached = 1;
            break;
        case 17:
            if (py == obj->target_y ||
                ((obj->y - obj->target_y) ^ (py - obj->target_y)) < 0)
                reached = 1;
            break;
        case 18:
            if (pz == obj->target_z ||
                ((obj->z - obj->target_z) ^ (pz - obj->target_z)) < 0)
                reached = 1;
            break;
        default:
            break;
        }

        if (reached != 0) {
            if (ctl[3] != 0) {
                px = obj->target_x;
                pz = obj->target_z;
                obj->velocity_x = 0;
                obj->velocity_z = 0;
                if (ctl[0] == 0) {
                    py = obj->target_y;
                    obj->velocity_y = 0;
                }
            }
            obj->target_x = TARGET_UNSET;
            obj->target_y = TARGET_UNSET;
            obj->target_z = TARGET_UNSET;
            act[0] = 0;
        }

        obj->x = px;
        obj->y = py;
        obj->z = pz;

        /* Flag 0x01 at +0x5a: steer the facing angle toward the direction of
         * travel, at most 0x1000 of a 0x10000 turn per frame. */
        if ((ctl[5] & 1) != 0) {
            px = obj->velocity_x;
            pz = obj->velocity_z;
            if (px != 0 || pz != 0) {
                work = (s16)(ArcTan2(pz, px) - obj->script_value);
                if (work > 0x1000)
                    work = 0x1000;
                if (work < -0x1000)
                    work = -0x1000;
                obj->script_value = obj->script_value + work;
            }
        }
    }
}
