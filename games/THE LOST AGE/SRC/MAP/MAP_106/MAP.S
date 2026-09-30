@ Field_Map106: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map106
Field_Map106:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map106, .LField_Map106_METATILES
	map_block Field_Map106, .LField_Map106_SHAPES
	map_block Field_Map106, .LField_Map106_GRID
	map_block Field_Map106, .LField_Map106_SHAPE_GRID
	map_block Field_Map106, .LField_Map106_ANIMATION
	map_block Field_Map106, .LField_Map106_BLEND
	map_block Field_Map106, .LField_Map106_SCENE
.LField_Map106_METATILES:
	.incbin "MAP/MAP_106/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map106_SHAPES:
	.incbin "MAP/MAP_106/SHAPES.table.lz"
	.balign 4, 0
.LField_Map106_GRID:
	.incbin "MAP/MAP_106/GRID.parts.lz"
	.balign 4, 0
.LField_Map106_SHAPE_GRID:
	.incbin "MAP/MAP_106/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map106_ANIMATION:
	.incbin "MAP/MAP_096/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map106_BLEND:
	.incbin "COMMON/MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map106_SCENE:
	.incbin "MAP/MAP_106/SCENE.parts.lz"
