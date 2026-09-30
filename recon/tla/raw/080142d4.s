.syntax unified
	.thumb
	.global VramBlock_LoadCached
	.thumb_func
VramBlock_LoadCached:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0801435c
	adds r5, r0, #0
	mov r8, r2
	lsls r2, r5, #2
	adds r6, r1, #0
	adds r7, r2, r3
	movs r0, #0
	cmp r5, #95
	bhi .L_08014354
	movs r2, #128
	lsls r2, r2, #6
	cmp r6, r2
	bhi .L_08014354
	ldrh r3, [r7]
	cmp r3, #16
	bls .L_0801430a
	cmp r3, r6
	beq .L_08014306
	adds r0, r5, #0
	bl Resource_ResetEntry
	b .L_0801430a
.L_08014306:
	ldrh r5, [r7, #2]
	b .L_08014314
.L_0801430a:
	adds r0, r5, #0
	adds r1, r6, #0
	bl Func_08014174
	adds r5, r0, #0
.L_08014314:
	movs r3, #1
	negs r3, r3
	cmp r5, r3
	beq .L_08014352
	ldr r2, .L_08014360
	strh r6, [r7]
	adds r1, r5, r2
	mov r2, r8
	strh r5, [r7, #2]
	cmp r2, #0
	beq .L_0801434e
	cmp r8, r3
	bne .L_0801433a
	adds r0, r1, #0
	ldr r3, .L_08014364
	adds r1, r6, #0
	mov lr, r3
	.2byte 0xf800
	b .L_0801434e
.L_0801433a:
	movs r4, #132
	movs r3, #128
	lsrs r2, r6, #2
	lsls r4, r4, #24
	lsls r3, r3, #19
	adds r3, #212
	mov r0, r8
	orrs r2, r4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
.L_0801434e:
	lsrs r0, r5, #5
	b .L_08014354
.L_08014352:
	movs r0, #0
.L_08014354:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0801435c:
	.4byte ResourceTableEntries
.L_08014360:
	.4byte 0x06010000
.L_08014364:
	.4byte IwramClearWords
