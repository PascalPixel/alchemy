.syntax unified
	.thumb
	.section .text.x0200a27c,"ax",%progbits
	.global Func_0200227c
	.thumb_func
Func_0200227c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #0
	bl Object_GetById
	mov r10, r0
	bl Engine_EventBegin
	movs r0, #5
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #9
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #11
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #10
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #14
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #13
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r2, #166
	movs r0, #5
	ldr r1, .L_0200a3e8
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #166
	movs r0, #9
	ldr r1, .L_0200a3ec
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #174
	movs r0, #11
	ldr r1, .L_0200a3f0
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #174
	movs r0, #10
	ldr r1, .L_0200a3f4
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r1, #230
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #153
	ldr r1, .L_0200a3f8
	lsls r2, r2, #17
	movs r0, #13
	bl Engine_ActorSetPosition
	movs r0, #5
	bl Object_GetById
	mov r1, r10
	str r1, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	movs r6, #1
	orrs r3, r6
	strb r3, [r2]
	ldr r3, .L_0200a3fc
	movs r1, #0
	mov r8, r3
	mov r9, r1
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #9
	bl Object_GetById
	mov r1, r10
	str r1, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	strb r3, [r2]
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #11
	bl Object_GetById
	mov r3, r10
	str r3, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	strb r3, [r2]
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #10
	bl Object_GetById
	mov r1, r10
	str r1, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	mov r1, r8
	strb r3, [r2]
	bl Engine_ObjectSetScript
	movs r0, #14
	bl Object_GetById
	mov r3, r10
	adds r5, r0, #0
	str r3, [r5, #104]
	adds r2, r5, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #11
	bl Object_GetById
	adds r0, #85
	ldrb r3, [r0]
	adds r2, r5, #0
	adds r2, #85
	mov r1, r9
	strb r3, [r2]
	adds r0, r5, #0
	str r1, [r5, #12]
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #13
	bl Object_GetById
	mov r3, r10
	str r3, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r6, r3
	strb r6, [r2]
	mov r1, r8
	bl Engine_ObjectSetScript
	bl Engine_EventEnd
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0200a3e8:
	.4byte 0x01db0000
.L_0200a3ec:
	.4byte 0x01eb0000
.L_0200a3f0:
	.4byte 0x01cb0000
.L_0200a3f4:
	.4byte 0x01fb0000
.L_0200a3f8:
	.4byte 0x01d70000
.L_0200a3fc:
	.4byte SoruStar_AngleScript
	.section .text.x0200c49c,"ax",%progbits
	.global Soru_UpdateRing
	.thumb_func
Soru_UpdateRing:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #202
	lsls r1, r1, #1
	movs r0, #33
	sub sp, #68
	bl Runtime_AllocateBlock
	str r0, [sp, #64]
	str r0, [sp, #60]
	ldr r1, [sp, #64]
	movs r0, #0
	movs r2, #200
	str r0, [sp, #56]
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_0200c4ce
	b .L_0200c762
.L_0200c4ce:
	adds r1, #8
	ldr r3, [sp, #64]
	ldr r4, [sp, #64]
	str r0, [sp, #8]
	ldr r0, .L_0200c774
	mov r10, r1
	ldr r1, .L_0200c778
	adds r3, #36
	adds r4, #37
	adds r0, #1
	str r3, [sp, #16]
	str r4, [sp, #12]
	str r0, [sp, #4]
	str r1, [sp, #0]
.L_0200c4ea:
	mov r3, r10
	ldr r3, [r3, #8]
	ldr r2, [sp, #60]
	ldr r5, [r2]
	str r3, [sp, #52]
	mov r4, r10
	ldr r4, [r4, #12]
	str r4, [sp, #48]
	mov r0, r10
	ldr r0, [r0, #16]
	str r0, [sp, #44]
	mov r1, r10
	ldr r1, [r1, #20]
	str r1, [sp, #40]
	mov r2, r10
	ldr r2, [r2, #24]
	ldr r4, [sp, #60]
	str r2, [sp, #36]
	ldr r3, [sp, #12]
	ldr r4, [r4, #4]
	ldrb r3, [r3]
	ldr r0, [sp, #60]
	str r4, [sp, #28]
	ldr r0, [r0, #8]
	ldr r2, [sp, #60]
	str r0, [sp, #24]
	ldr r2, [r2, #12]
	mov r11, r3
	str r2, [sp, #20]
	ldr r3, [sp, #16]
	ldrb r3, [r3]
	str r3, [sp, #32]
	adds r3, #255
	lsls r3, r3, #24
	lsrs r3, r3, #24
	mov r1, r11
	str r3, [sp, #32]
	cmp r3, #0
	beq .L_0200c53a
	b .L_0200c6ee
.L_0200c53a:
	movs r4, #3
	str r4, [sp, #32]
	cmp r1, #0
	bne .L_0200c58e
	ldr r0, [sp, #40]
	ldr r2, [sp, #36]
	ldr r4, [sp, #56]
	adds r0, r0, r2
	str r0, [sp, #40]
	ldr r3, .L_0200c77c
	lsls r2, r4, #2
	ldr r3, [r3, r2]
	cmp r0, r3
	blt .L_0200c560
	ldr r3, .L_0200c780
	ldr r3, [r3, r2]
	negs r3, r3
	str r3, [sp, #36]
	b .L_0200c588
.L_0200c560:
	ldr r0, [sp, #40]
	ldr r3, .L_0200c784
	cmp r0, r3
	bgt .L_0200c588
	ldr r3, .L_0200c780
	ldr r4, .L_0200c784
	ldr r3, [r3, r2]
	str r4, [sp, #40]
	str r3, [sp, #36]
	ldr r2, [r5, #8]
	str r2, [sp, #28]
	ldr r3, [r5, #12]
	str r3, [sp, #24]
	ldr r4, [r5, #16]
	movs r0, #24
	str r4, [sp, #20]
	str r1, [r5, #8]
	str r1, [r5, #12]
	str r1, [r5, #16]
	mov r11, r0
.L_0200c588:
	ldr r0, [sp, #40]
	str r0, [r5, #24]
	str r0, [r5, #28]
.L_0200c58e:
	bl Engine_RandomNext
	ldr r2, .L_0200c774
	ldr r1, [sp, #8]
	ldrb r3, [r1, r2]
	muls r3, r0
	lsrs r6, r3, #16
	bl Engine_RandomNext
	ldr r4, [sp, #4]
	ldrb r3, [r4]
	muls r3, r0
	lsrs r7, r3, #16
	bl Engine_RandomNext
	ldr r1, [sp, #4]
	ldrb r3, [r1, #1]
	muls r3, r0
	lsrs r3, r3, #16
	mov r8, r3
	cmp r6, #0
	beq .L_0200c5c8
	movs r1, #250
	lsls r0, r6, #16
	lsls r1, r1, #2
	bl IwramUnsignedDivideEntry
	adds r6, r0, #0
	b .L_0200c5ca
.L_0200c5c8:
	movs r6, #0
.L_0200c5ca:
	cmp r7, #0
	beq .L_0200c5dc
	movs r1, #250
	lsls r0, r7, #16
	lsls r1, r1, #2
	bl IwramUnsignedDivideEntry
	mov r9, r0
	b .L_0200c5e0
.L_0200c5dc:
	movs r2, #0
	mov r9, r2
.L_0200c5e0:
	mov r3, r8
	cmp r3, #0
	beq .L_0200c5f2
	movs r1, #250
	lsls r0, r3, #16
	lsls r1, r1, #2
	bl IwramUnsignedDivideEntry
	b .L_0200c5f4
.L_0200c5f2:
	movs r0, #0
.L_0200c5f4:
	ldr r2, .L_0200c788
	ldr r4, [sp, #8]
	ldrsb r3, [r2, r4]
	cmp r3, #1
	bne .L_0200c606
	ldr r1, [sp, #52]
	adds r1, r1, r6
	str r1, [sp, #52]
	b .L_0200c618
.L_0200c606:
	ldr r4, [sp, #52]
	movs r1, #1
	subs r4, r4, r6
	negs r1, r1
	str r4, [sp, #52]
	cmp r3, r1
	beq .L_0200c618
	movs r3, #0
	str r3, [sp, #52]
.L_0200c618:
	ldr r3, [sp, #8]
	adds r3, #1
	ldrsb r3, [r2, r3]
	cmp r3, #1
	bne .L_0200c62a
	ldr r4, [sp, #48]
	add r4, r9
	str r4, [sp, #48]
	b .L_0200c63e
.L_0200c62a:
	ldr r1, [sp, #48]
	mov r4, r9
	subs r1, r1, r4
	str r1, [sp, #48]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	beq .L_0200c63e
	movs r3, #0
	str r3, [sp, #48]
.L_0200c63e:
	ldr r3, [sp, #8]
	adds r3, #2
	ldrsb r3, [r2, r3]
	cmp r3, #1
	bne .L_0200c650
	ldr r4, [sp, #44]
	adds r4, r4, r0
	str r4, [sp, #44]
	b .L_0200c662
.L_0200c650:
	ldr r1, [sp, #44]
	movs r2, #1
	subs r1, r1, r0
	negs r2, r2
	str r1, [sp, #44]
	cmp r3, r2
	beq .L_0200c662
	movs r3, #0
	str r3, [sp, #44]
.L_0200c662:
	ldr r4, [sp, #0]
	ldr r1, [sp, #52]
	ldrb r3, [r4]
	adds r0, r3, #0
	muls r0, r1
	bl Engine_MathSin
	ldr r2, [sp, #0]
	ldr r4, [sp, #48]
	ldrb r3, [r2, #1]
	lsls r6, r0, #1
	adds r0, r3, #0
	muls r0, r4
	bl Engine_MathSin
	lsls r7, r0, #1
	ldr r0, [sp, #0]
	ldr r1, [sp, #44]
	ldrb r3, [r0, #2]
	adds r0, r3, #0
	muls r0, r1
	bl Engine_MathCos
	mov r2, r11
	lsls r0, r0, #1
	cmp r2, #0
	beq .L_0200c6d0
	ldr r3, [sp, #28]
	adds r3, r3, r6
	str r3, [sp, #28]
	mov r3, r11
	ldr r4, [sp, #24]
	ldr r1, [sp, #20]
	adds r3, #255
	lsls r3, r3, #24
	adds r4, r4, r7
	adds r1, r1, r0
	lsrs r3, r3, #24
	str r4, [sp, #24]
	str r1, [sp, #20]
	mov r11, r3
	cmp r3, #0
	bne .L_0200c6ee
	ldr r2, [sp, #28]
	mov r3, r9
	str r2, [r5, #8]
	str r2, [r5, #56]
	cmp r3, #0
	beq .L_0200c6c8
	str r4, [r5, #12]
	str r4, [r5, #60]
.L_0200c6c8:
	ldr r4, [sp, #20]
	str r4, [r5, #16]
	str r4, [r5, #64]
	b .L_0200c6ee
.L_0200c6d0:
	ldr r3, [r5, #8]
	mov r1, r9
	adds r3, r3, r6
	str r3, [r5, #8]
	str r3, [r5, #56]
	cmp r1, #0
	beq .L_0200c6e6
	ldr r3, [r5, #12]
	adds r3, r3, r7
	str r3, [r5, #12]
	str r3, [r5, #60]
.L_0200c6e6:
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r5, #16]
	str r3, [r5, #64]
.L_0200c6ee:
	ldr r2, [sp, #52]
	mov r3, r10
	str r2, [r3, #8]
	ldr r4, [sp, #48]
	str r4, [r3, #12]
	ldr r0, [sp, #44]
	str r0, [r3, #16]
	ldr r1, [sp, #40]
	str r1, [r3, #20]
	ldr r2, [sp, #36]
	str r2, [r3, #24]
	ldr r4, [sp, #12]
	mov r3, r11
	strb r3, [r4]
	ldr r0, [sp, #28]
	ldr r1, [sp, #60]
	str r0, [r1, #4]
	ldr r2, [sp, #24]
	mov r3, r10
	str r2, [r3]
	ldr r4, [sp, #20]
	add r0, sp, #32
	str r4, [r1, #12]
	ldrb r0, [r0]
	ldr r1, [sp, #16]
	strb r0, [r1]
	ldr r1, [sp, #0]
	ldr r2, [sp, #8]
	adds r1, #3
	adds r2, #3
	ldr r3, [sp, #4]
	ldr r4, [sp, #56]
	str r1, [sp, #0]
	str r2, [sp, #8]
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	adds r3, #3
	adds r4, #1
	adds r1, #40
	adds r2, #40
	str r3, [sp, #4]
	str r4, [sp, #56]
	str r1, [sp, #16]
	str r2, [sp, #12]
	ldr r3, [sp, #60]
	movs r0, #40
	adds r3, #40
	add r10, r0
	ldr r4, [sp, #64]
	movs r0, #200
	str r3, [sp, #60]
	lsls r0, r0, #1
	adds r3, r4, r0
	ldrh r3, [r3]
	ldr r1, [sp, #56]
	cmp r1, r3
	beq .L_0200c762
	b .L_0200c4ea
.L_0200c762:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0200c774:
	.4byte Soru_RingDrift
.L_0200c778:
	.4byte Soru_RingSwing
.L_0200c77c:
	.4byte Soru_RingOffsetX
.L_0200c780:
	.4byte Soru_RingOffsetZ
.L_0200c784:
	.4byte 0x00001999
.L_0200c788:
	.4byte Soru_RingDirection
	.section .rodata.x0200cb1c,"a",%progbits
.L_0200cb1c:
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
.L_0200cb54:
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
.L_0200cb8c:
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
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200cb1c
	.4byte .L_0200cb54
	.4byte .L_0200cb8c
	.global SoruStar_AngleScript
SoruStar_AngleScript:
	.4byte 0x00000022
	.4byte UpdateOverlayObjectAngle
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global SoruStar_PresentItemScript
SoruStar_PresentItemScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000f000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000f000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000010
	.global Placement_Scripts
Placement_Scripts:
	.4byte 0xffff0000
	.4byte 0x000001d8
	.4byte 0x40000142
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x00000011
	.4byte 0x0020a012
	.4byte 0x000001ff
	.global Placement_Actors
Placement_Actors:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02690000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01ad0000
	.4byte 0x00000000
	.4byte 0x00dd0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00710000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x01ff0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x018d0000
	.4byte 0x00000000
	.4byte 0x01a20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x021e0000
	.4byte 0x00000000
	.4byte 0x010d0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01a50000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01270000
	.4byte 0x00000000
	.4byte 0x00d20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02e60000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Effects
Placement_Effects:
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Sukureta_Talk
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte Jasmine_Talk
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Saturos_Talk
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Menardi_Talk
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Garcia_Talk
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Alex_Talk
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000010cb
	.4byte 0x00008d15
	.4byte 0xffff0005
	.4byte Jasmine_Talk
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000010ca
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte Menardi_Talk
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000010cc
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte SceneDialogue_RunLine1072WithPair9And10
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte FieldScene_Forward72b4
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte FieldScene_Forward72b4
	.4byte 0x00000003
	.4byte 0x083c0002
	.4byte Scene_BagVenusStar
	.4byte 0x00000003
	.4byte 0x083d0003
	.4byte Scene_BagMercuryStar
	.4byte 0x00000003
	.4byte 0x083e0004
	.4byte Scene_HandOverStars
	.4byte 0x00000003
	.4byte 0x083f0005
	.4byte Scene_BagMarsStar
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global SoruStar_StarCells
SoruStar_StarCells:
	.4byte 0x0028003b
	.4byte 0x00040003
	.4byte 0x003e0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280041
	.4byte 0x00040003
	.4byte 0x00440006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280047
	.4byte 0x00040003
	.4byte 0x004a0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x0028004d
	.4byte 0x00040003
	.4byte 0x00500006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280053
	.4byte 0x00040003
	.4byte 0xffff0000
	.global Soru_RingDrift
Soru_RingDrift:
	.4byte 0x04040404
	.4byte 0x00040300
	.4byte 0x04000404
	.4byte 0x04040003
	.4byte 0x00060406
	.4byte 0x03000404
	.4byte 0x02010002
	.2byte 0x0200
	.global Soru_RingSwing
Soru_RingSwing:
	.2byte 0x0101
	.4byte 0x02000102
	.4byte 0x01020001
	.4byte 0x00010100
	.4byte 0x02010102
	.4byte 0x01020001
	.4byte 0x00010200
	.4byte 0x02000102
	.global Soru_RingDirection
Soru_RingDirection:
	.4byte 0x01010101
	.4byte 0x00010100
	.4byte 0x0100ff01
	.4byte 0x01ff00ff
	.4byte 0x00ff0101
	.4byte 0xff00ffff
	.4byte 0xffff00ff
	.4byte 0x0000ff00
	.global Soru_RingOffsetX
Soru_RingOffsetX:
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x00009999
	.4byte 0x0000b333
	.global Soru_RingOffsetZ
Soru_RingOffsetZ:
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x0000028f
	.4byte 0x0000020c
	.4byte 0x0000028f
