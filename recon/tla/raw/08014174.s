.syntax unified
	.thumb
	.global ResourceTable_AllocateBlocks
	.thumb_func
ResourceTable_AllocateBlocks:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	lsrs r1, r1, #6
	cmp r5, #95
	bls .L_08014184
	movs r0, #1
	negs r0, r0
	b .L_080141e8
.L_08014184:
	ldr r7, .L_080141ec
	ldr r2, .L_080141f0
	movs r4, #0
	mov lr, r2
	adds r6, r7, #0
.L_0801418e:
	movs r3, #128
	movs r0, #1
	lsls r3, r3, #2
	negs r0, r0
	cmp r4, r3
	bge .L_080141e8
	ldrb r3, [r6, r4]
	cmp r3, #255
	bne .L_080141d8
	movs r2, #128
	movs r0, #1
	adds r3, r4, r1
	lsls r2, r2, #2
	negs r0, r0
	cmp r3, r2
	bhi .L_080141e8
	adds r0, r4, #0
	cmp r0, r3
	bcs .L_080141c6
	mov r12, r3
	adds r2, r0, r6
.L_080141b8:
	ldrb r3, [r2]
	adds r2, #1
	cmp r3, #255
	bne .L_080141d8
	adds r4, #1
	cmp r4, r12
	bcc .L_080141b8
.L_080141c6:
	movs r2, #0
	cmp r2, r1
	bcs .L_080141e6
.L_080141cc:
	adds r3, r0, r2
	adds r2, #1
	strb r5, [r7, r3]
	cmp r2, r1
	bcc .L_080141cc
	b .L_080141e6
.L_080141d8:
	ldrb r3, [r7, r4]
	mov r2, lr
	lsls r3, r3, #2
	ldrh r3, [r2, r3]
	lsrs r3, r3, #6
	adds r4, r4, r3
	b .L_0801418e
.L_080141e6:
	lsls r0, r0, #6
.L_080141e8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080141ec:
	.4byte ResourceBlockOwners
.L_080141f0:
	.4byte ResourceTableEntries
