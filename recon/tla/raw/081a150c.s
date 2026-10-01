.syntax unified
	.thumb
	.global Func_081a150c
	.thumb_func
Func_081a150c:
	push {r5, lr}
	ldr r3, .L_081a1578
	movs r2, #0
	strb r2, [r3]
	ldr r3, .L_081a157c
	strb r2, [r3]
	ldr r3, .L_081a1580
	strb r2, [r3]
	bl Scheduler_ResetTaskTable
	ldr r3, .L_081a1584
	movs r2, #0
	str r2, [r3]
	ldr r3, .L_081a1588
	ldr r1, .L_081a158c
	str r2, [r3]
	ldr r3, .L_081a1590
	strh r2, [r1]
	ldr r0, .L_081a1594
	str r2, [r3]
	ldr r1, .L_081a1598
	ldr r3, .L_081a1574
	strh r2, [r0]
	strh r3, [r1]
	ldr r3, .L_081a159c
	movs r1, #144
	strh r2, [r3]
	lsls r1, r1, #3
	ldr r0, .L_081a15a0
	bl Scheduler_AddOrUpdateCallback
	movs r3, #64
	movs r5, #128
	lsls r5, r5, #19
	strh r3, [r5]
	ldr r0, .L_081a15a4
	bl Func_081a0674
	ldr r0, .L_081a15a8
	bl Func_081a0674
	movs r0, #0
	bl Func_081a04d0
	movs r0, #1
	bl Func_081a04d0
	movs r2, #248
	movs r3, #128
	lsls r2, r2, #5
	lsls r3, r3, #19
	b .L_081a15ac
.L_081a1574:
	.4byte 0x00000001
.L_081a1578:
	.4byte Data_0300123c
.L_081a157c:
	.4byte Data_03001110
.L_081a1580:
	.4byte Data_03001200
.L_081a1584:
	.4byte Data_02007518
.L_081a1588:
	.4byte Data_0200751c
.L_081a158c:
	.4byte Data_02007520
.L_081a1590:
	.4byte Data_02007528
.L_081a1594:
	.4byte gScrollTarget
.L_081a1598:
	.4byte Data_02007524
.L_081a159c:
	.4byte Data_02007514
.L_081a15a0:
	.4byte Func_081a133c
.L_081a15a4:
	.4byte 0x06007800
.L_081a15a8:
	.4byte 0x0600f800
.L_081a15ac:
	adds r2, #138
	adds r3, #12
	strh r2, [r3]
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #131
	adds r3, #2
	strh r2, [r3]
	movs r3, #226
	lsls r3, r3, #5
	strh r3, [r5]
	movs r2, #160
	movs r3, #128
	lsls r2, r2, #6
	lsls r3, r3, #19
	adds r2, #68
	adds r3, #80
	strh r2, [r3]
	movs r2, #16
	adds r3, #2
	strh r2, [r3]
	bl Func_081a1830
	pop {r5, pc}
