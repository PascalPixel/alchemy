#include "TYPES.H"
#include "FIELD_EVENT.H"

void PositionSceneActorPair(s32 actor, s32 x, s32 z);

#define IN_COLUMNS(x) ((x) >= 6 && (x) <= 8)

/* Crossbone Isle push block, northward: by the row actor 18 stopped in and whether actors 19, 14 and 16 stand in columns 6 to 8, slide its pair back to the matching offset, then move the block's cell attributes. */
void TakaraShima_SettlePushedBlockNorth(void)
{
    s32 cell_x;
    s32 cell_z;
    s32 x19;
    s32 x14;
    s32 x16;

    cell_x = Object_GetById(18)->x.fixed >> 20;
    cell_z = Object_GetById(18)->z.fixed >> 20;
    x19 = Object_GetById(19)->x.fixed >> 20;
    x14 = Object_GetById(14)->x.fixed >> 20;
    x16 = Object_GetById(16)->x.fixed >> 20;
    if (cell_z == 19) {
        if (IN_COLUMNS(x19)) {
            PositionSceneActorPair(18, 0, -16);
        } else if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(18, 0, -64);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -112);
        } else {
            PositionSceneActorPair(18, 0, -64);
            PositionSceneActorPair(18, 0, -96);
        }
    } else if (cell_z == 18) {
        if (IN_COLUMNS(x19)) {
            return;
        }
        if (IN_COLUMNS(x14)) {
            PositionSceneActorPair(18, 0, -48);
        } else if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -96);
        } else {
            PositionSceneActorPair(18, 0, -144);
        }
    } else if (cell_z == 15) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -48);
        } else {
            PositionSceneActorPair(18, 0, -96);
        }
    } else if (cell_z == 14) {
        if (IN_COLUMNS(x14)) {
            return;
        }
        if (IN_COLUMNS(x16)) {
            PositionSceneActorPair(18, 0, -32);
        } else {
            PositionSceneActorPair(18, 0, -80);
        }
    } else if (cell_z == 12) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(18, 0, -48);
    } else if (cell_z == 11) {
        if (IN_COLUMNS(x16)) {
            return;
        }
        PositionSceneActorPair(18, 0, -32);
    } else if (cell_z == 9) {
        return;
    }
    Engine_TaskWait(2);
    Engine_MapCopyCellAttributes(cell_x - 1, cell_z, 3, 1, cell_x - 1, Object_GetById(18)->z.fixed >> 20);
    Engine_MapCopyCellAttributes(0, 0, 3, 1, cell_x - 1, cell_z);
}
