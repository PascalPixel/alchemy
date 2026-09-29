@ Field_Map095: a field map.
	.include "games/THE LOST AGE/SRC/MAP/MAP.INC"
	.section .rodata
	.global Field_Map095
Field_Map095:
	.ifdef TLA_EDITION_JA
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_METATILES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_SHAPE_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_SCENE
.LField_Map095_METATILES:
	.incbin "MAP/MAP_095/METATILES_JA.delta0.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "MAP/MAP_095/SHAPES.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "MAP/MAP_095/GRID.parts.lz"
	.balign 4, 0
.LField_Map095_SHAPE_GRID:
	.incbin "MAP/MAP_095/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map095_SCENE:
	.incbin "MAP/MAP_095/SCENE_JA.parts.lz"
	.else
	map_header 0, 0, 60, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 60, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map095, .LField_Map095_METATILES
	map_block Field_Map095, .LField_Map095_SHAPES
	map_block Field_Map095, .LField_Map095_GRID
	map_block Field_Map095, .LField_Map095_SHAPE_GRID
	map_block Field_Map095, .LField_Map095_ANIMATION
	map_block Field_Map095, .LField_Map095_BLEND
	map_block Field_Map095, .LField_Map095_SCENE
.LField_Map095_METATILES:
	.incbin "MAP/MAP_095/METATILES.delta0.lz"
	.balign 4, 0
.LField_Map095_SHAPES:
	.incbin "MAP/MAP_095/SHAPES.table.lz"
	.balign 4, 0
.LField_Map095_GRID:
	.incbin "MAP/MAP_095/GRID.parts.lz"
	.balign 4, 0
.LField_Map095_SHAPE_GRID:
	.incbin "MAP/MAP_095/SHAPE_GRID.plane.lz"
	.balign 4, 0
.LField_Map095_ANIMATION:
	.incbin "MAP/MAP_012/ANIMATION.script.lz"
	.balign 4, 0
.LField_Map095_BLEND:
	.incbin "MAP/MAP_001/BLEND.script.lz"
	.balign 4, 0
.LField_Map095_SCENE:
	.incbin "MAP/MAP_095/SCENE.parts.lz"
	.endif
