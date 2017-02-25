#include<stdio.h>
#include<bits/stdc++.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<fstream>
struct Node
{
	int data;
	Node *next;
};
struct bmember
{
	Node *kphead;
	Node *cphead;
	Node *kptail;
	Node *cptail;
	bmember *left;
	bmember *right;
	unsigned long long int privatekey;
	unsigned long long int publickey;	
	int id;
	bmember *parent;
};


using namespace std;

int l1count=1;
int Idbackup=0;
int lcount=1;
int Id=0;
int Id2=Id;
int ldepth=10000;
unsigned long long int g=5;
//unsigned long long int p=11287653;
unsigned long long int p=113;
int dh=0;
int uc=0;
int bc=0;
int rn=0;
bmember* lsponsor;
bmember* jsponsor;
bmember* insertion;
int ip=0;
bmember *renewed[1000];
int nre=0;
int ldepthid=-1;

void LeafNodescp(bmember* head,bmember* receiver)
{
      if(head == NULL)       
        return;
       if(head->left == NULL && head->right==NULL)      
        {
		head->cptail->data=receiver->publickey;
		struct Node* temp1=new Node;
		head->cptail->next=temp1;
		head->cptail=head->cptail->next;
	}
       LeafNodescp(head->left,receiver);
       LeafNodescp(head->right,receiver);      
}


void LeafNodeskp(bmember* head,bmember* parent)
{
	if(head == NULL||parent==NULL)       
        return;
       if(head->left == NULL && head->right==NULL )      
        {
		head->kptail->data=parent->privatekey;
		struct Node* temp1=new Node;
		head->kptail->next=temp1;
		head->kptail=head->kptail->next;
		return;
	}
	
     LeafNodeskp(head->left,parent);
      LeafNodeskp(head->right,parent);  
}

unsigned long long int power(int t,int u,int n)
{
unsigned long long int m=0;
unsigned long long int h=t;
while(m<u)
{
t=t*h;
m++;
}


return t;
}

unsigned long long int dihe(long long t,long long n,long long u) {   // args are base, exponent, modulus
// computes s = (t ^ u) mod n
// (see Bruce Schneier's book, _Applied Cryptography_ p. 244)

	unsigned long long int a1=power(t,u,n);
	//a1=a1%u;
	dh++;
   unsigned long long int s = 1;
   while (u!=0) { if (u&1) {s = ((s*t) % n);} u >>= 1; t = (t*t)%n; }
   return s;
}
	
		

void Unicast(bmember* sender, bmember* receiver)
{	
	//cout<<"\nreceiver Id "<<receiver->id;
	if(sender==NULL||receiver==NULL)
	return;
	else if(sender->parent->left==sender)
	{
		
		sender->parent->privatekey=dihe(sender->publickey,p,receiver->privatekey);
		sender->parent->publickey=dihe(g,p,sender->parent->privatekey);
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
		sender->kptail->data=sender->parent->privatekey;
		receiver->kptail->data=sender->parent->privatekey;
		//if(receiver->id==1)
		//cout<<"\nreceiver Id "<<receiver->id<<" receiver kp tail Data "<<receiver->kptail->data;
		struct Node* temp1=new Node;
		sender->kptail->next=temp1;
		sender->kptail=sender->kptail->next;	
		struct Node* temp3=new Node;
		receiver->kptail->next=temp3;
		receiver->kptail=receiver->kptail->next;
		dh=dh+1;
		
	}
	uc++;
}

void leafnodes1(bmember *sender)
{
	if(sender==NULL)
	return;
	if(sender->left==NULL&&sender->right==NULL)
	{
	dh++;
	}
	leafnodes1(sender->left);
	leafnodes1(sender->right);
}
void BroadcastNleaf1(bmember* sender,bmember* receiver)
{	
	if(sender==NULL||receiver==NULL)
	return;
	leafnodes1(sender);
	 if(sender->parent->left==sender)
	{
		
		sender->parent->privatekey=dihe(sender->publickey,p,receiver->privatekey);
		sender->parent->publickey=dihe(g,p,sender->parent->privatekey);
		LeafNodescp(sender,receiver);
		LeafNodescp(receiver,sender);
	}
	else 
	{
		LeafNodeskp(sender,sender->parent);
		LeafNodeskp(receiver,sender->parent);	
		
	}
	bc++;
	
}

