#include<stdio.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<fstream>

struct intt
{
	 int xc;
	int yc;
};
struct Node
{
	intt* data;
	Node *next;
};
struct intt* g1=new intt;

struct tmember
{
	Node *kphead;
	Node *cphead;
	Node *kptail;
	Node *cptail;
	tmember *left;
	tmember *right;
	tmember *middle;
	intt* privatekey;
	intt* publickey;	
	int id;
	tmember *parent;
};

using namespace std;


int l1count=1;
int x1=0;
int lcount=1;
int Id=0;
int x;
int Id2=Id;
int ldepth=10000;
int g=7;
int p=31;
int dh=0;
int uc=0;
int bc=0;
int rn=0;
tmember* insertion;
tmember* jsponsor;
tmember* lsponsor3;
int ip=0;
tmember *renewed[10000];
int nre=0;
int ldepthid=-1;
int Idbackup=0;
void LeafNodescp(tmember* head,tmember* receiver)
{
      if(head == NULL)       

        return;
       if(head->left == NULL && head->right==NULL && head->middle==NULL)      
        {
		head->cptail->data=receiver->publickey;
		struct Node* temp1=new Node;
		head->cptail->next=temp1;
		head->cptail=head->cptail->next;
	}
       LeafNodescp(head->left,receiver);
       LeafNodescp(head->middle,receiver); 
       LeafNodescp(head->right,receiver);      
}

void LeafNodeskp(tmember* head,tmember* parent)
{
	if(head == NULL||parent==NULL)       
        return;
       if(head->left == NULL && head->right==NULL && head->middle==NULL)      
        {
		head->kptail->data=parent->privatekey;
		struct Node* temp1=new Node;
		head->kptail->next=temp1;
		head->kptail=head->kptail->next;
		return;
	}
	
     LeafNodeskp(head->left,parent);
      LeafNodeskp(head->middle,parent);
      LeafNodeskp(head->right,parent);  
}

intt* point_add(intt* temp1,intt* temp2)
{
	intt* temp3=new intt;
	float xp=temp1->xc;
	float yp=temp1->yc;
	float xq=temp2->xc;
	float yq=temp2->yc;
	float l=((yq-yp)/(xq-xp));
	if(l<0)
	l=l*(-1);
	temp3->xc=(l*l)-xp-xq;
	temp3->yc=(l*(xp-(temp3->xc)))-yp;
	if(temp3->xc<0)
	temp3->xc=(temp3->xc)*(-1);
	if(temp3->yc<0)
	temp3->yc=(temp3->yc)*(-1);
	return temp3;
}

intt* point_double(intt* temp1)
{
	intt* temp3=new intt;
	float xp=temp1->xc;
	float yp=temp1->yc;

	float l=(((3*xp*xp)+(g1->xc))/(2*yp));
	if(l<0)
	l=l*(-1);
	temp3->xc=(l*l)-xp-xp;
	temp3->yc=(l*(xp-(temp3->xc)))-yp;
	if(temp3->xc<0)
	temp3->xc=(temp3->xc)*(-1);
	if(temp3->yc<0)
	temp3->yc=(temp3->yc)*(-1);
	return temp3;
}

intt* calpublic(intt* temp, int privatekey1)
{
	if(privatekey1==0)
	{
	struct intt* temp1=new intt;
	temp1->xc=0;
	temp1->yc=0;
	return temp1;
	}
	else if(privatekey1==1)
	{
	return temp;
	}
	else if(privatekey1%2==1)
	return point_add(temp,calpublic(temp,privatekey1-1));
	else if(privatekey1%2==0)
	return calpublic(point_double(temp) ,(privatekey1)/2);
	
}

intt* ecdh(intt* temp,int p,intt* privatekey1) //temp^privatekey1->x % p;
{
	dh++;
	struct intt* temppublic= new intt;
	struct intt* temppublic1=new intt;
	temppublic1->xc=temp->xc;
	temppublic1->yc=temp->yc;
	if(privatekey1->xc==0)
	privatekey1->xc=privatekey1->xc+ (rand()%5)+3;
	temppublic=calpublic(temppublic1,privatekey1->xc);
	temppublic->xc=(temppublic->xc) % p;
	temppublic->yc=(temppublic->yc) % p;
	if(temppublic->xc<0)
	temppublic->xc=(temppublic->xc)*(-1);
	if(temppublic->yc<0)
	temppublic->yc=(temppublic->yc)*(-1);
	return temppublic;
	
}
void Unicast(tmember* sender, tmember* receiver)
{
	if(sender==NULL||receiver==NULL)
	return;
	else if(sender->parent->left==sender)
	{
		
		sender->parent->privatekey=ecdh(sender->publickey,p,receiver->privatekey);
		sender->cptail->data=receiver->publickey;
		receiver->cptail->data=sender->publickey;
		struct Node* temp1=new Node;
		sender->cptail->next=temp1;
		sender->cptail=sender->cptail->next;
		struct Node* temp2=new Node;
		receiver->cptail->next=temp2;
		receiver->cptail=receiver->cptail->next;	
	}
	else 
	{
		sender->parent->privatekey=ecdh(sender->parent->privatekey,p,receiver->privatekey);
		//intt * temp;
		//cout<<"JJJJJJJJJJJJJJJ"<<sender->parent->privatekey->xc;
		//temp=ecdh(g1,p,sender->parent->privatekey);
		sender->parent->publickey=ecdh(g1,p,sender->parent->privatekey);
		receiver->kptail->data=receiver->parent->privatekey;
		struct Node* temp3=new Node;
		receiver->kptail->next=temp3;
		receiver->kptail=receiver->kptail->next;
	}
	uc++;
}

