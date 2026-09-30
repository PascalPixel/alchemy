.syntax unified
	.thumb
	.global Func_0811d24c
	.thumb_func
Func_0811d24c:
	push {r5, r6, r7, lr}
	mov r7, r9
	push {r7}
	sub sp, #4
	mov r3, sp
	mov r2, r9
	str r2, [r3]
	adds r7, r2, #0
	subs r3, r7, #4
	ldr r0, [r3]
	movs r1, #20
	bl Func_0801680c
	movs r3, #1
	movs r5, #150
	negs r3, r3
	movs r6, #0
	lsls r5, r5, #1
	cmp r0, r3
	bne .L_0811d298
	b .L_0811d2ec
.L_0811d276:
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_0811d2da
	ldr r3, .L_0811d2f4
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0811d296
	adds r6, #1
	cmp r6, #24
	ble .L_0811d298
	b .L_0811d2da
.L_0811d296:
	movs r6, #0
.L_0811d298:
	bl Func_0801692c
	cmp r0, #0
	bne .L_0811d276
	adds r3, r7, #0
	subs r3, #8
	ldr r1, [r3]
	cmp r1, #0
	beq .L_0811d2ea
	subs r3, #4
	ldr r0, [r3]
	bl Func_0801680c
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_0811d2e2
	b .L_0811d2ec
.L_0811d2bc:
	movs r0, #1
	subs r5, #1
	bl WaitFrames
	cmp r5, #0
	blt .L_0811d2da
	ldr r3, .L_0811d2f4
	ldrh r2, [r3]
	movs r3, #3
	ands r3, r2
	cmp r3, #3
	beq .L_0811d2e0
	adds r6, #1
	cmp r6, #24
	ble .L_0811d2e2
.L_0811d2da:
	movs r0, #1
	negs r0, r0
	b .L_0811d2ec
.L_0811d2e0:
	movs r6, #0
.L_0811d2e2:
	bl Func_0801692c
	cmp r0, #0
	bne .L_0811d2bc
.L_0811d2ea:
	movs r0, #0
.L_0811d2ec:
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {r5, r6, r7, pc}
.L_0811d2f4:
	.4byte gLinkStatus
