	.global Field_ChampaRooms
Field_ChampaRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaRooms, .LField_ChampaRooms_CELL_TILES
	map_block Field_ChampaRooms, .LField_ChampaRooms_SHAPES
	map_block Field_ChampaRooms, .LField_ChampaRooms_GRID
	map_block Field_ChampaRooms, .LField_ChampaRooms_SHAPE_GRID
	map_block Field_ChampaRooms, .LField_ChampaRooms_ANIMATION
	map_block Field_ChampaRooms, .LField_ChampaRooms_BLEND
	map_block Field_ChampaRooms, .LField_ChampaRooms_SCENE
.LField_ChampaRooms_CELL_TILES:
	.incbin "games/tla/src/map/champa_rooms/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ChampaRooms_SHAPES:
	.incbin "games/tla/src/map/champa_rooms/shapes.table.lz"
	.balign 4, 0
.LField_ChampaRooms_GRID:
	.incbin "games/tla/src/map/champa_rooms/map.grid.lz"
	.balign 4, 0
.LField_ChampaRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/champa_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_ChampaRooms_ANIMATION:
	.incbin "games/tla/src/map/izumo_rooms/animation.script.lz"
	.balign 4, 0
.LField_ChampaRooms_BLEND:
	.incbin "games/tla/src/map/izumo_rooms/blend.script.lz"
	.balign 4, 0
.LField_ChampaRooms_SCENE:
	.incbin "games/tla/src/map/champa_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteChampaRooms
Tileset_PaletteChampaRooms:
	.incbin "games/tla/src/graphics/tileset/palettes/champa_rooms.gbapal.lz"
	.byte 0x30	@ PACKER: left in the buffer by Field_ChampaRooms+0x193
