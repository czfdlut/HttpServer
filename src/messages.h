#ifndef __MESSAGES_H_
#define __MESSAGES_H_

#include <map>
#include <string>
#include <iostream>
#include "cJSON.h"

using std::map;
using std::string;

class Messages
{
public:
    static string CreateMessage();
    static string CreateSign(cJSON *);

private:
    static const string kExtraCode;
};

#endif 

