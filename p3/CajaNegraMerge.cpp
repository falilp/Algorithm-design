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


//////////////////////////////////
#pragma region PreordenFusion


#pragma region Fusion

void Fusion(std::vector<CellsMap>& cell,std::vector<CellsMap>& CellsFirst,std::vector<CellsMap>& CellsSecond){
    int sizeFirst = CellsFirst.size();
    int sizeSecond = CellsSecond.size();
    int cel,firstIndex,secondIndex;
    cel = firstIndex = secondIndex = 0;

    while(firstIndex < sizeFirst && secondIndex < sizeSecond){
        if(CellsFirst[firstIndex].value < CellsSecond[secondIndex].value){
            cell[cel] = CellsFirst[firstIndex];
            firstIndex++;
        }else{
            cell[cel] = CellsSecond[secondIndex];
            secondIndex++;
        }
        cel++;
    }

    for(firstIndex; firstIndex<sizeFirst; firstIndex++){
        cell[cel] = CellsFirst[firstIndex];
        cel++;
    }
    
    for(secondIndex; secondIndex<sizeSecond; secondIndex++){
        cell[cel] = CellsSecond[secondIndex];
        cel++;
    }
}

#pragma endregion


#pragma region OrdenFusion

void OrdenacionFusion(std::vector<CellsMap>& cell){
    if(cell.size() <= 1){
        return;
    }
    
    int MID = cell.size()/2;

    std::vector<CellsMap> CellsFirst;
    std::vector<CellsMap> CellsSecond;

    CellsFirst.reserve(MID);
    CellsSecond.reserve(cell.size()-MID);

    std::move(cell.begin(),cell.begin()+MID,std::back_inserter(CellsFirst));
    std::move(cell.begin()+MID,cell.end(),std::back_inserter(CellsSecond));

    OrdenacionFusion(CellsFirst);
    OrdenacionFusion(CellsSecond);
    Fusion(cell,CellsFirst,CellsSecond);
}

#pragma endregion 


#pragma region PlaceDefenses

void placeDefensesFusion(int nCellsWidth, int nCellsHeight, float mapWidth, float mapHeight){
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

    OrdenacionFusion(Cells);

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
        std::cout<<"Fusion pasa la prueba de caja negra"<<std::endl<<std::endl;
    }else{
        std::cout<<"Fusion no pasa la prueba de caja negra"<<std::endl<<std::endl;
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
    placeDefensesFusion(nCellsWidth,nCellsHeight,mapWidth,mapHeight);
    std::cout<<"Fin de las pruebas de caja negra"<<std::endl;
}