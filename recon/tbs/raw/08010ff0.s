.syntax unified
	.thumb
	.global MapAnimation_ApplyAffineFrame
	.thumb_func
MapAnimation_ApplyAffineFrame:
	push {r5, r6, lr}
	ldr r1, .L_080110c0
	movs r2, #200
	ldmia r1!, {r3}
	lsls r2, r2, #4
	adds r6, r3, r2
	movs r3, #128
	lsls r3, r3, #19
	ldrh r2, [r3]
	ldr r1, [r1]
	ldr r3, .L_080110c4
	mov r12, r1
	ands r3, r2
	ldr r1, .L_080110c8
	lsls r3, r3, #16
	ldrh r2, [r1, #10]
	asrs r5, r3, #16
	ldr r3, .L_080110cc
	ands r3, r2
	strh r3, [r1, #10]
	ldr r3, .L_080110d0
	ldrh r2, [r1, #10]
	ands r3, r2
	strh r3, [r1, #10]
	ldr r4, .L_080110d4
	ldrh r3, [r1, #10]
	cmp r6, #0
	beq .L_08011064
	ldr r3, .L_080110d8
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #10
	adds r0, r6, r0
	ldmia r0!, {r3}
	str r3, [r4]
	ldmia r0!, {r3}
	adds r4, #4
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	stmia r4!, {r3}
	ldmia r0!, {r3}
	ldr r2, .L_080110dc
	str r3, [r4]
	adds r3, r1, #0
	subs r1, #144
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_08011064:
	movs r3, #128
	lsls r3, r3, #1
	add r3, r12
	movs r2, #130
	ldrh r3, [r3]
	lsls r2, r2, #1
	add r2, r12
	strh r3, [r2]
	movs r3, #129
	lsls r3, r3, #1
	add r3, r12
	ldrh r0, [r3]
	movs r3, #131
	lsls r3, r3, #1
	add r3, r12
	strh r0, [r3]
	ldrh r1, [r2]
	movs r3, #0
	cmp r1, #199
	bhi .L_080110a4
	lsls r2, r0, #16
	lsrs r2, r2, #16
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	lsls r3, r3, #1
	cmp r1, r2
	bhi .L_080110a4
	movs r3, #0
	cmp r1, #0
	bne .L_080110a4
	movs r3, #2
.L_080110a4:
	orrs r5, r3
	lsls r3, r5, #16
	movs r2, #128
	lsrs r3, r3, #16
	lsls r2, r2, #19
	strh r3, [r2]
	movs r2, #132
	lsls r2, r2, #1
	add r2, r12
	movs r3, #0
	strh r3, [r2]
	pop {r5, r6}
	pop {r0}
	bx r0
.L_080110c0:
	.4byte gMapAnimationPages
.L_080110c4:
	.4byte 0x0000fff8
.L_080110c8:
	.4byte 0x040000b0
.L_080110cc:
	.4byte 0x0000c5ff
.L_080110d0:
	.4byte 0x00007fff
.L_080110d4:
	.4byte 0x04000020
.L_080110d8:
	.4byte gFrameCount
.L_080110dc:
	.4byte 0xa6600008
