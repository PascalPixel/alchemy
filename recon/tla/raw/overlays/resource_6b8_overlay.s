.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02001300
	.global Data_02000054
Data_02000054:
	.4byte 0x00004770
	.section .text.x02008058,"ax",%progbits
	.global Func_02000058
	.thumb_func
Func_02000058:
	push {lr}
	sub sp, #8
	mov r1, sp
	add r0, sp, #4
	bl Func_02000f34
	add sp, #8
	pop {pc}
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	push {lr}
	movs r0, #0
	bl Func_02000e04
	pop {pc}
	.2byte 0x0000
	.global Data_02000074
Data_02000074:
	.4byte 0x00004770
	.section .text.x02008078,"ax",%progbits
	.global Func_02000078
	.thumb_func
Func_02000078:
	push {r5, lr}
	adds r5, r0, #0
	bl UiWork_FinalizePendingCore
	adds r0, r5, #0
	movs r1, #5
	movs r2, #0
	movs r3, #34
	bl UiText_OpenMessageWindow
	b .L_02008094
.L_0200808e:
	movs r0, #1
	bl WaitFrames
.L_02008094:
	bl UiWork_IsComplete
	cmp r0, #0
	beq .L_0200808e
	movs r0, #1
	bl WaitFrames
	pop {r5, pc}
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r7, r1, #0
	movs r0, #206
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_02000dec
	movs r6, #0
	mov r8, r0
	cmp r6, r7
	bge .L_02008128
.L_020080c2:
	movs r0, #1
	movs r1, #1
	bl Func_02000df4
	movs r0, #5
	movs r1, #2
	bl Func_02000df4
	movs r0, #241
	lsls r0, r0, #9
	adds r0, #64
	movs r1, #5
	bl Func_02000df4
	adds r0, r5, #0
	bl Func_02000078
	b .L_020080f2
.L_020080e6:
	ldr r3, [r1]
	cmp r3, #0
	bne .L_02008122
	movs r0, #1
	bl WaitFrames
.L_020080f2:
	ldr r1, .L_0200813c
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
	cmp r3, #0
	bne .L_02008128
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_02008112
	ldr r3, [r1]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_02008116
.L_02008112:
	adds r5, #1
	b .L_02008122
.L_02008116:
	ldr r3, [r1]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_020080e6
	subs r5, #1
.L_02008122:
	adds r6, #1
	cmp r6, r7
	blt .L_020080c2
.L_02008128:
	bl UiWork_FinalizePendingCore
	mov r0, r8
	movs r1, #2
	bl UiWork_Finalize
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200813c:
	.4byte gInput
	.section .text.x02008140,"ax",%progbits
	.global Func_02000140
	.thumb_func
Func_02000140:
	push {lr}
	ldr r0, .L_02008150
	ldr r1, .L_02008154
	subs r1, r1, r0
	bl Func_020000a4
	pop {pc}
	.2byte 0x0000
.L_02008150:
	.4byte 0x0000124c
.L_02008154:
	.4byte 0x00001277
	.section .text.x02008158,"ax",%progbits
	.global Func_02000158
	.thumb_func
Func_02000158:
	push {lr}
	ldr r0, .L_02008168
	ldr r1, .L_0200816c
	subs r1, r0, r1
	bl Func_020000a4
	pop {pc}
	.2byte 0x0000
.L_02008168:
	.4byte 0x00001277
.L_0200816c:
	.4byte 0x0000124c
	.section .text.x02008170,"ax",%progbits
	.global Func_02000170
	.thumb_func
Func_02000170:
	push {lr}
	ldr r3, .L_02008180
	ldr r1, .L_02008184
	ldr r0, .L_02008188
	subs r1, r1, r3
	bl Func_020000a4
	pop {pc}
.L_02008180:
	.4byte 0x0000124c
.L_02008184:
	.4byte 0x00001277
.L_02008188:
	.4byte 0x000012a2
	.section .text.x0200818c,"ax",%progbits
	.global Func_0200018c
	.thumb_func
Func_0200018c:
	push {lr}
	ldr r0, .L_0200819c
	ldr r1, .L_020081a0
	subs r1, r1, r0
	bl Func_020000a4
	pop {pc}
	.2byte 0x0000
.L_0200819c:
	.4byte 0x000012d2
.L_020081a0:
	.4byte 0x000012fd
	.section .text.x020081a4,"ax",%progbits
	.global Func_020001a4
	.thumb_func
Func_020001a4:
	push {lr}
	adds r1, r0, #0
	movs r0, #1
	bl Func_02000f1c
	pop {pc}
	.section .text.x020081b0,"ax",%progbits
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {lr}
	adds r1, r0, #0
	movs r0, #2
	bl Func_02000f1c
	pop {pc}
	.section .text.x020081bc,"ax",%progbits
	.global Func_020001bc
	.thumb_func
Func_020001bc:
	push {lr}
	adds r1, r0, #0
	movs r0, #3
	bl Func_02000f1c
	pop {pc}
	.section .text.x020081c8,"ax",%progbits
	.global Func_020001c8
	.thumb_func
