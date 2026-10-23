	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout038
Field_Layout038:
	layout_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout038, .LField_Layout038_METATILES
	layout_offset Field_Layout038, .LField_Layout038_SHAPES
	layout_offset Field_Layout038, .LField_Layout038_GRID
	layout_offset Field_Layout038, .LField_Layout038_ANIMATION
	layout_offset Field_Layout038, .LField_Layout038_BLEND
	layout_offset Field_Layout038, .LField_Layout038_MARKERS
.LField_Layout038_METATILES:
	.incbin "games/tbs/src/layout/layout_038/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout038_SHAPES:
	.incbin "games/tbs/src/layout/layout_038/shapes.table.lz"
	.balign 4, 0
.LField_Layout038_GRID:
	.incbin "games/tbs/src/layout/layout_038/map.grid.lz"
	.balign 4, 0
.LField_Layout038_ANIMATION:
	.incbin "games/tbs/src/layout/layout_018/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout038_BLEND:
	.incbin "games/tbs/src/layout/layout_018/blend.script.lz"
	.balign 4, 0
.LField_Layout038_MARKERS:
	.incbin "games/tbs/src/layout/layout_038/treasure_spots.table"
	.balign 4, 0
	.global Tileset_Set30Palette
Tileset_Set30Palette:
	.incbin "games/tbs/src/graphics/tileset/set_30/palette.gbapal.lz"
	.byte 0x8e, 0x9e, 0x21	@ PACKER: left in the buffer by Field_Layout038+0x1a1
	.global Tileset_Set30TilesA
Tileset_Set30TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_30/tiles_a.4bpp.mtf"
	.byte 0x21, 0x2e	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x1902
