.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_02001bc4
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
	push {r5, r6, lr}
	ldr r5, .L_02008058
	ldrh r6, [r5]
	strh r5, [r5]
	bl Func_0200191c
	bl Func_02001914
	strh r6, [r5]
	pop {r5, r6, pc}
.L_02008058:
	.4byte 0x04000208
	.section .text.x0200805c,"ax",%progbits
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	ldr r0, .L_02008060
	bx lr
.L_02008060:
	.4byte Data_02001c6c
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	ldr r0, .L_02008068
	bx lr
.L_02008068:
	.4byte Data_02001c70
	.section .text.x0200806c,"ax",%progbits
	.global Func_0200006c
	.thumb_func
Func_0200006c:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008100
	movs r5, #1
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	adds r7, r0, #0
	negs r5, r5
	cmp r3, #3
	bne .L_020080de
	ldr r3, .L_02008104
	movs r0, #129
	ldr r3, [r3]
	lsls r0, r0, #2
	lsls r3, r3, #26
	adds r0, #255
	lsrs r5, r3, #30
	bl GameFlag_SetBit
	b .L_020080e8
.L_02008094:
	ldr r3, .L_02008108
	lsls r2, r7, #2
	adds r6, r2, r3
	cmp r5, #0
	beq .L_020080aa
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	b .L_020080b4
.L_020080aa:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
.L_020080b4:
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	movs r3, #1
	eors r0, r3
	lsls r2, r0, #1
	ldr r3, .L_0200810c
	adds r2, r2, r0
	lsls r2, r2, #3
	adds r2, r2, r3
	ldr r3, .L_02008110
	ldrb r3, [r3, r7]
	lsls r3, r3, #2
	ldr r2, [r2, r3]
	ldr r3, [r6]
	cmp r2, r3
	bne .L_020080fa
	movs r0, #1
	b .L_020080fc
.L_020080de:
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_ClearBit
.L_020080e8:
	cmp r5, #0
	blt .L_020080fa
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008094
.L_020080fa:
	movs r0, #0
.L_020080fc:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008100:
	.4byte gLinkStatus
.L_02008104:
	.4byte 0x04000128
.L_02008108:
	.4byte Data_02001ae4
.L_0200810c:
	.4byte Data_02003874
.L_02008110:
	.4byte Data_02001afc
	.section .text.x02008114,"ax",%progbits
	.global Func_02000114
	.thumb_func
Func_02000114:
	ldr r3, .L_02008128
	ldr r2, .L_0200812c
	lsls r1, r0, #2
	ldrb r3, [r3, r0]
	ldr r4, .L_02008130
	ldr r2, [r1, r2]
	lsls r3, r3, #2
	str r2, [r3, r4]
	bx lr
	.2byte 0x0000
.L_02008128:
	.4byte Data_02001afc
.L_0200812c:
	.4byte Data_02001ae4
.L_02008130:
	.4byte Data_02003a74
	.section .text.x02008134,"ax",%progbits
	.global Func_02000134
	.thumb_func
