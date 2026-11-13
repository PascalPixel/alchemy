	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_BilibinCaveEntrance
Field_BilibinCaveEntrance:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BilibinCaveEntrance, .LField_BilibinCaveEntrance_CELL_PICTURES
	map_offset Field_BilibinCaveEntrance, .LField_BilibinCaveEntrance_SHAPES
	map_offset Field_BilibinCaveEntrance, .LField_BilibinCaveEntrance_GRID
	map_offset Field_BilibinCaveEntrance, .LField_BilibinCaveEntrance_ANIMATION
	map_offset Field_BilibinCaveEntrance, .LField_BilibinCaveEntrance_BLEND
	map_offset Field_BilibinCaveEntrance, .LField_BilibinCaveEntrance_MARKERS
.LField_BilibinCaveEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/bilibin_cave_entrance/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_BilibinCaveEntrance_SHAPES:
	.incbin "games/tbs/src/map/bilibin_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_BilibinCaveEntrance_GRID:
	.incbin "games/tbs/src/map/bilibin_cave_entrance/map.grid.lz"
	.balign 4, 0
.LField_BilibinCaveEntrance_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_cave_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_BilibinCaveEntrance_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_BilibinCaveEntrance_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_BilibinCaveEntrancePalette
Tileset_BilibinCaveEntrancePalette:
	.incbin "games/tbs/src/graphics/tileset/bilibin_cave_entrance/palette.gbapal.lz"
	.global Tileset_BilibinCaveEntranceTilesA
Tileset_BilibinCaveEntranceTilesA:
	.incbin "games/tbs/src/graphics/tileset/bilibin_cave_entrance/tiles_a.4bpp.mtf"
	.byte 0x80, 0x03	@ PACKER: left in the buffer by Field_BilibinCaveEntrance+0x13da
