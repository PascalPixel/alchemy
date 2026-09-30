.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_02008078
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_02008078:
	.4byte 0x00001dfd
	.section .text.x0200807c,"ax",%progbits
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_020080a0
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020080a0:
	.4byte 0x00001e25
	.section .text.x020080a4,"ax",%progbits
	.global Func_020000a4
	.thumb_func
Func_020000a4:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080b8
	ldr r0, .L_020080bc
	b .L_020080ba
.L_020080b8:
	ldr r0, .L_020080c0
.L_020080ba:
	pop {pc}
.L_020080bc:
	.4byte Data_020014c0
.L_020080c0:
	.4byte Data_02001370
	.section .text.x020080c4,"ax",%progbits
	.global Func_020000c4
	.thumb_func
Func_020000c4:
	push {r5, lr}
	ldr r3, .L_020080fc
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
	ldr r2, .L_020080f8
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008100
	movs r0, #14
	adds r1, r5, #0
	bl Func_02001190
	b .L_0200811c
	.2byte 0x0000
.L_020080f8:
	.4byte 0xffffc000
.L_020080fc:
	.4byte gPartyState
.L_02008100:
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_02008120
	bl Func_02001138
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
.L_0200811c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008120:
	.4byte 0x00001e00
	.section .text.x02008124,"ax",%progbits
	.global Func_02000124
	.thumb_func
Func_02000124:
	push {r5, lr}
	ldr r3, .L_0200815c
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
	ldr r2, .L_02008158
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008160
	movs r0, #14
	adds r1, r5, #0
	bl Func_02001190
	b .L_0200817c
	.2byte 0x0000
.L_02008158:
	.4byte 0xffffc000
.L_0200815c:
	.4byte gPartyState
.L_02008160:
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_02008180
	bl Func_02001138
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
.L_0200817c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008180:
	.4byte 0x00001e28
	.section .text.x02008184,"ax",%progbits
	.global Func_02000184
	.thumb_func
Func_02000184:
	push {r5, lr}
	ldr r3, .L_020081bc
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
	ldr r2, .L_020081b8
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020081c0
	movs r0, #15
	adds r1, r5, #0
	bl Func_02001190
	b .L_020081dc
	.2byte 0x0000
.L_020081b8:
	.4byte 0xffffc000
.L_020081bc:
	.4byte gPartyState
.L_020081c0:
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_020081e0
	bl Func_02001138
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
.L_020081dc:
	pop {r5, pc}
	.2byte 0x0000
.L_020081e0:
	.4byte 0x00001e02
	.section .text.x020081e4,"ax",%progbits
	.global Func_020001e4
	.thumb_func
Func_020001e4:
	push {r5, lr}
	ldr r3, .L_0200821c
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
	ldr r2, .L_02008218
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008220
	movs r0, #15
	adds r1, r5, #0
	bl Func_02001190
	b .L_0200823c
	.2byte 0x0000
.L_02008218:
	.4byte 0xffffc000
.L_0200821c:
	.4byte gPartyState
.L_02008220:
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_02008240
	bl Func_02001138
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
.L_0200823c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008240:
	.4byte 0x00001e2a
	.section .text.x02008244,"ax",%progbits
	.global Func_02000244
	.thumb_func
Func_02000244:
	push {r5, lr}
	ldr r3, .L_0200827c
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
	ldr r2, .L_02008278
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008280
	movs r0, #5
	adds r1, r5, #0
	bl Func_02001198
	b .L_0200829c
	.2byte 0x0000
.L_02008278:
	.4byte 0xffffc000
.L_0200827c:
	.4byte gPartyState
.L_02008280:
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_020082a0
	bl Func_02001138
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
.L_0200829c:
	pop {r5, pc}
	.2byte 0x0000
.L_020082a0:
	.4byte 0x00001e07
	.section .text.x020082a4,"ax",%progbits
	.global Func_020002a4
	.thumb_func
Func_020002a4:
	push {r5, lr}
	ldr r3, .L_020082dc
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
	ldr r2, .L_020082d8
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020082e0
	movs r0, #5
	adds r1, r5, #0
	bl Func_02001198
	b .L_020082fc
	.2byte 0x0000
.L_020082d8:
	.4byte 0xffffc000
.L_020082dc:
	.4byte gPartyState
.L_020082e0:
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r0, .L_02008300
	bl Func_02001138
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
.L_020082fc:
	pop {r5, pc}
	.2byte 0x0000
.L_02008300:
	.4byte 0x00001e2f
	.section .text.x02008304,"ax",%progbits
	.global Func_02000304
	.thumb_func
