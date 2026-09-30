@ Field_Map059: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map059
Field_Map059:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map059, .LField_Map059_METATILES
	map_block Field_Map059, .LField_Map059_SHAPES
	map_block Field_Map059, .LField_Map059_GRID
	map_block Field_Map059, .LField_Map059_SHAPE_GRID
	map_block Field_Map059, .LField_Map059_ANIMATION
	map_block Field_Map059, .LField_Map059_BLEND
	map_block Field_Map059, .LField_Map059_SCENE
.LField_Map059_METATILES:
	.incbin "MAP/MAP_059/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map059_SHAPES:
	.incbin "MAP/MAP_059/SHAPES.table.lz"
	.balign 4, 0
.LField_Map059_GRID:
	.incbin "MAP/MAP_059/GRID.parts.lz"
	.balign 4, 0
.LField_Map059_SHAPE_GRID:
	.incbin "MAP/MAP_059/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map059_ANIMATION:
	.incbin "MAP/MAP_052/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map059_BLEND:
	.incbin "COMMON/MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map059_SCENE:
	.incbin "MAP/MAP_059/SCENE.parts.lz"
