.syntax unified
	.thumb
	.global Func_080dbda8
	.thumb_func
Func_080dbda8:
	push {lr}
	movs r2, #2
	bl Func_080dbb78
	ldrb r3, [r0, #2]
	movs r0, #1
	cmp r3, #233
	beq .L_080dbdc6
	movs r0, #2
	cmp r3, #234
	beq .L_080dbdc6
	movs r0, #3
	cmp r3, #235
	beq .L_080dbdc6
	movs r0, #0
.L_080dbdc6:
	pop {pc}
