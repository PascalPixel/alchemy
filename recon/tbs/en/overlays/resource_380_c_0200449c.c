/* NONMATCHING: 748-byte candidate, 358 differing halfwords, 205 edits.
 * Whole owner 752 bytes including six owned pool words.
 * Canonical draft for resource_380:0200449c and resource_381:0200301c;
 * own-ROM sibling check proves equivalent flow and per-instance bindings.
 * Both remain not-yet-c. Consolidate the two draft units into one instanced
 * unit only after an exact score (the registry requires exact instances).
 *
 * 2026-09-26 baseline: 744 bytes, 356 differing halfwords, 228 edits.
 * Read the complete normalized diff: frame 68 matches, but work/current
 * entry/index and the derived entry+8 pointer have different lifetimes.
 * Reference keeps angles, scale, speed, timer and position snapshots on
 * stack; hold survives in fp and the third random sample survives in r8.
 * The second divided random sample also gates BOTH y-coordinate stores.
 *
 * H1: separate advancing record cursor from indexed animation state.
 * Result 772 bytes, 366 differing halfwords, 239 edits; frame grows to 76.
 * Cursor spills as predicted, but +16 animation and +8 position induction
 * pointers remain separate and an unwanted index*4 induction appears.
 * This is a negative structural result, not a register-spelling target.
 * H2: restore indexed baseline and express the three IWRAM unsigned divides
 * as ordinary C arithmetic, bound through __udivsi3. Own division entry
 * identifies 0x030003f0 as IwramUnsignedDivide. The entire 744-byte candidate
 * is identical to baseline, so helper-call versus division is not the cause.
 * Initializer 380:0200478c confirms ten 40-byte entries, count at +0x190,
 * scale/negative speed at +28/+32, timer=3 at +36, callback priority 0xc80.
 * H3: per-record snapshot scope, sample temporaries scoped to the active
 * phase, timer declared before position and loaded before decrement.
 * Result 748 bytes, 354 differing halfwords, 232 edits, frame still 68.
 * Snapshot slots move, but scale still owns fp, hold sl, entry r8; the
 * third sample still spills around division. Scope alone does not recover
 * the reference's state ownership. Three hypotheses complete: STOP here.
 * H1 and H2 are preserved in preceding commits; H2 is the 228-edit baseline.
 * Legacy 2026-09-24 pointer/index/loop-test/operand-order sweeps exhausted
 * 226..249 edits; do not repeat those without new ownership evidence.
 * H4 (2026-09-27): exact RING_SETUP binds the object through ActorGet,
 * and ERUPTION_RING uses FieldActor for the same position/scale/target
 * fields. Replace the private object view with that existing shared type:
 * +56/+60/+64 are target coordinates, not draw coordinates. The full
 * 748-byte result is byte-identical to H3 (354 halfwords/232 edits).
 * FieldCoordinate union aliasing does not change the snapshot allocation;
 * stop the actor-type-only axis. Keep the proven ownership correction.
 * 2026-09-27 Sol spark-ring H5 typed suffix alone: 748 bytes, 354 halfwords,
 * 233 edits. The -dL dump shows initial loads recombined through the whole
 * record while writeback uses the +8 suffix. H5 fails ownership admission.
 * H6 advances whole record and typed suffix independently: 748 bytes,
 * 359 halfwords / 209 edits, frame 68. Whole record now spills at sp+60;
 * angle snapshots match sp+52/+48/+44 and load through the +8 suffix.
 * Scale remains fp, hold sl, suffix r8, third random sample still spills.
 * Not admitted as exact: remaining invariant is suffix sl / hold fp /
 * third sample r8, with scale spilled at sp+40. Preserved as diagnostic
 * draft before a separate scale-storage hypothesis.
 * H7 scalar-record scale snapshot: byte-identical to H6 (748 bytes / 359
 * halfwords / 209 edits); GCC scalarizes the record into the same fp value.
 * Same residual through resource_381 bindings with --symbol 0200449c.
 * STOP: suffix/cursor/scalar-storage axis bounded without an admitted
 * ownership shape. H6 remains diagnostic C not yet exact; no new DONE.
 * H8 follows the reference's initial whole-record y/z consumers and final
 * whole-record z writeback; angles/scale still use the independent +8 view.
 * Result 748 bytes / 358 halfwords / 205 edits, frame 68. The +8 cursor now
 * owns sl as in reference, but scale remains fp, hold r8, and position slots
 * remain displaced. Partial ownership witness, not an admitted exact shape.
 * Transferred DRIFT.C 74a903e43 has no matching random variant or callback
 * publication consumers here; those repairs were not applied.
 * H9 addressable nine-slot snapshot array: 796 bytes / 384 halfwords /
 * 331 edits. Memory residence is confirmed, but it requires an aggregate
 * base register and grows the frame from 68 to 80; the stack-slot gate fails.
 * Rejected that storage model; H8 is the canonical diagnostic draft.
 * STOP: tested consumer/cursor/scalar-record/array axes bounded. Next work
 * needs new structural evidence for scale spill / hold fp / third sample r8,
 * not allocation respellings. No new exact function or alignment bytes. */