int max(int a, int b)
{
	if(a>=b)
	return a;
	else
	return b;
}



bmember* rightmostparent(bmember *head)
{
	bmember *temp=head;
	if(temp->right!=NULL)
	return rightmostparent(head->right);
	else if(temp->left!=NULL)
	return rightmostparent(head->left);
	else
	return head;
	
}


int heightofmember(bmember *head)
{
	
   if (head==NULL) 
       return -1;
   else
   {
       
	int lDepth = heightofmember(head->left);
	int rDepth = heightofmember(head->right);
	return max(lDepth,rDepth)+1;
   }
}



void printGivenLevel(struct bmember* root, int level)
{
	
    if (root == NULL)
        return;
    if (level == 0&&l1count<=(Id2-(Id2%2))&&Id2!=1)
        {	
		bmember* temp=root;
		if(l1count%2==1)
		{
		BroadcastNleaf1(temp,temp->parent->right);
		}
			
		else if(l1count%2==0)
		{
		BroadcastNleaf1(temp,temp->parent->left);
		}
		l1count++;
	}
	
    else if (level>=1)
    {
        printGivenLevel(root->left, level-1);
        printGivenLevel(root->right, level-1);
    }
}

void groupkeynleafs(struct bmember* root)
{
    int h = heightofmember(root);
    int i;
	Id2=Id;
    for (i=h-1; i>=1; i--)
	{
	Id2=ceil((float)Id2/2);
	l1count=1;
        printGivenLevel(root, i);
	}
}

////////////////////////////

void groupkeyleafs(bmember* head)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL&& lcount<=(Id-(Id%2)))      
        { 	///cout<<"\nLeaf Id: "<<head->id;
		///cout<<"\nLcount :"<<lcount;
		if(lcount%2==1)
		Unicast(head,head->parent->right);
		else if(lcount%2==0)
		Unicast(head,head->parent->left);
		lcount++;
	}
       groupkeyleafs(head->left);
       groupkeyleafs(head->right);  
}

void groupkeycalc(bmember* head)
{
	struct bmember* temp=head;
	if((Id%2)==1)
	{uc++;bc--;}
	groupkeyleafs(head);
	groupkeynleafs(head);
	dh--;
}

int fullbinarytree()
{
	int k=Id-1;
	if(k==1)
	return 0;
	while(k>0&&k!=1)
	{
	int h=k%2;
	k=k/2;
	if(h==1)
	return 0;
	}
	return 1;
}

bmember* join(bmember *head,int no_of_new_members)
{
	
	for(int i=0;i<no_of_new_members;i++)
	{ //cout<<" \n\n\n Member: "<<i+1;
		struct bmember *temp=new bmember;
		temp->parent=NULL;
		temp->kphead=new Node;
		temp->kptail=temp->kphead;
		temp->left=NULL;
		temp->right=NULL;
		temp->cphead=new Node;
		temp->cptail=temp->cphead;
		unsigned long long int flag=rand()%500;
		if(flag<0)
		flag*=-1;
		temp->privatekey=flag;
		temp->publickey=dihe(g,p,temp->privatekey);
		//cout<<"\nPrivatekey :"<<temp->privatekey;
		//cout<<"\nPublic key :"<<temp->publickey;
		Id++;
		temp->id=Id+Idbackup;	
		jsponsor=temp;
		if(Id==1) // if no member exists
		{
			//cout<<"\nfirst element ";
			head->left=temp;
			head->right=NULL;
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
			int check=fullbinarytree();
			if(check==1) //check whether it is fully completed tree or not
			{	
				struct bmember* temp2=new bmember;
				temp2->left=head;
				temp2->right=temp;
				temp2->parent=NULL;
				temp2->kphead=NULL;
				temp2->cphead=NULL;
				temp2->kptail=NULL;
				temp2->cptail=NULL;
				temp2->id=0;	
				temp->parent=temp2;
				head->parent=temp2;
				head=temp2;
				temp2->privatekey=0;
				temp2->publickey=0;	
				//cout<<"\nfully binary\n"<<i+1;		
			}
			else
			{
				struct bmember* parent1=rightmostparent(head);
					while(1)
					{
						int hl=heightofmember(parent1->left);
						int hr=heightofmember(parent1->right);
						//if(i==7)
						//cout<<" hr and hl :" <<hr<<" "<<hl<<" "<<parent1->id;
						if(hr==-1&&hl==-1)//if it is a leaf
						{
						parent1=parent1->parent;
						//cout<<"\n going to it parent\n"; 
						}
						else if(hr==hl&&hr!=-1)//if left=right=middle
						{
							//cout<<"\n going to it's parent\n"; 
							parent1=parent1->parent;
						}
						else if(Id==2)
						{
							head->right=temp;
							temp->parent=head;
							break;
						}
						else if(hr<hl&&hr!=-1&&hl!=-1)//if right is less than the other,and left is not null
						{	
							

							struct bmember *temp3=new bmember;
							temp3->left=parent1->right;
							parent1->right->parent=temp3;
							temp3->right=temp;
							temp->parent=temp3;
							parent1->right=temp3;
							temp3->kphead=NULL;
							temp3->cphead=NULL;
							temp3->kptail=NULL;
							temp3->cptail=NULL;
							temp3->parent=parent1;
							temp3->id=0;
							temp3->privatekey=0;
							temp3->publickey=0;
							///cout<<"\nInsertedd in parent's right where right was not null";
							break;
						}
						
					}
			}
		}
		
		
	}
	
	return head;
}


