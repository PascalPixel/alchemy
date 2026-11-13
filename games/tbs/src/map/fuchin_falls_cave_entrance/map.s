	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_FuchinFallsCaveEntrance
Field_FuchinFallsCaveEntrance:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_FuchinFallsCaveEntrance, .LField_FuchinFallsCaveEntrance_CELL_PICTURES
	map_offset Field_FuchinFallsCaveEntrance, .LField_FuchinFallsCaveEntrance_SHAPES
	map_offset Field_FuchinFallsCaveEntrance, .LField_FuchinFallsCaveEntrance_GRID
	map_offset Field_FuchinFallsCaveEntrance, .LField_FuchinFallsCaveEntrance_ANIMATION
	map_offset Field_FuchinFallsCaveEntrance, .LField_FuchinFallsCaveEntrance_BLEND
	map_offset Field_FuchinFallsCaveEntrance, .LField_FuchinFallsCaveEntrance_MARKERS
.LField_FuchinFallsCaveEntrance_CELL_PICTURES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_FuchinFallsCaveEntrance_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_FuchinFallsCaveEntrance_GRID:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/map.grid.lz"
	.balign 4, 0
.LField_FuchinFallsCaveEntrance_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveEntrance_BLEND:
	.incbin "games/tbs/src/map/bilibin_treasury/blend.script.lz"
	.balign 4, 0
.LField_FuchinFallsCaveEntrance_MARKERS:
	treasure_spot 18, 27, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteFuchinFallsCave
Tileset_PaletteFuchinFallsCave:
	.incbin "games/tbs/src/graphics/tileset/palettes/fuchin_falls_cave.gbapal.lz"
	.byte 0x7e, 0x5e	@ PACKER: left in the buffer by Field_FuchinFallsCaveEntrance+0x156
