.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008060
	ldr r0, .L_02008078
	b .L_02008074
.L_02008060:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008072
	ldr r0, .L_0200807c
	b .L_02008074
.L_02008072:
	ldr r0, .L_02008080
.L_02008074:
	pop {pc}
	.2byte 0x0000
.L_02008078:
	.4byte Data_0200116c
.L_0200807c:
	.4byte Data_02000f14
.L_02008080:
	.4byte Data_02000cbc
	.section .text.x02008084,"ax",%progbits
	.global Func_02000084
	.thumb_func
Func_02000084:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #178
	bl GameFlag_SetBit
	bl Func_02000a94
	movs r0, #0
	bl Func_02000b8c
	ldr r0, .L_020081a4
	bl Func_02000b24
	movs r0, #31
	movs r1, #1
	bl Func_02000b84
	movs r1, #226
	movs r2, #80
	movs r0, #4
	lsls r1, r1, #1
	adds r2, #255
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02000b7c
	movs r1, #128
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #31
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #10
	adds r0, #31
	movs r1, #0
	bl Func_02000b34
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_02000b64
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #10
	adds r0, #31
	movs r1, #0
	bl Func_02000b34
	movs r1, #3
	movs r0, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #4
	bl Motion_SetModeAndWaitAnimation
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #10
	adds r0, #31
	movs r1, #0
	bl Func_02000b34
	movs r1, #2
	movs r0, #31
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	movs r2, #10
	adds r0, #31
	movs r1, #0
	bl Func_02000b34
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #8
	adds r0, #31
	movs r1, #0
	movs r2, #10
	bl Func_02000b34
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #31
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	ldr r3, .L_020081a8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02000b7c
	bl Func_02000a9c
	pop {pc}
	.2byte 0x0000
.L_020081a4:
	.4byte MsgSoYouVeFinally
.L_020081a8:
	.4byte gPartyState
	.section .text.x020081ac,"ax",%progbits
	.global Func_020001ac
	.thumb_func
Func_020001ac:
	push {r5, lr}
	ldr r3, .L_020081e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_020081e0
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020081e8
	movs r0, #9
	adds r1, r5, #0
	bl Func_02000bc4
	b .L_020081f6
	.2byte 0x0000
.L_020081e0:
	.4byte 0xffffc000
.L_020081e4:
	.4byte gPartyState
.L_020081e8:
	ldr r0, .L_020081f8
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
.L_020081f6:
	pop {r5, pc}
.L_020081f8:
	.4byte MsgDonTMention
	.section .text.x020081fc,"ax",%progbits
	.global Func_020001fc
	.thumb_func
Func_020001fc:
	push {r5, lr}
	ldr r3, .L_02008234
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008230
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008238
	movs r0, #2
	adds r1, r5, #0
	bl Func_02000bd4
	b .L_02008246
	.2byte 0x0000
.L_02008230:
	.4byte 0xffffc000
.L_02008234:
	.4byte gPartyState
.L_02008238:
	ldr r0, .L_02008248
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
.L_02008246:
	pop {r5, pc}
.L_02008248:
	.4byte MsgMaybeYouVe
	.section .text.x0200824c,"ax",%progbits
	.global Func_0200024c
	.thumb_func
Func_0200024c:
	push {r5, lr}
	ldr r3, .L_02008280
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200827c
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008284
	adds r0, r5, #0
	bl Func_02000bcc
	b .L_02008292
.L_0200827c:
	.4byte 0xffffc000
.L_02008280:
	.4byte gPartyState
.L_02008284:
	ldr r0, .L_02008294
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
.L_02008292:
	pop {r5, pc}
.L_02008294:
	.4byte MsgInTheMiddle
	.section .text.x02008298,"ax",%progbits
	.global Func_02000298
	.thumb_func
Func_02000298:
	push {r5, lr}
	movs r1, #8
	adds r1, #255
	movs r2, #30
	adds r5, r0, #0
	bl Func_02000b5c
	ldr r0, .L_020082b8
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
	pop {r5, pc}
	.2byte 0x0000
.L_020082b8:
	.4byte MsgAughWithAll
	.section .text.x020082bc,"ax",%progbits
	.global Func_020002bc
	.thumb_func