void BroadcastLeaf(tmember* sender)
{
	if(sender==NULL)
	return;
	tmember* lef=sender->parent->left;
	tmember* mid=sender->parent->middle;
	
	lef->kptail->data=sender->parent->privatekey;
	mid->kptail->data=sender->parent->privatekey;
	struct Node* temp1=new Node;
	lef->kptail->next=temp1;
	lef->kptail=lef->kptail->next;
	struct Node* temp2=new Node;
	mid->kptail->next=temp2;
	mid->kptail=mid->kptail->next;

	lef->cptail->data=sender->publickey;
	mid->cptail->data=sender->publickey;
	struct Node* temp3=new Node;
	lef->cptail->next=temp3;
	lef->cptail=lef->cptail->next;
	struct Node* temp4=new Node;
	mid->cptail->next=temp4;
	mid->cptail=mid->cptail->next;

	sender->cptail->data=lef->publickey;
	struct Node* temp5=new Node;
	sender->cptail->next=temp5;
	sender->cptail=sender->cptail->next;
	sender->cptail->data=mid->publickey;
	struct Node* temp6=new Node;
	sender->cptail->next=temp6;
	sender->cptail=sender->cptail->next;
	bc++;
	
}


void BroadcastNleaf11(tmember* sender,tmember* receiver)
{	
	if(receiver->left==NULL&&receiver->middle==NULL&&receiver->right==NULL)
	{
	//cout<<"\nuc bc "<<uc <<"  "<<bc;
	if(sender->left!=NULL&&sender->right!=NULL&&sender->middle!=NULL)
	uc++;
	bc--;
	}
	//if(receiver->privatekey!=NULL)
	//cout<<"JJJJJJJJJJJJJJJJJJ"<<receiver->privatekey->xc;
	if(sender==NULL||receiver==NULL)
	return;
	else if(sender->parent->left==sender)
	{
		sender->parent->privatekey=ecdh(g1,p,sender->privatekey);
		sender->parent->privatekey=ecdh(sender->parent->privatekey,p,receiver->privatekey);
		sender->parent->publickey=ecdh(g1,p,sender->parent->privatekey);	
		LeafNodescp(sender,receiver);
		LeafNodescp(receiver,sender);
	}
	else 
	{
		sender->parent->privatekey=ecdh(sender->parent->privatekey,p,receiver->privatekey);
		sender->parent->publickey=ecdh(g1,p,sender->parent->privatekey);	
	}
	bc++;
	
}

void leafnodes1(tmember *sender)
{
	if(sender==NULL)
	return;
	if(sender->left==NULL&&sender->middle==NULL&&sender->right==NULL)
	{
	dh++;
	}
	leafnodes1(sender->left);
	leafnodes1(sender->middle);
	leafnodes1(sender->right);
}


void BroadcastNleaf1(tmember* sender,tmember* receiver)
{	
	if(receiver->left==NULL&&receiver->middle==NULL&&receiver->right==NULL)
	{
	//cout<<"\nuc bc "<<uc <<"  "<<bc;
	if(sender->left!=NULL&&sender->right!=NULL&&sender->middle!=NULL)
	uc++;
	bc--;
	}
	
	leafnodes1(receiver);
	if(sender==NULL||receiver==NULL)
	return;
	else if(sender->parent->left==sender)
	{
		sender->parent->privatekey=ecdh(g1,p,sender->privatekey);
		sender->parent->privatekey=ecdh(sender->parent->privatekey,p,receiver->privatekey);
		LeafNodescp(sender,receiver);
		LeafNodescp(receiver,sender);
	}
	else 
	{
		sender->parent->privatekey=ecdh(sender->parent->privatekey,p,receiver->privatekey);
		sender->parent->publickey=ecdh(g1,p,sender->parent->privatekey);	
	}
	bc++;
	
}

void BroadcastNleaf2(tmember* sender)
{
	if(sender==NULL)
	return;
	
	leafnodes1(sender->parent->left);
	leafnodes1(sender->parent->middle);

	if(sender->left==NULL&&sender->middle==NULL&&sender->right==NULL)
	bc--;
	tmember* lef=sender->parent->left;
	tmember* mid=sender->parent->middle;

	LeafNodeskp(sender,sender->parent);
	
	
	LeafNodescp(lef,sender);
	LeafNodescp(mid,sender);

	LeafNodescp(sender,lef);
	LeafNodescp(sender,mid);

	LeafNodeskp(lef,sender->parent);
	LeafNodeskp(mid,sender->parent);

	bc++;
	
}

int max(int a, int b, int c)
{
	if(a>=b&&a>=c)
	return a;
	else if(b>=a&&b>=c)
	return b;
	else if(c>=a&&c>=b)
	return c;
}



tmember* rightmostparent(tmember *head)
{
	tmember *temp=head;
	if(temp->right!=NULL)
	return rightmostparent(temp->right);
	else if(temp->middle!=NULL)
	return rightmostparent(temp->middle);
	else if(temp->left!=NULL)
	return rightmostparent(temp->left);
	else
	return head;
	
}

int heightofmember(tmember *head)
{
	
   if (head==NULL) 
       return -1;
   else
   {
       
	int lDepth = heightofmember(head->left);
	int rDepth = heightofmember(head->right);
	int mDepth = heightofmember(head->middle);
	return max(lDepth,rDepth,mDepth)+1;
   }
}


void printGivenLevel(struct tmember* root, int level)
{
	
    if (root == NULL)
        return;
    if (level == 0&&root->parent->middle!=NULL&&root->parent->right!=NULL)
        {	
		tmember* temp=root;
		if(l1count%3==1)
		{
		
		BroadcastNleaf1(temp,temp->parent->middle);
		}
		else if(l1count%3==2)
		{
		BroadcastNleaf1(temp->parent->middle,temp->parent->right);
		}		
		else if(l1count%3==0)
		{
		BroadcastNleaf2(temp->parent->right);
		//cout<<"\nBroadcasts now "  <<bc;
		}
		l1count++;
		return;
	}
	else if(level==0 &&root->parent->right==NULL&&root->parent->left==root&&root->parent->middle!=NULL&&root->left!=NULL&&root->right!=NULL&&root->middle!=NULL)
	{	tmember* temp=root;
			BroadcastNleaf1(temp,temp->parent->middle);
			temp->parent->privatekey=ecdh(temp->parent->left->publickey,p,temp->parent->middle->privatekey);
			temp->parent->publickey=ecdh(g1,p,temp->parent->privatekey);		bc++;
			dh--;
			//cout<<"\n\n\nI am called "<<root->privatekey<<" "<<root->parent->middle->id;
			LeafNodeskp(temp,temp->parent);
			LeafNodeskp(temp->parent->left,temp->parent);
			return;
		
	}
    else if (level>=1)
    {
        printGivenLevel(root->left, level-1);
	printGivenLevel(root->middle, level-1);
        printGivenLevel(root->right, level-1);
    }
}