#include "FIELD_EVENT.H"

void *Runtime_AllocateBlock(s32 id, s32 size);

struct SparkMotion {
    s32 y;
    s32 z;
    s32 angle_x;
    s32 angle_y;
    s32 angle_z;
    s32 scale;
    s32 scale_speed;
};

struct Spark {
    struct FieldActor *obj;
    s32 x;
    struct SparkMotion motion;
    u8 timer;
    u8 hold;
};

struct SparkWork {
    struct Spark spark[10];
    u16 count;
};

extern u8 gSparkJitter[][3];
extern u8 gSparkFrequency[][3];
extern s8 gSparkDirection[][3];
extern s32 gSparkMaxScale[];
extern s32 gSparkScaleStep[];

void Effect_UpdateSparkRing(void)
{
    struct SparkWork *work;
    struct Spark *spark;
    struct SparkMotion *motion;
    s32 i;

    work = Runtime_AllocateBlock(33, 0x194);
    spark = work->spark;
    motion = &spark->motion;
    for (i = 0; i != work->count; i++) {
        struct FieldActor *obj;
        s32 ax;
        s32 ay;
        s32 az;
        s32 scale;
        s32 speed;
        u8 timer;
        s32 x;
        s32 y;
        s32 z;
        u8 hold;

        obj = spark->obj;
        ax = motion->angle_x;
        ay = motion->angle_y;
        az = motion->angle_z;
        scale = motion->scale;
        speed = motion->scale_speed;
        x = spark->x;
        y = spark->motion.y;
        z = spark->motion.z;
        hold = spark->hold;
        timer = spark->timer;
        timer--;
        if (timer == 0) {
            u32 rx;
            u32 ry;
            u32 rz;
            s32 dx;
            s32 dy;
            s32 dz;

            timer = 3;
            if (hold == 0) {
                scale += speed;
                if (scale >= gSparkMaxScale[i]) {
                    speed = -gSparkScaleStep[i];
                } else if (scale <= 0x1999) {
                    scale = 0x1999;
                    speed = gSparkScaleStep[i];
                    x = obj->x.fixed;
                    y = obj->y.fixed;
                    z = obj->z.fixed;
                    obj->x.fixed = 0;
                    obj->y.fixed = 0;
                    obj->z.fixed = 0;
                    hold = 24;
                }
                obj->scale_x = scale;
                obj->scale_y = scale;
            }
            rx = (u32)(gSparkJitter[i][0] * Engine_RandomNext()) >> 16;
            ry = (u32)(gSparkJitter[i][1] * Engine_RandomNext()) >> 16;
            rz = (u32)(gSparkJitter[i][2] * Engine_RandomNext()) >> 16;
            if (rx != 0)
                dx = (rx << 16) / 1000;
            else
                dx = 0;
            if (ry != 0)
                dy = (ry << 16) / 1000;
            else
                dy = 0;
            if (rz != 0)
                dz = (rz << 16) / 1000;
            else
                dz = 0;
            if (gSparkDirection[i][0] == 1) {
                ax += dx;
            } else {
                ax -= dx;
                if (gSparkDirection[i][0] != -1)
                    ax = 0;
            }
            if (gSparkDirection[i][1] == 1) {
                ay += dy;
            } else {
                ay -= dy;
                if (gSparkDirection[i][1] != -1)
                    ay = 0;
            }
            if (gSparkDirection[i][2] == 1) {
                az += dz;
            } else {
                az -= dz;
                if (gSparkDirection[i][2] != -1)
                    az = 0;
            }
            rx = Engine_MathSin(ax * gSparkFrequency[i][0]) << 1;
            ry = Engine_MathSin(ay * gSparkFrequency[i][1]) << 1;
            rz = Engine_MathCos(az * gSparkFrequency[i][2]) << 1;
            if (hold != 0) {
                x += rx;
                hold--;
                y += ry;
                z += rz;
                if (hold == 0) {
                    obj->x.fixed = x;
                    obj->target_x = x;
                    if (dy != 0) {
                        obj->y.fixed = y;
                        obj->target_y = y;
                    }
                    obj->z.fixed = z;
                    obj->target_z = z;
                }
            } else {
                obj->x.fixed += rx;
                obj->target_x = obj->x.fixed;
                if (dy != 0) {
                    obj->y.fixed += ry;
                    obj->target_y = obj->y.fixed;
                }
                obj->z.fixed += rz;
                obj->target_z = obj->z.fixed;
            }
        }
        motion->angle_x = ax;
        motion->angle_y = ay;
        motion->angle_z = az;
        motion->scale = scale;
        motion->scale_speed = speed;
        spark->hold = hold;
        spark->x = x;
        motion->y = y;
        spark->motion.z = z;
        spark->timer = timer;
        spark++;
        motion = (struct SparkMotion *)((u8 *)motion + sizeof(*spark));
    }
}
