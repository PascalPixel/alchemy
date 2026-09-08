    u8 *work;
    struct StagedActor **table;
    struct StagedActor *view;
    struct StagedActor *actor;
    u32 slot;
    u32 index;
    s32 kind;
    s32 view_x;
    s32 view_z;
    s32 tile_x;
    s32 tile_z;
    s32 ax;
    s32 az;
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;

    work = SceneCollisionWork;
    view = GetSceneViewActor(0);
    *direction_out = (u32)view->direction_and_kind >> 12;
    slot = 8;
    table = (struct StagedActor **)(work + 20);
    do {
        actor = table[slot];
        index = 0;
        kind = *STAGED_ACTOR_PROBE_DETAILS(actor)->unknown_28;
        do {
            if (kind == ActorKindProbeValues[index]) {
                *footprint_out = index;
                view_x = view->x.value;
                tile_x = ((view_x >> 16)
                    + (ActorDirectionSteps[*direction_out] >> 16)) >> 4;
                view_z = view->z.value;
                tile_z = ((view_z >> 16)
                    + (s16)ActorDirectionSteps[*direction_out]) >> 4;
                ax = actor->x.parts.cell;
                left = (ax + ActorFootprintBounds[index].x0) >> 4;
                az = actor->z.parts.cell;
                top = (az + ActorFootprintBounds[index].z0) >> 4;
                right = (ax + ActorFootprintBounds[index].x1) >> 4;
                bottom = (az + ActorFootprintBounds[index].z1) >> 4;
                if (left <= tile_x && tile_x < right &&
                    top <= tile_z && tile_z < bottom) {
                    if ((1 & index) != 0) {
                        if (left != (view_x >> 20)) {
                            *slot_out = slot;
                            return actor;
                        }
                    } else {
                        if (top != (view_z >> 20)) {
                            *slot_out = slot;
                            return actor;
                        }
                    }
                }
            }
            index++;
        } while (index <= 5);
        slot++;
    } while (slot <= 65);
    return 0;
