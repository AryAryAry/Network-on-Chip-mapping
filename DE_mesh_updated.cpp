#include<bits/stdc++.h>
#include<time.h>
#include<fstream>
#include<sstream>
#include<cstdio>

#define bw_file "BW files/G4.txt"
using namespace std;

void swap(int *a,int *b){
	int temp = *a; *a = *b; *b = temp;
}

int getIndex(int dest[],int a,int L){
	for(int i=0;i<L;i++){
		if(dest[i]==a)return i;
	}
}

float evaluate(int *core,int row,int col,float *BW,int L){
	float comCost = 0;
	int ex_core=2;
	for(int m=0;m<L-1;m++){
		int M = core[m];
		//if(M==ex_core)continue;
		for(int n=m+1;n<L;n++){
			int N = core[n];
			//if(N==ex_core)continue;
			int hopX =  (int)(m%col)-(int)(n%col);	if(hopX<0){hopX = -hopX;}
			//if(hopX>col/2){hopX = col-hopX;}
			int hopY =  (int)(m/col)-(int)(n/col);	if(hopY<0){hopY = -hopY;}
			//if(hopY>row/2){hopY = row-hopY;}
			int hops = hopX + hopY;
			comCost += (*(BW+M*L+N))*hops;
		}
	}
	return comCost;
}

void initialize(int NP,int L,int *P) {
	for (int i = 0; i<NP; i++){
		list<int> a;
		list<int>:: iterator it;
		for (int k = 0; k < L; k++)
			a.push_back(k);
		it = a.begin();
		int n=L;
		int j=0;
		while(n>0){
			int k= rand()%n;
			while(k-->0)it++;
			*(P+i*L+j)= *it;
			j++;n--;
			a.remove(*it);
			it = a.begin();
		}
	}
	cout << "INITIALIZATION COMPLETE\n\n";
}

