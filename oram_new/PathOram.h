//
//  PathOram.h
//  oram
//
//  Created by Xiaoqi Yu on 13/10/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#ifndef oram_PathOram_h
#define oram_PathOram_h

#include <iostream>
#include <map>
#include <time.h>
#include "BinaryTree.h"
#include "gDef.h"
using namespace std;


class PathOram{
public:
    ~PathOram(){
        bt.deleteBt();
    }
    
   //Input: ns: nodesize; leafNum: leaf number of the binary tree
    
    
    void init(long long leafNum, int ns, long long flushTime){
        bt.init(leafNum, ns);
        nodeSize = ns;
        srand(time(NULL));
        pmSize = leafNum;
        pm.resize(pmSize,  dummy);
        for(long long i = 0; i < pmSize; ++i){
            pm[i] = rand()%leafNum + 1;
        }
        for(long long i  = 0; i < leafNum; ++i){
            accessAndFlush(1, i);
        }
    }
    
    vector<RangeTreeNode*>  access(long long idx){
        vector<long long> paths;
        vector<RangeTreeNode *> retPaths;
        long long curPath = pm[idx];
        long long newPath = rand()% bt.getLeafNum() + 1;
        bool find = false;
        
        multimap<long long, long long>::iterator it;
        it = stash.find(curPath);
        
        if(it != stash.end()){
            pair <multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> ret = stash.equal_range(curPath);
            for(multimap<long long, long long>::iterator it= ret.first; it!=ret.second; ++it){
                if(it->second == idx){
                    find = true;
                    //cout<<"find id stash"<<endl;
                }
            }
        }
        
        if(find ==false){
            retPaths = bt.getPI(curPath, 1);
            TreeNode *tmpNode;
            vector<RangeTreeNode *> ::iterator iter;
            for(iter = retPaths.begin(); iter != retPaths.end(); ++iter){
                tmpNode = (*iter)->treenode;
                for(int i = 0; i < nodeSize *2; i+=2){
                    long long index = tmpNode -> val[i];
                    long long position =tmpNode ->val[i+1];
                    if(tmpNode -> val[i] != dummy){
                        if(index == idx){
                            find = true;
                            pm[idx] = newPath;
                        }
                        stash.insert(pair<long long, long long>(newPath, index));
                        tmpNode -> val[i] = dummy;
                    }
                }
            }
        }
        
        if(find == false){
            stash.insert(pair<long long, long long>(pm[idx], idx));
        }

        return retPaths;
    }
    
    void printStash(){
        multimap<long long, long long> ::iterator it;
        for(it = stash.begin(); it != stash.end(); ++it){
            cout<<"(idx: pos)"<< (*it).second<< " "<<(*it).first<<endl;
        }
    }
    
    void flush(vector<RangeTreeNode*> rtLst){
        
        vector<RangeTreeNode *> flushNodes;
        long long flushPath = rand()%pmSize+1;
        //flushNodes = bt.getPI(flushPath, S);
        //cout<<"flush path is: "<< flushPath<<endl;
        flushNodes = rtLst;
        
        reverse(flushNodes.begin(), flushNodes.end());
        //flushNodes = access(flushidx);
        
        vector<RangeTreeNode *> ::iterator iter;
        RangeTreeNode* tmpNode;
        long long low, high, randPath;
        vector<long long> stashLst;
        for(iter = flushNodes.begin(); iter != flushNodes.end(); ++ iter){
            tmpNode = *iter;
            low  = tmpNode -> low;
            high = tmpNode -> high;
            
            for(int i = 0; i < nodeSize * 2; i += 2){
                randPath = rand()%(high - low + 1) + low;
                //cout<<"random path is: "<<randPath<<endl;
                if(tmpNode->treenode->val[i] == dummy){
                    
                    /*
                     multimap<long long, long long>::iterator iter = stash.find(randPath);
                     int j = 0;
                     while (iter == stash.end() && j< REPEATTIME){
                     //cout<<"repeat"<<endl;
                     randPath = rand()%(high - low + 1) + low;
                     iter = stash.find(randPath);
                     j ++;
                     
                     }
                     */
                    
                    multimap<long long, long long>::iterator iter = stash.begin();
                    long long  j = low;
                    while (iter == stash.end() && j<= high){
                        //cout<<"repeat"<<endl;
                        //randPath = rand()%(high - low + 1) + low;
                        iter = stash.find(j);
                        j ++;
                        
                    }
                    
                    
                    if(iter != stash.end()){
                        long long idx  = (*iter).second;
                        long long pos = (*iter).first;
                        //cout<<"write node: "<< idx<<" "<<pos<<endl;
                        tmpNode->treenode->val[i] = idx;
                        tmpNode->treenode->val[i+1] = pos;
                        stash.erase(iter);
                    }
                }
            }
        }
    }
    
    vector<RangeTreeNode *> write(long long idx);
    
    
    void clearStash(){
        stash.clear();
    }
    
    long long getStashSize(){
        return stash.size();
    }

    
    void accessAndFlush(long long flushTimes, long long idx){
        long long initSize = getStashSize();
        vector<RangeTreeNode *> rtLst = access(idx);
        cout<<"size of access is: "<<rtLst.size()<<endl;
        long long accessSize = getStashSize();
        //cout<<"add: +++++" << accessSize - initSize<<endl;
        for(long long i  = 0; i < flushTimes; ++i){
            flush(rtLst);
        }
        //cout<<"write: ++++"<<accessSize - getStashSize()<<endl;
    }
    void getPathAndFlush(long long flushTimes, long long idx);
        

private:
    BinaryTree bt;
    vector<long long > pm;
    multimap<long long, long long> stash;
    long long pmSize;
    int nodeSize;
};


#endif
