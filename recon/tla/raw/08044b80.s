.syntax unified
	.thumb
	.global Func_08044b80
	.thumb_func
Func_08044b80:
	push {lr}
	cmp r0, #8
	bls .L_08044b8a
	movs r0, #0
	b .L_08044b90
.L_08044b8a:
	ldr r3, .L_08044b94
	lsls r2, r0, #1
	ldrsh r0, [r3, r2]
.L_08044b90:
	pop {pc}
	.2byte 0x0000
.L_08044b94:
	.4byte Data_0805f666
