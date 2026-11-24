	.global Field_GaiaRockInsideFirstAltar
Field_GaiaRockInsideFirstAltar:
	map_header 0, 0, 130, 112, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GaiaRockInsideFirstAltar, .LField_GaiaRockInsideFirstAltar_CELL_TILES
	map_block Field_GaiaRockInsideFirstAltar, .LField_GaiaRockInsideFirstAltar_SHAPES
	map_block Field_GaiaRockInsideFirstAltar, .LField_GaiaRockInsideFirstAltar_GRID
	map_block Field_GaiaRockInsideFirstAltar, .LField_GaiaRockInsideFirstAltar_SHAPE_GRID
	map_block Field_GaiaRockInsideFirstAltar, .LField_GaiaRockInsideFirstAltar_ANIMATION
	map_block Field_GaiaRockInsideFirstAltar, .LField_GaiaRockInsideFirstAltar_BLEND
	map_block Field_GaiaRockInsideFirstAltar, .LField_GaiaRockInsideFirstAltar_SCENE
.LField_GaiaRockInsideFirstAltar_CELL_TILES:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GaiaRockInsideFirstAltar_SHAPES:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/shapes.table.lz"
	.balign 4, 0
.LField_GaiaRockInsideFirstAltar_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/map.grid.lz"
	.balign 4, 0
.LField_GaiaRockInsideFirstAltar_SHAPE_GRID:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/shape_grid.plane.lz"
	.balign 4, 0
.LField_GaiaRockInsideFirstAltar_ANIMATION:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/animation.script.lz"
	.balign 4, 0
.LField_GaiaRockInsideFirstAltar_BLEND:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/blend.script.lz"
	.balign 4, 0
.LField_GaiaRockInsideFirstAltar_SCENE:
	.incbin "games/tla/src/map/gaia_rock_inside_first_altar/scene.scene.lz"
	.global Tileset_GaiaRockInsideFirstAltarPalette
Tileset_GaiaRockInsideFirstAltarPalette:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_first_altar/palette.gbapal.lz"
	.byte 0x5c, 0x13	@ PACKER: left in the buffer by Field_GaiaRockInside2+0x11e
	.global Tileset_GaiaRockInsideFirstAltarTilesA
Tileset_GaiaRockInsideFirstAltarTilesA:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_first_altar/tiles_a.4bpp.mtf"
	.byte 0x1f	@ PACKER: left in the buffer by Tileset_GaiaRockInsideMazeTilesA+0x1af7
	.global Tileset_GaiaRockInsideFirstAltarTilesB
Tileset_GaiaRockInsideFirstAltarTilesB:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_first_altar/tiles_b.4bpp.mtf"
	.byte 0xa2	@ PACKER: left in the buffer by Tileset_GaiaRockInsideFirstAltarTilesA+0x140b
	.global Tileset_GaiaRockInsideFirstAltarTilesC
Tileset_GaiaRockInsideFirstAltarTilesC:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_first_altar/tiles_c.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_GaiaRockInsideFirstAltarTilesB+0x553
	.global Tileset_GaiaRockInsideFirstAltarTilesD
Tileset_GaiaRockInsideFirstAltarTilesD:
	.incbin "games/tla/src/graphics/tileset/gaia_rock_inside_first_altar/tiles_d.4bpp.mtf"
	.byte 0xb9, 0x40, 0x00	@ PACKER: left in the buffer by Tileset_GaiaRockInsideFirstAltarTilesB+0x639