Func_020002bc:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #177
	bl GameFlag_SetBit
	bl Func_02000a94
	movs r0, #0
	bl Func_02000b8c
	ldr r5, .L_0200843c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r2, #164
	ldr r0, [r5]
	movs r1, #152
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #192
	ldr r0, [r5]
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	ldr r0, .L_02008440
	bl Func_02000b24
	movs r1, #0
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl Func_02000b34
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #15
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #15
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #0
	movs r2, #10
	bl Func_02000b34
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	movs r1, #32
	negs r1, r1
	movs r2, #0
	movs r0, #14
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #14
	movs r1, #0
	bl Func_02000b34
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #30
	movs r0, #13
	bl Func_02000b5c
	movs r2, #10
	movs r0, #13
	movs r1, #0
	bl Func_02000b34
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl Func_02000b64
	movs r0, #15
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl Func_02000b64
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #13
	bl ObjectMotion_ArmCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #10
	movs r0, #13
	bl Func_02000b34
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #156
	lsls r2, r2, #1
	movs r1, #152
	movs r0, #14
	bl ObjectMotion_SetPositionAndReset
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #15
	movs r1, #4
	bl Object_SetModeById
	movs r1, #4
	movs r0, #14
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	bl Func_02000a9c
	pop {r5, pc}
	.2byte 0x0000
.L_0200843c:
	.4byte gPartyState
.L_02008440:
	.4byte MsgArenTYouThirsty
	.section .text.x02008444,"ax",%progbits
	.global Func_02000444
	.thumb_func
Func_02000444:
	push {r5, lr}
	ldr r3, .L_02008474
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	adds r5, r0, #0
	bl Object_LinkPair
	ldr r0, .L_02008478
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	pop {r5, pc}
.L_02008474:
	.4byte gPartyState
.L_02008478:
	.4byte MsgWellISay
	.section .text.x0200847c,"ax",%progbits
	.global Func_0200047c
	.thumb_func
Func_0200047c:
	push {r5, lr}
	ldr r3, .L_020084b0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r1, #0
	movs r2, #0
	ldr r1, [r3]
	adds r0, r5, #0
	bl Object_LinkPair
	ldr r0, .L_020084b4
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
	movs r1, #224
	adds r0, r5, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	pop {r5, pc}
	.2byte 0x0000
.L_020084b0:
	.4byte gPartyState
.L_020084b4:
	.4byte MsgReallyIDo
	.section .text.x020084b8,"ax",%progbits
	.global Func_020004b8
	.thumb_func
Func_020004b8:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #184
	bl GameFlag_SetBit
	ldr r0, .L_020084f0
	bl Func_02000b24
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #132
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #30
	bl Func_02000b5c
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000b3c
	pop {r5, pc}
.L_020084f0:
	.4byte MsgWeVeAll
	.section .text.x020084f4,"ax",%progbits
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {r5, r6, lr}
	ldr r5, .L_0200853c
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_02000b24
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000ba4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008524
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000b24
	b .L_02008530
.L_02008524:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000b24
.L_02008530:
	adds r0, r6, #0
	movs r1, #0
	bl Func_02000b3c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200853c:
	.4byte MsgSoIHear
	.section .text.x02008540,"ax",%progbits
	.global Func_02000540
	.thumb_func
Func_02000540:
	push {lr}
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008554
	ldr r0, .L_0200856c
	b .L_02008568
.L_02008554:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008566
	ldr r0, .L_02008570
	b .L_02008568
.L_02008566:
	ldr r0, .L_02008574
.L_02008568:
	pop {pc}
	.2byte 0x0000
.L_0200856c:
	.4byte Data_020019dc
.L_02008570:
	.4byte Data_02001730
.L_02008574:
	.4byte Data_020013dc
	.section .text.x02008578,"ax",%progbits
	.global Func_02000578
	.thumb_func
