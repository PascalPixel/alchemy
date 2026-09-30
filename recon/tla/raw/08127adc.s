.syntax unified
	.thumb
	.global Summon_TakeCharge
	.thumb_func
Summon_TakeCharge:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #36]
	movs r1, #0
	adds r6, r5, #0
	adds r6, #64
	ldrb r4, [r6]
	sub sp, #4
	cmp r1, r4
	bge .L_08127b0a
	ldrh r3, [r5, #16]
	cmp r3, r0
	beq .L_08127b0a
	adds r2, r5, #0
	adds r2, #16
.L_08127afc:
	adds r1, #1
	cmp r1, r4
	bge .L_08127b0a
	adds r2, #2
	ldrh r3, [r2]
	cmp r3, r0
	bne .L_08127afc
.L_08127b0a:
	cmp r1, r4
	beq .L_08127b70
	adds r6, r1, #0
	adds r6, #52
	ldrsb r3, [r5, r6]
	movs r4, #0
	cmp r3, #0
	bge .L_08127b2e
	movs r3, #1
	strb r3, [r5, r6]
	movs r0, #128
	lsls r3, r1, #2
	adds r3, #28
	movs r2, #3
	lsls r0, r0, #8
	str r2, [r5, r3]
	adds r0, #1
	b .L_08127b9a
.L_08127b2e:
	lsls r7, r1, #2
	b .L_08127b34
.L_08127b32:
	adds r4, #1
.L_08127b34:
	cmp r4, #31
	bgt .L_08127b5c
	ldrsb r0, [r5, r6]
	movs r1, #9
	adds r0, #1
	str r4, [sp, #0]
	bl Math_Mod
	strb r0, [r5, r6]
	adds r3, r7, #0
	adds r3, #28
	lsls r0, r0, #24
	ldr r3, [r5, r3]
	asrs r0, r0, #24
	movs r2, #1
	lsls r2, r0
	ands r3, r2
	ldr r4, [sp, #0]
	cmp r3, #0
	bne .L_08127b32
.L_08127b5c:
	ldrsb r3, [r5, r6]
	adds r1, r7, #0
	adds r1, #28
	movs r2, #1
	lsls r2, r3
	ldr r3, [r5, r1]
	orrs r3, r2
	str r3, [r5, r1]
	ldrsb r0, [r5, r6]
	b .L_08127b9a
.L_08127b70:
	cmp r4, #4
	bgt .L_08127b96
	movs r1, #1
	adds r2, r4, #0
	negs r1, r1
	adds r2, #52
	adds r3, r1, #0
	strb r3, [r5, r2]
	lsls r3, r4, #1
	adds r3, #16
	strh r0, [r5, r3]
	lsls r3, r4, #2
	adds r3, #28
	movs r2, #0
	str r2, [r5, r3]
	adds r3, r4, #1
	strb r3, [r6]
	movs r0, #9
	b .L_08127b9a
.L_08127b96:
	movs r0, #1
	negs r0, r0
.L_08127b9a:
	add sp, #4
	pop {r5, r6, r7, pc}
	.2byte 0x0000
