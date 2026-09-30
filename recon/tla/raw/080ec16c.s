.syntax unified
	.thumb
	.global Func_080ec16c
	.thumb_func
Func_080ec16c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	adds r5, r0, #0
	bl Func_080cdf5c
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	ldr r3, [r3]
	mov r8, r3
	bl ObjectTable_Get
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080ec1c4
	ldr r2, .L_080ec1cc
	asrs r5, r5, #16
	movs r1, #192
	lsls r0, r5, #14
	lsls r1, r1, #2
	adds r0, r0, r2
	adds r1, #85
	bl Math_Div
	asrs r7, r7, #16
	adds r5, r0, #0
	movs r3, #224
	lsls r3, r3, #13
	lsls r5, r5, #16
	lsls r0, r7, #14
	movs r1, #160
	adds r0, r0, r3
	str r5, [r6, #8]
	lsls r1, r1, #2
	bl Math_Div
	mov r2, r8
	lsls r0, r0, #16
	str r0, [r6, #16]
	str r5, [r2]
	ldr r3, [r6, #16]
	str r3, [r2, #8]
.L_080ec1c4:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080ec1cc:
	.4byte 0x00434000
