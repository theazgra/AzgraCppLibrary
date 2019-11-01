#include <azgra/string/ascii_string.h>

namespace azgra
{
    namespace string
    {
        //TODO: Add asserts.

        char *AsciiString::alloc_string(const size_t &length)
        {
            assert(length > 0);
            size_t allocSize = (sizeof(char) * length);
            char *memory = static_cast<char *>(::operator new(allocSize));
            return memory;
        }

        char *AsciiString::realloc_string(const size_t &length, char *oldString, const size_t copyLen)
        {
            assert(length > copyLen);

            char *memory = alloc_string(length);
            memcpy(memory, oldString, copyLen);
            // Free old string memory.
            free_string(oldString);
            return memory;
        }

        void AsciiString::free_string(char *memory)
        {
            if (memory != nullptr)
            {
                ::operator delete(memory);
                memory = nullptr;
            }
        }

        size_t AsciiString::c_string_length(const char *cString)
        {
            size_t len = 0;
            azgra::i32 i = 0;

            for (i = 0; cString[i] != 0; i++)
                len++;

            return len;
        }

        void AsciiString::internal_initalize(const char *string, const size_t length)
        {
            size_t inputStringLen = length;
            _isEmpty = (inputStringLen == 0);
            _length = inputStringLen;
            _string = alloc_string(_length + 1);

            for (size_t i = 0; i < inputStringLen; i++)
                _string[i] = string[i];

            _string[_length] = '\0';
        }

        void AsciiString::internal_initalize(const char *string)
        {
            size_t inputStringLen = c_string_length(string);
            internal_initalize(string, inputStringLen);
        }

        AsciiString::AsciiString(const char *cString)
        {
            internal_initalize(cString);
        }

        AsciiString::AsciiString(char *cString, const size_t length, bool noAlloc)
        {
            assert(noAlloc);
            _string = cString;
            _length = length;
            _isEmpty = (length == 0);
        }

        AsciiString::AsciiString(const std::vector<const char *> &strings)
        {
            _length = 0;
            _string = nullptr;
            multi_append(strings);
            _isEmpty = (_length <= 0);
        }

        AsciiString::AsciiString()
        {
            _string = nullptr;
            _length = 0;
            _isEmpty = true;
        }

        AsciiString::operator const char *() const
        {
            return _string;
        }

        char &AsciiString::operator[](const azgra::i32 &index)
        {
            return _string[index];
        }

        char &AsciiString::at(const azgra::i32 &index)
        {
            return _string[index];
        }

        char const &AsciiString::at(const azgra::i32 &index) const
        {
            return _string[index];
        }

        void AsciiString::operator=(const char *cString)
        {
            free_string(_string);
            internal_initalize(cString);
        }

        void AsciiString::operator+=(const char *cString)
        {
            concat(cString);
        }

        void AsciiString::operator+=(const char c)
        {
            char stringToAdd[1] = {c};
            this->operator+=(stringToAdd);
        }

        AsciiString AsciiString::operator+(const char *cString) const
        {
            AsciiString result(_string);
            result.concat(cString);
            return result;
        }

        AsciiString AsciiString::operator+(const char c) const
        {
            AsciiString result(_string);
            char stringToAdd[1] = {c};
            result.concat(stringToAdd);
            return result;
        }

        bool AsciiString::operator==(const char *cString) const
        {
            return equals(cString);
        }

        bool AsciiString::operator==(const AsciiString &string) const
        {
            return equals(string);
        }

        AsciiString::~AsciiString()
        {
            free_string(_string);
            _length = 0;
        }

        void AsciiString::internal_concat(const char *string, const size_t length)
        {
            if (length == 0)
                return;

            size_t newLength = _length + length + 1;
            char *newString = realloc_string(newLength, _string, _length);

            free_string(_string);

            _string = newString;

            for (size_t i = _length; i < newLength - 1; i++)
                _string[i] = string[i - _length];

            _string[newLength - 1] = '\0';

            _length = newLength - 1;
        }

        void AsciiString::concat(const char *cString)
        {
            size_t cStringLen = c_string_length(cString);
            internal_concat(cString, cStringLen);
        }

        const char *AsciiString::get_c_string() const
        {
            return _string;
        }

        size_t AsciiString::length() const
        {
            return _length;
        }