Func_02000304:
	push {r5, lr}
	bl Func_020010e0
	movs r0, #0
	bl Func_02001180
	ldr r5, .L_02008384
	adds r0, r5, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	bl Func_02001188
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008370
	adds r0, r5, #2
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	bl Func_02001188
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008370
	ldr r3, .L_02008388
	ldr r3, [r3, #16]
	cmp r3, #19
	bls .L_0200835c
	bl Func_02000390
	bl Func_020010e8
	b .L_02008382
.L_0200835c:
	adds r0, r5, #4
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
	b .L_02008382
.L_02008370:
	ldr r0, .L_0200838c
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_020010e8
.L_02008382:
	pop {r5, pc}
.L_02008384:
	.4byte 0x00001daa
.L_02008388:
	.4byte gPartyState
.L_0200838c:
	.4byte 0x00001dab
	.section .text.x02008390,"ax",%progbits
	.global Func_02000390
	.thumb_func
Func_02000390:
	push {r5, r6, r7, lr}
	ldr r0, .L_02008728
	sub sp, #8
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #30
	bl Battle_WaitMode0
.L_020083a8:
	ldr r3, .L_0200872c
	movs r1, #133
	lsls r1, r1, #2
	adds r7, r3, r1
	ldr r0, [r7]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	add r0, sp, #4
	mov r1, sp
	bl Func_020011a0
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_020083ea
	ldr r5, .L_02008730
	adds r0, r5, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	bl Func_02001188
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_020083a8
	adds r0, r5, #1
	b .L_02008496
.L_020083ea:
	ldr r0, [sp, #4]
	bl Owner_GetState
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	movs r0, #18
	bl Object_GetById
	ldr r3, [sp, #0]
	lsls r3, r3, #1
	adds r3, #216
	ldrh r1, [r5, r3]
	bl Func_020010a0
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	lsls r1, r1, #18
	movs r0, #18
	bl Func_02001108
	movs r0, #60
	bl Battle_WaitMode0
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	ldr r0, .L_02008734
	bl UiText_ShowPositionedMessageAndWait
	ldr r6, .L_02008738
	adds r0, r6, #0
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	bl Func_02001188
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_020084a4
	movs r2, #0
	movs r0, #18
	movs r1, #0
	bl Func_02001108
	ldr r3, [sp, #0]
	movs r1, #2
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b0
	movs r1, #1
	adds r0, r6, #1
	bl UiText_ShowPositionedMessageAndWait
	adds r0, r6, #2
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl UiText_OpenMessageAtObject
	bl Func_02001188
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #1
	bne .L_020083a8
	adds r0, r6, #3
.L_02008496:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_0200875a
.L_020084a4:
	adds r0, r6, #4
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #128
	movs r1, #0
	lsls r0, r0, #9
	bl Func_02001170
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #1
	ldr r0, .L_0200873c
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #11
	bl Func_020011a8
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #10
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #166
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r1, #5
	movs r0, #10
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	adds r3, r5, #0
	ands r3, r2
	movs r1, #170
	strb r3, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #4
	movs r2, #90
	movs r0, #10
	bl ObjectMotion_Launch
	movs r0, #10
	bl Object_GetById
	adds r0, #90
	ldrb r3, [r0]
	movs r1, #168
	ands r5, r3
	strb r5, [r0]
	movs r2, #96
	movs r0, #10
	lsls r1, r1, #2
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r0, #10
	movs r1, #4
	movs r2, #30
	bl ObjectMotion_Launch
	movs r2, #30
	movs r0, #10
	movs r1, #4
	bl ObjectMotion_Launch
	movs r0, #10
	movs r1, #5
	bl Object_SetModeById
	movs r1, #0
	movs r0, #10
	bl Object_SetModeById
	movs r0, #11
	bl Func_020011a8
	movs r0, #128
	movs r1, #1
	lsls r0, r0, #9
	bl Func_02001168
	movs r0, #30
	bl Func_02001178
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	lsls r1, r1, #18
	movs r0, #20
	bl Func_02001108
	movs r0, #144
	bl Func_020011a8
	movs r1, #2
	movs r0, #20
	bl Object_SetModeById
	movs r0, #20
	bl ObjectMotion_WaitForAnimationChange
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl Func_02001108
	movs r0, #4
	bl Battle_WaitMode0
	movs r1, #168
	movs r2, #208
	lsls r2, r2, #15
	movs r0, #19
	lsls r1, r1, #18
	bl Func_02001108
	movs r1, #4
	movs r0, #19
	bl Object_SetModeById
	movs r0, #19
	bl ObjectMotion_WaitForAnimationChange
	movs r0, #15
	bl Battle_WaitMode0
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Func_02001108
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #0
	ldr r0, [r7]
	bl Func_02001160
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #10
	bl Func_020011a8
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #10
	bl Func_02001160
	movs r1, #0
	movs r0, #10
	bl Func_02001148
	ldr r0, [sp, #4]
	bl Owner_GetState
	ldr r3, [sp, #0]
	adds r5, r0, #0
	lsls r3, r3, #1
	adds r3, #216
	ldrh r0, [r5, r3]
	bl Func_020010b8
	ldr r3, [sp, #0]
	movs r2, #128
	lsls r3, r3, #1
	adds r3, #216
	ldrh r3, [r5, r3]
	ldr r1, .L_02008740
	lsls r2, r2, #1
	adds r2, #255
	ands r2, r3
	adds r3, r2, r1
	movs r1, #128
	lsls r3, r3, #16
	lsls r1, r1, #10
	cmp r3, r1
	bhi .L_020086c0
	bl Func_02000920
	b .L_02008748
.L_020086c0:
	movs r3, #163
	lsls r3, r3, #1
	cmp r2, r3
	bne .L_020086fc
	adds r0, r6, #0
	adds r0, #29
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086ea
	adds r0, r6, #0
	adds r0, #30
	b .L_020086ee
.L_020086ea:
	adds r0, r6, #0
	adds r0, #31
.L_020086ee:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_02008748
.L_020086fc:
	ldrb r0, [r0, #2]
	adds r2, r0, #0
	cmp r2, #6
	bne .L_0200870a
	bl Func_020007b4
	b .L_02008748
.L_0200870a:
	cmp r2, #1
	bne .L_02008714
	bl Func_02000760
	b .L_02008748
.L_02008714:
	adds r3, r0, #0
	adds r3, #254
	movs r1, #192
	lsls r3, r3, #24
	lsls r1, r1, #18
	cmp r3, r1
	bhi .L_02008744
	bl Func_02000838
	b .L_02008748
.L_02008728:
	.4byte 0x00001daf
.L_0200872c:
	.4byte gPartyState
.L_02008730:
	.4byte 0x00001db2
.L_02008734:
	.4byte 0x00001da9
.L_02008738:
	.4byte 0x00001db0
.L_0200873c:
	.4byte 0x0040250d
.L_02008740:
	.4byte 0xfffffe49
.L_02008744:
	bl Func_02000878
.L_02008748:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001108
	movs r0, #20
	negs r0, r0
	bl Party_AdjustSixDigitCounterA
.L_0200875a:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008760,"ax",%progbits
	.global Func_02000760
	.thumb_func
Func_02000760:
	push {r5, lr}
	ldr r5, .L_020087b0
	adds r0, r5, #0
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_0200097c
	cmp r0, #1
	beq .L_0200878e
	cmp r0, #1
	bgt .L_02008784
	cmp r0, #0
	beq .L_0200878a
	b .L_020087ac
.L_02008784:
	cmp r0, #2
	beq .L_0200879e
	b .L_020087ac
.L_0200878a:
	adds r0, r5, #1
	b .L_02008790
.L_0200878e:
	adds r0, r5, #2
.L_02008790:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_020087ac
.L_0200879e:
	adds r0, r5, #3
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
.L_020087ac:
	pop {r5, pc}
	.2byte 0x0000
.L_020087b0:
	.4byte 0x00001db6
	.section .text.x020087b4,"ax",%progbits
	.global Func_020007b4
	.thumb_func
Func_020007b4:
	push {r5, lr}
	ldr r5, .L_0200881c
	adds r0, r5, #0
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_020009a4
	subs r0, #3
	cmp r0, #4
	bhi .L_0200880c
	ldr r2, .L_02008820
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_020087d8:
	.4byte .L_020087ec
	.4byte .L_020087f0
	.4byte .L_020087f4
	.4byte .L_020087f8
	.4byte .L_020087fc
.L_020087ec:
	ldr r0, .L_02008824
	b .L_020087fe
.L_020087f0:
	ldr r0, .L_02008828
	b .L_020087fe
.L_020087f4:
	ldr r0, .L_0200882c
	b .L_020087fe
.L_020087f8:
	ldr r0, .L_02008830
	b .L_020087fe
.L_020087fc:
	ldr r0, .L_02008834
.L_020087fe:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_0200881a
.L_0200880c:
	adds r0, r5, #6
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
.L_0200881a:
	pop {r5, pc}
.L_0200881c:
	.4byte 0x00001dba
.L_02008820:
	.4byte .L_020087d8
.L_02008824:
	.4byte 0x00001dbb
.L_02008828:
	.4byte 0x00001dbc
.L_0200882c:
	.4byte 0x00001dbd
.L_02008830:
	.4byte 0x00001dbe
.L_02008834:
	.4byte 0x00001dbf
	.section .text.x02008838,"ax",%progbits
	.global Func_02000838
	.thumb_func
Func_02000838:
	push {r5, lr}
	bl Func_02000c04
	adds r5, r0, #0
	ldr r0, .L_02008874
	cmp r5, r0
	bne .L_02008856
	adds r0, r5, #0
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_02008872
.L_02008856:
	subs r0, #42
	bl Func_02001138
	movs r1, #0
	movs r0, #10
	bl Func_02001148
	adds r0, r5, #0
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
.L_02008872:
	pop {r5, pc}
.L_02008874:
	.4byte 0x00001dfa
	.section .text.x02008878,"ax",%progbits
	.global Func_02000878
	.thumb_func
Func_02000878:
	push {lr}
	ldr r0, .L_02008900
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_020009f8
	cmp r0, #13
	bhi .L_020088fe
	ldr r2, .L_02008904
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008898:
	.4byte .L_020088d0
	.4byte .L_020088fe
	.4byte .L_020088fe
	.4byte .L_020088fe
	.4byte .L_020088fe
	.4byte .L_020088fe
	.4byte .L_020088fe
	.4byte .L_020088fe
	.4byte .L_020088fe
	.4byte .L_020088d4
	.4byte .L_020088d8
	.4byte .L_020088dc
	.4byte .L_020088e0
	.4byte .L_020088f0
.L_020088d0:
	ldr r0, .L_02008908
	b .L_020088e2
.L_020088d4:
	ldr r0, .L_0200890c
	b .L_020088e2
.L_020088d8:
	ldr r0, .L_02008910
	b .L_020088e2
.L_020088dc:
	ldr r0, .L_02008914
	b .L_020088e2
.L_020088e0:
	ldr r0, .L_02008918
.L_020088e2:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_020088fe
.L_020088f0:
	ldr r0, .L_0200891c
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
.L_020088fe:
	pop {pc}
.L_02008900:
	.4byte 0x00001dc1
.L_02008904:
	.4byte .L_02008898
.L_02008908:
	.4byte 0x00001dc2
.L_0200890c:
	.4byte 0x00001dc3
.L_02008910:
	.4byte 0x00001dc4
.L_02008914:
	.4byte 0x00001dc5
.L_02008918:
	.4byte 0x00001dc6
.L_0200891c:
	.4byte 0x00001dc7
	.section .text.x02008920,"ax",%progbits
	.global Func_02000920
	.thumb_func
Func_02000920:
	push {r5, lr}
	ldr r5, .L_02008978
	adds r0, r5, #0
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	bl Func_02000a7c
	cmp r0, #15
	beq .L_02008952
	cmp r0, #15
	bgt .L_02008944
	cmp r0, #14
	beq .L_0200894e
	b .L_02008974
.L_02008944:
	cmp r0, #16
	beq .L_02008956
	cmp r0, #17
	beq .L_02008966
	b .L_02008974
.L_0200894e:
	adds r0, r5, #1
	b .L_02008958
.L_02008952:
	adds r0, r5, #2
	b .L_02008958
.L_02008956:
	adds r0, r5, #3
.L_02008958:
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
	b .L_02008974
.L_02008966:
	adds r0, r5, #4
	bl Func_02001138
	movs r0, #10
	movs r1, #0
	bl Func_02001148
.L_02008974:
	pop {r5, pc}
	.2byte 0x0000
.L_02008978:
	.4byte 0x00001dc8
	.section .text.x0200897c,"ax",%progbits
	.global Func_0200097c
	.thumb_func
Func_0200097c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008990
	movs r0, #0
	b .L_020089a2
.L_02008990:
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_Test
	adds r3, r0, #0
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	adds r0, #1
.L_020089a2:
	pop {pc}
	.section .text.x020089a4,"ax",%progbits
	.global Func_020009a4
	.thumb_func
Func_020009a4:
	push {r5, lr}
	movs r0, #144
	bl Func_020010d0
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	bne .L_020089b8
	movs r0, #3
	b .L_020089f4
.L_020089b8:
	movs r0, #139
	bl Func_020010d0
	cmp r0, r5
	bne .L_020089c6
	movs r0, #4
	b .L_020089f4
.L_020089c6:
	movs r0, #138
	bl Func_020010d0
	cmp r0, r5
	bne .L_020089d4
	movs r0, #5
	b .L_020089f4
.L_020089d4:
	movs r0, #213
	bl PartyInventory_FindOwner
	cmp r0, r5
	bne .L_020089e2
	movs r0, #6
	b .L_020089f4
.L_020089e2:
	movs r0, #214
	bl PartyInventory_FindOwner
	adds r3, r0, #0
	mvns r3, r3
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	adds r0, #7
.L_020089f4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020089f8,"ax",%progbits
	.global Func_020009f8
	.thumb_func
Func_020009f8:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a0c
	movs r0, #0
	b .L_02008a7a
.L_02008a0c:
	movs r0, #163
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_02008a68
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a78
	movs r0, #184
	adds r0, #255
	bl PartyInventory_FindOwner
	cmp r0, r5
	beq .L_02008a4e
	movs r0, #220
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	cmp r0, r5
	beq .L_02008a4e
	movs r0, #186
	adds r0, #255
	bl PartyInventory_FindOwner
	cmp r0, r5
	bne .L_02008a52
.L_02008a4e:
	movs r0, #9
	b .L_02008a7a
.L_02008a52:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #5
	bl GameFlag_Test
	adds r3, r0, #0
	negs r0, r3
	orrs r0, r3
	lsrs r0, r0, #31
	adds r0, #10
	b .L_02008a7a
.L_02008a68:
	movs r0, #148
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008a78
	movs r0, #12
	b .L_02008a7a
.L_02008a78:
	movs r0, #13
.L_02008a7a:
	pop {r5, pc}
	.section .text.x02008a7c,"ax",%progbits
	.global Func_02000a7c
	.thumb_func
Func_02000a7c:
	push {r5, r6, lr}
	movs r0, #184
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_02008aae
	movs r0, #186
	adds r0, #255
	bl PartyInventory_FindOwner
	adds r6, r0, #0
	cmp r6, r5
	bne .L_02008aaa
	movs r0, #220
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	cmp r0, r6
	beq .L_02008aae
.L_02008aaa:
	movs r0, #14
	b .L_02008b5c
.L_02008aae:
	movs r0, #184
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r6, #1
	negs r6, r6
	cmp r0, r6
	beq .L_02008ad8
	movs r0, #220
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	adds r5, r0, #0
	cmp r5, r6
	bne .L_02008ad8
	movs r0, #186
	adds r0, #255
	bl PartyInventory_FindOwner
	cmp r0, r5
	beq .L_02008b02
.L_02008ad8:
	movs r0, #184
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r6, #1
	negs r6, r6
	cmp r0, r6
	beq .L_02008b06
	movs r0, #220
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	adds r5, r0, #0
	cmp r5, r6
	bne .L_02008b06
	movs r0, #186
	adds r0, #255
	bl PartyInventory_FindOwner
	cmp r0, r5
	beq .L_02008b06
.L_02008b02:
	movs r0, #15
	b .L_02008b5c
.L_02008b06:
	movs r0, #184
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	beq .L_02008b32
	movs r0, #220
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	cmp r0, r5
	beq .L_02008b32
	movs r0, #186
	adds r0, #255
	bl PartyInventory_FindOwner
	cmp r0, r5
	bne .L_02008b32
	movs r0, #16
	b .L_02008b5c
.L_02008b32:
	movs r0, #184
	adds r0, #255
	bl PartyInventory_FindOwner
	movs r5, #1
	negs r5, r5
	cmp r0, r5
	beq .L_02008b5c
	movs r0, #220
	lsls r0, r0, #1
	bl PartyInventory_FindOwner
	cmp r0, r5
	beq .L_02008b5c
	movs r0, #186
	adds r0, #255
	bl PartyInventory_FindOwner
	cmp r0, r5
	beq .L_02008b5c
	movs r0, #17
.L_02008b5c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008b60,"ax",%progbits
	.global Func_02000b60
	.thumb_func
Func_02000b60:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r5, #17
.L_02008b66:
	lsls r3, r6, #2
	adds r3, r3, r6
	lsls r3, r3, #2
	adds r3, r3, r5
	adds r0, r3, #0
	adds r0, #48
	cmp r6, #0
	bne .L_02008b7a
	cmp r5, #7
	beq .L_02008b92
.L_02008b7a:
	cmp r6, #1
	bne .L_02008b86
	cmp r5, #9
	beq .L_02008b92
	cmp r5, #10
	beq .L_02008b92
.L_02008b86:
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b92
	adds r0, r5, #1
	b .L_02008b9c
.L_02008b92:
	subs r5, #1
	cmp r5, #6
	bgt .L_02008b66
	movs r0, #1
	negs r0, r0
.L_02008b9c:
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x02008ba0,"ax",%progbits
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #0
	mov r10, r0
	movs r5, #7
	sub sp, #4
	adds r7, r1, #0
	mov r8, r3
	cmp r5, r10
	bge .L_02008bf6
	str r2, [sp, #0]
.L_02008bba:
	cmp r7, #0
	bne .L_02008bc2
	cmp r5, #7
	beq .L_02008bf0
.L_02008bc2:
	cmp r7, #1
	bne .L_02008bce
	cmp r5, #9
	beq .L_02008bf0
	cmp r5, #10
	beq .L_02008bf0
.L_02008bce:
	lsls r3, r7, #2
	adds r3, r3, r7
	lsls r3, r3, #2
	adds r3, r3, r5
	adds r6, r3, #0
	adds r6, #48
	adds r0, r6, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008bf0
	ldr r3, [sp, #0]
	stmia r3!, {r6}
	adds r2, r3, #0
	str r2, [sp, #0]
	movs r2, #1
	add r8, r2
.L_02008bf0:
	adds r5, #1
	cmp r5, r10
	blt .L_02008bba
.L_02008bf6:
	mov r0, r8
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008c04,"ax",%progbits
	.global Func_02000c04
	.thumb_func
Func_02000c04:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r1, #160
	movs r0, #252
	lsls r1, r1, #1
	sub sp, #16
	bl Runtime_AllocateBlock
	movs r3, #156
	lsls r3, r3, #6
	movs r6, #0
	adds r3, #15
	adds r7, r0, #0
	mov r8, r3
	mov r10, r6
	movs r5, #0
.L_02008c28:
	adds r0, r5, #0
	bl Func_02000b60
	lsls r3, r5, #2
	mov r2, sp
	str r0, [r2, r3]
	cmp r8, r0
	ble .L_02008c3c
	mov r8, r0
	mov r10, r5
.L_02008c3c:
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02008c50
	lsls r2, r6, #2
	adds r2, r7, r2
	adds r1, r5, #0
	bl Func_02000ba0
	adds r6, r6, r0
.L_02008c50:
	adds r5, #1
	cmp r5, #3
	bls .L_02008c28
	cmp r6, #0
	bne .L_02008c8c
	movs r5, #0
.L_02008c5c:
	lsls r2, r6, #2
	adds r1, r5, #0
	adds r2, r7, r2
	movs r0, #18
	bl Func_02000ba0
	adds r5, #1
	adds r6, r6, r0
	cmp r5, #3
	bls .L_02008c5c
	cmp r6, #0
	bne .L_02008c7e
	movs r0, #252
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_02008e7c
	b .L_02008e72
.L_02008c7e:
	movs r0, #18
	mov r1, r10
	adds r2, r7, #0
	bl Func_02000ba0
	movs r0, #0
	b .L_02008c96
.L_02008c8c:
	bl Random16Far
	adds r1, r6, #0
	bl Engine_MathModulo
.L_02008c96:
	lsls r3, r0, #2
	ldr r3, [r3, r7]
	adds r0, r3, #0
	subs r0, #56
	cmp r0, #69
	bls .L_02008ca4
	b .L_02008e68
.L_02008ca4:
	ldr r2, .L_02008e80
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008cac:
	.4byte .L_02008dc4
	.4byte .L_02008dc8
	.4byte .L_02008dcc
	.4byte .L_02008dd0
	.4byte .L_02008dd4
	.4byte .L_02008dd8
	.4byte .L_02008ddc
	.4byte .L_02008de0
	.4byte .L_02008de4
	.4byte .L_02008de8
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008dec
	.4byte .L_02008df0
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008df4
	.4byte .L_02008df8
	.4byte .L_02008dfc
	.4byte .L_02008e00
	.4byte .L_02008e04
	.4byte .L_02008e08
	.4byte .L_02008e0c
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e10
	.4byte .L_02008e14
	.4byte .L_02008e18
	.4byte .L_02008e1c
	.4byte .L_02008e20
	.4byte .L_02008e24
	.4byte .L_02008e28
	.4byte .L_02008e2c
	.4byte .L_02008e30
	.4byte .L_02008e34
	.4byte .L_02008e38
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e68
	.4byte .L_02008e3c
	.4byte .L_02008e40
	.4byte .L_02008e44
	.4byte .L_02008e48
	.4byte .L_02008e4c
	.4byte .L_02008e50
	.4byte .L_02008e54
	.4byte .L_02008e58
	.4byte .L_02008e5c
	.4byte .L_02008e60
	.4byte .L_02008e64
.L_02008dc4:
	ldr r5, .L_02008e84
	b .L_02008e6a
.L_02008dc8:
	ldr r5, .L_02008e88
	b .L_02008e6a
.L_02008dcc:
	ldr r5, .L_02008e8c
	b .L_02008e6a
.L_02008dd0:
	ldr r5, .L_02008e90
	b .L_02008e6a
.L_02008dd4:
	ldr r5, .L_02008e94
	b .L_02008e6a
.L_02008dd8:
	ldr r5, .L_02008e98
	b .L_02008e6a
.L_02008ddc:
	ldr r5, .L_02008e9c
	b .L_02008e6a
.L_02008de0:
	ldr r5, .L_02008ea0
	b .L_02008e6a
.L_02008de4:
	ldr r5, .L_02008ea4
	b .L_02008e6a
.L_02008de8:
	ldr r5, .L_02008ea8
	b .L_02008e6a
.L_02008dec:
	ldr r5, .L_02008eac
	b .L_02008e6a
.L_02008df0:
	ldr r5, .L_02008eb0
	b .L_02008e6a
.L_02008df4:
	ldr r5, .L_02008eb4
	b .L_02008e6a
.L_02008df8:
	ldr r5, .L_02008eb8
	b .L_02008e6a
.L_02008dfc:
	ldr r5, .L_02008ebc
	b .L_02008e6a
.L_02008e00:
	ldr r5, .L_02008ec0
	b .L_02008e6a
.L_02008e04:
	ldr r5, .L_02008ec4
	b .L_02008e6a
.L_02008e08:
	ldr r5, .L_02008ec8
	b .L_02008e6a
.L_02008e0c:
	ldr r5, .L_02008ecc
	b .L_02008e6a
.L_02008e10:
	ldr r5, .L_02008ed0
	b .L_02008e6a
.L_02008e14:
	ldr r5, .L_02008ed4
	b .L_02008e6a
.L_02008e18:
	ldr r5, .L_02008ed8
	b .L_02008e6a
.L_02008e1c:
	ldr r5, .L_02008edc
	b .L_02008e6a
.L_02008e20:
	ldr r5, .L_02008ee0
	b .L_02008e6a
.L_02008e24:
	ldr r5, .L_02008ee4
	b .L_02008e6a
.L_02008e28:
	ldr r5, .L_02008ee8
	b .L_02008e6a
.L_02008e2c:
	ldr r5, .L_02008eec
	b .L_02008e6a
.L_02008e30:
	ldr r5, .L_02008ef0
	b .L_02008e6a
.L_02008e34:
	ldr r5, .L_02008ef4
	b .L_02008e6a
.L_02008e38:
	ldr r5, .L_02008ef8
	b .L_02008e6a
.L_02008e3c:
	ldr r5, .L_02008efc
	b .L_02008e6a
.L_02008e40:
	ldr r5, .L_02008f00
	b .L_02008e6a
.L_02008e44:
	ldr r5, .L_02008f04
	b .L_02008e6a
.L_02008e48:
	ldr r5, .L_02008f08
	b .L_02008e6a
.L_02008e4c:
	ldr r5, .L_02008f0c
	b .L_02008e6a
.L_02008e50:
	ldr r5, .L_02008f10
	b .L_02008e6a
.L_02008e54:
	ldr r5, .L_02008f14
	b .L_02008e6a
.L_02008e58:
	ldr r5, .L_02008f18
	b .L_02008e6a
.L_02008e5c:
	ldr r5, .L_02008f1c
	b .L_02008e6a
.L_02008e60:
	ldr r5, .L_02008f20
	b .L_02008e6a
.L_02008e64:
	ldr r5, .L_02008f24
	b .L_02008e6a
.L_02008e68:
	ldr r5, .L_02008e7c
.L_02008e6a:
	movs r0, #252
	bl Runtime_ReleaseHeapBlock
	adds r0, r5, #0
.L_02008e72:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008e7c:
	.4byte 0x00001dfa
.L_02008e80:
	.4byte .L_02008cac
.L_02008e84:
	.4byte 0x00001dd1
.L_02008e88:
	.4byte 0x00001dd2
.L_02008e8c:
	.4byte 0x00001dd3
.L_02008e90:
	.4byte 0x00001dd4
.L_02008e94:
	.4byte 0x00001dd5
.L_02008e98:
	.4byte 0x00001dd6
.L_02008e9c:
	.4byte 0x00001dd7
.L_02008ea0:
	.4byte 0x00001dd8
.L_02008ea4:
	.4byte 0x00001dd9
.L_02008ea8:
	.4byte 0x00001dda
.L_02008eac:
	.4byte 0x00001ddb
.L_02008eb0:
	.4byte 0x00001ddc
.L_02008eb4:
	.4byte 0x00001ddd
.L_02008eb8:
	.4byte 0x00001dde
.L_02008ebc:
	.4byte 0x00001ddf
.L_02008ec0:
	.4byte 0x00001de0
.L_02008ec4:
	.4byte 0x00001de1
.L_02008ec8:
	.4byte 0x00001de2
.L_02008ecc:
	.4byte 0x00001de3
.L_02008ed0:
	.4byte 0x00001de4
.L_02008ed4:
	.4byte 0x00001de5
.L_02008ed8:
	.4byte 0x00001de6
.L_02008edc:
	.4byte 0x00001de7
.L_02008ee0:
	.4byte 0x00001de8
.L_02008ee4:
	.4byte 0x00001de9
.L_02008ee8:
	.4byte 0x00001dea
.L_02008eec:
	.4byte 0x00001deb
.L_02008ef0:
	.4byte 0x00001dec
.L_02008ef4:
	.4byte 0x00001ded
.L_02008ef8:
	.4byte 0x00001dee
.L_02008efc:
	.4byte 0x00001def
.L_02008f00:
	.4byte 0x00001df0
.L_02008f04:
	.4byte 0x00001df1
.L_02008f08:
	.4byte 0x00001df2
.L_02008f0c:
	.4byte 0x00001df3
.L_02008f10:
	.4byte 0x00001df4
.L_02008f14:
	.4byte 0x00001df5
.L_02008f18:
	.4byte 0x00001df6
.L_02008f1c:
	.4byte 0x00001df7
.L_02008f20:
	.4byte 0x00001df8
.L_02008f24:
	.4byte 0x00001df9
	.section .text.x02008f28,"ax",%progbits
	.global Func_02000f28
	.thumb_func
Func_02000f28:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	ldr r3, .L_0200905c
	adds r2, #11
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #32
	orrs r3, r6
	movs r2, #0
	strb r3, [r0]
	movs r0, #17
	mov r8, r2
	bl Func_02001158
	movs r0, #18
	bl Func_02001158
	movs r1, #2
	movs r0, #10
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #4
	orrs r3, r5
	movs r1, #2
	strb r3, [r0]
	movs r0, #16
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	bl Object_GetById
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #18
	bl Object_GetById
	adds r3, r0, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	ldr r3, .L_02009060
	movs r5, #192
	str r3, [r0, #12]
	str r3, [r0, #20]
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #18
	bl ObjectMotion_SetActionVariant
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl Func_02001108
	movs r0, #19
	bl Object_GetById
	adds r3, r0, #0
	mov r2, r8
	lsls r5, r5, #12
	adds r3, #85
	strb r2, [r3]
	str r5, [r0, #12]
	str r5, [r0, #20]
	movs r0, #19
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r3, r6
	strb r3, [r0]
	movs r0, #19
	bl ObjectMotion_SetActionVariant
	movs r1, #0
	movs r2, #0
	movs r0, #19
	bl Func_02001108
	movs r0, #20
	bl Object_GetById
	adds r3, r0, #0
	mov r2, r8
	adds r3, #85
	strb r2, [r3]
	str r5, [r0, #12]
	str r5, [r0, #20]
	movs r0, #20
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r6, r3
	strb r6, [r0]
	movs r0, #20
	bl ObjectMotion_SetActionVariant
	movs r2, #0
	movs r1, #0
	movs r0, #20
	bl Func_02001108
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #20
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #20
	bl Object_GetById
	movs r1, #4
	bl Object_SetPartAttribute
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200905c:
	.4byte gPartyState
.L_02009060:
	.4byte 0xfffc0000
	.section .rodata.x020091b0,"a",%progbits
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
	.4byte 0x0000006a
	.4byte 0x10101069
	.4byte 0xffffffff
	.4byte 0x10202069
	.4byte 0xffffffff
	.4byte 0x10303069
	.4byte 0xffffffff
	.4byte 0x10404069
	.4byte 0xffffffff
	.4byte 0x10505069
	.4byte 0xffffffff
	.4byte 0x1060706a
	.4byte 0xffffffff
	.4byte 0x1070606a
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff0073
	.4byte 0x00000002
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00016000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00012000
	.4byte 0xffff0019
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x005e0000
	.4byte 0x00014000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x028b0000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00012000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x02b50000
	.4byte 0x00000000
	.4byte 0x006a0000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff006d
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00016000
	.4byte 0xffff0087
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00014000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00700000
	.4byte 0x00004000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff0106
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0xffff011d
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001370
Data_02001370:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001da4
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001da5
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001da8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001dfb
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001dfc
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_020000c4
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000184
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e06
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000244
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000304
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001da6
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001da7
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001dfd
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001dfe
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001dff
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e01
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e03
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e08
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e09
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte Func_02000054
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020014c0
Data_020014c0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001e1f
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001e20
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001da8
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001e23
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001e24
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000124
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_020001e4
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e2e
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_020002a4
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_02000304
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001e21
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001e22
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001e25
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001e26
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001e27
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e29
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e2b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e30
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e31
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte Func_0200007c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
