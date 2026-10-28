.syntax unified
	.thumb
	.section .text.x02008384,"ax",%progbits
	.balign 4
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #128
	adds r7, r0, #0
	lsls r2, r2, #1
	movs r0, #0
	sub sp, #24
	mov r10, r0
	mov r9, r2
.L_0200839c:
	movs r3, #1
	add r10, r3
	mov r5, r10
	cmp r5, #3
	bgt .L_02008436
	ldr r3, [r7, #8]
	add r0, sp, #12
	str r3, [r0]
	ldr r3, [r7, #12]
	mov r8, r0
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	str r3, [r0, #8]
	bl Random16Far
	movs r1, #128
	ldr r3, .L_02008464
	lsls r1, r1, #12
	mov lr, r3
	.2byte 0xf800
	movs r2, #128
	lsls r2, r2, #11
	adds r6, r0, #0
	adds r6, r6, r2
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldrh r1, [r7, #6]
	lsrs r5, r5, #2
	adds r1, r1, r5
	lsrs r0, r0, #2
	subs r1, r1, r0
	mov r2, r8
	adds r0, r6, #0
	mov r5, r8
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	adds r1, r3, #0
	cmp r3, #0
	bge .L_020083fa
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_020083fa:
	adds r2, r7, #0
	adds r2, #100
	movs r5, #0
	ldrsh r2, [r2, r5]
	asrs r3, r3, #16
	subs r0, r3, r2
	mov r3, r8
	ldr r2, [r3, #8]
	adds r4, r2, #0
	cmp r2, #0
	bge .L_02008418
	movs r5, #255
	lsls r5, r5, #8
	adds r5, #255
	adds r2, r2, r5
.L_02008418:
	adds r3, r7, #0
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
	cmp r3, r9
	ble .L_0200844a
	b .L_0200839c
.L_02008436:
	ldrh r3, [r7, #6]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	adds r2, r7, #0
	strh r3, [r7, #6]
	adds r2, #94
	movs r3, #1
	strh r3, [r2]
	b .L_02008456
.L_0200844a:
	mov r3, r8
	ldr r2, [r3, #4]
	adds r0, r7, #0
	adds r3, r4, #0
	bl Func_02002930
.L_02008456:
	movs r0, #0
	add sp, #24
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008464:
	.4byte IwramMulQ16
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
	.section .text.x02008844,"ax",%progbits
	.balign 4
	.global Func_02000844
	.thumb_func
Func_02000844:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_020088b0
	movs r3, #64
	movs r2, #70
	mov r10, r2
	strh r3, [r5]
	movs r3, #76
	strh r3, [r5, #4]
	mov r3, r10
	strh r3, [r5, #8]
	movs r3, #6
	strh r3, [r5, #12]
	movs r4, #10
	ldr r3, .L_020088a4
	mov r11, r4
	movs r4, #128
	movs r6, #74
	mov r2, r11
	lsls r4, r4, #2
	movs r0, #128
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r2, [r5, #10]
	strh r6, [r5, #14]
	strh r3, [r5, #16]
	strh r4, [r5, #18]
	lsls r0, r0, #2
	sub sp, #8
	bl GameFlag_Test
	movs r3, #156
	ldr r7, .L_020088a8
	ldr r2, .L_020088ac
	lsls r3, r3, #6
	adds r3, #15
	mov r8, r3
	strh r2, [r5, #30]
	eors r0, r7
	mov r2, r8
	mov r4, r8
	strh r0, [r5, #20]
	b .L_020088b4
.L_020088a4:
	.4byte 0x00000024
.L_020088a8:
	.4byte 0x00000001
.L_020088ac:
	.4byte 0x00000000
.L_020088b0:
	.4byte Data_02003450
.L_020088b4:
	strh r4, [r5, #24]
	strh r2, [r5, #22]
	mov r3, r10
	adds r5, #32
	strh r3, [r5]
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r6, [r5, #14]
	movs r3, #82
	ldr r6, .L_02008900
	movs r0, #129
	strh r3, [r5, #4]
	lsls r0, r0, #1
	movs r3, #78
	mov r4, r11
	strh r3, [r5, #8]
	adds r0, #255
	movs r3, #14
	strh r4, [r5, #10]
	strh r3, [r5, #12]
	strh r6, [r5, #16]
	strh r0, [r5, #18]
	bl GameFlag_Test
	ldr r2, .L_02008904
	eors r0, r7
	mov r3, r8
	mov r4, r8
	strh r0, [r5, #20]
	strh r2, [r5, #30]
	strh r3, [r5, #24]
	strh r4, [r5, #22]
	movs r3, #102
	adds r5, #32
	movs r2, #26
	strh r3, [r5, #4]
	b .L_02008908
	.2byte 0x0000
.L_02008900:
	.4byte 0x00000024
.L_02008904:
	.4byte 0x00000000
.L_02008908:
	mov r9, r2
	ldr r6, .L_02008944
	movs r3, #103
	strh r3, [r5, #8]
	ldr r4, .L_02008944
	movs r2, #15
	mov r3, r9
	movs r0, #128
	mov r11, r2
	strh r3, [r5, #10]
	lsls r0, r0, #2
	movs r3, #39
	strh r6, [r5]
	strh r3, [r5, #12]
	movs r6, #93
	mov r3, r11
	adds r0, #2
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r4, [r5, #14]
	strh r3, [r5, #16]
	strh r0, [r5, #18]
	bl GameFlag_Test
	ldr r4, .L_02008948
	eors r0, r7
	mov r2, r8
	mov r3, r8
	strh r0, [r5, #20]
	b .L_0200894c
.L_02008944:
	.4byte 0x0000005a
.L_02008948:
	.4byte 0x00000000
.L_0200894c:
	strh r4, [r5, #30]
	strh r2, [r5, #24]
	strh r3, [r5, #22]
	movs r4, #96
	adds r5, #32
	movs r3, #108
	mov r10, r4
	strh r3, [r5, #4]
	ldr r4, .L_02008998
	movs r3, #107
	movs r0, #130
	strh r3, [r5, #8]
	lsls r0, r0, #1
	mov r3, r9
	mov r2, r10
	strh r6, [r5, #2]
	strh r6, [r5, #6]
	strh r3, [r5, #10]
	mov r6, r11
	movs r3, #43
	adds r0, #255
	strh r2, [r5]
	strh r3, [r5, #12]
	strh r4, [r5, #14]
	strh r6, [r5, #16]
	strh r0, [r5, #18]
	bl GameFlag_Test
	ldr r2, .L_0200899c
	mov r4, r8
	eors r0, r7
	mov r3, r8
	strh r0, [r5, #20]
	strh r2, [r5, #30]
	strh r3, [r5, #24]
	strh r4, [r5, #22]
	movs r3, #116
	b .L_020089a0
.L_02008998:
	.4byte 0x0000005a
.L_0200899c:
	.4byte 0x00000000
.L_020089a0:
	adds r5, #32
	strh r3, [r5]
	movs r0, #208
	movs r3, #122
	movs r2, #95
	strh r3, [r5, #4]
	lsls r0, r0, #5
	movs r3, #32
	strh r2, [r5, #2]
	strh r2, [r5, #6]
	mov r6, r10
	strh r3, [r5, #10]
	strh r3, [r5, #12]
	mov r2, r10
	movs r3, #13
	adds r0, #107
	strh r6, [r5, #8]
	strh r3, [r5, #16]
	strh r2, [r5, #14]
	strh r0, [r5, #18]
	bl GameFlag_Test
	ldr r3, .L_020089f4
	eors r0, r7
	strh r0, [r5, #20]
	mov r4, r8
	mov r6, r8
	movs r0, #200
	strh r3, [r5, #30]
	strh r4, [r5, #24]
	strh r6, [r5, #22]
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089f8
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008a00
	.2byte 0x0000
.L_020089f4:
	.4byte 0x00000000
.L_020089f8:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02008a00:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a1a
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008a24
.L_02008a1a:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008a24:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a80
	ldr r5, .L_02008b00
	movs r2, #0
	ldrsh r0, [r5, r2]
	movs r6, #8
	ldrsh r2, [r5, r6]
	movs r3, #2
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #10
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002968
	movs r2, #4
	ldrsh r0, [r5, r2]
	movs r6, #12
	ldrsh r2, [r5, r6]
	movs r3, #6
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #14
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002958
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #15
	strh r3, [r5, #24]
	strh r3, [r5, #22]
	movs r3, #1
	strh r3, [r5, #20]
.L_02008a80:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ade
	ldr r5, .L_02008b04
	movs r2, #0
	ldrsh r0, [r5, r2]
	movs r6, #8
	ldrsh r2, [r5, r6]
	movs r3, #2
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #10
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002968
	movs r2, #4
	ldrsh r0, [r5, r2]
	movs r6, #12
	ldrsh r2, [r5, r6]
	movs r3, #6
	ldrsh r1, [r5, r3]
	movs r4, #16
	ldrsh r3, [r5, r4]
	movs r6, #14
	ldrsh r4, [r5, r6]
	adds r0, #3
	str r2, [sp, #0]
	movs r2, #3
	str r4, [sp, #4]
	bl Func_02002958
	movs r3, #156
	lsls r3, r3, #6
	adds r3, #15
	strh r3, [r5, #24]
	strh r3, [r5, #22]
	movs r3, #1
	strh r3, [r5, #20]
.L_02008ade:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008b08
	bl Task_Add
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008b0c
	bl Task_Add
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008b00:
	.4byte Data_02003450
.L_02008b04:
	.4byte Data_02003470
.L_02008b08:
	.4byte Func_02000740
.L_02008b0c:
	.4byte Func_02000580
	.section .text.x02008d80,"ax",%progbits
	.balign 4
	.global Func_02000d80
	.thumb_func
Func_02000d80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #104
	sub sp, #12
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008da0
	b .L_02008f5a
.L_02008da0:
	ldr r0, .L_02008f6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #133
	mov r10, r0
	lsls r3, r3, #2
	add r3, r10
	movs r1, #230
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	mov r8, r3
	mov r0, r8
	mov r9, r2
	bl Object_GetById
	adds r6, r0, #0
	ldr r0, .L_02008f70
	ldr r1, .L_02008f74
	movs r2, #0
	ldrsh r3, [r0, r2]
	mov r11, r1
	cmp r3, r11
	bne .L_02008ddc
	ldr r3, [r6, #20]
	cmp r3, #0
	bne .L_02008ddc
	b .L_02008f5e
.L_02008ddc:
	ldr r3, [r6, #8]
	mov r7, sp
	str r3, [r7]
	movs r2, #128
	ldr r3, [r6, #12]
	lsls r2, r2, #10
	str r3, [r7, #4]
	adds r0, r6, #0
	ldr r3, [r6, #16]
	adds r1, r7, #0
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02002970
	ldr r3, .L_02008f78
	movs r2, #4
	ldr r3, [r3]
	adds r5, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_02008e0c
	adds r0, r6, #0
	bl Func_02001d30
.L_02008e0c:
	cmp r5, #0
	bge .L_02008e5c
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_020029f8
	ldr r3, [r6, #16]
	ldr r0, .L_02008f7c
	ldr r1, [r6, #8]
	adds r3, r3, r0
	ldr r2, [r6, #12]
	adds r0, r6, #0
	bl Func_02002930
	adds r0, r6, #0
	movs r1, #7
	bl Func_02002910
	adds r0, r6, #0
	bl Func_02002938
.L_02008e38:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r6, #12]
	ldr r3, [r6, #20]
	cmp r2, r3
	bne .L_02008e38
	adds r0, r6, #0
	bl Func_02001d30
	adds r0, r6, #0
	movs r1, #6
	bl Func_02002910
	movs r0, #3
	bl WaitFrames
	b .L_02008f5e
.L_02008e5c:
	ldr r3, [r6, #8]
	ldr r1, .L_02008f7c
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02002970
	adds r5, r0, #0
	cmp r5, #0
	ble .L_02008f06
	ldr r0, .L_02008f70
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, r11
	bne .L_02008f5e
	bl Func_02002998
	movs r0, #0
	bl Func_02002a48
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #0
	adds r0, r6, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #18
	add r2, r10
	movs r3, #2
	strb r3, [r2]
	adds r0, r6, #0
	bl Func_02001d30
	movs r0, #5
	bl FieldEvent_WaitFrames
	movs r5, #59
.L_02008eb8:
	ldr r3, [r6, #12]
	movs r1, #128
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r6, #12]
	ldrh r3, [r6, #6]
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	bl Random16Far
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	cmp r0, r3
	bhi .L_02008ee0
	adds r0, r6, #0
	bl Func_02001d30
.L_02008ee0:
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_02008eb8
	ldr r3, .L_02008f6c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r3, r0
	movs r2, #0
	strb r2, [r3]
	bl Func_020029a0
	movs r0, #6
	bl Func_02002a18
	b .L_02008f5e
.L_02008f06:
	ldr r3, [r6, #8]
	ldr r1, .L_02008f80
	ldr r2, .L_02008f84
	adds r3, r3, r1
	str r3, [r7]
	adds r0, r6, #0
	ldr r3, [r6, #12]
	adds r1, r7, #0
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02002970
	adds r5, r0, #0
	cmp r5, #0
	bgt .L_02008f5e
	ldr r3, [r6, #8]
	ldr r0, .L_02008f84
	adds r1, r7, #0
	adds r3, r3, r0
	str r3, [r7]
	ldr r3, [r6, #12]
	str r3, [r7, #4]
	ldr r3, [r6, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r6, #0
	bl Func_02002970
	adds r5, r0, #0
	cmp r5, #0
	bgt .L_02008f5e
	mov r1, r9
	ldr r3, [r1, #16]
	ldr r2, .L_02008f88
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r6, #16]
	adds r3, r3, r2
	str r3, [r6, #16]
	b .L_02008f5e
.L_02008f5a:
	bl Func_02001e00
.L_02008f5e:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008f6c:
	.4byte gAdventure
.L_02008f70:
	.4byte Data_0200024c + 0x1d4
.L_02008f74:
	.4byte 0x0000009e
.L_02008f78:
	.4byte gFrameCount
.L_02008f7c:
	.4byte 0xfff80000
.L_02008f80:
	.4byte 0x0005b333
.L_02008f84:
	.4byte 0xfffa4ccd
.L_02008f88:
	.4byte 0xfffe0000
	.section .text.x02009e00,"ax",%progbits
	.balign 4
	.global Func_02001e00
	.thumb_func
Func_02001e00:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r6, .L_02009f4c
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #133
	ldr r2, [r3, #108]
	lsls r0, r0, #2
	adds r3, r6, r0
	movs r1, #230
	ldr r3, [r3]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	mov r8, r3
	mov r0, r8
	mov r10, r2
	sub sp, #12
	bl Object_GetById
	movs r2, #240
	lsls r2, r2, #1
	adds r3, r6, r2
	adds r5, r0, #0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_02009f50
	cmp r2, r3
	bne .L_02009e44
	ldr r3, [r5, #20]
	cmp r3, #0
	beq .L_02009f40
.L_02009e44:
	ldr r3, [r5, #8]
	mov r7, sp
	str r3, [r7]
	movs r1, #128
	ldr r3, [r5, #12]
	lsls r1, r1, #10
	str r3, [r7, #4]
	adds r0, r5, #0
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02002970
	ldr r3, .L_02009f54
	movs r2, #4
	ldr r3, [r3]
	adds r6, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_02009e74
	adds r0, r5, #0
	bl Func_02001d30
.L_02009e74:
	cmp r6, #0
	bge .L_02009ec6
	movs r1, #129
	mov r0, r8
	lsls r1, r1, #1
	bl Func_020029f8
	ldr r3, [r5, #16]
	movs r0, #128
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r5, #8]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	bl Func_02002930
	adds r0, r5, #0
	movs r1, #49
	bl Func_02002910
	adds r0, r5, #0
	bl Func_02002938
.L_02009ea2:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	cmp r2, r3
	bne .L_02009ea2
	adds r0, r5, #0
	bl Func_02001d30
	adds r0, r5, #0
	movs r1, #49
	bl Func_02002910
	movs r0, #3
	bl WaitFrames
	b .L_02009f40
.L_02009ec6:
	ldr r3, [r5, #8]
	movs r1, #128
	str r3, [r7]
	lsls r1, r1, #12
	ldr r3, [r5, #12]
	adds r0, r5, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r1
	str r3, [r7, #8]
	adds r1, r7, #0
	bl Func_02002970
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_02009f40
	ldr r3, [r5, #8]
	ldr r2, .L_02009f58
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Func_02002970
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_02009f40
	ldr r3, [r5, #8]
	ldr r2, .L_02009f5c
	ldr r0, .L_02009f58
	adds r3, r3, r2
	str r3, [r7]
	adds r1, r7, #0
	ldr r3, [r5, #12]
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02002970
	adds r6, r0, #0
	cmp r6, #0
	bgt .L_02009f40
	mov r1, r10
	ldr r3, [r1, #16]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r1, #16]
	ldr r3, [r5, #16]
	adds r3, r3, r2
	str r3, [r5, #16]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
.L_02009f40:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009f4c:
	.4byte gAdventure
.L_02009f50:
	.4byte 0x0000009e
.L_02009f54:
	.4byte gFrameCount
.L_02009f58:
	.4byte 0x0005b333
.L_02009f5c:
	.4byte 0xfffa4ccd
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
	.section .text.x0200a3c0,"ax",%progbits
	.balign 4
	.global Func_020023c0
	.thumb_func
Func_020023c0:
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
	ldr r3, .L_0200a520
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, [r1]
	ldr r7, .L_0200a524
	ldr r3, [r3, #4]
	mov r8, r2
	mov r9, r3
	ldrh r3, [r7, #4]
	ldr r2, .L_0200a528
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
	bcs .L_0200a44e
	ldr r6, [r7]
	cmp r6, #0
	beq .L_0200a44e
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
	ldr r0, .L_0200a52c
	lsls r2, r2, #3
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
	bls .L_0200a44e
	strh r2, [r7, #6]
.L_0200a44e:
	adds r5, r7, #0
	adds r5, #12
	movs r6, #63
.L_0200a454:
	ldr r1, [r5]
	cmp r1, #0
	beq .L_0200a50a
	ldr r2, [r5, #8]
	movs r0, #0
	bl Map_GetTerrainHeight
	ldr r3, [r5, #4]
	cmp r0, r3
	bge .L_0200a470
	ldr r2, .L_0200a530
	adds r3, r3, r2
	str r3, [r5, #4]
	b .L_0200a47a
.L_0200a470:
	ldr r3, [r5, #8]
	movs r0, #128
	lsls r0, r0, #10
	adds r3, r3, r0
	str r3, [r5, #8]
.L_0200a47a:
	ldr r3, [r5, #4]
	movs r1, #4
	asrs r3, r3, #20
	negs r1, r1
	cmp r3, r1
	bne .L_0200a48a
	movs r3, #0
	str r3, [r5]
.L_0200a48a:
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #50
	bne .L_0200a496
	movs r3, #0
	str r3, [r5]
.L_0200a496:
	ldr r3, [r5, #12]
	mov r2, r10
	adds r3, #1
	str r3, [r5, #12]
	ldr r3, [r5]
	ldr r1, [r5, #8]
	subs r4, r3, r2
	ldr r3, [r5, #4]
	mov r2, r8
	mov r0, r9
	subs r1, r1, r2
	subs r1, r1, r0
	subs r3, r3, r0
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
	bhi .L_0200a50a
	movs r0, #16
	negs r0, r0
	cmp r2, r0
	ble .L_0200a50a
	cmp r2, #239
	bgt .L_0200a50a
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
.L_0200a50a:
	subs r6, #1
	adds r5, #28
	cmp r6, #0
	bge .L_0200a454
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a520:
	.4byte 0xffff0000
.L_0200a524:
	.4byte gData_020034f0
.L_0200a528:
	.4byte gResourceTableEntries
.L_0200a52c:
	.4byte 0xfff40000
.L_0200a530:
	.4byte 0xfffe0000
	.section .text.x0200a5d4,"ax",%progbits
	.balign 4
	.global Func_020025d4
	.thumb_func
Func_020025d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #32]
	ldr r2, [r2, #116]
	adds r3, #228
	ldr r1, [r3]
	ldr r3, [r3, #4]
	mov r8, r2
	mov r5, r8
	movs r2, #0
	adds r5, #8
	mov r11, r1
	mov r9, r3
	mov r10, r2
.L_0200a5fe:
	ldrh r3, [r5, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	adds r2, r1, #0
	ands r2, r3
	strh r3, [r5, #28]
	cmp r2, r1
	bne .L_0200a614
	b .L_0200a74a
.L_0200a614:
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a626
	ldrh r3, [r5, #28]
	adds r3, #1
	strh r3, [r5, #28]
.L_0200a626:
	ldrh r2, [r5, #28]
	mov r1, r11
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_0200a6d4
	lsls r3, r3, #1
	adds r4, r3, r2
	ldr r3, [r5, #12]
	subs r2, r3, r1
	cmp r2, #0
	bge .L_0200a644
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_0200a644:
	movs r1, #0
	ldrsh r3, [r4, r1]
	asrs r2, r2, #16
	adds r7, r2, r3
	ldr r2, [r5, #16]
	ldr r3, [r5, #20]
	adds r4, #2
	subs r3, r3, r2
	mov r2, r9
	subs r3, r3, r2
	cmp r3, #0
	bge .L_0200a664
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
.L_0200a664:
	movs r1, #0
	ldrsh r2, [r4, r1]
	asrs r3, r3, #16
	adds r6, r3, r2
	adds r3, r7, #0
	adds r3, #16
	adds r4, #2
	cmp r3, #255
	bhi .L_0200a6f4
	movs r2, #32
	negs r2, r2
	cmp r6, r2
	blt .L_0200a6f4
	cmp r6, #159
	bgt .L_0200a6f4
	ldrb r3, [r5, #9]
	movs r1, #13
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	ldr r3, .L_0200a6c4
	ldr r2, .L_0200a6c8
	ands r7, r3
	ldrh r3, [r5, #6]
	strb r6, [r5, #4]
	ands r3, r2
	orrs r3, r7
	strh r3, [r5, #6]
	mov r2, r8
	ldrh r3, [r4]
	ldr r1, [r2, #4]
	ldr r2, .L_0200a6cc
	adds r1, r1, r3
	ldr r3, .L_0200a6d0
	adds r4, #2
	ands r1, r3
	ldrh r3, [r5, #8]
	ldrb r0, [r5, #5]
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	movs r2, #63
	ldrb r1, [r4]
	adds r3, r2, #0
	b .L_0200a6d8
.L_0200a6c4:
	.4byte 0x000001ff
.L_0200a6c8:
	.4byte 0xfffffe00
.L_0200a6cc:
	.4byte 0xfffffc00
.L_0200a6d0:
	.4byte 0x000003ff
.L_0200a6d4:
	.4byte gData_02002cd4
.L_0200a6d8:
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	ldrb r1, [r5, #7]
	ldrb r3, [r4, #2]
	ands r2, r1
	lsls r3, r3, #6
	orrs r2, r3
	strb r2, [r5, #7]
	adds r0, r5, #0
	movs r1, #240
	bl Func_020028d0
.L_0200a6f4:
	ldrh r3, [r5, #28]
	cmp r3, #0
	bne .L_0200a74a
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #8
	add r3, r8
	ldr r6, [r3]
	cmp r6, #0
	beq .L_0200a740
	bl Random16Far
	ldr r3, [r6]
	lsls r2, r0, #4
	ldr r1, .L_0200a764
	subs r2, r2, r0
	lsls r2, r2, #4
	adds r3, r3, r2
	adds r7, r3, r1
	bl Random16Far
	ldr r3, [r6, #8]
	lsls r2, r0, #2
	adds r2, r2, r0
	lsls r2, r2, #5
	adds r3, r3, r2
	ldr r2, .L_0200a768
	str r7, [r5, #12]
	adds r6, r3, r2
	str r6, [r5, #20]
	movs r0, #0
	adds r1, r7, #0
	adds r2, r6, #0
	bl Map_GetTerrainHeight
	movs r3, #16
	str r0, [r5, #16]
	b .L_0200a748
.L_0200a740:
	movs r3, #16
	str r6, [r5, #12]
	str r6, [r5, #20]
	str r6, [r5, #16]
.L_0200a748:
	strh r3, [r5, #28]
.L_0200a74a:
	movs r3, #1
	add r10, r3
	mov r1, r10
	adds r5, #32
	cmp r1, #63
	bhi .L_0200a758
	b .L_0200a5fe
.L_0200a758:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a764:
	.4byte 0xff880000
.L_0200a768:
	.4byte 0xffb00000
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
