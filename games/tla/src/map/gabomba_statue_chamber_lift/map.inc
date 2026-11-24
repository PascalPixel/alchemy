	.global Field_GabombaStatueChamberLift
Field_GabombaStatueChamberLift:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueChamberLift, .LField_GabombaStatueChamberLift_CELL_TILES
	map_block Field_GabombaStatueChamberLift, .LField_GabombaStatueChamberLift_SHAPES
	map_block Field_GabombaStatueChamberLift, .LField_GabombaStatueChamberLift_GRID
	map_block Field_GabombaStatueChamberLift, .LField_GabombaStatueChamberLift_SHAPE_GRID
	map_block Field_GabombaStatueChamberLift, .LField_GabombaStatueChamberLift_ANIMATION
	map_block Field_GabombaStatueChamberLift, .LField_GabombaStatueChamberLift_BLEND
	map_block Field_GabombaStatueChamberLift, .LField_GabombaStatueChamberLift_SCENE
.LField_GabombaStatueChamberLift_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_lift/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GabombaStatueChamberLift_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_lift/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueChamberLift_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_lift/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueChamberLift_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_lift/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueChamberLift_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamberLift_BLEND:
	.incbin "games/tla/src/map/gabomba_statue_chamber_lift/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamberLift_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_chamber_lift/scene.scene.lz"
	.global Tileset_PaletteGabombaStatueChamberLift
Tileset_PaletteGabombaStatueChamberLift:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_chamber_lift.gbapal.lz"
	.byte 0xcf, 0x39, 0x99	@ PACKER: left in the buffer by Field_GabombaStatueChamber2+0xc5
