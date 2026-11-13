	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_GatewayCave
Field_GatewayCave:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GatewayCave, .LField_GatewayCave_CELL_PICTURES
	map_offset Field_GatewayCave, .LField_GatewayCave_SHAPES
	map_offset Field_GatewayCave, .LField_GatewayCave_GRID
	map_offset Field_GatewayCave, .LField_GatewayCave_ANIMATION
	map_offset Field_GatewayCave, .LField_GatewayCave_BLEND
	map_offset Field_GatewayCave, .LField_GatewayCave_MARKERS
.LField_GatewayCave_CELL_PICTURES:
	.incbin "games/tbs/src/map/gateway_cave/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GatewayCave_SHAPES:
	.incbin "games/tbs/src/map/gateway_cave/shapes.table.lz"
	.balign 4, 0
.LField_GatewayCave_GRID:
	.incbin "games/tbs/src/map/gateway_cave/map.grid.lz"
	.balign 4, 0
.LField_GatewayCave_ANIMATION:
	.incbin "games/tbs/src/map/gateway_cave/tileset_anims.script.lz"
	.balign 4, 0
.LField_GatewayCave_BLEND:
	.incbin "games/tbs/src/map/gateway_cave/blend.script.lz"
	.balign 4, 0
.LField_GatewayCave_MARKERS:
	treasure_spot 33, 26, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette42
Tileset_Palette42:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_42.gbapal.lz"
	.byte 0x94, 0x1c	@ PACKER: left in the buffer by Field_GatewayCave+0x1b6
