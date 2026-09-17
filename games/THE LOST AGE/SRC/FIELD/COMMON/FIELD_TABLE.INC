/* Field overlay data tables for The Lost Age.

   A field overlay is linked at 0x02000000 and runs at 0x02008000, so a pointer
   from one table to another adds FIELD_OVERLAY_LOAD_BIAS to the label.

   Every layout below is read from the main-program code that consumes it:
     entrances  overlay entry 1, walked by 0x080cc67c
     exits      overlay entry 2, walked by 0x080c9694
     actors     overlay entry 3, walked by 0x080ca6e8
     events     overlay entry 4, walked by 0x080ccd78, 0x080cdfb0, 0x080ce31c, 0x080ce458
     scripts    actor script words, dispatched by 0x08023f9c through 0x0802f204
   Field names stay no more specific than those consumers show. */

	.set FIELD_OVERLAY_LOAD_BIAS, 0x8000

/* 24-byte entrance row. id matches the entrance number the scene was entered
   with; flag is a story flag (-1 always applies); x, y and z are whole pixels;
   left, top, right and bottom bound the camera, -1 leaving a bound unchanged. */
	.macro field_entrance id, flag, x, y, z, heading, left, top, right, bottom, word12=0, word22=0
	.2byte \id, \flag, \x, \y, \z, \heading, \word12, \left, \top, \right, \bottom, \word22
	.endm
	.macro field_entrance_end
	.2byte -1
	.space 22
	.endm

/* Packed exit words. A scene word opens the exits of that scene; each exit word
   holds scene (bits 0-11), entrance (bits 12-19) and exit number (bits 20-27).
   Bit 28 appends a story-flag word that must be clear for the exit to apply. */
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

/* 24-byte actor placement row. descriptor selects the actor graphics; flag is a
   story flag (-1 always places the actor); script is a built-in behaviour 1-11
   or a pointer to actor script words; x, y and z are whole pixels stored as
   16.16; heading is the facing word; byte22 has no traced reader yet. In mode
   (byte 23), bit 0 pairs the actor with the previous row and bit 1 passes x
   through the packed-position decoder at 0x080ca9cc. */
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

/* 12-byte event row. The low four bits of trigger select the event kind and the
   upper bits qualify it; target is the actor, tile or area the kind matches;
   flag is a story flag (-1 always applies); action is a message id, an overlay
   routine address or a packed exit word, as the kind requires. */
	.macro field_event trigger, target, flag, action
	.4byte \trigger
	.2byte \target, \flag
	.4byte \action
	.endm
	.macro field_event_end
	.4byte -1, 0, 0
	.endm

/* Flagged id list: u16 id and u16 story flag, closed by 0x0000ffff. */
	.macro field_flagged_id id, flag
	.2byte \id, \flag
	.endm
	.macro field_flagged_id_end
	.4byte 0xffff
	.endm

/* Actor script words. Each opcode word (0-63) is followed by its arguments; a
   word above 63 is skipped and serves as a jump label. */
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
