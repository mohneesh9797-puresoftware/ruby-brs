# Ruby bindings for brotli library.
# Copyright (c) 2019 AUTHORS, MIT License.

require "brs_ext"

require_relative "abstract"
require_relative "../../error"
require_relative "../../option"
require_relative "../../validation"

module BRS
  module Stream
    module Raw
      class Compressor < Abstract
        BUFFER_LENGTH_NAMES = %i[destination_buffer_length].freeze

        def initialize(options = {})
          options = Option.get_compressor_options options, BUFFER_LENGTH_NAMES

          size_hint = options[:size_hint]
          Validation.validate_not_negative_integer size_hint unless size_hint.nil?

          native_stream = NativeCompressor.new options

          super native_stream
        end

        def write(source, &writer)
          do_not_use_after_close

          Validation.validate_string source
          Validation.validate_proc writer

          total_bytes_written = 0

          loop do
            bytes_written, need_more_destination  = @native_stream.write source
            total_bytes_written                  += bytes_written

            if need_more_destination
              source = source.byteslice bytes_written, source.bytesize - bytes_written
              flush_destination_buffer(&writer)
              next
            end

            unless bytes_written == source.bytesize
              # :nocov:
              # Compressor write should eat all provided "source" without remainder.
              raise UnexpectedError, "unexpected error"
              # :nocov:
            end

            break
          end

          total_bytes_written
        end

        def flush(&writer)
          do_not_use_after_close

          Validation.validate_proc writer

          loop do
            need_more_destination = @native_stream.flush

            if need_more_destination
              flush_destination_buffer(&writer)
              next
            end

            break
          end

          super

          nil
        end

        def close(&writer)
          return nil if closed?

          Validation.validate_proc writer

          loop do
            need_more_destination = @native_stream.finish

            if need_more_destination
              flush_destination_buffer(&writer)
              next
            end

            break
          end

          super

          nil
        end
      end
    end
  end
end
