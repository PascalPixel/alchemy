@ BIOS SoundBias wrapper: svc 0x19 with r0 = 1 moves the sound bias up to
@ level 0x200. Neither approved compiler emits svc. These 6 bytes equal TBS
@ SYSTEM/SOUND_VSYNC_ON.S (0x08006870).
@ credit: library — TBS/TLA common BIOS SoundBias wrapper
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08016e04
	.global Bios_SoundBiasOn
	.thumb_func
Bios_SoundBiasOn:
	movs	r0, #1
	svc	25
	bx	lr
