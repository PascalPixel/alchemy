@ Field_Map269: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map269
Field_Map269:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map269, .LField_Map269_METATILES
	map_block Field_Map269, .LField_Map269_SHAPES
	map_block Field_Map269, .LField_Map269_GRID
	map_block Field_Map269, .LField_Map269_SHAPE_GRID
	map_block Field_Map269, .LField_Map269_ANIMATION
	map_block Field_Map269, .LField_Map269_BLEND
	map_block Field_Map269, .LField_Map269_SCENE
.LField_Map269_METATILES:
	.incbin "MAP/MAP_269/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map269_SHAPES:
	.incbin "MAP/MAP_269/SHAPES.table.lz"
	.balign 4, 0
.LField_Map269_GRID:
	.incbin "MAP/MAP_269/GRID.parts.lz"
	.balign 4, 0
.LField_Map269_SHAPE_GRID:
	.incbin "MAP/MAP_269/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map269_ANIMATION:
	.incbin "MAP/MAP_259/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map269_BLEND:
	.incbin "MAP/MAP_260/BLEND.script.lz"
	.balign 4, 0
.LField_Map269_SCENE:
	.incbin "MAP/MAP_269/SCENE.parts.lz"
