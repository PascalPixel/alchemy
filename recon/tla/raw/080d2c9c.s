.syntax unified
	.thumb
	.global Func_080d2c9c
	.thumb_func
Func_080d2c9c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	cmp r3, #0
	beq .L_080d2cc0
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #184
	adds r3, r3, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_080d2cc0
	bl Audio_PlayCue
.L_080d2cc0:
	pop {pc}
	.2byte 0x0000
