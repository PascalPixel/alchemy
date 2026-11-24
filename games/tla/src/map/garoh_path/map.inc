	.global Field_GarohPath
Field_GarohPath:
	map_header 0, 0, 128, 128, 2, 3, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 11, 14, 0, 0, 0x1f, 0x1f
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GarohPath, .LField_GarohPath_CELL_TILES
	map_block Field_GarohPath, .LField_GarohPath_SHAPES
	map_block Field_GarohPath, .LField_GarohPath_GRID
	map_block Field_GarohPath, .LField_GarohPath_SHAPE_GRID
	map_block Field_GarohPath, .LField_GarohPath_ANIMATION
	map_block Field_GarohPath, .LField_GarohPath_BLEND
	map_block Field_GarohPath, .LField_GarohPath_SCENE
.LField_GarohPath_CELL_TILES:
	.incbin "games/tla/src/map/garoh_path/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GarohPath_SHAPES:
	.incbin "games/tla/src/map/garoh_path/shapes.table.lz"
	.balign 4, 0
.LField_GarohPath_GRID:
	.incbin "games/tla/src/map/garoh_path/map.grid.lz"
	.balign 4, 0
.LField_GarohPath_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_path/shape_grid.plane.lz"
	.balign 4, 0
.LField_GarohPath_ANIMATION:
	.incbin "games/tla/src/map/garoh_path/animation.script.lz"
	.balign 4, 0
.LField_GarohPath_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GarohPath_SCENE:
	.incbin "games/tla/src/map/garoh_path/scene.scene.lz"
	.global Tileset_PaletteGarohPath
Tileset_PaletteGarohPath:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_path.gbapal.lz"
	.byte 0x98, 0xc4	@ PACKER: left in the buffer by Field_Garoh2+0x182
