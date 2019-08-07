#ifndef HN_STRINGJAVASCRIPT_HPP_INCLUDED
#define HN_STRINGJAVASCRIPT_HPP_INCLUDED

    // Depending header
    #include "HN_Javascript.hpp"

    // Prototype
    struct HN_JSInstruction *takeInstruction
                                (
                                    const char *buffer_t,
                                    int *cur_pos_t
                                );
    void getArgument
            (
                const char *buffer_t,
                struct HN_JSInstruction *instruction,
                int &cur_pos_t
            );
    void getStringByQuote
            (
                const char *buffer_t,
                std::string &word,
                int &cur_pos_t
            );
    void getStringByDoubleQuote
            (
                const char *buffer_t,
                std::string &word,
                int &cur_pos_t
            );

#endif // HN_STRINGJAVASCRIPT_HPP_INCLUDED
