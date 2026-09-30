.syntax unified
	.thumb
	.global Func_080d170c
	.thumb_func
Func_080d170c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #128
	adds r4, r1, #0
	ldr r1, [r3]
	cmp r1, #0
	beq .L_080d1728
	movs r3, #224
	lsls r3, r3, #4
	adds r2, r1, r3
	adds r3, r4, #0
	bl Func_080d0e1c
.L_080d1728:
	pop {pc}
	.2byte 0x0000
