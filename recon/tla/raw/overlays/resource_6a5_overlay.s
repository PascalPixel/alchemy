.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #152
	bl Func_020059f4
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008050,"ax",%progbits
	.global Func_02000050
	.thumb_func
Func_02000050:
	push {lr}
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #0
	pop {pc}
	.section .text.x02008090,"ax",%progbits
	.global Func_02000090
	.thumb_func
Func_02000090:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	adds r5, r0, #0
	movs r0, #31
	bl Object_GetById
	adds r6, r5, #0
	adds r6, #100
	ldrh r1, [r6]
	mov r10, r0
	mov r8, r1
	mov r0, r8
	bl Math_Cosine
	ldr r3, [r5, #48]
	mov r1, r10
	adds r3, #3
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #8]
	mov r0, r8
	adds r3, r3, r2
	str r3, [r5, #8]
	bl Math_Sine
	mov r2, r10
	ldr r3, [r2, #16]
	ldr r2, [r5, #8]
	lsls r0, r0, #1
	adds r3, r3, r0
	str r3, [r5, #16]
	str r2, [r5, #56]
	str r3, [r5, #64]
	ldr r1, .L_020080e8
	ldrh r3, [r6]
	adds r3, r3, r1
	strh r3, [r6]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020080e8:
	.4byte 0xfffff800
	.section .text.x020080ec,"ax",%progbits
	.global Func_020000ec
	.thumb_func
Func_020000ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	movs r0, #36
	bl Object_GetById
	adds r5, r7, #0
	adds r5, #100
	ldrh r6, [r5]
	mov r8, r0
	adds r0, r6, #0
	bl Math_Cosine
	movs r1, #98
	adds r1, r1, r7
	ldrb r2, [r1]
	ldr r3, [r7, #48]
	mov r10, r1
	adds r3, r3, r2
	mov r1, r8
	adds r3, #6
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #8]
	adds r0, r6, #0
	adds r3, r3, r2
	str r3, [r7, #8]
	bl Math_Sine
	mov r2, r10
	ldrb r3, [r2]
	mov r1, r8
	adds r3, #4
	adds r2, r3, #0
	muls r2, r0
	ldr r3, [r1, #16]
	adds r3, r3, r2
	ldr r2, [r7, #8]
	str r3, [r7, #16]
	str r2, [r7, #56]
	str r3, [r7, #64]
	ldr r2, .L_02008154
	ldrh r3, [r5]
	adds r3, r3, r2
	strh r3, [r5]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008154:
	.4byte 0xfffff800
	.section .text.x02008198,"ax",%progbits
	.global Func_02000198
	.thumb_func
Func_02000198:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #36
	bl Object_GetById
	ldr r5, .L_02008364
	adds r6, r0, #0
	ldr r3, [r5]
	movs r7, #0
	cmp r3, #48
	bls .L_020081b4
	b .L_02008390
.L_020081b4:
	ldr r2, .L_02008368
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_020081bc:
	.4byte .L_02008280
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_0200829a
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_020082ac
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_020082be
	.4byte .L_020082e6
	.4byte .L_020082fe
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008346
	.4byte .L_02008390
	.4byte .L_0200834a
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008390
	.4byte .L_02008380
.L_02008280:
	movs r0, #220
	bl Func_020059f4
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020057ac
	ldr r0, .L_0200836c
	b .L_0200829e
.L_0200829a:
	movs r0, #128
	lsls r0, r0, #9
.L_0200829e:
	movs r1, #1
	bl Func_02005954
	movs r0, #8
	bl Func_02005964
	b .L_02008390
.L_020082ac:
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #9
	bl Func_020057ac
	b .L_02008390
.L_020082be:
	movs r3, #152
	lsls r3, r3, #17
	str r3, [r6, #8]
	ldr r3, .L_02008370
	adds r0, r6, #0
	str r3, [r6, #12]
	movs r3, #164
	lsls r3, r3, #16
	str r3, [r6, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #24]
	str r3, [r6, #28]
	bl SceneActor_ParkRecord
	ldr r1, .L_02008374
	movs r0, #36
	bl ObjectMotion_EnableActionAndSetCallback
	b .L_02008390
.L_020082e6:
	ldr r3, [r5]
	subs r3, #1
	str r3, [r5]
	ldr r3, [r6, #12]
	cmp r3, #0
	ble .L_02008326
	ldr r0, .L_02008378
	movs r1, #0
	bl Func_02005954
	movs r0, #16
	b .L_0200831a
.L_020082fe:
	ldr r3, [r5]
	movs r2, #160
	subs r3, #1
	str r3, [r5]
	lsls r2, r2, #14
	ldr r3, [r6, #12]
	cmp r3, r2
	ble .L_02008326
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02005954
	movs r0, #40
.L_0200831a:
	bl Func_02005964
	ldr r3, [r5]
	adds r3, #1
	str r3, [r5]
	b .L_02008390
.L_02008326:
	ldr r3, .L_0200837c
	movs r2, #7
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	bne .L_02008338
	movs r0, #246
	bl Func_020059f4
.L_02008338:
	ldr r3, [r6, #12]
	movs r1, #144
	lsls r1, r1, #10
	adds r3, r3, r1
	movs r7, #1
	str r3, [r6, #12]
	b .L_02008390
.L_02008346:
	movs r7, #1
	b .L_02008390
.L_0200834a:
	movs r0, #187
	bl Func_020059f4
	movs r0, #254
	lsls r0, r0, #7
	adds r0, #255
	movs r1, #0
	bl Func_02005954
	movs r0, #12
	bl Func_02005964
	b .L_02008390
.L_02008364:
	.4byte gOverlayArea + 0x6a24
.L_02008368:
	.4byte .L_020081bc
.L_0200836c:
	.4byte 0x002063ff
.L_02008370:
	.4byte 0xfe980000
.L_02008374:
	.4byte Data_0200641c
.L_02008378:
	.4byte 0x00203210
.L_0200837c:
	.4byte Data_0300122c
.L_02008380:
	movs r0, #36
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
.L_02008390:
	cmp r7, #0
	beq .L_02008448
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	ldr r2, [r6, #12]
	lsls r3, r3, #4
	lsrs r3, r3, #16
	lsls r3, r3, #16
	subs r2, r2, r3
	ldr r3, .L_02008428
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r6, #8]
	ldr r3, [r6, #16]
	lsls r0, r0, #2
	bl Func_0200576c
	adds r7, r0, #0
	cmp r7, #0
	beq .L_02008448
	ldr r1, [r7, #80]
	movs r5, #0
	mov r10, r1
	ldr r1, .L_0200842c
	bl Func_02005764
	movs r1, #1
	adds r0, r7, #0
	bl Object_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_02008430
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	ldr r1, .L_02008424
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	mov r8, r1
	bl Random16Far
	adds r3, r7, #0
	lsrs r0, r0, #13
	adds r3, #98
	strb r0, [r3]
	ldr r3, .L_02008434
	str r3, [r7, #108]
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	str r3, [r7, #48]
	mov r1, r10
	movs r2, #50
	ldrsh r3, [r6, r2]
	str r3, [r7, #48]
	mov r3, r8
	b .L_02008438
	.2byte 0x0000
.L_02008424:
	.4byte 0x00000000
.L_02008428:
	.4byte 0xfff80000
.L_0200842c:
	.4byte Data_02006470
.L_02008430:
	.4byte 0x0ffff000
.L_02008434:
	.4byte Func_020000ec
.L_02008438:
	ldrb r2, [r1, #9]
	strb r3, [r1, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
.L_02008448:
	ldr r2, .L_02008458
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008458:
	.4byte gOverlayArea + 0x6a24
	.section .text.x0200845c,"ax",%progbits
	.global Func_0200045c
	.thumb_func
Func_0200045c:
	push {lr}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #35
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084c0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084c0
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	ldr r0, .L_020084c4
	bl Func_020058c4
	movs r1, #4
	movs r2, #30
	adds r1, #255
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	bl Func_020058dc
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	bl Func_020058dc
	bl Func_0200581c
.L_020084c0:
	pop {pc}
	.2byte 0x0000
.L_020084c4:
	.4byte 0x00002ac7
	.section .text.x020084c8,"ax",%progbits
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084ea
	ldr r3, .L_0200850c
	movs r1, #3
	ldr r0, [r3]
	bl Engine_MathModulo
	cmp r0, #0
	bne .L_020085b0
.L_020084ea:
	movs r0, #31
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008510
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #8
	b .L_0200851a
.L_0200850c:
	.4byte Data_0300122c
.L_02008510:
	bl Random16Far
	adds r2, r0, #0
	ldr r3, [r5, #12]
	lsls r2, r2, #6
.L_0200851a:
	lsrs r2, r2, #16
	lsls r2, r2, #16
	adds r2, r2, r3
	ldr r3, .L_020085a0
	movs r0, #168
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	lsls r0, r0, #2
	bl Func_0200576c
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020085b0
	ldr r1, .L_020085a4
	adds r0, r7, #0
	ldr r6, [r7, #80]
	bl Func_02005764
	movs r1, #1
	adds r0, r7, #0
	bl Object_SetPartAttribute
	adds r3, r7, #0
	adds r3, #85
	movs r5, #0
	strb r5, [r3]
	bl Random16Far
	ldr r3, .L_020085a8
	adds r2, r7, #0
	adds r2, #100
	ands r3, r0
	strh r3, [r2]
	adds r3, r7, #0
	adds r3, #102
	strh r5, [r3]
	ldr r3, .L_020085ac
	ldr r1, .L_0200859c
	str r3, [r7, #108]
	mov r8, r1
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #16
	subs r0, r0, r3
	lsrs r0, r0, #20
	bl Math_Sine
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	mov r3, r8
	ldrb r2, [r6, #9]
	strb r3, [r6, #26]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r6, #9]
	b .L_020085b0
	.2byte 0x0000
.L_0200859c:
	.4byte 0x00000000
.L_020085a0:
	.4byte 0xffe40000
.L_020085a4:
	.4byte Data_020068b0
.L_020085a8:
	.4byte 0x0ffff000
.L_020085ac:
	.4byte Func_02000090
.L_020085b0:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020085b8,"ax",%progbits
	.global Func_020005b8
	.thumb_func
Func_020005b8:
	push {r5, r6, lr}
	movs r0, #36
	bl Object_GetById
	movs r3, #192
	adds r5, r0, #0
	lsls r3, r3, #18
	ldr r6, [r3, #32]
	bl Random16Far
	cmp r5, #0
	beq .L_02008656
	adds r3, r6, #0
	adds r3, #232
	movs r2, #2
	ldrsh r3, [r3, r2]
	cmp r3, #177
	bgt .L_0200864c
	movs r0, #36
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r2, #1
	ldr r3, [r3, #40]
	strb r2, [r3, #5]
	ldr r3, .L_02008658
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_0200861a
	movs r1, #128
	movs r2, #132
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #36
	bl Func_02005884
	movs r0, #36
	bl Object_GetById
	movs r3, #223
	lsls r3, r3, #15
	str r3, [r0, #12]
	movs r0, #36
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #9
	b .L_02008640
.L_0200861a:
	movs r1, #128
	movs r2, #132
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #36
	bl Func_02005884
	movs r0, #36
	bl Object_GetById
	movs r3, #209
	lsls r3, r3, #15
	str r3, [r0, #12]
	movs r0, #36
	bl Object_GetById
	movs r5, #166
	lsls r5, r5, #9
	adds r5, #204
.L_02008640:
	str r5, [r0, #24]
	movs r0, #36
	bl Object_GetById
	str r5, [r0, #28]
	b .L_02008656
.L_0200864c:
	movs r0, #36
	movs r1, #0
	movs r2, #0
	bl Func_02005884
.L_02008656:
	pop {r5, r6, pc}
.L_02008658:
	.4byte Data_0300122c
	.section .text.x0200865c,"ax",%progbits
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #35
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200867c
	b .L_02008cac
.L_0200867c:
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	ldr r2, .L_020086c0
	movs r1, #5
	mov r8, r2
	mov r0, r8
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	mov r11, r0
	cmp r0, #0
	beq .L_020086c4
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #54
	movs r0, #4
	adds r1, #2
	adds r2, #255
	bl ObjectMotion_SetPositionAndReset
	b .L_02008ca8
.L_020086c0:
	.4byte 0x00002acd
.L_020086c4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_SetBit
	movs r0, #252
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #37
	bl GameFlag_ClearBit
	movs r0, #98
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #147
	lsls r2, r2, #1
	movs r0, #4
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #4
	bl Func_020058ec
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_0200595c
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #1
	adds r0, #3
	bl Func_02005954
	movs r0, #20
	bl Func_02005964
	movs r0, #20
	bl WaitFrames
	movs r0, #245
	bl PartyInventory_Remove
	movs r0, #4
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #12]
	movs r3, #128
	lsls r3, r3, #12
	movs r0, #234
	adds r2, r2, r3
	ldr r1, [r5, #8]
	ldr r3, [r5, #16]
	adds r0, #255
	bl Func_0200576c
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020087f4
	ldr r6, [r7, #80]
	mov r2, r11
	strb r2, [r6, #27]
	ldrb r2, [r6, #5]
	movs r3, #33
	negs r3, r3
	ands r3, r2
	ldrb r2, [r6, #9]
	strb r3, [r6, #5]
	movs r3, #15
	ands r3, r2
	adds r1, r7, #0
	movs r2, #13
	adds r1, #35
	negs r2, r2
	ands r3, r2
	ldrb r2, [r1]
	strb r3, [r6, #9]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r7, #0
	adds r3, #85
	mov r2, r11
	strb r2, [r3]
	adds r2, r7, #0
	adds r2, #92
	movs r3, #1
	strb r3, [r2]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r7, #48]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	movs r1, #193
	str r3, [r7, #52]
	lsls r1, r1, #3
	movs r0, #68
	bl Runtime_AllocateHeapBlockFar
	str r0, [sp, #4]
	movs r0, #246
	bl Func_020057d4
	ldr r3, [sp, #4]
	movs r2, #128
	lsls r2, r2, #3
	ldrb r0, [r6, #16]
	adds r2, r3, r2
	movs r1, #128
	str r2, [sp, #0]
	bl VramBlock_LoadCached
	movs r0, #68
	bl Runtime_ReleaseHeapBlock
.L_020087f4:
	movs r1, #1
	movs r0, #37
	bl ObjectMotion_SetActionVariant
	movs r0, #37
	bl Object_GetById
	ldr r3, [r5, #8]
	adds r6, r0, #0
	str r3, [r6, #8]
	movs r3, #192
	lsls r3, r3, #15
	str r3, [r6, #12]
	adds r2, r6, #0
	ldr r3, [r5, #16]
	adds r2, #85
	str r3, [r6, #16]
	movs r3, #3
	strb r3, [r2]
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	movs r2, #204
	str r3, [r6, #52]
	lsls r2, r2, #7
	movs r3, #128
	adds r2, #102
	lsls r3, r3, #6
	mov r0, r8
	adds r0, #1
	str r2, [r6, #48]
	str r3, [r6, #24]
	str r3, [r6, #28]
	mov r9, r2
	bl Func_020058c4
	movs r0, #36
	bl Func_020059f4
	cmp r7, #0
	bne .L_02008848
	b .L_020089ca
.L_02008848:
	movs r3, #85
	adds r3, r3, r7
	mov r2, r11
	mov r10, r3
	strb r2, [r3]
	movs r3, #153
	lsls r3, r3, #8
	adds r3, #153
	str r3, [r7, #72]
	movs r3, #204
	lsls r3, r3, #8
	movs r2, #128
	adds r3, #204
	lsls r2, r2, #12
	mov r8, r3
	str r3, [r7, #68]
	str r2, [r7, #40]
	movs r1, #128
	movs r2, #128
	movs r3, #147
	adds r0, r7, #0
	lsls r1, r1, #18
	lsls r2, r2, #16
	lsls r3, r3, #17
	bl Func_02005784
	movs r1, #128
	movs r2, #1
	movs r3, #147
	adds r0, r6, #0
	lsls r1, r1, #18
	negs r2, r2
	lsls r3, r3, #17
	bl Func_02005784
	ldr r3, .L_020089b8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #7
	bl Func_02005894
	movs r0, #1
	bl WaitFrames
	mov r1, r8
	movs r0, #7
	mov r2, r9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #7
	movs r1, #12
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #12
	movs r2, #0
	negs r1, r1
	movs r0, #4
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	movs r0, #7
	bl Object_SetModeById
	movs r0, #1
	bl WaitFrames
	movs r1, #192
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	adds r0, r7, #0
	bl Func_0200578c
	movs r3, #4
	mov r8, r3
	mov r2, r8
	mov r3, r10
	strb r2, [r3]
	movs r0, #90
	bl Battle_WaitMode0
	mov r2, r11
	mov r3, r10
	strb r2, [r3]
	movs r1, #128
	movs r2, #128
	movs r3, #132
	adds r0, r7, #0
	lsls r1, r1, #18
	lsls r2, r2, #16
	lsls r3, r3, #17
	bl Func_02005784
	movs r1, #128
	movs r2, #1
	movs r3, #132
	lsls r1, r1, #18
	adds r0, r6, #0
	negs r2, r2
	lsls r3, r3, #17
	bl Func_02005784
	mov r2, r8
	mov r3, r10
	strb r2, [r3]
	movs r0, #60
	bl Battle_WaitMode0
	mov r2, r11
	mov r3, r10
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #24
	mov r2, r11
	str r3, [r6, #56]
	str r3, [r6, #60]
	str r3, [r6, #64]
	adds r3, r6, #0
	str r2, [r6, #8]
	str r2, [r6, #12]
	str r2, [r6, #16]
	str r2, [r6, #36]
	str r2, [r6, #40]
	str r2, [r6, #44]
	adds r3, #100
	mov r2, r11
	strh r2, [r3]
	adds r0, r7, #0
	bl Func_0200578c
	ldr r6, .L_020089b4
	mov r3, r8
	mov r2, r10
	strb r3, [r2]
	movs r0, #90
	bl Battle_WaitMode0
	mov r3, r10
	strb r6, [r3]
	movs r1, #128
	movs r2, #184
	movs r3, #132
	lsls r2, r2, #15
	lsls r3, r3, #17
	adds r0, r7, #0
	lsls r1, r1, #18
	bl Func_02005784
	adds r0, r7, #0
	bl Func_0200578c
	movs r3, #128
	lsls r3, r3, #24
	mov r2, r11
	str r3, [r7, #56]
	str r3, [r7, #60]
	str r3, [r7, #64]
	adds r3, r7, #0
	str r2, [r7, #8]
	str r2, [r7, #12]
	b .L_020089bc
	.2byte 0x0000
.L_020089b4:
	.4byte 0x00000000
.L_020089b8:
	.4byte gPartyState
.L_020089bc:
	str r2, [r7, #16]
	str r2, [r7, #36]
	str r2, [r7, #40]
	str r2, [r7, #44]
	adds r3, #100
	mov r2, r11
	strh r2, [r3]
.L_020089ca:
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02005954
	movs r0, #20
	bl Func_02005964
	movs r0, #20
	bl WaitFrames
	movs r2, #16
	movs r0, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #7
	bl ObjectMotion_OffsetPositionAndReset
	ldr r6, .L_02008cbc
	movs r3, #133
	lsls r3, r3, #2
	adds r6, r6, r3
	ldr r0, [r6]
	bl ObjectMotion_CommitCurrentPositionAndActivate
	ldr r0, [r6]
	movs r1, #1
	bl Object_SetModeById
	movs r1, #1
	movs r0, #7
	bl Object_SetModeById
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #141
	bl Func_020059f4
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	lsls r0, r0, #9
	bl Func_020057ac
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_020057ac
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #0
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #1
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #2
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #3
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #4
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #5
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #7
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #6
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #30
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #31
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #32
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #132
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	bl Motion_CamBounds
	bl Func_02005924
	movs r0, #31
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	ldr r0, [r6]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #1
	movs r0, #7
	bl ObjectMotion_SetActionVariant
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #31
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #31
	bl Object_GetById
	ldr r3, .L_02008cc0
	movs r2, #200
	lsls r2, r2, #5
	str r3, [r0, #28]
	adds r2, #153
	mov r9, r3
	adds r3, r0, #0
	str r2, [r0, #24]
	mov r10, r2
	adds r3, #85
	mov r2, r11
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #18
	str r3, [r0, #8]
	mov r8, r3
	movs r6, #132
	movs r3, #128
	lsls r6, r6, #17
	lsls r3, r3, #16
	str r3, [r0, #12]
	str r6, [r0, #16]
	adds r0, #35
	ldrb r3, [r0]
	movs r7, #2
	orrs r3, r7
	strb r3, [r0]
	movs r0, #32
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #32
	bl Object_GetById
	movs r1, #7
	bl Object_SetPartAttribute
	movs r0, #32
	bl Object_GetById
	mov r3, r10
	str r3, [r0, #24]
	mov r2, r9
	adds r3, r0, #0
	str r2, [r0, #28]
	adds r3, #85
	mov r2, r11
	strb r2, [r3]
	mov r3, r8
	str r3, [r0, #8]
	movs r3, #160
	lsls r3, r3, #16
	str r3, [r0, #12]
	str r6, [r0, #16]
	adds r0, #35
	ldrb r3, [r0]
	ldr r1, .L_02008cc4
	orrs r3, r7
	strb r3, [r0]
	movs r0, #31
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #32
	ldr r1, .L_02008cc4
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r6, .L_02008cc8
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r6, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02005884
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02005884
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02005884
	movs r1, #0
	movs r2, #0
	movs r0, #24
	bl Func_02005884
	movs r0, #145
	bl Func_020059f4
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r2, r2, #9
	lsls r0, r0, #11
	lsls r1, r1, #11
	bl Func_020057ac
	movs r1, #0
	ldr r0, .L_02008ccc
	bl Func_02005954
	movs r0, #16
	bl Func_02005964
	movs r0, #16
	bl WaitFrames
	movs r0, #63
	bl Func_020059f4
	movs r0, #180
	bl WaitFrames
	movs r0, #254
	lsls r0, r0, #7
	movs r1, #0
	adds r0, #255
	bl Func_02005954
	movs r0, #60
	bl Func_02005964
	movs r0, #141
	bl Func_020059f4
	movs r0, #60
	bl WaitFrames
	adds r0, r6, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #141
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #2
	bl Func_02005934
.L_02008ca8:
	bl Func_0200581c
.L_02008cac:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cbc:
	.4byte gPartyState
.L_02008cc0:
	.4byte 0xffff0000
.L_02008cc4:
	.4byte Data_02006338
.L_02008cc8:
	.4byte Func_020004c8
.L_02008ccc:
	.4byte 0x004063ff
	.section .text.x02008cd0,"ax",%progbits
	.global Func_02000cd0
	.thumb_func
Func_02000cd0:
	push {r5, r6, r7, lr}
	movs r7, #192
	bl Func_02005814
	lsls r7, r7, #18
	movs r0, #0
	bl Func_02005984
	ldr r2, [r7, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r6, #128
	adds r3, r2, r1
	lsls r6, r6, #1
	str r6, [r3]
	movs r3, #218
	lsls r3, r3, #1
	adds r2, r2, r3
	movs r3, #120
	str r3, [r2]
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_0200592c
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #192
	movs r0, #128
	movs r2, #240
	lsls r1, r1, #15
	lsls r2, r2, #16
	lsls r0, r0, #18
	bl Motion_CamBounds
	movs r0, #1
	bl WaitFrames
	bl Func_0200577c
	movs r0, #1
	bl WaitFrames
	movs r5, #192
	ldr r0, .L_020090cc
	bl Func_020058c4
	lsls r5, r5, #8
	movs r1, #254
	movs r2, #139
	adds r3, r5, #0
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #133
	movs r2, #139
	adds r3, r5, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #7
	bl Func_0200588c
	movs r0, #141
	bl Func_020059f4
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020057ac
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #192
	movs r1, #192
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_020057ac
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #10
	bl Func_020057ac
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #195
	lsls r0, r0, #1
	bl Func_020059f4
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r0, #204
	movs r1, #192
	lsls r0, r0, #7
	lsls r1, r1, #4
	adds r0, #102
	adds r1, #204
	bl Func_02005914
	movs r0, #128
	movs r1, #192
	movs r2, #148
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005924
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020058ec
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #7
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #27
	movs r1, #32
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r1, #48
	negs r2, r2
	movs r0, #28
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #7
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #27
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #28
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	movs r0, #28
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #78
	bl Func_020059f4
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #34
	bl Func_020059f4
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	adds r1, r6, #0
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	adds r1, r6, #0
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	movs r0, #4
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	adds r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r2, #0
	movs r0, #4
	lsls r1, r1, #6
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #7
	bl Func_020058fc
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	adds r0, #28
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008fc4
	movs r0, #128
	lsls r0, r0, #5
	movs r1, #0
	movs r2, #0
	adds r0, #28
	bl Func_020058d4
	ldr r2, [r7, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02008fe0
.L_02008fc4:
	ldr r2, [r7, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #128
	adds r3, #1
	lsls r0, r0, #5
	strh r3, [r2]
	adds r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_02008fe0:
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #128
	movs r2, #0
	movs r0, #7
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #4
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #16
	movs r2, #24
	movs r0, #28
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
	movs r1, #0
	movs r2, #24
	movs r0, #28
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #28
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	movs r1, #160
	strb r3, [r0]
	lsls r1, r1, #8
	movs r0, #28
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r3, r3, r1
	ldrh r0, [r3]
	movs r1, #5
	adds r2, r0, #1
	lsls r0, r0, #16
	strh r2, [r3]
	asrs r0, r0, #16
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #224
	bl PartyInventory_Add
	movs r5, #0
	movs r6, #216
	b .L_020090d4
	.2byte 0x0000
.L_020090cc:
	.4byte 0x00002ace
.L_020090d0:
	adds r6, #2
	adds r5, #1
.L_020090d4:
	cmp r5, #14
	bgt .L_020090f8
	movs r0, #4
	bl Owner_GetState
	ldr r3, .L_020090f4
	ldrh r2, [r0, r6]
	ands r3, r2
	cmp r3, #224
	bne .L_020090d0
	movs r0, #4
	adds r1, r5, #0
	bl Func_020057ec
	b .L_020090f8
	.2byte 0x0000
.L_020090f4:
	.4byte 0x000001ff
.L_020090f8:
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #24
	movs r2, #8
	negs r1, r1
	negs r2, r2
	movs r0, #27
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #8
	strb r3, [r0]
	movs r1, #0
	movs r0, #4
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r1, #0
	movs r2, #0
	movs r0, #28
	bl Func_020058d4
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #10
	ands r5, r3
	movs r2, #30
	strb r5, [r0]
	adds r1, #255
	movs r0, #4
	bl Func_020058fc
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #0
	adds r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	movs r0, #7
	lsls r1, r1, #1
	bl Func_02005904
	ldr r3, .L_02009380
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	movs r1, #129
	ldr r0, [r3]
	lsls r1, r1, #1
	bl Func_02005904
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r1, #176
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r0, #28
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02009236
	movs r1, #3
	movs r0, #28
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #28
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_02009260
.L_02009236:
	movs r1, #3
	movs r0, #28
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r1, #226
	lsls r1, r1, #1
	adds r2, r2, r1
	ldrh r3, [r2]
	movs r0, #28
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_02009260:
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r1, #208
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #208
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #28
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #2
	adds r1, #255
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #176
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #28
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #4
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #7
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	ldr r3, .L_02009380
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_02009384
	movs r1, #10
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #10
	bl Party_SetFields1f2And1f4
	movs r0, #28
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #13
	movs r1, #5
	bl Func_0200593c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009380:
	.4byte gPartyState
.L_02009384:
	.4byte 0x00000109
	.section .text.x02009388,"ax",%progbits
	.global Func_02001388
	.thumb_func
Func_02001388:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02005814
	movs r6, #160
	movs r0, #0
	bl Func_02005984
	lsls r6, r6, #8
	movs r1, #128
	movs r2, #156
	adds r3, r6, #0
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #132
	movs r2, #152
	adds r3, r6, #0
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r2, #128
	lsls r2, r2, #6
	mov r8, r2
	movs r1, #252
	movs r2, #152
	mov r3, r8
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #130
	movs r2, #144
	mov r3, r8
	movs r0, #28
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r3, #192
	movs r1, #138
	movs r2, #196
	lsls r3, r3, #8
	lsls r2, r2, #17
	movs r0, #5
	lsls r1, r1, #18
	bl Func_0200588c
	movs r0, #27
	movs r1, #9
	bl Object_SetModeById
	movs r0, #28
	movs r1, #8
	bl Object_SetModeById
	movs r0, #4
	movs r1, #50
	bl Object_SetModeById
	movs r0, #7
	movs r1, #12
	bl Object_SetModeById
	movs r0, #130
	movs r1, #208
	movs r2, #156
	movs r3, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, .L_020096dc
	bl Func_020058c4
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #28
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #7
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r0, #4
	bl Object_GetById
	movs r5, #192
	lsls r5, r5, #10
	str r5, [r0, #40]
	movs r0, #7
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #27
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #28
	bl Object_GetById
	movs r1, #8
	str r5, [r0, #40]
	movs r2, #4
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #7
	movs r1, #8
	movs r2, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #27
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	negs r2, r2
	negs r1, r1
	movs r0, #28
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #7
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r0, #4
	bl Object_AttachWorkTargetToObject
	movs r1, #138
	movs r2, #172
	movs r0, #5
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #7
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #7
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #28
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	mov r1, r8
	movs r0, #28
	bl ObjectMotion_ArmCallback
	mov r1, r8
	movs r0, #27
	bl Func_020058ec
	adds r1, r6, #0
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	bl Func_020058dc
	movs r1, #8
	movs r2, #24
	negs r2, r2
	movs r0, #5
	negs r1, r1
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #4
	mov r1, r8
	bl Func_020058ec
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	adds r1, r6, #0
	bl Func_020058ec
	adds r1, r6, #0
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #16
	movs r2, #8
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #7
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #5
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #27
	movs r1, #16
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r2, #8
	movs r1, #16
	movs r0, #28
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #11
	str r5, [r0, #40]
	movs r0, #7
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #27
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #28
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r5, .L_020096e0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_020096ae
	movs r1, #10
	movs r0, #4
	negs r1, r1
	bl Owner_AdjustFirstValue
.L_020096ae:
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_020096e4
	movs r1, #11
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #11
	bl Party_SetFields1f2And1f4
	movs r0, #13
	movs r1, #6
	bl Func_0200593c
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020096dc:
	.4byte 0x00002ae4
.L_020096e0:
	.4byte gPartyState
.L_020096e4:
	.4byte 0x00000109
	.section .text.x020096e8,"ax",%progbits
	.global Func_020016e8
	.thumb_func
Func_020016e8:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	bl Func_02005814
	movs r6, #160
	movs r0, #0
	bl Func_02005984
	lsls r6, r6, #8
	movs r1, #128
	movs r2, #156
	adds r3, r6, #0
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #132
	movs r2, #152
	adds r3, r6, #0
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r2, #128
	lsls r2, r2, #6
	mov r8, r2
	movs r1, #252
	movs r2, #152
	mov r3, r8
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #130
	movs r2, #144
	mov r3, r8
	movs r0, #28
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #132
	movs r2, #160
	adds r3, r6, #0
	movs r0, #5
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r3, #192
	movs r1, #136
	movs r2, #192
	lsls r3, r3, #8
	movs r0, #6
	lsls r1, r1, #18
	lsls r2, r2, #17
	mov r10, r3
	bl Func_0200588c
	movs r0, #130
	movs r1, #208
	movs r2, #156
	movs r3, #0
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, .L_02009ab8
	bl Func_020058c4
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #9
	bl Object_SetModeById
	movs r0, #28
	movs r1, #8
	bl Object_SetModeById
	movs r0, #4
	movs r1, #50
	bl Object_SetModeById
	movs r0, #7
	movs r1, #12
	bl Object_SetModeById
	movs r0, #5
	movs r1, #27
	bl Object_SetModeById
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #4
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #5
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #28
	bl Func_020058fc
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #7
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #5
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	ands r5, r3
	strb r5, [r0]
	movs r0, #4
	bl Object_GetById
	movs r5, #192
	lsls r5, r5, #10
	str r5, [r0, #40]
	movs r0, #7
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #27
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #28
	bl Object_GetById
	movs r1, #8
	str r5, [r0, #40]
	movs r2, #4
	movs r0, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #7
	movs r1, #8
	movs r2, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #5
	movs r1, #8
	movs r2, #4
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #27
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	negs r2, r2
	negs r1, r1
	movs r0, #28
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #7
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #5
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r0, #4
	bl Object_AttachWorkTargetToObject
	movs r1, #138
	movs r2, #172
	movs r0, #6
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020058fc
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r2, #0
	movs r0, #28
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #27
	bl Func_020058d4
	movs r0, #10
	bl Battle_WaitMode0
	mov r1, r10
	movs r0, #6
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #24
	negs r2, r2
	movs r1, #0
	movs r0, #6
	bl ObjectMotion_OffsetPositionAndReset
	movs r0, #6
	bl ObjectMotion_CommitCurrentPositionAndActivate
	mov r1, r8
	movs r0, #4
	bl Func_020058ec
	movs r0, #6
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	adds r1, r6, #0
	bl Func_020058ec
	adds r1, r6, #0
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #16
	movs r2, #8
	movs r0, #4
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #7
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #5
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r1, #16
	movs r2, #8
	movs r0, #6
	negs r1, r1
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #27
	movs r1, #16
	movs r2, #8
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r2, #8
	movs r1, #16
	movs r0, #28
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #4
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #11
	str r5, [r0, #40]
	movs r0, #7
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #5
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #6
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #27
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #28
	bl Object_GetById
	str r5, [r0, #40]
	movs r0, #4
	bl Battle_WaitMode0
	ldr r5, .L_02009abc
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_02009a82
	movs r1, #10
	movs r0, #4
	negs r1, r1
	bl Owner_AdjustFirstValue
.L_02009a82:
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	movs r2, #2
	strb r2, [r3]
	ldr r5, .L_02009ac0
	movs r1, #12
	adds r0, r5, #0
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #13
	bl Party_SetFields1f2And1f4
	movs r0, #28
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #13
	movs r1, #7
	bl Func_0200593c
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
.L_02009ab8:
	.4byte 0x00002aea
.L_02009abc:
	.4byte gPartyState
.L_02009ac0:
	.4byte 0x00000109
	.section .text.x02009ac4,"ax",%progbits
	.global Func_02001ac4
	.thumb_func
Func_02001ac4:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r6, r1, #0
	bl Func_020059c4
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #2
	bl Func_020059dc
	movs r0, #201
	bl Func_020059f4
	movs r0, #60
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Object_GetById
	movs r1, #0
	bl Func_020059dc
	bl Func_020059d4
	bl Func_020059cc
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetModeById
	pop {r5, r6, pc}
	.section .text.x02009b04,"ax",%progbits
	.global Func_02001b04
	.thumb_func
Func_02001b04:
	push {r5, r6, r7, lr}
	mov r7, r9
	push {r7}
	sub sp, #4
	mov r1, r9
	str r1, [sp, #0]
	adds r7, r0, #0
	bl Party_CountActiveOwners
	movs r5, #0
	adds r6, r0, #0
	cmp r5, r6
	bge .L_02009bb6
.L_02009b1e:
	ldr r2, .L_02009bc0
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl Owner_GetState
	ldrh r1, [r0, #52]
	mov r9, r0
	mov r2, r9
	strh r1, [r2, #56]
	lsls r1, r1, #16
	asrs r1, r1, #16
	lsls r0, r1, #14
	bl Engine_MathDivide
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02009b4e
	movs r3, #0
	cmp r0, #0
	blt .L_02009b4e
	adds r3, r0, #0
.L_02009b4e:
	mov r1, r9
	strh r3, [r1, #20]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009b64
	movs r2, #56
	ldrsh r3, [r1, r2]
	cmp r3, #0
	beq .L_02009b64
	movs r3, #1
	strh r3, [r1, #20]
.L_02009b64:
	mov r3, r9
	movs r2, #58
	ldrsh r0, [r3, r2]
	movs r2, #54
	ldrsh r1, [r3, r2]
	lsls r0, r0, #14
	bl Engine_MathDivide
	movs r3, #128
	lsls r3, r3, #7
	cmp r0, r3
	bgt .L_02009b84
	movs r3, #0
	cmp r0, #0
	blt .L_02009b84
	adds r3, r0, #0
.L_02009b84:
	mov r1, r9
	strh r3, [r1, #22]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_02009b9a
	movs r2, #58
	ldrsh r3, [r1, r2]
	cmp r3, #0
	beq .L_02009b9a
	movs r3, #1
	strh r3, [r1, #22]
.L_02009b9a:
	cmp r7, #1
	bne .L_02009bb0
	movs r3, #50
	adds r3, #255
	add r3, r9
	movs r2, #0
	strb r2, [r3]
	movs r3, #160
	lsls r3, r3, #1
	add r3, r9
	strb r2, [r3]
.L_02009bb0:
	adds r5, #1
	cmp r5, r6
	blt .L_02009b1e
.L_02009bb6:
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009bc0:
	.4byte gPartyState
	.section .text.x02009bc4,"ax",%progbits
	.global Func_02001bc4
	.thumb_func
Func_02001bc4:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r0, #0
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #192
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	lsls r6, r6, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #3
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #4
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #5
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #7
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #6
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #30
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #252
	orrs r5, r3
	movs r2, #152
	movs r3, #160
	strb r5, [r0]
	lsls r3, r3, #8
	movs r0, #4
	lsls r1, r1, #17
	lsls r2, r2, #17
	mov r10, r3
	bl Func_0200588c
	movs r1, #132
	movs r2, #144
	mov r3, r10
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r3, #128
	movs r1, #244
	movs r2, #148
	lsls r3, r3, #6
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	mov r8, r3
	bl Func_0200588c
	movs r1, #252
	movs r2, #140
	mov r3, r8
	movs r0, #28
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #132
	movs r2, #152
	mov r3, r10
	movs r0, #5
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #144
	adds r3, r6, #0
	movs r0, #6
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #240
	movs r2, #180
	adds r3, r6, #0
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #148
	movs r2, #196
	lsls r2, r2, #17
	movs r3, #0
	movs r0, #30
	lsls r1, r1, #18
	bl Func_0200588c
	movs r0, #5
	movs r1, #19
	bl Object_SetModeById
	movs r0, #6
	movs r1, #19
	bl Object_SetModeById
	movs r0, #4
	movs r1, #19
	bl Object_SetModeById
	movs r1, #13
	movs r0, #7
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	ldr r3, [r0, #24]
	negs r3, r3
	str r3, [r0, #24]
	movs r0, #6
	bl Object_GetById
	ldr r3, [r0, #24]
	movs r1, #208
	negs r3, r3
	str r3, [r0, #24]
	movs r2, #156
	movs r0, #130
	movs r3, #0
	lsls r2, r2, #17
	lsls r1, r1, #15
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, .L_0200a150
	bl Func_020058c4
	movs r0, #27
	movs r1, #9
	bl Object_SetModeById
	movs r0, #28
	movs r1, #8
	bl Object_SetModeById
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #28
	movs r1, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #27
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	mov r1, r8
	movs r0, #28
	bl Func_020058ec
	mov r1, r8
	movs r0, #27
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #27
	bl Func_020058fc
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #0
	movs r2, #0
	movs r0, #29
	bl Func_020058d4
	movs r0, #78
	bl Func_020059f4
	movs r1, #128
	movs r2, #0
	movs r0, #27
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #28
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #130
	movs r1, #208
	movs r2, #172
	movs r3, #1
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005924
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #39
	bl Func_020059f4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #1
	bl Object_SetModeById
	movs r1, #192
	movs r2, #0
	movs r0, #28
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #29
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	movs r0, #30
	movs r1, #0
	bl Func_020058d4
	movs r0, #28
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #27
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	mov r1, r8
	movs r0, #28
	bl ObjectMotion_ArmCallback
	mov r1, r8
	movs r0, #27
	bl Func_020058ec
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #29
	adds r1, r6, #0
	bl Func_020058ec
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #29
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #28
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #27
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	ldr r5, .L_0200a154
	movs r0, #27
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #29
	bl Object_RefreshSelectorById
	movs r0, #29
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #64
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #29
	bl Object_GetById
	ldr r5, .L_0200a158
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	adds r1, r5, #0
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #27
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #28
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #29
	bl Object_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	movs r0, #29
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #29
	bl ObjectMotion_ArmCallback
	movs r0, #27
	bl Object_RefreshSelectorById
	movs r1, #166
	movs r2, #133
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r1, #145
	movs r2, #133
	movs r0, #28
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #28
	bl ObjectMotion_ArmCallback
	movs r0, #29
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r2, #1
	movs r0, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #78
	bl Func_020059f4
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r2, #164
	adds r3, r6, #0
	lsls r1, r1, #18
	b .L_0200a15c
.L_0200a150:
	.4byte 0x00002b1c
.L_0200a154:
	.4byte Data_02005ff8
.L_0200a158:
	.4byte Data_02006088
.L_0200a15c:
	lsls r2, r2, #17
	movs r0, #30
	bl Func_0200588c
	movs r0, #0
	bl Func_02003a6c
	bl Func_0200598c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #134
	movs r2, #184
	adds r3, r6, #0
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #134
	movs r2, #192
	adds r3, r6, #0
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #184
	adds r3, r6, #0
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #176
	adds r3, r6, #0
	movs r0, #3
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #134
	movs r2, #168
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #134
	movs r2, #176
	movs r0, #1
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #138
	movs r2, #168
	movs r0, #2
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #138
	movs r2, #160
	lsls r2, r2, #1
	lsls r1, r1, #2
	movs r0, #3
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #1
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #2
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #3
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #30
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #2
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #2
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #3
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #3
	movs r1, #0
	bl Func_020058d4
	mov r1, r10
	movs r0, #0
	bl Func_020058ec
	movs r0, #160
	movs r1, #192
	movs r2, #180
	movs r3, #1
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02005924
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #128
	movs r1, #192
	movs r2, #176
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005924
	movs r0, #0
	bl Func_02003c58
	movs r0, #1
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #1
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	mov r1, r10
	movs r0, #2
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #3
	bl Func_020058fc
	movs r2, #0
	movs r0, #3
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #4
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	adds r1, r6, #0
	movs r0, #30
	bl Func_020058ec
	movs r1, #2
	movs r0, #30
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #0
	movs r1, #0
	movs r0, #30
	bl Func_020058d4
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl ObjectMotion_SetVariantCallback
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #7
	bl ObjectMotion_SetVariantCallback
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	mov r1, r10
	movs r0, #1
	bl Func_020058ec
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl Func_020058d4
	mov r1, r8
	movs r0, #30
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #30
	movs r1, #0
	bl Func_020058d4
	adds r1, r6, #0
	movs r0, #30
	bl Func_020058ec
	movs r1, #48
	movs r0, #3
	negs r1, r1
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	adds r1, r6, #0
	movs r2, #0
	movs r0, #3
	bl ObjectMotion_ArmCallback
	movs r0, #3
	movs r1, #4
	bl Func_02001ac4
	mov r1, r8
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #4
	bl Object_GetById
	ldr r3, [r0, #24]
	movs r1, #40
	negs r3, r3
	str r3, [r0, #24]
	movs r2, #0
	movs r0, #3
	bl ObjectMotion_CommitPositionAndActivate
	movs r2, #16
	movs r0, #3
	movs r1, #8
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	adds r1, r6, #0
	movs r2, #0
	movs r0, #3
	bl ObjectMotion_ArmCallback
	movs r0, #3
	movs r1, #6
	bl Func_02001ac4
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #6
	bl ObjectMotion_ArmCallback
	movs r0, #6
	bl Object_GetById
	ldr r3, [r0, #24]
	mov r1, r10
	negs r3, r3
	str r3, [r0, #24]
	movs r2, #0
	movs r0, #3
	bl ObjectMotion_ArmCallback
	movs r0, #3
	movs r1, #7
	bl Func_02001ac4
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r0, #3
	movs r1, #5
	bl Func_02001ac4
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r0, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	mov r1, r10
	movs r0, #0
	bl Func_020058ec
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	mov r1, r8
	movs r0, #30
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #30
	bl Func_020058fc
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #1
	bl Func_020058fc
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #2
	bl Func_020058fc
	movs r1, #138
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #2
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #1
	adds r1, r6, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #2
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #2
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #5
	movs r1, #6
	movs r2, #0
	bl Object_LinkPair
	movs r2, #0
	movs r1, #4
	movs r0, #7
	bl Object_LinkPair
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r6, #0
	movs r0, #0
	bl Func_020058ec
	movs r0, #50
	bl Battle_WaitMode0
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	mov r1, r10
	bl Func_020058ec
	movs r0, #4
	mov r1, r8
	bl Func_020058ec
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200a678
	movs r1, #3
	movs r0, #0
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	b .L_0200a6fe
.L_0200a678:
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_020058fc
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #128
	adds r3, #2
	strh r3, [r2]
	movs r0, #5
	lsls r1, r1, #8
	bl Func_020058ec
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_0200a6fe:
	movs r0, #2
	movs r1, #2
	bl Object_SetModeById
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a71e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl ObjectMotion_ResetAndSetPosition
.L_0200a71e:
	movs r0, #3
	movs r1, #2
	bl Object_SetModeById
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a73e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl ObjectMotion_ResetAndSetPosition
.L_0200a73e:
	movs r0, #1
	movs r1, #2
	bl Object_SetModeById
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a75e
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl ObjectMotion_ResetAndSetPosition
.L_0200a75e:
	ldr r5, .L_0200aad4
	movs r0, #2
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #3
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #1
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #2
	bl Object_RefreshSelectorById
	movs r0, #3
	bl Object_RefreshSelectorById
	movs r0, #1
	bl Object_RefreshSelectorById
	movs r0, #0
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r0, #4
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r1, #0
	movs r0, #30
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #108
	adds r2, #255
	adds r1, #255
	movs r0, #0
	bl ObjectMotion_SetPositionAndReset
	movs r0, #0
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #159
	lsls r1, r1, #1
	movs r2, #179
	movs r0, #0
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #146
	movs r2, #150
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #146
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r1, #154
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #247
	bl ObjectMotion_SetPositionAndReset
	movs r1, #181
	lsls r1, r1, #1
	movs r2, #156
	lsls r2, r2, #1
	adds r1, #255
	movs r0, #0
	bl ObjectMotion_SetPositionAndReset
	movs r0, #0
	bl Object_GetById
	movs r6, #2
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200aad8
	movs r0, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #0
	bl Object_RefreshSelectorById
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #1
	bl Func_02003a6c
	movs r0, #100
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #1
	bl Func_02003c58
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #4
	bl Object_AttachWorkTargetToObject
	bl Func_02005924
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #4
	bl Func_020058ec
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a980
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200a980:
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a9a0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200a9a0:
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a9c0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200a9c0:
	movs r0, #30
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200a9e0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #30
	bl ObjectMotion_ResetAndSetPosition
.L_0200a9e0:
	adds r1, r5, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #30
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #7
	bl Object_RefreshSelectorById
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r0, #5
	bl Object_RefreshSelectorById
	movs r0, #30
	bl Object_RefreshSelectorById
	movs r0, #4
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #16
	movs r2, #0
	movs r0, #4
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	ldr r1, .L_0200aadc
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #4
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200aae0
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	lsls r2, r2, #1
	movs r0, #4
	adds r1, #255
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #2
	bl Func_02003a6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #84
	str r3, [r2]
	subs r3, #76
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #0
	mov r9, sp
	bl Func_02001b04
	movs r0, #1
	bl Func_02005934
	bl Func_0200581c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_0200aad4:
	.4byte Data_02005ff0
.L_0200aad8:
	.4byte Data_020061e0
.L_0200aadc:
	.4byte Data_02005ff8
.L_0200aae0:
	.4byte Data_02006088
	.section .text.x0200aae4,"ax",%progbits
	.global Func_02002ae4
	.thumb_func
Func_02002ae4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r0, #0
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #1
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #2
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	lsls r6, r6, #6
	orrs r3, r5
	strb r3, [r0]
	movs r0, #3
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #4
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #5
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #7
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #6
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #30
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #130
	orrs r5, r3
	movs r2, #152
	movs r3, #160
	strb r5, [r0]
	lsls r3, r3, #8
	movs r0, #4
	lsls r1, r1, #18
	lsls r2, r2, #17
	mov r8, r3
	bl Func_0200588c
	movs r1, #134
	movs r2, #144
	mov r3, r8
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #240
	movs r2, #148
	adds r3, r6, #0
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #248
	movs r2, #140
	adds r3, r6, #0
	movs r0, #28
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #136
	movs r2, #152
	mov r3, r8
	movs r0, #5
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r5, #192
	bl Func_0200588c
	lsls r5, r5, #8
	movs r1, #140
	movs r2, #144
	adds r3, r5, #0
	movs r0, #6
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #240
	movs r2, #180
	adds r3, r5, #0
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #148
	movs r2, #196
	lsls r2, r2, #17
	movs r3, #0
	movs r0, #30
	lsls r1, r1, #18
	bl Func_0200588c
	movs r0, #27
	movs r1, #10
	bl Object_SetModeById
	movs r0, #28
	movs r1, #9
	bl Object_SetModeById
	movs r0, #128
	adds r1, r6, #0
	lsls r0, r0, #9
	bl Func_02005914
	movs r0, #130
	movs r1, #208
	movs r2, #156
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #0
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r0, .L_0200add8
	bl Func_020058c4
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #6
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r2, #0
	movs r0, #4
	movs r1, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r0, #5
	bl Func_020058ec
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #0
	mov r1, r8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	mov r1, r8
	movs r0, #5
	bl Func_020058ec
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #5
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #27
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #6
	bl Func_020058ec
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #0
	movs r0, #28
	bl UiText_OpenMessageAtObject
	movs r1, #192
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200addc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200adf8
	.2byte 0x0000
.L_0200add8:
	.4byte 0x00002af0
.L_0200addc:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r0, #29
	adds r3, #1
	strh r3, [r2]
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_0200adf8:
	movs r0, #78
	bl Func_020059f4
	movs r0, #130
	movs r1, #208
	movs r2, #176
	movs r3, #1
	lsls r1, r1, #15
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02005924
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #39
	bl Func_020059f4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r1, #192
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #130
	movs r1, #208
	movs r2, #156
	movs r3, #1
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #240
	movs r2, #160
	movs r0, #29
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r0, #29
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #29
	movs r2, #0
	movs r1, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #7
	bl Func_020058fc
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #5
	bl Func_020058fc
	movs r1, #10
	adds r1, #255
	movs r2, #0
	movs r0, #6
	bl Func_020058fc
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #4
	bl Func_020058fc
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r2, #20
	negs r2, r2
	movs r0, #29
	movs r1, #20
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #29
	movs r1, #27
	bl Func_02001ac4
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #6
	bl Func_020058fc
	movs r2, #0
	movs r0, #6
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #29
	movs r1, #28
	bl Func_02001ac4
	movs r1, #20
	movs r2, #20
	movs r0, #29
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r0, #27
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #28
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #29
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #7
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #27
	bl Func_020058fc
	movs r2, #0
	movs r0, #27
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #28
	bl Func_020058fc
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r2, #0
	movs r0, #27
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #28
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	movs r0, #28
	bl Func_020058fc
	movs r1, #129
	movs r2, #30
	lsls r1, r1, #1
	movs r0, #27
	bl Func_020058fc
	movs r0, #29
	movs r1, #0
	bl Func_020058ec
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #28
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #29
	movs r1, #0
	bl Func_020058d4
	movs r1, #2
	movs r0, #6
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #132
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #29
	bl Func_020058fc
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #7
	movs r0, #6
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #29
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r2, #0
	movs r0, #5
	lsls r1, r1, #8
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #29
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #29
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	movs r0, #28
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #27
	bl Func_020058ec
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #28
	movs r1, #3
	bl Object_SetModeById
	movs r0, #27
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #27
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	ldr r5, .L_0200b544
	movs r0, #29
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #30
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #29
	bl Object_RefreshSelectorById
	movs r0, #29
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #64
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #29
	bl Object_GetById
	ldr r5, .L_0200b548
	movs r3, #2
	adds r0, #85
	strb r3, [r0]
	adds r1, r5, #0
	movs r0, #29
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #27
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #27
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #28
	bl Object_RefreshSelectorById
	adds r1, r5, #0
	movs r0, #28
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #29
	bl Object_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	movs r0, #29
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #29
	bl ObjectMotion_ArmCallback
	movs r0, #27
	bl Object_RefreshSelectorById
	movs r1, #166
	movs r2, #133
	movs r0, #27
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #27
	bl ObjectMotion_ArmCallback
	movs r0, #28
	bl Object_RefreshSelectorById
	movs r1, #145
	movs r2, #133
	movs r0, #28
	lsls r1, r1, #1
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #28
	bl ObjectMotion_ArmCallback
	movs r0, #29
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	movs r0, #27
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #28
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r2, #1
	movs r0, #1
	movs r3, #0
	negs r1, r1
	negs r2, r2
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #78
	bl Func_020059f4
	movs r5, #192
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	lsls r5, r5, #8
	movs r1, #128
	movs r2, #164
	adds r3, r5, #0
	movs r0, #30
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #134
	movs r2, #168
	adds r3, r5, #0
	movs r0, #0
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #134
	movs r2, #176
	adds r3, r5, #0
	movs r0, #1
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #176
	adds r3, r5, #0
	movs r0, #2
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #138
	movs r2, #184
	adds r3, r5, #0
	movs r0, #3
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_0200588c
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #7
	bl ObjectMotion_ArmCallback
	movs r0, #3
	bl Func_02003a6c
	bl Func_0200598c
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #4
	bl Func_020058fc
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #5
	bl Func_020058fc
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #1
	lsls r1, r1, #8
	bl Func_020058ec
	adds r1, r5, #0
	movs r0, #1
	bl Func_020058ec
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #0
	bl Func_02003c58
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #1
	bl Func_020058fc
	movs r2, #0
	movs r0, #1
	movs r1, #0
	bl Func_020058d4
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #8
	bl Func_020058ec
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	movs r2, #30
	adds r1, #255
	movs r0, #30
	bl Func_020058fc
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #6
	bl Func_020058ec
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #1
	bl Func_020058fc
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #2
	bl Func_020058fc
	movs r1, #138
	movs r2, #160
	lsls r2, r2, #1
	movs r0, #2
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	adds r1, r5, #0
	movs r0, #2
	bl Func_020058ec
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #2
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r0, #2
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r2, #0
	movs r0, #2
	movs r1, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #0
	bl Func_020058ec
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #0
	movs r0, #0
	movs r1, #0
	bl Func_020058d4
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #7
	bl Func_020058ec
	movs r1, #2
	movs r0, #2
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #224
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #6
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #224
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #30
	movs r1, #0
	bl Func_020058ec
	movs r0, #30
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	movs r0, #6
	lsls r1, r1, #7
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #60
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #1
	bl Func_020058ec
	movs r0, #3
	movs r1, #3
	bl Object_SetModeById
	movs r0, #1
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	lsls r1, r1, #8
	movs r0, #0
	bl Func_020058ec
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	b .L_0200b54c
	.2byte 0x0000
.L_0200b544:
	.4byte Data_02005ff8
.L_0200b548:
	.4byte Data_02006088
.L_0200b54c:
	movs r1, #3
	bl Object_SetModeById
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #0
	bl Func_020058ec
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #7
	bl Func_020058ec
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200b5be
	movs r1, #3
	movs r0, #0
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	b .L_0200b644
.L_0200b5be:
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #0
	bl Func_020058fc
	movs r0, #5
	movs r1, #4
	bl Object_SetModeById
	movs r0, #33
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #226
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	movs r1, #128
	adds r3, #2
	strh r3, [r2]
	movs r0, #5
	lsls r1, r1, #8
	bl Func_020058ec
	movs r2, #0
	movs r0, #5
	movs r1, #0
	bl Func_020058d4
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #5
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #5
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r0, #0
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
.L_0200b644:
	movs r0, #2
	movs r1, #2
	bl Object_SetModeById
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b664
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl ObjectMotion_ResetAndSetPosition
.L_0200b664:
	movs r0, #3
	movs r1, #2
	bl Object_SetModeById
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b684
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl ObjectMotion_ResetAndSetPosition
.L_0200b684:
	movs r0, #1
	movs r1, #2
	bl Object_SetModeById
	movs r0, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b6a4
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl ObjectMotion_ResetAndSetPosition
.L_0200b6a4:
	ldr r5, .L_0200ba10
	movs r0, #2
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #3
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #1
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #2
	bl Object_RefreshSelectorById
	movs r0, #3
	bl Object_RefreshSelectorById
	movs r0, #1
	bl Object_RefreshSelectorById
	movs r0, #0
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r0, #4
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r0, #7
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r0, #5
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r0, #6
	movs r1, #0
	bl Object_LinkObjectAndSetCallback
	movs r1, #0
	movs r0, #30
	bl Object_LinkObjectAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #192
	movs r0, #0
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #144
	lsls r1, r1, #1
	movs r2, #108
	adds r2, #255
	movs r0, #0
	adds r1, #255
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	lsls r1, r1, #8
	movs r0, #0
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #0
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_020058d4
	movs r1, #159
	lsls r1, r1, #1
	movs r2, #179
	movs r0, #0
	adds r1, #255
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #146
	movs r2, #150
	movs r0, #0
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #146
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #252
	bl ObjectMotion_SetPositionAndReset
	movs r1, #154
	movs r0, #0
	lsls r1, r1, #2
	movs r2, #247
	bl ObjectMotion_SetPositionAndReset
	movs r1, #181
	lsls r1, r1, #1
	movs r2, #156
	lsls r2, r2, #1
	adds r1, #255
	movs r0, #0
	bl ObjectMotion_SetPositionAndReset
	movs r0, #0
	bl Object_GetById
	movs r6, #2
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200ba14
	movs r0, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #0
	bl Object_RefreshSelectorById
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #0
	bl Func_020058ec
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	movs r0, #4
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #7
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #5
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #6
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #0
	movs r2, #0
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #1
	bl Func_02003a6c
	movs r0, #100
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #5
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #6
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #30
	bl ObjectMotion_ArmCallback
	movs r0, #1
	bl Func_02003c58
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #1
	movs r0, #4
	bl Object_AttachWorkTargetToObject
	bl Func_02005924
	movs r0, #60
	bl Battle_WaitMode0
	movs r1, #0
	movs r0, #4
	bl Func_020058ec
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #5
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #224
	movs r0, #4
	lsls r1, r1, #8
	bl Func_020058ec
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #6
	movs r1, #3
	bl Object_SetModeById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b8c6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #7
	bl ObjectMotion_ResetAndSetPosition
.L_0200b8c6:
	movs r0, #6
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b8e6
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #6
	bl ObjectMotion_ResetAndSetPosition
.L_0200b8e6:
	movs r0, #5
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b906
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #5
	bl ObjectMotion_ResetAndSetPosition
.L_0200b906:
	movs r0, #30
	movs r1, #2
	bl Object_SetModeById
	movs r0, #4
	bl Object_GetById
	cmp r0, #0
	beq .L_0200b926
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #30
	bl ObjectMotion_ResetAndSetPosition
.L_0200b926:
	adds r1, r5, #0
	movs r0, #7
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	adds r1, r5, #0
	movs r0, #30
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #7
	bl Object_RefreshSelectorById
	movs r0, #6
	bl Object_RefreshSelectorById
	movs r0, #5
	bl Object_RefreshSelectorById
	movs r0, #30
	bl Object_RefreshSelectorById
	movs r0, #4
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #16
	movs r2, #0
	movs r0, #4
	negs r1, r1
	bl ObjectMotion_CommitPositionAndActivate
	ldr r1, .L_0200ba18
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r0, #4
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r2, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #29
	bl Object_GetById
	adds r0, #85
	strb r6, [r0]
	ldr r1, .L_0200ba1c
	movs r0, #4
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #4
	bl Object_RefreshSelectorById
	movs r1, #56
	movs r2, #128
	lsls r2, r2, #1
	movs r0, #4
	adds r1, #255
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_020058ec
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	movs r0, #2
	bl Func_02003a6c
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	adds r3, #84
	str r3, [r2]
	subs r3, #76
	adds r2, r1, r3
	movs r3, #24
	str r3, [r2]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #1
	bl Func_02005934
	bl Func_0200581c
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ba10:
	.4byte Data_02005ff0
.L_0200ba14:
	.4byte Data_020061e0
.L_0200ba18:
	.4byte Data_02005ff8
.L_0200ba1c:
	.4byte Data_02006088
	.section .text.x0200ba28,"ax",%progbits
	.global Func_02003a28
	.thumb_func
Func_02003a28:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r4, r1, #0
	ldr r1, [r3, #108]
	asrs r2, r4, #20
	movs r3, #64
	adds r5, r0, #0
	subs r4, r3, r2
	movs r0, #0
	adds r1, #20
.L_0200ba3e:
	ldmia r1!, {r2}
	cmp r2, #0
	beq .L_0200ba62
	ldr r3, [r2, #8]
	ldr r2, [r2, #16]
	asrs r3, r3, #20
	subs r3, #4
	asrs r2, r2, #20
	cmp r3, #4
	bhi .L_0200ba62
	adds r3, r4, #0
	adds r3, #8
	cmp r3, r2
	bgt .L_0200ba62
	adds r3, #3
	cmp r2, r3
	bge .L_0200ba62
	stmia r5!, {r0}
.L_0200ba62:
	adds r0, #1
	cmp r0, #63
	bls .L_0200ba3e
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x0200ba6c,"ax",%progbits
	.global Func_02003a6c
	.thumb_func
Func_02003a6c:
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
	movs r1, #188
	movs r2, #200
	lsls r1, r1, #1
	lsls r2, r2, #5
	adds r1, r1, r3
	sub sp, #8
	adds r2, #153
	movs r3, #0
	str r2, [sp, #4]
	str r3, [sp, #0]
	ldr r2, .L_0200bc3c
	ldr r3, [r1, #12]
	mov r8, r0
	mov r11, r1
	str r3, [r2]
	cmp r0, #1
	beq .L_0200baba
	cmp r0, #1
	bgt .L_0200baac
	cmp r0, #0
	beq .L_0200bab6
	b .L_0200bac6
.L_0200baac:
	mov r1, r8
	cmp r1, #2
	beq .L_0200bac0
	cmp r1, #3
	bne .L_0200bac6
.L_0200bab6:
	ldr r2, .L_0200bc40
	b .L_0200bac8
.L_0200baba:
	ldr r3, .L_0200bc44
	mov r9, r3
	b .L_0200baca
.L_0200bac0:
	ldr r1, .L_0200bc48
	mov r9, r1
	b .L_0200baca
.L_0200bac6:
	ldr r2, .L_0200bc4c
.L_0200bac8:
	mov r9, r2
.L_0200baca:
	mov r1, r9
	ldr r3, [r1]
	movs r7, #0
	cmp r3, #64
	beq .L_0200bafc
	mov r5, r9
.L_0200bad6:
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #1
	ldmia r5!, {r0}
	bl ObjectMotion_SetActionVariant
	ldr r2, [sp, #0]
	adds r7, #1
	adds r2, #1
	str r2, [sp, #0]
	cmp r7, #4
	bhi .L_0200bafc
	ldr r3, [r5]
	cmp r3, #64
	bne .L_0200bad6
.L_0200bafc:
	movs r7, #0
.L_0200bafe:
	ldr r6, .L_0200bc50
	lsls r5, r7, #2
	ldr r0, [r6, r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #0
	ldr r0, [r6, r5]
	adds r7, #1
	bl ObjectMotion_SetActionVariant
	cmp r7, #5
	bls .L_0200bafe
	movs r0, #223
	bl Func_020059f4
	movs r7, #0
.L_0200bb28:
	mov r1, r11
	ldr r3, [r1, #12]
	ldr r2, [sp, #4]
	subs r3, r3, r2
	str r3, [r1, #12]
	ldr r1, [sp, #0]
	movs r3, #0
	mov r10, r3
	cmp r10, r1
	bcs .L_0200bb68
	mov r6, r9
.L_0200bb3e:
	ldr r0, [r6]
	bl Object_GetById
	ldr r3, [r0, #16]
	ldr r2, [sp, #4]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	ldmia r6!, {r0}
	bl Object_GetById
	ldr r3, [r0, #16]
	str r3, [r5, #64]
	ldr r1, [sp, #0]
	movs r3, #1
	add r10, r3
	cmp r10, r1
	bcc .L_0200bb3e
.L_0200bb68:
	movs r3, #3
	ands r3, r7
	cmp r3, #3
	bne .L_0200bb7c
	ldr r2, [sp, #4]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r2, r3
	str r2, [sp, #4]
.L_0200bb7c:
	ldr r1, [sp, #4]
	ldr r2, .L_0200bc54
	cmp r1, r2
	ble .L_0200bb8a
	movs r3, #192
	lsls r3, r3, #9
	str r3, [sp, #4]
.L_0200bb8a:
	movs r0, #1
	bl WaitFrames
	mov r1, r8
	cmp r1, #0
	bne .L_0200bba2
	cmp r7, #40
	bne .L_0200bbbe
	movs r0, #128
	movs r1, #208
	movs r2, #152
	b .L_0200bbb2
.L_0200bba2:
	mov r2, r8
	cmp r2, #1
	bne .L_0200bbc4
	cmp r7, #40
	bne .L_0200bbbe
	movs r0, #128
	movs r1, #208
	movs r2, #176
.L_0200bbb2:
	lsls r0, r0, #18
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
.L_0200bbbe:
	cmp r7, #120
	bne .L_0200bbda
	b .L_0200bbe0
.L_0200bbc4:
	mov r3, r8
	cmp r3, #3
	bne .L_0200bbda
	cmp r7, #40
	bne .L_0200bbd6
	movs r0, #0
	movs r1, #1
	bl Object_AttachWorkTargetToObject
.L_0200bbd6:
	cmp r7, #120
	beq .L_0200bbe0
.L_0200bbda:
	adds r7, #1
	cmp r7, #227
	bls .L_0200bb28
.L_0200bbe0:
	movs r7, #0
.L_0200bbe2:
	ldr r6, .L_0200bc50
	lsls r5, r7, #2
	ldr r0, [r6, r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #223
	ands r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r6, r5]
	adds r7, #1
	bl ObjectMotion_SetActionVariant
	cmp r7, #5
	bls .L_0200bbe2
	ldr r1, [sp, #0]
	movs r7, #0
	cmp r7, r1
	bcs .L_0200bc2c
	mov r5, r9
.L_0200bc0e:
	movs r2, #0
	ldr r0, [r5]
	movs r1, #0
	bl Func_02005884
	ldmia r5!, {r0}
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	ldr r2, [sp, #0]
	adds r7, #1
	cmp r7, r2
	bcc .L_0200bc0e
.L_0200bc2c:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200bc3c:
	.4byte gOverlayArea + 0x6a1c
.L_0200bc40:
	.4byte Data_020059fc
.L_0200bc44:
	.4byte Data_02005a0c
.L_0200bc48:
	.4byte Data_02005a14
.L_0200bc4c:
	.4byte Data_02005a1c
.L_0200bc50:
	.4byte Data_020069dc
.L_0200bc54:
	.4byte 0x00017fff
	.section .text.x0200bc58,"ax",%progbits
	.global Func_02003c58
	.thumb_func
Func_02003c58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	str r0, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #188
	lsls r2, r2, #1
	adds r2, r2, r3
	mov r10, r2
	ldr r2, .L_0200bd94
	movs r3, #192
	lsls r3, r3, #9
	mov r9, r3
	ldr r3, [r2]
	movs r4, #0
	mov r11, r4
	mov r8, r4
	cmp r3, #64
	beq .L_0200bcac
	adds r5, r2, #0
.L_0200bc8e:
	ldmia r5!, {r0}
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r3, #1
	add r8, r3
	mov r4, r8
	add r11, r3
	cmp r4, #4
	bhi .L_0200bcac
	ldr r3, [r5]
	cmp r3, #64
	bne .L_0200bc8e
.L_0200bcac:
	ldr r2, [sp, #0]
	cmp r2, #0
	beq .L_0200bd74
	movs r0, #223
	bl Func_020059f4
	movs r3, #0
	mov r4, r10
	mov r8, r3
	ldr r3, [r4, #12]
	movs r2, #192
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r4, #12]
	b .L_0200bd6c
.L_0200bcca:
	movs r7, #0
	cmp r7, r11
	bcs .L_0200bcf8
	ldr r6, .L_0200bd94
.L_0200bcd2:
	ldr r0, [r6]
	bl Object_GetById
	ldr r3, [r0, #16]
	mov r4, r9
	subs r3, r3, r4
	str r3, [r0, #16]
	adds r7, #1
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	ldmia r6!, {r0}
	bl Object_GetById
	ldr r3, [r0, #16]
	str r3, [r5, #64]
	cmp r7, r11
	bcc .L_0200bcd2
.L_0200bcf8:
	movs r3, #3
	mov r2, r8
	ands r3, r2
	cmp r3, #3
	bne .L_0200bd24
	mov r4, r9
	lsls r3, r4, #4
	subs r0, r3, r4
	mov r4, r10
	ldr r2, [r4, #12]
	ldr r1, .L_0200bd98
	lsls r3, r0, #1
	adds r3, r3, r2
	ldr r2, [r1]
	cmp r2, r3
	bge .L_0200bd24
	adds r3, r0, #0
	cmp r3, #0
	bge .L_0200bd20
	adds r3, #15
.L_0200bd20:
	asrs r3, r3, #4
	mov r9, r3
.L_0200bd24:
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #203
	cmp r9, r2
	bgt .L_0200bd36
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #204
	mov r9, r3
.L_0200bd36:
	ldr r4, [sp, #0]
	cmp r4, #1
	bne .L_0200bd54
	mov r2, r8
	cmp r2, #40
	bne .L_0200bd54
	movs r0, #144
	movs r1, #208
	movs r2, #184
	lsls r0, r0, #17
	lsls r1, r1, #15
	lsls r2, r2, #17
	movs r3, #1
	bl Motion_CamBounds
.L_0200bd54:
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	add r8, r3
	mov r4, r8
	cmp r4, #119
	bhi .L_0200bd74
	mov r2, r10
	ldr r3, [r2, #12]
	add r3, r9
	str r3, [r2, #12]
.L_0200bd6c:
	ldr r2, .L_0200bd98
	ldr r2, [r2]
	cmp r3, r2
	blt .L_0200bcca
.L_0200bd74:
	ldr r3, .L_0200bd98
	mov r4, r10
	ldr r3, [r3]
	str r3, [r4, #12]
	bl Func_0200577c
	movs r0, #2
	bl WaitFrames
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bd94:
	.4byte Data_02005a20
.L_0200bd98:
	.4byte gOverlayArea + 0x6a1c
	.section .text.x0200bd9c,"ax",%progbits
	.global Func_02003d9c
	.thumb_func
Func_02003d9c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #129
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	movs r0, #15
	movs r1, #1
	bl Func_020059b4
	movs r1, #1
	movs r0, #16
	bl Func_020059b4
	ldr r3, .L_0200bf10
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #17
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #19
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #20
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #21
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #22
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200be44
	bl Func_02004644
	b .L_0200be4e
.L_0200be44:
	movs r0, #24
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #24]
.L_0200be4e:
	ldr r5, .L_0200bf10
	movs r3, #241
	lsls r3, r3, #1
	adds r6, r5, r3
	ldrh r3, [r6]
	movs r1, #128
	subs r3, #1
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bls .L_0200be6e
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02005884
.L_0200be6e:
	movs r0, #36
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #36
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #144
	ldr r0, .L_0200bf14
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #9
	bne .L_0200beb4
	bl Func_02000cd0
	ldr r2, .L_0200bf18
	movs r1, #152
	lsls r1, r1, #2
	adds r3, r5, r1
	strh r2, [r3]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #98
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
	b .L_0200bf0a
.L_0200beb4:
	cmp r3, #10
	bne .L_0200bec4
	movs r0, #5
	bl Party_AddActiveOwner
	bl Func_02001388
	b .L_0200bf0a
.L_0200bec4:
	cmp r3, #11
	bne .L_0200bed4
	movs r0, #6
	bl Party_AddActiveOwner
	bl Func_020016e8
	b .L_0200bf0a
.L_0200bed4:
	cmp r3, #12
	bne .L_0200bede
	bl Func_02002ae4
	b .L_0200bf0a
.L_0200bede:
	cmp r3, #13
	bne .L_0200bee8
	bl Func_02001bc4
	b .L_0200bf0a
.L_0200bee8:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #33
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bf0a
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02005884
	movs r0, #28
	movs r1, #0
	movs r2, #0
	bl Func_02005884
.L_0200bf0a:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200bf10:
	.4byte gPartyState
.L_0200bf14:
	.4byte Func_020005b8
.L_0200bf18:
	.4byte 0x000000fb
	.section .text.x0200bf52,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200bf54,"ax",%progbits
	.global Func_02003f54
	.thumb_func
Func_02003f54:
	ldr r3, .L_0200bf5c
	str r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200bf5c:
	.4byte Data_020069f8
	.section .text.x0200bf60,"ax",%progbits
	.global Func_02003f60
	.thumb_func
Func_02003f60:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200c00c
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_0200bf72
	adds r0, #3
.L_0200bf72:
	asrs r0, r0, #2
	movs r1, #5
	bl Engine_MathRemainder
	ldr r3, .L_0200c010
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200bfc6
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_0200bfa6
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_0200c002
.L_0200bfa6:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200c002
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200c002
.L_0200bfc6:
	movs r5, #0
	movs r6, #4
.L_0200bfca:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl Engine_MathRemainder
	ldr r3, .L_0200c014
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_0200bfca
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_0200c018
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_0200c00c
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200c002:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c00c:
	.4byte Data_020069f4
.L_0200c010:
	.4byte Data_020069f8
.L_0200c014:
	.4byte gOverlayArea + 0x6a2c
.L_0200c018:
	.4byte 0x05000184
	.section .text.x0200c01c,"ax",%progbits
	.global Func_0200401c
	.thumb_func
Func_0200401c:
	push {r5, r6, lr}
	ldr r2, .L_0200c078
	movs r3, #1
	adds r6, r0, #0
	str r3, [r2]
	cmp r6, #2
	beq .L_0200c040
	ldr r1, .L_0200c07c
	movs r2, #32
	ldr r0, .L_0200c080
	ldr r5, .L_0200c084
	mov lr, r5
	.2byte 0xf800
	ldr r0, .L_0200c088
	ldr r1, .L_0200c08c
	movs r2, #32
	mov lr, r5
	.2byte 0xf800
.L_0200c040:
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c050
	cmp r6, #1
	bne .L_0200c062
.L_0200c050:
	ldr r3, .L_0200c090
	movs r2, #0
	movs r1, #144
	str r2, [r3]
	ldr r0, .L_0200c094
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	b .L_0200c076
.L_0200c062:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_0200c098
	ldr r1, .L_0200c09c
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_0200c076:
	pop {r5, r6, pc}
.L_0200c078:
	.4byte Data_020069f8
.L_0200c07c:
	.4byte 0x05000180
.L_0200c080:
	.4byte gOverlayArea + 0x6a2c
.L_0200c084:
	.4byte IwramCopyWords
.L_0200c088:
	.4byte gOverlayArea + 0x6a4c
.L_0200c08c:
	.4byte 0x050001a0
.L_0200c090:
	.4byte Data_020069f4
.L_0200c094:
	.4byte Func_02003f60
.L_0200c098:
	.4byte gOverlayArea + 0x6a50
.L_0200c09c:
	.4byte 0x05000184
	.section .text.x0200c0a0,"ax",%progbits
	.global Func_020040a0
	.thumb_func
Func_020040a0:
	push {r5, lr}
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #85
	movs r3, #4
	strb r3, [r1]
	movs r2, #0
	ldr r3, [r5, #20]
	str r2, [r5, #68]
	movs r2, #128
	lsls r2, r2, #14
	adds r3, r3, r2
	str r3, [r5, #12]
	subs r1, #50
	ldrb r2, [r1]
	movs r3, #128
	orrs r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	ldrb r0, [r3]
	bl Map_GetTerrainHeight
	adds r3, r0, #0
	asrs r3, r3, #19
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	adds r3, #6
	movs r2, #0
	bl Func_020057c4
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_020059ec
	pop {r5, pc}
	.section .text.x0200c0f4,"ax",%progbits
	.global Func_020040f4
	.thumb_func
Func_020040f4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200c200
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200c194
.L_0200c14e:
	ldr r3, [r7, #8]
	ldr r2, .L_0200c204
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_0200c16a
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_0200c16a:
	ldr r3, .L_0200c208
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_0200c14e
.L_0200c194:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_0200c1fc
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_0200c20c
.L_0200c1fc:
	.4byte 0x00000001
.L_0200c200:
	.4byte gPartyState
.L_0200c204:
	.4byte 0x0003ffff
.L_0200c208:
	.4byte Data_0300122c
.L_0200c20c:
	bl Motion_CamBounds
	bl Func_0200592c
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_0200c254
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02005784
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_0200c272
	b .L_0200c258
	.2byte 0x0000
.L_0200c254:
	.4byte 0x00000000
.L_0200c258:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200c272
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_0200c258
.L_0200c272:
	movs r0, #127
	bl Func_020059f4
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_0200c292
.L_0200c280:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200c292
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_0200c280
.L_0200c292:
	adds r0, r7, #0
	bl Func_0200578c
	ldr r5, .L_0200c2dc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Object_AttachWorkTargetToObject
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_0200581c
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200c2dc:
	.4byte gPartyState
	.section .text.x0200c2e0,"ax",%progbits
	.global Func_020042e0
	.thumb_func
Func_020042e0:
	push {lr}
	ldr r3, [r1]
	ldr r4, [r0]
	ldr r2, [r1, #8]
	subs r4, r4, r3
	ldr r3, [r0, #8]
	asrs r4, r4, #16
	subs r3, r3, r2
	asrs r3, r3, #16
	adds r2, r3, #0
	muls r2, r3
	adds r0, r4, #0
	muls r0, r4
	adds r3, r2, #0
	adds r0, r0, r3
	ldr r3, .L_0200c308
	mov lr, r3
	.2byte 0xf800
	pop {pc}
	.2byte 0x0000
.L_0200c308:
	.4byte IwramFillWords + 0x74
	.section .text.x0200c30c,"ax",%progbits
	.global Func_0200430c
	.thumb_func
Func_0200430c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200c374
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_0200c36a
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200c36a
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_0200c36a
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200c378
.L_0200c36a:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_0200c4b6
.L_0200c374:
	.4byte gPartyState
.L_0200c378:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_0200c398
	movs r0, #231
	bl Func_020059f4
.L_0200c398:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_020059e4
	cmp r0, #255
	beq .L_0200c49a
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02005994
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_0200c49a
	ldr r2, .L_0200c488
	cmp r5, r2
	blt .L_0200c49a
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0200c460
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_0200c3f8
	subs r5, r3, r2
.L_0200c3f8:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_020042e0
	cmp r0, #12
	bgt .L_0200c418
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_0200c418
	movs r2, #1
	mov r8, r2
.L_0200c418:
	mov r3, r8
	cmp r3, #0
	beq .L_0200c460
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c460
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_0200c48c
	movs r2, #128
	ldr r0, .L_0200c484
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_0200c460:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_0200c490
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_0200c494
.L_0200c484:
	.4byte 0x00000000
.L_0200c488:
	.4byte 0xffe00000
.L_0200c48c:
	.4byte gPartyState
.L_0200c490:
	.4byte IwramMulQ16
.L_0200c494:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_0200c4b6
.L_0200c49a:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_0200c4c4
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02005764
	movs r0, #228
	bl Func_020059f4
	ldr r3, .L_0200c4c8
	str r5, [r3]
.L_0200c4b6:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c4c4:
	.4byte Data_020069fc
.L_0200c4c8:
	.4byte gOverlayArea + 0x6a28
	.section .text.x0200c4cc,"ax",%progbits
	.global Func_020044cc
	.thumb_func
Func_020044cc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_020059f4
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_0200c580
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_0200576c
	movs r1, #2
	adds r7, r0, #0
	bl Func_02005754
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_0200c57c
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_0200c584
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_0200c588
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_0200c58c
.L_0200c57c:
	.4byte 0x00000000
.L_0200c580:
	.4byte IwramMulQ16
.L_0200c584:
	.4byte Func_0200430c
.L_0200c588:
	.4byte 0xfffa0000
.L_0200c58c:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02004bd4
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c5a4,"ax",%progbits
	.global Func_020045a4
	.thumb_func
Func_020045a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c640
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_0200c632
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl Engine_MathDivide
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02004bd4
.L_0200c632:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c640:
	.4byte Data_0300122c
	.section .text.x0200c644,"ax",%progbits
	.global Func_02004644
	.thumb_func
Func_02004644:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005884
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02005884
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02005884
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02005884
	movs r0, #24
	bl Object_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c720
	ldr r3, [r6, #12]
	ldr r2, .L_0200c7b8
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_0200c7bc
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200c7c0
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_0200c720:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c76a
	ldr r3, [r7, #12]
	ldr r2, .L_0200c7b8
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_0200c7bc
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_0200c7c4
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_0200c7c8
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_0200c76a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c7aa
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c7aa
	ldr r3, [r6, #12]
	ldr r2, .L_0200c7cc
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_0200c7d0
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_0200c7aa:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c7b8:
	.4byte 0x00066640
.L_0200c7bc:
	.4byte 0x0001eb80
.L_0200c7c0:
	.4byte 0xfffd70c0
.L_0200c7c4:
	.4byte 0x00028f40
.L_0200c7c8:
	.4byte 0xfffff800
.L_0200c7cc:
	.4byte 0x00199900
.L_0200c7d0:
	.4byte 0x001b8480
	.section .text.x0200c7d4,"ax",%progbits
	.global Func_020047d4
	.thumb_func
Func_020047d4:
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
	ldr r3, .L_0200c968
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_0200c96c
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_0200c970
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_0200c974
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_0200c978
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_0200c826
	b .L_0200c95a
.L_0200c826:
	ldr r2, .L_0200c97c
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_0200c834
	b .L_0200c94a
.L_0200c834:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_0200c83c
	b .L_0200c94a
.L_0200c83c:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_0200c980
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_0200c8b2
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_0200c94a
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_0200c94a
	cmp r4, #239
	bgt .L_0200c94a
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200c984
	b .L_0200c8ee
.L_0200c8b2:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_0200c94a
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_0200c94a
	cmp r4, #175
	bgt .L_0200c94a
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_0200c988
.L_0200c8ee:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_0200c98c
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_0200c92c
	adds r0, r5, #0
	bl Func_020059bc
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_0200c940
.L_0200c92c:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_0200c940:
	adds r0, r6, #0
	mov r1, r11
	bl Func_0200571c
	adds r6, #12
.L_0200c94a:
	ldr r3, .L_0200c978
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_0200c95a
	b .L_0200c826
.L_0200c95a:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c968:
	.4byte 0xffff0000
.L_0200c96c:
	.4byte gOverlayArea + 0x6a6c
.L_0200c970:
	.4byte ResourceTableEntries
.L_0200c974:
	.4byte gOverlayArea + 0x6ab0
.L_0200c978:
	.4byte gOverlayArea + 0x6a6e
.L_0200c97c:
	.4byte gOverlayArea + 0x6a70
.L_0200c980:
	.4byte gOverlayArea + 0x6b70
.L_0200c984:
	.4byte 0x40002000
.L_0200c988:
	.4byte 0xc000a000
.L_0200c98c:
	.4byte gOverlayArea + 0x6b72
	.section .text.x0200c990,"ax",%progbits
	.global Func_02004990
	.thumb_func
Func_02004990:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200c9f0
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_0200c9f4
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_0200c9f8
	bl Func_02005704
	ldr r5, .L_0200c9fc
	bl Resource_FindFreeEntry
	movs r1, #192
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ca00
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200ca04
	ldr r2, .L_0200c9e8
	strh r2, [r3]
	ldr r3, .L_0200ca08
	strh r2, [r3]
	ldr r2, .L_0200ca0c
	ldr r3, .L_0200c9ec
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200c9e8:
	.4byte 0x00000000
.L_0200c9ec:
	.4byte 0xffffffff
.L_0200c9f0:
	.4byte IwramClearWords
.L_0200c9f4:
	.4byte gOverlayArea + 0x6a70
.L_0200c9f8:
	.4byte Data_02005a24
.L_0200c9fc:
	.4byte gOverlayArea + 0x6a6c
.L_0200ca00:
	.4byte Func_020047d4
.L_0200ca04:
	.4byte gOverlayArea + 0x6a6e
.L_0200ca08:
	.4byte gOverlayArea + 0x6b70
.L_0200ca0c:
	.4byte gOverlayArea + 0x6b72
	.section .text.x0200ca10,"ax",%progbits
	.global Func_02004a10
	.thumb_func
Func_02004a10:
	push {r5, r6, lr}
	movs r0, #192
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200ca70
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_0200ca74
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_0200ca78
	bl Func_02005704
	ldr r5, .L_0200ca7c
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ca80
	bl Scheduler_AddOrUpdateCallback
	ldr r3, .L_0200ca84
	ldr r2, .L_0200ca68
	strh r2, [r3]
	ldr r3, .L_0200ca88
	strh r2, [r3]
	ldr r2, .L_0200ca8c
	ldr r3, .L_0200ca6c
	strh r3, [r2]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200ca68:
	.4byte 0x00000000
.L_0200ca6c:
	.4byte 0xffffffff
.L_0200ca70:
	.4byte IwramClearWords
.L_0200ca74:
	.4byte gOverlayArea + 0x6a70
.L_0200ca78:
	.4byte Data_02005b86 + 0x1
.L_0200ca7c:
	.4byte gOverlayArea + 0x6a6c
.L_0200ca80:
	.4byte Func_020047d4
.L_0200ca84:
	.4byte gOverlayArea + 0x6a6e
.L_0200ca88:
	.4byte gOverlayArea + 0x6b70
.L_0200ca8c:
	.4byte gOverlayArea + 0x6b72
	.section .text.x0200ca90,"ax",%progbits
	.global Func_02004a90
	.thumb_func
Func_02004a90:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #4
	bl Runtime_BumpAllocate
	ldr r3, .L_0200caf4
	adds r6, r0, #0
	movs r1, #64
	ldr r0, .L_0200caf8
	mov lr, r3
	.2byte 0xf800
	adds r1, r6, #0
	ldr r0, .L_0200cafc
	bl Func_02005704
	ldr r5, .L_0200cb00
	bl Resource_FindFreeEntry
	movs r1, #128
	strh r0, [r5]
	lsls r0, r0, #16
	adds r2, r6, #0
	lsls r1, r1, #4
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200cb04
	bl Scheduler_AddOrUpdateCallback
	ldr r2, .L_0200cb08
	ldr r3, .L_0200cae8
	strh r3, [r2]
	ldr r2, .L_0200cb0c
	ldr r3, .L_0200caec
	strh r3, [r2]
	ldr r2, .L_0200cb10
	ldr r3, .L_0200caf0
	strh r3, [r2]
	b .L_0200cb14
.L_0200cae8:
	.4byte 0x00000000
.L_0200caec:
	.4byte 0x00000001
.L_0200caf0:
	.4byte 0xffffffff
.L_0200caf4:
	.4byte IwramClearWords
.L_0200caf8:
	.4byte gOverlayArea + 0x6a70
.L_0200cafc:
	.4byte Data_02005db6
.L_0200cb00:
	.4byte gOverlayArea + 0x6a6c
.L_0200cb04:
	.4byte Func_020047d4
.L_0200cb08:
	.4byte gOverlayArea + 0x6a6e
.L_0200cb0c:
	.4byte gOverlayArea + 0x6b70
.L_0200cb10:
	.4byte gOverlayArea + 0x6b72
.L_0200cb14:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200cb18,"ax",%progbits
	.global Func_02004b18
	.thumb_func
Func_02004b18:
	push {r5, lr}
	adds r5, r1, #0
	bl Object_GetById
	adds r4, r0, #0
	cmp r4, #0
	beq .L_0200cb3e
	adds r3, r4, #0
	adds r3, #100
	strh r5, [r3]
	ldr r1, .L_0200cb40
	ldr r0, .L_0200cb44
	ldrh r2, [r1]
	movs r5, #0
	ldrsh r3, [r1, r5]
	adds r2, #1
	lsls r3, r3, #2
	str r4, [r0, r3]
	strh r2, [r1]
.L_0200cb3e:
	pop {r5, pc}
.L_0200cb40:
	.4byte gOverlayArea + 0x6a6e
.L_0200cb44:
	.4byte gOverlayArea + 0x6a70
	.section .text.x0200cb48,"ax",%progbits
	.global Func_02004b48
	.thumb_func
Func_02004b48:
	ldr r3, .L_0200cb50
	strh r0, [r3]
	bx lr
	.2byte 0x0000
.L_0200cb50:
	.4byte gOverlayArea + 0x6b72
	.section .text.x0200cb9a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200cb9c,"ax",%progbits
	.global Func_02004b9c
	.thumb_func
Func_02004b9c:
	ldr r3, [r0, #8]
	ldr r2, [r0, #68]
	ldr r1, [r0, #80]
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, [r0, #72]
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	ldr r2, [r0, #76]
	ldr r3, [r0, #16]
	adds r3, r3, r2
	str r3, [r0, #16]
	ldr r2, [r0, #48]
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r2, [r0, #52]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
	adds r0, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r0]
	adds r3, r3, r2
	strh r3, [r1, #18]
	bx lr
	.2byte 0x0000
	.section .text.x0200cbd4,"ax",%progbits
	.global Func_02004bd4
	.thumb_func
Func_02004bd4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200cd8c
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
	beq .L_0200cc1c
	cmp r7, #0
	beq .L_0200cc1c
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_0200cc24
.L_0200cc1c:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_0200cc24:
	mov r3, r10
	bl Func_0200576c
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200cc32
	b .L_0200cd7e
.L_0200cc32:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02005754
	ldr r2, .L_0200cd90
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02005764
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	ldr r3, .L_0200cd94
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
	ldr r3, .L_0200cd98
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200cd7e
	cmp r7, #0
	beq .L_0200cd7e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200ccb4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200ccb4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200ccd4
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200ccd4:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200cce8
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200cce8:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200cd2e
	ldr r3, .L_0200cd90
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200cd16
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200cd28
.L_0200cd16:
	ldr r2, .L_0200cd98
	adds r0, r3, r2
	bl Engine_MathDivide
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200cd98
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200cd28:
	bl Engine_MathDivide
	str r0, [r6, #52]
.L_0200cd2e:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200cd4a
	adds r0, r6, #0
	movs r1, #1
	bl Func_02005754
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02005764
.L_0200cd4a:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200cd5c
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200cd5c:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200cd6e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200cd6e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200cd7e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200cd7e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200cd8c:
	.4byte gPartyState
.L_0200cd90:
	.4byte Data_0200ea10
.L_0200cd94:
	.4byte Func_02004b9c
.L_0200cd98:
	.4byte 0xffff0000
	.section .text.x0200cd9c,"ax",%progbits
	.global Func_02004d9c
	.thumb_func
Func_02004d9c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200ceb4
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200cea8
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200ceb8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_0200cde8
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200cdf0
.L_0200cde8:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200cdf0:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200cebc
	cmp r3, r2
	beq .L_0200cea8
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200cea8
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_0200cec0
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_0200cea8
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200cea8
	cmp r2, #239
	bgt .L_0200cea8
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200cec4
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_0200571c
.L_0200cea8:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200ceb4:
	.4byte gOverlayArea + 0x6b74
.L_0200ceb8:
	.4byte gPartyState
.L_0200cebc:
	.4byte 0xffff0000
.L_0200cec0:
	.4byte ResourceTableEntries
.L_0200cec4:
	.4byte 0x80008800
	.section .text.x0200cec8,"ax",%progbits
	.global Func_02004ec8
	.thumb_func
Func_02004ec8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200d0f8
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200d0fc
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200d04c
.L_0200cf7c:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200d040
.L_0200cf90:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200d030
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200d030
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200d030
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200cfe4
	cmp r5, r10
	bne .L_0200d022
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200d022
.L_0200cfe4:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d022
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_0200579c
.L_0200d022:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200d030:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200cf90
.L_0200d040:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200cf7c
.L_0200d04c:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200d0a4
	ldr r3, .L_0200d100
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200d0a4
.L_0200d07e:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200d094
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200d094
	mov r0, r8
	strh r2, [r0, #12]
.L_0200d094:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200d07e
.L_0200d0a4:
	movs r0, #128
	lsls r0, r0, #1
	bl Runtime_BumpAllocate
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200d0b2:
	ldr r3, .L_0200d104
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200d0b2
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl Sys_Free
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200d108
	bl Scheduler_AddOrUpdateCallback
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d0f8:
	.4byte gOverlayArea + 0x6b74
.L_0200d0fc:
	.4byte IwramClearWords
.L_0200d100:
	.4byte gPartyState
.L_0200d104:
	.4byte 0x11111111
.L_0200d108:
	.4byte Func_02004d9c
	.section .text.x0200d10c,"ax",%progbits
	.global Func_0200510c
	.thumb_func
Func_0200510c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_0200d18c
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_0200592c
	ldr r2, .L_0200d190
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200d18c:
	.4byte gPartyState
.L_0200d190:
	.4byte 0xfff80000
	.section .text.x0200d194,"ax",%progbits
	.global Func_02005194
	.thumb_func
Func_02005194:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200d200
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_0200510c
	movs r0, #161
	bl Func_020059f4
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200579c
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200d200:
	.4byte gPartyState
	.section .text.x0200d204,"ax",%progbits
	.global Func_02005204
	.thumb_func
Func_02005204:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200d2b4
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_0200510c
	movs r0, #229
	bl Func_020059f4
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_0200579c
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200d2ac
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200d2b0
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200d2b8
	.2byte 0x0000
.L_0200d2ac:
	.4byte 0x00000000
.L_0200d2b0:
	.4byte 0x00008000
.L_0200d2b4:
	.4byte gPartyState
.L_0200d2b8:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200d2cc:
	cmp r7, #5
	bne .L_0200d2d6
	movs r0, #204
	bl Func_020059f4
.L_0200d2d6:
	ldr r3, [r6, #24]
	ldr r1, .L_0200d334
	ldr r2, .L_0200d338
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200d33c
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200d2cc
	ldr r3, .L_0200d340
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200d334:
	.4byte 0xfffffc00
.L_0200d338:
	.4byte 0xfffffd00
.L_0200d33c:
	.4byte 0xffff6667
.L_0200d340:
	.4byte gPartyState
	.section .text.x0200d344,"ax",%progbits
	.global Func_02005344
	.thumb_func
Func_02005344:
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
	bl Engine_MathDivide
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200d374
	adds r3, #15
.L_0200d374:
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
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200d39c,"ax",%progbits
	.global Func_0200539c
	.thumb_func
Func_0200539c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200d520
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02005814
	movs r0, #0
	bl Func_02005984
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_0200577c
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #204
	bl Func_020059f4
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200d524
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200d436:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200d528
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200d52c
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200d530
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02004bd4
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200d436
	movs r0, #188
	bl Func_020059f4
	ldr r5, .L_0200d520
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02005904
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_020057ac
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_020057ac
	bl Func_020057b4
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02005904
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_0200581c
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200d520:
	.4byte gPartyState
.L_0200d524:
	.4byte Func_02005344
.L_0200d528:
	.4byte 0xffffa000
.L_0200d52c:
	.4byte 0xffffd000
.L_0200d530:
	.4byte 0x01090001
	.section .text.x0200d534,"ax",%progbits
	.global Func_02005534
	.thumb_func
Func_02005534:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200d5dc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200d5e0
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200d5d0
.L_0200d568:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200d5c4
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200d5c4
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200d598
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02005194
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200d5d0
.L_0200d598:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200d5d0
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02005204
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200d5d2
.L_0200d5c4:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200d568
.L_0200d5d0:
	movs r0, #0
.L_0200d5d2:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200d5dc:
	.4byte gPartyState
.L_0200d5e0:
	.4byte gOverlayArea + 0x6b74
	.section .text.x0200d5e4,"ax",%progbits
	.global Func_020055e4
	.thumb_func
Func_020055e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200d694
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200d698
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200d632
	cmp r0, #0
	beq .L_0200d686
.L_0200d632:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_0200577c
	bl Func_0200539c
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200d686:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200d694:
	.4byte gPartyState
.L_0200d698:
	.4byte gOverlayArea + 0x6b74
	.section .rodata.x0200d9fc,"a",%progbits
	.global Data_020059fc
Data_020059fc:
	.4byte 0x0000001d
	.4byte 0x0000001b
	.4byte 0x0000001c
	.4byte 0x00000040
	.global Data_02005a0c
Data_02005a0c:
	.4byte 0x00000000
	.4byte 0x00000040
	.global Data_02005a14
Data_02005a14:
	.4byte 0x00000004
	.4byte 0x00000040
	.global Data_02005a1c
Data_02005a1c:
	.4byte 0x00000040
	.global Data_02005a20
Data_02005a20:
	.4byte 0x00000040
	.global Data_02005a24
Data_02005a24:
	.4byte 0x06345d01
	.4byte Runtime_ReciprocalTable + 0x1259
	.4byte 0x2f010026
	.4byte 0x5f1f7000
	.4byte 0x667b0906
	.4byte 0x01040901
	.4byte 0x0e287800
	.4byte 0x56053b3f
	.4byte 0x5f6f080e
	.4byte 0x08100800
	.4byte 0xa1076601
	.4byte 0x001d0800
	.4byte Text_MessageContexts + 0x10b38
	.4byte 0x660015bb
	.4byte 0x0e020016
	.4byte 0x66177000
	.4byte 0x3b02080d
	.4byte Data_02001024 + 0xbb
	.4byte 0x66128010
	.4byte 0x01037a01
	.4byte 0x04277910
	.4byte 0xfb44013d
	.4byte Data_0200752c + 0x2d4
	.4byte 0x20590414
	.4byte 0x57052700
	.4byte 0x50002066
	.4byte 0x20ff0016
	.4byte 0x00162a00
	.4byte 0x0016002e
	.4byte 0x162a0020
	.4byte 0x4e002000
	.4byte 0x7f320040
	.4byte 0x04aa023b
	.4byte 0x02500006
	.4byte 0xe7169207
	.4byte 0x0010e103
	.4byte 0x0301ff0d
	.4byte 0x0c01e709
	.4byte 0x5906dd03
	.4byte 0x210a6118
	.4byte 0xfd340030
	.4byte 0x0403a425
	.4byte 0x5f290630
	.4byte 0x02490100
	.4byte 0xf8063b58
	.4byte 0x3b5704af
	.4byte 0x163bb406
	.4byte Resource_DecodeHalfwordLzCode + 0x12
	.4byte 0x283ea00f
	.4byte 0x070820ff
	.4byte 0x2e050010
	.4byte 0x057807f8
	.4byte 0x0a980488
	.4byte 0xdd660ba8
	.4byte 0x07080030
	.4byte 0x38205f22
	.4byte 0x33481706
	.4byte 0x00603b58
	.4byte 0x783ffe78
	.4byte Text_MessageContexts + 0x15ce8
	.4byte 0x1f017900
	.4byte 0x0a782700
	.4byte 0x2aff5f11
	.4byte 0x701f07a8
	.4byte 0xef273000
	.4byte 0x00505704
	.4byte 0x4c002037
	.4byte 0xf7000040
	.4byte 0x0040002a
	.4byte 0x1100206e
	.4byte 0x403b0046
	.4byte 0x00200000
	.4byte 0x2a00402c
	.4byte 0x244819f7
	.4byte 0x2d004000
	.4byte 0x043b0803
	.4byte 0x05830722
	.4byte 0x1817b705
	.4byte 0x0028003b
	.4byte 0x66015920
	.4byte 0x00072260
	.4byte 0x6805041a
	.4byte 0x3f1b01ff
	.4byte 0x06150458
	.4byte 0x40182a2c
	.4byte 0x8f000900
	.4byte 0xe0130302
	.4byte 0x0030cd0c
	.2byte 0x0002
	.global Data_02005b86
Data_02005b86:
	.2byte 0x0000
	.4byte 0xac862b05
	.4byte 0xaf643138
	.4byte 0x426905df
	.4byte 0xabd8ac4e
	.4byte 0xbe810f20
	.4byte 0x7015130e
	.4byte 0x58aaf4f3
	.4byte 0x1c57fed1
	.4byte 0xba72f5fa
	.4byte 0x15f3c78f
	.4byte 0x2c4bddfc
	.4byte 0x8c405c2e
	.4byte 0x5a901050
	.4byte 0x669c3d02
	.4byte 0x813f657c
	.4byte 0xc48af4a0
	.4byte 0x70f27d1e
	.4byte 0xdfcf8f1f
	.4byte 0x0be667d7
	.4byte 0x54be3c30
	.4byte 0xa442f8f4
	.4byte 0xeb17cfc7
	.4byte 0x058022f4
	.4byte 0xc3a1d0c0
	.4byte 0xf05c0a2b
	.4byte 0xbcc4f23e
	.4byte 0xe8ae7760
	.4byte 0x38745730
	.4byte 0xe6259c57
	.4byte 0x0e73008e
	.4byte 0x075f2408
	.4byte 0x1297a050
	.4byte 0x8f028687
	.4byte 0xebc03222
	.4byte 0x3e706883
	.4byte 0x8034e810
	.4byte 0x161c479c
	.4byte 0x9c090038
	.4byte 0x0a75ee0f
	.4byte 0xbf6039f8
	.4byte 0xf613af4e
	.4byte Tileset_Set112TilesA + 0x21e
	.4byte 0x3df8973f
	.4byte 0xf81f363e
	.4byte 0x63e27cd8
	.4byte 0x9df381f3
	.4byte 0xf7e793f3
	.4byte 0xd0e1c9fa
	.4byte 0xf258ce1d
	.4byte 0x3873c780
	.4byte 0xb93be3cc
	.4byte 0x2f3be3cc
	.4byte 0x6118fcc1
	.4byte 0x980e30ca
	.4byte 0x7cef9c33
	.4byte 0x29f1be0c
	.4byte 0xc7c786f0
	.4byte 0x5edecbe6
	.4byte 0x9239f837
	.4byte 0xebc2b9d8
	.4byte 0xd2f88ad8
	.4byte 0xa9bcc731
	.4byte 0x71e99cc7
	.4byte 0x5f547988
	.4byte 0x27049ac1
	.4byte 0xd7ae264f
	.4byte 0xc532a694
	.4byte 0x12fa7ae5
	.4byte 0xc43ef833
	.4byte 0x7d1e54f6
	.4byte 0x97fdb2f9
	.4byte 0xc6a7c3dd
	.4byte 0x3415957d
	.4byte 0x1f1897ad
	.4byte 0xac141210
	.4byte 0x047b4ae7
	.4byte 0x9eb43c03
	.4byte 0xcd180907
	.4byte 0x000079ea
	.4byte 0xf6450423
	.4byte 0xa7d80d99
	.4byte 0x78059b49
	.4byte 0x467a0580
	.4byte 0xb85c205b
	.4byte 0xd050008c
	.4byte 0x500b31ab
	.4byte 0x7833d893
	.4byte 0x8307f302
	.4byte 0x3a9c0502
	.4byte 0x3831102c
	.4byte 0x306a301c
	.4byte 0x3831101d
	.4byte 0x3026301c
	.4byte 0x38f2201a
	.4byte 0x0f2f2e80
	.4byte 0xc0861624
	.4byte 0x1f1ec23c
	.4byte 0x04f95397
	.4byte 0xc067c07c
	.4byte 0xc701f244
	.4byte 0x644c7e42
	.4byte 0x28ae605c
	.4byte 0xa2b8f289
	.4byte Battle_PurpleCaveBackdrop + 0x3607
	.4byte 0xb7fbd5ce
	.4byte 0x8db92233
	.4byte 0xa5310573
	.4byte 0x8e792a73
	.4byte 0x80804174
	.4byte 0x783318f3
	.4byte 0x3905033e
	.4byte 0x7c0bd78f
	.4byte 0x71e9ce5c
	.4byte 0xebc2b80d
	.4byte 0xfe12e605
	.4byte 0x385d04be
	.4byte 0xc06a9047
	.4byte 0x83a09e03
	.4byte 0x1efc23d7
	.4byte 0xf02b869f
	.4byte 0x3002f9f9
	.4byte 0x3f0df1e0
	.4byte 0x793f904f
	.4byte 0xc78f423c
	.4byte 0x3e067c3b
	.4byte 0x3e29ebe7
	.4byte 0xf7f39f16
	.4byte 0x33967f4d
	.4byte 0x780c9067
	.4byte 0x6ce3c57c
	.4byte 0x37ce61c6
	.4byte 0x37cc0e8c
	.4byte 0xcfc6d8fc
	.4byte 0xfcf83ef7
	.4byte 0xe07f9f03
	.4byte 0xfe7c0ff3
	.4byte 0xf03fcf81
	.4byte 0xff3e07f9
	.4byte 0xf816c7c0
	.2byte 0x0000
	.global Data_02005db6
Data_02005db6:
	.2byte 0x0100
	.4byte 0xcf81fe7c
	.4byte 0x07f9f03f
	.4byte 0x6080ff3e
	.4byte 0xead9f7b4
	.4byte 0x1e7cf8d3
	.4byte 0xca9f3dc1
	.4byte 0x8bf7f3e3
	.4byte 0x5b2fc50f
	.4byte 0xf00e1c7d
	.4byte 0xe7c5f8b1
	.4byte 0x4a1f1bef
	.4byte 0x8bf0f3e0
	.4byte 0x3e3dc58f
	.4byte 0xa1f0df7f
	.4byte 0x0a7dbf05
	.4byte 0xf8d3e16e
	.4byte 0x829f7b46
	.4byte 0xe7b8e75e
	.4byte 0x828df1f3
	.4byte 0x5ad8295e
	.4byte 0x3e060bd0
	.4byte 0x49a8e07d
	.4byte 0x6264f270
	.4byte 0x32a6829c
	.4byte 0x127b65c5
	.4byte 0xf6fcf033
	.4byte 0x54f6c43c
	.4byte 0xb2f97d1e
	.4byte 0xc3dd97fd
	.4byte 0x957dc6a7
	.4byte 0x8914e015
	.4byte 0x012101f1
	.4byte 0x166b2b8f
	.4byte 0xf00c11e4
	.4byte 0x241e7ad0
	.4byte 0xe7ab3460
	.4byte 0x108c0001
	.4byte 0x3667d914
	.4byte 0xde0f9f60
	.4byte 0x1b4b2a37
	.4byte 0x2c23c02c
	.4byte 0x02da33d0
	.4byte 0x0465c6e1
	.4byte 0x8d5e8282
	.4byte 0xc49a8459
	.4byte 0x9813c19e
	.4byte 0x2854187f
	.4byte 0x8161d4e0
	.4byte 0x80e1c388
	.4byte 0x80e98751
	.4byte 0x80e1c388
	.4byte 0x00d18131
	.4byte 0x7401c791
	.4byte 0xb1207979
	.4byte 0x7c37dd8f
	.4byte 0x00000000
.L_0200de98:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200ded4:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
.L_0200df10:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000011
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0xffffee00
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000000
	.4byte 0x00000005
	.4byte 0x00000017
	.4byte 0x00000003
	.4byte 0x00001200
	.4byte 0x00000011
	.global Data_02005ff0
Data_02005ff0:
	.4byte 0x00000001
	.4byte 0x00000026
	.global Data_02005ff8
Data_02005ff8:
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00010000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x01e00000
	.4byte 0x00480000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01c80000
	.4byte 0x00480000
	.4byte 0x01680000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01b80000
	.4byte 0x00480000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01980000
	.4byte 0x00480000
	.4byte 0x01000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x01990000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006088
Data_02006088:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00038000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000004
	.4byte 0x01990000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x01780000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01580000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x01380000
	.4byte 0x00200000
	.4byte 0x01160000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020061e0
Data_020061e0:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000f
	.4byte 0x00038000
	.4byte 0x00000016
	.4byte 0x00000010
	.4byte 0x00010000
	.4byte 0x00000004
	.4byte 0x02680000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000004
	.4byte 0x02880000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02a90000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00200000
	.4byte 0x01380000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x00000004
	.4byte 0x02c80000
	.4byte 0x00200000
	.4byte 0x01080000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x0000000d
	.4byte 0x00050000
	.4byte 0x0000002e
	.4byte Func_02000038
	.4byte 0x00000001
	.4byte 0x0000002e
	.4byte Func_02000050
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02006338
Data_02006338:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000008c
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc80
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00020000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_0200641c
Data_0200641c:
.L_0200e41c:
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffa00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0010000
	.4byte 0x80020000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000600
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000004
	.4byte 0xc0020000
	.4byte 0x0000000d
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global Data_02006470
Data_02006470:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000109
	.4byte 0x001050fb
	.4byte 0x00248002
	.4byte 0x00b01103
	.4byte 0x00c02103
	.4byte 0x00d03103
	.4byte 0x00e04106
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0000
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01020000
	.4byte 0x0a210153
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00280000
	.4byte 0x01000000
	.4byte 0x00028000
	.4byte 0x0a210153
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00280000
	.4byte 0x01000000
	.4byte 0x01020000
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff0154
	.4byte 0x00000001
	.4byte 0x03700000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01028000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff01ac
	.4byte 0x00000001
	.4byte 0x03880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff0175
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01380000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x01780000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02880000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02a80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff012e
	.4byte 0x00000007
	.4byte 0x02c80000
	.4byte 0x00100000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0x0a2101a9
	.4byte 0x0000000b
	.4byte Data_02000000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00020000
	.4byte 0xffff01a9
	.4byte 0x0000000b
	.4byte Data_02000000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x01020000
	.4byte 0xffff01aa
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff01aa
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0x1a230021
	.4byte 0x00000006
	.4byte Data_02000000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0x1a230017
	.4byte 0x00000006
	.4byte 0x01e00000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff0126
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x1a21012f
	.4byte .L_0200e41c
	.4byte Data_02000000
	.4byte 0x00080000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020068b0
Data_020068b0:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x0000cccc
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000cccc
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000010
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x00000027
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000026
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00040000
	.4byte 0x00000000
	.4byte 0x00000008
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x0000000c
	.4byte 0x00000002
	.4byte GameFlagBytes + 0x24
	.4byte Func_0200045c
	.4byte 0x00000002
	.4byte 0x0a210065
	.4byte Func_0200065c
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x00002ac9
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x00002aca
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00002acb
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00002acc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020069dc
Data_020069dc:
	.4byte 0x00000011
	.4byte 0x00000012
	.4byte 0x00000013
	.4byte 0x00000014
	.4byte 0x00000015
	.4byte 0x00000016
	.global Data_020069f4
Data_020069f4:
	.4byte 0xffffffff
	.global Data_020069f8
Data_020069f8:
	.4byte 0x00000001
	.global Data_020069fc
Data_020069fc:
	.4byte 0x00000027
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x0000000c
	.4byte 0x00000026
	.global Data_0200ea10
Data_0200ea10:
	.4byte .L_0200de98
	.4byte .L_0200ded4
	.4byte .L_0200df10
