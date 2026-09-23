@ BIOS call wrappers. 0x08006864 runs CpuSet (svc 0x0b) with the caller's
@ r0-r2; 0x08006868 sets r0 to 0 and runs SoundBias (svc 0x19), which the
@ sleep code at 0x080033ee calls before Stop. Neither approved compiler emits
@ svc, and the game routes admit no inline assembly for it. Every TBS and TLA
@ edition has these bytes (TLA SYSTEM/CPU_SET.S).
@ credit: library — TBS/TLA common BIOS call wrappers
.syntax unified
	.thumb
	.global Func_08006864
	.thumb_func
Func_08006864:
	svc	11
	bx	lr
	.global Func_08006868
	.thumb_func
Func_08006868:
	movs	r0, #0
	svc	25
	bx	lr
