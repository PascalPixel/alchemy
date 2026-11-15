	.global Field_GabombaStatueTurntable
Field_GabombaStatueTurntable:
	map_header 0, 0, 100, 70, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueTurntable, .LField_GabombaStatueTurntable_CELL_PICTURES
	map_block Field_GabombaStatueTurntable, .LField_GabombaStatueTurntable_SHAPES
	map_block Field_GabombaStatueTurntable, .LField_GabombaStatueTurntable_GRID
	map_block Field_GabombaStatueTurntable, .LField_GabombaStatueTurntable_SHAPE_GRID
	map_block Field_GabombaStatueTurntable, .LField_GabombaStatueTurntable_ANIMATION
	map_block Field_GabombaStatueTurntable, .LField_GabombaStatueTurntable_BLEND
	map_block Field_GabombaStatueTurntable, .LField_GabombaStatueTurntable_SCENE
.LField_GabombaStatueTurntable_CELL_PICTURES:
	.incbin "games/tla/src/map/gabomba_statue_turntable/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GabombaStatueTurntable_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_bone_armlet_chest/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueTurntable_GRID:
	.incbin "games/tla/src/map/gabomba_statue_turntable/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueTurntable_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_turntable/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueTurntable_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueTurntable_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueTurntable_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_turntable/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatueTurntable
Tileset_PaletteGabombaStatueTurntable:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_turntable.gbapal.lz"
	.byte 0xe6	@ PACKER: left in the buffer by Field_GabombaStatue4+0xe3
