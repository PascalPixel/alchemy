.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.set sub_02000574, 0x02000574
	.set sub_02000580, 0x02000580
	.set sub_02000582, 0x02000582
	.set sub_0200059a, 0x0200059a
	.set sub_020005c0, 0x020005c0
	.set sub_020005dc, 0x020005dc
	.set sub_020005e0, 0x020005e0
	.set sub_020005e2, 0x020005e2
	.set sub_020005e4, 0x020005e4
	.set sub_020005fa, 0x020005fa
	.set sub_020005fc, 0x020005fc
	.set sub_02000602, 0x02000602
	.set sub_02000620, 0x02000620
	.set sub_0200063c, 0x0200063c
	.set sub_02000644, 0x02000644
	.set sub_02000648, 0x02000648
	.set sub_0200064a, 0x0200064a
	.set sub_0200065c, 0x0200065c
	.set sub_02000662, 0x02000662
	.set sub_02000674, 0x02000674
	.set sub_020006a4, 0x020006a4
	.set sub_020006a6, 0x020006a6
	.set sub_020006ac, 0x020006ac
	.set sub_020006c4, 0x020006c4
	.set sub_020006ca, 0x020006ca
	.set sub_020006d0, 0x020006d0
	.set sub_020006dc, 0x020006dc
	.set sub_020006e8, 0x020006e8
	.set sub_020006f0, 0x020006f0
	.set sub_020006f4, 0x020006f4
	.set sub_020006fc, 0x020006fc
	.set sub_02000702, 0x02000702
	.set sub_02000704, 0x02000704
	.set sub_02000714, 0x02000714
	.set sub_02000724, 0x02000724
	.set sub_0200073a, 0x0200073a
	.set sub_02000744, 0x02000744
	.set sub_02000746, 0x02000746
	.set sub_02000748, 0x02000748
	.set sub_0200075c, 0x0200075c
	.set sub_02000760, 0x02000760
	.set sub_02000764, 0x02000764
	.set sub_0200076c, 0x0200076c
	.set sub_0200077a, 0x0200077a
	.set sub_02000790, 0x02000790
	.set sub_020007aa, 0x020007aa
	.set sub_020007ac, 0x020007ac
	.set sub_020007b4, 0x020007b4
	.set sub_020007c6, 0x020007c6
	.set sub_020007ca, 0x020007ca
	.set sub_020007d0, 0x020007d0
	.set sub_020007ea, 0x020007ea
	.set sub_020007f0, 0x020007f0
	.set sub_020007f6, 0x020007f6
	.set sub_0200080a, 0x0200080a
	.set sub_0200080e, 0x0200080e
	.set sub_02000818, 0x02000818
	.set sub_0200081a, 0x0200081a
	.set sub_0200082e, 0x0200082e
	.set sub_02000832, 0x02000832
	.set sub_02000836, 0x02000836
	.set sub_02000856, 0x02000856
	.set sub_02000860, 0x02000860
	.set sub_02000868, 0x02000868
	.set sub_02000874, 0x02000874
	.set sub_02000878, 0x02000878
	.set sub_02000884, 0x02000884
	.set sub_0200089c, 0x0200089c
	.set sub_020008a2, 0x020008a2
	.set sub_020008b6, 0x020008b6
	.set sub_020008c2, 0x020008c2
	.set sub_020008c4, 0x020008c4
	.set sub_020008d6, 0x020008d6
	.set sub_020008da, 0x020008da
	.set sub_020008ee, 0x020008ee
	.set sub_020008fe, 0x020008fe
	.set sub_02000904, 0x02000904
	.set sub_0200090e, 0x0200090e
	.set sub_02000910, 0x02000910
	.set sub_0200091c, 0x0200091c
	.set sub_02000920, 0x02000920
	.set sub_02000934, 0x02000934
	.set sub_02000946, 0x02000946
	.set sub_0200094c, 0x0200094c
	.set sub_0200094e, 0x0200094e
	.set sub_02000958, 0x02000958
	.set sub_02000966, 0x02000966
	.set sub_02000978, 0x02000978
	.set sub_02000984, 0x02000984
	.set sub_0200098a, 0x0200098a
	.set sub_0200098c, 0x0200098c
	.set sub_02000994, 0x02000994
	.set sub_0200099a, 0x0200099a
	.set sub_020009a2, 0x020009a2
	.set sub_020009a6, 0x020009a6
	.set sub_020009ac, 0x020009ac
	.set sub_020009b4, 0x020009b4
	.set sub_020009ca, 0x020009ca
	.set sub_020009d8, 0x020009d8
	.set sub_020009f2, 0x020009f2
	.set sub_020009f4, 0x020009f4
	.set sub_020009fc, 0x020009fc
	.set sub_02000a14, 0x02000a14
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008598
	.global Func_02000038
	.thumb_func
