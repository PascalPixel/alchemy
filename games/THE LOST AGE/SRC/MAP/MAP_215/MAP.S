@ Field_Map215: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map215
Field_Map215:
	map_header 0, 0, 130, 112, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map215, .LField_Map215_METATILES
	map_block Field_Map215, .LField_Map215_SHAPES
	map_block Field_Map215, .LField_Map215_GRID
	map_block Field_Map215, .LField_Map215_SHAPE_GRID
	map_block Field_Map215, .LField_Map215_ANIMATION
	map_block Field_Map215, .LField_Map215_BLEND
	map_block Field_Map215, .LField_Map215_SCENE
.LField_Map215_METATILES:
	.incbin "MAP/MAP_215/METATILES.delta2.lz"
	.balign 4, 0
.LField_Map215_SHAPES:
	.incbin "MAP/MAP_215/SHAPES.table.lz"
	.balign 4, 0
.LField_Map215_GRID:
	.incbin "MAP/MAP_215/GRID.parts.lz"
	.balign 4, 0
.LField_Map215_SHAPE_GRID:
	.incbin "MAP/MAP_215/SHAPE_GRID.table.lz"
	.balign 4, 0
.LField_Map215_ANIMATION:
	.incbin "MAP/MAP_215/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map215_BLEND:
	.incbin "MAP/MAP_215/BLEND.script.lz"
	.balign 4, 0
.LField_Map215_SCENE:
	.incbin "MAP/MAP_215/SCENE.parts.lz"
