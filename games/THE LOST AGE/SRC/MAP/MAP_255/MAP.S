@ Field_Map255: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map255
Field_Map255:
	map_header 0, 0, 90, 72, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 72, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map255, .LField_Map255_METATILES
	map_block Field_Map255, .LField_Map255_SHAPES
	map_block Field_Map255, .LField_Map255_GRID
	map_block Field_Map255, .LField_Map255_SHAPE_GRID
	map_block Field_Map255, .LField_Map255_ANIMATION
	map_block Field_Map255, .LField_Map255_BLEND
	map_block Field_Map255, .LField_Map255_SCENE
.LField_Map255_METATILES:
	.incbin "MAP/MAP_255/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map255_SHAPES:
	.incbin "MAP/MAP_255/SHAPES.table.lz"
	.balign 4, 0
.LField_Map255_GRID:
	.incbin "MAP/MAP_255/GRID.parts.lz"
	.balign 4, 0
.LField_Map255_SHAPE_GRID:
	.incbin "MAP/MAP_255/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map255_ANIMATION:
	.incbin "MAP/MAP_255/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map255_BLEND:
	.incbin "MAP/MAP_255/BLEND.script.lz"
	.balign 4, 0
.LField_Map255_SCENE:
	.incbin "MAP/MAP_255/SCENE.parts.lz"
