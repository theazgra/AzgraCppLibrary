#pragma once

#include <azgra/stream/out_binary_stream_base.h>
#include <fstream>

namespace azgra
{
	class OutBinaryFileStream : public OutBinaryStreamBase
	{
	private:
		std::ofstream fileStream;
	public:
		OutBinaryFileStream(const char* fileName);
		~OutBinaryFileStream();
		void close_stream() override;
		size_t get_position() override;
		// Write byte to the stream.
		void write_byte(const byte& value) override;
		// Write bytes to the stream.
		void write_bytes(const ByteArray& bytes) override;
		void write_replicated_bytes(const byte& repValue, const size_t repCount) override;
	};
};