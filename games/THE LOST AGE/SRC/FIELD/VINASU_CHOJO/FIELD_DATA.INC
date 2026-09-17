/* VINASU_CHOJO field data: typed from resource_64e (runtime 0x2009ce4-0x200a5ac).
   Assembled through the macros of games/THE LOST AGE/SRC/FIELD/COMMON/FIELD_TABLE.INC. */

FieldActorScript_02001ce4:
	actor_wait 1
	actor_op 0x0d, 0x00000016, 0x00000000
	actor_wait 1
	actor_op 0x02, 0x00000000, 0x00000000, 0x00000000
	actor_wait 1
	actor_halt

FieldActorScript_02001d1c:
	actor_wait 1
	actor_op 0x0d, 0x0000002c, 0x00000000
	actor_wait 1
	actor_op 0x02, 0x00000000, 0x00000000, 0x00000000
	actor_wait 1
	actor_halt

FieldActorScript_02001d54:
	actor_wait 1
	actor_op 0x0d, 0x0000007e, 0x00000000
	actor_wait 1
	actor_op 0x02, 0x00000000, 0x00000000, 0x00000000
	actor_wait 1
	actor_halt

FieldWords_02001d8c:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000

FieldWords_02001dcc:
	.4byte FieldActorScript_02001ce4 + FIELD_OVERLAY_LOAD_BIAS
	.4byte FieldActorScript_02001d1c + FIELD_OVERLAY_LOAD_BIAS
	.4byte FieldActorScript_02001d54 + FIELD_OVERLAY_LOAD_BIAS

FieldActorScript_02001dd8:
	actor_op 0x27, 0x0000000c
	actor_property 0, 0x26, 0x02008571
	actor_label 1
	actor_property 0, 0x09, 0x00011999
	actor_property 0, 0x0a, 0x00011999
	actor_wait 2
	actor_property 0, 0x09, 0x0000e666
	actor_property 0, 0x0a, 0x0000e666
	actor_wait 2
	actor_loop 10, 1
	actor_property 0, 0x09, 0x00010000
	actor_property 0, 0x0a, 0x00010000
	actor_end

FieldEntrances_02001e58:
	field_entrance 0, -1, 172, 0, 149, 0x4000, -1, -1, -1, -1
	field_entrance 1, -1, 120, 0, 200, 0xc000, 0, 56, 240, 216
	field_entrance 2, -1, 120, 0, 104, 0x4000, 0, 56, 240, 216
	field_entrance 3, -1, 344, 0, 232, 0xc000, 256, 48, 512, 248
	field_entrance 4, -1, 472, 0, 184, 0x4000, 256, 48, 512, 248
	field_entrance 5, -1, 376, 0, 184, 0x4000, 256, 48, 512, 248
	field_entrance 6, -1, 344, 0, 104, 0x4000, 256, 48, 512, 248
	field_entrance 7, -1, 840, 0, 248, 0xc000, 672, 32, 1008, 256
	field_entrance 8, -1, 968, 0, 168, 0x4000, 672, 32, 1008, 256
	field_entrance 9, -1, 856, 0, 88, 0x4000, 672, 32, 1008, 256
	field_entrance 10, -1, 808, 0, 88, 0x4000, 672, 32, 1008, 256
	field_entrance 11, -1, 712, 0, 168, 0x4000, 672, 32, 1008, 256
	field_entrance 12, -1, 200, 0, 408, 0xc000, 16, 288, 256, 520
	field_entrance 13, -1, 104, 0, 504, 0xc000, 16, 288, 256, 520
	field_entrance 14, -1, 104, 0, 472, 0x4000, 16, 288, 256, 520
	field_entrance 20, -1, 520, 0, 696, 0x4000, 304, 344, 576, 728
	field_entrance 21, -1, 344, 0, 568, 0x4000, 304, 344, 576, 728
	field_entrance 22, -1, 728, 0, 936, 0x4000, 600, 736, 840, 952
	field_entrance 23, -1, 744, 0, 792, 0x4000, 600, 736, 840, 952
	field_entrance 24, -1, 392, 0, 952, 0x4000, 272, 808, 512, 984
	field_entrance 25, -1, 392, 0, 856, 0x4000, 272, 808, 512, 984
	field_entrance 26, -1, 760, 0, 936, 0x4000, 600, 736, 840, 952
	field_entrance 30, -1, 88, 0, 872, 0x4000, 16, 600, 256, 920
	field_entrance 31, -1, 184, 0, 872, 0x4000, 16, 600, 256, 920
	field_entrance 32, -1, 184, 0, 648, 0x4000, 16, 600, 256, 920
	field_entrance 98, -1, 744, 0, 792, 0x4000, 600, 736, 840, 952
	field_entrance 99, -1, 744, 0, 792, 0x4000, 576, 736, -1, 968
	field_entrance_end

