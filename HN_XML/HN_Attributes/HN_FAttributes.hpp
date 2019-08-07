#ifndef HN_FILE_ATTRIBUTES_HPP_INCLUDED
#define HN_FILE_ATTRIBUTES_HPP_INCLUDED

    #include "HN_Attributes.hpp"

    //Prototypes
    std::vector<struct HN_Attribute *> *takeAttributes
                                            (
                                                FILE *f,
                                                FILE *outf,
                                                FILE *cfgfile,
                                                bool &simple
                                            );
    std::string getAttributeName(FILE *f, bool &simple);
    std::string getAttributeValue(FILE *f, bool &simple, int &end_char_t);
    std::string getContent(FILE *f, bool &childNode_exist);


    // Attributes by string
    std::vector<struct HN_Attribute *> *takeAttributes
                                            (
                                                const char  *buffer_t,
                                                FILE        *outf,
                                                FILE        *cfgfile,
                                                bool        &simple,
                                                int         &cur_pos_t
                                            );
    std::string getAttributeName
                    (
                        const char  *buffer_t,
                        bool        &simple,
                        int         &cur_pos_t
                    );
    std::string getAttributeValue
                    (
                        const char  *buffer_t,
                        bool        &simple,
                        int         &cur_pos_t,
                        int         &end_char_t
                    );
    std::string getContent
                    (
                        const char  *buffer_t,
                        bool        &childNode_exist,
                        int         &cur_pos_t
                    );


#endif // HN_FILE_ATTRIBUTES_HPP_INCLUDED
