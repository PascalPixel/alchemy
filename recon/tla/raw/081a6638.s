.syntax unified
	.thumb
	.global Func_081a6638
	.thumb_func
Func_081a6638:
	push {r5, r6, lr}
	ldr r3, .L_081a6658
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r0, .L_081a6660
	movs r6, #0
	movs r4, #0
.L_081a6648:
	adds r3, r4, #0
	subs r3, #9
	cmp r3, #1
	bhi .L_081a6682
	ldr r5, .L_081a665c
	movs r1, #0
	b .L_081a6664
	.2byte 0x0000
.L_081a6658:
	.4byte 0x00000000
.L_081a665c:
	.4byte 0x00002000
.L_081a6660:
	.4byte 0x0600e800
.L_081a6664:
	adds r2, r6, #0
	lsls r3, r2, #16
	movs r6, #128
	lsls r2, r2, #16
	lsls r6, r6, #9
	lsrs r2, r2, #16
	adds r3, r3, r6
	orrs r2, r5
	adds r1, #1
	strh r2, [r0]
	asrs r6, r3, #16
	adds r0, #2
	cmp r1, #31
	ble .L_081a6664
	b .L_081a6690
.L_081a6682:
	ldr r3, .L_081a66b4
	movs r1, #31
.L_081a6686:
	subs r1, #1
	strh r3, [r0]
	adds r0, #2
	cmp r1, #0
	bge .L_081a6686
.L_081a6690:
	adds r4, #1
	cmp r4, #19
	ble .L_081a6648
	ldr r3, .L_081a66b8
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #14
	strh r3, [r2]
	ldr r2, .L_081a66bc
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r3, .L_081a66c0
	movs r2, #0
	b .L_081a66c4
.L_081a66b4:
	.4byte 0x00002000
.L_081a66b8:
	.4byte 0x00001d0a
.L_081a66bc:
	.4byte 0x00000000
.L_081a66c0:
	.4byte Data_03001120
.L_081a66c4:
	movs r1, #3
.L_081a66c6:
	subs r1, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r1, #0
	bge .L_081a66c6
	movs r0, #0
	movs r1, #0
	bl Func_081a81f4
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_081a81d4
	ldr r3, .L_081a6718
	movs r2, #128
	lsls r2, r2, #19
	movs r0, #40
	strh r3, [r2]
	bl Func_081a8228
	movs r0, #150
	bl Func_081a6094
	cmp r0, #0
	bne .L_081a671c
	movs r0, #2
	movs r1, #0
	bl Func_081a81d4
	movs r0, #40
	bl Func_081a8228
	movs r0, #60
	bl Func_081a6094
	cmp r0, #0
	beq .L_081a6720
	b .L_081a671c
	.2byte 0x0000
.L_081a6718:
	.4byte 0x00000800
.L_081a671c:
	movs r0, #0
	b .L_081a6722
.L_081a6720:
	movs r0, #1
.L_081a6722:
	pop {r5, r6, pc}
