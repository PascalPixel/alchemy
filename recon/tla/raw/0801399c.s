.syntax unified
	.thumb
	.global Func_0801399c
	.thumb_func
Func_0801399c:
	push {r5, r6, r7, lr}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #176
	ldrh r1, [r2, #10]
	movs r3, #197
	lsls r3, r3, #8
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	movs r3, #254
	ldrh r1, [r2, #10]
	lsls r3, r3, #7
	adds r3, #255
	ands r3, r1
	strh r3, [r2, #10]
	ldrh r3, [r2, #10]
	ldr r3, .L_08013a68
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_080139d6
	ldr r0, .L_08013a6c
	ldr r1, .L_08013a70
	ldr r5, .L_08013a74
	bl Func_080164e8
	strh r0, [r5]
	bl Func_08016990
.L_080139d6:
	bl SoundDriver_VSyncRefresh
	ldr r3, .L_08013a78
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080139e6
	bl Func_08013de4
.L_080139e6:
	ldr r2, .L_08013a7c
	ldrb r3, [r2]
	cmp r3, #0
	beq .L_08013a26
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_08013a80
	ldrh r3, [r2]
	cmp r3, #0
	beq .L_08013a16
	movs r1, #192
	lsls r1, r1, #18
	adds r2, r3, #0
	movs r4, #132
	movs r3, #128
	ldr r0, [r1, #80]
	lsls r4, r4, #24
	lsls r3, r3, #19
	movs r1, #224
	adds r3, #212
	lsls r1, r1, #19
	adds r2, r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08013a16:
	movs r1, #128
	lsls r1, r1, #19
	ldr r0, .L_08013a84
	adds r1, #16
	ldmia r0!, {r2, r3, r4, r5}
	stmia r1!, {r2, r3, r4, r5}
	bl Func_08013d84
.L_08013a26:
	ldr r1, .L_08013a88
	ldr r2, [r1]
	cmp r2, #0
	beq .L_08013a36
	movs r3, #0
	str r3, [r1]
	mov lr, r2
	.2byte 0xf800
.L_08013a36:
	movs r0, #200
	lsls r0, r0, #4
	bl Scheduler_RunCallbacksBeforeBoundary
	ldr r3, .L_08013a8c
	ldr r1, .L_08013a90
	ldrh r2, [r3]
	ldr r3, .L_08013a64
	adds r0, r3, #0
	ldr r3, [r1]
	eors r0, r2
	adds r2, r0, #0
	bics r2, r3
	str r2, [r1, #4]
	ldr r3, [r1, #28]
	orrs r3, r2
	str r3, [r1, #28]
	str r0, [r1]
	cmp r0, #0
	bne .L_08013a94
	movs r3, #19
	b .L_08013aaa
	.2byte 0x0000
.L_08013a64:
	.4byte 0x000003ff
.L_08013a68:
	.4byte gSerialExchangeActive
.L_08013a6c:
	.4byte gSerialTransfer
.L_08013a70:
	.4byte Data_02003870
.L_08013a74:
	.4byte gLinkStatus
.L_08013a78:
	.4byte gBlendDuration
.L_08013a7c:
	.4byte gFrameRenderPending
.L_08013a80:
	.4byte gOamUsage
.L_08013a84:
	.4byte Data_03001120
.L_08013a88:
	.4byte Data_030011f8
.L_08013a8c:
	.4byte 0x04000130
.L_08013a90:
	.4byte gInput
.L_08013a94:
	ldr r3, [r1, #8]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	eors r3, r2
	ldr r2, [r1]
	ands r2, r3
	cmp r2, #0
	beq .L_08013ab0
	movs r3, #1
	negs r3, r3
.L_08013aaa:
	str r3, [r1, #32]
	str r0, [r1, #12]
	b .L_08013abc
.L_08013ab0:
	ldr r3, [r1, #32]
	cmp r3, #0
	ble .L_08013abc
	ldr r3, [r1, #32]
	subs r3, #1
	str r3, [r1, #32]
.L_08013abc:
	ldr r3, .L_08013b14
	ldr r7, .L_08013b18
	str r0, [r3, #8]
	ldr r6, .L_08013b1c
	ldr r5, [r7]
	ldrb r3, [r6]
	mov r12, r3
	lsls r3, r5, #24
	lsrs r3, r3, #24
	cmp r12, r3
	beq .L_08013afa
	ldrb r3, [r6, #3]
	cmp r3, #0
	bne .L_08013afa
	ldr r3, .L_08013b20
	ldrb r2, [r6, #1]
	ldr r3, [r3]
	ldrb r3, [r3, #4]
	strb r3, [r6, #1]
	bl Func_081c0088
	bl SoundDriver_EnterFrameUpdate
	cmp r0, #0
	bne .L_08013af6
	ldr r3, .L_08013b24
	movs r0, #8
	mov lr, r3
	.2byte 0xf800
.L_08013af6:
	ldrb r3, [r6]
	strb r5, [r6]
.L_08013afa:
	adds r3, r5, #1
	str r3, [r7]
	ldr r2, .L_08013b28
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	ldr r2, .L_08013b2c
	ldr r3, .L_08013b10
	strh r3, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_08013b10:
	.4byte 0x00000001
.L_08013b14:
	.4byte gInput
.L_08013b18:
	.4byte gFrameTick
.L_08013b1c:
	.4byte Data_03001138
.L_08013b20:
	.4byte Data_03007ff0
.L_08013b24:
	.4byte IwramSoundRenderFrame
.L_08013b28:
	.4byte gLagFrameCount
.L_08013b2c:
	.4byte Data_0300121c
