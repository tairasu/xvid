cmake_minimum_required(VERSION 3.10)
project(XvidDecoder C)

string(TOUPPER "${CMAKE_SYSTEM_PROCESSOR}" SYSTEM_PROCESSOR)

if(${SYSTEM_PROCESSOR} MATCHES "AMD64|EM64T|X86_64|X86|I386|I686")
    enable_language(ASM_NASM)
    set(IS_X86 ON)
else()
    set(IS_X86 OFF)
endif()

set(SRC_GENERIC
    src/XvidDecoder.c
    src/decoder.c
    src/encoder.c
    src/xvid.c
    src/bitstream/bitstream.c
    src/bitstream/cbp.c
    src/bitstream/mbcoding.c
    src/dct/fdct.c
    src/dct/idct.c
    src/dct/simple_idct.c
    src/image/colorspace.c
    src/image/image.c
    src/image/interpolate8x8.c
    src/image/font.c
    src/image/postprocessing.c
    src/image/qpel.c
    src/image/reduced.c
    src/motion/estimation_bvop.c
    src/motion/estimation_common.c
    src/motion/estimation_gmc.c
    src/motion/estimation_pvop.c
    src/motion/estimation_rd_based.c
    src/motion/estimation_rd_based_bvop.c
    src/motion/gmc.c
    src/motion/motion_comp.c
    src/motion/vop_type_decision.c
    src/motion/sad.c
    src/prediction/mbprediction.c
    src/plugins/plugin_single.c
    src/plugins/plugin_2pass1.c
    src/plugins/plugin_2pass2.c
    src/plugins/plugin_lumimasking.c
    src/plugins/plugin_dump.c
    src/plugins/plugin_psnr.c
    src/plugins/plugin_ssim.c
    src/plugins/plugin_psnrhvsm.c
    src/quant/quant_h263.c
    src/quant/quant_matrix.c
    src/quant/quant_mpeg.c
    src/utils/emms.c
    src/utils/mbtransquant.c
    src/utils/mem_align.c
    src/utils/mem_transfer.c
    src/utils/timer.c
)

set(SRC_IA32
    src/bitstream/x86_asm/cbp_mmx.asm
    src/bitstream/x86_asm/cbp_sse2.asm
    src/dct/x86_asm/fdct_mmx_ffmpeg.asm
    src/dct/x86_asm/fdct_mmx_skal.asm
    src/dct/x86_asm/fdct_sse2_skal.asm
    src/dct/x86_asm/idct_3dne.asm
    src/dct/x86_asm/idct_mmx.asm
    src/dct/x86_asm/idct_sse2_dmitry.asm
    src/image/x86_asm/colorspace_rgb_mmx.asm
    src/image/x86_asm/colorspace_yuv_mmx.asm
    src/image/x86_asm/colorspace_yuyv_mmx.asm
    src/image/x86_asm/interpolate8x8_3dn.asm
    src/image/x86_asm/interpolate8x8_3dne.asm
    src/image/x86_asm/interpolate8x8_mmx.asm
    src/image/x86_asm/interpolate8x8_xmm.asm
    src/image/x86_asm/postprocessing_mmx.asm
    src/image/x86_asm/postprocessing_sse2.asm
    src/image/x86_asm/reduced_mmx.asm
    src/image/x86_asm/qpel_mmx.asm
    src/image/x86_asm/gmc_mmx.asm
    src/image/x86_asm/deintl_sse.asm
    src/motion/x86_asm/sad_xmm.asm
    src/motion/x86_asm/sad_sse2.asm
    src/motion/x86_asm/sad_mmx.asm
    src/motion/x86_asm/sad_3dne.asm
    src/motion/x86_asm/sad_3dn.asm
    src/quant/x86_asm/quantize_h263_mmx.asm
    src/quant/x86_asm/quantize_h263_3dne.asm
    src/quant/x86_asm/quantize_mpeg_xmm.asm
    src/quant/x86_asm/quantize_mpeg_mmx.asm
    src/utils/x86_asm/mem_transfer_mmx.asm
    src/utils/x86_asm/mem_transfer_3dne.asm
    src/utils/x86_asm/interlacing_mmx.asm
    src/utils/x86_asm/cpuid.asm
    src/plugins/x86_asm/plugin_ssim-a.asm
)

