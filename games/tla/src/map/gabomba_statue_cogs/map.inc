	.global Field_GabombaStatueCogs
Field_GabombaStatueCogs:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueCogs, .LField_GabombaStatueCogs_CELL_TILES
	map_block Field_GabombaStatueCogs, .LField_GabombaStatueCogs_SHAPES
	map_block Field_GabombaStatueCogs, .LField_GabombaStatueCogs_GRID
	map_block Field_GabombaStatueCogs, .LField_GabombaStatueCogs_SHAPE_GRID
	map_block Field_GabombaStatueCogs, .LField_GabombaStatueCogs_ANIMATION
	map_block Field_GabombaStatueCogs, .LField_GabombaStatueCogs_BLEND
	map_block Field_GabombaStatueCogs, .LField_GabombaStatueCogs_SCENE
.LField_GabombaStatueCogs_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_cogs/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GabombaStatueCogs_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_steel/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueCogs_GRID:
	.incbin "games/tla/src/map/gabomba_statue_cogs/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueCogs_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_cogs/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueCogs_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueCogs_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueCogs_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_cogs/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatueCogs
Tileset_PaletteGabombaStatueCogs:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_cogs.gbapal.lz"
	.byte 0x29	@ PACKER: left in the buffer by Field_GabombaStatue5+0xe3
