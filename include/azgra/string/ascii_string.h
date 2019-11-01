#pragma once

#include <cassert>
#include <memory>
#include <azgra/azgra.h>
#include <azgra/string/smart_string_view.h>

namespace azgra
{
    namespace string
    {
#define CHAR_AS_STRING(c) {c,'\0'}

        // NOTE: (We can probably keep bigger memory for StringBuilder like append utilizing less malloc calls.)
        //       This idea is left-out because it would increase complexity, while one can use multi_append.
        constexpr char AsciiCaseOffset = 'a' - 'A';

        class AsciiString
        {
        private:
            /// Flag indicating the empty string.
            bool m_isEmpty = true;

            /// Internal string memory.
            std::vector<char> m_string;

            /// Length of the string. Doesn't equal to allocated size of _string, which contains 0 at the end.
            size_t m_length = 0;

            /**
             * @brief Get size of c string.
             *
             * @param cString String to get size of.
             * @return size_t Size of cString
             */

            /// Get size of `C like` string.
            /// \param cString String memory.
            /// \return Number of characters in the string memory.
            static size_t c_string_length(const char *cString);

            /// Internal concatenation function.
            /// \param string String to add to current string.
            /// \param length Length of concatenated string.
            void internal_concat(const char *string, const size_t length);

            /// Initialize this instance string memory and set flags.
            /// \param string Base string from which this instance will be created.
            void internal_initalize(const char *string);

            /// Initialize this instance string with given length memory and set flags.
            /// \param string Base string from which this instance will be created.
            /// \param length Length of the string
            void internal_initalize(const char *string, const size_t length);

            /// Construct new AsciiString from vector.
            /// \param string Allocated string.
            explicit AsciiString(std::vector<char> &string);

        public:
            /// Default empty string constructor.
            AsciiString();

            /// Create instance from C string
            /// \param cString C string to copy from.
            explicit AsciiString(const char *cString);

            explicit AsciiString(const char *cString, const size_t len);

            /// Create instance by concatenating multiple C strings.
            /// \param strings C strings to concatenate to a single instance.
            explicit AsciiString(const std::vector<const char *> &strings);

            /// Free string memory.
            ~AsciiString();

            /// Get pointer to string, utilizing C like work with the string.
            /// \return Pointer to memory of this string.
            const char *get_c_string() const;

            /// Append another string to this instance.
            /// \param cString String to append.
            void concat(const char *cString);

            /// Get length of this string (number of characters).
            /// \return Number of characters in this string.
            size_t length() const;

            /// Get index of the first match.
            /// \param c Character to find.
            /// \param fromIndex Index from which the search will begin.
            /// \return Index of first match or -1 if character is not found.
            azgra::i32 index_of(const char &c, const size_t fromIndex = 0) const;

            /// Get index of first matched character in string.
            /// \param string String to find.
            /// \param fromIndex Index from which search will begin.
            /// \return Index of first matched character or -1 if string is not found.
            azgra::i32 index_of(const char *string, const size_t fromIndex = 0) const;

            /// Get index of the last match.
            /// \param c Character to find.
            /// \param fromIndex Index from which the search will begin.
            /// \return Index of last match or -1 if character is not found.
            azgra::i32 last_index_of(const char &c, const size_t fromIndex = 0) const;

            /// Get index of the last match.
            /// \param c Character to find.
            /// \param fromIndex Index from which search will begin.
            /// \param toIndex Index to which search will continue.
            /// \return Index of last matched character or -1 if string is not found.
            azgra::i32 last_index_of(const char &c, const size_t fromIndex, const size_t toIndex) const;

            /// Get index of first matched character in last matched string.
            /// \param string String to find.
            /// \param fromIndex Index from which search will begin.
            /// \return Index of first matched character in last found string or -1 if string is not found.
            azgra::i32 last_index_of(const char *string, const size_t fromIndex = 0) const;

            /// Check if character is found in this string.
            /// \param c Character to find.
            /// \return True if character is found.
            bool contains(const char &c) const;

            /// Check if string is found in this string.
            /// \param string String to find.
            /// \return True if string is found.
            bool contains(const char *string) const;

            /// Get number of occurrences of given character in this string.
            /// \param c Character to find.
            /// \return Occurrence count of character in this string.
            size_t count(const char &c) const;

            /// Get number of occurrences of given string in this string.
            /// \param string String to find.
            /// \return Occurrence count of string in this string.
            size_t count(const char *string) const;

            /// Check if this string starts with given character.
            /// \param c Character to test.
            /// \return True if string begins with given character.
            bool starts_with(const char &c) const;

            /// Check if this string starts with given string.
            /// \param string String to test.
            /// \return True if string begins with given string.
            bool starts_with(const char *string) const;

            /// Check if this string ends with given character.
            /// \param c Character to test.
            /// \return True if string ends with given character.
            bool ends_with(const char &c) const;

