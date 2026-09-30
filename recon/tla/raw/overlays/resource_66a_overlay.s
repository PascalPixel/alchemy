.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_02008070
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008074
	cmp r2, r3
	beq .L_0200806a
	ldr r3, .L_02008078
	cmp r2, r3
	bne .L_0200806a
	ldr r0, .L_0200807c
	b .L_0200806c
.L_0200806a:
	ldr r0, .L_02008080
.L_0200806c:
	pop {pc}
	.2byte 0x0000
.L_02008070:
	.4byte gPartyState
.L_02008074:
	.4byte 0x00000064
.L_02008078:
	.4byte 0x00000065
.L_0200807c:
	.4byte Data_02001868
.L_02008080:
	.4byte Data_02001778
	.section .text.x02008084,"ax",%progbits
	.global Func_02000084
	.thumb_func
Func_02000084:
	push {lr}
	adds r3, r0, #0
	adds r2, r1, #0
	ldr r0, .L_02008094
	adds r1, r3, #0
	bl Func_0200129c
	pop {pc}
.L_02008094:
	.4byte Data_020016e4
	.global Data_02000098
Data_02000098:
	.4byte 0x00004770
	.global Data_0200009c
Data_0200009c:
	.4byte 0x00004770
	.section .text.x020080a0,"ax",%progbits
	.global Func_020000a0
	.thumb_func
Func_020000a0:
	push {r5, r6, lr}
	ldr r5, .L_020080e8
	adds r6, r0, #0
	adds r0, r5, #0
	bl Func_020014b8
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001590
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020080d0
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_020014b8
	b .L_020080dc
.L_020080d0:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_020014b8
.L_020080dc:
	adds r0, r6, #0
	movs r1, #0
	bl Func_020014d0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020080e8:
	.4byte 0x00001c92
	.section .text.x020080ec,"ax",%progbits
	.global Func_020000ec
	.thumb_func
Func_020000ec:
	push {lr}
	ldr r0, .L_020080f8
	bl Func_02001580
	pop {pc}
	.2byte 0x0000
.L_020080f8:
	.4byte Data_02001608
	.section .text.x020080fc,"ax",%progbits
	.global Func_020000fc
	.thumb_func
Func_020000fc:
	push {lr}
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #200
	movs r1, #152
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #4
	bl Func_02001420
	movs r0, #208
	movs r1, #152
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #4
	bl Func_02001420
	movs r0, #216
	movs r1, #152
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #4
	bl Func_02001420
	movs r0, #192
	movs r1, #144
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #255
	lsls r0, r0, #17
	bl Func_020015d0
	movs r0, #144
	movs r1, #192
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #4
	bl Func_02001420
	movs r0, #168
	movs r1, #192
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #4
	bl Func_02001420
	pop {pc}
	.section .text.x02008168,"ax",%progbits
	.global Func_02000168
	.thumb_func
Func_02000168:
	push {lr}
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #200
	movs r1, #152
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02001420
	movs r0, #208
	movs r1, #152
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02001420
	movs r0, #216
	movs r1, #152
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02001420
	movs r0, #192
	movs r1, #144
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #17
	bl Func_020015d0
	movs r0, #144
	movs r1, #192
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02001420
	movs r0, #168
	movs r1, #192
	lsls r0, r0, #17
	lsls r1, r1, #17
	movs r2, #0
	movs r3, #0
	bl Func_02001420
	pop {pc}
	.section .text.x020081d4,"ax",%progbits
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #74
	adds r3, r3, r2
	movs r2, #2
	strh r2, [r3]
	bx lr
	.section .text.x020081e8,"ax",%progbits
	.global Func_020001e8
	.thumb_func
Func_020001e8:
	push {lr}
	ldr r3, .L_0200820c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008210
	cmp r2, r3
	beq .L_02008206
	ldr r3, .L_02008214
	cmp r2, r3
	bne .L_02008206
	ldr r0, .L_02008218
	b .L_02008208
.L_02008206:
	ldr r0, .L_0200821c
.L_02008208:
	pop {pc}
	.2byte 0x0000
.L_0200820c:
	.4byte gPartyState
