

 #include "HN_Javascript.hpp"
 #include "../HN_CSS/HN_CSS.hpp"

 /*
  * Global callback methods
  */

  static void (*onAlert)(std::string &, std::string &, ...) = NULL;


 /*
    Instruction initialization and deletion
  */
HN_JSInstruction::HN_JSInstruction()
{
    this->parent        = NULL;
    this->state         = JS_EMPTY_STATE;
    this->init_state    = JS_UN_INITIALIZE;
}


 // JS Instruction methods
void HN_JSInstruction::operator>>(std::ofstream &outFile)
{
    // Do not work if invalid descriptor
    if(!outFile)return;

    // If Simple element
    if(this->init_state != JS_CALLER)
        {
        outFile << this->type << " ";
        outFile << this->name;
    }

    // If function caller
    else{
        outFile << this->name;
        if(!this->arguments.empty())
            {
            //outFile
              //  << "(" << this->args
                //<< ")";
        }
    }

    // Print initialize data available
    if(this->init_state == JS_INITIALIZE)
        {
        outFile
            << "="
            << "\"" << this->value << "\"";
    }

    // End function
    if(this->init_state == JS_FUNCTION)
        {
        outFile << "(){\n" << std::endl;
    }

    // In other cases
    else{
        // Go to new line
        outFile << ";" << std::endl;
    }

    // Data pointer
    std::vector<struct HN_JSInstruction *>::iterator it;
    struct HN_JSInstruction *js_i;

    // Free public data
    for
    (
        it = this->publicData.begin();
        it!= this->publicData.end();
        it++
    ){
        js_i = *it;
        *js_i >> outFile;
    }

    // Free protected data
    for
    (
        it = this->protectedData.begin();
        it!= this->protectedData.end();
        it++
    ){
        js_i = *it;
        *js_i >> outFile;
    }

    // Free public data
    for
    (
        it = this->privateData.begin();
        it!= this->privateData.end();
        it++
    ){
        js_i = *it;
        *js_i >> outFile;
    }

    // End function
    if(this->init_state == JS_FUNCTION)
        {
        outFile << "}\n" << std::endl;
    }
}

void HN_JSInstruction::operator<<(std::ifstream &inFile)
{
    // Do not work if invalid descriptor
    if(!inFile)return;
}

void HN_JSInstruction::operator>>(std::string filename)
{
    std::ofstream outf(filename.c_str());
    if(outf)
        {
        *this >> outf;
        outf.close();
    }
}

void HN_JSInstruction::operator<<(std::string filename)
{
    std::ifstream inf(filename.c_str());
    if(inf)
        {
        *this << inf;
        inf.close();
    }
}


 // Destroyer
HN_JSInstruction::~HN_JSInstruction()
{
    // Data pointer
    std::vector<struct HN_JSInstruction *>::iterator it;
    struct HN_JSInstruction *js_i;

    // Free argument data
    for
    (
        it = this->arguments.begin();
        it!= this->arguments.end();
        it++
    ){
        js_i = *it;
        delete js_i;
    }

    // Free public data
    for
    (
        it = this->publicData.begin();
        it!= this->publicData.end();
        it++
        ){
        js_i = *it;
        delete js_i;
    }

    // Free protected data
    for
    (
        it = this->protectedData.begin();
        it!= this->protectedData.end();
        it++
        ){
        js_i = *it;
        delete js_i;
    }

    // Free public data
    for
    (
        it = this->privateData.begin();
        it!= this->privateData.end();
        it++
        ){
        js_i = *it;
        delete js_i;
    }
}


