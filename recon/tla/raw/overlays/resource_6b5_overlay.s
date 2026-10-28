	.set LOBBY_DE_ES_FR_IT, 0
	.ifdef TLA_EDITION_DE
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
	.ifdef TLA_EDITION_ES
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
	.ifdef TLA_EDITION_FR
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
	.ifdef TLA_EDITION_IT
	.set LOBBY_DE_ES_FR_IT, 1
	.endif
.syntax unified
	.thumb
	.section .text.x0200839c,"ax",%progbits
	.if LOBBY_DE_ES_FR_IT
	.global Func_0200039c
	.thumb_func
Func_0200039c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #170
	lsls r2, r2, #1
	mov r11, r2
	mov r0, r11
	sub sp, #48
	bl Heap_AllocEwram
	movs r3, #225
	lsls r3, r3, #2
	movs r2, #0
	mov r9, r0
	movs r7, #0
	mov r10, r3
	mov r8, r2
	b .L_de_02008496
.L_de_020083c8:
	ldrh r3, [r3]
	cmp r3, r11
	bls .L_de_020083d0
	b .L_de_020084fa
.L_de_020083d0:
	movs r0, #1
	bl wait
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r2, r10
	cmp r2, #0
	blt .L_de_020083ee
	ldr r3, .L_de_0200846c
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_de_020083f6
.L_de_020083ee:
	adds r5, #1
	cmp r5, #24
	ble .L_de_020083f6
	b .L_de_020084fa
.L_de_020083f6:
	bl Func_0200193c
	ldr r3, .L_de_02008470
	cmp r0, #0
	bne .L_de_020083c8
	ldrh r3, [r3]
	mov r12, r3
	cmp r12, r11
	beq .L_de_0200840a
	b .L_de_020084fa
.L_de_0200840a:
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_de_02008418
	adds r7, #1
.L_de_02008418:
	movs r0, #2
	bl wait
	ldr r0, .L_de_02008474
	mov r1, sp
	bl Message_DecodeRawUnbounded
	movs r1, #0
	mov r2, sp
	ldrh r3, [r2, r1]
	cmp r3, #0
	beq .L_de_0200843e
.L_de_02008430:
	adds r1, #1
	cmp r1, #7
	bgt .L_de_0200843e
	lsls r3, r1, #1
	ldrh r3, [r2, r3]
	cmp r3, #0
	bne .L_de_02008430
.L_de_0200843e:
	ldr r3, .L_de_02008468
	lsls r2, r1, #1
	mov r5, sp
	adds r4, r1, #1
	movs r1, #14
	strh r3, [r5, r2]
	cmp r1, r4
	blt .L_de_02008478
	subs r3, r6, r4
	adds r0, r6, #0
	adds r2, r3, #0
	adds r0, #14
	adds r2, #14
.L_de_02008458:
	ldrb r3, [r2]
	subs r1, #1
	strb r3, [r0]
	subs r2, #1
	subs r0, #1
	cmp r1, r4
	bge .L_de_02008458
	b .L_de_02008478
.L_de_02008468:
	.4byte 0x00000020
.L_de_0200846c:
	.4byte gLinkStatus
.L_de_02008470:
	.4byte gSerialReceivedSize
.L_de_02008474:
	.4byte 0x00000e25
.L_de_02008478:
	movs r1, #0
	cmp r1, r4
	bge .L_de_0200848e
	adds r2, r6, #0
.L_de_02008480:
	lsls r3, r1, #1
	ldrh r3, [r5, r3]
	adds r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r1, r4
	blt .L_de_02008480
