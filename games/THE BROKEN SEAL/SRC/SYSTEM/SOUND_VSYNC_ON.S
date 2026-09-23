@ BIOS SoundBias wrapper: sets r0 to 1 and runs svc 0x19, raising the sound
@ bias again after the sleep code at 0x080033f4 wakes from Stop. Neither
@ approved compiler emits svc. Every TBS and TLA edition has these bytes
@ (TLA SYSTEM/SOUND_VSYNC_ON.S).
@ credit: library — TBS/TLA common BIOS SoundBias wrapper
.syntax unified
	.thumb
	.global Func_08006870
	.thumb_func
Func_08006870:
	movs	r0, #1
	svc	25
	bx	lr
