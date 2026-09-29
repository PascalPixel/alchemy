@ Field_Map017: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map017
Field_Map017:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map017, .LField_Map017_METATILES
	map_block Field_Map017, .LField_Map017_SHAPES
	map_block Field_Map017, .LField_Map017_GRID
	map_block Field_Map017, .LField_Map017_SHAPE_GRID
	map_block Field_Map017, .LField_Map017_ANIMATION
	map_block Field_Map017, .LField_Map017_BLEND
	map_block Field_Map017, .LField_Map017_SCENE
.LField_Map017_METATILES:
	.incbin "MAP/MAP_017/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map017_SHAPES:
	.incbin "MAP/MAP_017/SHAPES.table.lz"
	.balign 4, 0
.LField_Map017_GRID:
	.incbin "MAP/MAP_017/GRID.parts.lz"
	.balign 4, 0
.LField_Map017_SHAPE_GRID:
	.incbin "MAP/MAP_017/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map017_ANIMATION:
	.incbin "MAP/MAP_017/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map017_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map017_SCENE:
	.incbin "MAP/MAP_017/SCENE.parts.lz"
