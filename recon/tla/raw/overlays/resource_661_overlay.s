.syntax unified
	.thumb
	.section .text.x0200807e,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080b8,"ax",%progbits
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02008270
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02008100
	cmp r7, #0
	beq .L_02008100
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02008108
.L_02008100:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02008108:
	mov r3, r10
	bl Func_02003fc0
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02008116
	b .L_02008262
.L_02008116:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02003fa8
	ldr r2, .L_02008274
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02003fb8
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_02008278
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200827c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008262
	cmp r7, #0
	beq .L_02008262
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02008198
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02008198:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020081b8
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_020081b8:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_020081cc
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_020081cc:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02008212
	ldr r3, .L_02008274
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_020081fa
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200820c
.L_020081fa:
	ldr r2, .L_0200827c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200827c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200820c:
	bl __divsi3
	str r0, [r6, #52]
.L_02008212:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200822e
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003fa8
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02003fb8
.L_0200822e:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008240
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_02008240:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008252
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_02008252:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02008262
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02008262:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008270:
	.4byte gPartyState
.L_02008274:
	.4byte Data_020045c8
.L_02008278:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_0200827c:
	.4byte 0xffff0000
	.section .text.x020083c8,"ax",%progbits
	.global Func_020003c8
	.thumb_func
Func_020003c8:
	push {lr}
	ldr r3, .L_02008454
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008458
	cmp r2, r3
	bne .L_020083e0
	ldr r0, .L_0200845c
	b .L_02008450
.L_020083e0:
	ldr r3, .L_02008460
	cmp r2, r3
	bne .L_020083ea
	ldr r0, .L_02008464
	b .L_02008450
.L_020083ea:
	ldr r3, .L_02008468
	cmp r2, r3
	bne .L_020083f4
	ldr r0, .L_0200846c
	b .L_02008450
.L_020083f4:
	ldr r3, .L_02008470
	cmp r2, r3
	bne .L_020083fe
	ldr r0, .L_02008474
	b .L_02008450
.L_020083fe:
	ldr r3, .L_02008478
	cmp r2, r3
	bne .L_02008408
	ldr r0, .L_0200847c
	b .L_02008450
.L_02008408:
	ldr r3, .L_02008480
	cmp r2, r3
	bne .L_02008412
	ldr r0, .L_02008484
	b .L_02008450
.L_02008412:
	ldr r3, .L_02008488
	cmp r2, r3
	bne .L_0200841c
	ldr r0, .L_0200848c
	b .L_02008450
.L_0200841c:
	ldr r3, .L_02008490
	cmp r2, r3
	bne .L_02008426
	ldr r0, .L_02008494
	b .L_02008450
.L_02008426:
	ldr r3, .L_02008498
	cmp r2, r3
	bne .L_02008430
	ldr r0, .L_0200849c
	b .L_02008450
.L_02008430:
	ldr r3, .L_020084a0
	cmp r2, r3
	bne .L_0200843a
	ldr r0, .L_020084a4
	b .L_02008450
.L_0200843a:
	ldr r3, .L_020084a8
	cmp r2, r3
	bne .L_02008444
	ldr r0, .L_020084ac
	b .L_02008450
.L_02008444:
	ldr r3, .L_020084b0
	cmp r2, r3
	bne .L_0200844e
	ldr r0, .L_020084b4
	b .L_02008450
.L_0200844e:
	ldr r0, .L_020084b8
.L_02008450:
	pop {pc}
	.2byte 0x0000
.L_02008454:
	.4byte gPartyState
.L_02008458:
	.4byte 0x00000043
.L_0200845c:
	.4byte Data_02005198
.L_02008460:
	.4byte 0x00000044
.L_02008464:
	.4byte Data_0200521c
.L_02008468:
	.4byte 0x00000045
.L_0200846c:
	.4byte Data_020053fc
.L_02008470:
	.4byte 0x00000046
.L_02008474:
	.4byte Data_020054e0
.L_02008478:
	.4byte 0x00000047
.L_0200847c:
	.4byte Data_02005594
.L_02008480:
	.4byte 0x00000048
.L_02008484:
	.4byte Data_02005630
.L_02008488:
	.4byte 0x00000049
.L_0200848c:
	.4byte Data_02005678
.L_02008490:
	.4byte 0x0000004a
.L_02008494:
	.4byte Data_020056b4
.L_02008498:
	.4byte 0x0000004b
.L_0200849c:
	.4byte Data_02005750
.L_020084a0:
	.4byte 0x0000004c
.L_020084a4:
	.4byte Data_0200578c
.L_020084a8:
	.4byte 0x0000004d
.L_020084ac:
	.4byte Data_020057bc
.L_020084b0:
	.4byte 0x0000004e
.L_020084b4:
	.4byte Data_020057e0
.L_020084b8:
	.4byte Data_0200518c
	.section .text.x020086ac,"ax",%progbits
	.global Func_020006ac
	.thumb_func
Func_020006ac:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	str r3, [sp, #4]
	ldr r3, .L_020087bc
	movs r2, #240
	lsls r2, r2, #1
	movs r1, #0
	adds r3, r3, r2
	mov r11, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020087c0
	cmp r2, r3
	bne .L_020086e0
	ldr r2, .L_020087c4
	mov r11, r2
	b .L_020086f6
.L_020086e0:
	ldr r3, .L_020087c8
	cmp r2, r3
	bne .L_020086ec
	ldr r3, .L_020087cc
	mov r11, r3
	b .L_020086f6
.L_020086ec:
	ldr r3, .L_020087d0
	cmp r2, r3
	bne .L_020086f6
	ldr r1, .L_020087d4
	mov r11, r1
.L_020086f6:
	mov r2, r11
	cmp r2, #0
	beq .L_020087ac
	movs r1, #255
	ldrh r3, [r2]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_020087a2
.L_02008708:
	mov r2, r11
	ldrh r0, [r2]
	bl Object_GetById
	movs r3, #4
	add r11, r3
	mov r1, r11
	adds r7, r0, #0
	ldrh r0, [r1]
	bl GameFlag_Test
	mov r9, r0
	cmp r0, #0
	bne .L_02008790
	adds r3, r7, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r1, #156
	mov r8, r3
	mov r2, r8
	lsls r3, r3, #3
	subs r3, r3, r2
	ldr r2, [sp, #4]
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r6, [r2, r3]
	ldr r3, .L_020087d8
	ldr r1, .L_020087dc
	adds r5, r6, r3
	asrs r5, r5, #2
	adds r0, r7, #0
	adds r5, r5, r1
	bl Func_020041e0
	ldr r1, [r7, #8]
	mov r10, r0
	ldr r2, [r7, #16]
	mov r0, r8
	bl Func_02004020
	str r0, [sp, #0]
	ldr r1, [r7, #8]
	ldr r2, [r7, #16]
	mov r0, r8
	bl Map_GetTerrainHeight
	mov r1, r10
	lsls r3, r1, #2
	adds r6, r6, r3
	adds r3, r7, #0
	mov r1, r9
	adds r3, #85
	strb r1, [r3]
	strb r1, [r6, #2]
	ldrb r1, [r6, #3]
	movs r3, #128
	orrs r3, r1
	adds r2, r0, #0
	strb r3, [r6, #3]
	asrs r2, r2, #19
	adds r2, #4
	mov r0, r8
	ldr r1, [sp, #0]
	bl Func_020041e8
	add r5, r10
	strb r0, [r5]
.L_02008790:
	movs r2, #2
	add r11, r2
	mov r1, r11
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008708
.L_020087a2:
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_020087ac:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020087bc:
	.4byte gPartyState
.L_020087c0:
	.4byte 0x00000045
.L_020087c4:
	.4byte Data_0200478e
.L_020087c8:
	.4byte 0x00000046
.L_020087cc:
	.4byte Data_020047d2
.L_020087d0:
	.4byte 0x00000047
.L_020087d4:
	.4byte Data_020047ec
.L_020087d8:
	.4byte 0xfdff0000
.L_020087dc:
	.4byte gMapShapeGrid
	.section .text.x020087e0,"ax",%progbits
	.global Func_020007e0
	.thumb_func
Func_020007e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	sub sp, #8
	str r3, [sp, #4]
	ldr r3, .L_020088f4
	movs r2, #240
	lsls r2, r2, #1
	movs r1, #0
	adds r3, r3, r2
	mov r11, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020088f8
	cmp r2, r3
	bne .L_02008814
	ldr r2, .L_020088fc
	mov r11, r2
	b .L_0200882a
.L_02008814:
	ldr r3, .L_02008900
	cmp r2, r3
	bne .L_02008820
	ldr r3, .L_02008904
	mov r11, r3
	b .L_0200882a
.L_02008820:
	ldr r3, .L_02008908
	cmp r2, r3
	bne .L_0200882a
	ldr r1, .L_0200890c
	mov r11, r1
.L_0200882a:
	mov r2, r11
	cmp r2, #0
	beq .L_020088e4
	movs r1, #255
	ldrh r3, [r2]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	beq .L_020088da
.L_0200883c:
	mov r2, r11
	ldrh r0, [r2]
	bl Object_GetById
	movs r3, #4
	add r11, r3
	mov r1, r11
	adds r7, r0, #0
	ldrh r0, [r1]
	bl GameFlag_Test
	str r0, [sp, #0]
	cmp r0, #0
	bne .L_020088c8
	adds r3, r7, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r1, #156
	mov r8, r3
	mov r2, r8
	lsls r3, r3, #3
	subs r3, r3, r2
	ldr r2, [sp, #4]
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r6, [r2, r3]
	ldr r3, .L_02008910
	ldr r1, .L_02008914
	adds r5, r6, r3
	asrs r5, r5, #2
	adds r0, r7, #0
	adds r5, r5, r1
	bl Func_020041e0
	ldr r1, [r7, #8]
	mov r10, r0
	ldr r2, [r7, #16]
	mov r0, r8
	bl Func_02004020
	ldr r1, [r7, #8]
	mov r9, r0
	ldr r2, [r7, #16]
	mov r0, r8
	bl Map_GetTerrainHeight
	mov r1, r10
	lsls r3, r1, #2
	mov r1, sp
	ldrb r1, [r1]
	adds r6, r6, r3
	adds r3, r7, #0
	adds r3, #85
	strb r1, [r3]
	ldrb r1, [r6, #3]
	movs r3, #255
	adds r2, r0, #0
	strb r3, [r6, #2]
	movs r3, #127
	ands r3, r1
	asrs r2, r2, #19
	strb r3, [r6, #3]
	subs r2, #4
	mov r0, r8
	mov r1, r9
	bl Func_020041e8
	add r5, r10
	strb r0, [r5]
.L_020088c8:
	movs r2, #2
	add r11, r2
	mov r1, r11
	movs r2, #255
	ldrh r3, [r1]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200883c
.L_020088da:
	movs r0, #134
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_020088e4:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020088f4:
	.4byte gPartyState
.L_020088f8:
	.4byte 0x00000045
.L_020088fc:
	.4byte Data_0200478e
.L_02008900:
	.4byte 0x00000046
.L_02008904:
	.4byte Data_020047d2
.L_02008908:
	.4byte 0x00000047
.L_0200890c:
	.4byte Data_020047ec
.L_02008910:
	.4byte 0xfdff0000
.L_02008914:
	.4byte gMapShapeGrid
	.section .text.x02008918,"ax",%progbits
	.global Func_02000918
	.thumb_func
Func_02000918:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	ldr r7, [r3, #32]
	b .L_0200894e
.L_02008924:
	ldrh r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	ldrh r0, [r5, #4]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200894c
	adds r0, r6, #0
	bl Func_020041e0
	movs r2, #212
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r3, [r3]
	lsls r0, r0, #2
	adds r3, r3, r0
	movs r2, #255
	strb r2, [r3, #2]
.L_0200894c:
	adds r5, #6
.L_0200894e:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008924
	pop {r5, r6, r7, pc}
	.section .text.x0200895c,"ax",%progbits
	.global Func_0200095c
	.thumb_func
Func_0200095c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r5, r0, #0
	ldr r6, [r3, #32]
	b .L_02008984
.L_02008968:
	ldrh r0, [r5]
	bl Object_GetById
	bl Func_020041e0
	movs r2, #212
	lsls r2, r2, #1
	adds r3, r6, r2
	ldr r3, [r3]
	lsls r0, r0, #2
	adds r3, r3, r0
	movs r2, #232
	adds r5, #6
	strb r2, [r3, #2]
.L_02008984:
	movs r2, #255
	ldrh r3, [r5]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02008968
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008b04,"ax",%progbits
	.global Func_02000b04
	.thumb_func
Func_02000b04:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_02008b34
	adds r3, #15
.L_02008b34:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r3, .L_02008b80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	ldr r4, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r4, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r4, #9]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008b80:
	.4byte gPartyState
	.section .text.x02008b84,"ax",%progbits
	.global Func_02000b84
	.thumb_func
Func_02000b84:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008c28
	sub sp, #68
	add r7, sp, #28
	str r3, [r7, #36]
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	str r3, [r7, #8]
	str r3, [r7, #12]
	mov r10, r2
	movs r2, #0
	mov r11, r0
	mov r9, r1
	mov r8, r2
.L_02008bae:
	mov r3, r8
	lsls r6, r3, #14
	adds r0, r6, #0
	bl Math_Cosine
	add r5, sp, #16
	movs r3, #0
	str r0, [r5]
	adds r0, r6, #0
	str r3, [r5, #4]
	bl Math_Sine
	ldr r3, [r5]
	str r0, [r5, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r5]
	bl Random16Far
	ldr r3, [r5]
	ldr r2, .L_02008c2c
	lsls r0, r0, #17
	lsrs r0, r0, #16
	adds r3, r3, r0
	adds r3, r3, r2
	str r3, [r5]
	bl Random16Far
	ldr r2, [r5, #8]
	ldr r3, .L_02008c30
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r2, r2, r0
	adds r2, r2, r3
	str r2, [r5, #8]
	ldr r3, [r5]
	ldr r1, [r5, #4]
	str r2, [sp, #4]
	movs r2, #128
	lsls r2, r2, #17
	adds r2, #1
	str r1, [sp, #0]
	str r2, [sp, #8]
	mov r0, r11
	mov r2, r10
	mov r1, r9
	str r7, [sp, #12]
	bl Func_020000b8
	movs r2, #1
	add r8, r2
	mov r3, r8
	cmp r3, #16
	bls .L_02008bae
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008c28:
	.4byte Func_02000b04
.L_02008c2c:
	.4byte 0xffff0000
.L_02008c30:
	.4byte 0xffff8000
	.section .text.x02008c34,"ax",%progbits
	.global Func_02000c34
	.thumb_func
Func_02000c34:
	push {r5, lr}
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	bl Func_020021d4
	ldr r5, .L_02008d40
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #16
	bl Object_LinkObjectAndSetCallback
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02008d44
	adds r1, #204
	bl Func_02004118
	movs r0, #150
	movs r1, #1
	movs r2, #248
	movs r3, #1
	lsls r0, r0, #17
	negs r1, r1
	lsls r2, r2, #16
	bl Motion_CamBounds
	movs r0, #16
	movs r1, #254
	movs r2, #248
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #176
	lsls r1, r1, #1
	movs r2, #240
	movs r0, #16
	bl ObjectMotion_SetPositionAndCommit
	ldr r0, .L_02008d48
	bl Scheduler_RemoveCallbackFar
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_02008d4c
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r0, #17
	bl Func_02004090
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #176
	movs r2, #248
	lsls r2, r2, #16
	movs r1, #0
	lsls r0, r0, #17
	bl Func_02000b84
	movs r0, #176
	bl Func_020041f8
	movs r0, #16
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #16
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #16
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #190
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #16
	bl ObjectMotion_SetPositionAndCommit
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #241
	bl Func_020041f8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #16
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #248
	movs r0, #16
	bl ObjectMotion_SetPositionAndReset
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020041f8
	movs r1, #0
	movs r2, #0
	movs r0, #16
	bl Func_02004090
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_SetBit
	bl Func_02004048
	pop {r5, pc}
.L_02008d40:
	.4byte gPartyState
.L_02008d44:
	.4byte 0x00026666
.L_02008d48:
	.4byte Func_02002180
.L_02008d4c:
	.4byte Func_02001c5c
	.section .text.x02008d50,"ax",%progbits
	.global Func_02000d50
	.thumb_func
Func_02000d50:
	push {r5, lr}
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	bl Func_020021d4
	ldr r5, .L_02008e68
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	movs r1, #21
	bl Object_LinkObjectAndSetCallback
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02008e6c
	adds r1, #204
	bl Func_02004118
	movs r0, #246
	movs r1, #1
	movs r2, #154
	movs r3, #1
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #18
	bl Motion_CamBounds
	movs r2, #162
	movs r0, #21
	movs r1, #248
	lsls r2, r2, #2
	bl ObjectMotion_SetPositionAndCommit
	movs r2, #144
	movs r1, #248
	lsls r2, r2, #2
	movs r0, #21
	bl ObjectMotion_SetPositionAndCommit
	ldr r0, .L_02008e70
	bl Scheduler_RemoveCallbackFar
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_02008e74
	bl Scheduler_RemoveCallbackFar
	movs r0, #1
	bl WaitFrames
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl Func_02004090
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #248
	movs r2, #144
	lsls r2, r2, #18
	movs r1, #0
	lsls r0, r0, #16
	bl Func_02000b84
	movs r0, #176
	bl Func_020041f8
	movs r0, #21
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r0, #21
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #21
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #128
	movs r2, #140
	lsls r1, r1, #1
	lsls r2, r2, #2
	movs r0, #21
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r5]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #241
	bl Func_020041f8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #21
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r2, #248
	movs r1, #248
	lsls r2, r2, #1
	movs r0, #21
	bl ObjectMotion_SetPositionAndReset
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020041f8
	movs r1, #0
	movs r2, #0
	movs r0, #21
	bl Func_02004090
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_SetBit
	bl Func_02004048
	pop {r5, pc}
	.2byte 0x0000
.L_02008e68:
	.4byte gPartyState
.L_02008e6c:
	.4byte 0x00026666
.L_02008e70:
	.4byte Func_02002180
.L_02008e74:
	.4byte Func_02001c5c
	.section .text.x02008f24,"ax",%progbits
	.global Func_02000f24
	.thumb_func
Func_02000f24:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #0
	sub sp, #8
	mov r10, r3
.L_02008f32:
	ldr r7, .L_02008fc0
	movs r3, #0
	mov r8, r3
.L_02008f38:
	bl Random16Far
	ldrh r5, [r7]
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r5, r5, r0
	bl Random16Far
	adds r6, r7, #2
	ldrh r3, [r6]
	lsls r0, r0, #2
	lsrs r0, r0, #16
	adds r3, r3, r0
	subs r5, #2
	lsls r5, r5, #16
	subs r3, #2
	movs r0, #30
	adds r1, r5, #0
	lsls r3, r3, #16
	adds r0, #255
	movs r2, #0
	bl Func_02003fc0
	adds r5, r0, #0
	adds r7, #4
	cmp r5, #0
	beq .L_02008f90
	movs r3, #144
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #1
	bl Func_02003fa8
	adds r0, r5, #0
	ldr r1, .L_02008fc4
	bl Func_02003fb8
.L_02008f90:
	movs r3, #1
	add r8, r3
	mov r3, r8
	cmp r3, #14
	bls .L_02008f38
	movs r3, #1
	add r10, r3
	mov r3, r10
	cmp r3, #1
	bls .L_02008f32
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #74
	movs r1, #71
	movs r2, #77
	movs r3, #9
	bl Func_02003ff0
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008fc0:
	.4byte Data_020042b4
.L_02008fc4:
	.4byte Data_02005828
	.section .text.x02008fc8,"ax",%progbits
	.global Func_02000fc8
	.thumb_func
Func_02000fc8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009264
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #21
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #139
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008ff6
	b .L_0200925e
.L_02008ff6:
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
.L_02009000:
	ldr r5, .L_02009268
	ldr r1, .L_0200926c
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #24]
	adds r3, r3, r1
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	adds r3, r3, r1
	str r3, [r2, #28]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #204
	ldr r3, [r3, #24]
	lsls r2, r2, #8
	adds r2, #203
	cmp r3, r2
	bgt .L_02009000
.L_02009026:
	ldr r5, .L_02009268
	movs r1, #200
	ldr r2, [r5]
	lsls r1, r1, #5
	ldr r3, [r2, #24]
	adds r1, #153
	adds r3, r3, r1
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r2, #28]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #230
	ldr r3, [r3, #24]
	lsls r2, r2, #9
	adds r2, #204
	cmp r3, r2
	ble .L_02009026
.L_02009050:
	ldr r5, .L_02009268
	ldr r1, .L_02009270
	ldr r2, [r5]
	movs r0, #1
	ldr r3, [r2, #24]
	adds r3, r3, r1
	str r3, [r2, #24]
	ldr r3, [r2, #28]
	adds r3, r3, r1
	str r3, [r2, #28]
	bl WaitFrames
	ldr r3, [r5]
	movs r2, #204
	ldr r3, [r3, #24]
	lsls r2, r2, #6
	adds r2, #50
	cmp r3, r2
	bgt .L_02009050
	movs r1, #0
	movs r2, #0
	movs r0, #22
	bl Func_02004090
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	movs r1, #1
	movs r2, #196
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	ldr r5, .L_02009264
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	movs r2, #0
	bl Func_02004100
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	movs r1, #232
	movs r2, #164
	bl ObjectMotion_SetPositionAndReset
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #128
	lsls r2, r2, #7
	mov r8, r2
	movs r1, #232
	movs r2, #184
	mov r3, r8
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #21
	bl Func_02004098
	movs r0, #1
	bl WaitFrames
	movs r0, #232
	movs r2, #184
	lsls r2, r2, #16
	movs r1, #0
	lsls r0, r0, #16
	bl Func_02000b84
	movs r0, #176
	bl Func_020041f8
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r7, #40]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	movs r0, #21
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #21
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #21
	ldr r1, .L_02009274
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #21
	movs r1, #216
	movs r2, #184
	bl ObjectMotion_SetPositionAndCommit
	movs r2, #0
	ldr r0, [r5]
	mov r1, r8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #21
	bl Func_020040e8
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02002ca0
	movs r0, #136
	bl Func_020041f8
	bl Func_02000f24
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02004108
	ldr r0, [r5]
	movs r1, #27
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #204
	bl Func_020041f8
	movs r5, #0
.L_020091ae:
	ldr r3, [r6, #24]
	ldr r2, .L_02009278
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	adds r5, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, .L_0200927c
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r5, #13
	bls .L_020091ae
	ldr r3, .L_02009264
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
	movs r0, #153
	bl Func_020041f8
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #40]
	movs r0, #21
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r1, #232
	movs r2, #168
	movs r0, #21
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #3
	bl Battle_WaitMode0
	movs r0, #204
	bl Func_020041f8
	movs r5, #0
.L_02009214:
	ldr r3, [r7, #24]
	ldr r2, .L_02009280
	movs r0, #1
	adds r3, r3, r2
	str r3, [r7, #24]
	ldr r3, [r7, #28]
	adds r5, #1
	adds r3, r3, r2
	str r3, [r7, #28]
	ldr r2, .L_02009284
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	bl WaitFrames
	cmp r5, #5
	bls .L_02009214
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #4
	bl Func_02004138
.L_0200925e:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02009264:
	.4byte gPartyState
.L_02009268:
	.4byte Data_0200480c
.L_0200926c:
	.4byte 0xffffeb86
.L_02009270:
	.4byte 0xfffff5c3
.L_02009274:
	.4byte 0x00019999
.L_02009278:
	.4byte 0xfffffc00
.L_0200927c:
	.4byte 0xfffe0000
.L_02009280:
	.4byte 0xfffff800
.L_02009284:
	.4byte 0xfff90000
	.section .text.x020092dc,"ax",%progbits
	.balign 4
	.global Func_020012dc
	.thumb_func
Func_020012dc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_020093f4
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	ldr r0, [r2]
	sub sp, #8
	mov r8, r2
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #64
	bl Object_GetById
	ldr r3, [r5, #8]
	asrs r7, r3, #20
	ldr r3, [r5, #16]
	asrs r6, r3, #20
	cmp r0, #0
	beq .L_02009316
	movs r1, #154
	movs r2, #222
	movs r0, #64
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02004090
.L_02009316:
	movs r3, #6
	str r3, [sp, #4]
	movs r5, #5
	movs r0, #82
	movs r1, #82
	movs r2, #15
	movs r3, #40
	str r5, [sp, #0]
	bl Func_02003ff0
	movs r3, #15
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #82
	movs r2, #5
	movs r3, #6
	bl Func_02004000
	movs r0, #88
	movs r1, #82
	movs r2, #36
	movs r3, #25
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003ff0
	movs r3, #36
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #88
	movs r1, #82
	movs r2, #5
	movs r3, #5
	bl Func_02004000
	cmp r7, #15
	bne .L_0200937e
	cmp r6, #41
	bne .L_0200936e
	movs r0, #14
	b .L_020093ac
.L_0200936e:
	cmp r6, #43
	bne .L_020093ec
	movs r0, #16
	bl Func_020012d4
	adds r5, r0, #0
	movs r0, #43
	b .L_020093b4
.L_0200937e:
	cmp r7, #16
	bne .L_020093a2
	cmp r6, #40
	bne .L_02009392
	movs r0, #16
	bl Func_020012d4
	adds r5, r0, #0
	movs r0, #39
	b .L_020093b4
.L_02009392:
	cmp r6, #41
	bne .L_020093ec
	movs r0, #16
	bl Func_020012d4
	adds r5, r0, #0
	movs r0, #42
	b .L_020093b4
.L_020093a2:
	cmp r7, #18
	bne .L_020093ec
	cmp r6, #40
	bne .L_020093ca
	movs r0, #18
.L_020093ac:
	bl Func_020012d4
	adds r5, r0, #0
	movs r0, #41
.L_020093b4:
	bl Func_020012d4
	lsls r5, r5, #16
	adds r2, r0, #0
	mov r3, r8
	ldr r0, [r3]
	lsls r2, r2, #16
	adds r1, r5, #0
	bl Func_02004090
	b .L_020093ec
.L_020093ca:
	cmp r6, #43
	bne .L_020093ec
	movs r0, #17
	bl Func_020012d4
	adds r5, r0, #0
	movs r0, #43
	bl Func_020012d4
	lsls r5, r5, #16
	adds r2, r0, #0
	mov r3, r8
	ldr r0, [r3]
	lsls r2, r2, #16
	adds r1, r5, #0
	bl Func_02004090
.L_020093ec:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020093f4:
	.4byte gPartyState
	.section .text.x020093f8,"ax",%progbits
	.global Func_020013f8
	.thumb_func
Func_020013f8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009514
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	ldr r0, [r2]
	sub sp, #8
	mov r8, r2
	bl Object_GetById
	ldr r3, [r0, #8]
	asrs r7, r3, #20
	ldr r3, [r0, #16]
	movs r0, #64
	asrs r6, r3, #20
	bl Object_GetById
	cmp r0, #0
	beq .L_0200942c
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02004090
.L_0200942c:
	movs r3, #6
	str r3, [sp, #4]
	movs r5, #5
	movs r0, #82
	movs r1, #89
	movs r2, #15
	movs r3, #40
	str r5, [sp, #0]
	bl Func_02003ff0
	movs r3, #15
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #82
	movs r1, #89
	movs r2, #5
	movs r3, #6
	bl Func_02004000
	movs r0, #88
	movs r1, #89
	movs r2, #36
	movs r3, #25
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02003ff0
	movs r3, #36
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #88
	movs r1, #89
	movs r2, #5
	movs r3, #5
	bl Func_02004000
	cmp r7, #15
	bne .L_020094a0
	cmp r6, #44
	bne .L_020094a0
	movs r0, #15
	bl Func_020012d4
	adds r5, r0, #0
	movs r0, #43
	bl Func_020012d4
	lsls r5, r5, #16
	adds r2, r0, #0
	mov r3, r8
	ldr r0, [r3]
	lsls r2, r2, #16
	adds r1, r5, #0
	bl Func_02004090
	b .L_0200950a
.L_020094a0:
	cmp r7, #17
	bne .L_020094b4
	cmp r6, #44
	bne .L_020094b4
	movs r0, #17
	bl Func_020012d4
	adds r6, r0, #0
	movs r0, #43
	b .L_020094c6
.L_020094b4:
	cmp r7, #19
	bne .L_0200950a
	cmp r6, #40
	bne .L_020094e2
	movs r0, #19
	bl Func_020012d4
	adds r6, r0, #0
	movs r0, #41
.L_020094c6:
	bl Func_020012d4
	ldr r5, .L_02009514
	movs r3, #133
	lsls r3, r3, #2
	adds r2, r0, #0
	adds r5, r5, r3
	lsls r6, r6, #16
	ldr r0, [r5]
	lsls r2, r2, #16
	adds r1, r6, #0
	bl Func_02004090
	b .L_0200950a
.L_020094e2:
	cmp r6, #42
	bne .L_0200950a
	movs r0, #18
	bl Func_020012d4
	adds r6, r0, #0
	movs r0, #42
	bl Func_020012d4
	ldr r5, .L_02009514
	movs r3, #133
	lsls r3, r3, #2
	adds r2, r0, #0
	adds r5, r5, r3
	lsls r6, r6, #16
	ldr r0, [r5]
	lsls r2, r2, #16
	adds r1, r6, #0
	bl Func_02004090
.L_0200950a:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009514:
	.4byte gPartyState
	.section .text.x02009690,"ax",%progbits
	.balign 4
	.global Func_02001690
	.thumb_func
Func_02001690:
	push {r5, r6, lr}
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	ldr r0, .L_02009a98
	bl Func_020040c8
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #9
	bl Func_02004100
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #9
	bl Func_020040d0
	movs r0, #8
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #12
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r6, .L_02009a9c
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	movs r1, #128
	ldr r0, [r6]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02004100
	movs r1, #128
	movs r2, #0
	ldr r0, [r6]
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	bl Func_02004130
	movs r1, #204
	movs r3, #0
	adds r0, #85
	lsls r1, r1, #6
	strb r3, [r0]
	adds r1, #51
	ldr r0, .L_02009aa0
	bl Func_02004118
	movs r0, #212
	movs r2, #212
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #16
	movs r1, #0
	bl Motion_CamBounds
	bl Func_02004128
	movs r0, #12
	movs r1, #0
	bl Func_020040d0
	movs r2, #10
	movs r0, #11
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #11
	movs r1, #0
	bl Func_020040d0
	movs r1, #176
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	movs r2, #40
	movs r0, #14
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #14
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #9
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #11
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #12
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r0, #13
	movs r1, #14
	bl Object_LinkObjectAndSetCallback
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #13
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009aa4
	movs r0, #14
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02004118
	movs r0, #240
	movs r1, #1
	movs r2, #130
	lsls r2, r2, #17
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02004128
	movs r0, #8
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #9
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #10
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #11
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #12
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #13
	bl ObjectMotion_EnableActionAndResetMotion
	movs r1, #152
	lsls r1, r1, #7
	ldr r0, .L_02009aa8
	adds r1, #204
	bl Func_02004118
	movs r0, #212
	movs r1, #1
	movs r2, #212
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02004128
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #9
	movs r1, #0
	bl Func_020040d0
	movs r0, #8
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r0, #8
	movs r1, #0
	bl Func_020040d0
	movs r0, #10
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #0
	movs r0, #10
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r0, #10
	movs r1, #4
	bl Object_SetModeById
	movs r0, #10
	movs r1, #0
	bl Func_020040d0
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #9
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #8
	bl Func_02004100
	movs r1, #129
	movs r2, #80
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02004100
	movs r0, #14
	movs r1, #0
	bl Func_020040d0
	movs r1, #208
	movs r0, #8
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #10
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #20
	movs r0, #13
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r0, #10
	movs r1, #3
	bl Object_SetModeById
	movs r0, #11
	movs r1, #3
	bl Object_SetModeById
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #13
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_02009aa0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #9
	ldr r1, .L_02009aa0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #10
	ldr r1, .L_02009aa0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #11
	ldr r1, .L_02009aa0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #12
	ldr r1, .L_02009aa0
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #13
	ldr r1, .L_02009aa0
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r6]
	movs r1, #13
	bl Object_LinkObjectAndSetCallback
	ldr r5, .L_02009aac
	movs r0, #13
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #12
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02004118
	movs r0, #240
	movs r1, #1
	movs r2, #140
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #16
	negs r1, r1
	bl Motion_CamBounds
	adds r1, r5, #0
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #9
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r5, .L_02009ab0
	movs r0, #10
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #11
	bl Object_SetActionCallbackAndRefreshById
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r6, #176
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	lsls r6, r6, #8
	movs r1, #130
	movs r2, #242
	adds r3, r6, #0
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02004098
	movs r3, #128
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_02009ab4
	ldr r2, .L_02009ab8
	b .L_02009abc
	.2byte 0x0000
.L_02009a98:
	.4byte MsgHeyLookUp
.L_02009a9c:
	.4byte gPartyState
.L_02009aa0:
	.4byte 0x00019999
.L_02009aa4:
	.4byte Data_0200468c
.L_02009aa8:
	.4byte 0x00026666
.L_02009aac:
	.4byte Data_020046dc
.L_02009ab0:
	.4byte Data_02004718
.L_02009ab4:
	.4byte 0x02150000
.L_02009ab8:
	.4byte 0x010f0000
.L_02009abc:
	bl Func_02004098
	movs r3, #192
	movs r2, #232
	lsls r3, r3, #6
	movs r0, #10
	ldr r1, .L_02009b64
	lsls r2, r2, #16
	bl Func_02004098
	movs r3, #160
	movs r2, #248
	lsls r3, r3, #7
	movs r0, #11
	ldr r1, .L_02009b68
	lsls r2, r2, #16
	movs r5, #208
	bl Func_02004098
	lsls r5, r5, #8
	movs r1, #231
	movs r2, #170
	adds r3, r5, #0
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004098
	movs r1, #241
	movs r2, #170
	adds r3, r5, #0
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004098
	movs r2, #160
	lsls r2, r2, #17
	adds r3, r6, #0
	movs r0, #14
	ldr r1, .L_02009b6c
	bl Func_02004098
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
	movs r0, #10
	movs r1, #1
	bl Object_SetModeById
	movs r0, #11
	movs r1, #1
	bl Object_SetModeById
	movs r0, #12
	movs r1, #1
	bl Object_SetModeById
	movs r0, #13
	movs r1, #1
	bl Object_SetModeById
	movs r1, #1
	movs r0, #14
	bl Object_SetModeById
	movs r0, #143
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #242
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02004048
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009b64:
	.4byte 0x01b30000
.L_02009b68:
	.4byte 0x01d30000
.L_02009b6c:
	.4byte 0x01e90000
	.section .text.x02009b70,"ax",%progbits
	.global Func_02001b70
	.thumb_func
Func_02001b70:
	push {r5, r6, r7, lr}
	movs r0, #234
	movs r1, #232
	movs r2, #128
	movs r3, #200
	adds r0, #255
	lsls r1, r1, #16
	lsls r2, r2, #14
	lsls r3, r3, #16
	bl Func_02003fc0
	adds r6, r0, #0
	movs r7, #0
	movs r0, #0
	cmp r6, #0
	beq .L_02009be8
	ldr r5, [r6, #80]
	movs r3, #33
	ldrb r2, [r5, #5]
	negs r3, r3
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	movs r2, #13
	negs r2, r2
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r5, #9]
	adds r3, r6, #0
	adds r3, #85
	adds r2, r6, #0
	strb r7, [r3]
	adds r2, #92
	movs r3, #1
	movs r1, #193
	strb r3, [r2]
	lsls r1, r1, #3
	strb r7, [r5, #26]
	strb r7, [r5, #27]
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	adds r7, r0, #0
	movs r0, #209
	bl Func_02004030
	movs r3, #128
	lsls r3, r3, #3
	adds r2, r7, r3
	movs r1, #128
	ldrb r0, [r5, #16]
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
	adds r0, r6, #0
.L_02009be8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02009cc4,"ax",%progbits
	.global Func_02001cc4
	.thumb_func
Func_02001cc4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_02009e40
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	adds r7, r0, #0
	ldr r0, [r3]
	mov r8, r1
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r5, r5, r3
	ldrb r3, [r5]
	cmp r3, #5
	beq .L_02009dce
	ldr r3, .L_02009e44
	ldr r5, [r3]
	movs r3, #1
	ands r5, r3
	cmp r5, #0
	bne .L_02009dce
	movs r0, #30
	ldr r1, [r7, #8]
	ldr r2, [r7, #12]
	ldr r3, [r7, #16]
	adds r0, #255
	bl Func_02003fc0
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02009dce
	adds r3, r6, #0
	adds r3, #85
	strb r5, [r3]
	adds r3, #4
	strb r5, [r3]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r6, #0
	movs r1, #1
	bl Func_02003fa8
	ldr r1, .L_02009e48
	adds r0, r6, #0
	bl Func_02003fb8
	ldr r1, [r6, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	ldr r5, .L_02009e4c
	ldr r3, [r5]
	cmp r3, #0
	beq .L_02009d88
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Cosine
	add r0, r8
	str r0, [r6, #68]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Sine
	add r0, r10
	str r0, [r6, #76]
	movs r2, #2
	ldr r3, [r5]
	ands r3, r2
	cmp r3, #0
	beq .L_02009db0
	ldr r3, [r6, #68]
	mov r2, r10
	add r3, r8
	str r3, [r6, #68]
	adds r3, r0, r2
	str r3, [r6, #76]
	b .L_02009db0
.L_02009d88:
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Cosine
	asrs r0, r0, #1
	add r0, r8
	str r0, [r6, #68]
	bl Random16Far
	lsls r0, r0, #3
	lsrs r0, r0, #16
	lsls r0, r0, #14
	bl Math_Sine
	asrs r0, r0, #1
	add r0, r10
	str r0, [r6, #76]
.L_02009db0:
	movs r5, #0
	str r5, [r6, #72]
	bl Random16Far
	ldr r3, .L_02009e50
	lsls r0, r0, #9
	lsrs r0, r0, #16
	adds r0, r0, r3
	adds r3, r6, #0
	adds r3, #100
	strh r0, [r3]
	ldr r3, .L_02009e54
	str r5, [r6, #48]
	str r5, [r6, #52]
	str r3, [r6, #108]
.L_02009dce:
	mov r2, r8
	cmp r2, #0
	beq .L_02009e06
	ldr r3, [r7, #8]
	movs r2, #240
	add r3, r8
	str r3, [r7, #8]
	ldr r3, .L_02009e4c
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009e36
	ldr r1, [r7, #16]
	asrs r3, r1, #16
	adds r2, r3, #0
	cmp r3, #0
	bge .L_02009df2
	adds r2, #15
.L_02009df2:
	asrs r2, r2, #4
	lsls r2, r2, #4
	subs r2, r3, r2
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #3
	adds r3, r1, r3
	str r3, [r7, #16]
	b .L_02009e36
.L_02009e06:
	ldr r3, [r7, #16]
	movs r2, #240
	add r3, r10
	str r3, [r7, #16]
	ldr r3, .L_02009e4c
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02009e36
	ldr r1, [r7, #8]
	asrs r3, r1, #16
	adds r2, r3, #0
	cmp r3, #0
	bge .L_02009e24
	adds r2, #15
.L_02009e24:
	asrs r2, r2, #4
	lsls r2, r2, #4
	subs r2, r3, r2
	movs r3, #8
	subs r3, r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #3
	adds r3, r1, r3
	str r3, [r7, #8]
.L_02009e36:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009e40:
	.4byte gPartyState
.L_02009e44:
	.4byte gFrameCount
.L_02009e48:
	.4byte Data_020042f0
.L_02009e4c:
	.4byte gInput
.L_02009e50:
	.4byte 0xffffff00
.L_02009e54:
	.4byte Func_02000b04
	.section .text.x02009e58,"ax",%progbits
	.global Func_02001e58
	.thumb_func
Func_02001e58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200a000
	movs r2, #133
	mov r9, r1
	movs r0, #192
	lsls r2, r2, #2
	lsls r0, r0, #18
	add r2, r9
	ldr r5, [r0, #108]
	mov r8, r0
	ldr r0, [r2]
	mov r11, r2
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	movs r7, #192
	lsls r7, r7, #9
	cmp r3, #0
	bne .L_02009eb0
	movs r2, #173
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	bne .L_02009eb0
	movs r1, #175
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02009ebe
.L_02009eb0:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	bl Map_EnableUpdateCallback
	b .L_02009ff4
.L_02009ebe:
	mov r3, r8
	adds r0, r6, #0
	ldr r5, [r3, #32]
	bl Func_020041e0
	ldr r3, [r6, #8]
	cmp r3, #0
	bge .L_02009ed2
	ldr r1, .L_0200a004
	adds r3, r3, r1
.L_02009ed2:
	ldr r2, [r6, #16]
	movs r1, #128
	lsls r1, r1, #11
	asrs r4, r3, #20
	adds r3, r2, r1
	cmp r3, #0
	bge .L_02009ee4
	ldr r1, .L_0200a008
	adds r3, r2, r1
.L_02009ee4:
	movs r1, #156
	lsls r1, r1, #1
	adds r2, r5, r1
	asrs r3, r3, #20
	ldr r2, [r2]
	lsls r3, r3, #7
	adds r3, r4, r3
	lsls r1, r0, #2
	lsls r3, r3, #2
	adds r0, r2, r1
	adds r2, r2, r3
	mov r8, r2
	movs r2, #184
	lsls r2, r2, #1
	adds r3, r5, r2
	ldr r3, [r3]
	mov r10, r0
	adds r5, r3, r1
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	bl Map_DisableUpdateCallback
	ldrb r3, [r5, #2]
	movs r0, #128
	adds r3, #246
	lsls r3, r3, #24
	lsls r0, r0, #19
	cmp r3, r0
	bhi .L_02009f42
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r9
	ldrb r3, [r3]
	cmp r3, #5
	bne .L_02009f34
	movs r7, #0
	b .L_02009f42
.L_02009f34:
	mov r1, r11
	ldr r0, [r1]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
.L_02009f42:
	mov r2, r8
	ldrb r3, [r2, #2]
	cmp r3, #255
	bne .L_02009f4c
	movs r7, #0
.L_02009f4c:
	mov r3, r10
	ldrb r2, [r3, #2]
	cmp r2, #11
	beq .L_02009f7a
	cmp r2, #11
	bgt .L_02009f5e
	cmp r2, #10
	beq .L_02009f68
	b .L_02009fac
.L_02009f5e:
	cmp r2, #12
	beq .L_02009f8a
	cmp r2, #13
	beq .L_02009f9c
	b .L_02009fac
.L_02009f68:
	ldr r3, .L_0200a00c
	movs r2, #64
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009fac
	negs r2, r7
	asrs r2, r2, #2
	b .L_02009fc8
.L_02009f7a:
	ldr r3, .L_0200a00c
	movs r2, #128
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009fac
	asrs r2, r7, #2
	b .L_02009fc8
.L_02009f8a:
	ldr r3, .L_0200a00c
	movs r2, #32
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009fac
	negs r1, r7
	asrs r1, r1, #2
	b .L_02009fe0
.L_02009f9c:
	ldr r3, .L_0200a00c
	movs r2, #16
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_02009fac
	asrs r1, r7, #2
	b .L_02009fe0
.L_02009fac:
	ldrb r0, [r5, #2]
	cmp r0, #11
	beq .L_02009fd2
	cmp r0, #11
	bgt .L_02009fbc
	cmp r0, #10
	beq .L_02009fc6
	b .L_02009ff4
.L_02009fbc:
	cmp r0, #12
	beq .L_02009fde
	cmp r0, #13
	beq .L_02009fea
	b .L_02009ff4
.L_02009fc6:
	negs r2, r7
.L_02009fc8:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001cc4
	b .L_02009ff4
.L_02009fd2:
	adds r0, r6, #0
	movs r1, #0
	adds r2, r7, #0
	bl Func_02001cc4
	b .L_02009ff4
.L_02009fde:
	negs r1, r7
.L_02009fe0:
	adds r0, r6, #0
	movs r2, #0
	bl Func_02001cc4
	b .L_02009ff4
.L_02009fea:
	adds r0, r6, #0
	adds r1, r7, #0
	movs r2, #0
	bl Func_02001cc4
.L_02009ff4:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a000:
	.4byte gPartyState
.L_0200a004:
	.4byte 0x000fffff
.L_0200a008:
	.4byte 0x0013ffff
.L_0200a00c:
	.4byte gInput
	.section .text.x0200a208,"ax",%progbits
	.global Func_02002208
	.thumb_func
Func_02002208:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #98
	ldrb r3, [r6]
	cmp r3, #9
	bhi .L_0200a30c
	ldr r2, .L_0200a310
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0200a220:
	.4byte .L_0200a248
	.4byte .L_0200a25c
	.4byte .L_0200a26a
	.4byte .L_0200a27e
	.4byte .L_0200a28c
	.4byte .L_0200a2a0
	.4byte .L_0200a2ae
	.4byte .L_0200a2c2
	.4byte .L_0200a2ea
	.4byte .L_0200a2fe
.L_0200a248:
	movs r1, #140
	movs r3, #200
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #1
	b .L_0200a30a
.L_0200a25c:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a30c
	movs r3, #2
	b .L_0200a30a
.L_0200a26a:
	movs r1, #140
	movs r3, #132
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #3
	b .L_0200a30a
.L_0200a27e:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a30c
	movs r3, #4
	b .L_0200a30a
.L_0200a28c:
	movs r1, #184
	movs r3, #132
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #5
	b .L_0200a30a
.L_0200a2a0:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a30c
	movs r3, #6
	b .L_0200a30a
.L_0200a2ae:
	movs r1, #184
	movs r3, #248
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #7
	b .L_0200a30a
.L_0200a2c2:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a30c
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2e6
	ldr r2, .L_0200a314
	movs r3, #0
	str r3, [r5, #108]
	movs r3, #200
	str r3, [r2]
	b .L_0200a30c
.L_0200a2e6:
	movs r3, #8
	b .L_0200a30a
.L_0200a2ea:
	movs r1, #184
	movs r3, #200
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #9
	b .L_0200a30a
.L_0200a2fe:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a30c
	movs r3, #0
.L_0200a30a:
	strb r3, [r6]
.L_0200a30c:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a310:
	.4byte .L_0200a220
.L_0200a314:
	.4byte Data_02004808
	.section .text.x0200a318,"ax",%progbits
	.global Func_02002318
	.thumb_func
Func_02002318:
	push {r5, r6, lr}
	movs r0, #16
	bl Object_GetById
	movs r1, #144
	lsls r1, r1, #3
	adds r6, r0, #0
	ldr r0, .L_0200a3b8
	bl Scheduler_AddOrUpdateCallback
	movs r0, #17
	ldr r5, .L_0200a3bc
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	str r0, [r5]
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r0, #17
	bl Func_020040f8
	ldr r3, [r5]
	movs r5, #3
	adds r3, #85
	strb r5, [r3]
	movs r0, #17
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #16
	ldr r1, .L_0200a3c0
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #16
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	bl Func_020040f8
	movs r0, #16
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a3a6
	adds r3, r6, #0
	adds r3, #98
	strb r0, [r3]
	ldr r3, .L_0200a3c4
	str r3, [r6, #108]
.L_0200a3a6:
	bl Func_020021d4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a3c8
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a3b8:
	.4byte Func_02002180
.L_0200a3bc:
	.4byte Data_0200480c
.L_0200a3c0:
	.4byte 0x00019999
.L_0200a3c4:
	.4byte Func_02002208
.L_0200a3c8:
	.4byte Func_02001c5c
	.section .text.x0200a3cc,"ax",%progbits
	.global Func_020023cc
	.thumb_func
Func_020023cc:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #98
	ldrb r3, [r6]
	cmp r3, #39
	bls .L_0200a3dc
	b .L_0200a696
.L_0200a3dc:
	ldr r2, .L_0200a698
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200a3e4:
	.4byte .L_0200a484
	.4byte .L_0200a498
	.4byte .L_0200a4a8
	.4byte .L_0200a4bc
	.4byte .L_0200a4cc
	.4byte .L_0200a4e0
	.4byte .L_0200a508
	.4byte .L_0200a51c
	.4byte .L_0200a52c
	.4byte .L_0200a540
	.4byte .L_0200a550
	.4byte .L_0200a564
	.4byte .L_0200a574
	.4byte .L_0200a588
	.4byte .L_0200a598
	.4byte .L_0200a5ac
	.4byte .L_0200a5ba
	.4byte .L_0200a5ce
	.4byte .L_0200a5e8
	.4byte .L_0200a5fc
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a696
	.4byte .L_0200a60a
	.4byte .L_0200a622
	.4byte .L_0200a630
	.4byte .L_0200a644
	.4byte .L_0200a652
	.4byte .L_0200a666
	.4byte .L_0200a674
	.4byte .L_0200a688
.L_0200a484:
	movs r1, #156
	movs r3, #166
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #1
	b .L_0200a694
.L_0200a498:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a4a4
	b .L_0200a696
.L_0200a4a4:
	movs r3, #2
	b .L_0200a694
.L_0200a4a8:
	movs r1, #156
	movs r3, #182
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #3
	b .L_0200a694
.L_0200a4bc:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a4c8
	b .L_0200a696
.L_0200a4c8:
	movs r3, #4
	b .L_0200a694
.L_0200a4cc:
	movs r1, #246
	movs r3, #182
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #5
	b .L_0200a694
.L_0200a4e0:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a4ec
	b .L_0200a696
.L_0200a4ec:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a504
	ldr r2, .L_0200a69c
	movs r3, #0
	str r3, [r5, #108]
	movs r3, #201
	str r3, [r2]
	b .L_0200a696
.L_0200a504:
	movs r3, #6
	b .L_0200a694
.L_0200a508:
	movs r1, #184
	movs r3, #182
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #7
	b .L_0200a694
.L_0200a51c:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a528
	b .L_0200a696
.L_0200a528:
	movs r3, #8
	b .L_0200a694
.L_0200a52c:
	movs r1, #184
	movs r3, #178
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #9
	b .L_0200a694
.L_0200a540:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a54c
	b .L_0200a696
.L_0200a54c:
	movs r3, #10
	b .L_0200a694
.L_0200a550:
	movs r1, #140
	movs r3, #178
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #11
	b .L_0200a694
.L_0200a564:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a570
	b .L_0200a696
.L_0200a570:
	movs r3, #12
	b .L_0200a694
.L_0200a574:
	movs r1, #140
	movs r3, #190
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #13
	b .L_0200a694
.L_0200a588:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a594
	b .L_0200a696
.L_0200a594:
	movs r3, #14
	b .L_0200a694
.L_0200a598:
	movs r1, #232
	movs r3, #190
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #15
	b .L_0200a694
.L_0200a5ac:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a696
	movs r3, #16
	b .L_0200a694
.L_0200a5ba:
	movs r1, #232
	movs r3, #186
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #17
	b .L_0200a694
.L_0200a5ce:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a696
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a692
	movs r3, #18
	b .L_0200a694
.L_0200a5e8:
	movs r1, #232
	movs r3, #166
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #19
	b .L_0200a694
.L_0200a5fc:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a696
	movs r3, #0
	b .L_0200a694
.L_0200a60a:
	bl Func_020021d4
	movs r1, #148
	movs r3, #186
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #33
	b .L_0200a694
.L_0200a622:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a696
	movs r3, #34
	b .L_0200a694
.L_0200a630:
	movs r1, #148
	movs r3, #190
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #35
	b .L_0200a694
.L_0200a644:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a696
	movs r3, #36
	b .L_0200a694
.L_0200a652:
	movs r1, #232
	movs r3, #190
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #37
	b .L_0200a694
.L_0200a666:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a696
	movs r3, #38
	b .L_0200a694
.L_0200a674:
	movs r1, #232
	movs r3, #186
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #39
	b .L_0200a694
.L_0200a688:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200a696
.L_0200a692:
	movs r3, #32
.L_0200a694:
	strb r3, [r6]
.L_0200a696:
	pop {r5, r6, pc}
.L_0200a698:
	.4byte .L_0200a3e4
.L_0200a69c:
	.4byte Data_02004808
	.section .text.x0200a6a0,"ax",%progbits
	.global Func_020026a0
	.thumb_func
Func_020026a0:
	push {r5, r6, lr}
	movs r0, #21
	bl Object_GetById
	movs r1, #144
	lsls r1, r1, #3
	adds r6, r0, #0
	ldr r0, .L_0200a750
	bl Scheduler_AddOrUpdateCallback
	movs r0, #22
	ldr r5, .L_0200a754
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	str r0, [r5]
	str r3, [r0, #24]
	str r3, [r0, #28]
	movs r0, #22
	bl Func_020040f8
	ldr r3, [r5]
	movs r5, #3
	adds r3, #85
	strb r5, [r3]
	movs r0, #22
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #21
	ldr r1, .L_0200a758
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #21
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	bl Func_020040f8
	movs r0, #21
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200a740
	movs r1, #232
	movs r2, #166
	movs r3, #0
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02004098
	adds r3, r6, #0
	adds r3, #98
	strb r5, [r3]
	ldr r3, .L_0200a75c
	str r3, [r6, #108]
.L_0200a740:
	bl Func_020021d4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a760
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
.L_0200a750:
	.4byte Func_02002180
.L_0200a754:
	.4byte Data_0200480c
.L_0200a758:
	.4byte 0x00019999
.L_0200a75c:
	.4byte Func_020023cc
.L_0200a760:
	.4byte Func_02001c5c
	.section .text.x0200a764,"ax",%progbits
	.global Func_02002764
	.thumb_func
Func_02002764:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #98
	ldrb r3, [r6]
	cmp r3, #55
	bls .L_0200a774
	b .L_0200ab80
.L_0200a774:
	ldr r2, .L_0200aaec
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0200a77c:
	.4byte .L_0200a85c
	.4byte .L_0200a870
	.4byte .L_0200a880
	.4byte .L_0200a894
	.4byte .L_0200a8a4
	.4byte .L_0200a8b8
	.4byte .L_0200a8da
	.4byte .L_0200a8ee
	.4byte .L_0200a8fe
	.4byte .L_0200a912
	.4byte .L_0200a922
	.4byte .L_0200a936
	.4byte .L_0200a956
	.4byte .L_0200a96a
	.4byte .L_0200a97a
	.4byte .L_0200a98e
	.4byte .L_0200a9b8
	.4byte .L_0200a9cc
	.4byte .L_0200a9dc
	.4byte .L_0200a9f0
	.4byte .L_0200aa00
	.4byte .L_0200aa14
	.4byte .L_0200aa36
	.4byte .L_0200aa4a
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200aa5a
	.4byte .L_0200aa72
	.4byte .L_0200aa82
	.4byte .L_0200aa9a
	.4byte .L_0200aaa8
	.4byte .L_0200aabc
	.4byte .L_0200aaca
	.4byte .L_0200aade
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200ab80
	.4byte .L_0200aaf4
	.4byte .L_0200ab0c
	.4byte .L_0200ab1a
	.4byte .L_0200ab2e
	.4byte .L_0200ab3c
	.4byte .L_0200ab50
	.4byte .L_0200ab5e
	.4byte .L_0200ab72
.L_0200a85c:
	movs r1, #156
	movs r3, #132
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #1
	b .L_0200ab7e
.L_0200a870:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a87c
	b .L_0200ab80
.L_0200a87c:
	movs r3, #2
	b .L_0200ab7e
.L_0200a880:
	movs r1, #156
	movs r3, #172
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #3
	b .L_0200ab7e
.L_0200a894:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a8a0
	b .L_0200ab80
.L_0200a8a0:
	movs r3, #4
	b .L_0200ab7e
.L_0200a8a4:
	movs r1, #148
	movs r3, #172
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #5
	b .L_0200ab7e
.L_0200a8b8:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a8c4
	b .L_0200ab80
.L_0200a8c4:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a8d6
	movs r3, #32
	b .L_0200ab7e
.L_0200a8d6:
	movs r3, #6
	b .L_0200ab7e
.L_0200a8da:
	movs r1, #168
	movs r3, #172
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #7
	b .L_0200ab7e
.L_0200a8ee:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a8fa
	b .L_0200ab80
.L_0200a8fa:
	movs r3, #8
	b .L_0200ab7e
.L_0200a8fe:
	movs r1, #168
	movs r3, #148
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #9
	b .L_0200ab7e
.L_0200a912:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a91e
	b .L_0200ab80
.L_0200a91e:
	movs r3, #10
	b .L_0200ab7e
.L_0200a922:
	movs r1, #184
	movs r3, #148
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #11
	b .L_0200ab7e
.L_0200a936:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a942
	b .L_0200ab80
.L_0200a942:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a952
	b .L_0200ab7c
.L_0200a952:
	movs r3, #12
	b .L_0200ab7e
.L_0200a956:
	movs r1, #140
	movs r3, #148
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #13
	b .L_0200ab7e
.L_0200a96a:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a976
	b .L_0200ab80
.L_0200a976:
	movs r3, #14
	b .L_0200ab7e
.L_0200a97a:
	movs r1, #140
	movs r3, #164
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #15
	b .L_0200ab7e
.L_0200a98e:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a99a
	b .L_0200ab80
.L_0200a99a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a9b4
	ldr r2, .L_0200aaf0
	movs r3, #0
	str r3, [r5, #108]
	movs r3, #202
	str r3, [r2]
	b .L_0200ab80
.L_0200a9b4:
	movs r3, #16
	b .L_0200ab7e
.L_0200a9b8:
	movs r1, #140
	movs r3, #180
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #17
	b .L_0200ab7e
.L_0200a9cc:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a9d8
	b .L_0200ab80
.L_0200a9d8:
	movs r3, #18
	b .L_0200ab7e
.L_0200a9dc:
	movs r1, #200
	movs r3, #180
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #19
	b .L_0200ab7e
.L_0200a9f0:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200a9fc
	b .L_0200ab80
.L_0200a9fc:
	movs r3, #20
	b .L_0200ab7e
.L_0200aa00:
	movs r1, #200
	movs r3, #156
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #21
	b .L_0200ab7e
.L_0200aa14:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200aa20
	b .L_0200ab80
.L_0200aa20:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aa32
	movs r3, #34
	b .L_0200ab7e
.L_0200aa32:
	movs r3, #22
	b .L_0200ab7e
.L_0200aa36:
	movs r1, #200
	movs r3, #132
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #23
	b .L_0200ab7e
.L_0200aa4a:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200aa56
	b .L_0200ab80
.L_0200aa56:
	movs r3, #0
	b .L_0200ab7e
.L_0200aa5a:
	bl Func_020021d4
	movs r1, #148
	movs r3, #156
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #33
	b .L_0200ab7e
.L_0200aa72:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	bne .L_0200aa7e
	b .L_0200ab80
.L_0200aa7e:
	movs r3, #34
	b .L_0200ab7e
.L_0200aa82:
	bl Func_020021d4
	movs r1, #164
	movs r3, #156
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #35
	b .L_0200ab7e
.L_0200aa9a:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200ab80
	movs r3, #36
	b .L_0200ab7e
.L_0200aaa8:
	movs r1, #164
	movs r3, #188
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #37
	b .L_0200ab7e
.L_0200aabc:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200ab80
	movs r3, #38
	b .L_0200ab7e
.L_0200aaca:
	movs r1, #148
	movs r3, #188
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	bl Func_02003fe0
	movs r3, #39
	b .L_0200ab7e
.L_0200aade:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200ab80
	movs r3, #32
	b .L_0200ab7e
.L_0200aaec:
	.4byte .L_0200a77c
.L_0200aaf0:
	.4byte Data_02004808
.L_0200aaf4:
	bl Func_020021d4
	movs r1, #184
	movs r3, #172
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #49
	b .L_0200ab7e
.L_0200ab0c:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200ab80
	movs r3, #50
	b .L_0200ab7e
.L_0200ab1a:
	movs r1, #168
	movs r3, #172
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #51
	b .L_0200ab7e
.L_0200ab2e:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200ab80
	movs r3, #52
	b .L_0200ab7e
.L_0200ab3c:
	movs r1, #168
	movs r3, #148
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #53
	b .L_0200ab7e
.L_0200ab50:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200ab80
	movs r3, #54
	b .L_0200ab7e
.L_0200ab5e:
	movs r1, #184
	movs r3, #148
	lsls r3, r3, #17
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	bl Func_02003fe0
	movs r3, #55
	b .L_0200ab7e
.L_0200ab72:
	adds r0, r5, #0
	bl Func_020021f0
	cmp r0, #0
	beq .L_0200ab80
.L_0200ab7c:
	movs r3, #48
.L_0200ab7e:
	strb r3, [r6]
.L_0200ab80:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200ab84,"ax",%progbits
	.global Func_02002b84
	.thumb_func
Func_02002b84:
	push {r5, r6, lr}
	movs r0, #21
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #22
	ldr r5, .L_0200ac58
	bl Object_GetById
	adds r1, r0, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	movs r3, #166
	lsls r3, r3, #9
	adds r3, #204
	str r3, [r0, #24]
	str r3, [r0, #28]
	str r0, [r5]
	movs r0, #22
	bl Func_020040f8
	movs r0, #22
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	ldr r3, [r5]
	movs r2, #204
	adds r3, #85
	movs r5, #3
	lsls r2, r2, #8
	strb r5, [r3]
	adds r2, #204
	movs r0, #21
	ldr r1, .L_0200ac5c
	bl ObjectMotion_SetSpeedParameters
	adds r0, r6, #0
	movs r1, #15
	bl Object_SetPartAttribute
	movs r1, #3
	movs r0, #21
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	bl Func_020040f8
	movs r0, #21
	bl Object_GetById
	adds r0, #85
	strb r5, [r0]
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #139
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ac14
	movs r3, #128
	movs r1, #232
	movs r2, #168
	lsls r3, r3, #7
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02004098
	b .L_0200ac46
.L_0200ac14:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ac60
	bl Scheduler_AddOrUpdateCallback
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0200ac46
	movs r1, #200
	movs r2, #132
	movs r3, #0
	movs r0, #21
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004098
	adds r3, r6, #0
	adds r3, #98
	strb r5, [r3]
	ldr r3, .L_0200ac64
	str r3, [r6, #108]
.L_0200ac46:
	bl Func_020021d4
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ac68
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ac58:
	.4byte Data_0200480c
.L_0200ac5c:
	.4byte 0x00019999
.L_0200ac60:
	.4byte Func_02002180
.L_0200ac64:
	.4byte Func_02002764
.L_0200ac68:
	.4byte Func_02001c5c
	.section .text.x0200ac6c,"ax",%progbits
	.global Func_02002c6c
	.thumb_func
Func_02002c6c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #196
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	sub sp, #8
	cmp r3, #0
	beq .L_0200ac9a
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #73
	movs r1, #76
	movs r2, #78
	movs r3, #11
	bl Func_02003ff0
.L_0200ac9a:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x0200ad6c,"ax",%progbits
	.global Func_02002d6c
	.thumb_func
Func_02002d6c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	mov r8, r1
	mov r10, r2
	mov r9, r3
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	ldr r5, .L_0200ae0c
	strb r3, [r2]
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	ldr r0, [r5]
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	mov r2, r10
	ldr r0, [r5]
	mov r1, r8
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	bl Func_020040e8
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r0, [r5]
	movs r1, #13
	bl Object_SetModeById
	ldr r2, [r6, #12]
	ldr r3, .L_0200ae10
	ldr r1, [r6, #8]
	adds r2, r2, r3
	adds r0, r6, #0
	ldr r3, [r6, #16]
	bl Func_02003fe0
	adds r0, r6, #0
	bl Func_02003fe8
	movs r1, #10
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #123
	bl Func_020041f8
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	mov r0, r9
	bl Func_02004138
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200ae0c:
	.4byte gPartyState
.L_0200ae10:
	.4byte 0xfff60000
	.section .text.x0200aebc,"ax",%progbits
	.global Func_02002ebc
	.thumb_func
Func_02002ebc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200b084
	movs r5, #0
	ldr r0, [r1]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #8
	bl Object_GetById
	mov r8, r0
	bl Func_02004040
	movs r0, #0
	bl Func_020041b8
	movs r0, #78
	bl Func_020041f8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r0, r0
	negs r1, r1
	negs r2, r2
	movs r3, #0
	bl Motion_CamBounds
	movs r0, #232
	movs r1, #1
	movs r2, #184
	movs r3, #0
	negs r1, r1
	lsls r2, r2, #16
	lsls r0, r0, #16
	bl Motion_CamBounds
	bl Func_02003fd8
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_0200b084
	adds r7, r6, #0
	ldr r0, [r2]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #130
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #8
	adds r7, #85
	strb r5, [r7]
	movs r0, #8
	str r3, [r6, #12]
	str r1, [r6, #72]
	str r5, [r6, #68]
	mov r9, r3
	mov r10, r1
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #128
	movs r1, #232
	movs r2, #148
	lsls r3, r3, #7
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02004098
	movs r2, #85
	add r2, r8
	strb r5, [r2]
	mov r1, r8
	mov r3, r9
	mov r11, r2
	mov r2, r10
	movs r0, #1
	str r3, [r1, #12]
	str r2, [r1, #72]
	str r5, [r1, #68]
	bl WaitFrames
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #130
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_StartScreenTransition
	movs r5, #3
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_020041f8
	ldr r2, .L_0200b084
	strb r5, [r7]
	ldr r0, [r2]
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #24
	bl Battle_WaitMode0
	ldr r2, [r6, #16]
	ldr r1, [r6, #12]
	ldr r0, [r6, #8]
	bl Func_02000b84
	movs r0, #188
	bl Func_020041f8
	ldr r3, .L_0200b084
	movs r1, #2
	ldr r0, [r3]
	adds r1, #255
	bl Func_02004108
	ldr r1, .L_0200b084
	ldr r0, [r1]
	movs r1, #49
	bl Motion_SetModeAndWaitAnimation
	movs r0, #204
	bl Func_020041f8
	mov r2, r11
	strb r5, [r2]
	movs r0, #8
	bl Object_GetById
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #24
	bl Battle_WaitMode0
	mov r3, r8
	ldr r0, [r3, #8]
	ldr r1, [r3, #12]
	ldr r2, [r3, #16]
	bl Func_02000b84
	movs r0, #188
	bl Func_020041f8
	ldr r1, .L_0200b084
	movs r2, #0
	ldr r0, [r1]
	movs r1, #192
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #147
	bl Func_020041f8
	movs r1, #3
	movs r0, #8
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #128
	mov r2, r8
	lsls r3, r3, #12
	str r3, [r2, #40]
	movs r3, #230
	lsls r3, r3, #8
	adds r3, #102
	str r3, [r2, #72]
	movs r2, #204
	lsls r2, r2, #8
	movs r0, #8
	ldr r1, .L_0200b088
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #232
	movs r2, #184
	bl ObjectMotion_SetPositionAndCommit
	ldr r3, .L_0200b08c
	movs r1, #166
	lsls r1, r1, #1
	adds r1, #255
	adds r2, r3, r1
	movs r3, #2
	strb r3, [r2]
	ldr r0, .L_0200b090
	movs r1, #2
	bl Party_SetFields1eeAnd1f0
	movs r0, #10
	movs r1, #0
	bl Func_02004140
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b084:
	.4byte Data_02000454
.L_0200b088:
	.4byte 0x00019999
.L_0200b08c:
	.4byte gPartyState
.L_0200b090:
	.4byte 0x0000004c
	.section .text.x0200b094,"ax",%progbits
	.global Func_02003094
	.thumb_func
Func_02003094:
	push {lr}
	bl Func_020021d4
	ldr r2, .L_0200b134
	movs r3, #0
	str r3, [r2]
	ldr r3, .L_0200b138
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_0200b13c
	cmp r2, r3
	bne .L_0200b0b8
	bl Func_0200316c
	b .L_0200b12e
.L_0200b0b8:
	ldr r3, .L_0200b140
	cmp r2, r3
	bne .L_0200b0c4
	bl Func_020031b4
	b .L_0200b12e
.L_0200b0c4:
	ldr r3, .L_0200b144
	cmp r2, r3
	bne .L_0200b0d0
	bl Func_020032f4
	b .L_0200b12e
.L_0200b0d0:
	ldr r3, .L_0200b148
	cmp r2, r3
	bne .L_0200b0dc
	bl Func_02003364
	b .L_0200b12e
.L_0200b0dc:
	ldr r3, .L_0200b14c
	cmp r2, r3
	bne .L_0200b0e8
	bl Func_02003404
	b .L_0200b12e
.L_0200b0e8:
	ldr r3, .L_0200b150
	cmp r2, r3
	bne .L_0200b0f4
	bl Func_020035b8
	b .L_0200b12e
.L_0200b0f4:
	ldr r3, .L_0200b154
	cmp r2, r3
	bne .L_0200b100
	bl Func_020035d8
	b .L_0200b12e
.L_0200b100:
	ldr r3, .L_0200b158
	cmp r2, r3
	bne .L_0200b10c
	bl Func_020036d0
	b .L_0200b12e
.L_0200b10c:
	ldr r3, .L_0200b15c
	cmp r2, r3
	bne .L_0200b118
	bl Func_0200371c
	b .L_0200b12e
.L_0200b118:
	ldr r3, .L_0200b160
	cmp r2, r3
	bne .L_0200b124
	bl Func_020037d4
	b .L_0200b12e
.L_0200b124:
	ldr r3, .L_0200b164
	cmp r2, r3
	bne .L_0200b12e
	bl Func_020037e8
.L_0200b12e:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_0200b134:
	.4byte Data_0200480c
.L_0200b138:
	.4byte gPartyState
.L_0200b13c:
	.4byte 0x00000043
.L_0200b140:
	.4byte 0x00000044
.L_0200b144:
	.4byte 0x00000045
.L_0200b148:
	.4byte 0x00000046
.L_0200b14c:
	.4byte 0x00000047
.L_0200b150:
	.4byte 0x00000048
.L_0200b154:
	.4byte 0x0000004a
.L_0200b158:
	.4byte 0x0000004b
.L_0200b15c:
	.4byte 0x0000004c
.L_0200b160:
	.4byte 0x0000004d
.L_0200b164:
	.4byte 0x0000004e
	.section .text.x0200b1b4,"ax",%progbits
	.global Func_020031b4
	.thumb_func
Func_020031b4:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #179
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b20c
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02004090
	b .L_0200b28c
.L_0200b20c:
	movs r0, #143
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b28c
	movs r6, #176
	lsls r6, r6, #8
	movs r1, #130
	movs r2, #242
	movs r0, #8
	lsls r1, r1, #18
	lsls r2, r2, #16
	adds r3, r6, #0
	bl Func_02004098
	movs r3, #128
	lsls r3, r3, #8
	movs r0, #9
	ldr r1, .L_0200b2ac
	ldr r2, .L_0200b2b0
	bl Func_02004098
	movs r3, #192
	movs r2, #232
	lsls r3, r3, #6
	movs r0, #10
	ldr r1, .L_0200b2b4
	lsls r2, r2, #16
	bl Func_02004098
	movs r3, #160
	movs r2, #248
	lsls r3, r3, #7
	movs r0, #11
	ldr r1, .L_0200b2b8
	lsls r2, r2, #16
	movs r5, #208
	bl Func_02004098
	lsls r5, r5, #8
	movs r1, #231
	movs r2, #170
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #17
	adds r3, r5, #0
	bl Func_02004098
	movs r1, #241
	movs r2, #170
	movs r0, #13
	lsls r1, r1, #17
	lsls r2, r2, #17
	adds r3, r5, #0
	bl Func_02004098
	movs r2, #160
	movs r0, #14
	ldr r1, .L_0200b2bc
	lsls r2, r2, #17
	adds r3, r6, #0
	bl Func_02004098
.L_0200b28c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #141
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b29e
	bl Func_02001594
.L_0200b29e:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b2c0
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200b2ac:
	.4byte 0x02150000
.L_0200b2b0:
	.4byte 0x010f0000
.L_0200b2b4:
	.4byte 0x01b30000
.L_0200b2b8:
	.4byte 0x01d30000
.L_0200b2bc:
	.4byte 0x01e90000
.L_0200b2c0:
	.4byte Func_02002010
	.section .text.x0200b404,"ax",%progbits
	.global Func_02003404
	.thumb_func
Func_02003404:
	push {lr}
	movs r0, #0
	sub sp, #8
	bl Func_02004180
	ldr r0, .L_0200b5b0
	bl Func_02003820
	movs r0, #11
	bl Func_02003338
	movs r0, #12
	bl Func_02003338
	movs r0, #13
	bl Func_02003338
	movs r0, #14
	bl Func_02003338
	movs r0, #15
	bl Func_02003338
	movs r0, #16
	bl Func_02003338
	movs r0, #17
	bl Func_02003338
	movs r0, #18
	bl Func_02003338
	movs r0, #19
	bl Func_02003338
	movs r0, #20
	bl Func_02003338
	ldr r3, .L_0200b5b4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_0200b48c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #137
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b476
	b .L_0200b5aa
.L_0200b476:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b486
	b .L_0200b5aa
.L_0200b486:
	bl Func_020026a0
	b .L_0200b5aa
.L_0200b48c:
	movs r1, #200
	movs r2, #248
	movs r0, #11
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02004090
	movs r1, #168
	movs r2, #140
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #152
	movs r2, #172
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #184
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #164
	movs r2, #132
	movs r0, #15
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #148
	movs r2, #148
	movs r0, #16
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #172
	movs r2, #156
	movs r0, #17
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #156
	movs r2, #180
	movs r0, #18
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #140
	movs r2, #188
	movs r0, #19
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004090
	movs r1, #164
	movs r2, #196
	movs r0, #20
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02004090
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #140
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b598
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #237
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b55c
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #68
	movs r1, #77
	movs r2, #13
	movs r3, #9
	bl Func_02003ff0
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #68
	movs r1, #77
	movs r2, #3
	movs r3, #3
	bl Func_02004000
	b .L_0200b582
.L_0200b55c:
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #74
	movs r1, #71
	movs r2, #77
	movs r3, #9
	bl Func_02003ff0
	movs r3, #13
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #74
	movs r1, #71
	movs r2, #3
	movs r3, #3
	bl Func_02004000
.L_0200b582:
	movs r3, #13
	movs r2, #73
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #63
	movs r1, #77
	movs r2, #3
	movs r3, #3
	bl Func_02004000
	b .L_0200b5aa
.L_0200b598:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #138
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b5aa
	bl Func_02002b84
.L_0200b5aa:
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_0200b5b0:
	.4byte Data_020047ec
.L_0200b5b4:
	.4byte gPartyState
	.section .text.x0200b5d8,"ax",%progbits
	.global Func_020035d8
	.thumb_func
Func_020035d8:
	.ifdef TLA_EDITION_JA
	push {r5, lr}
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #137
	bl Func_020041c8
	bl Func_020041d0
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #8
	movs r3, #9
	bl Func_020041d8
	cmp r5, #0
	beq .L_ja_0200b608
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02004090
.L_ja_0200b608:
	movs r0, #10
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #8
	adds r5, r0, #0
	adds r3, #51
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #8
	strh r3, [r5, #32]
	bl Func_02002d40
	movs r1, #144
	ldr r0, .L_ja_0200b65c
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_ja_0200b660
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_ja_0200b65a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_ClearBit
.L_ja_0200b65a:
	pop {r5, pc}
.L_ja_0200b65c:
	.4byte Func_02001e58
.L_ja_0200b660:
	.4byte gPartyState
	.else
	push {r5, lr}
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #137
	bl Func_020041c8
	bl Func_020041d0
	movs r1, #131
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #8
	movs r3, #9
	bl Func_020041d8
	cmp r5, #0
	beq .L_0200b608
	movs r0, #64
	movs r1, #0
	movs r2, #0
	bl Func_02004090
.L_0200b608:
	movs r0, #10
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #8
	adds r5, r0, #0
	adds r3, #51
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r3, #8
	strh r3, [r5, #32]
	bl Func_02002d40
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200b6bc
	bl Scheduler_AddOrUpdateCallback
	ldr r5, .L_0200b6c0
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r5, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_0200b65c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_ClearBit
	b .L_0200b6ba
.L_0200b65c:
	cmp r3, #4
	bne .L_0200b6ba
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #41
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b684
	ldr r2, .L_0200b6c4
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r5, r1
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r5, r3
	movs r3, #1
	b .L_0200b6b8
.L_0200b684:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b696
	ldr r2, .L_0200b6c8
	b .L_0200b6a6
.L_0200b696:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #43
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b6ba
	ldr r2, .L_0200b6cc
.L_0200b6a6:
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r5, r1
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r5, r3
	movs r3, #2
.L_0200b6b8:
	strh r3, [r2]
.L_0200b6ba:
	pop {r5, pc}
.L_0200b6bc:
	.4byte Func_02001e58
.L_0200b6c0:
	.4byte gPartyState
.L_0200b6c4:
	.4byte 0x00000043
.L_0200b6c8:
	.4byte 0x0000004a
.L_0200b6cc:
	.4byte 0x0000004b
	.endif
	.section .text.x0200b71c,"ax",%progbits
	.global Func_0200371c
	.thumb_func
Func_0200371c:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #162
	adds r2, #93
	str r2, [r3]
	lsls r0, r0, #1
	sub sp, #8
	bl GameFlag_SetBit
	ldr r3, .L_0200b7d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_0200b75c
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #140
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b75c
	bl Func_02002ebc
	b .L_0200b77e
.L_0200b75c:
	ldr r3, .L_0200b7d0
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	bne .L_0200b77e
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #140
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200b77e
	bl Func_02001bec
.L_0200b77e:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #237
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200b7ac
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #39
	movs r2, #13
	movs r3, #11
	bl Func_02003ff0
	movs r1, #232
	movs r2, #200
	movs r0, #9
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02004090
.L_0200b7ac:
	movs r0, #0
	movs r1, #240
	bl Func_02003da0
	movs r3, #64
	str r3, [sp, #0]
	movs r1, #9
	movs r2, #0
	movs r3, #0
	movs r0, #0
	bl Func_02003e3c
	movs r0, #10
	bl WaitFrames
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_0200b7d0:
	.4byte gPartyState
	.section .text.x0200b820,"ax",%progbits
	.global Func_02003820
	.thumb_func
Func_02003820:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200b884
	adds r7, r0, #0
.L_0200b836:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl ObjectDispatch_SetSingleChildFlags
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_02003910
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200b836
.L_0200b884:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200b88c,"ax",%progbits
	.global Func_0200388c
	.thumb_func
Func_0200388c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_0200b8f8
.L_0200b8a8:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200b8f4
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200b8d0
	ldr r3, [r6, #28]
	ldr r1, .L_0200b90c
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200b8d0:
	mov r2, r9
	cmp r2, #1
	bne .L_0200b902
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02003910
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200b902
.L_0200b8f4:
	adds r5, #6
	movs r1, #255
.L_0200b8f8:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200b8a8
.L_0200b902:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200b90c:
	.4byte 0xffffe100
	.section .text.x0200b9b8,"ax",%progbits
	.global Func_020039b8
	.thumb_func
Func_020039b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200bb6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	mov r9, r0
	movs r0, #158
	mov r4, r9
	lsls r0, r0, #1
	movs r3, #2
	ldrsh r2, [r4, r3]
	adds r3, r1, r0
	ldr r3, [r3]
	sub sp, #4
	str r3, [sp, #0]
	movs r4, #156
	lsls r4, r4, #1
	adds r3, r1, r4
	ldr r1, .L_0200bb70
	ldr r3, [r3]
	lsls r2, r2, #2
	mov r5, r9
	movs r0, #0
	adds r1, r1, r2
	adds r5, #4
	mov r11, r3
	mov r10, r0
	mov r8, r1
.L_0200b9fc:
	movs r2, #0
	ldrsh r0, [r5, r2]
	cmp r0, #0
	bne .L_0200ba06
	b .L_0200bb4c
.L_0200ba06:
	bl Object_GetById
	adds r7, r0, #0
	ldr r3, [r7, #8]
	cmp r3, #0
	bne .L_0200ba1a
	ldr r3, [r7, #16]
	cmp r3, #0
	bne .L_0200ba1a
	b .L_0200bb4c
.L_0200ba1a:
	movs r3, #2
	ldrsh r2, [r5, r3]
	movs r4, #8
	ldrsh r3, [r5, r4]
	cmp r2, r3
	bne .L_0200ba30
	movs r0, #206
	bl Func_020041f8
	movs r3, #4
	strh r3, [r5, #18]
.L_0200ba30:
	movs r0, #2
	ldrsh r2, [r5, r0]
	movs r1, #10
	ldrsh r3, [r5, r1]
	cmp r2, r3
	bne .L_0200ba4c
	movs r0, #140
	adds r0, #255
	bl Func_020041f8
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	strh r3, [r5, #18]
.L_0200ba4c:
	movs r4, #18
	ldrsh r3, [r5, r4]
	ldrh r2, [r5, #18]
	cmp r3, #0
	beq .L_0200ba86
	ldrh r3, [r5, #4]
	movs r0, #0
	adds r3, r3, r2
	movs r4, #12
	ldrsh r2, [r5, r4]
	strh r3, [r5, #4]
	lsls r3, r3, #16
	asrs r3, r3, #16
	ldrh r1, [r5, #12]
	cmp r3, r2
	blt .L_0200ba72
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200ba72:
	movs r1, #4
	ldrsh r2, [r5, r1]
	movs r4, #14
	ldrsh r3, [r5, r4]
	ldrh r1, [r5, #14]
	cmp r2, r3
	bgt .L_0200ba86
	strh r1, [r5, #4]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
.L_0200ba86:
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200baa6
	ldrh r3, [r5, #2]
	movs r1, #6
	ldrsh r2, [r5, r1]
	adds r3, #1
	strh r3, [r5, #2]
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, r2
	blt .L_0200baa6
	strh r0, [r5, #2]
.L_0200baa6:
	ldr r3, [r7, #16]
	ldr r2, [r7, #8]
	asrs r3, r3, #20
	asrs r2, r2, #20
	lsls r3, r3, #7
	adds r1, r2, r3
	mov r2, r9
	ldrb r3, [r2, #2]
	ldr r4, [sp, #0]
	add r3, r10
	strb r3, [r4, r1]
	movs r0, #14
	ldrsh r2, [r7, r0]
	movs r4, #4
	ldrsh r3, [r5, r4]
	adds r2, r2, r3
	cmp r2, #0
	bge .L_0200bacc
	adds r2, #7
.L_0200bacc:
	asrs r3, r2, #3
	lsls r3, r3, #8
	mov r0, r8
	str r3, [r0]
	mov r2, r11
	lsls r3, r1, #2
	adds r1, r2, r3
	movs r4, #18
	ldrsh r3, [r5, r4]
	cmp r3, #0
	beq .L_0200bae6
	movs r3, #0
	b .L_0200baec
.L_0200bae6:
	ldrb r2, [r1, #3]
	movs r3, #128
	orrs r3, r2
.L_0200baec:
	strb r3, [r1, #3]
	movs r0, #4
	ldrsh r3, [r5, r0]
	cmp r3, #0
	beq .L_0200bafc
	ldrb r2, [r1, #3]
	movs r3, #16
	orrs r3, r2
.L_0200bafc:
	strb r3, [r1, #3]
	ldr r6, [r5, #24]
	cmp r6, #0
	beq .L_0200bb4c
	movs r1, #4
	ldrsh r3, [r5, r1]
	cmp r3, #0
	bne .L_0200bb14
	ldr r3, [r7, #12]
	ldr r2, .L_0200bb74
	adds r3, r3, r2
	b .L_0200bb42
.L_0200bb14:
	ldrh r0, [r5, #22]
	movs r3, #128
	lsls r3, r3, #5
	adds r0, r0, r3
	strh r0, [r5, #22]
	lsls r0, r0, #16
	asrs r0, r0, #16
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	ldr r3, .L_0200bb78
	lsls r0, r0, #10
	mov lr, r3
	.2byte 0xf800
	movs r1, #4
	ldrsh r2, [r5, r1]
	ldr r3, [r7, #12]
	ldr r4, .L_0200bb7c
	lsls r2, r2, #16
	adds r0, r0, r4
	adds r3, r3, r2
	adds r3, r3, r0
.L_0200bb42:
	str r3, [r6, #12]
	ldr r3, [r7, #8]
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
.L_0200bb4c:
	movs r3, #1
	add r10, r3
	movs r2, #4
	mov r4, r10
	add r8, r2
	adds r5, #28
	cmp r4, #15
	bgt .L_0200bb5e
	b .L_0200b9fc
.L_0200bb5e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bb6c:
	.4byte Data_020023c4 + 0x188
.L_0200bb70:
	.4byte gMapCollision
.L_0200bb74:
	.4byte 0xfff00000
.L_0200bb78:
	.4byte IwramMulQ16
.L_0200bb7c:
	.4byte 0xfff20000
	.section .text.x0200bb80,"ax",%progbits
	.global Func_02003b80
	.thumb_func
Func_02003b80:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_0200bd84
	sub sp, #36
	adds r0, r2, #4
	str r0, [sp, #32]
	ldr r1, .L_0200bd88
	movs r0, #0
	ldrsh r3, [r2, r0]
	lsls r3, r3, #2
	adds r3, r3, r1
	ldrh r3, [r3, #2]
	movs r1, #226
	lsrs r3, r3, #5
	str r3, [sp, #24]
	lsls r1, r1, #1
	adds r2, r2, r1
	ldr r2, [r2]
	movs r3, #192
	mov r9, r2
	movs r2, #0
	str r2, [sp, #16]
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	str r3, [sp, #12]
	ldr r0, [sp, #12]
	ldr r3, .L_0200bd8c
	ands r0, r3
	str r0, [sp, #12]
	ldr r2, [r2, #4]
	ands r2, r3
	str r2, [sp, #8]
	ldr r3, [r1]
	movs r1, #15
	ldr r3, [r3, #4]
	str r1, [sp, #28]
	str r3, [sp, #4]
.L_0200bbda:
	ldr r3, [sp, #32]
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	bne .L_0200bbe6
	b .L_0200bd64
.L_0200bbe6:
	movs r1, #4
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200bbf0
	b .L_0200bd64
.L_0200bbf0:
	bl Object_GetById
	ldr r3, .L_0200bd90
	movs r1, #12
	mov r10, r0
	ldr r0, [r3]
	bl __umodsi3
	movs r1, #3
	bl __udivsi3
	lsls r0, r0, #3
	adds r0, #32
	str r0, [sp, #20]
	ldr r1, [sp, #32]
	movs r2, #0
	movs r0, #4
	ldrsh r3, [r1, r0]
	mov r11, r2
	cmp r11, r3
	bge .L_0200bcb4
.L_0200bc1a:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_0200bca6
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	subs r7, r3, r1
	mov r3, r11
	lsls r2, r3, #16
	ldr r3, [r0, #12]
	ldr r0, [sp, #4]
	adds r3, r3, r2
	mov r1, r10
	subs r0, r3, r0
	ldr r2, [sp, #8]
	ldr r3, [r1, #16]
	mov r8, r0
	ldr r0, [sp, #4]
	subs r3, r3, r2
	subs r5, r3, r0
	mov r1, r8
	subs r6, r5, r1
	asrs r3, r6, #16
	adds r6, r3, #0
	adds r3, r1, r5
	asrs r3, r3, #16
	asrs r2, r7, #16
	adds r1, r3, #0
	movs r3, #167
	adds r7, r2, #0
	lsls r3, r3, #1
	adds r2, #7
	subs r7, #8
	subs r6, #16
	adds r1, #58
	cmp r2, r3
	bhi .L_0200bca6
	movs r0, #16
	negs r0, r0
	cmp r6, r0
	ble .L_0200bca6
	cmp r6, #239
	bgt .L_0200bca6
	adds r3, #177
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_0200bd94
	orrs r6, r3
	stmia r4!, {r6}
	ldr r2, [sp, #24]
	ldr r0, [sp, #20]
	adds r3, r2, r0
	movs r2, #128
	lsls r2, r2, #4
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_02003f88
	ldr r2, [sp, #16]
	movs r1, #12
	adds r2, #1
	str r2, [sp, #16]
	add r9, r1
.L_0200bca6:
	ldr r1, [sp, #32]
	movs r3, #16
	add r11, r3
	movs r0, #4
	ldrsh r3, [r1, r0]
	cmp r11, r3
	blt .L_0200bc1a
.L_0200bcb4:
	ldr r2, [sp, #16]
	cmp r2, #79
	bgt .L_0200bd64
	mov r0, r10
	ldr r3, [r0, #8]
	ldr r1, [sp, #12]
	ldr r0, [sp, #32]
	subs r7, r3, r1
	movs r3, #4
	ldrsh r2, [r0, r3]
	mov r1, r10
	ldr r3, [r1, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [sp, #4]
	ldr r0, [sp, #8]
	subs r2, r3, r2
	ldr r3, [r1, #16]
	ldr r1, [sp, #4]
	subs r3, r3, r0
	subs r5, r3, r1
	ldr r3, [sp, #32]
	subs r6, r5, r2
	mov r8, r2
	movs r2, #22
	ldrsh r0, [r3, r2]
	bl Math_Sine
	ldr r3, .L_0200bd98
	adds r1, r0, #0
	ldr r0, .L_0200bd9c
	mov lr, r3
	.2byte 0xf800
	ldr r1, .L_0200bd9c
	adds r0, r6, r0
	mov r2, r8
	asrs r7, r7, #16
	adds r0, r0, r1
	adds r3, r2, r5
	mov r10, r7
	asrs r0, r0, #16
	asrs r3, r3, #16
	adds r1, r3, #0
	subs r6, r0, #4
	mov r3, r10
	movs r0, #167
	adds r3, #7
	lsls r0, r0, #1
	subs r7, #8
	adds r1, #58
	cmp r3, r0
	bhi .L_0200bd64
	movs r2, #16
	negs r2, r2
	cmp r6, r2
	ble .L_0200bd64
	cmp r6, #239
	bgt .L_0200bd64
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	mov r4, r9
	ands r6, r3
	movs r3, #0
	stmia r4!, {r3}
	lsls r3, r7, #16
	orrs r6, r3
	ldr r3, .L_0200bd94
	orrs r6, r3
	stmia r4!, {r6}
	ldr r0, [sp, #24]
	ldr r2, [sp, #20]
	adds r3, r0, r2
	movs r2, #128
	lsls r2, r2, #4
	subs r3, #32
	orrs r3, r2
	mov r0, r9
	str r3, [r4]
	bl Func_02003f88
	ldr r0, [sp, #16]
	movs r3, #12
	adds r0, #1
	str r0, [sp, #16]
	add r9, r3
.L_0200bd64:
	ldr r1, [sp, #28]
	ldr r2, [sp, #32]
	subs r1, #1
	adds r2, #28
	str r1, [sp, #28]
	str r2, [sp, #32]
	cmp r1, #0
	blt .L_0200bd76
	b .L_0200bbda
.L_0200bd76:
	add sp, #36
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bd84:
	.4byte Data_020023c4 + 0x188
.L_0200bd88:
	.4byte ResourceTableEntries
.L_0200bd8c:
	.4byte 0xffff0000
.L_0200bd90:
	.4byte gFrameCount
.L_0200bd94:
	.4byte 0x40002000
.L_0200bd98:
	.4byte IwramMulQ16
.L_0200bd9c:
	.4byte 0xfffe0000
	.section .text.x0200be3c,"ax",%progbits
	.balign 4
	.global Func_02003e3c
	.thumb_func
Func_02003e3c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r9, r2
	mov r10, r3
	ldr r2, .L_0200bed0
	lsls r3, r0, #3
	subs r3, r3, r0
	mov r8, r1
	lsls r3, r3, #2
	adds r3, r3, r2
	mov r0, r8
	ldr r7, [sp, #28]
	adds r5, r3, #4
	bl Object_GetById
	adds r6, r0, #0
	mov r0, r8
	bl Object_GetById
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200bed4
	cmp r7, r10
	bge .L_0200be86
	mov r12, r10
	mov r10, r7
	mov r7, r12
.L_0200be86:
	mov r3, r8
	strh r3, [r5]
	mov r3, r9
	strh r3, [r5, #2]
	movs r3, #180
	lsls r3, r3, #1
	strh r3, [r5, #6]
	movs r3, #60
	strh r3, [r5, #8]
	movs r3, #240
	strh r3, [r5, #10]
	mov r3, r10
	ldr r2, .L_0200becc
	strh r3, [r5, #14]
	movs r3, #1
	strh r3, [r5, #16]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	adds r2, r6, #0
	adds r2, #89
	movs r3, #8
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #2
	orrs r3, r2
	strh r0, [r5, #4]
	strh r7, [r5, #12]
	strh r0, [r5, #18]
	strh r0, [r5, #22]
	strh r0, [r5, #20]
	strb r3, [r1]
	b .L_0200bed4
.L_0200becc:
	.4byte 0x00000000
.L_0200bed0:
	.4byte Data_020023c4 + 0x188
.L_0200bed4:
	movs r0, #128
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	ldr r3, [r6, #16]
	lsls r0, r0, #8
	bl Func_02003fc0
	adds r1, r0, #0
	adds r2, r1, #0
	movs r3, #0
	adds r2, #89
	strb r3, [r2]
	subs r2, #4
	strb r3, [r2]
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #128
	orrs r3, r2
	strb r3, [r0]
	str r1, [r5, #24]
	adds r0, r5, #0
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
