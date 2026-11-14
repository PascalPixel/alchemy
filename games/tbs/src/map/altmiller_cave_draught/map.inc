	.global Field_AltmillerCaveDraught
Field_AltmillerCaveDraught:
	.ifeq EDITION_INTERNATIONAL
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_CELL_PICTURES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_SHAPES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_GRID
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_ANIMATION
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_BLEND
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_MARKERS
.LField_AltmillerCaveDraught_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/cell_pictures_ja.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_draught/map_ja.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_CELL_PICTURES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_SHAPES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_GRID
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_ANIMATION
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_BLEND
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_MARKERS
.LField_AltmillerCaveDraught_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_draught/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_CELL_PICTURES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_SHAPES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_GRID
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_ANIMATION
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_BLEND
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_MARKERS
.LField_AltmillerCaveDraught_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_draught/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_CELL_PICTURES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_SHAPES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_GRID
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_ANIMATION
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_BLEND
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_MARKERS
.LField_AltmillerCaveDraught_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_draught/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_CELL_PICTURES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_SHAPES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_GRID
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_ANIMATION
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_BLEND
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_MARKERS
.LField_AltmillerCaveDraught_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_draught/map_de.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.else
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_CELL_PICTURES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_SHAPES
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_GRID
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_ANIMATION
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_BLEND
	map_offset Field_AltmillerCaveDraught, .LField_AltmillerCaveDraught_MARKERS
.LField_AltmillerCaveDraught_CELL_PICTURES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_SHAPES:
	.incbin "games/tbs/src/map/altmiller_cave_draught/shapes.table.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_GRID:
	.incbin "games/tbs/src/map/altmiller_cave_draught/map.grid.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_ANIMATION:
	.incbin "games/tbs/src/map/altmiller_cave_squall/tileset_anims.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_AltmillerCaveDraught_MARKERS:
	treasure_spot 11, 7, 100
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