int main(){ //the MAIN LOOPS
	int NP = 100;
	int rows = 12;
	int cols = 12; 
	int L = rows*cols; //length of each individual mapping
	int G = 1000; //no of generations
	int P[NP][L] = {0}; //the L individual length, NP size populations array for current generation
	int P_next[NP][L]={0}; //the next generation
	int indBest[G] = {0};
	double sca_fac = 0.5;
	cout << "Working on it ..\n"<<endl;
	srand(time(0)); //almost random
	double CR = 0.6; // Crossover rate;

	//mapping combinations to bandwidth
	float BW[L][L];
	for(int y=0;y<L;y++)
		for(int x=0;x<L;x++)
			BW[x][y] = 0;

	ifstream fin;
	// by default open mode = ios::in mode
	fin.open(bw_file);
	// Execute a loop until EOF (End of File)
	while(fin){
		string line;
		// Read a Line from File
		getline(fin,line);
		int u=-1,v=-1;
		string temp;
		for(int it=0;it<line.length();it++){
			if(line[it]==' '){
				continue;
			}
			if(u==-1){
				temp=temp+line[it];
				if(line[it+1]==' '){
					char *str=(char*)malloc(temp.length()+1);
					for(int k=0;k<temp.length();k++)str[k]=temp[k];
					sscanf(str,"%d",&u);
					temp.clear();
					continue;
				}
			}
			else if(v==-1){
				temp=temp+line[it];
				if(line[it+1]==' '){
					char *str=(char*)malloc(temp.length()+1);
					for(int k=0;k<temp.length();k++){
						str[k]=temp[k];
					}
					sscanf(str,"%d",&v);
					temp.clear();
					continue;
				}
			}else{
				temp=temp+line[it];
			}
		}
		char *str=(char*)malloc(temp.length()+1);
		for(int k=0;k<temp.length();k++){
			str[k]=temp[k];
		}

		float cst;
		sscanf(str,"%f",&cst);
		BW[u][v]=cst;
		BW[v][u]=cst;
	}

	// Close the file
	fin.close();

	//randomly INITIALIZE the population of generation 0
	initialize(NP,L,(int *)P);
	float cost[G][NP] = {0};//contains costs of each population member in each generation

	time_t start_t, end_t;
	time(&start_t);

	for(int g=0;g<G-1;g++){ //goes till gen G-2 because there's no operation on gen G-1, just final evaluation
		cout << "Making generation " << g << "..\n";
		//DE algo starts
		for(int h=0;h<NP;h++){
			float eval_parent = evaluate((int *)P[h],rows,cols,(float *)BW,L);
			cost[g][h] = eval_parent; //filling the main cost table till gen G-1
			indBest[g] = (cost[g][h]<cost[g][indBest[g]])?h:indBest[g];
		}

		for (int i=0;i<NP;i++){
			int a1,a2;
			do{a1=rand()%NP;}while(a1==i || a1==indBest[g]); //get random index 1
			do{a2=rand()%NP;}while(a2==i || a2==a1 || a2 == indBest[g]); //get random index 2

			//find number of unequal cores between both vectors
			int cnt=0;
			for(int index=0;index<L;index++){
				if(P[a1][index]!=P[a2][index])cnt++;
			}
			
			int n_swap = sca_fac*cnt; //no of random swaps in the mutant

			int mutant[rows*cols];
			for(int k=0;k<L;k++){
				mutant[k] = P[indBest[g]][k];//new mutant vector initialized equal to indBest[g] vector
			}

			if(n_swap>0){
				// the MUTATION operation
				int ind_mut[n_swap] = {0};//indexes to be mutated at

				//take n_swap random indexes from a list and store in ind_mut, the indexes which have to be mutated
				list<int> ind_list;
				list<int>:: iterator it;
				for (int k = 0; k < L; k++)
					ind_list.push_back(k);
				it = ind_list.begin();
				int indL=0;
				while(indL < n_swap){
					int k = rand()%(L-indL);
					while(k-->0)it++;
					ind_mut[indL++] = *it;
					ind_list.remove(*it);
					it = ind_list.begin();
				}

				//mutation operation
				for(int j=0;j<n_swap;j++){
					int swap_ind;
					do{swap_ind=rand()%L;}while(swap_ind==ind_mut[j]);
					swap(&mutant[swap_ind],&mutant[ind_mut[j]]);
				}
			}

			//the CROSSOVER operation
			int j_rand = (rand())%L;
			for(int l=0;l<L;l++){
				double rand_n = (double)rand()/RAND_MAX;
				//if(g==G-2 && i==L-1){cout<< rand_n<<"\n\n";}
				if((l == j_rand) || (rand_n < CR)){
					int ind_i = getIndex(mutant,P[i][l],L); //random swaps in mutant vector from the P[g][i] vector
					swap(&mutant[l],&mutant[ind_i]);
				}
			}

			float eval_child = evaluate(mutant,rows,cols,(float *)BW,L);
			float eval_parent = evaluate((int *)P[i],rows,cols,(float *)BW,L);
			//cout << "gen " << g << "member "<< i << " : " << eval_parent << endl;

			if(eval_child<eval_parent){
				for(int j_new=0;j_new<L;j_new++){
					P_next[i][j_new]=mutant[j_new];
				}
			}
			else{
				for(int j_new=0;j_new<L;j_new++){
					P_next[i][j_new]=P[i][j_new];
				}
			}
		}

		for(int i=0;i<NP;i++){
			for(int j=0; j<L;j++){
				P[i][j]=P_next[i][j];
			}
		}

		if(g%100==99){
			cout << "Best cost for member " <<fixed<<setprecision(4)<<indBest[g] << " in generation " << g << " = " << cost[g][indBest[g]] << "\n";
		}
	}

	for(int k=0;k<NP;k++){ //Evaluation of gen G-1
		cost[G-1][k] = evaluate((int *)P[k],rows,cols,(float *)BW,L);
		indBest[G-1] = (cost[G-1][k]<cost[G-1][indBest[G-1]])?k:indBest[G-1];
	}


	cout << "Best cost for member "<<fixed<<setprecision(4)<<indBest[G-1]<<" in generation "<< G-1 << " = " << cost[G-1][indBest[G-1]] << " :\n";
	for(int i=0;i<rows;i++){
		for(int j=0;j<cols;j++){
			if(P[indBest[G-1]][i*cols+j]<10)cout<<"  ";
			if(P[indBest[G-1]][i*cols+j]<100 && P[indBest[G-1]][i*cols+j]>=10)cout<<" ";
			cout<< P[indBest[G-1]][i*cols+j]<<" ";
		}
			cout<<"\n";
	}

	time(&end_t);
	double exec_t = difftime(end_t, start_t);
	cout << "\nexecution time = " << exec_t <<" s\n";
}
