	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map279
Field_Map279:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map279, .LField_Map279_METATILES
	map_block Field_Map279, .LField_Map279_SHAPES
	map_block Field_Map279, .LField_Map279_GRID
	map_block Field_Map279, .LField_Map279_SHAPE_GRID
	map_block Field_Map279, .LField_Map279_ANIMATION
	map_block Field_Map279, .LField_Map279_BLEND
	map_block Field_Map279, .LField_Map279_SCENE
.LField_Map279_METATILES:
	.incbin "games/tla/src/map/map_279/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map279_SHAPES:
	.incbin "games/tla/src/map/map_279/shapes.table.lz"
	.balign 4, 0
.LField_Map279_GRID:
	.incbin "games/tla/src/map/map_279/grid.parts.lz"
	.balign 4, 0
.LField_Map279_SHAPE_GRID:
	.incbin "games/tla/src/map/map_279/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map279_ANIMATION:
	.incbin "games/tla/src/map/map_279/animation.script.lz"
	.balign 4, 0
.LField_Map279_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map279_SCENE:
	.incbin "games/tla/src/map/map_279/scene.parts.lz"
	.balign 4, 0
