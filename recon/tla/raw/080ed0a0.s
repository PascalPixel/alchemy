.syntax unified
	.thumb
	.global Func_080ed0a0
	.thumb_func
Func_080ed0a0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080ed27c
	ldr r1, .L_080ed280
	ldr r3, [r3]
	sub sp, #4
	ldmia r3!, {r2}
	adds r1, r1, r2
	ldr r3, [r3]
	ldr r2, .L_080ed284
	mov r10, r1
	adds r2, r2, r3
	movs r3, #224
	mov r8, r2
	lsls r3, r3, #16
	cmp r1, #0
	bge .L_080ed0d0
	movs r1, #0
	mov r10, r1
.L_080ed0d0:
	movs r2, #136
	lsls r2, r2, #17
	cmp r10, r2
	ble .L_080ed0da
	mov r10, r2
.L_080ed0da:
	mov r1, r8
	cmp r1, #0
	bge .L_080ed0e4
	movs r2, #0
	mov r8, r2
.L_080ed0e4:
	cmp r8, r3
	ble .L_080ed0ea
	mov r8, r3
.L_080ed0ea:
	ldr r3, .L_080ed288
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080ed0f6
	b .L_080ed262
.L_080ed0f6:
	ldr r2, .L_080ed28c
	mov r1, r10
	ldr r3, [r2]
	subs r0, r1, r3
	cmp r0, #0
	bge .L_080ed10a
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r0, r0, r2
.L_080ed10a:
	ldr r1, .L_080ed290
	asrs r0, r0, #16
	ldr r3, [r1]
	mov r2, r8
	mov r9, r0
	subs r0, r2, r3
	cmp r0, #0
	bge .L_080ed122
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r0, r0, r3
.L_080ed122:
	mov r1, r9
	asrs r6, r0, #16
	adds r3, r6, #0
	muls r3, r6
	mov r0, r9
	muls r0, r1
	adds r0, r0, r3
	ldr r3, .L_080ed294
	mov lr, r3
	.2byte 0xf800
	ldr r2, .L_080ed28c
	mov r1, r10
	ldr r3, [r2]
	adds r2, #4
	subs r1, r1, r3
	ldr r3, [r2]
	movs r2, #128
	mov r9, r1
	lsls r7, r0, #16
	mov r1, r8
	lsls r2, r2, #15
	subs r6, r1, r3
	cmp r7, r2
	bge .L_080ed174
	ldr r3, .L_080ed298
	mov r1, r9
	mov r11, r3
	mov r0, r9
	mov lr, r11
	.2byte 0xf800
	adds r1, r6, #0
	adds r7, r0, #0
	adds r0, r6, #0
	mov lr, r11
	.2byte 0xf800
	adds r7, r7, r0
	adds r0, r7, #0
	str r7, [sp, #0]
	bl Func_080149e0
	adds r7, r0, #0
.L_080ed174:
	adds r1, r7, #0
	cmp r7, #0
	bge .L_080ed17c
	adds r1, r7, #3
.L_080ed17c:
	movs r3, #128
	asrs r5, r1, #2
	lsls r3, r3, #12
	cmp r5, r3
	ble .L_080ed188
	adds r5, r3, #0
.L_080ed188:
	movs r1, #128
	lsls r1, r1, #7
	cmp r7, r1
	blt .L_080ed1f4
	cmp r7, r5
	ble .L_080ed1bc
	ldr r2, .L_080ed29c
	mov r1, r9
	mov r10, r2
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	ldr r3, .L_080ed298
	adds r1, r5, #0
	mov r8, r3
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	mov r9, r0
	adds r0, r7, #0
	mov lr, r10
	.2byte 0xf800
	adds r1, r5, #0
	mov lr, r8
	.2byte 0xf800
	adds r6, r0, #0
.L_080ed1bc:
	ldr r1, .L_080ed28c
	ldr r2, .L_080ed290
	ldr r3, [r1]
	add r3, r9
	mov r10, r3
	ldr r3, [r2]
	adds r3, r3, r6
	mov r8, r3
	mov r3, r10
	cmp r3, #0
	bge .L_080ed1d6
	movs r1, #0
	mov r10, r1
.L_080ed1d6:
	mov r2, r8
	cmp r2, #0
	bge .L_080ed1e0
	movs r3, #0
	mov r8, r3
.L_080ed1e0:
	movs r1, #136
	lsls r1, r1, #17
	cmp r10, r1
	ble .L_080ed1ea
	mov r10, r1
.L_080ed1ea:
	movs r2, #136
	lsls r2, r2, #17
	cmp r8, r2
	ble .L_080ed1f4
	mov r8, r2
.L_080ed1f4:
	mov r3, r10
	lsrs r6, r3, #19
	ldr r3, .L_080ed28c
	mov r1, r8
	lsrs r5, r1, #19
	ldr r1, [r3]
	mov r2, r10
	adds r3, r1, #0
	eors r3, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r2
	cmp r3, #0
	beq .L_080ed228
	cmp r1, r10
	bge .L_080ed220
	adds r0, r6, #0
	adds r0, #30
	adds r1, r5, #0
	bl Func_080ed030
	b .L_080ed228
.L_080ed220:
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080ed030
.L_080ed228:
	ldr r3, .L_080ed290
	mov r2, r8
	ldr r1, [r3]
	adds r3, r1, #0
	eors r3, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r2
	cmp r3, #0
	beq .L_080ed254
	cmp r1, r8
	bge .L_080ed24c
	adds r1, r5, #0
	adds r1, #20
	adds r0, r6, #0
	bl Func_080ecf74
	b .L_080ed254
.L_080ed24c:
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_080ecf74
.L_080ed254:
	ldr r3, .L_080ed2a0
	mov r1, r10
	asrs r2, r1, #16
	mov r1, r8
	strh r2, [r3, #4]
	asrs r2, r1, #16
	strh r2, [r3, #6]
.L_080ed262:
	ldr r3, .L_080ed28c
	mov r2, r10
	str r2, [r3]
	mov r1, r8
	adds r3, #4
	str r1, [r3]
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080ed27c:
	.4byte Data_0202a62c
.L_080ed280:
	.4byte 0xff880000
.L_080ed284:
	.4byte 0xffb00000
.L_080ed288:
	.4byte Data_0202a640
.L_080ed28c:
	.4byte Data_0202a630
.L_080ed290:
	.4byte Data_0202a634
.L_080ed294:
	.4byte IwramFillWords + 0x74
.L_080ed298:
	.4byte IwramMulQ16
.L_080ed29c:
	.4byte IwramRatioMulQ14
.L_080ed2a0:
	.4byte Data_03001120
