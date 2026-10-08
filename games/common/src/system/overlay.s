.macro overlay_veneer target
    ldr r4, .Loverlay_target\@
    bx r4
    .balign 4
.Loverlay_target\@:
    .4byte \target
.endm
