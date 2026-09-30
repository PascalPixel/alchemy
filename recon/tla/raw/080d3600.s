.syntax unified
	.thumb
	.global FacingObject_TurnPairToFaceEachOther
	.thumb_func
FacingObject_TurnPairToFaceEachOther:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r6, r0, #0
	sub sp, #4
	adds r5, r1, #0
	cmp r6, #0
	beq .L_080d369a
	cmp r5, #0
	beq .L_080d369a
	ldr r3, [r6, #16]
	ldr r0, [r5, #16]
	ldr r1, [r5, #8]
	subs r0, r0, r3
	ldr r3, [r6, #8]
	subs r1, r1, r3
	bl ArcTan2
	lsls r0, r0, #16
	lsrs r7, r0, #16
	movs r0, #128
	lsls r0, r0, #8
	adds r0, r0, r7
	mov r8, r0
	movs r4, #0
.L_080d3632:
	ldrh r2, [r6, #6]
	movs r1, #2
	subs r3, r7, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	beq .L_080d365a
	movs r0, #128
	lsls r0, r0, #5
	cmp r3, r0
	ble .L_080d364c
	movs r3, #128
	lsls r3, r3, #5
.L_080d364c:
	ldr r0, .L_080d36a4
	cmp r3, r0
	bge .L_080d3654
	ldr r3, .L_080d36a4
.L_080d3654:
	adds r3, r2, r3
	strh r3, [r6, #6]
	b .L_080d365c
.L_080d365a:
	movs r1, #1
.L_080d365c:
	ldrh r2, [r5, #6]
	mov r0, r8
	subs r3, r0, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	beq .L_080d3684
	movs r0, #128
	lsls r0, r0, #5
	cmp r3, r0
	ble .L_080d3676
	movs r3, #128
	lsls r3, r3, #5
.L_080d3676:
	ldr r0, .L_080d36a4
	cmp r3, r0
	bge .L_080d367e
	ldr r3, .L_080d36a4
.L_080d367e:
	adds r3, r2, r3
	strh r3, [r5, #6]
	b .L_080d3686
.L_080d3684:
	subs r1, #1
.L_080d3686:
	cmp r1, #0
	beq .L_080d369a
	movs r0, #1
	str r4, [sp, #0]
	bl WaitFrames
	ldr r4, [sp, #0]
	adds r4, #1
	cmp r4, #59
	ble .L_080d3632
.L_080d369a:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d36a4:
	.4byte 0xfffff000
