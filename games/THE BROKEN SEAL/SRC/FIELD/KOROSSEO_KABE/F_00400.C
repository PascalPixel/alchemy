#include "TYPES.H"
#include "FIELD_EVENT.H"

void SceneActor_MovePairByTileOffset(s32 actor, s32 dx, s32 dz);

/* The buttons held this frame. */
extern u32 Data_03001ae8;

/* When the leader stands in line with wall block 32 (or 33 on the far rows)
 * and pushes it along the row (left past column 51, right before it), moves
 * the block pair and redraws the cells under both blocks. */
void KorosseoKabe_Func02000400(void)
{
    struct FieldActor *leader = Engine_ActorGet(gGameState.selected_actor);
    s32 x = leader->x.fixed >> 20;
    s32 push = 0;
    s32 block = 32;

    if (leader->z.fixed >> 20 > 12) {
        block = 33;
    }
    if (Engine_ActorGet(block)->x.fixed >> 20 != x) {
        return;
    }
    if (x > 51) {
        if (Data_03001ae8 & 0x20) {
            push = -64;
        }
    } else if (Data_03001ae8 & 0x10) {
        push = 64;
    }
    if (push != 0) {
        SceneActor_MovePairByTileOffset(block, push, 0);
        Engine_MapCopyCellAttributes(120, 10, 5, 6, 48, 10);
        x = Engine_ActorGet(32)->x.fixed >> 20;
        Engine_MapCopyCellAttributes(52, 28, 1, 3, x, 10);
        x = Engine_ActorGet(33)->x.fixed >> 20;
        Engine_MapCopyCellAttributes(52, 28, 1, 3, x, 13);
    }
}
