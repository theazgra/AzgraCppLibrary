#include <azgra/stream/out_binary_stream_base.h>

namespace azgra
{
	bool OutBinaryStreamBase::is_open() const
	{
		return this->isOpen;
	}

	// Write boolean value in defined number of bytes.
	void OutBinaryStreamBase::write_bool(const bool& value, const azgra::u16 byteCount)
	{
		always_assert(isOpen);
		for (size_t i = 0; i < byteCount; i++)
		{
			write_byte(value ? 255 : 0);
		}
	}

	// Write short value to the stream.
	void OutBinaryStreamBase::write_short16(const azgra::i16& value)
	{
		always_assert(isOpen);
		write_bytes(short16_to_bytes(value));
	}

	// Write ushort value to the stream.
	void OutBinaryStreamBase::write_ushort16(const azgra::u16& value)
	{
		always_assert(isOpen);
		write_bytes(ushort16_to_bytes(value));
	}

	// Write int value to the stream.
	void OutBinaryStreamBase::write_int32(const azgra::i32& value)
	{
		always_assert(isOpen);
		write_bytes(int32_to_bytes(value));
	}

	// Write uint value to the stream.
	void OutBinaryStreamBase::write_uint32(const azgra::u32& value)
	{
		always_assert(isOpen);
		write_bytes(uint32_to_bytes(value));
	}

	// Write long value to the stream.
	void OutBinaryStreamBase::write_long64(const azgra::i64& value)
	{
		always_assert(isOpen);
		write_bytes(long64_to_bytes(value));
	}

	// Write ulong value to the stream.
	void OutBinaryStreamBase::write_ulong64(const azgra::u64& value)
	{
		always_assert(isOpen);
		write_bytes(ulong64_to_bytes(value));
	}

	// Write float value to the stream.
	void OutBinaryStreamBase::write_float(const float& value)
	{
		always_assert(isOpen);
		write_bytes(float_to_bytes(value));
	}

	// Write float value to the stream.
	void OutBinaryStreamBase::write_double(const double& value)
	{
		always_assert(isOpen);
		write_bytes(double_to_bytes(value));
	}

	// Write bytes from buffer to the stream.
	void OutBinaryStreamBase::write_bytes_from_buffer(const ByteArray& buffer, const size_t bufferPos, const size_t byteCount)
	{
		always_assert(isOpen);
		for (size_t i = 0; i < byteCount; i++)
		{
			write_byte(buffer[bufferPos + i]);
		}
	}

	void OutBinaryStreamBase::write_replicated_bytes(const byte& repValue, const size_t repCount)
	{
		always_assert(isOpen);
		for (size_t i = 0; i < repCount; i++)
		{
			write_byte(repValue);
		}
	}
};