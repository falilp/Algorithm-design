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
#pragma region PreordenMonticulo

#pragma region OrdenMonticulo

void OrdenacionMonticulo(std::vector<CellsMap>& cell){
    std::make_heap(cell.begin(),cell.end());

    std::sort_heap(cell.begin(),cell.end());
}

#pragma endregion

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

    OrdenacionMonticulo(Cells);

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
        std::cout<<"Monticulo pasa la prueba de caja negra"<<std::endl<<std::endl;
    }else{
        std::cout<<"Monticulo no pasa la prueba de caja negra"<<std::endl<<std::endl;
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