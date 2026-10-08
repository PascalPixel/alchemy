SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/tbs/src/field/kuupuappu_dou/entry.o"(.text)
        "*/games/tbs/src/field/kuupuappu_dou/dou.o"(.text)
        "*/games/tbs/src/field/kuupuappu_dou/import.o"(.text)
        *libgcc.a:_addsub_df.o(.text)
        *libgcc.a:_si_to_df.o(.text)
        *libgcc.a:_df_to_si.o(.text)
        *libgcc.a:_lshrdi3.o(.text)
        *libgcc.a:_pack_df.o(.text)
        *libgcc.a:_unpack_df.o(.text)
        "*/games/tbs/src/field/kuupuappu_dou/data.o"(.data)
        *libgcc.a:_thenan_df.o(.rodata)
        "*/games/tbs/src/field/kuupuappu_dou/data2.o"(.data)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
