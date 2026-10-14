	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map050
Field_Map050:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map050, .LField_Map050_METATILES
	map_block Field_Map050, .LField_Map050_SHAPES
	map_block Field_Map050, .LField_Map050_GRID
	map_block Field_Map050, .LField_Map050_ANIMATION
	map_block Field_Map050
	map_block Field_Map050, .LField_Map050_MARKERS
.LField_Map050_METATILES:
	.incbin "games/tbs/src/map/map_050/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map050_SHAPES:
	.incbin "games/tbs/src/map/map_050/shapes.table.lz"
	.balign 4, 0
.LField_Map050_GRID:
	.incbin "games/tbs/src/map/map_050/grid.parts.lz"
	.balign 4, 0
.LField_Map050_ANIMATION:
	.incbin "games/tbs/src/map/map_033/animation.script.lz"
	.balign 4, 0
.LField_Map050_MARKERS:
	.incbin "games/tbs/src/map/map_050/markers.table"
	.global Tileset_Set40Palette
Tileset_Set40Palette:
	.incbin "games/tbs/src/graphics/tileset/set_40/palette.gbapal.lz"
	.global Tileset_Set40TilesA
Tileset_Set40TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_40/tiles_a.4bpp.mtf"
	.byte 0x45, 0x58	@ PACKER: left in the buffer by Tileset_Set36TilesB+0x2966
