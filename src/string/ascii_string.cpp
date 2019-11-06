#include <azgra/string/ascii_string.h>

#include <memory>

namespace azgra
{
    namespace string
    {
        //TODO: Add asserts.
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
            m_isEmpty = (inputStringLen == 0);
            m_length = inputStringLen;
            m_string.resize(m_length + 1);

            for (size_t i = 0; i < inputStringLen; i++)
                m_string[i] = string[i];

            m_string[m_length] = '\0';
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

        AsciiString::AsciiString(std::vector<char> &string)
        {
            m_string = std::move(string);
            m_length = m_string.size() - 1;
            m_isEmpty = (m_length == 0);
        }

        AsciiString::AsciiString(const std::vector<const char *> &strings)
        {
            m_length = 0;
            multi_append(strings);
            m_isEmpty = (m_length <= 0);
        }

        AsciiString::AsciiString()
        {
            m_length = 0;
            m_isEmpty = true;
        }

        AsciiString::operator const char *() const
        {
            return m_string.data();
        }

        char &AsciiString::operator[](const azgra::i32 &index)
        {
            return m_string[index];
        }

        char const &AsciiString::operator[](const azgra::i32 &index) const
        {
            return m_string[index];
        }

        char &AsciiString::at(const azgra::i32 &index)
        {
            return m_string[index];
        }

        char const &AsciiString::at(const azgra::i32 &index) const
        {
            return m_string[index];
        }

        void AsciiString::operator=(const char *cString)
        {
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
            std::vector<char> copy = std::vector<char>(m_string.begin(), m_string.end());
            AsciiString result(copy);
            result.concat(cString);
            return result;
        }

        AsciiString AsciiString::operator+(const char c) const
        {
            std::vector<char> copy = std::vector<char>(m_string.begin(), m_string.end());
            AsciiString result(copy);
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
            m_string.resize(0);
            m_length = 0;
        }

        void AsciiString::internal_concat(const char *string, const size_t length)
        {
            if (length == 0)
                return;

            size_t newLength = m_length + length + 1;
            m_string.resize(newLength);

            for (size_t i = m_length; i < newLength - 1; i++)
                m_string[i] = string[i - m_length];

            m_string[newLength - 1] = '\0';

            m_length = newLength - 1;
        }

        void AsciiString::concat(const char *cString)
        {
            size_t cStringLen = c_string_length(cString);
            internal_concat(cString, cStringLen);
        }

        const char *AsciiString::get_c_string() const
        {
            return m_string.data();
        }

        size_t AsciiString::length() const
        {
            return m_length;
        }

        azgra::i32 AsciiString::index_of(const char &c, const size_t fromIndex) const
        {
            if (fromIndex >= m_length)
                return -1;

            for (size_t i = fromIndex; i < m_length; i++)
            {
                if (m_string[i] == c)
                {
                    return i;
                }
            }
            return -1;
        }

