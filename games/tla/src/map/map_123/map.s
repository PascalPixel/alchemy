	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map123
Field_Map123:
	map_header 0, 0, 96, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 96, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map123, .LField_Map123_CELL_PICTURES
	map_block Field_Map123, .LField_Map123_SHAPES
	map_block Field_Map123, .LField_Map123_GRID
	map_block Field_Map123, .LField_Map123_SHAPE_GRID
	map_block Field_Map123, .LField_Map123_ANIMATION
	map_block Field_Map123, .LField_Map123_BLEND
	map_block Field_Map123, .LField_Map123_SCENE
.LField_Map123_CELL_PICTURES:
	.incbin "games/tla/src/map/map_123/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map123_SHAPES:
	.incbin "games/tla/src/map/map_123/shapes.table.lz"
	.balign 4, 0
.LField_Map123_GRID:
	.incbin "games/tla/src/map/map_123/map.grid.lz"
	.balign 4, 0
.LField_Map123_SHAPE_GRID:
	.incbin "games/tla/src/map/map_123/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map123_ANIMATION:
	.incbin "games/tla/src/map/map_123/animation.script.lz"
	.balign 4, 0
.LField_Map123_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map123_SCENE:
	.incbin "games/tla/src/map/map_123/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set60Palette
Tileset_Set60Palette:
	.incbin "games/tla/src/graphics/tileset/set_60/palette.gbapal.lz"
	.global Tileset_Set60TilesA
Tileset_Set60TilesA:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_a.4bpp.mtf"
	.global Tileset_Set60TilesB
Tileset_Set60TilesB:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_b.4bpp.mtf"
	.global Tileset_Set60TilesC
Tileset_Set60TilesC:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_c.4bpp.mtf"
	.global Tileset_Set60TilesD
Tileset_Set60TilesD:
	.incbin "games/tla/src/graphics/tileset/set_60/tiles_d.4bpp.mtf"
	.byte 0xa4	@ PACKER: left in the buffer by Tileset_Set60TilesC+0x18db
