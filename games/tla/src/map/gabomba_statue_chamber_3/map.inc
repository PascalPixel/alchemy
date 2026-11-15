	.global Field_GabombaStatueChamber3
Field_GabombaStatueChamber3:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueChamber3, .LField_GabombaStatueChamber3_CELL_PICTURES
	map_block Field_GabombaStatueChamber3, .LField_GabombaStatueChamber3_SHAPES
	map_block Field_GabombaStatueChamber3, .LField_GabombaStatueChamber3_GRID
	map_block Field_GabombaStatueChamber3, .LField_GabombaStatueChamber3_SHAPE_GRID
	map_block Field_GabombaStatueChamber3, .LField_GabombaStatueChamber3_ANIMATION
	map_block Field_GabombaStatueChamber3, .LField_GabombaStatueChamber3_BLEND
	map_block Field_GabombaStatueChamber3, .LField_GabombaStatueChamber3_SCENE
.LField_GabombaStatueChamber3_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_3/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_GabombaStatueChamber3_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_3/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueChamber3_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_3/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueChamber3_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_3/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueChamber3_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamber3_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamber3_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_chamber_3/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatueChamber3
Tileset_PaletteGabombaStatueChamber3:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_chamber_3.gbapal.lz"
	.byte 0x5c	@ PACKER: left in the buffer by Field_GabombaStatueChamber3+0xd3