int compileJsFile(FILE * f, FILE * outf)
{
   fprintf(stderr, "Start scripting file\n");
   char endBuffer[10];
        endBuffer[0] = '\0';
        endBuffer[1] = '\0';
        endBuffer[2] = '\0';
        endBuffer[3] = '\0';
        endBuffer[4] = '\0';
        endBuffer[5] = '\0';
        endBuffer[6] = '\0';
        endBuffer[7] = '\0';
        endBuffer[8] = '\0';
        endBuffer[9] = '\0';

   if(f != NULL)
   {
       int i = 0;
       do{

           i = fgetc(f);

           if(i != EOF)
           {
                switch(i)
                {
                    //In case of end of instruction
                    case ';':
                            {
                                 printf("---%c", i);
                            }
                      break;

                    //In case of end of comment
                    case '/':
                            {
                                 //JumpComment(f, &i);
                            }
                      break;

                    //In case of function
                    case '(':
                            {
                                 //char **args = NULL;
                                 //int  argn = 0;
                                 //getArgument(f, &i, args, &argn);
                            }
                      break;

                    //In case of open delimitater
                    case '{':
                            {
                                 //Execute statement
                            }
                      break;

                    //In case of a prepocessor
                    case '#':
                            {
                                 //Execute statement
                            }
                      break;

                    //In case of default
                    default:
                            {
                                 fprintf(stderr, "%c", i);
                            }
                      break;
                }
           }

           //Test and of script
           endBuffer[0] = endBuffer[1];
           endBuffer[1] = endBuffer[2];
           endBuffer[2] = endBuffer[3];
           endBuffer[3] = endBuffer[4];
           endBuffer[4] = endBuffer[5];
           endBuffer[5] = endBuffer[6];
           endBuffer[6] = endBuffer[7];
           endBuffer[7] = endBuffer[8];
           endBuffer[8] = i;
           endBuffer[9] = '\0';

           //When end of file : break
           if(i == EOF){
               return -1;
           }

       }while(strcmp(endBuffer, "</script>") != 0);
       fprintf(stderr, "Script have been compiled\n");

   }
  return 0;
}

int JumpComment(FILE *f)
{

     int i = fgetc(f);
     int Lasti = 0, endInstruction = 0;

     //if '/'
     switch(i)
     {
         case '/':
                  //go to next line
                  do{
                      i = fgetc(f);
                      if(i == EOF){return i;}
                  }while(i != '\n' && i != '\r');
              break;
         case '*':
                  //go to next line
                  do{
                      i = fgetc(f);

                      //Verify if end of comment
                      if(i == '/' && Lasti == '*')
                        endInstruction = 1;

                      //save last i value
                      Lasti = i;

                  }while(endInstruction != 1);
              break;
         default:
                 //Return with one caracter
                 fseek(f, -1, SEEK_CUR);
                 i = '/';

              break;
     }

     return i;
}

int JumpComment(const char *buffer, int &cur_pos_t)
{
    // Data pointer
    int i = buffer[cur_pos_t], Lasti = 0, endInstruction = 0;
        cur_pos_t++;

     //if '/'
     switch(i)
     {
         case '/':
                  //go to next line
                  do{
                      i = buffer[cur_pos_t];
                      cur_pos_t++;

                      if(i == '\0'){return i;}
                  }while(i != '\n');
              break;
         case '*':
                  //go to next line
                  do{
                      i = buffer[cur_pos_t];
                      cur_pos_t++;

                      //Verify if end of comment
                      if((i == '/' && Lasti == '*')
                      ||  i == '\0')
                        endInstruction = 1;

                      //save last i value
                      Lasti = i;

                  }while(endInstruction != 1);
              break;
         default:
                 //Return with one caracter
                 cur_pos_t--;
                 i = '/';

              break;
     }

     return i;
}

bool isCharInside(std::string &word)
{
    size_t b_len = word.length();
    for(size_t i = 0; i < b_len; i++)
        {
        if(
            word[i] != ' '  && word[i] != '\t'
        &&  word[i] != '\n' && word[i] != '\r'
            )return true;
    }

    return false;
}

struct HN_JSInstruction *takeInstruction(FILE *f)
{
    // Data pointer
    int i;
    struct HN_JSInstruction *js_i = new struct HN_JSInstruction;
    std::string word = "ZZZZZZZZZZZZZ";

    // If Callback available
    if(onAlert != NULL)
        {
        onAlert(word, word);
        word.clear();
    }

    // In other case
    else{
        word.clear();
    }

