#pragma once
#include <azgra/stream/out_binary_stream_base.h>
#include <azgra/collection/vector_utilities.h>

namespace azgra
{
	class OutBinaryBufferStream : public OutBinaryStreamBase
	{
	private:
		ByteArray buffer;
		size_t bufferPtr;
		void ensure_capacity(const size_t& writeSize);
	public:
		OutBinaryBufferStream(const size_t initialBufferSize = 512);
		~OutBinaryBufferStream();

		size_t get_position() override;
		// Write byte to the stream.
		void write_byte(const byte& value) override;
		// Write bytes to the stream.
		void write_bytes(const ByteArray& bytes) override;

		ByteArray get_buffer_data() const;
	};
}