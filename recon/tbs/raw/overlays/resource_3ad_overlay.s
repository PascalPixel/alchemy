.syntax unified
.include "games/THE BROKEN SEAL/SRC/SYSTEM/OVERLAY.INC"
	.thumb
	.global Overlay_02000000
Overlay_02000000:
	.include "games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/ENTRY.INC"
	.global Func_02000030
	.thumb_func
Func_02000030:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009c34
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
	.4byte 0x02009cac
	.global Func_02000044
	.thumb_func
Func_02000044:
	push {lr}
	ldr r3, [pc, #28]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #20]
	cmp r2, r3
	bne .L_02000044_0
	ldr r0, [pc, #16]
	b .L_02000044_1
.L_02000044_0:
	ldr r0, [pc, #16]
.L_02000044_1:
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000006a
	.4byte 0x02009cd8
	.4byte 0x02009cc0
	.global Func_02000074
	.thumb_func
Func_02000074:
	ldr r0, [pc, #0]
	bx lr
	.4byte 0x02009dd4
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	movs r0, #8
	bl 0x02009b3c
	cmp r0, #0
	beq .L_0200007c_0
	movs r1, #0
	bl 0x02009af4
.L_0200007c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000094
	.thumb_func
Func_02000094:
	push {lr}
	movs r0, #9
	sub sp, #8
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02000094_0
	adds r1, r0, #0
	movs r2, #1
	adds r1, #35
	strb r2, [r1]
	adds r2, r0, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
.L_02000094_0:
	movs r3, #8
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #32
	movs r2, #1
	movs r3, #1
	movs r0, #7
	bl 0x02009aec
	movs r0, #129
	lsls r0, r0, #2
	bl 0x02009b0c
	sub sp, #-8
	pop {r0}
	bx r0
	.global Func_020000d4
	.thumb_func
Func_020000d4:
	push {r5, r6, lr}
	movs r0, #10
	sub sp, #8
	bl 0x02009b3c
	movs r1, #5
	adds r5, r0, #0
	movs r0, #10
	bl 0x02009b74
	cmp r5, #0
	beq .L_020000d4_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x02009af4
	adds r2, r5, #0
	adds r2, #35
	movs r3, #1
	strb r3, [r2]
.L_020000d4_0:
	movs r3, #59
	str r3, [sp, #4]
	movs r6, #21
	movs r1, #87
	movs r2, #2
	movs r3, #5
	movs r0, #41
	str r6, [sp, #0]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r3, #24
	movs r2, #62
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #3
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x02009ae4
	movs r3, #55
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #94
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl 0x02009ae4
	movs r5, #58
	movs r1, #87
	movs r2, #2
	movs r3, #5
	movs r0, #43
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r1, #87
	movs r2, #2
	movs r3, #5
	movs r0, #41
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r0, #4
	bl 0x02009ad4
	movs r3, #13
	str r3, [sp, #4]
	movs r0, #21
	movs r1, #11
	movs r2, #2
	movs r3, #2
	str r6, [sp, #0]
	bl 0x02009aec
	movs r3, #22
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #21
	movs r1, #11
	movs r2, #1
	movs r3, #1
	bl 0x02009aec
	movs r3, #14
	str r3, [sp, #4]
	movs r0, #19
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl 0x02009aec
	sub sp, #-8
	pop {r5, r6}
	pop {r0}
	bx r0
	.global Func_020001b0
	.thumb_func
Func_020001b0:
	push {r5, lr}
	movs r0, #10
	bl 0x02009b3c
	adds r5, r0, #0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009b0c
	cmp r5, #0
	beq .L_020001b0_0
	adds r0, r5, #0
	movs r1, #0
	bl 0x02009af4
	adds r2, r5, #0
	adds r2, #35
	movs r3, #1
	strb r3, [r2]
.L_020001b0_0:
	ldr r0, [pc, #36]
	bl 0x02009b04
	cmp r0, #0
	bne .L_020001b0_1
	movs r0, #157
	bl 0x02009c2c
	bl 0x020080d4
	movs r0, #80
	bl 0x02009c2c
	ldr r0, [pc, #8]
	bl 0x02009b0c
.L_020001b0_1:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00000202
	.global Func_02000200
	.thumb_func
Func_02000200:
	push {lr}
	ldr r0, [pc, #8]
	bl 0x02009b0c
	pop {r0}
	bx r0
	.4byte 0x00000203
	.global Func_02000210
	.thumb_func
Func_02000210:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r0, [pc, #280]
	sub sp, #8
	bl 0x02009b04
	cmp r0, #0
	beq .L_02000210_0
	movs r5, #21
	movs r6, #57
	movs r1, #86
	movs r2, #2
	movs r3, #6
	movs r0, #41
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r1, #86
	movs r2, #2
	movs r3, #6
	movs r0, #43
	str r6, [sp, #4]
	str r5, [sp, #0]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r6, #58
	movs r1, #86
	movs r2, #2
	movs r3, #6
	movs r0, #41
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r0, #43
	movs r1, #86
	movs r2, #2
	movs r3, #6
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
.L_02000210_0:
	movs r3, #24
	mov r10, r3
	movs r3, #62
	mov r9, r3
	mov r3, r10
	str r3, [sp, #0]
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x02009ae4
	movs r3, #55
	str r3, [sp, #4]
	movs r5, #21
	mov r8, r3
	movs r0, #2
	movs r1, #94
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009ae4
	movs r6, #59
	movs r1, #86
.L_020002bc:
	movs r2, #2
	movs r3, #6
	movs r0, #41
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	mov r3, r10
	str r3, [sp, #0]
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x02009ae4
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #94
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009ae4
	movs r2, #2
	movs r3, #6
	movs r1, #86
	movs r0, #43
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r0, #10
	movs r1, #3
	bl 0x02009bc4
	movs r3, #22
	movs r2, #15
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #19
	movs r1, #17
	movs r2, #1
	movs r3, #1
	bl 0x02009aec
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0202
	.2byte 0x0000
	.global Func_0200033c
	.thumb_func
Func_0200033c:
	push {lr}
	ldr r0, [pc, #56]
	bl 0x02009b04
	cmp r0, #0
	bne .L_0200033c_0
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009b04
	cmp r0, #0
	bne .L_0200033c_0
	movs r1, #1
	ldr r0, [pc, #36]
	bl 0x02009afc
	movs r0, #157
	bl 0x02009c2c
	bl 0x02008210
	ldr r0, [pc, #16]
	bl 0x02009b0c
	ldr r0, [pc, #16]
	bl 0x02009b14
.L_0200033c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000201
	.4byte 0x00001528
	.4byte 0x00000202
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	sub sp, #8
	movs r3, #59
	str r3, [sp, #4]
	movs r5, #21
	movs r1, #87
	movs r2, #2
	movs r3, #5
	movs r0, #41
	str r5, [sp, #0]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	movs r3, #24
	mov r10, r3
	movs r3, #62
	mov r9, r3
	mov r3, r10
	str r3, [sp, #0]
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #2
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x02009ae4
	movs r3, #55
	str r3, [sp, #4]
	mov r8, r3
	movs r0, #2
	movs r1, #94
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009ae4
	movs r6, #58
	movs r1, #87
	movs r2, #2
	movs r3, #5
	movs r0, #43
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009ae4
	movs r0, #4
	bl 0x02009ad4
	mov r3, r10
	str r3, [sp, #0]
	mov r3, r9
	str r3, [sp, #4]
	movs r0, #3
	movs r1, #93
	movs r2, #1
	movs r3, #1
	bl 0x02009ae4
	mov r3, r8
	str r3, [sp, #4]
	movs r0, #1
	movs r1, #94
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009ae4
	movs r0, #41
	movs r1, #87
	movs r2, #2
	movs r3, #5
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl 0x02009ae4
	movs r3, #13
	str r3, [sp, #4]
	movs r0, #21
	movs r1, #11
	movs r2, #2
	movs r3, #2
	str r5, [sp, #0]
	bl 0x02009aec
	movs r3, #14
	str r3, [sp, #4]
	movs r0, #19
	movs r1, #17
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	bl 0x02009aec
	sub sp, #-8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
	.global Func_02000460
	.thumb_func
Func_02000460:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009b04
	cmp r0, #0
	bne .L_02000460_0
	ldr r0, [pc, #44]
	bl 0x02009b04
	cmp r0, #0
	bne .L_02000460_0
	movs r1, #1
	ldr r0, [pc, #36]
	bl 0x02009afc
	movs r0, #157
	bl 0x02009c2c
	bl 0x02008384
	ldr r0, [pc, #16]
	bl 0x02009b0c
	ldr r0, [pc, #16]
	bl 0x02009b14
.L_02000460_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000202
	.4byte 0x00001528
	.4byte 0x00000201
	.global Func_020004a8
	.thumb_func
Func_020004a8:
	push {r5, lr}
	ldr r0, [pc, #860]
	bl 0x02009b04
	cmp r0, #0
	bne .L_020004a8_0
	b 0x02008800
.L_020004a8_0:
	ldr r0, [pc, #852]
	bl 0x02009b0c
	bl 0x02009b24
	movs r1, #144
	movs r2, #200
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x02009b6c
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009b44
	movs r2, #200
	lsls r2, r2, #1
	movs r1, #184
	movs r0, #12
	bl 0x02009b54
	movs r0, #12
	bl 0x02009b64
	movs r0, #12
	movs r1, #1
	bl 0x02009b74
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl 0x02009bbc
	movs r1, #1
	movs r0, #0
	bl 0x02009b8c
	movs r0, #30
	bl 0x02009b1c
	movs r0, #128
	movs r1, #128
	lsls r0, r0, #8
	lsls r1, r1, #5
	bl 0x02009be4
	movs r0, #192
	movs r1, #1
	movs r2, #216
	lsls r0, r0, #16
	negs r1, r1
	lsls r2, r2, #17
	movs r3, #1
	bl 0x02009bec
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_020004a8_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #13
	bl 0x02009b6c
.L_020004a8_1:
	movs r0, #13
	ldr r1, [pc, #712]
	ldr r2, [pc, #716]
	bl 0x02009b44
	movs r2, #232
	movs r1, #168
	lsls r2, r2, #1
	movs r0, #13
	bl 0x02009b54
	movs r0, #13
	bl 0x02009b64
	movs r1, #192
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq 0x0200857e
.L_02000574:
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #2
	bl 0x02009b6c
	movs r0, #2
	ldr r1, [pc, #652]
	ldr r2, [pc, #656]
	bl 0x02009b44
	movs r2, #244
	movs r1, #152
	lsls r2, r2, #1
	movs r0, #2
	bl 0x02009b54
	movs r0, #2
	bl 0x02009b64
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02000574_0
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #3
	bl 0x02009b6c
.L_02000574_0:
	movs r0, #3
	ldr r1, [pc, #592]
	ldr r2, [pc, #596]
	bl 0x02009b44
	movs r2, #244
	movs r1, #168
	lsls r2, r2, #1
	movs r0, #3
	bl 0x02009b54
	movs r0, #3
	bl 0x02009b64
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02000574_1
	ldr r1, [r0, #8]
	ldr r2, [r0, #16]
	movs r0, #1
	bl 0x02009b6c
.L_02000574_1:
	movs r0, #1
	ldr r1, [pc, #532]
	ldr r2, [pc, #536]
	bl 0x02009b44
	movs r2, #244
	movs r1, #184
	lsls r2, r2, #1
	movs r0, #1
	bl 0x02009b54
	movs r0, #1
	bl 0x02009b64
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #8
	movs r0, #1
	bl 0x02009bbc
	movs r0, #30
	bl 0x02009b1c
	movs r1, #1
	movs r0, #1
	bl 0x02009b8c
	ldr r5, [pc, #488]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r1, #0
	movs r0, #1
	bl 0x02009bb4
	movs r0, #30
	bl 0x02009b1c
	movs r1, #3
	movs r0, #3
	bl 0x02009b7c
	movs r0, #10
	bl 0x02009b1c
	adds r0, r5, #1
	bl 0x02009ba4
	movs r0, #3
	movs r1, #0
	bl 0x02009bb4
	movs r1, #129
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #70
	bl 0x02009bcc
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
	movs r2, #0
	movs r1, #2
	movs r0, #0
	bl 0x02009b94
	adds r0, r5, #2
	bl 0x02009ba4
	movs r1, #0
	movs r0, #2
	bl 0x02009bac
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
	movs r1, #0
	movs r0, #1
	movs r2, #0
	bl 0x02009b94
	movs r0, #30
	bl 0x02009b1c
	movs r0, #0
	movs r1, #0
	bl 0x02009b34
	cmp r0, #0
	bne .L_02000574_2
	adds r0, r5, #3
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	bl 0x02009bb4
	b .L_02000574_3
.L_02000574_2:
	adds r0, r5, #4
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	bl 0x02009bb4
.L_02000574_3:
	movs r1, #128
	movs r2, #70
	lsls r1, r1, #1
	movs r0, #13
	bl 0x02009bcc
	ldr r5, [pc, #316]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #13
	movs r1, #0
	bl 0x02009bb4
	movs r0, #0
	movs r1, #2
	bl 0x02009b84
	movs r0, #1
	movs r1, #2
	bl 0x02009b84
	movs r0, #2
	movs r1, #2
	bl 0x02009b84
	movs r0, #3
	movs r1, #2
	bl 0x02009b84
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #192
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #192
	movs r2, #0
	movs r0, #3
	lsls r1, r1, #8
	bl 0x02009bbc
	movs r0, #13
	movs r1, #2
	bl 0x02009b74
	movs r2, #16
	negs r2, r2
	movs r1, #0
	movs r0, #13
	bl 0x02009c24
	movs r0, #13
	bl 0x02009b64
	movs r1, #1
	movs r0, #13
	bl 0x02009b74
	adds r0, r5, #1
	bl 0x02009ba4
	movs r0, #13
	movs r1, #0
	bl 0x02009bb4
	movs r1, #128
	movs r2, #65
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02009bcc
	adds r0, r5, #2
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #13
	bl 0x02009b7c
	movs r0, #80
	bl 0x02009b1c
	movs r0, #12
	ldr r1, [pc, #136]
	ldr r2, [pc, #136]
	bl 0x02009b44
	movs r1, #13
	negs r1, r1
	movs r2, #0
	movs r0, #12
	bl 0x02009c24
	movs r0, #12
	bl 0x02009b64
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #129
	movs r2, #70
	lsls r1, r1, #1
	movs r0, #12
	adds r5, #3
	bl 0x02009bcc
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r2, #216
	movs r1, #168
	lsls r2, r2, #1
	movs r0, #12
	bl 0x02009b54
	movs r0, #40
	bl 0x02009b1c
	bl 0x02009c14
	bl 0x02009c1c
	movs r0, #20
	bl 0x02009b1c
	bl 0x02009b2c
	bl 0x02008828
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.2byte 0x0941
	.2byte 0x0000
	.2byte 0x094d
	.2byte 0x0000
	.4byte 0x00014ccc
	.4byte 0x0000a666
	.4byte 0x0000250d
	.4byte 0x00002512
	.4byte 0x00006666
	.4byte 0x00003333
	.global Func_02000828
	.thumb_func
Func_02000828:
	push {r5, lr}
	bl 0x02009b24
	movs r1, #200
	movs r2, #136
	movs r0, #1
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x02009b6c
	movs r1, #184
	movs r2, #136
	movs r0, #0
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x02009b6c
	movs r1, #168
	movs r2, #136
	movs r0, #3
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x02009b6c
	movs r1, #212
	movs r2, #132
	movs r0, #2
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x02009b6c
	movs r1, #200
	movs r2, #128
	movs r0, #13
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x02009b6c
	movs r1, #168
	movs r2, #128
	movs r0, #12
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl 0x02009b6c
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #192
	movs r0, #3
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl 0x02009bbc
	movs r0, #12
	movs r1, #1
	bl 0x02009b74
	movs r1, #0
	movs r0, #0
	bl 0x02009bdc
	movs r0, #1
	bl 0x02009ad4
	bl 0x02009adc
	movs r0, #1
	bl 0x02009ad4
	bl 0x02009c0c
	bl 0x02009c1c
	movs r0, #20
	bl 0x02009b1c
	movs r1, #3
	movs r0, #2
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	movs r1, #129
	movs r2, #70
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02009bcc
	ldr r5, [pc, #1016]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #13
	bl 0x02009b7c
	adds r0, r5, #1
	bl 0x02009ba4
	movs r0, #13
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #13
	movs r0, #2
	bl 0x02009b94
	movs r0, #30
	bl 0x02009b1c
	movs r1, #1
	movs r0, #2
	bl 0x02009b8c
	movs r0, #40
	bl 0x02009b1c
	adds r0, r5, #2
	bl 0x02009ba4
	ldr r0, [pc, #948]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #2
	movs r2, #0
	movs r0, #1
	bl 0x02009b94
	movs r0, #10
	bl 0x02009b1c
	movs r2, #80
	ldr r1, [pc, #924]
	movs r0, #1
	bl 0x02009bcc
	adds r0, r5, #3
	bl 0x02009ba4
	ldr r0, [pc, #916]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #1
	movs r2, #70
	bl 0x02009bcc
	movs r2, #0
	movs r1, #1
	movs r0, #2
	bl 0x02009b94
	adds r0, r5, #4
	bl 0x02009ba4
	ldr r0, [pc, #872]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #13
	bl 0x02009bbc
	movs r0, #60
	bl 0x02009b1c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #13
	bl 0x02009bbc
	movs r0, #60
	bl 0x02009b1c
	adds r0, r5, #5
	bl 0x02009ba4
	movs r1, #0
	ldr r0, [pc, #832]
	bl 0x02009bb4
	movs r0, #30
	bl 0x02009b1c
	movs r0, #0
	movs r1, #3
	bl 0x02009b74
	movs r0, #3
	movs r1, #3
	bl 0x02009b74
	movs r1, #3
	movs r0, #1
	bl 0x02009b74
	movs r0, #120
	bl 0x02009b1c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #13
	bl 0x02009bbc
	movs r0, #30
	bl 0x02009b1c
	movs r1, #132
	movs r2, #80
	lsls r1, r1, #1
	movs r0, #13
	bl 0x02009bcc
	adds r0, r5, #6
	bl 0x02009ba4
	ldr r0, [pc, #752]
	movs r1, #0
	bl 0x02009bb4
	movs r0, #2
	movs r1, #13
	movs r2, #0
	bl 0x02009b94
	movs r1, #132
	movs r2, #60
	movs r0, #2
	lsls r1, r1, #1
	bl 0x02009bcc
	movs r1, #3
	movs r0, #2
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	movs r0, #3
	ldr r1, [pc, #712]
	movs r2, #40
	bl 0x02009bcc
	movs r2, #0
	movs r1, #12
	movs r0, #3
	bl 0x02009b94
	adds r0, r5, #7
	bl 0x02009ba4
	ldr r0, [pc, #692]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #128
	movs r0, #13
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009bcc
	movs r2, #0
	movs r1, #12
	movs r0, #13
	bl 0x02009b94
	adds r0, r5, #0
	adds r0, #8
	bl 0x02009ba4
	ldr r0, [pc, #644]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #129
	movs r2, #70
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02009bcc
	adds r0, r5, #0
	adds r0, #9
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #12
	bl 0x02009b8c
	movs r0, #20
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #10
	bl 0x02009ba4
	ldr r0, [pc, #600]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #2
	movs r2, #0
	movs r0, #13
	bl 0x02009b9c
	movs r0, #60
	bl 0x02009b1c
	movs r0, #13
	movs r1, #12
	movs r2, #0
	bl 0x02009b94
	movs r2, #0
	movs r0, #2
	movs r1, #12
	bl 0x02009b94
	movs r1, #4
	movs r0, #12
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #11
	bl 0x02009ba4
	ldr r0, [pc, #532]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #1
	bl 0x02009b8c
	movs r0, #20
	bl 0x02009b1c
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x02009b94
	adds r0, r5, #0
	adds r0, #12
	bl 0x02009ba4
	ldr r0, [pc, #476]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #1
	movs r0, #12
	bl 0x02009b94
	movs r0, #20
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #13
	bl 0x02009ba4
	ldr r0, [pc, #460]
	movs r1, #0
	bl 0x02009bb4
	movs r0, #3
	movs r1, #12
	movs r2, #0
	bl 0x02009b94
	movs r2, #70
	ldr r1, [pc, #432]
	movs r0, #3
	bl 0x02009bcc
	adds r0, r5, #0
	adds r0, #14
	bl 0x02009ba4
	ldr r0, [pc, #420]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #12
	bl 0x02009b7c
	movs r0, #40
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #15
	bl 0x02009ba4
	ldr r0, [pc, #396]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #128
	movs r2, #70
	lsls r1, r1, #1
	movs r0, #13
	bl 0x02009bcc
	adds r0, r5, #0
	adds r0, #16
	bl 0x02009ba4
	ldr r0, [pc, #356]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r0, #12
	movs r1, #13
	bl 0x02009b94
	movs r1, #3
	movs r0, #12
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #17
	bl 0x02009ba4
	ldr r0, [pc, #328]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #13
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #18
	bl 0x02009ba4
	ldr r0, [pc, #284]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #132
	movs r2, #70
	movs r0, #12
	lsls r1, r1, #1
	bl 0x02009bcc
	movs r1, #1
	movs r0, #2
	bl 0x02009b84
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #19
	bl 0x02009ba4
	ldr r0, [pc, #232]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r0, #12
	movs r1, #2
	bl 0x02009b94
	movs r1, #3
	movs r0, #12
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #20
	bl 0x02009ba4
	ldr r0, [pc, #192]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #1
	bl 0x02009b8c
	adds r0, r5, #0
	adds r0, #21
	bl 0x02009ba4
	ldr r0, [pc, #176]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl 0x02009bbc
	movs r1, #4
	movs r0, #12
	bl 0x02009b7c
	adds r0, r5, #0
	adds r0, #22
	bl 0x02009ba4
	ldr r0, [pc, #156]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #3
	bl 0x02009b8c
	adds r0, r5, #0
	adds r0, #23
	bl 0x02009ba4
	ldr r0, [pc, #128]
	movs r1, #0
	bl 0x02009bb4
	movs r0, #12
	movs r1, #3
	movs r2, #0
	bl 0x02009b94
	movs r1, #128
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02009bcc
	adds r0, r5, #0
	adds r0, #24
	bl 0x02009ba4
	ldr r0, [pc, #92]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #13
	movs r0, #2
	bl 0x02009b9c
	movs r0, #20
	bl 0x02009b1c
	movs r1, #3
	movs r0, #2
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #25
	bl 0x02009ba4
	ldr r0, [pc, #24]
	movs r1, #0
	bl 0x02009bb4
	movs r0, #13
	movs r1, #3
	bl 0x02009b7c
	movs r0, #13
	movs r1, #12
	b .L_02000828_0
	.4byte 0x00002516
	.4byte 0x00004002
	.4byte 0x00000107
	.4byte 0x00004001
	.4byte 0x0000400d
	.4byte 0x00000101
	.4byte 0x00004003
	.4byte 0x0000400c
.L_02000828_0:
	movs r2, #0
	bl 0x02009b94
	movs r2, #0
	movs r1, #12
	movs r0, #2
	bl 0x02009b94
	movs r0, #20
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #26
	bl 0x02009ba4
	ldr r0, [pc, #632]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02009bd4
	adds r0, r5, #0
	adds r0, #27
	bl 0x02009ba4
	ldr r0, [pc, #604]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x02009b94
	adds r0, r5, #0
	adds r0, #28
	bl 0x02009ba4
	ldr r0, [pc, #584]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #70
	movs r0, #12
	ldr r1, [pc, #576]
	bl 0x02009bcc
	movs r1, #4
	movs r0, #3
	bl 0x02009b7c
	adds r0, r5, #0
	adds r0, #29
	bl 0x02009ba4
	ldr r0, [pc, #556]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #12
	bl 0x02009b8c
	adds r0, r5, #0
	adds r0, #30
	bl 0x02009ba4
	ldr r0, [pc, #520]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #12
	bl 0x02009b8c
	adds r0, r5, #0
	adds r0, #31
	bl 0x02009ba4
	movs r1, #0
	ldr r0, [pc, #512]
	bl 0x02009bb4
	movs r0, #30
	bl 0x02009b1c
	movs r2, #0
	movs r1, #13
	movs r0, #12
	bl 0x02009b94
	movs r0, #60
	bl 0x02009b1c
	movs r1, #4
	movs r0, #12
	bl 0x02009b7c
	adds r0, r5, #0
	adds r0, #32
	bl 0x02009ba4
	ldr r0, [pc, #452]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #13
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #60
	bl 0x02009bcc
	movs r1, #192
	movs r2, #192
	movs r0, #12
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x02009b44
	movs r2, #132
	movs r1, #144
	lsls r2, r2, #2
	movs r0, #12
	bl 0x02009b54
	movs r0, #12
	bl 0x02009b64
	movs r2, #140
	movs r1, #168
	lsls r2, r2, #2
	movs r0, #12
	bl 0x02009b54
	movs r0, #12
	bl 0x02009b64
	movs r0, #60
	bl 0x02009b1c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #12
	bl 0x02009bbc
	movs r0, #40
	bl 0x02009b1c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #12
	bl 0x02009bbc
	movs r0, #40
	bl 0x02009b1c
	movs r1, #192
	lsls r1, r1, #6
	movs r2, #0
	movs r0, #12
	bl 0x02009bbc
	movs r0, #40
	bl 0x02009b1c
	movs r2, #132
	movs r1, #144
	lsls r2, r2, #2
	movs r0, #12
	bl 0x02009b54
	movs r0, #12
	bl 0x02009b64
	movs r2, #244
	lsls r2, r2, #1
	movs r1, #168
	movs r0, #12
	bl 0x02009b54
	adds r0, r5, #0
	adds r0, #33
	bl 0x02009ba4
	movs r1, #0
	ldr r0, [pc, #276]
	bl 0x02009bb4
	movs r0, #12
	bl 0x02009b64
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl 0x02009bbc
	movs r0, #12
	movs r1, #1
	bl 0x02009b74
	movs r1, #4
	movs r0, #12
	bl 0x02009b7c
	movs r0, #20
	bl 0x02009b1c
	adds r0, r5, #0
	adds r0, #34
	bl 0x02009ba4
	movs r1, #0
	movs r0, #12
	bl 0x02009bb4
	movs r0, #20
	bl 0x02009b1c
	movs r1, #0
	movs r2, #0
	movs r0, #1
	bl 0x02009b9c
	movs r0, #60
	bl 0x02009b1c
	movs r1, #192
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #192
	movs r0, #1
	lsls r1, r1, #8
	movs r2, #0
	bl 0x02009bbc
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02009bcc
	adds r0, r5, #0
	adds r0, #35
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #3
	bl 0x02009b8c
	adds r0, r5, #0
	adds r0, #36
	bl 0x02009ba4
	ldr r0, [pc, #116]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #2
	bl 0x02009bcc
	adds r0, r5, #0
	adds r0, #37
	bl 0x02009ba4
	ldr r0, [pc, #96]
	movs r1, #0
	bl 0x02009bb4
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
	movs r1, #129
	lsls r1, r1, #1
	movs r2, #40
	movs r0, #2
	bl 0x02009bcc
	adds r0, r5, #0
	adds r0, #38
	bl 0x02009ba4
	movs r1, #0
	ldr r0, [pc, #40]
	bl 0x02009bac
	movs r0, #0
	movs r1, #0
	bl 0x02009b34
	cmp r0, #0
	bne .L_02000828_1
	adds r0, r5, #0
	adds r0, #39
	bl 0x02009ba4
	ldr r0, [pc, #8]
	movs r1, #0
	bl 0x02009bb4
	b .L_02000828_2
	.2byte 0x0000
	.4byte 0x0000400c
	.4byte 0x00004001
	.4byte 0x00000105
	.4byte 0x00004003
	.4byte 0x0000400d
	.4byte 0x00004002
.L_02000828_1:
	adds r0, r5, #0
	adds r0, #40
	bl 0x02009ba4
	ldr r0, [pc, #436]
	movs r1, #0
	bl 0x02009bb4
.L_02000828_2:
	movs r1, #192
	movs r0, #12
	lsls r1, r1, #6
	movs r2, #0
	bl 0x02009bbc
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x02009b94
	movs r0, #1
	movs r1, #12
	movs r2, #0
	bl 0x02009b94
	movs r0, #3
	movs r1, #12
	movs r2, #0
	bl 0x02009b94
	movs r0, #2
	movs r1, #12
	movs r2, #0
	bl 0x02009b94
	movs r0, #13
	movs r1, #12
	movs r2, #0
.L_02001026:
	bl 0x02009b94
	movs r2, #60
	ldr r1, [pc, #368]
	movs r0, #13
	bl 0x02009bcc
	ldr r5, [pc, #364]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r1, #0
	ldr r0, [pc, #360]
	bl 0x02009bb4
	movs r0, #20
	bl 0x02009b1c
	movs r1, #132
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #12
	bl 0x02009bcc
	adds r0, r5, #1
	bl 0x02009ba4
	ldr r0, [pc, #316]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #60
	ldr r1, [pc, #312]
	movs r0, #3
	bl 0x02009bcc
	adds r0, r5, #2
	bl 0x02009ba4
	ldr r0, [pc, #308]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #3
	movs r0, #12
	bl 0x02009b94
	movs r0, #10
	bl 0x02009b1c
	movs r0, #12
	movs r1, #3
	bl 0x02009b7c
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #12
	bl 0x02009bbc
	adds r0, r5, #3
	bl 0x02009ba4
	ldr r0, [pc, #244]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #12
	movs r0, #2
	bl 0x02009b94
	adds r0, r5, #4
	bl 0x02009ba4
	movs r1, #0
	ldr r0, [pc, #236]
	bl 0x02009bb4
	movs r0, #10
	bl 0x02009b1c
	movs r1, #4
	movs r0, #12
	bl 0x02009b7c
	adds r0, r5, #5
	bl 0x02009ba4
	ldr r0, [pc, #192]
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r2, #0
	movs r0, #0
	bl 0x02009b9c
	movs r0, #60
	bl 0x02009b1c
	movs r0, #0
	movs r1, #12
	movs r2, #0
	bl 0x02009b94
	movs r2, #0
	movs r1, #12
	movs r0, #1
	bl 0x02009b94
	adds r0, r5, #6
	bl 0x02009ba4
	ldr r0, [pc, #164]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r0, #12
	movs r1, #1
	bl 0x02009b94
	movs r1, #3
	movs r0, #12
	bl 0x02009b7c
	adds r5, #7
	movs r0, #20
	bl 0x02009b1c
	adds r0, r5, #0
	bl 0x02009ba4
	ldr r0, [pc, #100]
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #2
	movs r0, #13
	bl 0x02009b9c
	movs r0, #60
	bl 0x02009b1c
	movs r0, #13
	movs r1, #3
	bl 0x02009b74
	movs r1, #3
	movs r0, #2
	bl 0x02009b74
	movs r0, #60
	bl 0x02009b1c
	movs r0, #0
	movs r1, #3
	bl 0x02009b74
	movs r0, #1
	movs r1, #3
	bl 0x02009b74
	movs r0, #3
	movs r1, #3
	bl 0x02009b74
	movs r2, #0
	movs r0, #12
	movs r1, #0
	bl 0x02009b94
	movs r1, #3
	movs r0, #12
	bl 0x02009b74
	movs r0, #60
	bl 0x02009b1c
	bl 0x020091b8
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x0000400c
	.4byte 0x00000101
	.4byte 0x0000253f
	.4byte 0x0000400d
	.4byte 0x00004003
	.4byte 0x00004002
	.4byte 0x00004001
	.global Func_020011b8
	.thumb_func
Func_020011b8:
	push {r5, lr}
	ldr r5, [pc, #232]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	adds r5, #1
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x02009b94
	adds r0, r5, #0
	bl 0x02009ba4
	movs r1, #0
	movs r0, #1
	bl 0x02009bac
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
	movs r0, #3
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
.L_020011b8_8:
	bl 0x020092ac
	lsls r0, r0, #24
	cmp r0, #0
	beq .L_020011b8_0
.L_020011b8_7:
	bl 0x02009320
	lsls r0, r0, #24
	cmp r0, #0
	beq .L_020011b8_1
	bl 0x0200941c
	lsls r0, r0, #24
	movs r5, #0
	cmp r0, #0
	bne .L_020011b8_2
.L_020011b8_6:
	movs r5, #1
.L_020011b8_4:
	bl 0x0200934c
	movs r0, #0
	movs r1, #0
	bl 0x02009b34
	cmp r0, #0
	beq .L_020011b8_1
.L_020011b8_2:
	bl 0x02009394
	lsls r0, r0, #24
	cmp r0, #0
	bne .L_020011b8_3
	cmp r5, #0
	bne .L_020011b8_4
	b .L_020011b8_3
.L_020011b8_0:
	bl 0x020092c4
	lsls r0, r0, #24
	cmp r0, #0
	beq .L_020011b8_5
	bl 0x020092f0
	lsls r0, r0, #24
	cmp r0, #0
	beq .L_020011b8_6
	b .L_020011b8_3
.L_020011b8_5:
	bl 0x02009368
	lsls r0, r0, #24
	cmp r0, #0
	bne .L_020011b8_7
	ldr r5, [pc, #56]
	adds r0, r5, #0
	bl 0x02009ba4
	adds r5, #1
	movs r1, #0
	movs r0, #2
	bl 0x02009bb4
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	bl 0x02009bac
	b .L_020011b8_8
.L_020011b8_1:
	bl 0x02009760
	b .L_020011b8_9
.L_020011b8_3:
	bl 0x0200931c
	bl 0x02009448
.L_020011b8_9:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x00002547
	.4byte 0x0000254b
	.global Func_020012ac
	.thumb_func
Func_020012ac:
	push {lr}
	movs r1, #0
	movs r0, #0
	bl 0x02009b34
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
	pop {r1}
	bx r1
	.global Func_020012c4
	.thumb_func
Func_020012c4:
	push {lr}
	ldr r0, [pc, #36]
	bl 0x02009ba4
	movs r1, #0
	movs r0, #1
	bl 0x02009bac
	movs r1, #0
	movs r0, #0
	bl 0x02009b34
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00002549
	.global Func_020012f0
	.thumb_func
Func_020012f0:
	push {lr}
	ldr r0, [pc, #36]
	bl 0x02009ba4
	movs r1, #0
	movs r0, #1
	bl 0x02009bac
	movs r1, #0
	movs r0, #0
	bl 0x02009b34
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000254d
	.global Func_0200131c
	.thumb_func
Func_0200131c:
	movs r0, #1
	bx lr
	.global Func_02001320
	.thumb_func
Func_02001320:
	push {lr}
	ldr r0, [pc, #36]
	bl 0x02009ba4
	movs r1, #0
	movs r0, #12
	bl 0x02009bac
	movs r1, #0
	movs r0, #0
	bl 0x02009b34
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x00002557
	.global Func_0200134c
	.thumb_func
Func_0200134c:
	push {lr}
	ldr r0, [pc, #20]
	bl 0x02009ba4
	movs r1, #0
	movs r0, #1
	bl 0x02009bac
	movs r0, #1
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000255d
	.global Func_02001368
	.thumb_func
Func_02001368:
	push {lr}
	ldr r0, [pc, #36]
	bl 0x02009ba4
	movs r1, #0
	movs r0, #3
	bl 0x02009bac
	movs r1, #0
	movs r0, #0
	bl 0x02009b34
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000254a
	.global Func_02001394
	.thumb_func
Func_02001394:
	push {r5, lr}
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x02009b94
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #2
	bl 0x02009bcc
	ldr r5, [pc, #104]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #2
	movs r1, #0
	bl 0x02009bb4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #12
	bl 0x02009bbc
	movs r0, #30
	bl 0x02009b1c
	movs r1, #4
	movs r0, #12
	bl 0x02009b7c
	adds r0, r5, #1
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r1, #129
	movs r2, #60
	lsls r1, r1, #1
	movs r0, #3
	adds r5, #2
	bl 0x02009bcc
	adds r0, r5, #0
	bl 0x02009ba4
	movs r1, #0
	movs r0, #3
	bl 0x02009bac
	movs r1, #0
	movs r0, #0
	bl 0x02009b34
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x0000255e
	.global Func_0200141c
	.thumb_func
Func_0200141c:
	push {lr}
	ldr r0, [pc, #36]
	bl 0x02009ba4
	movs r1, #0
	movs r0, #1
	bl 0x02009bac
	movs r1, #0
	movs r0, #0
	bl 0x02009b34
	negs r3, r0
	orrs r3, r0
	lsrs r3, r3, #31
	movs r0, #1
	subs r0, r0, r3
	pop {r1}
	bx r1
	.2byte 0x0000
	.4byte 0x0000255c
	.global Func_02001448
	.thumb_func
Func_02001448:
	push {r5, lr}
	movs r0, #1
	ldr r1, [pc, #772]
	movs r2, #60
	bl 0x02009bcc
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x02009b94
	ldr r5, [pc, #760]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	bl 0x02009bb4
	movs r1, #1
	movs r0, #3
	bl 0x02009b8c
	adds r0, r5, #1
	bl 0x02009ba4
	movs r0, #3
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r1, #13
	movs r0, #2
	bl 0x02009b94
	movs r0, #60
	bl 0x02009b1c
	adds r0, r5, #2
	bl 0x02009ba4
	movs r0, #2
	movs r1, #0
	bl 0x02009bb4
	movs r0, #13
	movs r1, #2
	movs r2, #0
	bl 0x02009b94
	movs r2, #70
	ldr r1, [pc, #676]
	movs r0, #13
	bl 0x02009bcc
	adds r0, r5, #3
	bl 0x02009ba4
	movs r0, #13
	movs r1, #0
	bl 0x02009bb4
	movs r1, #4
	movs r0, #12
	bl 0x02009b7c
	adds r0, r5, #4
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r1, #192
	movs r2, #0
	lsls r1, r1, #6
	movs r0, #12
	bl 0x02009bbc
	movs r0, #20
	bl 0x02009b1c
	movs r0, #12
	movs r1, #3
	bl 0x02009b7c
	movs r0, #0
	movs r1, #3
	bl 0x02009b74
	movs r0, #1
	movs r1, #3
	bl 0x02009b74
	movs r0, #2
	movs r1, #3
	bl 0x02009b74
	movs r1, #3
	movs r0, #3
	bl 0x02009b74
	movs r0, #60
	bl 0x02009b1c
	movs r1, #16
	movs r2, #0
	negs r1, r1
	movs r0, #13
	bl 0x02009b5c
	movs r0, #13
	bl 0x02009b64
	movs r1, #1
	movs r0, #13
	bl 0x02009b74
	movs r0, #40
	bl 0x02009b1c
	movs r0, #13
	movs r1, #3
	bl 0x02009b7c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #13
	bl 0x02009bbc
	movs r0, #30
	bl 0x02009b1c
	movs r0, #0
	movs r1, #3
	bl 0x02009b74
	movs r0, #1
	movs r1, #3
	bl 0x02009b74
	movs r0, #2
	movs r1, #3
	bl 0x02009b74
	movs r0, #3
	movs r1, #3
	bl 0x02009b74
	movs r2, #132
	movs r1, #156
	lsls r2, r2, #2
	movs r0, #12
	bl 0x02009b54
	movs r0, #20
	bl 0x02009b1c
	movs r2, #132
	movs r1, #164
	lsls r2, r2, #2
	movs r0, #13
	bl 0x02009b54
	movs r0, #12
	bl 0x02009b64
	movs r2, #160
	movs r1, #168
	lsls r2, r2, #2
	movs r0, #12
	bl 0x02009b54
	movs r0, #13
	bl 0x02009b64
	movs r2, #160
	movs r0, #13
	movs r1, #168
	lsls r2, r2, #2
	bl 0x02009b54
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x02009bbc
	movs r0, #20
	bl 0x02009b1c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x02009bbc
	movs r0, #60
	bl 0x02009b1c
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009b6c
	movs r2, #0
	movs r1, #0
	movs r0, #12
	bl 0x02009b6c
	movs r0, #110
	bl 0x02009b1c
	adds r0, r5, #5
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #3
	bl 0x02009b7c
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #6
	bl 0x02009ba4
	movs r0, #3
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #2
	bl 0x02009b7c
	adds r0, r5, #7
	bl 0x02009ba4
	movs r1, #0
	movs r0, #2
	bl 0x02009bb4
	movs r0, #140
	bl 0x02009b1c
	adds r5, #8
	movs r2, #0
	movs r1, #0
	movs r0, #1
	bl 0x02009b94
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	bl 0x02009bb4
	movs r0, #1
	movs r1, #2
	bl 0x02009b74
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02001448_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009b4c
.L_02001448_0:
	movs r0, #1
	bl 0x02009b64
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009b6c
	movs r0, #2
	movs r1, #2
	bl 0x02009b74
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02001448_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x02009b4c
.L_02001448_1:
	movs r0, #2
	bl 0x02009b64
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x02009b6c
	movs r0, #3
	movs r1, #2
	bl 0x02009b74
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02001448_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x02009b4c
.L_02001448_2:
	movs r0, #3
	bl 0x02009b64
	movs r2, #0
	movs r1, #0
	movs r0, #3
	bl 0x02009b6c
	movs r0, #30
	bl 0x02009b1c
	movs r0, #0
	movs r1, #1
	bl 0x02009bfc
	bl 0x02009bf4
	movs r0, #0
	movs r1, #0
	bl 0x02009bdc
	ldr r0, [pc, #20]
	bl 0x02009b0c
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00000105
	.4byte 0x0000254e
	.4byte 0x0000094f
	.global Func_02001760
	.thumb_func
Func_02001760:
	push {r5, lr}
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #1
	bl 0x02009bbc
	ldr r5, [pc, #664]
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #1
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #2
	bl 0x02009b7c
	adds r0, r5, #1
	bl 0x02009ba4
	movs r0, #2
	movs r1, #0
	bl 0x02009bb4
	movs r2, #0
	movs r0, #13
	movs r1, #2
	bl 0x02009b94
	movs r1, #3
	movs r0, #13
	bl 0x02009b7c
	movs r0, #20
	bl 0x02009b1c
	adds r0, r5, #2
	bl 0x02009ba4
	movs r0, #13
	movs r1, #0
	bl 0x02009bb4
	movs r1, #192
	movs r2, #0
	movs r0, #12
	lsls r1, r1, #6
	bl 0x02009bbc
	movs r1, #3
	movs r0, #12
	bl 0x02009b7c
	adds r5, #3
	movs r0, #30
	bl 0x02009b1c
	adds r0, r5, #0
	bl 0x02009ba4
	movs r0, #12
	movs r1, #0
	bl 0x02009bb4
	movs r1, #3
	movs r0, #13
	bl 0x02009b7c
	movs r0, #20
	bl 0x02009b1c
	movs r0, #0
	movs r1, #3
	bl 0x02009b74
	movs r0, #1
	movs r1, #3
	bl 0x02009b74
	movs r0, #2
	movs r1, #3
	bl 0x02009b74
	movs r1, #3
	movs r0, #3
	bl 0x02009b74
	movs r0, #80
	bl 0x02009b1c
	movs r1, #16
	movs r2, #0
	negs r1, r1
	movs r0, #13
	bl 0x02009b5c
	movs r0, #13
	bl 0x02009b64
	movs r1, #1
	movs r0, #13
	bl 0x02009b74
	movs r0, #40
	bl 0x02009b1c
	movs r0, #13
	movs r1, #3
	bl 0x02009b7c
	movs r1, #160
	movs r2, #0
	lsls r1, r1, #7
	movs r0, #13
	bl 0x02009bbc
	movs r0, #30
	bl 0x02009b1c
	movs r0, #0
	movs r1, #3
	bl 0x02009b74
	movs r0, #1
	movs r1, #3
	bl 0x02009b74
	movs r0, #2
	movs r1, #3
	bl 0x02009b74
	movs r0, #3
	movs r1, #3
	bl 0x02009b74
	movs r2, #132
	movs r1, #152
	lsls r2, r2, #2
	movs r0, #12
	bl 0x02009b54
	movs r0, #20
	bl 0x02009b1c
	movs r2, #132
	movs r1, #160
	lsls r2, r2, #2
	movs r0, #13
	bl 0x02009b54
	movs r0, #12
	bl 0x02009b64
	movs r2, #160
	movs r1, #168
	lsls r2, r2, #2
	movs r0, #12
	bl 0x02009b54
	movs r0, #13
	bl 0x02009b64
	movs r2, #160
	movs r0, #13
	movs r1, #168
	lsls r2, r2, #2
	bl 0x02009b54
	movs r1, #160
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #160
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #160
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x02009bbc
	movs r0, #20
	bl 0x02009b1c
	movs r1, #128
	movs r0, #0
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	movs r0, #2
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	movs r0, #3
	lsls r1, r1, #7
	movs r2, #0
	bl 0x02009bbc
	movs r1, #128
	lsls r1, r1, #7
	movs r2, #0
	movs r0, #1
	bl 0x02009bbc
	movs r0, #200
	bl 0x02009b1c
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl 0x02009b6c
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl 0x02009b6c
	movs r0, #1
	movs r1, #2
	bl 0x02009b74
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02001760_0
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #1
	bl 0x02009b4c
.L_02001760_0:
	movs r0, #1
	bl 0x02009b64
	movs r0, #1
	movs r1, #0
	movs r2, #0
	bl 0x02009b6c
	movs r0, #2
	movs r1, #2
	bl 0x02009b74
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02001760_1
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #2
	bl 0x02009b4c
.L_02001760_1:
	movs r0, #2
	bl 0x02009b64
	movs r0, #2
	movs r1, #0
	movs r2, #0
	bl 0x02009b6c
	movs r0, #3
	movs r1, #2
	bl 0x02009b74
	movs r0, #0
	bl 0x02009b3c
	cmp r0, #0
	beq .L_02001760_2
	movs r3, #10
	ldrsh r1, [r0, r3]
	movs r3, #18
	ldrsh r2, [r0, r3]
	movs r0, #3
	bl 0x02009b4c
.L_02001760_2:
	movs r0, #3
	bl 0x02009b64
	movs r1, #0
	movs r2, #0
	movs r0, #3
	bl 0x02009b6c
	movs r0, #30
	bl 0x02009b1c
	movs r1, #16
	movs r2, #0
	negs r1, r1
	movs r0, #0
	bl 0x02009b5c
	movs r0, #0
	bl 0x02009b64
	movs r0, #0
	movs r1, #1
	bl 0x02009bfc
	movs r2, #160
	movs r1, #168
	lsls r2, r2, #2
	movs r0, #0
	bl 0x02009b54
	movs r0, #60
	bl 0x02009b1c
	bl 0x02009c14
	movs r0, #3
	bl 0x02009c04
	pop {r5}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00002558
	.global Func_02001a0c
	.thumb_func
Func_02001a0c:
	push {r5, lr}
	ldr r3, [pc, #168]
	movs r2, #224
	ldr r3, [r3]
	movs r5, #129
	lsls r2, r2, #1
	lsls r5, r5, #2
	str r5, [r3, r2]
	ldr r3, [pc, #156]
	ldrsh r2, [r3, r2]
	ldr r3, [pc, #156]
	cmp r2, r3
	bne .L_02001a0c_0
	movs r0, #8
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #9
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #10
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #11
	bl 0x02009b3c
	movs r1, #0
	bl 0x02009af4
	movs r0, #11
	bl 0x02009b3c
	ldr r3, [pc, #100]
	str r3, [r0, #28]
	ldr r0, [pc, #100]
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_1
	bl 0x02008210
.L_02001a0c_1:
	ldr r0, [pc, #92]
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_2
	bl 0x02008384
.L_02001a0c_2:
	movs r0, #128
	lsls r0, r0, #2
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_3
	bl 0x020080d4
.L_02001a0c_3:
	ldr r0, [pc, #64]
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_4
	movs r0, #11
	movs r1, #5
	bl 0x02009b74
.L_02001a0c_4:
	adds r0, r5, #0
	bl 0x02009b04
	cmp r0, #0
	beq .L_02001a0c_0
	movs r0, #9
	movs r1, #5
	bl 0x02009b74
.L_02001a0c_0:
	movs r0, #0
	pop {r5}
	pop {r1}
	bx r1
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000006a
	.4byte 0x0000f333
	.4byte 0x00000201
	.4byte 0x00000202
	.4byte 0x00000203
	.include "games/THE BROKEN SEAL/SRC/FIELD/RUNPA_DOU/IMPORT.INC"
AlchemyData_02001c34:
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0xc0000278
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000a8
	.4byte 0xc0000278
	.4byte 0x00580000
	.4byte 0x02880038
	.4byte 0x00000288
	.4byte 0xffff0002
	.4byte 0x00000228
	.4byte 0xc0000148
	.4byte 0x00580000
	.4byte 0x02880038
	.4byte 0x00000288
	.4byte 0xffff0003
	.4byte 0x00000228
	.4byte 0xc0000148
	.4byte 0x00580000
	.4byte 0x02880038
	.4byte 0x000001e8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000006a
	.4byte 0x00131002
	.4byte 0x00203068
	.4byte 0x00331002
	.4byte 0x000001ff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00004000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00028000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
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
	.4byte 0x00000002
	.4byte 0x094d000a
	.4byte 0x020084a9
	.4byte 0x00000003
	.4byte 0xffff0003
	.4byte 0x0200833d
	.4byte 0x00000003
	.4byte 0xffff0004
	.4byte 0x02008461
	.4byte 0x00001815
	.4byte 0xffff0008
	.4byte 0x0200807d
	.4byte 0x00001815
	.4byte 0xffff0009
	.4byte 0x02008095
	.4byte 0x00001815
	.4byte 0xffff000a
	.4byte 0x020081b1
	.4byte 0x00001815
	.4byte 0xffff000b
	.4byte 0x02008201
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
