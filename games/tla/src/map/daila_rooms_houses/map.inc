	.global Field_DailaRoomsHouses
Field_DailaRoomsHouses:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DailaRoomsHouses, .LField_DailaRoomsHouses_CELL_TILES
	map_block Field_DailaRoomsHouses, .LField_DailaRoomsHouses_SHAPES
	map_block Field_DailaRoomsHouses, .LField_DailaRoomsHouses_GRID
	map_block Field_DailaRoomsHouses, .LField_DailaRoomsHouses_SHAPE_GRID
	map_block Field_DailaRoomsHouses, .LField_DailaRoomsHouses_ANIMATION
	map_block Field_DailaRoomsHouses, .LField_DailaRoomsHouses_BLEND
	map_block Field_DailaRoomsHouses, .LField_DailaRoomsHouses_SCENE
.LField_DailaRoomsHouses_CELL_TILES:
	.incbin "games/tla/src/map/daila_rooms_houses/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_DailaRoomsHouses_SHAPES:
	.incbin "games/tla/src/map/daila_rooms_houses/shapes.table.lz"
	.balign 4, 0
.LField_DailaRoomsHouses_GRID:
	.incbin "games/tla/src/map/daila_rooms_houses/map.grid.lz"
	.balign 4, 0
.LField_DailaRoomsHouses_SHAPE_GRID:
	.incbin "games/tla/src/map/daila_rooms_houses/shape_grid.plane.lz"
	.balign 4, 0
.LField_DailaRoomsHouses_ANIMATION:
	.incbin "games/tla/src/map/daila_rooms_houses/animation.script.lz"
	.balign 4, 0
.LField_DailaRoomsHouses_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_DailaRoomsHouses_SCENE:
	.incbin "games/tla/src/map/daila_rooms_houses/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DailaRoomsHousesPalette
Tileset_DailaRoomsHousesPalette:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_houses/palette.gbapal.lz"
	.global Tileset_DailaRoomsHousesTilesA
Tileset_DailaRoomsHousesTilesA:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_houses/tiles_a.4bpp.mtf"
	.global Tileset_DailaRoomsHousesTilesB
Tileset_DailaRoomsHousesTilesB:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_houses/tiles_b.4bpp.mtf"
	.byte 0x58	@ PACKER: left in the buffer by Tileset_DailaRoomsHousesTilesA+0x12b3
	.global Tileset_DailaRoomsHousesTilesC
Tileset_DailaRoomsHousesTilesC:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_houses/tiles_c.4bpp.mtf"
	.global Tileset_DailaRoomsHousesTilesD
Tileset_DailaRoomsHousesTilesD:
	.incbin "games/tla/src/graphics/tileset/daila_rooms_houses/tiles_d.4bpp.mtf"
