#ifndef HEADER_HEADER_HPP
#define HEADER_HEADER_HPP

namespace AIO
{
typedef enum
    {
        CT_FOPEN,
        CT_FWRITE, 
        CT_FREAD,
        CT_CLOSE
    } CommType ;

struct CommHeader
{
    CommType type;
    union AllTypes
    {
        struct FOPEN
        {
            int path_len;
            char mode[3];
        };
        struct FWRITE
        {
            int something;
        };
    };
};

};   // namespace AIO
#endif
