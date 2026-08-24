#include "types.h"

struct BattleActorDefinition {
    u8 reserved_000[296];
    u8 class_id;
};

struct BattleActorDefinition *Func_08077008(u16 actor_id);
s32 Func_080c23c0(u8 class_id);

#define LayoutSummonPositions Func_080b7424

void LayoutSummonPositions(u16 *actor_ids, s32 count, s32 *x_positions, s32 *z_positions)
{
    s32 total_spacing = count <= 4 ? 30 : 27;
    s32 z = ((count - 1) * total_spacing) / 2;
    s32 index;

    for (index = 0; index != count; index++) {
        s32 spacing = 0;

        x_positions[index] = -80;
        if (index != 0) {
            struct BattleActorDefinition *actor;

            spacing = 25;
            if ((u16)(actor_ids[index] - 254) > 1) {
                actor = Func_08077008(actor_ids[index]);
                spacing = Func_080c23c0(actor->class_id) ? 27 : 38;
                if (actor->class_id == 148 || actor->class_id == 121) {
                    x_positions[index] = -50;
                }
            }
        }
        z -= spacing / 2;
        z_positions[index] = z;
        spacing = 25;
        if ((u16)(actor_ids[index] - 254) > 1) {
            struct BattleActorDefinition *actor = Func_08077008(actor_ids[index]);

            spacing = Func_080c23c0(actor->class_id) ? 27 : 38;
        }
        z -= spacing / 2;
    }
}
