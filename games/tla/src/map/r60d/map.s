	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Resource_Data60D
Resource_Data60D:
	map_header 0, 0, 64, 32, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 16, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 32, 16, 16, 0, 0, 0xff, 0xff
	map_block Resource_Data60D, .LResource_Data60D_METATILES
	map_block Resource_Data60D, .LResource_Data60D_SHAPES
	map_block Resource_Data60D, .LResource_Data60D_GRID
	map_block Resource_Data60D, .LResource_Data60D_SHAPE_GRID
	map_block Resource_Data60D, .LResource_Data60D_ANIMATION
	map_block Resource_Data60D, .LResource_Data60D_BLEND
	map_block Resource_Data60D, .LResource_Data60D_SCENE
.LResource_Data60D_METATILES:
	.incbin "games/tla/src/map/r60d/metatiles.delta1.lz"
	.balign 4, 0
.LResource_Data60D_SHAPES:
	.incbin "games/tla/src/map/map_230/shapes.table.lz"
	.balign 4, 0
.LResource_Data60D_GRID:
	.incbin "games/tla/src/map/r60d/grid.parts.lz"
	.balign 4, 0
.LResource_Data60D_SHAPE_GRID:
	.incbin "games/tla/src/map/r60d/shape_grid.plane.lz"
	.balign 4, 0
.LResource_Data60D_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LResource_Data60D_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LResource_Data60D_SCENE:
	.incbin "games/tla/src/map/map_000/scene.parts.lz"
