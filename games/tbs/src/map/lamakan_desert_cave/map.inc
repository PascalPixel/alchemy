	.global Field_LamakanDesertCave
Field_LamakanDesertCave:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LamakanDesertCave, .LField_LamakanDesertCave_CELL_PICTURES
	map_offset Field_LamakanDesertCave, .LField_LamakanDesertCave_SHAPES
	map_offset Field_LamakanDesertCave, .LField_LamakanDesertCave_GRID
	map_offset Field_LamakanDesertCave, .LField_LamakanDesertCave_ANIMATION
	map_offset Field_LamakanDesertCave, .LField_LamakanDesertCave_BLEND
	map_offset Field_LamakanDesertCave, .LField_LamakanDesertCave_MARKERS
.LField_LamakanDesertCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/lamakan_desert_cave/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LamakanDesertCave_SHAPES:
	.incbin "games/tbs/src/map/lamakan_desert_cave/shapes.table.lz"
	.balign 4, 0
.LField_LamakanDesertCave_GRID:
	.incbin "games/tbs/src/map/lamakan_desert_cave/map.grid.lz"
	.balign 4, 0
.LField_LamakanDesertCave_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_LamakanDesertCave_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_LamakanDesertCave_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteLamakanDesertCave
Tileset_PaletteLamakanDesertCave:
	.incbin "games/tbs/src/graphics/tileset/palettes/lamakan_desert_cave.gbapal.lz"
	.byte 0x14, 0x26	@ PACKER: left in the buffer by Field_LamakanDesertCave+0x162
	.balign 4, 0
