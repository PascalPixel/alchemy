	.global Field_ChampaCliffCavesBriggs
Field_ChampaCliffCavesBriggs:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ChampaCliffCavesBriggs, .LField_ChampaCliffCavesBriggs_CELL_TILES
	map_block Field_ChampaCliffCavesBriggs, .LField_ChampaCliffCavesBriggs_SHAPES
	map_block Field_ChampaCliffCavesBriggs, .LField_ChampaCliffCavesBriggs_GRID
	map_block Field_ChampaCliffCavesBriggs, .LField_ChampaCliffCavesBriggs_SHAPE_GRID
	map_block Field_ChampaCliffCavesBriggs, .LField_ChampaCliffCavesBriggs_ANIMATION
	map_block Field_ChampaCliffCavesBriggs, .LField_ChampaCliffCavesBriggs_BLEND
	map_block Field_ChampaCliffCavesBriggs, .LField_ChampaCliffCavesBriggs_SCENE
.LField_ChampaCliffCavesBriggs_CELL_TILES:
	.incbin "games/tla/src/map/champa_cliff_caves_briggs/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_ChampaCliffCavesBriggs_SHAPES:
	.incbin "games/tla/src/map/champa_cliff_caves_briggs/shapes.table.lz"
	.balign 4, 0
.LField_ChampaCliffCavesBriggs_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_briggs/map.grid.lz"
	.balign 4, 0
.LField_ChampaCliffCavesBriggs_SHAPE_GRID:
	.incbin "games/tla/src/map/champa_cliff_caves_briggs/shape_grid.plane.lz"
	.balign 4, 0
.LField_ChampaCliffCavesBriggs_ANIMATION:
	.incbin "games/tla/src/map/champa_cliff_caves_briggs/animation.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesBriggs_BLEND:
	.incbin "games/tla/src/map/garoh_tunnels/blend.script.lz"
	.balign 4, 0
.LField_ChampaCliffCavesBriggs_SCENE:
	.incbin "games/tla/src/map/champa_cliff_caves_briggs/scene.scene.lz"
	.balign 4, 0
	.global Tileset_ChampaCliffCavesBriggsPalette
Tileset_ChampaCliffCavesBriggsPalette:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_briggs/palette.gbapal.lz"
	.byte 0x78	@ PACKER: left in the buffer by Field_ChampaCliffCaves1+0x15f
	.global Tileset_ChampaCliffCavesBriggsTilesA
Tileset_ChampaCliffCavesBriggsTilesA:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_briggs/tiles_a.4bpp.mtf"
	.byte 0xe6	@ PACKER: left in the buffer by Field_AquaRockMoai+0x2aa3
	.global Tileset_ChampaCliffCavesBriggsTilesB
Tileset_ChampaCliffCavesBriggsTilesB:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_briggs/tiles_b.4bpp.mtf"
	.byte 0xc7, 0x69, 0x1f	@ PACKER: left in the buffer by Tileset_ChampaCliffCavesBriggsTilesA+0x21ad
	.global Tileset_ChampaCliffCavesBriggsTilesC
Tileset_ChampaCliffCavesBriggsTilesC:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_briggs/tiles_c.4bpp.mtf"
	.byte 0x92, 0xe5	@ PACKER: left in the buffer by Tileset_ChampaCliffCavesBriggsTilesB+0x1e76
	.global Tileset_ChampaCliffCavesBriggsTilesD
Tileset_ChampaCliffCavesBriggsTilesD:
	.incbin "games/tla/src/graphics/tileset/champa_cliff_caves_briggs/tiles_d.4bpp.mtf"
	.byte 0x11	@ PACKER: left in the buffer by Tileset_ChampaCliffCavesBriggsTilesC+0x13f
