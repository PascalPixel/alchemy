	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map039
Field_Map039:
	map_header 0, 0, 100, 68, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 1, -1, 0x3f, 0x3f
	map_block Field_Map039, .LField_Map039_CELL_PICTURES
	map_block Field_Map039, .LField_Map039_SHAPES
	map_block Field_Map039, .LField_Map039_GRID
	map_block Field_Map039, .LField_Map039_SHAPE_GRID
	map_block Field_Map039, .LField_Map039_ANIMATION
	map_block Field_Map039, .LField_Map039_BLEND
	map_block Field_Map039, .LField_Map039_SCENE
.LField_Map039_CELL_PICTURES:
	.incbin "games/tla/src/map/map_039/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map039_SHAPES:
	.incbin "games/tla/src/map/map_039/shapes.table.lz"
	.balign 4, 0
.LField_Map039_GRID:
	.incbin "games/tla/src/map/map_039/grid.parts.lz"
	.balign 4, 0
.LField_Map039_SHAPE_GRID:
	.incbin "games/tla/src/map/map_039/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map039_ANIMATION:
	.incbin "games/tla/src/map/map_039/animation.script.lz"
	.balign 4, 0
.LField_Map039_BLEND:
	.incbin "games/tla/src/map/map_039/blend.script.lz"
	.balign 4, 0
.LField_Map039_SCENE:
	.incbin "games/tla/src/map/map_039/scene.parts.lz"
	.global Tileset_Set22Palette
Tileset_Set22Palette:
	.incbin "games/tla/src/graphics/tileset/set_22/palette.gbapal.lz"
	.byte 0x12, 0x05, 0x08	@ PACKER: left in the buffer by Field_Map039+0x14d
	.global Tileset_Set22TilesA
Tileset_Set22TilesA:
	.incbin "games/tla/src/graphics/tileset/set_22/tiles_a.4bpp.mtf"
	.byte 0x00, 0x02	@ PACKER: left in the buffer by Field_Map034+0x288a
	.global Tileset_Set22TilesB
Tileset_Set22TilesB:
	.incbin "games/tla/src/graphics/tileset/set_22/tiles_b.4bpp.mtf"
	.byte 0x08, 0x04	@ PACKER: left in the buffer by Tileset_Set22TilesA+0x27ee
	.global Tileset_Set22TilesC
Tileset_Set22TilesC:
	.incbin "games/tla/src/graphics/tileset/set_22/tiles_c.4bpp.mtf"
	.byte 0x41, 0xf2	@ PACKER: left in the buffer by Tileset_Set22TilesB+0xf9e
	.global Tileset_Set22TilesD
Tileset_Set22TilesD:
	.incbin "games/tla/src/graphics/tileset/set_22/tiles_d.4bpp.mtf"
	.byte 0x03, 0x86, 0xc0	@ PACKER: left in the buffer by Tileset_Set22TilesB+0x2151
