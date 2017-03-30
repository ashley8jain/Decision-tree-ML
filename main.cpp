#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <stdlib.h>
#include <time.h>
#include <math.h>
using namespace std;

#define y_j 54


class NodeTree{

public:
    bool attr_leaf;
    int name;
    int cont_disc;float mid;
    NodeTree *left,*right;
    vector<vector<int> >* exmps;
    int pos,neg;
    NodeTree(int leaf){
        cout<<"leaf: "<<leaf<<endl;
        attr_leaf=false;
        name = leaf;
        left = right = NULL;
    }
    NodeTree(int attr,int cont_disc){
        name = attr;
        this->cont_disc = cont_disc;
        attr_leaf=true;
    }
    
};


int bestAttribute(vector<vector<int> > examples){
    
    srand(time(NULL));
    return (rand()%54);
}

pair<bool,int> same_examples(vector<vector<int> > exs){
    
    int y=exs[0][y_j];
    bool same  = true;
    for(int i=1;i<exs.size();i++){
        if(y!=exs[i][y_j]){
            same = false;
            break;
        }
    }
    pair<bool,int> ans(same,y);
    return ans;

}

float calc_mid(vector<vector<int> > exps,int attr){
    
    float mid = 0;
    for(int i=0;i<exps.size();i++){
        mid+=exps[i][attr];
    }
    return (mid/exps.size());
    
}

int most_occ_class(vector<vector<int> > exms){
    
    int arr[8]={0};
    for(int i=0;i<exms.size();i++){
        arr[exms[i][y_j]]++;
    }
    int max_i = 0;
    int max =0;
    for(int i=1;i<8;i++){
        if(max<arr[i]){
            max = arr[i];
            max_i=i;
        }
    }
    return max_i;
    
}

/*
 ID3 (Examples, Target_Attribute, Attributes)
 Create a root node for the tree
 If all examples are positive, Return the single-node tree Root, with label = +.
 If all examples are negative, Return the single-node tree Root, with label = -.
 If number of predicting attributes is empty, then Return the single node tree Root,
 with label = most common value of the target attribute in the examples.
 Otherwise Begin
 A ← The Attribute that best classifies examples.
 Decision Tree attribute for Root = A.
 For each possible value, vi, of A,
 Add a new tree branch below Root, corresponding to the test A = vi.
 Let Examples(vi) be the subset of examples that have the value vi for A
 If Examples(vi) is empty
 Then below this new branch add a leaf node with label = most common target value in the examples
 Else below this new branch add the subtree ID3 (Examples(vi), Target_Attribute, Attributes – {A})
 End
 Return Root
 */
NodeTree* buildTree(vector<vector<int> > examples){
    
    pair<bool,int> same_exps = same_examples(examples);
    if(same_exps.first){
        cout<<"here"<<endl;
        NodeTree* leaf = new NodeTree(same_exps.second);
        return leaf;
    }
    
    int attr = bestAttribute(examples);
    NodeTree* root = new NodeTree(attr,attr<10?1:0);
    root->exmps = new vector<vector<int> >(examples);
    
    vector<vector<int> > left_ex,right_ex;
    if(root->cont_disc){
        root->mid = calc_mid(examples,attr);
        for(int i=0;i<examples.size();i++){
            if(examples[i][attr]<root->mid){
                right_ex.push_back(examples[i]);
            }
            else{
                left_ex.push_back(examples[i]);
            }
        }
    }
    else{
        for(int i=0;i<examples.size();i++){
            if(examples[i][attr]){
                right_ex.push_back(examples[i]);
            }
            else{
                left_ex.push_back(examples[i]);
            }
        }
    }
    
    cout<<right_ex.size()<<endl;
    cout<<left_ex.size()<<endl;
    cout<<endl<<endl;
    if(left_ex.size()==0){
        return new NodeTree(most_occ_class(right_ex));
    }
    else if(right_ex.size()==0){
        return new NodeTree(most_occ_class(left_ex));
    }
    else{
        root->left = buildTree(left_ex);
        root->right = buildTree(right_ex);
    }
    
    return root;
    
}



//split into vetctor by delimiter
vector<int> split(const string &s, char delim) {
    vector<int> elems;
    stringstream ss(s);
    string number;
    while(getline(ss, number, delim)) {
        elems.push_back(stoi(number));
    }
    return elems;
}



int main(int argc, const char * argv[]) {
    
    vector<string> rows;
    
    //loading training datas
    ifstream infile("train.dat");char data[1600];
    if (!infile)
        cout<<"cannot open file: train.dat";
    infile >> data;
    
    while(infile){
        infile >> data;
        rows.push_back(data);
    }
    rows.pop_back();
    
    vector<vector<int> > X_train;
    vector<int> y_train,X_y_i;
    for(int i=0;i<rows.size();i++){
//        X_y_i = split(rows[i], ',');
//        y_train.push_back(X_y_i[X_y_i.size()-1]);
//        X_y_i.pop_back();
//        X_train.push_back(X_y_i);
        X_train.push_back(split(rows[i], ','));
    }
    NodeTree* dec_tree = buildTree(X_train);
    
    return 0;
}
