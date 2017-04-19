#include<iostream>
#include<string>
#include<queue>
#include<stack>

using namespace std;

bool is_Appeared = false; //judge whether the char is exited
int MaxorderNum = 20000;

/*
 * define a Node structure
 */
typedef struct Node
{
    int weight;
    int order;
    unsigned char content;
    bool is_NYT;
    Node* left;
    Node* right;
    Node* parent;
} Node;


bool is_leaf(Node*);
bool is_internal(Node*);
Node* SlideAndIncrement(Node*, Node*);
Node* swapNode(Node* , Node*);
string translate(unsigned char);
string findPath(Node*);
queue<Node*> blockQ;
stack<Node*> blockSt;
string Merge(string, int);

/*
 * check an unsigned char whether existed in the tree
 * if existed return a pointer to that node
 * else point to the NYT
 */
Node* findChar(unsigned char c, Node* r) //find whether the char is existed or not
{
    queue<Node*> Q;
    Q.push(r);
    Node* temp = new Node;
    Node* NYT_temp = new Node;
    while(!Q.empty())
    {
        temp = Q.front();
        Q.pop();
        if (temp->is_NYT == true) {
            NYT_temp = temp;
        }else if (temp->content ==c){
            is_Appeared = true;
            return temp;
        }else{
            if (temp->left)
                Q.push(temp->left);
            if(temp->right)
                Q.push(temp->right);
        }
    }
    is_Appeared = false;
    return NYT_temp;
}

/*
 * define a Node's block
 * leaf node's block is all the internal nodes whose weight is as same as the leaf node
 * internal node's block is all the leaf nodes whose weight equals to the internal's +1
 */
void Block(Node* root, Node* f)
{
    Node* temp;
    temp = root;

    if(is_leaf(f)){
        queue<Node*> Q;
        Q.push(temp);
        while(!Q.empty()){
            temp = Q.front();
            Q.pop();
            if(!is_leaf(temp) && temp->weight == f->weight && f->parent!=temp){
                blockSt.push(temp);
                blockQ.push(temp);
            }else{
                if (temp->right)
                    Q.push(temp->right);
                if(temp->left)
                    Q.push(temp->left);
            }
        }
    }else{
        queue<Node*> Q;
        Q.push(temp);
        while(!Q.empty()){
            temp = Q.front();
            Q.pop();
            if(is_leaf(temp) && temp->weight == f->weight+1){
                blockSt.push(temp);
                blockQ.push(temp);
            }else{
                if (temp->right)
                    Q.push(temp->right);
                if(temp->left)
                    Q.push(temp->left);
            }
        }
    }
}

/*
 * find a leaf whose weight is equal to f's and order is the max
 */
Node* findMaxleaf(Node* root, Node* f)
{
    int wt = f->weight;
    int ord = f->order;
    Node* temp;
    Node* max=f;
    queue<Node*> Q;
    Q.push(root);
    while(!Q.empty()){
        temp = Q.front();
        Q.pop();
        if(is_leaf(temp) && temp->weight == wt){
            if(temp->order>ord){
                max = temp;
                ord = max->order;
            }
        }else{
            if (temp->right)
                Q.push(temp->right);
            if(temp->left)
                Q.push(temp->left);
        }

    }
    return max;
}



/*
 * New is new external Node;
 * NNYT is new NYT;
 * NNode is old NYT;
 */
void Update(unsigned char c, Node* root){
    Node* leafToIncrement = NULL;
    Node* NNode = findChar(c, root); // leaf node

    if(is_Appeared == false)
    {
        Node* New = new Node;
        New->is_NYT = false;
        New->weight = 0;
        New->content = c;
        New->right = NULL;
        New->left = NULL;
        New->parent = NNode;
        New->order = New->parent->order - 1;
        NNode->right = New;
        NNode->is_NYT = false;
        Node* NNYT = new Node;
        NNYT->content = '\0';
        NNode->left = NNYT;
        NNYT->parent = NNode;
        NNYT->is_NYT = true;
        NNYT->left = NULL;
        NNYT->right = NULL;
        NNYT->weight = 0;
        NNYT->order = New->parent->order - 2;

        leafToIncrement = New;
    }

    else
    {
        Node* leader;
        leader = findMaxleaf(root, NNode);
        Node* pt;
        pt = swapNode(NNode, leader);
        NNode = pt;
        if(NNode->parent->left->is_NYT)
        {
            leafToIncrement = NNode;
            NNode = NNode->parent;
        }
    }

    while (NNode->order != MaxorderNum)
    {
        NNode = SlideAndIncrement(root, NNode);
    }

    if (leafToIncrement)
    {
        SlideAndIncrement(root, leafToIncrement);
    }
    NNode->weight+=1;
}


