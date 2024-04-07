#include <emscripten.h>
EMSCRIPTEN_KEEPALIVE
int bocchiShutUp(int flag,int seq[],int size){
    if(flag==1){
        int freq[6]={0};
        int max=0,loc=-1;
        for(int i=0;i<size;i++){
            if(seq[i]<=16){
                freq[seq[i]-11]++;
                if(max<freq[seq[i]-11]){
                    max=freq[seq[i]-11];
                    loc=seq[i]-11;
                }
            }
        }
        for(int i=0;i<6;i++){
            if(freq[i]==max&&loc!=i){
                return 10;
            }
        }
        return loc+11;
    }else{
        int freq[6]={0};
        int max=0,loc=-1;
        for(int i=0;i<size;i++){
            if(seq[i]>20){
                freq[seq[i]-21]++;
                if(max<freq[seq[i]-21]){
                    max=freq[seq[i]-21];
                    loc=seq[i]-21;
                }
            }
        }
        for(int i=0;i<6;i++){
            if(freq[i]==max&&loc!=i){
                return 10;
            }
        }
        return loc+21;
    }
}