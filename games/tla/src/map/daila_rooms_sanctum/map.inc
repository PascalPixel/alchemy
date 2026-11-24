	.global Field_DailaRoomsSanctum
Field_DailaRoomsSanctum:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DailaRoomsSanctum, .LField_DailaRoomsSanctum_CELL_TILES
	map_block Field_DailaRoomsSanctum, .LField_DailaRoomsSanctum_SHAPES
	map_block Field_DailaRoomsSanctum, .LField_DailaRoomsSanctum_GRID
	map_block Field_DailaRoomsSanctum, .LField_DailaRoomsSanctum_SHAPE_GRID
	map_block Field_DailaRoomsSanctum, .LField_DailaRoomsSanctum_ANIMATION
	map_block Field_DailaRoomsSanctum, .LField_DailaRoomsSanctum_BLEND
	map_block Field_DailaRoomsSanctum, .LField_DailaRoomsSanctum_SCENE
.LField_DailaRoomsSanctum_CELL_TILES:
	.incbin "games/tla/src/map/daila_rooms_sanctum/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_DailaRoomsSanctum_SHAPES:
	.incbin "games/tla/src/map/daila_rooms_sanctum/shapes.table.lz"
	.balign 4, 0
.LField_DailaRoomsSanctum_GRID:
	.incbin "games/tla/src/map/daila_rooms_sanctum/map.grid.lz"
	.balign 4, 0
.LField_DailaRoomsSanctum_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_rooms_sanctum/shape_grid.plane.lz"
	.balign 4, 0
.LField_DailaRoomsSanctum_ANIMATION:
	.incbin "games/tla/src/map/daila_rooms_sanctum/animation.script.lz"
	.balign 4, 0
.LField_DailaRoomsSanctum_BLEND:
	.incbin "games/tla/src/map/daila_rooms_sanctum/blend.script.lz"
	.balign 4, 0
.LField_DailaRoomsSanctum_SCENE:
	.incbin "games/tla/src/map/daila_rooms_sanctum/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DailaRoomsSanctumPalette
Tileset_DailaRoomsSanctumPalette:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_sanctum/palette.gbapal.lz"
	.byte 0x80	@ PACKER: left in the buffer by Field_DailaRooms2+0x197
	.global Tileset_DailaRoomsSanctumTilesA
Tileset_DailaRoomsSanctumTilesA:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_sanctum/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_DailaRoomsSanctumTilesB
Tileset_DailaRoomsSanctumTilesB:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_sanctum/tiles_b.4bpp.mtf"
	.byte 0x07	@ PACKER: left in the buffer by Tileset_DailaRoomsSanctumTilesA+0x17b3
	.global Tileset_DailaRoomsSanctumTilesC
Tileset_DailaRoomsSanctumTilesC:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_sanctum/tiles_c.4bpp.mtf"
	.byte 0x8e, 0x9f	@ PACKER: left in the buffer by Tileset_DailaRoomsSanctumTilesB+0xf9a
	.global Tileset_DailaRoomsSanctumTilesD
Tileset_DailaRoomsSanctumTilesD:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_sanctum/tiles_d.4bpp.mtf"
	.ifdef TLA_EDITION_EN
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.balign 4, 0
