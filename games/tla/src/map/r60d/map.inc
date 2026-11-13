	.global Resource_Data60D
Resource_Data60D:
	map_header 0, 0, 64, 32, 3, 3, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 16, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 32, 16, 16, 0, 0, 0xff, 0xff
	map_block Resource_Data60D, .LResource_Data60D_CELL_PICTURES
	map_block Resource_Data60D, .LResource_Data60D_SHAPES
	map_block Resource_Data60D, .LResource_Data60D_GRID
	map_block Resource_Data60D, .LResource_Data60D_SHAPE_GRID
	map_block Resource_Data60D, .LResource_Data60D_ANIMATION
	map_block Resource_Data60D, .LResource_Data60D_BLEND
	map_block Resource_Data60D, .LResource_Data60D_SCENE
.LResource_Data60D_CELL_PICTURES:
	.incbin "games/tla/src/map/r60d/cell_pictures.delta1.lz"
	.balign 4, 0
.LResource_Data60D_SHAPES:
	.incbin "games/tla/src/map/map_230/shapes.table.lz"
	.balign 4, 0
.LResource_Data60D_GRID:
	.incbin "games/tla/src/map/r60d/map.grid.lz"
	.balign 4, 0
.LResource_Data60D_SHAPE_GRID:
	.incbin "games/tla/src/map/r60d/shape_grid.plane.lz"
	.balign 4, 0
.LResource_Data60D_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LResource_Data60D_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LResource_Data60D_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set120Palette
Tileset_Set120Palette:
	.incbin "games/tla/src/graphics/tileset/set_120/palette.gbapal.lz"
	.global Tileset_Set120TilesA
Tileset_Set120TilesA:
	.incbin "games/tla/src/graphics/tileset/set_120/tiles_a.4bpp.mtf"
	.global Tileset_Set120TilesB
Tileset_Set120TilesB:
	.incbin "games/tla/src/graphics/tileset/set_120/tiles_b.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_Set120TilesA+0x13f
	.global Tileset_Set120TilesC
Tileset_Set120TilesC:
	.incbin "games/tla/src/graphics/tileset/set_120/tiles_c.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_Set120TilesB+0x13f
	.global Tileset_Set120TilesD
Tileset_Set120TilesD:
	.incbin "games/tla/src/graphics/tileset/set_120/tiles_d.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_Set120TilesC+0x13f
