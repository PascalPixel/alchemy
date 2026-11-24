	.global Field_GabombaStatueChamberIdol
Field_GabombaStatueChamberIdol:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueChamberIdol, .LField_GabombaStatueChamberIdol_CELL_TILES
	map_block Field_GabombaStatueChamberIdol, .LField_GabombaStatueChamberIdol_SHAPES
	map_block Field_GabombaStatueChamberIdol, .LField_GabombaStatueChamberIdol_GRID
	map_block Field_GabombaStatueChamberIdol, .LField_GabombaStatueChamberIdol_SHAPE_GRID
	map_block Field_GabombaStatueChamberIdol, .LField_GabombaStatueChamberIdol_ANIMATION
	map_block Field_GabombaStatueChamberIdol, .LField_GabombaStatueChamberIdol_BLEND
	map_block Field_GabombaStatueChamberIdol, .LField_GabombaStatueChamberIdol_SCENE
.LField_GabombaStatueChamberIdol_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_idol/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaStatueChamberIdol_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_idol/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueChamberIdol_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_idol/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueChamberIdol_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_idol/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueChamberIdol_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamberIdol_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamberIdol_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_chamber_idol/scene.scene.lz"
	.global Tileset_PaletteGabombaStatueChamber1
Tileset_PaletteGabombaStatueChamber1:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_chamber_idol.gbapal.lz"
	.byte 0x45	@ PACKER: left in the buffer by Field_GabombaStatueChamberIdol+0xff
