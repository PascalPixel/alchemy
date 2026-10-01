.syntax unified
	.thumb
	.section .text.x0200b01c,"ax",%progbits
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
	bne .L_0200b04e
	b .L_0200b2e2
.L_0200b04e:
	adds r1, #8
	ldr r3, [sp, #64]
	ldr r4, [sp, #64]
	str r0, [sp, #8]
	ldr r0, .L_0200b2f4
	mov r10, r1
	ldr r1, .L_0200b2f8
	adds r3, #36
	adds r4, #37
	adds r0, #1
	str r3, [sp, #16]
	str r4, [sp, #12]
	str r0, [sp, #4]
	str r1, [sp, #0]
.L_0200b06a:
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
	beq .L_0200b0ba
	b .L_0200b26e
.L_0200b0ba:
	movs r4, #3
	str r4, [sp, #32]
	cmp r1, #0
	bne .L_0200b10e
	ldr r0, [sp, #40]
	ldr r2, [sp, #36]
	ldr r4, [sp, #56]
	adds r0, r0, r2
	str r0, [sp, #40]
	ldr r3, .L_0200b2fc
	lsls r2, r4, #2
	ldr r3, [r3, r2]
	cmp r0, r3
	blt .L_0200b0e0
	ldr r3, .L_0200b300
	ldr r3, [r3, r2]
	negs r3, r3
	str r3, [sp, #36]
	b .L_0200b108
.L_0200b0e0:
	ldr r0, [sp, #40]
	ldr r3, .L_0200b304
	cmp r0, r3
	bgt .L_0200b108
	ldr r3, .L_0200b300
	ldr r4, .L_0200b304
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
.L_0200b108:
	ldr r0, [sp, #40]
	str r0, [r5, #24]
	str r0, [r5, #28]
.L_0200b10e:
	bl Engine_RandomNext
	ldr r2, .L_0200b2f4
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
	beq .L_0200b148
	movs r1, #250
	lsls r0, r6, #16
	lsls r1, r1, #2
	bl IwramUnsignedDivide
	adds r6, r0, #0
	b .L_0200b14a
.L_0200b148:
	movs r6, #0
.L_0200b14a:
	cmp r7, #0
	beq .L_0200b15c
	movs r1, #250
	lsls r0, r7, #16
	lsls r1, r1, #2
	bl IwramUnsignedDivide
	mov r9, r0
	b .L_0200b160
.L_0200b15c:
	movs r2, #0
	mov r9, r2
.L_0200b160:
	mov r3, r8
	cmp r3, #0
	beq .L_0200b172
	movs r1, #250
	lsls r0, r3, #16
	lsls r1, r1, #2
	bl IwramUnsignedDivide
	b .L_0200b174
.L_0200b172:
	movs r0, #0
.L_0200b174:
	ldr r2, .L_0200b308
	ldr r4, [sp, #8]
	ldrsb r3, [r2, r4]
	cmp r3, #1
	bne .L_0200b186
	ldr r1, [sp, #52]
	adds r1, r1, r6
	str r1, [sp, #52]
	b .L_0200b198
.L_0200b186:
	ldr r4, [sp, #52]
	movs r1, #1
	subs r4, r4, r6
	negs r1, r1
	str r4, [sp, #52]
	cmp r3, r1
	beq .L_0200b198
	movs r3, #0
	str r3, [sp, #52]
.L_0200b198:
	ldr r3, [sp, #8]
	adds r3, #1
	ldrsb r3, [r2, r3]
	cmp r3, #1
	bne .L_0200b1aa
	ldr r4, [sp, #48]
	add r4, r9
	str r4, [sp, #48]
	b .L_0200b1be
.L_0200b1aa:
	ldr r1, [sp, #48]
	mov r4, r9
	subs r1, r1, r4
	str r1, [sp, #48]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	beq .L_0200b1be
	movs r3, #0
	str r3, [sp, #48]
.L_0200b1be:
	ldr r3, [sp, #8]
	adds r3, #2
	ldrsb r3, [r2, r3]
	cmp r3, #1
	bne .L_0200b1d0
	ldr r4, [sp, #44]
	adds r4, r4, r0
	str r4, [sp, #44]
	b .L_0200b1e2
.L_0200b1d0:
	ldr r1, [sp, #44]
	movs r2, #1
	subs r1, r1, r0
	negs r2, r2
	str r1, [sp, #44]
	cmp r3, r2
	beq .L_0200b1e2
	movs r3, #0
	str r3, [sp, #44]
.L_0200b1e2:
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
	beq .L_0200b250
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
	bne .L_0200b26e
	ldr r2, [sp, #28]
	mov r3, r9
	str r2, [r5, #8]
	str r2, [r5, #56]
	cmp r3, #0
	beq .L_0200b248
	str r4, [r5, #12]
	str r4, [r5, #60]
.L_0200b248:
	ldr r4, [sp, #20]
	str r4, [r5, #16]
	str r4, [r5, #64]
	b .L_0200b26e
.L_0200b250:
	ldr r3, [r5, #8]
	mov r1, r9
	adds r3, r3, r6
	str r3, [r5, #8]
	str r3, [r5, #56]
	cmp r1, #0
	beq .L_0200b266
	ldr r3, [r5, #12]
	adds r3, r3, r7
	str r3, [r5, #12]
	str r3, [r5, #60]
.L_0200b266:
	ldr r3, [r5, #16]
	adds r3, r3, r0
	str r3, [r5, #16]
	str r3, [r5, #64]
.L_0200b26e:
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
	beq .L_0200b2e2
	b .L_0200b06a
.L_0200b2e2:
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_0200b2f4:
	.4byte Soru_RingDrift
.L_0200b2f8:
	.4byte Soru_RingSwing
.L_0200b2fc:
	.4byte Soru_RingOffsetX
.L_0200b300:
	.4byte Soru_RingOffsetZ
.L_0200b304:
	.4byte 0x00001999
.L_0200b308:
	.4byte Soru_RingDirection
	.section .rodata.x0200b62c,"a",%progbits
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00060000
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000a3d
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000a3d
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000012
	.4byte 0xc0010000
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000010
	.global Funka_ArcOrigins
Funka_ArcOrigins:
	.4byte 0xfffa0000
	.4byte 0x00000000
	.4byte 0x000a0000
	.4byte 0x000a0000
	.4byte 0x00080000
	.4byte 0x00190000
	.4byte 0x00040000
	.4byte 0x001e0000
	.4byte 0xfffb0000
	.4byte 0x00140000
	.4byte 0x00020000
	.4byte 0x00050000
	.4byte 0xfffa0000
	.4byte 0x00230000
	.4byte 0xfff80000
	.4byte 0x000f0000
	.4byte 0x00020000
	.4byte 0x00280000
	.4byte 0xfffe0000
	.4byte 0x000f0000
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
	.4byte 0x00000012
	.4byte 0x0050800b
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
	.4byte 0x00004000
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
	.4byte 0x00004000
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
	.4byte 0xffff00fc
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d8
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Effects
Placement_Effects:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
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
	.global Funka_EmberScript
Funka_EmberScript:
	.4byte 0x00000022
	.4byte OverlayObject_SetRecordAngleFromHeading
	.4byte 0x00000010
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
	.section .bss,"aw",%nobits
	.space 8
	.global gEmberState
gEmberState:
	.space 64
	.global gEmberTimer
gEmberTimer:
	.space 16
	.global gArcEffects
gArcEffects:
	.space 48
	.global gArcEffectTimers
gArcEffectTimers:
	.space 40
	.global gEmberMask
gEmberMask:
	.space 4
	.global gEmberLevel
gEmberLevel:
	.space 4
	.global gEmberLevelTimer
gEmberLevelTimer:
	.space 4
