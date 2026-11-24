	.global Field_MikasallaRoomsSanctum
Field_MikasallaRoomsSanctum:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MikasallaRoomsSanctum, .LField_MikasallaRoomsSanctum_CELL_TILES
	map_block Field_MikasallaRoomsSanctum, .LField_MikasallaRoomsSanctum_SHAPES
	map_block Field_MikasallaRoomsSanctum, .LField_MikasallaRoomsSanctum_GRID
	map_block Field_MikasallaRoomsSanctum, .LField_MikasallaRoomsSanctum_SHAPE_GRID
	map_block Field_MikasallaRoomsSanctum, .LField_MikasallaRoomsSanctum_ANIMATION
	map_block Field_MikasallaRoomsSanctum, .LField_MikasallaRoomsSanctum_BLEND
	map_block Field_MikasallaRoomsSanctum, .LField_MikasallaRoomsSanctum_SCENE
.LField_MikasallaRoomsSanctum_CELL_TILES:
	.incbin "games/tla/src/map/mikasalla_rooms_sanctum/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_MikasallaRoomsSanctum_SHAPES:
	.incbin "games/tla/src/map/mikasalla_rooms_sanctum/shapes.table.lz"
	.balign 4, 0
.LField_MikasallaRoomsSanctum_GRID:
	.incbin "games/tla/src/map/mikasalla_rooms_sanctum/map.grid.lz"
	.balign 4, 0
.LField_MikasallaRoomsSanctum_SHAPE_GRID:
	.incbin "games/tla/src/map/mikasalla_rooms_sanctum/shape_grid.plane.lz"
	.balign 4, 0
.LField_MikasallaRoomsSanctum_ANIMATION:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/animation.script.lz"
	.balign 4, 0
.LField_MikasallaRoomsSanctum_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_MikasallaRoomsSanctum_SCENE:
	.incbin "games/tla/src/map/mikasalla_rooms_sanctum/scene.scene.lz"
	.global Tileset_MikasallaRoomsSanctumPalette
Tileset_MikasallaRoomsSanctumPalette:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms_sanctum/palette.gbapal.lz"
	.byte 0xe7, 0x0b	@ PACKER: left in the buffer by Field_MikasallaRooms2+0x186
	.global Tileset_MikasallaRoomsSanctumTilesA
Tileset_MikasallaRoomsSanctumTilesA:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms_sanctum/tiles_a.4bpp.mtf"
	.byte 0x8c, 0xeb	@ PACKER: left in the buffer by Field_MikasallaRooms2+0x11f6
	.global Tileset_MikasallaRoomsSanctumTilesB
Tileset_MikasallaRoomsSanctumTilesB:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms_sanctum/tiles_b.4bpp.mtf"
	.byte 0xc8, 0x4f, 0x0d	@ PACKER: left in the buffer by Tileset_MikasallaRoomsSanctumTilesA+0xec1
	.global Tileset_MikasallaRoomsSanctumTilesC
Tileset_MikasallaRoomsSanctumTilesC:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms_sanctum/tiles_c.4bpp.mtf"
	.global Tileset_MikasallaRoomsSanctumTilesD
Tileset_MikasallaRoomsSanctumTilesD:
	.incbin "games/tla/src/graphics/tileset/mikasalla_rooms_sanctum/tiles_d.4bpp.mtf"
	.byte 0x20, 0x3c	@ PACKER: left in the buffer by Tileset_MikasallaRoomsSanctumTilesC+0xf1e
