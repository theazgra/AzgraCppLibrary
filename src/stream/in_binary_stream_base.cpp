#include <azgra/stream/in_binary_stream_base.h>

namespace azgra
{

	bool InBinaryStreamBase::is_open() const
	{
		return this->isOpen;
	}

	byte InBinaryStreamBase::consume_byte()
	{
		always_assert(this->isOpen);
		return consume_bytes(1)[0];
	}

	bool InBinaryStreamBase::consume_bool(const azgra::u16 byteCount)
	{
		always_assert(this->isOpen);

		auto bytes = consume_bytes(byteCount);
		azgra::i32 sum = 0;
		for (size_t i = 0; i < byteCount; i++)
			sum |= bytes[i];

		return (sum != 0);
	}

	azgra::i16 InBinaryStreamBase::consume_short16()
	{
		always_assert(this->isOpen);
		return bytes_to_i16(consume_bytes(2));
	}

	azgra::u16 InBinaryStreamBase::consume_ushort16()
	{
		always_assert(this->isOpen);
		return bytes_to_u16(consume_bytes(2));
	}

	azgra::i32 InBinaryStreamBase::consume_int32()
	{
		always_assert(this->isOpen);
		return bytes_to_i32(consume_bytes(4));
	}

	azgra::u32 InBinaryStreamBase::consume_uint32()
	{
		always_assert(this->isOpen);
		return bytes_to_u32(consume_bytes(4));
	}

	azgra::i64 InBinaryStreamBase::consume_long64()
	{
		always_assert(this->isOpen);
		return bytes_to_i64(consume_bytes(8));
	}

	azgra::u64 InBinaryStreamBase::consume_ulong64()
	{
		always_assert(this->isOpen);
		return bytes_to_u64(consume_bytes(8));
	}

	float InBinaryStreamBase::consume_float()
	{
		always_assert(this->isOpen);
		return bytes_to_float(consume_bytes(4));
	}

	double InBinaryStreamBase::consume_double()
	{
		always_assert(this->isOpen);
		return bytes_to_double(consume_bytes(8));
	}

	ByteArray InBinaryStreamBase::move_and_consume_bytes(const azgra::i64 position, const azgra::i64 byteCount)
	{
		always_assert(this->isOpen);
		move_to(position);
		return consume_bytes(byteCount);
	}

	void InBinaryStreamBase::consume_into(ByteArray& dst, size_t pos, size_t byteCount)
	{
		always_assert(this->isOpen);
		always_assert(dst.size() >= (pos + byteCount));
		ByteArray tmpBuffer = consume_bytes(byteCount);
		for (size_t i = 0; i < byteCount; i++)
		{
			dst[pos + i] = tmpBuffer[i];
		}
	}

} // namespace azgra