.L_02008210:
	.4byte 0x00000064
.L_02008214:
	.4byte 0x00000065
.L_02008218:
	.4byte Data_02001a90
.L_0200821c:
	.4byte Data_02001a18
	.section .text.x02008220,"ax",%progbits
	.global Func_02000220
	.thumb_func
Func_02000220:
	push {r5, lr}
	ldr r5, .L_02008254
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #12]
	movs r2, #128
	lsls r2, r2, #12
	cmp r3, r2
	bge .L_02008246
	movs r3, #226
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r5, r3
	movs r3, #0
	b .L_02008250
.L_02008246:
	movs r3, #226
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r5, r3
	movs r3, #1
.L_02008250:
	strb r3, [r2]
	pop {r5, pc}
.L_02008254:
	.4byte gPartyState
	.section .text.x02008258,"ax",%progbits
	.global Func_02000258
	.thumb_func
Func_02000258:
	push {lr}
	movs r0, #8
	bl Object_GetById
	ldr r4, .L_02008278
	ldr r2, [r0, #12]
	movs r3, #224
	lsls r3, r3, #13
	ldr r1, [r0, #8]
	adds r2, r2, r3
	ldr r3, [r0, #16]
	adds r0, r4, #0
	bl Func_020015f0
	movs r0, #0
	pop {pc}
.L_02008278:
	.4byte gOverlayArea + 0x1be0
	.section .text.x0200827c,"ax",%progbits
	.global Func_0200027c
	.thumb_func
Func_0200027c:
	push {lr}
	ldr r1, .L_020082ec
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r1, r0
	movs r0, #0
	ldrsh r2, [r3, r0]
	ldr r3, .L_020082f0
	cmp r2, r3
	bne .L_020082a6
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Func_020002f8
	b .L_020082e6
.L_020082a6:
	ldr r3, .L_020082f4
	cmp r2, r3
	bne .L_020082e6
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r1, r0
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_020082d2
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	subs r0, #54
	lsls r2, r2, #1
	adds r3, r3, r0
	adds r2, #255
	b .L_020082e0
.L_020082d2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
.L_020082e0:
	str r2, [r3]
	bl Func_0200051c
.L_020082e6:
	movs r0, #0
	pop {pc}
	.2byte 0x0000
.L_020082ec:
	.4byte gPartyState
.L_020082f0:
	.4byte 0x00000064
.L_020082f4:
	.4byte 0x00000065
	.section .text.x020082f8,"ax",%progbits
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #32]
	ldr r3, .L_02008510
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	ldrb r2, [r5, #23]
	strb r3, [r0]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	movs r1, #144
	strb r3, [r5, #23]
	ldr r0, .L_02008514
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #129
	lsls r0, r0, #2
	movs r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008344
	bl Func_020000fc
.L_02008344:
	movs r0, #8
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	movs r5, #192
	lsls r5, r5, #12
	adds r2, #85
	strb r6, [r2]
	movs r0, #9
	str r5, [r3, #12]
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r0, #10
	str r5, [r3, #12]
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r0, #8
	str r5, [r3, #12]
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #9
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #10
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	movs r5, #128
	adds r2, #85
	lsls r5, r5, #13
	strb r6, [r2]
	movs r0, #11
	str r5, [r3, #12]
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	movs r1, #128
	strb r6, [r2]
	movs r0, #13
	str r5, [r3, #12]
	lsls r1, r1, #5
	bl Func_02001598
	movs r0, #11
	movs r1, #2
	bl Object_SetModeById
	movs r0, #12
	movs r1, #3
	bl Object_SetModeById
	movs r0, #13
	movs r1, #2
	bl Object_SetModeById
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r1, #3
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #13
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008474
	movs r1, #200
	movs r2, #136
	movs r0, #14
	lsls r1, r1, #16
	lsls r2, r2, #16
	bl Func_02001488
	movs r1, #196
	movs r2, #180
	movs r0, #15
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001488
.L_02008474:
	movs r0, #14
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r2, #0
	ldr r1, [r3, #16]
	ldr r0, [r3, #8]
	str r6, [r3, #20]
	str r6, [r3, #12]
	movs r3, #4
	bl Func_02001420
	movs r0, #15
	bl Object_GetById
	adds r3, r0, #0
	adds r2, r3, #0
	adds r2, #85
	strb r6, [r2]
	movs r2, #0
	ldr r1, [r3, #16]
	ldr r0, [r3, #8]
	str r6, [r3, #20]
	str r6, [r3, #12]
	movs r3, #4
	bl Func_02001420
	ldr r0, .L_02008518
	bl Func_02001578
	movs r0, #16
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200850c
	movs r3, #13
	movs r2, #58
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #58
	movs r2, #12
	movs r3, #7
	bl Func_02001408
	movs r3, #25
	movs r2, #66
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #58
	movs r2, #2
	movs r3, #9
	bl Func_02001408
	movs r3, #15
	movs r2, #74
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #58
	movs r2, #10
	movs r3, #5
	bl Func_02001408
.L_0200850c:
	add sp, #8
	pop {r5, r6, pc}
.L_02008510:
	.4byte gPartyState
.L_02008514:
	.4byte Func_02000220
.L_02008518:
	.4byte Data_02001608
	.section .text.x0200851c,"ax",%progbits
	.global Func_0200051c
	.thumb_func
Func_0200051c:
	push {r5, r6, r7, lr}
	ldr r7, .L_02008790
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r6, #32
	orrs r3, r6
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	movs r0, #0
	bl Func_02001520
	ldr r5, .L_02008794
	movs r0, #0
	bl Func_020015e0
	movs r3, #128
	movs r2, #15
	lsls r3, r3, #23
	adds r0, r5, #0
	movs r1, #8
	bl Func_020015e8
	movs r1, #144
	ldr r0, .L_02008798
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r3, [r3]
	movs r2, #128
	subs r3, #8
	lsls r3, r3, #16
	lsls r2, r2, #9
	cmp r3, r2
	bhi .L_02008590
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #76
	adds r3, r3, r2
	movs r2, #3
	strh r2, [r3]
.L_02008590:
	bl Func_02001568
	movs r1, #128
	movs r2, #10
	lsls r1, r1, #2
	movs r3, #11
	movs r0, #0
	bl Func_02001570
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r3, r6
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r6
	strb r3, [r0]
	ldr r0, .L_0200879c
	bl Func_02001230
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #5
	bl Func_02001598
	movs r0, #12
	movs r1, #0
	bl Object_SetModeById
	movs r0, #13
	movs r1, #1
	bl Object_SetModeById
	movs r0, #14
	movs r1, #2
	bl Object_SetModeById
	movs r0, #12
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #1
	movs r0, #13
	bl ObjectMotion_SetActionVariant
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #13
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #14
	bl Object_GetById
	movs r1, #6
	bl Animation_ApplyChildValues
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200866c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	b .L_020086ee
.L_0200866c:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ee
	movs r2, #0
	movs r0, #23
	movs r1, #0
	bl Func_02001488
	movs r0, #18
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #16
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #17
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #19
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #20
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086d8
	movs r1, #178
	movs r2, #216
	movs r0, #17
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001488
	movs r1, #186
	movs r2, #216
	movs r0, #19
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001488
	b .L_02008748
.L_020086d8:
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	b .L_02008748
.L_020086ee:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #24
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r0, #23
	bl Object_GetById
	ldr r3, .L_020087a0
	str r3, [r0, #108]
.L_02008748:
	ldr r3, .L_02008790
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_0200878e
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200878e
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #152
	movs r2, #152
	lsls r1, r1, #7
	lsls r2, r2, #6
	movs r0, #8
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #8
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndResetMotion
	movs r0, #8
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_0200878e:
	pop {r5, r6, r7, pc}
.L_02008790:
	.4byte gPartyState
.L_02008794:
	.4byte gOverlayArea + 0x1be0
.L_02008798:
	.4byte Func_02000258
.L_0200879c:
	.4byte Data_020016e4
.L_020087a0:
	.4byte Func_02001220
	.section .text.x020087a4,"ax",%progbits
	.global Func_020007a4
	.thumb_func
Func_020007a4:
	push {lr}
	ldr r3, .L_0200880c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008810
	sub sp, #8
	cmp r2, r3
	bne .L_02008804
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008804
	movs r3, #21
	movs r2, #14
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #23
	movs r1, #46
	movs r2, #4
	movs r3, #5
	bl Func_02001408
	movs r3, #84
	movs r2, #12
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #86
	movs r1, #44
	movs r2, #5
	movs r3, #6
	bl Func_02001408
	movs r3, #20
	movs r2, #13
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #22
	movs r1, #45
	movs r2, #5
	movs r3, #6
	bl Func_02001400
.L_02008804:
	movs r0, #0
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_0200880c:
	.4byte gPartyState
.L_02008810:
	.4byte 0x00000065
	.global Data_02000814
Data_02000814:
	.4byte 0x00004770
	.section .text.x02008818,"ax",%progbits
	.global Func_02000818
	.thumb_func
Func_02000818:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #171
	sub sp, #12
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008832
	bl .L_02009206
.L_02008832:
	movs r0, #245
	lsls r0, r0, #3
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #1
	bne .L_02008844
	bl .L_02009206
.L_02008844:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #189
	bl GameFlag_SetBit
	bl Func_02001430
	movs r0, #0
	bl Func_02001528
	movs r1, #128
	movs r2, #128
	movs r0, #22
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #21
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #17
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	movs r0, #19
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #20
	bl ObjectMotion_SetSpeedParameters
	ldr r0, .L_02008c9c
	bl Func_020014b8
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #200
	movs r2, #144
	movs r0, #4
	lsls r1, r1, #2
	lsls r2, r2, #1
	bl ObjectMotion_SetPositionAndReset
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #186
	movs r1, #1
	movs r2, #132
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #18
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r0, #21
	movs r1, #6
	movs r2, #15
	bl ObjectMotion_Launch
	movs r0, #21
	movs r1, #6
	movs r2, #23
	bl ObjectMotion_Launch
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_020014e8
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #2
	movs r0, #16
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #16
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #128
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #22
	bl Func_020014e8
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #8
	adds r1, #255
	movs r2, #50
	movs r0, #22
	bl Func_020014e8
	movs r0, #186
	movs r1, #1
	movs r2, #156
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #3
	bl Object_SetModeById
	movs r0, #24
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #24
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02001508
	movs r1, #192
	movs r2, #192
	lsls r2, r2, #8
	movs r0, #24
	lsls r1, r1, #9
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02008ca0
	movs r0, #24
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #24
	bl Object_RefreshSelectorById
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #186
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #24
	bl Func_020014e8
	movs r2, #10
	movs r0, #24
	movs r1, #0
	bl Func_020014c8
	movs r1, #4
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #4
	adds r1, #255
	movs r2, #30
	movs r0, #24
	bl Func_020014e8
	movs r2, #10
	movs r1, #0
	movs r0, #24
	bl Func_020014c8
	movs r0, #21
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #15
	bl Object_GetById
	movs r1, #144
	lsls r1, r1, #5
	adds r6, r0, #0
	adds r1, #16
	movs r0, #8
	bl Func_020015a0
	bl Func_020015b0
	movs r0, #21
	bl Object_GetById
	movs r1, #2
	bl Func_020015c8
	movs r0, #201
	bl Func_02001600
	movs r0, #40
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl BattleFx_StartItemBreak
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	mov r8, r0
	ldr r3, [r6, #12]
	str r6, [r0, #104]
	movs r0, #128
	lsls r0, r0, #13
	adds r3, r3, r0
	str r3, [r5, #4]
	movs r1, #192
	ldr r3, [r6, #16]
	adds r2, r5, #0
	str r3, [r5, #8]
	lsls r1, r1, #8
	bl Vector_AddPolarOffsetFar
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	ldr r1, [r5]
	mov r0, r8
	bl Func_020013f8
	mov r0, r8
	bl BattleFx_SnapScaleToFull
	mov r0, r8
	movs r1, #4
	bl Func_020013f0
	ldr r3, .L_02008ca4
	movs r1, #192
	str r3, [r6, #108]
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #4
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #17
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #19
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020014e8
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #17
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #19
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #202
	movs r1, #1
	movs r2, #144
	movs r3, #1
	lsls r2, r2, #17
	lsls r0, r0, #18
	negs r1, r1
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #4
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #4
	bl Func_020014f0
	movs r0, #50
	bl Battle_WaitMode0
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #60
	movs r0, #20
	bl Func_020014e8
	movs r0, #186
	movs r1, #1
	movs r2, #136
	movs r3, #1
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02001508
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #16
	negs r1, r1
	movs r2, #0
	movs r0, #20
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #6
	adds r1, #255
	movs r2, #60
	movs r0, #20
	bl Func_020014e8
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #18
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl Func_020014c8
	movs r1, #128
	movs r2, #30
	movs r5, #0
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014e8
	movs r1, #0
	str r5, [r6, #108]
	adds r0, r6, #0
	bl Animation_ApplyChildValues
	mov r0, r8
	bl UpdateRisingParticleBurst
	movs r0, #21
	bl Object_GetById
	movs r1, #0
	bl Func_020015c8
	bl Func_020015c0
	bl Func_020015b8
	movs r0, #8
	bl Field_BeginPaletteTransition
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #4
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #16
	movs r1, #0
	movs r2, #0
	b .L_02008ca8
	.2byte 0x0000
.L_02008c9c:
	.4byte 0x00001c77
.L_02008ca0:
	.4byte Data_02001610
.L_02008ca4:
	.4byte Func_02001228
.L_02008ca8:
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #4
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #128
	movs r2, #40
	lsls r1, r1, #1
	movs r0, #21
	bl Func_020014e8
	movs r1, #4
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #144
	lsls r1, r1, #5
	adds r1, #16
	movs r0, #8
	bl Func_020015a0
	bl Func_020015b0
	movs r0, #21
	bl Object_GetById
	movs r1, #2
	bl Func_020015c8
	movs r0, #201
	bl Func_02001600
	movs r0, #40
	bl Battle_WaitMode0
	movs r2, #10
	movs r1, #0
	movs r0, #18
	bl Func_020014c8
	movs r0, #21
	bl Object_GetById
	movs r1, #0
	bl Func_020015c8
	bl Func_020015c0
	bl Func_020015b8
	movs r0, #8
	bl Field_BeginPaletteTransition
	movs r0, #16
	movs r1, #0
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	movs r0, #24
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #40
	bl Battle_WaitMode0
	movs r1, #208
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #10
	adds r1, #255
	movs r2, #50
	movs r0, #21
	bl Func_020014e8
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #129
	movs r2, #50
	lsls r1, r1, #1
	movs r0, #22
	bl Func_020014e8
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #192
	movs r0, #16
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #19
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #20
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #18
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #160
	movs r0, #22
	lsls r1, r1, #7
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #24
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #2
	movs r0, #21
	adds r1, #255
	bl Func_020014f0
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl Func_020014c8
	ldr r3, .L_02009210
	movs r1, #144
	str r5, [r3]
	lsls r1, r1, #3
	ldr r0, .L_02009214
	bl Scheduler_AddOrUpdateCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #30
	movs r0, #24
	bl Func_020014e8
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #0
	movs r2, #8
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #128
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #2
	movs r0, #21
	bl Motion_SetVarCbAndRefresh
	movs r0, #10
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	bl Func_020014f0
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	adds r0, #22
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #240
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #24
	bl Motion_SetModeAndWaitAnimation
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #0
	movs r2, #10
	bl Func_020014c8
	movs r1, #176
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #22
	bl ObjectMotion_ArmCallback
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #6
	movs r2, #10
	adds r0, #22
	movs r1, #0
	bl Func_020014c8
	movs r1, #3
	movs r0, #18
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Motion_SetModeAndWaitAnimation
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #128
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl ObjectMotion_ArmCallback
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #22
	bl Object_SetModeById
	movs r0, #30
	bl Battle_WaitMode0
	movs r1, #3
	movs r0, #21
	bl Motion_SetModeAndWaitAnimation
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #24
	movs r1, #21
	bl Object_LinkObjectAndSetCallback
	movs r2, #16
	movs r0, #22
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #16
	movs r0, #21
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #22
	movs r1, #52
	movs r2, #0
	bl ObjectMotion_OffsetPositionAndReset
	movs r2, #0
	movs r1, #52
	movs r0, #21
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #15
	bl Battle_WaitMode0
	movs r0, #22
	movs r1, #3
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl Func_020014f0
	movs r0, #40
	bl Battle_WaitMode0
	movs r0, #4
	movs r1, #21
	bl Object_LinkObjectAndSetCallback
	movs r0, #22
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r0, #21
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r5, .L_02009218
	movs r0, #22
	adds r1, r5, #0
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	adds r1, r5, #0
	movs r0, #21
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #20
	movs r1, #16
	movs r2, #0
	bl ObjectMotion_CommitPositionAndActivate
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r0, #17
	movs r1, #0
	movs r2, #16
	bl ObjectMotion_OffsetPositionAndReset
	movs r1, #0
	movs r2, #16
	movs r0, #19
	bl ObjectMotion_CommitPositionAndActivate
	movs r0, #5
	bl Battle_WaitMode0
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	movs r0, #19
	lsls r1, r1, #6
	movs r2, #0
	bl ObjectMotion_ArmCallback
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #18
	bl ObjectMotion_ArmCallback
	movs r0, #21
	bl Object_RefreshSelectorById
	movs r0, #24
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #4
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #22
	movs r1, #0
	movs r2, #0
	bl Func_02001488
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl Func_02001488
	movs r0, #24
	bl Object_GetById
	movs r1, #9
	bl Object_SetActionCallback
	ldr r3, .L_0200921c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02001438
.L_02009206:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009210:
	.4byte gOverlayArea + 0x1c08
.L_02009214:
	.4byte Data_02000814 + 0x1
.L_02009218:
	.4byte Data_02001674
.L_0200921c:
	.4byte gPartyState
	.section .text.x02009220,"ax",%progbits
	.global Func_02001220
	.thumb_func
Func_02001220:
	push {lr}
	bl Func_020015d8
	pop {pc}
	.section .text.x02009228,"ax",%progbits
	.global Func_02001228
	.thumb_func
Func_02001228:
	push {lr}
	bl ObjectGroup_ApplyRandomChildValues
	pop {pc}
	.section .text.x02009230,"ax",%progbits
	.global Func_02001230
	.thumb_func
Func_02001230:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_02009294
	adds r7, r0, #0
.L_02009246:
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
	bl ObjectDispatch_SetSingleChildField26
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
	bl Func_02001320
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_02009246
.L_02009294:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200929c,"ax",%progbits
	.global Func_0200129c
	.thumb_func
Func_0200129c:
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
	b .L_02009308
.L_020092b8:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_02009304
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_020092e0
	ldr r3, [r6, #28]
	ldr r1, .L_0200931c
	adds r3, r3, r1
	str r3, [r6, #28]
.L_020092e0:
	mov r2, r9
	cmp r2, #1
	bne .L_02009312
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02001320
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_02009312
.L_02009304:
	adds r5, #6
	movs r1, #255
.L_02009308:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_020092b8
.L_02009312:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200931c:
	.4byte 0xffffe100
	.section .text.x02009320,"ax",%progbits
	.global Func_02001320
	.thumb_func
Func_02001320:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r3, #0
	mov r8, r2
	adds r6, r1, #0
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	lsls r3, r5, #3
	subs r3, r3, r5
	movs r1, #156
	lsls r1, r1, #1
	lsls r3, r3, #3
	adds r3, r3, r1
	ldr r5, [r2, r3]
	adds r7, r0, #0
	bl Func_02001588
	lsls r0, r0, #2
	adds r5, r5, r0
	mov r0, r8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009394
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #0
	strb r3, [r5, #2]
	movs r3, #2
	orrs r3, r2
	strb r3, [r1]
	cmp r6, #1
	beq .L_02009380
	cmp r6, #1
	bcc .L_02009376
	cmp r6, #2
	beq .L_0200938a
	b .L_020093c2
.L_02009376:
	adds r0, r7, #0
	movs r1, #2
	bl Func_020013f0
	b .L_020093c2
.L_02009380:
	adds r0, r7, #0
	movs r1, #4
	bl Func_020013f0
	b .L_020093c2
.L_0200938a:
	adds r0, r7, #0
	movs r1, #6
	bl Func_020013f0
	b .L_020093c2
.L_02009394:
	movs r3, #255
	strb r3, [r5, #2]
	cmp r6, #1
	beq .L_020093b0
	cmp r6, #1
	bcc .L_020093a6
	cmp r6, #2
	beq .L_020093ba
	b .L_020093c2
.L_020093a6:
	adds r0, r7, #0
	movs r1, #1
	bl Func_020013f0
	b .L_020093c2
.L_020093b0:
	adds r0, r7, #0
	movs r1, #3
	bl Func_020013f0
	b .L_020093c2
.L_020093ba:
	adds r0, r7, #0
	movs r1, #5
	bl Func_020013f0
.L_020093c2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .rodata.x02009608,"a",%progbits
	.global Data_02001608
Data_02001608:
	.4byte 0x02030010
	.4byte 0x0000ffff
	.global Data_02001610
Data_02001610:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000005
	.4byte 0x00100000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffc40000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0xfffc0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02001674
Data_02001674:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x030c0000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000c0000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00200000
	.4byte 0x00000000
	.4byte 0x00200000
	.4byte 0x00000001
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00360000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_020016e4
Data_020016e4:
	.4byte 0x00010009
	.4byte 0xffff0201
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
	.4byte 0x00000064
	.4byte 0x1010c05f
	.4byte 0xffffffff
	.4byte 0x10203064
	.4byte 0xffffffff
	.4byte 0x10302064
	.4byte 0xffffffff
	.4byte 0x10406065
	.4byte 0xffffffff
	.4byte 0x10507065
	.4byte 0xffffffff
	.4byte 0x00000065
	.4byte 0x10604064
	.4byte 0xffffffff
	.4byte 0x10705064
	.4byte 0xffffffff
	.4byte 0x1080805f
	.4byte 0xffffffff
	.4byte 0x1090a065
	.4byte 0xffffffff
	.4byte 0x10a09065
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_02001778
Data_02001778:
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00020000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00020000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00020000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00028000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff0110
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001868
Data_02001868:
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00024000
	.4byte 0xffff0196
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01480000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x01024000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00020000
	.4byte 0xffff0173
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00028000
	.4byte 0xffff0171
	.4byte 0x00000007
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00024000
	.4byte 0xffff02ab
	.4byte 0x00000007
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x01500000
	.4byte 0x00024000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff0010
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00003000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00005000
	.4byte 0xffff0027
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00005000
	.4byte 0xffff0026
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x0002c000
	.4byte 0xffff0025
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x0002a000
	.4byte 0xffff00e7
	.4byte 0x00000002
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01400000
	.4byte 0x00004000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x02e00000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x0000b000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001a18
Data_02001a18:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000002
	.4byte 0xffff0014
	.4byte Func_02000168
	.4byte 0x00000002
	.4byte 0xffff0015
	.4byte Func_020000fc
	.4byte 0x00001815
	.4byte 0x02030010
	.4byte Func_020000ec
	.4byte 0x00008b85
	.4byte 0xffff0000
	.4byte Func_020001d4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001a90
Data_02001a90:
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000002
	.4byte 0x08bd0014
	.4byte Func_02000818
	.4byte 0x00000000
	.4byte 0x08a70017
	.4byte 0x00001b42
	.4byte 0x00008d15
	.4byte 0x08a70017
	.4byte 0x00001b43
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x000025ba
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x000025bb
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001c91
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_020000a0
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001c95
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001c96
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001c97
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001c98
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001c99
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001c9a
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001c9b
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001c9c
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001c9d
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001c9e
	.4byte 0x00008515
	.4byte 0x0200000a
	.4byte 0x00000000
	.4byte 0x50008615
	.4byte 0x02010009
	.4byte Func_02000084
	.4byte 0x00001815
	.4byte 0x02030010
	.4byte Func_020000ec
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Data_02000098 + 0x1
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Data_0200009c + 0x1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
