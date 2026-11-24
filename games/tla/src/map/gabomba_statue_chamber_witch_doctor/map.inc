	.global Field_GabombaStatueChamberWitchDoctor
Field_GabombaStatueChamberWitchDoctor:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueChamberWitchDoctor, .LField_GabombaStatueChamberWitchDoctor_CELL_TILES
	map_block Field_GabombaStatueChamberWitchDoctor, .LField_GabombaStatueChamberWitchDoctor_SHAPES
	map_block Field_GabombaStatueChamberWitchDoctor, .LField_GabombaStatueChamberWitchDoctor_GRID
	map_block Field_GabombaStatueChamberWitchDoctor, .LField_GabombaStatueChamberWitchDoctor_SHAPE_GRID
	map_block Field_GabombaStatueChamberWitchDoctor, .LField_GabombaStatueChamberWitchDoctor_ANIMATION
	map_block Field_GabombaStatueChamberWitchDoctor, .LField_GabombaStatueChamberWitchDoctor_BLEND
	map_block Field_GabombaStatueChamberWitchDoctor, .LField_GabombaStatueChamberWitchDoctor_SCENE
.LField_GabombaStatueChamberWitchDoctor_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_witch_doctor/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_GabombaStatueChamberWitchDoctor_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_witch_doctor/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueChamberWitchDoctor_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_witch_doctor/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueChamberWitchDoctor_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_witch_doctor/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueChamberWitchDoctor_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamberWitchDoctor_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamberWitchDoctor_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_chamber_witch_doctor/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatueChamberWitchDoctor
Tileset_PaletteGabombaStatueChamberWitchDoctor:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_chamber_witch_doctor.gbapal.lz"
	.byte 0x5c	@ PACKER: left in the buffer by Field_GabombaStatueChamber3+0xd3
