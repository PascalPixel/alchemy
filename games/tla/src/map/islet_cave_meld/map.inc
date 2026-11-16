	.global Field_IsletCaveMeld
Field_IsletCaveMeld:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_IsletCaveMeld, .LField_IsletCaveMeld_CELL_PICTURES
	map_block Field_IsletCaveMeld, .LField_IsletCaveMeld_SHAPES
	map_block Field_IsletCaveMeld, .LField_IsletCaveMeld_GRID
	map_block Field_IsletCaveMeld, .LField_IsletCaveMeld_SHAPE_GRID
	map_block Field_IsletCaveMeld, .LField_IsletCaveMeld_ANIMATION
	map_block Field_IsletCaveMeld, .LField_IsletCaveMeld_BLEND
	map_block Field_IsletCaveMeld, .LField_IsletCaveMeld_SCENE
.LField_IsletCaveMeld_CELL_PICTURES:
	.incbin "games/tla/src/map/islet_cave_meld/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_IsletCaveMeld_SHAPES:
	.incbin "games/tla/src/map/islet_cave_meld/shapes.table.lz"
	.balign 4, 0
.LField_IsletCaveMeld_GRID:
	.incbin "games/tla/src/map/islet_cave_meld/map.grid.lz"
	.balign 4, 0
.LField_IsletCaveMeld_SHAPE_GRID:
	.incbin "games/tla/src/map/islet_cave_meld/shape_grid.plane.lz"
	.balign 4, 0
.LField_IsletCaveMeld_ANIMATION:
	.incbin "games/tla/src/map/islet_cave_meld/animation.script.lz"
	.balign 4, 0
.LField_IsletCaveMeld_BLEND:
	.incbin "games/tla/src/map/yampi_desert_guardian_ring_chest/blend.script.lz"
	.balign 4, 0
.LField_IsletCaveMeld_SCENE:
	.incbin "games/tla/src/map/islet_cave_meld/scene.scene.lz"
	.balign 4, 0
	.global Tileset_IsletCaveMeldPalette
Tileset_IsletCaveMeldPalette:
	.incbin "games/tla/src/graphics/tileset/islet_cave_meld/palette.gbapal.lz"
	.byte 0x47, 0x90	@ PACKER: left in the buffer by Field_IsletCaveMeld+0x166
	.global Tileset_IsletCaveMeldTilesA
Tileset_IsletCaveMeldTilesA:
	.incbin "games/tla/src/graphics/tileset/islet_cave_meld/tiles_a.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Field_LohoRooms+0x2fbf
	.global Tileset_IsletCaveMeldTilesB
Tileset_IsletCaveMeldTilesB:
	.incbin "games/tla/src/graphics/tileset/islet_cave_meld/tiles_b.4bpp.mtf"
	.byte 0xc2, 0x04, 0x38	@ PACKER: left in the buffer by Tileset_IsletCaveMeldTilesA+0x126d
	.global Tileset_IsletCaveMeldTilesC
Tileset_IsletCaveMeldTilesC:
	.incbin "games/tla/src/graphics/tileset/islet_cave_meld/tiles_c.4bpp.mtf"
	.global Tileset_IsletCaveMeldTilesD
Tileset_IsletCaveMeldTilesD:
	.incbin "games/tla/src/graphics/tileset/islet_cave_meld/tiles_d.4bpp.mtf"
	.byte 0x81, 0x03	@ PACKER: left in the buffer by Tileset_IsletCaveMeldTilesA+0x1ac2