set(SRC_IA64
    src/dct/ia64_asm/fdct_ia64.s
    src/image/ia64_asm/interpolate8x8_ia64.s
    src/motion/ia64_asm/sad_ia64.s
    src/motion/ia64_asm/halfpel8_refine_ia64.s
    src/quant/ia64_asm/quant_h263_ia64.s
    src/utils/ia64_asm/mem_transfer_ia64.s
)

set(SRC_IA64_IDCT_GCC
    src/dct/ia64_asm/idct_ia64_gcc.s
)

set(SRC_IA64_IDCT_ECC
    src/dct/ia64_asm/idct_ia64_ecc.s
)

set(SRC_PPC_ALTIVEC
    src/utils/ppc_asm/altivec_trigger.c
    src/utils/ppc_asm/mem_transfer_altivec.c
    src/motion/ppc_asm/sad_altivec.c
    src/dct/ppc_asm/idct_altivec.c
    src/image/ppc_asm/interpolate8x8_altivec.c
    src/image/ppc_asm/colorspace_altivec.c
    src/image/ppc_asm/qpel_altivec.c
    src/quant/ppc_asm/quant_h263_altivec.c
    src/quant/ppc_asm/quant_mpeg_altivec.c
)

set(DEF_FILES)
set(ASM_FILES)

include (TestBigEndian)
TEST_BIG_ENDIAN(IS_BIG_ENDIAN)
if(IS_BIG_ENDIAN)
    add_compile_definitions($<$<COMPILE_LANGUAGE:C>:ARCH_IS_BIG_ENDIAN>)
else()
    add_compile_definitions($<$<COMPILE_LANGUAGE:C>:ARCH_IS_LITTLE_ENDIAN>)
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    add_compile_definitions($<$<COMPILE_LANGUAGE:C>:ARCH_IS_64BIT>)
else()
    add_compile_definitions($<$<COMPILE_LANGUAGE:C>:ARCH_IS_32BIT>)
endif()

if(IS_X86)
    if (CMAKE_SIZEOF_VOID_P EQUAL 8)
        add_compile_definitions($<$<COMPILE_LANGUAGE:C>:ARCH_IS_X86_64>)
        add_compile_definitions($<$<COMPILE_LANGUAGE:ASM_NASM>:NO_PREFIX>)
        add_compile_definitions($<$<COMPILE_LANGUAGE:ASM_NASM>:ARCH_IS_X86_64>)
    else()
        add_compile_definitions($<$<COMPILE_LANGUAGE:C>:ARCH_IS_IA32>)
    endif()

    if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
        add_compile_definitions($<$<COMPILE_LANGUAGE:ASM_NASM>:WINDOWS>)
    endif()

    set(ASM_FILES ${SRC_IA32})
else()
    add_compile_definitions($<$<COMPILE_LANGUAGE:C>:ARCH_IS_GENERIC>)
endif()

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    if(MSVC)
        add_compile_definitions($<$<COMPILE_LANGUAGE:C>:HAVE_PTHREAD>)
        add_compile_definitions($<$<COMPILE_LANGUAGE:C>:XVID_HAVE_PADDED_BS_BUFFER>)
        add_compile_definitions($<$<COMPILE_LANGUAGE:C>:_CRT_SECURE_NO_WARNINGS>)
    endif()

    set(DEF_FILES
        src/XvidDecoder.def
    )
else()
    set(DEF_FILES
        src/XvidDecoder.ld
    )
endif()

include_directories(src src/image/x86_asm)

add_library(${CMAKE_PROJECT_NAME} SHARED ${SRC_GENERIC} ${ASM_FILES} ${DEF_FILES})
