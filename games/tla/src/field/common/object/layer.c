#include "types.h"
#include "object_runtime.h"
#include "party_state.h"

struct ObjectLayerAnimation {
    u8 unknown_00[9];
    u8 unused_09 : 2;
    u8 layer : 2;
    u8 unknown_0a[27];
    u8 unused_25 : 2;
    u8 shadow_layer : 2;
};

// 先頭のキャラクターと同じ表示レイヤーにする
s32 Func_080d4ab4(struct ObjectRuntime *object)
{
    struct ObjectRuntime *leader = ObjectTable_Get(gPartyState.current_owner);
    s32 layer = ((struct ObjectLayerAnimation *)leader->animation)->layer;

    if ((object->animation_kind & 0xf) == 1) {
        struct ObjectLayerAnimation *animation = object->animation;

        animation->layer = layer;
        animation->shadow_layer = layer;
        object->priority_flags &= 0xfe;
    }
    return 0;
}
