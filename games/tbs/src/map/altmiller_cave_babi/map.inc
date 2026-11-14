	.global Field_AltmillerCaveBabi
Field_AltmillerCaveBabi:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveBabi, .LField_AltmillerCaveBabi_CELL_PICTURES
	map_offset Field_AltmillerCaveBabi, .LField_AltmillerCaveBabi_SHAPES
	map_offset Field_AltmillerCaveBabi, .LField_AltmillerCaveBabi_GRID
	map_offset Field_AltmillerCaveBabi, .LField_AltmillerCaveBabi_ANIMATION
	map_offset Field_AltmillerCaveBabi, .LField_AltmillerCaveBabi_BLEND
	map_offset Field_AltmillerCaveBabi, .LField_AltmillerCaveBabi_MARKERS
.LField_AltmillerCaveBabi_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_babi/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveBabi_SHAPES:
	.incbin "games/tbs/src/map/fuchin_falls_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveBabi_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_babi/map.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveBabi_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_babi/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveBabi_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveBabi_MARKERS:
	treasure_spot 28, 36, 100
	treasure_spots_end
	.balign 4, 0
