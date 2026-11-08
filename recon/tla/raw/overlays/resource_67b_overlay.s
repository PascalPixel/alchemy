.syntax unified
	.thumb
	.section .text.x02008580,"ax",%progbits
	.balign 4
	.global Func_02000580
	.thumb_func
Func_02000580:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_020085e0
	sub sp, #20
	movs r1, #0
	mov r10, r0
	movs r2, #2
	str r1, [sp, #16]
	add r2, r10
	mov r8, r2
.L_0200859e:
	mov r4, r8
	ldrh r3, [r4, #16]
	ldr r0, .L_020085d4
	mov r6, r8
	ands r0, r3
	bl GameFlag_Test
	ldrh r3, [r6, #16]
	ldr r2, .L_020085d8
	adds r4, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_020085bc
	movs r3, #1
	eors r4, r3
.L_020085bc:
	mov r1, r8
	movs r0, #18
	ldrsh r3, [r1, r0]
	cmp r3, r4
	beq .L_0200860a
	movs r2, #28
	ldrsh r3, [r1, r2]
	ldr r2, .L_020085dc
	lsls r3, r3, #1
	mov r6, r8
	b .L_020085e4
	.2byte 0x0000
.L_020085d4:
	.4byte 0x00000fff
.L_020085d8:
	.4byte 0x00001000
.L_020085dc:
	.4byte 0x00000000
.L_020085e0:
	.4byte Data_02003450
.L_020085e4:
	adds r3, #20
	strh r2, [r6, r3]
	movs r2, #0
	movs r0, #28
	ldrsh r3, [r6, r0]
	movs r0, #128
	lsls r3, r3, #1
	adds r3, #24
	strh r4, [r1, r3]
	lsls r0, r0, #9
	ldrh r3, [r1, #28]
	adds r3, #1
	strh r3, [r6, #28]
	lsls r3, r3, #16
	cmp r3, r0
	ble .L_02008606
	strh r2, [r1, #28]
.L_02008606:
	mov r2, r8
	strh r4, [r2, #18]
.L_0200860a:
	movs r4, #24
	str r4, [sp, #8]
	movs r3, #20
	movs r6, #1
	mov r9, r3
	mov r11, r6
.L_02008616:
	mov r0, r8
	str r0, [sp, #12]
	mov r3, r9
	mov r1, r9
	ldrsh r7, [r0, r3]
	movs r4, #14
	ldrsh r3, [r0, r4]
	ldrh r1, [r0, r1]
	lsls r3, r3, #2
	mov r12, r1
	cmp r7, r3
	bge .L_02008706
	mov r6, r10
	ldrh r4, [r6, #8]
	mov r2, r8
	adds r3, r7, #0
	ldrh r0, [r6]
	ldrh r1, [r2]
	mov lr, r4
	ldrh r5, [r6, #10]
	cmp r7, #0
	bge .L_02008644
	adds r3, r7, #3
.L_02008644:
	asrs r7, r3, #2
	ldr r3, [sp, #8]
	mov r6, r8
	ldrsh r4, [r6, r3]
	movs r3, #1
	adds r6, r4, #0
	mov r2, r12
	eors r6, r3
	ands r3, r2
	cmp r3, #0
	beq .L_020086aa
	mov r3, lr
	lsls r2, r3, #16
	lsls r3, r5, #16
	lsls r0, r0, #16
	lsls r5, r4, #1
	lsls r1, r1, #16
	asrs r3, r3, #16
	adds r5, r5, r4
	asrs r2, r2, #16
	adds r3, r3, r7
	asrs r0, r0, #16
	asrs r1, r1, #16
	adds r0, r5, r0
	adds r1, r1, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #3
	movs r3, #1
	bl Func_02002968
	mov r6, r10
	movs r4, #4
	ldrsh r2, [r6, r4]
	movs r0, #6
	ldrsh r1, [r6, r0]
	movs r3, #12
	ldrsh r0, [r6, r3]
	movs r4, #14
	ldrsh r3, [r6, r4]
	adds r5, r5, r2
	adds r3, r3, r7
	str r0, [sp, #0]
	str r3, [sp, #4]
	adds r1, r1, r7
	adds r0, r5, #0
	movs r2, #3
	movs r3, #1
	bl Func_02002958
	b .L_020086f8
.L_020086aa:
	mov r3, lr
	lsls r2, r3, #16
	lsls r3, r5, #16
	lsls r0, r0, #16
	lsls r5, r6, #1
	lsls r1, r1, #16
	asrs r3, r3, #16
	adds r5, r5, r6
	asrs r2, r2, #16
	adds r3, r3, r7
	asrs r0, r0, #16
	asrs r1, r1, #16
	adds r0, r5, r0
	adds r1, r1, r7
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r2, #3
	movs r3, #1
	bl Func_02002968
	mov r6, r10
	movs r4, #4
	ldrsh r2, [r6, r4]
	movs r0, #6
	ldrsh r1, [r6, r0]
	movs r3, #12
	ldrsh r0, [r6, r3]
	movs r4, #14
	ldrsh r3, [r6, r4]
	adds r5, r5, r2
	adds r3, r3, r7
	str r0, [sp, #0]
	str r3, [sp, #4]
	adds r1, r1, r7
	adds r0, r5, #0
	movs r2, #3
	movs r3, #1
	bl Func_02002958
.L_020086f8:
	ldr r6, [sp, #12]
	mov r0, r9
	ldrh r3, [r6, r0]
	adds r1, r6, #0
	adds r3, #1
	mov r2, r9
	strh r3, [r1, r2]
.L_02008706:
	ldr r4, [sp, #8]
	movs r6, #1
	negs r6, r6
	add r11, r6
	movs r3, #2
	adds r4, #2
	mov r0, r11
	add r9, r3
	str r4, [sp, #8]
	cmp r0, #0
	blt .L_0200871e
	b .L_02008616
.L_0200871e:
	ldr r1, [sp, #16]
	movs r2, #32
	adds r1, #1
	str r1, [sp, #16]
	add r8, r2
	add r10, r2
	cmp r1, #4
	bgt .L_02008730
	b .L_0200859e
.L_02008730:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200a250,"ax",%progbits
	.balign 4
	.global Func_02002250
	.thumb_func
Func_02002250:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_0200a3ac
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, [r1]
	ldr r7, .L_0200a3b0
	ldr r3, [r3, #4]
	mov r8, r2
	mov r9, r3
	ldrh r3, [r7, #4]
	ldr r2, .L_0200a3b4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	mov r10, r0
	lsrs r3, r3, #5
	mov r11, r3
	bl Random16Far
	movs r3, #128
	lsls r3, r3, #5
	cmp r0, r3
	bcs .L_0200a2de
	ldr r6, [r7]
	cmp r6, #0
	beq .L_0200a2de
	ldrh r2, [r7, #6]
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, r3, r7
	adds r5, r3, #0
	bl Random16Far
	ldr r3, [r6, #8]
	lsls r2, r0, #1
	adds r2, r2, r0
	ldr r0, .L_0200a3b8
	lsls r2, r2, #4
	adds r3, r3, r2
	adds r5, #12
	adds r3, r3, r0
	str r3, [r5]
	movs r2, #0
	ldr r3, [r6, #12]
	movs r1, #252
	str r3, [r5, #4]
	lsls r1, r1, #14
	ldr r3, [r6, #16]
	str r2, [r5, #12]
	str r3, [r5, #8]
	ldrh r3, [r7, #6]
	adds r3, #1
	strh r3, [r7, #6]
	lsls r3, r3, #16
	cmp r3, r1
	bls .L_0200a2de
	strh r2, [r7, #6]
.L_0200a2de:
	adds r5, r7, #0
	adds r5, #12
	movs r6, #63
.L_0200a2e4:
	ldr r1, [r5]
	cmp r1, #0
	beq .L_0200a396
	ldr r2, [r5, #8]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, [r5, #4]
	cmp r0, r3
	ble .L_0200a302
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #4]
	b .L_0200a30a
.L_0200a302:
	ldr r3, [r5, #8]
	ldr r0, .L_0200a3bc
	adds r3, r3, r0
	str r3, [r5, #8]
.L_0200a30a:
	ldr r3, [r5, #4]
	asrs r3, r3, #20
	cmp r3, #23
	bne .L_0200a316
	movs r3, #0
	str r3, [r5]
.L_0200a316:
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #48
	bne .L_0200a322
	movs r3, #0
	str r3, [r5]
.L_0200a322:
	ldr r3, [r5, #12]
	mov r1, r10
	adds r3, #1
	str r3, [r5, #12]
	ldr r3, [r5]
	mov r0, r8
	subs r4, r3, r1
	ldr r1, [r5, #8]
	ldr r3, [r5, #4]
	mov r2, r9
	subs r1, r1, r0
	subs r1, r1, r2
	subs r3, r3, r2
	subs r2, r1, r3
	adds r3, r3, r1
	asrs r3, r3, #16
	asrs r0, r4, #16
	adds r1, r3, #0
	movs r3, #167
	subs r4, r0, #4
	asrs r2, r2, #16
	adds r0, #11
	lsls r3, r3, #1
	subs r2, #4
	adds r1, #58
	cmp r0, r3
	bhi .L_0200a396
	movs r0, #16
	negs r0, r0
	cmp r2, r0
	ble .L_0200a396
	cmp r2, #239
	bgt .L_0200a396
	adds r3, #177
	ands r4, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r5, #16]
	lsls r3, r4, #16
	orrs r2, r3
	movs r3, #128
	lsls r3, r3, #6
	orrs r2, r3
	ldr r3, [r5, #12]
	str r2, [r5, #20]
	movs r2, #3
	ands r3, r2
	lsls r3, r3, #1
	movs r2, #128
	add r3, r11
	lsls r2, r2, #3
	orrs r3, r2
	adds r0, r5, #0
	str r3, [r5, #24]
	adds r0, #16
	bl Func_020028d0
.L_0200a396:
	subs r6, #1
	adds r5, #28
	cmp r6, #0
	bge .L_0200a2e4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a3ac:
	.4byte 0xffff0000
.L_0200a3b0:
	.4byte gData_020034f0
.L_0200a3b4:
	.4byte gResourceTableEntries
.L_0200a3b8:
	.4byte 0xffe80000
.L_0200a3bc:
	.4byte 0xfffe0000
	.section .bss,"aw",%nobits
	.space 0x00000002
	.global Data_02003450
Data_02003450:
	.space 0x00000020
	.global Data_02003470
Data_02003470:
	.space 0x00000080
	.global gData_020034f0
gData_020034f0:
