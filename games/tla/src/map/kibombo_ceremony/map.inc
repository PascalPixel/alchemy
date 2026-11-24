	.global Field_KibomboCeremony
Field_KibomboCeremony:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboCeremony, .LField_KibomboCeremony_CELL_TILES
	map_block Field_KibomboCeremony, .LField_KibomboCeremony_SHAPES
	map_block Field_KibomboCeremony, .LField_KibomboCeremony_GRID
	map_block Field_KibomboCeremony, .LField_KibomboCeremony_SHAPE_GRID
	map_block Field_KibomboCeremony, .LField_KibomboCeremony_ANIMATION
	map_block Field_KibomboCeremony, .LField_KibomboCeremony_BLEND
	map_block Field_KibomboCeremony, .LField_KibomboCeremony_SCENE
.LField_KibomboCeremony_CELL_TILES:
	.incbin "games/tla/src/map/kibombo_ceremony/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_KibomboCeremony_SHAPES:
	.incbin "games/tla/src/map/kibombo_ceremony/shapes.table.lz"
	.balign 4, 0
.LField_KibomboCeremony_GRID:
	.incbin "games/tla/src/map/kibombo_ceremony/map.grid.lz"
	.balign 4, 0
.LField_KibomboCeremony_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_ceremony/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboCeremony_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KibomboCeremony_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KibomboCeremony_SCENE:
	.incbin "games/tla/src/map/kibombo_ceremony/scene.scene.lz"
	.global Tileset_KibomboCeremonyPalette
Tileset_KibomboCeremonyPalette:
	.incbin "games/tla/src/graphics/tileset/kibombo_ceremony/palette.gbapal.lz"
	.byte 0xd0, 0xe0	@ PACKER: left in the buffer by Field_KibomboCeremony+0x166
	.global Tileset_KibomboCeremonyTilesA
Tileset_KibomboCeremonyTilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_ceremony/tiles_a.4bpp.mtf"
	.byte 0x0c, 0x09	@ PACKER: left in the buffer by Field_KibomboCeremony+0x2482
	.global Tileset_KibomboCeremonyTilesB
Tileset_KibomboCeremonyTilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_ceremony/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xbc, 0x0e	@ PACKER: left in the buffer by Tileset_KibomboCeremonyTilesA+0x20e5
	.global Tileset_KibomboCeremonyTilesC
Tileset_KibomboCeremonyTilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_ceremony/tiles_c.4bpp.mtf"
	.global Tileset_KibomboCeremonyTilesD
Tileset_KibomboCeremonyTilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_ceremony/tiles_d.4bpp.mtf"
	.byte 0x7c, 0xfe, 0x81	@ PACKER: left in the buffer by Tileset_KibomboCeremonyTilesC+0x3a1