    // Data loop
    for(;;)
    {
        i = fgetc(f);
        switch(i)
        {
              // In case of comment
            case '/': i = JumpComment(f);
                break;

              // In case of C DATA
            case '#':
                //go to next line
                do{
                    i = fgetc(f);
                    if(i == EOF){break;}
                }while(i != '\n' && i != '\r');
                break;

              // In case of new word
            case EOF:
            case ' ':
                    // Verify if work contain something
                    if(!isCharInside(word))
                        break;

                    // Get type if not yet
                    if(js_i->state == JS_EMPTY_STATE)
                        {
                        //printf("-- typedef equal to %s \n", word.c_str());
                        if(COMPARE(word.c_str(), "PUBLIC"))
                            {
                            js_i->ported = JS_PUBLIC_AREA;
                        }
                        else if(COMPARE(word.c_str(), "PROTECTED"))
                            {
                            js_i->ported = JS_PROTECTED_AREA;
                        }
                        else if(COMPARE(word.c_str(), "PRIVATE"))
                            {
                            js_i->ported = JS_PRIVATE_AREA;
                        }
                        else{
                            js_i->type = word;
                            js_i->state= JS_TAKING_NAME;

                            if(word == "function")
                                js_i->init_state = JS_FUNCTION;
                        }
                    }

                    // Try to get instruction name
                    else if(js_i->state == JS_TAKING_NAME)
                        {
                        //printf("++ Name equal to [%s] \n", word.c_str());
                        js_i->name = word;
                        js_i->state= JS_LOOKING_EQUAL;
                    }
                    else if(js_i->state == JS_WAITING_END
                        ||  js_i->state == JS_READY_FOR_USE
                        ||  js_i->state == JS_TAKING_VALUE)
                        {
                        // Detect object allocation
                        if(COMPARE(word.c_str(), "NEW"))
                            {
                            // printf("Allocating new object\n");
                            js_i->init_state = JS_OBJECT;
                        }
                        else{
                            js_i->type = word;
                            js_i->state= JS_WAITING_END;
                        }
                    }

                    else{

                    }
                    word.clear();
                break;

              // In case of end of instruction
            case ';':
                {
                    // If invoking function
                    if(js_i->state == JS_INVOKE)
                        {
                        return js_i;
                    }

                    // Try to get instruction name
                    else if(js_i->state == JS_TAKING_NAME)
                        {
                        //printf("Name equal to %s \n", word.c_str());
                        js_i->name      = word;
                        js_i->state     = JS_READY_FOR_USE;
                        js_i->init_state= JS_UN_INITIALIZE;
                        return js_i;
                    }

                    // Try to get instruction name
                    else if(js_i->state == JS_TAKING_VALUE)
                        {
                        //printf("Name equal to %s \n", word.c_str());
                        if(js_i->init_state == JS_OBJECT)
                            {
                            js_i->type  = word;
                            js_i->state = JS_READY_FOR_USE;
                        }
                        else{
                            js_i->value = word;
                            js_i->state     = JS_READY_FOR_USE;
                            js_i->init_state= JS_INITIALIZE;
                        }

                        return js_i;
                    }

                    // Verify if ready
                    else if(js_i->state == JS_WAITING_END)
                        {
                        js_i->state     = JS_READY_FOR_USE;
                        js_i->init_state= js_i->init_state != JS_OBJECT ? JS_INITIALIZE : JS_OBJECT;

                       // printf("\n\nInstruction ready{\n%s %s=\"%s\"\n}\n",
                         //      js_i->type.c_str(),
                           //    js_i->name.c_str(),
                             //  js_i->value.c_str()
                               //);
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
                        return js_i;
                    }

                    // Verify if empty state
                    else if(js_i->state == JS_EMPTY_STATE)
                        {
                        if(!word.empty())
                            {
                            js_i->name      = word;
                            js_i->state     = JS_READY_FOR_USE;
                            js_i->init_state= JS_UN_INITIALIZE;
                            return js_i;
                        }
                        /*
                        else{
                            delete js_i;
                            return NULL;
                        }
                        */
                    }
                }
                break;

              // After break stament
            case ':':
                {
                    js_i->value = word;
                    word.clear();
                    return js_i;
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
                    while((shell = takeInstruction(f)) != NULL)
                        {
                        js_i->publicData.push_back(shell);
                        //printf("LOOK\n");
                    }

                    // Return actual element us block instruction
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
                        getStringByQuote(f, js_i->value);
                        js_i->state = JS_WAITING_END;
                    }

                    // Otherwise :: stock in word
                    else{
                        word.clear();
                        getStringByQuote(f, word);
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
                        getStringByDoubleQuote(f, js_i->value);
                        js_i->state = JS_WAITING_END;
                    }

                    // Otherwise :: stock in word
                    else{
                        //word.clear();
                        getStringByDoubleQuote(f, word);
                    }
                }
                break;

              // In case of assignation
            case '=':
                    // Try to get instruction name
                    if(js_i->state == JS_TAKING_NAME)
                        {
                        // If taking name
                        if(!word.empty())
                            {
                            //printf("Name equal to %s \n", word.c_str());
                            js_i->name = word;
                            js_i->state= JS_TAKING_VALUE;
                        }
                        // If instanciate
                        else{
                            //printf("Ready for instance on %s \n", js_i->type.c_str());
                            js_i->name  = js_i->type;
                            js_i->state = JS_TAKING_VALUE;
                            js_i->type.clear();
                        }
                    }

                    // If looking equal
                    else if(js_i->state == JS_LOOKING_EQUAL)
                        {
                        js_i->state = JS_TAKING_VALUE;
                        //printf("*");
                    }

                    // If empty
                    else if(js_i->state == JS_EMPTY_STATE)
                        {
                        // If taking name
                        if(!word.empty())
                            {
                            //printf("Ready for instance on %s \n", js_i->type.c_str());
                            js_i->name = word;
                            js_i->state= JS_TAKING_VALUE;
                        }
                        // If instanciate
                        else{
                            delete js_i;
                            return NULL;
                        }
                    }

                    // Otherwise :: return error
                    else{
                        delete js_i;
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
                        if(
                            COMPARE(word.c_str(), "IF")     || COMPARE(word.c_str(), "SWITCH")
                        ||  COMPARE(word.c_str(), "WHILE")  || COMPARE(word.c_str(), "FOR")
                        ){
                            // Define statement
                            js_i->type  = "statement";
                            js_i->name  = word;
                            js_i->state = JS_VERIFY;
                            js_i->init_state = JS_STATEMENT;

                            // Read arguments
                            word.clear();
                            getArgument(f, js_i);
                            if(!isNextCharacter(f, '{'))
                                {
                                return js_i;
                            }
                        }else{
                            // Define statement
                            js_i->type  = "call";
                            js_i->name  = word;
                            js_i->state = JS_INVOKE;
                            js_i->init_state = JS_CALLER;

                            // Read arguments
                            word.clear();
                            getArgument(f, js_i);
                        }
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
                        getArgument(f, js_i);
                    }

                    else if(js_i->state == JS_WAITING_END
                        ||  js_i->state == JS_READY_FOR_USE
                        ||  js_i->state == JS_TAKING_VALUE)
                        {
                        // printf("ZZZZ:%s\n", word.c_str());
                        getArgument(f, js_i);
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
                    return NULL;
                }
                break;

              // By default
            default:
                    word+= i;
                break;
        }

