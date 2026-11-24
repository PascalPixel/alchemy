	.global Field_GarohRooms
Field_GarohRooms:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GarohRooms, .LField_GarohRooms_CELL_TILES
	map_block Field_GarohRooms, .LField_GarohRooms_SHAPES
	map_block Field_GarohRooms, .LField_GarohRooms_GRID
	map_block Field_GarohRooms, .LField_GarohRooms_SHAPE_GRID
	map_block Field_GarohRooms, .LField_GarohRooms_ANIMATION
	map_block Field_GarohRooms, .LField_GarohRooms_BLEND
	map_block Field_GarohRooms, .LField_GarohRooms_SCENE
.LField_GarohRooms_CELL_TILES:
	.incbin "games/tla/src/map/garoh_rooms/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GarohRooms_SHAPES:
	.incbin "games/tla/src/map/garoh_rooms/shapes.table.lz"
	.balign 4, 0
.LField_GarohRooms_GRID:
	.incbin "games/tla/src/map/garoh_rooms/map.grid.lz"
	.balign 4, 0
.LField_GarohRooms_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_rooms/shape_grid.plane.lz"
	.balign 4, 0
.LField_GarohRooms_ANIMATION:
	.incbin "games/tla/src/map/mikasalla_rooms/animation.script.lz"
	.balign 4, 0
.LField_GarohRooms_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_GarohRooms_SCENE:
	.incbin "games/tla/src/map/garoh_rooms/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGarohRooms
Tileset_PaletteGarohRooms:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_rooms.gbapal.lz"
	.byte 0xf3, 0xff	@ PACKER: left in the buffer by Field_GarohRooms+0x1c2
	.global Tileset_PaletteGarohRoomsB
Tileset_PaletteGarohRoomsB:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_rooms_b.gbapal.lz"
