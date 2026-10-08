SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/DOU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/IMPORT.o"(.text)
        *libgcc.a:_addsub_df.o(.text)
        *libgcc.a:_si_to_df.o(.text)
        *libgcc.a:_df_to_si.o(.text)
        *libgcc.a:_lshrdi3.o(.text)
        *libgcc.a:_pack_df.o(.text)
        *libgcc.a:_unpack_df.o(.text)
        "*/resource_3a7_overlay.o"(.rodata.x02009844)
        *libgcc.a:_thenan_df.o(.rodata)
        "*/resource_3a7_overlay.o"(.rodata.x0200989c)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
