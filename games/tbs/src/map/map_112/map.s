	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map112
Field_Map112:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map112, .LField_Map112_METATILES
	map_block Field_Map112, .LField_Map112_SHAPES
	map_block Field_Map112, .LField_Map112_GRID
	map_block Field_Map112, .LField_Map112_ANIMATION
	map_block Field_Map112
	map_block Field_Map112, .LField_Map112_MARKERS
.LField_Map112_METATILES:
	.incbin "map/map_112/metatiles.delta0.lz"
	.balign 4, 0
.LField_Map112_SHAPES:
	.incbin "map/map_112/shapes.table.lz"
	.balign 4, 0
.LField_Map112_GRID:
	.incbin "map/map_112/grid.parts.lz"
	.balign 4, 0
.LField_Map112_ANIMATION:
	.incbin "map/map_112/animation.script.lz"
	.balign 4, 0
.LField_Map112_MARKERS:
	.incbin "map/map_112/markers.table"
	.balign 4, 0
	.global Tileset_Palette17
Tileset_Palette17:
	.incbin "graphics/tileset/palettes/pal_17.gbapal.lz"
	.byte 0x53	@ PACKER: left in the buffer by Field_Map112+0x12b
	.global Tileset_Palette18
Tileset_Palette18:
	.incbin "graphics/tileset/palettes/pal_18.gbapal.lz"
	.byte 0x00, 0x00, 0x38	@ PACKER: unused post-stream residue; Map_LoadLayeredScene copies decoded palette RAM; writer unknown
