	.global Field_GabombaStatuePinion
Field_GabombaStatuePinion:
	map_header 0, 0, 128, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatuePinion, .LField_GabombaStatuePinion_CELL_TILES
	map_block Field_GabombaStatuePinion, .LField_GabombaStatuePinion_SHAPES
	map_block Field_GabombaStatuePinion, .LField_GabombaStatuePinion_GRID
	map_block Field_GabombaStatuePinion, .LField_GabombaStatuePinion_SHAPE_GRID
	map_block Field_GabombaStatuePinion, .LField_GabombaStatuePinion_ANIMATION
	map_block Field_GabombaStatuePinion, .LField_GabombaStatuePinion_BLEND
	map_block Field_GabombaStatuePinion, .LField_GabombaStatuePinion_SCENE
.LField_GabombaStatuePinion_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_pinion/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GabombaStatuePinion_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_pinion/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatuePinion_GRID:
	.incbin "games/tla/src/map/gabomba_statue_pinion/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatuePinion_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_pinion/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatuePinion_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatuePinion_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatuePinion_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_pinion/scene.scene.lz"
	.global Tileset_GabombaStatuePinionPalette
Tileset_GabombaStatuePinionPalette:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_pinion/palette.gbapal.lz"
	.byte 0xf0, 0x0c, 0xa6	@ PACKER: left in the buffer by Field_GabombaStatue7+0xc1
	.global Tileset_GabombaStatuePinionTilesA
Tileset_GabombaStatuePinionTilesA:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_pinion/tiles_a.4bpp.mtf"
	.global Tileset_GabombaStatuePinionTilesB
Tileset_GabombaStatuePinionTilesB:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_pinion/tiles_b.4bpp.mtf"
	.global Tileset_GabombaStatuePinionTilesC
Tileset_GabombaStatuePinionTilesC:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_pinion/tiles_c.4bpp.mtf"
	.byte 0xff	@ PACKER: left in the buffer by Tileset_GabombaStatuePinionTilesB+0x293
	.global Tileset_GabombaStatuePinionTilesD
Tileset_GabombaStatuePinionTilesD:
	.incbin "games/tla/src/graphics/tileset/gabomba_statue_pinion/tiles_d.4bpp.mtf"
	.byte 0xf3	@ PACKER: left in the buffer by Tileset_GabombaStatuePinionTilesC+0x13f
