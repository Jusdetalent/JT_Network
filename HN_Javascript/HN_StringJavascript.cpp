
/*
 * Js instruction parser from string
 * By Hénock @ Comedac :: 31/ 07/ 2017
 */

 #include "HN_StringJavascript.hpp"


static void update_curPos(int *__cur_pos, int &cur_pos)
{
    if(__cur_pos != NULL)*__cur_pos = cur_pos;
}


struct HN_JSInstruction *takeInstruction(const char *buffer_t, int *__cur_pos_t)
{
    // Data pointer
    int i;
    struct HN_JSInstruction *js_i = new struct HN_JSInstruction;
    std::string word;
    int cur_pos_t = __cur_pos_t != NULL ? *__cur_pos_t : 0;

    // Data loop
    for(;;)
    {
        i = buffer_t[cur_pos_t];
        switch(i)
        {
              // In case of comment
            case '/': i = JumpComment(buffer_t, cur_pos_t);
                break;

              // In case of new word
            case '\0':
            case ' ':
                    // Verify if work contain something
                    if(!isCharInside(word))
                        break;

                    // Get type if not yet
                    if(js_i->state == JS_EMPTY_STATE)
                        {
                        //printf("-- typedef equal to %s \n", word.c_str());
                        js_i->type = word;
                        js_i->state= JS_TAKING_NAME;

                        if(word == "function")
                            js_i->init_state = JS_FUNCTION;
                    }

                    // Try to get instruction name
                    else if(js_i->state == JS_TAKING_NAME)
                        {
                        //printf("++ Name equal to [%s] \n", word.c_str());
                        js_i->name = word;
                        js_i->state= JS_LOOKING_EQUAL;
                    }

                    word.clear();
                break;

              // In case of end of instruction
            case ';':
                {
                    // If invoking function
                    if(js_i->state == JS_INVOKE)
                        {
                        update_curPos(__cur_pos_t, cur_pos_t);
                        return js_i;
                    }

                    // Try to get instruction name
                    if(js_i->state == JS_TAKING_NAME)
                        {
                        //printf("Name equal to %s \n", word.c_str());
                        js_i->name = word;
                        js_i->state= JS_LOOKING_EQUAL;
                    }

                    // Verify if ready
                    if(js_i->state == JS_WAITING_END)
                        {
                        js_i->state     = JS_READY_FOR_USE;
                        js_i->init_state= JS_INITIALIZE;
                       // printf("\n\nInstruction ready{\n%s %s=\"%s\"\n}\n",
                         //      js_i->type.c_str(),
                           //    js_i->name.c_str(),
                             //  js_i->value.c_str()
                               //);
                        update_curPos(__cur_pos_t, cur_pos_t);
                        return js_i;
                    }

                    // Verify if not initialize
                    else if(js_i->state == JS_LOOKING_EQUAL)
                        {
                        js_i->state     = JS_READY_FOR_USE;
                        js_i->init_state= JS_UN_INITIALIZE;
                        //printf("\n\nInstruction ready but uninitialize{\n%s %s;\n}\n",
                          //     js_i->type.c_str(),
                            //   js_i->name.c_str()
                              // );
                        update_curPos(__cur_pos_t, cur_pos_t);
                        return js_i;
                    }
                }
                break;

              // Functions open block
            case '{':
                {
                    // Get type if not yet
                    if(js_i->state == JS_TAKING_NAME
                    && isCharInside(word))
                        {
                        //printf("Name equal to %s \n", word.c_str());
                        js_i->name = word;
                        js_i->state= JS_READY_FOR_USE;
                        js_i->init_state = JS_FUNCTION;
                    }

                    // If parent is empty
                    else if(js_i->state == JS_EMPTY_STATE)
                        {
                        // Initialize data
                        js_i->type      = "function";
                        js_i->state     = JS_READY_FOR_USE;
                        js_i->init_state= JS_FUNCTION;
                    }

                    // Add all children instruction
                    struct HN_JSInstruction *shell;
                    while((shell = takeInstruction(buffer_t, &cur_pos_t)) != NULL)
                        {
                        js_i->publicData.push_back(shell);
                        //printf("LOOK\n");
                    }

                    // Return actual element us block instruction
                    update_curPos(__cur_pos_t, cur_pos_t);
                    return js_i;
                }
                break;

              // String management by quote
            case '\'':
                {
                    // Take instruction value
                    if(js_i->state == JS_TAKING_VALUE)
                        {
                        js_i->value.clear();
                        getStringByQuote(buffer_t, js_i->value, cur_pos_t);
                        js_i->state = JS_WAITING_END;
                    }

                    // Otherwise :: stock in word
                    else{
                        word.clear();
                        getStringByQuote(buffer_t, word, cur_pos_t);
                    }
                }
                break;

              // String management by double quote
            case '"':
                {
                    // Take instruction value
                    if(js_i->state == JS_TAKING_VALUE)
                        {
                        js_i->value.clear();
                        getStringByDoubleQuote(buffer_t, js_i->value, cur_pos_t);
                        js_i->state = JS_WAITING_END;
                    }

                    // Otherwise :: stock in word
                    else{
                        word.clear();
                        getStringByDoubleQuote(buffer_t, word, cur_pos_t);
                    }
                }
                break;

              // In case of assignation
            case '=':
                    // Try to get instruction name
                    if(js_i->state == JS_TAKING_NAME)
                        {
                        //printf("Name equal to %s \n", word.c_str());
                        js_i->name = word;
                        js_i->state= JS_LOOKING_EQUAL;
                    }

                    // If looking equal
                    if(js_i->state == JS_LOOKING_EQUAL)
                        {
                        js_i->state = JS_TAKING_VALUE;
                        //printf("*");
                    }

                    // Otherwise :: return error
                    else{
                        delete js_i;
                        update_curPos(__cur_pos_t, cur_pos_t);
                        return NULL;
                    }
                break;

              // In case of duplicate instruction
            case ',':
                {
                    //printf("%s,", word.c_str());
                    word.clear();
                }
                break;

             // Avoid empty characters
            case '\n':      case '\t':
                break;

              // In case of argument
            case '(':
                {
                    // If invoking function
                    if(js_i->state == JS_EMPTY_STATE
                    && isCharInside(word))
                        {
                        // Initialize data
                        //printf("typedef equal to function\n");
                        js_i->type = "call";
                        js_i->name = word;
                        js_i->state= JS_INVOKE;
                        js_i->init_state = JS_CALLER;

                        // Read arguments
                        word.clear();
                        getArgument(buffer_t, js_i, cur_pos_t);

                        // Return instruction
                        update_curPos(__cur_pos_t, cur_pos_t);
                        return js_i;
                    }

                    // Try to get instruction name
                    else if(js_i->state == JS_TAKING_NAME)
                        {
                        // Initialize data
                        if(!word.empty())
                            {
                            //printf("++Name equal to [%s] \n", word.c_str());
                            js_i->name = word;
                            js_i->state= JS_WAITING_END;
                        }

                        // Other wise
                        else{
                            js_i->name = js_i->type;
                            js_i->type = "call";
                            js_i->state= JS_INVOKE;
                            js_i->init_state = JS_CALLER;
                        }

                        // Read arguments
                        word.clear();
                        getArgument(buffer_t, js_i, cur_pos_t);
                    }

                    //printf("%s", word.c_str());
                }
                break;

              // If end of little block
            case ')':
                break;

              // If end of great block
            case '}':
                {
                    // Free memory and return
                    delete js_i;
                    update_curPos(__cur_pos_t, cur_pos_t);
                    return NULL;
                }
                break;

              // By default
            default:
                    word+= i;
                break;
        }

        // If end of file
        cur_pos_t++;
        if(i == '\0')
            break;
    }

