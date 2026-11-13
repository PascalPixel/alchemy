	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_RoadToBabiLighthouse
Field_RoadToBabiLighthouse:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_RoadToBabiLighthouse, .LField_RoadToBabiLighthouse_CELL_PICTURES
	map_offset Field_RoadToBabiLighthouse, .LField_RoadToBabiLighthouse_SHAPES
	map_offset Field_RoadToBabiLighthouse, .LField_RoadToBabiLighthouse_GRID
	map_offset Field_RoadToBabiLighthouse, .LField_RoadToBabiLighthouse_ANIMATION
	map_offset Field_RoadToBabiLighthouse, .LField_RoadToBabiLighthouse_BLEND
	map_offset Field_RoadToBabiLighthouse, .LField_RoadToBabiLighthouse_MARKERS
.LField_RoadToBabiLighthouse_CELL_PICTURES:
	.incbin "games/tbs/src/map/road_to_babi_lighthouse/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_RoadToBabiLighthouse_SHAPES:
	.incbin "games/tbs/src/map/road_to_babi_lighthouse/shapes.table.lz"
	.balign 4, 0
.LField_RoadToBabiLighthouse_GRID:
	.incbin "games/tbs/src/map/road_to_babi_lighthouse/map.grid.lz"
	.balign 4, 0
.LField_RoadToBabiLighthouse_ANIMATION:
	.incbin "games/tbs/src/map/kolima/tileset_anims.script.lz"
	.balign 4, 0
.LField_RoadToBabiLighthouse_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_RoadToBabiLighthouse_MARKERS:
	treasure_spot 23, 12, 100
	treasure_spots_end
	.balign 4, 0
