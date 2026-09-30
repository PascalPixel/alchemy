.syntax unified
	.thumb
	.global Func_0803ed98
	.thumb_func
Func_0803ed98:
	push {r5, r6, r7, lr}
	movs r1, #192
	lsls r1, r1, #2
	adds r7, r0, #0
	adds r1, #158
	adds r6, r7, r1
	ldrh r1, [r6]
	bl Menu_ReloadNodeResource
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #162
	adds r2, r7, r3
	movs r3, #33
	strh r3, [r2]
	movs r0, #1
	bl WaitFrames
	movs r1, #229
	lsls r1, r1, #2
	adds r3, r7, r1
	ldrh r1, [r3]
	adds r3, r1, #0
	cmp r3, #5
	bhi .L_0803edcc
	b .L_0803eef0
.L_0803edcc:
	movs r2, #231
	lsls r2, r2, #2
	adds r5, r7, r2
	ldrh r1, [r5]
	ldrh r2, [r6]
	adds r3, r1, #0
	orrs r3, r2
	cmp r3, #0
	beq .L_0803ee24
	adds r6, r2, #0
	cmp r6, #1
	bne .L_0803ee0e
	adds r3, r1, #0
	cmp r3, #0
	beq .L_0803ee0e
	movs r3, #8
	strh r3, [r7, #8]
	movs r1, #255
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	strh r3, [r5]
	adds r0, r7, #0
	movs r1, #0
	bl Menu_ScrollSelectionList
	ldrh r3, [r5]
	cmp r3, #0
	bne .L_0803ee0a
	strh r3, [r7, #10]
.L_0803ee0a:
	strh r6, [r7, #62]
	b .L_0803ef0c
.L_0803ee0e:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #158
	adds r2, r7, r3
	ldrh r3, [r2]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	strh r3, [r2]
	b .L_0803ef0c
.L_0803ee24:
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r7, r2
	movs r0, #0
	ldr r5, [r3]
	strh r0, [r7, #62]
	movs r1, #64
	ldr r3, [r5, #4]
	cmp r3, #0
	beq .L_0803ee4c
	movs r2, #12
.L_0803ee3a:
	ldrh r3, [r5, #16]
	strh r2, [r5, #20]
	adds r3, r3, r1
	strh r3, [r5, #24]
	ldr r5, [r5, #4]
	subs r1, #16
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0803ee3a
.L_0803ee4c:
	movs r1, #210
	lsls r1, r1, #2
	adds r3, r7, r1
	ldr r5, [r3]
	b .L_0803ee5c
.L_0803ee56:
	movs r0, #1
	bl WaitFrames
.L_0803ee5c:
	movs r3, #16
	ldrsh r2, [r5, r3]
	movs r1, #24
	ldrsh r3, [r5, r1]
	cmp r2, r3
	bne .L_0803ee56
	movs r2, #229
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrh r3, [r3]
	movs r1, #0
	cmp r3, #5
	beq .L_0803ee82
	adds r3, r7, r2
	ldrh r3, [r3]
	subs r3, #5
.L_0803ee7c:
	adds r1, #1
	cmp r1, r3
	bne .L_0803ee7c
.L_0803ee82:
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r7, r2
	adds r2, #84
	ldr r5, [r3]
	adds r3, r7, r2
	strh r1, [r3]
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #158
	adds r2, r7, r3
	movs r3, #4
	strh r3, [r2]
	cmp r5, #0
	beq .L_0803eebe
	lsls r3, r1, #1
	movs r1, #213
	adds r3, r3, r7
	lsls r1, r1, #2
	adds r6, r3, r1
.L_0803eeaa:
	ldrh r0, [r6]
	ldrh r1, [r6, #32]
	adds r2, r5, #0
	movs r3, #1
	bl MenuSelection_SetupEntry
	ldr r5, [r5, #4]
	adds r6, #2
	cmp r5, #0
	bne .L_0803eeaa
.L_0803eebe:
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r7, r2
	movs r1, #192
	ldr r5, [r3]
	lsls r1, r1, #2
	adds r1, #150
	adds r3, r7, r1
	ldrh r1, [r3]
	ldr r3, [r5, #4]
	cmp r3, #0
	beq .L_0803eeea
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #244
.L_0803eedc:
	strh r1, [r5, #24]
	strh r2, [r5, #20]
	ldr r5, [r5, #4]
	adds r1, #16
	ldr r3, [r5, #4]
	cmp r3, #0
	bne .L_0803eedc
.L_0803eeea:
	movs r3, #1
	strh r3, [r7, #10]
	b .L_0803ef0c
.L_0803eef0:
	ldrh r2, [r6]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0803ef02
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	b .L_0803ef0a
.L_0803ef02:
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r1, r2
.L_0803ef0a:
	strh r3, [r6]
.L_0803ef0c:
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #162
	adds r2, r7, r3
	movs r1, #192
	movs r3, #1
	strh r3, [r2]
	lsls r1, r1, #2
	adds r1, #158
	adds r3, r7, r1
	ldrh r1, [r3]
	adds r0, r7, #0
	bl Menu_LoadSelectionNodeResource
	movs r0, #1
	bl WaitFrames
	movs r2, #210
	lsls r2, r2, #2
	adds r3, r7, r2
	ldr r3, [r3]
	movs r1, #0
	ldrh r0, [r3, #10]
	bl Menu_OpenSelectionWindow
	movs r0, #1
	bl WaitFrames
	pop {r5, r6, r7, pc}
	.2byte 0x0000
