	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout145
Field_Layout145:
	layout_header 0, 0, 100, 100, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout145, .LField_Layout145_METATILES
	layout_offset Field_Layout145, .LField_Layout145_SHAPES
	layout_offset Field_Layout145, .LField_Layout145_GRID
	layout_offset Field_Layout145, .LField_Layout145_ANIMATION
	layout_offset Field_Layout145, .LField_Layout145_BLEND
	layout_offset Field_Layout145, .LField_Layout145_MARKERS
.LField_Layout145_METATILES:
	.incbin "games/tbs/src/layout/layout_145/metatiles.delta1.lz"
	.balign 4, 0
.LField_Layout145_SHAPES:
	.incbin "games/tbs/src/layout/layout_015/shapes.table.lz"
	.balign 4, 0
.LField_Layout145_GRID:
	.incbin "games/tbs/src/layout/layout_145/map.grid.lz"
	.balign 4, 0
.LField_Layout145_ANIMATION:
	.incbin "games/tbs/src/layout/layout_145/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout145_BLEND:
	.incbin "games/tbs/src/layout/layout_009/blend.script.lz"
	.balign 4, 0
.LField_Layout145_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set63Palette
Tileset_Set63Palette:
	.incbin "games/tbs/src/graphics/tileset/set_63/palette.gbapal.lz"
	.byte 0x4a, 0xf0, 0x09	@ PACKER: left in the buffer by Field_Layout145+0x161
	.global Tileset_Set63TilesA
Tileset_Set63TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_63/tiles_a.4bpp.mtf"
	.byte 0x02, 0x04, 0x30	@ PACKER: left in the buffer by Field_Layout143+0x29c9
