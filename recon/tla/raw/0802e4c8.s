.syntax unified
	.thumb
	.global Func_0802e4c8
	.thumb_func
Func_0802e4c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #16]
	adds r7, r2, #0
	ldr r3, [r3, #40]
	movs r2, #2
	ldrb r4, [r3, #4]
	ldr r3, .L_0802e6ac
	sub sp, #4
	ldr r3, [r3]
	movs r5, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0802e4fa
	ldr r3, .L_0802e6b0
	ldr r3, [r3]
	lsls r3, r3, #24
	asrs r5, r3, #16
.L_0802e4fa:
	cmp r4, #6
	beq .L_0802e5ae
	cmp r4, #6
	bhi .L_0802e510
	cmp r4, #4
	beq .L_0802e5ae
	cmp r4, #4
	bhi .L_0802e56c
	cmp r4, #3
	beq .L_0802e528
	b .L_0802e654
.L_0802e510:
	cmp r4, #20
	beq .L_0802e5ea
	cmp r4, #20
	bhi .L_0802e51e
	cmp r4, #8
	beq .L_0802e56c
	b .L_0802e654
.L_0802e51e:
	cmp r4, #44
	beq .L_0802e56c
	cmp r4, #88
	beq .L_0802e56c
	b .L_0802e654
.L_0802e528:
	lsls r0, r0, #16
	mov r8, r0
	lsls r6, r1, #16
	movs r4, #5
.L_0802e530:
	lsls r5, r5, #16
	movs r3, #0
	mov r2, r8
	lsrs r5, r5, #16
	movs r0, #224
	str r2, [r7]
	str r3, [r7, #4]
	str r6, [r7, #8]
	adds r1, r5, #0
	adds r2, r7, #0
	lsls r0, r0, #14
	str r4, [sp, #0]
	bl Vector_AddPolarOffset
	movs r3, #168
	lsls r3, r3, #6
	ldr r4, [sp, #0]
	adds r3, #170
	adds r5, r5, r3
	lsls r5, r5, #16
	subs r4, #1
	adds r7, #16
	asrs r5, r5, #16
	cmp r4, #0
	bge .L_0802e530
	movs r4, #6
.L_0802e564:
	adds r4, #1
	cmp r4, #9
	ble .L_0802e564
	b .L_0802e69e
.L_0802e56c:
	lsls r0, r0, #16
	mov r8, r0
	lsls r6, r1, #16
	movs r4, #7
.L_0802e574:
	lsls r5, r5, #16
	movs r3, #0
	mov r2, r8
	lsrs r5, r5, #16
	movs r0, #224
	str r2, [r7]
	str r3, [r7, #4]
	str r6, [r7, #8]
	adds r1, r5, #0
	adds r2, r7, #0
	lsls r0, r0, #14
	str r4, [sp, #0]
	bl Vector_AddPolarOffset
	movs r3, #128
	ldr r4, [sp, #0]
	lsls r3, r3, #6
	adds r5, r5, r3
	lsls r5, r5, #16
	subs r4, #1
	adds r7, #16
	asrs r5, r5, #16
	cmp r4, #0
	bge .L_0802e574
	movs r4, #8
.L_0802e5a6:
	adds r4, #1
	cmp r4, #9
	ble .L_0802e5a6
	b .L_0802e69e
.L_0802e5ae:
	lsls r0, r0, #16
	movs r4, #0
	mov r8, r0
	lsls r6, r1, #16
.L_0802e5b6:
	lsls r5, r5, #16
	movs r3, #0
	mov r2, r8
	lsrs r5, r5, #16
	movs r0, #224
	str r2, [r7]
	str r3, [r7, #4]
	str r6, [r7, #8]
	adds r1, r5, #0
	adds r2, r7, #0
	lsls r0, r0, #14
	str r4, [sp, #0]
	bl Vector_AddPolarOffset
	movs r3, #200
	lsls r3, r3, #5
	ldr r4, [sp, #0]
	adds r3, #153
	adds r5, r5, r3
	lsls r5, r5, #16
	adds r4, #1
	adds r7, #16
	asrs r5, r5, #16
	cmp r4, #9
	ble .L_0802e5b6
	b .L_0802e69e
.L_0802e5ea:
	movs r2, #128
	lsls r3, r5, #16
	lsls r2, r2, #23
	adds r3, r3, r2
	asrs r5, r3, #16
	movs r3, #160
	movs r4, #0
	lsls r0, r0, #16
	lsls r1, r1, #16
	lsls r3, r3, #14
	mov r10, r0
	mov r11, r4
	mov r8, r1
	mov r9, r3
	adds r6, r7, #0
.L_0802e608:
	mov r2, r10
	lsls r5, r5, #16
	str r2, [r6]
	mov r3, r11
	lsrs r5, r5, #16
	mov r2, r8
	str r3, [r6, #4]
	str r2, [r6, #8]
	adds r1, r5, #0
	adds r2, r7, #0
	mov r0, r9
	str r4, [sp, #0]
	bl Vector_AddPolarOffset
	mov r3, r10
	str r3, [r6, #16]
	mov r3, r11
	str r3, [r6, #20]
	adds r2, r7, #0
	mov r3, r8
	adds r2, #16
	str r3, [r6, #24]
	adds r1, r5, #0
	mov r0, r9
	bl Vector_AddPolarOffset
	movs r2, #128
	ldr r4, [sp, #0]
	lsls r2, r2, #8
	adds r5, r5, r2
	lsls r5, r5, #16
	adds r4, #1
	adds r6, #32
	adds r7, #32
	asrs r5, r5, #16
	cmp r4, #1
	ble .L_0802e608
	b .L_0802e69e
.L_0802e654:
	movs r2, #128
	lsls r3, r5, #16
	lsls r2, r2, #22
	adds r3, r3, r2
	lsls r0, r0, #16
	asrs r5, r3, #16
	mov r8, r0
	lsls r6, r1, #16
	movs r4, #3
.L_0802e666:
	mov r3, r8
	lsls r5, r5, #16
	str r3, [r7]
	lsrs r5, r5, #16
	movs r3, #0
	movs r0, #224
	adds r2, r7, #0
	str r3, [r7, #4]
	str r6, [r7, #8]
	adds r1, r5, #0
	lsls r0, r0, #14
	str r4, [sp, #0]
	bl Vector_AddPolarOffset
	movs r2, #128
	ldr r4, [sp, #0]
	lsls r2, r2, #7
	adds r5, r5, r2
	lsls r5, r5, #16
	subs r4, #1
	adds r7, #16
	asrs r5, r5, #16
	cmp r4, #0
	bge .L_0802e666
	movs r4, #5
.L_0802e698:
	subs r4, #1
	cmp r4, #0
	bge .L_0802e698
.L_0802e69e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0802e6ac:
	.4byte gInput
.L_0802e6b0:
	.4byte gFrameTick
