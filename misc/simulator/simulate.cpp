#include <bits/stdc++.h>

using namespace std;

std::vector< std::vector<bool> > msb;
std::vector< std::vector<bool> > lsb;

int main(int argc,char const *argv[]){

    if(argc<6){
        cout<<"Correct Usage ./simulatestar_C <Indv> <SNPS> <POP> <fst> <filename>"<<endl;
        exit(-1);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);
    int k = atoi(argv[3]);
    float fst = atof(argv[4]);

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0.05, 0.95);

    double panc[m];
    for (int m_iter = 0; m_iter < m; m_iter++) {
        //Use dis to transform the random unsigned int generated by gen into a double in [1, 2)
        panc[m_iter] = dis(gen); //Each call to dis(gen) generates a new random double
    }
    
    double drift = fst;
    int nk = round(n*1.0/k);
    int ntotal = nk*k;

    msb.resize(m,std::vector<bool>(ntotal));
    lsb.resize(m,std::vector<bool>(ntotal));
    
    for(int l=0;l<k;l++){
        
	double* tmpp = (double *) malloc(m*sizeof(double));
	//double tmpp[m];
        
	for(int m_iter=0;m_iter<m;m_iter++){
            double std = sqrt(panc[m_iter]*(1-panc[m_iter])*drift);
            std::normal_distribution<> d(panc[m_iter],std);
            tmpp[m_iter] = d(gen);
            if(tmpp[m_iter]<0)
                tmpp[m_iter]=0;
            else if(tmpp[m_iter]>1)
                tmpp[m_iter] = 1;
        }
        std::uniform_real_distribution<> uni(0, 1);    
        for(int m_iter=0;m_iter<m;m_iter++){
            for(int nk_iter=0;nk_iter<nk;nk_iter++){
                double g1 = uni(gen);
                double g2 = uni(gen);
                int g = ( (g1<tmpp[m_iter]) ? 0 : 1) + ( (g2<tmpp[m_iter]) ? 0 : 1);
                if(g==0){
                    msb[m_iter][l*nk + nk_iter] = false;
                    lsb[m_iter][l*nk + nk_iter] = false;  
                }
                else if(g==1){
                    msb[m_iter][l*nk + nk_iter] = false;
                    lsb[m_iter][l*nk + nk_iter] = true;  
                }
                else if(g==2){
                    msb[m_iter][l*nk + nk_iter] = true;
                    lsb[m_iter][l*nk + nk_iter] = false;  
                }
            }       
        }
	free(tmpp);
    }
	cout<<"Writing to file"<<endl;
    FILE *fp;
    fp = fopen(argv[5],"w");
    //fprintf(fp,"%d %d\n",m,ntotal);
    for(int m_iter=0;m_iter<m;m_iter++){
        for(int n_iter=0;n_iter<ntotal;n_iter++){
            int g = msb[m_iter][n_iter]*2 + lsb[m_iter][n_iter];
            fprintf(fp,"%d",g);
        }
        fprintf(fp,"\n");
    }
}
