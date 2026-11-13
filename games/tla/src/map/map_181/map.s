	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map181
Field_Map181:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map181, .LField_Map181_CELL_PICTURES
	map_block Field_Map181, .LField_Map181_SHAPES
	map_block Field_Map181, .LField_Map181_GRID
	map_block Field_Map181, .LField_Map181_SHAPE_GRID
	map_block Field_Map181, .LField_Map181_ANIMATION
	map_block Field_Map181, .LField_Map181_BLEND
	map_block Field_Map181, .LField_Map181_SCENE
.LField_Map181_CELL_PICTURES:
	.incbin "games/tla/src/map/map_181/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map181_SHAPES:
	.incbin "games/tla/src/map/map_181/shapes.table.lz"
	.balign 4, 0
.LField_Map181_GRID:
	.incbin "games/tla/src/map/map_181/grid.parts.lz"
	.balign 4, 0
.LField_Map181_SHAPE_GRID:
	.incbin "games/tla/src/map/map_181/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map181_ANIMATION:
	.incbin "games/tla/src/map/map_181/animation.script.lz"
	.balign 4, 0
.LField_Map181_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map181_SCENE:
	.incbin "games/tla/src/map/map_181/scene.parts.lz"
	.global Tileset_Map181TilesA
Tileset_Map181TilesA:
	.incbin "games/tla/src/graphics/tileset/map_181/tiles_a.4bpp.mtf"
	.byte 0xa7, 0xe6	@ PACKER: left in the buffer by Tileset_Set81TilesD+0x1f0a
	.global Tileset_Map181TilesB
Tileset_Map181TilesB:
	.incbin "games/tla/src/graphics/tileset/map_181/tiles_b.4bpp.mtf"
	.byte 0x12	@ PACKER: left in the buffer by Tileset_Set81TilesD+0x2363
	.global Tileset_Map181TilesC
Tileset_Map181TilesC:
	.incbin "games/tla/src/graphics/tileset/map_181/tiles_c.4bpp.mtf"
	.global Tileset_Map181TilesD
Tileset_Map181TilesD:
	.incbin "games/tla/src/graphics/tileset/map_181/tiles_d.4bpp.mtf"
