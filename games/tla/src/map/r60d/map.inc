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
	.incbin "games/tla/src/map/anemos_inner_sanctum_4/shapes.table.lz"
	.balign 4, 0
.LResource_Data60D_GRID:
	.incbin "games/tla/src/map/r60d/map.grid.lz"
	.balign 4, 0
.LResource_Data60D_SHAPE_GRID:
	.incbin "games/tla/src/map/r60d/shape_grid.plane.lz"
	.balign 4, 0
.LResource_Data60D_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LResource_Data60D_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LResource_Data60D_SCENE:
	.incbin "games/tla/src/map/map_000/scene.scene.lz"
	.balign 4, 0
	.global Tileset_R60dPalette
Tileset_R60dPalette:
	.incbin "games/tla/src/graphics/tileset/r60d/palette.gbapal.lz"
	.global Tileset_R60dTilesA
Tileset_R60dTilesA:
	.incbin "games/tla/src/graphics/tileset/r60d/tiles_a.4bpp.mtf"
	.global Tileset_R60dTilesB
Tileset_R60dTilesB:
	.incbin "games/tla/src/graphics/tileset/r60d/tiles_b.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_R60dTilesA+0x13f
	.global Tileset_R60dTilesC
Tileset_R60dTilesC:
	.incbin "games/tla/src/graphics/tileset/r60d/tiles_c.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_R60dTilesB+0x13f
	.global Tileset_R60dTilesD
Tileset_R60dTilesD:
	.incbin "games/tla/src/graphics/tileset/r60d/tiles_d.4bpp.mtf"
	.byte 0xe0	@ PACKER: left in the buffer by Tileset_R60dTilesC+0x13f
