	.global Field_KibomboStatueGrounds
Field_KibomboStatueGrounds:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboStatueGrounds, .LField_KibomboStatueGrounds_CELL_TILES
	map_block Field_KibomboStatueGrounds, .LField_KibomboStatueGrounds_SHAPES
	map_block Field_KibomboStatueGrounds, .LField_KibomboStatueGrounds_GRID
	map_block Field_KibomboStatueGrounds, .LField_KibomboStatueGrounds_SHAPE_GRID
	map_block Field_KibomboStatueGrounds, .LField_KibomboStatueGrounds_ANIMATION
	map_block Field_KibomboStatueGrounds, .LField_KibomboStatueGrounds_BLEND
	map_block Field_KibomboStatueGrounds, .LField_KibomboStatueGrounds_SCENE
.LField_KibomboStatueGrounds_CELL_TILES:
	.incbin "games/tla/src/map/kibombo_statue_grounds/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_KibomboStatueGrounds_SHAPES:
	.incbin "games/tla/src/map/kibombo_statue_grounds/shapes.table.lz"
	.balign 4, 0
.LField_KibomboStatueGrounds_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_statue_grounds/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/kibombo_statue_grounds/map.grid.lz"
	.endif
	.balign 4, 0
.LField_KibomboStatueGrounds_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_statue_grounds/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboStatueGrounds_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KibomboStatueGrounds_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KibomboStatueGrounds_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_statue_grounds/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/kibombo_statue_grounds/scene.scene.lz"
	.endif
	.global Tileset_KibomboStatueGroundsPalette
Tileset_KibomboStatueGroundsPalette:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds/palette.gbapal.lz"
	.byte 0x3c, 0x88, 0x03	@ PACKER: left in the buffer by Field_KibomboStatueGrounds+0x11d
	.global Tileset_KibomboStatueGroundsTilesA
Tileset_KibomboStatueGroundsTilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds/tiles_a.4bpp.mtf"
	.byte 0x5b	@ PACKER: left in the buffer by Tileset_KibomboTilesA+0x28ab
	.global Tileset_KibomboStatueGroundsTilesB
Tileset_KibomboStatueGroundsTilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds/tiles_b.4bpp.mtf"
	.byte 0xc8, 0xf7	@ PACKER: left in the buffer by Tileset_KibomboStatueGroundsTilesA+0x1f92
	.global Tileset_KibomboStatueGroundsTilesC
Tileset_KibomboStatueGroundsTilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds/tiles_c.4bpp.mtf"
	.byte 0xad	@ PACKER: left in the buffer by Tileset_KibomboStatueGroundsTilesB+0x101f
	.global Tileset_KibomboStatueGroundsTilesD
Tileset_KibomboStatueGroundsTilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds/tiles_d.4bpp.mtf"
	.byte 0x05	@ PACKER: left in the buffer by Tileset_KibomboStatueGroundsTilesC+0x13f
