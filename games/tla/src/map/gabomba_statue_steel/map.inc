	.global Field_GabombaStatueSteel
Field_GabombaStatueSteel:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueSteel, .LField_GabombaStatueSteel_CELL_TILES
	map_block Field_GabombaStatueSteel, .LField_GabombaStatueSteel_SHAPES
	map_block Field_GabombaStatueSteel, .LField_GabombaStatueSteel_GRID
	map_block Field_GabombaStatueSteel, .LField_GabombaStatueSteel_SHAPE_GRID
	map_block Field_GabombaStatueSteel, .LField_GabombaStatueSteel_ANIMATION
	map_block Field_GabombaStatueSteel, .LField_GabombaStatueSteel_BLEND
	map_block Field_GabombaStatueSteel, .LField_GabombaStatueSteel_SCENE
.LField_GabombaStatueSteel_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_steel/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaStatueSteel_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_steel/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueSteel_GRID:
	.incbin "games/tla/src/map/gabomba_statue_steel/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueSteel_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_steel/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueSteel_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueSteel_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueSteel_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_steel/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatueSteel
Tileset_PaletteGabombaStatueSteel:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_steel.gbapal.lz"
	.byte 0x08	@ PACKER: left in the buffer by Field_GabombaStatueSteel+0xe3
