.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_02000030_0
	ldr r0, [pc, #24]
	b .L_02000030_1
.L_02000030_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_02000030_2
	ldr r0, [pc, #24]
	b .L_02000030_1
.L_02000030_2:
	ldr r0, [pc, #24]
.L_02000030_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000004b
	.4byte 0x02009120
	.4byte 0x0000004c
	.4byte 0x02009288
	.4byte 0x020090f0
	.global Func_02000070
	.thumb_func
Func_02000070:
	movs r0, #0
	bx lr
	.global Func_02000074
	.thumb_func
Func_02000074:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009390
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {r5, lr}
	ldr r3, [pc, #116]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #108]
	cmp r2, r3
	bne .L_0200007c_0
	ldr r0, [pc, #104]
	bl 0x02008e88
	cmp r0, #0
	beq .L_0200007c_1
	ldr r3, [pc, #100]
	adds r1, r3, #0
	movs r2, #0
	adds r1, #142
	adds r3, #166
	strb r2, [r1]
	strb r2, [r3]
.L_0200007c_1:
	ldr r0, [pc, #84]
	b .L_0200007c_2
.L_0200007c_0:
	ldr r3, [pc, #84]
	cmp r2, r3
	bne .L_0200007c_3
	ldr r0, [pc, #84]
	bl 0x02008e88
	cmp r0, #0
	beq .L_0200007c_4
	ldr r3, [pc, #76]
	movs r2, #1
	adds r3, #46
	strb r2, [r3]
.L_0200007c_4:
	ldr r0, [pc, #72]
	bl 0x02008e88
	cmp r0, #0
	bne .L_0200007c_5
	ldr r0, [pc, #68]
	bl 0x02008e88
	cmp r0, #0
	beq .L_0200007c_6
.L_0200007c_5:
	ldr r3, [pc, #48]
	movs r2, #1
	adds r3, #94
	strb r2, [r3]
.L_0200007c_6:
	ldr r5, [pc, #40]
	adds r0, r5, #0
	bl 0x02008eb8
	adds r0, r5, #0
	b .L_0200007c_2
.L_0200007c_3:
	ldr r0, [pc, #40]
.L_0200007c_2:
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000004b
	.4byte 0x00000909
	.4byte 0x0200940c
	.4byte 0x0000004c
	.4byte 0x000008fd
	.4byte 0x020095bc
	.4byte 0x000008fe
	.4byte 0x00000907
	.4byte 0x020093f4
	.global Func_0200011c
	.thumb_func
Func_0200011c:
	push {lr}
	ldr r3, [pc, #36]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #28]
	cmp r2, r3
	bne .L_0200011c_0
	ldr r0, [pc, #24]
	b .L_0200011c_1
.L_0200011c_0:
	ldr r3, [pc, #24]
	cmp r2, r3
	bne .L_0200011c_2
	ldr r0, [pc, #24]
	b .L_0200011c_1
.L_0200011c_2:
	ldr r0, [pc, #24]
.L_0200011c_1:
	pop {r1}
	bx r1
	.4byte 0x02000240
	.4byte 0x0000004b
	.4byte 0x02009730
	.4byte 0x0000004c
	.4byte 0x020099f4
	.4byte 0x02009724
	.global Func_0200015c
	.thumb_func
Func_0200015c:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #68]
	bl 0x02008f30
	movs r1, #0
	movs r0, #8
	bl 0x02008f38
	movs r0, #0
	movs r1, #0
	bl 0x02008ec0
	cmp r0, #1
	bne .L_0200015c_0
	movs r0, #8
	movs r1, #0
	bl 0x02008f40
	b .L_0200015c_1
.L_0200015c_0:
	ldr r3, [pc, #36]
	ldr r2, [r3]
	movs r3, #236
	lsls r3, r3, #1
	adds r2, r2, r3
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	movs r0, #8
	movs r1, #0
	bl 0x02008f50
.L_0200015c_1:
	bl 0x02008eb0
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000018bd
	.4byte 0x03001ebc
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #20]
	bl 0x02008f30
	movs r1, #0
	movs r0, #9
	bl 0x02008f50
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x00001918
	.global Func_020001d0
	.thumb_func
Func_020001d0:
	push {r5, r6, lr}
	bl 0x02008ea8
	ldr r0, [pc, #176]
	bl 0x02008e88
	cmp r0, #0
	beq .L_020001d0_0
	ldr r0, [pc, #168]
	bl 0x02008f30
	movs r0, #14
	movs r1, #0
	bl 0x02008f50
	b .L_020001d0_1
.L_020001d0_0:
	movs r1, #4
	movs r0, #14
	bl 0x02008f00
	ldr r0, [pc, #148]
	bl 0x02008f30
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x02008f48
	ldr r0, [pc, #136]
	bl 0x02008e88
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020001d0_2
	movs r0, #18
	bl 0x02008ec8
	adds r5, r0, #0
	bl 0x02008f88
	adds r0, #85
	strb r6, [r0]
	movs r1, #128
	movs r0, #128
	lsls r0, r0, #9
	lsls r1, r1, #6
	bl 0x02008f70
	movs r3, #1
	ldr r0, [r5, #8]
	ldr r1, [r5, #12]
	ldr r2, [r5, #16]
	bl 0x02008f78
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02008f18
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #14
	bl 0x02008f58
	bl 0x02008f80
	movs r0, #120
	bl 0x02008ea0
	movs r0, #0
	bl 0x02008ec8
	ldr r3, [r0, #8]
	ldr r1, [r0, #12]
	ldr r2, [r0, #16]
	adds r0, r3, #0
	movs r3, #1
	bl 0x02008f78
	bl 0x02008f80
.L_020001d0_2:
	movs r0, #14
	movs r1, #4
	bl 0x02008f08
.L_020001d0_1:
	bl 0x02008eb0
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x00000909
	.4byte 0x0000191f
	.4byte 0x000018c7
	.4byte 0x000008ff
	.global Func_02000298
	.thumb_func
Func_02000298:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #20]
	bl 0x02008f30
	movs r1, #0
	movs r0, #17
	bl 0x02008f50
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x00001924
	.global Func_020002b8
	.thumb_func
Func_020002b8:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #20]
	bl 0x02008f30
	movs r1, #0
	movs r0, #9
	bl 0x02008f50
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x00001932
	.global Func_020002d8
	.thumb_func
Func_020002d8:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #20]
	bl 0x02008f30
	movs r1, #0
	movs r0, #10
	bl 0x02008f50
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x000018d9
	.global Func_020002f8
	.thumb_func
Func_020002f8:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #20]
	bl 0x02008f30
	movs r1, #0
	movs r0, #14
	bl 0x02008f50
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x000018e1
	.global Func_02000318
	.thumb_func
Func_02000318:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #20]
	bl 0x02008f30
	movs r1, #0
	movs r0, #21
	bl 0x02008f50
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x0000194a
	.global Func_02000338
	.thumb_func
Func_02000338:
	push {lr}
	movs r0, #0
	bl 0x02008ec8
	ldr r2, [pc, #20]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #20]
	lsls r3, r3, #16
	movs r0, #1
	cmp r3, r2
	bls .L_02000338_0
	movs r0, #0
.L_02000338_0:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00005fff
	.4byte 0x3ffe0000
	.global Func_02000360
	.thumb_func
Func_02000360:
	push {lr}
	ldr r0, [pc, #92]
	bl 0x02008e88
	cmp r0, #0
	bne .L_02000360_0
	bl 0x02008ea8
	ldr r0, [pc, #80]
	bl 0x02008f30
	movs r1, #0
	movs r0, #15
	bl 0x02008f50
	bl 0x02008eb0
	b .L_02000360_1
.L_02000360_0:
	bl 0x02008338
	cmp r0, #0
	beq .L_02000360_2
	movs r0, #19
	movs r1, #15
	bl 0x02008fb0
	b .L_02000360_1
.L_02000360_2:
	bl 0x02008ea8
	ldr r0, [pc, #44]
	bl 0x02008f30
	ldr r0, [pc, #40]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000360_3
	ldr r0, [pc, #36]
	bl 0x02008f30
.L_02000360_3:
	movs r0, #15
	movs r1, #0
	bl 0x02008f40
	bl 0x02008eb0
.L_02000360_1:
	pop {r0}
	bx r0
	.4byte 0x00000242
	.4byte 0x000018e7
	.4byte 0x000018ea
	.4byte 0x00000909
	.4byte 0x00001941
	.global Func_020003d4
	.thumb_func
Func_020003d4:
	push {lr}
	ldr r0, [pc, #92]
	bl 0x02008e88
	cmp r0, #0
	bne .L_020003d4_0
	bl 0x02008ea8
	ldr r0, [pc, #80]
	bl 0x02008f30
	movs r0, #20
	movs r1, #0
	bl 0x02008f40
	bl 0x02008eb0
	b .L_020003d4_1
.L_020003d4_0:
	bl 0x02008338
	cmp r0, #0
	beq .L_020003d4_2
	movs r0, #20
	movs r1, #17
	bl 0x02008fb0
	b .L_020003d4_1
.L_020003d4_2:
	bl 0x02008ea8
	ldr r0, [pc, #44]
	bl 0x02008f30
	ldr r0, [pc, #40]
	bl 0x02008e88
	cmp r0, #0
	beq .L_020003d4_3
	ldr r0, [pc, #36]
	bl 0x02008f30
.L_020003d4_3:
	movs r0, #17
	movs r1, #0
	bl 0x02008f40
	bl 0x02008eb0
.L_020003d4_1:
	pop {r0}
	bx r0
	.4byte 0x00000241
	.4byte 0x000018ed
	.4byte 0x000018ee
	.4byte 0x00000909
	.4byte 0x00001943
	.global Func_02000448
	.thumb_func
Func_02000448:
	push {lr}
	movs r0, #144
	lsls r0, r0, #2
	bl 0x02008e88
	cmp r0, #0
	bne .L_02000448_0
	bl 0x02008ea8
	ldr r0, [pc, #80]
	bl 0x02008f30
	movs r0, #21
	movs r1, #0
	bl 0x02008f40
	bl 0x02008eb0
	b .L_02000448_1
.L_02000448_0:
	bl 0x02008338
	cmp r0, #0
	beq .L_02000448_2
	movs r0, #21
	movs r1, #16
	bl 0x02008fb0
	b .L_02000448_1
.L_02000448_2:
	bl 0x02008ea8
	ldr r0, [pc, #40]
	bl 0x02008f30
	ldr r0, [pc, #40]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000448_3
	ldr r0, [pc, #32]
	bl 0x02008f30
.L_02000448_3:
	movs r0, #16
	movs r1, #0
	bl 0x02008f40
	bl 0x02008eb0
.L_02000448_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x000018f1
	.4byte 0x000018f2
	.4byte 0x00000909
	.4byte 0x00001945
	.global Func_020004bc
	.thumb_func
Func_020004bc:
	push {lr}
	movs r0, #0
	bl 0x02008ec8
	ldr r2, [pc, #72]
	ldrh r3, [r0, #6]
	adds r3, r3, r2
	ldr r2, [pc, #72]
	lsls r3, r3, #16
	cmp r3, r2
	bhi .L_020004bc_0
	movs r0, #6
	movs r1, #18
	bl 0x02008fb8
	b .L_020004bc_1
.L_020004bc_0:
	bl 0x02008ea8
	ldr r0, [pc, #52]
	bl 0x02008e88
	cmp r0, #0
	beq .L_020004bc_2
	ldr r0, [pc, #48]
	bl 0x02008f30
	movs r0, #18
	movs r1, #0
	bl 0x02008f40
	b .L_020004bc_3
.L_020004bc_2:
	ldr r0, [pc, #36]
	bl 0x02008f30
	movs r0, #18
	movs r1, #0
	bl 0x02008f50
.L_020004bc_3:
	bl 0x02008eb0
.L_020004bc_1:
	pop {r0}
	bx r0
	.4byte 0x00005fff
	.4byte 0x3ffe0000
	.4byte 0x00000909
	.4byte 0x00001947
	.4byte 0x000018f5
	.global Func_02000524
	.thumb_func
Func_02000524:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, [pc, #364]
	ldr r3, [r3]
	sub sp, #8
	mov r9, r3
	bl 0x02008ea8
	movs r5, #8
	movs r6, #0
.L_02000524_1:
	adds r0, r5, #0
	bl 0x02008ec8
	cmp r0, #0
	beq .L_02000524_0
	adds r3, r0, #0
	adds r3, #85
	strb r6, [r3]
.L_02000524_0:
	adds r5, #1
	cmp r5, #65
	bls .L_02000524_1
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	ldrh r3, [r3]
	subs r3, #2
	lsls r3, r3, #16
	asrs r3, r3, #16
	ldr r5, [pc, #316]
	lsls r6, r3, #3
	mov r10, r3
	adds r3, r6, #4
	ldrsh r1, [r5, r3]
	adds r3, r3, r5
	mov r2, r10
	mov r8, r1
	movs r1, #2
	ldrsh r7, [r3, r1]
	cmp r2, #1
	bne .L_02000524_2
	movs r0, #188
	bl 0x02008fc0
	movs r5, #2
	movs r0, #42
	movs r1, #33
	mov r2, r8
	adds r3, r7, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008e68
	mov r6, r8
	adds r6, #2
	movs r1, #35
	adds r2, r6, #0
	adds r3, r7, #0
	movs r0, #42
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008e68
	movs r0, #4
	bl 0x02008ea0
	movs r0, #40
	movs r1, #33
	mov r2, r8
	adds r3, r7, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008e68
	movs r0, #40
	movs r1, #35
	adds r2, r6, #0
	adds r3, r7, #0
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008e68
	movs r0, #4
	bl 0x02008ea0
	b .L_02000524_3
.L_02000524_2:
	movs r0, #158
	bl 0x02008fc0
	mov r3, r10
	cmp r3, #3
	bne .L_02000524_4
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #33
	movs r1, #42
	movs r2, #8
	movs r3, #17
	bl 0x02008e68
.L_02000524_4:
	ldr r0, [r5, r6]
	mov r1, r8
	adds r2, r7, #0
	bl 0x02008e60
.L_02000524_3:
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl 0x02008ed8
	ldr r3, [pc, #140]
	movs r1, #224
	ldr r3, [r3]
	lsls r1, r1, #1
	movs r2, #128
	adds r3, r3, r1
	lsls r2, r2, #1
	str r2, [r3]
	movs r0, #0
	bl 0x02008ec8
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	movs r0, #0
	bl 0x02008f00
	mov r2, r10
	cmp r2, #6
	bne .L_02000524_5
	movs r0, #0
	movs r1, #2
	movs r2, #0
	bl 0x02008ee8
	b .L_02000524_6
.L_02000524_5:
	mov r3, r10
	cmp r3, #1
	beq .L_02000524_7
	movs r2, #4
	movs r0, #0
	movs r1, #2
	negs r2, r2
	bl 0x02008ee8
	b .L_02000524_6
.L_02000524_7:
	movs r0, #0
	movs r1, #2
	bl 0x02008f60
	movs r2, #4
	movs r0, #0
	movs r1, #0
	negs r2, r2
	bl 0x02008ef0
.L_02000524_6:
	movs r0, #10
	bl 0x02008ea0
	movs r3, #182
	lsls r3, r3, #1
	add r3, r9
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl 0x02008f90
	bl 0x02008f98
	bl 0x02008fa0
	bl 0x02008eb0
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02009e70
	.global Func_020006a4
	.thumb_func
Func_020006a4:
	push {r5, lr}
	ldr r3, [pc, #68]
	ldr r5, [r3]
	bl 0x02008ea8
	movs r0, #0
	bl 0x02008ec8
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #123
	bl 0x02008fc0
	movs r2, #16
	movs r1, #2
	negs r2, r2
	movs r0, #0
	bl 0x02008ee8
	movs r3, #182
	lsls r3, r3, #1
	adds r5, r5, r3
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl 0x02008f90
	bl 0x02008f98
	bl 0x02008fa0
	bl 0x02008eb0
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ebc
	.global Func_020006f0
	.thumb_func
Func_020006f0:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #102
	movs r1, #0
	ldrsh r3, [r6, r1]
	ldrh r2, [r6]
	cmp r3, #0
	beq .L_020006f0_0
	subs r3, r2, #1
	movs r2, #128
	strh r3, [r6]
	lsls r2, r2, #9
	lsls r3, r3, #16
	cmp r3, r2
	bne .L_020006f0_0
	movs r0, #1
	movs r1, #1
	negs r0, r0
	negs r1, r1
	ldr r2, [pc, #148]
	bl 0x02008e80
.L_020006f0_0:
	ldr r7, [r5, #40]
	cmp r7, #0
	bne 0x02008766
	movs r1, #1
	adds r0, r5, #0
	bl 0x02008e40
	ldr r3, [r5, #12]
	ldr r1, [pc, #132]
	ldr r2, [r5, #20]
.L_02000732:
	adds r3, r3, r1
	str r3, [r5, #12]
	cmp r3, r2
	bge .L_02000732_0
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_02000732_1
	movs r0, #229
	bl 0x02008fc0
	movs r3, #4
	movs r1, #128
	movs r2, #128
	str r7, [r5, #104]
	lsls r2, r2, #9
	strh r3, [r6]
	movs r0, #0
	lsls r1, r1, #9
	bl 0x02008e80
	ldr r2, [r5, #20]
.L_02000732_1:
	str r2, [r5, #12]
.L_02000732_0:
	adds r2, r5, #0
	adds r2, #91
	movs r3, #1
	b .L_02000732_2
	.2byte 0x1c2a
	.2byte 0x325b
	.2byte 0x2300
.L_02000732_2:
	strb r3, [r2]
	adds r6, r5, #0
	adds r6, #100
	movs r1, #0
	ldrsh r3, [r6, r1]
	ldrh r2, [r6]
	cmp r3, #0
	bne .L_02000732_3
	movs r0, #152
	bl 0x02008fc0
	movs r3, #1
	adds r0, r5, #0
	movs r1, #2
	str r3, [r5, #104]
	bl 0x02008e40
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r5, #40]
	ldrh r2, [r6]
.L_02000732_3:
	adds r3, r2, #1
	movs r2, #240
	strh r3, [r6]
	lsls r2, r2, #14
	lsls r3, r3, #16
	cmp r3, r2
	bne 0x020087a8
	movs r3, #0
	strh r3, [r6]
.L_020007a8:
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0xe666
	.2byte 0x0000
	.2byte 0x8000
	.2byte 0xfffe
	.global Func_020007b8
	.thumb_func
Func_020007b8:
	push {lr}
	movs r0, #18
	bl 0x02008ec8
	adds r1, r0, #0
	adds r3, r1, #0
	movs r2, #0
	adds r3, #100
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldr r3, [pc, #104]
	str r3, [r1, #72]
	ldr r3, [pc, #104]
	movs r0, #18
	str r3, [r1, #108]
	ldr r2, [pc, #100]
	ldr r1, [pc, #104]
	bl 0x02008ed8
	movs r2, #230
	movs r0, #18
	movs r1, #28
	lsls r2, r2, #1
	bl 0x02008ee0
	movs r2, #224
	movs r1, #24
	lsls r2, r2, #1
	movs r0, #18
	bl 0x02008ee0
	movs r0, #229
	bl 0x02008fc0
	movs r0, #18
	bl 0x02008ed0
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #9
	lsls r2, r2, #9
	movs r0, #0
	bl 0x02008e80
	movs r0, #4
	bl 0x02008ea0
	movs r0, #1
	movs r1, #1
	negs r1, r1
	ldr r2, [pc, #40]
	negs r0, r0
	bl 0x02008e80
	movs r0, #40
	bl 0x02008ea0
	movs r0, #18
	movs r1, #1
	bl 0x02008f00
	pop {r0}
	bx r0
	.4byte 0x00006666
	.4byte 0x020086f1
	.4byte 0x00009999
	.4byte 0x00013333
	.4byte 0x0000e666
	.global Func_0200084c
	.thumb_func
Func_0200084c:
	push {lr}
	movs r0, #19
	bl 0x02008ec8
	ldr r3, [r0, #8]
	asrs r3, r3, #20
	cmp r3, #22
	bne .L_0200084c_0
	ldr r0, [pc, #16]
	bl 0x02008e90
	b .L_0200084c_1
.L_0200084c_0:
	ldr r0, [pc, #8]
	bl 0x02008e98
.L_0200084c_1:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000906
	.global Func_02000874
	.thumb_func
Func_02000874:
	push {lr}
	ldr r0, [pc, #108]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_0
	movs r0, #144
	lsls r0, r0, #2
	bl 0x02008e90
.L_02000874_0:
	ldr r0, [pc, #92]
	bl 0x02008e88
	cmp r0, #0
	bne .L_02000874_1
	ldr r0, [pc, #88]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_2
.L_02000874_1:
	ldr r0, [pc, #80]
	bl 0x02008e90
.L_02000874_2:
	ldr r0, [pc, #68]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_3
	ldr r0, [pc, #60]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000874_3
	ldr r0, [pc, #60]
	bl 0x02008e90
.L_02000874_3:
	ldr r3, [pc, #56]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #48]
	cmp r2, r3
	bne .L_02000874_4
	bl 0x02008904
	b .L_02000874_5
.L_02000874_4:
	ldr r3, [pc, #40]
	cmp r2, r3
	bne .L_02000874_5
	bl 0x02008b2c
.L_02000874_5:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x000008fd
	.4byte 0x000008fe
	.4byte 0x00000907
	.4byte 0x00000241
	.4byte 0x00000242
	.4byte 0x02000240
	.4byte 0x0000004b
	.4byte 0x0000004c
	.global Func_02000904
	.thumb_func
Func_02000904:
	push {r5, r6, r7, lr}
	movs r0, #0
	sub sp, #8
	bl 0x02008ec8
	adds r7, r0, #0
	ldr r0, [pc, #508]
	bl 0x02008e88
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000904_0
	movs r3, #32
	movs r0, #64
	movs r1, #32
	movs r2, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	bl 0x02008e68
	movs r3, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #32
	movs r2, #32
	movs r3, #32
	bl 0x02008e70
	movs r0, #20
	b .L_02000904_1
.L_02000904_0:
	ldr r0, [pc, #464]
	bl 0x02008e88
	adds r6, r0, #0
	cmp r6, #0
	beq .L_02000904_2
	movs r3, #32
	movs r0, #64
	movs r1, #0
	movs r2, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	bl 0x02008e68
	movs r1, #0
	movs r2, #32
	movs r3, #32
	movs r0, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl 0x02008e70
	movs r0, #17
	bl 0x02008ed0
	movs r0, #20
	b .L_02000904_1
.L_02000904_2:
	movs r0, #144
	lsls r0, r0, #2
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000904_3
	movs r3, #32
	movs r0, #0
	movs r1, #64
	movs r2, #0
	str r3, [sp, #0]
	str r3, [sp, #4]
	bl 0x02008e68
	movs r1, #64
	movs r2, #32
	movs r3, #32
	movs r0, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl 0x02008e70
	movs r0, #16
	bl 0x02008ed0
	movs r0, #17
.L_02000904_1:
	bl 0x02008ed0
	movs r0, #21
	bl 0x02008ed0
	b .L_02000904_4
.L_02000904_3:
	str r0, [sp, #0]
	str r0, [sp, #4]
	movs r1, #32
	movs r2, #32
	movs r3, #32
	movs r0, #0
	bl 0x02008e70
	movs r0, #15
	bl 0x02008ed0
	movs r0, #16
	bl 0x02008ed0
	movs r0, #17
	bl 0x02008ed0
.L_02000904_4:
	ldr r0, [pc, #316]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000904_5
	movs r0, #18
	bl 0x02008ed0
	b .L_02000904_6
.L_02000904_5:
	movs r0, #170
	bl 0x02008fa8
	movs r0, #18
	movs r1, #2
	bl 0x02008f20
	movs r0, #18
	movs r1, #3
	bl 0x02008f00
	movs r1, #200
	ldr r0, [pc, #276]
	lsls r1, r1, #4
	bl 0x02008e28
.L_02000904_6:
	ldr r3, [pc, #272]
	movs r2, #225
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #3
	bne .L_02000904_7
	ldr r0, [pc, #260]
	bl 0x02008e98
.L_02000904_7:
	movs r3, #20
	movs r2, #41
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #0
	movs r1, #33
	movs r2, #4
	movs r3, #3
	bl 0x02008e70
	ldr r0, [pc, #240]
	bl 0x02008e88
	cmp r0, #0
	beq .L_02000904_8
	movs r1, #180
	movs r2, #168
	movs r0, #19
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl 0x02008ef8
.L_02000904_8:
	movs r0, #19
	bl 0x02008ec8
	movs r1, #0
	bl 0x02008e78
	movs r0, #22
	movs r1, #15
	bl 0x02008f20
	movs r0, #23
	movs r1, #15
	bl 0x02008f20
	movs r1, #15
	movs r0, #24
	bl 0x02008f20
	movs r0, #22
	bl 0x02008ec8
	adds r0, #89
	ldrb r3, [r0]
	movs r5, #8
	orrs r3, r5
	strb r3, [r0]
	movs r0, #23
	bl 0x02008ec8
	adds r0, #89
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #24
	bl 0x02008ec8
	adds r0, #89
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #22
	bl 0x02008ec8
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #2
	orrs r3, r5
	strb r3, [r0]
	movs r0, #23
	bl 0x02008ec8
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #24
	bl 0x02008ec8
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r1, #1
	movs r0, #22
	bl 0x02008f60
	movs r0, #23
	movs r1, #1
	bl 0x02008f60
	movs r1, #1
	movs r0, #24
	bl 0x02008f60
	movs r0, #1
	bl 0x02008e20
	bl 0x02008ea8
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	ldr r2, [r7, #16]
	movs r3, #0
	bl 0x02008f78
	bl 0x02008e58
	bl 0x02008eb0
	movs r0, #1
	bl 0x02008e20
	sub sp, #-8
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000242
	.4byte 0x00000241
	.4byte 0x000008ff
	.4byte 0x02008d09
	.4byte 0x02000240
	.4byte 0x0000012f
	.4byte 0x00000906
	.global Func_02000b2c
	.thumb_func
Func_02000b2c:
	push {lr}
	movs r0, #144
	lsls r0, r0, #2
	bl 0x02008e88
	cmp r0, #0
	bne .L_02000b2c_0
	movs r1, #202
	lsls r1, r1, #18
	ldr r2, [pc, #176]
	movs r0, #8
	bl 0x02008ef8
	movs r0, #8
	bl 0x02008ec8
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r0, #6]
	ldr r1, [pc, #160]
	movs r0, #9
	ldr r2, [pc, #160]
	bl 0x02008ef8
.L_02000b2c_0:
	ldr r0, [pc, #156]
	bl 0x02008e88
	cmp r0, #0
	bne .L_02000b2c_1
	movs r1, #140
	lsls r1, r1, #18
	ldr r2, [pc, #148]
	movs r0, #10
	bl 0x02008ef8
	movs r0, #10
	bl 0x02008ec8
	movs r3, #128
	lsls r3, r3, #5
	movs r1, #144
	strh r3, [r0, #6]
	lsls r1, r1, #18
	movs r0, #11
	ldr r2, [pc, #120]
	bl 0x02008ef8
.L_02000b2c_1:
	ldr r0, [pc, #120]
	bl 0x02008e88
	cmp r0, #0
	bne .L_02000b2c_2
	movs r2, #186
	movs r0, #15
	ldr r1, [pc, #108]
	lsls r2, r2, #18
	bl 0x02008ef8
	movs r0, #15
	bl 0x02008ec8
	movs r3, #176
	lsls r3, r3, #8
	strh r3, [r0, #6]
	b .L_02000b2c_3
.L_02000b2c_2:
	movs r0, #15
	bl 0x02008ec8
	adds r1, r0, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #4
	orrs r3, r2
	strb r3, [r1]
.L_02000b2c_3:
	movs r0, #17
	bl 0x02008ec8
	cmp r0, #0
	beq .L_02000b2c_4
	adds r1, r0, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #4
	orrs r3, r2
	strb r3, [r1]
.L_02000b2c_4:
	movs r0, #16
	bl 0x02008ec8
	cmp r0, #0
	beq .L_02000b2c_5
	adds r1, r0, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #4
	orrs r3, r2
	strb r3, [r1]
.L_02000b2c_5:
	pop {r0}
	bx r0
	.4byte 0x02d70000
	.4byte 0x031a0000
	.4byte 0x03390000
	.4byte 0x00000241
	.4byte 0x02c60000
	.4byte 0x00000242
	.4byte 0x01270000
	.global Func_02000c0c
	.thumb_func
Func_02000c0c:
	push {r5, lr}
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r1, [r5, #80]
	ldrb r2, [r1, #9]
	subs r3, #13
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	strb r3, [r1, #9]
	movs r1, #3
	bl 0x02008f28
	adds r0, r5, #0
	movs r1, #0
	bl 0x02008e78
	ldr r3, [pc, #8]
	str r3, [r5, #24]
	str r3, [r5, #28]
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00004ccc
	.global Func_02000c44
	.thumb_func
Func_02000c44:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl 0x02008e38
	adds r6, r5, #0
	lsls r0, r0, #1
	adds r6, #100
	lsrs r0, r0, #16
	movs r1, #0
	ldrsh r2, [r6, r1]
	subs r0, #1
	lsls r0, r0, #16
	ldr r3, [r5, #8]
	lsls r2, r2, #12
	asrs r0, r0, #1
	adds r2, r2, r0
	adds r3, r3, r2
	str r3, [r5, #8]
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #3
	bgt .L_02000c44_0
	bl 0x02008e38
	ldr r3, [r5, #16]
	lsls r0, r0, #15
	ldr r1, [pc, #124]
	lsrs r0, r0, #16
	subs r3, r3, r0
	adds r3, r3, r1
	str r3, [r5, #16]
	ldr r2, [pc, #120]
	ldr r3, [r5, #24]
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r1, [pc, #116]
	ldr r3, [r5, #28]
	adds r3, r3, r1
	b .L_02000c44_1
.L_02000c44_0:
	ldr r3, [r5, #16]
	movs r2, #128
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #16]
	ldr r2, [pc, #100]
	ldr r3, [r5, #24]
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
.L_02000c44_1:
	str r3, [r5, #28]
	bl 0x02008e38
	movs r1, #0
	ldrsh r3, [r6, r1]
	muls r3, r0
	lsrs r3, r3, #16
	ldrh r2, [r6]
	cmp r3, #0
	bne .L_02000c44_2
	adds r0, r5, #0
	movs r1, #7
	bl 0x02008f28
	ldrh r2, [r6]
.L_02000c44_2:
	lsls r3, r2, #16
	cmp r3, #0
	beq .L_02000c44_3
	subs r3, r2, #1
	b .L_02000c44_4
.L_02000c44_3:
	bl 0x02008e38
	lsls r3, r0, #2
	adds r3, r3, r0
	lsrs r3, r3, #16
	lsls r3, r3, #1
	adds r3, #2
.L_02000c44_4:
	strh r3, [r6]
	ldr r3, [r5, #104]
	subs r3, #1
	str r3, [r5, #104]
	cmp r3, #0
	bne .L_02000c44_5
	adds r0, r5, #0
	str r3, [r5, #108]
	bl 0x02008e50
.L_02000c44_5:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0xffff0000
	.4byte 0x00002666
	.4byte 0xfffff5c3
	.4byte 0x000007ae
	.global Func_02000d08
	.thumb_func
Func_02000d08:
	push {r5, r6, lr}
	ldr r3, [pc, #68]
	ldr r6, [r3]
	movs r3, #3
	ands r6, r3
	cmp r6, #0
	bne .L_02000d08_0
	movs r1, #128
	movs r3, #200
	movs r0, #222
	lsls r1, r1, #15
	movs r2, #0
	lsls r3, r3, #17
	bl 0x02008e48
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02000d08_0
	adds r3, r5, #0
	movs r2, #20
	adds r3, #100
	strh r2, [r3]
	adds r3, #2
	strh r6, [r3]
	str r2, [r5, #104]
	bl 0x02008c0c
	ldr r3, [pc, #20]
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r1, #1
	bl 0x02008e40
.L_02000d08_0:
	pop {r5, r6}
	pop {r0}
	bx r0
	.4byte 0x03001e40
	.4byte 0x02008c45
	.global Func_02000d58
	.thumb_func
Func_02000d58:
	push {lr}
	bl 0x02008ea8
	ldr r0, [pc, #176]
	ldr r1, [pc, #176]
	bl 0x02008f70
	movs r0, #252
	movs r1, #1
	movs r2, #225
	movs r3, #1
	lsls r2, r2, #17
	negs r1, r1
	lsls r0, r0, #14
	bl 0x02008f78
	bl 0x02008f80
	movs r0, #30
	bl 0x02008ea0
	movs r1, #1
	movs r0, #18
	bl 0x02008f00
	movs r0, #1
	negs r0, r0
	bl 0x02008fa8
	ldr r0, [pc, #132]
	bl 0x02008e30
	movs r0, #20
	bl 0x02008ea0
	movs r0, #0
	movs r1, #18
	movs r2, #0
	bl 0x02008f18
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02008f58
	movs r0, #18
	movs r1, #0
	movs r2, #20
	bl 0x02008f58
	movs r1, #208
	movs r2, #40
	lsls r1, r1, #8
	movs r0, #18
	bl 0x02008f58
	movs r0, #147
	bl 0x02008fc0
	movs r1, #2
	movs r0, #18
	bl 0x02008f10
	movs r0, #20
	bl 0x02008ea0
	movs r1, #176
	movs r2, #40
	movs r0, #18
	lsls r1, r1, #8
	bl 0x02008f58
	bl 0x020087b8
	movs r0, #0
	movs r1, #1
	bl 0x02008f68
	bl 0x02008f80
	movs r1, #4
	movs r0, #14
	bl 0x02008f08
	ldr r0, [pc, #24]
	bl 0x02008e90
	bl 0x02008eb0
	pop {r0}
	bx r0
	.4byte 0x00006666
	.4byte 0x00000ccc
	.4byte 0x02008d09
	.4byte 0x000008ff
	.include "games/THE BROKEN SEAL/SRC/FIELD/ARUTIN_MURA/IMPORT.INC"
	.4byte 0x00000016
	.4byte 0x0000001e
	.4byte 0x00000080
	.4byte 0x00000015
	.4byte 0x0000000f
	.4byte 0x00026666
	.4byte 0x00000015
	.4byte 0x00000010
	.4byte 0x00013333
	.4byte 0x80010000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01560000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01640000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01560000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01800000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x0000001c
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00001000
	.4byte 0x00000000
	.4byte 0x0000005a
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0xc0010000
	.4byte 0xffff0000
	.4byte 0x0000017c
	.4byte 0x400001b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x0000017c
	.4byte 0x400001b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000001e8
	.4byte 0x800000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000150
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000001a8
	.4byte 0x40000158
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000088
	.4byte 0x40000138
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000118
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x000001c8
	.4byte 0x400000e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x000000b8
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000a8
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000018
	.4byte 0x200001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x00000108
	.4byte 0x400000e0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000148
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00000157
	.4byte 0x400000b1
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0000
	.4byte 0x000000c8
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x0000009f
	.4byte 0xc0000102
	.4byte 0x00190000
	.4byte 0x00ff0010
	.4byte 0x0000012c
	.4byte 0xffff0002
	.4byte 0x00000150
	.4byte 0xc00000d5
	.4byte 0x010c0000
	.4byte 0x01f40028
	.4byte 0x000000fa
	.4byte 0xffff0003
	.4byte 0x000002b0
	.4byte 0xc00000cc
	.4byte 0x021e0000
	.4byte 0x03000028
	.4byte 0x000000fa
	.4byte 0xffff0004
	.4byte 0x000000f0
	.4byte 0xc00001f4
	.4byte 0x006e0000
	.4byte 0x015e0150
	.4byte 0x0000021c
	.4byte 0xffff0005
	.4byte 0x000001f0
	.4byte 0xc00001f8
	.4byte 0x01680000
	.4byte 0x02580150
	.4byte 0x0000021c
	.4byte 0xffff0006
	.4byte 0x000002f0
	.4byte 0xc0000204
	.4byte 0x027b0000
	.4byte 0x034d0118
	.4byte 0x0000022c
	.4byte 0xffff0007
	.4byte 0x00000050
	.4byte 0xc0000360
	.4byte 0x00100000
	.4byte 0x01b0027e
	.4byte 0x00000390
	.4byte 0xffff0008
	.4byte 0x00000044
	.4byte 0x400002d8
	.4byte 0x00100000
	.4byte 0x01b0027e
	.4byte 0x00000390
	.4byte 0xffff0009
	.4byte 0x000002b2
	.4byte 0x400002dc
	.4byte 0x01e00000
	.4byte 0x03800272
	.4byte 0x00000372
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000004b
	.4byte 0x00110002
	.4byte 0x0020704c
	.4byte 0x0031d009
	.4byte 0x0040104c
	.4byte 0x0050204c
	.4byte 0x0060304c
	.4byte 0x0070504c
	.4byte 0x0080604c
	.4byte 0x0090404c
	.4byte 0x00a0104d
	.4byte 0x00b01050
	.4byte 0x00c04054
	.4byte 0x00d03053
	.4byte 0x0000004c
	.4byte 0x0010404b
	.4byte 0x0020504b
	.4byte 0x0030604b
	.4byte 0x0040904b
	.4byte 0x0050704b
	.4byte 0x0060804b
	.4byte 0x0070204b
	.4byte 0x0080904c
	.4byte 0x0090804c
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00015000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00005000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x00d90000
	.4byte 0x00005000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00ba0000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000003
	.4byte 0x003a0000
	.4byte 0x00000000
	.4byte 0x00ce0000
	.4byte 0x00001000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x00011000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x00290000
	.4byte 0x00000000
	.4byte 0x01b90000
	.4byte 0x00013000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00b00000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00003000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x01cd0000
	.4byte 0x00001000
	.4byte 0xffff0067
	.4byte 0x00000002
	.4byte 0x015e0000
	.4byte 0x00000000
	.4byte 0x011e0000
	.4byte 0x0000c000
	.4byte 0xffff00be
	.4byte 0x00000001
	.4byte 0x003c0000
	.4byte 0x00000000
	.4byte 0x01da0000
	.4byte 0x00002000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00004000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x018d0000
	.4byte 0x00001000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x01a60000
	.4byte 0x00000000
	.4byte 0x00d00000
	.4byte 0x00001000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x01c40000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00024000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x014c0000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x01024000
	.4byte 0xffff00df
	.4byte 0x00000007
	.4byte 0x014c0000
	.4byte 0x00000000
	.4byte 0x018c0000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00001000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00690000
	.4byte 0x00000000
	.4byte 0x00e90000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01c80000
	.4byte 0x00000000
	.4byte 0x00b60000
	.4byte 0x00039000
	.4byte 0xffff006f
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00007000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x02b20000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00017000
	.4byte 0x0000006a
	.4byte 0x00000001
	.4byte 0x029d0000
	.4byte 0x00000000
	.4byte 0x00800000
	.4byte 0x00011000
	.4byte 0x00000066
	.4byte 0x00000001
	.4byte 0x025c0000
	.4byte 0x00000000
	.4byte 0x00910000
	.4byte 0x00033000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01070000
	.4byte 0x00000000
	.4byte 0x01b40000
	.4byte 0x00005000
	.4byte 0x00000076
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01c60000
	.4byte 0x00005000
	.4byte 0x0000007d
	.4byte 0x00000001
	.4byte 0x02e70000
	.4byte 0x00000000
	.4byte 0x01c60000
	.4byte 0x00003000
	.4byte 0x00000077
	.4byte 0x00000001
	.4byte 0x00670000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00003000
	.4byte 0x00000080
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x02f40000
	.4byte 0x00007000
	.4byte 0x0000007b
	.4byte 0x02008fc8
	.4byte 0x015e0000
	.4byte 0x00000000
	.4byte 0x02ea0000
	.4byte 0x00007000
	.4byte 0x0000006c
	.4byte 0x00000001
	.4byte 0x02860000
	.4byte 0x00000000
	.4byte 0x02de0000
	.4byte 0x00003000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000c402
	.4byte 0xffff000a
	.4byte 0x020086a5
	.4byte 0x0000c402
	.4byte 0xffff000b
	.4byte 0x020086a5
	.4byte 0x0000c402
	.4byte 0xffff000c
	.4byte 0x020086a5
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008525
	.4byte 0x0000c602
	.4byte 0xffff0009
	.4byte 0x02008525
	.4byte 0x00000000
	.4byte 0x09090008
	.4byte 0x0200815d
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001917
	.4byte 0x00000000
	.4byte 0x09090009
	.4byte 0x000018c2
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020081b1
	.4byte 0x00000000
	.4byte 0x0909000a
	.4byte 0x000018c3
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x0000191b
	.4byte 0x00000000
	.4byte 0x0909000b
	.4byte 0x000018c4
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x0000191c
	.4byte 0x00000000
	.4byte 0x0909000c
	.4byte 0x000018c5
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000191d
	.4byte 0x00000000
	.4byte 0x0909000d
	.4byte 0x000018c6
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000191e
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x020081d1
	.4byte 0x00000000
	.4byte 0x0909000f
	.4byte 0x000018c8
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001922
	.4byte 0x00000000
	.4byte 0x09090010
	.4byte 0x000018c9
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001923
	.4byte 0x00000000
	.4byte 0x09090011
	.4byte 0x000018ca
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x02008299
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x020083d5
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008449
	.4byte 0x00008d15
	.4byte 0x09090008
	.4byte 0x000018cb
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001927
	.4byte 0x00008d15
	.4byte 0x09090009
	.4byte 0x000018cc
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001928
	.4byte 0x00008d15
	.4byte 0x0909000a
	.4byte 0x000018cd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001929
	.4byte 0x00008d15
	.4byte 0x0909000b
	.4byte 0x000018ce
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000192a
	.4byte 0x00008d15
	.4byte 0x0909000c
	.4byte 0x000018cf
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000192b
	.4byte 0x00008d15
	.4byte 0x0909000d
	.4byte 0x000018d0
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000192c
	.4byte 0x00008d15
	.4byte 0x0909000e
	.4byte 0x000018d1
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x0000192d
	.4byte 0x00008d15
	.4byte 0x0909000f
	.4byte 0x000018d2
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x0000192e
	.4byte 0x00008d15
	.4byte 0x09090010
	.4byte 0x000018d3
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x0000192f
	.4byte 0x00008d15
	.4byte 0x09090011
	.4byte 0x000018d4
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001930
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x000018ef
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x000018f3
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte 0x0200884d
	.4byte 0x00000002
	.4byte 0x08ff0014
	.4byte 0x02008d59
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
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000000
	.4byte 0x02400008
	.4byte 0x00001902
	.4byte 0x00000000
	.4byte 0x09090008
	.4byte 0x000018d5
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001931
	.4byte 0x00000000
	.4byte 0x02400009
	.4byte 0x00001903
	.4byte 0x00000000
	.4byte 0x09090009
	.4byte 0x000018d6
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x020082b9
	.4byte 0x00000000
	.4byte 0x0241000a
	.4byte 0x00001906
	.4byte 0x00000000
	.4byte 0x0909000a
	.4byte 0x020082d9
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001937
	.4byte 0x00000000
	.4byte 0x0241000b
	.4byte 0x00001907
	.4byte 0x00000000
	.4byte 0x0909000b
	.4byte 0x000018dc
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001938
	.4byte 0x00000000
	.4byte 0x0909000c
	.4byte 0x000018df
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x0000193b
	.4byte 0x00000000
	.4byte 0x0909000d
	.4byte 0x000018e0
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x0000193c
	.4byte 0x00000000
	.4byte 0x0909000e
	.4byte 0x020082f9
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x0000193d
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x02008361
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x020083d5
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x02008449
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x020084bd
	.4byte 0x00000000
	.4byte 0x09090013
	.4byte 0x000018f8
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001948
	.4byte 0x00000000
	.4byte 0x09090014
	.4byte 0x000018f9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001949
	.4byte 0x00000000
	.4byte 0x02400015
	.4byte 0x000018fa
	.4byte 0x00000000
	.4byte 0x02410015
	.4byte 0x000018fb
	.4byte 0x00000000
	.4byte 0x09090015
	.4byte 0x000018fc
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x02008319
	.4byte 0x00008d15
	.4byte 0x02400008
	.4byte 0x00001904
	.4byte 0x00008d15
	.4byte 0x09090008
	.4byte 0x000018d7
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001935
	.4byte 0x00008d15
	.4byte 0x02400009
	.4byte 0x00001905
	.4byte 0x00008d15
	.4byte 0x09090009
	.4byte 0x000018d8
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001936
	.4byte 0x00008d15
	.4byte 0x0241000a
	.4byte 0x00001908
	.4byte 0x00008d15
	.4byte 0x0909000a
	.4byte 0x000018dd
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001939
	.4byte 0x00008d15
	.4byte 0x0241000b
	.4byte 0x00001909
	.4byte 0x00008d15
	.4byte 0x0909000b
	.4byte 0x000018de
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x0000193a
	.4byte 0x00008d15
	.4byte 0x0909000c
	.4byte 0x000018e4
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x0000193e
	.4byte 0x00008d15
	.4byte 0x0909000d
	.4byte 0x000018e5
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x0000193f
	.4byte 0x00008d15
	.4byte 0x0909000e
	.4byte 0x000018e6
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001940
	.4byte 0x00008d15
	.4byte 0x0242000f
	.4byte 0x000018eb
	.4byte 0x00008d15
	.4byte 0x0909000f
	.4byte 0x000018ec
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001942
	.4byte 0x00008d15
	.4byte 0x09090011
	.4byte 0x000018f0
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001944
	.4byte 0x00008d15
	.4byte 0x09090010
	.4byte 0x000018f4
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001946
	.4byte 0x00008d15
	.4byte 0x09090012
	.4byte 0x000018fd
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x0000194d
	.4byte 0x00008d15
	.4byte 0x09090013
	.4byte 0x000018fe
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000194e
	.4byte 0x00008d15
	.4byte 0x09090014
	.4byte 0x000018ff
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000194f
	.4byte 0x00008d15
	.4byte 0x02400015
	.4byte 0x00001900
	.4byte 0x00008d15
	.4byte 0x09090015
	.4byte 0x00001901
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001950
	.4byte 0x00000033
	.4byte 0x0f730064
	.4byte 0x001000b5
	.4byte 0x00000033
	.4byte 0x0f740065
	.4byte 0x00200009
	.4byte 0x000000e3
	.4byte 0x0f750066
	.4byte 0x001000e3
	.4byte 0x00000173
	.4byte 0xffff00c8
	.4byte 0x004029a4
	.4byte 0x00000173
	.4byte 0xffff00c9
	.4byte 0x004029a5
	.4byte 0x00000173
	.4byte 0xffff00ca
	.4byte 0x004029a6
	.4byte 0x000000f3
	.4byte 0xffff00cb
	.4byte 0x004029a7
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0021002a
	.4byte 0x00020002
	.4byte 0x002a0000
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x00210028
	.4byte 0x00020002
	.4byte 0x00280000
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020030
	.4byte 0x00050002
	.4byte 0x00300021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00230023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020023
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x0002002e
	.4byte 0x00050002
	.4byte 0x002e0021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00320023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020032
	.4byte 0x00050002
	.4byte 0x0023ffff
	.4byte 0x00020021
	.4byte 0x00050002
	.4byte 0x00210021
	.4byte 0x00020002
	.4byte 0xffff0005
	.4byte 0x00340023
	.4byte 0x00020002
	.4byte 0x00210005
	.4byte 0x00020034
	.4byte 0x00050002
	.4byte 0x0000ffff
	.4byte 0x02009dea
	.4byte 0x00050029
	.4byte 0x02009dc0
	.4byte 0x00060033
	.4byte 0x02009e2c
	.4byte 0x00110039
	.4byte 0x02009e42
	.4byte 0x00110027
	.4byte 0x02009e58
	.4byte 0x00190030
	.4byte 0x02009e00
	.4byte 0x0009003b
	.4byte 0x02009e16
	.4byte 0x0017002a
	.4byte 0x02009e42
	.4byte 0x001b0029