    // Do not return empty instruction
    if(
       js_i != NULL
    && js_i->state == JS_EMPTY_STATE
        ){
        delete js_i;
        update_curPos(__cur_pos_t, cur_pos_t);
        return NULL;
    }

    // Return instruction
    update_curPos(__cur_pos_t, cur_pos_t);
    return js_i;
}

static void addArgumentByValue(struct HN_JSInstruction *js_i,
                        std::string &word)
{
    // Build new instruction
    struct HN_JSInstruction *new_js_i = new struct HN_JSInstruction;
    if(new_js_i != NULL)
        {
        new_js_i->value     = word;
        new_js_i->type      = "argument";
        new_js_i->state     = JS_READY_FOR_USE;
        new_js_i->init_state= JS_INITIALIZE;
        new_js_i->ported    = JS_ARGUMENT;
        js_i->arguments.push_back(new_js_i);
    }
}

static void addArgumentByName(struct HN_JSInstruction *js_i,
                        std::string &word)
{
    // Build new instruction
    struct HN_JSInstruction *new_js_i = new struct HN_JSInstruction;
    if(new_js_i != NULL)
        {
        new_js_i->name      = word;
        new_js_i->type      = "argument";
        new_js_i->state     = JS_READY_FOR_USE;
        new_js_i->init_state= JS_INITIALIZE;
        new_js_i->ported    = JS_ARGUMENT;
        js_i->arguments.push_back(new_js_i);
    }
}

