#ifndef JSONCPP
#define JSONCPP

#include <string>
#include <map>

#define toStr(x) (*(string*)(x))
#define toMap(x) (*(map<string,JsonObject*>*)(x))

typedef enum JsonObjectType{STRING,MAP} JsonObjectType;

struct JsonObject{
    JsonObjectType type;
    void* value;
};

struct MapObject{
    std::string key;
    JsonObject* value;
    std::map<std::string,JsonObject*>* parentMap;
};

std::map<std::string,JsonObject*>* jsonDecode(std::string jsonString);

#endif