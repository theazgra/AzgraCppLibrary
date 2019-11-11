#include <azgra/cli/cli_arguments.h>

namespace azgra
{
    namespace cli
    {
        CliArguments::CliArguments(const string::SmartStringView<char> &name, const string::SmartStringView<char> &description, int width)
        {
            appName = name;
            appDescription = description;
            outputWidth = width;
        }

        bool CliArguments::process_matched_flag(const string::SmartStringView<char> &match, bool shortMatch, const char **arguments,
                                                int &parseIndex)
        {
            CliOption *matchedFlag = nullptr;
            // Find the flag.
            for (CliOption *flag : flags)
            {
                if ((!shortMatch && flag->matchString == match.substring(2)) ||
                    (shortMatch && flag->hasMatchCharacter && flag->matchCharacter == match[1]))
                {
                    flag->mark_as_matched();
                    matchedFlag = flag;
                    break;
                }
            }

            if (!matchedFlag)
            {
                errorStream << "Flag " << match.data() << " wasn't specified." << std::endl;
                return false;
            }
            ++parseIndex;

            // Next we check if we found normal or value flag.
            auto *normalFlag = dynamic_cast<CliFlag *>(matchedFlag);
            if (normalFlag)
            {
                //fprintf(stdout, "Captured normal flag: %s\n", normalFlag->name.data());
                return true;
            }
            else
            {
                // We found value flag.
                if (process_matched_value_flag(matchedFlag, arguments[parseIndex++]))
                    return true;
            }
            return false;
        }

        bool CliArguments::process_matched_value_flag(CliOption *matchedFlag, const char *rawFlagValue)
        {
            if (!rawFlagValue)
            {
                errorStream << "There is no value for '" << matchedFlag->name.string_view() << "' flag.";
                return false;
            }

            // Supported types: int, uint, string, float
            auto *intFlag = dynamic_cast<CliValueFlag<int> *> (matchedFlag);
            if (intFlag)
            {
                intFlag->internalValue = std::stoi(rawFlagValue);
                //fprintf(stdout, "Captured CliValueFlag<int> flag: %s with value: %i\n", intFlag->name.data(), intFlag->value());
                return true;
            }

            auto *uintFlag = dynamic_cast<CliValueFlag<unsigned int> *> (matchedFlag);
            if (uintFlag)
            {
                uintFlag->internalValue = static_cast<unsigned int> (std::stoul(rawFlagValue));
                //fprintf(stdout, "Captured CliValueFlag<uint> flag: %s with value: %u\n", uintFlag->name.data(), uintFlag->value());
                return true;
            }

            auto *floatFlag = dynamic_cast<CliValueFlag<float> *> (matchedFlag);
            if (floatFlag)
            {
                floatFlag->internalValue = std::stof(rawFlagValue);
                //fprintf(stdout, "Captured CliValueFlag<float> flag: %s with value: %f\n", floatFlag->name.data(), floatFlag->value());
                return true;
            }

            auto *constCharFlag = dynamic_cast<CliValueFlag<const char *> *> (matchedFlag);
            if (constCharFlag)
            {
                constCharFlag->internalValue = rawFlagValue;
                //fprintf(stdout, "Captured CliValueFlag<int> flag: %s with value: %s\n",
                //        constCharFlag->name.data(), constCharFlag->value());
                return true;
            }

            auto *stringFlag = dynamic_cast<CliValueFlag<std::string> *> (matchedFlag);
            if (stringFlag)
            {
                stringFlag->internalValue = rawFlagValue;
//                fprintf(stdout, "Captured CliValueFlag<int> flag: %s with value: %s\n",
//                        stringFlag->name.data(), stringFlag->value().c_str());
                return true;
            }

            errorStream << "Unable to match ValueFlag, unsupported type. Supported types:" <<
                        " int, uint, float, const char*, std::string" << std::endl;
            return false;
        }

        bool CliArguments::process_matched_method(const string::SmartStringView<char> &match)
        {
            for (CliMethod *method : methods)
            {
                if (method->name == match)
                {
                    //fprintf(stdout, "Captured method: %s\n", method->name.data());
                    method->mark_as_matched();
                    someMethodMatched = true;
                    return true;
                }
            }
            errorStream << "Unable to match `possible method` " << match.string_view() << std::endl;
            return false;
        }

        bool CliArguments::process_multiflag(const string::SmartStringView<char> &match)
        {
            bool allFound = true;
            for (size_t i = 1; i < match.length(); ++i)
            {
                bool found = false;
                for (CliOption *flag : flags)
                {
                    if (flag->hasMatchCharacter && flag->matchCharacter == match[i])
                    {
                        found = true;
                        flag->mark_as_matched();
                        //fprintf(stdout, "MultiFlag: matched flag: %s\n", flag->name.data());
                        break;
                    }
                }
                allFound &= found;
            }
            return allFound;
        }