FieldExits_020020f8:
	field_exit_scene 0x005
	field_exit 0x01, 0x004, 9
	field_exit 0x02, 0x005, 3
	field_exit 0x03, 0x005, 2
	field_exit 0x04, 0x005, 31
	field_exit 0x05, 0x005, 30
	field_exit 0x06, 0x005, 7
	field_exit 0x07, 0x005, 6
	field_exit 0x08, 0x005, 32
	field_exit 0x09, 0x005, 22
	field_exit 0x0a, 0x005, 12
	field_exit 0x0b, 0x005, 13
	field_exit 0x0c, 0x005, 10
	field_exit 0x0d, 0x005, 11
	field_exit 0x0e, 0x005, 24
	field_exit 0x14, 0x005, 26
	field_exit 0x15, 0x005, 25
	field_exit 0x16, 0x005, 9
	field_exit 0x18, 0x005, 14
	field_exit 0x19, 0x005, 21
	field_exit 0x1a, 0x005, 20
	field_exit 0x1e, 0x005, 5
	field_exit 0x1f, 0x005, 4
	field_exit 0x20, 0x005, 8
	field_exit_end

FieldActorScript_0200215c:
	actor_property 0, 0x23, 0x00002126
	actor_property 0, 0x27, 0x00000001
	actor_call 0x02008665
	actor_end

FieldActorScript_02002180:
	actor_property 0, 0x09, 0x00009999
	actor_property 0, 0x0a, 0x00009999
	actor_property 0, 0x1e, 0x00000000
	actor_call 0x02008679
	actor_end

FieldActors_020021b0:
	field_actor 0x038, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x039, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 664, 0, 792, 0x4000, 2, 0
	field_actor 0x0e5, -1, 1, 8, 0, 12, 0x4000, 2, 0
	field_actor 0x0e6, -1, 1, 8, 0, 12, 0x4000, 2, 0
	field_actor 0x0e5, -1, 1, 8, 0, 8, 0x0000, 0, 0
	field_actor 0x13b, -1, 1, 872, 0, 120, 0xc000, 2, 0
	field_actor 0x13b, -1, 1, 840, 0, 104, 0xc000, 2, 1
	field_actor 0x1c4, -1, FieldActorScript_02002180 + FIELD_OVERLAY_LOAD_BIAS, 488, 0, 536, 0x4000, 2, 0
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 72, 0, 104, 0x4000, 2, 0
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 72, 0, 168, 0x4000, 2, 1
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 168, 0, 104, 0x4000, 2, 1
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 168, 0, 168, 0x4000, 2, 1
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 72, 0, 344, 0x4000, 2, 0
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 104, 0, 344, 0x4000, 2, 1
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 136, 0, 344, 0x4000, 2, 1
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 168, 0, 344, 0x4000, 2, 1
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 200, 0, 344, 0x4000, 2, 1
	field_actor_end

FieldActors_02002378:
	field_actor 0x004, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x005, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x038, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x039, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x150, -1, FieldActorScript_0200215c + FIELD_OVERLAY_LOAD_BIAS, 664, 0, 792, 0x4000, 2, 0
	field_actor_end

FieldEvents_02002408:
	field_event 0x0001, 0x0001, -1, 0x0001
	field_event 0x0001, 0x0002, -1, 0x0002
	field_event 0x0001, 0x0003, -1, 0x0003
	field_event 0x0031, 0x0004, -1, 0x0004
	field_event 0x0031, 0x0005, -1, 0x0005
	field_event 0x0001, 0x0006, -1, 0x0006
	field_event 0x0001, 0x0007, -1, 0x0007
	field_event 0x0031, 0x0008, -1, 0x0008
	field_event 0x0021, 0x0009, -1, 0x0009
	field_event 0x0001, 0x000a, -1, 0x000a
	field_event 0x0001, 0x000b, -1, 0x000b
	field_event 0x0001, 0x000c, -1, 0x000c
	field_event 0x0001, 0x000d, -1, 0x000d
	field_event 0x0021, 0x000e, -1, 0x000e
	field_event 0x0031, 0x0014, -1, 0x0014
	field_event 0x0031, 0x0015, -1, 0x0015
	field_event 0x0031, 0x0016, -1, 0x0016
	field_event 0x0031, 0x0017, -1, 0x0017
	field_event 0x0031, 0x0018, -1, 0x0018
	field_event 0x0021, 0x0019, -1, 0x0019
	field_event 0x0021, 0x001a, -1, 0x001a
	field_event 0x0021, 0x001e, -1, 0x001e
	field_event 0x0021, 0x001f, -1, 0x001f
	field_event 0x0021, 0x0020, -1, 0x0020
	field_event 0x0002, 0x002d, 0x200, 0x020086ad
	field_event 0x0002, 0x002e, -1, 0x0200878d
	field_event 0x0003, 0x0023, -1, 0x020088d5
	field_event 0x0003, 0x0024, -1, 0x020088d5
	field_event 0x0003, 0x0025, -1, 0x020088d5
	field_event 0x0003, 0x0026, -1, 0x020088d5
	field_event 0x0003, 0x0027, -1, 0x020088d5
	field_event 0x0003, 0x0028, -1, 0x020088fd
	field_event 0x50009085, 0x0000, -1, 0x02008999
	field_event 0x40009085, 0x0000, -1, 0x02008925
	field_event_end