void printGivenLevel1(struct tmember* root, int level)
{
	
    if (root == NULL)
        return;
    if (level == 0&&root->parent->middle!=NULL&&root->parent->right!=NULL)
        {	
		tmember* temp=root;
		if(l1count%3==1)
		{
		
		BroadcastNleaf11(temp,temp->parent->middle);
		}
		else if(l1count%3==2)
		{
		BroadcastNleaf1(temp->parent->middle,temp->parent->right);
		}		
		else if(l1count%3==0)
		{
		BroadcastNleaf2(temp->parent->right);
		//cout<<"\nBroadcasts now "  <<bc;
		}
		l1count++;
		return;
	}
	else if(level==0 &&root->parent->right==NULL&&root->parent->left==root&&root->parent->middle!=NULL&&root->left!=NULL&&root->right!=NULL&&root->middle!=NULL)
	{	tmember* temp=root;
			BroadcastNleaf1(temp,temp->parent->middle);
			temp->parent->privatekey=ecdh(temp->parent->left->publickey,p,temp->parent->middle->privatekey);
			temp->parent->publickey=ecdh(g1,p,temp->parent->privatekey);		bc++;
			dh--;
			//cout<<"\n\n\nI am called "<<root->privatekey<<" "<<root->parent->middle->id;
			LeafNodeskp(temp,temp->parent);
			LeafNodeskp(temp->parent->left,temp->parent);
			return;
		
	}
    else if (level>=1)
    {
        printGivenLevel1(root->left, level-1);
	printGivenLevel1(root->middle, level-1);
        printGivenLevel1(root->right, level-1);
    }
}

void groupkeynleafs(struct tmember* root)
{
    int h = heightofmember(root);
    int i;
	Id2=Id;
    for (i=h-1; i>=1; i--)
	{
	l1count=1;
	int x=i;
	//cout<<"\n x is "<<x;
       printGivenLevel(root, i);
	}
}

void groupkeynleafs1(struct tmember* root)
{
    int h = heightofmember(root);
    int i;
	Id2=Id;
    for (i=h-1; i>=1; i--)
	{
	l1count=1;
	int x=i;
	//cout<<"\n x is "<<x;
       printGivenLevel1(root, i);
	}
}


void groupkeyleafs(tmember* head)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL && head->middle==NULL&&lcount<=(Id-(Id%3)))      
        {
		if(lcount%3==1)
		Unicast(head,head->parent->middle);
		else if(lcount%3==2)
		Unicast(head,head->parent->right);
		else if(lcount%3==0);
		BroadcastLeaf(head);
		lcount++;
	}
       groupkeyleafs(head->left);
       groupkeyleafs(head->middle); 
       groupkeyleafs(head->right);  
}

void groupkeycalc1(tmember* head)
{
	struct tmember* temp=head;
	groupkeyleafs(head);
	//cout<<"\nUnicasts now "<<uc;
	if(Id%3==2)
	{
	tmember* temp=rightmostparent(head);
	Unicast(temp->parent->left,temp);
	uc++;
	temp->parent->privatekey=ecdh(temp->parent->left->publickey,p,temp->privatekey);
	temp->parent->publickey=ecdh(g1,p,temp->parent->privatekey);
	
	temp->kptail->data=temp->parent->privatekey;
	temp->parent->left->kptail->data=temp->parent->privatekey;
	struct Node* temp1=new Node;
	temp->kptail->next=temp1;
	temp->kptail=temp->kptail->next;
	struct Node* temp2=new Node;
	temp->parent->left->kptail->next=temp2;
	temp->parent->left->kptail=temp->parent->left->kptail->next;
	}
	//cout<<"\nUnicasts now "<<uc;
	groupkeynleafs1(head);
}



void groupkeycalc(tmember* head)
{
	struct tmember* temp=head;
	groupkeyleafs(head);
	//cout<<"\nUnicasts now "<<uc;
	if(Id%3==2)
	{
	tmember* temp=rightmostparent(head);
	Unicast(temp->parent->left,temp);
	uc++;
	temp->parent->privatekey=ecdh(temp->parent->left->publickey,p,temp->privatekey);
	temp->parent->publickey=ecdh(g1,p,temp->parent->privatekey);
	
	temp->kptail->data=temp->parent->privatekey;
	temp->parent->left->kptail->data=temp->parent->privatekey;
	struct Node* temp1=new Node;
	temp->kptail->next=temp1;
	temp->kptail=temp->kptail->next;
	struct Node* temp2=new Node;
	temp->parent->left->kptail->next=temp2;
	temp->parent->left->kptail=temp->parent->left->kptail->next;
	}
	//cout<<"\nUnicasts now "<<uc;
	groupkeynleafs(head);
}

int fullternarytree()
{
	int k=Id-1;
	if(k==1)
	return 0;
	while(k>0&&k!=1)
	{
	int h=k%3;
	k=k/3;
	if(h!=0)
	return 0;
	}
	return 1;
}