/*
 * this function is used to modified the tree structure
 * if the node's block not empty, 
 * keep swapping util the node has the largest order.
 */
Node* SlideAndIncrement(Node* root, Node* p)
{
    Node* oldparent;
    oldparent = p->parent;
    bool flag = is_leaf(p);
    Block(root,p);
    if((is_leaf(p) && !blockSt.empty()) or (is_internal(p) && !blockSt.empty())){

        while(!blockSt.empty())
        {
            Node* temp;
            temp = blockSt.top();
            blockSt.pop();
            Node* pt;
            pt = swapNode(p, temp);
            p = pt;
        }
        p->weight += 1;
        if(flag){p = p->parent;}else{p = oldparent;}
    }
    else
    {
        p->weight += 1;
        p = p->parent;

    }
    return p;
}



Node* swapNode(Node* a, Node* b) //b is a node in a's block
{
    Node* temp1 = a->parent;
    Node* temp2 = b->parent;
    int ord1 = a->order;
    int ord2 = b->order;
    a->parent = temp2;
    b->parent = temp1;
    if(ord1%2 == 1){
        temp1->right = b;
    }else{
        temp1->left = b;
    }
    if(ord2%2 == 1){
        temp2->right = a;
    }else{
        temp2->left = a;
    }
    int temp;
    temp = a->order;
    a->order = b->order;
    b->order = temp;
    return a;
}


bool is_leaf(Node* f)
{
    bool leaf;
    if(f->order != MaxorderNum && (f->left == NULL || f->right==NULL)){
        leaf=true;
    }else{leaf = false;}
    return leaf;
}

bool is_internal(Node* f)
{
    bool internal;
    if(f->order != MaxorderNum && (f->left != NULL || f->right != NULL))
        internal=true;
    else
        internal=false;
    return internal;
}

/*******************************/
/*
 *  read each byte and updating the tree
 */

string encoder(Node* root, string Input, bool sFlag)
{
	string result="";
	//string s_result;
	Node* current;
	int sz = Input.size();
	int i = 0;
	unsigned char C;
	while(i<sz){
		
		C = (unsigned char)Input[i];
		current = findChar(C, root);
		if(!is_Appeared){
            		if(current->order==MaxorderNum){
                    		result += translate(C);
	   	    		if(sFlag && i!=sz-1){
					result += " ";
		    		}
           	 	}else{
                		result += findPath(current);
                		result += translate(C);
  				if(sFlag && i!=sz-1){
					result += " ";
				}
            		}
        	
        	}else{
            		result += findPath(current);
	    		if(sFlag && i!=sz-1){
				result += " ";
	    		}
        	}
        	Update(C, root);
		i++;
        }
	return result;
}

/*
 * find the path of a node
 */
string findPath(Node* f)
{
    Node* temp;
    temp = f;
    string result;
    stack<string> st;
    while(temp->order != MaxorderNum)
    {
        if(temp==temp->parent->left){
            st.push("0");
        }
        if(temp==temp->parent->right){
            st.push("1");
        }
        temp = temp->parent;
    }
    while(!st.empty())
    {
        result += st.top();
        st.pop();
    }
    return  result;
}

/*
 * translate an unsigned char to a digital string
 */
string translate(unsigned char c)
{
    string result;
    int sz = 8;
    int a = (int)c;
    stack<string> st;
    string zero = "0";
    string one = "1";
    while (a!=0){
        int i;
        i = a%2;
        if(i==1){
            st.push(one);
        }else{
            st.push(zero);
        }
        a = a/2;
    }
    int z = sz - st.size();
    while(z!=0){
        result += "0";
        z--;
    }

    while(!st.empty()){
        result += st.top();
        st.pop();
    }
    return result;
}



/*******************************/
int main(int argc, char* argv[])
{

    string Input;
    bool sFlag;
    char model[2];
    model[0]='-';
    model[1]='s';
    if(argc==2 && argv[1][0]==model[0] && argv[1][1]==model[1]){
        sFlag = true;
    }else{
        sFlag = false;
    }
    while(getline(cin,Input)){

        if(Input=="^D"){
            break;
        }else{
            Node root;
            Node* p_root = &root;
            p_root->weight=0, p_root->content='\0', p_root->order=20000, p_root->left=NULL, p_root->right=NULL, p_root->parent=NULL, p_root->is_NYT = true;

            string enc;
            enc = encoder(p_root,Input,sFlag);
            
            cout<<enc<<endl;
        }

    }


    return 0;

}


