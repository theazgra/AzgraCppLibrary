#pragma once

//#ifndef GCC_CXX17
//#define GCC_CXX17
//#endif

#ifdef GCC_CXX17

#include <string_view>

template<typename CharType>
using basic_string_view__ = std::basic_string_view<CharType>;
#else
#include <experimental/string_view>
template<typename CharType>
using basic_string_view__ = std::experimental::basic_string_view<CharType>;
#endif


#include <vector>

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
            basic_string_view__<CharType> sw;
        public:
            SmartStringView() = default;

            explicit SmartStringView(const CharType *cString) : SmartStringView(basic_string_view__<CharType>(cString))
            {}

            explicit SmartStringView(const basic_string_view__<CharType> stringView)
            {
                sw = stringView;
            }

            ~SmartStringView() = default;

            constexpr basic_string_view__<CharType> string_view() const noexcept
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

            constexpr bool contains(const basic_string_view__<CharType> &otherSw) const noexcept
            {
                return (sw.find(otherSw) != sw.npos);
            }

            constexpr int index_of(const CharType c, const size_t fromIndex) const noexcept
            {
                auto index = sw.find(c, fromIndex);
                return static_cast<int>(index != sw.npos ? index : -1);
            }

            constexpr int index_of(const basic_string_view__<CharType> &otherSw, const size_t fromIndex = 0) const noexcept
            {
                auto index = sw.find(otherSw, fromIndex);
                return static_cast<int>(index != sw.npos ? index : -1);
            }

            constexpr int last_index_of(const basic_string_view__<CharType> &otherSw) const noexcept
            {
                auto index = rfind(otherSw);
                return static_cast<int>(index != sw.npos ? index : -1);
            }

            constexpr bool starts_with(const basic_string_view__<CharType> &otherSw) const noexcept
            {
                return (index_of(otherSw) == 0);
            }

            constexpr bool starts_with(const CharType testChar) const noexcept
            {
                return (sw[0] == testChar);
            }

            constexpr bool ends_with(const basic_string_view__<CharType> &otherSw) const noexcept
            {
                return (index_of(otherSw) == (sw.length() - otherSw.length()));
            }

            constexpr bool ends_with(const CharType testChar) const noexcept
            {
                if (sw.length() == 0)
                    return false;
                return (sw[sw.length() - 1] == testChar);
            }

            constexpr int count(const basic_string_view__<CharType> &otherSw) const noexcept
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

            constexpr bool operator==(const SmartStringView<CharType> &otherSw) const noexcept
            {
                return (sw == otherSw.sw);
            }

            constexpr bool operator==(const basic_string_view__<CharType> &otherSw) const noexcept
            {
                return (sw == otherSw);
            }

            constexpr bool equals(const basic_string_view__<CharType> &otherSw) const noexcept
            {
                return (sw == otherSw);
            }

            constexpr SmartStringView substring(size_t fromIndex) const
            {
                SmartStringView result(basic_string_view__<CharType>(sw.data() + fromIndex));
                return result;
            }

            constexpr SmartStringView substring(size_t fromIndex, size_t len) const
            {
                SmartStringView result(basic_string_view__<CharType>(sw.data() + fromIndex, len));
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
                                    basic_string_view__<CharType>(sw.data() + searchFrom, index - searchFrom)));

                    searchFrom = index + 1;
                    index = index_of(separatorChar, searchFrom);
                }
                result.push_back(
                        SmartStringView(basic_string_view__<CharType>(sw.data() + searchFrom)));

                return result;
            }

            constexpr std::vector<SmartStringView<CharType>>
            split(const basic_string_view__<CharType> &separatorString) const noexcept
            {
                std::vector<SmartStringView<CharType>> result;
                size_t separatorLen = separatorString.length();

                size_t searchFrom = 0;
                int index = index_of(separatorString, searchFrom);

                while (index != -1)
                {
                    result.push_back(
                            SmartStringView(
                                    basic_string_view__<CharType>(sw.data() + searchFrom, index - searchFrom)));

                    searchFrom = index + separatorLen;
                    index = index_of(separatorString, searchFrom);
                }
                if (sw.length() != searchFrom)
                {
                    result.push_back(SmartStringView(basic_string_view__<CharType>(sw.data() + searchFrom)));
                }

                return result;
            }

        };
    };
};