tmember* join(tmember *head,int no_of_new_members)
{
	
	for(int i=0;i<no_of_new_members;i++)
	{ // cout<<"\n \n Member: "<<i+1;
		struct tmember *temp=new tmember;
		temp->parent=NULL;
		temp->kphead=new Node;
		temp->kptail=temp->kphead;
		temp->left=NULL;
		temp->right=NULL;
		temp->middle=NULL;
		temp->cphead=new Node;
		temp->cptail=temp->cphead;
		int flag=rand()%5;
		temp->privatekey=new intt;
		temp->privatekey->xc=flag;
		temp->privatekey->yc=0;
		temp->publickey=new intt;
		temp->publickey=ecdh(g1,p,temp->privatekey);
		//cout<<"\nPrivate key :"<<temp->privatekey;
		//cout<<"\nPublickey :"<<temp->publickey;
		Id++;
		temp->id=Id+Idbackup;	
		jsponsor=temp;
		if(Id==1) // if no member exists
		{
			///cout<<"\nFirst elemenet";
			
			head->left=temp;
			head->right=NULL;
			head->middle=NULL;
			head->parent=NULL;
			head->id=0;
			head->kphead=NULL;
			head->cphead=NULL;	
			head->kptail=NULL;
			head->cptail=NULL;
			temp->parent=head;
			head->privatekey=temp->privatekey;
			head->publickey=temp->publickey;
		}
		else
		{	
			int check=fullternarytree();
			if(check==1) //check whether it is fully completed tree or not
			{	
				///cout<<"\nfully ternary\n";
				struct tmember* temp2=new tmember;
				temp2->left=head;
				temp2->middle=temp;
				temp2->right=NULL;
				temp2->parent=NULL;
				temp2->kphead=NULL;
				temp2->cphead=NULL;
				temp2->kptail=NULL;
				temp2->cptail=NULL;
				temp2->id=0;	
				temp->parent=temp2;
				head->parent=temp2;
				head=temp2;
				temp2->privatekey=new intt;
				temp2->publickey=new intt;
				temp2->privatekey->xc=0;
				temp2->privatekey->yc=0;
				temp2->publickey->xc=0;
				temp2->publickey->yc=0;			
			}
			
			else
			{
				struct tmember* parent1=rightmostparent(head);
					///cout<<"\nId is "<<parent1->id<<"\n";
					
					while(1)
					{ 	
						
						int hl=heightofmember(parent1->left);
						int hm=heightofmember(parent1->middle);
						int hr=heightofmember(parent1->right);					
						if(hr==-1&&hl==-1&&hm==-1)//if it is a leaf
						{
						parent1=parent1->parent;
						///cout<<"\n going to it's parent\n"; 
						}
						else if(hr==hl&&hl==hm)//if left=right=middle
						{
							///cout<<"\n going to it's parent\n"; 
							parent1=parent1->parent;
						}
						else if(hr==-1&&hm==-1&&hl!=-1)//right is null, middle is null, left is not null
						{
							parent1->middle=temp;
							temp->parent=parent1;
							///cout<<"\nInsertedd in parent's middle where middle was null";
							break;
						}						
						else if(hr==-1&&hm<hl&&hm!=-1)//right is null,middle not null,middle less than left 
						{
							struct tmember *temp3=new tmember;
							temp3->left=parent1->middle;
							parent1->middle->parent=temp3;
							temp3->middle=temp;
							temp->parent=temp3;
							parent1->middle=temp3;
							temp3->kphead=NULL;
							temp3->cphead=NULL;
							temp3->kptail=NULL;
							temp3->cptail=NULL;
							temp3->right=NULL;
							temp3->parent=parent1;
							temp3->id=0;
							temp3->privatekey=new intt;
							temp3->publickey=new intt;
							temp3->privatekey->xc=0;
							temp3->privatekey->yc=0;
							temp3->publickey->xc=0;
							temp3->publickey->yc=0;
							///cout<<"Inserted in parent's middle where middle is not null\n";
							break;
						}
						else if(hr==-1&&hl==hm)//if right is null,but left=middle
						{
							parent1->right=temp;
							temp->parent=parent1;
							///cout<<"\nInsertedd in parent's right where right was null";
							break;
						}
						else if(hl==hm&&hr<hl)//if right is less than the other,and left=middle
						{
							struct tmember *temp3=new tmember;
							temp3->left=parent1->right;
							parent1->right->parent=temp3;
							temp3->middle=temp;
							temp->parent=temp3;
							parent1->right=temp3;
							temp3->kphead=NULL;
							temp3->cphead=NULL;
							temp3->kptail=NULL;
							temp3->cptail=NULL;
							temp3->right=NULL;
							temp3->parent=parent1;
							temp3->id=0;
							temp3->privatekey=new intt;
							temp3->publickey=new intt;
							temp3->privatekey->xc=0;
							temp3->privatekey->yc=0;
							temp3->publickey->xc=0;
							temp3->publickey->yc=0;
							///cout<<"\nInsertedd in parent's right where right was not null";
							break;
						}
						
					
					}
				///cout<<"\nBroken from first loop";
			}
		}
		
		
	}
	return head;
}

void keychanges(tmember *head)
{
	if(head == NULL)       
        return;
	
	if(head->left==NULL&&head->right==NULL&&head->middle==NULL)
	{	
		struct Node* temp2 = new Node;
		head->kphead=temp2;
		head->kptail=head->kphead;
		tmember *temp=head->parent;
		while(temp!=NULL)
		{
			head->kptail->data=temp->privatekey;
			struct Node *temp3=new Node;
			head->kptail->next=temp3;
			head->kptail=head->kptail->next;
			temp=temp->parent;
		}
	}

	keychanges(head->left);
	keychanges(head->middle);
	keychanges(head->right);
}

