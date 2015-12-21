//
//  Oram.h
//  oram
//
//  Created by Xiaoqi Yu on 13/10/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#ifndef oram_Oram_h
#define oram_Oram_h

#include <iostream>
#include <map>
#include <time.h>
#include "BinaryTree.h"
#include "gDef.h"
using namespace std;


class ORAM{
public:
    ~ORAM(){
        bt.deleteBt();
    }
    int init(long long leafNum, int ns, long long ts, long long s, long long flushTime){
        bt.init(leafNum, ns);
        trunkSize = ts;
        S = s;
        nodeSize = ns;
        srand(time(NULL));
        pmSize = leafNum / ts +1;
        pm.resize(pmSize, dummy);
        //cout<<"pm size: "<<pmSize<<endl;
        for(long long i = 0; i < pmSize; ++ i){
            //pm[i] = i * trunkSize;
            pm[i] = rand()%leafNum +1;
        }
        
        vector<RangeTreeNode *> rtLst;
        int isSuccess = 0;
        
        
        for(long long i = 0; i < leafNum; ++i){
           accessAndFlush(1, i);
            //getPathAndFlush(1, i);
        }
        
        //bt.printBt();
        
        /*
        cout<<"********************line**************************"<<endl;
        for(long long i = 0; i < leafNum; ++i){
            long long trunkId = i / trunkSize;
            long long tmppiId = pm[trunkId];
            for(long long j = tmppiId; j <= tmppiId + S; j ++){
                if(bt.writeToLeaf(tmppiId+j, i,  tmppiId) == 1){
                    isSuccess = 1;
                    break;
                }
            }
        }
        bt.printBt();
         */
        
        return isSuccess;
    }
    
    
    vector<RangeTreeNode*>  access(long long idx){
        long long trunkId = idx / trunkSize;
        vector<long long> paths;
        vector<RangeTreeNode *> retPaths;
        long long piId = pm[trunkId];
        long long newPath = rand()% bt.getLeafNum() + 1;
        //cout<<"access idx is: "<< idx<<"  access path is: "<<piId<< "new path is: "<<newPath<< endl;
        
        bool find = false;
        //cout<<"insert id: "<< idx<<"old trunk id: "<<piId<<"new trunk id is: "<<pm[trunkId]<< endl;
        
        multimap< long long, long long >::iterator it;
        it = stash.find(piId);
        
        pair <multimap<long long, long long>::iterator, multimap<long long, long long>::iterator> ret = stash.equal_range(piId);
        
        if(!(ret.first == ret.second)){
            multimap<long long , long long >::iterator it;
            vector<long long> idxlst;
            for(it = ret.first; it != ret.second; ++it){
                if(it->second == idx){find = true;}
                idxlst.push_back(it->second);
            }
            if(find == false){
                stash.erase(piId);
                for(int j = 0; j < idxlst.size(); ++ j){
                    stash.insert(multimap<long long, long long>::value_type(newPath, idxlst[j]));
                }
            }
        }
        
        
        //printStash();
        if(find == false){
            retPaths = bt.getPI(piId, S);
            //cout<<"Size got from path interval: "<<retPaths.size()<<endl;
            TreeNode * tmpNode;
            vector<RangeTreeNode *>::iterator iter;
            pm[trunkId] = newPath;
            for(iter = retPaths.begin(); iter != retPaths.end(); ++iter){
                tmpNode = (*iter)->treenode;
                for(int i = 0; i < tmpNode->val.size(); i+=2){
                    long long index = tmpNode -> val[i];
                    long long position = tmpNode -> val[i+1];
                    if(tmpNode->val[i] != dummy){
                        if(index == idx) {
                            find = true;
                            //pm[trunkId] = newPath;
                        }
                        if((index/trunkSize) == trunkId ){
                            stash.insert(pair<long long, long long>(newPath, index));
                        }else{
                            stash.insert(pair<long long, long long>(position, index));
                        }
                        //cout<<"insert stash(index, newpath): "<<index<<" "<<newPath<<endl;
                        tmpNode->val[i] = dummy;
                    }
                }
            }
        }
        //cout<<"----------------------------"<<endl;
        //printStash();
        
        if(find == false){
            //cout<<"insert directly(idx, pos)"<<idx<< " "<< piId <<" "<<endl;
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
            //cout<<"before node size:++++++++++++++++++++++++++++++++++++++++++++"<<endl;
            for(int i = 0; i < tmpNode->treenode->val.size(); i += 2){
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
                    
                    //cout<<"low is, high is: "<<low <<" "<< high<<endl;
                    int count = 0;
                    multimap<long long, long long>::iterator iter = stash.begin();
                    iter = iter = stash.find(low);
                    long long  j = low+1;
                    while (iter == stash.end() && j<= high){
                        //cout<<"repeat"<<endl;
                        //randPath = rand()%(high - low + 1) + low;
                        iter = stash.find(j);
                        j ++;
                        count ++;
                
                    }
                    //cout << "while count its: "<<count <<" range is: "<< high - low <<endl;
                    
                    if(iter != stash.end()){
                        //cout <<"find00000000000000000000000000000000000000"<<endl;
                        long long idx  = (*iter).second;
                        long long pos = (*iter).first;
                        //cout<<"write node: "<< idx<<" "<<pos<<endl;
                        tmpNode->treenode->val[i] = idx;
                        tmpNode->treenode->val[i+1] = pos;
                        stash.erase(iter);
                    }else{
                        //cout<<"can not find111111111111111111111111111111111"<<endl;
                    }
                }
            }
        }
    }
    
    vector<RangeTreeNode *> write(long long idx){
        long long trunkId = idx/trunkSize;
        long long pos = pm[trunkId];
        long long newPath = rand()% bt.getLeafNum() + 1;
        pm[trunkId] = newPath;
        
        vector<RangeTreeNode *> retPaths = bt.getPathRT(pos);
        TreeNode * tmpNode;
        vector<RangeTreeNode *>::iterator iter;
        for(iter = retPaths.begin(); iter != retPaths.end(); ++iter){
            tmpNode = (*iter)->treenode;
            for(int i = 0; i < tmpNode->val.size(); i+=2){
                long long index = tmpNode -> val[i];
                long long position = tmpNode -> val[i+1];
                if(tmpNode->val[i] != dummy){
                    stash.insert(pair<long long, long long>(newPath, index));
                    //cout<<"insert stash: "<<idx<<" "<<pos<<endl;
                    tmpNode->val[i] = dummy;
                }
            }
        }
        return retPaths;
    }
    
    
    void clearStash(){
        stash.clear();
    }
    
    long long getStashSize(){
        return stash.size();
    }
    
    
    void accessAndFlush(long long flushTimes, long long idx){
        long long initSize = getStashSize();
        vector<RangeTreeNode *> rtLst = access(idx);
        long long accessSize = getStashSize();

        //cout<<"before access: after access "<<initSize<<" "<<accessSize<< endl;
        for(long long i = 0; i < flushTimes; ++i){
            flush(rtLst);
            //cout<<"after flush: "<<i<<" "<<getStashSize()<<endl;
        }
        
    }
    
    void getPathAndFlush(long long flushTimes, long long idx){
        long long trunkId = idx / trunkSize;
        vector<long long> paths;
        vector<RangeTreeNode *> retPaths;
        long long piId = pm[trunkId];
        long long newPath = rand()% bt.getLeafNum() + 1;
        
        bool find = false;
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
        
        if(find == false){
            //cout<<"search in tree"<<endl;
            pm[trunkId] = newPath;
            
            for (long k = 0; k < S; ++k){
                // cout<<"before: ++++++++++"<<getStashSize()<<endl;
                retPaths = bt.getPathRT(piId+k);
                //retPaths = bt.getPI(piId+k, 1);
                //cout<<"read: "<<retPaths.size()<<endl;
                //cout<<"Size got from path interval: "<<retPaths.size()<<endl;
                TreeNode * tmpNode;
                vector<RangeTreeNode *>::iterator iter;
                for(iter = retPaths.begin(); iter != retPaths.end(); ++iter){
                    tmpNode = (*iter)->treenode;
                    for(int i = 0; i < tmpNode->val.size(); i+=2){
                        long long index = tmpNode -> val[i];
                        long long position = tmpNode -> val[i+1];
                        if(tmpNode->val[i] != dummy){
                            if(index == idx) {
                                find = true;
                            }
                            stash.insert(pair<long long, long long>(newPath, index));
                            //cout<<"insert stash: "<<index<<" "<<position<<endl;
                            tmpNode->val[i] = dummy;
                        }
                    }
                    
                    
                }
                flush(retPaths);
                // cout<<"after:------"<<getStashSize()<<endl;
                
            }
            
        }
        if(find == false){
            
            //cout<<"insert directly"<<pm[trunkId]<<" "<<idx<<endl;
            stash.insert(pair<long long, long long>(pm[trunkId], idx));
        }
        
    }
    
private:
    BinaryTree bt;
    vector<long long > pm;
    multimap<long long, long long> stash;
    long long pmSize;
    long long trunkSize, S;
    int nodeSize;
    
};



#endif
