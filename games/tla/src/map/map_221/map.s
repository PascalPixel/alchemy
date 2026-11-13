	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map221
Field_Map221:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map221, .LField_Map221_CELL_PICTURES
	map_block Field_Map221, .LField_Map221_SHAPES
	map_block Field_Map221, .LField_Map221_GRID
	map_block Field_Map221, .LField_Map221_SHAPE_GRID
	map_block Field_Map221, .LField_Map221_ANIMATION
	map_block Field_Map221, .LField_Map221_BLEND
	map_block Field_Map221, .LField_Map221_SCENE
.LField_Map221_CELL_PICTURES:
	.incbin "games/tla/src/map/map_221/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map221_SHAPES:
	.incbin "games/tla/src/map/map_221/shapes.table.lz"
	.balign 4, 0
.LField_Map221_GRID:
	.incbin "games/tla/src/map/map_221/grid.parts.lz"
	.balign 4, 0
.LField_Map221_SHAPE_GRID:
	.incbin "games/tla/src/map/map_221/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map221_ANIMATION:
	.incbin "games/tla/src/map/map_221/animation.script.lz"
	.balign 4, 0
.LField_Map221_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map221_SCENE:
	.incbin "games/tla/src/map/map_221/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set95Palette
Tileset_Set95Palette:
	.incbin "games/tla/src/graphics/tileset/set_95/palette.gbapal.lz"
	.byte 0x80, 0x07, 0x98	@ PACKER: left in the buffer by Field_Map221+0x1d5
	.global Tileset_Set95TilesA
Tileset_Set95TilesA:
	.incbin "games/tla/src/graphics/tileset/set_95/tiles_a.4bpp.mtf"
	.global Tileset_Set95TilesB
Tileset_Set95TilesB:
	.incbin "games/tla/src/graphics/tileset/set_95/tiles_b.4bpp.mtf"
	.byte 0x5b, 0x44, 0x03	@ PACKER: left in the buffer by Tileset_Set95TilesA+0x1679
	.global Tileset_Set95TilesC
Tileset_Set95TilesC:
	.incbin "games/tla/src/graphics/tileset/set_95/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0xff, 0xf3	@ PACKER: left in the buffer by Tileset_Set95TilesA+0x1751
	.else
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.endif
	.global Tileset_Set95TilesD
Tileset_Set95TilesD:
	.incbin "games/tla/src/graphics/tileset/set_95/tiles_d.4bpp.mtf"
	.global Tileset_Palette49
Tileset_Palette49:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_49.gbapal.lz"
	.byte 0xa7	@ PACKER: left in the buffer by Tileset_Set95TilesD+0x1cb
