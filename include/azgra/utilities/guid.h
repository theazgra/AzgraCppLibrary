#pragma once

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <azgra/azgra.h>

namespace azgra::guid
{
    typedef boost::uuids::uuid GUID;

    GUID create_guid_from_bytes(const ByteArray &bytes);

    GUID generate_guid();

    ByteArray get_guid_bytes(const GUID &guid);
}