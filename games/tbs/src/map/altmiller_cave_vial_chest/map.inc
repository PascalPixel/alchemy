	.global Field_AltmillerCaveVialChest
Field_AltmillerCaveVialChest:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveVialChest, .LField_AltmillerCaveVialChest_CELL_PICTURES
	map_offset Field_AltmillerCaveVialChest, .LField_AltmillerCaveVialChest_SHAPES
	map_offset Field_AltmillerCaveVialChest, .LField_AltmillerCaveVialChest_GRID
	map_offset Field_AltmillerCaveVialChest, .LField_AltmillerCaveVialChest_ANIMATION
	map_offset Field_AltmillerCaveVialChest, .LField_AltmillerCaveVialChest_BLEND
	map_offset Field_AltmillerCaveVialChest, .LField_AltmillerCaveVialChest_MARKERS
.LField_AltmillerCaveVialChest_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_vial_chest/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltmillerCaveVialChest_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_entrance/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveVialChest_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_vial_chest/map.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveVialChest_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_babi/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveVialChest_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveVialChest_MARKERS:
	treasure_spot 25, 13, 100
	treasure_spots_end
	.balign 4, 0
