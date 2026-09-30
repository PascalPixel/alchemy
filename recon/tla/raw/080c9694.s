.syntax unified
	.thumb
	.global Func_080c9694
	.thumb_func
Func_080c9694:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_080c9770
	mov r8, r0
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r1, #0
	ldrsh r7, [r3, r1]
	ldr r3, .L_080c9774
	ldr r0, [r3, #20]
	mov lr, r0
	.2byte 0xf800
	movs r1, #186
	lsls r1, r1, #2
	movs r2, #0
	adds r1, #255
	adds r6, r0, #0
	mov r9, r2
	cmp r8, r1
	beq .L_080c9766
	b .L_080c972c
.L_080c96c6:
	cmp r5, r7
	bne .L_080c972c
	mov r10, r6
	b .L_080c96f0
.L_080c96ce:
	cmp r2, #255
	beq .L_080c96d6
	cmp r2, r8
	bne .L_080c96f0
.L_080c96d6:
	cmp r0, #0
	beq .L_080c96ea
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_080c96ea
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080c96f0
.L_080c96ea:
	adds r1, r5, #0
	mov r9, r7
	b .L_080c9748
.L_080c96f0:
	ldmia r6!, {r3}
	movs r2, #255
	lsls r2, r2, #12
	ands r2, r3
	movs r5, #240
	lsrs r7, r2, #12
	movs r0, #128
	movs r2, #255
	lsls r5, r5, #4
	lsls r2, r2, #20
	lsls r0, r0, #21
	adds r5, #255
	ands r2, r3
	ands r0, r3
	ands r5, r3
	lsrs r2, r2, #20
	cmp r0, #0
	beq .L_080c9716
	ldmia r6!, {r0}
.L_080c9716:
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	cmp r5, r3
	beq .L_080c9724
	cmp r2, #0
	bne .L_080c96ce
.L_080c9724:
	mov r2, r10
	ldr r1, [r2]
	ands r1, r3
	b .L_080c9748
.L_080c972c:
	ldmia r6!, {r5}
	ldr r3, .L_080c9778
	ands r3, r5
	cmp r3, #0
	bne .L_080c972c
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #255
	ands r5, r3
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	cmp r5, r3
	bne .L_080c96c6
.L_080c9748:
	movs r0, #186
	lsls r0, r0, #2
	adds r0, #255
	cmp r1, r0
	beq .L_080c9766
	ldr r2, .L_080c9770
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	strh r1, [r3]
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	mov r2, r9
	strh r2, [r3]
.L_080c9766:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_080c9770:
	.4byte gPartyState
.L_080c9774:
	.4byte gOverlayArea
.L_080c9778:
	.4byte 0xfffff000
