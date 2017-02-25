#include<stdio.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<fstream>
#include <pthread.h>
#include <thread>
struct intt
{
	unsigned long long int xc;
	unsigned long long int yc;
};
struct Node
{
	intt* data;
	Node *next;
};


struct qmember
{
	Node *kphead;
	Node *cphead;
	Node *kptail;
	Node *cptail;
	qmember *left;
	qmember *right;
	qmember *middle1;
	qmember *middle2;
	intt* privatekey;
	intt* publickey;	
	int id;
	qmember *parent;
};

using namespace std;


struct Info
{
int Id;
int dh;
int uc;
int bc;
int rn;
struct intt* g1;
qmember* insertion;
qmember* jsponsor;
qmember* lsponsor3;
};


void LeafNodescp(qmember* head,qmember* receiver,Info* info[10]) //Changing the cptail of nodes
{
      if(head == NULL)       

        return;
       if(head->left == NULL && head->right==NULL && head->middle2==NULL&&head->middle1==NULL)      
        {
		head->cptail->data=receiver->publickey;
		struct Node* temp1=new Node;
		head->cptail->next=temp1;
		head->cptail=head->cptail->next;
	}
       LeafNodescp(head->left,receiver,info);
       LeafNodescp(head->middle1,receiver,info); 
	LeafNodescp(head->middle2,receiver,info); 
       LeafNodescp(head->right,receiver,info);      
}

void LeafNodeskp(qmember* head,qmember* parent,Info* info[10]) //Changing the kptail of nodes
{
	if(head == NULL||parent==NULL)       
        return;
       if(head->left == NULL && head->right==NULL && head->middle2==NULL&&head->middle1==NULL)      
        {
		head->kptail->data=parent->privatekey;
		struct Node* temp1=new Node;
		head->kptail->next=temp1;
		head->kptail=head->kptail->next;
		return;
	}
	
     LeafNodeskp(head->left,parent,info);
      LeafNodeskp(head->middle1,parent,info);
	 LeafNodeskp(head->middle2,parent,info);
      LeafNodeskp(head->right,parent,info);  
}



intt* point_add(intt* temp1,intt* temp2,Info* info[10]) //adding two points
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

intt* point_double(intt* temp1,Info* info[10]) //double the points
{
	intt* temp3=new intt;
	float xp=temp1->xc;
	float yp=temp1->yc;

	float l=(((3*xp*xp)+((info[0]->g1)->xc))/(2*yp));
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

intt* calpublic(intt* temp,int privatekey1,Info* info[10]) //calculating the key
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
	return point_add(temp,calpublic(temp,privatekey1-1,info),info);
	else if(privatekey1%2==0)
	return calpublic(point_double(temp,info) ,(privatekey1)/2,info);
	
}

intt* ecdh(intt* temp,intt* privatekey1,Info *info[10]) // ecdh calculation
{
	info[0]->dh=(info[0]->dh)+1;
	unsigned long long int p=7;
	struct intt* temppublic= new intt;
	struct intt* temppublic1=new intt;
	temppublic1->xc=temp->xc;
	temppublic1->yc=temp->yc;
	if(privatekey1->xc==0)
	privatekey1->xc=privatekey1->xc+ (rand()%5)+3;
	temppublic=calpublic(temppublic1,privatekey1->xc,info);
	temppublic->xc=(temppublic->xc) % p;
	temppublic->yc=(temppublic->yc) % p;
	if(temppublic->xc<0)
	temppublic->xc=(temppublic->xc)*(-1);
	if(temppublic->yc<0)
	temppublic->yc=(temppublic->yc)*(-1);
	return temppublic;
	
}


void Unicast(qmember* sender, qmember* receiver,Info* info[10]) //Unicast
{
	if(sender==NULL||receiver==NULL)
	return;
	else if(sender->parent->left==sender)
	{
		qmember *temp10=sender->parent->middle2;
		
		
		sender->parent->privatekey=ecdh(sender->publickey,receiver->privatekey,info);
		sender->parent->publickey=ecdh((info[0]->g1),sender->parent->privatekey,info);	
		sender->cptail->data=receiver->publickey;
		receiver->cptail->data=sender->publickey;
		struct Node* temp1=new Node;
		sender->cptail->next=temp1;
		sender->cptail=sender->cptail->next;
		struct Node* temp2=new Node;
		receiver->cptail->next=temp2;
		receiver->cptail=receiver->cptail->next;
		if(temp10!=NULL)
		{
		temp10->cptail->data=sender->publickey;
		struct Node* temp11=new Node;
		temp10->cptail->next=temp11;
		temp10->cptail=temp10->cptail->next;
		}	
	}
	else if(sender->parent->middle1==sender)
	{
		qmember *temp12=sender->parent->left;
		
		sender->parent->privatekey=ecdh(sender->parent->privatekey,receiver->privatekey,info);
		sender->parent->publickey=ecdh((info[0]->g1),sender->parent->privatekey,info);	
		sender->cptail->data=receiver->publickey;
		receiver->cptail->data=sender->publickey;
		struct Node* temp1=new Node;
		sender->cptail->next=temp1;
		sender->cptail=sender->cptail->next;
		struct Node* temp2=new Node;
		receiver->cptail->next=temp2;
		receiver->cptail=receiver->cptail->next;
		if(temp12!=NULL)
		{
		temp12->cptail->data=sender->publickey;
		struct Node* temp13=new Node;
		temp12->cptail->next=temp13;
		temp12->cptail=temp12->cptail->next;
		}	
	}
	else if(sender->parent->middle2==sender)
	{
		sender->parent->privatekey=ecdh(sender->parent->privatekey,receiver->privatekey,info);
		
		sender->parent->publickey=ecdh((info[0]->g1),sender->parent->privatekey,info);
		receiver->kptail->data=receiver->parent->privatekey;
		struct Node* temp3=new Node;
		receiver->kptail->next=temp3;
		receiver->kptail=receiver->kptail->next;
	}
	info[0]->uc=(info[0]->uc)+1;
}

void BroadcastLeaf(qmember* sender,Info* info[10]) //Broadcast
{
	if(sender==NULL)
	return;
	qmember* lef=sender->parent->left;
	qmember* mid=sender->parent->middle1;
	qmember* mid2=sender->parent->middle2;
	
	lef->kptail->data=sender->parent->privatekey;
	mid->kptail->data=sender->parent->privatekey;
	mid2->kptail->data=sender->parent->privatekey;

	struct Node* temp1=new Node;
	lef->kptail->next=temp1;
	lef->kptail=lef->kptail->next;
	struct Node* temp2=new Node;
	mid->kptail->next=temp2;
	mid->kptail=mid->kptail->next;
	struct Node* temp7=new Node;
	mid2->kptail->next=temp7;
	mid2->kptail=mid2->kptail->next;

	lef->cptail->data=sender->publickey;
	mid->cptail->data=sender->publickey;
	mid2->cptail->data=sender->publickey;
	struct Node* temp3=new Node;
	lef->cptail->next=temp3;
	lef->cptail=lef->cptail->next;
	struct Node* temp4=new Node;
	mid->cptail->next=temp4;
	mid->cptail=mid->cptail->next;
	struct Node* temp8=new Node;
	mid2->cptail->next=temp8;
	mid2->cptail=mid2->cptail->next;
	

	sender->cptail->data=lef->publickey;
	struct Node* temp5=new Node;
	sender->cptail->next=temp5;
	sender->cptail=sender->cptail->next;
	sender->cptail->data=mid->publickey;
	struct Node* temp6=new Node;
	sender->cptail->next=temp6;
	sender->cptail=sender->cptail->next;
	sender->cptail->data=mid2->publickey;
	struct Node* temp9=new Node;
	sender->cptail->next=temp9;
	sender->cptail=sender->cptail->next;
	info[0]->bc=info[0]->bc+1;
	
}

void leafnodes1(qmember *sender,Info* info[10]) //Accessing all the leaf nodes
{
	if(sender==NULL)
	return;
	if(sender->left==NULL&&sender->middle1==NULL&&sender->middle2==NULL&&sender->right==NULL)
	{
	info[0]->dh=info[0]->dh+1;
	}
	leafnodes1(sender->left,info);
	leafnodes1(sender->middle1,info);
	leafnodes1(sender->middle2,info);
	leafnodes1(sender->right,info);
}