        azgra::i32 AsciiString::index_of(const char *string, const size_t fromIndex) const
        {
            size_t matchLen = c_string_length(string);
            if (matchLen > m_length || fromIndex >= m_length)
            {
                return -1;
            }

            size_t searchFrom = fromIndex;
            azgra::i32 indexOfFirstChar = index_of(string[0], searchFrom);
            bool found = false;

            while (indexOfFirstChar != -1)
            {
                if (indexOfFirstChar + (matchLen - 1) > m_length)
                    return -1;

                found = true;
                for (size_t matchIndex = 1; matchIndex < matchLen; matchIndex++)
                {
                    if (m_string[indexOfFirstChar + matchIndex] != string[matchIndex])
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
            return last_index_of(c, fromIndex, m_length - 1);
        }

        azgra::i32 AsciiString::last_index_of(const char &c, const size_t fromIndex, const size_t toIndex) const
        {
            if (fromIndex >= m_length)
                return -1;

            for (auto i = (azgra::i32) toIndex; i >= (azgra::i32) fromIndex; i--)
            {
                if (m_string[i] == c)
                {
                    return i;
                }
            }
            return -1;
        }

        azgra::i32 AsciiString::last_index_of(const char *string, const size_t fromIndex) const
        {
            size_t matchLen = c_string_length(string);
            if (matchLen > m_length || fromIndex >= m_length)
            {
                return -1;
            }

            size_t searchUpperBound = m_length - 1;
            azgra::i32 indexOfLastChar = last_index_of(string[matchLen - 1], 0, searchUpperBound);
            bool found = false;

            while (indexOfLastChar != -1)
            {
                if (indexOfLastChar - (matchLen - 1) < fromIndex)
                    return -1;

                found = true;
                for (size_t matchIndex = 1; matchIndex < matchLen; matchIndex++)
                {
                    if (m_string[indexOfLastChar - matchIndex] != string[matchLen - matchIndex - 1])
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
            if (matchLen > m_length)
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
            return (last_index_of(c) == (azgra::i64) (m_length - 1));
        }

        bool AsciiString::ends_with(const char *string) const
        {
            return (last_index_of(string) == (azgra::i64) (m_length - c_string_length(string)));
        }

        bool AsciiString::equals(const AsciiString &other) const
        {
            if (m_string.size() != other.m_string.size())
                return false;
            for (size_t i = 0; i < m_length; ++i)
            {
                if (m_string[i] != other.m_string[i])
                    return false;
            }
            return true;
        }

        bool AsciiString::equals(const char *string) const
        {
            if (m_length != c_string_length(string))
                return false;

            for (size_t i = 0; i < m_length; i++)
            {
                if (m_string[i] != string[i])
                    return false;
            }
            return true;
        }

        void AsciiString::to_upper()
        {
            for (size_t i = 0; i < m_length; i++)
            {
                char c = m_string[i];
                if (c >= 'a' && c <= 'z')
                    m_string[i] -= AsciiCaseOffset;
            }
        }

        void AsciiString::to_lower()
        {
            for (size_t i = 0; i < m_length; i++)
            {
                char c = m_string[i];
                if (c >= 'A' && c <= 'Z')
                    m_string[i] += AsciiCaseOffset;
            }
        }

        void AsciiString::replace(const char &oldChar, const char &newChar)
        {
            for (size_t i = 0; i < m_length; i++)
            {
                if (m_string[i] == oldChar)
                    m_string[i] = newChar;
            }
        }

        void AsciiString::replace(const char *oldString, const char *newString)
        {
            size_t matchLen = c_string_length(oldString);
            size_t matchCount = count(oldString);
            if (matchCount == 0 || matchLen == 0)
                return;

            size_t replaceStringLen = c_string_length(newString);
            size_t newLength = m_length - (matchCount * matchLen) + (matchCount * replaceStringLen);
            std::vector<char> newStringMemory(newLength);

            size_t searchFrom = 0;
            size_t newStringIndex = 0;
            azgra::i32 index = index_of(oldString, searchFrom);

            while (index != -1)
            {
                // Copy all in front of match.
                for (azgra::i32 i = searchFrom; i < index; i++)
                    newStringMemory[newStringIndex++] = m_string[i];

                // Copy replace string into new string.
                for (size_t i = 0; i < replaceStringLen; i++)
                {
                    newStringMemory[newStringIndex++] = newString[i];
                }

                searchFrom = index + matchLen;
                index = index_of(oldString, searchFrom);
            }

            // Copy remaining old content.
            for (size_t i = searchFrom; i < m_length; i++)
                newStringMemory[newStringIndex++] = m_string[i];

            m_string = newStringMemory;
            m_length = newLength;
        }

        void AsciiString::remove(const char &c)
        {
            size_t removeCount = count(c);
            if (removeCount == 0)
                return;

            size_t newLength = m_length - removeCount;
            std::vector<char> newString(newLength);
            size_t index = 0;
            for (size_t i = 0; i < m_length; i++)
            {
                if (m_string[i] != c)
                {
                    newString[index++] = m_string[i];
                }
            }

            m_length = newLength;
            m_string = newString;
        }

        void AsciiString::remove(const char *string)
        {
            size_t matchLen = c_string_length(string);
            if (matchLen == 0)
                return;

            size_t matchCount = count(string);
            size_t newLen = m_length - (matchCount * matchLen);

            std::vector<char> newString(newLen);

            size_t searchFrom = 0;
            size_t newStringIndex = 0;
            azgra::i32 index = index_of(string, searchFrom);

            while (index != -1)
            {
                // Copy all in front of match.
                for (azgra::i32 i = searchFrom; i < index; i++)
                    newString[newStringIndex++] = m_string[i];

                searchFrom = index + matchLen;
                index = index_of(string, searchFrom);
            }

            // Copy remaining old content.
            for (size_t i = searchFrom; i < m_length; i++)
                newString[newStringIndex++] = m_string[i];

            m_string = newString;
            m_length = newLen;
        }

        AsciiString AsciiString::substring(const size_t fromIndex) const
        {
            return substring(fromIndex, m_length - fromIndex);
        }

        AsciiString AsciiString::substring(const size_t fromIndex, const size_t length) const
        {
            assert(fromIndex < m_length && (length - fromIndex) <= m_length);

            std::vector<char> subsMemory(length + 1);
            AsciiString result(subsMemory);

            for (size_t i = fromIndex; i < fromIndex + length; i++)
            {
                result[i - fromIndex] = m_string[i];
            }
            result[length] = '\0';

            return result;
        }

        AsciiString AsciiString::replicate(const char *cString, const azgra::i32 replicationCount)
        {
            size_t replLen = c_string_length(cString);
            size_t resultLen = replLen * replicationCount;
            assert(replLen > 0 && replicationCount > 0);

            std::vector<char> resultMemory(resultLen + 1);
            AsciiString result(resultMemory);

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

            if (m_length - from > 0)
            {
                result.push_back(std::make_shared<AsciiString>(substring(from)));
            }

            return result;
        }

        void AsciiString::pad_left(const char padChar, const size_t desiredLength)
        {
            if ((m_length <= desiredLength) || (m_length == desiredLength))
                return;

            std::vector<char> result(desiredLength + 1);
            int fillSize = desiredLength - m_length;
            for (int i = 0; i < fillSize; ++i)
            {
                result[i] = padChar;
            }
            for (size_t j = 0; j < m_length; ++j)
            {
                result[fillSize + j] = m_string[j];
            }
            result[desiredLength] = '\0';

            m_string = result;
            m_length = desiredLength;
        }

        void AsciiString::pad_right(const char padChar, const size_t desiredLength)
        {
            if ((m_length > desiredLength) || (m_length == desiredLength))
                return;

            m_string.resize(desiredLength + 1);
            int fillSize = desiredLength - m_length;
            for (int i = 0; i < fillSize; ++i)
            {
                m_string[m_length + i] = padChar;
            }
            m_length = desiredLength;
            m_string[desiredLength] = '\0';
        }

        void AsciiString::multi_append(const std::vector<const char *> &strings)
        {
            size_t finalLen = m_length;
            std::vector<size_t> lengths;
            for (const char *string : strings)
            {
                size_t len = c_string_length(string);
                lengths.push_back(len);
                finalLen += len;
            }

            if (finalLen > 0)
            {
                m_string.resize(finalLen + 1);

                size_t offset = m_length;
                for (size_t stringIndex = 0; stringIndex < strings.size(); ++stringIndex)
                {
                    memcpy(m_string.data() + offset, strings[stringIndex], lengths[stringIndex]);
                    offset += lengths[stringIndex];
                }

                m_string[finalLen] = '\0';
                m_length = finalLen;
            }
        }

        void AsciiString::fill_left(const char fillChar, const size_t fillCount)
        {
            size_t finalLen = m_length + fillCount;
            std::vector<char> newString(finalLen + 1);
            for (size_t i = 0; i < fillCount; ++i)
            {
                newString[i] = fillChar;
            }
            memcpy(newString.data() + (fillCount * sizeof(char)), m_string.data(), m_length);
            newString[finalLen] = '\0';
            m_string = newString;
            m_length = finalLen;
        }

        void AsciiString::fill_right(const char fillChar, const size_t fillCount)
        {
            size_t finalLen = m_length + fillCount;
            m_string.resize(finalLen + 1);
            for (size_t i = 0; i < fillCount; ++i)
            {
                m_string[m_length + i] = fillChar;
            }
            m_string[finalLen] = '\0';
            m_length = finalLen;
        }

        void AsciiString::operator+=(const std::vector<const char *> &strings)
        {
            multi_append(strings);
        }

        SmartStringView<char> AsciiString::get_ssw() const noexcept
        {
            SmartStringView<char> result(basic_string_view__<char>(m_string.data(), m_length));
            return result;
        }

        AsciiString::AsciiString(const char *cString, const size_t len)
        {
            assert(len <= c_string_length(cString));
            internal_initalize(cString, len);
        }


    }; // namespace string
}; // namespace azgra
