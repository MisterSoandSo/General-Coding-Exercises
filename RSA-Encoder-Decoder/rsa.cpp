#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

bool chk_prime(unsigned int num)
{
	if((num % 2)==0)
	{
		//cout<<"Here="<<num<<endl;
		if(num==2)return true;
		return false;
	}
	unsigned int n,m;
	for(n=3;n<=num;n+=2)
	{
		m=num%n;
		if((m==0)&&(num>n))return false;
	}
	return true;
}
unsigned int encrytp(unsigned int m,unsigned int e,unsigned int n)
{
	unsigned int s=m;
	for(unsigned int i=1;i<e;i++)
	{
		s=s*m;
		if(s>n)s=s%n;
	}
	//cout<<"en="<<s<<endl;
	return ( (s)% n);
}

unsigned int decrytp(unsigned int c,unsigned int d, unsigned int n)
{
	unsigned int s=c;
		
	for(unsigned int i=1;i<d;i++)
	{
		s=s*c;
		if(s>n)s=s%n;
	}
	//cout<<"de="<<s<<endl;
	return ( (s)% n);
}

unsigned int dkey_with_e_theta(unsigned int e,unsigned int theta)
{
	unsigned int p,q,sum=0;
	vector<unsigned int>bufp;
	for(unsigned int pr=2;pr<theta*2;pr+=2)
	{
		if(chk_prime(pr))
		{
			bufp.push_back(pr);
		}
		if(pr==2)pr=1;
	}
	unsigned int q_size=bufp.size();
	
	for(unsigned int i=0;i<q_size;i++)
		for(unsigned int j=0;j<q_size;j++)
		{
			if(((bufp[i]-1)*(bufp[j]-1))==theta)
			{
				p=bufp[i];
				q=bufp[j];
				i=q_size;  //terminate for loops
				break;
			}
		}
	unsigned int thn=(p-1)*(q-1);
	for(unsigned int x=1; x<p*q;x++)
	{
		sum=thn*x+1;
		if((sum % e)==0)
		{
			return (sum/e);
		}
	}
	return 1;	
}
unsigned int dkey_with_e_p_q(unsigned int e, unsigned int p,unsigned int q)
{
	unsigned int sum=0;
	unsigned int thn=(p-1)*(q-1);
	for(unsigned int x=1; x<p*q;x++)
	{
		sum=thn*x+1;
		if((sum % e)==0)
		{
			return (sum/e);
		}
	}
	return 1;
}

bool find_e_with_p_q(unsigned int p,unsigned int q)
{
	unsigned int n,theta;
	bool returnf=false;
	vector<unsigned int>bufe;
	n=p*q;
	theta=(p-1)*(q-1);
	for(unsigned int e=2;e<n;e+=2)
	{
		if((theta % e)==0)
		{
			if(chk_prime(e))
			{
				bufe.push_back(e); //all the avoided candidates
				//cout<<"Avoid="<<e<<"  ";
			}
		}
		if(e==2)e=1; //next time is e=1+2=3
	}
	unsigned int q_size=bufe.size();
	int cnt=6;
	bool not_good=true;
	
	cout<<"The possible e values :";
	for(unsigned int e=2;e<n;e+=2)
	{
		not_good=false;
		if(chk_prime(e))
		{
			for(unsigned int i=0;i<q_size;i++)
			{
				if(e==bufe[i])not_good=true; //skip this one
				
			}
			if(not_good==false)
			{
				cout<<" "<<e;
				returnf=true;
				cnt--;
				if(cnt<1)
				{
					cout<<endl;
					return true;
				}
			}
		}
		if(e==2)e=1;
	}
	if(returnf==true)cout<<endl;
	return returnf;
	
}

bool find_p_q_with_e_n(unsigned int e,unsigned int n, unsigned int *p,unsigned int *q)
{
	//guess for p,q
	vector<unsigned int>bufp;
	
	
	//locate all the possible primes for p,q
	for(unsigned int pr=2; pr<=n;pr+=2)
	{
		if(pr!=e)
		{
			if(chk_prime(pr))
			{
				bufp.push_back(pr); //save prime candidates
				
			}
			
		}
		if(pr==2)pr=1;
	}
	int q_size=bufp.size();
	for(int i=0;i<q_size-1;i++)
		for(int j=0;j<q_size-1;j++)
		{
			if((bufp[i]*bufp[j])==n)
			{
				*p=bufp[i];
				*q=bufp[j];
				
				return true;
			}
		}
	return false;
	
}
unsigned int dkey_with_e_n(unsigned int e,unsigned int n)
{
	//guess for p,q
	vector<unsigned int>bufp;
	
	
	unsigned int p=1;
	unsigned int q=1;
	unsigned int d=1;
	//locate all the possible primes for p,q
	if( find_p_q_with_e_n(e,n,&p,&q)==true)
	{
		//cout<<"Found p="<<p<<"  q="<<q<<endl;
		d=dkey_with_e_p_q(e,p,q);
		return d;
	}
	
	cout<<"NOT Found p or q!!!"<<endl;
	return 1;
} 

void decrypt_file()
{
	
	string sFileName;
	const char* pFileName;
	cout<<"Enter the file name?"; cin>>sFileName;
	pFileName=sFileName.c_str();
	ifstream input(pFileName);
	bool header_ready=false;
	unsigned int m,d,e,n,A,space,comma,period,semic;
	cout<<endl;
	while (!input.eof())
	{
		unsigned int nVal;
		if(header_ready==false)
		{
		input>>e; input>>n; input>>A; input>>space;
		input>>comma; input>>period; input>>semic;
		d=dkey_with_e_n(e,n);
		header_ready=true;
		}
		input>>nVal;
		m=decrytp(nVal,d,n);
		if(m<A)
		{
			if(m==space)cout<<" ";
			if(m==comma)cout<<",";
			if(m==period)cout<<".";
			if(m==semic)cout<<";";
		}else
		{
			char chr;
			chr=m+59;   //letter A=65, offset by 6 means 65-6=59
			cout<<chr;
		}
		
		
		if(input.fail())break;
	}
	
	
}

