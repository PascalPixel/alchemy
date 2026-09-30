.syntax unified
	.thumb
	.global Func_080cd680
	.thumb_func
Func_080cd680:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_080cd804
	movs r3, #192
	movs r1, #133
	lsls r3, r3, #18
	lsls r1, r1, #2
	ldr r6, [r3, #108]
	adds r3, r2, r1
	adds r1, #88
	adds r7, r0, #0
	ldr r0, [r3]
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r3
	cmp r3, #0
	beq .L_080cd6aa
	b .L_080cd7fc
.L_080cd6aa:
	movs r3, #32
	ands r3, r7
	cmp r3, #0
	beq .L_080cd6da
	movs r2, #194
	lsls r2, r2, #1
	adds r5, r6, r2
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #12
	ble .L_080cd6da
	adds r2, #2
	adds r3, r6, r2
	ldrh r2, [r3]
	movs r3, #128
	ands r3, r2
	cmp r3, #0
	beq .L_080cd6da
	movs r1, #6
	movs r2, #0
	bl Func_080d3460
	mov r3, r8
	strh r3, [r5]
.L_080cd6da:
	movs r3, #128
	ands r3, r7
	cmp r3, #0
	beq .L_080cd6fa
	movs r1, #194
	lsls r1, r1, #1
	adds r5, r6, r1
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #12
	ble .L_080cd6fa
	bl Func_080d4d08
	cmp r0, #0
	bne .L_080cd6fa
	strh r0, [r5]
.L_080cd6fa:
	movs r3, #64
	ands r3, r7
	cmp r3, #0
	beq .L_080cd75c
	movs r1, #194
	lsls r1, r1, #1
	adds r3, r6, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #12
	ble .L_080cd75c
	adds r1, #2
	adds r3, r6, r1
	ldrh r3, [r3]
	movs r2, #240
	movs r0, #1
	ands r2, r3
	negs r0, r0
	cmp r2, #128
	bne .L_080cd728
	bl Func_080d50f8
	b .L_080cd730
.L_080cd728:
	cmp r2, #64
	bne .L_080cd730
	bl Func_080d53b8
.L_080cd730:
	cmp r0, #0
	bne .L_080cd75c
	movs r2, #194
	lsls r2, r2, #1
	movs r1, #173
	adds r3, r6, r2
	lsls r1, r1, #1
	strh r0, [r3]
	subs r2, #38
	adds r3, r6, r1
	strh r0, [r3]
	adds r1, #2
	adds r3, r6, r2
	strh r0, [r3]
	adds r2, #6
	adds r3, r6, r1
	strh r0, [r3]
	adds r1, #10
	adds r3, r6, r2
	strh r0, [r3]
	adds r3, r6, r1
	strh r0, [r3]
.L_080cd75c:
	movs r3, #2
	ands r3, r7
	cmp r3, #0
	beq .L_080cd7fc
	movs r2, #194
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #12
	ble .L_080cd7fc
	bl Func_080cdc74
	cmp r0, #0
	beq .L_080cd7fc
	ldr r3, .L_080cd804
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Func_080cd91c
	movs r3, #1
	negs r3, r3
	adds r5, r0, #0
	mov r8, r3
	cmp r5, r8
	beq .L_080cd7fc
	movs r0, #8
	adds r1, r5, #0
	bl Func_080ccd78
	cmp r0, #0
	beq .L_080cd7c0
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_080cd7c0
	movs r1, #128
	lsls r1, r1, #9
	cmp r3, r1
	bge .L_080cd7b8
	mov r0, r8
	adds r1, r3, #0
	bl Func_080cdea8
	b .L_080cd7c0
.L_080cd7b8:
	adds r0, r7, #0
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_080cd7c0:
	bl Func_080cdd80
	movs r0, #9
	adds r1, r7, #0
	bl Func_080ccd78
	cmp r0, #0
	beq .L_080cd7f2
	ldr r3, [r0, #8]
	cmp r3, #0
	beq .L_080cd7f2
	movs r2, #128
	lsls r2, r2, #9
	cmp r3, r2
	bge .L_080cd7ea
	movs r0, #1
	negs r0, r0
	adds r1, r3, #0
	bl Func_080cdea8
	b .L_080cd7f2
.L_080cd7ea:
	adds r0, r7, #0
	adds r1, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_080cd7f2:
	movs r3, #194
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #0
	strh r3, [r2]
.L_080cd7fc:
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080cd804:
	.4byte gPartyState