void cochanges(tmember* head)
{
	if(head == NULL)       
        return;
	
	if(head->left==NULL&&head->right==NULL&&head->middle==NULL)
	{	
		struct Node* temp2 = new Node;
		head->cphead=temp2;
		head->cptail=head->cphead;
		tmember *temp=head;
		while(temp->parent!=NULL)
		{	
			if(temp->parent->right==temp)
			{
			if(temp->parent->left!=NULL)
			{
			head->cptail->data=temp->parent->left->publickey;
			struct Node *temp4=new Node;
			head->cptail->next=temp4;
			head->cptail=head->cptail->next;
			}
			if(temp->parent->middle!=NULL)
			{
			head->cptail->data=temp->parent->middle->publickey;
			struct Node *temp5=new Node;
			head->cptail->next=temp5;
			head->cptail=head->cptail->next;
			}
			}
			
			else if(temp->parent->middle==temp)
			{
			if(temp->parent->left!=NULL)
			{
			head->cptail->data=temp->parent->left->publickey;
			struct Node *temp6=new Node;
			head->cptail->next=temp6;
			head->cptail=head->cptail->next;
			}
			if(temp->parent->right!=NULL)
			{
			head->cptail->data=temp->parent->right->publickey;
			struct Node *temp7=new Node;
			head->cptail->next=temp7;
			head->cptail=head->cptail->next;
			}
			}

			
			else if(temp->parent->left==temp)
			{
			if(temp->parent->middle!=NULL)
			{
			head->cptail->data=temp->parent->middle->publickey;
			struct Node *temp8=new Node;
			head->cptail->next=temp8;
			head->cptail=head->cptail->next;
			}
			if(temp->parent->right!=NULL)
			{
			head->cptail->data=temp->parent->right->publickey;
			struct Node *temp9=new Node;
			head->cptail->next=temp9;
			head->cptail=head->cptail->next;
			}
			}

			temp=temp->parent;
		}
	}

	cochanges(head->left);
	cochanges(head->middle);
	cochanges(head->right);
}

void updatekeypath2(tmember* head)
{
	struct Node *h=new Node;
	head->kptail->next=h;
	head->kptail=head->kptail->next;
}

void updatekeypath1(tmember* head,intt* oldprivate,intt* newprivate,tmember* head1)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle==NULL)      
        { 
		Node* kph=head->kphead;
		Node* kph1;
		while(kph->next!=NULL)
		{
		kph1=kph;
		kph=kph->next;
		}
			if(kph1->data==oldprivate)
			{
			//cout<<"JJJJJJJJ"<<kph1->data->xc;
			kph1->data->xc=newprivate->xc;
			kph1->data->yc=newprivate->yc;
			}
			else if(kph1->data==head1->left->privatekey||kph1->data==head1->middle->privatekey)
			{
			kph1->next->data=newprivate;
			kph1->next->next=NULL;
			updatekeypath2(head);
			}
		return;
	}

       updatekeypath1(head->left,oldprivate,newprivate,head1);
	 updatekeypath1(head->middle,oldprivate,newprivate,head1);
       updatekeypath1(head->right,oldprivate,newprivate,head1);  
}

void updatekeypath3(tmember* head)
{
	struct tmember* temp=jsponsor->parent;
	jsponsor->kptail=jsponsor->kphead;
	while(temp!=NULL)
	{
		jsponsor->kptail->data=temp->privatekey;
		struct Node *temp1=new Node;
		jsponsor->kptail->next=temp1;
		jsponsor->kptail=jsponsor->kptail->next;
		temp=temp->parent;
		//cout<<"\nOnce ";
	}
}


void updatekeypath(tmember* head,intt* oldprivate,intt* newprivate)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle==NULL)      
        { int x=0;
		Node* kph=head->kphead;
		while(kph!=NULL)
		{
			if(kph->data==oldprivate)
			{
			kph->data=newprivate;
			x=1;
			break;
			}
		kph=kph->next;
		}
		if(x==0)
		{
			head->kptail->data=newprivate;
			struct Node* temp2=new Node;
			head->kptail->next=temp2;
			head->kptail=head->kptail->next;
		}
		return;
	}

       updatekeypath(head->left,oldprivate,newprivate);
       updatekeypath(head->middle,oldprivate,newprivate);
       updatekeypath(head->right,oldprivate,newprivate);  
}

void updatecopath(tmember *head,intt* oldpublic,intt* newpublic)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle==NULL)      
        { int x=0;
		Node* cph=head->cphead;
		while(cph->next!=NULL)
		{
			if(cph->data==oldpublic)
			{
			cph->data=newpublic;
			x=1;
			break;
			}
		cph=cph->next;
		}
		if(x==0)
		{	
			if(head->parent->right==jsponsor)
			{
			struct Node* temp3=new Node;
			temp3->data=newpublic;
			temp3->next=head->cphead->next;
			head->cphead->next=temp3;
			}
			else
			{
			struct Node* temp2=new Node;
			temp2->data=newpublic;
			temp2->next=head->cphead;
			head->cphead=temp2;
			}
		}
		return;
	}

       updatecopath(head->left,oldpublic,newpublic);
       updatecopath(head->middle,oldpublic,newpublic);
       updatecopath(head->right,oldpublic,newpublic);  
}


tmember *updategroupkey(tmember* head,tmember *temp)
{
	if(head==NULL||temp==head)
		return head;
	if(temp->left==NULL&&temp->middle==NULL&&temp->right==NULL)
	{temp->privatekey->xc=rand()%5;
	temp->privatekey->yc=0;
	temp->publickey=ecdh(g1,p,temp->privatekey);
	temp=temp->parent;
	}
	
	while(temp!=NULL&&temp!=head)
	{
		rn++;
		intt* oldprivate=temp->privatekey;
		intt* oldpublic=temp->publickey;
		if(temp->left!=NULL&&temp->right!=NULL&&temp->middle!=NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);
		temp->privatekey=ecdh(temp->privatekey,p,temp->right->privatekey);
		}
		else if(temp->left==NULL)
		temp->privatekey=ecdh(temp->middle->publickey,p,temp->right->publickey);
		else if(temp->middle==NULL)
		temp->privatekey=ecdh(temp->left->publickey,p,temp->right->publickey);
		else if(temp->right==NULL)
		temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->publickey);
		temp->publickey=ecdh(g1,p,temp->privatekey);
		updatekeypath(temp,oldprivate,temp->privatekey);
		updatecopath(temp->parent->left,oldpublic,temp->publickey);
		if(temp->parent->middle!=temp)
		updatecopath(temp->parent->middle,oldpublic,temp->publickey);
		temp=temp->parent;
	}
	
	
}

void updatecopath1(tmember *head,intt* oldpublic)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle==NULL)      
        { 
		Node* cph=head->cphead;
		Node *cph1;
		while(cph->next!=NULL)
		{
			if(cph->data==oldpublic)
			{
			cph1->next=cph->next;
			break;
			}
		cph1=cph;
		cph=cph->next;
		}
		
		return;
	}

       updatecopath1(head->left,oldpublic);
	updatecopath1(head->middle,oldpublic);
       updatecopath1(head->right,oldpublic);  
}


