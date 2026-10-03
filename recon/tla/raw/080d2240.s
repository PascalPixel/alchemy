.syntax unified
	.thumb
	.global EventRuntime_Wait
	.thumb_func
EventRuntime_Wait:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #220
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r3, [r3]
	cmp r3, #0
	bne .L_080d225c
	cmp r0, #0
	beq .L_080d225c
	bl WaitFrames
.L_080d225c:
	pop {pc}
	.2byte 0x0000
