#include <iostream>
#include <map>
#include <stack>
#include "jsoncpp.h"

using namespace std;

int main(){
    string test = "{test:2,gt:{1:{\"key with space\":\"value with space\",2:test2,3:test3,4:test4},4:gta},abc:def,obj:\"{a:b,c:d}\"}";
    
    map<string,JsonObject*>* currentMap = jsonDecode(test);
    
    cout << toStr((currentMap->at("test"))->value) << endl;

    cout << toStr(toMap(toMap(currentMap->at("gt")->value).at("1")->value).at("2")->value) << endl;

    cout << toStr(toMap(currentMap->at("gt")->value).at("4")->value) << endl;

    cout << toStr((currentMap->at("abc"))->value) << endl;

    cout << toStr((currentMap->at("obj"))->value) << endl;

    for(auto i : toMap(toMap(currentMap->at("gt")->value).at("1")->value)){
        cout << i.first << " : " << toStr(i.second->value) << endl;
    }
}