#pragma once

#include <azgra/azgra.h>
#include <algorithm>

namespace azgra
{
    namespace string
    {
        /*
         * This is wrapper for string_view, which introduces helper methods,
         * which are not yet present in C++17
         */
        template<typename CharType>
        class SmartStringView
        {
        private:
            BasicStringView <CharType> sw;
        public:
            SmartStringView() = default;

            SmartStringView(const CharType *cString) : SmartStringView(
                    BasicStringView<CharType>(cString)) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            {}

            SmartStringView(
                    const BasicStringView <CharType> stringView) // NOLINT(google-explicit-constructor,hicpp-explicit-conversions)
            {
                sw = stringView;
            }

            ~SmartStringView() = default;

            constexpr BasicStringView <CharType> string_view() const noexcept
            {
                return sw;
            }

            constexpr CharType operator[](int index) const
            {
                return sw[index];
            }

            constexpr auto data() const noexcept
            {
                return sw.data();
            }

            constexpr auto begin() const noexcept
            {
                return sw.begin();
            }

            constexpr auto end() const noexcept
            {
                return sw.end();
            }

            constexpr size_t length() const noexcept
            {
                return sw.length();
            }

            constexpr bool contains(const BasicStringView <CharType> &otherSw) const noexcept
            {
                return (sw.find(otherSw) != sw.npos);
            }

            constexpr int index_of(const CharType c, const size_t fromIndex) const noexcept
            {
                auto index = sw.find(c, fromIndex);
                return static_cast<int>(index != sw.npos ? index : -1);
            }

            constexpr int index_of_first(const std::vector<CharType> &chars, const size_t fromIndex) const noexcept
            {
                bool found = false;
                size_t index = length();
                for (const char &c : chars)
                {
                    size_t cIndex = sw.find(c, fromIndex);
                    if (cIndex < index)
                    {
                        index = cIndex;
                        found = true;
                    }
                }
                return static_cast<int>(found ? index : -1);
            }

            constexpr int index_of(const BasicStringView <CharType> &otherSw, const size_t fromIndex = 0) const noexcept
            {
                auto index = sw.find(otherSw, fromIndex);
                return static_cast<int>(index != sw.npos ? index : -1);
            }

            constexpr int last_index_of(const BasicStringView <CharType> &otherSw) const noexcept
            {
                auto index = rfind(otherSw);
                return static_cast<int>(index != sw.npos ? index : -1);
            }

            constexpr bool starts_with(const BasicStringView <CharType> &otherSw) const noexcept
            {
                return (index_of(otherSw) == 0);
            }

            constexpr bool starts_with(const CharType testChar) const noexcept
            {
                return (sw[0] == testChar);
            }

            constexpr bool ends_with(const BasicStringView <CharType> &otherSw) const noexcept
            {
                return (index_of(otherSw) == (sw.length() - otherSw.length()));
            }

            constexpr bool ends_with(const CharType testChar) const noexcept
            {
                if (sw.length() == 0)
                    return false;
                return (sw[sw.length() - 1] == testChar);
            }

            constexpr int count(const BasicStringView <CharType> &otherSw) const noexcept
            {
                int result = 0;

                size_t searchLen = otherSw.length();
                size_t searchFrom = 0;
                int lastIndex = index_of(otherSw, searchFrom);

                while (lastIndex != -1)
                {
                    ++result;
                    searchFrom = lastIndex + searchLen;
                    lastIndex = index_of(otherSw, searchFrom);
                }

                return result;
            }

            constexpr bool is_empty() const noexcept
            {
                return !(sw.length() > 0);
            }

            constexpr bool is_number() const noexcept
            {
                return (sw.length() > 0) && (std::find_if(sw.begin(), sw.end(), [](const char c)
                {
                    return !std::isdigit(c);
                }) == sw.end());
            }


            constexpr bool operator==(const char c) const noexcept
            {
                return (sw.length() == 1 && sw[0] == c);
            }

            constexpr bool operator==(const SmartStringView<CharType> &otherSw) const noexcept
            {
                return (sw == otherSw.sw);
            }

            constexpr bool operator==(const BasicStringView <CharType> &otherSw) const noexcept
            {
                return (sw == otherSw);
            }

            constexpr bool equals(const BasicStringView <CharType> &otherSw) const noexcept
            {
                return (sw == otherSw);
            }

            constexpr SmartStringView substring(size_t fromIndex) const
            {
                SmartStringView result(BasicStringView<CharType>(sw.data() + fromIndex));
                return result;
            }

            constexpr SmartStringView substring(size_t fromIndex, size_t len) const
            {
                SmartStringView result(BasicStringView<CharType>(sw.data() + fromIndex, len));
                return result;
            }

            constexpr std::vector<SmartStringView<CharType>> multi_split(const std::vector<CharType> &separatorChars) const noexcept
            {
                if (separatorChars.size() == 1)
                {
                    return split(separatorChars[0]);
                }
                std::vector<SmartStringView<CharType>> result;

                size_t searchFrom = 0;
                int index = index_of_first(separatorChars, searchFrom);
                while (index != -1)
                {
                    result.push_back(
                            SmartStringView(
                                    BasicStringView<CharType>(sw.data() + searchFrom, index - searchFrom)));

                    searchFrom = index + 1;
                    index = index_of_first(separatorChars, searchFrom);
                }
                result.push_back(
                        SmartStringView(BasicStringView<CharType>(sw.data() + searchFrom)));

                return result;
            }

            constexpr std::vector<SmartStringView<CharType>> split(const CharType &separatorChar) const noexcept
            {
                std::vector<SmartStringView<CharType>> result;

                size_t searchFrom = 0;
                int index = index_of(separatorChar, searchFrom);
                while (index != -1)
                {
                    result.push_back(
                            SmartStringView(
                                    BasicStringView<CharType>(sw.data() + searchFrom, index - searchFrom)));

                    searchFrom = index + 1;
                    index = index_of(separatorChar, searchFrom);
                }
                result.push_back(
                        SmartStringView(BasicStringView<CharType>(sw.data() + searchFrom)));

                return result;
            }

            constexpr std::vector<SmartStringView<CharType>>
            split(const BasicStringView <CharType> &separatorString) const noexcept
            {
                std::vector<SmartStringView<CharType>> result;
                size_t separatorLen = separatorString.length();

                size_t searchFrom = 0;
                int index = index_of(separatorString, searchFrom);

                while (index != -1)
                {
                    result.push_back(
                            SmartStringView(
                                    BasicStringView<CharType>(sw.data() + searchFrom, index - searchFrom)));

                    searchFrom = index + separatorLen;
                    index = index_of(separatorString, searchFrom);
                }
                if (sw.length() != searchFrom)
                {
                    result.push_back(SmartStringView(BasicStringView<CharType>(sw.data() + searchFrom)));
                }

                return result;
            }

        };
    };
};
