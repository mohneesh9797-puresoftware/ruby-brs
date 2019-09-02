// Ruby bindings for brotli library.
// Copyright (c) 2019 AUTHORS, MIT License.

#include <brotli/decode.h>
#include <brotli/encode.h>

#include "ruby.h"

#include "brs_ext/error.h"
#include "brs_ext/option.h"

static inline VALUE get_option(VALUE options, const char *name)
{
  return rb_funcall(options, rb_intern("[]"), 1, ID2SYM(rb_intern(name)));
}

static inline unsigned long get_option_value(VALUE option, brs_ext_option_t type)
{
  if (type == BRS_EXT_OPTION_TYPE_MODE) {
    Check_Type(option, T_SYMBOL);

    ID id = SYM2ID(option);
    if (id == rb_intern("text")) {
      return BROTLI_MODE_TEXT;
    }
    else if (id == rb_intern("font")) {
      return BROTLI_MODE_FONT;
    }
    else if (id == rb_intern("generic")) {
      return BROTLI_MODE_GENERIC;
    }
    else {
      brs_ext_raise_error("ValidateError", "invalid mode option");
    }
  }

  if (type == BRS_EXT_OPTION_TYPE_BOOL) {
    int type = TYPE(option);
    if (type != T_TRUE && type != T_FALSE) {
      brs_ext_raise_error("ValidateError", "invalid bool option");
    }

    return type == T_TRUE ? 1 : 0;
  }

  if (type == BRS_EXT_OPTION_TYPE_FIXNUM) {
    Check_Type(option, T_FIXNUM);

    return rb_num2uint(option);
  }

  brs_ext_raise_error("ValidateError", "invalid option type");
}

void brs_ext_set_compressor_option(BrotliEncoderState *state_ptr, BrotliEncoderParameter param, VALUE options, const char *name, brs_ext_option_t type)
{
  VALUE option = get_option(options, name);

  if (option != Qnil) {
    uint32_t value = get_option_value(option, type);

    BROTLI_BOOL result = BrotliEncoderSetParameter(state_ptr, param, value);
    if (!result) {
      brs_ext_raise_error("ValidateError", "invalid param value");
    }
  }
}

void brs_ext_set_decompressor_option(BrotliDecoderState *state_ptr, BrotliDecoderParameter param, VALUE options, const char *name, brs_ext_option_t type)
{
  VALUE option = get_option(options, name);

  if (option != Qnil) {
    uint32_t value = get_option_value(option, type);

    BROTLI_BOOL result = BrotliDecoderSetParameter(state_ptr, param, value);
    if (!result) {
      brs_ext_raise_error("ValidateError", "invalid param value");
    }
  }
}

unsigned long brs_ext_get_fixnum_option(VALUE options, const char *name)
{
  VALUE option = get_option(options, name);

  return get_option_value(option, BRS_EXT_OPTION_TYPE_FIXNUM);
}
