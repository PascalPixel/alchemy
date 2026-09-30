.syntax unified
	.thumb
	.global Func_080b106c
	.thumb_func
Func_080b106c:
	push {lr}
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	cmp r0, #196
	bgt .L_080b1082
	cmp r0, #191
	blt .L_080b1082
	movs r0, #1
	b .L_080b1084
.L_080b1082:
	movs r0, #0
.L_080b1084:
	pop {pc}
	.2byte 0x0000
