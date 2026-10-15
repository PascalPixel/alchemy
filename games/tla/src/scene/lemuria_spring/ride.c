#include "types.h"
#include "callback_scheduler.h"
#include "party_state.h"

struct SpringPoint {
    s32 x;
    s32 y;
    s32 z;
};

struct SpringRide {
    u16 unk00;
    u16 hold;
    struct SpringPoint trail[5];
    struct SpringPoint velocity;
    s32 frames;
};

extern struct SpringRide gData_02001760;
extern s32 gData_020017b0;
extern u8 gData_020017c0[];
extern s32 gData_02001820;
extern s32 gData_02001824;
extern s32 gData_02001828;
extern u8 gData_0200174c[];
extern u8 gData_02001750[];
extern u16 gData_02001754[];

struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetModeById(s32 id, s32 mode);
void WaitFrames(s32 frames);
void Func_020008f0(s32 actor, s32 active);
void Func_02000904(void);
void Func_020012e8(struct ObjectRuntime *object, s32 mode);
void Func_02001408(s32 cue);

// Places the four actors from their tables, resets the ride and installs
// its task.
void Func_020010f4(void)
{
    struct SpringRide *ride = &gData_02001760;
    s32 i = 0;
    u8 *xtbl;
    u16 *htbl;
    u8 *rec;
    u8 *ztbl;

    htbl = gData_02001754;
    ztbl = gData_02001750;
    xtbl = gData_0200174c;
    rec = gData_020017c0;

    /* FAKEMATCH: a goto loop; as ☀️'s do/while its loop-end note keeps CSE from reusing the loop's zero for the ride's stores (differ 31). */
next:
    *(s32 *)(rec + 0) = *xtbl << 16;
    *(s32 *)(rec + 8) = *ztbl << 16;
    *(s32 *)(rec + 4) = 0;
    *(u16 *)(rec + 12) = *htbl;
    *(u16 *)(rec + 14) = 0;
    *(u16 *)(rec + 16) = 0;
    *(u16 *)(rec + 18) = 0;
    *(u16 *)(rec + 20) = 0;
    i++;
    xtbl++;
    ztbl++;
    htbl++;
    rec += 24;
    if (i != 4)
        goto next;

    ride->trail[0].x = 0xffe20000;
    ride->trail[0].y = 0;
    ride->trail[0].z = 0x640000;
    ride->velocity.x = 0;
    ride->velocity.y = 0;
    ride->velocity.z = 0;
    ride->frames = 0;

    Func_020012e8(Object_GetById(20), 2);
    Func_020012e8(Object_GetById(21), 2);

    Scheduler_AddOrUpdateCallback((SchedulerCallback)Func_02000904, 0x483);
}

// Runs the spring ride from one side until it ends, and returns its result.
s32 Func_02001184(s32 side)
{
    struct SpringRide *ride;

    ride = &gData_02001760;
    ride->trail[0].y = 0;
    ride->trail[1].y = 0;
    ride->trail[2].y = 0;
    ride->trail[3].y = 0;
    gData_020017b0 = side;
    gData_02001824 = 0;
    ride->hold = 0xffff;
    gData_02001820 = 0;
    /* FAKEMATCH: a goto loop; as ☀️'s for loop, loop.c keeps the frame counter's and the party's addresses in registers (differ 64). */
    goto start;
next:
    gData_02001820++;
start:
    if (gData_02001820 == 50)
        Func_02001408(300);
    /* FAKEMATCH: the volatile read keeps the frame's reload where both paths meet; plain, PRE copies it onto the cue's path (differ 2). */
    if (*(volatile s32 *)&gData_02001820 == 16) {
        Object_SetModeById(gPartyState.currentOwner, 39);
        ride->hold = 0;
        ride->velocity.x = 0x14ccc;
        ride->velocity.y = 0x40000;
        ride->velocity.z = -0x20000;
        ride->trail[0].x = 0x780000;
        ride->trail[0].y = 0x100000;
        ride->trail[0].z = 0x980000;
        ride->frames = 300;
        if (gData_020017b0 == 1) {
            Func_020012e8(Object_GetById(16), 3);
            Func_020012e8(Object_GetById(17), 0);
            Func_020008f0(15, 1);
            Func_020008f0(14, 1);
            Func_020008f0(13, 1);
        } else {
            Func_020012e8(Object_GetById(11), 3);
            Func_020012e8(Object_GetById(12), 0);
            Func_020008f0(10, 1);
            Func_020008f0(9, 1);
            Func_020008f0(8, 1);
        }
    }
    WaitFrames(1);
    if (gData_02001824 != 1)
        goto next;
    return gData_02001828;
}