void BroadcastNleaf1(qmember* sender,qmember* receiver,Info* info[10]) //Broadcast of type 1 for non leafs
{	
	if(sender==NULL||receiver==NULL)
	return;
	leafnodes1(receiver,info);

	if(receiver->left==NULL&&receiver->middle1==NULL&&receiver->middle2==NULL&&receiver->right==NULL)
	{
	
	//if(sender->left!=NULL&&sender->right!=NULL&&sender->middle1!=NULL&&sender->middle2!=NULL)
	info[0]->uc=(info[0]->uc)+1;
	info[0]->bc=info[0]->bc-1;
	}
	if(sender==NULL||receiver==NULL)
	return;
	else if(sender->parent->left==sender)
	{	
		qmember *temp14=sender->parent->middle2;
		sender->parent->privatekey=ecdh((info[0]->g1),sender->privatekey,info);
		sender->parent->privatekey=ecdh(sender->parent->privatekey,receiver->privatekey,info);
		sender->parent->publickey=ecdh((info[0]->g1),sender->parent->privatekey,info);	
		LeafNodescp(sender,receiver,info);
		LeafNodescp(receiver,sender,info);
		if(temp14!=NULL)
		{
		LeafNodescp(sender,temp14,info);
		LeafNodescp(temp14,sender,info);
		}
	}
	else if(sender->parent->middle1==sender)
	{
		sender->parent->privatekey=ecdh(sender->parent->privatekey,receiver->privatekey,info);
		sender->parent->publickey=ecdh((info[0]->g1),sender->parent->privatekey,info);	
		LeafNodescp(sender,receiver,info);
		LeafNodescp(receiver,sender,info);
	}
	else if(sender->parent->middle2==sender)
	{
		sender->parent->privatekey=ecdh(sender->parent->privatekey,receiver->privatekey,info);
		sender->parent->publickey=ecdh((info[0]->g1),sender->parent->privatekey,info);	
	}
	info[0]->bc=info[0]->bc+1;
	
}

void BroadcastNleaf2(qmember* sender,Info* info[10]) //Broadcast of type 2 for non leafs
{
	if(sender==NULL)
	return;
	
	leafnodes1(sender->parent->left,info);
	leafnodes1(sender->parent->middle1,info);
	leafnodes1(sender->parent->middle2,info);

	if(sender->left==NULL&&sender->middle1==NULL&&sender->middle2==NULL&&sender->right==NULL)
	info[0]->bc=info[0]->bc-1;
	qmember* lef=sender->parent->left;
	qmember* mid=sender->parent->middle1;
	qmember* mid2=sender->parent->middle2;

	LeafNodeskp(sender,sender->parent,info);
	
	
	LeafNodescp(lef,sender,info);
	LeafNodescp(mid,sender,info);
	LeafNodescp(mid2,sender,info);

	LeafNodescp(sender,lef,info);
	LeafNodescp(sender,mid,info);
	LeafNodescp(sender,mid2,info);

	LeafNodeskp(lef,sender->parent,info);
	LeafNodeskp(mid,sender->parent,info);
	LeafNodeskp(mid2,sender->parent,info);

	info[0]->bc=info[0]->bc+1;
	
}

int max(int a, int b, int c,int d,Info* info[10]) //max of three values
{
	if(a>=b&&a>=c&&a>=d)
	return a;
	else if(b>=a&&b>=c&&b>=d)
	return b;
	else if(c>=a&&c>=b&&c>=d)
	return c;
	else if(d>=a&&d>=b&&d>=c)
	return d;
}



qmember* rightmostparent(qmember *head,Info* info[10]) //selecting the insertion point roughly
{
	qmember *temp=head;
	if(temp->right!=NULL)
	return rightmostparent(temp->right,info);
	else if(temp->middle2!=NULL)
	return rightmostparent(temp->middle2,info);
	else if(temp->middle1!=NULL)
	return rightmostparent(temp->middle1,info);
	else if(temp->left!=NULL)
	return rightmostparent(temp->left,info);
	else
	return head;
	
}

int heightofmember(qmember *head,Info* info[10]) // finding height
{
	
   if (head==NULL) 
       return -1;
   else
   {
       
	int lDepth = heightofmember(head->left,info);
	int rDepth = heightofmember(head->right,info);
	int mDepth = heightofmember(head->middle1,info);
	int mDepth2 = heightofmember(head->middle2,info);
	return max(lDepth,rDepth,mDepth,mDepth2,info)+1;
   }
}


int printGivenLevel(struct qmember* root, int level,int l1count,Info* info[10]) //= Accessing nodes at levels and calculating group key
{
	
    if (root == NULL)
        return l1count;
	//			danger
    if (level == 0&&root->parent->left!=NULL&&root->parent->middle1!=NULL&&root->parent->middle2!=NULL&&root->parent->right!=NULL)
        {	//cout<<"\nEntered";
		qmember* temp=root;
		if(l1count%4==1)
		{
		
		BroadcastNleaf1(temp,temp->parent->middle1,info);
		}
		else if(l1count%4==2)
		{
		BroadcastNleaf1(temp->parent->middle1,temp->parent->middle2,info);
		}
		else if(l1count%4==3)
		{
		BroadcastNleaf1(temp->parent->middle1,temp->parent->middle2,info);
		}		
		else if(l1count%4==0)
		{
		BroadcastNleaf2(temp->parent->right,info);
		}
		l1count++;
		return l1count;
	}
	//				danger
	else if(level==0)
	{	
		qmember* lef=root->parent->left;
		qmember* mid1=root->parent->middle1;
		qmember* mid2=root->parent->middle2;
		qmember* rig=root->parent->right;
		BroadcastNleaf1(lef,mid1,info);
		BroadcastNleaf1(mid1,mid2,info);
		BroadcastNleaf1(mid2,rig,info);
		BroadcastNleaf2(rig,info);
		if(right==NULL)
		{
			LeafNodeskp(lef,root->parent,info);
			LeafNodeskp(mid1,root->parent,info);
			LeafNodeskp(mid2,root->parent,info);
		}
	}
    else if (level>=1)
    {
        l1count=printGivenLevel(root->left, level-1,l1count,info);
	l1count=printGivenLevel(root->middle1, level-1,l1count,info);
	l1count=printGivenLevel(root->middle2, level-1,l1count,info);
        l1count=printGivenLevel(root->right, level-1,l1count,info);
    }
	return l1count;
}


void groupkeynleafs(struct qmember* root,Info* info[10]) // accessing all the levels, calling them
{
    int h = heightofmember(root,info);
    int i;
	//(info[0]->Id)2=(info[0]->Id);
    for (i=h-1; i>=1; i--)
	{
	
	//int x=i;
	//cout<<"\n x is "<<x;
       printGivenLevel(root, i,1,info);
	}
}



