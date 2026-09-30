.syntax unified
	.thumb
	.global Func_081a6724
	.thumb_func
Func_081a6724:
	push {r5, r6, r7, lr}
	ldr r3, .L_081a6740
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r1, .L_081a6748
	movs r6, #64
	movs r0, #0
.L_081a6734:
	subs r3, r0, #7
	cmp r3, #2
	bhi .L_081a676a
	ldr r4, .L_081a6744
	movs r5, #0
	b .L_081a674c
.L_081a6740:
	.4byte 0x00000000
.L_081a6744:
	.4byte 0x00003000
.L_081a6748:
	.4byte 0x0600e800
.L_081a674c:
	adds r2, r6, #0
	lsls r3, r2, #16
	movs r6, #128
	lsls r2, r2, #16
	lsls r6, r6, #9
	lsrs r2, r2, #16
	adds r3, r3, r6
	orrs r2, r4
	adds r5, #1
	strh r2, [r1]
	asrs r6, r3, #16
	adds r1, #2
	cmp r5, #31
	ble .L_081a674c
	b .L_081a6796
.L_081a676a:
	adds r3, r0, #0
	subs r3, #10
	cmp r3, #1
	bhi .L_081a6788
	ldr r3, .L_081a6784
	movs r5, #0
.L_081a6776:
	adds r5, #1
	strh r3, [r1]
	adds r1, #2
	cmp r5, #31
	ble .L_081a6776
	b .L_081a6796
	.2byte 0x0000
.L_081a6784:
	.4byte 0x00003060
.L_081a6788:
	ldr r3, .L_081a67b0
	movs r5, #31
.L_081a678c:
	subs r5, #1
	strh r3, [r1]
	adds r1, #2
	cmp r5, #0
	bge .L_081a678c
.L_081a6796:
	adds r0, #1
	cmp r0, #19
	ble .L_081a6734
	ldr r1, .L_081a67b8
	movs r6, #160
	movs r0, #0
.L_081a67a2:
	adds r3, r0, #0
	subs r3, #10
	cmp r3, #1
	bhi .L_081a67da
	ldr r4, .L_081a67b4
	movs r5, #0
	b .L_081a67bc
.L_081a67b0:
	.4byte 0x00003040
.L_081a67b4:
	.4byte 0x00003000
.L_081a67b8:
	.4byte 0x0600e000
.L_081a67bc:
	adds r2, r6, #0
	lsls r3, r2, #16
	movs r6, #128
	lsls r2, r2, #16
	lsls r6, r6, #9
	lsrs r2, r2, #16
	adds r3, r3, r6
	orrs r2, r4
	adds r5, #1
	strh r2, [r1]
	asrs r6, r3, #16
	adds r1, #2
	cmp r5, #31
	ble .L_081a67bc
	b .L_081a67e8
.L_081a67da:
	ldr r3, .L_081a6808
	movs r5, #31
.L_081a67de:
	subs r5, #1
	strh r3, [r1]
	adds r1, #2
	cmp r5, #0
	bge .L_081a67de
.L_081a67e8:
	adds r0, #1
	cmp r0, #19
	ble .L_081a67a2
	ldr r3, .L_081a680c
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #14
	strh r3, [r2]
	ldr r3, .L_081a6810
	subs r2, #6
	strh r3, [r2]
	ldr r3, .L_081a6814
	adds r2, #72
	strh r3, [r2]
	ldr r3, .L_081a6818
	b .L_081a681c
.L_081a6808:
	.4byte 0x00003040
.L_081a680c:
	.4byte 0x00001d0a
.L_081a6810:
	.4byte 0x00001c09
.L_081a6814:
	.4byte 0x00000841
.L_081a6818:
	.4byte 0x00001000
.L_081a681c:
	adds r2, #2
	strh r3, [r2]
	ldr r3, .L_081a6870
	movs r2, #0
	movs r5, #3
.L_081a6826:
	subs r5, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r5, #0
	bge .L_081a6826
	movs r0, #0
	movs r1, #0
	bl Func_081a81f4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_081a81d4
	ldr r3, .L_081a6868
	movs r2, #128
	lsls r2, r2, #19
	movs r0, #40
	strh r3, [r2]
	bl Func_081a8228
	movs r0, #60
	bl Func_081a6094
	cmp r0, #0
	bne .L_081a68ac
	movs r7, #128
	ldr r6, .L_081a686c
	lsls r7, r7, #19
	movs r5, #1
	adds r7, #82
	b .L_081a6874
.L_081a6868:
	.4byte 0x00000900
.L_081a686c:
	.4byte 0x00001000
.L_081a6870:
	.4byte Data_03001120
.L_081a6874:
	adds r3, r5, #0
	orrs r3, r6
	strh r3, [r7]
	movs r0, #3
	bl Func_081a6094
	cmp r0, #0
	bne .L_081a68ac
	adds r5, #1
	cmp r5, #16
	ble .L_081a6874
	movs r0, #150
	bl Func_081a6094
	cmp r0, #0
	bne .L_081a68ac
	movs r0, #2
	movs r1, #0
	bl Func_081a81d4
	movs r0, #40
	bl Func_081a8228
	movs r0, #60
	bl Func_081a6094
	cmp r0, #0
	beq .L_081a68b0
.L_081a68ac:
	movs r0, #0
	b .L_081a68b2
.L_081a68b0:
	movs r0, #1
.L_081a68b2:
	pop {r5, r6, r7, pc}
