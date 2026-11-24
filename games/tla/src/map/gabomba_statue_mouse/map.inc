	.global Field_GabombaStatueMouse
Field_GabombaStatueMouse:
	map_header 0, 0, 100, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueMouse, .LField_GabombaStatueMouse_CELL_TILES
	map_block Field_GabombaStatueMouse, .LField_GabombaStatueMouse_SHAPES
	map_block Field_GabombaStatueMouse, .LField_GabombaStatueMouse_GRID
	map_block Field_GabombaStatueMouse, .LField_GabombaStatueMouse_SHAPE_GRID
	map_block Field_GabombaStatueMouse, .LField_GabombaStatueMouse_ANIMATION
	map_block Field_GabombaStatueMouse, .LField_GabombaStatueMouse_BLEND
	map_block Field_GabombaStatueMouse, .LField_GabombaStatueMouse_SCENE
.LField_GabombaStatueMouse_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_mouse/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaStatueMouse_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_bone_armlet_chest/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueMouse_GRID:
	.incbin "games/tla/src/map/gabomba_statue_mouse/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueMouse_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_mouse/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueMouse_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueMouse_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueMouse_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_mouse/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatueMouse
Tileset_PaletteGabombaStatueMouse:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_mouse.gbapal.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Field_GabombaStatue4+0xe3
