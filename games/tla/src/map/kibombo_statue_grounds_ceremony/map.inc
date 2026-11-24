	.global Field_KibomboStatueGroundsCeremony
Field_KibomboStatueGroundsCeremony:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboStatueGroundsCeremony, .LField_KibomboStatueGroundsCeremony_CELL_TILES
	map_block Field_KibomboStatueGroundsCeremony, .LField_KibomboStatueGroundsCeremony_SHAPES
	map_block Field_KibomboStatueGroundsCeremony, .LField_KibomboStatueGroundsCeremony_GRID
	map_block Field_KibomboStatueGroundsCeremony, .LField_KibomboStatueGroundsCeremony_SHAPE_GRID
	map_block Field_KibomboStatueGroundsCeremony, .LField_KibomboStatueGroundsCeremony_ANIMATION
	map_block Field_KibomboStatueGroundsCeremony, .LField_KibomboStatueGroundsCeremony_BLEND
	map_block Field_KibomboStatueGroundsCeremony, .LField_KibomboStatueGroundsCeremony_SCENE
.LField_KibomboStatueGroundsCeremony_CELL_TILES:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_statue_grounds_ceremony/cell_tiles_ja.delta1.lz"
	.else
	.incbin "games/tla/src/map/kibombo_statue_grounds_ceremony/cell_tiles.delta1.lz"
	.endif
	.balign 4, 0
.LField_KibomboStatueGroundsCeremony_SHAPES:
	.incbin "games/tla/src/map/kibombo_statue_grounds/shapes.table.lz"
	.balign 4, 0
.LField_KibomboStatueGroundsCeremony_GRID:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_statue_grounds_ceremony/map_ja.grid.lz"
	.else
	.incbin "games/tla/src/map/kibombo_statue_grounds_ceremony/map.grid.lz"
	.endif
	.balign 4, 0
.LField_KibomboStatueGroundsCeremony_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_statue_grounds/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboStatueGroundsCeremony_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KibomboStatueGroundsCeremony_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KibomboStatueGroundsCeremony_SCENE:
	.ifeq EDITION_INTERNATIONAL
	.incbin "games/tla/src/map/kibombo_statue_grounds_ceremony/scene_ja.scene.lz"
	.else
	.incbin "games/tla/src/map/kibombo_statue_grounds_ceremony/scene.scene.lz"
	.endif
	.balign 4, 0
	.global Tileset_KibomboStatueGroundsCeremonyPalette
Tileset_KibomboStatueGroundsCeremonyPalette:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds_ceremony/palette.gbapal.lz"
	.byte 0x3c, 0x78, 0xf0	@ PACKER: left in the buffer by Field_KibomboStatueGroundsCeremony+0x165
	.global Tileset_KibomboStatueGroundsCeremonyTilesA
Tileset_KibomboStatueGroundsCeremonyTilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds_ceremony/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_KibomboCeremony+0x27f7
	.global Tileset_KibomboStatueGroundsCeremonyTilesB
Tileset_KibomboStatueGroundsCeremonyTilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds_ceremony/tiles_b.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_KibomboStatueGroundsCeremonyTilesA+0x2033
	.global Tileset_KibomboStatueGroundsCeremonyTilesC
Tileset_KibomboStatueGroundsCeremonyTilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds_ceremony/tiles_c.4bpp.mtf"
	.global Tileset_KibomboStatueGroundsCeremonyTilesD
Tileset_KibomboStatueGroundsCeremonyTilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_statue_grounds_ceremony/tiles_d.4bpp.mtf"
	.byte 0xe0, 0x7c	@ PACKER: left in the buffer by Tileset_KibomboStatueGroundsCeremonyTilesC+0xbae
