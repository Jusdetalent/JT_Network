#ifndef HN_JAVASCRIPT_HPP_INCLUDED
#define HN_JAVASCRIPT_HPP_INCLUDED

    //Includes
    #include <cstdio>
    #include <cstdlib>
    #include <cstring>
    #include <string>
    #include <vector>
    #include <fstream>

    #ifdef _MSC_VER
    #define COMPARE(a, b) (!stricmp((a), (b)))
    #define I_COMPARE(a, b, c) (!strincmp((a), (b), (c))
    #else
    #define COMPARE(a, b) (!strcasecmp((a), (b)))
    #define I_COMPARE(a, b, c) (!strncasecmp((a), (b), (c)))
    #endif

    // Enumeration for read level
    enum JS_READ_STATE{JS_EMPTY_STATE, JS_TAKING_NAME, JS_LOOKING_EQUAL, JS_TAKING_VALUE,
                        JS_WAITING_END, JS_READY_FOR_USE, JS_INVOKE, JS_VERIFY};

    // Enumeration for object type
    enum JS_INIT_STATE{JS_UN_INITIALIZE, JS_INITIALIZE, JS_CLASS, JS_FUNCTION, JS_CALLER,
                        JS_OBJECT, JS_STATEMENT};

    // Enumeration for instruction ported
    enum JS_SHELL_PORTED{JS_ARGUMENT, JS_PUBLIC_AREA, JS_PROTECTED_AREA, JS_PRIVATE_AREA};

    // Instruction structure
    struct HN_JSInstruction
    {
        // Builder
        HN_JSInstruction();

        // Identifier
        std::string
            name,   value,      type,   settings;
        enum JS_READ_STATE      state;
        enum JS_INIT_STATE      init_state;
        enum JS_SHELL_PORTED    ported;

        // Family member
        struct HN_JSInstruction *parent;
        std::vector<struct HN_JSInstruction *> arguments;
        std::vector<struct HN_JSInstruction *> publicData;
        std::vector<struct HN_JSInstruction *> protectedData;
        std::vector<struct HN_JSInstruction *> privateData;

        // Input - output management
        void operator>>(std::ofstream &outFile);
        void operator<<(std::ifstream &inFile);
        void operator>>(std::string filename);
        void operator<<(std::string filename);

        // Destroyer
        ~HN_JSInstruction();
    };

    //Prototypes
    int  JumpComment(FILE *f);
    bool isCharInside(std::string &word);
    struct HN_JSInstruction *takeInstruction(FILE *f);

    // Buffer reading
    int JumpComment(const char *buffer, int &cur_pos_t);
    void getArgument(FILE *f, struct HN_JSInstruction *instruction);
    void getStringByQuote(FILE *f, std::string &word);
    void getStringByDoubleQuote(FILE *f, std::string &word);

    // Setup callback
    void setOnAlert(void (*f)(std::string &, std::string &, ...));

#endif // HN_JAVASCRIPT_H_INCLUDED
