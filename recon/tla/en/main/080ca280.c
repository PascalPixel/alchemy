#include "TYPES.H"

struct SceneAreaNameRule {
    s16 id;
    s16 entrance : 15;
    u16 use_scene_id : 1;
    s32 area_name_index;
};

extern const struct SceneAreaNameRule Data_080ef4a4[];

s32 BattleFx_GetResourceGroup(s32 effect_id);

/* ☀️'s: selects an area-name index by scene or scene group and entrance. A
   rule with entrance -1 matches every entrance. */
s32 Scene_GetAreaNameIndex(s32 scene, s32 entrance)
{
    const struct SceneAreaNameRule *entry = Data_080ef4a4;
    s32 area_name_index = 0;
    s32 group = BattleFx_GetResourceGroup(scene);

    while (entry->id != -1) {
        if (entry->use_scene_id) {
            if (entry->id == scene) {
                if (entry->entrance == -1 || entry->entrance == entrance) {
                    area_name_index = entry->area_name_index;
                    break;
                }
            }
        } else {
            if (entry->id == group) {
                if (entry->entrance == -1 || entry->entrance == entrance) {
                    area_name_index = entry->area_name_index;
                    break;
                }
            }
        }
        entry++;
    }
    return area_name_index;
}