int groupkeyleafs(qmember* head,int lcount,Info* info[10]) //accessing all the  leaf nodes to calculate group key
{
	if(head == NULL)       
        return lcount;
	
       if(head->left == NULL && head->right==NULL && head->middle1==NULL&&head->middle2==NULL&&head->parent->right!=NULL)      
        {
		if(lcount%4==1)
		Unicast(head,head->parent->middle1,info);
		else if(lcount%4==2)
		Unicast(head->parent->middle1,head->parent->middle2,info);
		else if(lcount%4==3)
		Unicast(head->parent->middle2,head->parent->right,info);
		else
		BroadcastLeaf(head->parent->right,info);
		lcount++;
	}
	else if(head->left == NULL && head->right==NULL && head->middle1==NULL&&head->middle2==NULL&&head->parent->right==NULL)      
        {
		qmember *lef=head->parent->left;
		qmember *mid1=head->parent->middle1;
		qmember *mid2=head->parent->middle2;
		if(mid1!=NULL&&lef!=NULL)
		{
		if(mid1->left==NULL&&mid1->right==NULL&&mid1->middle1==NULL&&mid1->middle2==NULL&&lef->left==NULL&&lef->middle1==NULL&&lef->middle2==NULL&&lef->right==NULL)
		Unicast(head->parent->left,head->parent->middle1,info);
		}
		
		if(mid2!=NULL&&mid1!=NULL)
		{
		if(mid1->left==NULL&&mid1->right==NULL&&mid1->middle1==NULL&&mid1->middle2==NULL && mid2->left==NULL&&mid2->right==NULL&&mid2->middle1==NULL&&mid2->middle2==NULL)
		Unicast(head->parent->middle1,head->parent->middle2,info);
		}
		
		if(lef!=NULL)
		{
		if(head->parent->left!=NULL&&lef->left==NULL&&lef->middle1==NULL&&lef->middle2==NULL&&lef->right==NULL)
		{
		head->parent->left->kphead->data=head->parent->privatekey;
		struct Node* temp14=new Node;
		head->parent->left->kphead->next=temp14;
		head->parent->left->kphead=head->parent->left->kphead->next;
		}
		}
		if(mid1!=NULL)
		{
		if(head->parent->middle1!=NULL&&mid1->left==NULL&&mid1->right==NULL&&mid1->middle1==NULL&&mid1->middle2==NULL)
		{
		head->parent->middle1->kphead->data=head->parent->privatekey;
		struct Node* temp15=new Node;
		head->parent->middle1->kphead->next=temp15;
		head->parent->middle1->kphead=head->parent->middle1->kphead->next;
		}
		}
		if(mid2!=NULL)
		{
		if(head->parent->middle2!=NULL && mid2->left==NULL&&mid2->right==NULL&&mid2->middle1==NULL&&mid2->middle2==NULL)
		{
		head->parent->middle2->kphead->data=head->parent->privatekey;
		struct Node* temp16=new Node;
		head->parent->middle2->kphead->next=temp16;
		head->parent->middle2->kphead=head->parent->middle2->kphead->next;
		}
		}
		return lcount;
	}
       lcount=groupkeyleafs(head->left,lcount,info);
       lcount=groupkeyleafs(head->middle1,lcount,info); 
	 lcount=groupkeyleafs(head->middle2,lcount,info); 
       lcount=groupkeyleafs(head->right,lcount,info);  
	return lcount;
}



void groupkeycalc(qmember* head,Info* info[10]) // calculate group key
{
	struct qmember* temp=head;
	groupkeyleafs(head,1,info);
	//cout<<"\nUnicasts now "<<uc;
	groupkeynleafs(head,info);
}

int fullternarytree(Info* info[10]) //check whether fully balanced or not
{
	int k=(info[0]->Id)-1;
	if(k==1)
	return 0;
	while(k>0&&k!=1)
	{
	int h=k%4;
	k=k/4;
	if(h!=0)
	return 0;
	}
	return 1;
}

void leavet(qmember* head,Info* info[10]) //accessing all the leaves to perform leave operation
{
	if(head==NULL)
	return ;
	if(head->left==NULL&&head->right==NULL&&head->middle1==NULL&&head->middle2==NULL)
	{
		cout<<" \nLeaf "<<head->id;		
	}
	leavet(head->left,info);
	leavet(head->middle1,info);
	leavet(head->middle2,info);
	leavet(head->right,info);
	
}


qmember* join(qmember *head,int no_of_new_members,Info* info[10]) // join n members
{
	int Idbackup=(info[0]->Id);
	info[0]->Id=0;
	for(int i=0;i<no_of_new_members;i++)
	{   	//cout<<"\n \n Member: "<<i+1;
		//leavet(head);
		struct qmember *temp=new qmember;
		temp->parent=NULL;
		temp->kphead=new Node;
		temp->kptail=temp->kphead;
		temp->left=NULL;
		temp->right=NULL;
		temp->middle1=NULL;
		temp->middle2=NULL;
		temp->cphead=new Node;
		temp->cptail=temp->cphead;
		unsigned long long int flag=rand()%3;
		temp->privatekey=new intt;
		temp->privatekey->xc=flag;
		temp->privatekey->yc=0;
		temp->publickey=new intt;
		temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
		//cout<<"\nPrivate key :"<<temp->privatekey;
		//cout<<"\nPublickey :"<<temp->publickey;
		(info[0]->Id)++;
		temp->id=(info[0]->Id)+(Idbackup);	
		(info[0]->jsponsor)=temp;
		if((info[0]->Id)==1) // if no member exists
		{
			///cout<<"\nFirst elemenet";
			
			head->left=temp;
			head->right=NULL;
			head->middle1=NULL;
			head->middle2=NULL;
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
			int check=fullternarytree(info);
			if(check==1) //check whether it is fully completed tree or not
			{	
				//cout<<"\nfully ternary\n";
				struct qmember* temp2=new qmember;
				temp2->left=head;
				temp2->middle1=temp;
				temp2->middle2=NULL;
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
				//leavet(head);
				//return head;		
			}
			
			else
			{
				struct qmember* parent1=rightmostparent(head,info);
					///cout<<"\n(info[0]->Id) is "<<parent1->id<<"\n";
					
					while(1)
					{ 	
						
						int hl=heightofmember(parent1->left,info);
						int hm1=heightofmember(parent1->middle1,info);
						int hm2=heightofmember(parent1->middle2,info);
						int hr=heightofmember(parent1->right,info);					
						if(hr==-1&&hl==-1&&hm1==-1&&hm2==-1)//if it is a leaf
						{
						parent1=parent1->parent;
						//cout<<"\n going to it's parent\n"; 
						}
						else if(hr==hl&&hl==hm1&&hm1==hm2)//if left=right=middle
						{
							//cout<<"\n going to it's parent\n"; 
							parent1=parent1->parent;
						}
						else if(hr==-1&&hm1==-1&&hm2==-1&&hl!=-1)//right is null, middle is null, left is not null
						{
							parent1->middle1=temp;
							temp->parent=parent1;
							//cout<<"\nInsertedd in parent's middlel where middle was null";
							
							break;
						}
			
						else if(hr==-1&&hm1==hl&&hm2==-1&&hl!=-1)//right is null, middle is null, left is not null
						{
							parent1->middle2=temp;
							temp->parent=parent1;
							//cout<<"\nInsertedd in parent's middler where middle was null";
							break;
						}						
						else if(hr==-1&&hm2==-1&&hm1<hl&&hm1!=-1&&hl!=-1)//right is null,middle not null,middle less than left 
						{
							struct qmember *temp3=new qmember;
							temp3->left=parent1->middle1;
							parent1->middle1->parent=temp3;
							temp3->middle1=temp;
							temp3->middle2=NULL;
							temp->parent=temp3;
							parent1->middle1=temp3;
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
							//scout<<"Inserted in parent's middlel where middle is not null\n";
							break;
						}



						else if(hr==-1&&hm2!=-1&&hm2<hm1&&hm1==hl&&hm1!=-1)//right is null,middle not null,middle less than left 
						{
							struct qmember *temp3=new qmember;
							temp3->left=parent1->middle2;
							parent1->middle2->parent=temp3;
							temp3->middle1=temp;
							temp3->middle2=NULL;
							temp->parent=temp3;
							parent1->middle2=temp3;
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
							//scout<<"Inserted in parent's middler where middle is not null\n";
							break;
						}

						else if(hr==-1&&hl==hm1&&hm1==hm2)//if right is null,but left=middle
						{
							parent1->right=temp;
							temp->parent=parent1;
							//cout<<"\nInsertedd in parent's right where right was null";
							break;
						}
						else if(hl==hm1&&hm1==hm2&&hr<hl)//if right is less than the other,and left=middle
						{
							struct qmember *temp3=new qmember;
							temp3->left=parent1->right;
							parent1->right->parent=temp3;
							temp3->middle1=temp;
							temp3->middle2=NULL;
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
							//cout<<"\nInsertedd in parent's right where right was not null";
							break;
						}
						
					
					}
				///cout<<"\nBroken from first loop";
			}
		}
		
		
	}
	return head;
}

void keychanges(qmember *head,Info* info[10]) //calculating group key kp paths again for cross checking
{
	if(head == NULL)       
        return;
	
	if(head->left==NULL&&head->right==NULL&&head->middle1==NULL&&head->middle2==NULL)
	{	
		struct Node* temp2 = new Node;
		head->kphead=temp2;
		head->kptail=head->kphead;
		qmember *temp=head->parent;
		while(temp!=NULL)
		{
			head->kptail->data=temp->privatekey;
			struct Node *temp3=new Node;
			head->kptail->next=temp3;
			head->kptail=head->kptail->next;
			temp=temp->parent;
		}
	}

	keychanges(head->left,info);
	keychanges(head->middle1,info);
	keychanges(head->middle2,info);
	keychanges(head->right,info);
}