void getArgument(const char *buffer_t, struct HN_JSInstruction *js_i, int &cur_pos_t)
{
    //Get argument from file
    int i, li = 0;
    std::string word;
    //printf("(");

    for(;;)
        {
        cur_pos_t++;
        i = buffer_t[cur_pos_t];

        if(i == ' ' || i == '\n' || i == '\t')
            continue;
        else if((i == ')' && li != '\\') || i == '\0')
            {
            if(!word.empty())
                {
                //printf("%s...\n", word.c_str());
                addArgumentByName(js_i, word);
                word.clear();
            }
            break;
        }
        else if(i == '\\' && li != '\\')
            {
            li = i;
            continue;
        }
        else if(i == '"')
            {
            word.clear();
            getStringByDoubleQuote(buffer_t, word, cur_pos_t);
            addArgumentByValue(js_i, word);
            word.clear();
            continue;
        }
        else if(i == '\'')
            {
            word.clear();
            getStringByQuote(buffer_t, word, cur_pos_t);
            addArgumentByValue(js_i, word);
            word.clear();
            continue;
        }
        else if(i == ',')
            {
            if(!word.empty())
                {
                //printf("%s++\n", word.c_str());
                addArgumentByName(js_i, word);
                word.clear();
                continue;
            }
        }

        word+= i;
        li = i;
     }

     //printf(")");
}

void getStringByQuote(const char *buffer_t, std::string &word, int &cur_pos_t)
{
    //Get string from file
    int i, li = 0;
    //printf("'");

    for(;;)
        {
        cur_pos_t++;
        i = buffer_t[cur_pos_t];

        if((i == '\'' && li != '\\') || i == '\0')
            break;
        else if(i == '\\' && li != '\\')
            {
            li = i;
            continue;
        }

        word+= i;
        //printf("%c", i);
        li = i;
     }

     //printf("'");
}

void getStringByDoubleQuote(const char *buffer_t, std::string &word, int &cur_pos_t)
{
    //Get argument from file
    int i, li = 0;
    //printf("\"");

    for(;;)
        {
        cur_pos_t++;
        i = buffer_t[cur_pos_t];

        if((i == '"' && li != '\\') || i == '\0')
            break;
        else if(i == '\\' && li != '\\')
            {
            li = i;
            continue;
        }
        word+= i;
        //printf("%c", i);
        li = i;
     }

     //printf("\"");
}



