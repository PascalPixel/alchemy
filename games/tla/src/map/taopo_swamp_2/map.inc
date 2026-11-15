	.global Field_TaopoSwamp2
Field_TaopoSwamp2:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwamp2, .LField_TaopoSwamp2_CELL_PICTURES
	map_block Field_TaopoSwamp2, .LField_TaopoSwamp2_SHAPES
	map_block Field_TaopoSwamp2, .LField_TaopoSwamp2_GRID
	map_block Field_TaopoSwamp2, .LField_TaopoSwamp2_SHAPE_GRID
	map_block Field_TaopoSwamp2, .LField_TaopoSwamp2_ANIMATION
	map_block Field_TaopoSwamp2, .LField_TaopoSwamp2_BLEND
	map_block Field_TaopoSwamp2, .LField_TaopoSwamp2_SCENE
.LField_TaopoSwamp2_CELL_PICTURES:
	.incbin "games/tla/src/map/taopo_swamp_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TaopoSwamp2_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_2/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwamp2_GRID:
	.incbin "games/tla/src/map/taopo_swamp_2/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwamp2_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwamp2_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_2/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwamp2_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwamp2_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_2/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TaopoSwamp2Palette
Tileset_TaopoSwamp2Palette:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_2/palette.gbapal.lz"
	.byte 0x27, 0x95	@ PACKER: left in the buffer by Field_TaopoSwamp2+0x10a
	.global Tileset_TaopoSwamp2TilesA
Tileset_TaopoSwamp2TilesA:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_2/tiles_a.4bpp.mtf"
	.byte 0x4c, 0x4f, 0x3a	@ PACKER: left in the buffer by Field_TaopoSwamp2+0x2091
	.global Tileset_TaopoSwamp2TilesB
Tileset_TaopoSwamp2TilesB:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_2/tiles_b.4bpp.mtf"
	.global Tileset_TaopoSwamp2TilesC
Tileset_TaopoSwamp2TilesC:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_2/tiles_c.4bpp.mtf"
	.global Tileset_TaopoSwamp2TilesD
Tileset_TaopoSwamp2TilesD:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_2/tiles_d.4bpp.mtf"
	.byte 0x4b	@ PACKER: left in the buffer by Tileset_TaopoSwamp2TilesB+0x162b