void cochanges(qmember* head,Info* info[10]) ///calculating group key kp paths again for cross checking
{
	if(head == NULL)       
        return;
	
	if(head->left==NULL&&head->right==NULL&&head->middle1==NULL&&head->middle2==NULL)
	{	
		struct Node* temp2 = new Node;
		head->cphead=temp2;
		head->cptail=head->cphead;
		qmember *temp=head;
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
			if(temp->parent->middle1!=NULL)
			{
			head->cptail->data=temp->parent->middle1->publickey;
			struct Node *temp5=new Node;
			head->cptail->next=temp5;
			head->cptail=head->cptail->next;
			}
			if(temp->parent->middle2!=NULL)
			{
			head->cptail->data=temp->parent->middle2->publickey;
			struct Node *tempa1=new Node;
			head->cptail->next=tempa1;
			head->cptail=head->cptail->next;
			}
			}
			
			else if(temp->parent->middle1==temp)
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
			if(temp->parent->middle2!=NULL)
			{
			head->cptail->data=temp->parent->middle2->publickey;
			struct Node *tempa2=new Node;
			head->cptail->next=tempa2;
			head->cptail=head->cptail->next;
			}
			}

			else if(temp->parent->middle2==temp)
			{
			if(temp->parent->left!=NULL)
			{
			head->cptail->data=temp->parent->left->publickey;
			struct Node *tempa3=new Node;
			head->cptail->next=tempa3;
			head->cptail=head->cptail->next;
			}
			if(temp->parent->right!=NULL)
			{
			head->cptail->data=temp->parent->right->publickey;
			struct Node *tempa4=new Node;
			head->cptail->next=tempa4;
			head->cptail=head->cptail->next;
			}
			if(temp->parent->middle1!=NULL)
			{
			head->cptail->data=temp->parent->middle2->publickey;
			struct Node *tempa5=new Node;
			head->cptail->next=tempa5;
			head->cptail=head->cptail->next;
			}
			}

			
			else if(temp->parent->left==temp)
			{
			if(temp->parent->middle1!=NULL)
			{
			head->cptail->data=temp->parent->middle1->publickey;
			struct Node *temp8=new Node;
			head->cptail->next=temp8;
			head->cptail=head->cptail->next;
			}
			if(temp->parent->middle2!=NULL)
			{
			head->cptail->data=temp->parent->middle2->publickey;
			struct Node *tempa6=new Node;
			head->cptail->next=tempa6;
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

	cochanges(head->left,info);
	cochanges(head->middle1,info);
	cochanges(head->middle2,info);
	cochanges(head->right,info);
}




void updatekeypath2(qmember* head,Info* info[10]) //function1
{
	struct Node *h=new Node;
	head->kptail->next=h;
	head->kptail=head->kptail->next;
}



void updatekeypath1(qmember* head,intt* oldprivate,intt* newprivate,qmember* head1,Info* info[10]) // updating key heads of all the leaves of this head
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle1==NULL&&head->middle2==NULL)      
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
			else if(kph1->data==head1->left->privatekey||kph1->data==head1->middle1->privatekey||kph1->data==head1->middle2->privatekey||kph1->data==head1->right->privatekey)// 		Doubt
			{
			kph1->next->data=newprivate;
			kph1->next->next=NULL;
			updatekeypath2(head,info);
			}
		return;
	}

       updatekeypath1(head->left,oldprivate,newprivate,head1,info);
	 updatekeypath1(head->middle1,oldprivate,newprivate,head1,info);
	 updatekeypath1(head->middle2,oldprivate,newprivate,head1,info);
       updatekeypath1(head->right,oldprivate,newprivate,head1,info);  
}


void updatekeypath3(qmember* head,Info* info[10]) //function1
{
	struct qmember* temp=(info[0]->jsponsor)->parent;
	
	(info[0]->jsponsor)->kptail=(info[0]->jsponsor)->kphead;
	while(temp!=NULL)
	{
		(info[0]->jsponsor)->kptail->data=temp->privatekey;
		struct Node *temp1=new Node;
		(info[0]->jsponsor)->kptail->next=temp1;
		(info[0]->jsponsor)->kptail=(info[0]->jsponsor)->kptail->next;
		temp=temp->parent;
		//cout<<"\nOnce ";
	}
}

void updatekeypath(qmember* head,intt* oldprivate,intt* newprivate,Info* info[10])  // updating key heads of all the leaves of this head accessing leaves
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle1==NULL&&head->middle2==NULL)      
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

       updatekeypath(head->left,oldprivate,newprivate,info);
       updatekeypath(head->middle1,oldprivate,newprivate,info);
	updatekeypath(head->middle2,oldprivate,newprivate,info);
       updatekeypath(head->right,oldprivate,newprivate,info);  
}



void updatecopath(qmember *head,intt* oldpublic,intt* newpublic,Info* info[10]) //// updating key heads of all the leaves of this head accessing leaves
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle1==NULL&&head->middle2==NULL)      
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
			if(head->parent->right==(info[0]->jsponsor))
			{
			struct Node* temp3=new Node;
			temp3->data=newpublic;
			temp3->next=head->cphead->next->next;
			head->cphead->next->next=temp3;
			}
			else if(head->parent->middle2==(info[0]->jsponsor))
			{
			struct Node* temp2=new Node;
			temp2->data=newpublic;
			temp2->next=head->cphead->next;
			head->cphead->next=temp2;
			}
			else
			{
				struct Node* temp12=new Node;
				temp12->data=newpublic;
				temp12->next=head->cphead;
				head->cphead=temp12;
			}
		}
		return;
	}

       updatecopath(head->left,oldpublic,newpublic,info);
       updatecopath(head->middle1,oldpublic,newpublic,info);
	 updatecopath(head->middle2,oldpublic,newpublic,info);
       updatecopath(head->right,oldpublic,newpublic,info);  
}



qmember *updategroupkey(qmember* head,qmember *temp,Info* info[10])	//updating group key by going to parent and parent until i reach the head
{
	if(head==NULL||temp==head)
		return head;
	if(temp->left==NULL&&temp->middle1==NULL&&temp->middle2==NULL&&temp->right==NULL)
	{temp->privatekey->xc=rand()%5;
	temp->privatekey->yc=0;
	temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
	temp=temp->parent;
	}
	
	while(temp!=NULL&&temp!=head)
	{
		info[0]->rn=info[0]->rn+1;
		intt* oldprivate=temp->privatekey;
		intt* oldpublic=temp->publickey;
		if(temp->left!=NULL&&temp->right!=NULL&&temp->middle1!=NULL&&temp->middle2!=NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);
		temp->privatekey=ecdh(temp->privatekey,temp->middle2->privatekey,info);
		temp->privatekey=ecdh(temp->privatekey,temp->right->privatekey,info);
		}
		else if(temp->left==NULL&&temp->right!=NULL&&temp->middle1!=NULL&&temp->middle2!=NULL)
		{
		temp->privatekey=ecdh(temp->middle1->publickey,temp->middle2->privatekey,info);
		temp->privatekey=ecdh(temp->privatekey,temp->right->privatekey,info);
		}
		
		else if(temp->left!=NULL&&temp->right!=NULL&&temp->middle1==NULL&&temp->middle2!=NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,temp->middle2->privatekey,info);
		temp->privatekey=ecdh(temp->privatekey,temp->right->privatekey,info);
		}
		

		else if(temp->left!=NULL&&temp->right!=NULL&&temp->middle1!=NULL&&temp->middle2==NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);
		temp->privatekey=ecdh(temp->privatekey,temp->right->privatekey,info);
		}

		else if(temp->left!=NULL&&temp->right==NULL&&temp->middle1!=NULL&&temp->middle2!=NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);
		temp->privatekey=ecdh(temp->privatekey,temp->middle2->privatekey,info);
		}

		else if(temp->left==NULL&&temp->right!=NULL&&temp->middle1==NULL&&temp->middle2!=NULL)
		{
		temp->privatekey=ecdh(temp->middle2->publickey,temp->right->privatekey,info);
		
		}
		else if(temp->left==NULL&&temp->right!=NULL&&temp->middle1!=NULL&&temp->middle2==NULL)
		{
		temp->privatekey=ecdh(temp->middle1->publickey,temp->right->privatekey,info);
		
		}

		else if(temp->left==NULL&&temp->right==NULL&&temp->middle1!=NULL&&temp->middle2!=NULL)
		{
		temp->privatekey=ecdh(temp->middle1->publickey,temp->middle2->privatekey,info);
		}
		else if(temp->left!=NULL&&temp->right!=NULL&&temp->middle1==NULL&&temp->middle2==NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,temp->right->privatekey,info);
		}
		else if(temp->left!=NULL&&temp->right==NULL&&temp->middle1==NULL&&temp->middle2!=NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,temp->middle2->privatekey,info);
		}
		else if(temp->left!=NULL&&temp->right==NULL&&temp->middle1!=NULL&&temp->middle2==NULL)
		{
		temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);
		}


		temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
		updatekeypath(temp,oldprivate,temp->privatekey,info);
		updatecopath(temp->parent->left,oldpublic,temp->publickey,info);
		if(temp->parent->middle1!=temp)
		updatecopath(temp->parent->middle1,oldpublic,temp->publickey,info);
		if(temp->parent->middle2!=temp)
		updatecopath(temp->parent->middle2,oldpublic,temp->publickey,info);
		temp=temp->parent;
	}
	
	
}



