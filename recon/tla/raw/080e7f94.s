.syntax unified
	.thumb
	.global Func_080e7f94
	.thumb_func
Func_080e7f94:
	push {r5, r6, lr}
	mov r6, r11
	mov r5, r10
	push {r5, r6}
	mov r6, r9
	mov r5, r8
	push {r5, r6}
	mov r8, r2
	mov r0, r8
	mov r9, r1
	mov r11, r3
	bl Trig_Cos
	ldr r3, .L_080e7fe8
	mov r1, r9
	mov r10, r3
	mov lr, r10
	.2byte 0xf800
	adds r5, r0, #0
	mov r0, r8
	bl Trig_Sin
	mov r1, r9
	mov lr, r10
	.2byte 0xf800
	movs r6, #63
	asrs r0, r0, #16
	asrs r5, r5, #16
	subs r5, r6, r5
	subs r6, r6, r0
	adds r1, r6, #0
	adds r0, r5, #0
	mov r2, r11
	bl Func_080eb2f0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r3}
	mov r11, r3
	pop {r5, r6, pc}
.L_080e7fe8:
	.4byte IwramMulQ16
