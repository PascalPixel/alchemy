.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/ENTRY.INC"
	.section .text.x020089dc,"ax",%progbits
	.global Func_020009dc
	.thumb_func
Func_020009dc:
	push {lr}
	movs r0, #15
	movs r1, #0
	movs r2, #6
	bl 0x0200a5f0
	pop {r0}
	bx r0
	.global Func_020009ec
	.thumb_func
Func_020009ec:
	bx lr
	.2byte 0x0000
	.global Func_020009f0
	.thumb_func
Func_020009f0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_020009f0_0
	ldr r0, [pc, #36]
	b .L_020009f0_1
.L_020009f0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020009f0_2
	ldr r0, [pc, #36]
	b .L_020009f0_1
.L_020009f0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_020009f0_3
	ldr r0, [pc, #32]
	b .L_020009f0_1
.L_020009f0_3:
	ldr r0, [pc, #32]
.L_020009f0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000075
	.4byte 0x0200a898
	.4byte 0x00000076
	.4byte 0x0200a8e0
	.4byte 0x00000078
	.4byte 0x0200a928
	.4byte 0x0200a868
	.global Func_02000a44
	.thumb_func
Func_02000a44:
	movs r0, #0
	bx lr
	.global Func_02000a48
	.thumb_func
Func_02000a48:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x0200a970
	.global Func_02000a50
	.thumb_func
Func_02000a50:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000a50_0
	ldr r0, [pc, #36]
	b 0x02008a7e
.L_02000a50_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne 0x02008a72
.L_02000a6e:
	ldr r0, [pc, #36]
	b .L_02000a6e_0
	.2byte 0x4b09
	.2byte 0x429a
	.2byte 0xd101
	.2byte 0x4808
	.2byte 0xe000
	.2byte 0x4808
.L_02000a6e_0:
	pop {r1}
.L_02000a80:
	bx r1
	.2byte 0x0000
	.2byte 0x0240
	.2byte 0x0200
	.2byte 0x0075
	.2byte 0x0000
	.2byte 0xa9b0
	.2byte 0x0200
	.2byte 0x0076
	.2byte 0x0000
	.2byte 0xaa40
	.2byte 0x0200
	.2byte 0x0078
	.2byte 0x0000
	.2byte 0xaad0
	.2byte 0x0200
	.2byte 0xa998
	.2byte 0x0200
	.global Func_02000aa4
	.thumb_func
Func_02000aa4:
	bx lr
	.2byte 0x0000
	.global Func_02000aa8
	.thumb_func
Func_02000aa8:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	bl 0x0200a540
	adds r2, r0, #0
	ldr r3, [r5, #16]
	ldr r0, [r2, #16]
	ldr r1, [r2, #8]
	subs r0, r0, r3
	ldr r3, [r5, #8]
	subs r1, r1, r3
	bl 0x0200a480
	strh r0, [r5, #6]
	movs r0, #0
	pop {r5}
	pop {r1}
.L_02000acc:
	bx r1
	.2byte 0x0000
	.global Func_02000ad0
	.thumb_func
Func_02000ad0:
	push {lr}
	ldr r0, [pc, #144]
	bl 0x0200a508
	cmp r0, #0
	bne .L_02000ad0_0
	ldr r0, [pc, #132]
	bl 0x0200a510
	bl 0x0200a528
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200a5b0
	movs r0, #15
	movs r1, #1
	bl 0x0200a5c0
	bl 0x0200a5b8
	movs r2, #20
	movs r0, #15
	movs r1, #0
	bl 0x0200a598
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl 0x0200a5a8
	movs r1, #2
	movs r0, #15
	bl 0x0200a588
	movs r0, #20
	bl 0x0200a520
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #15
	bl 0x0200a548
	movs r0, #152
	bl 0x0200a600
	movs r0, #15
	bl 0x0200a540
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #146
	movs r2, #170
	str r3, [r0, #40]
	lsls r1, r1, #2
	movs r0, #15
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #7
	movs r2, #20
	bl 0x0200a598
	bl 0x0200a530
.L_02000ad0_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000009c8
	.global Func_02000b68
	.thumb_func
Func_02000b68:
	push {r5, r6, lr}
	ldr r0, [pc, #336]
	bl 0x0200a508
	cmp r0, #0
	bne .L_02000b68_0
	b 0x02008cb4
.L_02000b68_0:
	ldr r0, [pc, #328]
	bl 0x0200a508
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02000b68_1
	b 0x02008cb4
.L_02000b68_1:
	ldr r0, [pc, #312]
	bl 0x0200a510
	bl 0x0200a528
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #10
	lsls r1, r1, #7
	bl 0x0200a5b0
	movs r0, #15
	movs r1, #1
	bl 0x0200a5c0
	bl 0x0200a5b8
	movs r1, #128
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #7
	bl 0x0200a598
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl 0x0200a5a8
	movs r1, #2
	movs r0, #15
	bl 0x0200a588
	movs r0, #20
	bl 0x0200a520
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #15
	bl 0x0200a548
	movs r0, #152
	bl 0x0200a600
	movs r0, #15
	bl 0x0200a540
	movs r3, #160
	lsls r3, r3, #12
	movs r1, #146
	movs r2, #166
	str r3, [r0, #40]
	lsls r1, r1, #2
.L_02000bf0:
	movs r0, #15
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #128
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #7
	bl 0x0200a598
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #15
	bl 0x0200a5a8
	movs r0, #30
	bl 0x0200a520
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #12
	lsls r2, r2, #7
	bl 0x0200a548
	movs r1, #166
	movs r2, #166
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #186
	movs r2, #166
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #206
	movs r2, #166
	lsls r1, r1, #2
	lsls r2, r2, #2
	movs r0, #15
	bl 0x0200a560
	movs r0, #10
	bl 0x0200a520
	movs r0, #208
	bl 0x0200a600
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	lsls r0, r0, #11
	bl 0x0200a4e8
	movs r0, #20
	bl 0x0200a520
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #76]
	negs r0, r0
	bl 0x0200a4e8
	movs r0, #30
	bl 0x0200a520
	movs r1, #222
	movs r2, #166
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #15
	bl 0x0200a578
	movs r0, #15
	bl 0x0200a540
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r3, #248
	lsls r3, r3, #8
	strh r3, [r2, #30]
	movs r1, #0
	strh r6, [r5, #6]
	bl 0x0200a4b0
	ldr r1, [pc, #28]
	adds r0, r5, #0
	bl 0x0200a4b8
	bl 0x0200a530
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x09c8
	.2byte 0x0000
	.2byte 0x09c9
	.2byte 0x0000
	.4byte 0x0000e666
	.4byte 0x0200a6fc
	.global Func_02000ccc
	.thumb_func
Func_02000ccc:
	push {r5, lr}
	ldr r0, [pc, #360]
	bl 0x0200a508
	cmp r0, #0
	bne .L_02000ccc_0
	b .L_02000ccc_1
.L_02000ccc_0:
	ldr r0, [pc, #352]
	bl 0x0200a508
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000ccc_2
	b .L_02000ccc_1
.L_02000ccc_2:
	ldr r0, [pc, #336]
	bl 0x0200a510
	bl 0x0200a528
	movs r0, #15
	bl 0x0200a540
	ldr r3, [r0, #80]
	movs r1, #16
	strh r5, [r3, #30]
	bl 0x0200a4b0
	movs r0, #152
	bl 0x0200a600
	movs r0, #15
	bl 0x0200a540
	movs r3, #128
	lsls r3, r3, #12
	movs r1, #128
	str r3, [r0, #40]
	movs r2, #30
	movs r0, #15
	lsls r1, r1, #8
	bl 0x0200a598
	movs r1, #129
	movs r0, #15
	lsls r1, r1, #1
	bl 0x0200a5a8
	movs r1, #2
	movs r0, #15
	bl 0x0200a588
	movs r0, #20
	bl 0x0200a520
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #8
	movs r0, #15
	bl 0x0200a548
	movs r0, #152
	bl 0x0200a600
	movs r0, #15
	bl 0x0200a540
	movs r3, #128
	lsls r3, r3, #11
	movs r1, #220
	movs r2, #170
	str r3, [r0, #40]
	lsls r2, r2, #2
	lsls r1, r1, #2
	movs r0, #15
	bl 0x0200a560
	movs r0, #10
	bl 0x0200a520
	movs r0, #15
	ldr r1, [pc, #208]
	bl 0x0200a5a8
	movs r1, #128
	movs r2, #128
	movs r0, #15
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a548
	movs r1, #220
	movs r2, #174
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a560
	movs r2, #176
	movs r0, #15
	ldr r1, [pc, #172]
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #220
	movs r2, #178
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a560
	movs r2, #180
	movs r0, #15
	ldr r1, [pc, #152]
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #220
	movs r2, #182
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a560
	movs r2, #184
	movs r0, #15
	ldr r1, [pc, #120]
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #220
	movs r2, #186
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a560
	movs r2, #188
	movs r0, #15
	ldr r1, [pc, #100]
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #220
	movs r2, #190
	movs r0, #15
	lsls r1, r1, #2
	lsls r2, r2, #2
	bl 0x0200a560
	movs r1, #214
	movs r2, #206
	lsls r1, r1, #18
	lsls r2, r2, #18
	movs r0, #15
	bl 0x0200a578
	movs r0, #10
	bl 0x0200a520
	movs r1, #192
	movs r2, #20
	movs r0, #15
	lsls r1, r1, #8
	bl 0x0200a598
	movs r1, #128
	movs r0, #15
	lsls r1, r1, #1
	bl 0x0200a5a8
	movs r0, #15
	bl 0x0200a540
	ldr r3, [pc, #32]
	str r3, [r0, #108]
	bl 0x0200a530
.L_02000ccc_1:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000009c9
	.4byte 0x000009ca
	.4byte 0x00000101
	.4byte 0x00000372
	.4byte 0x0000036e
	.4byte 0x02008aa9
	.global Func_02000e50
	.thumb_func
Func_02000e50:
	push {r5, r6, lr}
	ldr r3, [pc, #96]
	ldr r0, [pc, #96]
	ldr r5, [r3]
	bl 0x0200a508
	cmp r0, #0
	beq .L_02000e50_0
	ldr r2, [pc, #88]
	ldr r3, [pc, #92]
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #15
	beq .L_02000e50_0
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r0, #15
	movs r2, #0
	ldrsh r5, [r3, r2]
	bl 0x0200a540
	adds r6, r0, #0
	movs r0, #0
	bl 0x0200a540
	ldr r3, [r0, #48]
	movs r0, #15
	str r3, [r6, #48]
	bl 0x0200a540
	adds r6, r0, #0
	movs r0, #0
	bl 0x0200a540
	ldr r3, [r0, #48]
	subs r5, #30
	str r3, [r6, #52]
	ldr r2, [pc, #36]
	lsls r5, r5, #3
	adds r3, r5, #4
	ldr r1, [r2, r5]
	movs r0, #15
	ldr r2, [r2, r3]
	bl 0x0200a558
.L_02000e50_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.4byte 0x000009ca
	.4byte 0x0000024a
	.4byte 0x02000240
	.4byte 0x0200a808
	.global Func_02000ec8
	.thumb_func
Func_02000ec8:
	push {lr}
	bl 0x0200a528
	movs r0, #15
	movs r1, #0
	bl 0x0200a580
	bl 0x0200a530
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000ee0
	.thumb_func
Func_02000ee0:
	push {lr}
	ldr r3, [pc, #48]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000ee0_0
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_0:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_2
	ldr r0, [pc, #36]
	b .L_02000ee0_1
.L_02000ee0_2:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_02000ee0_3
	ldr r0, [pc, #32]
	b .L_02000ee0_1
.L_02000ee0_3:
	ldr r0, [pc, #32]
.L_02000ee0_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x00000075
	.4byte 0x0200abb4
	.4byte 0x00000076
	.4byte 0x0200acb0
	.4byte 0x00000078
	.4byte 0x0200adac
	.4byte 0x0200aba8
	.global Func_02000f34
	.thumb_func
Func_02000f34:
	push {lr}
	bl 0x0200a5c8
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x02008fdc,"ax",%progbits
	.p2align 2
	.global Func_02000fdc
	.thumb_func
Func_02000fdc:
	push {r5, lr}
	movs r3, #128
	lsls r3, r3, #1
	ands r0, r3
	sub sp, #8
	cmp r0, #0
	beq .L_02000fdc_0
	movs r0, #157
	bl 0x0200a600
	movs r0, #128
	movs r1, #128
	movs r2, #128
	lsls r0, r0, #10
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl 0x0200a4e8
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #96]
	bl 0x0200a4e8
	movs r3, #70
	movs r2, #49
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #84
	movs r1, #29
	movs r2, #1
	movs r3, #3
	bl 0x0200a4d0
	movs r0, #60
	bl 0x0200a468
.L_02000fdc_0:
	movs r3, #70
	movs r2, #49
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #85
	movs r1, #29
	movs r2, #1
	movs r3, #3
	bl 0x0200a4d0
	movs r3, #50
	str r3, [sp, #4]
	movs r5, #6
	movs r0, #6
	movs r1, #49
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a4d8
	movs r3, #51
	str r3, [sp, #4]
	movs r0, #6
	movs r1, #49
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200a4d8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000e666
	.global Func_02001070
	.thumb_func
Func_02001070:
	push {lr}
	movs r0, #0
	bl 0x0200a540
	movs r2, #192
	ldrh r3, [r0, #6]
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_02001070_0
	ldr r0, [pc, #48]
	bl 0x0200a508
	cmp r0, #0
	bne .L_02001070_0
	movs r0, #243
	bl 0x0200a4f8
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_02001070_0
	ldr r0, [pc, #24]
	bl 0x0200a510
	movs r0, #128
	lsls r0, r0, #1
	bl 0x02008fdc
	movs r0, #243
	bl 0x0200a500
.L_02001070_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000009c4
	.global Func_020010b8
	.thumb_func
Func_020010b8:
	push {r5, r6, lr}
	adds r6, r0, #0
	movs r0, #0
	bl 0x0200a540
	movs r2, #192
	ldrh r3, [r0, #6]
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020010b8_0
	movs r3, #156
	lsls r3, r3, #4
	adds r5, r6, r3
	adds r0, r5, #0
	bl 0x0200a508
	cmp r0, #0
	bne .L_020010b8_0
	movs r0, #244
	bl 0x0200a4f8
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	beq .L_020010b8_0
	adds r0, r5, #0
	bl 0x0200a510
	movs r0, #128
	lsls r0, r0, #1
	orrs r0, r6
	bl 0x02008f40
	movs r0, #244
	bl 0x0200a500
.L_020010b8_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001108
	.thumb_func
Func_02001108:
	push {lr}
	movs r0, #0
	bl 0x020090b8
	pop {r0}
	bx r0
	.global Func_02001114
	.thumb_func
Func_02001114:
	push {lr}
	movs r0, #1
	bl 0x020090b8
	pop {r0}
	bx r0
	.global Func_02001120
	.thumb_func
Func_02001120:
	push {r5, lr}
	movs r0, #2
	bl 0x020090b8
	movs r0, #0
	bl 0x0200a540
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_02001120_0
	ldr r3, [pc, #28]
	movs r2, #206
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #12
	ble .L_02001120_0
	bl 0x0200a5c8
	movs r3, #0
	strh r3, [r5]
.L_02001120_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_0200115c
	.thumb_func
Func_0200115c:
	push {r5, lr}
	movs r0, #3
	bl 0x020090b8
	movs r0, #0
	bl 0x0200a540
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_0200115c_0
	ldr r3, [pc, #28]
	movs r2, #206
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r5, r3, r2
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #12
	ble .L_0200115c_0
	bl 0x0200a5c8
	movs r3, #0
	strh r3, [r5]
.L_0200115c_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_02001198
	.thumb_func
Func_02001198:
	push {lr}
	sub sp, #8
	movs r3, #25
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #45
	movs r2, #1
	movs r3, #2
	bl 0x0200a4d0
	ldr r0, [pc, #32]
	bl 0x0200a508
	cmp r0, #0
	bne .L_02001198_0
	movs r1, #204
	movs r2, #194
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200a578
.L_02001198_0:
	movs r0, #1
	bl 0x0200a520
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000eeb
	.global Func_020011d8
	.thumb_func
Func_020011d8:
	push {lr}
	sub sp, #8
	movs r3, #25
	movs r2, #48
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r0, #24
	movs r1, #48
	movs r2, #1
	bl 0x0200a4d0
	movs r1, #128
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #12
	lsls r2, r2, #12
	bl 0x0200a578
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02001204
	.thumb_func
Func_02001204:
	push {lr}
	movs r0, #244
	movs r1, #3
	bl 0x0200a5e0
	movs r0, #0
	movs r1, #1
	bl 0x0200a580
	movs r1, #0
	movs r0, #244
	bl 0x0200a538
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl 0x0200a578
	ldr r0, [pc, #8]
	bl 0x0200a510
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000ee7
	.global Func_02001238
	.thumb_func
Func_02001238:
	push {lr}
	movs r0, #244
	movs r1, #3
	bl 0x0200a5e0
	movs r0, #0
	movs r1, #1
	bl 0x0200a580
	movs r1, #0
	movs r0, #244
	bl 0x0200a538
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl 0x0200a578
	ldr r0, [pc, #8]
	bl 0x0200a510
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000ee8
	.global Func_0200126c
	.thumb_func
Func_0200126c:
	push {lr}
	movs r0, #244
	movs r1, #3
	bl 0x0200a5e0
	movs r0, #0
	movs r1, #1
	bl 0x0200a580
	movs r1, #0
	movs r0, #244
	bl 0x0200a538
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x0200a578
	ldr r0, [pc, #8]
	bl 0x0200a510
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000ee9
	.global Func_020012a0
	.thumb_func
Func_020012a0:
	push {lr}
	movs r0, #244
	movs r1, #3
	bl 0x0200a5e0
	movs r0, #0
	movs r1, #1
	bl 0x0200a580
	movs r1, #0
	movs r0, #244
	bl 0x0200a538
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x0200a578
	ldr r0, [pc, #8]
	bl 0x0200a510
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000eea
	.global Func_020012d4
	.thumb_func
Func_020012d4:
	push {lr}
	movs r0, #243
	movs r1, #3
	bl 0x0200a5e0
	movs r0, #0
	movs r1, #1
	bl 0x0200a580
	movs r1, #0
	movs r0, #243
	bl 0x0200a538
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x0200a578
	ldr r0, [pc, #8]
	bl 0x0200a510
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000eeb
	.section .text.x0200938c,"ax",%progbits
	.p2align 2
	.global Func_0200138c
	.thumb_func
Func_0200138c:
	push {lr}
	bl 0x0200a528
	movs r0, #0
	ldr r1, [pc, #124]
	ldr r2, [pc, #128]
	bl 0x0200a548
	movs r0, #8
	ldr r1, [pc, #116]
	ldr r2, [pc, #116]
	bl 0x0200a548
	movs r0, #188
	bl 0x0200a600
	movs r0, #0
	bl 0x0200a540
	cmp r0, #0
	beq .L_0200138c_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #8
	bl 0x0200a550
.L_0200138c_0:
	movs r0, #8
	bl 0x0200a570
	movs r1, #0
	movs r2, #24
	movs r0, #0
	bl 0x0200a568
	movs r0, #4
	bl 0x0200a520
	movs r0, #188
	bl 0x0200a600
	movs r1, #0
	movs r2, #16
	movs r0, #8
	bl 0x0200a568
	movs r0, #0
	bl 0x0200a570
	movs r1, #180
	lsls r1, r1, #1
	movs r2, #152
	movs r0, #8
	bl 0x0200a550
	movs r0, #8
	bl 0x0200a570
	bl 0x0200a530
	movs r0, #136
	lsls r0, r0, #2
	bl 0x0200a518
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0001e666
	.4byte 0x0000f333
	.global Func_0200141c
	.thumb_func
Func_0200141c:
	push {r5, r6, r7, lr}
	movs r0, #0
	bl 0x0200a540
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_0200141c_0
	ldr r1, [pc, #96]
	adds r3, r3, r1
.L_0200141c_0:
	ldr r0, [r0, #16]
	asrs r7, r3, #20
	cmp r0, #0
	bge .L_0200141c_1
	ldr r2, [pc, #84]
	adds r0, r0, r2
.L_0200141c_1:
	asrs r5, r0, #20
	ldr r3, [pc, #80]
	movs r0, #136
	lsls r0, r0, #2
	ldr r6, [r3]
	bl 0x0200a508
	cmp r0, #0
	bne .L_0200141c_2
	ldr r2, [pc, #68]
	movs r1, #147
	lsls r1, r1, #2
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200141c_2
	ldr r1, [pc, #56]
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	beq .L_0200141c_2
	cmp r7, #10
	bne .L_0200141c_2
	adds r3, r5, #0
	subs r3, #16
	cmp r3, #2
	bhi .L_0200141c_2
	movs r0, #136
	lsls r0, r0, #2
	bl 0x0200a510
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r6, r3
	movs r3, #92
	strh r3, [r2]
.L_0200141c_2:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000024a
	.global Func_0200149c
	.thumb_func
Func_0200149c:
	push {lr}
	bl 0x0200a528
	movs r0, #0
	ldr r1, [pc, #124]
	ldr r2, [pc, #128]
	bl 0x0200a548
	movs r0, #9
	ldr r1, [pc, #116]
	ldr r2, [pc, #116]
	bl 0x0200a548
	movs r0, #188
	bl 0x0200a600
	movs r0, #0
	bl 0x0200a540
	cmp r0, #0
	beq .L_0200149c_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #9
	bl 0x0200a550
.L_0200149c_0:
	movs r0, #9
	bl 0x0200a570
	movs r1, #0
	movs r2, #24
	movs r0, #0
	bl 0x0200a568
	movs r0, #188
	bl 0x0200a600
	movs r0, #4
	bl 0x0200a520
	movs r1, #0
	movs r2, #16
	movs r0, #9
	bl 0x0200a568
	movs r0, #0
	bl 0x0200a570
	movs r2, #132
	movs r1, #168
	lsls r2, r2, #1
	movs r0, #9
	bl 0x0200a550
	movs r0, #9
	bl 0x0200a570
	bl 0x0200a530
	movs r0, #136
	lsls r0, r0, #2
	bl 0x0200a518
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0001b333
	.4byte 0x0000d999
	.global Func_0200152c
	.thumb_func
Func_0200152c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r7, r0, #0
	movs r0, #0
	bl 0x0200a540
	ldr r1, [r0, #8]
	cmp r1, #0
	bge .L_0200152c_0
	ldr r2, [pc, #120]
	adds r1, r1, r2
.L_0200152c_0:
	ldr r0, [r0, #16]
	asrs r6, r1, #20
	cmp r0, #0
	bge .L_0200152c_1
	ldr r3, [pc, #108]
	adds r0, r0, r3
.L_0200152c_1:
	ldr r2, [pc, #108]
	ldr r3, [pc, #108]
	adds r3, r3, r2
	adds r5, r7, #0
	movs r2, #0
	ldrsh r3, [r3, r2]
	asrs r0, r0, #20
	adds r5, #10
	mov r10, r0
	cmp r3, r5
	beq .L_0200152c_2
	ldr r3, [pc, #92]
	lsls r7, r7, #2
	mov r8, r3
	ldr r3, [r3, r7]
	cmp r6, r3
	beq .L_0200152c_2
	movs r1, #144
	movs r2, #144
	lsls r1, r1, #11
	lsls r2, r2, #10
	adds r0, r5, #0
	bl 0x0200a548
	movs r0, #188
	bl 0x0200a600
	lsls r1, r6, #4
	movs r2, #180
	lsls r2, r2, #1
	adds r1, #8
	adds r0, r5, #0
	bl 0x0200a550
	mov r2, r8
	mov r3, r10
	str r6, [r2, r7]
	cmp r3, #22
	bgt .L_0200152c_3
	movs r0, #0
	movs r1, #0
	movs r2, #8
	bl 0x0200a568
.L_0200152c_3:
	movs r0, #0
	bl 0x0200a570
.L_0200152c_2:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x0000024a
	.4byte 0x02000240
	.4byte 0x0200af74
	.global Func_020015cc
	.thumb_func
Func_020015cc:
	push {lr}
	movs r0, #0
	bl 0x0200952c
	pop {r0}
	bx r0
	.global Func_020015d8
	.thumb_func
Func_020015d8:
	push {lr}
	movs r0, #1
	bl 0x0200952c
	pop {r0}
	bx r0
	.global Func_020015e4
	.thumb_func
Func_020015e4:
	push {lr}
	movs r0, #2
	bl 0x0200952c
	pop {r0}
	bx r0
	.global Func_020015f0
	.thumb_func
Func_020015f0:
	push {lr}
	ldr r2, [pc, #144]
	ldr r3, [pc, #144]
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	beq .L_020015f0_0
	bl 0x0200a528
	movs r0, #0
	ldr r1, [pc, #132]
	ldr r2, [pc, #132]
	bl 0x0200a548
	movs r0, #10
	ldr r1, [pc, #120]
	ldr r2, [pc, #124]
	bl 0x0200a548
	movs r0, #188
	bl 0x0200a600
	movs r0, #0
	bl 0x0200a540
	cmp r0, #0
	beq .L_020015f0_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #10
	bl 0x0200a550
.L_020015f0_1:
	movs r0, #10
	bl 0x0200a570
	movs r1, #0
	movs r2, #24
	movs r0, #0
	bl 0x0200a568
	movs r0, #4
	bl 0x0200a520
	movs r0, #188
	bl 0x0200a600
	movs r1, #0
	movs r2, #16
	movs r0, #10
	bl 0x0200a568
	movs r0, #0
	bl 0x0200a570
	movs r1, #132
	movs r2, #180
	lsls r1, r1, #1
	lsls r2, r2, #1
	movs r0, #10
	bl 0x0200a550
	movs r0, #10
	bl 0x0200a570
	movs r0, #10
	bl 0x0200a520
	bl 0x0200a530
.L_020015f0_0:
	pop {r0}
	bx r0
	.4byte 0x0000024a
	.4byte 0x02000240
	.4byte 0x0001b333
	.4byte 0x0000d999
	.section .text.x020097ac,"ax",%progbits
	.p2align 2
	.global Func_020017ac
	.thumb_func
Func_020017ac:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #0
	bl 0x0200a540
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_020017ac_0
	ldr r2, [pc, #104]
	adds r3, r3, r2
.L_020017ac_0:
	ldr r0, [r0, #16]
	asrs r3, r3, #20
	mov r8, r3
	cmp r0, #0
	bge .L_020017ac_1
	ldr r3, [pc, #88]
	adds r0, r0, r3
.L_020017ac_1:
	ldr r3, [pc, #88]
	ldr r2, [pc, #92]
	ldr r5, [pc, #92]
	ldr r7, [r3]
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	asrs r6, r0, #20
	cmp r3, #12
	beq .L_020017ac_2
	movs r0, #136
	lsls r0, r0, #2
	bl 0x0200a508
	cmp r0, #0
	bne .L_020017ac_2
	movs r2, #147
	lsls r2, r2, #2
	adds r3, r5, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020017ac_2
	mov r3, r8
	cmp r3, #19
	bne .L_020017ac_2
	adds r3, r6, #0
	subs r3, #15
	cmp r3, #1
	bhi .L_020017ac_2
	movs r0, #136
	lsls r0, r0, #2
	bl 0x0200a510
	movs r3, #193
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #96
	strh r3, [r2]
.L_020017ac_2:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.4byte 0x03001ebc
	.4byte 0x0000024a
	.4byte 0x02000240
	.global Func_02001838
	.thumb_func
Func_02001838:
	push {lr}
	bl 0x0200a528
	movs r0, #0
	ldr r1, [pc, #116]
	ldr r2, [pc, #120]
	bl 0x0200a548
	movs r0, #12
	ldr r1, [pc, #108]
	ldr r2, [pc, #108]
	bl 0x0200a548
	movs r0, #188
	bl 0x0200a600
	movs r0, #0
	bl 0x0200a540
	cmp r0, #0
	beq .L_02001838_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #12
	bl 0x0200a550
.L_02001838_0:
	movs r0, #12
	bl 0x0200a570
	movs r1, #0
	movs r2, #24
	movs r0, #0
	bl 0x0200a568
	movs r0, #188
	bl 0x0200a600
	movs r1, #0
	movs r2, #16
	movs r0, #12
	bl 0x0200a568
	movs r0, #0
	bl 0x0200a570
	movs r1, #156
	lsls r1, r1, #1
	movs r2, #232
	movs r0, #12
	bl 0x0200a550
	movs r0, #12
	bl 0x0200a570
	bl 0x0200a530
	movs r0, #136
	lsls r0, r0, #2
	bl 0x0200a518
	pop {r0}
	bx r0
	.4byte 0x0001b333
	.4byte 0x0000d999
	.global Func_020018c0
	.thumb_func
Func_020018c0:
	ldr r3, [pc, #8]
	movs r2, #208
	lsls r2, r2, #4
	strh r2, [r3]
	bx lr
	.2byte 0x0000
	.4byte 0x04000052
	.global Func_020018d0
	.thumb_func
Func_020018d0:
	ldr r2, [pc, #4]
	ldr r3, [pc, #8]
	strh r2, [r3]
	bx lr
	.4byte 0x00000607
	.4byte 0x04000052
	.global Func_020018e0
	.thumb_func
Func_020018e0:
	push {lr}
	movs r0, #0
	sub sp, #8
	bl 0x0200a540
	movs r2, #192
	ldrh r3, [r0, #6]
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020018e0_0
	ldr r0, [pc, #128]
	bl 0x0200a508
	cmp r0, #0
	beq .L_020018e0_1
	movs r3, #45
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #43
	movs r2, #1
	movs r3, #1
	bl 0x0200a4d8
.L_020018e0_1:
	ldr r0, [pc, #104]
	bl 0x0200a518
	bl 0x0200a5d8
	b .L_020018e0_2
.L_020018e0_0:
	movs r2, #128
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_020018e0_3
	bl 0x0200a5d0
	b .L_020018e0_2
.L_020018e0_3:
	cmp r3, #0
	bne .L_020018e0_4
	ldr r0, [pc, #68]
	bl 0x0200a508
	cmp r0, #0
	beq .L_020018e0_5
	movs r3, #45
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #58
	movs r1, #36
	movs r2, #1
	movs r3, #1
	bl 0x0200a4d8
.L_020018e0_5:
	ldr r0, [pc, #44]
	bl 0x0200a510
	bl 0x0200a5c8
	b .L_020018e0_2
.L_020018e0_4:
	movs r2, #128
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_020018e0_2
	ldr r3, [r0, #12]
	cmp r3, #0
	bne .L_020018e0_6
	bl 0x02009b60
	b .L_020018e0_2
.L_020018e0_6:
	bl 0x0200a5c8
.L_020018e0_2:
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x00000206
	.4byte 0x00000207
	.global Func_02001980
	.thumb_func
Func_02001980:
	bx lr
	.2byte 0x0000
	.global Func_02001984
	.thumb_func
Func_02001984:
	push {r5, lr}
	movs r0, #13
	sub sp, #8
	bl 0x0200a540
	movs r3, #40
	movs r2, #55
	adds r5, r0, #0
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #40
	movs r1, #54
	movs r2, #1
	movs r3, #1
	bl 0x0200a4d8
	cmp r5, #0
	beq .L_02001984_0
	movs r0, #13
	bl 0x0200a540
	movs r3, #0
	adds r0, #85
	adds r2, r5, #0
	strb r3, [r0]
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
.L_02001984_0:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a510
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.global Func_020019cc
	.thumb_func
Func_020019cc:
	push {lr}
	movs r0, #0
	bl 0x0200a540
	ldrh r3, [r0, #6]
	cmp r3, #0
	bne .L_020019cc_0
	bl 0x0200a5c8
	b .L_020019cc_1
.L_020019cc_0:
	bl 0x02009b60
.L_020019cc_1:
	pop {r0}
	bx r0
	.global Func_020019e8
	.thumb_func
Func_020019e8:
	push {r5, lr}
	sub sp, #8
	movs r3, #40
	str r3, [sp, #0]
	movs r5, #42
	movs r0, #57
	movs r1, #42
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r3, #41
	str r3, [sp, #0]
	movs r0, #57
	movs r1, #42
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r0, #58
	movs r1, #42
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r3, #37
	str r3, [sp, #0]
	movs r1, #37
	movs r2, #3
	movs r3, #1
	movs r0, #62
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r0, #8
	bl 0x0200a540
	movs r3, #1
	adds r0, #85
	strb r3, [r0]
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.global Func_02001a48
	.thumb_func
Func_02001a48:
	push {lr}
	sub sp, #8
	movs r3, #42
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r1, #41
	movs r2, #1
	movs r3, #1
	movs r0, #58
	bl 0x0200a4d8
	movs r0, #8
	bl 0x0200a540
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_02001a70
	.thumb_func
Func_02001a70:
	push {lr}
	sub sp, #8
	movs r3, #41
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #42
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl 0x0200a4d8
	movs r0, #8
	bl 0x0200a540
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001a9c
	.thumb_func
Func_02001a9c:
	push {lr}
	sub sp, #8
	movs r3, #40
	movs r2, #42
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #42
	movs r2, #1
	movs r3, #1
	movs r0, #39
	bl 0x0200a4d8
	movs r0, #8
	bl 0x0200a540
	movs r3, #2
	adds r0, #35
	strb r3, [r0]
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	push	{r5, r6, lr}
	movs	r0, #8
	sub	sp, #8
	bl 0x0200a540
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #12]
	cmp	r3, #0
	bge.n	.L_02001ade
	ldr	r1, [pc, #128]
	adds	r3, r3, r1
.L_02001ade:
	asrs	r5, r3, #20
	cmp	r2, #0
	bne.n	.L_02001af0
	movs	r0, #8
	bl 0x0200a540
	movs	r3, #2
	adds	r0, #35
	strb	r3, [r0, #0]
.L_02001af0:
	bl 0x020099e8
	movs	r0, #8
	bl 0x0200a540
	movs	r3, #3
	adds	r0, #85
	movs	r6, #0
	strb	r3, [r0, #0]
	cmp	r5, #40
	bne.n	.L_02001b0c
	bl 0x02009a9c
	b.n	.L_02001b54
.L_02001b0c:
	cmp	r5, #42
	bne.n	.L_02001b16
	bl 0x02009a48
	b.n	.L_02001b54
.L_02001b16:
	cmp	r5, #41
	bne.n	.L_02001b20
	bl 0x02009a70
	b.n	.L_02001b54
.L_02001b20:
	cmp	r5, #39
	beq.n	.L_02001b2c
	cmp	r5, #38
	beq.n	.L_02001b2c
	cmp	r5, #37
	bne.n	.L_02001b54
.L_02001b2c:
	movs	r3, #42
	str	r3, [sp, #4]
	movs	r1, #36
	movs	r3, #1
	movs	r2, #1
	movs	r0, #61
	str	r5, [sp, #0]
	bl 0x0200a4d8
	movs	r0, #8
	bl 0x0200a540
	adds	r0, #85
	strb	r6, [r0, #0]
	movs	r0, #8
	bl 0x0200a540
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r0, #12]
.L_02001b54:
	add	sp, #8
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.2byte 0xffff
	.2byte 0x000f
	.global Func_02001b60
	.thumb_func
Func_02001b60:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a540
	adds r5, r0, #0
	movs r0, #8
	bl 0x0200a540
	ldr r3, [r5, #8]
	cmp r3, #0
	bge .L_02001b60_0
	ldr r2, [pc, #72]
	adds r3, r3, r2
.L_02001b60_0:
	ldr r0, [r0, #8]
	asrs r3, r3, #20
	cmp r0, #0
	bge .L_02001b60_1
	ldr r2, [pc, #60]
	adds r0, r0, r2
.L_02001b60_1:
	asrs r0, r0, #20
	cmp r3, #38
	bne .L_02001b60_2
	cmp r0, #38
	beq .L_02001b60_2
	movs r3, #192
	ldrh r0, [r5, #6]
	lsls r3, r3, #8
	cmp r0, r3
	bne .L_02001b60_3
	bl 0x0200a5d8
	b .L_02001b60_4
.L_02001b60_3:
	movs r2, #128
	lsls r2, r2, #7
	cmp r0, r2
	bne .L_02001b60_2
	bl 0x0200a5d0
	b .L_02001b60_4
.L_02001b60_2:
	bl 0x020099e8
	bl 0x020080c4
	bl 0x02009ac8
.L_02001b60_4:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.global Func_02001bc4
	.thumb_func
Func_02001bc4:
	push {lr}
	ldr r0, [pc, #84]
	sub sp, #8
	bl 0x0200a478
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl 0x0200a578
	ldr r0, [pc, #68]
	bl 0x0200a508
	cmp r0, #0
	beq .L_02001bc4_0
	movs r3, #45
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #58
	movs r1, #36
	movs r2, #1
	movs r3, #1
	bl 0x0200a4d8
	b .L_02001bc4_1
.L_02001bc4_0:
	movs r3, #45
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #46
	movs r1, #43
	movs r2, #1
	movs r3, #1
	bl 0x0200a4d8
.L_02001bc4_1:
	bl 0x02009ec0
	ldr r0, [pc, #16]
	bl 0x0200a510
	sub sp, #-8
	pop {r0}
	bx r0
	.4byte 0x02009e95
	.4byte 0x00000207
	.4byte 0x00000206
	.global Func_02001c28
	.thumb_func
Func_02001c28:
	push {lr}
	movs r0, #9
	bl 0x0200a540
	ldr r3, [r0, #8]
	cmp r3, #0
	bge .L_02001c28_0
	ldr r2, [pc, #32]
	adds r3, r3, r2
.L_02001c28_0:
	ldr r0, [r0, #16]
	asrs r3, r3, #20
	cmp r0, #0
	bge .L_02001c28_1
	ldr r2, [pc, #20]
	adds r0, r0, r2
.L_02001c28_1:
	asrs r0, r0, #20
	cmp r3, #45
	bne .L_02001c28_2
	cmp r0, #43
	bne .L_02001c28_2
	bl 0x02009bc4
.L_02001c28_2:
	pop {r0}
	bx r0
	.4byte 0x000fffff
	.global Func_02001c5c
	.thumb_func
Func_02001c5c:
	push {lr}
	bl 0x020080c4
	bl 0x02009c28
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001c6c
	.thumb_func
Func_02001c6c:
	.global SceneState_ApplyTwoRectsAtRow56
	.thumb_func
SceneState_ApplyTwoRectsAtRow56:
	push {r5, lr}
	sub sp, #8
	movs r3, #38
	str r3, [sp, #0]
	movs r5, #55
	movs r0, #38
.L_02001c78:
	movs r1, #56
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r3, #42
	str r3, [sp, #0]
	movs r0, #42
	movs r1, #56
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001ca0
	.thumb_func
Func_02001ca0:
	.global SceneState_ApplyRectAndClearSlotTenByte85
	.thumb_func
SceneState_ApplyRectAndClearSlotTenByte85:
	push {lr}
	sub sp, #8
	movs r3, #38
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #54
	movs r2, #1
	movs r3, #1
	movs r0, #40
	bl 0x0200a4d8
	movs r0, #10
	bl 0x0200a540
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001ccc
	.thumb_func
Func_02001ccc:
	.global SceneState_ApplyRectAndClearActor10Byte85
	.thumb_func
SceneState_ApplyRectAndClearActor10Byte85:
	push {lr}
	sub sp, #8
	movs r3, #42
	movs r2, #55
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #54
	movs r2, #1
	movs r3, #1
	movs r0, #40
	bl 0x0200a4d8
	movs r0, #10
	bl 0x0200a540
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	sub sp, #-8
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .text.x02009d64,"ax",%progbits
	.p2align 2
	.global Func_02001d64
	.thumb_func
Func_02001d64:
	push {lr}
	bl 0x02009c6c
	bl 0x020080c4
	bl 0x02009cf8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001d78
	.thumb_func
Func_02001d78:
	push {lr}
	movs r0, #0
	bl 0x0200a540
	movs r2, #128
	ldrh r3, [r0, #6]
	lsls r2, r2, #7
	cmp r3, r2
	bne .L_02001d78_0
	movs r0, #0
	movs r1, #6
	movs r2, #0
	bl 0x0200a590
	b .L_02001d78_1
.L_02001d78_0:
	bl 0x02009d64
.L_02001d78_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001da0
	.thumb_func
Func_02001da0:
	push {r5, lr}
	sub sp, #8
	movs r3, #49
	str r3, [sp, #0]
	movs r5, #55
	movs r0, #48
	movs r1, #55
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #48
	movs r1, #55
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r3, #51
	str r3, [sp, #0]
	movs r0, #48
	movs r1, #55
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r3, #52
	str r3, [sp, #0]
	movs r0, #48
	movs r1, #55
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001df8
	.thumb_func
Func_02001df8:
	push {r5, lr}
	sub sp, #8
	bl 0x02009da0
	movs r0, #11
	bl 0x0200a540
	ldr r0, [r0, #8]
	cmp r0, #0
	bge .L_02001df8_0
	ldr r3, [pc, #64]
	adds r0, r0, r3
.L_02001df8_0:
	asrs r0, r0, #20
	str r0, [sp, #0]
	movs r1, #55
	movs r0, #53
	movs r2, #1
	movs r3, #1
	movs r5, #55
	str r5, [sp, #4]
	bl 0x0200a4d8
	movs r0, #12
	bl 0x0200a540
	ldr r0, [r0, #8]
	cmp r0, #0
	bge 0x02009e34
.L_02001e30:
	ldr r3, [pc, #28]
	adds r0, r0, r3
	asrs r0, r0, #20
	str r0, [sp, #0]
	movs r1, #55
	movs r0, #53
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl 0x0200a4d8
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000fffff
	.global Func_02001e54
	.thumb_func
Func_02001e54:
	push {lr}
	bl 0x02009df8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001e60
	.thumb_func
Func_02001e60:
	push {lr}
	bl 0x02009da0
	bl 0x020080c4
	bl 0x02009e54
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001e74
	.thumb_func
Func_02001e74:
	push {lr}
	bl 0x02009df8
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001e80
	.thumb_func
Func_02001e80:
	push {lr}
	bl 0x02009da0
	bl 0x020080c4
	bl 0x02009e74
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001e94
	.thumb_func
Func_02001e94:
	push {r5, lr}
	movs r0, #14
	bl 0x0200a540
	adds r5, r0, #0
	movs r0, #9
	bl 0x0200a540
	movs r3, #128
	lsls r3, r3, #14
	str r3, [r5, #12]
	ldr r3, [r0, #8]
	str r3, [r5, #8]
	movs r2, #128
	ldr r3, [r0, #16]
	lsls r2, r2, #9
	adds r3, r3, r2
	str r3, [r5, #16]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02001ec0
	.thumb_func
Func_02001ec0:
	push {lr}
	movs r0, #9
	bl 0x0200a540
	movs r3, #128
	ldr r2, [r0, #16]
	lsls r3, r3, #9
	ldr r1, [r0, #8]
	adds r2, r2, r3
	movs r0, #107
	bl 0x0200a5e8
	pop {r0}
	bx r0
	.global Func_02001edc
	.thumb_func
Func_02001edc:
	push {r5, lr}
	movs r0, #0
	bl 0x0200a540
	movs r2, #128
	ldr r3, [r0, #12]
	lsls r2, r2, #13
	cmp r3, r2
	ble .L_02001edc_0
	movs r0, #8
	bl 0x0200a540
	movs r5, #2
	adds r0, #35
	strb r5, [r0]
	movs r0, #10
	bl 0x0200a540
	ldr r3, [r0, #12]
	cmp r3, #0
	bne .L_02001edc_1
	movs r0, #10
	bl 0x0200a540
	adds r0, #35
	strb r5, [r0]
.L_02001edc_1:
	movs r0, #11
	bl 0x0200a540
	b 0x02009f60
.L_02001edc_0:
	movs r0, #10
	bl 0x0200a540
	ldr r3, [r0, #12]
	cmp r3, #0
	bne 0x02009f44
	movs r0, #0
	bl 0x0200a540
	ldr r3, [r0, #16]
.L_02001f2c:
	cmp r3, #0
	bge .L_02001f2c_0
	ldr r2, [pc, #64]
	adds r3, r3, r2
.L_02001f2c_0:
	asrs r3, r3, #20
	cmp r3, #56
	ble .L_02001f2c_1
	movs r0, #10
	movs r1, #3
	bl 0x0200a5a0
	b 0x02009f58
.L_02001f2c_1:
	movs r0, #10
	movs r1, #1
	bl 0x0200a5a0
.L_02001f4c:
	movs r0, #10
	bl 0x0200a540
	movs r3, #1
	adds r0, #35
	strb r3, [r0]
	movs r0, #11
	bl 0x0200a540
	movs r5, #0
	adds r0, #35
	strb r5, [r0]
	movs r0, #12
	bl 0x0200a540
	adds r0, #35
	strb r5, [r0]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0xffff
	.2byte 0x000f
	.global Func_02001f78
	.thumb_func
Func_02001f78:
	push {r5, lr}
	ldr r3, [pc, #72]
	movs r2, #224
	ldr r1, [r3]
	movs r3, #129
	lsls r2, r2, #1
	lsls r3, r3, #2
	str r3, [r1, r2]
	ldr r3, [pc, #60]
	adds r5, r3, r2
	movs r3, #0
	ldrsh r2, [r5, r3]
	ldr r3, [pc, #56]
	ldrh r1, [r5]
	cmp r2, r3
	bne .L_02001f78_0
	bl 0x0200a188
	ldrh r1, [r5]
.L_02001f78_0:
	lsls r3, r1, #16
	ldr r2, [pc, #44]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02001f78_1
	bl 0x0200a290
	ldrh r1, [r5]
.L_02001f78_1:
	lsls r3, r1, #16
	ldr r2, [pc, #32]
	asrs r3, r3, #16
	cmp r3, r2
	bne .L_02001f78_2
	bl 0x0200a334
.L_02001f78_2:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x00000075
	.4byte 0x00000076
	.4byte 0x00000078
	.section .text.x0200a188,"ax",%progbits
	.p2align 2
	.global Func_02002188
	.thumb_func
Func_02002188:
	push {lr}
	movs r0, #1
	bl 0x0200a468
	movs r0, #12
	movs r1, #243
	bl 0x0200a0c4
	movs r0, #11
	movs r1, #244
	bl 0x0200a0c4
	movs r0, #10
	movs r1, #244
	bl 0x0200a0c4
	movs r0, #9
	movs r1, #244
	bl 0x0200a0c4
	movs r0, #8
	movs r1, #244
	bl 0x0200a0c4
	ldr r0, [pc, #180]
	bl 0x0200a508
	cmp r0, #0
	bne 0x0200a1d0
	movs r1, #232
	movs r2, #218
.L_020021c6:
	movs r0, #8
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x0200a578
	ldr r0, [pc, #160]
.L_020021d2:
	bl 0x0200a508
	cmp r0, #0
	bne 0x0200a1e8
	movs r1, #148
	movs r2, #206
.L_020021de:
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200a578
	ldr r0, [pc, #140]
	bl 0x0200a508
	cmp r0, #0
	bne .L_020021de_0
	movs r1, #164
	movs r2, #190
	movs r0, #10
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200a578
.L_020021de_0:
	ldr r0, [pc, #120]
	bl 0x0200a508
	cmp r0, #0
	bne .L_020021de_1
	movs r1, #180
	movs r2, #218
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #18
	bl 0x0200a578
.L_020021de_1:
	movs r0, #156
.L_0200221a:
	lsls r0, r0, #4
	bl 0x0200a508
	cmp r0, #0
	beq .L_0200221a_0
	movs r0, #0
	bl 0x02008f40
.L_0200221a_0:
	ldr r0, [pc, #84]
	bl 0x0200a508
	cmp r0, #0
	beq .L_0200221a_1
	movs r0, #1
	bl 0x02008f40
.L_0200221a_1:
	ldr r0, [pc, #72]
	bl 0x0200a508
	cmp r0, #0
	beq .L_0200221a_2
	movs r0, #2
	bl 0x02008f40
.L_0200221a_2:
	ldr r0, [pc, #60]
	bl 0x0200a508
	cmp r0, #0
	beq .L_0200221a_3
	movs r0, #3
	bl 0x02008f40
.L_0200221a_3:
	ldr r0, [pc, #48]
	bl 0x0200a508
	cmp r0, #0
	beq .L_0200221a_4
	movs r0, #0
	bl 0x02008fdc
.L_0200221a_4:
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0ee7
	.2byte 0x0000
	.2byte 0x0ee8
	.2byte 0x0000
	.2byte 0x0ee9
	.2byte 0x0000
	.2byte 0x0eea
	.2byte 0x0000
	.4byte 0x000009c1
	.4byte 0x000009c2
	.4byte 0x000009c3
	.4byte 0x000009c4
	.global Func_02002290
	.thumb_func
Func_02002290:
	push {r5, lr}
	movs r0, #8
	bl 0x0200a540
	movs r5, #1
	adds r0, #89
	strb r5, [r0]
	movs r0, #9
	bl 0x0200a540
	adds r0, #89
	strb r5, [r0]
	movs r0, #10
	bl 0x0200a540
	adds r0, #89
	strb r5, [r0]
	movs r0, #11
	bl 0x0200a540
	adds r0, #89
	strb r5, [r0]
	movs r0, #8
	bl 0x0200a540
	ldr r5, [pc, #84]
	str r5, [r0, #24]
	movs r0, #9
	bl 0x0200a540
	str r5, [r0, #24]
	movs r0, #10
	bl 0x0200a540
	str r5, [r0, #24]
	movs r0, #11
	bl 0x0200a540
	str r5, [r0, #24]
	movs r0, #12
	bl 0x0200a540
	movs r1, #200
	str r5, [r0, #24]
	lsls r1, r1, #4
	ldr r0, [pc, #48]
	bl 0x0200a470
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #40]
	bl 0x0200a470
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #36]
	bl 0x0200a470
	ldr r2, [pc, #32]
	ldr r3, [pc, #36]
	strh r2, [r3]
	ldr r2, [pc, #36]
	adds r3, #2
	strh r2, [r3]
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x0000b333
	.4byte 0x020097ad
	.4byte 0x0200941d
	.4byte 0x02009309
	.4byte 0x00003f42
	.4byte 0x04000050
	.4byte 0x00000607
	.global Func_02002334
	.thumb_func
Func_02002334:
	push {r5, lr}
	movs r0, #14
	bl 0x0200a540
	movs r5, #0
	adds r0, #85
	movs r1, #200
	strb r5, [r0]
	lsls r1, r1, #4
	ldr r0, [pc, #256]
	bl 0x0200a470
	movs r1, #200
	lsls r1, r1, #4
	ldr r0, [pc, #248]
	bl 0x0200a470
	movs r0, #107
	movs r1, #0
	movs r2, #0
	bl 0x0200a5e8
	ldr r0, [pc, #236]
	bl 0x0200a508
	cmp r0, #0
	beq .L_02002334_0
	movs r0, #14
	movs r1, #2
	bl 0x0200a580
.L_02002334_0:
	bl 0x02009ac8
	bl 0x02009c28
	bl 0x02009cf8
	bl 0x02009e54
	bl 0x02009e74
	movs r1, #3
	movs r0, #8
	bl 0x0200a5a0
	movs r0, #11
	bl 0x0200a540
	adds r0, #85
	strb r5, [r0]
	movs r0, #12
	bl 0x0200a540
	adds r0, #85
	strb r5, [r0]
	bl 0x02009df8
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200a508
	cmp r0, #0
	beq .L_02002334_1
	bl 0x02009984
	movs r0, #13
	movs r1, #5
	bl 0x0200a580
.L_02002334_1:
	ldr r0, [pc, #148]
	bl 0x0200a508
	cmp r0, #0
	bne .L_02002334_2
	ldr r0, [pc, #140]
	bl 0x0200a508
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02002334_3
	movs r1, #214
	movs r2, #206
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200a578
	movs r0, #15
	bl 0x0200a540
	ldr r3, [pc, #112]
	str r3, [r0, #108]
	b .L_02002334_2
.L_02002334_3:
	ldr r0, [pc, #112]
	bl 0x0200a508
	cmp r0, #0
	beq .L_02002334_4
	movs r1, #222
	movs r2, #166
	lsls r1, r1, #18
	movs r0, #15
	lsls r2, r2, #18
	bl 0x0200a578
	movs r0, #15
	bl 0x0200a540
	ldr r3, [r0, #80]
	movs r1, #16
	strh r5, [r3, #30]
	bl 0x0200a4b0
	b .L_02002334_2
.L_02002334_4:
	ldr r0, [pc, #72]
	bl 0x0200a508
	cmp r0, #0
	beq .L_02002334_5
	movs r1, #146
	movs r2, #170
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200a578
	b .L_02002334_2
.L_02002334_5:
	movs r1, #146
	movs r2, #166
	movs r0, #15
	lsls r1, r1, #18
	lsls r2, r2, #18
	bl 0x0200a578
.L_02002334_2:
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x02009e95
	.4byte 0x02009edd
	.4byte 0x00000ed9
	.4byte 0x00000109
	.4byte 0x000009ca
	.4byte 0x02008aa9
	.4byte 0x000009c9
	.4byte 0x000009c8
	.include "games/THE BROKEN SEAL/SRC/FIELD/TAKARA_ASHIBA/IMPORT.INC"
@ The compiler library links here from its licensed container.
	.section .rodata.part1,"a",%progbits
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0xffffc000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00002000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00002000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00001000
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0xfffff800
	.4byte 0x00000000
	.4byte 0x00000040
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000378
	.4byte 0x00000358
	.4byte 0x00000368
	.4byte 0x00000358
	.4byte 0x00000358
	.4byte 0x00000358
	.4byte 0x00000348
	.4byte 0x00000358
	.4byte 0x00000338
	.4byte 0x00000358
	.4byte 0x00000338
	.4byte 0x00000348
	.4byte 0x00000338
	.4byte 0x00000338
	.4byte 0x00000348
	.4byte 0x00000338
	.4byte 0x00000358
	.4byte 0x00000338
	.4byte 0x00000368
	.4byte 0x00000338
	.4byte 0x00000378
	.4byte 0x00000338
	.4byte 0x00000378
	.4byte 0x00000348
	.4byte 0xffff0000
	.4byte 0x00000078
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc00003c8
	.4byte 0x00180000
	.4byte 0x01f80230
	.4byte 0x000003f0
	.4byte 0xffff0002
	.4byte 0x00000058
	.4byte 0xc00003c8
	.4byte 0x00180000
	.4byte 0x01f80230
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x000000c8
	.4byte 0xc0000198
	.4byte 0x00280000
	.4byte 0x01d00000
	.4byte 0x000001b8
	.4byte 0xffff0002
	.4byte 0x00000058
	.4byte 0xc0000188
	.4byte 0x00280000
	.4byte 0x01d00000
	.4byte 0x000001b8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000308
	.4byte 0xc00003b8
	.4byte 0x02000000
	.4byte 0x03a00220
	.4byte 0x000003e0
	.4byte 0xffff0002
	.4byte 0x00000298
	.4byte 0xc00003b8
	.4byte 0x02000000
	.4byte 0x03a00220
	.4byte 0x000003e0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000075
	.4byte 0x00103080
	.4byte 0x00204080
	.4byte 0x00000076
	.4byte 0x00103081
	.4byte 0x00204081
	.4byte 0x00000078
	.4byte 0x00103083
	.4byte 0x00204083
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00500000
	.4byte 0x00000000
	.4byte 0x03f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01080000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00024000
	.4byte 0xffff01f4
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02680000
	.4byte 0x00000000
	.4byte 0x02a80000
	.4byte 0x00024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x02580000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x01024000
	.4byte 0xffff00d3
	.4byte 0x00000001
	.4byte 0x03480000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x01024000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x02880000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00024000
	.4byte 0xffff0014
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0x0036005a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x02009199
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020091d9
	.4byte 0x00009415
	.4byte 0xffff0008
	.4byte 0x02009205
	.4byte 0x00009415
	.4byte 0xffff0009
	.4byte 0x02009239
	.4byte 0x00009415
	.4byte 0xffff000a
	.4byte 0x0200926d
	.4byte 0x00009415
	.4byte 0xffff000b
	.4byte 0x020092a1
	.4byte 0x00009415
	.4byte 0xffff000c
	.4byte 0x020092d5
	.4byte 0x00000002
	.4byte 0xffff0032
	.4byte 0x02009071
	.4byte 0x00000002
	.4byte 0xffff0033
	.4byte 0x02009109
	.4byte 0x00000002
	.4byte 0xffff0034
	.4byte 0x02009115
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x02009121
	.4byte 0x00000002
	.4byte 0xffff0036
	.4byte 0x0200915d
	.4byte 0x00004602
	.4byte 0xffff0035
	.4byte 0x02008f35
	.4byte 0x00004602
	.4byte 0xffff0036
	.4byte 0x02008f35
	.4byte 0x00000013
	.4byte 0x0eca0064
	.4byte 0x0020014d
	.4byte 0x00000013
	.4byte 0x0ecb0065
	.4byte 0x0010010b
	.4byte 0x00000013
	.4byte 0x0ecc0066
	.4byte 0x001000c0
	.4byte 0x00000013
	.4byte 0x0ecd0067
	.4byte 0x001000e2
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000002
	.4byte 0xffff0035
	.4byte 0x020095cd
	.4byte 0x00000002
	.4byte 0xffff0036
	.4byte 0x020095d9
	.4byte 0x00000002
	.4byte 0xffff003d
	.4byte 0x020095f1
	.4byte 0x00000002
	.4byte 0xffff003e
	.4byte 0x02009695
	.4byte 0x00000006
	.4byte 0xffff0060
	.4byte 0x02009839
	.4byte 0x00000006
	.4byte 0xffff005c
	.4byte 0x0200949d
	.4byte 0x00000006
	.4byte 0xffff005b
	.4byte 0x0200938d
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte 0x020098c1
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte 0x020098d1
	.4byte 0x00009115
	.4byte 0xffff0008
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff0009
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff000a
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff000b
	.4byte 0x02008aa5
	.4byte 0x00009115
	.4byte 0xffff000c
	.4byte 0x02008aa5
	.4byte 0x00000013
	.4byte 0x0ece0064
	.4byte 0x002001bc
	.4byte 0x00000013
	.4byte 0x0ecf0065
	.4byte 0x001000e3
	.4byte 0x00000013
	.4byte 0x0ed00066
	.4byte 0x00100060
	.4byte 0x00000013
	.4byte 0x0ed10067
	.4byte 0x001000ba
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000202
	.4byte 0xffff0032
	.4byte 0x02009d65
	.4byte 0x00000202
	.4byte 0xffff0033
	.4byte 0x02009b61
	.4byte 0x00000202
	.4byte 0xffff0034
	.4byte 0x02009c5d
	.4byte 0x00000202
	.4byte 0xffff0035
	.4byte 0x02009e61
	.4byte 0x00000202
	.4byte 0xffff003d
	.4byte 0x02009b61
	.4byte 0x00000202
	.4byte 0xffff0036
	.4byte 0x02009d79
	.4byte 0x00000202
	.4byte 0xffff003c
	.4byte 0x020098e1
	.4byte 0x00000202
	.4byte 0xffff0046
	.4byte 0x020099cd
	.4byte 0x00000002
	.4byte 0x00360014
	.4byte 0x02008ad1
	.4byte 0x00000002
	.4byte 0x00360015
	.4byte 0x02008b69
	.4byte 0x00000002
	.4byte 0x00360016
	.4byte 0x02008ccd
	.4byte 0x00000002
	.4byte 0x0036001e
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x0036001f
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360020
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360021
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360022
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360023
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360024
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360025
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360026
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360027
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360028
	.4byte 0x02008e51
	.4byte 0x00000002
	.4byte 0x00360029
	.4byte 0x02008e51
	.4byte 0x00000007
	.4byte 0xffff000f
	.4byte 0x020089dd
	.4byte 0x00009115
	.4byte 0xffff000f
	.4byte 0x02008ec9
	.4byte 0x00001815
	.4byte 0x0200000d
	.4byte 0x02009985
	.4byte 0x00008c15
	.4byte 0x02010008
	.4byte 0x02009ac9
	.4byte 0x00008c15
	.4byte 0x02060009
	.4byte 0x02009c29
	.4byte 0x00008c15
	.4byte 0x0203000a
	.4byte 0x02009cf9
	.4byte 0x00008c15
	.4byte 0x0204000b
	.4byte 0x02009e55
	.4byte 0x00008c15
	.4byte 0x0205000c
	.4byte 0x02009e75
	.4byte 0x00000013
	.4byte 0x0ed60068
	.4byte 0x0020029a
	.4byte 0x00000013
	.4byte 0x0ed70069
	.4byte 0x001000bd
	.4byte 0x00000013
	.4byte 0x0ed8006a
	.4byte 0x001000bc
	.4byte 0x00000013
	.4byte 0x0ed9006b
	.4byte 0x00100026
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000000
	.global TakaraAshiba_IconTimer
TakaraAshiba_IconTimer:
	.4byte 0x00000000