void postorder1(bmember* p, int indent)
{
    /*if(p != NULL) {
        if(p->right) {
            postorder1(p->right, indent+4);
        }
        if (indent) {
            std::cout << std::setw(indent) << ' ';
        }
        if (p->right) std::cout<<" /\n" << std::setw(indent) << ' ';
        std::cout<< p->id << "\n ";
	
        if(p->left) {
            std::cout << std::setw(indent) << ' ' <<" \\\n";
            postorder1(p->left, indent+4);
        }

    }*/
}

void updatekeypath2(bmember* head)
{
	struct Node *h=new Node;
	head->kptail->next=h;
	head->kptail=head->kptail->next;
}

void updatekeypath1(bmember* head,int oldprivate,int newprivate,bmember* head1)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL)      
        { 
		Node* kph=head->kphead;
		Node* kph1;
		while(kph->next!=NULL)
		{
		kph1=kph;
		kph=kph->next;
		}
			if(kph1->data==oldprivate)
			kph1->data=newprivate;
			else if(kph1->data==head1->left->privatekey)
			{
			kph1->next->data=newprivate;
			kph1->next->next=NULL;
			updatekeypath2(head);
			}
		return;
	}

       updatekeypath1(head->left,oldprivate,newprivate,head1);
       updatekeypath1(head->right,oldprivate,newprivate,head1);  
}

void updatekeypath3(bmember* head)
{
	struct bmember* temp=jsponsor->parent;
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

void updatekeypath(bmember* head,int oldprivate,int newprivate)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL)      
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
       updatekeypath(head->right,oldprivate,newprivate);  
}

void updatecopath(bmember *head,int oldpublic,int newpublic)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL)      
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
			struct Node* temp2=new Node;
			temp2->data=newpublic;
			temp2->next=head->cphead;
			head->cphead=temp2;
		}
		return;
	}

       updatecopath(head->left,oldpublic,newpublic);
       updatecopath(head->right,oldpublic,newpublic);  
}


bmember *updategroupkey(bmember* head,bmember *temp)
{
	if(head==NULL)
		return head;

	
	
	while(temp!=NULL&&temp!=head)
	{
		rn++;
		int oldprivate=temp->privatekey;
		int oldpublic=temp->publickey;
		temp->privatekey=dihe(temp->left->publickey,p,temp->right->privatekey);
		temp->publickey=dihe(g,p,temp->privatekey);
		updatekeypath(temp,oldprivate,temp->privatekey);
		updatecopath(temp->parent->left,oldpublic,temp->publickey);
		temp=temp->parent;
	}
	
	
}

void updatecopath1(bmember *head,int oldpublic)
{
	if(head == NULL)       
        return;
	
       if(head->left == NULL && head->right==NULL)      
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
       updatecopath1(head->right,oldpublic);  
}



