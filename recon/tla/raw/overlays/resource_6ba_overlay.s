.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02000ebc
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_02000f64
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02000f70
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	mov r8, r1
	movs r0, #206
	movs r1, #0
	movs r2, #0
	movs r3, #0
	bl Func_02000d00
	movs r7, #0
	mov r10, r0
	cmp r7, r8
	bge .L_02008114
.L_02008074:
	movs r0, #1
	movs r1, #1
	bl Func_02000d08
	movs r0, #5
	movs r1, #2
	bl Func_02000d08
	movs r0, #241
	lsls r0, r0, #9
	adds r0, #64
	movs r1, #5
	bl Func_02000d08
	bl UiWork_FinalizePendingCore
	movs r2, #0
	movs r3, #34
	adds r0, r6, #0
	movs r1, #5
	bl UiText_OpenMessageWindow
	ldr r3, .L_02008128
	movs r2, #3
	ldr r3, [r3]
	movs r5, #0
	ands r3, r2
	cmp r3, #0
	bne .L_020080d6
.L_020080ae:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_020080d6
	ldr r3, .L_02008128
	movs r2, #3
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020080ae
	b .L_020080d6
.L_020080c8:
	movs r3, #1
	str r3, [r1]
	movs r0, #1
	str r3, [r1, #4]
	str r3, [r1, #12]
	bl WaitFrames
.L_020080d6:
	bl UiWork_IsComplete
	ldr r1, .L_02008128
	cmp r0, #0
	beq .L_020080c8
	ldr r3, [r1]
	movs r2, #2
	ands r3, r2
	movs r5, #0
	b .L_020080fe
.L_020080ea:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	bgt .L_0200810c
	ldr r1, .L_02008128
	movs r2, #2
	ldr r3, [r1]
	ands r3, r2
.L_020080fe:
	cmp r3, #0
	bne .L_02008114
	ldr r3, [r1]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020080ea
.L_0200810c:
	adds r7, #1
	adds r6, #1
	cmp r7, r8
	blt .L_02008074
.L_02008114:
	bl UiWork_FinalizePendingCore
	mov r0, r10
	movs r1, #2
	bl UiWork_Finalize
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008128:
	.4byte gInput
	.section .text.x0200812c,"ax",%progbits
	.global Func_0200012c
	.thumb_func
Func_0200012c:
	push {lr}
	ldr r0, .L_0200813c
	ldr r1, .L_02008140
	subs r1, r1, r0
	bl Func_02000054
	pop {pc}
	.2byte 0x0000
.L_0200813c:
	.4byte 0x0000124c
.L_02008140:
	.4byte 0x00001277
	.section .text.x02008144,"ax",%progbits
	.global Func_02000144
	.thumb_func
Func_02000144:
	push {lr}
	ldr r0, .L_02008154
	ldr r1, .L_02008158
	subs r1, r0, r1
	bl Func_02000054
	pop {pc}
	.2byte 0x0000
.L_02008154:
	.4byte 0x00001277
.L_02008158:
	.4byte 0x0000124c
	.section .text.x0200815c,"ax",%progbits
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {lr}
	ldr r3, .L_0200816c
	ldr r1, .L_02008170
	ldr r0, .L_02008174
	subs r1, r1, r3
	bl Func_02000054
	pop {pc}
.L_0200816c:
	.4byte 0x0000124c
.L_02008170:
	.4byte 0x00001277
.L_02008174:
	.4byte 0x000012a2
	.section .text.x02008178,"ax",%progbits
	.global Func_02000178
	.thumb_func
Func_02000178:
	push {lr}
	ldr r0, .L_02008188
	ldr r1, .L_0200818c
	subs r1, r1, r0
	bl Func_02000054
	pop {pc}
	.2byte 0x0000
.L_02008188:
	.4byte 0x000012d2
.L_0200818c:
	.4byte 0x000012fd
	.section .text.x02008190,"ax",%progbits
	.global Func_02000190
	.thumb_func
Func_02000190:
	push {lr}
	adds r1, r0, #0
	movs r0, #1
	bl Func_02000e40
	pop {pc}
	.section .text.x0200819c,"ax",%progbits
	.global Func_0200019c
	.thumb_func
Func_0200019c:
	push {lr}
	adds r1, r0, #0
	movs r0, #2
	bl Func_02000e40
	pop {pc}
	.section .text.x020081a8,"ax",%progbits
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	push {lr}
	adds r1, r0, #0
	movs r0, #3
	bl Func_02000e40
	pop {pc}
	.section .text.x020081b4,"ax",%progbits
	.global Func_020001b4
	.thumb_func
Func_020001b4:
	push {lr}
	adds r1, r0, #0
	movs r0, #24
	bl Func_02000e40
	pop {pc}
	.section .text.x020081c0,"ax",%progbits
	.global Func_020001c0
	.thumb_func
Func_020001c0:
	push {lr}
	adds r1, r0, #0
	movs r0, #0
	bl Func_02000e50
	pop {pc}
	.section .text.x020081cc,"ax",%progbits
	.global Func_020001cc
	.thumb_func
Func_020001cc:
	push {lr}
	bl Func_02000e48
	pop {pc}
	.section .text.x020081d4,"ax",%progbits
	.global Func_020001d4
	.thumb_func
Func_020001d4:
	push {r5, r6, lr}
	adds r5, r0, #0
	movs r0, #16
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200821a
	ldr r6, .L_02008298
	adds r0, r6, #0
	bl Func_02000e08
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000e30
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_0200820c
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r6, #1
	b .L_02008234
.L_0200820c:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r6, #2
	bl Func_02000e08
	b .L_02008238
.L_0200821a:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200822a
	ldr r0, .L_0200829c
	b .L_02008234
.L_0200822a:
	bl Func_02000e70
	cmp r0, #0
	beq .L_02008242
	ldr r0, .L_020082a0
.L_02008234:
	bl Func_02000e08
.L_02008238:
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e18
	b .L_02008294
.L_02008242:
	ldr r6, .L_020082a4
	adds r0, r6, #0
	bl Func_02000e08
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000e30
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008286
	adds r0, r6, #1
	bl Func_02000e08
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e18
	adds r0, r5, #0
	bl Func_02000e58
	bl Func_02000e70
	cmp r0, #0
	beq .L_02008294
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008294
.L_02008286:
	adds r0, r6, #2
	bl Func_02000e08
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e18
.L_02008294:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008298:
	.4byte 0x00001307
.L_0200829c:
	.4byte 0x0000130d
.L_020082a0:
	.4byte 0x0000130e
.L_020082a4:
	.4byte 0x0000130a
	.section .text.x020082a8,"ax",%progbits
	.global Func_020002a8
	.thumb_func
Func_020002a8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #16
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020082c0
	ldr r0, .L_02008380
	b .L_02008330
.L_020082c0:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082d0
	ldr r0, .L_02008384
	b .L_02008330
.L_020082d0:
	bl Func_02000e70
	cmp r0, #0
	beq .L_0200836c
	bl Func_02000e70
	adds r6, r0, #0
	bl Func_02000d48
	ldrh r0, [r0]
	movs r1, #2
	mov r8, r0
	adds r0, r6, #0
	bl Func_02000d08
	movs r1, #5
	mov r0, r8
	bl Func_02000d08
	ldr r7, .L_02008388
	adds r0, r7, #0
	bl Func_02000e08
	movs r1, #0
	adds r0, r5, #0
	bl UiText_OpenMessageAtObject
	bl Func_02000e30
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	beq .L_02008318
	adds r0, r7, #1
	b .L_02008330
.L_02008318:
	adds r0, r6, #0
	bl Func_02000dc8
	cmp r0, #0
	bge .L_02008326
	adds r0, r7, #2
	b .L_02008330
.L_02008326:
	ldr r3, .L_0200838c
	ldr r3, [r3, #16]
	cmp r3, r8
	bcs .L_0200833e
	adds r0, r7, #3
.L_02008330:
	bl Func_02000e08
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e18
	b .L_0200837a
.L_0200833e:
	adds r0, r7, #4
	bl Func_02000e08
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e18
	adds r0, r6, #0
	movs r1, #3
	bl Func_02000e28
	movs r1, #0
	adds r0, r6, #0
	bl PartyInventory_GiveItem
	movs r0, #0
	bl Func_02000e68
	mov r3, r8
	negs r0, r3
	bl Party_AdjustSixDigitCounterA
	b .L_0200837a
.L_0200836c:
	ldr r0, .L_02008390
	bl Func_02000e08
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000e18
.L_0200837a:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_02008380:
	.4byte 0x0000130f
.L_02008384:
	.4byte 0x00001310
.L_02008388:
	.4byte 0x00001311
.L_0200838c:
	.4byte gPartyState
.L_02008390:
	.4byte 0x00001316
	.section .text.x02008394,"ax",%progbits
	.global Func_02000394
	.thumb_func
Func_02000394:
	push {lr}
	sub sp, #8
	mov r1, sp
	add r0, sp, #4
	bl Func_02000e60
	add sp, #8
	pop {pc}
	.section .text.x020083a4,"ax",%progbits
	.global Func_020003a4
	.thumb_func
Func_020003a4:
	push {r5, lr}
	movs r5, #0
.L_020083a8:
	adds r0, r5, #0
	adds r5, #1
	bl Func_02000d20
	cmp r5, #7
	ble .L_020083a8
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020083b8,"ax",%progbits
	.global Func_020003b8
	.thumb_func
Func_020003b8:
	push {r5, lr}
	ldr r0, .L_02008514
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	ldr r2, .L_02008518
	ldr r3, .L_0200851c
	movs r5, #9
	str r3, [r2, #16]
.L_020083ca:
	movs r1, #228
	movs r0, #4
	bl Inventory_AddItem
	subs r5, #1
	movs r0, #4
	movs r1, #229
	bl Inventory_AddItem
	cmp r5, #0
	bge .L_020083ca
	movs r1, #184
	movs r0, #4
	bl Inventory_AddItem
	movs r1, #204
	movs r0, #4
	bl Inventory_AddItem
	movs r1, #224
	movs r0, #4
	bl Inventory_AddItem
	movs r1, #11
	movs r0, #4
	bl Inventory_AddItem
	movs r1, #12
	adds r1, #255
	movs r0, #4
	bl Inventory_AddItem
	movs r1, #223
	movs r0, #4
	bl Inventory_AddItem
	movs r1, #226
	movs r0, #5
	bl Inventory_AddItem
	movs r1, #227
	movs r0, #5
	bl Inventory_AddItem
	movs r1, #230
	movs r0, #5
	bl Inventory_AddItem
	movs r1, #232
	movs r0, #5
	bl Inventory_AddItem
	movs r1, #231
	movs r0, #5
	bl Inventory_AddItem
	movs r1, #237
	movs r0, #5
	bl Inventory_AddItem
	movs r1, #10
	adds r1, #255
	movs r0, #5
	bl Inventory_AddItem
	movs r1, #242
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #252
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #174
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #174
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #174
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #178
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #180
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #162
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #162
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #209
	lsls r1, r1, #1
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #164
	adds r1, #255
	movs r0, #6
	bl Inventory_AddItem
	movs r1, #189
	movs r0, #7
	bl Inventory_AddItem
	movs r1, #200
	movs r0, #7
	bl Inventory_AddItem
	movs r1, #201
	movs r0, #7
	bl Inventory_AddItem
	movs r1, #202
	movs r0, #7
	bl Inventory_AddItem
	movs r1, #203
	movs r0, #7
	bl Inventory_AddItem
	movs r1, #204
	movs r0, #7
	bl Inventory_AddItem
	movs r1, #207
	movs r0, #7
	bl Inventory_AddItem
	movs r0, #0
	bl Party_RemoveActiveOwner
	movs r0, #1
	bl Party_RemoveActiveOwner
	movs r0, #2
	bl Party_RemoveActiveOwner
	movs r0, #3
	bl Party_RemoveActiveOwner
	pop {r5, pc}
	.2byte 0x0000
.L_02008514:
	.4byte 0x0000114d
.L_02008518:
	.4byte gPartyState
.L_0200851c:
	.4byte 0x000bde31
	.section .text.x02008520,"ax",%progbits
	.global Func_02000520
	.thumb_func
Func_02000520:
	push {r5, r6, r7, lr}
	ldr r0, .L_020085f8
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
	movs r2, #160
	adds r7, r0, #0
	lsls r2, r2, #1
	movs r6, #50
	adds r3, r7, r2
	movs r5, #1
	adds r6, #255
	strb r5, [r7, r6]
	movs r0, #5
	strb r5, [r3]
	bl Owner_GetState
	movs r2, #152
	adds r7, r0, #0
	lsls r2, r2, #1
	adds r3, r7, r2
	strb r5, [r3]
	movs r3, #2
	strb r3, [r7, r6]
	movs r0, #4
	bl Owner_GetState
	movs r5, #0
	adds r7, r0, #0
	movs r6, #216
	b .L_020085a4
.L_020085a0:
	adds r6, #2
	adds r5, #1
.L_020085a4:
	cmp r5, #14
	bgt .L_020085c2
	ldrh r0, [r6, r7]
	bl Func_02000d48
	ldrh r3, [r6, r7]
	cmp r3, #0
	beq .L_020085c2
	ldrb r3, [r0, #12]
	cmp r3, #2
	bne .L_020085a0
	movs r0, #4
	adds r1, r5, #0
	bl Inventory_Break
.L_020085c2:
	movs r0, #5
	bl Owner_GetState
	movs r5, #0
	adds r7, r0, #0
	movs r6, #216
	b .L_020085d4
.L_020085d0:
	adds r6, #2
	adds r5, #1
.L_020085d4:
	cmp r5, #14
	bgt .L_020085f2
	ldrh r0, [r6, r7]
	bl Func_02000d48
	ldrh r3, [r6, r7]
	cmp r3, #0
	beq .L_020085f2
	ldrb r3, [r0, #12]
	cmp r3, #2
	bne .L_020085d0
	movs r0, #5
	adds r1, r5, #0
	bl Inventory_Break
.L_020085f2:
	bl Func_02000dc0
	pop {r5, r6, r7, pc}
.L_020085f8:
	.4byte 0x0000114c
	.section .text.x020085fc,"ax",%progbits
	.global Func_020005fc
	.thumb_func
Func_020005fc:
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
	bne .L_02008632
	adds r0, r5, #0
	adds r1, r6, #0
	adds r2, r7, #0
	bl Djinn_Activate
.L_02008632:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02008638,"ax",%progbits
	.global Func_02000638
	.thumb_func
Func_02000638:
	push {r5, r6, lr}
	ldr r0, .L_02008660
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	movs r6, #0
.L_02008644:
	movs r5, #0
.L_02008646:
	adds r1, r5, #0
	adds r0, r6, #0
	adds r5, #1
	bl Djinn_AddToLeastLoadedOwner
	cmp r5, #17
	ble .L_02008646
	adds r6, #1
	cmp r6, #3
	ble .L_02008644
	bl Func_02000dc0
	pop {r5, r6, pc}
.L_02008660:
	.4byte 0x0000114e
	.global Data_02000664
Data_02000664:
	.4byte 0x00004770
	.section .text.x02008668,"ax",%progbits
	.global Func_02000668
	.thumb_func
Func_02000668:
	push {lr}
	movs r1, #0
	movs r0, #11
	bl PartyInventory_GiveItem
	movs r0, #22
	bl Func_02000d38
	movs r0, #22
	bl Func_02000dd0
	pop {pc}
	.section .text.x02008680,"ax",%progbits
	.global Func_02000680
	.thumb_func
Func_02000680:
	ldr r0, .L_02008684
	bx lr
.L_02008684:
	.4byte Data_020011f8
	.section .text.x02008688,"ax",%progbits
	.global Func_02000688
	.thumb_func
Func_02000688:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r5, .L_020086fc
	subs r2, #172
	str r2, [r3]
	movs r3, #139
	lsls r3, r3, #2
	adds r2, r5, r3
	movs r3, #2
	strb r3, [r2]
	movs r0, #16
	movs r1, #5
	bl Object_SetModeById
	movs r0, #17
	movs r1, #5
	bl Object_SetModeById
	movs r0, #18
	movs r1, #5
	bl Object_SetModeById
	movs r0, #19
	movs r1, #5
	bl Object_SetModeById
	movs r0, #30
	movs r1, #6
	bl Object_SetModeById
	movs r0, #31
	movs r1, #6
	bl Object_SetModeById
	movs r0, #32
	movs r1, #6
	bl Object_SetModeById
	movs r0, #33
	movs r1, #6
	bl Object_SetModeById
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #99
	bne .L_020086f8
	bl Func_02000db0
.L_020086f8:
	movs r0, #0
	pop {r5, pc}
.L_020086fc:
	.4byte gPartyState
	.section .text.x02008700,"ax",%progbits
	.global Func_02000700
	.thumb_func
Func_02000700:
	movs r0, #0
	bx lr
	.section .text.x02008704,"ax",%progbits
	.global Func_02000704
	.thumb_func
Func_02000704:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #32
	mov r8, r0
	mov r0, sp
	bl Party_ListActiveOwners
	movs r6, #0
	adds r7, r0, #0
	cmp r6, r7
	bge .L_0200873e
.L_0200871c:
	lsls r3, r6, #1
	mov r2, sp
	ldrh r5, [r2, r3]
	adds r6, #1
	adds r0, r5, #0
	bl Owner_GetState
	ldrb r1, [r0, #15]
	adds r0, r5, #0
	add r1, r8
	bl Party_AdvanceOwnerCountToTarget
	adds r0, r5, #0
	bl Owner_RecalculateStats
	cmp r6, r7
	blt .L_0200871c
.L_0200873e:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008748,"ax",%progbits
	.global Func_02000748
	.thumb_func
Func_02000748:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008838
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #4
	bl Owner_GetState
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #30
	movs r3, #9
	mov r8, r0
	movs r0, #0
	bl UiWindow_Create
	ldr r5, .L_0200883c
	adds r6, r0, #0
	adds r1, r6, #0
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawResource
	adds r0, r5, #1
	adds r1, r6, #0
	movs r2, #0
	movs r3, #16
	adds r5, #2
	bl UiText_DrawResource
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #32
	movs r7, #1
	bl UiText_DrawResource
.L_0200879c:
	cmp r7, #0
	beq .L_020087d2
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRect
	mov r0, r8
	adds r1, r6, #0
	movs r2, #0
	movs r3, #48
	bl UiText_DrawStringAtOffset
	ldr r0, .L_02008840
	adds r1, r6, #0
	movs r2, #48
	movs r3, #48
	bl UiText_DrawStringInWindow
	mov r3, r8
	ldrb r0, [r3, #15]
	movs r3, #48
	str r3, [sp, #0]
	movs r1, #0
	adds r2, r6, #0
	movs r3, #72
	movs r7, #0
	bl UiText_DrawNumberInWindow
.L_020087d2:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_02008844
	movs r2, #8
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_020087ee
	ldr r3, [r1, #4]
	movs r2, #4
	ands r3, r2
	cmp r3, #0
	beq .L_020087fc
.L_020087ee:
	movs r0, #5
	bl Func_02000704
	movs r0, #93
	bl Func_02000e78
	movs r7, #1
.L_020087fc:
	ldr r5, .L_02008844
	movs r2, #1
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_02008816
	movs r0, #1
	bl Func_02000704
	movs r0, #91
	bl Func_02000e78
	movs r7, #1
.L_02008816:
	ldr r3, [r5, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0200879c
	movs r0, #113
	bl Func_02000e78
	adds r0, r6, #0
	movs r1, #2
	bl UiWork_Finalize
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008838:
	.4byte gPartyState
.L_0200883c:
	.4byte 0x00001151
.L_02008840:
	.4byte Data_02000e80
.L_02008844:
	.4byte gInput
	.section .text.x02008848,"ax",%progbits
	.global Func_02000848
	.thumb_func
Func_02000848:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r2, #0
	sub sp, #4
	movs r5, #2
	mov r8, r2
	movs r1, #0
	movs r2, #30
	movs r3, #7
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r1, #8
	adds r7, r0, #0
	movs r2, #13
	movs r3, #10
	movs r0, #0
	str r5, [sp, #0]
	bl UiWindow_Create
	movs r3, #128
	movs r2, #128
	movs r6, #1
	lsls r3, r3, #19
	lsls r2, r2, #24
	mov r9, r0
	mov r10, r6
	adds r3, #212
	ldr r0, .L_02008a88
	ldr r1, .L_02008a8c
	adds r2, #16
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #24
	ldr r0, .L_02008a90
	adds r1, #28
	adds r2, #1
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #112
	bl Func_02000e78
	movs r0, #1
	bl WaitFrames
.L_020088ac:
	mov r3, r10
	cmp r3, #0
	beq .L_02008924
	movs r2, #0
	adds r0, r7, #0
	mov r10, r2
	bl RenderOutput_PrepareForRedraw
	mov r0, r9
	bl RenderOutput_PrepareForRedraw
	ldr r0, .L_02008a94
	adds r1, r7, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringInWindow
	mov r3, r10
	str r3, [sp, #0]
	adds r0, r6, #0
	movs r1, #0
	adds r2, r7, #0
	movs r3, #80
	bl UiText_DrawNumberAtOffset
	bl PartyInventory_HasSpace
	cmp r0, #0
	beq .L_02008918
	ldr r0, .L_02008a98
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawStringInWindow
	ldr r0, .L_02008a9c
	adds r1, r7, #0
	adds r0, r6, r0
	movs r2, #120
	movs r3, #0
	bl UiText_DrawCharacterAtOffset
	ldr r0, .L_02008aa0
	adds r1, r7, #0
	adds r0, r6, r0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawResource
	mov r0, r9
	adds r1, r6, #0
	bl ItemMenu_DrawItemDetails
	b .L_02008924
.L_02008918:
	ldr r0, .L_02008aa4
	adds r1, r7, #0
	movs r2, #0
	movs r3, #32
	bl UiText_DrawStringInWindow
.L_02008924:
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_02008aa8
	movs r2, #1
	ldr r3, [r3, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0200894a
	adds r0, r6, #0
	bl PartyInventory_Add
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_02008956
	movs r0, #175
	bl Func_02000e78
.L_0200894a:
	ldr r5, .L_02008aa8
	movs r2, #2
	ldr r3, [r5, #4]
	ands r3, r2
	cmp r3, #0
	beq .L_0200895e
.L_02008956:
	movs r0, #113
	bl Func_02000e78
	b .L_02008a6c
.L_0200895e:
	ldr r3, [r5, #12]
	movs r2, #128
	ands r3, r2
	cmp r3, #0
	beq .L_02008976
	movs r3, #1
	movs r0, #111
	mov r8, r3
	adds r6, #1
	mov r10, r3
	bl Func_02000e78
.L_02008976:
	ldr r3, [r5, #12]
	movs r2, #64
	ands r3, r2
	cmp r3, #0
	beq .L_02008990
	movs r2, #255
	movs r3, #1
	movs r0, #111
	mov r8, r2
	subs r6, #1
	mov r10, r3
	bl Func_02000e78
.L_02008990:
	ldr r3, [r5, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_020089a8
	movs r2, #1
	movs r0, #111
	mov r8, r2
	adds r6, #10
	mov r10, r2
	bl Func_02000e78
.L_020089a8:
	ldr r3, [r5, #12]
	movs r2, #32
	ands r3, r2
	cmp r3, #0
	beq .L_020089c2
	movs r3, #255
	movs r2, #1
	movs r0, #111
	mov r8, r3
	subs r6, #10
	mov r10, r2
	bl Func_02000e78
.L_020089c2:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_020089dc
	movs r3, #1
	movs r0, #111
	mov r8, r3
	adds r6, #30
	mov r10, r3
	bl Func_02000e78
.L_020089dc:
	ldr r3, [r5, #12]
	movs r2, #128
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_020089f8
	movs r2, #255
	movs r3, #1
	movs r0, #111
	mov r8, r2
	subs r6, #30
	mov r10, r3
	bl Func_02000e78
.L_020089f8:
	mov r2, r8
	lsls r5, r2, #24
	movs r2, #1
	asrs r3, r5, #24
	negs r2, r2
	cmp r3, r2
	bne .L_02008a32
	movs r3, #250
	lsls r3, r3, #1
	movs r1, #250
	adds r0, r6, r3
	b .L_02008a18
.L_02008a10:
	movs r2, #244
	adds r2, #255
	movs r1, #250
	adds r0, r6, r2
.L_02008a18:
	lsls r1, r1, #1
	bl Engine_MathRemainder
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r6
	bl Func_02000d48
	ldrh r3, [r0, #6]
	cmp r3, #0
	beq .L_02008a10
.L_02008a32:
	movs r3, #128
	lsls r3, r3, #17
	cmp r5, r3
	bne .L_02008a66
	movs r2, #250
	lsls r2, r2, #1
	movs r1, #250
	adds r0, r6, r2
	b .L_02008a4c
.L_02008a44:
	movs r3, #246
	adds r3, #255
	movs r1, #250
	adds r0, r6, r3
.L_02008a4c:
	lsls r1, r1, #1
	bl Engine_MathRemainder
	adds r6, r0, #0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r6
	bl Func_02000d48
	ldrh r3, [r0, #6]
	cmp r3, #0
	beq .L_02008a44
.L_02008a66:
	movs r2, #0
	mov r8, r2
	b .L_020088ac
.L_02008a6c:
	adds r0, r7, #0
	movs r1, #2
	bl UiWork_Finalize
	mov r0, r9
	movs r1, #2
	bl UiWork_Finalize
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008a88:
	.4byte 0x05000200
.L_02008a8c:
	.4byte 0x050001c0
.L_02008a90:
	.4byte 0x050001e8
.L_02008a94:
	.4byte Data_02000e84
.L_02008a98:
	.4byte Data_02000e90
.L_02008a9c:
	.4byte 0x0000025f
.L_02008aa0:
	.4byte 0x00000092
.L_02008aa4:
	.4byte Data_02000ea8
.L_02008aa8:
	.4byte gInput
	.section .text.x02008aac,"ax",%progbits
	.global Func_02000aac
	.thumb_func
Func_02000aac:
	push {lr}
	bl Func_02000d28
	pop {pc}
	.section .text.x02008ab4,"ax",%progbits
	.global Func_02000ab4
	.thumb_func
Func_02000ab4:
	push {lr}
	bl Func_02000d30
	pop {pc}
	.section .text.x02008abc,"ax",%progbits
	.global Func_02000abc
	.thumb_func
Func_02000abc:
	push {lr}
	sub sp, #8
	adds r4, r3, #0
	cmp r0, #1
	bne .L_02008ada
	ldr r3, [sp, #16]
	adds r0, r1, #0
	str r3, [sp, #0]
	ldr r3, [sp, #20]
	adds r1, r2, #0
	str r3, [sp, #4]
	adds r2, r4, #0
	ldr r3, [sp, #12]
	bl Func_02000c90
.L_02008ada:
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008ae0,"ax",%progbits
	.global Func_02000ae0
	.thumb_func
Func_02000ae0:
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
	bne .L_02008b10
	mov r3, r8
	lsls r2, r3, #16
	lsls r1, r7, #16
	adds r0, r6, #0
	bl Func_02000df8
	movs r3, #0
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02008b10:
	mov r2, r10
	cmp r2, #2
	bne .L_02008b46
	ldr r3, [r5, #24]
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_02008b3e
.L_02008b20:
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
	ble .L_02008b20
.L_02008b3e:
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
.L_02008b46:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008b50,"ax",%progbits
	.global Func_02000b50
	.thumb_func
Func_02000b50:
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
	bne .L_02008b96
	mov r3, r8
	lsls r2, r3, #16
	adds r0, r7, #0
	lsls r1, r5, #16
	bl Func_02000df8
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
.L_02008b96:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008ba0,"ax",%progbits
	.global Func_02000ba0
	.thumb_func
Func_02000ba0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	adds r7, r1, #0
	mov r8, r2
	mov r10, r3
	cmp r6, #1
	bne .L_02008bd6
	movs r0, #177
	lsls r3, r2, #16
	lsls r1, r7, #16
	lsls r0, r0, #1
	movs r2, #0
	bl Func_02000c88
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008bd6
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r2, r5, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02008bd6:
	cmp r6, #2
	bne .L_02008c52
	mov r2, r8
	lsls r3, r2, #16
	lsls r1, r7, #16
	movs r0, #252
	movs r2, #0
	bl Func_02000c88
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02008c52
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
.L_02008c04:
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
	ble .L_02008c04
	adds r3, #1
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldr r3, .L_02008c5c
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
	bl Func_02000e20
.L_02008c52:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008c5c:
	.4byte gPartyState
	.section .rodata.x02008e80,"a",%progbits
	.global Data_02000e80
Data_02000e80:
	.4byte 0x0000764c
	.global Data_02000e84
Data_02000e84:
	.4byte 0x6d657449
	.4byte 0x3a6f4e20
	.4byte 0x00000000
	.global Data_02000e90
Data_02000e90:
	.4byte 0x65473a41
	.4byte 0x74492074
	.4byte 0x20206d65
	.4byte 0x65523a42
	.4byte 0x6e727574
	.4byte 0x00000000
	.global Data_02000ea8
Data_02000ea8:
	.4byte 0x4d455449
	.4byte 0x4c554620
	.4byte 0x2e2e2e4c
	.4byte 0x2e2e2e2e
	.4byte 0x0000002e
	.global Data_02000ebc
Data_02000ebc:
	.4byte 0xffff0000
	.4byte 0x00000198
	.4byte 0xc0000138
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0001
	.4byte 0x00000268
	.4byte 0x40000148
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0002
	.4byte 0x00000288
	.4byte 0xc0000098
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x40000058
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0xc0000198
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0xffff0005
	.4byte 0x000001f8
	.4byte 0x400000e8
	.4byte 0x01180000
	.4byte 0x030c0000
	.4byte 0x000001d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000f64
Data_02000f64:
	.4byte 0x00000142
	.4byte 0x01402142
	.4byte 0x000001ff
	.global Data_02000f70
Data_02000f70:
	.4byte 0xffff00d1
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00cd
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00ce
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00c6
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff0046
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00d7
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff00c8
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff01c2
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff0141
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0177
	.4byte 0x00000001
	.4byte 0x02a00000
	.4byte 0x00000000
	.4byte 0x00600000
	.4byte 0x00004000
	.4byte 0xffff0128
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff00ee
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01e7
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff00f3
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff0019
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00004000
	.4byte 0xffff01e8
	.4byte 0x00000001
	.4byte 0x02c80000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020011f8
Data_020011f8:
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000190
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_0200019c
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_020001a8
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_020001b4
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_020001c0
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_020001cc
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_020001d4
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_020002a8
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte Func_0200012c
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000144
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_0200015c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02000178
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte Func_02000394
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte Func_020003a4
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000668
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte Func_02000aac
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte Func_02000ab4
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte Func_02000748
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_02000848
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte Func_02000520
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte Func_020003b8
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte Func_02000638
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte Data_02000664 + 0x1
	.4byte 0x00000000
	.4byte 0xffff001f
	.4byte Data_02000664 + 0x1
	.4byte 0x00000000
	.4byte 0xffff0020
	.4byte Data_02000664 + 0x1
	.4byte 0x00000000
	.4byte 0xffff0021
	.4byte Data_02000664 + 0x1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
