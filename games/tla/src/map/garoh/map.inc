	.global Field_Garoh
Field_Garoh:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Garoh, .LField_Garoh_CELL_TILES
	map_block Field_Garoh, .LField_Garoh_SHAPES
	map_block Field_Garoh, .LField_Garoh_GRID
	map_block Field_Garoh, .LField_Garoh_SHAPE_GRID
	map_block Field_Garoh, .LField_Garoh_ANIMATION
	map_block Field_Garoh, .LField_Garoh_BLEND
	map_block Field_Garoh, .LField_Garoh_SCENE
.LField_Garoh_CELL_TILES:
	.incbin "games/tla/src/map/garoh/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_Garoh_SHAPES:
	.incbin "games/tla/src/map/garoh/shapes.table.lz"
	.balign 4, 0
.LField_Garoh_GRID:
	.incbin "games/tla/src/map/garoh/map.grid.lz"
	.balign 4, 0
.LField_Garoh_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh/shape_grid.plane.lz"
	.balign 4, 0
.LField_Garoh_ANIMATION:
	.incbin "games/tla/src/map/garoh/animation.script.lz"
	.balign 4, 0
.LField_Garoh_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_Garoh_SCENE:
	.incbin "games/tla/src/map/garoh/scene.scene.lz"
	.global Tileset_PaletteGaroh
Tileset_PaletteGaroh:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh.gbapal.lz"
	.byte 0x0e	@ PACKER: left in the buffer by Field_Garoh1+0x173
