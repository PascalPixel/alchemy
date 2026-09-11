    u8 *workspace;
    StagedActorRecord *actor;
    StagedActorPosition original_position;
    StagedActorPosition tile_position;
    u8 *footprint_table;
    s32 direction;
    s32 horizontal_extent;
    s32 vertical_extent;

    workspace = (u8 *)gCam;
    direction = StagedActor_GetActive(0)->orientation >> 12;
    actor = StagedActor_Get(request.actor_id);
    footprint_table = (u8 *)gStagedFootprint;
    {
        s32 footprint_offset = request.movement_index << 4;
        s32 table_offset = footprint_offset + 4;
        s32 extent_a = *(s32 *)(footprint_table + table_offset);
        s32 extent_b;
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 12;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        vertical_extent = (extent_a + extent_b) >> 4;
        extent_a = *(s32 *)(footprint_table + footprint_offset);
        if (extent_a < 0)
            extent_a = -extent_a;
        table_offset = footprint_offset;
        table_offset += 8;
        extent_b = *(s32 *)(footprint_table + table_offset);
        if (extent_b < 0)
            extent_b = -extent_b;
        horizontal_extent = (extent_a + extent_b) >> 4;
    }

    actor->movement_rate = 0x8000;
    actor->movement_step = 0x1999;
    original_position.x = actor->x;
    original_position.y = actor->y;
    {
        s32 table_offset = request.movement_index << 4;
        tile_position.x =
            (actor->x +
             (*(s32 *)(footprint_table + table_offset) << 16));
        table_offset += 4;
        tile_position.y =
            (actor->y +
             (*(s32 *)(footprint_table + table_offset) << 16));
        tile_position.x >>= 20;
        tile_position.y >>= 20;
    }

    StagedActor_FillTileRect(0, tile_position.x, tile_position.y, horizontal_extent,
                  vertical_extent, 0);
    StagedActor_SetSpeed(0, 0x8000, 0x1999);
    StagedActor_SetStep(0, 8);
    StagedActor_SetFacing(15);

    {
        s32 horizontal_delta = request.target_x - original_position.x;
        if (horizontal_delta < 0)
            horizontal_delta += 0x1ffff;
        horizontal_delta >>= 17;
        {
            s32 vertical_delta =
                request.tail.target_y - original_position.y;
            if (vertical_delta < 0)
                vertical_delta += 0x1ffff;
            vertical_delta >>= 17;
            StagedActor_AddDelta(0, horizontal_delta, vertical_delta);
        }
    }

    StagedActor_Motion(0)->callback = (u32)&Value_020082a9;
    StagedActor_SetMode(4);
    if ((u32)(direction - 6) <= 7)
        StagedActor_SetFlagA(actor, 3);
    else
        StagedActor_SetFlagB(actor, 2);

    StagedActor_PlayCue(239);
    StagedActor_SetTarget(actor, request.target_x, request.target_depth,
                  request.tail.target_y);
    StagedActor_Commit(0);
    StagedActor_SetState(0, 2);
    ConfigureStagedActorMotion(0, 0x4ccc, 0x1999);

    {
        s32 packed_orientation = gStagedHeading[direction];
        StagedActor_SetHeading(0, ((s16)(packed_orientation >> 16)) / 2,
                      ((s16)packed_orientation) / 2);
    }
    if (request.tail.callback != 0)
        request.tail.callback();

    StagedActor_ClearHeading(0);
    StagedActor_SetLock(0, 1);
    StagedActor_Motion2(0)->callback = 0;
    RefreshStagedActor(actor);
    StagedActor_PlayCue2(288);
    StagedActor_PlayCue3(213);

    actor->x = request.target_x;
    actor->y = request.tail.target_y;
    actor->horizontal_velocity = 0;
    actor->vertical_velocity = 0;
    StagedActor_SetFlagC(actor, 1);

    {
        u8 *table = (u8 *)gStagedFootprint;
        s32 horizontal_origin;
        s32 vertical_origin;

        {
            s32 table_offset = request.movement_index << 4;
            request.target_x =
                (request.target_x +
                 (*(s32 *)(table + table_offset) << 16));
            table_offset += 4;
            request.tail.target_y =
                (request.tail.target_y +
                 (*(s32 *)(table + table_offset) << 16));
            request.target_x >>= 20;
            request.tail.target_y >>= 20;
        }
        horizontal_origin = *(s32 *)(workspace + 316) >> 20;
        vertical_origin = *(s32 *)(workspace + 320) >> 20;

        RedrawStagedActorRegion(request.target_x, request.tail.target_y,
                                horizontal_extent, vertical_extent,
                                horizontal_origin + request.target_x,
                                vertical_origin + request.tail.target_y);
        StagedActor_FillDestRect(0, request.target_x, request.tail.target_y,
                      horizontal_extent, vertical_extent, 255);
        StagedActor_FillDestRect2(2, request.target_x, request.tail.target_y,
                      horizontal_extent, vertical_extent, 255);

        {
            s32 table_offset = request.movement_index << 4;
            original_position.x =
                (original_position.x +
                 (*(s32 *)(table + table_offset) << 16));
            table_offset += 4;
            original_position.y =
                (original_position.y +
                 (*(s32 *)(table + table_offset) << 16));
            original_position.x >>= 20;
            original_position.y >>= 20;
        }
        horizontal_origin += original_position.x;
        vertical_origin += original_position.y;

        StagedActor_RedrawAt(horizontal_origin, vertical_origin, horizontal_extent,
                      vertical_extent, original_position.x,
                      original_position.y);
        StagedActor_RestoreRect(2, original_position.x, original_position.y,
                      horizontal_extent, vertical_extent, 0);
    }
    StagedActor_Finish();
