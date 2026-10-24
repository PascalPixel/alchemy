	.include "games/tbs/src/layout/layout.inc"
	.section .rodata
	.global Field_Layout049
Field_Layout049:
	layout_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	layout_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	layout_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	layout_offset Field_Layout049, .LField_Layout049_METATILES
	layout_offset Field_Layout049, .LField_Layout049_SHAPES
	layout_offset Field_Layout049, .LField_Layout049_GRID
	layout_offset Field_Layout049, .LField_Layout049_ANIMATION
	layout_offset Field_Layout049
	layout_offset Field_Layout049, .LField_Layout049_MARKERS
.LField_Layout049_METATILES:
	.incbin "games/tbs/src/layout/layout_049/metatiles.delta2.lz"
	.balign 4, 0
.LField_Layout049_SHAPES:
	.incbin "games/tbs/src/layout/layout_049/shapes.table.lz"
	.balign 4, 0
.LField_Layout049_GRID:
	.incbin "games/tbs/src/layout/layout_049/map.grid.lz"
	.balign 4, 0
.LField_Layout049_ANIMATION:
	.incbin "games/tbs/src/layout/layout_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Layout049_MARKERS:
	treasure_spot 35, 28, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set39Palette
Tileset_Set39Palette:
	.incbin "games/tbs/src/graphics/tileset/set_39/palette.gbapal.lz"
	.byte 0xdc, 0x32	@ PACKER: left in the buffer by Field_Layout049+0x18e
	.global Tileset_Set39TilesA
Tileset_Set39TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_39/tiles_a.4bpp.mtf"
	.byte 0x71	@ PACKER: left in the buffer by Tileset_Set37TilesC+0x22b7
