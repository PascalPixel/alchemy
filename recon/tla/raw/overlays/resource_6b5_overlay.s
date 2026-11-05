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
	.section .bss,"aw",%nobits
	.global gData_020022b8
gData_020022b8:
	.space 0x00000004
	.global gData_020022bc
gData_020022bc:
