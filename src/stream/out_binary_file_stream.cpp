#include <azgra/stream/out_binary_file_stream.h>

namespace azgra
{
	OutBinaryFileStream::OutBinaryFileStream(const char* fileName)
	{
		fileStream = std::ofstream(fileName, std::ios::out | std::ios::binary);
		isOpen = fileStream.is_open();
		always_assert(isOpen && "Failed to open ofstream.");
	}

	OutBinaryFileStream::~OutBinaryFileStream()
	{
		close_stream();
	}

	void OutBinaryFileStream::close_stream()
	{
		fileStream.flush();
		fileStream.close();
		isOpen = false;
	}

	size_t OutBinaryFileStream::get_position()
	{
		return static_cast<size_t>(fileStream.tellp());
	}

	void OutBinaryFileStream::write_byte(const byte& value)
	{
		always_assert(isOpen);
		fileStream.write(reinterpret_cast<const char*>(&value), 1);
	}

	void OutBinaryFileStream::write_bytes(const ByteArray& bytes)
	{
		always_assert(isOpen);
		fileStream.write(reinterpret_cast<const char*>(bytes.data()), bytes.size());
	}

	void OutBinaryFileStream::write_replicated_bytes(const byte& repValue, const size_t repCount)
	{
		always_assert(isOpen);
		byte* buffer = static_cast<byte*> (::operator new(repCount));
		std::memset(buffer, repValue, repCount);
		fileStream.write(reinterpret_cast<const char*>(buffer), repCount);
		std::free(buffer);
	}
}