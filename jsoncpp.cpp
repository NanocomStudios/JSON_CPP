#include "jsoncpp.h"
#include <iostream>
#include <stack>
#include <map>

using namespace std;
/*
Constrains:
    1. All keys must have unique values and not null
*/

map<string,JsonObject*>* jsonDecode(string jsonString){
    stack<MapObject*> mapOpbectStack;

    bool isValue = false;
    bool isString = false;
    bool isValueString = false;

    MapObject* currentMapObject = NULL;
    map<string,JsonObject*>* currentMap = new map<string,JsonObject*>;

    for(char x : jsonString){
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
                    return NULL;
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
    return currentMap;
}