void encrypt_file()
{
	
	string sFileName,sOutFileName;
	const char* pFileName;
	const char* pOutFileName;
	char buf[2048];
	cout<<"Enter the  text file name?"; cin>>sFileName;
	pFileName=sFileName.c_str();
	ifstream input(pFileName);
	cout<<"Enter the output file name?"; cin>>sOutFileName;
	pOutFileName=sOutFileName.c_str();
	ofstream output(pOutFileName);
	bool header_ready=false;
	unsigned int c,e,n,A,space,comma,period,semic;
	cout<<"Enter e="; cin>>e;  cout<<"Enter n="; cin>>n;
	cout<<"Enter A starting at="; cin>>A;
	cout<<"Enter Space=";         cin>>space;
	cout<<"Enter comma=";         cin>>comma;
	cout<<"Enter period=";        cin>>period;
	cout<<"Enter semicolon=";     cin>>semic;
	cout<<endl;
	while (!input.eof())
	{
		//char nVal;
		if(header_ready==false)
		{
		//input>>e; input>>n; input>>A; input>>space;
		//input>>comma; input>>period; input>>semic;
		//d=dkey_with_e_n(e,n);
		header_ready=true;
		output<<e; output<<endl; output<<n; output<<endl; output<<A; output<<endl;
		output<<space; output<<endl; output<<comma; output<<endl;  output<<period; output<<endl;
		output<<semic; output<<endl; 
		
		}
		char chr;
		unsigned int nVal;
		int i=0;
		for(i=0;i<2048;i++)buf[i]=0;
		input.getline(buf,2048,'\n');
		//input>>chr;
		
			for(i=0;buf[i];i++)
			{
				chr=buf[i];
				cout<<chr;
			
				if(chr==' '){c=encrytp(2,e,n); output<<c; output<<endl; }
				if(chr==','){c=encrytp(3,e,n); output<<c; output<<endl; }
				if(chr=='.'){c=encrytp(4,e,n); output<<c; output<<endl; }
				if(chr==';'){c=encrytp(5,e,n); output<<c; output<<endl; }
			
				nVal=(unsigned int)chr;
				if(nVal>59)
				{
				nVal=nVal-59;   //letter A=65, offset by 6 means 65-6=59
				c=encrytp(nVal,e,n); 
				output<<c; output<<endl; 
				}
			}
		
		
		if(input.fail())break;
	}
	output.close();
	cout<<endl<<"Encrypted file="<<sOutFileName<<" is generated."<<endl;
	cout<<endl;
	
	
}

int main(int argc, char *argv[])
{ 
	unsigned int c,e,d,m,n,p,q,theta,k=1;
	while(true)
	{
	
			printf("\n==== RSA handlers Version 1.0====\n");
			printf("0   Quit this program\n");
			printf("1   Enter p, q and e to find d\n");
			printf("2   Enter e and n to find d\n");
			printf("3   Enter p and q to find all the possible e\n");
			printf("4   Enter e and n to encrypt or decrypt ...\n"); 
			printf("5   Enter m,e,and n to c...\n");
			printf("6   Enter e and theta to find d\n");
			printf("7   Decrypt an encrytped file...\n");
			printf("8   Encrypt a text file ...\n");
			printf("? "); cin>>k;
			if(k==0)return 0;
			if(k==1)
			{
				cout<<"Enter p="; cin>>p;
				cout<<"Enter q="; cin>>q;
				cout<<"Enter e="; cin>>e;
				d=dkey_with_e_p_q(e,p,q);
				cout<<"d="<<d<<endl;
				//return 0;
			}
			if(k==2)
			{
				
				cout<<"Enter e="; cin>>e;
				cout<<"Enter n="; cin>>n;
				d=dkey_with_e_n(e,n);
				cout<<"d="<<d<<endl;
				//return 0;
			}
			if(k==3)
			{
				cout<<"Enter p="; cin>>p;
				cout<<"Enter q="; cin>>q;
				if(find_e_with_p_q(p,q)==false)
				cout<<"No e value is found!!!";
				//return 0;
				
			}
			if(k==4)
			{
				
				cout<<"Enter e="; cin>>e;
				cout<<"Enter n="; cin>>n;
				d=dkey_with_e_n(e,n);
				m=1;
				while(m!=0)
				{
				cout<<"Enter a number(0 for exit):";
				cin>>m;
				if(m==0)break;
				cout<<"You type in "<<m<<endl;
				c=encrytp(m,e,n);
				cout<<"Encrypted to "<<c<<endl;
				cout<<"Decrypted to "<<decrytp(c,d,n)<<endl;
				
				}
			}
			if(k==5)
			{
				cout<<"Enter m="; cin>>m;
				cout<<"Enter e="; cin>>e;
				cout<<"Enter n="; cin>>n;
				c=encrytp(m,e,n);
				cout<<"c="<<c;
				
			}
			if(k==6)
			{
				cout<<"Enter e="; cin>>e;
				cout<<"Enter theta="; cin>>theta;
				d=dkey_with_e_theta(e,theta);
				cout<<"d="<<d;
			}
			if(k==7)
				decrypt_file();
			if(k==8)
				encrypt_file();
	}
	return 0;
  
  
   }