void updatecopath1(qmember *head,intt* oldpublic,Info* info[10]) 	// updating copath values of all the leaves of this head accessing leaves
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&&head->middle2==NULL&&head->middle1==NULL)      
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

       updatecopath1(head->left,oldpublic,info);
	updatecopath1(head->middle1,oldpublic,info);
	updatecopath1(head->middle2,oldpublic,info);
       updatecopath1(head->right,oldpublic,info);  
}



qmember *leave(qmember* head,int idd,qmember *head1,Info* info[10])	//removing one member with id
{
	if(head==NULL)
	return head1;
	if(head->left==NULL&&head->right==NULL&&head->middle1==NULL&&head->middle2==NULL)
	{
		if(head->id==idd)
		{	//cout<<"\nLLLLLLLLLLL"<<head->id;
			qmember *temp1=head->parent->left;
			qmember *temp21=head->parent->middle1;
			qmember *temp22=head->parent->middle2;
			qmember *temp3=head->parent->right;
			qmember *temp4=head->parent;
			
			if(temp4==head1)
			{//if it is a head's child
				if(temp1!=NULL&&temp21!=NULL&&temp22!=NULL&&temp3!=NULL)
				{//all the four are not null
					if(temp1==head)
					{
					info[0]->lsponsor3=rightmostparent(temp3,info);
					
					head1->left=NULL;
					
					return head1;
					}
					else if(temp21==head)
					{
					info[0]->lsponsor3=rightmostparent(temp3,info);
					head1->middle1=NULL;
					return head1;
					}
					else if(temp22==head)
					{
					info[0]->lsponsor3=rightmostparent(temp3,info);
					head1->middle2=NULL;
					return head1;
					}
					else if(temp3==head)
					{
					info[0]->lsponsor3=rightmostparent(temp22,info);
					head1->right=NULL;
					
					return head1;
					}
				}

				//ANY ONE OF THEM IS NOT EQUAL TO NULL

				if(temp21==NULL&&temp22==NULL&&temp1==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->left=NULL;
				temp3->parent=NULL;
				return temp3;
				}
				if(temp21!=NULL&&temp22==NULL&&temp1==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->left=NULL;
				temp21->parent=NULL;
				return temp21;
				}
				if(temp21==NULL&&temp22!=NULL&&temp1==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->left=NULL;
				temp22->parent=NULL;
				return temp22;
				}

				//ANY ONE OF THEM IS NOT EQUAL TO NULL
				if(temp1==NULL&&temp22==NULL&&temp21==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle1=NULL;
				temp3->parent=NULL;
				return temp3;
				}
				if(temp1!=NULL&&temp22==NULL&&temp21==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle1=NULL;
				temp1->parent=NULL;
				return temp1;
				}
				if(temp1==NULL&&temp22!=NULL&&temp21==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle1=NULL;
				temp22->parent=NULL;
				return temp22;
				}

				//ANY ONE OF THEM IS NOT NULL
				if(temp1==NULL&&temp21==NULL&&temp22==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle2=NULL;
				temp3->parent=NULL;
				return temp3;
				}
				if(temp1!=NULL&&temp21==NULL&&temp22==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle2=NULL;
				temp1->parent=NULL;
				return temp1;
				}
				if(temp1==NULL&&temp21!=NULL&&temp22==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle2=NULL;
				temp21->parent=NULL;
				return temp21;
				}

				//ANY ONE OF THEM IS NOT NULL
				if(temp1==NULL&&temp21==NULL&&temp3==head&&temp22!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->right=NULL;
				temp22->parent=NULL;
				return temp22;
				}
				if(temp1!=NULL&&temp21==NULL&&temp3==head&&temp22==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->right=NULL;
				temp1->parent=NULL;
				return temp1;
				}
				if(temp1==NULL&&temp21!=NULL&&temp3==head&&temp22==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->right=NULL;
				temp21->parent=NULL;
				return temp21;
				}

				//ANY TWO OF THEM IS NOT NULL
				if(temp21==NULL&&temp22!=NULL&&temp1==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->left=NULL;
				return head1;
				}
				if(temp21!=NULL&&temp22==NULL&&temp1==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->left=NULL;
				return head1;
				}
				if(temp21!=NULL&&temp22!=NULL&&temp1==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->left=NULL;
				return head1;
				}

				//ANY TWO OF THEM IS NOT NULL
				if(temp1==NULL&&temp22!=NULL&&temp21==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle1=NULL;
				return head1;
				}
				if(temp1!=NULL&&temp22==NULL&&temp21==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle1=NULL;
				return head1;
				}
				if(temp1!=NULL&&temp22!=NULL&&temp21==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle1=NULL;
				return head1;
				}

				//ANY TWO OF THEM IS NOT NULL
				if(temp1==NULL&&temp21!=NULL&&temp22==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle2=NULL;
				return head1;
				}
				if(temp1!=NULL&&temp21==NULL&&temp22==head&&temp3!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle2=NULL;
				return head1;
				}
				if(temp1!=NULL&&temp21!=NULL&&temp22==head&&temp3==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->middle2=NULL;
				return head1;
				}


				//ANY TWO OF THEM IS NOT NULL
				if(temp1==NULL&&temp21!=NULL&&temp3==head&&temp22!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->right=NULL;
				return head1;
				}
				if(temp1!=NULL&&temp21==NULL&&temp3==head&&temp22!=NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->right=NULL;
				return head1;
				}
				if(temp1!=NULL&&temp21!=NULL&&temp3==head&&temp22==NULL)
				{
				info[0]->lsponsor3=rightmostparent(temp3,info);
				head1->right=NULL;
				return head1;
				}
				
			}
	
			//where parent is not head
			//ALL THE THREE NOT EQUAL TO NULL
			 if(temp1==head&&temp21!=NULL&&temp22!=NULL&&temp3!=NULL)
					{
					if(temp21->left==NULL&&temp21->middle1==NULL&&temp21->middle2==NULL&&temp21->right==NULL)
					temp21->cphead=temp21->cphead->next;
					if(temp22->left==NULL&&temp22->middle1==NULL&&temp22->middle2==NULL&&temp22->right==NULL)
					temp22->cphead=temp22->cphead->next;
					if(temp3->left==NULL&&temp3->middle1==NULL&&temp3->middle2==NULL&&temp3->right==NULL)
					temp3->cphead=temp3->cphead->next;
					temp4->left=NULL;
					
					info[0]->lsponsor3=rightmostparent(temp3,info);
					}
			else if(temp21==head&&temp1!=NULL&&temp3!=NULL&&temp22!=NULL)
					{
					if(temp1->left==NULL&&temp1->middle1==NULL&&temp1->middle2==NULL&&temp1->right==NULL)
					temp1->cphead=temp1->cphead->next;
					if(temp3->left==NULL&&temp3->middle1==NULL&&temp3->middle2==NULL&&temp3->right==NULL)
					temp3->cphead=temp3->cphead->next;
					if(temp22->left==NULL&&temp22->middle1==NULL&&temp22->middle2==NULL&&temp22->right==NULL)
					temp22->cphead=temp22->cphead->next;
					temp21->parent=NULL;
					
					temp4->middle1=NULL;
					
					info[0]->lsponsor3=rightmostparent(temp3,info);
					}

			else if(temp22==head&&temp1!=NULL&&temp3!=NULL&&temp21!=NULL)
					{
					cout<<"\nFiru"<<head->id;
					if(temp1->left==NULL&&temp1->middle1==NULL&&temp1->middle2==NULL&&temp1->right==NULL)
					temp1->cphead=temp1->cphead->next;
					if(temp3->left==NULL&&temp3->middle1==NULL&&temp3->middle2==NULL&&temp3->right==NULL)
					temp3->cphead=temp3->cphead->next;
					if(temp21->left==NULL&&temp21->middle1==NULL&&temp21->middle2==NULL&&temp21->right==NULL)
					temp21->cphead=temp21->cphead->next;
					temp22->parent=NULL;
					
					temp4->middle2=NULL;
					
					info[0]->lsponsor3=rightmostparent(temp3,info);
					
					}
			else if(temp3==head&&temp1!=NULL&&temp21!=NULL&&temp22!=NULL)
					{
					if(temp1->left==NULL&&temp1->middle1==NULL&&temp1->middle2==NULL&&temp1->right==NULL)
					temp1->cphead=temp1->cphead->next;
					if(temp21->left==NULL&&temp21->middle1==NULL&&temp21->middle2==NULL&&temp21->right==NULL)
					temp21->cphead=temp21->cphead->next;
					if(temp22->left==NULL&&temp22->middle1==NULL&&temp22->middle2==NULL&&temp22->right==NULL)
					temp22->cphead=temp22->cphead->next;
					
					temp4->right=NULL;
					
					info[0]->lsponsor3=rightmostparent(temp22,info);
					}

				//ANY ONE NOT EQUAL TO NULL
			else if((temp1==head&&temp21!=NULL&&temp3==NULL&&temp22==NULL)||(temp3==head&&temp21!=NULL&&temp22==NULL&&temp1==NULL)||(temp21!=NULL&&temp22==head&&temp1==NULL&&temp3==NULL))
					{
					if(temp21->left==NULL&&temp21->right==NULL&&temp21->middle1==NULL&&temp22->middle2==NULL)
					temp21->cphead=temp21->cphead->next;
					if(temp4->parent->left==temp4)
					{
					temp21->parent=temp4->parent;
					temp4->parent->left=temp21;
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					
					}

					else if(temp4->parent->middle1==temp4)
					{
					temp21->parent=temp4->parent;
					temp4->parent->middle1=temp21;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					//temp4->parent->middle1=temp21;
					
					}

					else if(temp4->parent->middle2==temp4)
					{
					temp21->parent=temp4->parent;
					temp4->parent->middle2=temp21;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					//temp4->parent->middle2=temp21;
					
					}
	
					else if(temp4->parent->right==temp4)
					{
					temp21->parent=temp4->parent;
					temp4->parent->right=temp21;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					//temp4->parent->right=temp21;
					}

					(info[0]->lsponsor3)=rightmostparent(temp21,info);
					}
					//ANY ONE NOT EQUAL TO NULL
				
					else if((temp1==head&&temp22!=NULL&&temp3==NULL&&temp21==NULL)||(temp3==head&&temp22!=NULL&&temp21==NULL&&temp1==NULL)||(temp22!=NULL&&temp21==head&&temp1==NULL&&temp3==NULL))
					{
					if(temp22->left==NULL&&temp22->right==NULL&&temp22->middle1==NULL&&temp22->middle2==NULL)
					temp22->cphead=temp22->cphead->next;
					if(temp4->parent->left==temp4)
					{
					temp22->parent=temp4->parent;
					temp4->parent->left=temp22;
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					
					}

					else if(temp4->parent->middle1==temp4)
					{
					temp22->parent=temp4->parent;
					temp4->parent->middle1=temp22;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					//temp4->parent->middle1=temp22;
					
					}

					else if(temp4->parent->middle2==temp4)
					{
					temp22->parent=temp4->parent;
					temp4->parent->middle2=temp22;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					//temp4->parent->middle2=temp21;
					
					}
	
					else if(temp4->parent->right==temp4)
					{
					temp22->parent=temp4->parent;
					temp4->parent->right=temp22;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					//temp4->parent->right=temp21;
					}

					(info[0]->lsponsor3)=rightmostparent(temp22,info);
					}
					//ANY ONE NOT EQUAL TO NULL
								

					else if((temp22==head&&temp1!=NULL&&temp3==NULL&&temp21==NULL)||(temp3==head&&temp1!=NULL&&temp21==NULL&&temp22==NULL)||(temp1!=NULL&&temp21==head&&temp22==NULL&&temp3==NULL))
					{
					if(temp1->left==NULL&&temp1->right==NULL&&temp1->middle1==NULL&&temp1->middle2==NULL)
					temp1->cphead=temp1->cphead->next;
					if(temp4->parent->left==temp4)
					{
					temp1->parent=temp4->parent;
					temp4->parent->left=temp1;
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					
					}

					else if(temp4->parent->middle1==temp4)
					{
					temp1->parent=temp4->parent;
					temp4->parent->middle1=temp1;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
				//	temp4->parent->middle1=temp22;
					
					}

					else if(temp4->parent->middle2==temp4)
					{
					temp1->parent=temp4->parent;
					temp4->parent->middle2=temp1;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					//temp4->parent->middle2=temp21;
					
					}
	
					else if(temp4->parent->right==temp4)
					{
					temp1->parent=temp4->parent;
					temp4->parent->right=temp1;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					//temp4->parent->right=temp21;
					}

					(info[0]->lsponsor3)=rightmostparent(temp1,info);
					}
					//ANY ONE NOT EQUAL TO NULL
					else if((temp1==head&&temp3!=NULL&&temp22==NULL&&temp21==NULL)||(temp22==head&&temp3!=NULL&&temp21==NULL&&temp1==NULL)||(temp3!=NULL&&temp21==head&&temp1==NULL&&temp22==NULL))
					{
					if(temp3->left==NULL&&temp3->right==NULL&&temp3->middle1==NULL&&temp3->middle2==NULL)
					temp3->cphead=temp3->cphead->next;
					if(temp4->parent->left==temp4)
					{
					temp3->parent=temp4->parent;
					temp4->parent->left=temp3;
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					
					}

					else if(temp4->parent->middle1==temp4)
					{
					temp3->parent=temp4->parent;
					temp4->parent->middle1=temp3;
					updatecopath1(temp4->parent->left,temp4->publickey,info);

					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					//temp4->parent->middle1=temp22;
					
					}

					else if(temp4->parent->middle2==temp4)
					{
					temp22->parent=temp4->parent;
					temp4->parent->middle2=temp3;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->right,temp4->publickey,info);
					//temp4->parent->middle2=temp21;
					
					}
	
					else if(temp4->parent->right==temp4)
					{
					temp3->parent=temp4->parent;
					temp4->parent->right=temp3;
					updatecopath1(temp4->parent->left,temp4->publickey,info);
					updatecopath1(temp4->parent->middle1,temp4->publickey,info);
					updatecopath1(temp4->parent->middle2,temp4->publickey,info);
					//temp4->parent->right=temp21;
					}

					(info[0]->lsponsor3)=rightmostparent(temp22,info);
					}
					//ANT TWO NOT EQUAL TO NULL
					
					if((temp1==head)&&((temp21==NULL&&temp22!=NULL&&temp3!=NULL)||(temp21!=NULL&&temp22==NULL&&temp3!=NULL)||(temp21!=NULL&&temp22!=NULL&&temp3==NULL)))
					{
					temp4->left=NULL;
					temp1->parent=NULL;
					if(temp3!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp3,info);
					else if(temp22!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp22,info);
					else if(temp21!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp21,info);
					
					}
					//ANY TWO NOT EQUAL TO NULL
					if((temp21==head)&&((temp1==NULL&&temp22!=NULL&&temp3!=NULL)||(temp1!=NULL&&temp22==NULL&&temp3!=NULL)||(temp1!=NULL&&temp22!=NULL&&temp3==NULL)))
					{
					//cout<<"KKKKKKKKKKKKKK";
					temp4->middle1=NULL;
					temp21->parent=NULL;
					if(temp3!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp3,info);
					else if(temp22!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp22,info);
					else if(temp1!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp1,info);
					
					}
					//ANY TWO NOT EQUAL TO NULL
					if((temp22==head)&&((temp21==NULL&&temp1!=NULL&&temp3!=NULL)||(temp21!=NULL&&temp1==NULL&&temp3!=NULL)||(temp21!=NULL&&temp1!=NULL&&temp3==NULL)))
					{
					temp4->middle2=NULL;
					temp22->parent=NULL;
					if(temp3!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp3,info);
					else if(temp21!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp21,info);
					else if(temp1!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp1,info);
					
					}
					//ANY TWO NOT EQUAL TO NULL
					if((temp3==head)&&((temp21==NULL&&temp22!=NULL&&temp1!=NULL)||(temp21!=NULL&&temp22==NULL&&temp1!=NULL)||(temp21!=NULL&&temp22!=NULL&&temp1==NULL)))
					{
					
					temp4->right=NULL;
					temp3->parent=NULL;
					 if(temp22!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp22,info);
					else if(temp21!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp21,info);
					else if(temp1!=NULL)
					(info[0]->lsponsor3)=rightmostparent(temp1,info);
					
					}
					
					
					
					
					
			return head1;
		}
		return head1;
	}
	head1=leave(head->left,idd,head1,info);
	head1=leave(head->middle1,idd,head1,info);
	head1=leave(head->middle2,idd,head1,info);
	head1=leave(head->right,idd,head1,info);
	return head1;
}






