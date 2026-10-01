.syntax unified
	.thumb
	.global UiWork_AnimateSpriteSlots
	.thumb_func
UiWork_AnimateSpriteSlots:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_08019270
	ldr r3, [r3]
	movs r2, #160
	mov r9, r3
	lsls r2, r2, #3
	add r2, r9
	movs r3, #0
	sub sp, #24
	mov r10, r2
	mov r11, r3
.L_080191ee:
	mov r3, r10
	ldrh r2, [r3, #22]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_080191fc
	b .L_08019618
.L_080191fc:
	ldr r3, .L_08019274
	mov r2, r10
	ldr r3, [r3]
	ldr r6, [r2]
	b .L_0801960c
.L_08019206:
	mov r2, r10
	ldrh r3, [r2, #18]
	adds r7, r6, #0
	adds r7, #16
	cmp r3, #4
	bne .L_0801921a
	movs r3, #2
	strh r3, [r6, #12]
	movs r3, #8
	strb r3, [r6, #5]
.L_0801921a:
	ldrb r3, [r6, #5]
	subs r3, #2
	cmp r3, #16
	bls .L_08019224
	b .L_080195e0
.L_08019224:
	ldr r2, .L_08019278
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0801922c:
	.4byte .L_0801927c
	.4byte .L_080195e0
	.4byte .L_0801947c
	.4byte .L_08019310
	.4byte .L_08019380
	.4byte .L_080193fc
	.4byte .L_08019548
	.4byte .L_080195da
	.4byte .L_080195da
	.4byte .L_080195da
	.4byte .L_080195da
	.4byte .L_080195e0
	.4byte .L_080194f6
	.4byte .L_080194f6
	.4byte .L_080194f6
	.4byte .L_080194e0
	.4byte .L_0801950c
.L_08019270:
	.4byte gWindowWork
.L_08019274:
	.4byte gFrameTick
.L_08019278:
	.4byte .L_0801922c
.L_0801927c:
	ldr r1, .L_080192d4
	add r1, r9
	ldrh r3, [r1]
	cmp r3, #96
	bne .L_08019288
	b .L_080195e0
.L_08019288:
	ldr r3, .L_080192d8
	lsls r2, r0, #7
	adds r2, r2, r3
	ldrh r0, [r1]
	movs r1, #128
	bl VramBlock_LoadCached
	ldr r3, .L_080192d0
	ldrh r2, [r7, #8]
	ands r0, r3
	ldr r3, .L_080192dc
	ands r3, r2
	orrs r3, r0
	strh r3, [r7, #8]
	strb r3, [r6, #14]
	ldrb r3, [r7, #5]
	movs r5, #13
	negs r5, r5
	ands r5, r3
	movs r3, #17
	negs r3, r3
	ands r5, r3
	movs r3, #32
	ldrb r2, [r7, #7]
	orrs r5, r3
	movs r3, #63
	adds r4, r3, #0
	ands r5, r3
	movs r3, #128
	ands r4, r2
	orrs r5, r3
	strb r4, [r7, #7]
	strb r5, [r7, #5]
	ldrb r3, [r6, #8]
	b .L_080192e0
	.2byte 0x0000
.L_080192d0:
	.4byte 0x000003ff
.L_080192d4:
	.4byte 0x000012b6
.L_080192d8:
	.4byte Data_080368d4
.L_080192dc:
	.4byte 0xfffffc00
.L_080192e0:
	mov r8, r3
	ldr r3, .L_08019374
	ldr r2, .L_08019378
	ldr r0, [r3]
	movs r1, #80
	str r2, [sp, #4]
	str r4, [sp, #0]
	bl __umodsi3
	ldr r2, [sp, #4]
	ldrb r3, [r2, r0]
	mov r2, r8
	adds r3, r2, r3
	adds r3, #2
	strb r3, [r7, #4]
	movs r3, #4
	negs r3, r3
	ldr r4, [sp, #0]
	ands r5, r3
	subs r3, #59
	ands r3, r4
	strb r5, [r7, #5]
	strb r3, [r7, #7]
	b .L_080195e0
.L_08019310:
	ldr r3, .L_08019374
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	bne .L_0801931e
	b .L_080195e0
.L_0801931e:
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r2, r5, #1
	lsls r3, r0, #1
	adds r2, r2, r5
	adds r3, r3, r0
	lsrs r3, r3, #16
	lsrs r2, r2, #16
	ldrh r1, [r6, #6]
	adds r2, r2, r3
	lsrs r2, r2, #1
	adds r1, r1, r2
	ldr r3, .L_08019370
	subs r1, #1
	ands r1, r3
	ldrh r2, [r7, #6]
	ldr r3, .L_0801937c
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #6]
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r2, r5, #1
	lsls r3, r0, #1
	adds r2, r2, r5
	adds r3, r3, r0
	lsrs r2, r2, #16
	lsrs r3, r3, #16
	ldrb r1, [r6, #8]
	adds r2, r2, r3
	lsrs r2, r2, #1
	adds r1, r1, r2
	subs r1, #1
	strb r1, [r7, #4]
	b .L_080195e0
.L_08019370:
	.4byte 0x000001ff
.L_08019374:
	.4byte gFrameTick
.L_08019378:
	.4byte Data_08033e60
.L_0801937c:
	.4byte 0xfffffe00
.L_08019380:
	ldrh r3, [r6, #12]
	cmp r3, #0
	bne .L_08019388
	b .L_080195b0
.L_08019388:
	ldr r1, .L_080193ec
	ldr r3, [sp, #16]
	movs r2, #128
	ands r3, r1
	lsls r2, r2, #2
	ldr r5, .L_080193f0
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #18
	ands r3, r5
	orrs r3, r2
	str r3, [sp, #16]
	add r0, sp, #16
	ldr r3, [r0, #4]
	ands r3, r1
	str r3, [r0, #4]
	bl AffineMatrix_BuildForEffect
	movs r3, #31
	ldrb r2, [r7, #7]
	ands r0, r3
	movs r3, #63
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #1
	orrs r3, r0
	strb r3, [r7, #7]
	ldrb r3, [r7, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r7, #5]
	ldr r3, .L_080193f4
	ldrh r2, [r6, #6]
	adds r2, r2, r3
	ldr r3, .L_080193e8
	ldrh r1, [r7, #6]
	ands r2, r3
	ldr r3, .L_080193f8
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	ldrb r3, [r6, #8]
	adds r3, #251
	strb r3, [r7, #4]
	ldrh r3, [r6, #12]
	adds r3, r3, r5
	strh r3, [r6, #12]
	b .L_080195e0
.L_080193e8:
	.4byte 0x000001ff
.L_080193ec:
	.4byte 0xffff0000
.L_080193f0:
	.4byte 0x0000ffff
.L_080193f4:
	.4byte 0x0000fffb
.L_080193f8:
	.4byte 0xfffffe00
.L_080193fc:
	movs r3, #128
	add r5, sp, #16
	lsls r3, r3, #1
	strh r3, [r5]
	strh r3, [r5, #2]
	movs r2, #192
	ldrh r3, [r6, #12]
	lsls r2, r2, #2
	adds r3, r3, r2
	strh r3, [r6, #12]
	strh r3, [r5, #4]
	adds r0, r5, #0
	bl AffineMatrix_BuildForEffect
	movs r3, #31
	ldrb r2, [r7, #7]
	ands r0, r3
	movs r3, #63
	negs r3, r3
	lsls r0, r0, #1
	ands r3, r2
	orrs r3, r0
	ldrb r2, [r7, #5]
	strb r3, [r7, #7]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	movs r2, #1
	orrs r3, r2
	strb r3, [r7, #5]
	ldrh r0, [r5, #4]
	movs r3, #232
	lsls r3, r3, #8
	adds r0, r0, r3
	bl Trig_Sin
	ldrh r2, [r6, #6]
	asrs r0, r0, #14
	subs r2, r2, r0
	ldr r3, .L_08019474
	subs r2, #2
	ands r2, r3
	ldrh r1, [r7, #6]
	ldr r3, .L_08019478
	ands r3, r1
	orrs r3, r2
	ldrh r0, [r5, #4]
	movs r2, #208
	lsls r2, r2, #7
	strh r3, [r7, #6]
	adds r0, r0, r2
	bl Trig_Cos
	ldrb r3, [r6, #8]
	asrs r0, r0, #14
	subs r3, r3, r0
	subs r3, #2
	strb r3, [r7, #4]
	b .L_080195e0
	.2byte 0x0000
.L_08019474:
	.4byte 0x000001ff
.L_08019478:
	.4byte 0xfffffe00
.L_0801947c:
	ldr r3, .L_080194d4
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0801948e
	ldrh r3, [r6, #12]
	adds r3, #1
	strh r3, [r6, #12]
.L_0801948e:
	ldr r4, .L_080194d8
	ldrh r0, [r6, #12]
	movs r1, #20
	str r4, [sp, #0]
	bl __umodsi3
	ldr r4, [sp, #0]
	lsls r0, r0, #16
	lsrs r0, r0, #15
	ldrsb r3, [r4, r0]
	ldrh r2, [r6, #6]
	adds r2, r2, r3
	ldr r3, .L_080194d0
	ldrh r1, [r7, #6]
	ands r2, r3
	ldr r3, .L_080194dc
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	movs r1, #20
	ldrh r0, [r6, #12]
	bl __umodsi3
	lsls r0, r0, #16
	lsrs r0, r0, #15
	ldr r4, [sp, #0]
	adds r0, #1
	ldrb r5, [r6, #8]
	ldrb r3, [r4, r0]
	adds r5, r5, r3
	subs r5, #2
	strb r5, [r7, #4]
	b .L_080195e0
.L_080194d0:
	.4byte 0x000001ff
.L_080194d4:
	.4byte gFrameTick
.L_080194d8:
	.4byte Data_08033eb0
.L_080194dc:
	.4byte 0xfffffe00
.L_080194e0:
	ldrh r3, [r6, #12]
	ldr r0, .L_08019540
	adds r3, #1
	movs r2, #15
	strh r3, [r6, #12]
	ands r3, r2
	ldrb r1, [r6, #8]
	ldrb r3, [r0, r3]
	subs r1, r1, r3
	strb r1, [r7, #4]
	b .L_080195e0
.L_080194f6:
	ldrh r3, [r6, #12]
	ldr r0, .L_08019540
	adds r3, #1
	movs r2, #15
	strh r3, [r6, #12]
	ands r3, r2
	ldrb r1, [r6, #8]
	ldrb r3, [r0, r3]
	adds r1, r1, r3
	strb r1, [r7, #4]
	b .L_080195e0
.L_0801950c:
	ldrh r3, [r6, #12]
	ldr r4, .L_08019540
	adds r3, #1
	movs r0, #15
	strh r3, [r6, #12]
	ands r3, r0
	ldrh r2, [r6, #6]
	ldrsb r3, [r4, r3]
	subs r2, r2, r3
	ldr r3, .L_0801953c
	ldrh r1, [r7, #6]
	ands r2, r3
	ldr r3, .L_08019544
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	ldrh r3, [r6, #12]
	ands r0, r3
	ldrb r2, [r6, #8]
	ldrb r3, [r4, r0]
	adds r2, r2, r3
	strb r2, [r7, #4]
	b .L_080195e0
	.2byte 0x0000
.L_0801953c:
	.4byte 0x000001ff
.L_08019540:
	.4byte Data_08033ee8
.L_08019544:
	.4byte 0xfffffe00
.L_08019548:
	ldrh r3, [r6, #12]
	cmp r3, #0
	beq .L_080195b0
	movs r3, #160
	lsls r3, r3, #1
	add r0, sp, #16
	movs r2, #0
	strh r3, [r0]
	strh r3, [r0, #2]
	strh r2, [r0, #4]
	bl AffineMatrix_BuildForEffect
	movs r3, #31
	ldrb r2, [r7, #7]
	ands r0, r3
	movs r3, #63
	negs r3, r3
	ands r3, r2
	lsls r0, r0, #1
	orrs r3, r0
	strb r3, [r7, #7]
	ldrb r3, [r7, #5]
	movs r2, #3
	orrs r3, r2
	strb r3, [r7, #5]
	ldr r3, .L_080195a4
	ldrh r2, [r6, #6]
	adds r2, r2, r3
	ldr r3, .L_080195a0
	ldrh r1, [r7, #6]
	ands r2, r3
	ldr r3, .L_080195a8
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	ldrb r3, [r6, #8]
	adds r3, #248
	strb r3, [r7, #4]
	ldr r2, .L_080195ac
	ldrh r3, [r6, #12]
	adds r3, r3, r2
	strh r3, [r6, #12]
	b .L_080195e0
	.2byte 0x0000
.L_080195a0:
	.4byte 0x000001ff
.L_080195a4:
	.4byte 0x0000fff8
.L_080195a8:
	.4byte 0xfffffe00
.L_080195ac:
	.4byte 0x0000ffff
.L_080195b0:
	ldrb r2, [r7, #7]
	movs r3, #63
	negs r3, r3
	ands r3, r2
	strb r3, [r7, #7]
	ldrb r2, [r7, #5]
	movs r3, #4
	negs r3, r3
	ands r3, r2
	strb r3, [r7, #5]
	ldr r2, .L_0801963c
	ldrh r3, [r6, #6]
	ldrh r1, [r7, #6]
	ands r2, r3
	ldr r3, .L_08019640
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #6]
	ldrh r3, [r6, #8]
	strb r3, [r7, #4]
	b .L_080195e0
.L_080195da:
	adds r0, r6, #0
	bl RenderOutput_UpdateScaleAnimation
.L_080195e0:
	ldrb r3, [r6, #5]
	cmp r3, #2
	bne .L_080195fa
	ldr r3, .L_08019644
	add r3, r9
	ldrh r3, [r3]
	cmp r3, #96
	beq .L_08019606
	ldrb r1, [r6, #15]
	adds r0, r7, #0
	bl Runtime_PushSlotEntry
	b .L_08019606
.L_080195fa:
	cmp r3, #13
	beq .L_08019606
	ldrb r1, [r6, #15]
	adds r0, r7, #0
	bl Runtime_PushSlotEntry
.L_08019606:
	ldr r3, .L_08019648
	ldr r3, [r3]
	ldr r6, [r6]
.L_0801960c:
	lsrs r0, r3, #2
	movs r3, #7
	ands r0, r3
	cmp r6, #0
	beq .L_08019618
	b .L_08019206
.L_08019618:
	movs r2, #1
	movs r3, #36
	add r11, r2
	add r10, r3
	mov r3, r11
	cmp r3, #8
	beq .L_08019628
	b .L_080191ee
.L_08019628:
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0801963c:
	.4byte 0x000001ff
.L_08019640:
	.4byte 0xfffffe00
.L_08019644:
	.4byte 0x000012b6
.L_08019648:
	.4byte gFrameTick