        azgra::i32 AsciiString::index_of(const char &c, const size_t fromIndex) const
        {
            if (fromIndex >= _length)
                return -1;

            for (size_t i = fromIndex; i < _length; i++)
            {
                if (_string[i] == c)
                {
                    return i;
                }
            }
            return -1;
        }

        azgra::i32 AsciiString::index_of(const char *string, const size_t fromIndex) const
        {
            size_t matchLen = c_string_length(string);
            if (matchLen > _length || fromIndex >= _length)
            {
                return -1;
            }

            size_t searchFrom = fromIndex;
            azgra::i32 indexOfFirstChar = index_of(string[0], searchFrom);
            bool found = false;

            while (indexOfFirstChar != -1)
            {
                if (indexOfFirstChar + (matchLen - 1) > _length)
                    return -1;

                found = true;
                for (size_t matchIndex = 1; matchIndex < matchLen; matchIndex++)
                {
                    if (_string[indexOfFirstChar + matchIndex] != string[matchIndex])
                    {
                        found = false;
                        break;
                    }
                }

                if (found)
                    return indexOfFirstChar;
                else
                {
                    searchFrom = indexOfFirstChar + 1;
                    indexOfFirstChar = index_of(string[0], searchFrom);
                }
            }

            return -1;
        }

        azgra::i32 AsciiString::last_index_of(const char &c, const size_t fromIndex) const
        {
            return last_index_of(c, fromIndex, _length - 1);
        }

        azgra::i32 AsciiString::last_index_of(const char &c, const size_t fromIndex, const size_t toIndex) const
        {
            if (fromIndex >= _length)
                return -1;

            for (azgra::i32 i = (azgra::i32) toIndex; i >= (azgra::i32) fromIndex; i--)
            {
                if (_string[i] == c)
                {
                    return i;
                }
            }
            return -1;
        }

        azgra::i32 AsciiString::last_index_of(const char *string, const size_t fromIndex) const
        {
            size_t matchLen = c_string_length(string);
            if (matchLen > _length || fromIndex >= _length)
            {
                return -1;
            }

            size_t searchUpperBound = _length - 1;
            azgra::i32 indexOfLastChar = last_index_of(string[matchLen - 1], 0, searchUpperBound);
            bool found = false;

            while (indexOfLastChar != -1)
            {
                if (indexOfLastChar - (matchLen - 1) < fromIndex)
                    return -1;

                found = true;
                for (size_t matchIndex = 1; matchIndex < matchLen; matchIndex++)
                {
                    if (_string[indexOfLastChar - matchIndex] != string[matchLen - matchIndex - 1])
                    {
                        found = false;
                        break;
                    }
                }

                if (found)
                    return indexOfLastChar - (matchLen - 1);
                else
                {
                    searchUpperBound = indexOfLastChar - 1;
                    indexOfLastChar = last_index_of(string[matchLen - 1], 0, searchUpperBound);
                }
            }

            return -1;
        }

        bool AsciiString::contains(const char &c) const
        {
            return (index_of(c) != -1);
        }

        bool AsciiString::contains(const char *string) const
        {
            return (index_of(string) != -1);
        }

        size_t AsciiString::count(const char &c) const
        {
            size_t result = 0;
            size_t fromIndex = 0;

            azgra::i32 matchIndex = index_of(c, fromIndex);

            while (matchIndex != -1)
            {
                ++result;
                fromIndex = matchIndex + 1;
                matchIndex = index_of(c, fromIndex);
            }
            return result;
        }

        size_t AsciiString::count(const char *string) const
        {
            size_t result = 0;
            size_t fromIndex = 0;
            size_t matchLen = c_string_length(string);
            if (matchLen > _length)
                return 0;

            azgra::i32 matchIndex = index_of(string, fromIndex);
            while (matchIndex != -1)
            {
                ++result;
                fromIndex = matchIndex + matchLen;
                matchIndex = index_of(string, fromIndex);
            }
            return result;
        }

        bool AsciiString::starts_with(const char &c) const
        {
            return (index_of(c) == 0);
        }

        bool AsciiString::starts_with(const char *string) const
        {
            return (index_of(string) == 0);
        }

        bool AsciiString::ends_with(const char &c) const
        {
            return (last_index_of(c) == (azgra::i64) (_length - 1));
        }

        bool AsciiString::ends_with(const char *string) const
        {
            return (last_index_of(string) == (azgra::i64) (_length - c_string_length(string)));
        }

