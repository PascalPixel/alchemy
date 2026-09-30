.syntax unified
	.thumb
	.global UiWork_IsComplete
	.thumb_func
UiWork_IsComplete:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	movs r1, #215
	lsls r1, r1, #3
	adds r2, r3, r1
	movs r1, #0
.L_0803a3c8:
	ldr r3, [r2]
	cmp r3, #0
	beq .L_0803a3d6
	ldrh r3, [r3, #20]
	movs r0, #0
	cmp r3, #0
	beq .L_0803a3e0
.L_0803a3d6:
	adds r1, #1
	adds r2, #40
	cmp r1, #3
	bne .L_0803a3c8
	movs r0, #1
.L_0803a3e0:
	pop {pc}
	.2byte 0x0000
