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

pair<float,float> MI(vector<vector<int> > exms,int attr){
    
    float mid = 0;
    float mi;  int C[8]={0};
    
    if(attr<10){
        for(int i=0;i<exms.size();i++){
            mid+=exms[i][attr];
            C[exms[i][y_j]]++;
        }
        mid=mid/exms.size();
    }
    else{
        for(int i=0;i<exms.size();i++){
            C[exms[i][y_j]]++;
        }
        
    }
    
    float first = 0;
    for(int i=1;i<8;i++){
        C[i]/=exms.size();
        first+=(-(C[i]*log(C[i])));
    }
    float second = 0;
    vector<vector<int> > ft,sd;
    
    if(attr<10){
        for(int i=0;i<exms.size();i++){
            if(exms[i][attr]<mid){
                ft.push_back(exms[i]);
            }
            else{
                sd.push_back(exms[i]);
            }
        }
    }
    else{
        for(int i=0;i<exms.size();i++){
            if(exms[i][attr]){
                ft.push_back(exms[i]);
            }
            else{
                sd.push_back(exms[i]);
            }
        }
    }
    
    int Cft[8]={0};
    int Csd[8]={0};
    float a,b;
    a=b=0;
    for(int i=0;i<ft.size();i++){
        Cft[ft[i][y_j]]++;
    }
    for(int i=1;i<8;i++){
        Cft[i]/=ft.size();
        a+=(-(Cft[i]*log(Cft[i])));
    }
    
    for(int i=0;i<sd.size();i++){
        Csd[sd[i][y_j]]++;
    }
    for(int i=1;i<8;i++){
        Csd[i]/=sd.size();
        b+=(-(Csd[i]*log(Csd[i])));
    }
    second = (ft.size()*a+sd.size()*b)/exms.size();
    
    mi = first-second;
    
    return make_pair(mi, mid);
    
}

pair<int,float> bestAttribute(vector<vector<int> > examples,int parent_attr){
    
    float max_MI = -1;
    float max_mid = -1;
    int max_MI_attr = -1;
    pair<float,float> mi;
    for(int i=0;i<54;i++){
        if(i!=parent_attr){
            mi = MI(examples,i) ;
            if(max_MI < mi.first){
                max_MI_attr = i ;
                max_MI = mi.first;
                max_mid = mi.second;
            }
        }
    }
    return make_pair(max_MI_attr, max_mid);
    
    
    //    srand(time(NULL));
    //    return (rand()%54);
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

//build decision tree by classifying training datas
NodeTree* buildTree(vector<vector<int> > examples,int parent_attr){
    
    pair<bool,int> same_exps = same_examples(examples);
    if(same_exps.first){
        cout<<"here"<<endl;
        NodeTree* leaf = new NodeTree(same_exps.second);
        return leaf;
    }
    
    pair<int,float> attr_pair = bestAttribute(examples,parent_attr);
    NodeTree* root = new NodeTree(attr_pair.first,attr_pair.first<10?1:0);
    root->exmps = new vector<vector<int> >(examples);
    
    vector<vector<int> > left_ex,right_ex;
    if(root->cont_disc){
        root->mid = attr_pair.second;
        for(int i=0;i<examples.size();i++){
            if(examples[i][attr_pair.first]<root->mid){
                right_ex.push_back(examples[i]);
            }
            else{
                left_ex.push_back(examples[i]);
            }
        }
    }
    else{
        for(int i=0;i<examples.size();i++){
            if(examples[i][attr_pair.first]){
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
        root->left = buildTree(left_ex,attr_pair.first);
        root->right = buildTree(right_ex,attr_pair.first);
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
    NodeTree* dec_tree = buildTree(X_train,-1);
    
    return 0;
}