        bool AsciiString::equals(const AsciiString &string) const
        {
            return equals(string._string);
        }

        bool AsciiString::equals(const char *string) const
        {
            if (_length != c_string_length(string))
                return false;

            for (size_t i = 0; i < _length; i++)
            {
                if (_string[i] != string[i])
                    return false;
            }
            return true;
        }

        void AsciiString::to_upper()
        {
            for (size_t i = 0; i < _length; i++)
            {
                char c = _string[i];
                if (c >= 'a' && c <= 'z')
                    _string[i] -= AsciiCaseOffset;
            }
        }

        void AsciiString::to_lower()
        {
            for (size_t i = 0; i < _length; i++)
            {
                char c = _string[i];
                if (c >= 'A' && c <= 'Z')
                    _string[i] += AsciiCaseOffset;
            }
        }

        void AsciiString::replace(const char &oldChar, const char &newChar)
        {
            for (size_t i = 0; i < _length; i++)
            {
                if (_string[i] == oldChar)
                    _string[i] = newChar;
            }
        }

        void AsciiString::replace(const char *oldString, const char *newString)
        {
            size_t matchLen = c_string_length(oldString);
            size_t matchCount = count(oldString);
            if (matchCount == 0 || matchLen == 0)
                return;

            size_t replaceStringLen = c_string_length(newString);
            size_t newLength = _length - (matchCount * matchLen) + (matchCount * replaceStringLen);
            char *newStringMemory = alloc_string(newLength);

            size_t searchFrom = 0;
            size_t newStringIndex = 0;
            azgra::i32 index = index_of(oldString, searchFrom);

            while (index != -1)
            {
                // Copy all in front of match.
                for (azgra::i32 i = searchFrom; i < index; i++)
                    newStringMemory[newStringIndex++] = _string[i];

                // Copy replace string into new string.
                for (size_t i = 0; i < replaceStringLen; i++)
                {
                    newStringMemory[newStringIndex++] = newString[i];
                }

                searchFrom = index + matchLen;
                index = index_of(oldString, searchFrom);
            }

            // Copy remaining old content.
            for (size_t i = searchFrom; i < _length; i++)
                newStringMemory[newStringIndex++] = _string[i];

            free_string(_string);
            _string = newStringMemory;
            _length = newLength;
        }

        void AsciiString::remove(const char &c)
        {
            size_t removeCount = count(c);
            if (removeCount == 0)
                return;

            size_t newLength = _length - removeCount;
            char *newString = alloc_string(newLength);
            size_t index = 0;
            for (size_t i = 0; i < _length; i++)
            {
                if (_string[i] != c)
                {
                    newString[index++] = _string[i];
                }
            }

            free_string(_string);
            _length = newLength;
            _string = newString;
        }

        void AsciiString::remove(const char *string)
        {
            size_t matchLen = c_string_length(string);
            if (matchLen == 0)
                return;

            size_t matchCount = count(string);
            size_t newLen = _length - (matchCount * matchLen);

            char *newString = alloc_string(newLen);

            size_t searchFrom = 0;
            size_t newStringIndex = 0;
            azgra::i32 index = index_of(string, searchFrom);

            while (index != -1)
            {
                // Copy all in front of match.
                for (azgra::i32 i = searchFrom; i < index; i++)
                    newString[newStringIndex++] = _string[i];

                searchFrom = index + matchLen;
                index = index_of(string, searchFrom);
            }

            // Copy remaining old content.
            for (size_t i = searchFrom; i < _length; i++)
                newString[newStringIndex++] = _string[i];

            free_string(_string);
            _string = newString;
            _length = newLen;
        }

        AsciiString AsciiString::substring(const size_t fromIndex) const
        {
            return substring(fromIndex, _length - fromIndex);
        }

        AsciiString AsciiString::substring(const size_t fromIndex, const size_t length) const
        {
            assert(fromIndex < _length && (length - fromIndex) <= _length);

            char *subsMemory = alloc_string(length + 1);
            AsciiString result(subsMemory);

            for (size_t i = fromIndex; i < fromIndex + length; i++)
            {
                result[i - fromIndex] = _string[i];
            }
            result[length] = '\0';

            return result;
        }

