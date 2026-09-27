/* NONMATCHING: 556 bytes, candidate 558, 244 differing halfwords, 42 aligned
 * edits (2026-09-27). Reconstructed both Q16 waves and all four actor-height
 * blocks. The complete topology now agrees. Remaining: halfword scroll
 * reload and stack-slot ownership, saved position pointer, and spawn stores.
 * Corrected BLDALPHA and all six neighbouring base-height addresses from
 * the raw listing's literal words. An addressable scroll array restores the
 * +18 slot and its address scheduling. Reusing bob for the reload keeps
 * an unwanted r6 copy; position-pointer and x lifetimes still differ.
 * H1 transfers the shared FixedPointPosition type and a typed inline spawn
 * consumer around the second random draw. This fixes the ObjectCreate type
 * load order (43 to 42 edits), but x still spills at +0 while the position
 * remains r6, opposite the ROM. The scroll r6 copy, Q16 alignment padding,
 * position load/store/call setup and resulting pool offsets remain.
 * Correct BLDALPHA/base addresses and all spawn stores are unchanged.
 * H2 passes the addressable PositionWork record to that consumer instead
 * of passing only the position pointer: same full normalized diff and score.
 * STOP: the bounded typed spawn-consumer interface did not restore the saved
 * pointer lifetime. H1 is preserved at 148eb9259; no register-only sweeps.
 * Earlier structural attempts are preserved in draft commits. No DONE. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"
#include "FIXED_POINT_POSITION.H"

struct MapLayer {
    s32 unknown_00[3];
    s32 y;
    s32 unknown_10[8];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

struct Half {
    u16 v;
};

struct PositionWork {
    struct FixedPointPosition *pos;
};

extern struct MapWork *Data_03001e70;
extern s32 Data_020097e8;
extern s32 Data_020097ec;
extern s32 Data_020097f0;
extern s32 Data_020097f8;
extern s32 Data_020097fc;
extern s32 Data_02009800;
extern s32 Data_02009804;
extern s32 Data_02009808;
extern s32 Data_0200980c;
extern s32 Data_02009810;
extern s32 Data_02009814;
extern s32 Data_02009818;

void BabiFune_UpdateDriftingObject(u8 *obj);

/* FAKEMATCH: the inline consumer retains object-type argument setup order. */
static __inline__ struct FieldActor *SpawnDriftingObject(struct PositionWork *work,
    struct FixedPointPosition *pos, s32 x, s32 z)
{
    work->pos = pos;
    work->pos->y = 0;
    work->pos->x = x;
    z += Engine_RandomNext() * 160;
    z += 0x1e0000;
    work->pos->z = z;
    return Engine_ObjectCreate(0x1f7, work->pos->x, work->pos->y, z);
}

void BabiFune_UpdateWaves(void)
{
    volatile u16 scroll[1];
    struct Half zero;
    struct FixedPointPosition buf;
    struct PositionWork work;
    struct MapWork *map;
    struct FieldActor *actor;
    s32 bob;
    s32 x, z;

    map = Data_03001e70;
    if (Data_020097e8 != 0) {
        bob = Iwram_MulQ16(Engine_MathSin(Data_020097ec << 9), 3);
        /* FAKEMATCH: explicit halfword accesses retain truncation before I/O. */
        scroll[0] = Data_020097f0 + ((bob + 8) << 8);
        bob = scroll[0];
        *(volatile u16 *)0x04000052 = bob;
        Data_020097ec++;
    }
    if (Data_020097fc != 0) {
        bob = Iwram_MulQ16(Engine_MathSin(Data_02009800 << 9), 2) << 16;
        map->layers[6].y = Data_02009804 + bob;
        map->layers[7].y = Data_02009808 + bob;
        if (Data_0200980c != -0x10000) {
            actor = Engine_ActorGet(0);
            actor->y.fixed = Data_0200980c + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009810 != -0x10000) {
            actor = Engine_ActorGet(1);
            actor->y.fixed = Data_02009810 + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009814 != -0x10000) {
            actor = Engine_ActorGet(3);
            actor->y.fixed = Data_02009814 + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009818 != -0x10000) {
            actor = Engine_ActorGet(2);
            actor->y.fixed = Data_02009818 + bob;
            *(s32 *)actor->unknown_14 = Data_0200980c + bob;
            actor->motion_flags = 0;
        }
        Data_02009800++;
    }
    if (Data_020097f8 != 0 && (gFrameCount & 1) != 0) {
        x = map->layers[4].unknown_10[0] & -0x10000;
        z = map->layers[4].unknown_10[1] & -0x10000;
        x += Engine_RandomNext() * 240;
        actor = SpawnDriftingObject(&work, &buf, x, z);
        if (actor != 0) {
            actor->update = (void (*)(union FieldObject *))BabiFune_UpdateDriftingObject;
            actor->unknown_64 = 60;
            /* FAKEMATCH: a halfword zero retains the short literal-pool reach. */
            zero.v = 0;
            actor->unknown_66 = 1;
            actor->motion_flags = zero.v;
            actor->priority_flags = 2;
            actor->sprite->priority = 2;
            Engine_ObjectSetBlendMode(actor, 0);
            Engine_ObjectSetAnimation(actor, 0);
        }
    }
}
