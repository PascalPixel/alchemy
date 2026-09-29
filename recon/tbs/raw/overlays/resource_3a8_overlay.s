.syntax unified
	.thumb
	.section .text.x02008590,"ax",%progbits
	.align 2
	.global Func_02000590
	.thumb_func
Func_02000590:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, [pc, #260]
	ldr r7, [r3]
	bl 0x0200bbf4
	movs r0, #145
	lsls r0, r0, #4
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_02000590_0
	bl 0x0200951c
.L_02000590_0:
	ldr r0, [pc, #240]
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_02000590_1
	bl 0x0200951c
.L_02000590_1:
	ldr r0, [pc, #228]
	bl 0x0200bc04
	movs r1, #252
	movs r2, #136
	movs r0, #20
	lsls r1, r1, #16
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #142
	movs r2, #132
	movs r0, #27
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #142
	movs r2, #140
	movs r0, #28
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #150
	movs r2, #132
	movs r0, #29
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #150
	movs r2, #140
	movs r0, #30
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #158
	movs r2, #132
	movs r0, #32
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #158
	movs r2, #140
	movs r0, #31
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #166
	movs r2, #132
	movs r0, #33
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #166
	movs r2, #140
	movs r0, #34
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl 0x0200bc74
	movs r1, #182
	movs r2, #136
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #21
	bl 0x0200bc74
	movs r0, #17
	bl 0x0200bd7c
	movs r0, #20
	bl 0x0200bcec
	movs r2, #0
	movs r1, #1
	ldr r0, [pc, #72]
	bl 0x0200bbc4
	movs r0, #9
	bl 0x0200bd7c
	movs r0, #10
	bl 0x0200bbec
	movs r0, #0
	movs r1, #2
	bl 0x0200bc9c
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_02000590_2
	ldr r0, [pc, #36]
	ldr r1, [pc, #36]
	bl 0x0200bd0c
	movs r1, #224
	movs r0, #0
	lsls r1, r1, #8
	movs r2, #20
	bl 0x0200bce4
	b .L_02000590_3
	.4byte 0x03001ebc
	.4byte 0x00000911
	.4byte 0x0200c948
	.4byte 0x00001a91
	.4byte 0x00026666
	.4byte 0x00004ccc
.L_02000590_2:
	ldr r0, [pc, #1012]
	ldr r1, [pc, #1016]
	bl 0x0200bd0c
	movs r0, #0
	movs r1, #0
	movs r2, #20
	bl 0x0200bce4
.L_02000590_3:
	movs r0, #20
	ldr r1, [pc, #1000]
	ldr r2, [pc, #1004]
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #27
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #28
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r0, #29
	ldr r1, [pc, #972]
	ldr r2, [pc, #972]
	bl 0x0200bc2c
	movs r0, #30
	ldr r1, [pc, #960]
	ldr r2, [pc, #964]
	bl 0x0200bc2c
	movs r0, #32
	ldr r1, [pc, #960]
	ldr r2, [pc, #960]
	bl 0x0200bc2c
	movs r0, #31
	ldr r1, [pc, #948]
	ldr r2, [pc, #952]
	bl 0x0200bc2c
	movs r0, #33
	ldr r1, [pc, #948]
	ldr r2, [pc, #948]
	bl 0x0200bc2c
	movs r0, #34
	ldr r1, [pc, #936]
	ldr r2, [pc, #940]
	bl 0x0200bc2c
	ldr r2, [pc, #936]
	movs r0, #21
	ldr r1, [pc, #936]
	bl 0x0200bc2c
	ldr r5, [pc, #936]
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #27
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #28
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #29
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #30
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #32
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #31
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #33
	adds r1, r5, #0
	bl 0x0200bc34
	adds r1, r5, #0
	movs r0, #34
	bl 0x0200bc34
	movs r0, #21
	bl 0x0200bc1c
	adds r6, r0, #0
	movs r3, #0
	adds r6, #100
	strh r3, [r6]
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #186
	movs r1, #1
	movs r2, #136
	negs r1, r1
	lsls r2, r2, #17
	lsls r0, r0, #16
	movs r3, #1
	bl 0x0200bd14
	movs r0, #20
	bl 0x0200bc3c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
.L_02000590_4:
	movs r0, #1
	bl 0x0200bb14
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #0
	beq .L_02000590_4
	movs r0, #40
	bl 0x0200bbec
	movs r0, #27
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #160
	lsls r1, r1, #7
	movs r2, #20
	movs r0, #27
	bl 0x0200bce4
	ldr r0, [pc, #768]
	bl 0x0200bcbc
	movs r2, #10
	movs r0, #27
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #28
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #176
	movs r2, #10
	movs r0, #28
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #28
	movs r1, #3
	bl 0x0200bc7c
	movs r2, #10
	movs r0, #28
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #32
	bl 0x0200bd04
	movs r0, #40
	bl 0x0200bbec
	movs r0, #32
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bcfc
	movs r1, #176
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #128
	movs r2, #10
	movs r0, #31
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #31
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #31
	movs r1, #4
	bl 0x0200bc7c
	movs r0, #31
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #176
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #160
	movs r2, #20
	movs r0, #32
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r0, #31
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #32
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200bd04
	movs r0, #40
	bl 0x0200bbec
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #2
	movs r0, #20
	bl 0x0200bc9c
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r2, [r0]
	movs r5, #254
	adds r3, r5, #0
	ands r3, r2
	movs r2, #0
	mov r8, r2
	movs r2, #132
	strb r3, [r0]
	movs r1, #172
	lsls r2, r2, #1
	movs r0, #20
	bl 0x0200bc5c
	movs r0, #1
	bl 0x0200bbec
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r6, #1
	orrs r3, r6
	movs r1, #128
	strb r3, [r0]
	lsls r1, r1, #8
	movs r0, #27
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r2, #20
	movs r0, #31
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #3
	movs r0, #20
	bl 0x0200bc84
	movs r0, #20
	bl 0x0200bbec
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #136
	ands r5, r3
	movs r1, #172
	lsls r2, r2, #1
	strb r5, [r0]
	movs r0, #20
	bl 0x0200bc5c
	movs r0, #1
	bl 0x0200bbec
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #90
	ldrb r3, [r0]
	movs r2, #136
	orrs r6, r3
	strb r6, [r0]
	movs r1, #180
	movs r0, #20
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r2, #0
	movs r0, #34
	ldr r1, [pc, #340]
	bl 0x0200bcfc
	movs r0, #34
	movs r1, #1
	bl 0x0200bc9c
	movs r0, #34
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #34
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #33
	movs r1, #1
	bl 0x0200bc9c
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #33
	movs r1, #4
	bl 0x0200bc7c
	movs r2, #10
	movs r0, #33
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #21
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #129
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #0
	bl 0x0200bcfc
	movs r0, #21
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #2
	movs r2, #20
	bl 0x0200bc8c
	movs r2, #40
	movs r0, #20
	movs r1, #4
	bl 0x0200bc8c
	movs r0, #20
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #21
	ldr r1, [pc, #204]
	ldr r2, [pc, #168]
	bl 0x0200bc2c
	movs r2, #141
	movs r0, #21
	ldr r1, [pc, #196]
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #142
	movs r0, #21
	movs r1, #251
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r2, #148
	movs r0, #21
	movs r1, #246
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	lsls r1, r1, #8
	movs r2, #0
	movs r0, #21
	bl 0x0200bce4
	bl 0x02009e6c
	movs r0, #40
	bl 0x0200bbec
	movs r0, #21
	ldr r1, [pc, #136]
	ldr r2, [pc, #100]
	bl 0x0200bc2c
	movs r2, #148
	movs r0, #21
	movs r1, #228
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #21
	movs r1, #212
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #21
	movs r1, #192
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r2, #40
	movs r0, #21
	lsls r1, r1, #8
	b .L_02000590_5
	.2byte 0x0000
	.4byte 0x00013333
	.4byte 0x00002666
	.4byte 0x00011999
	.4byte 0x00008ccc
	.4byte 0x0000e666
	.4byte 0x00007333
	.4byte 0x0000cccc
	.4byte 0x00006666
	.4byte 0x0000b333
	.4byte 0x00005999
	.4byte 0x00004ccc
	.4byte 0x00009999
	.4byte 0x0200bdc4
	.4byte 0x00001a92
	.4byte 0x00000105
	.4byte 0x00019999
	.4byte 0x00000109
.L_02000590_5:
	bl 0x0200bce4
	movs r0, #21
	movs r1, #2
	bl 0x0200bc94
	movs r1, #128
	movs r0, #21
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200bce4
	movs r2, #143
	movs r0, #21
	movs r1, #184
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #176
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #21
	bl 0x0200bd04
	movs r0, #40
	bl 0x0200bbec
	movs r0, #21
	movs r1, #4
	bl 0x0200bc84
	movs r2, #40
	movs r0, #20
	ldr r1, [pc, #1008]
	bl 0x0200bcfc
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	movs r0, #21
	movs r1, #0
	movs r2, #60
	bl 0x0200bce4
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #6
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #21
	movs r1, #3
	bl 0x0200bc84
	movs r0, #21
	movs r1, #0
	movs r2, #0
	bl 0x0200bce4
	ldr r2, [pc, #920]
	movs r0, #20
	ldr r1, [pc, #920]
	bl 0x0200bc2c
	ldr r1, [pc, #920]
	movs r0, #20
	bl 0x0200bc4c
	movs r2, #148
	movs r0, #20
	movs r1, #228
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #20
	movs r1, #212
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r2, #148
	movs r0, #20
	movs r1, #192
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #192
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #176
	movs r0, #20
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #192
	movs r2, #10
	movs r0, #21
	lsls r1, r1, #6
	bl 0x0200bce4
	movs r1, #129
	lsls r1, r1, #1
	movs r0, #20
	bl 0x0200bd04
	movs r0, #60
	bl 0x0200bbec
	movs r1, #4
	movs r0, #20
	bl 0x0200bc84
	ldr r5, [pc, #796]
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r0, #20
	movs r1, #0
	movs r2, #40
	bl 0x0200bcd4
	bl 0x02009ed8
	movs r2, #136
	movs r0, #20
	movs r1, #178
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r1, #0
	movs r2, #0
	movs r0, #20
	bl 0x0200bce4
	movs r0, #240
	bl 0x0200bbec
	movs r0, #27
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r0, #27
	ldr r1, [pc, #704]
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #0
	movs r2, #10
	movs r0, #27
	bl 0x0200bcd4
	movs r0, #27
	bl 0x02009ea4
	movs r0, #80
	bl 0x0200bbec
	movs r0, #28
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #208
	movs r2, #20
	movs r0, #28
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #28
	movs r1, #2
	bl 0x0200bc9c
	movs r1, #0
	movs r2, #10
	movs r0, #28
	bl 0x0200bcd4
	movs r0, #28
	bl 0x02009ea4
	movs r0, #160
	bl 0x0200bbec
	movs r0, #32
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #160
	movs r0, #32
	lsls r1, r1, #7
	movs r2, #10
	bl 0x0200bce4
	movs r0, #32
	ldr r1, [pc, #592]
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #0
	movs r2, #10
	movs r0, #32
	bl 0x0200bcd4
	movs r0, #32
	bl 0x02009ea4
	movs r0, #80
	bl 0x0200bbec
	movs r0, #30
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #176
	movs r2, #10
	movs r0, #30
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #1
	movs r0, #30
	bl 0x0200bc9c
	adds r0, r5, #6
	bl 0x0200bcbc
	movs r1, #0
	movs r2, #10
	movs r0, #30
	bl 0x0200bcd4
	ldr r0, [pc, #536]
	bl 0x0200bb24
	movs r0, #20
	bl 0x0200bc44
	movs r0, #21
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r0, #20
	bl 0x0200bc1c
	mov r3, r8
	adds r0, #100
	strh r3, [r0]
	movs r0, #21
	bl 0x0200bc1c
	mov r2, r8
	adds r0, #100
	strh r2, [r0]
	ldr r1, [pc, #472]
	movs r0, #20
	ldr r2, [pc, #488]
	bl 0x0200bc2c
	ldr r2, [pc, #480]
	movs r0, #21
	ldr r1, [pc, #456]
	bl 0x0200bc2c
	ldr r1, [pc, #476]
	movs r0, #20
	bl 0x0200bc34
	ldr r1, [pc, #472]
	movs r0, #21
	bl 0x0200bc34
	movs r0, #29
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #160
	movs r2, #10
	movs r0, #29
	lsls r1, r1, #7
	bl 0x0200bce4
	adds r5, #5
	movs r1, #2
	movs r0, #29
	bl 0x0200bc9c
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r0, #29
	movs r1, #0
	movs r2, #20
	bl 0x0200bcd4
	movs r0, #29
	bl 0x02009ea4
	movs r0, #30
	bl 0x02009ea4
.L_02000590_6:
	movs r0, #1
	bl 0x0200bb14
	movs r0, #20
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #0
	beq .L_02000590_6
	movs r0, #21
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #1
	bne .L_02000590_6
	ldr r1, [pc, #368]
	movs r0, #20
	bl 0x0200bc34
	ldr r1, [pc, #364]
	movs r0, #21
	bl 0x0200bc34
	movs r0, #31
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r1, #160
	movs r2, #10
	movs r0, #31
	lsls r1, r1, #7
	bl 0x0200bce4
	movs r0, #31
	movs r1, #1
	bl 0x0200bc9c
	movs r1, #4
	movs r0, #31
	bl 0x0200bc84
	ldr r5, [pc, #320]
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r1, #0
	movs r2, #10
	movs r0, #31
	bl 0x0200bcd4
	movs r0, #31
	bl 0x02009ea4
	movs r0, #34
	bl 0x0200bc44
	movs r0, #33
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r0, #34
	ldr r1, [pc, #280]
	movs r2, #40
	bl 0x0200bcfc
	movs r0, #33
	ldr r1, [pc, #268]
	movs r2, #60
	bl 0x0200bcfc
	movs r1, #176
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #10
	bl 0x0200bce4
	movs r1, #160
	movs r2, #10
	movs r0, #33
	lsls r1, r1, #7
	bl 0x0200bce4
	adds r5, #3
	movs r1, #4
	movs r0, #34
	bl 0x0200bc84
	adds r0, r5, #0
	bl 0x0200bcbc
	movs r2, #10
	movs r0, #34
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #33
	movs r1, #1
	bl 0x0200bc9c
	movs r0, #33
	movs r1, #4
	bl 0x0200bc7c
	movs r0, #33
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #129
	movs r0, #34
	lsls r1, r1, #1
	movs r2, #60
	bl 0x0200bcfc
	movs r2, #0
	movs r0, #20
	ldr r1, [pc, #172]
	bl 0x0200bcfc
	movs r1, #2
	movs r0, #20
	bl 0x0200bc9c
	ldr r0, [pc, #164]
	bl 0x0200bcbc
	movs r1, #0
	movs r2, #10
	movs r0, #20
	bl 0x0200bcd4
	movs r0, #27
	bl 0x0200bc44
	movs r0, #28
	bl 0x0200bc44
	movs r0, #29
	bl 0x0200bc44
	movs r0, #30
	bl 0x0200bc44
	movs r0, #32
	bl 0x0200bc44
	movs r0, #31
	bl 0x0200bc44
	movs r0, #33
	bl 0x0200bc44
	movs r0, #34
	bl 0x0200bc44
	movs r0, #20
	bl 0x0200bc44
	movs r0, #21
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	movs r0, #27
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #28
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #29
	movs r1, #2
	movs r2, #0
	b .L_02000590_7
	.4byte 0x00000101
	.4byte 0x0000cccc
	.4byte 0x00019999
	.4byte 0x0200bfb0
	.4byte 0x00001a9e
	.4byte 0x02009f15
	.4byte 0x00006666
	.4byte 0x0200c034
	.4byte 0x0200c0cc
	.4byte 0x0200c164
	.4byte 0x0200c1ac
	.4byte 0x00001aa2
	.4byte 0x00000105
	.4byte 0x00000103
	.4byte 0x00001ab2
.L_02000590_7:
	bl 0x0200bc8c
	movs r0, #30
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #32
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #31
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #33
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #34
	movs r1, #2
	movs r2, #0
	bl 0x0200bc8c
	movs r0, #21
	movs r1, #2
	movs r2, #40
	bl 0x0200bc8c
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r0, #21
	movs r1, #4
	movs r2, #40
	bl 0x0200bc8c
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #1
	bl 0x0200bc9c
	movs r2, #10
	movs r0, #20
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #21
	movs r1, #3
	bl 0x0200bc84
	movs r2, #10
	movs r0, #21
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #129
	movs r2, #40
	movs r0, #27
	lsls r1, r1, #1
	bl 0x0200bcfc
	movs r0, #27
	movs r1, #1
	bl 0x0200bc94
	movs r0, #27
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #129
	movs r0, #28
	lsls r1, r1, #1
	movs r2, #40
	bl 0x0200bcfc
	movs r2, #10
	movs r0, #28
	movs r1, #0
	bl 0x0200bcd4
	movs r1, #4
	movs r0, #21
	bl 0x0200bc84
	movs r0, #40
	bl 0x0200bbec
	movs r0, #21
	movs r1, #3
	bl 0x0200bc84
	movs r2, #20
	movs r0, #21
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #20
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #160
	movs r0, #27
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #160
	movs r0, #29
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r0, #30
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #160
	movs r0, #32
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #160
	movs r0, #33
	lsls r1, r1, #7
	movs r2, #0
	bl 0x0200bce4
	movs r1, #176
	movs r2, #4
	movs r0, #34
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r0, #27
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #28
	movs r1, #3
	bl 0x0200bc84
	movs r0, #29
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #30
	movs r1, #3
	bl 0x0200bc84
	movs r0, #32
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #31
	movs r1, #3
	bl 0x0200bc84
	movs r0, #33
	movs r1, #3
	bl 0x0200bc7c
	movs r0, #34
	movs r1, #3
	bl 0x0200bc84
	movs r0, #20
	movs r1, #2
	movs r2, #40
	bl 0x0200bc8c
	movs r0, #20
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r1, #128
	movs r0, #27
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #28
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #128
	movs r0, #29
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #30
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #128
	movs r0, #32
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #31
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r1, #128
	movs r0, #33
	lsls r1, r1, #8
	movs r2, #0
	bl 0x0200bce4
	movs r1, #128
	movs r0, #34
	lsls r1, r1, #8
	movs r2, #4
	bl 0x0200bce4
	movs r0, #20
	ldr r1, [pc, #872]
	ldr r2, [pc, #872]
	bl 0x0200bc2c
	movs r0, #27
	ldr r1, [pc, #868]
	ldr r2, [pc, #872]
	bl 0x0200bc2c
	movs r0, #28
	ldr r1, [pc, #860]
	ldr r2, [pc, #860]
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #29
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r1, #128
	movs r2, #128
	movs r0, #30
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r0, #32
	ldr r1, [pc, #828]
	ldr r2, [pc, #832]
	bl 0x0200bc2c
	movs r0, #31
	ldr r1, [pc, #820]
	ldr r2, [pc, #820]
	bl 0x0200bc2c
	movs r0, #33
	ldr r1, [pc, #816]
	ldr r2, [pc, #820]
	bl 0x0200bc2c
	movs r0, #34
	ldr r1, [pc, #808]
	ldr r2, [pc, #808]
	bl 0x0200bc2c
	ldr r2, [pc, #808]
	movs r0, #21
	ldr r1, [pc, #808]
	bl 0x0200bc2c
	movs r0, #27
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #28
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #29
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #30
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #32
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #31
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #33
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #34
	movs r1, #1
	bl 0x0200bcf4
	movs r0, #20
	movs r1, #1
	bl 0x0200bcf4
	movs r1, #1
	movs r0, #21
	bl 0x0200bcf4
	movs r0, #27
	bl 0x0200bc44
	movs r0, #28
	bl 0x0200bc44
	movs r0, #29
	bl 0x0200bc44
	movs r0, #30
	bl 0x0200bc44
	movs r0, #32
	bl 0x0200bc44
	movs r0, #31
	bl 0x0200bc44
	movs r0, #33
	bl 0x0200bc44
	movs r0, #34
	bl 0x0200bc44
	movs r0, #20
	bl 0x0200bc44
	movs r0, #21
	bl 0x0200bc44
	movs r0, #1
	bl 0x0200bb14
	ldr r5, [pc, #660]
	movs r0, #20
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #27
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #28
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #29
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #30
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #32
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #31
	adds r1, r5, #0
	bl 0x0200bc34
	movs r0, #33
	adds r1, r5, #0
	bl 0x0200bc34
	adds r1, r5, #0
	movs r0, #34
	bl 0x0200bc34
	movs r0, #21
	bl 0x0200bc1c
	adds r2, r0, #0
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	movs r0, #21
	adds r1, r5, #0
	bl 0x0200bc34
.L_02000590_8:
	movs r0, #1
	bl 0x0200bb14
	movs r0, #21
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #1
	bne .L_02000590_8
	movs r0, #80
	bl 0x0200bbec
	movs r1, #170
	movs r2, #137
	lsls r1, r1, #17
	lsls r2, r2, #17
	movs r0, #14
	bl 0x0200bc74
	movs r0, #1
	bl 0x0200bb14
	movs r1, #128
	movs r2, #128
	movs r0, #14
	lsls r1, r1, #9
	lsls r2, r2, #8
	bl 0x0200bc2c
	movs r2, #137
	movs r0, #14
	movs r1, #224
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl 0x0200bce4
	movs r1, #128
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #160
	movs r0, #14
	lsls r1, r1, #7
	movs r2, #40
	bl 0x0200bce4
	movs r0, #14
	ldr r1, [pc, #444]
	movs r2, #60
	bl 0x0200bcfc
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #14
	movs r1, #0
	movs r2, #40
	bl 0x0200bce4
	movs r1, #192
	movs r0, #14
	lsls r1, r1, #8
	movs r2, #40
	bl 0x0200bce4
	movs r1, #128
	movs r2, #40
	movs r0, #14
	lsls r1, r1, #8
	bl 0x0200bce4
	movs r1, #129
	movs r0, #14
	lsls r1, r1, #1
	bl 0x0200bd04
	movs r0, #14
	movs r1, #4
	movs r2, #40
	bl 0x0200bc8c
	movs r2, #20
	movs r0, #14
	movs r1, #0
	bl 0x0200bcd4
	movs r0, #14
	movs r1, #2
	bl 0x0200bc9c
	movs r0, #14
	movs r1, #0
	movs r2, #10
	bl 0x0200bcd4
	movs r0, #14
	movs r1, #4
	movs r2, #40
	bl 0x0200bc8c
	ldr r1, [pc, #340]
	ldr r2, [pc, #340]
	movs r0, #14
	bl 0x0200bc2c
	movs r0, #14
	bl 0x0200bc1c
	adds r3, r0, #0
	adds r3, #100
	movs r2, #0
	strh r2, [r3]
	ldr r1, [pc, #308]
	movs r0, #14
	bl 0x0200bc34
.L_02000590_9:
	movs r0, #1
	bl 0x0200bb14
	movs r0, #14
	bl 0x0200bc1c
	adds r0, #100
	movs r2, #0
	ldrsh r3, [r0, r2]
	cmp r3, #1
	bne .L_02000590_9
	movs r2, #157
	ldr r1, [pc, #292]
	lsls r2, r2, #17
	movs r0, #14
	bl 0x0200bc74
	movs r0, #14
	bl 0x0200bc1c
	movs r5, #208
	lsls r5, r5, #8
	movs r2, #217
	ldr r1, [pc, #272]
	lsls r2, r2, #17
	strh r5, [r0, #6]
	movs r0, #20
	bl 0x0200bc74
	movs r0, #20
	bl 0x0200bc1c
	movs r1, #232
	movs r2, #208
	lsls r2, r2, #17
	lsls r1, r1, #17
	strh r5, [r0, #6]
	movs r0, #21
	bl 0x0200bc74
	movs r0, #21
	bl 0x0200bc1c
	movs r3, #160
	lsls r3, r3, #7
	strh r3, [r0, #6]
	movs r0, #27
	bl 0x0200bc24
	movs r0, #28
	bl 0x0200bc24
	movs r0, #29
	bl 0x0200bc24
	movs r0, #30
	bl 0x0200bc24
	movs r0, #31
	bl 0x0200bc24
	movs r0, #32
	bl 0x0200bc24
	movs r0, #33
	bl 0x0200bc24
	movs r0, #34
	bl 0x0200bc24
	movs r0, #17
	bl 0x0200bd7c
	movs r2, #182
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #9
	bne .L_02000590_10
	movs r2, #229
	movs r0, #0
	movs r1, #224
	lsls r2, r2, #1
	bl 0x0200bc5c
	movs r0, #0
	bl 0x0200bc1c
	movs r3, #192
	lsls r3, r3, #8
	b .L_02000590_11
.L_02000590_10:
	movs r0, #0
	movs r1, #40
	movs r2, #248
	bl 0x0200bc5c
	movs r0, #0
	bl 0x0200bc1c
	movs r3, #128
	lsls r3, r3, #7
.L_02000590_11:
	strh r3, [r0, #6]
	bl 0x0200bd54
	ldr r0, [pc, #112]
	bl 0x0200bbdc
	b .L_02000590_12
	.2byte 0x207b
	.2byte 0xf002
	.2byte 0xfc2d
	.2byte 0x22b6
	.2byte 0x0052
	.2byte 0x18bb
	.2byte 0x2200
	.2byte 0x5e98
	.2byte 0xf002
	.2byte 0xfbfa
	.2byte 0xf002
	.2byte 0xfc00
	.2byte 0xf002
	.2byte 0xfc02
.L_02000590_12:
	bl 0x0200bbfc
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x00011999
	.4byte 0x00008ccc
	.4byte 0x00010ccc
	.4byte 0x00008666
	.4byte 0x0000f333
	.4byte 0x00007999
	.4byte 0x0000e666
	.4byte 0x00007333
	.4byte 0x00006ccc
	.4byte 0x0000d999
	.4byte 0x0200be00
	.4byte 0x00000101
	.4byte 0x00013333
	.4byte 0x00009999
	.4byte 0x01670000
	.4byte 0x01c70000
	.4byte 0x00000911
	.global Func_0200158c
	.thumb_func
Func_0200158c:
	push {lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bbd4
	cmp r0, #0
	beq .L_0200158c_0
	ldr r3, [pc, #20]
	ldr r2, [r3]
	movs r0, #128
	movs r3, #0
	str r3, [r2, #24]
	lsls r0, r0, #2
	bl 0x0200bbe4
.L_0200158c_0:
	pop {r0}
	bx r0
	.2byte 0x0000
	.4byte 0x03001ee0
	.global SceneState_LinkRecordZeroWhenFlag200Clear
	.thumb_func
SceneState_LinkRecordZeroWhenFlag200Clear:
	.global Func_020015b4
	.thumb_func
Func_020015b4:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bbd4
	cmp r0, #0
	bne .L_020015b4_0
	ldr r3, [pc, #24]
	movs r0, #0
	ldr r5, [r3]
	bl 0x0200bc1c
	str r0, [r5, #24]
	movs r0, #128
	lsls r0, r0, #2
	bl 0x0200bbdc
.L_020015b4_0:
	pop {r5}
	pop {r0}
	bx r0
	.4byte 0x03001ee0
	.section .text.x0200964c,"ax",%progbits
	.align 2
	.global Func_0200164c
	.thumb_func
Func_0200164c:
	push {lr}
	ldr r0, [pc, #80]
	bl 0x0200bbdc
	ldr r3, [pc, #76]
	movs r1, #224
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, [pc, #68]
	cmp r2, r3
	bne .L_0200164c_0
	bl 0x020096bc
	b .L_0200164c_1
.L_0200164c_0:
	ldr r3, [pc, #60]
	cmp r2, r3
	bne .L_0200164c_2
	bl 0x020097e8
	b .L_0200164c_1
.L_0200164c_2:
	ldr r3, [pc, #52]
	cmp r2, r3
	bne .L_0200164c_3
	bl 0x02009858
	b .L_0200164c_1
.L_0200164c_3:
	ldr r3, [pc, #44]
	cmp r2, r3
	bne .L_0200164c_4
	bl 0x020098a4
	b .L_0200164c_1
.L_0200164c_4:
	ldr r3, [pc, #36]
	cmp r2, r3
	bne .L_0200164c_1
	bl 0x02009930
.L_0200164c_1:
	movs r0, #0
	pop {r1}
	bx r1
	.4byte 0x0000087a
	.4byte 0x02000240
	.4byte 0x00000063
	.4byte 0x00000066
	.4byte 0x00000099
	.4byte 0x0000009b
	.4byte 0x0000009c
	.section .rodata.part1,"a",%progbits
	.global KareiMachi_Data01
KareiMachi_Data01:
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
	.global SceneAction_GroupMotion
SceneAction_GroupMotion:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xffb00000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global SceneAction_GroupOffsetMotion
SceneAction_GroupOffsetMotion:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0xffb80000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xff9c0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0xff9c0000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x00de0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global KareiMachi_Script01
KareiMachi_Script01:
	.4byte 0x00000022
	.4byte 0x02008041
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global KareiMachi_DanceScriptA
KareiMachi_DanceScriptA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global KareiMachi_DanceScriptB
KareiMachi_DanceScriptB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a40000
	.4byte 0x00000000
	.4byte 0x01160000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00ee0000
	.4byte 0x00000000
	.4byte 0x01300000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000011
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000013
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x01260000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000011
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000012
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x00000013
	.4byte 0x80000000
	.4byte 0x00000015
	.4byte 0x0000000c
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x0000000e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ff0000
	.4byte 0x00000000
	.4byte 0x01210000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x01090000
	.4byte 0x00000000
	.4byte 0x01180000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x011c0000
	.4byte 0x00000000
	.4byte 0x01100000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global KareiMachi_Data02
KareiMachi_Data02:
	.4byte 0x00000022
	.4byte 0x020080ad
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global KareiMachi_Data03
KareiMachi_Data03:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x01840000
	.4byte 0x00000000
	.4byte 0x012c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global KareiMachi_ActionScript01
KareiMachi_ActionScript01:
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000222
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x00000222
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000006c
	.4byte 0x00000000
	.4byte 0x00000010
	.global gKareiMachiEntrancesOther
gKareiMachiEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEntrances1
gKareiMachiEntrances1:
	.4byte 0xffff0000
	.4byte 0x000000e0
	.4byte 0x40000118
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0x400000c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000198
	.4byte 0x400000b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000198
	.4byte 0x400000d8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000198
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0x00000148
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0x00000078
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0007
	.4byte 0x00000138
	.4byte 0x40000108
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0008
	.4byte 0x00000098
	.4byte 0x400001b8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0009
	.4byte 0x000000e0
	.4byte 0xc00001d0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x00000038
	.4byte 0x00000110
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000e0
	.4byte 0x40000058
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x00000128
	.4byte 0x40000198
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x00000098
	.4byte 0x400000e8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x000001b8
	.4byte 0x40000058
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEntrances2
gKareiMachiEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000c0
	.4byte 0xc0000118
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000160
	.4byte 0xffff0002
	.4byte 0x000000c0
	.4byte 0x400000e8
	.4byte 0x00000000
	.4byte 0x01900000
	.4byte 0x00000160
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEntrances3
gKareiMachiEntrances3:
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000f8
	.4byte 0x40000098
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000001a8
	.4byte 0xc0000208
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000001b8
	.4byte 0xc000021c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEntrances4
gKareiMachiEntrances4:
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000128
	.4byte 0x40000068
	.4byte 0x00640000
	.4byte 0x01b80032
	.4byte 0x000001bd
	.4byte 0xffff0002
	.4byte 0x00000118
	.4byte 0xc0000188
	.4byte 0x00640000
	.4byte 0x01b80032
	.4byte 0x000001bd
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEntrances5
gKareiMachiEntrances5:
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000128
	.4byte 0x400001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x00000278
	.4byte 0x400001c8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x00000098
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x00000238
	.4byte 0x40000098
	.4byte 0x01860000
	.4byte 0x02760000
	.4byte 0x000000dc
	.4byte 0xffff0005
	.4byte 0x000001d8
	.4byte 0x40000048
	.4byte 0x01860000
	.4byte 0x02760000
	.4byte 0x000000dc
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEntrances6
gKareiMachiEntrances6:
	.4byte 0xffff0000
	.4byte 0x000000a8
	.4byte 0x400000a8
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000198
	.4byte 0x40000278
	.4byte 0x00550000
	.4byte 0x01d10190
	.4byte 0x00000370
	.4byte 0xffff0002
	.4byte 0x00000128
	.4byte 0x400001e8
	.4byte 0x00550000
	.4byte 0x01d10190
	.4byte 0x00000370
	.4byte 0xffff0003
	.4byte 0x000002e8
	.4byte 0x400001e8
	.4byte 0x02530000
	.4byte 0x03a20190
	.4byte 0x00000370
	.4byte 0xffff0004
	.4byte 0x00000358
	.4byte 0xc0000358
	.4byte 0x02530000
	.4byte 0x03a20190
	.4byte 0x00000370
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global KareiMachi_SceneTable01
KareiMachi_SceneTable01:
	.4byte 0x00000063
	.4byte 0x00101064
	.4byte 0x00202064
	.4byte 0x00303064
	.4byte 0x00404064
	.4byte 0x00506064
	.4byte 0x00607064
	.4byte 0x0070a064
	.4byte 0x00801065
	.4byte 0x00914002
	.4byte 0x00a13002
	.4byte 0x00b01066
	.4byte 0x00c05064
	.4byte 0x00d08064
	.4byte 0x00e0409c
	.4byte 0x00000066
	.4byte 0x0010b063
	.4byte 0x00201067
	.4byte 0x00514067
	.4byte 0x00000099
	.4byte 0x0010209a
	.4byte 0x0023c002
	.4byte 0x00a47002
	.4byte 0x0000009a
	.4byte 0x0010509b
	.4byte 0x00201099
	.4byte 0x0000009b
	.4byte 0x0010209c
	.4byte 0x0020309c
	.4byte 0x0030409b
	.4byte 0x0040309b
	.4byte 0x0050109a
	.4byte 0x0000009c
	.4byte 0x0010b067
	.4byte 0x0020109b
	.4byte 0x0030209b
	.4byte 0x0040e063
	.4byte 0x000001ff
	.global gKareiMachiPlacementsOther
gKareiMachiPlacementsOther:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiPlacements1
gKareiMachiPlacements1:
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00bc0000
	.4byte 0x00000000
	.4byte 0x01dc0000
	.4byte 0x00013000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x01040000
	.4byte 0x00000000
	.4byte 0x01dc0000
	.4byte 0x00015000
	.4byte 0xffff006c
	.4byte 0x00000002
	.4byte 0x00de0000
	.4byte 0x00000000
	.4byte 0x017e0000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00860000
	.4byte 0x00000000
	.4byte 0xffff0065
	.4byte 0x00000002
	.4byte 0x00730000
	.4byte 0x00000000
	.4byte 0x01c50000
	.4byte 0x00000000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x01e40000
	.4byte 0x00000000
	.4byte 0x01020000
	.4byte 0x00005000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x01670000
	.4byte 0x00000000
	.4byte 0x013a0000
	.4byte 0x0001d000
	.4byte 0xffff006f
	.4byte 0x00000002
	.4byte 0x01940000
	.4byte 0x00000000
	.4byte 0x010c0000
	.4byte 0x00004000
	.4byte 0xffff006b
	.4byte 0x00000001
	.4byte 0x009a0000
	.4byte 0x00000000
	.4byte 0x00770000
	.4byte 0x00000000
	.4byte 0xffff0066
	.4byte 0x00000001
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x00540000
	.4byte 0x00008000
	.4byte 0xffff0074
	.4byte 0x00000001
	.4byte 0x00c70000
	.4byte 0x00000000
	.4byte 0x00e90000
	.4byte 0x00018000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x002e0000
	.4byte 0x00000000
	.4byte 0x00d90000
	.4byte 0x00003000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01d40000
	.4byte 0x00000000
	.4byte 0x01b20000
	.4byte 0x0001b000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x01d00000
	.4byte 0x00000000
	.4byte 0x01a00000
	.4byte 0x00013000
	.4byte 0xffff007c
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00015000
	.4byte 0xffff007d
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00013000
	.4byte 0xffff0076
	.4byte 0x00000001
	.4byte 0x01160000
	.4byte 0x00000000
	.4byte 0x013e0000
	.4byte 0x00018000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x014c0000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00008000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiPlacements2
gKareiMachiPlacements2:
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00015000
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00013000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiPlacements3
gKareiMachiPlacements3:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002c000
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
	.4byte 0xffff0099
	.4byte 0x00000001
	.4byte 0x00f60000
	.4byte 0x00000000
	.4byte 0x00c60000
	.4byte 0x00023000
	.4byte 0xffff0111
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0031
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0fd60016
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiPlacements6
gKareiMachiPlacements6:
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x033a0000
	.4byte 0x00024000
	.4byte 0xffff00cb
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02dc0000
	.4byte 0x00024000
	.4byte 0xffff0044
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00e3
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x005b005c
	.4byte 0x00000001
	.4byte 0x02780000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEventsOther
gKareiMachiEventsOther:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEvents1
gKareiMachiEvents1:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0007
	.4byte 0x02008465
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte 0x02008465
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000002
	.4byte 0xffff0009
	.4byte 0x02008591
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x02008591
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001a71
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001a72
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x00001a73
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte 0x00001a74
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x00001a75
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001a76
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001a77
	.4byte 0x00000000
	.4byte 0x0911000f
	.4byte 0x00001a78
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001b9b
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001a79
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001a7a
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001a7b
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x020082c9
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001a7f
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001a80
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte 0x02008395
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte 0x020083d9
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte 0x02008421
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001a81
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001a82
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x00001a83
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte 0x00001a84
	.4byte 0x00008d15
	.4byte 0xffff000c
	.4byte 0x00001a85
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001a86
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001a87
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001a88
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001a89
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001a8a
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001a8b
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001a8c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001a8d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001a8e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001ad0
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00001ad4
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte 0x00001ad6
	.4byte 0x00008c15
	.4byte 0xffff0019
	.4byte 0x00000000
	.4byte 0x00009415
	.4byte 0x0916001a
	.4byte 0x02009611
	.4byte 0x00000023
	.4byte 0x0f810064
	.4byte 0x001000e3
	.4byte 0x00000023
	.4byte 0x0f820065
	.4byte 0x001000b6
	.4byte 0x00000003
	.4byte 0xffff005a
	.4byte 0x020081e5
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEvents2
gKareiMachiEvents2:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte 0x02008505
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00001b97
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte 0x00001b98
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00001b99
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte 0x00001b9a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEvents3
gKareiMachiEvents3:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x020082e9
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x00002587
	.4byte 0x00009415
	.4byte 0x0fd6000c
	.4byte 0x0200820d
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEvents4
gKareiMachiEvents4:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000013
	.4byte 0x0f220064
	.4byte 0x001000c1
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEvents5
gKareiMachiEvents5:
	.4byte 0x00000031
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000031
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000021
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gKareiMachiEvents6
gKareiMachiEvents6:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000021
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000021
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte 0x02008375
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte 0x000025b6
	.4byte 0x00008c15
	.4byte 0x03020008
	.4byte 0x0200b2a5
	.4byte 0x00008c15
	.4byte 0x03030009
	.4byte 0x0200b2a5
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte 0x0200b1b9
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte 0x0200b1b9
	.4byte 0x00008602
	.4byte 0xffff000e
	.4byte 0x0200b1b9
	.4byte 0x00004602
	.4byte 0xffff000f
	.4byte 0x0200b1b9
	.4byte 0x00008602
	.4byte 0xffff000f
	.4byte 0x0200b1b9
	.4byte 0x00001815
	.4byte 0x0201000b
	.4byte 0x0200b971
	.4byte 0x00000002
	.4byte 0xffff000a
	.4byte 0x0200958d
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte 0x020095b5
	.4byte 0x00000002
	.4byte 0xffff0012
	.4byte 0x020095f9
	.4byte 0x00000002
	.4byte 0xffff0013
	.4byte 0x020095e1
	.4byte 0x00000013
	.4byte 0x0f1c0064
	.4byte 0x00100084
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte 0x02008031
	.4byte 0x00000013
	.4byte 0x0f1e0066
	.4byte 0x001000e3
	.4byte 0x00000013
	.4byte 0x0f1f0067
	.4byte 0x001000e5
	.4byte 0x00000013
	.4byte 0x0f200068
	.4byte 0x002000c8
	.4byte 0x00000013
	.4byte 0x0f210069
	.4byte 0x001000b7
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00210023
	.4byte 0x00020001
	.4byte 0x00250005
	.4byte 0x00010021
	.4byte 0x00050002
	.4byte 0x0000ffff
	.global KareiMachi_DoorCells
KareiMachi_DoorCells:
	.4byte 0x0200d0c8
	.4byte 0x000a0024
	.4byte 0x0200d0c8
	.4byte 0x00070039
	.4byte 0x0200d0c8
	.4byte 0x000b0039
	.4byte 0x0200d0c8
	.4byte 0x00170039
	.4byte 0x0200d0c8
	.4byte 0x00190034
	.4byte 0x0200d0c8
	.4byte 0x000e0027
	.4byte 0x0200d0c8
	.4byte 0x000e0033
	.4byte 0x0200d0c8
	.4byte 0x00190029
	.global KareiMachi_Script02
KareiMachi_Script02:
	.4byte 0x00000022
	.4byte 0x0200b6f9
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.section .bss,"aw",%nobits
	.space 16
	.global KareiMachi_DanceStep
KareiMachi_DanceStep:
	.space 4
