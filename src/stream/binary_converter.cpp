#include <azgra/stream/binary_converter.h>

namespace azgra
{

	azgra::i16 bytes_to_i16(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		always_assert(bytes.size() >= sizeof(azgra::i16));
		always_assert(fromIndex <= bytes.size() - sizeof(azgra::i16));

#ifdef AZGRA_LITTLE_ENDIAN
		auto bit = &bytes[fromIndex];

		if (fromIndex % 2 == sizeof(azgra::i16))
		{
			return (*((azgra::i16*)bit));
		}
		else
		{
			return (azgra::i16)((*bit) | (*(bit + 1) << 8));
		}
#else
		const byte* bytePtr = &bytes[fromIndex];
		return (azgra::i16)((*(bytePtr + 1)) | ((*bytePtr) << 8));
#endif
	}

	azgra::i32 bytes_to_i32(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		always_assert(bytes.size() >= sizeof(azgra::i32));
		always_assert(fromIndex <= bytes.size() - sizeof(azgra::i32));

#ifdef AZGRA_LITTLE_ENDIAN
		auto bit = &bytes[fromIndex];

		if (fromIndex % sizeof(azgra::i32) == 0)
		{
			return (*((azgra::i32*)bit));
		}
		else
		{
			return (*bit | (*(bit + 1) << 8) | (*(bit + 2) << 16) | (*(bit + 3) << 24));
		}
#else
		const byte* bytePtr = &bytes[fromIndex];
		return (((*bytePtr) << 24) | ((*(bytePtr + 1)) << 16) | ((*(bytePtr + 2)) << 8) | (*(bytePtr + 3)));
#endif
	}

	azgra::i64 bytes_to_i64(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		always_assert(bytes.size() >= sizeof(azgra::i64));
		always_assert(fromIndex <= bytes.size() - sizeof(azgra::i64));

		// Little endian
		auto bit = &bytes[fromIndex];
		if (fromIndex % sizeof(azgra::i64) == 0)
		{
			return (*((azgra::i64*)bit));
		}
		else
		{
			azgra::i32 i1 = (*bit) | (*(bit + 1) << 8) | (*(bit + 2) << 16) | (*(bit + 3) << 24);
			azgra::i32 i2 = (*(bit + 4)) | (*(bit + 5) << 8) | (*(bit + 6) << 16) | (*(bit + 7) << 24);
			return ((azgra::u32)i1 | ((azgra::i64)i2 << 32));
		}
	}

	float bytes_to_float(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		always_assert(bytes.size() >= sizeof(float));
		always_assert(fromIndex <= bytes.size() - sizeof(float));

		float result;
		memcpy(&result, (bytes.data() + fromIndex), sizeof(float));
		return result;
	}

	double bytes_to_double(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		always_assert(bytes.size() >= sizeof(double));
		always_assert(fromIndex <= bytes.size() - sizeof(double));

		double result;
		memcpy(&result, (bytes.data() + fromIndex), sizeof(double));
		return result;
	}

	azgra::u16 bytes_to_u16(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		return ((azgra::u16)bytes_to_i16(bytes, fromIndex));
	}

	azgra::u32 bytes_to_u32(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		return ((azgra::u32)bytes_to_i32(bytes, fromIndex));
	}

	azgra::u64 bytes_to_u64(const ByteArray& bytes, const azgra::u64 fromIndex)
	{
		return ((azgra::u64)bytes_to_i64(bytes, fromIndex));
	}

	std::string utf8bytes_to_string(const ByteArray& bytes)
	{
		return utf8bytes_to_string(bytes, 0, bytes.size());
	}

	std::string utf8bytes_to_string(const ByteArray& bytes, const azgra::u64 fromIndex, const azgra::u32 byteCount)
	{
		if (bytes.size() == 0)
			return "";
		auto fromIt = bytes.begin() + fromIndex;
		ByteArray stringBytes(fromIt, fromIt + byteCount);
		std::string result = boost::locale::conv::from_utf<char>((char*)stringBytes.data(), "UTF-8");
		return result;
	}

	// Convert string to bytes.
	ByteArray string_to_bytes(const std::string& text)
	{
		ByteArray bytes(text.length());
		const char* textPtr = text.data();
		for (size_t i = 0; i < text.length(); i++)
		{
			bytes[i] = *(textPtr + i);
		}
		return bytes;
	}

