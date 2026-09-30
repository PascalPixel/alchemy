.syntax unified
	.thumb
	.global Render_ApplyProjectedPlacement
	.thumb_func
Render_ApplyProjectedPlacement:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldrb r0, [r7, #20]
	sub sp, #64
	lsrs r0, r0, #1
	str r0, [sp, #24]
	ldrb r0, [r7, #21]
	lsrs r0, r0, #1
	str r0, [sp, #20]
	adds r0, r7, #0
	ldmia r2!, {r6}
	ldr r5, [r2]
	ldmia r1!, {r2}
	str r2, [sp, #12]
	ldmia r1!, {r4}
	str r4, [sp, #8]
	ldmia r1!, {r2}
	str r2, [sp, #4]
	ldr r1, [r1]
	mov r8, r1
	adds r1, r3, #0
	bl Func_08021a84
	mov r12, r0
	cmp r0, #0
	bne .L_0802214c
	movs r3, #128
	lsls r3, r3, #9
	cmp r6, r3
	bne .L_0802214c
	cmp r5, r6
	bne .L_0802214c
	ldrh r2, [r7, #18]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0802214e
	movs r4, #0
	str r4, [sp, #16]
	str r4, [sp, #28]
	b .L_08022196
.L_0802214c:
	ldrh r2, [r7, #18]
.L_0802214e:
	movs r1, #1
	str r1, [sp, #16]
	add r0, sp, #32
	ldr r3, [r0, #4]
	ldr r4, .L_08022200
	ands r3, r4
	orrs r3, r2
	str r3, [r0, #4]
	ldr r1, [sp, #32]
	lsls r3, r6, #8
	lsrs r3, r3, #16
	ands r1, r4
	movs r2, #255
	orrs r1, r3
	lsls r2, r2, #8
	lsls r3, r5, #8
	adds r2, #255
	lsrs r3, r3, #16
	ands r1, r2
	lsls r3, r3, #16
	orrs r1, r3
	mov r2, r12
	str r1, [sp, #32]
	cmp r2, #0
	beq .L_08022190
	ldrh r3, [r0]
	adds r2, r4, #0
	negs r3, r3
	lsls r3, r3, #16
	lsrs r3, r3, #16
	ands r2, r1
	orrs r2, r3
	str r2, [sp, #32]
.L_08022190:
	bl AffineMatrix_BuildForEffect
	str r0, [sp, #28]
.L_08022196:
	movs r3, #128
	lsls r3, r3, #9
	cmp r6, r3
	bgt .L_080221a2
	cmp r5, r3
	ble .L_080221b2
.L_080221a2:
	ldr r4, [sp, #24]
	ldr r1, [sp, #20]
	movs r3, #3
	lsls r4, r4, #1
	lsls r1, r1, #1
	str r3, [sp, #16]
	str r4, [sp, #24]
	str r1, [sp, #20]
.L_080221b2:
	ldr r2, [sp, #4]
	mov r4, r8
	subs r3, r2, r4
	ldrb r2, [r7, #26]
	asrs r3, r3, #16
	subs r4, r3, #4
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08022208
	cmp r4, #159
	bgt .L_08022208
	adds r0, r7, #0
	adds r0, #28
	ldrb r2, [r0, #5]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	strb r3, [r0, #5]
	ldr r1, [sp, #12]
	ldr r3, .L_080221fc
	asrs r2, r1, #16
	subs r2, #8
	ldrh r1, [r0, #6]
	ands r2, r3
	ldr r3, .L_08022204
	strb r4, [r0, #4]
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r1, [sp, #4]
	add r1, r8
	asrs r1, r1, #16
	adds r1, #60
	bl Func_080140d8
	b .L_08022208
.L_080221fc:
	.4byte 0x000001ff
.L_08022200:
	.4byte 0xffff0000
.L_08022204:
	.4byte 0xfffffe00
.L_08022208:
	cmp r6, #0
	bne .L_0802220e
	b .L_0802230a
.L_0802220e:
	cmp r5, #0
	bne .L_08022214
	b .L_0802230a
.L_08022214:
	ldrh r3, [r7, #18]
	cmp r3, #0
	bne .L_08022234
	movs r3, #22
	ldrsb r3, [r7, r3]
	adds r2, r6, #0
	muls r2, r3
	ldrb r3, [r7, #21]
	mov r11, r2
	movs r2, #23
	ldrsb r2, [r7, r2]
	lsrs r3, r3, #1
	subs r3, r3, r2
	adds r0, r5, #0
	muls r0, r3
	b .L_08022288
.L_08022234:
	ldrh r0, [r7, #18]
	bl Trig_Sin
	str r0, [sp, #0]
	ldrh r0, [r7, #18]
	bl Trig_Cos
	movs r3, #22
	ldrsb r3, [r7, r3]
	movs r2, #23
	ldrsb r2, [r7, r2]
	adds r4, r6, #0
	muls r4, r3
	ldrb r3, [r7, #21]
	mov r9, r4
	lsrs r3, r3, #1
	subs r3, r3, r2
	adds r1, r5, #0
	muls r1, r3
	ldr r5, .L_080222f8
	mov r8, r1
	mov r1, r9
	mov r10, r0
	mov lr, r5
	.2byte 0xf800
	mov r1, r8
	adds r6, r0, #0
	ldr r0, [sp, #0]
	mov lr, r5
	.2byte 0xf800
	mov r1, r8
	adds r6, r6, r0
	mov r0, r10
	mov lr, r5
	.2byte 0xf800
	mov r11, r6
	mov r1, r9
	adds r6, r0, #0
	ldr r0, [sp, #0]
	mov lr, r5
	.2byte 0xf800
	subs r0, r6, r0
.L_08022288:
	ldr r3, [sp, #12]
	ldr r4, [sp, #24]
	movs r1, #255
	asrs r2, r3, #16
	lsls r1, r1, #8
	subs r2, r2, r4
	adds r1, #255
	mov r4, r11
	adds r3, r4, r1
	asrs r3, r3, #16
	adds r6, r2, r3
	ldr r4, [sp, #8]
	ldr r2, [sp, #4]
	adds r1, r0, r1
	subs r3, r2, r4
	ldr r2, [sp, #20]
	asrs r3, r3, #16
	subs r3, r3, r2
	asrs r1, r1, #16
	subs r4, r3, r1
	cmp r6, #239
	bgt .L_0802230a
	cmp r4, #159
	bgt .L_0802230a
	ldr r3, .L_080222f4
	adds r0, r7, #0
	ldrh r2, [r0, #6]
	ands r6, r3
	ldr r3, .L_080222fc
	strb r4, [r0, #4]
	ands r3, r2
	orrs r3, r6
	strh r3, [r0, #6]
	ldrb r2, [r0, #5]
	ldr r4, [sp, #16]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	orrs r3, r4
	strb r3, [r0, #5]
	ldr r1, [sp, #28]
	movs r3, #31
	ands r1, r3
	str r1, [sp, #28]
	movs r3, #63
	ldrb r2, [r0, #7]
	negs r3, r3
	lsls r1, r1, #1
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #7]
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	b .L_08022300
.L_080222f4:
	.4byte 0x000001ff
.L_080222f8:
	.4byte IwramMulQ16
.L_080222fc:
	.4byte 0xfffffe00
.L_08022300:
	adds r1, r2, r3
	asrs r1, r1, #16
	adds r1, #62
	bl Func_080140d8
.L_0802230a:
	add sp, #64
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
