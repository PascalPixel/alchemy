	.global Field_MadraRoomsCavernHomes
Field_MadraRoomsCavernHomes:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MadraRoomsCavernHomes, .LField_MadraRoomsCavernHomes_CELL_TILES
	map_block Field_MadraRoomsCavernHomes, .LField_MadraRoomsCavernHomes_SHAPES
	map_block Field_MadraRoomsCavernHomes, .LField_MadraRoomsCavernHomes_GRID
	map_block Field_MadraRoomsCavernHomes, .LField_MadraRoomsCavernHomes_SHAPE_GRID
	map_block Field_MadraRoomsCavernHomes, .LField_MadraRoomsCavernHomes_ANIMATION
	map_block Field_MadraRoomsCavernHomes, .LField_MadraRoomsCavernHomes_BLEND
	map_block Field_MadraRoomsCavernHomes, .LField_MadraRoomsCavernHomes_SCENE
.LField_MadraRoomsCavernHomes_CELL_TILES:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MadraRoomsCavernHomes_SHAPES:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/shapes.table.lz"
	.balign 4, 0
.LField_MadraRoomsCavernHomes_GRID:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/map.grid.lz"
	.balign 4, 0
.LField_MadraRoomsCavernHomes_SHAPE_GRID:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/shape_grid.plane.lz"
	.balign 4, 0
.LField_MadraRoomsCavernHomes_ANIMATION:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/animation.script.lz"
	.balign 4, 0
.LField_MadraRoomsCavernHomes_BLEND:
	.incbin "games/tla/src/map/daila_rooms_houses/blend.script.lz"
	.balign 4, 0
.LField_MadraRoomsCavernHomes_SCENE:
	.incbin "games/tla/src/map/madra_rooms_cavern_homes/scene.scene.lz"
	.balign 4, 0
	.global Tileset_MadraRoomsCavernHomesPalette
Tileset_MadraRoomsCavernHomesPalette:
	.incbin "games/tla/src/graphics/tileset/madra_rooms_cavern_homes/palette.gbapal.lz"
	.byte 0xc7	@ PACKER: left in the buffer by Field_MadraRoomsCavernHomes+0x177
	.global Tileset_MadraRoomsCavernHomesTilesA
Tileset_MadraRoomsCavernHomesTilesA:
	.incbin "games/tla/src/graphics/tileset/madra_rooms_cavern_homes/tiles_a.4bpp.mtf"
	.byte 0x08, 0x0a	@ PACKER: left in the buffer by Field_MadraRoomsCavernHomes+0x14be
	.global Tileset_MadraRoomsCavernHomesTilesB
Tileset_MadraRoomsCavernHomesTilesB:
	.incbin "games/tla/src/graphics/tileset/madra_rooms_cavern_homes/tiles_b.4bpp.mtf"
	.byte 0x3c, 0x00	@ PACKER: left in the buffer by Field_MadraRoomsCavernHomes+0x1802
	.global Tileset_MadraRoomsCavernHomesTilesC
Tileset_MadraRoomsCavernHomesTilesC:
	.incbin "games/tla/src/graphics/tileset/madra_rooms_cavern_homes/tiles_c.4bpp.mtf"
	.ifdef TLA_EDITION_JA
	.byte 0x1d, 0x1e	@ PACKER: left in the buffer by Field_MadraRoomsCavernHomes+0x1976
	.else
	.byte 0x01, 0x00	@ PACKER: unused post-stream residue; Decompress_MoveToFrontLz uses decoded RAM and output length; writer unknown
	.endif
	.global Tileset_MadraRoomsCavernHomesTilesD
Tileset_MadraRoomsCavernHomesTilesD:
	.incbin "games/tla/src/graphics/tileset/madra_rooms_cavern_homes/tiles_d.4bpp.mtf"
	.byte 0x22, 0xb9	@ PACKER: left in the buffer by Tileset_MadraRoomsCavernHomesTilesC+0xf1e
