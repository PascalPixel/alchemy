	.global Field_DehkanPlateauTightropes
Field_DehkanPlateauTightropes:
	map_header 0, 0, 128, 96, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_DehkanPlateauTightropes, .LField_DehkanPlateauTightropes_CELL_TILES
	map_block Field_DehkanPlateauTightropes, .LField_DehkanPlateauTightropes_SHAPES
	map_block Field_DehkanPlateauTightropes, .LField_DehkanPlateauTightropes_GRID
	map_block Field_DehkanPlateauTightropes, .LField_DehkanPlateauTightropes_SHAPE_GRID
	map_block Field_DehkanPlateauTightropes, .LField_DehkanPlateauTightropes_ANIMATION
	map_block Field_DehkanPlateauTightropes, .LField_DehkanPlateauTightropes_BLEND
	map_block Field_DehkanPlateauTightropes, .LField_DehkanPlateauTightropes_SCENE
.LField_DehkanPlateauTightropes_CELL_TILES:
	.incbin "games/tla/src/map/dehkan_plateau_tightropes/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_DehkanPlateauTightropes_SHAPES:
	.incbin "games/tla/src/map/dehkan_plateau_tightropes/shapes.table.lz"
	.balign 4, 0
.LField_DehkanPlateauTightropes_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_tightropes/map.grid.lz"
	.balign 4, 0
.LField_DehkanPlateauTightropes_SHAPE_GRID:
	.incbin "games/tla/src/map/dehkan_plateau_tightropes/shape_grid.plane.lz"
	.balign 4, 0
.LField_DehkanPlateauTightropes_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_DehkanPlateauTightropes_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_DehkanPlateauTightropes_SCENE:
	.incbin "games/tla/src/map/dehkan_plateau_tightropes/scene.scene.lz"
	.balign 4, 0
	.global Tileset_DehkanPlateauTightropesPalette
Tileset_DehkanPlateauTightropesPalette:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_tightropes/palette.gbapal.lz"
	.global Tileset_DehkanPlateauTightropesTilesA
Tileset_DehkanPlateauTightropesTilesA:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_tightropes/tiles_a.4bpp.mtf"
	.global Tileset_DehkanPlateauTightropesTilesB
Tileset_DehkanPlateauTightropesTilesB:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_tightropes/tiles_b.4bpp.mtf"
	.byte 0xe7, 0x01	@ PACKER: left in the buffer by Tileset_DehkanPlateauTightropesTilesA+0xfda
	.global Tileset_DehkanPlateauTightropesTilesC
Tileset_DehkanPlateauTightropesTilesC:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_tightropes/tiles_c.4bpp.mtf"
	.byte 0xd4	@ PACKER: left in the buffer by Field_DehkanPlateauTightropes+0x217f
	.global Tileset_DehkanPlateauTightropesTilesD
Tileset_DehkanPlateauTightropesTilesD:
	.incbin "games/tla/src/graphics/tileset/dehkan_plateau_tightropes/tiles_d.4bpp.mtf"
	.byte 0x70	@ PACKER: left in the buffer by Tileset_DehkanPlateauTightropesTilesC+0x13f
