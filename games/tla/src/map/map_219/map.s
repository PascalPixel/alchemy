	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map219
Field_Map219:
	map_header 0, 0, 128, 128, 3, 1, 0, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map219, .LField_Map219_CELL_PICTURES
	map_block Field_Map219, .LField_Map219_SHAPES
	map_block Field_Map219, .LField_Map219_GRID
	map_block Field_Map219, .LField_Map219_SHAPE_GRID
	map_block Field_Map219, .LField_Map219_ANIMATION
	map_block Field_Map219, .LField_Map219_BLEND
	map_block Field_Map219, .LField_Map219_SCENE
.LField_Map219_CELL_PICTURES:
	.incbin "games/tla/src/map/map_219/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map219_SHAPES:
	.incbin "games/tla/src/map/map_219/shapes.table.lz"
	.balign 4, 0
.LField_Map219_GRID:
	.incbin "games/tla/src/map/map_219/grid.parts.lz"
	.balign 4, 0
.LField_Map219_SHAPE_GRID:
	.incbin "games/tla/src/map/map_219/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map219_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map219_BLEND:
	.incbin "games/tla/src/map/map_007/blend.script.lz"
	.balign 4, 0
.LField_Map219_SCENE:
	.incbin "games/tla/src/map/map_219/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set93Palette
Tileset_Set93Palette:
	.incbin "games/tla/src/graphics/tileset/set_93/palette.gbapal.lz"
	.global Tileset_Set93TilesA
Tileset_Set93TilesA:
	.incbin "games/tla/src/graphics/tileset/set_93/tiles_a.4bpp.mtf"
	.byte 0x20, 0xce	@ PACKER: left in the buffer by Field_Map219+0x281e
	.global Tileset_Set93TilesB
Tileset_Set93TilesB:
	.incbin "games/tla/src/graphics/tileset/set_93/tiles_b.4bpp.mtf"
	.global Tileset_Set93TilesC
Tileset_Set93TilesC:
	.incbin "games/tla/src/graphics/tileset/set_93/tiles_c.4bpp.mtf"
	.global Tileset_Set93TilesD
Tileset_Set93TilesD:
	.incbin "games/tla/src/graphics/tileset/set_93/tiles_d.4bpp.mtf"
	.byte 0xc2, 0x8d	@ PACKER: left in the buffer by Tileset_Set93TilesB+0x120e
