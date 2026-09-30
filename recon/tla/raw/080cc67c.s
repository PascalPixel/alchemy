.syntax unified
	.thumb
	.global Func_080cc67c
	.thumb_func
Func_080cc67c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #32]
	ldr r3, .L_080cc7b4
	adds r5, r0, #0
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r3, r0
	movs r2, #0
	ldrsh r1, [r3, r2]
	ldr r3, .L_080cc7b8
	mov r8, r1
	ldr r0, [r3, #12]
	mov lr, r0
	.2byte 0xf800
	movs r3, #0
	adds r6, r0, #0
	mov r10, r3
	cmp r5, #0
	beq .L_080cc6dc
	movs r5, #1
	negs r5, r5
	b .L_080cc6b4
.L_080cc6b2:
	adds r6, #24
.L_080cc6b4:
	movs r4, #0
	ldrsh r3, [r6, r4]
	cmp r3, r5
	beq .L_080cc6d4
	cmp r3, r8
	bne .L_080cc6b2
	movs r1, #2
	ldrsh r0, [r6, r1]
	cmp r0, r5
	beq .L_080cc6d0
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080cc6b2
.L_080cc6d0:
	movs r2, #1
	mov r10, r2
.L_080cc6d4:
	mov r3, r10
	movs r0, #0
	cmp r3, #0
	beq .L_080cc7ac
.L_080cc6dc:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_080cc726
	ldr r1, .L_080cc7b4
	movs r4, #254
	lsls r4, r4, #1
	adds r2, r1, r4
	movs r4, #4
	ldrsh r3, [r6, r4]
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #128
	lsls r3, r3, #2
	adds r2, r1, r3
	movs r4, #6
	ldrsh r3, [r6, r4]
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #129
	lsls r3, r3, #2
	adds r2, r1, r3
	movs r4, #8
	ldrsh r3, [r6, r4]
	movs r4, #131
	lsls r3, r3, #16
	str r3, [r2]
	movs r3, #130
	lsls r3, r3, #2
	adds r2, r1, r3
	ldrh r3, [r6, #10]
	lsls r4, r4, #2
	str r3, [r2]
	adds r3, r1, r4
	strh r0, [r3]
.L_080cc726:
	movs r0, #14
	ldrsh r3, [r6, r0]
	movs r1, #1
	negs r1, r1
	cmp r3, r1
	beq .L_080cc73c
	adds r5, r7, #0
	adds r5, #236
	lsls r3, r3, #16
	str r3, [r5]
	b .L_080cc740
.L_080cc73c:
	adds r5, r7, #0
	adds r5, #236
.L_080cc740:
	movs r2, #16
	ldrsh r3, [r6, r2]
	cmp r3, r1
	beq .L_080cc752
	adds r4, r7, #0
	adds r4, #240
	lsls r3, r3, #16
	str r3, [r4]
	b .L_080cc756
.L_080cc752:
	adds r4, r7, #0
	adds r4, #240
.L_080cc756:
	movs r0, #18
	ldrsh r3, [r6, r0]
	cmp r3, r1
	beq .L_080cc768
	adds r2, r7, #0
	adds r2, #244
	lsls r3, r3, #16
	str r3, [r2]
	b .L_080cc76c
.L_080cc768:
	adds r2, r7, #0
	adds r2, #244
.L_080cc76c:
	movs r3, #20
	ldrsh r0, [r6, r3]
	cmp r0, r1
	beq .L_080cc77e
	adds r1, r7, #0
	adds r1, #248
	lsls r3, r0, #16
	str r3, [r1]
	b .L_080cc782
.L_080cc77e:
	adds r1, r7, #0
	adds r1, #248
.L_080cc782:
	ldr r3, [r5]
	movs r0, #240
	ldr r2, [r2]
	lsls r0, r0, #16
	adds r3, r3, r0
	cmp r3, r2
	ble .L_080cc796
	ldr r0, .L_080cc7bc
	adds r3, r2, r0
	str r3, [r5]
.L_080cc796:
	ldr r3, [r4]
	movs r2, #160
	lsls r2, r2, #16
	adds r3, r3, r2
	ldr r2, [r1]
	cmp r3, r2
	ble .L_080cc7aa
	ldr r0, .L_080cc7c0
	adds r3, r2, r0
	str r3, [r4]
.L_080cc7aa:
	movs r0, #1
.L_080cc7ac:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080cc7b4:
	.4byte gPartyState
.L_080cc7b8:
	.4byte Data_02008000
.L_080cc7bc:
	.4byte 0xff100000
.L_080cc7c0:
	.4byte 0xff600000
