	.global Field_Map278
Field_Map278:
	map_header 0, 0, 96, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map278, .LField_Map278_CELL_PICTURES
	map_block Field_Map278, .LField_Map278_SHAPES
	map_block Field_Map278, .LField_Map278_GRID
	map_block Field_Map278, .LField_Map278_SHAPE_GRID
	map_block Field_Map278, .LField_Map278_ANIMATION
	map_block Field_Map278, .LField_Map278_BLEND
	map_block Field_Map278, .LField_Map278_SCENE
.LField_Map278_CELL_PICTURES:
	.incbin "games/tla/src/map/map_278/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map278_SHAPES:
	.incbin "games/tla/src/map/map_278/shapes.table.lz"
	.balign 4, 0
.LField_Map278_GRID:
	.incbin "games/tla/src/map/map_278/map.grid.lz"
	.balign 4, 0
.LField_Map278_SHAPE_GRID:
	.incbin "games/tla/src/map/map_278/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map278_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map278_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map278_SCENE:
	.incbin "games/tla/src/map/map_278/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set113Palette
Tileset_Set113Palette:
	.incbin "games/tla/src/graphics/tileset/set_113/palette.gbapal.lz"
	.byte 0x9e, 0x4d	@ PACKER: left in the buffer by Field_Map278+0x142
	.global Tileset_Set113TilesA
Tileset_Set113TilesA:
	.incbin "games/tla/src/graphics/tileset/set_113/tiles_a.4bpp.mtf"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global Tileset_Set113TilesB
Tileset_Set113TilesB:
	.incbin "games/tla/src/graphics/tileset/set_113/tiles_b.4bpp.mtf"
	.global Tileset_Set113TilesC
Tileset_Set113TilesC:
	.incbin "games/tla/src/graphics/tileset/set_113/tiles_c.4bpp.mtf"
	.byte 0x06, 0x6b, 0x35	@ PACKER: left in the buffer by Tileset_Set113TilesB+0xbe5
	.global Tileset_Set113TilesD
Tileset_Set113TilesD:
	.incbin "games/tla/src/graphics/tileset/set_113/tiles_d.4bpp.mtf"
	.byte 0x88, 0x5f	@ PACKER: left in the buffer by Tileset_Set113TilesC+0x3fa
