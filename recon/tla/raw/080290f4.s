.syntax unified
	.thumb
	.global Func_080290f4
	.thumb_func
Func_080290f4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #7
	str r3, [r7, #52]
	ldr r3, .L_080292e4
	movs r4, #128
	ldrb r3, [r3]
	movs r0, #12
	lsls r4, r4, #8
	sub sp, #12
	mov r11, r0
	str r4, [r7, #48]
	cmp r3, #0
	beq .L_08029124
	ldr r2, .L_080292e8
	ldr r3, [r2]
	b .L_08029126
.L_08029124:
	ldr r2, .L_080292e8
.L_08029126:
	ldr r3, [r2]
	ldr r1, .L_080292ec
	movs r2, #15
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r3, [r1, r3]
	movs r2, #255
	lsls r1, r3, #16
	lsls r2, r2, #8
	movs r3, #4
	lsrs r0, r1, #16
	adds r2, #255
	mov r9, r3
	cmp r0, r2
	bne .L_08029148
	b .L_08029268
.L_08029148:
	movs r2, #240
	lsls r2, r2, #8
	movs r3, #14
	ands r2, r0
	mov r11, r3
	cmp r2, #0
	beq .L_08029162
	movs r0, #15
	mov r11, r0
	cmp r2, r4
	beq .L_08029162
	movs r2, #10
	mov r11, r2
.L_08029162:
	mov r6, sp
	movs r0, #128
	movs r3, #0
	adds r2, r6, #0
	lsls r0, r0, #12
	lsrs r1, r1, #16
	mov r9, r3
	str r3, [r6]
	str r3, [r6, #4]
	str r3, [r6, #8]
	bl Vector_AddPolarOffset
	ldr r2, [r7, #8]
	ldr r3, [r6]
	adds r3, r3, r2
	ldr r2, [r6, #8]
	str r3, [r6]
	cmp r2, #0
	bge .L_0802918e
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r7, #6]
.L_0802918e:
	cmp r2, #0
	ble .L_08029198
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r7, #6]
.L_08029198:
	ldr r3, [r7, #12]
	ldr r2, [r6, #8]
	adds r0, r7, #0
	subs r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r7, #16]
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	bl Func_080c8710
	movs r2, #34
	adds r2, r2, r7
	mov r8, r2
	ldrb r2, [r2]
	lsls r0, r0, #2
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r3, r3, #3
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r1, [r5, r3]
	ldr r3, [r6]
	adds r0, r0, r1
	mov r10, r0
	cmp r3, #0
	bge .L_080291d6
	ldr r0, .L_080292f0
	adds r3, r3, r0
.L_080291d6:
	asrs r2, r3, #20
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_080291e2
	ldr r0, .L_080292f0
	adds r3, r3, r0
.L_080291e2:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r2, r3
	lsls r3, r3, #2
	adds r0, r6, #0
	adds r5, r1, r3
	bl Func_0802da24
	cmp r0, #0
	bne .L_0802920e
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r1, #64
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	beq .L_0802920e
	ldrb r2, [r5, #3]
	adds r3, r1, #0
	ands r3, r2
	cmp r3, #0
	bne .L_08029218
.L_0802920e:
	movs r0, #4
	movs r2, #12
	mov r9, r0
	mov r11, r2
	b .L_08029268
.L_08029218:
	ldr r2, .L_080292e8
	ldr r3, [r2]
	ands r3, r1
	cmp r3, #0
	beq .L_08029240
	mov r3, r8
	ldrb r0, [r3]
	ldr r2, [r6, #8]
	ldr r3, .L_080292f4
	ldr r1, [r6]
	adds r2, r2, r3
	bl Func_0802d45c
	ldr r3, [r7, #12]
	subs r3, r0, r3
	movs r0, #128
	lsls r0, r0, #13
	cmp r3, r0
	bge .L_08029268
	b .L_08029260
.L_08029240:
	ldr r3, [r2]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_08029268
	mov r2, r8
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_0802d45c
	ldr r3, [r7, #12]
	subs r3, r0, r3
	ldr r0, .L_080292f8
	cmp r3, r0
	ble .L_08029268
.L_08029260:
	movs r2, #1
	movs r3, #12
	mov r9, r2
	mov r11, r3
.L_08029268:
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	cmp r1, #0
	beq .L_0802929e
	movs r2, #3
	mov r0, r9
	ands r2, r0
	cmp r2, #0
	beq .L_0802928a
	movs r3, #194
	lsls r3, r3, #1
	adds r2, r1, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_08029292
.L_0802928a:
	movs r0, #194
	lsls r0, r0, #1
	adds r3, r1, r0
	strh r2, [r3]
.L_08029292:
	ldr r3, .L_080292e8
	movs r0, #195
	ldr r3, [r3]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
.L_0802929e:
	adds r0, r7, #0
	mov r1, r11
	bl ObjectDispatch_ApplyArgumentToChildren
	mov r2, r9
	cmp r2, #0
	beq .L_080292c0
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #40]
	str r3, [r7, #44]
	b .L_080292ce
.L_080292c0:
	mov r3, sp
	ldr r1, [r3]
	ldr r2, [r3, #4]
	adds r0, r7, #0
	ldr r3, [r3, #8]
	bl Object_SetMoveTarget
.L_080292ce:
	ldrh r3, [r7, #4]
	movs r0, #1
	adds r3, #1
	strh r3, [r7, #4]
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080292e4:
	.4byte gDebugMode
.L_080292e8:
	.4byte gInput
.L_080292ec:
	.4byte Data_0802ec5c
.L_080292f0:
	.4byte 0x000fffff
.L_080292f4:
	.4byte 0xfff00000
.L_080292f8:
	.4byte 0xfff80000
