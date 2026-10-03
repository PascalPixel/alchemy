.syntax unified
	.balign 4
	.thumb
	.global UiWork_IsIdle
	.thumb_func
UiWork_IsIdle:
	push {lr}
	cmp r0, #0
	bne .L_0803a3ee
	movs r0, #1
	b .L_0803a400
.L_0803a3ee:
	ldrh r3, [r0, #22]
	cmp r3, #0
	bne .L_0803a3fe
	movs r2, #26
	ldrsh r3, [r0, r2]
	movs r0, #1
	cmp r3, #0
	beq .L_0803a400
.L_0803a3fe:
	movs r0, #0
.L_0803a400:
	pop {pc}
	.2byte 0x0000
