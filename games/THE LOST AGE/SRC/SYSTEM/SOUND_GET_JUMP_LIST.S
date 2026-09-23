@ BIOS SoundGetJumpList wrapper: software interrupt 0x2a, then return. No bl
@ reaches it and no literal word names it in TLA. Neither approved compiler
@ emits svc outside inline assembly. These 4 bytes equal TBS
@ SYSTEM/SOUND_GET_JUMP_LIST.S (0x080fa674) byte for byte.
@ credit: library — TBS/TLA common BIOS SoundGetJumpList wrapper
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x081c2310
	.global Bios_SoundGetJumpList
	.thumb_func
Bios_SoundGetJumpList:
	svc	42
	bx	lr
