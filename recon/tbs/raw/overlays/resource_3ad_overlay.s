.syntax unified
	.thumb
	.section .text.x02008044,"ax",%progbits
	.balign 4
	.global Scene_GetPlacements
	.thumb_func
Scene_GetPlacements:
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
	.section .text.x020084a8,"ax",%progbits
	.balign 4
	.global Reunion_Begin
	.thumb_func
Reunion_Begin:
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
	.global Reunion_Converse
	.thumb_func
Reunion_Converse:
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
	.section .text.x02009394,"ax",%progbits
	.balign 4
	.global Bunza_CannotWait
	.thumb_func
Bunza_CannotWait:
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
	.section .text.x02009448,"ax",%progbits
	.balign 4
	.global Party_StaysBehind
	.thumb_func
Party_StaysBehind:
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
	.global Party_RidesWagon
	.thumb_func
Party_RidesWagon:
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
	.global Scene_Initialize
	.thumb_func
Scene_Initialize:
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
