#pragma once

#include <sstream>
#include <azgra/string/smart_string_view.h>
#include <azgra/collection/vector_linq.h>

namespace azgra
{
    namespace cli
    {
        enum CliGroupMatchPolicy
        {
            CliGroupMatchPolicy_All,
            CliGroupMatchPolicy_AtLeastOne,
            CliGroupMatchPolicy_AtLeastOneIfGroupIsRequired,
            CliGroupMatchPolicy_NoPolicy
        };

        class CliOption
        {
            friend class CliArguments;

        protected:
            char matchCharacter;
            string::SmartStringView<char> matchString;
            bool isMatched = false;
            bool isRequired = false;
            bool hasMatchCharacter = true;
            CliOption *group = nullptr;
            string::SmartStringView<char> name;
            string::SmartStringView<char> description;

            CliOption(const string::SmartStringView<char> &name, const string::SmartStringView<char> &description,
                      char matchCharacter, const string::SmartStringView<char> &matchString, bool required = false)
            {
                this->name = name;
                this->description = description;
                this->matchCharacter = matchCharacter;
                this->matchString = matchString;
                isRequired = required;
            }

            void mark_as_matched()
            {
                isMatched = true;
                if (group)
                {
                    group->isMatched = true;
                }
            }

        public:
            virtual ~CliOption()
            = default;;

            bool is_matched() const noexcept
            {
                return isMatched;
            }

            operator bool() const noexcept
            {
                return isMatched;
            }

            bool is_required() const noexcept
            {
                return isRequired;
            }

            bool is_grouped() const noexcept
            {
                return (group != nullptr);
            }
        };

        class CliMethod : public CliOption
        {
        private:
            std::vector<CliOption *> requiredFlags;
            std::vector<CliOption *> optionalFlags;
        public:
            CliMethod(string::SmartStringView<char> methodMatchName, string::SmartStringView<char> description,
                      const std::vector<CliOption *> &requiredFlags = std::vector<CliOption *>(),
                      const std::vector<CliOption *> &optionalFlags = std::vector<CliOption *>()) :
                    CliOption(methodMatchName, description, '\0', methodMatchName, false)
            {
                this->requiredFlags = requiredFlags;
                this->optionalFlags = optionalFlags;
                hasMatchCharacter = false;
            }

            std::vector<CliOption *> const &get_required_flags() const
            {
                return requiredFlags;
            }

            std::vector<CliOption *> const &get_optional_flags() const
            {
                return optionalFlags;
            }

        };


        class CliFlag : public CliOption
        {
        public:
            CliFlag(const string::SmartStringView<char> &flagName, const string::SmartStringView<char> &description, char matchChar,
                    const string::SmartStringView<char> &matchString, bool required = false) :
                    CliOption(flagName, description, matchChar, matchString, required)
            {
                if (matchChar == '\0')
                {
                    hasMatchCharacter = false;
                }
            }
        };


        template<typename ValueType>
        class CliValueFlag : public CliOption
        {
            friend class CliArguments;

        private:
            ValueType internalValue;
        public:
            CliValueFlag(const string::SmartStringView<char> &flagName, const string::SmartStringView<char> &description, char matchChar,
                         const string::SmartStringView<char> &matchString, bool required = false, ValueType defaultValue = ValueType()) :
                    CliOption(flagName, description, matchChar, matchString, required)
            {
                internalValue = defaultValue;
                if (matchChar == '\0')
                {
                    hasMatchCharacter = false;
                }
            }

            ValueType const &value() const
            {
                return internalValue;
            }
        };

        class CliFlagGroup : public CliOption
        {
            friend class CliArguments;

        private:
            //NOTE: We can have group rules. Like AtLeastOne, OnlyOne, All
            std::vector<CliOption *> options;
            CliGroupMatchPolicy m_matchPolicy = CliGroupMatchPolicy_NoPolicy;
        public:
            CliFlagGroup(const string::SmartStringView<char> &name, const std::vector<CliOption *> &flagsInGroup,
                         CliGroupMatchPolicy matchPolicy = CliGroupMatchPolicy_NoPolicy) :
                    CliOption(name, azgra::string::SmartStringView<char>(""), '\0', azgra::string::SmartStringView<char>(""), false)
            {
                hasMatchCharacter = false;
                isRequired = false;
                options = flagsInGroup;
                m_matchPolicy = matchPolicy;
            }

            bool check_group_policy(std::stringstream &errorStream) const
            {
                std::function<bool(CliOption *)> testCondition = [](CliOption *flag)
                { return flag->is_matched(); };

                switch (m_matchPolicy)
                {
                    case CliGroupMatchPolicy_All:
                    {
                        bool result = azgra::collection::all(options, testCondition);
                        if (!result)
                        {
                            errorStream << '<' << name.string_view()
                                        << ">: CliGroupMatchPolicy_All requires all flags in group to be matched.";
                        }
                        return result;
                        //return linq::all(linq::where(options, filterFunction), testCondition);
                    }

                    case CliGroupMatchPolicy_AtLeastOne:
                    {
                        bool result = azgra::collection::any(options, testCondition);
                        if (!result)
                        {
                            errorStream << '<' << name.string_view()
                                        << "> CliGroupMatchPolicy_AtLeastOne requires at least one flag to be matched.";
                        }
                        return result;
                    }
                    case CliGroupMatchPolicy_AtLeastOneIfGroupIsRequired:
                    {
                        if (isRequired)
                        {
                            bool result = azgra::collection::any(options, testCondition);
                            if (!result)
                            {
                                errorStream << '<' << name.string_view() <<
                                            "> CliGroupMatchPolicy_AtLeastOneIfGroupIsRequired requires at least one flag to be matched,"
                                            << " when this group is required.";
                            }
                            return result;
                        }
                        return true;
                    }
                    case CliGroupMatchPolicy_NoPolicy:
                    {
                        return true;
                    }
                    default:
                        INVALID_CASE;
                }
                return false;
            }
        };
    };
};