tmember *leave(tmember* head,int idd,tmember *head1)
{
	if(head==NULL)
	return head1;
	if(head->left==NULL&&head->right==NULL&&head->middle==NULL)
	{
		if(head->id==idd)
		{	
			tmember *temp1=head->parent->left;
			tmember *temp2=head->parent->middle;
			tmember *temp3=head->parent->right;
			tmember *temp4=head->parent;
			//tmember *temp5=temp4->parent;
			if(temp4==head1)
			{
				if(temp1!=NULL&&temp2!=NULL&&temp3!=NULL)
				{
					if(temp1==head)
					{
					lsponsor3=rightmostparent(temp3);
					//updategroupkey(head,lsponsor);
					head1->left=NULL;
					//lsponsor=NULL;
					return head1;
					}
					else if(temp2==head)
					{
					lsponsor3=rightmostparent(temp3);
					//updategroupkey(head,lsponsor);
					head1->middle=NULL;
					//lsponsor=NULL;
					return head1;
					}
					else if(temp2==head)
					{
					lsponsor3=rightmostparent(temp2);
					//updategroupkey(head,lsponsor);
					head1->right=NULL;
					//lsponsor=NULL;
					return head1;
					}
				}
				if(temp2==NULL&&temp1==head)
				{
				lsponsor3=rightmostparent(temp3);
				head1->left=NULL;
				return temp3;
				}
				if(temp3==NULL&&temp1==head)
				{
				lsponsor3=rightmostparent(temp2);
				head1->left=NULL;
				return temp2;
				}
				if(temp1==NULL&&temp2==head)
				{
				lsponsor3=rightmostparent(temp1);
				head1->middle=NULL;
				return temp1;
				}
				if(temp3==NULL&&temp2==head)
				{
				lsponsor3=rightmostparent(temp3);
				head1->middle=NULL;
				return temp3;
				}
				if(temp2==NULL&&temp3==head)
				{
				lsponsor3=rightmostparent(temp1);
				head1->right=NULL;
				return temp1;
				}
				if(temp1==NULL&&temp3==head)
				{
				lsponsor3=rightmostparent(temp2);
				head1->right=NULL;
				return temp2;
				}
			}
	
			if((temp1==head&&temp2==NULL&&temp3==NULL)||(temp2==head&&temp1==NULL&&temp3==NULL)||(temp3==head&&temp1==NULL&&temp2==NULL))
				{	
					lsponsor3=temp4->parent;
					if(temp4->parent->left==temp4)
					{
					updatecopath1(temp4->parent->middle,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					temp4->parent->left=NULL;
					}

					else if(temp4->parent->middle==temp4)
					{
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					temp4->parent->middle=NULL;
					}
	
					else if(temp4->parent->right==temp4)
					{
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->middle,temp4->publickey);
					temp4->parent->right=NULL;
					}
					
				}
			else if(temp1==head&&temp2!=NULL&&temp3!=NULL)
					{
					if(temp2->left==NULL&&temp2->middle==NULL&&temp2->right==NULL)
					temp2->cphead=temp2->cphead->next;
					if(temp3->left==NULL&&temp3->middle==NULL&&temp3->right==NULL)
					temp3->cphead=temp3->cphead->next;
					
					temp4->left=NULL;
					
					lsponsor3=rightmostparent(temp3);
					}
			else if(temp2==head&&temp1!=NULL&&temp3!=NULL)
					{
					if(temp1->left==NULL&&temp1->middle==NULL&&temp1->right==NULL)
					temp1->cphead=temp1->cphead->next;
					if(temp3->left==NULL&&temp3->middle==NULL&&temp3->right==NULL)
					temp3->cphead=temp3->cphead->next;
					temp2->parent=NULL;
					
					temp4->middle=NULL;
					
					lsponsor3=rightmostparent(temp3);
					}
			else if(temp3==head&&temp1!=NULL&&temp2!=NULL)
					{
					if(temp1->left==NULL&&temp1->middle==NULL&&temp1->right==NULL)
					temp1->cphead=temp1->cphead->next;
					if(temp2->left==NULL&&temp2->middle==NULL&&temp2->right==NULL)
					temp2->cphead=temp2->cphead->next;
					
					temp4->right=NULL;
					//lsponsor=temp2;
					//lsponsor2=temp4;
					//lsponsor2=rightmostparent(temp4);
					lsponsor3=rightmostparent(temp2);
					}
			else if((temp1==head&&temp2!=NULL&&temp3==NULL)||(temp3==head&&temp2!=NULL&&temp1==NULL))
					{
					if(temp2->left==NULL&&temp2->right==NULL&&temp2->middle==NULL)
					temp2->cphead=temp2->cphead->next;
					if(temp4->parent->left==temp4)
					{
					temp2->parent=temp4->parent;
					temp4->parent->left=temp2;
					updatecopath1(temp4->parent->middle,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					
					}

					else if(temp4->parent->middle==temp4)
					{
					temp2->parent=temp4->parent;
					temp4->parent->middle=temp2;
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					temp4->parent->middle=temp2;
					
					}
	
					else if(temp4->parent->right==temp4)
					{
					temp2->parent=temp4->parent;
					temp4->parent->middle=temp2;
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->middle,temp4->publickey);
					temp4->parent->right=temp2;
					}

					
					
					lsponsor3=rightmostparent(temp2);
					}
			else if((temp1==head&&temp2==NULL&&temp3!=NULL)||(temp2==head&&temp1==NULL&&temp3!=NULL))
					{
					if(temp3->left==NULL&&temp3->right==NULL&&temp3->middle==NULL)
					temp3->cphead=temp3->cphead->next;
					if(temp4->parent->left==temp4)
					{
					temp3->parent=temp4->parent;
					temp4->parent->left=temp3;
					updatecopath1(temp4->parent->middle,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					}

					else if(temp4->parent->middle==temp4)
					{
					temp3->parent=temp4->parent;
					temp4->parent->middle=temp3;
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					
					}
	
					else if(temp4->parent->right==temp4)
					{
					temp3->parent=temp4->parent;
					temp4->parent->right=temp3;
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->middle,temp4->publickey);
					temp4->parent->right=temp3;
					}

					lsponsor3=rightmostparent(temp3);
					}
			else if((temp2==head&&temp1!=NULL&&temp3==NULL)||(temp3==head&&temp1!=NULL&&temp2==NULL))
					{
					if(temp1->left==NULL&&temp1->right==NULL&&temp1->middle==NULL)
					temp1->cphead=temp1->cphead->next;
					if(temp4->parent->left==temp4)
					{
					temp1->parent=temp4->parent;
					temp4->parent->left=temp1;
					updatecopath1(temp4->parent->middle,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					
					}

					else if(temp4->parent->middle==temp4)
					{
					
					temp1->parent=temp4->parent;
					temp4->parent->middle=temp1;
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->right,temp4->publickey);
					
					}
	
					else if(temp4->parent->right==temp4)
					{
					temp1->parent=temp4->parent;
					temp4->parent->right=temp1;
					updatecopath1(temp4->parent->left,temp4->publickey);
					updatecopath1(temp4->parent->middle,temp4->publickey);
					temp4->parent->right=temp1;
					}
					
					lsponsor3=rightmostparent(temp1);
					}
					
					
					
					
			return head1;
		}
		return head1;
	}
	head1=leave(head->left,idd,head1);
	head1=leave(head->middle,idd,head1);
	head1=leave(head->right,idd,head1);
	return head1;
}

