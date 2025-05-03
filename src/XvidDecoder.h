#ifndef _XVID_DECODER_H_
#define _XVID_DECODER_H_

#ifdef __cplusplus
extern "C" {
#endif

#if defined(ARCH_IS_IA32) && defined(_WIN32)
#  if defined(_MSC_VER)
#    define XVDEC_CALL __cdecl
#  elif defined(__GNUC__) || defined(__clang__)
#    define XVDEC_CALL __attribute__((cdecl))
#  else
#    define XVDEC_CALL
#endif
#else
#  define XVDEC_CALL
#endif

typedef struct {
    void* output;
    void* input;
    unsigned int input_size;
    int update;
    int zero0;
    int zero1;
} decoder_data_t;

int XVDEC_CALL InitializeDivxDecoder(unsigned int index, unsigned int width, unsigned int height);
int XVDEC_CALL SetOutputFormat(unsigned int index, unsigned int one, unsigned int width, unsigned int height);
int XVDEC_CALL DivxDecode(unsigned int index, void *data, unsigned int zero);
int XVDEC_CALL UnInitializeDivxDecoder(unsigned int index);

#ifdef __cplusplus
}
#endif

#endif
