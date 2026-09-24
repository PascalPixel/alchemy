#include "TYPES.H"

/*
 * Places the battle view for the current entrance: finds the entrance's
 * entry in the overlay's view table (the first entry whose flag is clear
 * when none matches), sets the view position and heading from it unless
 * flag 0x109 is set, then applies the entry's scroll limits and keeps the
 * view window at least one screen (240 by 160) inside them.
 */

struct EntranceView {
    s16 entrance;                   /* 0x00; -1 ends the table */
    s16 flag;                       /* 0x02; -1 when unconditional */
    s16 x;                          /* 0x04 */
    s16 y;                          /* 0x06 */
    s16 z;                          /* 0x08 */
    u16 heading;                    /* 0x0a */
    s16 unknown_0c;
    s16 left;                       /* 0x0e; -1 keeps the current limit */
    s16 top;                        /* 0x10 */
    s16 right;                      /* 0x12 */
    s16 bottom;                     /* 0x14 */
    s16 unknown_16;
};

struct ViewState {
    u8 unknown_000[0x1c2];
    s16 entrance;                   /* 0x1c2 */
    u8 unknown_1c4[0x18];
    s32 x;                          /* 0x1dc */
    s32 y;                          /* 0x1e0 */
    s32 z;                          /* 0x1e4 */
    u32 heading;                    /* 0x1e8 */
    u16 turn;                       /* 0x1ec */
};

struct ViewServices {
    u8 unknown_00[0x0c];
    struct EntranceView *(*entrance_views)(void);
};

struct MapScrollWork {
    u8 unknown_000[0xec];
    s32 left;                       /* 0x0ec */
    s32 top;                        /* 0x0f0 */
    s32 right;                      /* 0x0f4 */
    s32 bottom;                     /* 0x0f8 */
};

extern struct MapScrollWork *gMapWork;
extern struct ViewState gGameState;
extern struct ViewServices gOv;

s32 GameFlag_TestFar(s32 flag);

void BattleMap_ApplyEntranceView(void)
{
    struct MapScrollWork *work = gMapWork;
    s32 entrance = gGameState.entrance;
    struct EntranceView *view = gOv.entrance_views();
    s32 found = 0;

    while (view->entrance != -1) {
        if (view->entrance == entrance
            && (view->flag == -1 || GameFlag_TestFar(view->flag))) {
            found = 1;
            break;
        }
        view++;
    }
    if (!found)
        view = gOv.entrance_views();

    if (!GameFlag_TestFar(0x109)) {
        gGameState.x = view->x << 16;
        gGameState.y = view->y << 16;
        gGameState.z = view->z << 16;
        gGameState.heading = view->heading;
        gGameState.turn = 0;
    }

    if (view->left != -1)
        work->left = view->left << 16;
    if (view->top != -1)
        work->top = view->top << 16;
    if (view->right != -1)
        work->right = view->right << 16;
    if (view->bottom != -1)
        work->bottom = view->bottom << 16;

    if (work->left + (240 << 16) > work->right)
        work->left = work->right - (240 << 16);
    if (work->top + (160 << 16) > work->bottom)
        work->top = work->bottom - (160 << 16);
}
