#include<iostream>
#include<string>
#include<queue>
#include<stack>

using namespace std;

bool is_Appeared = false; //judge whether the unsigned char is exited
int MaxorderNum = 20000;

/*
 * define Node structure
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
int nBytes(int);
int readOneByte(string, int);

/*
 * find whether the unsigned char is exited or not
 * if existed return a pointer to that node
 * else pointer to NYT
 */

Node* findChar(unsigned char c, Node* r) 
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
 * Slide And Increment is used to update the tree's structure
 * keep swap node with nodes in its block util its order is the largest
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


/* 
 * swap node a and node b
 */ 
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
 * specific desciption is in README.txt
 * import point is path finding
 * and situations according to NYT and leaf node
 * if NYT the node is new; if leaf node just read the content of the node
 */

string decoder(Node* root, string Input, bool sFlag)
{
    Node* NN; 
    NN = root;
    int sz = Input.size();
    int count = 0;
    string c; // store each character

    string result;
    int n = 0;
    int nbyts = 0;
    int utf = 0;
    while(count!=sz)
    {   	
        if(sFlag && ((int)((unsigned char)Input[count]))==32){ //space
            count++;
    	}
	if(NN->is_NYT == true){
	    utf = readOneByte(Input,count);
	    count+=8;
	    Update((unsigned char)utf, root);
	    NN = root;

	    nbyts = nBytes(utf); // count how many bytes the character should have
            int j = 0;
            c += (unsigned char)utf;
            while(j < (nbyts-1))
            {
		if(sFlag && ((int)((unsigned char)Input[count]))==32){ //space
            	    count++;
    		}
		while(NN->left!= NULL or NN->right!= NULL){
                    n = (int)((unsigned char)Input[count])-48;
                    if(n==1){
                        NN= NN->right;
                    }else{
                        NN = NN->left;
                    }
                    count ++;
                }
	 	if(NN->is_NYT){
                    utf = readOneByte(Input,count);
                    count+=8;
                    Update((unsigned char)utf, root);
                    c += (unsigned char)utf;
                }
                else if(is_leaf(NN)){
                    c += NN->content;
                    Update(NN->content, root);
                }
 		j++;
		NN = root;
            }
	    result += c;
	    c = "";
	    NN = root;

	}else{
	    while(NN->left!= NULL or NN->right!= NULL){
                n = (int)((unsigned char)Input[count])-48;
                if(n==1){
                    NN= NN->right;
                }else{
                    NN = NN->left;
                }
                count ++;
            }
	    if(NN->is_NYT){
	    	utf = readOneByte(Input,count);
	        count+=8;
	        Update((unsigned char)utf, root);
	        NN = root;

	        nbyts = nBytes(utf); // count how many bytes the character should have
                int j = 0;
                c += (unsigned char)utf;
                while(j < (nbyts-1))
                {
		    if(sFlag && ((int)((unsigned char)Input[count]))==32){ //space
            	        count++;
    		    }
		    while(NN->left!= NULL or NN->right!= NULL){
                        n = (int)((unsigned char)Input[count])-48;
                        if(n==1){
                            NN= NN->right;
                        }else{
                            NN = NN->left;
                        }
                        count ++;
                    }
                    if(NN->is_NYT){
		    	utf = readOneByte(Input,count);
	   		count+=8;
			Update((unsigned char)utf, root);
                	c += (unsigned char)utf;	 		
                    }
		    else if(is_leaf(NN)){
                        c += NN->content;
		        Update(NN->content, root);	 	 
		    }
		    j++;
		    NN = root;
	 	   
                }
	        result += c;
	        c = "";
	        NN = root;
            }

	    else if(is_leaf(NN)){
                c += NN->content;
                Update(NN->content,root);
		utf = (int)NN->content;
                NN = root;
		
		nbyts = nBytes(utf); // count how many bytes the character should have
            	int j = 0;
     
            	while(j < (nbyts-1))
            	{
		    if(sFlag && ((int)((unsigned char)Input[count]))==32){ //space
            	        count++;
    		    }
		    while(NN->left!= NULL or NN->right!= NULL){
                        n = (int)((unsigned char)Input[count])-48;
                        if(n==1){
                            NN= NN->right;
                        }else{
                            NN = NN->left;
                    	}
                    	count ++;
                    }
		    if(NN->is_NYT){
		    	utf = readOneByte(Input,count);
	   		count+=8;
			Update((unsigned char)utf, root);
                	c += (unsigned char)utf;	 		
                    }
		    else if(is_leaf(NN)){
                        c += NN->content;
		        Update(NN->content, root);	 	 
		    }
		    j++;
		    NN = root;
                }
	    	result += c;
	    	c = "";
	    	NN = root;
            }

	}
     }
	
     return result;
}


int readOneByte(string Input, int index)
{
    int len = 0;
    int utf = 0;
    int n = 0;
    while (len < 8) {
        n = (int)((unsigned char)Input[index])-48;
        utf = utf << 1;
        utf += n;
        len++;
	index++;
    }
    return utf;
}

/*
 * count how many bytes 
 * do we need to output a valid completed character
 */
int nBytes(int n)
{
    int nBytes = 0;
    if (n > 252){  // 6bytes
        nBytes++;
    }
    if (n > 248){  // 5bytes
        nBytes++;
    }
    if (n > 240){  // 4bytes
        nBytes++;
    }
    if (n > 224){  // 3bytes
        nBytes++;
    }
    if (n > 192){  // 2bytes
        nBytes++;
    }
    if (n > 0){  // 1bytes
        nBytes++;
    }
    return nBytes;
}




/*****************************/
int main(int argc, char* argv[])
{
    string Input;
    bool sFlag;
    char model[2];
    model[0] = '-';
    model[1] = 's';
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

            string dec;
            dec = decoder(p_root,Input,sFlag);
            cout<<dec<<endl;
        }

    }

    return 0;
}