Func_02000578:
	push {lr}
	ldr r4, .L_02008594
	ldr r2, [r0, #12]
	movs r3, #192
	lsls r3, r3, #12
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	adds r0, r4, #0
	bl Func_02000bbc
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008594:
	.4byte Data_02001c90
	.section .text.x02008598,"ax",%progbits
	.global Func_02000598
	.thumb_func
Func_02000598:
	push {lr}
	ldr r4, .L_020085b4
	ldr r2, [r0, #12]
	movs r3, #128
	lsls r3, r3, #12
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	adds r0, r4, #0
	bl Func_02000bbc
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020085b4:
	.4byte Data_02001c60
	.section .text.x020085b8,"ax",%progbits
	.global Func_020005b8
	.thumb_func
Func_020005b8:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #14
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #15
	bl ObjectMotion_SetActionVariant
	movs r0, #0
	bl Func_02000bac
	movs r3, #128
	ldr r0, .L_02008720
	movs r1, #8
	movs r2, #15
	lsls r3, r3, #23
	bl Func_02000bb4
	ldr r5, .L_02008724
	movs r3, #128
	movs r1, #8
	movs r2, #15
	adds r0, r5, #0
	lsls r3, r3, #23
	bl Func_02000bb4
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r5, #24]
	movs r0, #14
	bl Object_GetById
	ldr r3, .L_02008728
	str r3, [r0, #108]
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_0200872c
	str r3, [r0, #108]
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086d4
	ldr r3, .L_02008730
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #18
	cmp r3, r2
	blt .L_02008678
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008680
.L_02008678:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02008680:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086c6
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #35
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086c6
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086c6
	movs r1, #188
	movs r2, #158
	movs r0, #31
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000aec
	movs r1, #196
	movs r2, #172
	movs r0, #32
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02000aec
	b .L_0200871c
.L_020086c6:
	movs r0, #31
	movs r1, #0
	movs r2, #0
	bl Func_02000aec
	movs r0, #32
	b .L_020086f2
.L_020086d4:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086fc
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #113
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200871c
	movs r0, #31
.L_020086f2:
	movs r1, #0
	movs r2, #0
	bl Func_02000aec
	b .L_0200871c
.L_020086fc:
	movs r0, #24
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #28
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #25
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_0200871c:
	movs r0, #0
	pop {r5, pc}
.L_02008720:
	.4byte Data_02001c90
.L_02008724:
	.4byte Data_02001c60
.L_02008728:
	.4byte Func_02000578
.L_0200872c:
	.4byte Func_02000598
.L_02008730:
	.4byte gPartyState
	.section .text.x02008738,"ax",%progbits
	.global Func_02000738
	.thumb_func
Func_02000738:
	push {r5, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200874a
	b .L_02008a6a
.L_0200874a:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008758
	b .L_02008a6a
.L_02008758:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #3
	bl GameFlag_SetBit
	bl Func_02000a94
	movs r0, #0
	bl Func_02000b8c
	ldr r0, .L_02008a6c
	bl Func_02000b24
	movs r1, #252
	movs r2, #164
	lsls r2, r2, #1
	movs r0, #4
	lsls r1, r1, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	bl Func_02000b4c
	movs r0, #204
	movs r1, #1
	movs r2, #164
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #17
	bl Motion_CamBounds
	bl Func_02000b7c
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #31
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #5
	bl Func_02000b34
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #32
	bl Func_02000b5c
	movs r0, #32
	movs r1, #0
	movs r2, #5
	bl Func_02000b34
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #31
	bl Func_02000b5c
	movs r0, #31
	movs r1, #0
	movs r2, #5
	bl Func_02000b34
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #32
	bl Func_02000b5c
	movs r2, #5
	movs r0, #32
	movs r1, #0
	bl Func_02000b34
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #5
	bl Func_02000b34
	movs r0, #32
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #32
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r2, #5
	movs r0, #32
	movs r1, #0
	bl Func_02000b34
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #0
	movs r2, #5
	bl Func_02000b34
	movs r1, #10
	adds r1, #255
	movs r2, #30
	movs r0, #32
	bl Func_02000b5c
	movs r2, #5
	movs r0, #32
	movs r1, #0
	bl Func_02000b34
	movs r1, #2
	movs r0, #31
	bl Motion_SetVarCbAndRefresh
	movs r0, #5
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #31
	movs r1, #0
	bl Func_02000b34
	movs r1, #3
	movs r0, #32
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #32
	movs r1, #0
	bl Func_02000b34
	movs r1, #208
	lsls r1, r1, #8
	movs r0, #31
	bl Func_02000b4c
	movs r0, #15
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #31
	movs r1, #0
	bl Func_02000b34
	movs r1, #3
	movs r0, #32
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #5
	movs r0, #32
	movs r1, #0
	bl Func_02000b34
	movs r1, #192
	lsls r1, r1, #6
	movs r0, #31
	bl Func_02000b4c
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #31
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #32
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #128
	movs r0, #31
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #8
	movs r0, #32
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #31
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	movs r1, #197
	movs r2, #171
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #31
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #225
	movs r2, #173
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #32
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #31
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #225
	movs r2, #173
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #31
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #235
	movs r2, #164
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #32
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #31
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #235
	movs r2, #164
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #31
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #243
	movs r2, #164
	lsls r2, r2, #1
	lsls r1, r1, #1
	movs r0, #32
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #32
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #32
	movs r1, #1
	bl Object_SetModeById
	movs r1, #0
	movs r0, #32
	bl Func_02000b4c
	movs r0, #31
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #31
	movs r1, #1
	bl Object_SetModeById
	movs r0, #31
	movs r1, #0
	bl Func_02000b4c
	movs r2, #16
	negs r2, r2
	movs r0, #4
	movs r1, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #4
	bl Func_02000b4c
	movs r0, #10
	bl Battle_WaitMode0
	ldr r5, .L_02008a70
	movs r0, #32
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #31
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #32
	movs r0, #4
	bl Object_LinkObjectAndSetCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #31
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #32
	bl ObjectMotion_SetActionVariant
	movs r0, #32
	bl Object_RefreshSelectorById
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #0
	movs r2, #0
	movs r0, #32
	bl Func_02000aec
	movs r0, #123
	bl Func_02000bdc
	movs r0, #31
	bl Object_RefreshSelectorById
	movs r1, #0
	movs r2, #0
	movs r0, #31
	bl Func_02000aec
	movs r0, #123
	bl Func_02000bdc
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02000a9c
.L_02008a6a:
	pop {r5, pc}
.L_02008a6c:
	.4byte MsgLegendsSaidThat
.L_02008a70:
	.4byte Data_02000be4
	.section .rodata.x02008be4,"a",%progbits
	.global Data_02000be4
Data_02000be4:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00000001
	.4byte 0x00000011
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
	.4byte 0x00000060
	.4byte 0x1010105f
	.4byte 0xffffffff
	.4byte 0x1020205f
	.4byte 0xffffffff
	.4byte 0x1050505f
	.4byte 0xffffffff
	.4byte 0x1060605f
	.4byte 0xffffffff
	.4byte 0x1070705f
	.4byte 0xffffffff
	.4byte 0x11414060
	.4byte 0xffffffff
	.4byte 0x11515060
	.4byte 0xffffffff
	.4byte 0x11616060
	.4byte 0xffffffff
	.4byte 0x11717060
	.4byte 0xffffffff
	.4byte 0x11818060
	.4byte 0xffffffff
	.4byte 0x11919060
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02000cbc
Data_02000cbc:
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0001e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0002b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00022000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00006000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00014000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02200000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00004000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00008000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000f14
Data_02000f14:
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00014000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00010000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000a000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0001e000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200116c
Data_0200116c:
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00a00000
	.4byte 0x00000000
	.4byte 0x02200000
	.4byte 0x00014000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000e000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x0000b000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00002000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00004000
	.4byte 0xffff00c7
	.4byte 0x00000001
	.4byte 0x02980000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00014000
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x0000a000
	.4byte 0xffff0074
	.4byte 0x00000002
	.4byte 0x03600000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02fa0000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x0001c000
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00010000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0001c000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00012000
	.4byte 0xffff0080
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000a000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x0000e000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0xffc80000
	.4byte 0x00000000
	.4byte 0xffc80000
	.4byte 0x00014000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x02000000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00005000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x0000a000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x013c0000
	.4byte 0x00003000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020013dc
Data_020013dc:
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_020001ac
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte Func_020001fc
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_0200024c
	.4byte 0x00000173
	.4byte 0xffff00d6
	.2byte MsgSeasonalVeggiesBoiling, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d7
	.2byte MsgThereIsA, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d8
	.2byte MsgItLooksLikeSome, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d9
	.2byte MsgItSAlhafra, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgThatSeafoodStew, 0x0040
	.4byte 0x00000002
	.4byte 0x08b2001e
	.4byte Func_02000084
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgIDonTCareIf
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgThoseGuysWho
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgIWonderIfTheMayor
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgWhyWouldAnyonePay
	.4byte 0x00000000
	.4byte 0x08b1000e
	.4byte Func_020002bc
	.4byte 0x00008d15
	.4byte 0x08b1040e
	.4byte Func_020002bc
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgWeReTaking
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000298
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000444
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgWhenWeLostOur
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgIJustWishThat
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgIDonTMind
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgAllIAsk
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgIsThisHow
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgDonTMention
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgIfAllMy
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgMaybeYouVe
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgYouSeeHow
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgTheTownOught
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgICanTBelieveThe
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgTheInnkeeperTold
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgTheInnkeeperServes
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgWeReAllGuests
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgOhhhIM
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgIDonTThinkI
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgILlNeverForgive
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgOurGuestsAre
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgMyPopSBeen
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte MsgWhyDoWe
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgWeAllHave
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgIHeardThereS
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte MsgTheBrokenMast
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgINeverImagined
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgPerhapsTheMayor
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgIBetThe
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgICanTBelieveTheInnkeeper
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgICanTBelieveI
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgMaybeItS
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgIThinkI
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgMaybeIM
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgSniffSniffThat
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgIHopeTheMayor
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgIKnowOur
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgEveryoneSaysThe
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte MsgIThinkThey
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgWhenYouRe
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgThatWarriorNext
	.4byte 0x00008d15
	.4byte 0xffff041f
	.4byte Func_0200047c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgInTheMiddle
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgThePeopleOfGaroh
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001730
Data_02001730:
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_020001ac
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte Func_020001fc
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_0200024c
	.4byte 0x00000173
	.4byte 0xffff00d6
	.2byte MsgSeasonalVeggiesBoiling, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d7
	.2byte MsgThereIsA, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d8
	.2byte MsgItLooksLikeSome, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d9
	.2byte MsgItSAlhafra, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgThatSeafoodStew, 0x0040
	.4byte 0x00000000
	.4byte 0x1971001e
	.4byte MsgThatCuteBoy
	.4byte 0x00008d15
	.4byte 0x1971001e
	.4byte MsgTheFellowNext
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgIMAmazed
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgIThoughtItWas
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgPiratesHereIn
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgThoseGuysWere
	.4byte 0x00000000
	.4byte 0x08b8000c
	.4byte Func_020004b8
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgOoohNotMuch
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgOnceMyOld
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgOnceEveryoneStarted
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgIWonderWhatHappened
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgAhhItS
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgItWonTBe
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgTheLadyOf
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgTheLadyOfThe
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgTheMadransDropped
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgTheMadransMust
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgAsSoonAsI
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgWeReAllTaking
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgEveryoneSWorking
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgThereSStill
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgTonightSMeal
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgMomSaidWe
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgMySonHasTerrible
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgTheGuyNext
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Func_020004f4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgWhyDoIHaveTo
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgIMayNot
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgWeHeardA
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgIWonderWhatOur
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgIWasTold
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgOhIfWe
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgIGuessI
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgOurNeighborIs
	.4byte 0x00008d15
	.4byte 0xffff001f
	.4byte MsgFeelFreeTo
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgCapturingTheDreaded
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgIThinkThis
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020019dc
Data_020019dc:
	.4byte 0x00004401
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00004401
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00004401
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00004401
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00004401
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000001
	.4byte 0xffff0015
	.4byte 0x00000015
	.4byte 0x00000001
	.4byte 0xffff0016
	.4byte 0x00000016
	.4byte 0x00000001
	.4byte 0xffff0017
	.4byte 0x00000017
	.4byte 0x00000001
	.4byte 0xffff0018
	.4byte 0x00000018
	.4byte 0x00000001
	.4byte 0xffff0019
	.4byte 0x00000019
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_020001ac
	.4byte 0x0000c400
	.4byte 0xffff0010
	.4byte Func_020001fc
	.4byte 0x0000c400
	.4byte 0xffff0009
	.4byte Func_0200024c
	.4byte 0x00000173
	.4byte 0xffff00d6
	.2byte MsgSeasonalVeggiesBoiling, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d7
	.2byte MsgThereIsA, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d8
	.2byte MsgItLooksLikeSome, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00d9
	.2byte MsgItSAlhafra, 0x0040
	.4byte 0x00000173
	.4byte 0xffff00c8
	.2byte MsgThatSeafoodStew, 0x0040
	.4byte 0x00000002
	.4byte 0x1823001f
	.4byte Func_02000738
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte MsgIDonTWantToSay
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte MsgBriggsDidnTCommit
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgBriggsPaidFor
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte MsgIDonTBelieve
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte MsgFixingThatOld
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte MsgTheRepairsOn
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte MsgMyHouseIs
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte MsgMyHusbandAnd
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte MsgWillTheyEver
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte MsgIDonTHave
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgIDonTMindNot
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte MsgIHatedThis
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte MsgTheBridgeTo
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte MsgPeopleWillStart
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte MsgWeReStillRecovering
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte MsgTheMayorUsed
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte MsgItSeemsThat
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte MsgBriggsMightHave
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgABiggerBoat
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte MsgIJustWantA
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte MsgSomeoneSaidMy
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte MsgSoThatAlex
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte MsgAfterTheTidal
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte MsgTheMayorMay
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte MsgBriggsAndHisWife
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte MsgIfIHadAKey
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgSoWhatIf
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgIDReally
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte MsgIsnTAnyone
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte MsgHeWasJust
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte MsgIHeardYouWere
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgTheyWereSo
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 0x00000008
	.global Data_02001c60
Data_02001c60:
	.space 0x00000030
	.global Data_02001c90
Data_02001c90:
