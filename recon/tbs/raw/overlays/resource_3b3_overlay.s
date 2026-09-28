.syntax unified
	.thumb
	.section .text.x020080a0,"ax",%progbits
	.balign 4
	.global OverlayObject_CreateConfiguredObject
	.thumb_func
OverlayObject_CreateConfiguredObject:
	push {r5, r6, lr}
	adds r4, r0, #0
	adds r5, r1, #0
	adds r6, r2, #0
	adds r0, r3, #0
	adds r2, r5, #0
	adds r1, r4, #0
	adds r3, r6, #0
	bl 0x0200aa74
	adds r5, r0, #0
	cmp r5, #0
	beq .L_020000a0_0
	ldr r1, [r5, #80]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r2, r5, #0
	strb r3, [r1, #9]
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r2, #4
	movs r3, #8
	strb r3, [r2]
	movs r1, #0
	bl 0x0200aad4
	adds r0, r5, #0
	movs r1, #15
	bl 0x0200ab6c
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #2
	orrs r3, r2
	strb r3, [r1]
	adds r0, r5, #0
	b .L_020000a0_1
.L_020000a0_0:
	movs r0, #0
.L_020000a0_1:
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.section .text.x0200813c,"ax",%progbits
	.align 2
	.global Effect_Spawn
	.thumb_func
Effect_Spawn:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #8
	adds r6, r1, #0
	ldr r1, [sp, #48]
	adds r5, r0, #0
	movs r0, #0
	mov r8, r2
	str r3, [sp, #4]
	mov r10, r1
	ldr r7, [sp, #52]
	bl 0x0200ab1c
	movs r3, #128
	lsls r3, r3, #13
	mov r2, r10
	ands r3, r2
	mov r9, r0
	cmp r3, #0
	beq .L_0200013c_0
	cmp r7, #0
	beq .L_0200013c_0
	movs r3, #24
	ldrsh r0, [r7, r3]
	adds r2, r6, #0
	b .L_0200013c_1
.L_0200013c_0:
	adds r2, r6, #0
	movs r0, #222
.L_0200013c_1:
	adds r1, r5, #0
	mov r3, r8
	bl 0x0200aa74
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200013c_2
	b .L_0200013c_3
.L_0200013c_2:
	ldr r1, [r6, #80]
	mov r8, r1
	mov r1, r10
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	bl 0x0200aa64
	mov r3, r10
	ldr r2, [pc, #356]
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r11, r3
	bl 0x0200aa6c
	adds r3, r6, #0
	movs r0, #0
	adds r3, #85
	strb r0, [r3]
	mov r3, r8
	adds r3, #38
	strb r0, [r3]
	ldr r3, [pc, #328]
	str r3, [r6, #108]
	ldr r3, [sp, #4]
	str r3, [r6, #68]
	ldr r3, [sp, #40]
	str r3, [r6, #72]
	ldr r3, [sp, #44]
	mov r1, r9
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	mov r3, r8
	ldrb r1, [r3, #9]
	movs r3, #13
	negs r3, r3
	mov r9, r3
	ands r3, r1
	orrs r3, r2
	adds r2, r6, #0
	mov r1, r8
	adds r2, #100
	strb r3, [r1, #9]
	adds r3, r2, #0
	str r0, [r6, #48]
	str r0, [r6, #52]
	str r2, [sp, #0]
	strh r0, [r3]
	ldr r3, [pc, #276]
	mov r1, r10
	ands r3, r1
	movs r5, #3
	cmp r3, #0
	beq .L_0200013c_3
	cmp r7, #0
	beq .L_0200013c_3
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_4
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl 0x0200ab6c
.L_0200013c_4:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_5
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	mov r3, r8
	ldrb r2, [r7]
	ldrb r1, [r3, #9]
	ands r2, r5
	mov r3, r9
	ands r3, r1
	lsls r2, r2, #2
	orrs r3, r2
	mov r1, r8
	strb r3, [r1, #9]
.L_0200013c_5:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r10
	ands r2, r3
	cmp r2, #0
	beq .L_0200013c_6
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200013c_6:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_7
	ldr r3, [pc, #156]
	mov r1, r11
	ldr r5, [r3, r1]
	cmp r2, #0
	beq .L_0200013c_8
	ldr r0, [r7, #16]
	ldr r3, [r6, #24]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	bl 0x0200aa2c
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200013c_9
.L_0200013c_8:
	ldr r0, [r7, #16]
	ldr r2, [pc, #128]
	ldr r1, [r5, #12]
	adds r0, r0, r2
	bl 0x0200aa2c
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [pc, #116]
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200013c_9:
	bl 0x0200aa2c
	str r0, [r6, #52]
.L_0200013c_7:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_0200013c_10
	adds r0, r6, #0
	movs r1, #1
	bl 0x0200aa64
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl 0x0200aa6c
.L_0200013c_10:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_11
	ldrh r3, [r7, #32]
	mov r1, r8
	strh r3, [r1, #30]
.L_0200013c_11:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_12
	ldrh r3, [r7, #34]
	ldr r1, [sp, #0]
	strh r3, [r1]
.L_0200013c_12:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r10
	ands r3, r2
	cmp r3, #0
	beq .L_0200013c_3
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200013c_3:
	sub sp, #-8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x0200ad58
	.4byte 0x02008105
	.4byte 0xffff0000
	.section .text.x02008cc0,"ax",%progbits
	.align 2
	.global TakaraHashira_CopyCellBlock
	.thumb_func
TakaraHashira_CopyCellBlock:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	lsls	r1, r1, #7
	ldr	r4, [sp, #48]
	mov	sl, r2
	adds	r1, r1, r0
	ldr	r2, [pc, #132]
	lsls	r1, r1, #2
	adds	r3, r4, r3
	adds	r5, r1, r2
	cmp	r4, r3
	bge.n	.L_02000d4e
	str	r3, [sp, #4]
	mov	r6, sl
	movs	r3, #128
	subs	r3, r3, r6
	lsls	r3, r3, #2
	mov	fp, r3
	ldr	r3, [sp, #40]
	lsls	r3, r3, #4
	mov	r9, r3
.L_02000cf6:
	ldr	r0, [sp, #44]
	mov	r1, sl
	adds	r2, r0, r1
	cmp	r0, r2
	bge.n	.L_02000d44
	ldr	r3, [pc, #96]
	movs	r7, #15
	mov	r8, r3
	adds	r3, r4, #0
	ands	r3, r7
	add	r3, r9
	lsls	r3, r3, #5
	ldr	r6, [pc, #88]
	str	r3, [sp, #0]
	mov	lr, r6
	mov	ip, r2
.L_02000d16:
	ldr	r6, [sp, #0]
	ldmia	r5!, {r1}
	adds	r3, r0, #0
	mov	r2, r8
	ands	r3, r7
	ands	r1, r2
	adds	r3, r6, r3
	ldr	r6, [pc, #68]
	lsls	r1, r1, #3
	adds	r2, r1, r6
	ldr	r2, [r2, #0]
	lsls	r3, r3, #2
	mov	r6, lr
	str	r2, [r3, r6]
	ldr	r6, [pc, #60]
	adds	r2, r1, r6
	ldr	r1, [pc, #60]
	ldr	r2, [r2, #0]
	adds	r3, r3, r1
	adds	r0, #1
	str	r2, [r3, #0]
	cmp	r0, ip
	blt.n	.L_02000d16
.L_02000d44:
	ldr	r2, [sp, #4]
	adds	r4, #1
	add	r5, fp
	cmp	r4, r2
	blt.n	.L_02000cf6
.L_02000d4e:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02010000
	.4byte 0x00000fff
	.4byte 0x06002800
	.4byte 0x02020000
	.4byte 0x02020004
	.2byte 0x2840
	.2byte 0x0600
	.section .text.x02009440,"ax",%progbits
	.balign 4
	.global Scene_GetEntrances
	.thumb_func
Scene_GetEntrances:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02001440_0
	ldr r0, [pc, #56]
	b .L_02001440_1
.L_02001440_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02001440_2
	ldr r0, [pc, #56]
	b .L_02001440_1
.L_02001440_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02001440_3
	ldr r0, [pc, #52]
	b .L_02001440_1
.L_02001440_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02001440_4
	ldr r0, [pc, #52]
	b .L_02001440_1
.L_02001440_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02001440_5
	ldr r0, [pc, #48]
	b .L_02001440_1
.L_02001440_5:
	ldr r0, [pc, #48]
.L_02001440_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000073
	.4byte 0x0200aeac
	.4byte 0x00000074
	.4byte 0x0200aef4
	.4byte 0x00000077
	.4byte 0x0200af3c
	.4byte 0x00000079
	.4byte 0x0200af84
	.4byte 0x0000007a
	.4byte 0x0200afcc
	.4byte 0x0200ae7c
	.section .text.x020094c4,"ax",%progbits
	.balign 4
	.global Scene_GetPlacements
	.thumb_func
Scene_GetPlacements:
	push {lr}
	ldr r3, [pc, #56]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #48]
.L_020014d4:
	cmp r2, r3
	bne .L_020014d4_0
	ldr r0, [pc, #44]
	b .L_020014d4_1
.L_020014d4_0:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020014d4_2
	ldr r0, [pc, #44]
	b .L_020014d4_1
.L_020014d4_2:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_020014d4_3
	ldr r0, [pc, #40]
	b .L_020014d4_1
.L_020014d4_3:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020014d4_4
	ldr r0, [pc, #40]
	b .L_020014d4_1
.L_020014d4_4:
	ldr r0, [pc, #40]
.L_020014d4_1:
	pop {r1}
	bx r1
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0073
	.2byte 0x0000
	.4byte 0x0200b06c
	.4byte 0x00000074
	.4byte 0x0200b0e4
	.4byte 0x00000077
	.4byte 0x0200b174
	.4byte 0x0000007a
	.4byte 0x0200b2dc
	.4byte 0x0200b264
	.section .text.x02009d84,"ax",%progbits
	.align 2
	.global TakaraHashira_UpdatePillarActors
	.thumb_func
TakaraHashira_UpdatePillarActors:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #556]
	sub	sp, #20
	movs	r0, #8
	movs	r2, #16
	mov	r8, r1
	add	r2, sp
	movs	r3, #0
	str	r0, [sp, #12]
	str	r0, [sp, #4]
	mov	r9, r2
	mov	sl, r3
	mov	fp, r8
.L_02001daa:
	ldr	r0, [sp, #12]
	bl 0x0200ab1c
	adds	r6, r0, #0
	adds	r2, r6, #0
	adds	r2, #34
	movs	r3, #2
	strb	r3, [r2, #0]
	ldr	r0, [sp, #12]
	subs	r0, #8
	str	r0, [sp, #8]
	mov	r1, sl
	ldr	r3, [r6, #8]
	mov	r0, r8
	ldr	r2, [r1, r0]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02001de2
	ldr	r1, [sp, #4]
	ldr	r3, [r6, #16]
	ldr	r2, [r1, r0]
	asrs	r3, r3, #20
	cmp	r3, r2
	bne.n	.L_02001de2
	ldr	r3, [r6, #40]
	cmp	r3, #0
	bne.n	.L_02001de2
	b.n	.L_02001f90
.L_02001de2:
	adds	r0, r6, #0
	ldr	r3, [pc, #476]
	adds	r0, #8
	ldr	r1, [pc, #476]
	ldr	r2, [pc, #480]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #128
	ldr	r1, [pc, #464]
	lsls	r2, r2, #24
.L_02001df6:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_02001df6
	adds	r0, r6, #0
	ldr	r1, [pc, #452]
	bl 0x0200aacc
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	bne.n	.L_02001e18
	adds	r7, r6, #0
	adds	r7, #85
	movs	r3, #3
	strb	r3, [r7, #0]
	b.n	.L_02001e1c
.L_02001e18:
	adds	r7, r6, #0
	adds	r7, #85
.L_02001e1c:
	mov	r0, r8
	mov	r3, sl
	mov	r5, r8
	ldr	r1, [r3, r0]
	adds	r5, #12
	ldr	r3, [sp, #4]
	add	r5, sl
	ldr	r2, [r3, r0]
	movs	r0, #0
	adds	r3, r5, #0
	bl 0x0200901c
	mov	r0, fp
	ldr	r3, [sp, #4]
	ldr	r1, [r0, #0]
	mov	r0, r8
	ldr	r2, [r3, r0]
	adds	r3, r5, #0
	movs	r0, #2
	bl 0x0200901c
	ldrb	r2, [r7, #0]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02001ed0
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #2
.L_02001e56:
	bl 0x0200aab4
	cmp	r0, #50
	bne.n	.L_02001e82
	movs	r0, #189
	bl 0x0200abb4
	adds	r5, r6, #0
	adds	r5, #35
	ldrb	r3, [r5, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r5, #0]
	movs	r1, #1
	ldr	r0, [sp, #12]
	bl 0x02009074
	ldrb	r3, [r5, #0]
	movs	r1, #1
	orrs	r3, r1
	strb	r3, [r5, #0]
	b.n	.L_02001ecc
.L_02001e82:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	movs	r0, #2
	bl 0x0200aab4
	cmp	r0, #51
	bne.n	.L_02001ec6
	movs	r1, #0
	adds	r0, r6, #0
	bl 0x02008da8
	movs	r0, #189
	bl 0x0200abb4
	movs	r2, #0
	str	r2, [r6, #12]
	adds	r5, r6, #0
	adds	r5, #35
	ldrb	r3, [r5, #0]
	movs	r2, #254
	ands	r2, r3
	strb	r2, [r5, #0]
	ldr	r0, [sp, #12]
	bl 0x02009174
	movs	r3, #0
	str	r3, [r6, #8]
	str	r3, [r6, #12]
	str	r3, [r6, #16]
	ldrb	r3, [r5, #0]
	movs	r0, #1
	orrs	r3, r0
	strb	r3, [r5, #0]
	b.n	.L_02001ecc
.L_02001ec6:
	adds	r0, r6, #0
	bl 0x02008d78
.L_02001ecc:
	movs	r1, #0
	strb	r1, [r7, #0]
.L_02001ed0:
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	ldr	r3, [pc, #248]
	asrs	r2, r2, #20
	asrs	r1, r1, #20
	add	r3, sl
	movs	r0, #0
	bl 0x02008fcc
	ldr	r2, [r6, #12]
	cmp	r2, #0
	blt.n	.L_02001f2a
	asrs	r2, r2, #20
.L_02001eea:
	adds	r2, #6
	movs	r0, #0
	movs	r1, #27
	mov	r3, r9
	bl 0x02008fcc
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #0
	mov	r3, r9
	bl 0x0200901c
	mov	r3, fp
	ldrb	r2, [r3, #13]
	mov	r0, r9
	ldrb	r1, [r0, #1]
	lsrs	r2, r2, #6
	movs	r3, #63
	lsls	r2, r2, #6
	ands	r3, r1
	orrs	r3, r2
	ldr	r1, [r6, #8]
	ldr	r2, [r6, #16]
	strb	r3, [r0, #1]
	asrs	r1, r1, #20
	asrs	r2, r2, #20
	movs	r0, #2
	mov	r3, r9
	bl 0x0200901c
.L_02001f2a:
	ldr	r3, [r6, #8]
	mov	r1, r8
	asrs	r3, r3, #20
	mov	r2, sl
	str	r3, [r1, r2]
	ldr	r3, [r6, #12]
	adds	r2, #4
	asrs	r3, r3, #20
	str	r3, [r1, r2]
	ldr	r3, [r6, #16]
	adds	r2, #4
	asrs	r3, r3, #20
	str	r3, [r1, r2]
	movs	r5, #0
	movs	r7, #16
.L_02001f48:
	ldr	r3, [sp, #8]
	cmp	r5, r3
	beq.n	.L_02001f88
	ldr	r1, [pc, #112]
	ldr	r0, [r1, r7]
	str	r1, [sp, #0]
	bl 0x0200aafc
	adds	r0, r5, #0
	adds	r0, #8
	bl 0x0200ab1c
	ldr	r2, [r6, #8]
	ldr	r3, [r0, #8]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	ldr	r1, [sp, #0]
	cmp	r2, r3
	bne.n	.L_02001f88
	ldr	r2, [r6, #16]
	ldr	r3, [r0, #16]
	asrs	r2, r2, #20
	asrs	r3, r3, #20
	cmp	r2, r3
	bne.n	.L_02001f88
	ldr	r2, [r6, #12]
	ldr	r3, [r0, #12]
	cmp	r2, r3
	ble.n	.L_02001f88
	ldr	r0, [r1, r7]
	bl 0x0200aaf4
.L_02001f88:
	adds	r5, #1
	adds	r7, #20
	cmp	r5, #3
	bls.n	.L_02001f48
.L_02001f90:
	ldr	r1, [sp, #4]
	ldr	r2, [sp, #12]
	movs	r0, #20
	adds	r1, #20
	adds	r2, #1
	add	sl, r0
	add	fp, r0
	str	r1, [sp, #4]
	str	r2, [sp, #12]
	cmp	r2, #11
	bhi.n	.L_02001fa8
	b.n	.L_02001daa
.L_02001fa8:
	bl 0x02009be8
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0x0200b6d0
	.4byte 0x040000d4
	.4byte 0x0200b720
	.4byte 0x84000003
	.2byte 0xb6dc
	.2byte 0x0200
	.section .text.x0200a580,"ax",%progbits
	.align 2
	.global Scene_GetEvents
	.thumb_func
Scene_GetEvents:
	push {lr}
	ldr r3, [pc, #68]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_02002580_0
	ldr r0, [pc, #56]
	b .L_02002580_1
.L_02002580_0:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02002580_2
	ldr r0, [pc, #56]
	b .L_02002580_1
.L_02002580_2:
	ldr r3, [pc, #56]
	cmp r2, r3
	bne .L_02002580_3
	ldr r0, [pc, #52]
	b .L_02002580_1
.L_02002580_3:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02002580_4
	ldr r0, [pc, #52]
	b .L_02002580_1
.L_02002580_4:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_02002580_5
	ldr r0, [pc, #48]
	b .L_02002580_1
.L_02002580_5:
	ldr r0, [pc, #48]
.L_02002580_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000073
	.4byte 0x0200b3a8
	.4byte 0x00000074
	.4byte 0x0200b438
	.4byte 0x00000077
	.4byte 0x0200b498
	.4byte 0x00000079
	.4byte 0x0200b51c
	.4byte 0x0000007a
	.4byte 0x0200b618
	.4byte 0x0200b39c
	.section .text.x0200a74c,"ax",%progbits
	.balign 4
	.global TakaraHashira_SetupArea
	.thumb_func
TakaraHashira_SetupArea:
	push {r5, lr}
	ldr r3, [pc, #692]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r3, r3, #2
	lsls r2, r2, #1
	str r3, [r1, r2]
	ldr r1, [pc, #680]
	ldrsh r2, [r1, r2]
	ldr r3, [pc, #680]
	sub sp, #8
	cmp r2, r3
	bne .L_0200274c_0
	movs r0, #8
	bl 0x02008ba4
	movs r0, #9
	bl 0x02008ba4
	movs r0, #10
	bl 0x02008ba4
	movs r0, #11
	bl 0x02008ba4
	movs r0, #12
	bl 0x02008ba4
	b .L_0200274c_1
.L_0200274c_0:
	ldr r3, [pc, #644]
	cmp r2, r3
	bne .L_0200274c_2
	movs r3, #0
	movs r2, #64
	str r3, [sp, #0]
	movs r1, #0
	movs r3, #32
	movs r0, #32
	str r2, [sp, #4]
	bl 0x0200aabc
	movs r0, #8
	bl 0x02008ba4
	movs r0, #9
	bl 0x02008ba4
	movs r0, #10
	bl 0x02008ba4
	movs r0, #11
	bl 0x02008ba4
	movs r0, #12
	bl 0x02008ba4
	movs r0, #13
	bl 0x02008ba4
	movs r0, #14
	bl 0x02008ba4
	movs r0, #15
	bl 0x02008ba4
	ldr r0, [pc, #576]
	bl 0x0200aaec
	cmp r0, #0
	bne .L_0200274c_3
	b .L_0200274c_1
.L_0200274c_3:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200aaec
	cmp r0, #0
	bne .L_0200274c_4
	b .L_0200274c_1
.L_0200274c_4:
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #79
	movs r1, #34
	movs r2, #84
	movs r3, #24
	bl 0x0200aaa4
	movs r5, #32
	movs r0, #0
	movs r1, #32
	movs r2, #32
	movs r3, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200aaa4
	movs r1, #32
	movs r2, #64
	movs r3, #0
	movs r0, #32
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200aaa4
	movs r0, #9
	bl 0x02008ec8
	movs r0, #10
	bl 0x02008ec8
	movs r0, #11
	bl 0x02008ec8
	movs r0, #12
	bl 0x02008ec8
	movs r0, #13
	bl 0x02008ec8
	movs r0, #14
	bl 0x02008ec8
	movs r0, #15
	bl 0x02008ec8
	movs r3, #24
	movs r2, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #24
	movs r1, #3
	b .L_0200274c_5
.L_0200274c_2:
	ldr r3, [pc, #444]
	cmp r2, r3
	bne .L_0200274c_6
	movs r0, #146
	movs r2, #200
	lsls r0, r0, #18
	lsls r2, r2, #16
	movs r1, #0
	movs r3, #223
	bl 0x020080a0
	ldr r0, [pc, #420]
	bl 0x0200aaec
	cmp r0, #0
	bne .L_0200274c_8
	movs r0, #0
	bl 0x0200ab1c
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
.L_0200274c_8:
	movs r0, #0
	bl 0x0200aba4
	movs r0, #0
	bl 0x0200ab1c
	adds r0, #98
	ldrb r3, [r0]
	cmp r3, #0
	bne .L_0200274c_9
	bl 0x0200a09c
.L_0200274c_9:
	movs r0, #8
	bl 0x0200a694
	movs r0, #9
	bl 0x0200a694
	movs r0, #10
	bl 0x0200a694
	movs r0, #11
	bl 0x0200a694
	movs r4, #128
	ldr r2, [pc, #356]
	movs r1, #0
	movs r0, #0
	lsls r4, r4, #2
.L_0200274c_10:
	adds r3, r1, r4
	adds r1, #1
	str r0, [r2]
	str r0, [r2, #4]
	str r0, [r2, #8]
	str r3, [r2, #16]
	adds r2, #20
	cmp r1, #3
	bls .L_0200274c_10
	bl 0x02009d84
	movs r0, #1
	bl 0x0200ab04
	movs r1, #200
	ldr r0, [pc, #320]
	lsls r1, r1, #4
	bl 0x0200aa3c
	ldr r0, [pc, #300]
	bl 0x0200aaec
	cmp r0, #0
	bne .L_0200274c_11
	b .L_0200274c_1
.L_0200274c_11:
	movs r5, #8
	b .L_0200274c_12
.L_0200274c_13:
	adds r5, #1
.L_0200274c_12:
	cmp r5, #11
	bhi .L_0200274c_1
	adds r0, r5, #0
	bl 0x0200ab1c
	ldr r3, [r0, #8]
	asrs r2, r3, #20
	cmp r2, #37
	bne .L_0200274c_13
	ldr r3, [r0, #16]
	asrs r0, r3, #20
.L_0200274c_7:
	cmp r0, #9
	bne .L_0200274c_13
	str r2, [sp, #0]
	str r0, [sp, #4]
	movs r1, #8
	movs r0, #27
.L_0200274c_5:
	movs r2, #1
	movs r3, #1
	bl 0x0200aac4
	b .L_0200274c_1
.L_0200274c_6:
	movs r2, #224
	lsls r2, r2, #1
	adds r3, r1, r2
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #244]
	cmp r2, r3
	bne .L_0200274c_1
	movs r0, #10
	movs r1, #2
	bl 0x0200ab54
	movs r1, #6
	movs r0, #10
	bl 0x0200ab64
	movs r0, #8
	bl 0x02008ba4
	movs r0, #9
	bl 0x02008ba4
	movs r0, #8
	bl 0x0200ab1c
	movs r5, #0
	adds r0, #85
	strb r5, [r0]
	movs r0, #9
	bl 0x0200ab1c
	adds r0, #85
	strb r5, [r0]
	bl 0x0200a4cc
	movs r0, #11
	bl 0x0200a5f8
	movs r0, #12
	bl 0x0200a5f8
	movs r0, #13
	bl 0x0200a5f8
	movs r0, #11
	bl 0x0200a63c
	movs r0, #12
	bl 0x0200a63c
	movs r0, #13
	bl 0x0200a63c
	movs r0, #13
	bl 0x0200ab1c
	str r5, [r0, #108]
	movs r0, #14
	bl 0x0200a5f8
	movs r0, #14
	bl 0x0200ab1c
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #8
	orrs r3, r2
	strb r3, [r0]
	ldr r0, [pc, #124]
	bl 0x0200aaec
	cmp r0, #0
	bne .L_0200274c_1
	movs r0, #13
	bl 0x0200ab1c
	movs r5, #192
	lsls r5, r5, #9
	str r5, [r0, #24]
	movs r0, #13
	bl 0x0200ab1c
	str r5, [r0, #28]
	movs r0, #13
	bl 0x0200ab1c
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	movs r5, #12
	orrs r3, r5
	strb r3, [r2, #9]
	movs r0, #14
	bl 0x0200ab1c
	ldr r2, [r0, #80]
	ldrb r3, [r2, #9]
	orrs r3, r5
	strb r3, [r2, #9]
	movs r3, #22
	movs r2, #16
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #26
	movs r1, #12
	movs r2, #1
	movs r3, #1
	bl 0x0200aac4
.L_0200274c_1:
	movs r0, #0
	sub sp, #-8
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000074
	.4byte 0x00000077
	.4byte 0x00000109
	.4byte 0x00000079
	.4byte 0x0200b6d0
	.4byte 0x0200a6b9
	.4byte 0x0000007a
	.4byte 0x00000202
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
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
	.4byte 0x0000001b
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0x0200abf8
	.4byte 0x0200ac30
	.4byte 0x0200ac68
	.global TakaraHashira_ActionTable
TakaraHashira_ActionTable:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000002
	.4byte 0x00000022
	.4byte 0x02009429
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte 0x02009429
	.4byte 0x00000000
	.4byte 0x00000006
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global TakaraHashira_ShiftSteps1
TakaraHashira_ShiftSteps1:
	.4byte 0x0022004c
	.4byte 0x00020001
	.4byte 0x004d0004
	.4byte 0x00010022
	.4byte 0x00040002
	.4byte 0x0022004e
	.4byte 0x00020001
	.4byte 0x004f0004
	.4byte 0x00010022
	.4byte 0x00040002
	.2byte 0xffff
	.global TakaraHashira_ShiftSteps2
TakaraHashira_ShiftSteps2:
	.2byte 0x004f
	.4byte 0x00010022
	.4byte 0x00040002
	.4byte 0x0022004e
	.4byte 0x00020001
	.4byte 0x004d0004
	.4byte 0x00010022
	.4byte 0x00040002
	.4byte 0x0022004c
	.4byte 0x00020001
	.4byte 0xffff0004
	.global TakaraHashira_ShiftSteps3
TakaraHashira_ShiftSteps3:
	.4byte 0x00220043
	.4byte 0x00050002
	.4byte 0x00450006
	.4byte 0x00020022
	.4byte 0x00060005
	.4byte 0x00220047
	.4byte 0x00050002
	.4byte 0x00490006
	.4byte 0x00020022
	.4byte 0x00060005
	.2byte 0xffff
	.global TakaraHashira_ShiftSteps4
TakaraHashira_ShiftSteps4:
	.2byte 0x0047
	.4byte 0x00020032
	.4byte 0x00060005
	.4byte 0x00320045
	.4byte 0x00050002
	.4byte 0x00430006
	.4byte 0x00020032
	.4byte 0x00060005
	.4byte 0x00320041
	.4byte 0x00050002
	.4byte 0xffff0006
	.global TakaraHashira_ShiftSteps5
TakaraHashira_ShiftSteps5:
	.4byte 0x0026004b
	.4byte 0x00010002
	.4byte 0x004d0004
	.4byte 0x00020026
	.4byte 0x00060001
	.4byte 0x0026004f
	.4byte 0x00010002
	.4byte 0x00410008
	.4byte 0x00020035
	.4byte 0x00010001
	.4byte 0x0000ffff
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000002c8
	.4byte 0xc00002f8
	.4byte 0x02300000
	.4byte 0x03e00170
	.4byte 0x00000318
	.4byte 0xffff0002
	.4byte 0x00000258
	.4byte 0xc00002f8
	.4byte 0x02300000
	.4byte 0x03e00170
	.4byte 0x00000318
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0xc0000168
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000068
	.4byte 0xc0000168
	.4byte 0x00200000
	.4byte 0x01800020
	.4byte 0x00000190
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000b8
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000040
	.4byte 0xc00001d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000002e8
	.4byte 0xc0000208
	.4byte 0x01b80000
	.4byte 0x03c00010
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x00000278
	.4byte 0xc0000208
	.4byte 0x01b80000
	.4byte 0x03c00010
	.4byte 0x00000220
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000118
	.4byte 0xc00001e8
	.4byte 0x00100000
	.4byte 0x01d00020
	.4byte 0x00000210
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0xc00001e8
	.4byte 0x00100000
	.4byte 0x01d00020
	.4byte 0x00000210
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gTakaraHashiraExits
gTakaraHashiraExits:
	.4byte 0x00000073
	.4byte 0x0010307e
	.4byte 0x0020407e
	.4byte 0x00000074
	.4byte 0x0010307f
	.4byte 0x0020407f
	.4byte 0x00000077
	.4byte 0x00103082
	.4byte 0x00204082
	.4byte 0x00000079
	.4byte 0x00103084
	.4byte 0x00204084
	.4byte 0x0000007a
	.4byte 0x00103085
	.4byte 0x00204085
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff012a
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00024000
	.4byte 0xffff0129
	.4byte 0x00000007
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00f00000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00900000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01000000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00b00000
	.4byte 0x00024000
	.4byte 0xffff00cd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01100000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x01600000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff00d6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x01880000
	.4byte 0x0002c000
	.4byte 0xffff0100
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00d00000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff00cf
	.4byte 0x00000007
	.4byte 0x00e00000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00024000
	.4byte 0xffff0125
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02009529
	.4byte 0x00000013
	.4byte 0x0ec20064
	.4byte 0x001000bb
	.4byte 0x00000013
	.4byte 0x0ec30065
	.4byte 0x001000b5
	.4byte 0x00000013
	.4byte 0x0ec40066
	.4byte 0x0020006f
	.4byte 0x00000013
	.4byte 0x0ec50067
	.4byte 0x001000c2
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02009545
	.4byte 0x00000013
	.4byte 0x0ec60064
	.4byte 0x001000c4
	.4byte 0x00000013
	.4byte 0x0ec70065
	.4byte 0x00100018
	.4byte 0x00000013
	.4byte 0x0ec80066
	.4byte 0x002000de
	.4byte 0x00000013
	.4byte 0x0ec90067
	.4byte 0x001000bc
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x02009545
	.4byte 0x00000413
	.4byte 0x0ed20064
	.4byte 0x0020022b
	.4byte 0x0000c413
	.4byte 0x0ed20064
	.4byte 0x0020022b
	.4byte 0x00000413
	.4byte 0x0ed30065
	.4byte 0x001000e5
	.4byte 0x0000e413
	.4byte 0x0ed30065
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0ed40066
	.4byte 0x001000b7
	.4byte 0x00000013
	.4byte 0x0ed50067
	.4byte 0x00100062
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte 0x0200975d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte 0x02009fd5
	.4byte 0x0000c602
	.4byte 0xffff001f
	.4byte 0x0200a035
	.4byte 0x00004602
	.4byte 0xffff001f
	.4byte 0x0200a041
	.4byte 0x00000202
	.4byte 0xffff001f
	.4byte 0x02009fd5
	.4byte 0x00000002
	.4byte 0xffff0028
	.4byte 0x0200a081
	.4byte 0x00000002
	.4byte 0xffff0029
	.4byte 0x0200a09d
	.4byte 0x00000013
	.4byte 0x0eda0064
	.4byte 0x00200309
	.4byte 0x00000013
	.4byte 0x0edb0065
	.4byte 0x001000e5
	.4byte 0x00000413
	.4byte 0x0edc0066
	.4byte 0x001000ba
	.4byte 0x00000013
	.4byte 0x0edd0067
	.4byte 0x00100032
	.4byte 0x00008c15
	.4byte 0x02000008
	.4byte 0x02009d85
	.4byte 0x00008c15
	.4byte 0x02010009
	.4byte 0x02009d85
	.4byte 0x00008c15
	.4byte 0x0202000a
	.4byte 0x02009d85
	.4byte 0x00008c15
	.4byte 0x0203000b
	.4byte 0x02009d85
	.4byte 0x00009315
	.4byte 0x02000008
	.4byte 0x02009d85
	.4byte 0x00009315
	.4byte 0x02010009
	.4byte 0x02009d85
	.4byte 0x00009315
	.4byte 0x0202000a
	.4byte 0x02009d85
	.4byte 0x00009315
	.4byte 0x0203000b
	.4byte 0x02009d85
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x0200a53d
	.4byte 0x00000013
	.4byte 0x0ede0064
	.4byte 0x00200378
	.4byte 0x00000013
	.4byte 0x0edf0065
	.4byte 0x001000b7
	.4byte 0x00000013
	.4byte 0x0ee00066
	.4byte 0x0010010c
	.4byte 0x00000013
	.4byte 0x0ee10067
	.4byte 0x001000e2
	.4byte 0x10008c15
	.4byte 0x0204000a
	.4byte 0x0200a499
	.4byte 0x00008c15
	.4byte 0x0204000a
	.4byte 0x0200a4cd
	.4byte 0x00001815
	.4byte 0x0200000b
	.4byte 0x0200a145
	.4byte 0x00001815
	.4byte 0x0201000c
	.4byte 0x0200a151
	.4byte 0x10001815
	.4byte 0x0202000d
	.4byte 0x0200a15d
	.4byte 0x50001815
	.4byte 0x0202000d
	.4byte 0x0200a1ad
	.4byte 0x00001815
	.4byte 0x0202000d
	.4byte 0x0200a385
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 4
	.global TakaraHashira_PillarSlots
TakaraHashira_PillarSlots:
	.space 80
	.space 12
	.global TakaraHashira_ShakenScroll
TakaraHashira_ShakenScroll:
	.space 12
	.global TakaraHashira_ShakeChance
TakaraHashira_ShakeChance:
	.space 4
