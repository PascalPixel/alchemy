@ Field_Map180: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map180
Field_Map180:
	map_header 0, 0, 128, 128, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 4, -4, 0x1f, 0x1f
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map180, .LField_Map180_METATILES
	map_block Field_Map180, .LField_Map180_SHAPES
	map_block Field_Map180, .LField_Map180_GRID
	map_block Field_Map180, .LField_Map180_SHAPE_GRID
	map_block Field_Map180, .LField_Map180_ANIMATION
	map_block Field_Map180, .LField_Map180_BLEND
	map_block Field_Map180, .LField_Map180_SCENE
.LField_Map180_METATILES:
	.incbin "MAP/MAP_180/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map180_SHAPES:
	.incbin "MAP/MAP_180/SHAPES.table.lz"
	.balign 4, 0
.LField_Map180_GRID:
	.incbin "MAP/MAP_180/GRID.parts.lz"
	.balign 4, 0
.LField_Map180_SHAPE_GRID:
	.incbin "MAP/MAP_180/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map180_ANIMATION:
	.incbin "MAP/MAP_180/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map180_BLEND:
	.incbin "MAP/MAP_180/BLEND.script.lz"
	.balign 4, 0
.LField_Map180_SCENE:
	.incbin "MAP/MAP_180/SCENE.parts.lz"
