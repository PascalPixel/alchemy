	.global Field_LamakanDesertSmog
Field_LamakanDesertSmog:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0x7f, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_LamakanDesertSmog, .LField_LamakanDesertSmog_CELL_PICTURES
	map_offset Field_LamakanDesertSmog, .LField_LamakanDesertSmog_SHAPES
	map_offset Field_LamakanDesertSmog, .LField_LamakanDesertSmog_GRID
	map_offset Field_LamakanDesertSmog, .LField_LamakanDesertSmog_ANIMATION
	map_offset Field_LamakanDesertSmog
	map_offset Field_LamakanDesertSmog, .LField_LamakanDesertSmog_MARKERS
.LField_LamakanDesertSmog_CELL_PICTURES:
	.incbin "games/tbs/src/map/lamakan_desert_smog/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_LamakanDesertSmog_SHAPES:
	.incbin "games/tbs/src/map/lamakan_desert_smog/shapes.table.lz"
	.balign 4, 0
.LField_LamakanDesertSmog_GRID:
	.incbin "games/tbs/src/map/lamakan_desert_smog/map.grid.lz"
	.balign 4, 0
.LField_LamakanDesertSmog_ANIMATION:
	.incbin "games/tbs/src/map/lamakan_desert_smog/tileset_anims.script.lz"
	.balign 4, 0
.LField_LamakanDesertSmog_MARKERS:
	treasure_spot 15, 12, 102
	treasure_spot 58, 19, 103
	treasure_spot 40, 25, 104
	treasure_spot 18, 36, 105
	treasure_spot 45, 56, 106
	treasure_spots_end
	.balign 4, 0
