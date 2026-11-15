	.global Field_GabombaStatueChamber1
Field_GabombaStatueChamber1:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueChamber1, .LField_GabombaStatueChamber1_CELL_PICTURES
	map_block Field_GabombaStatueChamber1, .LField_GabombaStatueChamber1_SHAPES
	map_block Field_GabombaStatueChamber1, .LField_GabombaStatueChamber1_GRID
	map_block Field_GabombaStatueChamber1, .LField_GabombaStatueChamber1_SHAPE_GRID
	map_block Field_GabombaStatueChamber1, .LField_GabombaStatueChamber1_ANIMATION
	map_block Field_GabombaStatueChamber1, .LField_GabombaStatueChamber1_BLEND
	map_block Field_GabombaStatueChamber1, .LField_GabombaStatueChamber1_SCENE
.LField_GabombaStatueChamber1_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_1/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GabombaStatueChamber1_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_1/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueChamber1_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_1/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueChamber1_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_1/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueChamber1_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamber1_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamber1_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_chamber_1/scene.scene.lz"
	.global Tileset_PaletteGabombaStatueChamber1
Tileset_PaletteGabombaStatueChamber1:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_chamber_1.gbapal.lz"
	.byte 0x45	@ PACKER: left in the buffer by Field_GabombaStatueChamber1+0xff
