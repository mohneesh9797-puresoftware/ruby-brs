// Ruby bindings for brotli library.
// Copyright (c) 2019 AUTHORS, MIT License.

#if !defined(BRS_EXT_OPTIONS_H)
#define BRS_EXT_OPTIONS_H

#include <brotli/decode.h>
#include <brotli/encode.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "ruby.h"

enum {
  BRS_EXT_OPTION_TYPE_BOOL = 1,
  BRS_EXT_OPTION_TYPE_FIXNUM,
  BRS_EXT_OPTION_TYPE_MODE
};

typedef uint_fast8_t brs_ext_option_t;

bool          brs_ext_get_bool_option(VALUE options, const char* name);
unsigned long brs_ext_get_fixnum_option(VALUE options, const char* name);
unsigned long brs_ext_get_mode_option(VALUE options, const char* name);

void brs_ext_set_compressor_option(
  BrotliEncoderState*    state_ptr,
  BrotliEncoderParameter param, VALUE options, brs_ext_option_t type, const char* name);

void brs_ext_set_decompressor_option(
  BrotliDecoderState*    state_ptr,
  BrotliDecoderParameter param, VALUE options, brs_ext_option_t type, const char* name);

#define BRS_EXT_SET_COMPRESSOR_FIXNUM_OPTION(param, name) \
  brs_ext_set_compressor_option(state_ptr, param, options, BRS_EXT_OPTION_TYPE_FIXNUM, #name);

#define BRS_EXT_SET_COMPRESSOR_BOOL_OPTION(param, name) \
  brs_ext_set_compressor_option(state_ptr, param, options, BRS_EXT_OPTION_TYPE_BOOL, #name);

#define BRS_EXT_SET_COMPRESSOR_MODE_OPTION(param, name) \
  brs_ext_set_compressor_option(state_ptr, param, options, BRS_EXT_OPTION_TYPE_MODE, #name);

#define BRS_EXT_SET_DECOMPRESSOR_BOOL_OPTION(param, name) \
  brs_ext_set_decompressor_option(state_ptr, param, options, BRS_EXT_OPTION_TYPE_BOOL, #name);

#define BRS_EXT_SET_COMPRESSOR_OPTIONS(state_ptr, options)                                                             \
  BRS_EXT_SET_COMPRESSOR_MODE_OPTION(BROTLI_PARAM_MODE, mode);                                                         \
  BRS_EXT_SET_COMPRESSOR_FIXNUM_OPTION(BROTLI_PARAM_QUALITY, quality);                                                 \
  BRS_EXT_SET_COMPRESSOR_FIXNUM_OPTION(BROTLI_PARAM_LGWIN, lgwin);                                                     \
  BRS_EXT_SET_COMPRESSOR_FIXNUM_OPTION(BROTLI_PARAM_LGBLOCK, lgblock);                                                 \
  BRS_EXT_SET_COMPRESSOR_BOOL_OPTION(BROTLI_PARAM_DISABLE_LITERAL_CONTEXT_MODELING, disable_literal_context_modeling); \
  BRS_EXT_SET_COMPRESSOR_FIXNUM_OPTION(BROTLI_PARAM_SIZE_HINT, size_hint);                                             \
  BRS_EXT_SET_COMPRESSOR_BOOL_OPTION(BROTLI_PARAM_LARGE_WINDOW, large_window);

#define BRS_EXT_SET_DECOMPRESSOR_OPTIONS(state_ptr, options)                                                                     \
  BRS_EXT_SET_DECOMPRESSOR_BOOL_OPTION(BROTLI_DECODER_PARAM_DISABLE_RING_BUFFER_REALLOCATION, disable_ring_buffer_reallocation); \
  BRS_EXT_SET_DECOMPRESSOR_BOOL_OPTION(BROTLI_DECODER_PARAM_LARGE_WINDOW, large_window);

#define BRS_EXT_GET_BUFFER_LENGTH_OPTION(options, name) \
  size_t name = brs_ext_get_fixnum_option(options, #name);

void brs_ext_option_exports(VALUE root_module);

#endif // BRS_EXT_OPTIONS_H
