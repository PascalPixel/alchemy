	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout050
Field_Layout050:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout050, .LField_Layout050_METATILES
	layout_offset Field_Layout050, .LField_Layout050_SHAPES
	layout_offset Field_Layout050, .LField_Layout050_GRID
	layout_offset Field_Layout050, .LField_Layout050_ANIMATION
	layout_offset Field_Layout050
	layout_offset Field_Layout050, .LField_Layout050_MARKERS
.LField_Layout050_METATILES:
	.incbin "games/tbs/src/layout/layout_050/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout050_SHAPES:
	.incbin "games/tbs/src/layout/layout_050/shapes.table.lz"
	.balign 4, 0
.LField_Layout050_GRID:
	.incbin "games/tbs/src/layout/layout_050/map.grid.lz"
	.balign 4, 0
.LField_Layout050_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout050_MARKERS:
	treasure_spot 32, 16, 100
	treasure_spot 53, 36, 101
	treasure_spot 52, 37, 102
	treasure_spots_end
	.global Tileset_Set40Palette
Tileset_Set40Palette:
	.incbin "games/tbs/src/graphics/tileset/set_40/palette.gbapal.lz"
	.global Tileset_Set40TilesA
Tileset_Set40TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_40/tiles_a.4bpp.mtf"
	.byte 0x45, 0x58	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x2966