        AsciiString AsciiString::replicate(const char *cString, const azgra::i32 replicationCount)
        {
            size_t replLen = c_string_length(cString);
            size_t resultLen = replLen * replicationCount;
            assert(replLen > 0 && replicationCount > 0);
            char *resultMemory = alloc_string(resultLen + 1);

            AsciiString result(resultMemory, resultLen, true);

            for (azgra::i32 repl = 0; repl < replicationCount; repl++)
            {
                for (size_t charIndex = 0; charIndex < replLen; charIndex++)
                {
                    result[(repl * replLen) + charIndex] = cString[charIndex];
                }
            }
            result[resultLen] = '\0';
            return result;
        }

        std::vector<std::shared_ptr<AsciiString>> AsciiString::split(const char &separator) const
        {
            std::vector<std::shared_ptr<AsciiString>> result;
            if (count(separator) <= 0)
                return result;

            size_t from = 0;
            azgra::i32 match = index_of(separator, from);

            while (match != -1)
            {
                std::shared_ptr<AsciiString> s(new AsciiString(substring(from, match - from)));

                result.push_back(s);
                from = match + 1;
                match = index_of(separator, from);
            }

            if (_length - from > 0)
            {
                result.push_back(std::shared_ptr<AsciiString>(new AsciiString(substring(from))));
            }

            return result;
        }

        void AsciiString::pad_left(const char padChar, const size_t desiredLength)
        {
            if ((_length <= desiredLength) || (_length == desiredLength))
                return;

            char *result = alloc_string(desiredLength + 1);
            int fillSize = desiredLength - _length;
            for (int i = 0; i < fillSize; ++i)
            {
                result[i] = padChar;
            }
            for (size_t j = 0; j < _length; ++j)
            {
                result[fillSize + j] = _string[j];
            }
            result[desiredLength] = '\0';

            free_string(_string);
            _string = result;
            _length = desiredLength;
        }

        void AsciiString::pad_right(const char padChar, const size_t desiredLength)
        {
            if ((_length > desiredLength) || (_length == desiredLength))
                return;

            _string = realloc_string(desiredLength + 1, _string, _length);
            int fillSize = desiredLength - _length;
            for (int i = 0; i < fillSize; ++i)
            {
                _string[_length + i] = padChar;
            }
            _length = desiredLength;
            _string[desiredLength] = '\0';
        }

        void AsciiString::multi_append(const std::vector<const char *> &strings)
        {
            size_t finalLen = _length;
            std::vector<size_t> lengths;
            for (const char *string : strings)
            {
                size_t len = c_string_length(string);
                lengths.push_back(len);
                finalLen += len;
            }

            if (finalLen > 0)
            {
                // If we are appending to existing string we reallocate, copying the old string into new memory.
                {
                    char *newMemory = (_length > 0) ? realloc_string(finalLen + 1, _string, _length) : alloc_string(finalLen + 1);
                    _string = newMemory;
                }

                size_t offset = _length;
                for (size_t stringIndex = 0; stringIndex < strings.size(); ++stringIndex)
                {
                    memcpy(_string + offset, strings[stringIndex], lengths[stringIndex]);
                    offset += lengths[stringIndex];
                }

                _string[finalLen] = '\0';
                _length = finalLen;
            }
        }

        void AsciiString::fill_left(const char fillChar, const size_t fillCount)
        {
            size_t finalLen = _length + fillCount;
            char *newString = alloc_string(finalLen + 1);
            for (size_t i = 0; i < fillCount; ++i)
            {
                newString[i] = fillChar;
            }
            memcpy(newString + (fillCount * sizeof(char)), _string, _length);
            newString[finalLen] = '\0';
            free_string(_string);
            _string = newString;
            _length = finalLen;
        }

        void AsciiString::fill_right(const char fillChar, const size_t fillCount)
        {
            size_t finalLen = _length + fillCount;
            _string = realloc_string(finalLen + 1, _string, _length);
            for (size_t i = 0; i < fillCount; ++i)
            {
                _string[_length + i] = fillChar;
            }
            _string[finalLen] = '\0';
            _length = finalLen;
        }

        void AsciiString::operator+=(const std::vector<const char *> &strings)
        {
            multi_append(strings);
        }

        SmartStringView<char> AsciiString::get_ssw() const noexcept
        {
            SmartStringView<char> result(basic_string_view__<char>(_string, _length));
            return result;
        }

        AsciiString::AsciiString(const char *cString, const size_t len)
        {
            assert(len <= c_string_length(cString));
            internal_initalize(cString, len);
        }




    }; // namespace string
}; // namespace azgra