	ByteArray string_to_bytes_null_terminated(const std::string& text)
	{
		size_t len = text.length();
		ByteArray bytes(len);
		const char* textPtr = text.data();
		for (size_t i = 0; i < len; i++)
		{
			bytes[i] = *(textPtr + i);
			if (i == (len - 1) && (bytes[i] != 0))
			{
				bytes.resize(len + 1);
				bytes[len] = 0;
			}
		}
		return bytes;
	}


	ByteArray short16_to_bytes(const azgra::i16& value)
	{
		ByteArray shortBytes(sizeof(azgra::i16));
		std::memcpy(shortBytes.data(), &value, sizeof(azgra::i16));
		return shortBytes;
	}

	ByteArray ushort16_to_bytes(const azgra::u16& value)
	{
		ByteArray ushortBytes(sizeof(azgra::u16));
		std::memcpy(ushortBytes.data(), &value, sizeof(azgra::u16));
		return ushortBytes;
	}

	ByteArray int32_to_bytes(const azgra::i32& value)
	{
		ByteArray intBytes(sizeof(azgra::i32));
		std::memcpy(intBytes.data(), &value, sizeof(azgra::i32));
		return intBytes;
	}

	ByteArray uint32_to_bytes(const azgra::u32& value)
	{
		ByteArray uintBytes(sizeof(azgra::u32));
		std::memcpy(uintBytes.data(), &value, sizeof(azgra::u32));
		return uintBytes;
	}

	ByteArray long64_to_bytes(const azgra::i64& value)
	{
		ByteArray longBytes(sizeof(azgra::i64));
		std::memcpy(longBytes.data(), &value, sizeof(azgra::i64));
		return longBytes;
	}

	ByteArray ulong64_to_bytes(const azgra::u64& value)
	{
		ByteArray ulongBytes(sizeof(azgra::u64));
		std::memcpy(ulongBytes.data(), &value, sizeof(azgra::u64));
		return ulongBytes;
	}

	ByteArray float_to_bytes(const float& value)
	{
		ByteArray floatBytes(sizeof(float));
		std::memcpy(floatBytes.data(), &value, sizeof(float));
		return floatBytes;
	}

	ByteArray double_to_bytes(const double& value)
	{
		ByteArray doubleBytes(sizeof(double));
		std::memcpy(doubleBytes.data(), &value, sizeof(double));
		return doubleBytes;
	}

	void int32_to_bytes(const azgra::i32& i, ByteArray& buffer, size_t bufferPosition)
	{
		always_assert(buffer.size() - bufferPosition >= sizeof(azgra::i32));
		std::memcpy((buffer.data() + bufferPosition), &i, sizeof(azgra::i32));
	}

	void ushort16_to_bytes(const azgra::u16& x, ByteArray& buffer, size_t bufferPosition)
	{
		always_assert(buffer.size() - bufferPosition >= sizeof(azgra::u16));
		std::memcpy((buffer.data() + bufferPosition), &x, sizeof(azgra::u16));
	}

	std::vector<azgra::u16> bytes_to_ushort_array(const ByteArray& data)
	{
		std::vector<azgra::u16> result(data.size() / sizeof(azgra::u16));
		std::memcpy(result.data(), data.data(), data.size());
		return result;
	}

	std::vector<azgra::i32> bytes_to_int_array(const ByteArray& data)
	{
		std::vector<azgra::i32> result(data.size() / sizeof(azgra::i32));
		std::memcpy(result.data(), data.data(), data.size());
		return result;
	}

	ByteArray int_array_to_bytes(const std::vector<azgra::i32>& data)
	{
		ByteArray result;
		result.resize(data.size() * sizeof(azgra::i32));
		std::memcpy(result.data(), data.data(), sizeof(azgra::i32) * data.size());
		return result;
	}

	ByteArray uint_array_to_bytes(const std::vector<azgra::u32>& data)
	{
		ByteArray result;
		result.resize(data.size() * sizeof(azgra::u32));
		std::memcpy(result.data(), data.data(), sizeof(azgra::u32) * data.size());
		return result;
	}

	ByteArray ushort_array_to_bytes(const std::vector<azgra::u16>& data)
	{
		ByteArray result;
		result.resize(data.size() * sizeof(azgra::u16));
		std::memcpy(result.data(), data.data(), sizeof(azgra::u16) * data.size());
		return result;
	}

} // namespace azgra