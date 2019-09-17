#pragma once

#include <azgra/cli/cli_option.h>
#include <azgra/string/simple_string.h>

#include <vector>
#include <cstdio>
#include <cassert>


namespace azgra
{
    namespace cli
    {
        //TODO: Handle missing ValueFlag values.

        class CliArguments
        {
        private:
            const int FIRST_COLUMN_WIDTH = 30;
            const int SECOND_COLUMN_WIDTH = 60;
            std::stringstream errorStream;
            string::SmartStringView<char> appName;
            string::SmartStringView<char> appDescription;
            int outputWidth = 80;
            bool someMethodMatched = false;
            bool printHelpOnParserError = false;
            std::vector<CliFlagGroup> groups;

            bool process_matched_flag(const string::SmartStringView<char> &match, bool shortMatch, const char **arguments, int &parseIndex);

            bool process_matched_value_flag(CliOption *matchedFlag, const char *rawFlagValue);

            bool process_matched_method(const string::SmartStringView<char> &match);

            bool process_multiflag(const string::SmartStringView<char> &match);

            void print_flags(std::stringstream &outStream, const std::vector<CliOption *> &flags) const;
            std::vector<CliOption*> get_flags_not_in_group() const;
            void mark_required_groups();

        public:
            std::vector<CliMethod *> methods;
            std::vector<CliOption *> flags;

            CliArguments(const string::SmartStringView<char> &name, const string::SmartStringView<char> &description, int width = 80);

            void add_group(CliFlagGroup &flagGroup);

            bool parse(const int argc, const char **argv);

            void print_help() const;

            std::string get_error() const;

            bool is_any_method_matched() const;

            void print_help_on_parser_error();

        };
    };
};