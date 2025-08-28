#include <iostream>
#include <map>
#include <stack>
using namespace std;
#include "jsoncpp.h"

#define toStr(x) (*(string*)(x))
#define toMap(x) (*(map<string,JsonObject*>*)(x))

typedef enum JsonObjectType{STRING,MAP} JsonObjectType;

struct JsonObject{
    JsonObjectType type;
    void* value;
};

struct MapObject{
    string key;
    JsonObject* value;
    map<string,JsonObject*>* parentMap;
};

int main(){
    string test = "{test:2,gt:{1:{\"key with space\":\"value with space\",2:test2,3:test3,4:test4},4:gta},abc:def}";
    
    stack<MapObject*> mapOpbectStack;

    bool isValue = false;
    bool isString = false;
    bool isValueString = false;

    MapObject* currentMapObject = NULL;
    map<string,JsonObject*>* currentMap = new map<string,JsonObject*>;

    for(char x : test){
        if(isString && x != '"'){
            if(isValue){
                if(!isValueString){
                    currentMapObject->value = new JsonObject;
                    currentMapObject->value->type = STRING;
                    currentMapObject->value->value = new string;
                    cout << "value started " << currentMapObject->value->value << endl;
                    isValueString = true;
                }
                ((string*)currentMapObject->value->value)->push_back(x);
            }else{
                currentMapObject->key.push_back(x);
            }
        }else{
            switch (x){
            case '{':
                if(isValueString){
                    cout << "Error: unexpected {" << endl;
                    return -1;
                }
                if(currentMapObject == NULL){
                    currentMapObject = new MapObject;
                    currentMapObject->key = "";
                    currentMapObject->parentMap = currentMap;
                }else{
                    currentMapObject->value = new JsonObject;
                    currentMapObject->value->type = MAP;
                    currentMapObject->value->value = new map<string,JsonObject*>;
                    currentMapObject->parentMap = currentMap;
        
                    currentMap = (map<string,JsonObject*>*)currentMapObject->value->value;
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
                isValueString = false;
                break;
    
            case '}':
                if(!mapOpbectStack.empty()){
                    currentMap->insert({currentMapObject->key,currentMapObject->value});
    
                    currentMapObject = mapOpbectStack.top();
                    mapOpbectStack.pop();
    
                    currentMap = currentMapObject->parentMap;
                    isValue = false;
                    isString = false;
                    isValueString = false;
                }else{
                    currentMap->insert({currentMapObject->key,currentMapObject->value});
                }
                break;
            
            case '"':
                isString = !isString;
                break;
            
            case ' ':
                break;
    
            default:
                
                if(!isValue){
                    currentMapObject->key.push_back(x);
                }else{
                    if(!isValueString){
                        currentMapObject->value = new JsonObject;
                        currentMapObject->value->type = STRING;
                        currentMapObject->value->value = new string;
                        cout << "value started " << currentMapObject->value->value << endl;
                        isValueString = true;
                    }
                    ((string*)currentMapObject->value->value)->push_back(x);
                }
                break;
            }
        }
    }
    
    cout << toStr((currentMap->at("test"))->value) << endl;

    cout << toStr(toMap(toMap(currentMap->at("gt")->value).at("1")->value).at("2")->value) << endl;

    cout << toStr(toMap(currentMap->at("gt")->value).at("4")->value) << endl;

    cout << toStr((currentMap->at("abc"))->value) << endl;

    for(auto i : toMap(toMap(currentMap->at("gt")->value).at("1")->value)){
        cout << i.first << " : " << toStr(i.second->value) << endl;
    }
}