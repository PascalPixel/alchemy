.syntax unified
	.thumb
	.global Func_080156f8
	.thumb_func
Func_080156f8:
	push {r5, lr}
	sub sp, #48
	mov r5, sp
	adds r2, r5, #0
	bl Func_08015510
	ldr r3, .L_08015710
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	add sp, #48
	pop {r5, pc}
.L_08015710:
	.4byte IwramTransformMatrix
