	.global Field_CrossboneIsleFloatingLogs
Field_CrossboneIsleFloatingLogs:
	.ifdef TBS_EDITION_ES
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_CELL_PICTURES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_SHAPES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_GRID
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_ANIMATION
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_BLEND
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_MARKERS
.LField_CrossboneIsleFloatingLogs_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/map_es.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_BLEND:
	.incbin "games/tbs/src/map/bilibin_treasury/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_FR
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_CELL_PICTURES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_SHAPES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_GRID
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_ANIMATION
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_BLEND
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_MARKERS
.LField_CrossboneIsleFloatingLogs_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/map_es.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_BLEND:
	.incbin "games/tbs/src/map/bilibin_treasury/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.else
	.ifdef TBS_EDITION_IT
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_CELL_PICTURES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_SHAPES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_GRID
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_ANIMATION
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_BLEND
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_MARKERS
.LField_CrossboneIsleFloatingLogs_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/map_es.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_BLEND:
	.incbin "games/tbs/src/map/bilibin_treasury/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.else
	map_header 0, 0, 80, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_CELL_PICTURES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_SHAPES
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_GRID
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_ANIMATION
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_BLEND
	map_offset Field_CrossboneIsleFloatingLogs, .LField_CrossboneIsleFloatingLogs_MARKERS
.LField_CrossboneIsleFloatingLogs_CELL_PICTURES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_SHAPES:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/shapes.table.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_GRID:
	.incbin "games/tbs/src/map/crossbone_isle_floating_logs/map.grid.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_ANIMATION:
	.incbin "games/tbs/src/map/bilibin_treasury/tileset_anims.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_BLEND:
	.incbin "games/tbs/src/map/bilibin_treasury/blend.script.lz"
	.balign 4, 0
.LField_CrossboneIsleFloatingLogs_MARKERS:
	treasure_spot 12, 3, 102
	treasure_spot 26, 4, 103
	treasure_spot 18, 14, 101
	treasure_spot 28, 15, 100
	treasure_spots_end
	.endif
	.endif
	.endif
	.balign 4, 0
	.global Tileset_PaletteCrossboneIsleFloatingLogs
Tileset_PaletteCrossboneIsleFloatingLogs:
	.incbin "games/tbs/src/graphics/tileset/palettes/crossbone_isle_floating_logs.gbapal.lz"
	.byte 0xfe, 0xa6	@ PACKER: left in the buffer by Field_CrossboneIsleFloatingLogs+0x1ba
	.balign 4, 0
