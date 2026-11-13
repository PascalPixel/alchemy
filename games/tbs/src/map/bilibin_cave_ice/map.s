	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_BilibinCaveIce
Field_BilibinCaveIce:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BilibinCaveIce, .LField_BilibinCaveIce_CELL_PICTURES
	map_offset Field_BilibinCaveIce, .LField_BilibinCaveIce_SHAPES
	map_offset Field_BilibinCaveIce, .LField_BilibinCaveIce_GRID
	map_offset Field_BilibinCaveIce, .LField_BilibinCaveIce_ANIMATION
	map_offset Field_BilibinCaveIce
	map_offset Field_BilibinCaveIce, .LField_BilibinCaveIce_MARKERS
.LField_BilibinCaveIce_CELL_PICTURES:
	.incbin "games/tbs/src/map/bilibin_cave_ice/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_BilibinCaveIce_SHAPES:
	.incbin "games/tbs/src/map/bilibin_cave_ice/shapes.table.lz"
	.balign 4, 0
.LField_BilibinCaveIce_GRID:
	.incbin "games/tbs/src/map/bilibin_cave_ice/map.grid.lz"
	.balign 4, 0
.LField_BilibinCaveIce_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_BilibinCaveIce_MARKERS:
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
