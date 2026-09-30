.syntax unified
	.thumb
	.global Func_08015160
	.thumb_func
Func_08015160:
	push {r5, r6, lr}
	sub sp, #48
	adds r6, r0, #0
	mov r12, r1
	mov lr, r2
	mov r5, sp
	adds r0, r5, #0
	movs r1, #128
	lsls r1, r1, #9
	movs r2, #0
	movs r3, #0
	movs r4, #0
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	stmia r0!, {r1, r2, r3, r4}
	mov r3, r12
	str r3, [r5, #40]
	mov r3, lr
	str r3, [r5, #44]
	str r6, [r5, #36]
	ldr r3, .L_08015194
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	add sp, #48
	pop {r5, r6, pc}
.L_08015194:
	.4byte IwramTransformMatrix
