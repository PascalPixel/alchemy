	.global Field_KibomboStatueFace
Field_KibomboStatueFace:
	map_header 0, 0, 128, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboStatueFace, .LField_KibomboStatueFace_CELL_TILES
	map_block Field_KibomboStatueFace, .LField_KibomboStatueFace_SHAPES
	map_block Field_KibomboStatueFace, .LField_KibomboStatueFace_GRID
	map_block Field_KibomboStatueFace, .LField_KibomboStatueFace_SHAPE_GRID
	map_block Field_KibomboStatueFace, .LField_KibomboStatueFace_ANIMATION
	map_block Field_KibomboStatueFace, .LField_KibomboStatueFace_BLEND
	map_block Field_KibomboStatueFace, .LField_KibomboStatueFace_SCENE
.LField_KibomboStatueFace_CELL_TILES:
	.incbin "games/tla/src/map/kibombo_statue_face/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_KibomboStatueFace_SHAPES:
	.incbin "games/tla/src/map/kibombo_statue_face/shapes.table.lz"
	.balign 4, 0
.LField_KibomboStatueFace_GRID:
	.incbin "games/tla/src/map/kibombo_statue_face/map.grid.lz"
	.balign 4, 0
.LField_KibomboStatueFace_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_statue_face/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboStatueFace_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KibomboStatueFace_BLEND:
	.incbin "games/tla/src/map/gabomba_statue_arrows/blend.script.lz"
	.balign 4, 0
.LField_KibomboStatueFace_SCENE:
	.incbin "games/tla/src/map/kibombo_statue_face/scene.scene.lz"
	.balign 4, 0
	.global Tileset_KibomboStatueFacePalette
Tileset_KibomboStatueFacePalette:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_face/palette.gbapal.lz"
	.global Tileset_KibomboStatueFaceTilesA
Tileset_KibomboStatueFaceTilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_face/tiles_a.4bpp.mtf"
	.byte 0x00, 0x04	@ PACKER: left in the buffer by Field_KibomboStatueFace+0x3d2
	.global Tileset_KibomboStatueFaceTilesB
Tileset_KibomboStatueFaceTilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_face/tiles_b.4bpp.mtf"
	.byte 0x4d, 0xcb, 0x82	@ PACKER: left in the buffer by Tileset_GabombaStatuePinionTilesA+0x1479
	.global Tileset_KibomboStatueFaceTilesC
Tileset_KibomboStatueFaceTilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_face/tiles_c.4bpp.mtf"
	.global Tileset_KibomboStatueFaceTilesD
Tileset_KibomboStatueFaceTilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_face/tiles_d.4bpp.mtf"
	.byte 0xdc	@ PACKER: left in the buffer by Tileset_KibomboStatueFaceTilesC+0x13f
