.syntax unified
	.thumb
	.global Func_080dbca8
	.thumb_func
Func_080dbca8:
	push {lr}
	movs r2, #2
	bl Func_080dbb78
	ldrb r3, [r0, #2]
	movs r2, #0
	cmp r3, #231
	bne .L_080dbcba
	movs r2, #1
.L_080dbcba:
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