        std::vector<CliOption *> CliArguments::get_flags_not_in_group() const
        {
            std::function<bool(CliOption *)> filter = [](CliOption *flag)
            { return !(flag->is_grouped()); };

            std::vector<CliOption *> result = azgra::collection::where(flags, filter);
            return result;
        }

        void CliArguments::print_flags(std::stringstream &outStream, const std::vector<CliOption *> &flagsToPrint) const
        {
            std::vector<int> vec;
            vec.resize(10);
            vec.reserve(10);

            for (const CliOption *flag : flagsToPrint)
            {
                const char *req = (flag->isRequired ? "Required" : "Optional");

                string::AsciiString matcherString;

                if (flag->hasMatchCharacter)
                {
                    char matchCharString[2] = CHAR_AS_STRING(flag->matchCharacter);
                    matcherString += {"{-", matchCharString, ", --", flag->matchString.data(), "} "};
                }
                else
                {
                    matcherString += {"{--", flag->matchString.data(), "} "};
                }

                matcherString.pad_right(' ', FIRST_COLUMN_WIDTH);
                outStream << '\t' << matcherString.get_c_string();


                string::AsciiString flagNameDesc({flag->name.data(), " - ", flag->description.data()});
                flagNameDesc.pad_right(' ', SECOND_COLUMN_WIDTH);

                outStream << flagNameDesc.get_c_string() << " [" << req;

                do
                {
                    auto *intFlag = dynamic_cast<const CliValueFlag<int> *> (flag);
                    if (intFlag)
                    {
                        outStream << ", int";
                        if (intFlag->hasDefaultValue)
                        {
                            outStream << "=" << intFlag->internalValue;
                        }
                        break;
                    }

                    auto *uintFlag = dynamic_cast<const CliValueFlag<unsigned int> *> (flag);
                    if (uintFlag)
                    {
                        outStream << ", uint";
                        if (uintFlag->hasDefaultValue)
                        {
                            outStream << "=" << uintFlag->internalValue;
                        }

                        break;
                    }

                    auto *sizeTFlag = dynamic_cast<const CliValueFlag<size_t> *> (flag);
                    if (sizeTFlag)
                    {
                        outStream << ", size_t";
                        if (sizeTFlag->hasDefaultValue)
                        {
                            outStream << "=" << sizeTFlag->internalValue;
                        }
                        break;
                    }

                    auto *floatFlag = dynamic_cast<const CliValueFlag<float> *> (flag);
                    if (floatFlag)
                    {
                        outStream << ", float";
                        if (floatFlag->hasDefaultValue)
                        {
                            outStream << "=" << floatFlag->internalValue;
                        }
                        break;
                    }

                    auto *constCharFlag = dynamic_cast<const CliValueFlag<const char *> *> (flag);
                    if (constCharFlag)
                    {
                        outStream << ", const char*";
                        if (constCharFlag->hasDefaultValue)
                        {
                            outStream << "=" << constCharFlag->internalValue;
                        }
                        break;
                    }

                    auto *stringFlag = dynamic_cast<const CliValueFlag<std::string> *> (flag);
                    if (stringFlag)
                    {
                        outStream << ", std::string";
                        if (stringFlag->hasDefaultValue)
                        {
                            outStream << "=" << stringFlag->internalValue;
                        }
                        break;
                    }
                } while (false);

                outStream << "]" << std::endl;
            }
        }

        void CliArguments::mark_required_groups()
        {
            std::function<bool(CliOption *)> filter_isGroup = [](CliOption *option)
            {
                CliFlagGroup *group = dynamic_cast<CliFlagGroup *>(option);
                return (group != nullptr);
            };
            std::function<void(CliOption *)> work_markAsRequired = [](CliOption *option)
            {
                option->isRequired = true;
            };

            for (const CliMethod *method : methods)
            {
                auto methodRequiredFlags = method->get_required_flags();
                auto requiredGroups = azgra::collection::where(methodRequiredFlags, filter_isGroup);
                azgra::collection::for_each(requiredGroups, work_markAsRequired);
            }
        }

        bool CliArguments::parse(const int argc, char **argv)
        {
            return parse(argc, const_cast<const char **>(argv));
        }

