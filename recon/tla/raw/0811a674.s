.syntax unified
	.thumb
	.global Func_0811a674
	.thumb_func
Func_0811a674:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Owner_GetState
	movs r1, #1
	adds r6, r0, #0
	adds r0, r5, #0
	bl Inventory_FindEquippedFar
	movs r5, #0
	cmp r0, #0
	blt .L_0811a6f6
	lsls r3, r0, #1
	adds r3, #216
	ldrh r3, [r6, r3]
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	ands r0, r3
	bl Func_0811a640
	movs r2, #165
	lsls r2, r2, #1
	adds r3, r6, r2
	ldrh r3, [r3]
	cmp r3, #7
	bhi .L_0811a6f6
	ldr r2, .L_0811a6fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0811a6b4:
	.4byte .L_0811a6d4
	.4byte .L_0811a6d8
	.4byte .L_0811a6dc
	.4byte .L_0811a6e0
	.4byte .L_0811a6e8
	.4byte .L_0811a6e4
	.4byte .L_0811a6ec
	.4byte .L_0811a6f0
.L_0811a6d4:
	ldr r2, .L_0811a700
	b .L_0811a6f2
.L_0811a6d8:
	ldr r2, .L_0811a704
	b .L_0811a6f2
.L_0811a6dc:
	ldr r2, .L_0811a708
	b .L_0811a6f2
.L_0811a6e0:
	ldr r2, .L_0811a70c
	b .L_0811a6f2
.L_0811a6e4:
	ldr r2, .L_0811a710
	b .L_0811a6f2
.L_0811a6e8:
	ldr r2, .L_0811a714
	b .L_0811a6f2
.L_0811a6ec:
	ldr r2, .L_0811a718
	b .L_0811a6f2
.L_0811a6f0:
	ldr r2, .L_0811a71c
.L_0811a6f2:
	lsls r3, r0, #1
	ldrh r5, [r2, r3]
.L_0811a6f6:
	adds r0, r5, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0811a6fc:
	.4byte .L_0811a6b4
.L_0811a700:
	.4byte Data_081287d4
.L_0811a704:
	.4byte Data_081287e2
.L_0811a708:
	.4byte Data_081287f0
.L_0811a70c:
	.4byte Data_081287fe
.L_0811a710:
	.4byte Data_0812880c
.L_0811a714:
	.4byte Data_0812881a
.L_0811a718:
	.4byte Data_08128828
.L_0811a71c:
	.4byte Data_08128836