Func_02000134:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r2, #1
	movs r0, #4
	ldr r7, [r3, #108]
	mov r8, r2
	bl Object_GetById
	ldr r3, [r0, #16]
	movs r2, #224
	lsls r2, r2, #16
	cmp r3, r2
	ble .L_0200815c
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_0200815c:
	movs r2, #181
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #2
	beq .L_02008242
	movs r0, #0
	bl Func_0200006c
	movs r0, #129
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_Test
	ldr r2, .L_020082cc
	cmp r0, #0
	bne .L_020081b0
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
	cmp r3, #25
	ble .L_020081b4
	movs r6, #0
	movs r5, #3
.L_0200818e:
	ldr r0, .L_020082d0
	ldr r3, .L_020082d4
	adds r0, r6, r0
	movs r1, #20
	subs r5, #1
	mov lr, r3
	.2byte 0xf800
	adds r6, #24
	cmp r5, #0
	bge .L_0200818e
	ldr r2, .L_020082cc
	movs r3, #0
	str r3, [r2]
	movs r0, #4
	bl Func_02000114
	b .L_020081b4
.L_020081b0:
	movs r3, #0
	str r3, [r2]
.L_020081b4:
	ldr r3, .L_020082cc
	ldr r3, [r3]
	cmp r3, #0
	bne .L_02008210
	movs r0, #0
	bl Func_0200006c
	cmp r0, #0
	beq .L_02008202
	movs r0, #1
	bl Func_0200006c
	cmp r0, #0
	bne .L_020081da
	movs r0, #2
	bl Func_0200006c
	cmp r0, #0
	beq .L_02008202
.L_020081da:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020081fc
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #1
	strh r3, [r2]
.L_020081fc:
	movs r2, #1
	mov r8, r2
	b .L_02008210
.L_02008202:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r3, #0
	mov r8, r3
.L_02008210:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008242
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008242
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008242
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #1
	strh r3, [r2]
.L_02008242:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200825e
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082aa
.L_0200825e:
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020082aa
	movs r0, #0
	bl Func_0200006c
	cmp r0, #0
	bne .L_020082aa
	ldr r3, .L_020082cc
	ldr r3, [r3]
	cmp r3, #24
	ble .L_020082aa
	movs r3, #181
	lsls r3, r3, #1
	movs r0, #131
	adds r2, r7, r3
	lsls r0, r0, #1
	movs r3, #2
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
	bl Func_02000114
.L_020082aa:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020082c2
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #2
	strh r3, [r2]
.L_020082c2:
	mov r0, r8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082cc:
	.4byte gOverlayArea + 0x22b8
.L_020082d0:
	.4byte Data_02003874
.L_020082d4:
	.4byte IwramClearWords
	.section .text.x020082d8,"ax",%progbits
	.global Func_020002d8
	.thumb_func
Func_020002d8:
	push {lr}
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008338
	ldr r2, .L_0200833c
	movs r1, #150
	ldr r3, [r2]
	lsls r1, r1, #1
	adds r3, #1
	str r3, [r2]
	cmp r3, r1
	bne .L_02008302
	str r0, [r2]
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
.L_02008302:
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008338
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r0, .L_02008340
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #5
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02001a1c
.L_02008338:
	pop {pc}
	.2byte 0x0000
.L_0200833c:
	.4byte gOverlayArea + 0x22bc
.L_02008340:
	.4byte 0x000013ea
	.section .text.x02008344,"ax",%progbits
	.global Func_02000344
	.thumb_func
Func_02000344:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #4
	ldr r5, [r3, #108]
	bl Func_02000114
	movs r0, #128
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #0
	strh r3, [r2]
	ldr r0, .L_02008398
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_02001a1c
	pop {r5, pc}
	.2byte 0x0000
.L_02008398:
	.4byte 0x000013e3
	.section .text.x0200839c,"ax",%progbits
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
	bl Runtime_BumpAllocateAlternatePool
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
	bl WaitFrames
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
	bl WaitFrames
	ldr r0, .L_02008534
	mov r1, sp
	bl Ui_AdjustValueWithoutLimit
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
	bl Owner_GetState
	adds r6, r0, #0
	bl Party_Check
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
	bl WaitFrames
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
	bl WaitFrames
	b .L_02008516
.L_020084ec:
	mov r0, r9
	bl Sys_Free
	movs r2, #170
	lsls r2, r2, #1
	mov r11, r2
	mov r0, r11
	bl Runtime_BumpAllocateAlternatePool
	mov r9, r0
	movs r0, #1
	bl Trade_GetOfferState
	bl Party_Check
	movs r3, #1
	negs r3, r3
	movs r5, #0
	cmp r0, r3
	bne .L_020084c8
.L_02008514:
	adds r7, r0, #0
.L_02008516:
	mov r0, r9
	bl Sys_Free
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
	.4byte Data_02005354
.L_02008534:
	.4byte 0x00000c58
	.section .text.x02008538,"ax",%progbits
	.global Func_02000538
	.thumb_func
Func_02000538:
	push {r5, lr}
	adds r5, r0, #0
	bl Party_CountActiveOwners
	cmp r0, #3
	ble .L_02008546
	movs r0, #3
.L_02008546:
	movs r2, #0
	cmp r2, r0
	bge .L_02008564
	ldr r1, .L_02008574
.L_0200854e:
	movs r4, #134
	lsls r4, r4, #2
	adds r3, r2, r4
	ldrb r3, [r1, r3]
	cmp r5, #0
	beq .L_0200855e
	strh r3, [r5]
	adds r5, #2
.L_0200855e:
	adds r2, #1
	cmp r2, r0
	blt .L_0200854e
.L_02008564:
	cmp r5, #0
	beq .L_0200856c
	ldr r3, .L_02008570
	strh r3, [r5]
.L_0200856c:
	pop {r5, pc}
	.2byte 0x0000
.L_02008570:
	.4byte 0x000000ff
.L_02008574:
	.4byte gPartyState
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
	bl Runtime_BumpAllocateAlternatePool
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
	bl Owner_GetState
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
	bl WaitFrames
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
	bl WaitFrames
	ldr r2, [sp, #4]
	adds r6, #1
	cmp r6, r2
	blt .L_020085c4
	b .L_02008690
.L_02008654:
	movs r0, #1
	bl WaitFrames
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
	bl WaitFrames
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
	bl Sys_Free
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	adds r7, r0, #0
	movs r0, #0
	bl Trade_GetOfferState
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
	bl WaitFrames
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
	bl WaitFrames
	movs r0, #2
	bl WaitFrames
.L_0200879a:
	adds r0, r7, #0
	bl Sys_Free
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
	.section .text.x020087b8,"ax",%progbits
	.global Func_020007b8
	.thumb_func
Func_020007b8:
	push {r5, r6, lr}
	movs r6, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	ldr r3, .L_02008848
	strb r6, [r3]
	cmp r0, #0
	bne .L_020087f2
	movs r0, #5
	bl WaitFrames
	bl Func_02000578
	adds r6, r0, #0
	cmp r6, #0
	blt .L_0200881e
	movs r0, #5
	bl WaitFrames
	bl Func_0200039c
	adds r6, r0, #0
	adds r5, r6, #0
	cmp r6, #0
	bge .L_0200880e
	b .L_0200881a
.L_020087f2:
	bl Func_0200039c
	adds r6, r0, #0
	adds r5, r6, #0
	cmp r6, #0
	blt .L_0200881e
	movs r0, #10
	bl WaitFrames
	bl Func_02000578
	adds r6, r0, #0
	cmp r6, #0
	blt .L_0200881e
.L_0200880e:
	movs r0, #252
	lsls r0, r0, #2
	adds r1, r5, #0
	bl GameFlag_SetByte
	adds r6, r5, #0
.L_0200881a:
	cmp r5, #0
	bge .L_02008842
.L_0200881e:
	ldr r1, .L_0200884c
	ldr r0, .L_02008850
	ldrh r4, [r0]
	strh r0, [r0]
	movs r2, #0
	movs r3, #128
	strb r3, [r1, #1]
	ldr r3, .L_02008854
	strb r2, [r1, #3]
	str r2, [r3]
	ldr r3, .L_02008858
	strb r2, [r1, #2]
	strh r2, [r3]
	ldr r3, .L_0200885c
	str r2, [r3]
	ldr r3, .L_02008860
	strh r2, [r3]
	strh r4, [r0]
.L_02008842:
	adds r0, r6, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008848:
	.4byte Data_020054c0
.L_0200884c:
	.4byte Data_02003a70
.L_02008850:
	.4byte 0x04000208
.L_02008854:
	.4byte Data_020038d0
.L_02008858:
	.4byte Data_020036d4
.L_0200885c:
	.4byte Data_020055d0
.L_02008860:
	.4byte Data_02005354
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
	bl Func_02000114
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
	bl Func_02000114
	movs r0, #2
	bl Func_0200006c
	cmp r0, #0
	bne .L_0200896e
	ldr r0, .L_02008b20
	movs r1, #5
	movs r2, #4
	movs r3, #1
	bl UiText_OpenMessageWindow
	adds r7, r0, #0
	b .L_0200896e
.L_02008922:
	movs r0, #1
	bl WaitFrames
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
	bl UiWork_Finalize
.L_02008984:
	movs r0, #5
	bl WaitFrames
.L_0200898a:
	mov r2, r8
	cmp r2, #0
	beq .L_02008992
	b .L_02008b8a
.L_02008992:
	movs r1, #249
	lsls r1, r1, #3
	movs r0, #216
	bl Runtime_AllocateBlock
	ldr r5, .L_02008b24
	adds r6, r0, #0
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #5
	bl Func_0200198c
	movs r0, #8
	bl WaitFrames
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
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_02008b2c
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #45
	bl WaitFrames
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #216
	movs r2, #184
	movs r0, #4
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #4
	movs r1, #216
	movs r2, #168
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	b .L_02008b00
.L_02008a1a:
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r1, #216
	movs r2, #200
	movs r0, #4
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r1, #200
	movs r2, #192
	lsls r1, r1, #5
	lsls r2, r2, #4
	movs r0, #4
	adds r1, #153
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r0, #4
	movs r1, #216
	movs r2, #168
	bl ObjectMotion_ResetAndSetPositionInMode2
	bl Func_020007b8
	cmp r0, #0
	bge .L_02008aec
	movs r1, #128
	movs r2, #128
	movs r0, #4
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl ObjectMotion_SetSpeedParameters
	movs r2, #200
	movs r1, #216
	movs r0, #4
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r0, #5
	bl Func_0200198c
	movs r0, #8
	bl WaitFrames
	movs r0, #5
	bl Func_02001994
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #216
	bl Runtime_ReleaseHeapBlock
	movs r0, #0
	bl Func_02000114
	movs r0, #4
	bl Func_02000114
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
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
	bl ObjectMotion_SetSpeedParameters
	movs r0, #4
	bl ObjectMotion_CommitCurrentPositionAndActivate
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
	.4byte 0x000013e4
.L_02008b24:
	.4byte Func_02000134
.L_02008b28:
	.4byte gPartyState
.L_02008b2c:
	.4byte 0x000013f7
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
	bl Runtime_ReleaseHeapBlock
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
	.4byte gPartyState
.L_02008ba0:
	.4byte Data_02003a74
.L_02008ba4:
	.4byte Data_02018000
.L_02008ba8:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.section .text.x02008bb0,"ax",%progbits
	.global Func_02000bb0
	.thumb_func
Func_02000bb0:
	push {r5, r6, r7, lr}
	ldr r6, .L_02008d94
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r7, .L_02008d98
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r7, r2
	movs r0, #8
	ldr r1, [r3]
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #0
	bl Func_0200006c
	cmp r0, #0
	bne .L_02008be0
	movs r0, #1
	bl WaitFrames
.L_02008be0:
	movs r0, #0
	bl Func_0200006c
	cmp r0, #0
	bne .L_02008c70
	movs r0, #5
	bl Func_02000114
	bl Func_02000044
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cb0
	adds r0, r6, #5
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	adds r5, r0, #0
	cmp r5, #0
	bne .L_02008c56
	movs r0, #250
	movs r1, #0
	lsls r0, r0, #2
	bl GameFlag_SetByte
	movs r0, #116
	adds r0, #255
	bl GameFlag_SetBit
	movs r0, #185
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #202
	adds r3, r7, r2
	adds r0, r6, #7
	strh r5, [r3]
	b .L_02008cb2
.L_02008c56:
	movs r0, #116
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #0
	bl Func_02000114
	adds r0, r6, #6
	b .L_02008cb2
.L_02008c70:
	movs r0, #116
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ca2
	movs r0, #0
	bl Func_02000114
	ldr r0, .L_02008d9c
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_ClearBit
	movs r0, #116
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008ca2:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008cc0
.L_02008cb0:
	adds r0, r6, #3
.L_02008cb2:
	bl Func_02001a64
.L_02008cb6:
	movs r0, #8
	movs r1, #0
	bl UiText_OpenMessageAtObject
	b .L_02008d8c
.L_02008cc0:
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cf2
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008cf2
	adds r0, r6, #0
	bl Func_02001a64
	movs r0, #8
	movs r1, #0
	bl UiText_OpenMessageAtObject
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_02008d8c
.L_02008cf2:
	movs r0, #192
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d10
	adds r0, r6, #2
	bl Func_02001a64
	b .L_02008d16
.L_02008d10:
	adds r0, r6, #1
	bl Func_02001a64
.L_02008d16:
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008d7e
	movs r0, #0
	bl Func_0200006c
	cmp r0, #0
	beq .L_02008d72
	movs r0, #182
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #185
	lsls r0, r0, #1
	bl GameFlag_SetBit
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008d5a
	adds r0, r6, #3
	bl Func_02001a64
	b .L_02008d60
.L_02008d5a:
	adds r0, r6, #4
	bl Func_02001a64
.L_02008d60:
	movs r0, #1
	bl Func_02000114
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	b .L_02008cb6
.L_02008d72:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008d8c
.L_02008d7e:
	adds r0, r6, #0
	bl Func_02001a64
	movs r0, #8
	movs r1, #0
	bl UiText_OpenMessageAtObject
.L_02008d8c:
	bl Func_02001a1c
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008d94:
	.4byte 0x000013ec
.L_02008d98:
	.4byte gPartyState
.L_02008d9c:
	.4byte 0x000013f9
	.section .text.x02008da0,"ax",%progbits
	.global Func_02000da0
	.thumb_func
Func_02000da0:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r0, #0
	movs r0, #4
	bl Object_GetById
	ldrh r5, [r0, #6]
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r6, .L_02008e34
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r1, [r3]
	movs r2, #0
	adds r0, r7, #0
	bl ObjectMotion_SetAngleToward
	ldr r3, .L_02008e38
	movs r2, #252
	lsls r2, r2, #6
	adds r5, r5, r3
	adds r2, #254
	cmp r5, r2
	bhi .L_02008dee
	ldr r3, .L_02008e3c
	movs r2, #179
	lsls r2, r2, #2
	adds r5, r6, r2
	mov r8, r3
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_02008e00
	ldr r0, .L_02008e40
	b .L_02008e10
.L_02008dee:
	movs r2, #128
	ldr r3, .L_02008e44
	lsls r2, r2, #2
	adds r2, #210
	adds r5, r6, r2
	mov r8, r3
	ldrh r3, [r5]
	cmp r3, #0
	beq .L_02008e1e
.L_02008e00:
	bl UiWork_ClearValueNameTables
	movs r1, #5
	ldrh r0, [r5]
	bl Func_020019e4
	mov r0, r8
	adds r0, #1
.L_02008e10:
	bl Func_02001a64
	adds r0, r7, #0
	movs r1, #0
	bl Func_02001a74
	b .L_02008e2c
.L_02008e1e:
	ldr r0, .L_02008e48
	bl Func_02001a64
	adds r0, r7, #0
	movs r1, #0
	bl Func_02001a74
.L_02008e2c:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e34:
	.4byte gPartyState
.L_02008e38:
	.4byte 0xffff5fff
.L_02008e3c:
	.4byte 0x0000145e
.L_02008e40:
	.4byte 0x00001472
.L_02008e44:
	.4byte 0x00001460
.L_02008e48:
	.4byte 0x00001473
	.section .text.x02008e4c,"ax",%progbits
	.global Func_02000e4c
	.thumb_func
Func_02000e4c:
	push {r5, r6, r7, lr}
	sub sp, #4
	adds r5, r0, #0
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r3, .L_02008f38
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r2, #0
	adds r0, r5, #0
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_02008f3c
	bl Func_02001a64
	adds r0, r5, #0
	movs r1, #0
	bl Func_02001a74
	movs r3, #2
	str r3, [sp, #0]
	movs r0, #0
	movs r1, #0
	movs r2, #6
	movs r3, #4
	bl UiWindow_Create
	movs r7, #1
	adds r6, r0, #0
	movs r5, #0
	negs r7, r7
	b .L_02008ea6
.L_02008e96:
	ldr r3, [r1, #4]
	movs r2, #2
	ands r3, r2
	cmp r3, #0
	bne .L_02008efe
	movs r0, #1
	bl WaitFrames
.L_02008ea6:
	cmp r5, r7
	beq .L_02008ec0
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedraw
	movs r3, #0
	adds r0, r5, #0
	movs r1, #3
	adds r2, r6, #0
	str r3, [sp, #0]
	bl UiText_DrawNumber
	adds r7, r5, #0
.L_02008ec0:
	ldr r1, .L_02008f40
	movs r2, #32
	ldr r3, [r1, #12]
	ands r3, r2
	cmp r3, #0
	beq .L_02008ece
	subs r5, #1
.L_02008ece:
	ldr r3, [r1, #12]
	movs r2, #16
	ands r3, r2
	cmp r3, #0
	beq .L_02008eda
	adds r5, #1
.L_02008eda:
	cmp r5, #0
	bge .L_02008ee0
	movs r5, #0
.L_02008ee0:
	ldr r3, [r1, #4]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_02008e96
.L_02008eea:
	adds r0, r6, #0
	movs r1, #1
	bl UiWork_Finalize
	cmp r5, #0
	blt .L_02008f04
	adds r0, r5, #0
	bl DebugParty_LoadPreset
	b .L_02008f08
.L_02008efe:
	movs r5, #1
	negs r5, r5
	b .L_02008eea
.L_02008f04:
	ldr r0, .L_02008f44
	b .L_02008f0e
.L_02008f08:
	cmp r0, #0
	beq .L_02008f1c
	ldr r0, .L_02008f48
.L_02008f0e:
	bl Func_02001a64
	movs r0, #9
	movs r1, #0
	bl Func_02001a74
	b .L_02008f2a
.L_02008f1c:
	ldr r0, .L_02008f4c
	bl Func_02001a64
	movs r0, #9
	movs r1, #0
	bl Func_02001a74
.L_02008f2a:
	movs r0, #10
	bl WaitFrames
	bl Func_02001a1c
	add sp, #4
	pop {r5, r6, r7, pc}
.L_02008f38:
	.4byte gPartyState
.L_02008f3c:
	.4byte 0x00000e2f
.L_02008f40:
	.4byte gInput
.L_02008f44:
	.4byte 0x00000e30
.L_02008f48:
	.4byte 0x00000e31
.L_02008f4c:
	.4byte 0x00000e32
	.section .text.x02008f50,"ax",%progbits
	.global Func_02000f50
	.thumb_func
Func_02000f50:
	push {r5, lr}
	adds r5, r0, #0
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	adds r0, r5, #0
	bl Func_02001abc
	bl Func_02001a1c
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008f6c,"ax",%progbits
	.global Func_02000f6c
	.thumb_func
Func_02000f6c:
	push {r5, r6, r7, lr}
	adds r7, r0, #0
	bl Party_CountActiveOwners
	adds r5, r0, #0
	movs r0, #185
	lsls r0, r0, #1
	movs r6, #3
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f86
	movs r6, #4
.L_02008f86:
	cmp r5, r6
	ble .L_02008f8c
	adds r5, r6, #0
.L_02008f8c:
	movs r1, #0
	cmp r1, r5
	bge .L_02008fae
.L_02008f92:
	ldr r0, .L_02008fb4
	movs r3, #134
	lsls r3, r3, #2
	adds r2, r1, r3
	ldrb r3, [r0, r2]
	cmp r3, #255
	beq .L_02008fae
	ldrb r3, [r0, r2]
	movs r0, #1
	cmp r3, r7
	beq .L_02008fb0
	adds r1, #1
	cmp r1, r5
	blt .L_02008f92
.L_02008fae:
	movs r0, #0
.L_02008fb0:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008fb4:
	.4byte gPartyState
	.4byte 0x00004770
	.section .text.x02008fbc,"ax",%progbits
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
	bl ObjectMotion_SetAngleToward
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
	bl UiWindow_Create
	movs r2, #1
	negs r2, r2
	adds r7, r0, #0
	mov r10, r2
.L_02009028:
	cmp r5, r10
	beq .L_02009042
	adds r0, r7, #0
	bl RenderOutput_PrepareForRedraw
	movs r3, #0
	adds r0, r5, #0
	movs r1, #3
	adds r2, r7, #0
	str r3, [sp, #0]
	bl UiText_DrawNumber
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
	bl WaitFrames
	b .L_02009028
.L_020090be:
	adds r0, r7, #0
	movs r1, #1
	bl UiWork_Finalize
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
	bl WaitFrames
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
	bl WaitFrames
	bl Func_02001a1c
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009118:
	.4byte gPartyState
.L_0200911c:
	.4byte 0x00001485
.L_02009120:
	.4byte gInput
.L_02009124:
	.4byte Data_02001b02
.L_02009128:
	.4byte Data_02003860
.L_0200912c:
	.4byte 0x00001486
	.section .text.x02009130,"ax",%progbits
	.global Func_02001130
	.thumb_func
Func_02001130:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	movs r0, #4
	ldr r5, .L_020091f0
	bl Func_02000f6c
	mov r8, r0
	adds r0, r6, #0
	bl Func_02000f6c
	adds r7, r0, #0
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r3, .L_020091f4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r0, r6, #0
	ldr r1, [r3]
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #193
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091ac
	movs r0, #189
	lsls r0, r0, #2
	bl GameFlag_Test
	movs r3, #188
	lsls r3, r3, #2
	adds r0, r6, r3
	bl GameFlag_Test
	adds r5, r0, #0
	movs r0, #192
	lsls r0, r0, #2
	adds r0, #5
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020091a0
	cmp r5, #0
	beq .L_0200919c
	ldr r5, .L_020091f8
	b .L_020091bc
.L_0200919c:
	ldr r5, .L_020091fc
	b .L_020091bc
.L_020091a0:
	cmp r5, #0
	beq .L_020091a8
	ldr r5, .L_02009200
	b .L_020091bc
.L_020091a8:
	ldr r5, .L_02009204
	b .L_020091bc
.L_020091ac:
	mov r2, r8
	cmp r2, #0
	beq .L_020091ba
	cmp r7, #0
	bne .L_020091bc
	ldr r5, .L_02009208
	b .L_020091bc
.L_020091ba:
	ldr r5, .L_0200920c
.L_020091bc:
	adds r0, r5, r6
	bl Func_02001a64
	adds r0, r6, #0
	movs r1, #0
	bl Func_02001a74
	ldr r3, .L_0200920c
	cmp r5, r3
	bne .L_020091e6
	cmp r6, #5
	bne .L_020091e6
	movs r1, #8
	movs r0, #5
	adds r1, #255
	movs r2, #0
	bl Func_02001a7c
	movs r0, #10
	bl Battle_WaitMode0
.L_020091e6:
	bl Func_02001a1c
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020091f0:
	.4byte 0x0000140a
.L_020091f4:
	.4byte gPartyState
.L_020091f8:
	.4byte 0x00001432
.L_020091fc:
	.4byte 0x0000143a
.L_02009200:
	.4byte 0x00001442
.L_02009204:
	.4byte 0x0000144a
.L_02009208:
	.4byte 0x00001412
.L_0200920c:
	.4byte 0x0000141a
	.section .text.x02009210,"ax",%progbits
	.global Func_02001210
	.thumb_func
Func_02001210:
	push {lr}
	bl Func_0200191c
	movs r0, #2
	bl Sound_LoadPresetParameters
	ldr r0, .L_02009228
	movs r1, #1
	bl Func_02001a84
	pop {pc}
	.2byte 0x0000
.L_02009228:
	.4byte 0x00000001
	.section .text.x0200922c,"ax",%progbits
	.global Func_0200122c
	.thumb_func
Func_0200122c:
	push {r5, r6, lr}
	adds r6, r0, #0
	cmp r6, #14
	bne .L_0200924a
	ldr r3, .L_020092ec
	movs r2, #192
	ldr r3, [r3]
	lsls r2, r2, #2
	ands r3, r2
	cmp r3, #0
	beq .L_0200924a
	movs r0, #14
	bl Func_02000fbc
	b .L_020092e8
.L_0200924a:
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009276
	cmp r6, #13
	beq .L_0200926e
	cmp r6, #14
	beq .L_02009272
	ldr r5, .L_020092f0
	b .L_02009294
.L_0200926e:
	ldr r5, .L_020092f4
	b .L_02009294
.L_02009272:
	ldr r5, .L_020092f8
	b .L_02009294
.L_02009276:
	cmp r6, #13
	beq .L_02009280
	cmp r6, #14
	beq .L_02009284
	b .L_02009288
.L_02009280:
	ldr r5, .L_020092fc
	b .L_0200928a
.L_02009284:
	ldr r5, .L_020092f8
	b .L_0200928a
.L_02009288:
	ldr r5, .L_020092f0
.L_0200928a:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #6
	bl GameFlag_SetBit
.L_02009294:
	adds r0, r5, #0
	bl Func_02001a64
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	movs r0, #1
	bl Battle_WaitMode0
	ldr r3, .L_020092fc
	cmp r5, r3
	bne .L_020092e4
	movs r0, #14
	movs r1, #1
	bl ObjectMotion_SetVariantCallback
	movs r1, #4
	movs r0, #14
	bl Object_SetModeById
	movs r0, #15
	bl Battle_WaitMode0
	ldr r3, .L_02009300
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r1, [r3]
	movs r0, #14
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #5
	bl Battle_WaitMode0
	movs r0, #14
	movs r1, #0
	bl UiText_OpenMessageAtObject
.L_020092e4:
	bl Func_02001a1c
.L_020092e8:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020092ec:
	.4byte gInput
.L_020092f0:
	.4byte 0x00001482
.L_020092f4:
	.4byte 0x0000146a
.L_020092f8:
	.4byte 0x0000146b
.L_020092fc:
	.4byte 0x00001468
.L_02009300:
	.4byte gPartyState
	.section .text.x02009304,"ax",%progbits
	.global Func_02001304
	.thumb_func
Func_02001304:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r3, .L_02009374
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r0, r6, #0
	ldr r1, [r3]
	movs r2, #0
	bl ObjectMotion_SetAngleToward
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009354
	bl Party_CountActiveOwners
	cmp r0, #8
	bne .L_0200933c
	ldr r5, .L_02009378
	b .L_0200934a
.L_0200933c:
	bl Party_CountActiveOwners
	cmp r0, #3
	bgt .L_02009348
	ldr r5, .L_0200937c
	b .L_0200934a
.L_02009348:
	ldr r5, .L_02009380
.L_0200934a:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	b .L_0200935e
.L_02009354:
	movs r0, #129
	lsls r0, r0, #2
	ldr r5, .L_02009384
	bl GameFlag_ClearBit
.L_0200935e:
	adds r0, r5, #0
	bl Func_02001a64
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001a1c
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009374:
	.4byte gPartyState
.L_02009378:
	.4byte 0x00001479
.L_0200937c:
	.4byte 0x00001477
.L_02009380:
	.4byte 0x00001476
.L_02009384:
	.4byte 0x00001478
	.section .text.x02009388,"ax",%progbits
	.global Func_02001388
	.thumb_func
Func_02001388:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	ldr r5, .L_020093d8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r1, [r3]
	movs r2, #0
	adds r0, r6, #0
	bl ObjectMotion_SetAngleToward
	movs r3, #178
	lsls r3, r3, #2
	adds r2, r5, r3
	ldrh r3, [r2]
	cmp r3, #0
	beq .L_020093c4
	adds r0, r3, #0
	movs r1, #5
	bl Func_020019e4
	ldr r0, .L_020093dc
	bl Func_02001a64
	b .L_020093ca
.L_020093c4:
	ldr r0, .L_020093e0
	bl Func_02001a64
.L_020093ca:
	movs r1, #0
	adds r0, r6, #0
	bl UiText_OpenMessageAtObject
	bl Func_02001a1c
	pop {r5, r6, pc}
.L_020093d8:
	.4byte gPartyState
.L_020093dc:
	.4byte 0x00001474
.L_020093e0:
	.4byte 0x00001475
	.section .text.x020093e4,"ax",%progbits
	.global Func_020013e4
	.thumb_func
Func_020013e4:
	ldr r0, .L_020093e8
	bx lr
.L_020093e8:
	.4byte Data_02002150
	.section .text.x020093ec,"ax",%progbits
	.global Func_020013ec
	.thumb_func
Func_020013ec:
	push {lr}
	bl Func_020019ec
	ldr r0, .L_020093fc
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	pop {pc}
.L_020093fc:
	.4byte 0x000013e7
	.section .text.x02009400,"ax",%progbits
	.global Func_02001400
	.thumb_func
Func_02001400:
	push {lr}
	bl Func_020019ec
	ldr r0, .L_02009410
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
	pop {pc}
.L_02009410:
	.4byte 0x000013e9
	.section .text.x02009414,"ax",%progbits
	.global Func_02001414
	.thumb_func
Func_02001414:
	push {r5, r6, lr}
	movs r3, #186
	lsls r3, r3, #2
	adds r5, r0, #0
	adds r3, #255
	sub sp, #8
	cmp r5, r3
	ble .L_02009426
	adds r5, r3, #0
.L_02009426:
	movs r6, #0
.L_02009428:
	adds r0, r5, #0
	movs r1, #10
	bl Engine_MathRemainder
	movs r3, #1
	movs r2, #16
	adds r1, r0, #0
	subs r2, r2, r6
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #27
	movs r3, #8
	bl Func_02001984
	adds r0, r5, #0
	movs r1, #10
	bl Engine_MathDivide
	adds r6, #1
	adds r5, r0, #0
	cmp r6, #2
	ble .L_02009428
	bl Func_0200197c
	add sp, #8
	pop {r5, r6, pc}
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
	bl Func_02000114
	movs r0, #1
	bl WaitFrames
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
	.4byte gPartyState
.L_02009510:
	.4byte gOverlayArea + 0x22bc
.L_02009514:
	.4byte gOverlayArea + 0x22b8
.L_02009518:
	.4byte Data_02003a74
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #5
	bl Func_02000114
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
	bl ObjectMotion_SetAngleToward
	ldr r0, .L_020097f8
	adds r0, r5, r0
	bl Func_02001a64
	movs r1, #0
	movs r0, #8
	bl UiText_OpenMessageAtObject
	movs r0, #4
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
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
	bl UiText_OpenMessageAtObject
	bl Func_02001400
	b .L_02009638
.L_0200962a:
	ldr r0, .L_02009800
	bl Func_02001a64
	movs r0, #8
	movs r1, #0
	bl UiText_OpenMessageAtObject
.L_02009638:
	movs r0, #0
	bl Func_02000114
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #5
	bl Func_02000114
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r6, r1
	ldr r1, [r3]
	movs r0, #8
	movs r2, #0
	bl ObjectMotion_SetAngleToward
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
	bl UiText_OpenMessageAtObject
	bl Func_02001400
	b .L_020096bc
.L_020096ae:
	ldr r0, .L_02009804
	bl Func_02001a64
	movs r0, #8
	movs r1, #0
	bl UiText_OpenMessageAtObject
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
	bl Func_02000114
.L_020096e4:
	bl Func_02001a1c
	b .L_02009876
.L_020096ea:
	cmp r3, #10
	bne .L_02009796
	bl Func_02001a14
	movs r0, #0
	bl Func_02001ab4
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #0
	bl Func_02000114
	movs r0, #4
	bl Func_02000114
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
	bl WaitFrames
	bl Func_02000044
	movs r0, #0
	bl Func_02000114
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #0
	bl Func_02000114
	movs r0, #4
	bl Func_02000114
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
	.4byte gPartyState
.L_020097f8:
	.4byte 0x000013fa
.L_020097fc:
	.4byte 0x000013f8
.L_02009800:
	.4byte 0x000013f5
.L_02009804:
	.4byte 0x000013f6
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
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r6, r1
	ldr r1, [r3]
	movs r2, #0
	movs r0, #8
	bl ObjectMotion_SetAngleToward
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
	bl Func_02000114
	movs r0, #4
	bl Func_02000114
.L_02009876:
	ldr r5, .L_020098c0
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
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
	bl BattlePlacement_UpdateTimedEntriesTwentyTimes
.L_020098b0:
	movs r0, #0
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020098b8:
	.4byte 0x000013e5
.L_020098bc:
	.4byte Data_03001200
.L_020098c0:
	.4byte Func_02000134
.L_020098c4:
	.4byte gPartyState
	.section .text.x020098c8,"ax",%progbits
	.global Func_020018c8
	.thumb_func
Func_020018c8:
	movs r0, #0
	bx lr
	.section .rodata.x02009ae4,"a",%progbits
	.global Data_02001ae4
Data_02001ae4:
	.4byte 0x43314773
	.4byte 0x33323130
	.4byte 0x31434241
	.4byte 0x32454443
	.4byte 0x33474645
	.4byte 0x434d4753
	.global Data_02001afc
Data_02001afc:
	.4byte 0x01010100
	.2byte 0x0001
	.global Data_02001b02
Data_02001b02:
	.2byte 0x02c5
	.4byte 0x004a0044
	.4byte 0x000102d7
	.4byte 0x00030002
	.4byte 0x00050004
	.4byte 0x00070006
	.4byte 0x00090008
	.4byte 0x000b000a
	.4byte 0x000d000c
	.4byte 0x000f000e
	.4byte 0x00110010
	.4byte 0x00140012
	.4byte 0x00160015
	.4byte 0x00180017
	.4byte 0x001a0019
	.4byte 0x001c001b
	.4byte 0x001e001d
	.4byte 0x0022001f
	.4byte 0x00240023
	.4byte 0x00260025
	.4byte 0x00280027
	.4byte 0x002a0029
	.4byte 0x002c002b
	.4byte 0x004c0037
	.4byte 0x00320031
	.4byte 0x00340033
	.4byte 0x00390038
	.4byte 0x00360035
	.4byte 0x003b003a
	.4byte 0x003d003c
	.4byte 0x003f003e
	.4byte 0x00410040
	.4byte 0x00470046
	.4byte 0x00490048
	.4byte 0x004b0043
	.4byte 0x02bd02bc
	.4byte 0x02bf02be
	.4byte 0x02c102c0
	.4byte 0x02c302c2
	.4byte 0x02c602c4
	.4byte 0x02d102d0
	.4byte 0x02d302d2
	.4byte 0x02d502d4
	.4byte 0x02d802d6
	.4byte 0x02da02d9
	.4byte 0x02e602e5
	.4byte 0x02ee02e7
	.4byte 0x02f002ef
	.4byte 0x02f202f1
	.global Data_02001bc4
Data_02001bc4:
	.4byte 0xffff000a
	.4byte 0x00000150
	.4byte 0xc00000a4
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff000b
	.4byte 0x00000150
	.4byte 0xc00000a4
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0008
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x800000c8
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000120
	.4byte 0x00100000
	.4byte 0x01a00000
	.4byte 0x00000136
	.4byte 0xffff0000
	.4byte 0x000000d8
	.4byte 0xc00000d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02001c6c
Data_02001c6c:
	.4byte 0x000001ff
	.global Data_02001c70
Data_02001c70:
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00018000
	.4byte 0xffff0158
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x017c0000
	.4byte 0x00014000
	.4byte 0xffff0056
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00fc0000
	.4byte 0x00012000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x011c0000
	.4byte 0x00010000
	.4byte 0xffff00f2
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x00700000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00018000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x00600000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00010000
	.4byte 0xffff00e8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00010000
	.4byte 0xffff00e7
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00ea
	.4byte 0x00000002
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x0001c000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0001c000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00010000
	.4byte 0xffff00c0
	.4byte 0x00000008
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0x10000000
	.4byte 0x00000001
	.4byte 0x01440000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00016000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0001e000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0001c000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x10060006
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x10070007
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x01004000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00cc0000
	.4byte 0x00014000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01300000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00010000
	.4byte 0xffff00c0
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00010000
	.4byte 0x00000158
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0xffff0056
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x005c0000
	.4byte 0x00014000
	.4byte 0xffff0078
	.4byte 0x00000001
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00740000
	.4byte 0x00010000
	.4byte 0xffff00f2
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff00bf
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0xffff00ba
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00014000
	.4byte 0x000100e8
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x000200e7
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x0001c000
	.4byte 0x000300ea
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0xffff00c4
	.4byte 0x00000001
	.4byte 0x00400000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0001c000
	.4byte 0xffff00bc
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x00014000
	.4byte 0xffff00df
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00014000
	.4byte 0xffff00bb
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00010000
	.4byte 0xffff00c0
	.4byte 0x00000008
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00010000
	.4byte 0x10000000
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00010000
	.4byte 0x10010001
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x006c0000
	.4byte 0x00014000
	.4byte 0x10020002
	.4byte 0x00000001
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x0001c000
	.4byte 0x10030003
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00018000
	.4byte 0x10060006
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00018000
	.4byte 0x10070007
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00900000
	.4byte 0x00010000
	.4byte 0x10050005
	.4byte 0x00000001
	.4byte 0x00800000
	.4byte 0x00000000
	.4byte 0x007c0000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02002150
Data_02002150:
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte Func_02001130
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte Func_02001130
	.4byte 0x00000000
	.4byte 0xffff0002
	.4byte Func_02001130
	.4byte 0x00000000
	.4byte 0xffff0003
	.4byte Func_02001130
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte Func_02001130
	.4byte 0x00000000
	.4byte 0xffff0006
	.4byte Func_02001130
	.4byte 0x00000000
	.4byte 0xffff0007
	.4byte Func_02001130
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000bb0
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000da0
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000f50
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000013eb
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000146c
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_0200122c
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_0200122c
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte Func_02000e4c
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte Func_02001304
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x0000147a
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x0000147c
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x0000147d
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x0000147e
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0000147b
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02001388
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x00001482
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte Func_02000fbc
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001453
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte Func_02000864
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte Func_02001210
	.4byte 0x00000006
	.4byte 0xffff0001
	.4byte Func_020002d8
	.4byte 0x00000006
	.4byte 0xffff0002
	.4byte Func_02000344
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
