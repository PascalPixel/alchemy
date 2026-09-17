/* VINASU_IRIGUCHI field data: typed from resource_64d (runtime 0x200ad20-0x200b050).
   Assembled through the macros of games/THE LOST AGE/SRC/FIELD/COMMON/FIELD_TABLE.INC. */

FieldEntrances_02002d20:
	field_entrance 0, -1, 172, 0, 149, 0x4000, -1, -1, -1, -1
	field_entrance 9, -1, 376, 0, 104, 0x4000, -1, -1, -1, -1
	field_entrance 10, -1, 376, 0, 232, 0x4000, -1, -1, -1, -1
	field_entrance 11, -1, 384, 0, 392, 0x4000, -1, -1, -1, -1
	field_entrance 12, -1, 112, 0, 272, 0x4000, -1, -1, -1, -1
	field_entrance 90, -1, 376, 0, 104, 0x4000, -1, -1, -1, -1
	field_entrance 98, -1, 384, 0, 168, 0x4000, -1, -1, -1, -1
	field_entrance 99, -1, 72, 0, 264, 0x8000, -1, -1, -1, -1
	field_entrance_end

FieldExits_02002df8:
	field_exit_scene 0x004
	field_exit 0x01, 0x006, 5
	field_exit 0x02, 0x005, 1
	field_exit_end

FieldActors_02002e08:
	field_actor 0x038, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x039, -1, 1, 8, 0, 8, 0xc000, 2, 0
	field_actor 0x05c, -1, 1, 352, 0, 176, 0x0000, 2, 0
	field_actor 0x05c, -1, 1, 416, 0, 152, 0x8000, 2, 0
	field_actor 0x0e8, -1, 1, 424, 0, 408, 0xa000, 2, 0
	field_actor 0x0e8, -1, 1, 440, 0, 408, 0xa000, 2, 0
	field_actor 0x0e8, -1, 1, 424, 0, 424, 0xa000, 2, 0
	field_actor 0x0e8, -1, 1, 440, 0, 440, 0xa000, 2, 0
	field_actor 0x0e8, -1, 1, 456, 0, 408, 0xa000, 2, 0
	field_actor 0x0bb, -1, 1, 328, 0, 408, 0xe000, 2, 0
	field_actor 0x0c3, -1, 1, 312, 0, 408, 0xe000, 2, 0
	field_actor 0x0bb, -1, 1, 328, 0, 424, 0xe000, 2, 0
	field_actor 0x0c3, -1, 1, 312, 0, 440, 0xe000, 2, 0
	field_actor 0x0bb, -1, 1, 296, 0, 408, 0xe000, 2, 0
	field_actor 0x05d, -1, 1, 24, 0, 264, 0x8000, 2, 0
	field_actor_end

FieldEvents_02002f88:
	field_event 0x0001, 0x0001, -1, 0x0001
	field_event 0x0001, 0x0002, -1, 0x0002
	field_event 0x0002, 0x0009, 0x892, 0x02008441
	field_event 0x0002, 0x000a, 0x890, 0x02008511
	field_event 0x0002, 0x000b, 0x893, 0x02009a2d
	field_event 0x0002, 0x000c, -1, 0x02009bb1
	field_event 0x0000, 0x0016, -1, 0x1608
	field_event 0x0000, 0x000e, -1, 0x02008055
	field_event 0x0000, 0x000f, -1, 0x020080c9
	field_event_end

FieldActorScript_02003000:
	actor_await_motion
	actor_wait 1
	actor_op 0x27, 0x00000002
	actor_wait 4
	actor_property 0, 0x1a, 0x00000003
	actor_property 0, 0x0d, 0x00000000
	actor_wait 1
	actor_end

FieldActorScript_02003040:
	actor_await_motion
	actor_wait 1
	actor_halt
