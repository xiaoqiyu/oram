//
//  TesterOram.h
//  oram
//
//  Created by Xiaoqi Yu on 13/10/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#ifndef oram_TesterOram_h
#define oram_TesterOram_h


#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include "Oram.h"
#include "gDef.h"
using namespace std;


void testOram(){
    ORAM oram;
    long long logN = log2(LEAFNUM);
    vector<long long> maxStash;
    maxStash.resize(ROUND, 0);
    
    for(int i = 0; i < ROUND; ++i){
        oram.clearStash();
        //long long flushPath = rand()%LEAFNUM + 1;
        cout<<"*********:"<< i << "//"<<ROUND<<"*********"<<endl;
        //init(leafnum, nodesize, trunksize, path interval size)
        oram.init(LEAFNUM, NODESIZE, logN, logN, FLUSHNUM);
        cout<<"stash size after init  is: "<<oram.getStashSize()<<endl;
        
        vector<RangeTreeNode*> rtLst;
        
        for(long long j = 0; j < ACCESSTIME; ++j){
            //cout<<"Flush:"<<j << "//"<<LEAFNUM<<endl;
            long long flushPath = rand()%LEAFNUM+1;
            //oram.accessAndFlush(FLUSHNUM, flushPath);
            long long stashSize = oram.getStashSize();
            if(stashSize > maxStash[i]){
                maxStash[i] = stashSize;
            }
        }
        cout<<"stash size : "<<maxStash[i]<<endl;
    }
   
    vector<long long>::iterator iter;
    long long failTime = 0;
    long long thresHold = 3*logN*logN;
    
    for(iter = maxStash.begin(); iter != maxStash.end(); ++ iter){
        //cout<<*iter<<endl;
        if( *iter > thresHold){
            failTime ++;
        }
    }
    cout<<"fail time:\t"<<failTime<<endl;
}






#endif
