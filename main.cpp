#include <iostream>
#include <unordered_map>
#include <stack>
using namespace std;
#include "jsoncpp.h"

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
    string test = "{test:2,gt:{1:{2:3}}}";
    
    stack<MapObject*> mapOpbectStack;

    bool isValue = false;
    bool isString = false;

    MapObject* currentMapObject = new MapObject;
    unordered_map<string,JsonObject*>* currentMap = new unordered_map<string,JsonObject*>;

    currentMapObject->key = "";
    currentMapObject->value = NULL;

    for(char x : test){
        switch (x)
        {
        case '{':
            if(isString){
                cout << "Error: Unexpected '{'" << endl;
                break;
            }
            currentMapObject->value = new JsonObject;
            currentMapObject->value->type = MAP;
            currentMapObject->value->value = new unordered_map<string,JsonObject*>;
            currentMapObject->parentMap = currentMap;

            currentMap = (unordered_map<string,JsonObject*>*)currentMapObject->value->value;
            mapOpbectStack.push(currentMapObject);

            currentMapObject = new MapObject;
            isValue = false;
            currentMapObject->key = "";

            break;

        case ':':
            isValue = true;
            cout << currentMapObject->key << endl;
            break;
        
        case ',':
            currentMap->insert({currentMapObject->key,currentMapObject->value});

            cout << "inserted " << *(string*)currentMapObject->value->value << endl;

            currentMapObject = new MapObject;
            currentMapObject->key = "";
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
                    isString = true;
                }
                ((string*)currentMapObject->value->value)->push_back(x);
            }
            break;
        }
    }
}