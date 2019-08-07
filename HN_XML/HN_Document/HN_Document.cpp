
/*
 * Xml document management source file
 * Henock @ Comedac :: 26/ 09/ 2017
 */

 #include "HN_Document.hpp"
 #include <iostream>

 // Builder
HN_XMLDocument::HN_XMLDocument(std::string path)
{
    // Open file for reading management
    this->f = fopen(path.c_str(), "r");
    if(!this->f)return;

    // Parse head data
    int i, li = 0;
    bool
        second_space    = false,
        inside_prologue = false;

    // Loop to fetch data
    do{
        i = fgetc(this->f);

        switch(i)
        {
            // Jump empty character
            case '\t':
            case '\n':
            case EOF:
                break;
            case ' ':
                if(!second_space && inside_prologue)
                    this->prologue+= i;
                second_space = true;
                break;

                // If document prologue begin
            case '?':
                if(li == '<')
                    {
                    inside_prologue = true;
                }
                break;

                // If document prologue end
            case '>':
                if(li == '?')
                    {
                    return;
                }
                break;
            case '<':
                second_space = false;
                break;

            default:
                if(li == '<')
                    {
                    fseek(this->f, -2, SEEK_CUR);
                    return;
                }
                if(inside_prologue)this->prologue+= i;
                second_space = false;
                break;
        }

        li = i;

    }while(i != EOF);
}

 // Iterator operator
FILE *HN_XMLDocument::getDescriptor(void)
{
    return this->f;
}

struct HN_Node *HN_XMLDocument::operator++(void)
{
    return takeNode(this->f);
}

 // Destroyer
HN_XMLDocument::~HN_XMLDocument()
{
    // Close file if exist
    if(this->f != NULL)
        {
        fclose(this->f);
    }
}

