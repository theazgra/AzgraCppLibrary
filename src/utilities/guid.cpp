#include <azgra/utilities/guid.h>

namespace azgra
{
	namespace guid
	{
		GUID create_guid_from_bytes(const ByteArray& bytes)
		{
			always_assert(bytes.size() == 16);

			GUID result;
			std::memcpy(&result, bytes.data(), 16);

			return result;
		}

		GUID generate_guid()
		{
			GUID generatedGUID = boost::uuids::random_generator()();
			return generatedGUID;
		}

		ByteArray get_guid_bytes(const GUID& guid)
		{
			ByteArray bytes(16);
			std::memcpy(bytes.data(), &guid, 16);
			return bytes;
		}
	};
}