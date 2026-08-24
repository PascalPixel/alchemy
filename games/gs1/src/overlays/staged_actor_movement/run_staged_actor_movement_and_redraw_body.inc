    u8 *workspace;
    StagedActorRecord *actor;
    StagedActorPosition original_position;
    StagedActorPosition tile_position;
    u8 *footprint_table;
    s32 direction;
    s32 horizontal_extent;
    s32 vertical_extent;

    workspace = Data_03001e70;
    direction = Func_02006610(0)->orientation >> 12;
    actor = Func_0200661c(request.actor_id);
    footprint_table = (u8 *)Data_0200e1e8;
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

    Func_02000902(0, tile_position.x, tile_position.y, horizontal_extent,
                  vertical_extent, 0);
    Func_020066b4(0, 0x8000, 0x1999);
    Func_02006714(0, 8);
    Func_0200668a(15);

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
            Func_0200672c(0, horizontal_delta, vertical_delta);
        }
    }

    Func_020066ea(0)->callback = (u32)&Value_020082a9;
    Func_020066c4(4);
    if ((u32)(direction - 6) <= 7)
        Func_02006614(actor, 3);
    else
        Func_0200661e(actor, 2);

    Func_0200687c(239);
    Func_02006658(actor, request.target_x, request.target_depth,
                  request.tail.target_y);
    Func_02006776(0);
    Func_0200678e(0, 2);
    ConfigureStagedActorMotion(0, 0x4ccc, 0x1999);

    {
        s32 packed_orientation = Data_0200e190[direction];
        Func_020067a4(0, ((s16)(packed_orientation >> 16)) / 2,
                      ((s16)packed_orientation) / 2);
    }
    if (request.tail.callback != 0)
        request.tail.callback();

    Func_020067bc(0);
    Func_020067d4(0, 1);
    Func_0200677a(0)->callback = 0;
    RefreshStagedActor(actor);
    Func_020068f4(288);
    Func_020068fa(213);

    actor->x = request.target_x;
    actor->y = request.tail.target_y;
    actor->horizontal_velocity = 0;
    actor->vertical_velocity = 0;
    Func_020066b8(actor, 1);

    {
        u8 *table = (u8 *)Data_0200e1e8;
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
        Func_02000a6e(0, request.target_x, request.tail.target_y,
                      horizontal_extent, vertical_extent, 255);
        Func_02000a80(2, request.target_x, request.tail.target_y,
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

        Func_020067ae(horizontal_origin, vertical_origin, horizontal_extent,
                      vertical_extent, original_position.x,
                      original_position.y);
        Func_02000acc(2, original_position.x, original_position.y,
                      horizontal_extent, vertical_extent, 0);
    }
    Func_020069d0();
