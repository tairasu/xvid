#ifndef _XVID_DECODER_H_
#define _XVID_DECODER_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    void* output;
    void* input;
    unsigned int input_size;
    int update;
    int zero0;
    int zero1;
} decoder_data_t;

int __cdecl InitializeDivxDecoder(unsigned int index, unsigned int width, unsigned int height);
int __cdecl SetOutputFormat(unsigned int index, unsigned int one, unsigned int width, unsigned int height);
int __cdecl DivxDecode(unsigned int index, void *data, unsigned int zero);
int __cdecl UnInitializeDivxDecoder(unsigned int index);

#ifdef __cplusplus
}
#endif

#endif
