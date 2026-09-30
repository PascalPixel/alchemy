.syntax unified
	.thumb
	.global Func_080e9ee4
	.thumb_func
Func_080e9ee4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r0, #0
	ldr r3, [r7]
	ldr r6, [r7, #12]
	ldr r2, [r7, #16]
	subs r6, r6, r3
	ldr r3, [r7, #4]
	mov r10, r1
	subs r2, r2, r3
	mov r9, r2
	adds r1, r6, #0
	mov r0, r9
	bl ArcTan2
	adds r5, r0, #0
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r0, r5, #0
	bl Trig_Cos
	ldr r3, .L_080e9f64
	mov r1, r10
	mov r8, r3
	mov lr, r8
	.2byte 0xf800
	ldr r3, [r7]
	adds r3, r3, r0
	str r3, [r7]
	adds r0, r5, #0
	bl Trig_Sin
	mov r1, r10
	mov lr, r8
	.2byte 0xf800
	ldr r1, [r7, #4]
	ldr r4, [r7, #12]
	ldr r3, [r7]
	ldr r2, [r7, #16]
	adds r1, r1, r0
	subs r3, r4, r3
	str r1, [r7, #4]
	subs r0, r2, r1
	eors r3, r6
	movs r1, #128
	lsls r1, r1, #24
	cmp r3, #0
	bge .L_080e9f4c
	str r4, [r7]
.L_080e9f4c:
	mov r3, r9
	eors r3, r0
	ands r3, r1
	cmp r3, #0
	beq .L_080e9f5a
	ldr r3, [r7, #16]
	str r3, [r7, #4]
.L_080e9f5a:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080e9f64:
	.4byte IwramMulQ16