void keychanges(bmember *head)
{
	if(head == NULL)       
        return;
	
	if(head->left==NULL&&head->right==NULL)
	{	
		struct Node* temp2 = new Node;
		head->kphead=temp2;
		head->kptail=head->kphead;
		bmember *temp=head->parent;
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
	keychanges(head->right);
}

void cochanges(bmember* head)
{
	if(head == NULL)       
        return;
	
	if(head->left==NULL&&head->right==NULL)
	{	
		struct Node* temp2 = new Node;
		head->cphead=temp2;
		head->cptail=head->cphead;
		bmember *temp=head;
		while(temp->parent!=NULL)
		{	
			if(temp->parent->right==temp&&temp->parent->left!=NULL)
			{
			head->cptail->data=temp->parent->left->publickey;
			struct Node *temp3=new Node;
			head->cptail->next=temp3;
			head->cptail=head->cptail->next;
			temp=temp->parent;
			}

			else if(temp->parent->left==temp&&temp->parent->right!=NULL)
			{
			head->cptail->data=temp->parent->right->publickey;
			struct Node *temp4=new Node;
			head->cptail->next=temp4;
			head->cptail=head->cptail->next;
			temp=temp->parent;
			}
			
			
		}
	}

	cochanges(head->left);
	cochanges(head->right);
}

bmember *leave(bmember* head,int idd,bmember *head1)
{
	if(head==NULL)
	return head1;
	if(head->left==NULL&&head->right==NULL)
	{bmember *temp1;
		
		 if(head->id==idd)
		{	
			if(head->parent->left==head)
			temp1=head->parent->right;
			else if(head->parent->right==head)
			temp1=head->parent->left;
			lsponsor=temp1;
			temp1->privatekey=rand()%500;
			temp1->publickey=dihe(g,p,temp1->privatekey);
			if(temp1->left==NULL&&temp1->right==NULL)
			temp1->cphead=temp1->cphead->next;
			bmember *temp2=temp1->parent;
			bmember *temp3;
			if(temp2->parent->left==temp2)
			temp3=temp2->parent->right;
			else
			temp3=temp2->parent->left;
			bmember *temp4=head->parent->parent;
			
			if(temp1->parent==head1)
			{
				temp1->parent=NULL;
				free(head1);
				head1=temp1;
				return temp1;
			}
			else if(temp4->right==head->parent)
			{	
				free(temp2);
				temp1->parent=temp4;
				temp4->right=temp1;
			}
			else if(temp4->left==head->parent)
			{
				free(temp2);
				//cout<<temp4->privatekey;
			//	cout<<"  "<<temp4->left->parent->privatekey;
				temp1->parent=temp4;
				temp4->left=temp1;
				//cout<<"\nnaseem";
				postorder1(head1,0);
			}
			temp1->parent=temp4;
			return head1;
			updatecopath1(temp3,temp2->publickey);
		}
		return head1;
	}
	head1=leave(head->left,idd,head1);
	head1=leave(head->right,idd,head1);
	return head1;
}

void leavet(bmember* head)
{
	if(head==NULL)
	return ;
	if(head->left==NULL&&head->right==NULL)
	{
		cout<<" \nLeaf "<<head->id;		
	}
	leavet(head->left);
	leavet(head->right);
	
}


bmember *individualjoin(bmember* head)
{
	head=join(head,1);
	updatecopath(jsponsor->parent->left,jsponsor->publickey,jsponsor->publickey);
	head=updategroupkey(head,jsponsor->parent);
	
}

bmember *individualleave(bmember* head,int idd)
{
	head=leave(head,idd,head);
	head=updategroupkey(head,lsponsor->parent);
	
	return head;
}




void TraverseGivenLevel(struct bmember* head, int level,int level1,int height1,int height2)
{
	
    if (head == NULL)
        return;
    if (level == 0&&ip!=0)
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
        TraverseGivenLevel(head->right, level-1,level1,height1,height2);
    }
}


void traverse(bmember *head,bmember *head1)
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

int search(int l[100],int nl,int m,bmember *temp)
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

void LeafNodes(bmember* head,int nl,int l[100],int x)
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
       LeafNodes(head->right,nl,l,x+1);      
}



void shallowestleaf(bmember *head,int nl,int l[100])
{
	LeafNodes(head,nl,l,0);
}

