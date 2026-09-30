.syntax unified
	.thumb
	.global Func_0802dcd8
	.thumb_func
Func_0802dcd8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	movs r6, #0
	b .L_0802dcf4
.L_0802dce4:
	movs r0, #1
	bl WaitFrames
	movs r3, #150
	adds r6, #1
	lsls r3, r3, #1
	cmp r6, r3
	bge .L_0802dd00
.L_0802dcf4:
	ldr r3, [r5, #4]
	cmp r3, #255
	bgt .L_0802dce4
	ldr r3, [r5, #8]
	cmp r3, #255
	bgt .L_0802dce4
.L_0802dd00:
	movs r3, #0
	str r3, [r5, #12]
	pop {r5, r6, pc}
	.2byte 0x0000
