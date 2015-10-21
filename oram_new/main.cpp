//
//  main.cpp
//  oram
//
//  Created by Xiaoqi Yu on 16/9/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include "TesterOram.h"
#include "TesterOram2Tree.h"
#include "TesterPathOram.h"
#include "TesterCircuitOram.h"
#include "gDef.h"
using namespace std;



int main(int argc, const char * argv[]) {
   
    /*
    multimap<int, int> mymap;
    mymap.insert(pair<int, int>(1, 2));
    mymap.insert(pair<int, int>(2, 3));
    mymap.insert(pair<int,  int>(1,4));
    
    pair<multimap<int, int>::iterator, multimap<int, int>::iterator> ret = mymap.equal_range(3);
    multimap<int, int>::iterator it;
    if(ret.second == mymap.end()){
        cout<<"it is end"<<endl;
    }
    if(ret.first == ret.second){
        cout<<"equal"<<endl;
    }
    mymap.erase(1);
    cout<<mymap.size()<<endl;
     */
     
    testOram();
    //testOram2Tree();
    //testPathOram();
    return 0;
}
