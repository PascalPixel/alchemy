.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	movs r0, #13
	movs r1, #26
	bl 0x02008964
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000040
	.thumb_func
Func_02000040:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_02000040_0
	ldr r0, [pc, #24]
	b .L_02000040_1
.L_02000040_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_02000040_2
	ldr r0, [pc, #24]
	b .L_02000040_1
.L_02000040_2:
	ldr r0, [pc, #24]
.L_02000040_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000aa
	.4byte 0x02008a40
	.4byte 0x000000ab
	.4byte 0x02008ad0
	.4byte 0x02008998
	.global Func_02000080
	.thumb_func
Func_02000080:
	movs r0, #0
	bx lr
	.global Func_02000084
	.thumb_func
Func_02000084:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02008b48
	.global Func_0200008c
	.thumb_func
Func_0200008c:
	push {lr}
	ldr r3, [pc, #52]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200008c_0
	ldr r0, [pc, #40]
	b .L_0200008c_1
.L_0200008c_0:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_0200008c_2
	ldr r0, [pc, #40]
	bl 0x0200887c
	cmp r0, #0
	beq .L_0200008c_3
	ldr r0, [pc, #32]
	b .L_0200008c_1
.L_0200008c_3:
	ldr r0, [pc, #32]
	b .L_0200008c_1
.L_0200008c_2:
	ldr r0, [pc, #32]
.L_0200008c_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x000000aa
	.4byte 0x02008ba8
	.4byte 0x000000a9
	.4byte 0x0000096f
	.4byte 0x02008c98
	.4byte 0x02008c50
	.4byte 0x02008b90
	.global Func_020000e4
	.thumb_func
Func_020000e4:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_020000e4_0
	ldr r0, [pc, #24]
	b .L_020000e4_1
.L_020000e4_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_020000e4_2
	ldr r0, [pc, #24]
	b .L_020000e4_1
.L_020000e4_2:
	ldr r0, [pc, #24]
.L_020000e4_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x000000aa
	.4byte 0x02008ddc
	.4byte 0x000000ab
	.4byte 0x02008e54
	.4byte 0x02008d10
	.global Func_02000124
	.thumb_func
Func_02000124:
	push {r5, r6, lr}
	ldr r0, [pc, #304]
	sub sp, #8
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_0
	ldr r1, [pc, #296]
	movs r0, #226
	ldr r3, [pc, #296]
	lsls r0, r0, #1
	adds r2, r1, r0
	strh r3, [r2]
	movs r3, #227
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r3, #10
	strh r3, [r2]
.L_02000124_0:
	ldr r5, [pc, #272]
	movs r0, #224
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [pc, #268]
	cmp r6, r3
	bne .L_02000124_1
	ldr r0, [pc, #268]
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_2
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl 0x020088ec
.L_02000124_2:
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02000124_3
	ldr r0, [pc, #236]
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_4
	movs r0, #144
	lsls r0, r0, #2
	adds r3, r5, r0
	strh r6, [r3]
	ldr r3, [pc, #224]
	adds r2, r5, r3
	movs r3, #1
	strh r3, [r2]
.L_02000124_4:
	ldr r0, [pc, #220]
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000124_5
	movs r0, #144
	lsls r0, r0, #2
	adds r3, r5, r0
	strh r6, [r3]
	ldr r3, [pc, #196]
	adds r2, r5, r3
	movs r3, #5
	strh r3, [r2]
.L_02000124_5:
	ldr r0, [pc, #196]
	bl 0x0200888c
.L_02000124_3:
	ldr r5, [pc, #164]
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02000124_6
	ldr r0, [pc, #164]
	bl 0x02008884
	ldr r0, [pc, #172]
	bl 0x0200887c
	cmp r0, #0
	bne .L_02000124_6
	movs r3, #8
	movs r2, #27
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #0
	movs r2, #2
	movs r3, #1
	bl 0x0200886c
.L_02000124_6:
	movs r0, #225
	lsls r0, r0, #1
	adds r3, r5, r0
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #5
	bne .L_02000124_7
	ldr r0, [pc, #120]
	bl 0x02008884
	b .L_02000124_7
.L_02000124_1:
	ldr r3, [pc, #124]
	cmp r6, r3
	bne .L_02000124_7
	movs r0, #8
	movs r1, #4
	bl 0x020088f4
	movs r0, #9
	movs r1, #4
	bl 0x020088f4
	movs r0, #10
	movs r1, #3
	bl 0x020088f4
	movs r0, #11
	movs r1, #4
	bl 0x020088f4
	movs r1, #3
	movs r0, #12
	bl 0x020088f4
	movs r0, #15
	bl 0x020088b4
	ldr r3, [pc, #76]
	movs r2, #56
	str r3, [r0, #28]
	movs r3, #102
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #108
	movs r1, #38
	movs r2, #1
	movs r3, #1
	bl 0x0200886c
.L_02000124_7:
	movs r0, #0
	sub sp, #-8
	pop {r5, r6}
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000089f
	.4byte 0x02000240
	.4byte 0x00000069
	.4byte 0x000000a9
	.4byte 0x00000897
	.4byte 0x000008fb
	.4byte 0x00000242
	.4byte 0x000008fc
	.4byte 0x0000012f
	.4byte 0x0000096f
	.4byte 0x000000aa
	.4byte 0x00019999
	.global Func_02000288
	.thumb_func
Func_02000288:
	push {r5, r6, lr}
	ldr r3, [pc, #104]
	movs r2, #182
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #0
	sub sp, #8
	movs r2, #0
	ldrsh r6, [r3, r2]
	bl 0x020088b4
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #158
	bl 0x0200896c
	movs r5, #2
	movs r1, #36
	movs r2, #71
	movs r3, #8
	movs r0, #66
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008864
	movs r0, #4
	bl 0x0200885c
	movs r3, #8
	movs r1, #36
	movs r2, #71
	movs r0, #68
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008864
	movs r0, #4
	bl 0x0200885c
	movs r2, #16
	movs r1, #3
	negs r2, r2
	movs r0, #0
	bl 0x020088dc
	adds r0, r6, #0
	bl 0x0200895c
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push {r5, lr}
	ldr r3, [pc, #44]
	movs r2, #182
	ldr r3, [r3]
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r0, #123
	movs r2, #0
	ldrsh r5, [r3, r2]
	bl 0x0200896c
	ldr r0, [pc, #28]
	bl 0x0200888c
	ldr r0, [pc, #24]
	bl 0x0200888c
	adds r0, r5, #0
	bl 0x0200895c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x000008fb
	.4byte 0x000008fc
	.global Func_02000334
	.thumb_func
Func_02000334:
	push {r5, lr}
	sub sp, #8
	bl 0x0200889c
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020088bc
	movs r1, #128
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x020088bc
	movs r2, #192
	movs r0, #8
	movs r1, #136
	lsls r2, r2, #1
	bl 0x020088cc
	movs r2, #192
	movs r0, #9
	movs r1, #152
	lsls r2, r2, #1
	bl 0x020088d4
	movs r1, #128
	movs r0, #8
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02008944
	movs r1, #128
	movs r2, #0
	movs r0, #9
	lsls r1, r1, #7
	bl 0x02008944
	movs r0, #8
	movs r1, #1
	bl 0x020088f4
	movs r3, #27
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #6
	movs r1, #27
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200886c
	movs r3, #26
	str r3, [sp, #4]
	movs r0, #9
	movs r1, #26
	movs r2, #2
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200886c
	bl 0x020088a4
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_020003c4
	.thumb_func
Func_020003c4:
	push {r5, lr}
	sub sp, #8
	bl 0x0200889c
	movs r0, #0
	ldr r1, [pc, #840]
	ldr r2, [pc, #840]
	bl 0x020088bc
	movs r2, #219
	movs r0, #0
	movs r1, #120
	lsls r2, r2, #1
	bl 0x020088d4
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008944
	movs r0, #0
	bl 0x020088b4
	cmp r0, #0
	beq .L_020003c4_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #11
	bl 0x020088ec
.L_020003c4_0:
	movs r0, #1
	bl 0x0200885c
	movs r0, #11
	ldr r1, [pc, #780]
	ldr r2, [pc, #780]
	bl 0x020088bc
	movs r0, #11
	movs r1, #108
	ldr r2, [pc, #776]
	bl 0x020088d4
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02008944
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #20
	bl 0x0200894c
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #20
	bl 0x02008944
	movs r0, #11
	movs r1, #0
	movs r2, #40
	bl 0x02008944
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #40
	bl 0x02008944
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x02008944
	movs r1, #2
	movs r0, #11
	bl 0x02008904
	ldr r0, [pc, #696]
	bl 0x0200891c
	movs r0, #11
	movs r1, #0
	movs r2, #40
	bl 0x02008934
	movs r1, #128
	movs r2, #0
	movs r0, #8
	lsls r1, r1, #1
	bl 0x0200894c
	movs r0, #8
	movs r1, #2
	bl 0x0200890c
	movs r0, #8
	movs r1, #0
	movs r2, #10
	bl 0x02008934
	movs r2, #210
	movs r0, #11
	movs r1, #132
	lsls r2, r2, #1
	bl 0x020088d4
	movs r1, #208
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008944
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008944
	movs r2, #208
	movs r0, #11
	movs r1, #138
	lsls r2, r2, #1
	bl 0x020088d4
	movs r1, #176
	movs r2, #10
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02008944
	movs r0, #11
	movs r1, #2
	bl 0x02008904
	movs r2, #40
	movs r0, #11
	movs r1, #0
	bl 0x02008934
	movs r0, #8
	movs r1, #2
	bl 0x0200890c
	movs r0, #8
	movs r1, #0
	movs r2, #40
	bl 0x02008934
	movs r1, #128
	movs r2, #20
	movs r0, #9
	lsls r1, r1, #1
	bl 0x0200894c
	movs r0, #9
	movs r1, #2
	bl 0x0200890c
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x02008934
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02008944
	movs r2, #210
	lsls r2, r2, #1
	movs r1, #144
	movs r0, #11
	bl 0x020088d4
	movs r0, #20
	bl 0x02008894
	movs r0, #9
	movs r1, #2
	bl 0x0200890c
	movs r2, #20
	movs r0, #9
	movs r1, #0
	bl 0x02008934
	movs r1, #129
	movs r0, #9
	lsls r1, r1, #1
	bl 0x02008954
	movs r0, #9
	movs r1, #3
	bl 0x02008904
	movs r0, #9
	movs r1, #0
	movs r2, #20
	bl 0x02008934
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #20
	bl 0x02008944
	movs r0, #11
	movs r1, #0
	bl 0x0200893c
	movs r0, #155
	lsls r0, r0, #4
	bl 0x0200887c
	cmp r0, #0
	beq .L_020003c4_1
	movs r1, #208
	movs r2, #40
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02008944
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #11
	bl 0x02008954
	movs r0, #40
	bl 0x02008894
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008934
	b .L_020003c4_2
.L_020003c4_1:
	ldr r3, [pc, #380]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
.L_020003c4_2:
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02008944
	movs r0, #11
	movs r1, #0
	movs r2, #40
	bl 0x02008934
	movs r1, #128
	movs r0, #11
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200894c
	movs r1, #176
	movs r0, #11
	lsls r1, r1, #8
	movs r2, #10
	bl 0x02008944
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008934
	movs r2, #208
	movs r0, #11
	movs r1, #138
	lsls r2, r2, #1
	bl 0x020088d4
	movs r1, #176
	movs r2, #20
	movs r0, #11
	lsls r1, r1, #8
	bl 0x02008944
	movs r0, #8
	movs r1, #2
	bl 0x02008904
	movs r2, #10
	movs r0, #8
	movs r1, #0
	bl 0x02008934
	movs r1, #129
	movs r0, #11
	lsls r1, r1, #1
	bl 0x02008954
	movs r1, #1
	movs r0, #11
	bl 0x0200890c
	movs r0, #20
	bl 0x02008894
	movs r2, #20
	movs r0, #11
	movs r1, #0
	bl 0x02008934
	movs r1, #2
	movs r0, #9
	bl 0x0200890c
	movs r0, #20
	bl 0x02008894
	movs r1, #224
	movs r2, #10
	movs r0, #0
	lsls r1, r1, #8
	bl 0x02008944
	movs r1, #1
	movs r0, #9
	bl 0x0200890c
	movs r0, #20
	bl 0x02008894
	movs r2, #10
	movs r0, #9
	movs r1, #0
	bl 0x02008934
	movs r0, #8
	movs r1, #2
	bl 0x0200890c
	movs r2, #20
	movs r0, #8
	movs r1, #0
	bl 0x02008934
	movs r1, #1
	movs r0, #11
	bl 0x0200890c
	movs r0, #20
	bl 0x02008894
	movs r0, #11
	movs r1, #0
	movs r2, #20
	bl 0x02008934
	movs r1, #160
	movs r0, #11
	lsls r1, r1, #7
	movs r2, #10
	bl 0x02008944
	movs r0, #11
	movs r1, #0
	movs r2, #10
	bl 0x02008934
	movs r0, #11
	movs r1, #2
	bl 0x020088f4
	movs r0, #0
	bl 0x020088b4
	cmp r0, #0
	beq .L_020003c4_3
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #11
	bl 0x020088c4
.L_020003c4_3:
	movs r0, #11
	bl 0x020088e4
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl 0x020088ec
	movs r3, #27
	str r3, [sp, #4]
	movs r5, #7
	movs r0, #6
	movs r1, #27
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x0200886c
	movs r3, #26
	str r3, [sp, #4]
	movs r1, #26
	movs r2, #2
	movs r3, #1
	movs r0, #9
	str r5, [sp, #0]
	bl 0x0200886c
	ldr r0, [pc, #36]
	bl 0x02008884
	bl 0x020088a4
	sub sp, #-8
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00019999
	.4byte 0x0000cccc
	.4byte 0x000001af
	.4byte 0x00002654
	.4byte 0x03001ebc
	.4byte 0x0000089f
	.global Func_02000730
	.thumb_func
Func_02000730:
	push {lr}
	bl 0x0200889c
	ldr r0, [pc, #100]
	bl 0x0200887c
	cmp r0, #0
	beq .L_02000730_0
	ldr r0, [pc, #92]
	bl 0x0200891c
	b .L_02000730_1
.L_02000730_0:
	ldr r0, [pc, #88]
	bl 0x0200891c
	movs r1, #0
	movs r0, #9
	bl 0x02008924
	movs r0, #0
	movs r1, #0
	bl 0x020088ac
	cmp r0, #0
	bne .L_02000730_2
	movs r0, #9
	movs r1, #0
	bl 0x0200892c
	movs r0, #9
	movs r1, #4
	bl 0x020088fc
.L_02000730_1:
	movs r0, #9
	movs r1, #0
	bl 0x0200892c
	b .L_02000730_3
.L_02000730_2:
	ldr r3, [pc, #40]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #2
	strh r3, [r2]
	movs r0, #9
	movs r1, #0
	bl 0x0200892c
.L_02000730_3:
	bl 0x020088a4
	pop {r0}
	bx r0
	.4byte 0x0000089f
	.4byte 0x00002668
	.4byte 0x0000264e
	.4byte 0x03001ebc
	.global Func_020007ac
	.thumb_func
Func_020007ac:
	push {r5, lr}
	bl 0x0200889c
	ldr r0, [pc, #160]
	bl 0x0200891c
	movs r0, #10
	movs r1, #0
	movs r2, #10
	bl 0x02008934
	movs r5, #0
.L_020007ac_0:
	movs r1, #0
	movs r0, #10
	bl 0x02008914
	movs r0, #10
	bl 0x020088b4
	movs r1, #1
	bl 0x02008874
	movs r0, #4
	bl 0x0200885c
	movs r1, #15
	movs r0, #10
	bl 0x02008914
	movs r0, #10
	bl 0x020088b4
	movs r1, #0
	bl 0x02008874
	adds r5, #1
	movs r0, #4
	bl 0x0200885c
	cmp r5, #5
	bls .L_020007ac_0
	movs r5, #0
.L_020007ac_1:
	movs r1, #0
	movs r0, #10
	bl 0x02008914
	movs r0, #10
	bl 0x020088b4
	movs r1, #1
	bl 0x02008874
	movs r0, #2
	bl 0x0200885c
	movs r1, #15
	movs r0, #10
	bl 0x02008914
	movs r0, #10
	bl 0x020088b4
	movs r1, #0
	bl 0x02008874
	adds r5, #1
	movs r0, #2
	bl 0x0200885c
	cmp r5, #11
	bls .L_020007ac_1
	movs r1, #0
	movs r2, #0
	movs r0, #10
	bl 0x020088ec
	ldr r0, [pc, #16]
	bl 0x02008884
	bl 0x020088a4
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000266d
	.4byte 0x00000897
	.include "games/THE BROKEN SEAL/SRC/FIELD/SUHARA_GATE/IMPORT.INC"
	.section .rodata,"a",%progbits
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte 0x0000001c
	.4byte 0x00000005
	.4byte 0x00000010
	.4byte 0xffff0000
	.4byte 0x00000098
	.4byte 0x400001f8
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0001
	.4byte 0x00000098
	.4byte 0xc0000208
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0002
	.4byte 0x000000f8
	.4byte 0x80000118
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0003
	.4byte 0x00000088
	.4byte 0x40000130
	.4byte 0x00200000
	.4byte 0x01100020
	.4byte 0x00000220
	.4byte 0xffff0004
	.4byte 0x00000168
	.4byte 0x00000128
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0xffff0005
	.4byte 0x00000298
	.4byte 0x800001a8
	.4byte 0x01500000
	.4byte 0x02e00020
	.4byte 0x000001f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000028
	.4byte 0x00000130
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0002
	.4byte 0x00000188
	.4byte 0x80000170
	.4byte 0x00100000
	.4byte 0x01a00020
	.4byte 0x000001e0
	.4byte 0xffff0003
	.4byte 0x00000028
	.4byte 0x00000390
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0004
	.4byte 0x000002a8
	.4byte 0x80000388
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0xffff0005
	.4byte 0x00000268
	.4byte 0x40000398
	.4byte 0x00100000
	.4byte 0x02d00260
	.4byte 0x000003f0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0xc0000108
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000b8
	.4byte 0x400000a8
	.4byte 0x00180000
	.4byte 0x01180018
	.4byte 0x00000100
	.4byte 0xffff0003
	.4byte 0x00000118
	.4byte 0x40000158
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0xffff0004
	.4byte 0x00000298
	.4byte 0xc0000268
	.4byte 0x00d00000
	.4byte 0x02c00120
	.4byte 0x00000280
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x000000a9
	.4byte 0x0011d002
	.4byte 0x002010aa
	.4byte 0x0030a069
	.4byte 0x004040aa
	.4byte 0x0052a002
	.4byte 0x000000aa
	.4byte 0x001020a9
	.4byte 0x002030aa
	.4byte 0x003020aa
	.4byte 0x004040a9
	.4byte 0x005010ab
	.4byte 0x000000ab
	.4byte 0x001050aa
	.4byte 0x002030ab
	.4byte 0x003020ab
	.4byte 0x00421002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01700000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x03900000
	.4byte 0x00025000
	.4byte 0xffff00ee
	.4byte 0x00000007
	.4byte 0x02280000
	.4byte 0x00000000
	.4byte 0x03700000
	.4byte 0x00025000
	.4byte 0x004a005b
	.4byte 0x00000001
	.4byte 0x01080000
	.4byte 0x00000000
	.4byte 0x03780000
	.4byte 0x00002000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0xffff0098
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01800000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0098
	.4byte 0x02008980
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x02008974
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0xffff0098
	.4byte 0x02008974
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00004000
	.4byte 0xffff0039
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0xffff0001
	.4byte 0x020082f9
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0xffff0005
	.4byte 0x020082f9
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008289
	.4byte 0x00000002
	.4byte 0x096f000a
	.4byte 0x02008335
	.4byte 0x00000002
	.4byte 0x089f000a
	.4byte 0x020083c5
	.4byte 0x00000000
	.4byte 0x089f0008
	.4byte 0x0000264d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002667
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x02008731
	.4byte 0x00008d15
	.4byte 0x089f0008
	.4byte 0x00002652
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002669
	.4byte 0x00008d15
	.4byte 0x089f0009
	.4byte 0x00002653
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x0000266a
	.4byte 0x00000000
	.4byte 0x0897000a
	.4byte 0x020087ad
	.4byte 0x00008d15
	.4byte 0x0897040a
	.4byte 0x020087ad
	.4byte 0xffffffff
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
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x02008031
	.4byte 0x0000c413
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0x00000413
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0x00008413
	.4byte 0x0fb40064
	.4byte 0x001000c3
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