.L_de_0200848e:
	movs r3, #0
	strb r3, [r6, #14]
	movs r3, #1
	add r8, r3
.L_de_02008496:
	mov r2, r8
	cmp r2, #2
	bgt .L_de_02008508
	mov r0, r8
	adds r0, #128
	bl Unit_Get
	adds r6, r0, #0
	bl Serial_StartReceive
	movs r3, #1
	negs r3, r3
	movs r5, #0
	cmp r0, r3
	bne .L_de_020083f6
	b .L_de_02008530
.L_de_020084b6:
	ldrh r3, [r3]
	movs r2, #170
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_de_020084fa
	movs r0, #1
	bl wait
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r2, r10
	cmp r2, #0
	blt .L_de_020084de
	ldr r3, .L_de_02008548
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_de_020084e4
.L_de_020084de:
	adds r5, #1
	cmp r5, #24
	bgt .L_de_020084fa
.L_de_020084e4:
	bl Func_0200193c
	ldr r3, .L_de_0200854c
	cmp r0, #0
	bne .L_de_020084b6
	ldrh r3, [r3]
	mov r12, r3
	movs r3, #170
	lsls r3, r3, #1
	cmp r12, r3
	beq .L_de_02008500
.L_de_020084fa:
	movs r7, #1
	negs r7, r7
	b .L_de_02008532
.L_de_02008500:
	movs r0, #2
	bl wait
	b .L_de_02008532
.L_de_02008508:
	mov r0, r9
	bl Heap_Release
	movs r2, #170
	lsls r2, r2, #1
	mov r11, r2
	mov r0, r11
	bl Heap_AllocEwram
	mov r9, r0
	movs r0, #1
	bl Djinn_GetRecoveryTable
	bl Serial_StartReceive
	movs r3, #1
	negs r3, r3
	movs r5, #0
	cmp r0, r3
	bne .L_de_020084e4
.L_de_02008530:
	adds r7, r0, #0
.L_de_02008532:
	mov r0, r9
	bl Heap_Release
	adds r0, r7, #0
	sub sp, #-48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_de_02008548:
	.4byte gLinkStatus
.L_de_0200854c:
	.4byte gSerialReceivedSize
	.else
	.global Func_0200039c
	.thumb_func
Func_0200039c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #170
	lsls r2, r2, #1
	mov r11, r2
	mov r0, r11
	sub sp, #48
	bl Heap_AllocEwram
	movs r3, #225
	lsls r3, r3, #2
	movs r2, #0
	mov r9, r0
	movs r7, #0
	mov r10, r3
	mov r8, r2
	b .L_0200847a
.L_020083c8:
	ldrh r3, [r3]
	cmp r3, r11
	bls .L_020083d0
	b .L_020084de
.L_020083d0:
	movs r0, #1
	bl wait
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r2, r10
	cmp r2, #0
	blt .L_020083ee
	ldr r3, .L_0200852c
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_020083f4
.L_020083ee:
	adds r5, #1
	cmp r5, #24
	bgt .L_020084de
.L_020083f4:
	bl Func_0200193c
	ldr r3, .L_02008530
	cmp r0, #0
	bne .L_020083c8
	ldrh r3, [r3]
	mov r12, r3
	cmp r12, r11
	bne .L_020084de
	movs r2, #149
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02008414
	adds r7, #1
.L_02008414:
	movs r0, #2
	bl wait
	ldr r0, .L_02008534
	mov r1, sp
	bl Message_DecodeRawUnbounded
	movs r1, #0
	mov r2, sp
	ldrh r3, [r2, r1]
	cmp r3, #0
	beq .L_0200843a
.L_0200842c:
	adds r1, #1
	cmp r1, #4
	bgt .L_0200843a
	lsls r3, r1, #1
	ldrh r3, [r2, r3]
	cmp r3, #0
	bne .L_0200842c
.L_0200843a:
	adds r4, r1, #0
	movs r1, #14
	cmp r1, r4
	blt .L_0200845a
	subs r3, r6, r4
	adds r0, r6, #0
	adds r2, r3, #0
	adds r0, #14
	adds r2, #14
.L_0200844c:
	ldrb r3, [r2]
	subs r1, #1
	strb r3, [r0]
	subs r2, #1
	subs r0, #1
	cmp r1, r4
	bge .L_0200844c
.L_0200845a:
	movs r1, #0
	cmp r1, r4
	bge .L_02008472
	adds r0, r6, #0
.L_02008462:
	lsls r2, r1, #1
	mov r3, sp
	ldrh r3, [r3, r2]
	adds r1, #1
	strb r3, [r0]
	adds r0, #1
	cmp r1, r4
	blt .L_02008462
.L_02008472:
	movs r3, #0
	strb r3, [r6, #14]
	movs r3, #1
	add r8, r3
.L_0200847a:
	mov r2, r8
	cmp r2, #2
	bgt .L_020084ec
	mov r0, r8
	adds r0, #128
	bl Unit_Get
	adds r6, r0, #0
	bl Serial_StartReceive
	movs r3, #1
	negs r3, r3
	movs r5, #0
	cmp r0, r3
	bne .L_020083f4
	b .L_02008514
.L_0200849a:
	ldrh r3, [r3]
	movs r2, #170
	lsls r2, r2, #1
	cmp r3, r2
	bgt .L_020084de
	movs r0, #1
	bl wait
	movs r3, #1
	negs r3, r3
	add r10, r3
	mov r2, r10
	cmp r2, #0
	blt .L_020084c2
	ldr r3, .L_0200852c
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_020084c8
.L_020084c2:
	adds r5, #1
	cmp r5, #24
	bgt .L_020084de
.L_020084c8:
	bl Func_0200193c
	ldr r3, .L_02008530
	cmp r0, #0
	bne .L_0200849a
	ldrh r3, [r3]
	mov r12, r3
	movs r3, #170
	lsls r3, r3, #1
	cmp r12, r3
	beq .L_020084e4
.L_020084de:
	movs r7, #1
	negs r7, r7
	b .L_02008516
.L_020084e4:
	movs r0, #2
	bl wait
	b .L_02008516
.L_020084ec:
	mov r0, r9
	bl Heap_Release
	movs r2, #170
	lsls r2, r2, #1
	mov r11, r2
	mov r0, r11
	bl Heap_AllocEwram
	mov r9, r0
	movs r0, #1
	bl Djinn_GetRecoveryTable
	bl Serial_StartReceive
	movs r3, #1
	negs r3, r3
	movs r5, #0
	cmp r0, r3
	bne .L_020084c8
.L_02008514:
	adds r7, r0, #0
.L_02008516:
	mov r0, r9
	bl Heap_Release
	adds r0, r7, #0
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200852c:
	.4byte gLinkStatus
.L_02008530:
	.4byte gSerialReceivedSize
.L_02008534:
	.4byte 0x00000c58
	.endif
	.section .text.x02008578,"ax",%progbits
	.global Func_02000578
	.thumb_func
Func_02000578:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #170
	lsls r5, r5, #1
	adds r0, r5, #0
	sub sp, #32
	bl Heap_AllocEwram
	add r3, sp, #16
	mov r8, r3
	movs r2, #150
	movs r1, #0
	lsls r2, r2, #2
	adds r7, r0, #0
	mov r0, r8
	mov r11, r1
	mov r9, r2
	bl Func_02000538
	str r0, [sp, #4]
	movs r6, #0
.L_020085ac:
	mov r1, sp
	adds r1, #8
	movs r3, #0
	str r1, [sp, #0]
	strb r3, [r1, r6]
	adds r6, #1
	cmp r6, #7
	ble .L_020085ac
	ldr r2, [sp, #4]
	movs r6, #0
	cmp r6, r2
	bge .L_02008690
.L_020085c4:
	mov r1, r8
	lsls r5, r6, #1
	movs r3, #0
	ldrh r0, [r1, r5]
	mov r10, r3
	bl Unit_Get
	movs r2, #170
	adds r1, r0, #0
	ldr r3, .L_020087b0
	lsls r2, r2, #1
	adds r0, r7, #0
	mov lr, r3
	.2byte 0xf800
	movs r3, #149
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #2
	strb r3, [r2]
	mov r1, r8
	ldrh r2, [r1, r5]
	ldr r1, [sp, #0]
	adds r3, r6, #0
	subs r3, #128
	strb r3, [r1, r2]
	movs r1, #170
	adds r0, r7, #0
	lsls r1, r1, #1
	bl Func_0200192c
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_0200863c
	mov r11, r0
	b .L_0200879a
.L_0200860c:
	movs r0, #1
	bl wait
	movs r3, #1
	negs r3, r3
	add r9, r3
	mov r1, r9
	cmp r1, #0
	blt .L_0200862a
	ldr r3, .L_020087b4
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0200863c
.L_0200862a:
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #24
	ble .L_0200863c
	movs r1, #1
	negs r1, r1
	mov r11, r1
	b .L_0200879a
.L_0200863c:
	bl Func_0200193c
	cmp r0, #0
	bne .L_0200860c
	movs r0, #2
	bl wait
	ldr r2, [sp, #4]
	adds r6, #1
	cmp r6, r2
	blt .L_020085c4
	b .L_02008690
.L_02008654:
	movs r0, #1
	bl wait
	movs r3, #1
	negs r3, r3
	add r9, r3
	mov r1, r9
	cmp r1, #0
	blt .L_02008672
	ldr r3, .L_020087b4
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_02008680
.L_02008672:
	adds r5, #1
	cmp r5, #24
	ble .L_02008680
	movs r2, #1
	negs r2, r2
	mov r11, r2
	b .L_0200879a
.L_02008680:
	bl Func_0200193c
	cmp r0, #0
	bne .L_02008654
	movs r0, #2
	bl wait
	adds r6, #1
.L_02008690:
	cmp r6, #2
	bgt .L_020086b2
	movs r1, #149
	lsls r1, r1, #1
	adds r3, r7, r1
	movs r5, #0
	strb r5, [r3]
	adds r0, r7, #0
	adds r1, #42
	bl Func_0200192c
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_02008680
	mov r11, r0
	b .L_0200879a
.L_020086b2:
	movs r5, #170
	adds r0, r7, #0
	lsls r5, r5, #1
	bl Heap_Release
	adds r0, r5, #0
	bl Heap_AllocEwram
	adds r7, r0, #0
	movs r0, #0
	bl Djinn_GetRecoveryTable
	ldr r3, .L_020087b0
	adds r1, r0, #0
	adds r2, r5, #0
	adds r0, r7, #0
	mov lr, r3
	.2byte 0xf800
	adds r4, r7, #0
	movs r2, #148
	movs r3, #0
	lsls r2, r2, #1
	mov r8, r3
	adds r3, r7, r2
	ldr r3, [r3]
	movs r1, #150
	lsls r1, r1, #2
	movs r6, #144
	adds r4, #8
	mov r10, r1
	movs r5, #0
	lsls r6, r6, #1
	cmp r8, r3
	bge .L_02008740
	adds r0, r4, #0
.L_020086f8:
	ldrb r3, [r0, #2]
	add r2, sp, #8
	ldrb r3, [r2, r3]
	strb r3, [r0, #2]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_02008732
	ldr r3, [r4, r6]
	adds r1, r5, #0
	subs r3, #1
	cmp r5, r3
	bge .L_02008722
	lsls r3, r5, #2
	adds r2, r3, r4
.L_02008714:
	ldr r3, [r2, #4]
	adds r1, #1
	stmia r2!, {r3}
	ldr r3, [r4, r6]
	subs r3, #1
	cmp r1, r3
	blt .L_02008714
.L_02008722:
	movs r3, #144
	lsls r3, r3, #1
	adds r2, r4, r3
	ldr r3, [r2]
	subs r0, #4
	subs r3, #1
	str r3, [r2]
	subs r5, #1
.L_02008732:
	movs r6, #144
	lsls r6, r6, #1
	ldr r3, [r4, r6]
	adds r5, #1
	adds r0, #4
	cmp r5, r3
	blt .L_020086f8
.L_02008740:
	movs r1, #170
	lsls r1, r1, #1
	adds r0, r7, #0
	bl Func_0200192c
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	bne .L_02008786
	mov r11, r0
	b .L_0200879a
.L_02008756:
	movs r0, #1
	bl wait
	movs r2, #1
	negs r2, r2
	add r10, r2
	mov r3, r10
	cmp r3, #0
	blt .L_02008774
	ldr r3, .L_020087b4
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_02008786
.L_02008774:
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #24
	ble .L_02008786
	movs r3, #1
	negs r3, r3
	mov r11, r3
	b .L_0200879a
.L_02008786:
	bl Func_0200193c
	cmp r0, #0
	bne .L_02008756
	movs r0, #1
	bl wait
	movs r0, #2
	bl wait
.L_0200879a:
	adds r0, r7, #0
	bl Heap_Release
	mov r0, r11
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020087b0:
	.4byte IwramCopyWords
.L_020087b4:
	.4byte gLinkStatus
	.section .text.x02008864,"ax",%progbits
	.global Func_02000864
	.thumb_func
Func_02000864:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #116
	movs r2, #0
	adds r0, #255
	mov r8, r2
	movs r7, #0
	mov r10, r3
	movs r6, #0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008894
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	b .L_0200898a
.L_02008894:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020088a2
	b .L_02008ba8
.L_020088a2:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020088ee
	b .L_02008ba8
.L_020088b2:
	movs r2, #181
	lsls r2, r2, #1
	movs r0, #131
	movs r3, #2
	add r2, r10
	lsls r0, r0, #1
	strh r3, [r2]
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	movs r0, #4
	bl LinkLobby_WriteSlotValue
	movs r0, #128
	movs r3, #1
	lsls r0, r0, #2
	mov r8, r3
	bl GameFlag_ClearBit
	b .L_02008978
.L_020088ee:
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #2
	bl LinkLobby_WriteSlotValue
	movs r0, #2
	bl Func_0200006c
	cmp r0, #0
	bne .L_0200896e
	ldr r0, .L_02008b20
	movs r1, #5
	movs r2, #4
	movs r3, #1
	bl Message_OpenWindow
	adds r7, r0, #0
	b .L_0200896e
.L_02008922:
	movs r0, #1
	bl wait
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	movs r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200893a
	movs r5, #1
.L_0200893a:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200894a
	movs r5, #1
.L_0200894a:
	movs r0, #2
	bl Func_0200006c
	cmp r0, #0
	bne .L_02008968
	movs r0, #1
	bl Func_0200006c
	cmp r0, #0
	bne .L_02008968
	adds r6, #1
	cmp r6, #25
	ble .L_0200896a
	movs r5, #1
	b .L_0200896a
.L_02008968:
	movs r6, #0
.L_0200896a:
	cmp r5, #0
	bne .L_020088b2
.L_0200896e:
	movs r0, #2
	bl Func_0200006c
	cmp r0, #0
	beq .L_02008922
.L_02008978:
	cmp r7, #0
	beq .L_02008984
	adds r0, r7, #0
	movs r1, #1
	bl Window_Close
.L_02008984:
	movs r0, #5
	bl wait
.L_0200898a:
	mov r2, r8
	cmp r2, #0
	beq .L_02008992
	b .L_02008b8a
.L_02008992:
	movs r1, #249
	lsls r1, r1, #3
	movs r0, #216
	bl HeapSlot_AllocEwram
	ldr r5, .L_02008b24
	adds r6, r0, #0
	adds r0, r5, #0
	bl Task_RemoveFar
	movs r0, #5
	bl Func_0200198c
	movs r0, #8
	bl wait
	movs r0, #5
	bl Func_02001994
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008a1a
	ldr r3, .L_02008b28
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl Actor_SetAngleToward
	ldr r0, .L_02008b2c
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl FieldMessage_ShowAtActor
	movs r0, #45
	bl wait
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #216
	movs r2, #184
	movs r0, #4
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #4
	bl Actor_WaitForMoveById
	movs r0, #4
	movs r1, #216
	movs r2, #168
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #4
	bl Actor_WaitForMoveById
	b .L_02008b00
.L_02008a1a:
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r1, #216
	movs r2, #200
	movs r0, #4
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #4
	bl Actor_WaitForMoveById
	movs r1, #200
	movs r2, #192
	lsls r1, r1, #5
	lsls r2, r2, #4
	movs r0, #4
	adds r1, #153
	adds r2, #204
	bl Actor_SetSpeedParameters
	movs r0, #4
	movs r1, #216
	movs r2, #168
	bl Actor_ResetAndSetPositionInMode2
	bl Func_020007b8
	cmp r0, #0
	bge .L_02008aec
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl Actor_SetSpeedParameters
	movs r2, #200
	movs r1, #216
	movs r0, #4
	bl Actor_ResetAndSetPositionInMode2
	movs r0, #5
	bl Func_0200198c
	movs r0, #8
	bl wait
	movs r0, #5
	bl Func_02001994
	movs r0, #4
	bl Actor_WaitForMoveById
	movs r0, #216
	bl HeapSlot_Release
	movs r0, #0
	bl LinkLobby_WriteSlotValue
	movs r0, #4
	bl LinkLobby_WriteSlotValue
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Task_Add
	movs r1, #1
	adds r0, r5, #0
	bl Func_02001944
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r2, #181
	lsls r2, r2, #1
	add r2, r10
	movs r3, #2
	strh r3, [r2]
	b .L_02008b8a
.L_02008aec:
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl Actor_SetSpeedParameters
	movs r0, #4
	bl Actor_WaitForMoveById
.L_02008b00:
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	ldr r5, .L_02008b30
	cmp r0, #0
	beq .L_02008b34
	adds r0, r5, #0
	movs r1, #8
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #9
	bl Party_SetFields1f2And1f4
	b .L_02008b44
.L_02008b20:
	.4byte MsgAwaitingYourOpponent
.L_02008b24:
	.4byte Func_02000134
.L_02008b28:
	.4byte gAdventure
.L_02008b2c:
	.4byte MsgGoodLuck2
.L_02008b30:
	.4byte 0x00000138
.L_02008b34:
	adds r0, r5, #0
	movs r1, #10
	bl Party_SetFields1eeAnd1f0
	adds r0, r5, #0
	movs r1, #11
	bl Party_SetFields1f2And1f4
.L_02008b44:
	ldr r3, .L_02008b9c
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #4
	strb r2, [r3]
	movs r0, #1
	movs r1, #1
	bl Func_02001a8c
	ldr r2, .L_02008ba0
	ldr r3, .L_02008b90
	ldr r1, .L_02008b94
	strh r3, [r2, #2]
	ldr r3, .L_02008b98
	movs r4, #217
	strh r3, [r2, #6]
	lsls r4, r4, #3
	strh r1, [r2]
	strh r1, [r2, #4]
	adds r4, #255
	movs r1, #0
	adds r0, r6, #0
.L_02008b74:
	ldr r3, .L_02008ba4
	adds r2, r1, r3
	ldrb r3, [r0]
	adds r1, #1
	adds r0, #1
	strb r3, [r2]
	cmp r1, r4
	bls .L_02008b74
	movs r0, #216
	bl HeapSlot_Release
.L_02008b8a:
	bl Func_02001a1c
	b .L_02008ba8
.L_02008b90:
	.4byte 0x00000058
.L_02008b94:
	.4byte 0x00000045
.L_02008b98:
	.4byte 0x00000043
.L_02008b9c:
	.4byte gAdventure
.L_02008ba0:
	.4byte gData_02003a74
.L_02008ba4:
	.4byte Data_02018000
.L_02008ba8:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x02008fbc,"ax",%progbits
	.balign 4
	.global Func_02000fbc
	.thumb_func
Func_02000fbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r3, .L_02009118
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	mov r0, r8
	bl Actor_SetAngleToward
	ldr r0, .L_0200911c
	bl Func_02001a64
	mov r0, r8
	movs r1, #0
	bl Func_02001a74
	ldr r3, .L_02009120
	movs r2, #146
	ldr r3, [r3]
	lsls r2, r2, #2
	movs r5, #0
	ands r3, r2
	mov r9, r5
	cmp r3, r2
	bne .L_0200900a
	movs r3, #1
	mov r9, r3
.L_0200900a:
	movs r0, #78
	bl Func_02001ad4
.L_02009010:
	movs r3, #2
	str r3, [sp, #0]
	movs r2, #6
	movs r0, #12
	movs r1, #8
	movs r3, #3
	bl Window_Create
	movs r2, #1
	negs r2, r2
	adds r7, r0, #0
	mov r10, r2
.L_02009028:
	cmp r5, r10
	beq .L_02009042
	adds r0, r7, #0
	bl WindowSprite_PrepareForRedraw
	movs r3, #0
	adds r0, r5, #0
	movs r1, #3
	adds r2, r7, #0
	str r3, [sp, #0]
	bl Text_DrawNumber
	mov r10, r5
.L_02009042:
	ldr r1, .L_02009120
	movs r2, #32
	ldr r3, [r1, #12]
	movs r6, #0
	ands r3, r2
	cmp r3, #0
	beq .L_02009052
	subs r6, #1
.L_02009052:
	ldr r3, [r1, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_0200906e
	movs r6, #1
	b .L_0200906e
.L_02009060:
	ldr r2, .L_02009124
	lsls r3, r5, #1
	ldrh r0, [r2, r3]
	bl Func_02001adc
	cmp r0, #0
	bne .L_020090a0
.L_0200906e:
	cmp r6, #0
	beq .L_020090a0
	adds r5, r5, r6
	cmp r5, #0
	bge .L_0200907a
	movs r5, #96
.L_0200907a:
	cmp r5, #96
	bls .L_02009080
	movs r5, #0
.L_02009080:
	mov r3, r9
	cmp r3, #0
	bne .L_020090a0
	ldr r3, .L_02009118
	movs r2, #152
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_020090a0
	ldr r3, .L_02009128
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02009060
.L_020090a0:
	ldr r1, .L_02009120
	movs r2, #1
	ldr r3, [r1, #4]
	ands r3, r2
	cmp r3, #0
	bne .L_020090be
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_020090d6
	movs r0, #1
	bl wait
	b .L_02009028
.L_020090be:
	adds r0, r7, #0
	movs r1, #1
	bl Window_Close
	cmp r5, #0
	blt .L_020090dc
	ldr r3, .L_02009124
	lsls r2, r5, #1
	ldrh r0, [r3, r2]
	bl Func_02001ad4
	b .L_02009010
.L_020090d6:
	movs r5, #1
	negs r5, r5
	b .L_020090be
.L_020090dc:
	movs r0, #78
	bl Func_02001ad4
	movs r0, #120
	bl wait
	movs r0, #227
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02001ad4
	ldr r0, .L_0200912c
	bl Func_02001a64
	mov r0, r8
	movs r1, #0
	bl Func_02001a74
	movs r0, #10
	bl wait
	bl Func_02001a1c
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009118:
	.4byte gAdventure
.L_0200911c:
	.4byte MsgYouHaveA
.L_02009120:
	.4byte gInput
.L_02009124:
	.4byte gData_02001b02
.L_02009128:
	.4byte gData_02003860
.L_0200912c:
	.4byte MsgComeBackWhen
	.section .text.x0200945c,"ax",%progbits
	.global Func_0200145c
	.thumb_func
Func_0200145c:
	push {r5, r6, r7, lr}
	movs r0, #191
	lsls r0, r0, #1
	sub sp, #8
	bl GameFlag_SetBit
	ldr r5, .L_0200950c
	movs r1, #144
	lsls r1, r1, #2
	movs r2, #0
	adds r3, r5, r1
	adds r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	strh r2, [r3]
	ldr r3, .L_02009510
	movs r0, #2
	str r2, [r3]
	ldr r3, .L_02009514
	str r2, [r3]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Sound_LoadPresetParameters
	movs r3, #180
	lsls r3, r3, #2
	adds r5, r5, r3
	ldrh r0, [r5]
	bl Func_02001414
	movs r3, #13
	movs r2, #10
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #1
	movs r2, #1
	movs r1, #11
	movs r0, #11
	bl Func_0200199c
	movs r0, #4
	bl LinkLobby_WriteSlotValue
	movs r0, #1
	bl wait
	movs r0, #5
	bl Func_02001994
	ldr r2, .L_02009518
	ldr r3, .L_020094fc
	movs r5, #0
	strh r3, [r2, #8]
	ldr r3, .L_02009500
	strh r3, [r2, #10]
	ldr r3, .L_02009504
	strh r3, [r2, #12]
	ldr r3, .L_02009508
	strh r3, [r2, #14]
.L_020094de:
	movs r1, #188
	lsls r1, r1, #2
	adds r6, r5, r1
	adds r0, r6, #0
	bl GameFlag_ClearBit
	adds r0, r5, #0
	bl Func_02000f6c
	cmp r0, #0
	beq .L_0200951c
	adds r0, r6, #0
	bl GameFlag_SetBit
	b .L_0200951c
.L_020094fc:
	.4byte 0x00000054
.L_02009500:
	.4byte 0x00000041
.L_02009504:
	.4byte 0x0000004c
.L_02009508:
	.4byte 0x0000004b
.L_0200950c:
	.4byte gAdventure
.L_02009510:
	.4byte gData_020022bc
.L_02009514:
	.4byte gData_020022b8
.L_02009518:
	.4byte gData_02003a74
.L_0200951c:
	adds r5, #1
	cmp r5, #7
	ble .L_020094de
	ldr r6, .L_020097f4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #8
	beq .L_02009534
	b .L_02009640
.L_02009534:
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #5
	bl LinkLobby_WriteSlotValue
	movs r3, #177
	lsls r3, r3, #2
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r1, #128
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #2
	adds r1, #202
	adds r2, r6, r1
	ldrh r3, [r2]
	movs r0, #254
	adds r3, #1
	strh r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_GetByte
	lsls r0, r0, #24
	asrs r6, r0, #24
	lsls r3, r6, #1
	adds r5, r3, #2
	cmp r5, #14
	ble .L_0200957c
	movs r5, #14
.L_0200957c:
	movs r0, #250
	lsls r0, r0, #2
	bl GameFlag_GetByte
	cmp r0, #2
	bne .L_02009598
	movs r0, #250
	lsls r0, r0, #2
	movs r1, #0
	bl GameFlag_SetByte
	adds r6, #1
	adds r5, #1
	b .L_020095a2
.L_02009598:
	adds r1, r0, #1
	movs r0, #250
	lsls r0, r0, #2
	bl GameFlag_SetByte
.L_020095a2:
	ldr r7, .L_020097f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl Actor_SetAngleToward
	ldr r0, .L_020097f8
	adds r0, r5, r0
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl FieldMessage_ShowAtActor
	movs r0, #4
	movs r1, #0
	bl Event_AskYesNoWithActorAnimation
	cmp r0, #0
	bne .L_020095e2
	cmp r6, #90
	ble .L_020095d6
	movs r6, #90
.L_020095d6:
	movs r0, #254
	lsls r0, r0, #2
	adds r1, r6, #0
	bl GameFlag_SetByte
	b .L_020096e4
.L_020095e2:
	movs r0, #116
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #254
	movs r1, #1
	lsls r0, r0, #2
	negs r1, r1
	bl GameFlag_SetByte
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #202
	adds r5, r7, r3
	ldrh r0, [r5]
	movs r1, #5
	bl Func_020019e4
	movs r1, #178
	lsls r1, r1, #2
	adds r2, r7, r1
	ldrh r5, [r5]
	ldrh r3, [r2]
	cmp r3, r5
	bcs .L_0200962a
	strh r5, [r2]
	ldr r0, .L_020097fc
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl FieldMessage_ShowAtActor
	bl Func_02001400
	b .L_02009638
.L_0200962a:
	ldr r0, .L_02009800
	bl Func_02001a64
	movs r0, #8
	movs r1, #0
	bl FieldMessage_ShowAtActor
.L_02009638:
	movs r0, #0
	bl LinkLobby_WriteSlotValue
	b .L_020096e4
.L_02009640:
	cmp r3, #9
	bne .L_020096ea
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #198
	adds r2, r6, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #5
	bl LinkLobby_WriteSlotValue
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r6, r1
	ldr r1, [r3]
	movs r0, #8
	movs r2, #0
	bl Actor_SetAngleToward
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #202
	adds r5, r6, r2
	ldrh r0, [r5]
	movs r1, #5
	bl Func_020019e4
	movs r3, #178
	lsls r3, r3, #2
	adds r2, r6, r3
	ldrh r5, [r5]
	ldrh r3, [r2]
	cmp r3, r5
	bcs .L_020096ae
	strh r5, [r2]
	ldr r0, .L_020097fc
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl FieldMessage_ShowAtActor
	bl Func_02001400
	b .L_020096bc
.L_020096ae:
	ldr r0, .L_02009804
	bl Func_02001a64
	movs r0, #8
	movs r1, #0
	bl FieldMessage_ShowAtActor
.L_020096bc:
	ldr r3, .L_020097f4
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #202
	adds r3, r3, r1
	movs r2, #0
	movs r0, #116
	strh r2, [r3]
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #254
	movs r1, #1
	lsls r0, r0, #2
	negs r1, r1
	bl GameFlag_SetByte
	movs r0, #0
.L_020096e0:
	bl LinkLobby_WriteSlotValue
.L_020096e4:
	bl Func_02001a1c
	b .L_02009876
.L_020096ea:
	cmp r3, #10
	bne .L_02009796
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #0
	bl LinkLobby_WriteSlotValue
	movs r0, #4
	bl LinkLobby_WriteSlotValue
	movs r0, #250
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200974c
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #250
	ldr r5, [r3, #108]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r0, #193
	movs r3, #2
	strh r3, [r2]
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #20
	bl wait
	bl Func_02000044
	movs r0, #0
	bl LinkLobby_WriteSlotValue
	movs r0, #4
	b .L_020096e0
.L_0200974c:
	movs r1, #179
	lsls r1, r1, #2
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #210
	adds r2, r6, r3
	ldrh r3, [r2]
	adds r1, r3, #1
	strh r1, [r2]
	movs r2, #180
	lsls r2, r2, #2
	adds r5, r6, r2
	ldrh r2, [r5]
	lsls r3, r1, #16
	lsrs r3, r3, #16
	cmp r2, r3
	bcs .L_02009778
	strh r1, [r5]
.L_02009778:
	ldrh r0, [r5]
	bl Func_02001414
	bl Func_020013ec
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_SetBit
	b .L_020096e4
.L_02009796:
	cmp r3, #11
	bne .L_02009808
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #0
	bl LinkLobby_WriteSlotValue
	movs r0, #4
	bl LinkLobby_WriteSlotValue
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020097e0
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #206
	adds r3, r6, r1
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #210
	adds r3, r6, r2
	strh r0, [r3]
	bl Func_020013ec
.L_020097e0:
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_ClearBit
	b .L_020096e4
.L_020097f4:
	.4byte gAdventure
.L_020097f8:
	.4byte MsgNotBadWould
.L_020097fc:
	.4byte MsgYouWonBattle
.L_02009800:
	.4byte MsgYouWonBattlePlease
.L_02009804:
	.4byte MsgYouWonBattleI
.L_02009808:
	bl Func_02001914
	movs r0, #185
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #254
	movs r1, #1
	lsls r0, r0, #2
	negs r1, r1
	bl GameFlag_SetByte
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #74
	adds r5, r6, r3
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_02009862
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r6, r1
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl Actor_SetAngleToward
	ldr r0, .L_020098b8
	bl Func_02001a64
	movs r0, #8
	movs r1, #0
	bl Func_02001a74
	bl Func_02001a1c
.L_02009862:
	ldr r3, .L_020098bc
	movs r2, #0
	strb r2, [r5]
	movs r0, #0
	strb r2, [r3]
	bl LinkLobby_WriteSlotValue
	movs r0, #4
	bl LinkLobby_WriteSlotValue
.L_02009876:
	ldr r5, .L_020098c0
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Task_Add
	movs r1, #1
	adds r0, r5, #0
	bl Func_02001944
	ldr r3, .L_020098c4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #8
	bne .L_020098a6
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020098b0
.L_020098a6:
	movs r0, #1
	bl Func_02001a04
	bl DjinnRecovery_CountDownTwenty
.L_020098b0:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020098b8:
	.4byte MsgNoteYouCan
.L_020098bc:
	.4byte gAutoSleepEnabled
.L_020098c0:
	.4byte Func_02000134
.L_020098c4:
	.4byte gAdventure
	.section .bss,"aw",%nobits
	.global gData_020022b8
gData_020022b8:
	.space 0x00000004
	.global gData_020022bc
gData_020022bc:
