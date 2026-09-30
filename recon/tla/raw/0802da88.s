.syntax unified
	.thumb
	.global Func_0802da88
	.thumb_func
Func_0802da88:
	push {lr}
	ldr r3, [r0, #8]
	ldr r0, [r0]
	cmp r0, #0
	bge .L_0802da96
	ldr r1, .L_0802dab8
	adds r0, r0, r1
.L_0802da96:
	asrs r0, r0, #21
	movs r2, #31
	ands r0, r2
	cmp r3, #0
	bge .L_0802daa4
	ldr r1, .L_0802dab8
	adds r3, r3, r1
.L_0802daa4:
	asrs r3, r3, #21
	ands r3, r2
	lsls r3, r3, #5
	ldr r2, .L_0802dabc
	adds r3, r0, r3
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrb r0, [r3, #2]
	pop {pc}
	.2byte 0x0000
.L_0802dab8:
	.4byte 0x001fffff
.L_0802dabc:
	.4byte gMapBlocks
