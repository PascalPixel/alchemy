/* NONMATCHING: 548 bytes, candidate 544, 96 differing halfwords (2026-09-24).
 * Single-overlay unit binding Engine_* at their import veneers. Remaining:
 * hand-written; the body matches except register choice. The reference keeps
 * pos in r7 and copies it into a separate pointer (r5) at every ShiftFocus
 * loop, including the two after the door flashes, with the counter in r6;
 * here the inline parameter is copied only in the first two loops (pos is
 * dead after the last ones, so the copy coalesces), pos lands in r6 and the
 * counter in r5, and the flash loops keep 2 in r7. Loops count up with != so
 * they are not reversed; the pointer is passed as the array so the inline
 * copies it. A shared p pointer variable (p = pos before each loop), a shared
 * counter, and an explicit copy inside the inline all still give 96: cse
 * folds p back into the frame address, so the copy never survives
 * (2026-09-24, ovl8a). */
#include "TYPES.H"
#include "FIELD_EVENT.H"

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* Moves the camera focus up or down one pixel a frame for thirty frames. */
static __inline__ void ShiftFocus(union FieldCoordinate *p, s32 step)
{
    s32 i;

    for (i = 0; i != 30; i++) {
        p[2].fixed += step;
        Engine_EventWait(1);
    }
}

/* Swaps the two door frames, holding each for the given frames. */
static __inline__ void FlashDoor(s32 count, s32 frames)
{
    s32 i;

    for (i = 0; i != count; i++) {
        Engine_MapCopyCellsTo(2, 28, 34, 10, 4, 2);
        Engine_EventWait(frames);
        Engine_MapCopyCellsTo(2, 30, 34, 10, 4, 2);
        Engine_EventWait(frames);
    }
}

void Local_02000c8c(void)
{
    union FieldCoordinate **camera;
    union FieldCoordinate *saved;
    union FieldCoordinate pos[3];
    struct FieldActor *leader;
    s32 side;

    camera = *(union FieldCoordinate ***)0x03001e70;
    leader = Engine_ActorGet(0);
    if (leader->z.fixed < 0xb30000) {
        Call3((void (*)())Engine_ActorWalkToAndWait, 0, 0x23f, 132);
        Call3((void (*)())Engine_ActorFaceDirection, 0, 0x4000, 0);
        Engine_EventWait(30);
        saved = *camera;
        pos[0].fixed = leader->x.fixed;
        pos[1].fixed = leader->y.fixed;
        pos[2].fixed = leader->z.fixed;
        *camera = pos;
        ShiftFocus(pos, 0x10000);
        Engine_EventWait(40);
        side = 1;
    } else {
        Call3((void (*)())Engine_ActorWalkToAndWait, 0, 0x241, 222);
        Call3((void (*)())Engine_ActorFaceDirection, 0, 0xc000, 0);
        Engine_EventWait(30);
        pos[0].fixed = leader->x.fixed;
        pos[1].fixed = leader->y.fixed;
        pos[2].fixed = leader->z.fixed;
        saved = *camera;
        *camera = pos;
        ShiftFocus(pos, -0x10000);
        Engine_EventWait(40);
        side = 2;
    }
    FlashDoor(6, 8);
    FlashDoor(10, 4);
    FlashDoor(12, 2);
    Engine_MapCopyCellsTo(2, 28, 34, 10, 4, 2);
    Engine_MapCopyCellsTo(8, 55, 32, 40, 8, 4);
    Engine_EventWait(60);
    if (side == 1)
        ShiftFocus(pos, -0x10000);
    else if (side == 2)
        ShiftFocus(pos, 0x10000);
    *camera = saved;
}
