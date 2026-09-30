.syntax unified
	.thumb
	.global Func_080d1ee8
	.thumb_func
Func_080d1ee8:
	push {lr}
	ldr r3, .L_080d1f08
	movs r1, #1
	adds r2, r0, #0
	negs r1, r1
.L_080d1ef2:
	ldr r0, [r3]
	cmp r0, r1
	beq .L_080d1f04
	cmp r0, r2
	bne .L_080d1f00
	ldr r0, [r3, #4]
	b .L_080d1f04
.L_080d1f00:
	adds r3, #8
	b .L_080d1ef2
.L_080d1f04:
	pop {pc}
	.2byte 0x0000
.L_080d1f08:
	.4byte Data_080f3228