void leavet(tmember* head)
{
	if(head==NULL)
	return ;
	if(head->left==NULL&&head->right==NULL&&head->middle==NULL)
	{
		cout<<" \nLeaf "<<head->id;		
	}
	leavet(head->left);
	leavet(head->middle);
	leavet(head->right);
	
}


tmember *individualleave(tmember* head,int idd)
{
	head=leave(head,idd,head);
	head=updategroupkey(head,lsponsor3->parent);
	
	return head;
}



tmember *individualjoin(tmember* head)
{
	head=join(head,1);
	if(jsponsor->parent->middle!=jsponsor)
	updatecopath(jsponsor->parent->middle,jsponsor->publickey,jsponsor->publickey);
	updatecopath(jsponsor->parent->left,jsponsor->publickey,jsponsor->publickey);
	head=updategroupkey(head,jsponsor->parent);
	
}


void TraverseGivenLevel(struct tmember* head, int level,int level1,int height1,int height2)
{
	
    if (head == NULL)
        return;
    if (level == 0&&ip!=1)
        {	
		int height3=heightofmember(head);
		if(((level1+height3)<height2)&&((level1+height1)<height2))
		{
			ip=1;
			insertion=head;
		}
	}
	
    else if (level>=1)
    {
        TraverseGivenLevel(head->left, level-1,level1,height1,height2);
	TraverseGivenLevel(head->middle, level-1,level1,height1,height2);
        TraverseGivenLevel(head->right, level-1,level1,height1,height2);
    }
}


void traverse(tmember *head,tmember *head1)
{
	int height1= heightofmember(head1);
	int height2= heightofmember(head);
	if(height1>=height2)
	return;
	for (int i=1; i<=height2; i++)
	{
        TraverseGivenLevel(head, i,i,height1,height2);
	}
}

int search(int l[100],int nl,int m,tmember *temp)
{
	int x=0;
	for(int i=0;i<nl;i++)
	{
		if(l[i]==m)
		{
		x=1;
		break;
		}
	}
	return x;
}

void LeafNodes(tmember* head,int nl,int l[100],int x)
{
      if(head == NULL)       
        return;
       if(head->left == NULL && head->right==NULL)      
        {
		if((search(l,nl,head->id,head)))
		{	
			if(x<ldepth)
			{
			ldepth=x;
			ldepthid=head->id;
			insertion=head;
			}
		}
	}
       LeafNodes(head->left,nl,l,x+1);
       LeafNodes(head->middle,nl,l,x+1);
       LeafNodes(head->right,nl,l,x+1);      
}



void shallowestleaf(tmember *head,int nl,int l[100])
{
	LeafNodes(head,nl,l,0);
}



