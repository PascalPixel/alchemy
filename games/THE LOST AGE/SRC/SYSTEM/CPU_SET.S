@ BIOS call wrappers: CpuSet (svc 0x0b) and SoundBias (svc 0x19) with r0 = 0,
@ which moves the sound bias down to level 0. Neither approved compiler
@ emits svc. These 10 bytes equal TBS SYSTEM/CPU_SET.S (0x08006864).
@ credit: library — TBS/TLA common BIOS call wrappers
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08016df8
	.global Bios_CpuSet
	.thumb_func
Bios_CpuSet:
	svc	11
	bx	lr
	.global Bios_SoundBiasOff
	.thumb_func
Bios_SoundBiasOff:
	movs	r0, #0
	svc	25
	bx	lr
