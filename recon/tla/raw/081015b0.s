.syntax unified
	.thumb
	.global Graphics_AdjustPaletteBank
	.thumb_func
Graphics_AdjustPaletteBank:
	push {r5, r6, r7, lr}
	movs r1, #0
	adds r5, r0, #0
	movs r3, #15
	mov r12, r1
	movs r7, #31
.L_081015bc:
	lsls r3, r3, #4
	movs r6, #0
	mov lr, r3
.L_081015c2:
	mov r2, lr
	adds r3, r2, r6
	movs r1, #160
	lsls r1, r1, #19
	lsls r0, r3, #1
	adds r3, r0, r1
	ldrh r3, [r3]
	adds r1, r7, #0
	lsrs r4, r3, #10
	ands r4, r7
	lsrs r2, r3, #5
	ands r2, r7
	ands r1, r3
	adds r4, r4, r5
	adds r2, r2, r5
	adds r1, r1, r5
	cmp r4, #31
	ble .L_081015e8
	movs r4, #31
.L_081015e8:
	cmp r2, #31
	ble .L_081015ee
	movs r2, #31
.L_081015ee:
	cmp r1, #31
	ble .L_081015f4
	movs r1, #31
.L_081015f4:
	cmp r4, #0
	bge .L_081015fa
	movs r4, #0
.L_081015fa:
	cmp r2, #0
	bge .L_08101600
	movs r2, #0
.L_08101600:
	cmp r1, #0
	bge .L_08101606
	movs r1, #0
.L_08101606:
	lsls r2, r2, #5
	lsls r3, r4, #10
	orrs r3, r2
	orrs r3, r1
	ldr r1, .L_08101634
	adds r6, #1
	adds r2, r0, r1
	strh r3, [r2]
	cmp r6, #15
	ble .L_081015c2
	mov r2, r12
	movs r3, #5
	cmp r2, #0
	beq .L_08101628
	movs r5, #12
	movs r3, #7
	negs r5, r5
.L_08101628:
	movs r1, #1
	add r12, r1
	mov r2, r12
	cmp r2, #2
	ble .L_081015bc
	pop {r5, r6, r7, pc}
.L_08101634:
	.4byte 0x04ffffe0
