	.global Field_CavernsAngara
Field_CavernsAngara:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_CavernsAngara, .LField_CavernsAngara_CELL_TILES
	map_block Field_CavernsAngara, .LField_CavernsAngara_SHAPES
	map_block Field_CavernsAngara, .LField_CavernsAngara_GRID
	map_block Field_CavernsAngara, .LField_CavernsAngara_SHAPE_GRID
	map_block Field_CavernsAngara, .LField_CavernsAngara_ANIMATION
	map_block Field_CavernsAngara, .LField_CavernsAngara_BLEND
	map_block Field_CavernsAngara, .LField_CavernsAngara_SCENE
.LField_CavernsAngara_CELL_TILES:
	.incbin "games/tla/src/map/caverns_angara/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_CavernsAngara_SHAPES:
	.incbin "games/tla/src/map/caverns_angara/shapes.table.lz"
	.balign 4, 0
.LField_CavernsAngara_GRID:
	.incbin "games/tla/src/map/caverns_angara/map.grid.lz"
	.balign 4, 0
.LField_CavernsAngara_SHAPE_GRID:
	.incbin "games/tla/src/map/caverns_angara/shape_grid.plane.lz"
	.balign 4, 0
.LField_CavernsAngara_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_CavernsAngara_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_CavernsAngara_SCENE:
	.incbin "games/tla/src/map/caverns_angara/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteCavernsAngara
Tileset_PaletteCavernsAngara:
	.incbin "games/tla/src/graphics/tileset/palettes/caverns_angara.gbapal.lz"
	.byte 0x68, 0xe9, 0x10	@ PACKER: left in the buffer by Field_Caverns4+0x151
