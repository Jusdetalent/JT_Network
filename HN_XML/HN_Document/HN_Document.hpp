#ifndef HN_DOCUMENT_HPP_INCLUDED
#define HN_DOCUMENT_HPP_INCLUDED

    #include "../HN_Node/HN_Node.hpp"
    #include "../HN_Node/HN_FNode.hpp"


    //namespace hnapi
    //{
        //namespace webkit
        //{
            class HN_XMLDocument
            {
                public:
                    // Builder
                    HN_XMLDocument(std::string path);

                    // Iterator operator
                    FILE *getDescriptor(void);
                    struct HN_Node *operator++(void);

                    // Destroyer
                    ~HN_XMLDocument();
                private:
                    FILE        *f;
                    std::string prologue;
            };
        //}
    //}

#endif // HN_DOCUMENT_HPP_INCLUDED
