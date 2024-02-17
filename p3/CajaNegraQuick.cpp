#include <iostream>
#include "../simulador/Asedio.h"
#include "../simulador/Defense.h"
#include "cronometro.h"
#include <vector>

using namespace Asedio; 


typedef struct CellsMap{
    CellsMap();
    CellsMap(float val,int posiI,int posiJ):value(val),PositionI(posiI),PositionJ(posiJ){}
    float value;
    int PositionI;
    int PositionJ;
    inline bool operator <(const CellsMap& cell){return this->value<cell.value;}
}CellsMap;


//////////////////////////////////+-
#pragma region PreordenRapidez

//FIN
#pragma region Rapidez

int partition(std::vector<CellsMap>& cell, int INICIO, int FIN){
    int pivote = cell[INICIO].value;
    int count = 0;

    for(int iterador = INICIO+1; iterador <= FIN; ++iterador){
        if(cell[iterador].value <= pivote){
            count++;
        }
    }

    int pivoteIndex = INICIO+count;
    std::swap(cell[pivoteIndex],(cell[INICIO]));

    int i = INICIO;
    int j = FIN;

    while(i < pivoteIndex && j > pivoteIndex){
        while(cell[i].value <= pivote){
            i++;
        }
        while(cell[j].value > pivote){
            j--;
        }
        if(i < pivoteIndex && j > pivoteIndex){
            std::swap(cell[i++],cell[j--]);
        }
    }

    return pivoteIndex;
}

#pragma endregion

//FIN
#pragma region OrdenRapida

void OrdenacionRapida(std::vector<CellsMap>& cell,int INICIO,int FIN){
    if(INICIO > FIN){
        return;
    }

    int MID = partition(cell,INICIO,FIN);

    OrdenacionRapida(cell,INICIO,MID-1);
    OrdenacionRapida(cell,MID+1,FIN);
}

#pragma endregion

//FIN
#pragma region PlaceDefenses

void placeDefensesRapida(int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight){
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight;
    std::vector<CellsMap> Cells;

    for(int indexI=0; indexI<nCellsWidth; ++indexI){
        for(int indexJ=0; indexJ<nCellsHeight; ++indexJ){
            Cells.push_back(CellsMap(rand()%777+1,indexI,indexJ));
        }
    }

    std::cout<<"Elementos sin ordenar: "<<std::endl<<std::endl;

    for(int index=0; index<(nCellsWidth*nCellsHeight); ++index){
        std::cout<<"[ value:"<<Cells[index].value<<" X:"<<Cells[index].PositionI<<" Y:"<<Cells[index].PositionJ<<"] "<<std::endl;
    }

    std::cout<<std::endl<<std::endl;

    OrdenacionRapida(Cells,0,Cells.size());

    std::cout<<"Elementos ordenados: "<<std::endl<<std::endl;

    for(int index=0; index<(nCellsWidth*nCellsHeight); ++index){
        std::cout<<"[ value:"<<Cells[index].value<<" X:"<<Cells[index].PositionI<<" Y:"<<Cells[index].PositionJ<<"] "<<std::endl;
    }

    bool Prueba = true;

    for(int index=1; index<(nCellsWidth*nCellsHeight); ++index){
        if(Cells[index-1].value>Cells[index].value){
            Prueba = false;
        }
    }

    if(Prueba == true){
        std::cout<<"Rapidez pasa la prueba de caja negra"<<std::endl<<std::endl;
    }else{
        std::cout<<"Rapidez no pasa la prueba de caja negra"<<std::endl<<std::endl;
    }
}

#pragma endregion

#pragma endregion
//////////////////////////////////

int main(){
    int nCellsWidth = 10, nCellsHeight = 10;
    float mapWidth = 40, mapHeight = 40;
    float cellWidth = mapWidth / nCellsWidth;
    float cellHeight = mapHeight / nCellsHeight; 

    std::cout<<"Inicio de las pruebas de caja negra"<<std::endl<<std::endl;
    placeDefensesRapida(nCellsWidth,nCellsHeight,mapWidth,mapHeight);
    std::cout<<"Fin de las pruebas de caja negra"<<std::endl;
}