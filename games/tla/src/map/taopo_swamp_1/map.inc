	.global Field_TaopoSwamp1
Field_TaopoSwamp1:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwamp1, .LField_TaopoSwamp1_CELL_PICTURES
	map_block Field_TaopoSwamp1, .LField_TaopoSwamp1_SHAPES
	map_block Field_TaopoSwamp1, .LField_TaopoSwamp1_GRID
	map_block Field_TaopoSwamp1, .LField_TaopoSwamp1_SHAPE_GRID
	map_block Field_TaopoSwamp1, .LField_TaopoSwamp1_ANIMATION
	map_block Field_TaopoSwamp1, .LField_TaopoSwamp1_BLEND
	map_block Field_TaopoSwamp1, .LField_TaopoSwamp1_SCENE
.LField_TaopoSwamp1_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_TaopoSwamp1_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_1/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp1_GRID:
	.incbin "games/tla/src/map/taopo_swamp_1/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwamp1_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwamp1_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_1/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp1_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_1/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TaopoSwamp1Palette
Tileset_TaopoSwamp1Palette:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_1/palette.gbapal.lz"
	.byte 0x18, 0x8c	@ PACKER: left in the buffer by Field_TaopoSwamp1+0x116
	.global Tileset_TaopoSwamp1TilesA
Tileset_TaopoSwamp1TilesA:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_1/tiles_a.4bpp.mtf"
	.byte 0x00, 0x1b, 0x22	@ PACKER: left in the buffer by Field_YallamInteriors+0x2091
	.global Tileset_TaopoSwamp1TilesB
Tileset_TaopoSwamp1TilesB:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_1/tiles_b.4bpp.mtf"
	.global Tileset_TaopoSwamp1TilesC
Tileset_TaopoSwamp1TilesC:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_1/tiles_c.4bpp.mtf"
	.byte 0xf5	@ PACKER: left in the buffer by Tileset_TaopoSwamp1TilesB+0x697
	.global Tileset_TaopoSwamp1TilesD
Tileset_TaopoSwamp1TilesD:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_1/tiles_d.4bpp.mtf"
