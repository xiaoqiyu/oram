//
//  Oram2Tree.h
//  oram
//
//  Created by Xiaoqi Yu on 13/10/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#ifndef oram_Oram2Tree_h
#define oram_Oram2Tree_h

#include <iostream>
#include <map>
#include <time.h>
#include "oram.h"
#include "gDef.h"
#include<algorithm>
using namespace std;



class ORAM2Tree{
public:
    ~ORAM2Tree(){
        bt1.deleteBt();
        bt2.deleteBt();
    }
    void init(long long leafNum, int ns, long long ts, long long s, long long flushTime){
        long long subLeafNum = leafNum/2;
        bt1.init(subLeafNum, ns);
        bt2.init(subLeafNum, ns);
        trunkSize = ts;
        S = s;
        nodeSize = ns;
        srand(time(NULL));
        pmSize = subLeafNum/ts + 1;
        pm.resize(pmSize, dummy);
        tm.resize(leafNum, true);//true indicates in tree 1, false indicates in tree 2
        //cout<<"pm size: "<<pmSize<<endl;
        for(long long i = 0; i < pmSize; ++ i){
            //pm[i] = i * trunkSize;
            pm[i] = rand()%subLeafNum +1;
        }
        for(long long i = leafNum/2; i < leafNum; ++i){
            tm[i] = false;
        }
        
        for(long long i = 0; i < subLeafNum; ++i){
            accessAndFlush( 1, i);
        }
        for(long long i = subLeafNum; i < leafNum; ++i){
            accessAndFlush( 1, i);
        }
    }
    
    vector<RangeTreeNode* > access(long long idx){
        long long trunkId = idx/trunkSize;
        vector<RangeTreeNode *> retPaths;
        if(tm[idx] == true){
            retPaths = accessTree(bt1, idx);
        }else if(tm[idx]== false){
            retPaths = accessTree(bt2, idx);
        }
        return retPaths;
    }
    
    vector<RangeTreeNode* >  accessTree(BinaryTree tn, long long idx){
        long long trunkId = idx / trunkSize;
        vector<long long> paths;
        vector<RangeTreeNode *> retPaths;
        long long piId = pm[trunkId];
        long long newPath = rand()% tn.getLeafNum() + 1;
        
        bool find = false;
        //cout<<"insert id: "<< idx<<"old trunk id: "<<piId<<"new trunk id is: "<<pm[trunkId]<< endl;
        
        //
        multimap< long long, long long >::iterator it;
        it = stash.find(piId);
        
        if(it != stash.end()){
            pair <multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> ret = stash.equal_range(piId);
            for(multimap<long long, long long>::iterator it= ret.first; it!=ret.second; ++it){
                if(it->second == idx){
                    find = true;
                    //cout<<"find id stash"<<endl;
                }
            }
        }
        //
        //cout<<"+++++++++++++++++++++++++++"<<endl;
        //printStash();
        if(find == false){
            retPaths = tn.getPI(piId, S);
            //cout<<"Size got from path interval: "<<retPaths.size()<<endl;
            TreeNode * tmpNode;
            vector<RangeTreeNode *>::iterator iter;
            for(iter = retPaths.begin(); iter != retPaths.end(); ++iter){
                tmpNode = (*iter)->treenode;
                for(int i = 0; i < nodeSize * 2; i+=2){
                    long long index = tmpNode -> val[i];
                    long long position = tmpNode -> val[i+1];
                    if(tmpNode->val[i] != dummy){
                        if(index == idx) {
                            find = true;
                            pm[trunkId] = newPath;
                        }
                        stash.insert(pair<long long, long long>(newPath, index));
                        //cout<<"insert stash: "<<idx<<" "<<pos<<endl;
                        tmpNode->val[i] = dummy;
                    }
                }
            }
        }
        // cout<<"----------------------------"<<endl;
        //printStash();
        
        if(find == false){
            //cout<<"insert directly"<<pm[trunkId]<<" "<<idx<<endl;
            stash.insert(pair<long long, long long>(pm[trunkId], idx));
        }
        //cout<<"stash size after access: "<<getStashSize()<<endl;
        return retPaths;
    }
    
    void printStash(){
        multimap<long long, long long> ::iterator it;
        for(it = stash.begin(); it != stash.end(); ++it){
            cout<<"(idx: pos)"<< (*it).second<< " "<<(*it).first<<endl;
        }
    }
    
    void flushTree(vector<RangeTreeNode*> rtLst, long long idx, bool tag){
        
        vector<RangeTreeNode *> flushNodes = rtLst;
        reverse(flushNodes.begin(), flushNodes.end());
        
        vector<RangeTreeNode *> flushNodes_false ;
        
        if(tag == true){
            flushNodes_false = bt2.getPI(pm[idx/trunkSize], S);
        }else{
            flushNodes_false = bt1.getPI(pm[idx/trunkSize], S);
        }
        reverse(flushNodes_false.begin(), flushNodes_false.end());
        
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
                        tm[idx] = tag;
                        tmpNode->treenode->val[i] = idx;
                        tmpNode->treenode->val[i+1] = pos;
                        stash.erase(iter);
                    }
                }
            }
        }
        
        
        for(iter = flushNodes_false.begin(); iter != flushNodes_false.end(); ++ iter){
            tmpNode = *iter;
            low  = tmpNode -> low;
            high = tmpNode -> high;
            
            for(int i = 0; i < nodeSize * 2; i += 2){
                randPath = rand()%(high - low + 1) + low;
                //cout<<"random path is: "<<randPath<<endl;
                if(tmpNode->treenode->val[i] == dummy){
                    
                    
                    multimap<long long, long long>::iterator iter = stash.find(randPath);
                    int j = 0;
                    while (iter == stash.end() && j< REPEATTIME){
                        //cout<<"repeat"<<endl;
                        randPath = rand()%(high - low + 1) + low;
                        iter = stash.find(randPath);
                        j ++;
                        
                    }
                    
                    
                    /*
                     multimap<long long, long long>::iterator iter = stash.begin();
                     long long  j = low;
                     while (iter == stash.end() && j<= high){
                     //cout<<"repeat"<<endl;
                     //randPath = rand()%(high - low + 1) + low;
                     iter = stash.find(j);
                     j ++;
                     }
                     */
                    
                    if(iter != stash.end()){
                        long long idx  = (*iter).second;
                        long long pos = (*iter).first;
                        //cout<<"write node: "<< idx<<" "<<pos<<endl;
                        tm[idx] = !tag;
                        tmpNode->treenode->val[i] = idx;
                        tmpNode->treenode->val[i+1] = pos;
                        stash.erase(iter);
                    }
                }
            }
        }
    }
    
    
    void clearStash(){
        stash.clear();
    }
    
    long long getStashSize(){
        return stash.size();
    }
    
    
    void accessAndFlush(long long flushTimes, long long idx){
        
        vector<RangeTreeNode *> rtLst = access(idx);
        // cout<<idx<<"+++++++"<<endl;
        //printStash();
        for(long long i = 0; i < flushTimes; ++i){
            flushTree(rtLst, idx, tm[idx]);
        }
        //cout<<idx<<"--------"<<endl;
    }
    
private:
    BinaryTree bt1, bt2;
    vector<long long > pm;
    vector<bool> tm;
    multimap<long long, long long> stash;
    long long pmSize;
    long long trunkSize, S;
    int nodeSize;
};




#endif