qmember *individualleave(qmember* head,int idd,Info* info[10])	//leave one member and update key
{
	head=leave(head,idd,head,info);
	head=updategroupkey(head,(info[0]->lsponsor3)->parent,info);
	
	return head;
}



qmember *individualjoin(qmember* head,Info* info[10])	//join one person 
{
	head=join(head,1,info);
	if((info[0]->jsponsor)->parent->middle1!=(info[0]->jsponsor))
	updatecopath((info[0]->jsponsor)->parent->middle1,(info[0]->jsponsor)->publickey,(info[0]->jsponsor)->publickey,info);
	if((info[0]->jsponsor)->parent->middle2!=(info[0]->jsponsor))
	updatecopath((info[0]->jsponsor)->parent->middle2,(info[0]->jsponsor)->publickey,(info[0]->jsponsor)->publickey,info);
	if((info[0]->jsponsor)->parent->left!=(info[0]->jsponsor))
	updatecopath((info[0]->jsponsor)->parent->left,(info[0]->jsponsor)->publickey,(info[0]->jsponsor)->publickey,info);
	if((info[0]->jsponsor)->parent->right!=(info[0]->jsponsor))
	updatecopath((info[0]->jsponsor)->parent->right,(info[0]->jsponsor)->publickey,(info[0]->jsponsor)->publickey,info);

	head=updategroupkey(head,(info[0]->jsponsor)->parent,info);
	
}


