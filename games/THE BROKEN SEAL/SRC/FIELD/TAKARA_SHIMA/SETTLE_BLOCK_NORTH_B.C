#include "TYPES.H"
#include "FIELD_EVENT.H"

void PositionSceneActorPair(s32 actor, s32 x, s32 z);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

#define IN_COLUMNS(x) ((x) >= 9 && (x) <= 11)

/* Crossbone Isle push block (actor 9), northward: by the row it stopped in and whether actors 19, 14 and 16 stand in columns 9 to 11, slide its pair back to the matching offset, then move the block's cell attributes. */
void TakaraShima_SettleSecondBlockNorth(void)
{
    s32 cell_x;
    s32 cell_z;
    s32 x19;
    s32 x14;
    s32 x16;

    cell_x = Engine_ActorGet(9)->x.fixed >> 20;
    cell_z = Engine_ActorGet(9)->z.fixed >> 20;
    x19 = Engine_ActorGet(19)->x.fixed >> 20;
    x14 = Engine_ActorGet(14)->x.fixed >> 20;
    x16 = Engine_ActorGet(16)->x.fixed >> 20;
    if (cell_z == 19) {
        if (IN_COLUMNS(x19)) {
            PositionSceneActorPair(9, 0, -16);
        } else if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(9, 0, -64);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -112);
        } else {
            PositionSceneActorPair(9, 0, -80);
            PositionSceneActorPair(9, 0, -96);
        }
    } else if (cell_z == 18) {
        if (IN_COLUMNS(x19)) {
            return;
        }
        if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(9, 0, -48);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -96);
        } else {
            PositionSceneActorPair(9, 0, -96);
            PositionSceneActorPair(9, 0, -64);
        }
    } else if (cell_z == 15) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -48);
        } else {
            PositionSceneActorPair(9, 0, -112);
        }
    } else if (cell_z == 14) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(9, 0, -32);
        } else {
            PositionSceneActorPair(9, 0, -96);
        }
    } else if (cell_z == 12) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(9, 0, -64);
    } else if (cell_z == 11) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(9, 0, -48);
    } else if ((u32)cell_z <= 9) {
        return;
    }
    Engine_TaskWait(2);
    Call6(Engine_MapCopyCellAttributes, cell_x - 1, cell_z, 3, 1, cell_x - 1, Engine_ActorGet(9)->z.fixed >> 20);
    Call6(Engine_MapCopyCellAttributes, 0, 0, 3, 1, cell_x - 1, cell_z);
}