            /// Check if this string ends with given string.
            /// \param string String to test.
            /// \return True if string ends with given string.
            bool ends_with(const char *string) const;

            /// Equality test.
            /// \param other AsciiString to test.
            /// \return True if two azgra::string::AsciiString strings are equal.
            bool equals(const AsciiString &other) const;

            /// Equality test for azgra::string::AsciiString and C like string.
            /// \param string String to check.
            /// \return True if content of this string and C like string memory are equal.
            bool equals(const char *string) const;

            /// Transform all characters to upper-case.
            void to_upper();

            /// Transform all characters to lower-case.
            void to_lower();

            /// Replaces all occurrences of oldChar with newChar.
            /// \param oldChar Character to replace.
            /// \param newChar New character.
            void replace(const char &oldChar, const char &newChar);

            /// Replaces all occurrences of oldString with newString.
            /// \param oldString String to replace.
            /// \param newString New string character.
            void replace(const char *oldString, const char *newString);

            /// Remove all occurrences of character.
            /// \param c Character to remove.
            void remove(const char &c);

            /// Remove all occurrences of stringToRemove.
            /// \param stringToRemove String to remove.
            void remove(const char *stringToRemove);

            /// Create substring from given index to the end of this string.
            /// \param fromIndex Index from which to create substring.
            /// \return Substring from given index.
            AsciiString substring(const size_t fromIndex) const;

            /// Create substring from given index with length.
            /// \param fromIndex Index from which to create substring
            /// \param length Length of the substring.
            /// \return Substring from given index, with given length.
            AsciiString substring(const size_t fromIndex, const size_t length) const;

            /// Split this string into multiple strings separated be separator.
            /// \param separator Character separator.
            /// \return Collection of separated string.
            std::vector<std::shared_ptr<AsciiString>> split(const char &separator) const;

            /// Implicit conversion to C like string.
            /// \return Pointer to C like string memory.
            operator const char *() const;

            /// Character indexer operator.
            /// \param index Index of the character in string.
            /// \return Reference to the character.
            char &operator[](const azgra::i32 &index);

            /// Constant character indexer operator.
            /// \param index Index of the character in string.
            /// \return Constant reference to the character.
            char const &operator[](const azgra::i32 &index) const;

            /// Character indexer.
            /// \param index Index of the character in string.
            /// \return Reference to the character.
            char &at(const azgra::i32 &index);

            /// Constant character indexer.
            /// \param index Index of the character in string.
            /// \return Reference to the character.
            char const &at(const azgra::i32 &index) const;

            /// Initialize this string with C like string.
            /// \param cString
            void operator=(const char *cString);

            /// Equality operator. See `azgra::string::AsciiString::equals`.
            /// \param cString C like string to test.
            /// \return True if strings are equal.
            bool operator==(const char *cString) const;

            /// Equality operator. See `azgra::string::AsciiString::equals`.
            /// \param string AsciiString to test.
            /// \return True if strings are equal.
            bool operator==(const AsciiString &string) const;

            /// Concatenate operator.
            /// \param cString String to concatenate.
            void operator+=(const char *cString);

            /// Multi concatenate operator.
            /// \param strings Strings to concatenate.
            void operator+=(const std::vector<const char *> &strings);

            /// Concatenate operator for character.
            /// \param c Character to concatenate.
            void operator+=(const char c);

            /// Plus operator.
            /// \param cString String to concatenate.
            /// \return New string, result of string concatenation.
            AsciiString operator+(const char *cString) const;

            /// Plus operator for character.
            /// \param c Character to concatenate.
            /// \return New string, result of string concatenation.
            AsciiString operator+(const char c) const;

            /// Multi concatenate.
            /// \param strings Strings to concatenate.
            void multi_append(const std::vector<const char *> &strings);

             /// Replicate the string replicationCount times.
             /// \param cString String to replicate.
             /// \param replicationCount Number of replications.
             /// \return Result of replication.
            static AsciiString replicate(const char *cString, const azgra::i32 replicationCount);

            /// Pad this string from the left to the desired length.
            /// \param padChar Character used for padding.
            /// \param desiredLength Desired length.
            void pad_left(const char padChar, const size_t desiredLength);

            /// Pad this string from the right to the desired length.
            /// \param padChar Character used for padding.
            /// \param desiredLength Desired length.
            void pad_right(const char padChar, const size_t desiredLength);

            /// Fill this string from the left with given number of characters.
            /// \param fillChar Character used for filling.
            /// \param fillCount Number of fill characters.
            void fill_left(const char fillChar, const size_t fillCount);

            /// Fill this string from the right with given number of characters.
            /// \param fillChar Character used for filling.
            /// \param fillCount Number of fill characters.
            void fill_right(const char fillChar, const size_t fillCount);

            /// Get SmartStringView wrapper.
            /// \return SmartStringView wrapper.
            SmartStringView<char> get_ssw() const noexcept;
        };
    }; // namespace string
} // namespace azgra