bmember *batchrekeying(bmember *head,int nj,int nl,int l[1000])
{
	struct bmember *head1=new bmember;
	Idbackup=Id+Idbackup;
	Id=0;
	head1=join(head1,nj);
	groupkeycalc(head1);
	head1->publickey=dihe(g,p,head1->privatekey);
	if(nl==0)
	{
		traverse(head,head1);
		if(ip==0)
		insertion=head;
		if(insertion==head)
		{
			bmember* temp=new bmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=head;
			temp->right=head1;
			head->parent=temp;
			head1->parent=temp;
			temp->parent=NULL;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=dihe(temp->left->publickey,p,temp->right->privatekey);	
			temp->publickey=dihe(g,p,temp->privatekey);
			head=temp;		
		}
		else
		{
			bmember *iparent=insertion->parent;
			if(iparent->left==insertion)
			{
			bmember* temp=new bmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->right=head1;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=dihe(temp->left->publickey,p,temp->right->privatekey);	
			temp->publickey=dihe(g,p,temp->privatekey);
			iparent->left=temp;	
			updatecopath(iparent->right,temp->publickey,temp->publickey);
			}	
			else if(iparent->right==insertion)
			{
			bmember* temp=new bmember;
			temp->cphead=NULL;
			temp->kphead=NULL;
			temp->left=insertion;
			temp->right=head1;
			insertion->parent=temp;
			head1->parent=temp;
			temp->parent=iparent;
			temp->id=0;
			temp->kptail=NULL;
			temp->cptail=NULL;
			Node *kptail;
			temp->privatekey=dihe(temp->left->publickey,p,temp->right->privatekey);	
			temp->publickey=dihe(g,p,temp->privatekey);
			iparent->right=temp;		
			updatecopath(iparent->left,temp->publickey,temp->publickey);
			}	
		}
		keychanges(head1);
		cochanges(head1);
		updatecopath(insertion,head1->publickey,head1->publickey);
		renewed[nre++]=head1->parent;
	}

		if(nl>0)
		{
			shallowestleaf(head,nl,l);
			
			//cout<<"\nInsertion Id "<<ldepthid;
			bmember *iparent=insertion->parent;
			bmember *tempp;
			if(iparent->left==insertion)
			{
				head1->parent=iparent;
				iparent->left=head1;
				tempp=iparent->right;
			}	
			else if(iparent->right==insertion)
			{
				head1->parent=iparent;
				iparent->right=head1;
				tempp=iparent->left;
			}
			keychanges(head1);
			cochanges(head1);
			updatecopath(tempp,head1->publickey,head1->publickey);
			renewed[nre++]=head1->parent;
			
			for(int i=0;i<nl;i++)
			{
				if(l[i]!=ldepthid)
				{
					
					head=individualleave(head,l[i]);
					renewed[nre++]=lsponsor->parent;
				}
			}
		}

		for(int i=0;i<=nre;i++)
		{
			updategroupkey(head,renewed[i]);
		}
		
return head;

}


int main()
{
	ifstream file;
	file.open("input.txt");
	cout<<"\n			Binary Tree\n\n\n ";
	clock_t tStart = clock();
	struct bmember* head=new bmember;
	int x11;
	file>>x11;
	head=join(head,x11);
	printf("\nTime taken for creating group: %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	groupkeycalc(head);
	printf("\nTime taken for group key calculation : %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	head->publickey=dihe(g,p,head->privatekey);
	
	keychanges(head);
	cochanges(head);

	int nj,nl;
	int l[100];
	cout<<"\nNo. of leaves and No. of joins :";
	file>>nl>>nj;

	l[0]=2;
	for(int j=0;j<nl;j++)
	l[j]=l[j-1]+2;
	
	head=batchrekeying(head,nj,nl,l);
	int old2=head->privatekey;
	head->privatekey=dihe(head->left->publickey,p,head->right->privatekey);
	updatekeypath1(head,old2,head->privatekey,head);
	rn++;
	keychanges(head);
	cochanges(head);
	
	cout<<"\nNo. of members :"<<Id+Idbackup-nl;
	cout<<"\nGroup Key:"<<head->privatekey;
	cout<<"\nheight :"<<heightofmember(head);
	cout<<"\n No. of Unicast :"<<uc;
	cout<<"\n No. of Broadcast :"<<bc;
	cout<<"\n No. of Renewed Nodes : "<<rn;
	cout<<"\n No. of Modular Expenontiations :"<<dh;
	printf("\nTime taken for batch rekeying : %.8fms\n", ((double)(clock() - tStart)/CLOCKS_PER_SEC)*1000);
	return 0;
	
}
