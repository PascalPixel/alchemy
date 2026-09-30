.syntax unified
	.thumb
	.global Func_0801489c
	.thumb_func
Func_0801489c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	mov r10, r0
	movs r0, #128
	mov r8, r1
	lsls r0, r0, #7
	add r0, r8
	adds r5, r2, #0
	bl Trig_Sin
	ldr r6, .L_080148e4
	adds r1, r0, #0
	mov r0, r10
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5]
	adds r3, r3, r0
	stmia r5!, {r3}
	mov r0, r8
	bl Trig_Sin
	adds r1, r0, #0
	mov r0, r10
	mov lr, r6
	.2byte 0xf800
	adds r5, #4
	ldr r3, [r5]
	adds r3, r3, r0
	str r3, [r5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_080148e4:
	.4byte IwramMulQ16
