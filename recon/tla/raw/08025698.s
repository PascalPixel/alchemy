.syntax unified
	.thumb
	.global Func_08025698
	.thumb_func
Func_08025698:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r0, #4
	ldrsh r2, [r6, r0]
	ldr r3, [r6]
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r3, #4
	ldmia r3!, {r2}
	sub sp, #32
	str r2, [sp, #4]
	ldmia r3!, {r5}
	ldr r3, [r3]
	mov r11, r5
	cmp r3, #0
	bge .L_080256cc
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_080256cc:
	asrs r3, r3, #16
	adds r5, r3, #0
	muls r5, r3
	str r3, [sp, #0]
	str r5, [sp, #0]
	movs r2, #0
	mov r9, r2
.L_080256da:
	movs r0, #1
	add r9, r0
	mov r2, r9
	cmp r2, #7
	ble .L_080256e6
	b .L_080257f2
.L_080256e6:
	ldr r3, [r6, #8]
	add r7, sp, #20
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	str r3, [r7, #8]
	bl Random16
	mov r1, r11
	ldr r3, .L_08025828
	mov lr, r3
	.2byte 0xf800
	ldr r3, [sp, #4]
	adds r3, r3, r0
	mov r8, r3
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldrh r3, [r6, #6]
	lsrs r5, r5, #2
	lsrs r0, r0, #2
	adds r3, r3, r5
	subs r3, r3, r0
	mov r10, r3
	mov r0, r8
	mov r1, r10
	adds r2, r7, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_08024f20
	cmp r0, #0
	bne .L_080256da
	adds r0, r6, #0
	adds r1, r7, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080256da
	ldr r3, [r6, #8]
	movs r5, #128
	lsls r5, r5, #12
	add r8, r5
	add r5, sp, #8
	str r3, [r5]
	mov r0, r8
	ldr r3, [r6, #12]
	mov r1, r10
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	ldr r3, [r6, #8]
	movs r1, #128
	str r3, [r5]
	lsls r1, r1, #6
	ldr r3, [r6, #12]
	add r1, r10
	str r3, [r5, #4]
	mov r0, r8
	ldr r3, [r6, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080256da
	ldr r3, [r6, #8]
	ldr r1, .L_0802582c
	str r3, [r5]
	add r1, r10
	ldr r3, [r6, #12]
	mov r0, r8
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_0802d87c
	cmp r0, #0
	bne .L_080256da
	ldr r3, [r7]
	adds r1, r3, #0
	cmp r3, #0
	bge .L_080257b6
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_080257b6:
	adds r2, r6, #0
	adds r2, #100
	movs r5, #0
	ldrsh r2, [r2, r5]
	asrs r3, r3, #16
	subs r0, r3, r2
	ldr r2, [r7, #8]
	adds r4, r2, #0
	cmp r2, #0
	bge .L_080257d2
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_080257d2:
	adds r3, r6, #0
	adds r3, #102
	movs r5, #0
	ldrsh r3, [r3, r5]
	asrs r2, r2, #16
	subs r2, r2, r3
	adds r3, r0, #0
	muls r3, r0
	adds r0, r2, #0
	muls r0, r2
	adds r2, r0, #0
	adds r3, r3, r2
	ldr r2, [sp, #0]
	cmp r3, r2
	ble .L_08025808
	b .L_080256da
.L_080257f2:
	ldrh r3, [r6, #6]
	movs r5, #128
	lsls r5, r5, #8
	adds r3, r3, r5
	adds r2, r6, #0
	strh r3, [r6, #6]
	adds r2, #94
	movs r3, #1
	strh r3, [r2]
	movs r0, #0
	b .L_0802581a
.L_08025808:
	adds r0, r6, #0
	adds r3, r4, #0
	ldr r2, [r7, #4]
	bl Object_SetMoveTarget
	ldrh r3, [r6, #4]
	movs r0, #1
	adds r3, #4
	strh r3, [r6, #4]
.L_0802581a:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08025828:
	.4byte IwramMulQ16
.L_0802582c:
	.4byte 0xffffe000