int TraverseGivenLevel(struct qmember* head, int level,int level1,int height1,int height2,int ip,Info* info[10])	//Finding the insertion parent
{
	
    if (head == NULL)
        return ip;
    if (level == 0&&ip!=1)
        {	
		int height3=heightofmember(head,info);
		if(((level1+height3)<height2)&&((level1+height1)<height2))
		{
			ip=1;
			(info[0]->insertion)=head;
		}
	}
	
    else if (level>=1)
    {
        ip=TraverseGivenLevel(head->left, level-1,level1,height1,height2,ip,info);
	ip=TraverseGivenLevel(head->middle1, level-1,level1,height1,height2,ip,info);
	ip=TraverseGivenLevel(head->middle2, level-1,level1,height1,height2,ip,info);
        ip=TraverseGivenLevel(head->right, level-1,level1,height1,height2,ip,info);
    }
	return ip;
}


int traverse(qmember *head,qmember *head1,int ip,Info* info[10])	//traversing all the levels by traversing the heights
{
	int height1= heightofmember(head1,info);
	int height2= heightofmember(head,info);
	if(height1>=height2)
	return ip;
	for (int i=1; i<=height2; i++)
	{
        ip=TraverseGivenLevel(head, i,i,height1,height2,ip,info);
	}
	return ip;
}

int search(int l[100],int nl,int m,qmember *temp,Info* info[10])	//searching in an array
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

int LeafNodes(qmember* head,int nl,int l[100],int x,int ldepthid,int ldepth,Info* info[10])	//accepting all the leafs and finding the shallowest leaf
{
      if(head == NULL)       
        return ldepth;
       if(head->left == NULL && head->right==NULL)      
        {
		if((search(l,nl,head->id,head,info)))
		{	
			if(x<ldepth)
			{
			ldepth=x;
			ldepthid=head->id;
			(info[0]->insertion)=head;
			}
		}
	}
      ldepth=LeafNodes(head->left,nl,l,x+1,ldepthid,ldepth,info);
       ldepth=LeafNodes(head->middle1,nl,l,x+1,ldepthid,ldepth,info);
	ldepth=LeafNodes(head->middle2,nl,l,x+1,ldepthid,ldepth,info);
         ldepth=LeafNodes(head->right,nl,l,x+1,ldepthid,ldepth,info);      
	return ldepth;
}




void shallowestleaf(qmember *head,int nl,int l[100],int ldepthid,int ldepth,Info* info[10])
{
	ldepth=LeafNodes(head,nl,l,0,ldepthid,ldepth,info);
}

