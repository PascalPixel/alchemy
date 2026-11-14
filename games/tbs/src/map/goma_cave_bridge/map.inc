	.global Field_GomaCaveBridge
Field_GomaCaveBridge:
	.ifdef TBS_EDITION_DE
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_CELL_PICTURES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_SHAPES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_GRID
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_ANIMATION
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_BLEND
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_MARKERS
.LField_GomaCaveBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_bridge/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GomaCaveBridge_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_bridge/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveBridge_GRID:
	.incbin "games/tbs/src/map/goma_cave_bridge/map_de.grid.lz"
	.balign 4, 0
.LField_GomaCaveBridge_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_CELL_PICTURES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_SHAPES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_GRID
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_ANIMATION
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_BLEND
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_MARKERS
.LField_GomaCaveBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_bridge/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GomaCaveBridge_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_bridge/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveBridge_GRID:
	.incbin "games/tbs/src/map/goma_cave_bridge/map_de.grid.lz"
	.balign 4, 0
.LField_GomaCaveBridge_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_CELL_PICTURES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_SHAPES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_GRID
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_ANIMATION
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_BLEND
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_MARKERS
.LField_GomaCaveBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_bridge/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GomaCaveBridge_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_bridge/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveBridge_GRID:
	.incbin "games/tbs/src/map/goma_cave_bridge/map_de.grid.lz"
	.balign 4, 0
.LField_GomaCaveBridge_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_MARKERS:
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_CELL_PICTURES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_SHAPES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_GRID
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_ANIMATION
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_BLEND
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_MARKERS
.LField_GomaCaveBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_bridge/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GomaCaveBridge_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_bridge/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveBridge_GRID:
	.incbin "games/tbs/src/map/goma_cave_bridge/map_de.grid.lz"
	.balign 4, 0
.LField_GomaCaveBridge_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_MARKERS:
	treasure_spots_end
	.else
	map_header 0, 0, 96, 60, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 60, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_CELL_PICTURES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_SHAPES
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_GRID
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_ANIMATION
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_BLEND
	map_offset Field_GomaCaveBridge, .LField_GomaCaveBridge_MARKERS
.LField_GomaCaveBridge_CELL_PICTURES:
	.incbin "games/tbs/src/map/goma_cave_bridge/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_GomaCaveBridge_SHAPES:
	.incbin "games/tbs/src/map/goma_cave_bridge/shapes.table.lz"
	.balign 4, 0
.LField_GomaCaveBridge_GRID:
	.incbin "games/tbs/src/map/goma_cave_bridge/map.grid.lz"
	.balign 4, 0
.LField_GomaCaveBridge_ANIMATION:
	.incbin "games/tbs/src/map/goma_cave_pillar/tileset_anims.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_BLEND:
	.incbin "games/tbs/src/map/goma_cave_pillar/blend.script.lz"
	.balign 4, 0
.LField_GomaCaveBridge_MARKERS:
	treasure_spots_end
	.endif
	.endif
	.endif
	.endif
	.balign 4, 0
