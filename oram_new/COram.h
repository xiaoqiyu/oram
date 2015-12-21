//
//  COram.h
//  oram_new
//
//  Created by Xiaoqi Yu on 13/12/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#ifndef __oram_new__COram__
#define __oram_new__COram__

#include <stdio.h>
#include <iostream>
#include <map>
#include <time.h>
#include "BinaryTree.h"
#include "gDef.h"
using namespace std;

class CORAM{
public:
    ~CORAM(){
        bt.deleteBt();
    }
    int init(long long leafNum, int ns, long long ts, long long s, long long flushTime){
        int isSuccess = 0;
        bt.init(leafNum, ns);
        return isSuccess;
    }
    int mergeNode(vector<long long>& retNode, vector<long long> in1, vector<long long> in2, int ns){
        vector<long long> ret;
        vector<long long>::iterator it;
        int count = 0;
        for(it = in1.begin(); it != in1.end(); it = it+2){
            long long idx = *it;
            long long pos = *(it+1);
            if(idx != dummy){
                ret.push_back(idx);
                ret.push_back(pos);
                count += 1;
            }
        }
        
        for(it = in1.begin(); it != in2.end(); it = it+2){
            long long idx = *it;
            long long pos = *(it+1);
            if(idx != dummy){
                ret.push_back(idx);
                ret.push_back(pos);
                count += 1;
            }
        }

        if(count > ns){
            cout<<"fail, exceed the size"<<endl;
        }
        
        retNode = ret;
        
        return 0;
    }
    
  
    int add2Root(long long idx, long long pos){
        TreeNode *r = bt.getRoot();
        int len = r->val.size();
        for(int i = 0; i < len; i+=2){
            if(r->val[i] == dummy){
                r->val[i] = idx;
                r->val[i+1] = pos;
            }
        }
        return 0;
    }
    
    int flushPath(long long path, long long leafnum, int ns){
        long long low = 0;
        long long high = leafnum;
        long long mid;
        TreeNode *curNode = bt.getRoot();
        while(high > low){
            mid = (low + high)/2;
            if(path <= mid){// left child
                if(curNode -> right!= NULL){
                    curNode -> right = curNode;
                }
                if(curNode -> left != NULL){
                    mergeNode(curNode -> left->val, curNode->val, curNode->left->val, ns);
                    curNode = curNode->left;
                    high = (low+high)/2;
                }
            }else{//right child
                if(curNode -> left != NULL){
                    curNode -> left = curNode;
                }
                if(curNode -> right != NULL){
                    mergeNode(curNode -> right->val, curNode -> val, curNode->right->val, ns);
                    curNode = curNode->right;
                    low = (low+high)/2 + 1;
                }
            }
        }
        return 0;
    }
    
    int flush(){
        return 0;
    }
    
        
    
private:
    BinaryTree bt;
    
};



#endif /* defined(__oram_new__COram__) */
