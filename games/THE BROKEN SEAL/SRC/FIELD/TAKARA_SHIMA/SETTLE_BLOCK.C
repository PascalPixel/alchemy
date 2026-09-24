#include "TYPES.H"
#include "FIELD_EVENT.H"

void PositionSceneActorPair(s32 actor, s32 x, s32 z);

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* Crossbone Isle push block: by the column actor 19 stopped in and where actors 18 and 9 stand, slide its pair to the matching offset, then move the block's cell attributes to its new column. */
void TakaraShima_SettlePushedBlock(void)
{
    s32 cell_x;
    s32 cell_z;
    s32 z18;
    s32 z9;

    cell_x = Engine_ActorGet(19)->x.fixed >> 20;
    cell_z = Engine_ActorGet(19)->z.fixed >> 20;
    z18 = Engine_ActorGet(18)->z.fixed >> 20;
    z9 = Engine_ActorGet(9)->z.fixed >> 20;
    if (cell_x == 3) {
        if (z18 == 15) {
            PositionSceneActorPair(19, 32, 0);
        } else if (z9 == 15) {
            PositionSceneActorPair(19, 80, 0);
        } else {
            PositionSceneActorPair(19, 112, 0);
            PositionSceneActorPair(19, 48, 0);
        }
    } else if (cell_x == 5) {
        if (z18 == 15) {
            return;
        }
        if (z9 == 15) {
            PositionSceneActorPair(19, 48, 0);
        } else {
            PositionSceneActorPair(19, 128, 0);
        }
    } else if (cell_x == 6) {
        if (z9 == 15) {
            PositionSceneActorPair(19, 32, 0);
        } else {
            PositionSceneActorPair(19, 112, 0);
        }
    } else if (cell_x == 8) {
        if (z9 == 15) {
            return;
        }
        PositionSceneActorPair(19, 80, 0);
    } else if (cell_x == 9) {
        PositionSceneActorPair(19, 64, 0);
    } else if (cell_x == 12) {
        PositionSceneActorPair(19, 16, 0);
    }
    Engine_TaskWait(2);
    Call6(Engine_MapCopyCellAttributes, cell_x, cell_z - 1, 1, 3, Engine_ActorGet(19)->x.fixed >> 20, cell_z - 1);
    Call6(Engine_MapCopyCellAttributes, 0, 0, 1, 3, cell_x, cell_z - 1);
}
