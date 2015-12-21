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
#include "TesterCORAM.h"
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
    
    //cout<<"stable version1"<<endl;
    //testOram();
    //testOram2Tree();
    //testPathOram();
    testEncode();
    cout<<"begin~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    vector<int> idxlst;
    for(int i = 0; i < 8; ++i){
        int q = i/2;
        int r = i%2;
       
        int idx;
        if(r){
            idx = 7 - q;
        }else{
            idx = 0 + q;
        }
        idxlst.push_back(idx);
        //cout<<idx<<endl;
    }
    vector<int>::iterator it1;
    for(it1 = idxlst.begin(); it1 != idxlst.end(); ++it1){
        cout<<(*it1)*2<<endl;
    }
    for(it1 = idxlst.begin(); it1 != idxlst.end(); ++it1){
        cout<<(*it1)*2+1<<endl;
    }
    
    
    cout<<"end~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
    return 0;
}
