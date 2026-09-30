@ Field_Map089: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map089
Field_Map089:
	.ifdef TLA_EDITION_JA
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map089, .LField_Map089_METATILES
	map_block Field_Map089, .LField_Map089_SHAPES
	map_block Field_Map089, .LField_Map089_GRID
	map_block Field_Map089, .LField_Map089_SHAPE_GRID
	map_block Field_Map089, .LField_Map089_ANIMATION
	map_block Field_Map089, .LField_Map089_BLEND
	map_block Field_Map089, .LField_Map089_SCENE
.LField_Map089_METATILES:
	.incbin "MAP/MAP_089/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "MAP/MAP_089/SHAPES.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.incbin "MAP/MAP_089/GRID_JA.parts.lz"
	.balign 4, 0
.LField_Map089_SHAPE_GRID:
	.incbin "MAP/MAP_089/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "MAP/MAP_052/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "COMMON/MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map089_SCENE:
	.incbin "MAP/MAP_089/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map089, .LField_Map089_METATILES
	map_block Field_Map089, .LField_Map089_SHAPES
	map_block Field_Map089, .LField_Map089_GRID
	map_block Field_Map089, .LField_Map089_SHAPE_GRID
	map_block Field_Map089, .LField_Map089_ANIMATION
	map_block Field_Map089, .LField_Map089_BLEND
	map_block Field_Map089, .LField_Map089_SCENE
.LField_Map089_METATILES:
	.incbin "MAP/MAP_089/METATILES.delta1.lz"
	.balign 4, 0
.LField_Map089_SHAPES:
	.incbin "MAP/MAP_089/SHAPES.table.lz"
	.balign 4, 0
.LField_Map089_GRID:
	.incbin "MAP/MAP_089/GRID.parts.lz"
	.balign 4, 0
.LField_Map089_SHAPE_GRID:
	.incbin "MAP/MAP_089/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map089_ANIMATION:
	.incbin "MAP/MAP_052/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map089_BLEND:
	.incbin "COMMON/MAP/MAP_018/BLEND.script.lz"
	.balign 4, 0
.LField_Map089_SCENE:
	.incbin "MAP/MAP_089/SCENE.parts.lz"
	.endif
