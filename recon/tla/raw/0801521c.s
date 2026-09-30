.syntax unified
	.thumb
	.global Func_0801521c
	.thumb_func
Func_0801521c:
	push {r5, r6, lr}
	sub sp, #48
	adds r5, r0, #0
	mov r6, sp
	adds r0, r6, #0
	movs r1, #128
	lsls r1, r1, #9
	movs r2, #0
	movs r3, #0
	movs r4, #0
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	str r5, [r6]
	str r5, [r6, #16]
	str r5, [r6, #32]
	ldr r3, .L_08015248
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	add sp, #48
	pop {r5, r6, pc}
.L_08015248:
	.4byte IwramTransformMatrix