        // If end of file
        if(i == EOF)
            break;
    }

    // Do not return empty instruction
    if(
       js_i != NULL
    && js_i->state == JS_EMPTY_STATE
        ){
        delete js_i;
        return NULL;
    }

    // Return instruction
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

void getArgument(FILE *f, struct HN_JSInstruction *js_i)
{
    //Get argument from file
    int i, li = 0;
    std::string word;
    //printf("(");

    for(;;)
        {
        i = fgetc(f);

        if(i == ' ' || i == '\n' || i == '\t')
            continue;
        else if((i == ')' && li != '\\') || i == EOF)
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
            getStringByDoubleQuote(f, word);
            addArgumentByValue(js_i, word);
            word.clear();
            continue;
        }
        else if(i == '\'')
            {
            word.clear();
            getStringByQuote(f, word);
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

void getStringByQuote(FILE *f, std::string &word)
{
    //Get string from file
    int i, li = 0;
    //printf("'");

    for(;;)
        {
        i = fgetc(f);

        if((i == '\'' && li != '\\') || i == EOF)
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

void getStringByDoubleQuote(FILE *f, std::string &word)
{
    //Get argument from file
    int i, li = 0;
    //printf("\"");

    for(;;)
        {
        i = fgetc(f);

        if((i == '"' && li != '\\') || i == EOF)
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


// Setup callback
void setOnAlert(void (*f)(std::string &, std::string &, ...))
{
    onAlert = f;
}


