//
//  BinaryTree.h
//  oram
//
//  Created by Xiaoqi Yu on 13/10/15.
//  Copyright (c) 2015 Xiaoqi Yu. All rights reserved.
//

#ifndef oram_BinaryTree_h
#define oram_BinaryTree_h

#include<iostream>
#include<vector>
#include<math.h>
#include<queue>

using namespace std;
#define dummy -1


//* Definition for a binary tree node
struct TreeNode {
    vector<long long> val; //(idx_0, pos_0, idx_1, pos_1,...,idx_{Z-1}, pos_{Z-1})
    TreeNode *left;
    TreeNode *right;
    TreeNode(){
        left = right = NULL;
    }
};

//Definition of range TreeNode
struct RangeTreeNode{
    TreeNode *treenode;
    long long low;
    long long high;
    RangeTreeNode(TreeNode *tn, long long l, long long h):treenode(tn), low(l), high(h){}
};

//Definition of the binary tree
class BinaryTree{
public:
    BinaryTree(){
        root = new TreeNode();
    }
    
    void init(long long lf, int Z){
        long long curNodeNum = 0;
        long long totalNodeNum = 2*lf - 1;
        leafNum = lf;
        nodeSize = Z;
        for(int i = 0;i < 2 * 10 * nodeSize; ++ i){
            root->val.push_back(dummy);
        }
        curNodeNum ++;
        queue<TreeNode *> q;
        q.push(root);
        
        while(!q.empty() && curNodeNum < totalNodeNum){
            TreeNode *cur = q.front();
            q.pop();
            TreeNode *leftChild = new TreeNode();
            
            if (curNodeNum <= 256){
                //curNodeSize = 2*Z;
                leftChild->val.resize(2*2*nodeSize, dummy);
            }else{
                leftChild->val.resize(2*nodeSize, dummy);
            }

            curNodeNum ++;
            //if(curNodeNum >= leafNum ){
            //    leftChild->val[0] = curNodeNum - leafNum;
            //    leftChild->val[1] = curNodeNum - leafNum;
            //}
            q.push(leftChild);
            TreeNode *rightChild = new TreeNode();
            
            if (curNodeNum <= 256){
                //curNodeSize = 2*Z;
                rightChild->val.resize(2*2*nodeSize, dummy);
            }else{
                rightChild->val.resize(2*nodeSize, dummy);
            }
         
            curNodeNum ++;
            //if(curNodeNum >= leafNum){
            //    rightChild->val[0] = curNodeNum - leafNum;
            //    rightChild->val[1] = curNodeNum - leafNum;
            //}
            q.push(rightChild);
            cur -> left = leftChild;
            cur -> right = rightChild;
        }
    }
    
    
    void deleteBt(){
        TreeNode *tmpNode = root;
        queue<TreeNode *> q;
        q.push(tmpNode);
        while(! q.empty()){
            tmpNode = q.front();
            q.pop();
            //cout<<tmpNode->val[0]<<endl;
            if(tmpNode -> left != NULL)
            q.push(tmpNode -> left);
            if(tmpNode -> right != NULL)
            q.push(tmpNode -> right);
            delete tmpNode;
        }
        
    }
    TreeNode * getRoot(){ return root;}
    
    
    //get level number of the tree
    long long getLevelNum(){
        long long levelNum = 0;
        TreeNode *tmpNode = root;
        while(tmpNode != NULL){
            levelNum ++;
            tmpNode =  tmpNode -> left;
        }
        return levelNum;
    }
    
    vector <TreeNode*> getPaths(vector<long long> paths){
        vector<long long>::iterator iter;
        vector<TreeNode*> retPath;
        vector <TreeNode* > tmpPath;
        for(iter = paths.begin(); iter != paths.end(); ++ iter){
            tmpPath = getPath(*iter);
            retPath.insert(retPath.begin(), tmpPath.begin(), tmpPath.end());
        }
        cout<<"size of paths: "<<retPath.size()<<endl;
        return retPath;
    }
    