tmember *batchrekeying(tmember *head,int nj,int nl,int l[1000])
{
	struct tmember *head1=new tmember;
	head1->privatekey= new intt;
	head1->publickey=new intt;
	Idbackup=Idbackup+Id;
	Id=0;
	head1=join(head1,nj);
	nre=0;
	groupkeycalc(head1);
	head1->privatekey=head1->left->publickey;
	if(head1->middle!=NULL)
	head1->privatekey=ecdh(head1->privatekey,p,head1->middle->privatekey);
	if(head1->right!=NULL)
	head1->privatekey=ecdh(head1->privatekey,p,head1->right->privatekey);
	//cout<<"MMMMMMMMMM"<<head1->privatekey->xc;
	head1->publickey=ecdh(g1,p,head1->privatekey);
	if(nl==0)
	{
		traverse(head,head1);
		if(ip==0)
		insertion=head;
		if(insertion==head)
		{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=head;
			temp->middle=head1;
			temp->right=NULL;
			head->parent=temp;
			head1->parent=temp;
			temp->parent=NULL;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			head=temp;		
		}
		else
		{
			tmember *iparent=insertion->parent;
			if(iparent->left==insertion)
			{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->middle=head1;
			temp->right=NULL;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			iparent->left=temp;
			if(iparent->middle!=NULL)	
			updatecopath(iparent->middle,temp->publickey,temp->publickey);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey);
			}
			else if(iparent->middle==insertion)
			{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->middle=head1;
			temp->right=NULL;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			iparent->middle=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey);
			}	
			else if(iparent->right==insertion)
			{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->middle=head1;
			temp->right=NULL;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			iparent->right=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey);
			if(iparent->middle!=NULL)
			updatecopath(iparent->middle,temp->publickey,temp->publickey);
			}	
		}
		keychanges(head1);
		cochanges(head1);
		updatecopath(insertion,head1->publickey,head1->publickey);
		renewed[nre++]=head1->parent;
	}

		else if(nl>0)
		{
			shallowestleaf(head,nl,l);
			if(ldepth+1+heightofmember(head1)>heightofmember(head))
			{	
				ip=0;
				x1=1;
				ldepthid=-1;
				traverse(head,head1);
				if(ip==0)
				insertion=head;
			}
			for(int i=0;i<nl;i++)
			{
				if(l[i]!=ldepthid)
				{
					
					head=leave(head,l[i],head);
					if(lsponsor3!=NULL)
					{
					renewed[nre]=lsponsor3;
					nre=nre+1;
					}
					
				}
			}
			
			if(x1==1)
			{
				
		//	cout<<"\n insertion id : "<<insertion->id;

			if(insertion==head)
			{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=head;
			temp->middle=head1;
			temp->right=NULL;
			head->parent=temp;
			head1->parent=temp;
			temp->parent=NULL;
			temp->id=0;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			head=temp;	
			}
		else
		{
			tmember *iparent=insertion->parent;
			
			if(iparent->left==insertion)
			{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->middle=head1;
			temp->right=NULL;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			iparent->left=temp;
			if(iparent->middle!=NULL)	
			updatecopath(iparent->middle,temp->publickey,temp->publickey);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey);
			}
			else if(iparent->middle==insertion)
			{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->middle=head1;
			temp->right=NULL;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			iparent->middle=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey);
			}	
			else if(iparent->right==insertion)
			{
			tmember* temp=new tmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->middle=head1;
			temp->right=NULL;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,p,temp->middle->privatekey);	
			temp->publickey=ecdh(g1,p,temp->privatekey);
			iparent->right=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey);
			if(iparent->middle!=NULL)
			updatecopath(iparent->middle,temp->publickey,temp->publickey);
			}
			}
		keychanges(head1);
		cochanges(head1);
		updatecopath(insertion,head1->publickey,head1->publickey);
		renewed[nre++]=head1->parent;
	}


	
			
			tmember *iparent=insertion->parent;
			tmember *tempp1;
			tmember *tempp2;
			
			if(insertion->parent->left==insertion&&x1==0)
			{
				head1->parent=insertion->parent;
				insertion->parent->left=head1;
				tempp1=iparent->middle;
				if(iparent->middle!=NULL);
			        updatecopath(iparent->middle,head1->publickey,head1->publickey);
				if(iparent->right!=NULL);
			        updatecopath(iparent->right,head1->publickey,head1->publickey);
			
				
			}
			
			else if(iparent->middle==insertion&&x1==0)
			{
				head1->parent=iparent;
				iparent->middle=head1;
				if(iparent->left!=NULL);
			        updatecopath(iparent->left,head1->publickey,head1->publickey);
				if(iparent->right!=NULL);
			        updatecopath(iparent->right,head1->publickey,head1->publickey);
			}
			else if(iparent->right==insertion&&x1==0)
			{
				head1->parent=iparent;
				iparent->right=head1;
				if(iparent->left!=NULL);
			        updatecopath(iparent->left,head1->publickey,head1->publickey);
				if(iparent->middle!=NULL);
			        updatecopath(iparent->middle,head1->publickey,head1->publickey);
			}
			keychanges(head1);
			cochanges(head1);
			
			
		
		} 

		for(int i=0;i<nre;i++)
		{
			if(renewed[i]->parent!=NULL)
			updategroupkey(head,renewed[i]);
		}
			rn/=3;
		
return head;

}


void postorder1(tmember* p, int indent)
{
    if(p != NULL) {
        if(p->right) {
            postorder1(p->right, indent+6);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
        std::cout<< p->id << "\n ";
	if(p->middle) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder1(p->middle, indent+6);
        }
	
	
        if(p->left) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder1(p->left, indent+6);
        }

    }
}

int main()
{
	ifstream file;
	file.open("input.txt");
	cout<<"\n			Ternary tree \n\n\n";
	g1->xc=13;
        g1->yc=14;
	clock_t tStart = clock();
	struct tmember* head=new tmember;
	head->privatekey=new intt;
	head->publickey=new intt;
	int x11;
	file>>x11;
	head=join(head,x11);
	printf("\nTime taken for creating group: %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	groupkeycalc(head);
	printf("\nTime taken for group key calculation : %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	head->publickey=ecdh(g1,p,head->privatekey);
	
	
	
	keychanges(head);
	cochanges(head);
	
	int nj,nl,l[1000];
	cout<<"\nNo. of leaves and No. of joins :";
	file>>nl>>nj;
	//nl=100;
	//nj=200;

	l[0]=2;
	
	for(int j=0;j<nl;j++)
	l[j]=l[j-1]+2;
	
	head=batchrekeying(head,nj,nl,l);
	intt* old2=head->privatekey;
	head->privatekey=head->left->publickey;
	if(head->middle!=NULL)
	head->privatekey=ecdh(head->privatekey,p,head->middle->privatekey);
	if(head->right!=NULL)
	head->privatekey=ecdh(head->privatekey,p,head->right->privatekey);
	updatekeypath1(head,old2,head->privatekey,head);
	rn++;
	keychanges(head);
	cochanges(head);
	
	cout<<"\nNo. of members :"<<Id+Idbackup-nl;
	
	cout<<"\nGroup Key: xc= "<<head->privatekey->xc<<"   yc= "<<head->privatekey->yc;
	cout<<"\nheight :"<<heightofmember(head);
	cout<<"\n No. of Unicast :"<<uc;
	cout<<"\n No. of Broadcast :"<<bc;
	cout<<"\n No. of Renewed Nodes : "<<rn;
	cout<<"\n No. of Modular Expenontiations :"<<dh;
	printf("\nTime taken for batch rekeying : %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	


	return 0;
}




/*for i in {1..50} ; do ./a.out ; done 
gcc sample.c 2> filename.txt
for i in {1..50} ; do ./a.out &>>save1.txt ; done

*/

