//
//  TesterCORAM.cpp
//  oram_new
//
//  Created by Xiaoqi Yu on 11/12/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#include "TesterCORAM.h"
#include <vector>

#define l 128
#define N 32
#define k 4
#define p 7

void testEncode(){
    srand(time(NULL));
    vector<int> t;
    for(int i = 0; i < l; i++){
        for(int j = 0; j < 7; ++j){
            int val = rand()%l;
            t.push_back(val);
        }
    }
    vector<int>::iterator it;
    
    
    for(int i = 0; i < 1; ++i){
        vector<int> ret;
        int flag[k];
        memset(flag, 0, sizeof(flag));
        int count = 0;
        for(int j = 0; j < 2*p; ++j){
            int path = rand()%l;
            for(int m = 0; m < k; ++m){
                ret.push_back(t[path+m]);
            }
        }
        
        for(it = ret.begin(); it != ret.end(); it+=1){
            int blockid = i/k;
            int offset;
            cout<<"push: "<<*it<<endl;
         
            if(((*it) >= blockid*k) && ((*it) <= blockid*k + k -1) ){
                offset = (*it) - blockid*k;
            }
            if(!flag[offset])flag[offset] =  1;
        }
        
        for(int j = 0; j < k; ++j){
            if(!flag[j]){
                count +=1;
            }
        }
        cout<<i<<" th read" << " "<<count<<endl;
        
    }


       }