    vector<RangeTreeNode *> getPathRT(long long pos){
        long lowPath, highPath, midPath;
        //cout<<"read pos: "<<pos<<endl;
        lowPath = 0;
        highPath = leafNum - 1;
        midPath = (highPath + lowPath)/2;
        vector<RangeTreeNode *> retPath;
        RangeTreeNode *curNode = new RangeTreeNode(root, lowPath, highPath);
        RangeTreeNode *tmpNode;
        
        retPath.push_back(curNode);
        
        while(curNode ->high > curNode->low){
            //cout<<"("<<curNode->low<<" , "<<curNode->high<<")"<<endl;
            midPath = (curNode->high + curNode->low)/2;
            //cout<<midPath<<endl;
            //cout<<"in while"<<endl;
            if(pos <= midPath && curNode->treenode->left != NULL){//left tree
                //cout<<"left tree:   "<<"left:"<<lowPath<<"right:"<<midPath<<endl;
                tmpNode = new RangeTreeNode(curNode->treenode->left, curNode->low, midPath);
                retPath.push_back(tmpNode);
                curNode = tmpNode;
                //highPath = midPath;
            }else if(pos > midPath && curNode ->treenode ->right != NULL){
                tmpNode = new RangeTreeNode(curNode->treenode->right, midPath+1, curNode->high);
                //cout<<"right tree:"<<"left:"<<midPath+1<<"right: "<<highPath<<endl;
                retPath.push_back(tmpNode);
                curNode = tmpNode;
                //lowPath = midPath + 1;
            }
        }
        //cout<<" size is: "<<retPath.size()<<endl;
        return retPath;
    }
    
    vector<TreeNode*> getPath(long long pos){
        vector<TreeNode *> retPath;
        vector<int> bitVec;
        long long levelNum = getLevelNum();
        for(long long i = 0; i < levelNum - 1; ++i){
            bitVec.push_back(pos & 1);
            pos = pos >> 1;
        }
        
        reverse(bitVec.begin(), bitVec.end());
        cout<<endl;
        //reverse(bitVec.begin(), bitVec.end());
        retPath.push_back(root);
        TreeNode * tmpNode = root;
        for(vector<int>::iterator iter =  bitVec.begin(); iter!= bitVec.end(); ++ iter){
            if(*iter){
                
                if(tmpNode -> right != NULL){
                    //cout<<"right\n";
                    tmpNode = tmpNode -> right;
                    retPath.push_back(tmpNode);
                }
            }else{
                
                if(tmpNode -> left != NULL){
                    //cout<<"left\n";
                    tmpNode = tmpNode -> left;
                    retPath.push_back(tmpNode);
                }
            }
        }
        //cout<<"size is: "<< retPath.size()<<endl;
        return retPath;
    }
    
    
    vector<RangeTreeNode *> getPI(long long pi, long long Q){
        long long lowPath, highPath;
        vector<RangeTreeNode *> retPath;
        lowPath = pi;
        highPath = pi + Q;
        queue<RangeTreeNode *> q;
        RangeTreeNode * rt = new RangeTreeNode(root, 0, leafNum - 1);
        RangeTreeNode * tmpRt;
        q.push(rt);
        
        while(! q.empty()){
            tmpRt = q.front();
            q.pop();
            long long curLow  = tmpRt -> low;
            long long curHigh = tmpRt -> high;
            long long curMid  = (curLow + curHigh) / 2;
            //cout<<"idx range: "<<curLow<<" "<<curHigh<<endl;
            retPath.push_back(tmpRt);
            
            if(!(tmpRt->treenode->left == NULL && tmpRt->treenode->right ==NULL)){
                if(curMid >= lowPath){
                    //add left subtree
                    RangeTreeNode *leftRt = new RangeTreeNode(tmpRt->treenode->left, curLow, curMid);
                    q.push(leftRt);
                }
                if(curMid < highPath){
                    //add right subtree
                    RangeTreeNode *rightRt = new RangeTreeNode(tmpRt->treenode->right, curMid + 1, curHigh);
                    q.push(rightRt);
                }
            }
            
        }
        return retPath;
        
    }
    
    long long getLeafNum(){
        return leafNum;
    }
    
    int writeToLeaf(long long leafId, long long idx, long long pos){
        vector<TreeNode *> ret = getPath(leafId);
        reverse(ret.begin(), ret.end());
        int flag = 0;
        TreeNode *leaf = ret[0];
        for(int i = 0; i < leaf->val.size(); i+=2){
            if(leaf->val[i] == dummy){
                leaf->val[i] = idx;
                leaf->val[i+1] = pos;
                flag = 1;
                break;
            }
        }
        
        return flag;
    }
    
    void printBt(){
        TreeNode *tmpNode = root;
        queue<TreeNode *> q;
        q.push(tmpNode);
        while(! q.empty()){
            tmpNode = q.front();
            q.pop();
            for(int i = 0; i < tmpNode->val.size(); i++){
                cout<<tmpNode->val[i]<<" ";
            }
            cout<<endl;
            if(tmpNode -> left != NULL)
            q.push(tmpNode -> left);
            if(tmpNode -> right != NULL)
            q.push(tmpNode -> right);
        }
    }
    
private:
    TreeNode * root;
    long long leafNum;
    int nodeSize;
};



#endif