Func_02000038:
	movs r0, #0
	bx lr
	.global Func_0200003c
	.thumb_func
Func_0200003c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008688
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {r5, lr}
	ldr r5, [pc, #16]
	adds r0, r5, #0
	bl sub_02000574
	adds r0, r5, #0
	pop {r5}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x020086b0
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	push {r5, lr}
	movs r0, #0
	bl sub_0200059a
	ldrh r5, [r0, #6]
	bl sub_02000580
	ldr r3, [pc, #60]
	adds r5, r5, r3
	ldr r3, [pc, #60]
	cmp r5, r3
	bhi .L_0200005c_0
	movs r0, #7
	movs r1, #16
	bl sub_02000602
	b .L_0200005c_1
.L_0200005c_0:
	ldr r0, [pc, #48]
	bl sub_02000582
	cmp r0, #0
	bne .L_0200005c_2
	ldr r0, [pc, #40]
	bl sub_020005dc
	b .L_0200005c_3
.L_0200005c_2:
	ldr r0, [pc, #36]
	bl sub_020005e4
.L_0200005c_3:
	movs r0, #16
	movs r1, #0
	bl sub_020005fc
.L_0200005c_1:
	bl sub_020005c0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000845
	.4byte 0x000013e3
	.4byte 0x000016f5
	.global Func_020000bc
	.thumb_func
Func_020000bc:
	push {r5, lr}
	movs r0, #0
	bl sub_020005fa
	ldrh r5, [r0, #6]
	bl sub_020005e0
	ldr r3, [pc, #60]
	adds r5, r5, r3
	ldr r3, [pc, #60]
	cmp r5, r3
	bhi .L_020000bc_0
	movs r0, #9
	movs r1, #18
	bl sub_02000662
	b .L_020000bc_1
.L_020000bc_0:
	ldr r0, [pc, #48]
	bl sub_020005e2
	cmp r0, #0
	bne .L_020000bc_2
	ldr r0, [pc, #40]
	bl sub_0200063c
	b .L_020000bc_3
.L_020000bc_2:
	ldr r0, [pc, #36]
	bl sub_02000644
.L_020000bc_3:
	movs r0, #18
	movs r1, #0
	bl sub_0200065c
.L_020000bc_1:
	bl sub_02000620
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000845
	.4byte 0x000013e9
	.4byte 0x000016f9
	.global Func_0200011c
	.thumb_func
Func_0200011c:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x020088f0
	.global Func_02000124
	.thumb_func
Func_02000124:
	push {r5, lr}
	movs r0, #0
	bl sub_02000662
	ldrh r5, [r0, #6]
	bl sub_02000648
	ldr r3, [pc, #96]
	adds r5, r5, r3
	ldr r3, [pc, #96]
	cmp r5, r3
	bhi .L_02000124_0
	movs r0, #8
	movs r1, #17
	bl sub_020006ca
	b .L_02000124_1
.L_02000124_0:
	ldr r0, [pc, #84]
	bl sub_0200064a
	cmp r0, #0
	bne .L_02000124_2
	ldr r0, [pc, #76]
	bl sub_020006a4
	movs r2, #0
	movs r1, #0
	movs r0, #17
	bl sub_020006a6
	movs r0, #10
	bl sub_02000674
	movs r1, #0
	movs r0, #17
	bl sub_020006dc
	movs r1, #192
	movs r0, #17
	lsls r1, r1, #6
	movs r2, #10
	bl sub_020006f0
	b .L_02000124_1
.L_02000124_2:
	ldr r0, [pc, #36]
	bl sub_020006d0
	movs r0, #17
	movs r1, #0
	bl sub_020006e8
.L_02000124_1:
	bl sub_020006ac
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000845
	.4byte 0x000013e5
	.4byte 0x000016f7
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	push {lr}
	bl sub_020006c4
	ldr r0, [pc, #44]
	bl sub_02000702
	movs r2, #0
	movs r0, #21
	movs r1, #0
	bl sub_02000704
	movs r0, #21
	movs r1, #0
	bl sub_02000724
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #10
	bl sub_02000748
	bl sub_020006f4
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000013ed
	.global Func_020001e0
	.thumb_func
Func_020001e0:
	push {lr}
	bl sub_020006fc
	ldr r0, [pc, #96]
	bl sub_0200073a
	movs r0, #24
	movs r1, #0
	movs r2, #20
	bl sub_0200075c
	movs r1, #0
	movs r2, #0
	movs r0, #24
	bl sub_02000746
	movs r0, #10
	bl sub_02000714
	movs r1, #0
	movs r0, #24
	bl sub_02000764
	movs r0, #0
	movs r1, #0
	bl sub_02000744
	cmp r0, #0
	beq .L_020001e0_0
	ldr r3, [pc, #48]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020001e0_0:
	movs r0, #24
	movs r1, #0
	bl sub_02000790
	movs r1, #128
	movs r0, #24
	lsls r1, r1, #7
	movs r2, #10
	bl sub_020007b4
	bl sub_02000760
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000013f0
	.4byte 0x03001ebc
	.global Func_02000250
	.thumb_func
Func_02000250:
	push {lr}
	bl sub_0200076c
	ldr r0, [pc, #84]
	bl sub_020007aa
	movs r1, #0
	movs r2, #0
	movs r0, #27
	bl sub_020007ac
	movs r0, #10
	bl sub_0200077a
	movs r1, #0
	movs r0, #27
	bl sub_020007ca
	movs r0, #0
	movs r1, #0
	bl sub_020007aa
	cmp r0, #0
	beq .L_02000250_0
	ldr r3, [pc, #44]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_02000250_0:
	movs r0, #27
	movs r1, #0
	bl sub_020007f6
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #7
	movs r2, #10
	bl sub_0200081a
	bl sub_020007c6
	pop {r0}
	bx r0
	.4byte 0x000013f6
	.4byte 0x03001ebc
	.global Func_020002b4
	.thumb_func
Func_020002b4:
	push {lr}
	bl sub_020007d0
	ldr r0, [pc, #20]
	bl sub_0200080e
	movs r1, #0
	movs r0, #8
	bl sub_02000836
	bl sub_020007ea
	pop {r0}
	bx r0
	.4byte 0x000016e1
	.global Func_020002d4
	.thumb_func
Func_020002d4:
	push {lr}
	bl sub_020007f0
	ldr r0, [pc, #20]
	bl sub_0200082e
	movs r1, #0
	movs r0, #13
	bl sub_02000856
	bl sub_0200080a
	pop {r0}
	bx r0
	.4byte 0x000016ec
	.global Func_020002f4
	.thumb_func
Func_020002f4:
	push {r5, lr}
	movs r0, #0
	bl sub_02000832
	ldrh r5, [r0, #6]
	bl sub_02000818
	ldr r3, [pc, #68]
	adds r5, r5, r3
	ldr r3, [pc, #68]
	cmp r5, r3
	bhi .L_020002f4_0
	movs r0, #2
	movs r1, #19
	bl sub_020008a2
	b .L_020002f4_1
.L_020002f4_0:
	ldr r0, [pc, #56]
	bl sub_0200081a
	cmp r0, #0
	beq .L_020002f4_2
	ldr r0, [pc, #48]
	bl sub_02000874
	movs r0, #19
	movs r1, #0
	bl sub_0200089c
	b .L_020002f4_1
.L_020002f4_2:
	ldr r0, [pc, #36]
	bl sub_02000884
	movs r0, #19
	movs r1, #0
	bl sub_0200089c
.L_020002f4_1:
	bl sub_02000860
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0xffff5fff
	.4byte 0x00003ffe
	.4byte 0x00000845
	.4byte 0x000016fb
	.4byte 0x000013eb
	.global Func_0200035c
	.thumb_func
Func_0200035c:
	push {lr}
	bl sub_02000878
	movs r0, #192
	lsls r0, r0, #2
	bl sub_02000868
	cmp r0, #0
	bne .L_0200035c_0
	ldr r0, [pc, #124]
	bl sub_020008c2
	movs r0, #21
	movs r1, #0
	bl sub_020008da
	movs r1, #128
	movs r2, #20
	movs r0, #21
	lsls r1, r1, #8
	bl sub_020008fe
	movs r0, #21
	movs r1, #0
	bl sub_020008ee
	movs r0, #22
	movs r1, #2
	bl sub_020008d6
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #22
	bl sub_02000920
	movs r0, #60
	bl sub_020008b6
	movs r0, #22
	movs r1, #0
	bl sub_0200090e
	movs r0, #10
	bl sub_020008c4
	movs r0, #192
	lsls r0, r0, #2
	bl sub_020008c4
.L_0200035c_0:
	movs r2, #0
	movs r1, #0
	movs r0, #21
	bl sub_0200090e
	ldr r0, [pc, #36]
	bl sub_0200091c
	movs r0, #21
	movs r1, #0
	bl sub_02000934
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #10
	bl sub_02000958
	bl sub_02000904
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000016ff
	.4byte 0x00001702
	.global Func_020003f4
	.thumb_func
Func_020003f4:
	push {lr}
	bl sub_02000910
	ldr r0, [pc, #48]
	bl sub_0200094e
	movs r0, #22
	movs r1, #0
	bl sub_02000966
	movs r2, #0
	movs r0, #22
	movs r1, #0
	bl sub_02000958
	movs r0, #22
	movs r1, #0
	bl sub_02000978
	movs r0, #22
	movs r1, #0
	movs r2, #10
	bl sub_0200099a
	bl sub_02000946
	pop {r0}
	bx r0
	.4byte 0x00001703
	.global Func_02000430
	.thumb_func
Func_02000430:
	push {lr}
	bl sub_0200094c
	ldr r0, [pc, #52]
	bl sub_0200098a
	movs r0, #23
	movs r1, #0
	bl sub_020009a2
	movs r2, #0
	movs r0, #23
	movs r1, #0
	bl sub_02000994
	movs r0, #23
	movs r1, #0
	bl sub_020009b4
	movs r1, #192
	movs r0, #23
	lsls r1, r1, #8
	movs r2, #10
	bl sub_020009d8
	bl sub_02000984
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00001705
	.global Func_02000470
	.thumb_func
Func_02000470:
	push {lr}
	bl sub_0200098c
	ldr r0, [pc, #20]
	bl sub_020009ca
	movs r1, #0
	movs r0, #27
	bl sub_020009f2
	bl sub_020009a6
	pop {r0}
	bx r0
	.4byte 0x0000170a
	.global Func_02000490
	.thumb_func
Func_02000490:
	push {lr}
	bl sub_020009ac
	movs r0, #3
	bl sub_0200099a
	cmp r0, #0
	beq .L_02000490_0
	ldr r0, [pc, #28]
	bl sub_020009f4
	b .L_02000490_1
.L_02000490_0:
	ldr r0, [pc, #24]
	bl sub_020009fc
.L_02000490_1:
	movs r0, #10
	movs r1, #0
	bl sub_02000a14
	bl sub_020009d8
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000146f
	.4byte 0x000013d9
	.global Func_020004c8
	.thumb_func
Func_020004c8:
	push {lr}
	ldr r3, [pc, #48]
	movs r2, #224
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #73
	str r2, [r3]
	movs r0, #27
	bl sub_02000a14
	adds r2, r0, #0
	adds r2, #35
	movs r3, #0
	strb r3, [r2]
	ldr r1, [r0, #80]
	ldrb r2, [r1, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #8
	orrs r3, r2
	strb r3, [r1, #9]
	movs r0, #0
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.include "games/THE BROKEN SEAL/SRC/FIELD/BIRIBINO_HEYA/IMPORT.INC"
	.4byte 0xffff0000
	.4byte 0x000000b0
	.4byte 0xc00000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000b0
	.4byte 0xc00000d8
	.4byte 0x00180000
	.4byte 0x01080020
	.4byte 0x000000f0
	.4byte 0xffff0002
	.4byte 0x00000190
	.4byte 0xc0000108
	.4byte 0x01000000
	.4byte 0x01f00000
	.4byte 0x00000120
	.4byte 0xffff0003
	.4byte 0x000002b0
	.4byte 0xc0000108
	.4byte 0x02200000
	.4byte 0x03100000
	.4byte 0x00000120
	.4byte 0xffff0004
	.4byte 0x000000f0
	.4byte 0xc00001f8
	.4byte 0x00480000
	.4byte 0x01380150
	.4byte 0x00000210
	.4byte 0xffff0005
	.4byte 0x00000240
	.4byte 0xc0000218
	.4byte 0x01900000
	.4byte 0x02800130
	.4byte 0x00000228
	.4byte 0xffff0006
	.4byte 0x00000160
	.4byte 0xc0000368
	.4byte 0x00000000
	.4byte 0x01900280
	.4byte 0x00000380
	.4byte 0xffff0007
	.4byte 0x00000290
	.4byte 0x800002b8
	.4byte 0x01b00000
	.4byte 0x03500280
	.4byte 0x00000380
	.4byte 0xffff0008
	.4byte 0x00000158
	.4byte 0x000002b8
	.4byte 0x00000000
	.4byte 0x01900280
	.4byte 0x00000380
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001f
	.4byte 0x0010401e
	.4byte 0x0020501e
	.4byte 0x0030601e
	.4byte 0x0040801e
	.4byte 0x0050701e
	.4byte 0x0060301e
	.4byte 0x0070801f
	.4byte 0x0080701f
	.4byte 0x000001ff
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0x0000006a
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x00000065
	.4byte 0x00000001
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x00c00000
	.4byte 0x00004000
	.4byte 0x0000006b
	.4byte 0x00000002
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0x00000073
	.4byte 0x00000001
	.4byte 0x02800000
	.4byte 0x00000000
	.4byte 0x009c0000
	.4byte 0x00004000
	.4byte 0x00000066
	.4byte 0x00000002
	.4byte 0x02c00000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00004000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x01d80000
	.4byte 0x00004000
	.4byte 0x0000006f
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00014000
	.4byte 0x0000007c
	.4byte 0x00000001
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00013000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01b80000
	.4byte 0x00013000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x01c80000
	.4byte 0x00014000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00014000
	.4byte 0x00000080
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00004000
	.4byte 0x00000084
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x0002c000
	.4byte 0x00000084
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00020000
	.4byte 0x00000094
	.4byte 0x00000001
	.4byte 0x00480000
	.4byte 0x00000000
	.4byte 0x03080000
	.4byte 0x0002e000
	.4byte 0x000000ab
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00024000
	.4byte 0x000000ab
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x0001c000
	.4byte 0x00000074
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x03400000
	.4byte 0x0002a000
	.4byte 0x00000087
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x031c0000
	.4byte 0x00014000
	.4byte 0x00000067
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00018000
	.4byte 0x00000090
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x03060000
	.4byte 0x00004000
	.4byte 0x00000081
	.4byte 0x00000001
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03000000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x08450008
	.4byte 0x000013d3
	.4byte 0x00000000
	.4byte 0x08450009
	.4byte 0x000013d4
	.4byte 0x00000000
	.4byte 0x0845000a
	.4byte 0x000013d7
	.4byte 0x00000000
	.4byte 0x0845000b
	.4byte 0x000013d8
	.4byte 0x00000000
	.4byte 0x0845000c
	.4byte 0x000013db
	.4byte 0x00000000
	.4byte 0x0845000d
	.4byte 0x000013dc
	.4byte 0x00000000
	.4byte 0x0845000e
	.4byte 0x000013df
	.4byte 0x00000000
	.4byte 0x0845000f
	.4byte 0x000013e0
	.4byte 0x00000000
	.4byte 0x08450010
	.4byte 0x0200805d
	.4byte 0x00000000
	.4byte 0x08450011
	.4byte 0x02008125
	.4byte 0x00000000
	.4byte 0x08450012
	.4byte 0x020080bd
	.4byte 0x00000000
	.4byte 0x08450013
	.4byte 0x020082f5
	.4byte 0x00000000
	.4byte 0x08450014
	.4byte 0x000013ec
	.4byte 0x00000000
	.4byte 0x08450015
	.4byte 0x020081a9
	.4byte 0x00000000
	.4byte 0x08450016
	.4byte 0x000013ee
	.4byte 0x00000000
	.4byte 0x08450017
	.4byte 0x000013ef
	.4byte 0x00000000
	.4byte 0x08450018
	.4byte 0x020081e1
	.4byte 0x00000000
	.4byte 0x08450019
	.4byte 0x000013f4
	.4byte 0x00000000
	.4byte 0x0845001a
	.4byte 0x000013f5
	.4byte 0x00000000
	.4byte 0x0845001b
	.4byte 0x02008251
	.4byte 0x00000000
	.4byte 0x0845001c
	.4byte 0x000013f9
	.4byte 0x00000000
	.4byte 0x0845001d
	.4byte 0x000013fa
	.4byte 0x00000000
	.4byte 0x0845001e
	.4byte 0x000013fb
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082b5
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x000016e4
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x000016e7
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x000016e8
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x000016eb
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x020082d5
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x000016f1
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x000016f2
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x0200805d
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008125
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020080bd
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020082f5
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x000016fe
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x0200835d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x020083f5
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x02008431
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x00001707
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte 0x00001708
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte 0x00001709
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte 0x02008471
	.4byte 0x00000000
	.4byte 0xffff001c
	.4byte 0x0000170d
	.4byte 0x00000000
	.4byte 0xffff001d
	.4byte 0x0000170e
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x0000170f
	.4byte 0x00008d15
	.4byte 0x08450008
	.4byte 0x000013d5
	.4byte 0x00008d15
	.4byte 0x08450009
	.4byte 0x000013d6
	.4byte 0x00008d15
	.4byte 0x0845000a
	.4byte 0x02008491
	.4byte 0x00008d15
	.4byte 0x0845000b
	.4byte 0x000013da
	.4byte 0x00008d15
	.4byte 0x0845000c
	.4byte 0x000013dd
	.4byte 0x00008d15
	.4byte 0x0845000d
	.4byte 0x000013de
	.4byte 0x00008d15
	.4byte 0x0845000e
	.4byte 0x000013e1
	.4byte 0x00008d15
	.4byte 0x0845000f
	.4byte 0x000013e2
	.4byte 0x00008d15
	.4byte 0x08450010
	.4byte 0x000013e4
	.4byte 0x00008d15
	.4byte 0x08450011
	.4byte 0x000013e8
	.4byte 0x00008d15
	.4byte 0x08450012
	.4byte 0x000013ea
	.4byte 0x00008d15
	.4byte 0x08450013
	.4byte 0x000013fc
	.4byte 0x00008d15
	.4byte 0x08450014
	.4byte 0x000013fd
	.4byte 0x00008d15
	.4byte 0x08450015
	.4byte 0x000013fe
	.4byte 0x00008d15
	.4byte 0x08450016
	.4byte 0x000013ff
	.4byte 0x00008d15
	.4byte 0x08450017
	.4byte 0x00001400
	.4byte 0x00008d15
	.4byte 0x08450018
	.4byte 0x00001401
	.4byte 0x00008d15
	.4byte 0x08450019
	.4byte 0x00001402
	.4byte 0x00008d15
	.4byte 0x0845001a
	.4byte 0x00001403
	.4byte 0x00008d15
	.4byte 0x0845001b
	.4byte 0x00001404
	.4byte 0x00008d15
	.4byte 0x0845001c
	.4byte 0x00001405
	.4byte 0x00008d15
	.4byte 0x0845001d
	.4byte 0x00001406
	.4byte 0x00008d15
	.4byte 0x0845001e
	.4byte 0x00001407
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x000016e5
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x000016e6
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000016e9
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x000016ea
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x000016ef
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x000016f0
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x000016f3
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x000016f4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x000016f6
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x000016f8
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x000016fa
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001710
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001711
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001712
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001713
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001714
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001715
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte 0x00001716
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001717
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte 0x00001718
	.4byte 0x00008d15
	.4byte 0xffff001c
	.4byte 0x00001719
	.4byte 0x00008d15
	.4byte 0xffff001d
	.4byte 0x0000171a
	.4byte 0x00008d15
	.4byte 0xffff001e
	.4byte 0x0000171b
	.4byte 0x00000033
	.4byte 0x0f520064
	.4byte 0x001000bb
	.4byte 0x000000e3
	.4byte 0x0f530065
	.4byte 0x001000e3
	.4byte 0x00000033
	.4byte 0x0f540066
	.4byte 0x001000c2
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x00402996
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x00402997
	.4byte 0x000000f3
	.4byte 0xffff00ca
	.4byte 0x00402998
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
