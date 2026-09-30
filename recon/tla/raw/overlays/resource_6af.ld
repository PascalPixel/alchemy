/* resource_6af: its entry veneers, its listing and its import veneers
   linked in place, as pret links objects in ROM order. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6AF/ENTRY.o"(.text)
        "*/resource_6af_overlay.o"(.text.x02008038)
        "*/resource_6af_overlay.o"(.text.x02008080)
        "*/resource_6af_overlay.o"(.text.x020080d0)
        "*/resource_6af_overlay.o"(.text.x02008124)
        "*/resource_6af_overlay.o"(.text.x0200815c)
        "*/resource_6af_overlay.o"(.text.x0200833c)
        "*/resource_6af_overlay.o"(.text.x02008344)
        "*/resource_6af_overlay.o"(.text.x02008348)
        "*/resource_6af_overlay.o"(.text.x02008350)
        "*/resource_6af_overlay.o"(.text.x020083b4)
        "*/resource_6af_overlay.o"(.text.x020083fc)
        "*/resource_6af_overlay.o"(.text.x0200843c)
        "*/resource_6af_overlay.o"(.text.x020085ac)
        "*/resource_6af_overlay.o"(.text.x02008684)
        "*/resource_6af_overlay.o"(.text.x02008728)
        "*/resource_6af_overlay.o"(.text.x02008760)
        "*/resource_6af_overlay.o"(.text.x02008864)
        "*/resource_6af_overlay.o"(.text.x02008870)
        "*/resource_6af_overlay.o"(.text.x02008880)
        "*/resource_6af_overlay.o"(.text.x02008914)
        "*/resource_6af_overlay.o"(.text.x02008940)
        "*/resource_6af_overlay.o"(.text.x02008970)
        "*/resource_6af_overlay.o"(.text.x020089a0)
        "*/resource_6af_overlay.o"(.text.x020089d0)
        "*/resource_6af_overlay.o"(.text.x020089fc)
        "*/resource_6af_overlay.o"(.text.x02008a2c)
        "*/resource_6af_overlay.o"(.text.x02008a5c)
        "*/resource_6af_overlay.o"(.text.x02008a8c)
        "*/resource_6af_overlay.o"(.text.x02008b3c)
        "*/resource_6af_overlay.o"(.text.x02008ba0)
        "*/resource_6af_overlay.o"(.text.x02008cf4)
        "*/resource_6af_overlay.o"(.text.x02008f48)
        "*/resource_6af_overlay.o"(.text.x02008f4c)
        "*/games/THE LOST AGE/SRC/FIELD/OVERLAY_6AF/IMPORT.o"(.text)
        "*/resource_6af_overlay.o"(.rodata.x02009074)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
