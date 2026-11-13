	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_SuhallaDesertCaves
Field_SuhallaDesertCaves:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_SuhallaDesertCaves, .LField_SuhallaDesertCaves_CELL_PICTURES
	map_offset Field_SuhallaDesertCaves, .LField_SuhallaDesertCaves_SHAPES
	map_offset Field_SuhallaDesertCaves, .LField_SuhallaDesertCaves_GRID
	map_offset Field_SuhallaDesertCaves, .LField_SuhallaDesertCaves_ANIMATION
	map_offset Field_SuhallaDesertCaves, .LField_SuhallaDesertCaves_BLEND
	map_offset Field_SuhallaDesertCaves, .LField_SuhallaDesertCaves_MARKERS
.LField_SuhallaDesertCaves_CELL_PICTURES:
	.incbin "games/tbs/src/map/suhalla_desert_caves/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_SuhallaDesertCaves_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_SuhallaDesertCaves_GRID:
	.incbin "games/tbs/src/map/suhalla_desert_caves/map.grid.lz"
	.balign 4, 0
.LField_SuhallaDesertCaves_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_SuhallaDesertCaves_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_SuhallaDesertCaves_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteSuhallaDesertCaves
Tileset_PaletteSuhallaDesertCaves:
	.incbin "games/tbs/src/graphics/tileset/palettes/suhalla_desert_caves.gbapal.lz"
	.byte 0xb5, 0x03	@ PACKER: left in the buffer by Field_SuhallaDesertCaves+0x172
