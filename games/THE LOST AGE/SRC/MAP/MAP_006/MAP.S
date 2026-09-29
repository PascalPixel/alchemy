@ Field_Map006: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map006
Field_Map006:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map006, .LField_Map006_METATILES
	map_block Field_Map006, .LField_Map006_SHAPES
	map_block Field_Map006, .LField_Map006_GRID
	map_block Field_Map006, .LField_Map006_SHAPE_GRID
	map_block Field_Map006, .LField_Map006_ANIMATION
	map_block Field_Map006, .LField_Map006_BLEND
	map_block Field_Map006, .LField_Map006_SCENE
.LField_Map006_METATILES:
	.incbin "MAP/MAP_006/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map006_SHAPES:
	.incbin "MAP/MAP_006/SHAPES.table.lz"
	.balign 4, 0
.LField_Map006_GRID:
	.incbin "MAP/MAP_006/GRID.parts.lz"
	.balign 4, 0
.LField_Map006_SHAPE_GRID:
	.incbin "MAP/MAP_006/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map006_ANIMATION:
	.incbin "MAP/MAP_006/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map006_BLEND:
	.incbin "MAP/MAP_006/BLEND.script.lz"
	.balign 4, 0
.LField_Map006_SCENE:
	.incbin "MAP/MAP_006/SCENE.parts.lz"
