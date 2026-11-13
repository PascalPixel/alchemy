	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_AltinPeakJunction
Field_AltinPeakJunction:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltinPeakJunction, .LField_AltinPeakJunction_CELL_PICTURES
	map_offset Field_AltinPeakJunction, .LField_AltinPeakJunction_SHAPES
	map_offset Field_AltinPeakJunction, .LField_AltinPeakJunction_GRID
	map_offset Field_AltinPeakJunction, .LField_AltinPeakJunction_ANIMATION
	map_offset Field_AltinPeakJunction, .LField_AltinPeakJunction_BLEND
	map_offset Field_AltinPeakJunction, .LField_AltinPeakJunction_MARKERS
.LField_AltinPeakJunction_CELL_PICTURES:
	.incbin "games/tbs/src/map/altin_peak_junction/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_AltinPeakJunction_SHAPES:
	.incbin "games/tbs/src/map/altin_peak_junction/shapes.table.lz"
	.balign 4, 0
.LField_AltinPeakJunction_GRID:
	.incbin "games/tbs/src/map/altin_peak_junction/map.grid.lz"
	.balign 4, 0
.LField_AltinPeakJunction_ANIMATION:
	.incbin "games/tbs/src/map/altin_peak_south_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltinPeakJunction_BLEND:
	.incbin "games/tbs/src/map/vault_cave_passage/blend.script.lz"
	.balign 4, 0
.LField_AltinPeakJunction_MARKERS:
	treasure_spot 6, 27, 100
	treasure_spots_end
	.balign 4, 0