        bool CliArguments::parse(const int argc, const char **argv)
        {
            bool failed = false;
            // Lets mark required groups from methods.
            mark_required_groups();

            const char *helpIdentifier = "--help";
            const char *helpIdentifierShort = "-h";

            int argIndex = 1;
            // We skip the program path. Although we can use it later maybe.
            while ((!failed) && (argIndex < argc))
            {
                string::SmartStringView<char> word(argv[argIndex]);
                if (strcmp(word.data(), helpIdentifier) == 0 || strcmp(word.data(), helpIdentifierShort) == 0)
                {
                    print_help();
                    return true;
                }
                if (word.starts_with("--"))
                {
                    // It can be long flag.
                    if (!process_matched_flag(word, false, argv, argIndex))
                    {
                        failed = true;
                        break;
                    }
                }
                else if (word.starts_with('-') && word.length() == 2)
                {
                    // It can be short flag or short flags.
                    if (!process_matched_flag(word, true, argv, argIndex))
                    {
                        failed = true;
                        break;
                    }
                }
                else if (word.starts_with('-'))
                {

                    if (!process_multiflag(word))
                    {
                        failed = true;
                        break;
                    }
                    ++argIndex;
                }

                else
                {
                    // It can be method.
                    if (!process_matched_method(word))
                    {
                        failed = true;
                        break;
                    }
                    ++argIndex;
                }
            }

            if (!failed)
            {
                // Check if required flags for methods were matched.
                for (const CliMethod *method : methods)
                {
                    if (!method->isMatched)
                        continue;
                    for (const CliOption *methodOption : method->get_required_flags())
                    {
                        if (!methodOption->isMatched)
                        {
                            errorStream << "Method " << method->name.string_view() << " require flag " << methodOption->name.string_view()
                                        <<
                                        std::endl;
                            failed = true;
                        }
                    }
                }

                // Check if all required flags were matched.
                for (CliOption *flag : flags)
                {
                    if (flag->isRequired && !flag->isMatched)
                    {
                        failed = true;
                        errorStream << "Flag: " << flag->name.data() << " is required, but wasn't matched." << std::endl;
                    }
                }

                for (const CliFlagGroup &group : groups)
                {
                    failed |= !(group.check_group_policy(errorStream));
                }
            }

            if (failed && printHelpOnParserError)
            {
                print_help();
                print_colorized(azgra::ConsoleColor::ConsoleColor_Yellow, "Parser errors:\n%s\n", get_error().c_str());
            }

            return !(failed);
        }

        void CliArguments::print_help() const
        {
            // This is just stupid print. So we assert this condition.
            string::AsciiString nameSs(appName.data());
            nameSs.fill_left(' ', FIRST_COLUMN_WIDTH);

            std::stringstream helpStream;
            helpStream << '\t' << nameSs.get_c_string() << std::endl << appDescription.string_view() << std::endl << std::endl;
            helpStream << "Methods:" << std::endl;
            for (const auto &method : methods)
            {
                string::AsciiString methodName(method->name.data());
                string::AsciiString methodDesc(method->description.data());
                methodName.pad_right(' ', FIRST_COLUMN_WIDTH);
                methodDesc.pad_right(' ', SECOND_COLUMN_WIDTH);
                helpStream << "\t" << methodName.get_c_string() << methodDesc.get_c_string();

                auto methodReqFlags = method->get_required_flags();
                auto methodOptionalFlags = method->get_optional_flags();
                if (!methodReqFlags.empty() || !methodOptionalFlags.empty())
                {
                    helpStream << " Flags: ";
                }

                for (const auto &methodReqFlag : methodReqFlags)
                {
                    helpStream << '<' << methodReqFlag->name.string_view() << "> ";
                }
                for (const auto &methodOptionalFlag : methodOptionalFlags)
                {
                    helpStream << '[' << methodOptionalFlag->name.string_view() << "] ";
                }
                helpStream << std::endl;
            }

            helpStream << "Flags:" << std::endl;
            print_flags(helpStream, get_flags_not_in_group());
            for (const CliFlagGroup &group : groups)
            {
                helpStream << "Flags - " << group.name.string_view() << std::endl;
                print_flags(helpStream, group.options);
            }

            fprintf(stdout, "%s", helpStream.str().c_str());
        }

        std::string CliArguments::get_error() const
        {
            std::string error = errorStream.str();
            return error;
        }

        bool CliArguments::is_any_method_matched() const
        {
            return someMethodMatched;
        }

        void CliArguments::add_group(CliFlagGroup &flagGroup)
        {
            groups.push_back(flagGroup);
            for (CliOption *flag : flagGroup.options)
            {
                flag->group = &flagGroup;
                flags.push_back(flag);
            }
        }

        void CliArguments::print_help_on_parser_error()
        {
            printHelpOnParserError = true;
        }
    };
};