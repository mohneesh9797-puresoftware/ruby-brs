# Ruby bindings for brotli library.
# Copyright (c) 2019 AUTHORS, MIT License.

require "brs_ext"

require_relative "../../error"
require_relative "../../validation"

module BRS
  module Stream
    module Raw
      class Abstract
        def initialize(native_stream)
          @native_stream = native_stream
          @is_closed     = false
        end

        # -- write --

        def flush(&writer)
          write_result(&writer)
        end

        protected def flush_destination_buffer(&writer)
          result_bytesize = write_result(&writer)
          raise NotEnoughDestinationError, "not enough destination" if result_bytesize == 0
        end

        protected def write_result(&_writer)
          result = @native_stream.read_result
          yield result

          result.bytesize
        end

        # -- close --

        protected def do_not_use_after_close
          raise UsedAfterCloseError, "used after close" if closed?
        end

        def close(&writer)
          write_result(&writer)

          @native_stream.close
          @is_closed = true
        end

        def closed?
          @is_closed
        end
      end
    end
  end
end
