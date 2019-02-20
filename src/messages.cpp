#include "messages.h"
#include <string.h>
#include <string>
#include <iostream>
#include "openssl_md5_wrapper.h"

using std::string;
using std::cout;
using std::cin;

const string Messages::kExtraCode = "abx23579436";

string Messages::CreateMessage()
{
    string appid = "35385640507";
    string secret = "a14c4ab485a60833fe09064e27ae013e";
    string version = "1.0";
    string openid = "";
    string extra = "abx23579436";
    string command = "1001";
    string timestamp = "1550130142439";
    string token = "";
    string sign = "";
    string message = "";

    // root
    cJSON * jroot = cJSON_CreateObject();
    if (!jroot) {
        return message;
    }
    
    do 
    {
        // ver
        cJSON * jversion = cJSON_CreateString(version.c_str());
        if (nullptr == jversion) {
            break;
        }
        cJSON_AddItemToObject(jroot, "ver", jversion);

        // openid
        cJSON * jopenid = cJSON_CreateString(openid.c_str());
        if (nullptr == jopenid) {
            break;
        }
        cJSON_AddItemToObject(jroot, "openid", jopenid);

        // command
        cJSON * jcommand = cJSON_CreateString(command.c_str());
        if (nullptr == jcommand) {
            break;
        }
        cJSON_AddItemToObject(jroot, "command", jcommand);

        // timestamp
        cJSON * jtimestamp = cJSON_CreateString(timestamp.c_str());
        if (nullptr == jtimestamp) {
            break;
        }
        cJSON_AddItemToObject(jroot, "timestamp", jtimestamp);

        // token
        cJSON * jtoken = cJSON_CreateString(token.c_str());
        if (nullptr == jtoken) {
            break;
        }
        cJSON_AddItemToObject(jroot, "token", jtoken);

        // param
        cJSON * jparam = cJSON_CreateObject();
        if (nullptr == jparam) {
            break;
        }
        cJSON_AddItemToObject(jroot, "param", jparam);

        // appid
        cJSON * jappid = cJSON_CreateString(appid.c_str());
        if (nullptr == jappid) {
            break;
        }
        cJSON_AddItemToObject(jparam, "appid", jappid);

        // secret
        cJSON * jsecret = cJSON_CreateString(secret.c_str());
        if (nullptr == jsecret) {
            break;
        }
        cJSON_AddItemToObject(jparam, "secret", jsecret);

        // create sign
        sign = CreateSign(jroot);
        cJSON * jsign = cJSON_CreateString(sign.c_str());
        if (nullptr == jsign) {
            break;
        }
        cJSON_AddItemToObject(jroot, "sign", jsign);

    } while (0);
    
    // message 
    char* temp = cJSON_PrintUnformatted(jroot);
    if (temp) {
        message = temp;
        free(temp);
    }
    
    free(jroot);
    cout << "sign=" << sign << " message=" << message;

    return message;
}

string Messages::CreateSign(cJSON * jroot)
{
    map<string, string> params;
    int items_count = cJSON_GetArraySize(jroot);
    for (int i = 0; i < items_count; ++i) 
    {
        cJSON * jitem = cJSON_GetArrayItem(jroot, i);
        if (!jitem) {
            continue;
        }
        if (jitem->type == cJSON_String) {
            params.insert(std::make_pair(jitem->string, jitem->valuestring));
        } else {
            char* temp = cJSON_PrintUnformatted(jitem);
            if (temp) {
                string value = temp;
                free(temp);
                params.insert(std::make_pair(jitem->string, value));
            }
        }
    }

    string message;
    for (auto& iter : params) {
        if (strcasecmp(iter.first.c_str(), "sign") == 0)
            continue;
        message += iter.second;
    }
    message += kExtraCode;

    string sign = "";
    md5(message.c_str(), message.length(), sign);

    return sign;
}

