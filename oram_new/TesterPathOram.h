//
//  TesterPathOram.h
//  oram
//
//  Created by Xiaoqi Yu on 13/10/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#ifndef oram_TesterPathOram_h
#define oram_TesterPathOram_h

#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include "PathOram.h"
#include "gDef.h"
using namespace std;


void testPathOram(){
    PathOram oram;
    long long logN = log2(LEAFNUM);
    vector<long long> maxStash;
    maxStash.resize(ROUND, 0);
    
    for(int i = 0; i < ROUND; ++i){
        oram.clearStash();
        //long long flushPath = rand()%LEAFNUM + 1;
        cout<<"*********:"<< i << "//"<<ROUND<<"*********"<<endl;
        oram.init(LEAFNUM, NODESIZE, FLUSHNUM);
        cout<<"stash size after init  is: "<<oram.getStashSize()<<endl;
        
        vector<RangeTreeNode*> rtLst;
        
        for(long long j = 0; j < ACCESSTIME; ++j){
            //cout<<"Flush:"<<j << "//"<<LEAFNUM<<endl;
            long long flushPath = rand()%LEAFNUM+1;
            oram.accessAndFlush(FLUSHNUM, flushPath);
            //oram.getPathAndFlush(FLUSHNUM, flushPath);
            long long stashSize = oram.getStashSize();
            if(stashSize > maxStash[i]){
                maxStash[i] = stashSize;
            }
        }
        cout<<"stash size : "<<maxStash[i]<<endl;
    }
    
    //cout<<"max stash size:"<<endl;
    vector<long long>::iterator iter;
    long long failTime = 0;
    long long thresHold = logN;
    
    for(iter = maxStash.begin(); iter != maxStash.end(); ++ iter){
        //cout<<*iter<<endl;
        if( *iter > thresHold){
            failTime ++;
        }
    }
    cout<<"fail time:\t"<<failTime<<endl;
}


#endif