Func_020001c8:
	push {lr}
	adds r1, r0, #0
	movs r0, #24
	bl Func_02000f1c
	pop {pc}
	.section .text.x020081d4,"ax",%progbits
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {lr}
	adds r1, r0, #0
	movs r0, #0
	bl Func_02000f2c
	pop {pc}
	.global Data_020001e0
Data_020001e0:
	.4byte 0x00004770
	.section .text.x020081e4,"ax",%progbits
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	push {lr}
	movs r1, #1
	ldr r0, .L_02008220
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #4
	bl Owner_GetState
	ldrb r1, [r0, #15]
	movs r0, #4
	adds r1, #10
	bl Party_AdvanceOwnerCountToTarget
	movs r0, #5
	bl Owner_GetState
	ldrb r1, [r0, #15]
	movs r0, #5
	adds r1, #10
	bl Party_AdvanceOwnerCountToTarget
	movs r0, #6
	bl Owner_GetState
	ldrb r1, [r0, #15]
	movs r0, #6
	adds r1, #10
	bl Party_AdvanceOwnerCountToTarget
	pop {pc}
.L_02008220:
	.4byte 0x0000114b
	.section .text.x02008224,"ax",%progbits
	.global Func_02000224
	.thumb_func
Func_02000224:
	push {r5, lr}
	ldr r0, .L_02008260
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r2, .L_02008264
	ldr r3, .L_02008268
	movs r5, #9
	str r3, [r2, #16]
.L_02008236:
	movs r1, #228
	movs r0, #4
	bl Inventory_AddItem
	subs r5, #1
	movs r0, #4
	movs r1, #229
	bl Inventory_AddItem
	cmp r5, #0
	bge .L_02008236
	movs r0, #4
	bl Owner_RecalculateStats
	movs r0, #5
	bl Owner_RecalculateStats
	movs r0, #6
	bl Owner_RecalculateStats
	pop {r5, pc}
.L_02008260:
	.4byte 0x0000114d
.L_02008264:
	.4byte gPartyState
.L_02008268:
	.4byte 0x000bde31
	.section .text.x0200826c,"ax",%progbits
	.global Func_0200026c
	.thumb_func
Func_0200026c:
	push {r5, r6, lr}
	ldr r0, .L_020082f0
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r1, #100
	negs r1, r1
	movs r0, #4
	bl Owner_AdjustFirstValue
	movs r1, #100
	negs r1, r1
	movs r0, #5
	bl Owner_AdjustFirstValue
	movs r1, #33
	negs r1, r1
	movs r0, #6
	bl Owner_AdjustFirstValue
	movs r1, #50
	negs r1, r1
	movs r0, #4
	bl Owner_AdjustSecondValue
	movs r1, #40
	negs r1, r1
	movs r0, #5
	bl Owner_AdjustSecondValue
	movs r1, #35
	negs r1, r1
	movs r0, #6
	bl Owner_AdjustSecondValue
	movs r0, #4
	bl Owner_GetState
	movs r6, #50
	movs r2, #160
	movs r5, #1
	adds r6, #255
	lsls r2, r2, #1
	strb r5, [r0, r6]
	adds r0, r0, r2
	strb r5, [r0]
	movs r0, #5
	bl Owner_GetState
	movs r2, #152
	lsls r2, r2, #1
	adds r3, r0, r2
	strb r5, [r3]
	movs r3, #2
	strb r3, [r0, r6]
	movs r0, #4
	bl Owner_RecalculateStats
	movs r0, #5
	bl Owner_RecalculateStats
	movs r0, #6
	bl Owner_RecalculateStats
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020082f0:
	.4byte 0x0000114c
	.section .text.x020082f4,"ax",%progbits
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r1, #0
	adds r5, r0, #0
	lsls r0, r6, #2
	adds r0, r0, r6
	adds r7, r2, #0
	lsls r0, r0, #2
	adds r0, r0, r7
	adds r0, #48
	mov r8, r3
	bl GameFlag_SetBit
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Djinn_AddToOwner
	mov r3, r8
	cmp r3, #1
	bne .L_0200832a
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Djinn_Activate
.L_0200832a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008330,"ax",%progbits
	.global Func_02000330
	.thumb_func
Func_02000330:
	push {lr}
	ldr r0, .L_02008440
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r0, #4
	movs r1, #0
	movs r2, #0
	movs r3, #1
	bl Func_020002f4
	movs r0, #4
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_020002f4
	movs r0, #4
	movs r1, #0
	movs r2, #2
	movs r3, #1
	bl Func_020002f4
	movs r0, #4
	movs r1, #0
	movs r2, #3
	movs r3, #1
	bl Func_020002f4
	movs r0, #4
	movs r1, #0
	movs r2, #4
	movs r3, #1
	bl Func_020002f4
	movs r0, #4
	movs r1, #0
	movs r2, #5
	movs r3, #1
	bl Func_020002f4
	movs r0, #4
	movs r1, #0
	movs r2, #6
	movs r3, #1
	bl Func_020002f4
	movs r0, #5
	movs r1, #2
	movs r2, #0
	movs r3, #1
	bl Func_020002f4
	movs r0, #5
	movs r1, #2
	movs r2, #1
	movs r3, #1
	bl Func_020002f4
	movs r0, #5
	movs r1, #2
	movs r2, #2
	movs r3, #1
	bl Func_020002f4
	movs r0, #5
	movs r1, #2
	movs r2, #3
	movs r3, #1
	bl Func_020002f4
	movs r0, #5
	movs r1, #2
	movs r2, #4
	movs r3, #1
	bl Func_020002f4
	movs r0, #5
	movs r1, #2
	movs r2, #5
	movs r3, #1
	bl Func_020002f4
	movs r0, #5
	movs r1, #2
	movs r2, #6
	movs r3, #1
	bl Func_020002f4
	movs r0, #6
	movs r1, #1
	movs r2, #0
	movs r3, #1
	bl Func_020002f4
	movs r0, #6
	movs r1, #1
	movs r2, #1
	movs r3, #1
	bl Func_020002f4
	movs r0, #6
	movs r1, #1
	movs r2, #2
	movs r3, #1
	bl Func_020002f4
	movs r0, #6
	movs r1, #1
	movs r2, #3
	movs r3, #1
	bl Func_020002f4
	movs r0, #6
	movs r1, #1
	movs r2, #4
	movs r3, #1
	bl Func_020002f4
	movs r1, #1
	movs r2, #5
	movs r3, #1
	movs r0, #6
	bl Func_020002f4
	movs r0, #4
	bl Owner_RecalculateStats
	movs r0, #5
	bl Owner_RecalculateStats
	movs r0, #6
	bl Owner_RecalculateStats
	pop {pc}
	.2byte 0x0000
.L_02008440:
	.4byte 0x0000114e
	.section .text.x02008444,"ax",%progbits
	.global Func_02000444
	.thumb_func
Func_02000444:
	push {lr}
	movs r0, #4
	bl Object_GetById
	movs r3, #2
	adds r0, #34
	strb r3, [r0]
	pop {pc}
	.section .text.x02008454,"ax",%progbits
	.global Func_02000454
	.thumb_func
Func_02000454:
	push {lr}
	movs r0, #4
	bl Object_GetById
	movs r3, #0
	adds r0, #34
	strb r3, [r0]
	pop {pc}
	.section .text.x02008464,"ax",%progbits
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {r5, lr}
	adds r5, r1, #0
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #50
	adds r0, r5, #0
	bl Func_02000eac
	adds r0, r5, #0
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	adds r0, r5, #0
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084ca
	ldr r0, .L_02008500
	bl Func_02000e94
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e9c
	movs r1, #180
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #72
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_020084f8
.L_020084ca:
	ldr r0, .L_02008504
	bl Func_02000e94
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e9c
	movs r1, #196
	adds r0, r5, #0
	lsls r1, r1, #1
	movs r2, #104
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	adds r0, r5, #0
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_020084f8:
	movs r0, #20
	bl Battle_WaitMode0
	pop {r5, pc}
.L_02008500:
	.4byte 0x0000156d
.L_02008504:
	.4byte 0x0000156e
	.section .text.x02008508,"ax",%progbits
	.global Func_02000508
	.thumb_func
Func_02000508:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	adds r0, r5, #0
	adds r1, #255
	movs r2, #50
	bl Func_02000eac
	adds r0, r5, #0
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #15
	movs r1, #6
	adds r0, r5, #0
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_02008558
	bl Func_02000e94
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e9c
	movs r0, #10
	bl Battle_WaitMode0
	pop {r5, pc}
.L_02008558:
	.4byte 0x0000156f
	.section .text.x0200855c,"ax",%progbits
	.global Func_0200055c
	.thumb_func
Func_0200055c:
	push {r5, lr}
	adds r5, r1, #0
	adds r0, r5, #0
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #4
	adds r0, r5, #0
	adds r1, #255
	movs r2, #50
	bl Func_02000eac
	adds r0, r5, #0
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r2, #15
	movs r1, #6
	adds r0, r5, #0
	bl ObjectMotion_Launch
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, .L_020085ac
	bl Func_02000e94
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e9c
	movs r0, #10
	bl Battle_WaitMode0
	pop {r5, pc}
.L_020085ac:
	.4byte 0x0000156f
	.section .text.x020085b0,"ax",%progbits
	.global Func_020005b0
	.thumb_func
Func_020005b0:
	push {lr}
	movs r1, #2
	movs r0, #21
	sub sp, #8
	bl Object_SetModeById
	movs r0, #10
	bl Battle_WaitMode0
	movs r3, #25
	movs r2, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #57
	movs r1, #16
	movs r2, #4
	movs r3, #6
	bl Func_02000dac
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x020085dc,"ax",%progbits
	.global Func_020005dc
	.thumb_func
Func_020005dc:
	push {lr}
	sub sp, #8
	cmp r0, #1
	bne .L_02008600
	movs r0, #21
	movs r1, #2
	bl Object_SetModeById
	movs r3, #25
	movs r2, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #57
	movs r1, #16
	movs r2, #4
	movs r3, #6
	bl Func_02000dac
.L_02008600:
	add sp, #8
	pop {pc}
	.section .text.x02008604,"ax",%progbits
	.global Func_02000604
	.thumb_func
Func_02000604:
	push {lr}
	sub sp, #12
	movs r3, #2
	movs r2, #10
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r0, #34
	movs r1, #26
	movs r2, #5
	movs r3, #4
	bl Func_02000ecc
	add sp, #12
	pop {pc}
	.section .text.x02008624,"ax",%progbits
	.global Func_02000624
	.thumb_func
Func_02000624:
	push {lr}
	sub sp, #12
	movs r3, #2
	movs r2, #10
	movs r1, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	str r1, [sp, #8]
	movs r2, #5
	movs r3, #4
	movs r0, #34
	movs r1, #26
	bl Func_02000ecc
	ldr r0, .L_0200864c
	movs r1, #6
	bl Func_02000eb4
	add sp, #12
	pop {pc}
.L_0200864c:
	.4byte 0x00000142
	.section .text.x02008650,"ax",%progbits
	.global Func_02000650
	.thumb_func
Func_02000650:
	push {lr}
	ldr r0, .L_0200865c
	movs r1, #8
	bl Func_02000eb4
	pop {pc}
.L_0200865c:
	.4byte 0x00000142
	.section .text.x02008660,"ax",%progbits
	.global Func_02000660
	.thumb_func
Func_02000660:
	push {lr}
	movs r0, #249
	movs r1, #40
	movs r2, #168
	bl Func_02000f14
	ldr r3, .L_02008684
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r0, #14
	movs r1, #0
	bl Func_02000ebc
	pop {pc}
.L_02008684:
	.4byte gPartyState
	.section .text.x02008688,"ax",%progbits
	.global Func_02000688
	.thumb_func
Func_02000688:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	sub sp, #12
	bl GameFlag_SetBit
	movs r3, #1
	str r3, [sp, #0]
	movs r3, #10
	str r3, [sp, #4]
	str r3, [sp, #8]
	adds r0, r5, #0
	movs r1, #36
	movs r2, #28
	movs r3, #1
	bl Func_0200092c
	add sp, #12
	pop {r5, pc}
	.section .text.x020086b0,"ax",%progbits
	.global Func_020006b0
	.thumb_func
Func_020006b0:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_SetBit
	adds r0, r5, #0
	movs r1, #18
	movs r2, #184
	movs r3, #184
	bl Func_02000950
	pop {r5, pc}
	.section .text.x020086cc,"ax",%progbits
	.global Func_020006cc
	.thumb_func
Func_020006cc:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	adds r0, r5, #0
	movs r1, #19
	movs r2, #200
	movs r3, #200
	bl Func_020009c0
	pop {r5, pc}
	.section .text.x020086e8,"ax",%progbits
	.global Func_020006e8
	.thumb_func
Func_020006e8:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	sub sp, #4
	bl GameFlag_SetBit
	movs r3, #0
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #216
	movs r2, #216
	movs r3, #14
	bl Func_02000a10
	add sp, #4
	pop {r5, pc}
	.section .text.x0200870c,"ax",%progbits
	.global Func_0200070c
	.thumb_func
Func_0200070c:
	push {lr}
	movs r0, #19
	bl Object_GetById
	movs r2, #196
	adds r0, #98
	ldrb r1, [r0]
	lsls r2, r2, #2
	movs r0, #19
	bl Func_02000e5c
	pop {pc}
	.section .text.x02008724,"ax",%progbits
	.global Func_02000724
	.thumb_func
Func_02000724:
	push {lr}
	sub sp, #8
	movs r3, #16
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #48
	movs r1, #26
	movs r2, #2
	movs r3, #4
	bl Func_02000dac
	add sp, #8
	pop {pc}
	.global Data_02000740
Data_02000740:
	.4byte 0x00004770
	.global Data_02000744
Data_02000744:
	.4byte 0x00004770
	.section .text.x02008748,"ax",%progbits
	.global Func_02000748
	.thumb_func
Func_02000748:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Object_GetById
	ldr r1, .L_02008788
	bl Func_02000c4c
	ldr r5, .L_0200878c
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	ldr r1, .L_02008790
	bl Func_02000c4c
	movs r0, #100
	bl WaitFrames
	adds r0, r6, #0
	bl Object_GetById
	bl Func_02000d4c
	ldr r0, [r5]
	bl Object_GetById
	bl Func_02000d4c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008788:
	.4byte Data_02001860
.L_0200878c:
	.4byte gPartyState
.L_02008790:
	.4byte Data_020017b0
	.section .text.x0200879c,"ax",%progbits
	.global Func_0200079c
	.thumb_func
Func_0200079c:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r1, r2, r3
	subs r3, #172
	str r3, [r1]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #55
	adds r3, r2, r1
	movs r1, #0
	strb r1, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #54
	adds r2, r2, r3
	strb r1, [r2]
	movs r0, #5
	movs r1, #1
	sub sp, #8
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #5
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #5
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #6
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #6
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #7
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #106
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #108
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #109
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #113
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #123
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #130
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #140
	bl Item_AdjustCounter
	movs r1, #1
	movs r0, #151
	bl Item_AdjustCounter
	ldr r3, .L_02008914
	movs r1, #139
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	movs r0, #15
	movs r1, #2
	bl Object_SetModeById
	movs r0, #16
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #8
	bl Func_02000edc
	movs r1, #128
	movs r0, #10
	lsls r1, r1, #8
	bl Func_02000ed4
	movs r0, #12
	movs r1, #0
	bl Func_02000ed4
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200888e
	movs r3, #10
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #36
	movs r1, #28
	movs r2, #1
	movs r3, #1
	bl Func_02000dac
.L_0200888e:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088aa
	movs r1, #184
	movs r2, #184
	movs r0, #18
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02000e74
.L_020088aa:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088d2
	movs r0, #196
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020088d2
	movs r1, #200
	movs r2, #200
	movs r0, #19
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02000e74
.L_020088d2:
	movs r0, #19
	bl Object_GetById
	movs r1, #50
	bl Func_02000dc4
	ldr r5, .L_02008914
	movs r2, #241
	lsls r2, r2, #1
	adds r6, r5, r2
	movs r1, #0
	ldrsh r3, [r6, r1]
	cmp r3, #6
	bne .L_020088f8
	adds r2, #50
	adds r3, r5, r2
	ldr r0, [r3]
	bl Func_02000f04
.L_020088f8:
	movs r1, #0
	ldrsh r3, [r6, r1]
	cmp r3, #8
	bne .L_0200890c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Func_02000f0c
.L_0200890c:
	movs r0, #0
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008914:
	.4byte gPartyState
	.section .text.x0200891c,"ax",%progbits
	.global Func_0200091c
	.thumb_func
Func_0200091c:
	push {lr}
	bl Func_02000e0c
	pop {pc}
	.section .text.x02008924,"ax",%progbits
	.global Func_02000924
	.thumb_func
Func_02000924:
	push {lr}
	bl Func_02000f24
	pop {pc}
	.section .text.x0200892c,"ax",%progbits
	.global Func_0200092c
	.thumb_func
Func_0200092c:
	push {lr}
	sub sp, #8
	adds r4, r3, #0
	cmp r0, #1
	bne .L_0200894a
	ldr r3, [sp, #16]
	adds r0, r1, #0
	str r3, [sp, #0]
	ldr r3, [sp, #20]
	adds r1, r2, #0
	str r3, [sp, #4]
	adds r2, r4, #0
	ldr r3, [sp, #12]
	bl Func_02000dac
.L_0200894a:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008950,"ax",%progbits
	.global Func_02000950
	.thumb_func
Func_02000950:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r1, #0
	mov r10, r0
	adds r0, r6, #0
	adds r7, r2, #0
	mov r8, r3
	bl Object_GetById
	mov r2, r10
	adds r5, r0, #0
	cmp r2, #1
	bne .L_02008980
	mov r3, r8
	lsls r2, r3, #16
	lsls r1, r7, #16
	adds r0, r6, #0
	bl Func_02000e74
	movs r3, #0
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02008980:
	mov r2, r10
	cmp r2, #2
	bne .L_020089b6
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_020089ae
.L_02008990:
	movs r0, #1
	bl WaitFrames
	ldr r3, [r5, #24]
	movs r2, #160
	lsls r2, r2, #3
	adds r2, #30
	adds r3, r3, r2
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	str r3, [r5, #24]
	str r3, [r5, #28]
	cmp r3, r2
	ble .L_02008990
.L_020089ae:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_020089b6:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020089c0,"ax",%progbits
	.global Func_020009c0
	.thumb_func
Func_020009c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r1, #0
	mov r10, r0
	adds r0, r7, #0
	mov r8, r3
	adds r5, r2, #0
	bl Object_GetById
	mov r3, r10
	adds r6, r0, #0
	cmp r3, #2
	bne .L_02008a06
	mov r3, r8
	lsls r2, r3, #16
	adds r0, r7, #0
	lsls r1, r5, #16
	bl Func_02000e74
	movs r3, #128
	lsls r3, r3, #14
	adds r2, r6, #0
	str r3, [r6, #12]
	adds r2, #85
	movs r3, #3
	strb r3, [r2]
	movs r3, #184
	lsls r3, r3, #5
	adds r3, #10
	str r3, [r6, #72]
	movs r0, #50
	bl WaitFrames
.L_02008a06:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008a10,"ax",%progbits
	.global Func_02000a10
	.thumb_func
Func_02000a10:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r10, r3
	cmp r6, #1
	bne .L_02008a46
	movs r0, #177
	lsls r3, r2, #16
	lsls r1, r7, #16
	lsls r0, r0, #1
	movs r2, #0
	bl Func_02000da4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008a46
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02008a46:
	cmp r6, #2
	bne .L_02008ac2
	mov r2, r8
	lsls r3, r2, #16
	lsls r1, r7, #16
	movs r0, #252
	movs r2, #0
	bl Func_02000da4
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008ac2
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r5, #40]
	movs r3, #204
	lsls r3, r3, #7
	adds r3, #102
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02008a74:
	movs r0, #1
	bl WaitFrames
	ldr r2, [r5, #24]
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #30
	adds r2, r2, r3
	ldrh r3, [r5, #6]
	movs r1, #128
	lsls r1, r1, #5
	adds r3, r3, r1
	strh r3, [r5, #6]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	str r2, [r5, #24]
	str r2, [r5, #28]
	cmp r2, r3
	ble .L_02008a74
	adds r3, #1
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r3, .L_02008acc
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #8
	adds r3, r3, r2
	strh r3, [r5, #6]
	mov r0, r10
	ldr r1, [sp, #24]
	bl Func_02000ebc
.L_02008ac2:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008acc:
	.4byte gPartyState
	.section .text.x02008ad0,"ax",%progbits
	.global Func_02000ad0
	.thumb_func
Func_02000ad0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	ldr r2, [r7, #104]
	adds r6, r7, #0
	adds r6, #99
	mov r8, r2
	ldrb r2, [r6]
	movs r3, #1
	ands r3, r2
	sub sp, #24
	cmp r3, #0
	beq .L_02008b0a
	ldrb r0, [r6]
	movs r1, #6
	lsrs r0, r0, #1
	bl Engine_MathModulo
	adds r1, r0, #0
	lsls r1, r1, #24
	lsrs r1, r1, #24
	adds r0, r7, #0
	bl Animation_ApplyChildValues
.L_02008b0a:
	adds r3, r7, #0
	adds r3, #98
	ldrb r5, [r3]
	cmp r5, #0
	bne .L_02008b48
	ldrb r2, [r6]
	movs r3, #15
	ands r3, r2
	cmp r3, #0
	bne .L_02008b7e
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #86
	bl Func_02000f3c
	mov r1, r8
	adds r1, #166
	movs r2, #0
	ldrsh r3, [r1, r2]
	mov r2, r8
	lsls r3, r3, #1
	adds r3, #160
	strh r5, [r2, r3]
	ldr r2, .L_02008b44
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	b .L_02008b7e
	.2byte 0x0000
.L_02008b44:
	.4byte 0x00000001
.L_02008b48:
	cmp r5, #1
	bne .L_02008b7e
	mov r3, r8
	adds r3, #160
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_02008b7e
	mov r3, r8
	adds r3, #162
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #31
	ble .L_02008b7e
	adds r0, r7, #0
	movs r1, #0
	bl Animation_ApplyChildValues
	mov r3, r8
	adds r3, #164
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Resource_ResetEntry
	movs r3, #0
	str r3, [r7, #108]
	b .L_02008c38
.L_02008b7e:
	ldrb r3, [r6]
	movs r2, #1
	adds r3, #1
	strb r3, [r6]
	movs r3, #0
	str r3, [sp, #0]
	mov r6, r8
	mov r11, r2
	adds r6, #160
.L_02008b90:
	movs r3, #0
	ldrsh r0, [r6, r3]
	lsls r0, r0, #10
	bl Math_Sine
	str r0, [sp, #4]
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	blt .L_02008c24
	cmp r3, #31
	bgt .L_02008c24
	ldr r3, [r7, #8]
	add r5, sp, #12
	str r3, [r5]
	adds r0, r5, #0
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r7, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r7, #16]
	str r3, [r5, #8]
	bl Func_02000ec4
	ldr r2, [r5]
	movs r3, #0
	str r2, [sp, #8]
	mov r10, r3
	ldr r5, [r5, #8]
	mov r9, r5
	ldr r5, [sp, #0]
	add r5, r8
.L_02008bd4:
	ldr r2, [sp, #8]
	mov r3, r9
	str r3, [r5, #16]
	str r2, [r5, #12]
	ldr r2, [sp, #4]
	mov r3, r10
	str r2, [r5, #20]
	str r2, [r5, #24]
	cmp r3, #0
	bne .L_02008bf4
	adds r0, r7, #0
	bl Func_02000efc
	subs r0, #1
	strh r0, [r5, #30]
	b .L_02008c0c
.L_02008bf4:
	adds r0, r7, #0
	bl Func_02000efc
	ldr r3, [r5, #16]
	ldr r2, .L_02008c48
	adds r0, #1
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r3, [r5, #24]
	strh r0, [r5, #30]
	negs r3, r3
	str r3, [r5, #24]
.L_02008c0c:
	adds r0, r5, #0
	bl Func_02000eec
	movs r3, #1
	add r10, r3
	mov r2, r10
	adds r5, #40
	cmp r2, #1
	ble .L_02008bd4
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
.L_02008c24:
	ldr r3, [sp, #0]
	movs r2, #1
	negs r2, r2
	adds r3, #80
	add r11, r2
	str r3, [sp, #0]
	mov r3, r11
	adds r6, #2
	cmp r3, #0
	bge .L_02008b90
.L_02008c38:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008c48:
	.4byte 0xffff0000
	.section .text.x02008c4c,"ax",%progbits
	.global Func_02000c4c
	.thumb_func
Func_02000c4c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	adds r7, r1, #0
	sub sp, #4
	bl Resource_FindFreeEntry
	movs r1, #164
	adds r1, r1, r7
	mov r8, r1
	ldr r2, .L_02008ca8
	mov r3, r8
	strh r0, [r3]
	movs r1, #128
	lsls r0, r0, #16
	mov r10, r2
	lsls r1, r1, #1
	ldr r2, .L_02008cac
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r3, r7, #0
	movs r2, #186
	movs r5, #0
	adds r3, #166
	lsls r2, r2, #2
	strh r5, [r3]
	adds r2, #255
	subs r3, #6
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	adds r3, #6
	str r6, [r3]
	adds r3, r6, #0
	mov r0, r10
	adds r3, #98
	strb r0, [r3]
	adds r3, #1
	b .L_02008cb0
	.2byte 0x0000
.L_02008ca8:
	.4byte 0x00000000
.L_02008cac:
	.4byte Data_02000f44
.L_02008cb0:
	strb r0, [r3]
	ldr r3, .L_02008d40
	mov r0, r8
	str r3, [r6, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	ldr r2, .L_02008d44
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	str r7, [r6, #104]
	lsrs r3, r3, #5
	mov r11, r3
	mov r9, r5
	mov r10, r5
.L_02008cce:
	movs r1, #1
	mov r2, r10
	mov r8, r1
	adds r5, r2, r7
.L_02008cd6:
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #16
	movs r2, #16
	ldr r3, .L_02008d48
	bl Func_02000ee4
	ldrb r3, [r5, #5]
	movs r0, #33
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	ldrb r2, [r5, #9]
	strb r3, [r5, #5]
	movs r3, #15
	ands r3, r2
	strb r3, [r5, #9]
	adds r0, r6, #0
	bl Func_02000ef4
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r5, #9]
	negs r1, r1
	adds r2, r1, #0
	ands r3, r2
	movs r2, #1
	lsls r0, r0, #2
	negs r2, r2
	orrs r3, r0
	add r8, r2
	strb r3, [r5, #9]
	mov r3, r8
	adds r5, #40
	cmp r3, #0
	bge .L_02008cd6
	movs r1, #1
	add r9, r1
	movs r0, #80
	mov r2, r9
	add r10, r0
	cmp r2, #1
	ble .L_02008cce
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d40:
	.4byte Func_02000ad0
.L_02008d44:
	.4byte ResourceTableEntries
.L_02008d48:
	.4byte 0x80004000
	.section .text.x02008d4c,"ax",%progbits
	.global Func_02000d4c
	.thumb_func
Func_02000d4c:
	adds r0, #98
	movs r3, #1
	strb r3, [r0]
	bx lr
	.section .rodata.x02008f44,"a",%progbits
	.global Data_02000f44
Data_02000f44:
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x70000000
	.4byte 0xf7700000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x77770000
	.4byte 0xffff7770
	.4byte 0xfffffff7
	.4byte 0x7777ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007777
	.4byte 0x0777ffff
	.4byte 0x7fffffff
	.4byte 0xffff7777
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000007
	.4byte 0x0000077f
	.4byte 0xfff70000
	.4byte 0xffff7000
	.4byte 0x7ffff700
	.4byte 0x07ffff70
	.4byte 0x007fff70
	.4byte 0x007ffff7
	.4byte 0x0007fff7
	.4byte 0x0007fff7
	.4byte 0x000077ff
	.4byte 0x00000077
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff770000
	.4byte 0x77000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00007fff
	.4byte 0x0007ffff
	.4byte 0x007ffff7
	.4byte 0x07ffff70
	.4byte 0x07fff700
	.4byte 0x7ffff700
	.4byte 0x7fff7000
	.4byte 0x7fff7000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xf4400000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x44440000
	.4byte 0xffff4440
	.4byte 0xfffffff4
	.4byte 0x4444ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004444
	.4byte 0x0444ffff
	.4byte 0x4fffffff
	.4byte 0xffff4444
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000044f
	.4byte 0xfff40000
	.4byte 0xffff4000
	.4byte 0x4ffff400
	.4byte 0x04ffff40
	.4byte 0x004fff40
	.4byte 0x004ffff4
	.4byte 0x0004fff4
	.4byte 0x0004fff4
	.4byte 0x000044ff
	.4byte 0x00000044
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xff440000
	.4byte 0x44000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004fff
	.4byte 0x0004ffff
	.4byte 0x004ffff4
	.4byte 0x04ffff40
	.4byte 0x04fff400
	.4byte 0x4ffff400
	.4byte 0x4fff4000
	.4byte 0x4fff4000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedeb0000
	.4byte 0x0000ebed
	.4byte 0xeb000000
	.4byte 0x000000eb
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xefed0000
	.4byte 0x0000edef
	.4byte 0xedebeb00
	.4byte 0x00ebebed
	.4byte 0x00ebe9e9
	.4byte 0xe9e9eb00
	.4byte 0x0000e900
	.4byte 0x00e90000
	.4byte 0xe9000000
	.4byte 0x000000e9
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xeeedebe9
	.4byte 0xe9ebedee
	.4byte 0xedebe900
	.4byte 0x00e9ebed
	.4byte 0xebe90000
	.4byte 0x0000e9eb
	.4byte 0xe9000000
	.4byte 0x000000e9
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x00000040
	.4byte 0xc0000038
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000070
	.4byte 0xc0000038
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000040
	.4byte 0x40000078
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000070
	.4byte 0xc0000078
	.4byte 0x00100000
	.4byte 0x01300010
	.4byte 0x000000e0
	.4byte 0xffff0004
	.4byte 0x00000158
	.4byte 0x00000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000058
	.4byte 0x000000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x000000b8
	.4byte 0x000000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000001a8
	.4byte 0x00000088
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000000b8
	.4byte 0x000000c8
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
	.4byte 0x00000142
	.4byte 0x01402142
	.4byte 0x000001ff
	.global Data_02001300
Data_02001300:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00280000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00280000
	.4byte 0x00004000
	.4byte 0xffff0047
	.4byte 0x00000001
	.4byte 0x00520000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff00f5
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00280000
	.4byte 0x00004000
	.4byte 0xffff0133
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00480000
	.4byte 0x00004000
	.4byte 0xffff012b
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0038
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0xffc00000
	.4byte 0x00000000
	.4byte 0xffc00000
	.4byte 0x00024000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte Data_02000054 + 0x1
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte Data_02000054 + 0x1
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte Func_02000748
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_020001a4
	.4byte 0x00008400
	.4byte 0xffff0008
	.4byte Func_020001b0
	.4byte 0x00000400
	.4byte 0xffff0008
	.4byte Func_020001bc
	.4byte 0x00004400
	.4byte 0xffff0008
	.4byte Func_020001c8
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Data_020001e0 + 0x1
	.4byte 0x00004400
	.4byte 0xffff000a
	.4byte Func_020001d4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000924
	.4byte 0x0000c400
	.4byte 0xffff000b
	.4byte Func_02000140
	.4byte 0x00008400
	.4byte 0xffff000b
	.4byte Func_02000158
	.4byte 0x00000400
	.4byte 0xffff000b
	.4byte Func_02000170
	.4byte 0x00004400
	.4byte 0xffff000b
	.4byte Func_0200018c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Data_02000054 + 0x1
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_0200091c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Data_02000054 + 0x1
	.4byte 0x0000c400
	.4byte 0xffff000f
	.4byte Func_02000068
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000058
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_0200070c
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002535
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00002536
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00002537
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00002538
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00002539
	.4byte 0x00000001
	.4byte 0xffff0014
	.4byte 0x00000014
	.4byte 0x00000003
	.4byte 0xffff000a
	.4byte Func_0200026c
	.4byte 0x00000003
	.4byte 0xffff000b
	.4byte Func_020001e4
	.4byte 0x00000003
	.4byte 0xffff000c
	.4byte Func_02000224
	.4byte 0x00000003
	.4byte 0xffff000d
	.4byte Func_02000330
	.4byte 0x00000013
	.4byte 0xffff0064
	.4byte 0x0020014d
	.4byte 0x00008e15
	.4byte 0xffff000e
	.4byte Data_02000074 + 0x1
	.4byte 0x00008e15
	.4byte 0xffff000f
	.4byte Data_02000074 + 0x1
	.4byte 0x00004e15
	.4byte 0xffff000f
	.4byte Data_02000074 + 0x1
	.4byte 0x00009415
	.4byte 0xffff000f
	.4byte Data_02000074 + 0x1
	.4byte 0x00001815
	.4byte 0xffff0012
	.4byte Data_02000054 + 0x1
	.4byte 0x50008615
	.4byte 0xffff0015
	.4byte Func_020005dc
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_02000444
	.4byte 0x40008b85
	.4byte 0xffff0000
	.4byte Func_02000454
	.4byte 0x10009a15
	.4byte 0xffff0010
	.4byte Data_02000054 + 0x1
	.4byte 0x60009a15
	.4byte 0xffff0011
	.4byte Func_02000464
	.4byte 0x20009a15
	.4byte 0xffff0011
	.4byte Func_02000508
	.4byte 0x10009a15
	.4byte 0xffff0011
	.4byte Data_02000054 + 0x1
	.4byte 0x20009d15
	.4byte 0xffff0011
	.4byte Func_0200055c
	.4byte 0x20009d15
	.4byte 0xffff0015
	.4byte Func_020005b0
	.4byte 0x50008805
	.4byte 0x03000032
	.4byte Func_02000688
	.4byte 0x50008805
	.4byte 0x03010033
	.4byte Func_020006b0
	.4byte 0x50008805
	.4byte 0x03020034
	.4byte Func_020006cc
	.4byte 0x50008805
	.4byte 0x03030035
	.4byte Func_020006e8
	.4byte 0x50008a05
	.4byte 0xffff003c
	.4byte Func_02000724
	.4byte 0x50008905
	.4byte 0xffff0037
	.4byte Func_02000604
	.4byte 0x50008905
	.4byte 0xffff0038
	.4byte Func_02000624
	.4byte 0x50008905
	.4byte 0xffff000d
	.4byte Func_02000650
	.4byte 0x50008905
	.4byte 0xffff0039
	.4byte Func_02000660
	.4byte 0x00008715
	.4byte 0xffff000d
	.4byte Data_02000740 + 0x1
	.4byte 0x00008715
	.4byte 0xffff000f
	.4byte Data_02000740 + 0x1
	.4byte 0x00009815
	.4byte 0xffff000b
	.4byte Data_02000744 + 0x1
	.4byte 0x00009815
	.4byte 0xffff000f
	.4byte Data_02000744 + 0x1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 0x0000000c
	.global Data_020017b0
Data_020017b0:
	.space 0x000000b0
	.global Data_02001860
Data_02001860:
