// ###### Config options ################


// #######################################

#define BUILDING_DEF_STRATEGY_LIB 1
#define FLOAT_MAX 3.40282347e+28F

#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include <iostream>

using namespace Asedio;

#pragma region values

float ValueDefenses(Defense* defensa,std::list<Defense*> defenses){
    if(defensa->id == 0){
        return FLOAT_MAX;
    }else if(50 < defenses.size() && defenses.size()  <= 60){
        return ((defensa->health*defensa->dispersion)+pow(defensa->dispersion,2))/(defensa->radio);
    }else{
        return ((defensa->health+defensa->dispersion)+pow(defensa->dispersion,2))/(defensa->radio);
    }
}

#pragma endregion

#pragma region MatrizValores

void CrearMatrizvalores(float** &matrizAuxiliar, std::list<Defense*> defenses){
    std::list<Defense*>::iterator iterador = defenses.begin();
    
    matrizAuxiliar = new float*[defenses.size()]; 
    for(int index=0; index<defenses.size(); index++){
        matrizAuxiliar[index] = new float[3];
        matrizAuxiliar[index][0] = (*iterador)->id;
        matrizAuxiliar[index][1] = (*iterador)->cost;
        matrizAuxiliar[index][2] = ValueDefenses((*iterador),defenses);
        
        ++iterador;
    }

    float auxiliar[3];
    
    for(int index=0; index<defenses.size()-1; index++){
        for(int index2=0; index2<defenses.size()-index-1; index2++){
            if(matrizAuxiliar[index2][2] > matrizAuxiliar[index2+1][2]){
                for(int i=0; i<3; i++){
                    auxiliar[i] = matrizAuxiliar[index2][i];
                    matrizAuxiliar[index2][i] = matrizAuxiliar[index2+1][i];
                    matrizAuxiliar[index2+1][i] = auxiliar[i];
                }
            }
        }
    }
}

#pragma endregion

#pragma region selectDefenses

void DEF_LIB_EXPORTED selectDefenses(std::list<Defense*> defenses, unsigned int ases, std::list<int> &selectedIDs, float mapWidth, float mapHeight, std::list<Object*> obstacles){
    unsigned int cost = 0;
    float matrizFinal[defenses.size()][ases];
    float** matrizValores;
    std::list<Defense*>::iterator iterador = defenses.begin();

    CrearMatrizvalores(matrizValores,defenses);
    
    for(int j=0; j<ases; j++){
        if(j < matrizValores[0][1]){
            matrizFinal[0][j] = 0;
        }
        else{
            matrizFinal[0][j] = matrizValores[0][2];
        }
    }
    for(int i=2; i<defenses.size(); i++){
        for(int j=0; j<ases; j++){
            if(j < matrizValores[i][1]){
                matrizFinal[i][j] = matrizFinal[i-1][j];
            }
            else{
                matrizFinal[i][j] = std::max(matrizFinal[i-1][j],(matrizFinal[i-1][j - (int)matrizValores[i][1]] + matrizValores[i][2]));
            }
        }
    }
    
    for(int INDEXi=defenses.size()-1; INDEXi>1; INDEXi--){
        for(int INDEXj=ases-1; INDEXj>1; INDEXj--){
            if(matrizFinal[INDEXi][INDEXj] != matrizFinal[INDEXi-1][INDEXj]){
                if((cost + matrizValores[INDEXi][1]) <= ases){
                    selectedIDs.push_back(matrizValores[INDEXi][0]);
                    cost += matrizValores[INDEXi][1];
                    INDEXj -= matrizValores[INDEXi][1];
                    INDEXi--;
                }
            }else{
                INDEXi--;
            }                
        }
    }

    //original
    /*while(iterador != defenses.end()){
        if((cost + (*iterador)->cost) <= ases){
            selectedIDs.push_back((*iterador)->id);
            cost += (*iterador)->cost;
        }

        ++iterador;
    }*/
    //Comprobacion de coste
    /*if((cost + matrizValores[INDEXi][1]) <= ases){
        selectedIDs.push_back(matrizValores[INDEXi][0]);
        cost += matrizValores[INDEXi][1];
        INDEXi--;
    }*/
}

#pragma endregion