	.global Field_GabombaStatueChamber2
Field_GabombaStatueChamber2:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueChamber2, .LField_GabombaStatueChamber2_CELL_PICTURES
	map_block Field_GabombaStatueChamber2, .LField_GabombaStatueChamber2_SHAPES
	map_block Field_GabombaStatueChamber2, .LField_GabombaStatueChamber2_GRID
	map_block Field_GabombaStatueChamber2, .LField_GabombaStatueChamber2_SHAPE_GRID
	map_block Field_GabombaStatueChamber2, .LField_GabombaStatueChamber2_ANIMATION
	map_block Field_GabombaStatueChamber2, .LField_GabombaStatueChamber2_BLEND
	map_block Field_GabombaStatueChamber2, .LField_GabombaStatueChamber2_SCENE
.LField_GabombaStatueChamber2_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_2/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_GabombaStatueChamber2_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_chamber_2/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueChamber2_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_2/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueChamber2_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_chamber_2/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueChamber2_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_2/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamber2_BLEND:
	.incbin "games/tla/src/map/gabomba_statue_chamber_2/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueChamber2_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_chamber_2/scene.scene.lz"
	.global Tileset_PaletteGabombaStatueChamber2
Tileset_PaletteGabombaStatueChamber2:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_chamber_2.gbapal.lz"
	.byte 0xcf, 0x39, 0x99	@ PACKER: left in the buffer by Field_GabombaStatueChamber2+0xc5
