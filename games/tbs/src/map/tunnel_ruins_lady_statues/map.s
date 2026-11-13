	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_TunnelRuinsLadyStatues
Field_TunnelRuinsLadyStatues:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_TunnelRuinsLadyStatues, .LField_TunnelRuinsLadyStatues_CELL_PICTURES
	map_offset Field_TunnelRuinsLadyStatues, .LField_TunnelRuinsLadyStatues_SHAPES
	map_offset Field_TunnelRuinsLadyStatues, .LField_TunnelRuinsLadyStatues_GRID
	map_offset Field_TunnelRuinsLadyStatues, .LField_TunnelRuinsLadyStatues_ANIMATION
	map_offset Field_TunnelRuinsLadyStatues, .LField_TunnelRuinsLadyStatues_BLEND
	map_offset Field_TunnelRuinsLadyStatues, .LField_TunnelRuinsLadyStatues_MARKERS
.LField_TunnelRuinsLadyStatues_CELL_PICTURES:
	.incbin "games/tbs/src/map/tunnel_ruins_lady_statues/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_TunnelRuinsLadyStatues_SHAPES:
	.incbin "games/tbs/src/map/tunnel_ruins_lady_statues/shapes.table.lz"
	.balign 4, 0
.LField_TunnelRuinsLadyStatues_GRID:
	.incbin "games/tbs/src/map/tunnel_ruins_lady_statues/map.grid.lz"
	.balign 4, 0
.LField_TunnelRuinsLadyStatues_ANIMATION:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/tileset_anims.script.lz"
	.balign 4, 0
.LField_TunnelRuinsLadyStatues_BLEND:
	.incbin "games/tbs/src/map/mercury_lighthouse_entrance/blend.script.lz"
	.balign 4, 0
.LField_TunnelRuinsLadyStatues_MARKERS:
	treasure_spot 49, 43, 101
	treasure_spot 8, 49, 100
	treasure_spots_end
	.balign 4, 0
