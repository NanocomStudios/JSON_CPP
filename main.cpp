#include <iostream>
#include <unordered_map>
#include <stack>
using namespace std;
#include "jsoncpp.h"

#define toStr(x) (*(string*)(x))
#define toMap(x) (*(unordered_map<string,JsonObject*>*)(x))

typedef enum JsonObjectType{STRING,MAP} JsonObjectType;

struct JsonObject{
    JsonObjectType type;
    void* value;
};

struct MapObject{
    string key;
    JsonObject* value;
    unordered_map<string,JsonObject*>* parentMap;
};

int main(){
    string test = "{test:2,gt:{1:{2:hello},4:gta},abc:def}";
    
    stack<MapObject*> mapOpbectStack;

    bool isValue = false;
    bool isString = false;

    MapObject* currentMapObject = NULL;
    unordered_map<string,JsonObject*>* currentMap = new unordered_map<string,JsonObject*>;

    for(char x : test){
        switch (x)
        {
        case '{':
            if(isString){
                cout << "Error: Unexpected '{'" << endl;
                break;
            }

            if(currentMapObject == NULL){
                currentMapObject = new MapObject;
                currentMapObject->key = "";
                currentMapObject->parentMap = currentMap;
            }else{
                currentMapObject->value = new JsonObject;
                currentMapObject->value->type = MAP;
                currentMapObject->value->value = new unordered_map<string,JsonObject*>;
                currentMapObject->parentMap = currentMap;
    
                currentMap = (unordered_map<string,JsonObject*>*)currentMapObject->value->value;
                mapOpbectStack.push(currentMapObject);
    
                currentMapObject = new MapObject;
                isValue = false;
                currentMapObject->key = "";
            }
            break;

        case ':':
            isValue = true;
            cout << currentMapObject->key << endl;
            break;
        
        case ',':
            currentMap->insert({currentMapObject->key,currentMapObject->value});

            cout << "inserted " << currentMapObject->value << endl;

            currentMapObject = new MapObject;
            currentMapObject->key = "";
            currentMapObject->parentMap = currentMap;

            isValue = false;
            isString = false;
            break;

        case '}':
            if(!mapOpbectStack.empty()){
                currentMap->insert({currentMapObject->key,currentMapObject->value});

                currentMapObject = mapOpbectStack.top();
                mapOpbectStack.pop();

                currentMap = currentMapObject->parentMap;
                isValue = false;
                isString = false;
            }else{
                currentMap->insert({currentMapObject->key,currentMapObject->value});
            }
            break;

        default:
            
            if(!isValue){
                if(x != ' '){
                    currentMapObject->key.push_back(x);
                }
            }else{
                if(!isString){
                    currentMapObject->value = new JsonObject;
                    currentMapObject->value->type = STRING;
                    currentMapObject->value->value = new string;
                    cout << "value started " << currentMapObject->value->value << endl;
                    isString = true;
                }
                ((string*)currentMapObject->value->value)->push_back(x);
            }
            break;
        }
    }
    
    cout << toStr((currentMap->at("test"))->value) << endl;

    cout << toStr(toMap(toMap(currentMap->at("gt")->value).at("1")->value).at("2")->value) << endl;

    cout << toStr(toMap(currentMap->at("gt")->value).at("4")->value) << endl;

    cout << toStr((currentMap->at("abc"))->value) << endl;
}