qmember *batchrekeying(qmember *head,int nj,int nl,int l[1000],Info* info[10])	//batch rekeying by considering all the cases
{
	struct qmember *head1=new qmember;
	head1->privatekey= new intt;
	head1->publickey=new intt;
	qmember *renewed[5000];
	
	head1=join(head1,nj,info);
	int nre=0;
	groupkeycalc(head1,info);
	head1->privatekey=head1->left->publickey;
	if(head1->middle1!=NULL)
	head1->privatekey=ecdh(head1->privatekey,head1->middle1->privatekey,info);
	if(head1->middle2!=NULL)
	head1->privatekey=ecdh(head1->privatekey,head1->middle2->privatekey,info);
	if(head1->right!=NULL)
	head1->privatekey=ecdh(head1->privatekey,head1->right->privatekey,info);
	
	head1->publickey=ecdh((info[0]->g1),head1->privatekey,info);
	if(nl==0)
	{
		int ip=0;
		ip=traverse(head,head1,ip,info);
		if(ip==0)
		(info[0]->insertion)=head;
		if((info[0]->insertion)==head)
		{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=head;
			temp->middle1=head1;
			temp->middle2=NULL;
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
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			head=temp;		
		}
		else
		{
			qmember *iparent=(info[0]->insertion)->parent;
			if(iparent->left==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->middle2=NULL;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->left=temp;
			if(iparent->middle1!=NULL)	
			updatecopath(iparent->middle1,temp->publickey,temp->publickey,info);
			if(iparent->middle2!=NULL)	
			updatecopath(iparent->middle2,temp->publickey,temp->publickey,info);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey,info);
			}
			else if(iparent->middle1==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->middle2=NULL;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->middle1=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey,info);
			if(iparent->middle2!=NULL)	
			updatecopath(iparent->middle2,temp->publickey,temp->publickey,info);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey,info);
			}	

			else if(iparent->middle2==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->middle2=NULL;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->middle1=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey,info);
			if(iparent->middle1!=NULL)	
			updatecopath(iparent->middle1,temp->publickey,temp->publickey,info);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey,info);
			}				
			
			else if(iparent->right==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->right=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey,info);
			if(iparent->middle1!=NULL)
			updatecopath(iparent->middle1,temp->publickey,temp->publickey,info);
			if(iparent->middle2!=NULL)
			updatecopath(iparent->middle2,temp->publickey,temp->publickey,info);
			}	
		}
		keychanges(head1,info);
		cochanges(head1,info);
		updatecopath((info[0]->insertion),head1->publickey,head1->publickey,info);
		renewed[nre++]=head1->parent;
	}

		if(nl>0)
		{
			int x1=0;
			int ldepthid=-1;
			int ldepth=1000;
			shallowestleaf(head,nl,l,ldepthid,ldepth,info);
			if(ldepth+1+heightofmember(head1,info)>heightofmember(head,info))
			{	
				int ip=0;
				x1=1;
				ldepthid=-1;
				ip=traverse(head,head1,ip,info);
				if(ip==0)
				(info[0]->insertion)=head;
			}
			for(int i=0;i<nl;i++)
			{
				if(l[i]!=ldepthid)
				{
					head=leave(head,l[i],head,info);
					if((info[0]->lsponsor3)!=NULL)
					{
					renewed[nre]=(info[0]->lsponsor3);
					nre=nre+1;
					}
				}
			}
			
			if(x1==1)
			{
				
		

			if((info[0]->insertion)==head)
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=head;
			temp->middle1=head1;
			temp->middle2=NULL;
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
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			head=temp;	
			}
		else
		{
			qmember *iparent=(info[0]->insertion)->parent;
			
			if(iparent->left==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->middle2=NULL;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->left=temp;
			if(iparent->middle1!=NULL)	
			updatecopath(iparent->middle1,temp->publickey,temp->publickey,info);
			if(iparent->middle2!=NULL)	
			updatecopath(iparent->middle2,temp->publickey,temp->publickey,info);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey,info);
			}
			else if(iparent->middle1==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->middle2=NULL;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->middle1=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey,info);
			if(iparent->middle2!=NULL)	
			updatecopath(iparent->middle2,temp->publickey,temp->publickey,info);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey,info);
			}	

			else if(iparent->middle2==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->middle2=NULL;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->middle2=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey,info);
			if(iparent->middle1!=NULL)	
			updatecopath(iparent->middle1,temp->publickey,temp->publickey,info);
			if(iparent->right!=NULL)
			updatecopath(iparent->right,temp->publickey,temp->publickey,info);
			}	

			else if(iparent->right==(info[0]->insertion))
			{
			qmember* temp=new qmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=(info[0]->insertion);
			temp->middle1=head1;
			temp->middle2=NULL;
			temp->right=NULL;
			(info[0]->insertion)->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=new intt;
			temp->publickey=new intt;
			temp->privatekey=ecdh(temp->left->publickey,temp->middle1->privatekey,info);	
			temp->publickey=ecdh((info[0]->g1),temp->privatekey,info);
			iparent->right=temp;
			if(iparent->left!=NULL)	
			updatecopath(iparent->left,temp->publickey,temp->publickey,info);
			if(iparent->middle1!=NULL)
			updatecopath(iparent->middle1,temp->publickey,temp->publickey,info);
			if(iparent->middle2!=NULL)
			updatecopath(iparent->middle2,temp->publickey,temp->publickey,info);
			}
			}
		keychanges(head1,info);
		cochanges(head1,info);
		updatecopath((info[0]->insertion),head1->publickey,head1->publickey,info);
		renewed[nre++]=head1->parent;
	}


	
			
			qmember *iparent=(info[0]->insertion)->parent;
			qmember *tempp1;
			qmember *tempp2;
			
			if((info[0]->insertion)->parent->left==(info[0]->insertion)&&x1==0)
			{
				head1->parent=(info[0]->insertion)->parent;
				(info[0]->insertion)->parent->left=head1;
				
				if(iparent->middle1!=NULL);
			        updatecopath(iparent->middle1,head1->publickey,head1->publickey,info);
				if(iparent->middle2!=NULL);
			        updatecopath(iparent->middle2,head1->publickey,head1->publickey,info);
				if(iparent->right!=NULL);
			        updatecopath(iparent->right,head1->publickey,head1->publickey,info);
			
				
			}
			
			else if(iparent->middle1==(info[0]->insertion)&&x1==0)
			{
				head1->parent=iparent;
				iparent->middle1=head1;

				if(iparent->left!=NULL);
			        updatecopath(iparent->left,head1->publickey,head1->publickey,info);

				if(iparent->middle2!=NULL);
			        updatecopath(iparent->middle2,head1->publickey,head1->publickey,info);

				if(iparent->right!=NULL);
			        updatecopath(iparent->right,head1->publickey,head1->publickey,info);
			}

			else if(iparent->middle2==(info[0]->insertion)&&x1==0)
			{
				head1->parent=iparent;
				iparent->middle2=head1;

				if(iparent->left!=NULL);
			        updatecopath(iparent->left,head1->publickey,head1->publickey,info);

				if(iparent->middle1!=NULL);
			        updatecopath(iparent->middle1,head1->publickey,head1->publickey,info);

				if(iparent->right!=NULL);
			        updatecopath(iparent->right,head1->publickey,head1->publickey,info);
			}


			else if(iparent->right==(info[0]->insertion)&&x1==0)
			{
				head1->parent=iparent;
				iparent->right=head1;

				if(iparent->left!=NULL);
			        updatecopath(iparent->left,head1->publickey,head1->publickey,info);
				if(iparent->middle1!=NULL);
			        updatecopath(iparent->middle1,head1->publickey,head1->publickey,info);
				if(iparent->middle2!=NULL);
			        updatecopath(iparent->middle2,head1->publickey,head1->publickey,info);
			}
			keychanges(head1,info);
			cochanges(head1,info);
			
			
		
		} 

		for(int i=0;i<nre;i++)
		{
			updategroupkey(head,renewed[i],info);
		}
		info[0]->rn=info[0]->rn/3;
			
		
return head;

}


void postorder1(qmember* p, int indent)	//printing the tree
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
	if(p->middle2) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder1(p->middle2, indent+6);
        }
		
	if(p->middle1) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder1(p->middle1, indent+6);
        }
	
	
        if(p->left) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder1(p->left, indent+6);
        }

    }
}

void task(int x11,int nl,int nj)	//important function which is getting threaded
{
	ifstream file;
	file.open("input.txt");
	cout<<"\n			Quad Tree\n\n\n";
	struct Info* info[10];
	info[0]=new Info;
	info[0]->Id=0;
	info[0]->dh=0;
	info[0]->uc=0;
	info[0]->bc=0;
	info[0]->rn=0;
	info[0]->g1=new intt;
	info[0]->g1->xc=5;
	info[0]->g1->yc=6;
	
	clock_t tStart = clock();
	struct qmember* head=new qmember;
	head->privatekey=new intt;
	head->publickey=new intt;
	
	head=join(head,x11,info);
	printf("\nTime taken for creating group: %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	groupkeycalc(head,info);
	printf("\nTime taken for group key calculation : %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	head->publickey=ecdh((info[0]->g1),head->privatekey,info);
	

	int l[5000];
	

	if(nl>0)
	l[0]=2;
	for(int j=0;j<nl;j++)
	l[j]=l[j-1]+2;
	
	head=batchrekeying(head,nj,nl,l,info);
	intt* old2=head->privatekey;
	head->privatekey=head->left->publickey;
	if(head->middle1!=NULL)
	head->privatekey=ecdh(head->privatekey,head->middle1->privatekey,info);
	if(head->middle2!=NULL)
	head->privatekey=ecdh(head->privatekey,head->middle2->privatekey,info);
	if(head->right!=NULL)
	head->privatekey=ecdh(head->privatekey,head->right->privatekey,info);
	updatekeypath1(head,old2,head->privatekey,head,info);
	info[0]->rn=info[0]->rn+1;
	keychanges(head,info);
	cochanges(head,info);
	cout<<"\nNo. of members :"<<(info[0]->Id)+x11-nl;
	cout<<"\nGroup Key: xc= "<<head->privatekey->xc<<"   yc= "<<head->privatekey->yc;
	cout<<"\nheight :"<<heightofmember(head,info);
	cout<<"\n No. of Unicast :"<<info[0]->uc;
	cout<<"\n No. of Broadcast :"<<info[0]->bc;
	cout<<"\n No. of Renewed Nodes : "<<info[0]->rn;
	cout<<"\n No. of Modular Expenontiations :"<<info[0]->dh;
	printf("\nTime taken for batch rekeying : %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	return;
}

int main()
{
	ifstream file;
	file.open("input.txt");
	int x11,nl,nj,n;
	cout<<"\nNo.of clusters";
	file>>n;
	for(int i=0;i<n;i++)
	{
		file>>x11>>nl>>nj; 
		thread x(task,x11,nl,nj);
		x.join();
	}
	return 0;
}


/*$ 
 

g++ -std=c++11 -o quad3 -pthread quad3.cpp
./quad3.out 
./quad3.out &>>renu4.txt

 */
