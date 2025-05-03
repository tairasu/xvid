#include "XvidDecoder.h"

#include <string.h>

#include "xvid.h"


#define XVID_MAX_DECODERS 11


static xvid_gbl_init_t g_xvid_init = { 0, 0, 0 };
static xvid_dec_create_t g_xvid_decoders[XVID_MAX_DECODERS];


int __cdecl
InitializeDivxDecoder(unsigned int index,
                      unsigned int width,
                      unsigned int height)
{
    int result;

    result = 0;

    if (g_xvid_init.version == 0) {
        memset(&g_xvid_init, 0, sizeof(xvid_gbl_init_t));
        g_xvid_init.version = XVID_VERSION;

        result = xvid_global(NULL, XVID_GBL_INIT, &g_xvid_init, NULL);

        if (result < 0) {
            memset(&g_xvid_init, 0, sizeof(xvid_gbl_init_t));
            return -result;
        }
    }

    if (index < 1 || index >= XVID_MAX_DECODERS) {
        return XVID_ERR_FAIL;
    }

    if (g_xvid_decoders[index].handle) {
        return XVID_ERR_FAIL;
    }

    memset(&g_xvid_decoders[index], 0, sizeof(xvid_dec_create_t));
    g_xvid_decoders[index].version = XVID_VERSION;
    g_xvid_decoders[index].width = (int) width;
    g_xvid_decoders[index].height = (int) height;

    result = xvid_decore(NULL, XVID_DEC_CREATE, &g_xvid_decoders[index], NULL);

    if (result < 0) {
        memset(&g_xvid_decoders[index], 0, sizeof(xvid_dec_create_t));
        return -result;
    }

    return result;
}

int __cdecl
SetOutputFormat(unsigned int index,
                unsigned int one,
                unsigned int width,
                unsigned int height)
{
    (void) one;

    if (index < 1 || index >= XVID_MAX_DECODERS) {
        return -XVID_ERR_FAIL;
    }

    if (!g_xvid_decoders[index].handle) {
        return -XVID_ERR_FAIL;
    }

    if (g_xvid_decoders[index].width == 0 ||
        g_xvid_decoders[index].height == 0)
    {
        g_xvid_decoders[index].width = width;
        g_xvid_decoders[index].height = height;
    }

    if (g_xvid_decoders[index].width != width ||
        g_xvid_decoders[index].height != height)
    {
        return -XVID_ERR_FAIL;
    }

    return 0;
}

int __cdecl
DivxDecode(unsigned int index,
           void *data,
           unsigned int zero)
{
    int result;
    decoder_data_t *d;
    xvid_dec_frame_t frame;
    xvid_dec_stats_t stats;

    (void) zero;

    d = (decoder_data_t *) data;
    memset(&frame, 0, sizeof(xvid_dec_frame_t));
    memset(&stats, 0, sizeof(xvid_dec_stats_t));

    frame.version = XVID_VERSION;
    frame.bitstream = d->input;
    frame.length = (int) d->input_size;
    frame.output.csp = XVID_CSP_BGRA | XVID_CSP_VFLIP;
    frame.output.plane[0] = d->output;
    frame.output.stride[0] = g_xvid_decoders[index].width * 4;

    stats.version = XVID_VERSION;

    while (frame.length > 0) {
        result = xvid_decore(g_xvid_decoders[index].handle, XVID_DEC_DECODE, &frame, NULL);

        if (result < 0) {
            return -result;
        }

        if (result == 0) {
            break;
        }

        frame.bitstream = (char *) frame.bitstream + result;
        frame.length -= result;
    }

    return 0;
}

int __cdecl
UnInitializeDivxDecoder(unsigned int index)
{
    if (index < 1 || index >= XVID_MAX_DECODERS) {
        return -XVID_ERR_FAIL;
    }

    if (!g_xvid_decoders[index].handle) {
        return -XVID_ERR_FAIL;
    }

    xvid_decore(g_xvid_decoders[index].handle, XVID_DEC_DESTROY, NULL, NULL);
    memset(&g_xvid_decoders[index], 0, sizeof(xvid_dec_create_t));

    return 0;
}
