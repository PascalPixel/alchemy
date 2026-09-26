/* NONMATCHING: 556 bytes, candidate 556, 124 differing halfwords, 77 aligned
 * edits (2026-09-26). Reconstructed both Q16 waves and all four actor-height
 * blocks. The complete topology now agrees. Remaining: halfword scroll
 * reload and stack-slot ownership, saved position pointer, and spawn stores.
 * A 20-byte aggregate restores the halfword load and complete size; its
 * volatile member adds an unwanted read before the scroll store. */
#include "TYPES.H"
#include "FIELD_EVENT.H"
#include "IWRAM_CALL.H"

struct MapLayer {
    s32 unknown_00[3];
    s32 y;
    s32 unknown_10[8];
};

struct MapWork {
    u8 unknown_00[20];
    struct MapLayer layers[8];
};

struct Position {
    s32 x, y, z;
};

/* FAKEMATCH: the stack aggregate retains the scroll and position views. */
struct WaveScratch {
    struct Position *pos;
    struct Position buf;
    u16 unused;
    volatile u16 scroll;
};

extern struct MapWork *Data_03001e70;
extern s32 Data_020097e8;
extern s32 Data_020097ec;
extern s32 Data_020097f0;
extern s32 Data_020097f8;
extern s32 Data_020097fc;
extern s32 Data_02009800;
extern s32 Data_02009810;
extern s32 Data_02009820;
extern s32 Data_02009830;
extern s32 Data_02009840;
extern s32 Data_02009850;
extern s32 Data_02009860;
extern u8 Value_00000000;

void BabiFune_UpdateDriftingObject(u8 *obj);

void BabiFune_UpdateWaves(void)
{
    struct WaveScratch work;
    struct MapWork *map;
    struct FieldActor *actor;
    s32 bob;
    s32 x, z;

    map = Data_03001e70;
    if (Data_020097e8 != 0) {
        bob = Iwram_MulQ16(Engine_MathSin(Data_020097ec << 9), 3);
        work.scroll = Data_020097f0 + ((bob + 8) << 8);
        *(volatile u16 *)0x0400001a = work.scroll;
        Data_020097ec++;
    }
    if (Data_020097fc != 0) {
        bob = Iwram_MulQ16(Engine_MathSin(Data_02009800 << 9), 2) << 16;
        map->layers[6].y = Data_02009810 + bob;
        map->layers[7].y = Data_02009820 + bob;
        if (Data_02009830 != -0x10000) {
            actor = Engine_ActorGet(0);
            actor->y.fixed = Data_02009830 + bob;
            *(s32 *)actor->unknown_14 = Data_02009830 + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009840 != -0x10000) {
            actor = Engine_ActorGet(1);
            actor->y.fixed = Data_02009840 + bob;
            *(s32 *)actor->unknown_14 = Data_02009830 + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009850 != -0x10000) {
            actor = Engine_ActorGet(3);
            actor->y.fixed = Data_02009850 + bob;
            *(s32 *)actor->unknown_14 = Data_02009830 + bob;
            actor->motion_flags = 0;
        }
        if (Data_02009860 != -0x10000) {
            actor = Engine_ActorGet(2);
            actor->y.fixed = Data_02009860 + bob;
            *(s32 *)actor->unknown_14 = Data_02009830 + bob;
            actor->motion_flags = 0;
        }
        Data_02009800++;
    }
    if (Data_020097f8 != 0 && (gFrameCount & 1) != 0) {
        x = map->layers[4].unknown_10[0] & -0x10000;
        z = map->layers[4].unknown_10[1] & -0x10000;
        x += Engine_RandomNext() * 240;
        work.pos = &work.buf;
        work.pos->y = 0;
        work.pos->x = x;
        z += Engine_RandomNext() * 160;
        z += 0x1e0000;
        work.pos->z = z;
        actor = Engine_ObjectCreate(0x1f7, work.pos->x, work.pos->y, z);
        if (actor != 0) {
            actor->update = (void (*)(union FieldObject *))BabiFune_UpdateDriftingObject;
            actor->unknown_64 = 60;
            actor->unknown_66 = 1;
            /* FAKEMATCH: the zero-valued link symbol preserves its pool load. */
            actor->motion_flags = (s32)&Value_00000000;
            actor->priority_flags = 2;
            actor->sprite->priority = 2;
            Engine_ObjectSetBlendMode(actor, 0);
            Engine_ObjectSetAnimation(actor, 0);
        }
    }
}
