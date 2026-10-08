@ Unlinked field table macros from the removed games/tla/src/field/common/field_table.inc, kept as a GNU as draft (S4); nothing includes it.

	.set FIELD_OVERLAY_LOAD_BIAS, 0x8000

	.macro field_entrance id, flag, x, y, z, heading, left, top, right, bottom, word12=0, word22=0
	.2byte \id, \flag, \x, \y, \z, \heading, \word12, \left, \top, \right, \bottom, \word22
	.endm
	.macro field_entrance_end
	.2byte -1
	.space 22
	.endm

	.macro field_exit_scene scene
	.4byte \scene
	.endm
	.macro field_exit exit, scene, entrance
	.4byte (\exit << 20) | (\entrance << 12) | \scene
	.endm
	.macro field_exit_when exit, scene, entrance, flag
	.4byte 0x10000000 | (\exit << 20) | (\entrance << 12) | \scene
	.4byte \flag
	.endm
	.macro field_exit_end
	.4byte 0x1ff
	.endm

	.macro field_actor descriptor, flag, script, x, y, z, heading, byte22, mode
	.2byte \descriptor, \flag
	.4byte \script, (\x) << 16, (\y) << 16, (\z) << 16
	.2byte \heading
	.byte \byte22, \mode
	.endm
	.macro field_actor_end
	.2byte -1
	.space 22
	.endm

	.macro field_event trigger, target, flag, action
	.4byte \trigger
	.2byte \target, \flag
	.4byte \action
	.endm
	.macro field_event_end
	.4byte -1, 0, 0
	.endm

	.macro field_flagged_id id, flag
	.2byte \id, \flag
	.endm
	.macro field_flagged_id_end
	.4byte 0xffff
	.endm

	.set FOREVER, 0xffff
	.macro actor_op op, args:vararg
	.4byte \op
	.ifnb \args
	.4byte \args
	.endif
	.endm
	.macro actor_label number
	.4byte 0x80000000 | (\number << 16)
	.endm
	.macro actor_wait frames
	actor_op 0x00, \frames
	.endm
	.macro actor_await_motion
	actor_op 0x01
	.endm
	.macro actor_move_to x, y, z
	actor_op 0x04, \x, \y, \z
	.endm
	.macro actor_move_by dx, dy, dz
	actor_op 0x05, \dx, \dy, \dz
	.endm
	.macro actor_loop count, label
	actor_op 0x0d, \count, 0xc0000000 | (\label << 16)
	.endm
	.macro actor_jump label
	actor_op 0x0e, 0xc0000000 | (\label << 16)
	.endm
	.macro actor_end
	actor_op 0x11
	.endm
	.macro actor_property mode, property, value
	actor_op 0x16 + \mode, \property, \value
	.endm
	.macro actor_halt
	actor_op 0x26
	.endm
	.macro actor_call routine
	actor_op 0x2e, \routine
	.endm
