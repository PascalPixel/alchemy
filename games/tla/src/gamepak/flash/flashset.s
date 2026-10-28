	.ifndef GUARD_FLASHSET_S
	.set GUARD_FLASHSET_S, 1

@ Settings for each flash chip model (44 bytes).
	.macro flash_setup program, erase_chip, erase_sector, wait, max_time, rom_size, sector_size, shift, count, top, wait0, wait1, id
	.4byte \program, \erase_chip, \erase_sector, \wait, \max_time
	.4byte \rom_size, \sector_size
	.byte \shift, 0
	.2byte \count, \top, 0
	.2byte \wait0, \wait1, \id, 0
	.endm

@ Wait time, initial timer value, timer control.
	.macro flash_max_time ms, reload, control
	.2byte \ms, \reload, \control
	.endm

	.endif
