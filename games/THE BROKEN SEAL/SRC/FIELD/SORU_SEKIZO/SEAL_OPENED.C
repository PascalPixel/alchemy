#include "TYPES.H"
#include "FIELD_EVENT.H"

/* A point the camera follows, in 16.16 fixed point. */
struct FocusPoint {
    s32 x;
    s32 y;
    s32 z;
};

/* The map work begins with the point the camera follows. */
struct FocusWork {
    struct FocusPoint *focus;
};

extern struct FocusWork *gMapWork;

/* After the seal opens: the leader turns toward the seal from whichever side
 * of it he stands, the camera slides thirty pixels toward it, the seal's
 * cells pulse faster and faster, the opened seal is drawn, and the camera
 * slides back. */
void SoruSekizo_RunSealOpenedSequence(void)
{
    struct FocusWork *work;
    struct FieldActor *leader;
    struct FocusPoint *saved;
    struct FocusPoint point;
    s32 side;
    s32 i;

    work = gMapWork;
    leader = Engine_ActorGet(0);
    if (leader->z.fixed < 0xb30000) {
        Actor_WalkToAndWait(0, 0x23f, 132);
        Actor_FaceDirection(0, 0x4000, 0);
        Event_Wait(30);
        saved = work->focus;
        point.x = leader->x.fixed;
        point.y = leader->y.fixed;
        point.z = leader->z.fixed;
        work->focus = &point;
        for (i = 0; i != 30; i++) {
            point.z += 0x10000;
            Event_Wait(1);
        }
        Event_Wait(40);
        side = 1;
    } else {
        Actor_WalkToAndWait(0, 0x241, 222);
        Actor_FaceDirection(0, 0xc000, 0);
        Event_Wait(30);
        point.x = leader->x.fixed;
        point.y = leader->y.fixed;
        point.z = leader->z.fixed;
        saved = work->focus;
        work->focus = &point;
        for (i = 0; i != 30; i++) {
            point.z -= 0x10000;
            Event_Wait(1);
        }
        Event_Wait(40);
        side = 2;
    }
    for (i = 0; i != 6; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Event_Wait(8);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Event_Wait(8);
    }
    for (i = 0; i != 10; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Event_Wait(4);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Event_Wait(4);
    }
    for (i = 0; i != 12; i++) {
        Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
        Event_Wait(2);
        Map_CopyCellsTo(2, 30, 34, 10, 4, 2);
        Event_Wait(2);
    }
    Map_CopyCellsTo(2, 28, 34, 10, 4, 2);
    Map_CopyCellsTo(8, 55, 32, 40, 8, 4);
    Event_Wait(60);
    if (side == 1) {
        for (i = 0; i != 30; i++) {
            point.z -= 0x10000;
            Event_Wait(1);
        }
    } else if (side == 2) {
        for (i = 0; i != 30; i++) {
            point.z += 0x10000;
            Event_Wait(1);
        }
    }
    work->focus = saved;
}
