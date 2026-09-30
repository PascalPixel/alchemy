.syntax unified
	.thumb
	.global Func_0815b68c
	.thumb_func
Func_0815b68c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #92]
	ldr r3, .L_0815b75c
	sub sp, #24
	ldr r4, [r3, #4]
	ldr r3, [r3]
	add r1, sp, #8
	str r3, [sp, #0]
	str r4, [sp, #4]
	movs r2, #192
	movs r3, #0
	str r3, [r1, #12]
	str r3, [r1, #4]
	lsls r2, r2, #3
	adds r2, #228
	adds r5, r7, r2
	ldr r3, [r5]
	movs r2, #238
	str r3, [r1]
	movs r3, #221
	lsls r3, r3, #3
	adds r6, r7, r3
	ldr r3, [r6]
	lsls r2, r2, #7
	adds r2, #248
	str r3, [r1, #8]
	adds r3, r7, r2
	ldr r0, [r3]
	mov r2, sp
	movs r3, #0
	bl Func_08020010
	movs r3, #222
	lsls r3, r3, #3
	adds r2, r7, r3
	ldr r2, [r2]
	ldr r3, [r5]
	adds r3, r3, r2
	movs r2, #192
	str r3, [r5]
	lsls r2, r2, #3
	adds r2, #244
	adds r1, r7, r2
	ldr r2, [r1]
	ldr r3, [r6]
	adds r3, r3, r2
	movs r2, #238
	str r3, [r6]
	lsls r2, r2, #7
	adds r2, #140
	adds r3, r7, r2
	ldr r3, [r3]
	cmp r3, #19
	bgt .L_0815b704
	ldr r3, [r1]
	movs r2, #128
	lsls r2, r2, #5
	b .L_0815b708
.L_0815b704:
	ldr r3, [r1]
	ldr r2, .L_0815b760
.L_0815b708:
	adds r3, r3, r2
	str r3, [r1]
	movs r3, #238
	lsls r3, r3, #7
	adds r3, #140
	adds r5, r7, r3
	ldr r0, [r5]
	cmp r0, #11
	ble .L_0815b72c
	movs r1, #222
	lsls r1, r1, #3
	adds r2, r7, r1
	ldr r3, [r2]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	str r3, [r2]
	ldr r0, [r5]
.L_0815b72c:
	movs r2, #238
	lsls r2, r2, #7
	adds r2, #248
	adds r3, r7, r2
	movs r1, #7
	ldr r6, [r3]
	bl Math_Div
	adds r1, r0, #0
	cmp r0, #0
	bge .L_0815b744
	adds r1, r0, #7
.L_0815b744:
	asrs r1, r1, #3
	lsls r1, r1, #3
	subs r1, r0, r1
	adds r1, #7
	adds r0, r6, #0
	bl Animation_ApplyChildArgumentFar
	ldr r3, [r5]
	add sp, #24
	adds r3, #1
	str r3, [r5]
	pop {r5, r6, r7, pc}
.L_0815b75c:
	.4byte Data_08196e4c
.L_0815b760:
	.4byte 0xfffff000
